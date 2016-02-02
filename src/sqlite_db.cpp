//
//  sqlite_db.cpp
//  GloopCore
//
//  Created by Robert Parker on 13/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#include <sqlite3.h>
#include <typeinfo>

#include "sqlite_db.h"
#include "sqlite_constants.h"
#include "sqlite_stmt.h"
#include "sqlite_conn.h"
#include "sqlite_stmt_raw.h"

//#define __GUCore__SQLite__LogThreading(message) printf(message)
#define __GUCore__SQLite__LogThreading(message)

using namespace GloopCore;
using namespace SQLite;

Database::Database(std::string databasePath) {
    this->databasePath = databasePath;
    this->connections = new ConnectionPool(databasePath);
    
    Connection *connection = connections->getReservedConnection(ConnectionTypeReadWrite); // Gets a dummy readwrite connection to ensure the database is created!
    connection->release();
}

Database::~Database() {
    delete connections;
}

bool Database::runStatement(Statement *statement) {
    Connection *connection = connections->getReservedConnection(statement->requiredConnectionType);
    if(!statement->run(connection)) {                                                                // Its up to the statement to release the Connection now
        GLPLogError("%s Did not Execute.",statement->name().c_str());
        return false;
    }
    free(statement);
    return true;
}

bool Database::runStatement(const char *statement, ...) {
    va_list argList;
    va_start(argList, statement);
    va_list *argListPointer = &argList;
    RawStatement *rawStatement = new RawStatement(statement, argListPointer);
    if(rawStatement->rawStatementType == StatementTypeSelect)
        GLPLogWarn("A Select statement was run, but the results of it were not requested?");
    runStatement(rawStatement);
    va_end(argList);
    return true;
}

bool Database::runStatement(Results *resultsPointer, const char *statement, ...) {
    if(resultsPointer)
        resultsPointer->reset();
    va_list argList;
    va_start(argList, statement);
    va_list *argListPointer = &argList;
    RawStatement *rawStatement = new RawStatement(statement, argListPointer);
    rawStatement->results = resultsPointer;
    runStatement(rawStatement);
    va_end(argList);
    return true;
}

bool Database::runStatement(Response *responsePointer, const char *statement, ...) {
    if(responsePointer)
        responsePointer->reset();
    va_list argList;
    va_start(argList, statement);
    va_list *argListPointer = &argList;
    RawStatement *rawStatement = new RawStatement(statement, argListPointer);
    rawStatement->response = responsePointer;
    runStatement(rawStatement);
    va_end(argList);
    return true;
}

bool Database::runStatement(long long *rowidPointer, const char *statement, ...) {
    va_list argList;
    va_start(argList, statement);
    va_list *argListPointer = &argList;
    RawStatement *rawStatement = new RawStatement(statement, argListPointer);
    Response response;
    rawStatement->response = &response;
    runStatement(rawStatement);
    va_end(argList);
    if(rowidPointer)
        *rowidPointer = response->lastInsertedRowID();
    response.release();
    return true;
}


bool Database::begin() {
    Connection *connection = connections->getReservedConnection(ConnectionTypeReadWrite);
    return connection->beginTransaction();
}

bool Database::commit(bool rollback) {
    Connection *connection = connections->getReservedConnection(ConnectionTypeReadWrite);
    bool sucess = connection->commitTransaction(rollback);
    connection->release(); // We just got another connection so we have to release it manually!
    return sucess;
}

bool Database::rollback() {
    return commit(true);
}

bool Database::commit() {
    return commit(false);
}
