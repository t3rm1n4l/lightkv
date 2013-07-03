// Base class for db layer interface

#ifndef INTERFACE_HH
#define INTERFACE_HH 1

#include <string>
using namespace std;

class DBIterator {
public:
    virtual bool Next(uint64_t &token, string &key, string &val) = 0;
};

class BaseDB {
public:
    virtual uint64_t Insert(string &key, string &val) = 0;
    virtual uint64_t Update(uint64_t token, string &key, string &val) = 0;
    virtual bool Get(uint64_t token, string &key, string &val) = 0;
    virtual bool Delete(uint64_t token) = 0;
    virtual void Sync() = 0;
    virtual DBIterator *Iterator() = 0;
};

#endif
