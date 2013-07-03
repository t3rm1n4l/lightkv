#ifndef LIGHTKV_DB_HH
#define LIGHTKV_DB_HH 1

#include "interface.hh"
#include "lightkv.h"
#include <iostream>
#include <stdint.h>

using namespace std;

class LightKVIterator: public DBIterator {
public:
    lightkv_iter *iter;
    LightKVIterator(lightkv_iter *it):iter(it) {}

    bool Next(uint64_t &token, string &key, string &val) {
        bool rv;
        char *k, *v;
        uint32_t len;

        rv = lightkv_next(iter, &token, &k, &v, &len);
        if (rv) {
            key.assign(k);
            val.assign(v, len);
            free(k);
            free(v);
        }
        return rv;
    }
};

class LightKVDB: public BaseDB {
public:
    lightkv *kv;
    LightKVDB(string b) {
        lightkv_init(&kv, b.c_str(), true);
    }

    uint64_t Insert(string &key, string &val) {
        lightkv_insert(kv, key.c_str(), val.c_str(), val.length());
    }

    uint64_t Update(uint64_t token, string &key, string &val) {
        return lightkv_update(kv, token, key.c_str(), val.c_str(), val.length());
    }

    bool Get(uint64_t token, string &key, string &val) {
        bool rv;
        char *k, *v;
        uint32_t len;

        rv = lightkv_get(kv, token, &k, &v, &len);
        if (rv) {
            key.assign(k);
            val.assign(v, len);
            free(k);
            free(v);
        }
        return rv;
    }

    bool Delete(uint64_t token) {
        return lightkv_delete(kv, token);
    }

    void Sync() {
        lightkv_sync(kv);
    }

    DBIterator *Iterator() {
        return new LightKVIterator(lightkv_iterator(kv));
    }

};

#endif
