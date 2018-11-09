#ifndef DATA_H
#define DATA_H

#include <string>

class data {
private:
    std::string key;
    std::string value;

public:
    explicit data(const std::string&& key, const std::string&& value);
    virtual ~data();
};

#endif
