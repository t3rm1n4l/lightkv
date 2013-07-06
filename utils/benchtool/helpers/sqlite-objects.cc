/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */

#include "sqlite-objects.hh"

#define MAX_STEPS 10000

PreparedStatement::PreparedStatement(sqlite3 *d, const char *query) {
    assert(d);
    assert(query);
    db = d;
    if(sqlite3_prepare_v2(db, query, (int)strlen(query), &st, NULL)
       != SQLITE_OK) {
        std::stringstream ss;
        ss << sqlite3_errmsg(db) << " while building query: ``" << query << "''";
        std::cout<<"ERROR: "<<ss.str()<<std::endl;
        exit(1);
    }
}

PreparedStatement::~PreparedStatement() {
    sqlite3_finalize(st);
}

int PreparedStatement::bind(int pos, const char *s, size_t nbytes) {
    sqlite3_bind_text(st, pos, s, (int)nbytes, SQLITE_STATIC);
    return 1;
}

int PreparedStatement::bind_blob(int pos, const char *s, size_t nbytes) {
    sqlite3_bind_blob(st, pos, s, (int)nbytes, SQLITE_STATIC);
    return 1;
}

int PreparedStatement::bind(int pos, int v) {
    sqlite3_bind_int(st, pos, v);
    return 1;
}

int PreparedStatement::bind64(int pos, uint64_t v) {
    sqlite3_bind_int64(st, pos, v);
    return 1;
}

size_t PreparedStatement::paramCount() {
    return static_cast<size_t>(sqlite3_bind_parameter_count(st));
}

int PreparedStatement::execute() {
    int steps_run = 0, rc = 0, busy = 0;
    while ((rc = sqlite3_step(st)) != SQLITE_DONE) {
        if (++steps_run > MAX_STEPS) {
            std::cout<<"ERROR: Too many db steps, erroring (busy="<<busy<<")"<<std::endl;
            exit(1);
        }
        if (rc == SQLITE_ROW) {
            // This is rather normal
        } else if (rc == SQLITE_BUSY) {
            ++busy;
        } else {
            const char *msg = sqlite3_errmsg(db);
            std::cout<<"ERROR: sqlite error: "<<msg<<std::endl;
            exit(1);
        }
    }
    return sqlite3_changes(db);
}

bool PreparedStatement::fetch() {
    bool rv = true;
    assert(st);
    int rc = sqlite3_step(st);
    switch(rc) {
    case SQLITE_ROW:
        break;
    case SQLITE_DONE:
        rv = false;
        break;
    default:
        std::stringstream ss;
        ss << "Unhandled case in sqlite-pst: ";
        const char *msg = sqlite3_errmsg(db);
        std::cout<<"ERROR: "<<ss.str()<<msg<<std::endl;
        exit(1);
    }
    return rv;
}

const char *PreparedStatement::column(int x) {
    return (char*)sqlite3_column_text(st, x);
}

const void *PreparedStatement::column_blob(int x) {
    return (char*)sqlite3_column_text(st, x);
}

int PreparedStatement::column_bytes(int x) {
    return sqlite3_column_bytes(st, x);
}

int PreparedStatement::column_int(int x) {
    return sqlite3_column_int(st, x);
}

uint64_t PreparedStatement::column_int64(int x) {
    return sqlite3_column_int64(st, x);
}

void PreparedStatement::reset() {
    // The result of this is ignored as it indicates the last error
    // returned from step calls, not whether reset works.
    // http://www.sqlite.org/c3ref/reset.html
    sqlite3_reset(st);
}

