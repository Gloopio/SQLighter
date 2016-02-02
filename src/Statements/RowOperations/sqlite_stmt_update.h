//
//  sqlite_stmt_update.h
//  GloopCore
//
//  Created by Robert Parker on 16/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_stmt_update__
#define __GloopCore__sqlite_stmt_update__

#include "sqlite_stmt_value.h"

namespace GloopCore { namespace SQLite {
    
    class Update : public ValueBindingStatement {
        
    private:
        std::string tableName;
        std::string where;
        
    protected:
        std::string sql();
        
    public:
        Update(std::string tableName);
        Update(std::string tableName, std::string objectId);
        std::string name();
        
    };
    
}   }

#endif /* defined(__GloopCore__sqlite_stmt_update__) */
