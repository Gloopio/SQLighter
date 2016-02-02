//
//  sqlite_stmt_delete_column.cpp
//  GloopCore
//
//  Created by Robert Parker on 25/02/2015.
//  Copyright (c) 2015 Parob. All rights reserved.
//

#include "sqlite_stmt_delete_column.h"

using namespace GloopCore;
using namespace SQLite;

DeleteColumn::DeleteColumn(std::string tableName, std::string columnName) {
    this->tableName = tableName;
}

std::string DeleteColumn::sql() {
    std::string sql = "";
    sql.append("ALTER TABLE " + this->tableName + " RENAME TO " + this->tableName.append("_delete_column") + ";");
    sql.append("CREATE TABLE " + this->tableName + " ( col_a INT , col_b INT );");
    sql.append("INSERT INTO " + this->tableName + " SELECT * FROM " + this->tableName.append("_delete_column") + ";");
    sql.append("DROP TABLE " + this->tableName.append("_delete_column") + ";");
    return sql;
}

std::string DeleteColumn::name() {
    return "DeleteColumnStatement";
}