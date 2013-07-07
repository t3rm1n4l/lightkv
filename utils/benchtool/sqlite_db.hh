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

const char *STMT_JOURNALMODE =
    "PRAGMA JOURNAL_MODE=DELETE;";

const char *STMT_TRANS_START =
    "BEGIN;";

const char *STMT_TRANS_STOP =
    "COMMIT;";

const char *STMT_UPDATE =
    "UPDATE kv SET k=?, v=? WHERE rowid=?;";

const char *STMT_GET =
    "SELECT k,v FROM kv WHERE rowid=?;";

const char *STMT_DELETE =
    "DELETE FROM kv WHERE rowid=?;";

class SqliteDB: public BaseDB {
public:
    sqlite3 *db;
    bool intransaction;

    SqliteDB(string f) {
        int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        assert(sqlite3_open_v2(f.c_str(), &db, flags, NULL) == SQLITE_OK);

        PreparedStatement p(db, STMT_INIT);
        p.execute();

        PreparedStatement pj(db, STMT_JOURNALMODE);
        pj.execute();

        PreparedStatement bt(db, STMT_TRANS_START);
        bt.execute();
        intransaction = true;
    }

    ~SqliteDB() {
        if (intransaction) {
            PreparedStatement et(db, STMT_TRANS_STOP);
            et.execute();
        }
    }

    uint64_t Insert(string &key, string &val) {
        PreparedStatement p(db, STMT_INSERT);
        p.bind(0, key.c_str(), key.length());
        p.bind_blob(1, val.c_str(), val.length());
        p.execute();
        return static_cast<uint64_t>(sqlite3_last_insert_rowid(db));
    }

    uint64_t Update(uint64_t token, string &key, string &val) {
        PreparedStatement p(db, STMT_UPDATE);
        p.bind(0, key.c_str(), key.length());
        p.bind_blob(1, val.c_str(), val.length());
        p.bind64(2, token);
        p.execute();
        return static_cast<uint64_t>(sqlite3_last_insert_rowid(db));
    }

    bool Get(uint64_t token, string &key, string &val) {
        PreparedStatement p(db, STMT_GET);
        p.bind64(0, token);
        if (p.fetch()) {
            key.assign(p.column(1), p.column_bytes(1));
            val.assign((char *) p.column_blob(2), p.column_bytes(2));
            return true;
        }

        return false;
    }

    bool Delete(uint64_t token) {
        PreparedStatement p(db, STMT_DELETE);
        p.bind64(0, token);
        if (p.execute()) {
            return true;
        }

        return false;
    }

    // TODO(aniraj): Implement all of the following methods
    void Sync() {}
    DBIterator *Iterator() { return NULL; }
};

#endif
