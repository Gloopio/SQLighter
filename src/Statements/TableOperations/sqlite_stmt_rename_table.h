//
//  sqlite_stmt_update_table.h
//  GloopCore
//
//  Created by Robert Parker on 15/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_stmt_rename_table__
#define __GloopCore__sqlite_stmt_rename_table__

#include "sqlite_stmt.h"

namespace GloopCore {  namespace SQLite {
    
    class RenameTable : public Statement {
    
    private:
        std::string tableName;
        std::string newTableName;
        
    protected:
        std::string sql();
        
    public:
        RenameTable(std::string tableName, std::string newTableName);
        
        std::string name();
    };
    
}   }

#endif /* defined(__GloopCore__sqlite_stmt_update_table__) */
