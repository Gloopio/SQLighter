//
//  GUCoreSQLiteConnection.cpp
//  GUCore
//
//  Created by Robert Parker on 18/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#include "sqlite_conn.h"
#include "sqlite_tran.h"

using namespace GloopCore;
using namespace SQLite;

Connection::Connection(std::string databasePath, ConnectionType type, std::vector<Connection *> *pool) {
    this->type = type;
    this->pool = pool;
    this->lockCount = 0;
    int openFlags;
    switch (type) {
        case ConnectionTypeRead:
            this->transaction = nullptr;
            openFlags = SQLITE_OPEN_READONLY | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_PRIVATECACHE;
            break;
        case ConnectionTypeReadWrite: default:
            this->transaction = new Transaction(this); // Only setup the Transaction if its a write connection
            openFlags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_PRIVATECACHE;
            break;
    }
    if (sqlite3_open_v2(databasePath.c_str(), &db, openFlags, NULL) != SQLITE_OK) {
        GLPLogError("Unable to open SQLite %s connection %d %s at path %s.",(type == ConnectionTypeReadWrite ? "read/write":"read"),sqlite3_errcode(db),sqlite3_errmsg(db),databasePath.c_str());
        closeConnection();
    }
    else if(type == ConnectionTypeReadWrite) {
        if(sqlite3_exec(db,"PRAGMA foreign_keys = ON; PRAGMA journal_mode = WAL; PRAGMA auto_vacuum = FULL;", NULL, NULL, NULL) != SQLITE_OK) {
            GLPLogError("Unable to setup SQLite read/write connection %d %s for path %s.",sqlite3_errcode(db),sqlite3_errmsg(db),databasePath.c_str());
            closeConnection();
            return;
        }
    }
    sqlite3_stmt *stmt = 0x00;
    if(sqlite3_prepare_v2(db,"SELECT name FROM sqlite_master WHERE type='table'", -1, &stmt, 0) == SQLITE_OK) {
        int rc = sqlite3_step(stmt);
        if((rc != SQLITE_ROW && rc != SQLITE_DONE) || sqlite3_finalize(stmt) != SQLITE_OK)
            GLPLogError("Bad Connection, %d %s.",sqlite3_errcode(db),sqlite3_errmsg(db));
    }
}

Connection::~Connection() {
    GLPLogInfo("Info Destructing connection.");
    closeConnection();
}

void Connection::closeConnection() {
    int rc = sqlite3_close(db);
    if(rc != SQLITE_OK && (SQLITE_BUSY == rc || SQLITE_LOCKED == rc)) {
        sqlite3_stmt *pStmt;
        while ((pStmt = sqlite3_next_stmt(db, NULL)) != 0) {
            GLPLogWarn("Closing leaked statement before closing connection.");
            sqlite3_finalize(pStmt);
        }
        if (sqlite3_close(db) != SQLITE_OK)
            GLPLogError("Unable to close SQLite %s connection %d %s.",(type == ConnectionTypeReadWrite ? "read/write":"read"),sqlite3_errcode(db),sqlite3_errmsg(db));
    }
    
    std::vector<Connection *>::iterator connectionPosition = std::find(pool->begin(), pool->end(), this);
    if(connectionPosition == pool->end()) {
        GLPLogError("Unable to remove the Connection from the pool.");
    }
    else {
        pool->erase(connectionPosition);
    }
    db = NULL;
}

bool Connection::is_reserved_by_me() {
    return (std::this_thread::get_id() == reservedByThread);
}

bool Connection::try_reserve() {
    bool locked;
    if(type == ConnectionTypeRead) { // A Read connection will return false if it is locked
        locked = mutexLock.try_lock();
    }
    else if(type == ConnectionTypeReadWrite) {  // A Read/Write connection will await till it is unlocked and then return
        bool tryLock = mutexLock.try_lock();
        if(!tryLock)
            mutexLock.lock();
        locked = true;
    }
    else {
        GLPLogError("Attempted to reserve a unknown connection type (%i).",type);
        return false;
    }
    if(locked) {
        if(lockCount == 0)
            reservedByThread = std::this_thread::get_id();
        lockCount++;
    }
    return locked;
}

sqlite3 *Connection::handle() { // You can only get the handle if the thread you are on reserved that connection
    if(std::this_thread::get_id() == reservedByThread)
        return db;
    GLPLogError("Attempted to get another threads Connection Handle.");
    return nullptr;
}

bool Connection::beginTransaction() {
    if(type == ConnectionTypeRead) {
        GLPLogError("Attempted to Begin a transaction on a Read connection.");
        return false;
    }
    if(std::this_thread::get_id() == reservedByThread)
        return this->transaction->begin();
    GLPLogError("Attempted to Begin another threads transaction.");
    return false;
}

bool Connection::commitTransaction(bool rollback) {
    if(type == ConnectionTypeRead) {
        GLPLogError("Attempted to Commit(%s) a transaction on a Read connection.",(rollback?"Rollback":"Save"));
        return false;
    }
    if(std::this_thread::get_id() == reservedByThread){
        bool sucess = this->transaction->commit(rollback);
        if(sucess)
            this->release();
        return sucess;
    }
    GLPLogError("Attempted to %s another threads Transaction.",(rollback?"Rollback":"Commit"));
    return false;
}

void Connection::release() {
    if(std::this_thread::get_id() == reservedByThread) {
        lockCount--;
        if(lockCount == 0)
            reservedByThread = Thread();
        mutexLock.unlock();
    }
    else {
        double thisThreadIdHash = std::hash<std::thread::id>()(std::this_thread::get_id());
        double ownerThreadIdHash = std::hash<std::thread::id>()(reservedByThread);
        GLPLogWarn("Attempted to release a connection that the current thread didnt reserve currentThreadId = %f reserverId = %f.",thisThreadIdHash,ownerThreadIdHash);
    }
}