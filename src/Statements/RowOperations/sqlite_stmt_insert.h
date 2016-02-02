//
//  sqlite_stmt_insert.h
//  GloopCore
//
//  Created by Robert Parker on 15/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_stmt_insert__
#define __GloopCore__sqlite_stmt_insert__

#include "sqlite_stmt_value.h"

namespace GloopCore { namespace SQLite {
    
    class Insert : public ValueBindingStatement {

    private:
        std::string tableName;

    protected:
        bool validate();
        std::string sql();

    public:
        Insert(std::string tableName);
        std::string name();
        
    };
    
}   }

#endif /* defined(__GUCore__GUCoreSQLiteInsert__) */
