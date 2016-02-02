//
//  GUCoreSQLiteConnectionPool.h
//  GUCore
//
//  Created by Robert Parker on 18/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_conn_pool__
#define __GloopCore__sqlite_conn_pool__

#include <vector>
#include "sqlite_constants.h"
#include "sqlite_conn.h"

namespace GloopCore { namespace SQLite {

    class ConnectionPool : public std::vector<Connection *>{
    
    private:
        std::string databasePath;
        bool containsWriteConnection = false;
    
    public:
        ConnectionPool(std::string databasePath);
        ~ConnectionPool();
        Connection *getReservedConnection(ConnectionType type); // Tries to reserve and return a connection of the specified type, if we are on a thread thats in a connection, we must use that existing connection
        
    };

}   }
    
#endif /* defined(__GUCore__GUCoreSQLiteConnectionPool__) */
