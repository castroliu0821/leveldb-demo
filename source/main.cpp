#include <chrono>
#include <string>
#include <iostream>
#include <map>

#include <boost/format.hpp>

#include "leveldb/db.h"
#include "leveldb/env.h"
#include "leveldb/write_batch.h"
#include "leveldb/cache.h"
#include "leveldb/filter_policy.h"
#include "easylogging++.h"

#define LOG_INFO(FORMAT, ...) \
    do { \
        if (nullptr != logger) { \
            logger->info(FORMAT, ##__VA_ARGS__); \
        } \
    } while (0);

#define LOG_WARN(FORMAT, ...) \
    do { \
        if (nullptr != logger) { \
            logger->warn(FORMAT, ##__VA_ARGS__); \
        } \
    } while (0);

#define LOG_ERR(FORMAT, ...) \
    do { \
        if (nullptr != logger) { \
            logger->error(FORMAT, ##__VA_ARGS__); \
        } \
    } while (0);

#define LOG_DEBUG(FORMAT, ...) \
    do { \
        if (nullptr != logger) { \
            logger->debug(FORMAT, ##__VA_ARGS__); \
        } \
    } while (0);

using namespace std;

INITIALIZE_EASYLOGGINGPP

static el::Logger* logger = nullptr;
const int64_t max_index = 1000000ll;

class global_env {
public:
    global_env() {
        el::Configurations conf("./log.conf");
        bool res = conf.parseFromFile("./log.conf");
        if (!res) {
            LOG(INFO) << "load conf failed";
        }
        else {
            LOG(INFO) << "load conf success";
        }
        el::Loggers::reconfigureAllLoggers(conf);
        logger = el::Loggers::getLogger("default");
    }
};

const global_env env;

void test_db_init(leveldb::DB** pDB) {
    // create leveldb instance
    leveldb::Options options;
    options.create_if_missing = true;
    options.compression = leveldb::kSnappyCompression;      // without compression
    options.filter_policy = leveldb::NewBloomFilterPolicy(10);

    options.block_cache = leveldb::NewLRUCache(100*1048576);

    leveldb::Status status = leveldb::DB::Open(options, "/home/liuhu/testdb", pDB);
    if (!status.ok() || nullptr == *pDB) {
        LOG_ERR("open error: %v" ,status.ToString());
    }
}

void test_db_write(leveldb::DB* pDB) {
    // assert parameter check
    if (nullptr == pDB) {
        LOG_INFO("ptr handle invalid")
        return;
    }

    chrono::system_clock::time_point tp_start =  chrono::high_resolution_clock::now();
    LOG_INFO("start recording .....");
    const int page_size = 100000;

    // create write data batch
    leveldb::WriteOptions options;
    leveldb::WriteBatch* dataBatch = new leveldb::WriteBatch();
    if (nullptr == dataBatch) {
        LOG_INFO("new batch failed");
        return;
    }

    // fill write batch

    int64_t pos = 0;
    while (max_index > pos) {
        int64_t tmp = pos + page_size;
        int size = tmp <= max_index ? page_size : max_index % page_size;
        pos = tmp;

        int64_t index = 0;

        dataBatch->Clear();

        while (size > index++) {
            static int64_t kv = 0 ;
            leveldb::Slice key = str(boost::format("%1%") % kv);
            leveldb::Slice value = str(boost::format("%1%") % kv);

            dataBatch->Put(key, value);
            ++kv;
        }

        // invoke leveldb fill data interface
        leveldb::Status status = pDB->Write(leveldb::WriteOptions(), dataBatch);
        if (!status.ok()) {
            LOG_INFO("error index: %v", index);
        }
   }

    LOG_INFO("stop recording .....");
    chrono::system_clock::time_point tp_end =  chrono::high_resolution_clock::now();

    // statistic consume time
    chrono::milliseconds time = chrono::duration_cast<chrono::milliseconds>(tp_end - tp_start);
    LOG_INFO("wirte %v consume time: %v", max_index, time.count());
}

void test_db_read(leveldb::DB* pDB) {
    // assert parameter check
    if (nullptr == pDB) {
        LOG_INFO("ptr handle invalid")
        return;
    }

    chrono::system_clock::time_point tp_start =  chrono::high_resolution_clock::now();
    LOG_INFO("start reading .....");

    std::map<string, string> content;

    leveldb::ReadOptions readOpt;
    readOpt.verify_checksums = true;
    const leveldb::Snapshot* snap = pDB->GetSnapshot();
    readOpt.snapshot = snap;

    leveldb::Iterator* it = pDB->NewIterator(readOpt);
    // statistic leveldb number (attention: read from where hard disk or ram cache????)
    int64_t index = 0;
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        index ++;
        content[it->key().ToString()] = it->value().ToString();
    }

    pDB->ReleaseSnapshot(snap);

    LOG_INFO("stop reading .....");
    chrono::system_clock::time_point tp_end =  chrono::high_resolution_clock::now();

    // statistic consume time
    chrono::milliseconds time = chrono::duration_cast<chrono::milliseconds>(tp_end - tp_start);
    LOG_INFO("read %v consume time: %v ms", index, time.count());
}

void test_db_exit(leveldb::DB** pDB) {
    // recycle leveldb instance resource
    delete *pDB;
    *pDB = nullptr;
}

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    leveldb::DB* db = nullptr;

    // step 1
    test_db_init(&db);

    // step 2
    test_db_write(db);

    // step 3
    test_db_read(db);

    // step 4
    test_db_exit(&db);

    return 0;
}
