//
//  sqlite_stmt_delete_column.h
//  GloopCore
//
//  Created by Robert Parker on 25/02/2015.
//  Copyright (c) 2015 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_stmt_delete_column__
#define __GloopCore__sqlite_stmt_delete_column__

#include "sqlite_stmt.h"

namespace GloopCore { namespace SQLite {
    
    class DeleteColumn : public Statement {
        
    private:
        std::string tableName;
        std::string columnName;
        std::string sql();
        
    public:
        DeleteColumn(std::string tableName, std::string columnName);
        std::string name();
    };
    
}   }


#endif /* defined(__GloopCore__sqlite_stmt_delete_column__) */
