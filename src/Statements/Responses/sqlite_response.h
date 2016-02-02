//
//  GUCoreSQLiteResponse.h
//  GUCore
//
//  Created by Robert Parker on 17/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#ifndef __GloopCore__response__
#define __GloopCore__response__

#include <sqlite3.h>
#include "sqlite_constants.h"

namespace GloopCore {  namespace SQLite {
    
    class Connection;
    class Statement;
    class ResponseHolder;
    
    typedef std::unique_ptr<ResponseHolder> Response;
    
    class ResponseHolder {
        
    private:
        Connection *connection;
        Statement *statement;
        
    public:
        ResponseHolder(Connection *connection, Statement *statement);
        ~ResponseHolder();
        void close();
        
        long long lastInsertedRowID();
    };
    
}   }

#endif /* defined(__GUCore__GUCoreSQLiteResponse__) */
