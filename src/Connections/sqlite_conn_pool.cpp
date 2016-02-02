//
//  GloopCoreSQLiteConnectionPool.cpp
//  GloopCore
//
//  Created by Robert Parker on 18/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#include "sqlite_conn_pool.h"

using namespace GloopCore;
using namespace SQLite;

ConnectionPool::ConnectionPool(std::string databasePath) {
    this->databasePath = databasePath;
}

ConnectionPool::~ConnectionPool() {
    for(ConnectionPool::size_type i = 0; i != this->size(); i++)
        delete (*this)[i];
}

Connection *ConnectionPool::getReservedConnection(ConnectionType type) {
    
    // Find any connections in the pool that we are already using.
    for(ConnectionPool::size_type i = 0; i != this->size(); i++) {
        Connection *connection = (*this)[i];
        if(connection->is_reserved_by_me() && (connection->type == ConnectionTypeReadWrite || connection->type == type) && connection->try_reserve())
            return connection;
    }
    
    // Were not in a connection so find one in the pool that we can reserve
    for(ConnectionPool::size_type i = 0; i != this->size(); i++) {
        Connection *connection = (*this)[i];
        if(connection->type == type && connection->try_reserve()) // A Write connection will sit here until its unlocked.
            return connection;
    }
    
    if(type == ConnectionTypeReadWrite && containsWriteConnection)
        GLPLogError("A write connection could not be found but supposedly the pool has already created one!\n");

    // We couldnt find a free connection so instread we try create a new one.
    Connection *connection = new Connection(databasePath, type, this);
    if(connection->try_reserve()) {
        this->push_back(connection);
        if(type == ConnectionTypeReadWrite && !containsWriteConnection)
            containsWriteConnection = true;
        return connection;
    } else {
        GLPLogError("Unable to retrieve a %s connection. Then created a new connection, but the new connection would not unlock.\n",(type == ConnectionTypeReadWrite ? "read/write":"read"));
        return nullptr;
    }
}