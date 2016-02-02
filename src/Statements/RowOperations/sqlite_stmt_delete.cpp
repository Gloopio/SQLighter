//
//  sqlite_stmt_delete.cpp
//  GloopCore
//
//  Created by Alex Untertrifaller on 29.07.15.
//  Copyright (c) 2015 Parob. All rights reserved.
//

#include "sqlite_stmt_delete.h"

using namespace GloopCore;
using namespace SQLite;

Delete::Delete(std::string tableName, std::string stringWhere) {
    this->tableName = tableName;
    this->stringWhere = stringWhere;
    this->requiredConnectionType = ConnectionTypeReadWrite;
}

std::string Delete::sql() {
    if (this->stringWhere.empty())
        return "DELETE FROM " + this->tableName + ";";
    else
        return "DELETE FROM " + this->tableName + " WHERE " + this->stringWhere + ";";
}

std::string Delete::name() {
    return "DeleteTableStatement";
}