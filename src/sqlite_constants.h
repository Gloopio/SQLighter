//
//  sqlite_constants.h
//  GUCore
//
//  Created by Robert Parker on 14/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_constants__
#define __GloopCore__sqlite_constants__

#include <string>
#include <thread>
#include <vector>
#include "constants.h"

namespace GloopCore { namespace SQLite {
    
    typedef std::thread::id Thread;
    
    enum ConnectionType {
        ConnectionTypeRead          = 0,
        ConnectionTypeReadWrite     = 1 << 1 };
    typedef int ConnectionTypes;
    
    enum StatementType {
        StatementTypeUnknown,
        StatementTypePragma,
        StatementTypeSelect,
        StatementTypeInsert };

    enum ColumnConstraint {
        ColumnConstraintNone        = 0,
        ColumnConstraintNotNull     = 1 << 1,
        ColumnConstraintUnique      = 1 << 2,
        ColumnConstraintForeignKey  = 1 << 3,
        ColumnConstraintPrimaryKey  = 1 << 4 };
    typedef int ColumnConstraints;
    
    enum ColumnAffinity {
        ColumnAffinityNone,
        ColumnAffinityText,
        ColumnAffinityNumeric,
        ColumnAffinityInteger,
        ColumnAffinityReal };
    
    enum ForeignKeyAction {
        ForeignKeyActionNoAction,
        ForeignKeyActionRestrict,
        ForeignKeyActionSetNull,
        ForeignKeyActionSetDefault,
        ForeignKeyActionCascade };
    
    extern "C" ColumnAffinity TypeToColumnAffinity(Type type);
    
    class ForeignKey;
    
    struct Column {
        std::string name;
        ColumnConstraints constraints = ColumnConstraintNone;
        ColumnAffinity affinity = ColumnAffinityNone;
        ForeignKey *foreignKey;
        
        Column(std::string name) {
            this->name = name;
        }
        
        Column(std::string name, ColumnAffinity affinity, ColumnConstraints constraints) {
            this->name = name;
            this->affinity = affinity;
            this->constraints = constraints;
        }
        
        Column(std::string name, ColumnAffinity affinity, ColumnConstraints constraints, ForeignKey *foreignKey){
            this->name = name;
            this->affinity = affinity;
            this->constraints = constraints;
            addForeignKey(foreignKey);
        }
        
        void addForeignKey(ForeignKey *foreignKey) {
            this->foreignKey = foreignKey;
            this->constraints = this->constraints | ColumnConstraintForeignKey;
        }
    };
    
    typedef std::vector<Column> Columns;
    
    struct Table {
        std::string name;
        Columns columns;
        Table(){};
        Table(std::string name){
            this->name = name;
        }
        void addColumn(Column column) {
            columns.push_back(column);
        }
    };
    
    struct ForeignKey {
        std::string foreignTableName;
        std::string foreignColumnName;
        ForeignKeyAction onDelete;
        ForeignKeyAction onUpdate;
        
        ForeignKey(){};
        ForeignKey(std::string foreignTableName, std::string foreignColumnName) {
            this->foreignColumnName = foreignColumnName;
            this->foreignTableName = foreignTableName;
            this->onDelete = ForeignKeyActionNoAction;
            this->onUpdate = ForeignKeyActionNoAction;
        };
        
        ForeignKey(std::string foreignTableName, std::string foreignColumnName, ForeignKeyAction onDelete, ForeignKeyAction onUpdate) {
            this->foreignColumnName = foreignColumnName;
            this->foreignTableName = foreignTableName;
            this->onDelete = onDelete;
            this->onUpdate = onUpdate;
        };
    };
    
    struct Where {
        
        Where(){};
        
    };
    
}   }
#endif
