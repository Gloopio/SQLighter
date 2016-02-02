//
//  sqlite_stmt_raw.h
//  GloopCore
//
//  Created by Robert Parker on 14/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_stmt_raw__
#define __GloopCore__sqlite_stmt_raw__

#include <stdio.h>
#include "sqlite_stmt.h"
#include "sqlite_response.h"
#include "sqlite_results.h"

namespace GloopCore {  namespace SQLite {
    
    class RawStatement: public Statement {
    
    private:
        std::string printf_sql;
        va_list *va_args;
        
        std::string sql();
        bool bind(sqlite3 *db);
    
    public:
        RawStatement(std::string printf_sql, va_list *va_args);
        StatementType rawStatementType;
        bool run(Connection *connection);
        std::string name();
        
        Response *response = nullptr; // Unused unless Insert
        Results *results = nullptr;   // Unsed unless query
    };
    
}   }

#endif /* defined(__GUCore__GUSQLitePrintfStatement__) */
