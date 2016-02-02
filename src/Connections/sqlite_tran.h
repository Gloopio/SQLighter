//
//  GUCoreSQLiteTransaction.h
//  GUCore
//
//  Created by Robert Parker on 18/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_tran__
#define __GloopCore__sqlite_tran__

#include <vector>
#include <stack>
#include "sqlite_constants.h"

namespace GloopCore { namespace SQLite {
    
    class Connection;
    
    typedef std::stack<std::string> Savepoints;
    
    class Transaction {
        
    private:
        Connection *connection;
        Savepoints savepoints;
    public:
        Transaction(Connection *connection);
        ~Transaction();
        
        bool begin();
        bool commit(bool rollback);
        bool inTransaction;
    };
    
}   }

#endif /* defined(__GUCore__GUCoreSQLiteTransaction__) */
