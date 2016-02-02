//
//  sqlite_constants.cpp
//  GloopCore
//
//  Created by Robert Parker on 24/02/2015.
//  Copyright (c) 2015 Parob. All rights reserved.
//

#include "sqlite_constants.h"

using namespace GloopCore;
using namespace SQLite;

extern "C" ColumnAffinity TypeToColumnAffinity(Type type) {
    
    switch (type) {
           
        case TypeChar:
        case TypeShort:
        case TypeInt:
        case TypeLong:
        case TypeLongLong:
            return ColumnAffinityInteger;
            
        case TypeFloat:
        case TypeDouble:
        case TypeLongDouble:
            return ColumnAffinityReal;
            
        case TypeString:
        case TypeCppString:
        case TypeBytes: // ColumnAffinityText is used for bytes, but bytes should generally be inserted as blob, ColumnAffinityText only means numeric values will be flattened.
            return ColumnAffinityText;
            
        default:
            return ColumnAffinityNone;
    }
}