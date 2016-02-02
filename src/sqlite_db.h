//
//  sqlite_db.h
//  GloopCores
//
//  Created by Robert Parker on 13/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_db__
#define __GloopCore__sqlite_db__

#ifdef __GNUC__
    #define __GloopCore__printf__(arg1index, arg2index) __attribute__ (( format( printf, arg1index + 1, arg2index + 1 ) ))
#else
    #define __GloopCore__printf__
#endif

#include <stdio.h>
#include <string>   // std::string
#include <stack>
#include <map>
#include <sstream>
#include <sqlite3.h>

#include "sqlite_response.h"
#include "sqlite_results.h"
#include "sqlite_conn_pool.h"

namespace GloopCore { namespace SQLite {
    
    class Database {

    friend class ResultsHolder;
        
    private:
        std::string databasePath;
        ConnectionPool *connections;
        bool commit(bool rollback);
        
    public: // Constructors
        Database(std::string databasePath);
        ~Database();
        
        /**
         *  Deadlocks
         *
         *  Both Results and Responses keep a handles to the SQLite Database open
         *      - Results keep a read-only handle open
         *      - Responses keep a read-write handle open
         *
         *  This handle is Automatially closed when the Results/Responses go out of scope, 
         *  It can be manually the close by calling .close() on the Results/Response.
         *
         *  Why must you close the handle?
         *      - For Read-Only handle, it is just ineffienct, as it will mean lots of read-only handle must be created.
         *      - For Read-Write handle, it may cause Deadlocks as only one Read-Write handle may be active at once.
         *
         */

        bool runStatement(Statement *statement);
        bool runStatement(const char *statement, ...)__GloopCore__printf__(1,2);
        
        bool runStatement(Results *resultsPointer, const char *statement, ...)__GloopCore__printf__(2,3);      // For Querys  @warning see Deadlocks above
        bool runStatement(Response *responsePointer, const char *statement, ...)__GloopCore__printf__(2,3);    // For Inserts @warning see Deadlocks above
        bool runStatement(long long *rowidPointer, const char *statement, ...)__GloopCore__printf__(2,3);      // For Inserts
        
        bool runStatements(const char *statements, ...)__GloopCore__printf__(1,2); // Statements should be seperated by ';'

        bool begin();
        bool rollback();
        bool commit();
        
    };
    
}   }

#endif
