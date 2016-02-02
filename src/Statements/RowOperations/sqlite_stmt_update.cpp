//
//  sqlite_stmt_update.cpp
//  GloopCore
//
//  Created by Robert Parker on 16/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#include "sqlite_stmt_update.h"

using namespace GloopCore;
using namespace SQLite;

Update::Update(std::string tableName) {
    this->tableName = tableName;
    this->requiredConnectionType = ConnectionTypeReadWrite;
}

Update::Update(std::string tableName, std::string objectId) {
    this->tableName = tableName;
    this->requiredConnectionType = ConnectionTypeReadWrite;
    this->where = " WHERE objectId = '" + objectId + "'";
}

std::string Update::sql() {
    std::string sql = "UPDATE ";
    sql += "\"" + tableName + "\"";
    sql += " SET ";
    int columnCount = (int)data.size();
    int count = 0;
    for(auto &value : data) {
        sql += value.first;
        if(count == columnCount - 1)
            sql += " = ? ";
        else
            sql += " = ?, ";
        count++;
    }
    sql += this->where;
    sql += ";";
    return sql;
}

std::string Update::name() {
    return "UpdateStatement";
}