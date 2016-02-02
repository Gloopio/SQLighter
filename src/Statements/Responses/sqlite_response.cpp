//
//  GUCoreSQLiteResponse.cpp
//  GUCore
//
//  Created by Robert Parker on 17/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#include "sqlite_response.h"
#include "sqlite_conn.h"

using namespace GloopCore;
using namespace SQLite;

ResponseHolder::ResponseHolder(Connection *connection, Statement *statement) {
    this->connection = connection;
    this->statement = statement;
}

ResponseHolder::~ResponseHolder() {
    connection->release(); 
}

void ResponseHolder::close() {
    delete this;
}

long long ResponseHolder::lastInsertedRowID() {
    return sqlite3_last_insert_rowid(connection->handle());
}