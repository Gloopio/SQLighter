//
//  sqlite_stmt.h
//  GloopCore
//
//  Created by Robert Parker on 13/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_stmt__
#define __GloopCore__sqlite_stmt__

#include <stdio.h>
#include <string>
#include <vector>
#include <sqlite3.h>
#include "sqlite_constants.h"
#include "sqlite_conn.h"

namespace GloopCore {  namespace SQLite {
    
    class Statement {
        
    protected:
        virtual bool validate(); // To use validate: Override and return true when you have enough data in the statement to ensure proper execution
        virtual std::string sql();
        virtual bool prepare(sqlite3 *db);
        virtual bool bind(sqlite3 *db);
        virtual bool step(sqlite3 *db);
        virtual bool finalize(sqlite3 *db);
        
    public:
        sqlite3_stmt *stmt = 0x00;
        ConnectionType requiredConnectionType = ConnectionTypeRead;
        virtual bool run(Connection *connection); // We can safely assume this connection has been reserved for this statement.
        virtual std::string name();
        
    };
    
}   }

#endif /* defined(__GUCore__GUSQLiteStatement__) */
