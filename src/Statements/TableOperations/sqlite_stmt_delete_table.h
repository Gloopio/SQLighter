//
//  sqlite_stmt_delete_table.h
//  GloopCore
//
//  Created by Robert Parker on 15/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_stmt_delete_table__
#define __GloopCore__sqlite_stmt_delete_table__

#include "sqlite_stmt.h"

namespace GloopCore {  namespace SQLite {
    
    class DeleteTable : public Statement {
    
    private:
        std::string tableName;
    
    protected:
        std::string sql();
        
    public:
        DeleteTable(std::string tableName);
        
        std::string name();
    };
    
}   }

#endif /* defined(__GloopCore__sqlite_stmt_delete_table__) */
