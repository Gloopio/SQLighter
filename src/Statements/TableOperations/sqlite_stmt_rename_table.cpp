//
//  sqlite_stmt_rename_table.cpp
//  GloopCore
//
//  Created by Robert Parker on 15/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#include "sqlite_stmt_rename_table.h"

using namespace GloopCore;
using namespace SQLite;

RenameTable::RenameTable(std::string tableName, std::string newTableName) {
    this->tableName = tableName;
    this->newTableName = newTableName;
    this->requiredConnectionType = ConnectionTypeReadWrite;
}

std::string RenameTable::sql() {
    
    return NULL;
}

std::string RenameTable::name() {
    return "RenameTableStatement";
}