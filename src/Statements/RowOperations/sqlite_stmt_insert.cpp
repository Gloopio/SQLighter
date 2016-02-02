//
//  sqlite_stmt_insert.cpp
//  GloopCore
//
//  Created by Robert Parker on 15/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#include "sqlite_stmt_insert.h"

using namespace GloopCore;
using namespace SQLite;

Insert::Insert(std::string tableName) {
    this->tableName = tableName;
    this->requiredConnectionType = ConnectionTypeReadWrite;
}

bool Insert::validate() {
    if((int)data.size() > 0) {
        if(tableName.size() > 0)
            return true;
        else
            GLPLogWarn("Could not validate: Must specify the tableName.");
    }
    else {
        GLPLogWarn("Could not validate: Must provide atleast one value to be inserted.");
    }
    
    return false;
}

std::string Insert::sql() {
    int columnCount = (int)data.size();
    std::string sql = "INSERT OR REPLACE INTO ";
    sql += "\"" + tableName + "\"";
    if(columnCount == 0)
        return sql += " () VALUES ();";
    sql += " (";
    for(const auto &value : data)
        sql += value.first + ",";
    sql.pop_back();
    sql += ") VALUES (";
    for(int i = 0; i != columnCount; i++)
        sql += (i != columnCount - 1?"?,":"?);");

    return sql;
}

std::string Insert::name() {
    return "InsertStatement";
}