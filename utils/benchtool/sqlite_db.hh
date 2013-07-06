#ifndef SQLITE_DB_HH
#define SQLITE_DB_HH 1

#include <iostream>
#include <stdint.h>

#include "interface.hh"
#include "helpers/sqlite-objects.hh"
#include "third_party/sqlite3/sqlite3.h"

using namespace std;

const char *STMT_INIT =
    "CREATE TABLE IF NOT EXISTS kv(k varchar(250), v blob);";

const char *STMT_INSERT =
    "INSERT INTO kv VALUES(?,?);";

class SqliteDB: public BaseDB {
public:
    sqlite3 *db;

    SqliteDB(string f) {
        int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        assert(sqlite3_open_v2(f.c_str(), &db, flags, NULL) == SQLITE_OK);

        PreparedStatement p(db, STMT_INIT);
        p.execute();
    }

    uint64_t Insert(string &key, string &val) {
        PreparedStatement p(db, STMT_INSERT);
        p.bind(0, key.c_str(), key.length());
        p.bind_blob(1, val.c_str(), val.length());
        p.execute();
        return static_cast<uint64_t>(sqlite3_last_insert_rowid(db));
    }

    // TODO(aniraj): Implement all of the following methods
    uint64_t Update(uint64_t token, string &key, string &val) {return 0; }
    bool Get(uint64_t token, string &key, string &val) { return true; }
    bool Delete(uint64_t token) { return true; }
    void Sync() {}
    DBIterator *Iterator() { return NULL; }
};

#endif
