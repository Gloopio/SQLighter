//
//  sqlite_stmt_does_table_exist.h
//  GloopCore
//
//  Created by Robert Parker on 29/01/2015.
//  Copyright (c) 2015 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_stmt_does_table_exist__
#define __GloopCore__sqlite_stmt_does_table_exist__

#include "sqlite_stmt_query.h"
#include "sqlite_stmt_select.h"

namespace GloopCore { namespace SQLite {
    
    enum LookupTableResult {
        LookupTableFound = 0,
        LookupTableNotFound = 1,
        LookupTableFail = 2 };
    
    class LookupTable : public Query {
        
        
    private:
        std::string tableName;
        LookupTableResult *result = nullptr;
        void resultsHandler(ResultsHolder *results);
        std::string sql();
        
    public:
        LookupTable(std::string tableName, LookupTableResult *result);
        
        std::string name();
    };
    
    class AllTables : public Select {
        
    public:
        AllTables(Results *results);
    };
    
    
}   }

#endif /* defined(__GloopCore__sqlite_stmt_does_table_exist__) */
