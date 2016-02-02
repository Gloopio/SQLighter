//
//  GUCoreSQLiteResults.cpp
//  GUCore
//
//  Created by Robert Parker on 17/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#include "sqlite_results.h"
#include "sqlite_stmt.h"

using namespace GloopCore;
using namespace SQLite;

ResultsHolder::ResultsHolder(Connection *connection, Statement *statement) {
    this->connection = connection;
    this->statement = statement;
    this->db = connection->handle();
    this->stmt = statement->stmt;
}

ResultsHolder::~ResultsHolder() {
    this->close();
}

void ResultsHolder::close() {
    if(hasClosed) {
        GLPLogWarn("Results was closed multiple times; remeber Results auto-close when the are dealloced. Have you manually called close()?");
        return;
    }
    hasClosed = true;
    if(stmt)
        sqlite3_finalize(stmt);
    connection->release();
}

bool ResultsHolder::autoNextRow() {
    if(!hasStepped)
        hasStepped = true;
    int rc = sqlite3_step(stmt);
    if(rc == SQLITE_ROW) {
        return true;
    }
    else if(rc == SQLITE_DONE) {
        GLPLogError("%s Auto stepped through a results table with 0 rows. Please ensure there is atleast one row, or use while(results.nextRow()). The resulting value is Undefined. (%i:%s).",statement->name().c_str(),sqlite3_errcode(db),sqlite3_errmsg(db));
        return false;
    }
    else {
        GLPLogError("%s Unable to step through Results (%i:%s).",statement->name().c_str(),sqlite3_errcode(db),sqlite3_errmsg(db));
        return false;
    }
}

bool ResultsHolder::nextRow() {
    if(!hasStepped)
        hasStepped = true;
    if(hasEnded) {
        GLPLogError("%s Results set has already been stepped through.",statement->name().c_str());
        return false;
    }
    int rc = sqlite3_step(stmt);
    if(rc == SQLITE_ROW) {
        return true;
    }
    else if(rc == SQLITE_DONE) {
        hasEnded = true;
        return false;
    }
    else {
        GLPLogError("%s Unable to step through Results (%i:%s).",statement->name().c_str(),sqlite3_errcode(db),sqlite3_errmsg(db));
        return false;
    }
}

int ResultsHolder::columnCount() {
    if(privateColumnCount == -1)
        privateColumnCount = sqlite3_column_count(stmt);
    return privateColumnCount;
}

std::vector<std::string> ResultsHolder::columnNames() {
    if (privateColumnNames.size() == 0) {
        int currentColumnCount = columnCount();
        for (int index = 0; index < currentColumnCount; index++)
            privateColumnNames.push_back(std::string(sqlite3_column_name(stmt, index)));
    }
    return privateColumnNames;
}

int ResultsHolder::indexForColumnName(std::string columnName) {
    if(privateColumnNames.size() == 0)
        columnNames();
    
    int pos = (int)(std::find(privateColumnNames.begin(), privateColumnNames.end(), columnName) - privateColumnNames.begin());
    if( pos < privateColumnNames.size() )
        return pos;
    else
        return -1;
}

int ResultsHolder::typeForColumnIndex(int index) {
    return sqlite3_column_type(stmt, index);
}

int ResultsHolder::intForColumnIndex(int index) {
    if(!hasStepped && !autoNextRow())
        return -1;
    return sqlite3_column_int(stmt, index);
}

int ResultsHolder::intForColumnName(std::string columnName) {
    return intForColumnIndex(indexForColumnName(columnName));
}

long long ResultsHolder::longLongForColumnIndex(int index) {
    if(!hasStepped && !autoNextRow())
        return -1;
    return sqlite3_column_int64(stmt, index);
}

long long ResultsHolder::longLongForColumnName(std::string columnName) {
    return longLongForColumnIndex(indexForColumnName(columnName));
}

double ResultsHolder::doubleForColumnIndex(int index) {
    if(!hasStepped && !autoNextRow())
        return -1;
    return sqlite3_column_double(stmt, index);
}

double ResultsHolder::doubleForColumnName(std::string columnName) {
    return doubleForColumnIndex(indexForColumnName(columnName));
}

const unsigned char *ResultsHolder::stringForColumnIndex(int index) {
    if(!hasStepped && !autoNextRow())
        return NULL;
    
    if (sqlite3_column_type(stmt, index) == SQLITE_NULL || index < 0)
        return NULL;
    
    return sqlite3_column_text(stmt, index);
}

const unsigned char *ResultsHolder::stringForColumnName(std::string columnName) {
    return stringForColumnIndex(indexForColumnName(columnName));
}

int indexOfColumnNamed(const char *columnName,const char **columns) {
    for(int i = 0; i < sizeof(columns) / sizeof(const char); i++) {
        if(strcmp(columns[i],columnName) == 0)
            return i;
    }
    return -1;
}

void ResultsHolder::print(int rows) {
    GLPLogError();
}

std::string ResultsHolder::stringify(int rows) {
    std::string stringResults;
    if(rows > 0 ) {
        stringResults = "Stringifying %s rows from result set.";
    }
    int index = 0;
    while (this->nextRow() && index <= rows) {

    }
    return stringResults;
}