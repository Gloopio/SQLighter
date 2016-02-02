//
//  GUCoreSQLiteResults.h
//  GUCore
//
//  Created by Robert Parker on 17/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__results__
#define __GloopCore__results__

#include <string>
#include <vector>
#include <memory>
#include <sqlite3.h>

namespace GloopCore { namespace SQLite {
    
    class Connection;
    class Statement;
    class ResultsHolder;
    
    typedef std::unique_ptr<ResultsHolder> Results;
    
    class ResultsHolder {
        
    private:
        sqlite3 *db;
        sqlite3_stmt *stmt;
        Connection *connection;
        Statement *statement;
        
    private:
        bool hasStepped = false;
        bool hasEnded = false;
        bool hasClosed = false;
        bool autoNextRow();
        int privateColumnCount = -1;
        std::vector<std::string> privateColumnNames;
        
    public:
        ResultsHolder(Connection *connection, Statement *statement);
        ~ResultsHolder();
        void close();

    public:
        bool nextRow();
        int columnCount();
        std::vector<std::string> columnNames();
        int indexForColumnName(std::string columnName);
        int typeForColumnIndex(int index);
        
        // First index is 0
        int intForColumnIndex(int index);
        int intForColumnName(std::string columnName);
        
        long long longLongForColumnIndex(int index);
        long long longLongForColumnName(std::string columnName);
        
        double doubleForColumnIndex(int index);
        double doubleForColumnName(std::string columnName);
        
        const unsigned char *stringForColumnIndex(int index);
        const unsigned char *stringForColumnName(std::string columnName);
        
        // Note: The following methods are for debgging only, once printed, the index will have to be moved back manually.
        
        void print(int rows = 20);              // rows <= 0 print all
        std::string stringify(int rows = 20);   // rows <=0 print all
        
    };
    
}   }

#endif /* defined(__GUCore__GUCoreSQLiteResults__) */
