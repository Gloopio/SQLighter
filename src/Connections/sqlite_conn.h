//
//  GUCoreSQLiteConnection.h
//  GUCore
//
//  Created by Robert Parker on 18/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_conn__
#define __GloopCore__sqlite_conn__

#include <sqlite3.h>
#include <thread>   // std::thread
#include <mutex>    // std::mutex
#include <vector>
#include "sqlite_constants.h"
#include "sqlite_tran.h"

namespace GloopCore { namespace SQLite {

    class Transaction;
    
    class Connection {
        
    private:
        std::recursive_mutex mutexLock;
        int lockCount = 0;
        sqlite3 *db;
        
    protected:
        Transaction *transaction;
        Thread reservedByThread;
        std::vector<Connection *> *pool; // A connection will automatically remove itself from this pool if it is closed/destructed.
        void closeConnection();
        
    public:
        Connection(std::string databasePath, ConnectionType type, std::vector<Connection *> *pool);
        ~Connection();
        ConnectionType type;
        
        bool is_reserved_by_me();
        bool try_reserve(); // Note that when this returns true, You must later release this connection
        void release();
        
        /** The following methods will only work if the current thread sucesfully reserved this connection */
        sqlite3 *handle();
        bool beginTransaction();
        bool commitTransaction(bool rollback);

        
    };
    
}   }

#endif /* defined(__GUCore__GUCoreSQLiteConnection__) */
