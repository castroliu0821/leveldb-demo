################################################################################
#                          project attribution setting                         #
#   BINARY      :    execute binary                                            #
#   STATIC      :    static library                                            #
#   SHARED      :    shared library                                            #
################################################################################
SET(PRIVATE_MODULE_NAME "LevelDB_Demo")
SET(PRIVATE_MODULE_TYPE "BINARY")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

cmake_minimum_required(VERSION 2.8)

################################################################################
#                   dependent linked third library setting                     #
################################################################################
SET(PRIVATE_DYNAMIC_LIBRARY
    "leveldb"
    "boost_system"
)

LINK_DIRECTORIES(
    "${CMAKE_CURRENT_SOURCE_DIR}/externLib/leveldb/lib"
)

LINK_LIBRARIES(${PRIVATE_DYNAMIC_LIBRARY})

################################################################################
#                        complier options setting                              #
################################################################################
SET(CMAKE_CXX_FLAGS "-std=c++11")
SET(CMAKE_C_FLAGS)

################################################################################
#                       header file configurer setting                         #
#  Refs:                                                                       #
#  PROJECT_SOURCE_DIR        : top level source directory                      #
#  CMAKE_CURRENT_BINARY_DIR  : target complie directory                        #
################################################################################
SET(PRIVATE_MODULE_INC_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/externLib/rapidjson/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/externLib/leveldb/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/externLib/easyloggingcpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/externLib/pugixml"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/utils"
    "${CMAKE_CURRENT_SOURCE_DIR}/source"
)

INCLUDE_DIRECTORIES(
    ${PRIVATE_MODULE_INC_PATH}
)

################################################################################
#                       source file configurer setting                         #
#  Refs:                                                                       #
#  PROJECT_SOURCE_DIR      : top level source directory of current project     #
################################################################################
SET(PRIVATE_STATIC_SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/externLib/easyloggingcpp/easylogging++.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/externLib/pugixml/pugixml.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/utils/json_helper.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/utils/xml_helper.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/data.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp"
)

################################################################################
#                           generate project target                            #
################################################################################

PROJECT(${PRIVATE_MODULE_NAME})

IF (${PRIVATE_MODULE_TYPE} MATCHES "BINARY")
    ADD_EXECUTABLE(${PRIVATE_MODULE_NAME} ${PRIVATE_STATIC_SOURCE})
ELSEIF (${PRIVATE_MODULE_TYPE} MATCHES "STATIC")
    ADD_LIBRARY(${PRIVATE_MODULE_NAME} STATIC ${PRIVATE_STATIC_SOURCE})
ELSEIF (${PRIVATE_MODULE_TYPE} MATCHES "SHARED")
    ADD_LIBRARY(${PRIVATE_MODULE_NAME} SHARED ${PRIVATE_STATIC_SOURCE})
ELSE ()
    MESSAGE (FATAL_ERROR "Sorry target type do not support")
ENDIF ()
