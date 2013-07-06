/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
#ifndef SQLITE_PST_H
#define SQLITE_PST_H 1

#include <sqlite3.h>
#include <assert.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>

class PreparedStatement;
class SQLiteDB;

/**
 * A sqlite prepared statement.
 *
 * Note that each bind method returns the number of columns it
 * consumed (allowing you to, for example, bind a std::pair over two
 * columns).
 */
class PreparedStatement {
public:

    /**
     * Construct a prepared statement.
     *
     * @param d the DB where the prepared statement will execute
     * @param query the query to prepare
     */
    PreparedStatement(sqlite3 *d, const char *query);

    /**
     * Clean up.
     */
    ~PreparedStatement();

    /**
     * Bind a string parameter to a binding in this statement.
     *
     * @param pos the binding position (starting at 1)
     * @param s the value to bind
     * @param nbytes number of bytes in the string.
     *
     * @return 1
     */
    int bind(int pos, const char *s, size_t nbytes);

    /**
     * Bind a blob parameter to a binding in this statement.
     *
     * @param pos the binding position (starting at 1)
     * @param s the value to bind
     * @param nbytes number of bytes in the string.
     *
     * @return 1
     */
    int bind_blob(int pos, const char *s, size_t nbytes);

    /**
     * Bind a uint32 value.
     *
     * @param pos the binding position (starting at 1)
     * @param d the value to bind
     */
    int bind(int pos, int d);

    /**
     * Bind a uint64 value.
     *
     * @param pos the binding position (starting at 1)
     * @param d the value to bind
     *
     * @return 1
     */
    int bind64(int pos, uint64_t d);

    /**
     * Return the number of parameters required by this statement.
     */
    size_t paramCount();

    /**
     * Execute a prepared statement that does not return results.
     *
     * @return how many rows were affected
     */
    int execute();

    /**
     * Execute a prepared statement that does return results
     * and/or return the next row.
     *
     * @return true if there are more rows after this one
     */
    bool fetch();

    /**
     * Reset the bindings.
     *
     * Call this before reusing a prepared statement.
     */
    void reset();

    /**
     * Get the value at a given column in the current row.
     *
     * Use this along with fetch.
     *
     * @param x the column number
     * @return the value
     */
    const char *column(int x);

    /**
     * Get the value as a blob from the given column in the current
     * row.
     *
     * Use this along with fetch and column_bytes().
     *
     * @param x the column number
     * @return the value
     */
    const void *column_blob(int x);

    /**
     * Get the number of bytes stored in the given column for the
     * current row.
     *
     * Use this along with fetch and column() or column_blob().
     *
     * @param x the column number
     * @return the number of bytes found
     */
    int column_bytes(int x);

    /**
     * Get the integer valueof the given column at the current row.
     *
     * Use this along with fetch.
     *
     * @param x the column number
     * @return the value
     */
    int column_int(int x);

    /**
     * Get the 64-bit integer value of the given column at the current
     * row.
     *
     * Use this along with fetch.
     *
     * @param x the column number
     * @return the value
     */
    uint64_t column_int64(int x);

private:
    sqlite3      *db;
    sqlite3_stmt *st;
};

#endif /* SQLITE_PST_H */
