//
//  sqlite_stmt_create_table.cpp
//  GloopCore
//
//  Created by Robert Parker on 15/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#include "sqlite_stmt_create_table.h"

using namespace GloopCore;
using namespace SQLite;

CreateTable::CreateTable(Table table) {
    this->table = table;
    this->requiredConnectionType = ConnectionTypeReadWrite;
}

std::string CreateTable::sql() {
    if(table.columns.size() == 0) {
        GLPLogWarn("Attempting to create a table with 0 columns.");
    }
    
    std::string primaryKeys = "";
    std::string sql = "CREATE TABLE " + table.name + " ( ";
    
    for(std::vector<Column>::size_type i = 0; i != table.columns.size(); i++) {
        Column column = table.columns[i];
        sql += "\"" + column.name + "\"";
        
        switch (column.affinity) {
            case ColumnAffinityInteger:
                sql += " INTEGER";
                break;
            case ColumnAffinityNumeric:
                sql += " NUMERIC";
                break;
            case ColumnAffinityReal:
                sql += " REAL";
                break;
            case ColumnAffinityText:
                sql += " TEXT";
                break;
            default:
                break;
        }
        
        if(column.constraints & ColumnConstraintPrimaryKey)
            primaryKeys += column.name + ",";
        if(column.constraints & ColumnConstraintUnique)
            sql += " UNIQUE";
        if(column.constraints & ColumnConstraintNotNull)
            sql += " NOT NULL";
        
        if(column.constraints & ColumnConstraintForeignKey) {
            sql += " REFERENCES " + column.foreignKey->foreignTableName + "(" + column.foreignKey->foreignColumnName + ")";
            
            switch (column.foreignKey->onDelete) {
                case ForeignKeyActionNoAction:
                    sql += " ON DELETE NO ACTION";
                    break;
                case ForeignKeyActionCascade:
                    sql += " ON DELETE CASCADE";
                    break;
                case ForeignKeyActionRestrict:
                    sql += " ON DELETE RESTRICT";
                    break;
                case ForeignKeyActionSetDefault:
                    sql += " ON DELETE SET NULL";
                    break;
                case ForeignKeyActionSetNull:
                    sql += " ON DELETE SET DEFAULT";
                    break;
                    
            }
            
            switch (column.foreignKey->onUpdate) {
                case ForeignKeyActionNoAction:
                    sql += " ON UPDATE NO ACTION";
                    break;
                case ForeignKeyActionCascade:
                    sql += " ON UPDATE CASCADE";
                    break;
                case ForeignKeyActionRestrict:
                    sql += " ON UPDATE RESTRICT";
                    break;
                case ForeignKeyActionSetDefault:
                    sql += " ON UPDATE SET NULL";
                    break;
                case ForeignKeyActionSetNull:
                    sql += " ON UPDATE SET DEFAULT";
                    break;
                    
            }
        }
        
        if(i != table.columns.size() - 1)
            sql += ", ";
    
    }
    if (!primaryKeys.empty())
        sql += ", PRIMARY KEY(" + primaryKeys.substr(0, primaryKeys.length()-1) + ")";
    sql += ");";
    return sql;
}

std::string CreateTable::name() {
    return "CreateTableStatement";
}