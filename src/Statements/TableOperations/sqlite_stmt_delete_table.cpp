//
//  sqlite_stmt_delete_table.cpp
//  GloopCore
//
//  Created by Robert Parker on 15/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#include "sqlite_stmt_delete_table.h"

using namespace GloopCore;
using namespace SQLite;

DeleteTable::DeleteTable(std::string tableName) {
    this->tableName = tableName;
}

std::string DeleteTable::sql() {
    return "DROP TABLE " + this->tableName + ";";
}

std::string DeleteTable::name() {
    return "DeleteTableStatement";
}