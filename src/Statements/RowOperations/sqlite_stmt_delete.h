//
//  sqlite_stmt_delete.h
//  GloopCore
//
//  Created by Alex Untertrifaller on 29.07.15.
//  Copyright (c) 2015 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_stmt_delete__
#define __GloopCore__sqlite_stmt_delete__

#include "sqlite_stmt.h"

namespace GloopCore {  namespace SQLite {
    
    class Delete : public Statement {
        
    private:
        std::string tableName;
        std::string stringWhere;
        
    protected:
        std::string sql();
        
    public:
        Delete(std::string tableName, std::string stringWhere);
        
        std::string name();
    };
    
}   }

#endif /* defined(__GloopCore__sqlite_stmt_delete__) */
