//
//  GUCoreSQLiteUpsert.h
//  GUCore
//
//  Created by Robert Parker on 15/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_stmt_value__
#define __GloopCore__sqlite_stmt_value__

#include "sqlite_stmt.h"

#include <map>

namespace GloopCore { namespace SQLite {
    
    class ValueBindingStatement : public Statement {

    protected:
        bool bind(sqlite3 *db);
        std::map<std::string,Value> data;
        
    public:
        void addValue(Value value, std::string columnName);
        void addValues(std::map<std::string,Value> values);
        
    };
    
} }

#endif /* defined(__GUCore__GUCoreSQLiteUpsert__) */
