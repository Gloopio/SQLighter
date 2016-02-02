//
//  GUCoreSQLiteTransaction.cpp
//  GUCore
//
//  Created by Robert Parker on 18/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#include <sqlite3.h>
#include <sstream>
#include "sqlite_tran.h"
#include "sqlite_conn.h"

using namespace GloopCore;
using namespace SQLite;

Transaction::Transaction(Connection *connection) {
    this->connection = connection;
    this->inTransaction = false;
}

Transaction::~Transaction() {
    GLPLogInfo("Destructing Transaction.");
}

bool Transaction::begin() {
    sqlite3 *db = this->connection->handle();
    std::string sql = "BEGIN TRANSACTION";
    if(inTransaction) {
        std::ostringstream oss;
        oss << savepoints.size();
        std::string savepointName = "savepoint" + oss.str();
        savepoints.push(savepointName);
        sql = "SAVEPOINT " + savepointName;
    }
    if(sqlite3_exec(db, sql.c_str(), 0, 0, 0) != SQLITE_OK) {
        GLPLogError("Could not Begin a %s, Error ('%s') Sql ('%s')",(inTransaction?"Savepoint":"Transaction"),sqlite3_errmsg(db),sql.c_str());
        if(inTransaction)
            savepoints.pop();
        return false;
    }
    if(!inTransaction)
        inTransaction = true;
   
    return true;
}

bool Transaction::commit(bool rollback) {
    sqlite3 *db = this->connection->handle();
    std::string sql = (rollback?"ROLLBACK TRANSACTION":"COMMIT TRANSACTION");
    bool inSavepoint = (!savepoints.empty());
    if(inSavepoint)
        sql = (rollback?"ROLLBACK TO SAVEPOINT ":"RELEASE SAVEPOINT ") + savepoints.top();
    if(!(sqlite3_exec(db, sql.c_str(), 0, 0, 0) == SQLITE_OK)) {
        GLPLogError("Could not %s a %s, Error ('%s')",(rollback?"Rollback":"Commit"),(inSavepoint?std::string("Savepoint(%s)",savepoints.top().c_str()).c_str():"Transaction"),sqlite3_errmsg(db));
        return false;
    }
    if(inSavepoint)
        savepoints.pop();
    else
        inTransaction = false;
    
    return true;
}