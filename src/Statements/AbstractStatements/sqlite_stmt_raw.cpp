//
//  sqlite_stmt_raw.cpp
//  GloopCore
//
//  Created by Robert Parker on 14/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#include "sqlite_stmt_raw.h"

using namespace GloopCore;
using namespace SQLite;

StatementType getStatementType(const char *sql);
ConnectionType getRequiredConnectionType(StatementType type);

RawStatement::RawStatement(std::string printf_sql, va_list *va_args) : Statement() {
    this->printf_sql = printf_sql;
    this->va_args = va_args;
    this->rawStatementType = getStatementType(printf_sql.c_str());
    this->requiredConnectionType = getRequiredConnectionType(this->rawStatementType);
};

bool RawStatement::run(Connection *connection) {
    sqlite3 *db = connection->handle();
    bool sucess = (prepare(db) && bind(db));
    bool requestedResults = (results != NULL && results != nullptr);
    bool requestedResponse = (response != NULL && response != nullptr);
    
    if(requestedResults && requestedResponse) {
        GLPLogWarn("%s requested Results and a Response, this is not possible, ignoring response request.",name().c_str());
        requestedResponse = false;
    }
    
    if(requestedResponse && (this->rawStatementType == StatementTypeInsert || this->rawStatementType == StatementTypePragma)) {      // The Response should now release the Connection
        *response = Response(new ResponseHolder(connection, this));
        sucess = step(db);
        sucess = (finalize(db) && sucess);
    }
    else if(requestedResults && (this->rawStatementType == StatementTypeSelect || this->rawStatementType == StatementTypePragma)) {   // The Resuls should now step, finalize, and release the Connection
        *results = Results(new ResultsHolder(connection, this));
    }
    else {                                                                         // No Results or Response was requested, so we should step, finalize, and release the connection
        sucess = step(db);
        sucess = (finalize(db) && sucess);
        connection->release();

        if(requestedResults || requestedResponse)
            GLPLogWarn("%s requested %s, but this type of statement does not return %s.",name().c_str(),(requestedResults?"Results":"a Response"),(requestedResults?"Results":"a Response"));
    }
    return sucess;
}

static std::string lengthModifiers = "hhhlllqjLztl";

std::string RawStatement::sql() {
    std::string sql;
    std::string::size_type size = printf_sql.size();
    for(std::string::size_type i = 0 ; i < size ; ++i) {
        char currentChar = printf_sql[i];
        if (currentChar == '%') {
            char formatSpecifier;
            do
                formatSpecifier = printf_sql[++i];
            while (lengthModifiers.find(formatSpecifier) != std::string::npos && i < size);
            
            if(i < size){
                if(formatSpecifier == '%')
                    sql+= '%';
                else
                    sql+= '?';
            }
        }
        else {
            sql += currentChar;
        }
    }
    
    return sql;
}

bool RawStatement::bind(sqlite3 *db) {
    int boundCount = 0;
    int bindCount = sqlite3_bind_parameter_count(stmt);
    std::string::size_type size = printf_sql.size();
    for(std::string::size_type i = 0 ; i < size && boundCount < bindCount ; ++i) {
        char currentChar = printf_sql[i];
        if (currentChar == '%') {
            char formatSpecifier;
            int lengthModifierLength = -1;
            do {
                formatSpecifier = printf_sql[++i];
                lengthModifierLength++;
            } while (lengthModifiers.find(formatSpecifier) != std::string::npos && i < size);
            
            /** Gets the the lengthModifier if we need to use it in the future 
             char *lengthModifier;
             if(lengthModifierLength > 0){
             lengthModifier = malloc(sizeof(char) * (lengthModifierLength + 1));
             for (int p = 0; p < lengthModifierLength; p ++) {
             lengthModifier[p] = formattedSql[i - lengthModifierLength];
             }
             lengthModifier[lengthModifierLength] = '\0';
             }*/
            
            switch (formatSpecifier) {
                case 'd': case 'D': case 'i': case 'c': case 'C':
                    sqlite3_bind_int(stmt, ++boundCount, va_arg(*va_args, int));
                    break;
                case 'u': case 'U':
                    sqlite3_bind_int64(stmt, ++boundCount, va_arg(*va_args, unsigned int));
                    break;
                case 'f': case 'F': case 'E': case 'e':
                    sqlite3_bind_double(stmt, ++boundCount, va_arg(*va_args, double));
                    break;
                case 's':
                    sqlite3_bind_text(stmt, ++boundCount, va_arg(*va_args, char *), -1, SQLITE_STATIC);
                    break;
                case 'S':
                    sqlite3_bind_text16(stmt, ++boundCount, va_arg(*va_args, char *), -1, SQLITE_STATIC);
                    break;
                case '%': // This means there was a percent sign in the statement
                    break;
                default:
                    break;
            }
        }
    }
    return true;
}

std::string RawStatement::name() {
    return std::string((rawStatementType == StatementTypeSelect? "Select":rawStatementType == StatementTypeInsert? "Insert" :"Generic")) + std::string("Statement('") + std::string(sql()) + std::string("')");
}

StatementType getStatementType(const char *sql) {
    const char *sqlPtr = sql;
    while(isspace(*sqlPtr)) sqlPtr++;
    if(*sqlPtr == 0)
        return StatementTypeUnknown;
    
    if (strncasecmp(sqlPtr,"SELECT",6) == 0) {
        return StatementTypeSelect;
    }
    else if (strncasecmp(sqlPtr,"INSERT",6) == 0) {
        return StatementTypeInsert;
    }
    else if (strncasecmp(sqlPtr,"PRAGMA",6) == 0) {
        return StatementTypePragma;
    }
    return StatementTypeUnknown;
}

ConnectionType getRequiredConnectionType(StatementType rawStatementType){
    switch (rawStatementType) {
        case StatementTypeUnknown: case StatementTypeInsert: default:
            return ConnectionTypeReadWrite;
            
        case StatementTypeSelect:
            return ConnectionTypeRead;
    }
}