//
//  sqlite_stmt_select.cpp
//  GloopCore
//
//  Created by Robert Parker on 16/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#include "sqlite_stmt_select.h"

using namespace GloopCore;
using namespace SQLite;



Select::Select(Results *results, std::string fromTableName, bool isRawQuery) {
    this->isRawQuery = isRawQuery;
    this->results = results;
    this->columns = Columns();
    this->columns.push_back(Column("*"));
    this->fromTableName = fromTableName;
}

Select::Select(Results *results, std::string fromTableName, std::string orderBy, bool isRawQuery) {
    this->isRawQuery = isRawQuery;
    this->results = results;
    this->columns = Columns();
    this->columns.push_back(Column("*"));
    this->fromTableName = fromTableName;
    this->orderBy = orderBy;
}

Select::Select(Results *results, std::string fromTableName, std::string stringWhere, std::string orderBy, bool isRawQuery) {
    this->isRawQuery = isRawQuery;
    this->results = results;
    this->columns = Columns();
    this->columns.push_back(Column("*"));
    this->fromTableName = fromTableName;
    this->orderBy = orderBy;
    this->stringWhere = stringWhere;
}

Select::Select(Results *results, std::string fromTableName, std::string stringWhere) {
    this->results = results;
    this->columns = Columns();
    this->columns.push_back(Column("*"));
    this->fromTableName = fromTableName;
    this->stringWhere = stringWhere;
}

Select::Select(Results *results, Columns columns, std::string fromTableName, Where where) {
    
}


Select::Select(Results *results, Columns columns, std::string fromTableName, std::string stringWhere) {
    this->results = results;
    this->columns = columns;
    this->fromTableName = fromTableName;
    this->stringWhere = stringWhere;
}

bool Select::run(Connection *connection) {
    sqlite3 *db = connection->handle();
    bool sucess = (prepare(db) && bind(db));
    
    if(results != NULL && results != nullptr) {      // The Resuls should now step, finalize, and then release the Connection
        *results = Results(new ResultsHolder(connection, this));
    }
    else {                      // No Results were requested so we should step, finalize, and release the connection
        GLPLogWarn("A Select statement was run, but the results of it were not requested?");
        sucess = step(db);
        sucess = (finalize(db) && sucess);
        connection->release();
    }
    return sucess;
}

std::string Select::sql() {
    if(isRawQuery)
        return this->fromTableName;
    if(this->columns.size() == 0) {
        GLPLogWarn("No columns were specified, retrieveing all the columns.");
        this->columns.push_back(Column("*"));
    }
    std::string sql = "SELECT ";
    for (auto & column : this->columns)
        sql += column.name + ",";
    sql.pop_back();
    sql += " FROM \"" + fromTableName + "\"" + (stringWhere.length() > 0?" WHERE " + stringWhere:"");

    sql += " " + orderBy;
    
    return sql;
}

std::string Select::name() {
    return "SelectStatement";
}