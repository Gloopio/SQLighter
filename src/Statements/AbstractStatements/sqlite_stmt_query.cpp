//
//  sqlite_stmt_query.cpp
//  GloopCore
//
//  Created by Robert Parker on 29/01/2015.
//  Copyright (c) 2015 Parob. All rights reserved.
//

#include "sqlite_stmt_query.h"

using namespace GloopCore;
using namespace SQLite;

bool Query::run(Connection *connection) {
    sqlite3 *db = connection->handle();
    bool sucess = (prepare(db) && bind(db));
    ResultsHolder *results = new ResultsHolder(connection, this);
    resultsHandler(results);
    results->close();
    return sucess;
}

bool Query::prepare(sqlite3 *db) {
    std::string sqlString = sql();
    if(sqlString == "")
        return false;
    int prepare = sqlite3_prepare_v2(db, sqlString.c_str(), -1, &stmt, 0);
    if (prepare != SQLITE_OK) {
        GLPLogError("%s Prepare Error ('%s').",name().c_str(),sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }
    return true;
}

void Query::resultsHandler(ResultsHolder *results) {
    GLPLogError("%s SQL Error, the resultsHandler() was not overridden.",name().c_str());
}

std::string Query::name() {
    return "QueryStatement";
}

