//
//  sqlite_stmt.cpp
//  GloopCore
//
//  Created by Robert Parker on 13/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#include <sqlite3.h>
#include "strings.h"
#include "sqlite_stmt.h"

using namespace GloopCore;
using namespace SQLite;

bool Statement::run(Connection *connection) {
    sqlite3 *db = connection->handle();
    bool sucess = (prepare(db) && bind(db) && step(db));
    finalize(db);
    connection->release();
    return sucess;
}

bool Statement::prepare(sqlite3 *db) {
    if(!validate()) {
        GLPLogError("%s Prepare Error, unable to Validate statement.",name().c_str());
        return false;
    }
    std::string sqlString = sql();
    if(sqlString == "")
        return false;
    int prepare = sqlite3_prepare_v2(db, sqlString.c_str(), -1, &stmt, 0);
    if (prepare != SQLITE_OK) {
        const char* errorMessage = sqlite3_errmsg(db);
        GLPLogError("%s Prepare Error('%s') SQL(%s).",name().c_str(),errorMessage,(strstr(errorMessage, "syntax") != NULL?sqlString.c_str():""));
        sqlite3_finalize(stmt);
        return false;
    }
    return true;
}

bool Statement::validate() {
    return true;
}

std::string Statement::sql() {
    GLPLogError("%s SQL Error, the sql() function was not overridden.",name().c_str());
    return "";
}

bool Statement::bind(sqlite3 *db) {
    return true;
}

bool Statement::step(sqlite3 *db) {
    int rc = sqlite3_step(stmt);
    bool sucess = (rc == SQLITE_DONE || rc == SQLITE_OK || rc == SQLITE_ROW);
    if(!sucess)
        GLPLogError("%s Step Error('%s')(%i)", name().c_str(), sqlite3_errmsg(db), rc);
    return sucess;
}

bool Statement::finalize(sqlite3 *db) {
    int rc = sqlite3_finalize(stmt);
    bool sucess = (rc == SQLITE_DONE || rc == SQLITE_OK || rc == SQLITE_ROW);
    if(!sucess)
        GLPLogError("%s Finalize Error('%s')(%i)", name().c_str(), sqlite3_errmsg(db), rc);
    return sucess;
}

std::string Statement::name() {
    return "GenericStatement";
}