//
//  sqlite_stmt_query.h
//  GloopCore
//
//  Created by Robert Parker on 29/01/2015.
//  Copyright (c) 2015 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_stmt_query__
#define __GloopCore__sqlite_stmt_query__

#include "sqlite_stmt.h"
#include "sqlite_results.h"

namespace GloopCore { namespace SQLite {
    
    class Query : public Statement {
        
    private:
        bool prepare(sqlite3 *db);
        
    protected:
        virtual void resultsHandler(ResultsHolder *results);
        
    public:
        bool run(Connection *connection);
        std::string name();
    };
    
}   }


#endif /* defined(__GloopCore__sqlite_stmt_query__) */
