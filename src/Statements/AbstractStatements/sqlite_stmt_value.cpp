//
//  GUCoreSQLiteUpsert.cpp
//  GUCore
//
//  Created by Robert Parker on 15/11/2014.
//  Copyright (c) 2014 Parob. All rights reserved.
//

#include "sqlite_stmt_value.h"
#include <map>
#include "stdlib.h"

using namespace GloopCore;
using namespace SQLite;

void ValueBindingStatement::addValue(Value value, std::string columnName) {
    data[columnName] = value;
}

void ValueBindingStatement::addValues(std::map<std::string,Value> values) {
    if(data.size() != 0) {
        for(auto &value : values)
            addValue(value.second, value.first);
    }
    else {
        data = values;
    }
}

bool ValueBindingStatement::bind(sqlite3 *db){
    int index = 0;
    for(auto &wrappedValue : data) {
        int sqlite3_columnIndex = index + 1;
        std::string name = wrappedValue.first;
        Value value = wrappedValue.second;
        
        int rc = SQLITE_OK;
        bool fellbackToNull = false;
        switch (value.typeFormat.type) {
            case TypeBool:
                rc = sqlite3_bind_int(stmt, sqlite3_columnIndex, value.rawValue.bool_value);
                break;
            case TypeChar:
                rc = sqlite3_bind_int(stmt, sqlite3_columnIndex, value.rawValue.char_value);
                break;
            case TypeShort:
                rc = sqlite3_bind_int(stmt, sqlite3_columnIndex, value.rawValue.short_value);
                break;
            case TypeInt:
                rc = sqlite3_bind_int(stmt, sqlite3_columnIndex, value.rawValue.int_value);
                break;
            case TypeLong:
                rc = sqlite3_bind_int64(stmt, sqlite3_columnIndex, value.rawValue.long_value);
                break;
            case TypeLongLong:
                rc = sqlite3_bind_int64(stmt, sqlite3_columnIndex, value.rawValue.long_long_value);
                break;
            case TypeFloat:
                rc = sqlite3_bind_double(stmt, sqlite3_columnIndex, value.rawValue.float_value);
                break;
            case TypeDouble:
                rc = sqlite3_bind_double(stmt, sqlite3_columnIndex, value.rawValue.double_value);
                break;
            case TypeLongDouble:
                rc = sqlite3_bind_double(stmt, sqlite3_columnIndex, value.rawValue.long_double_value);
                break;
            case TypeString:
                rc = sqlite3_bind_text(stmt, sqlite3_columnIndex, value.rawValue.string_value, -1, SQLITE_STATIC);
                break;
            case TypeCppString:
                rc = sqlite3_bind_text(stmt, sqlite3_columnIndex, static_cast<std::string *>(value.rawValue.cpp_string_value)->c_str(), -1, SQLITE_STATIC);
                break;
            case TypeByte:
                rc = sqlite3_bind_int(stmt, sqlite3_columnIndex, value.rawValue.byte_value);
                break;
            case TypeBytes: {
                rc = sqlite3_bind_text(stmt, sqlite3_columnIndex, value.rawValue.bytes_value, -1, SQLITE_STATIC);
                //bytes byte_value = value.rawValue.bytes_value;
                //rc = sqlite3_bind_blob(stmt, sqlite3_columnIndex, byte_value.start_addr, byte_value.length, SQLITE_STATIC);
                break;
            }
            case TypeNull:
                rc = sqlite3_bind_null(stmt, sqlite3_columnIndex);
                break;
            case TypeUnknown:
                rc = sqlite3_bind_null(stmt, sqlite3_columnIndex);
                break;
            default:
                fellbackToNull = true;
                GLPLogError("Attempted to bind a unhandled Type description(%s) number(%i) on %s, binding Null as a fallback, SQL (%s).", describeType(value.typeFormat.type), value.typeFormat.type, name.c_str(), this->sql().c_str());
                rc = sqlite3_bind_null(stmt, sqlite3_columnIndex);
                break;
                
                /**case DataTypeInt64:
                 rc = sqlite3_bind_int64(stmt, sqlite3_columnIndex, *(sqlite_int64 *)valuePointer);
                 break;
                 
                 case DataTypeText16:
                 rc = sqlite3_bind_text16(stmt, sqlite3_columnIndex, *(const void **)valuePointer, -1, SQLITE_STATIC);
                 break;*/
                
        }
        if(rc != SQLITE_OK) {
            GLPLogError("Binding error (%d) %s on %s, couldn't bind value with Type %s %s, SQL (%s).", rc, sqlite3_errmsg(db), name.c_str(), describeType(value.typeFormat.type),(fellbackToNull?"Note: This value was null-bound as a fallback, but null was not bound correctly":""), this->sql().c_str());
            return false;
        }
        index++;
    }
    return true;
}