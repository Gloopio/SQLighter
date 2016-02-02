//
//  sqlite_stmt_select.h
//  GloopCore
//
//  Created by Robert Parker on 16/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__sqlite_stmt_select__
#define __GloopCore__sqlite_stmt_select__

#include "sqlite_stmt.h"
#include "sqlite_results.h"

namespace GloopCore { namespace SQLite {
    
    class Where;
    
    class Select : public Statement {
    
    private:
        Columns columns;
        bool isRawQuery = false;
        std::string fromTableName;
        std::string stringWhere;
        std::string sql();
        std::string orderBy;
        
    public:
        
        /**
         *  isRawQuery, allows you to get the results from a raw sql query.
         *      When true; the fromTableName should be a valid SQL QUERY, Example: "SELECT * FROM users;"
         */
        
        Select(Results *results, std::string fromTableName, bool isRawQuery = false);
        Select(Results *results, std::string fromTableName, std::string orderBy, bool isRawQuery = false);
        Select(Results *results, std::string fromTableName, std::string stringWhere, std::string orderBy, bool isRawQuery);
        Select(Results *results, std::string fromTableName, Where where);
        Select(Results *results, std::string fromTableName, std::string stringWhere);
        Select(Results *results, Columns columns, std::string fromTableName, Where where);
        Select(Results *results, Columns columns, std::string fromTableName, std::string stringWhere);
        
        bool run(Connection *connection);
        std::string name();
        
        Results *results = nullptr;   // Unsed unless query
    };
    
}   }


#endif /* defined(__GloopCore__sqlite_stmt_select__) */
