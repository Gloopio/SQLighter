//
//  sqlite_stmt_does_table_exist.cpp
//  GloopCore
//
//  Created by Robert Parker on 29/01/2015.
//  Copyright (c) 2015 Parob. All rights reserved.
//

#include "sqlite_stmt_lookup_table.h"

using namespace GloopCore;
using namespace SQLite;

LookupTable::LookupTable(std::string tableName, LookupTableResult *result) {
    this->tableName = tableName;
    this->result = result;
}

std::string LookupTable::sql() {
    std::string sql = "SELECT count(*) FROM sqlite_master WHERE type == 'table' AND lower(name) == lower('" + tableName + "')";
    return sql;
}

void LookupTable::resultsHandler(ResultsHolder *results) {
    if (results->nextRow()) {
        int count = results->intForColumnIndex(0);
        *result = (count >= 1 ? LookupTableFound : LookupTableNotFound);
    }
    else {
        *result = LookupTableFail;
    }
}

std::string LookupTable::name() {
    return "LookupTableStatement";
}



AllTables::AllTables(Results *results) : Select(results, "SELECT name FROM sqlite_master WHERE type='table'", true) {
    
}