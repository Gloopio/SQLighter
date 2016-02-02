//
//  sqlite_stmt_create_table.h
//  GloopCore
//
//  Created by Robert Parker on 15/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_stmt_create_table__
#define __GloopCore__sqlite_stmt_create_table__

#include "sqlite_stmt.h"

namespace GloopCore {  namespace SQLite {
    
    class CreateTable : public Statement {
    
    private:
        Table table;
    
    protected:
        std::string sql();
        
    public:
        CreateTable(Table table);
        std::string name();
    };
    
}   }

#endif /* defined(__GUCore__GUCoreSQLiteCreateTable__) */
