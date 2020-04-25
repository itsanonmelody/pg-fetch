#pragma once

#include "pqxx/result"
#include "shapefil.h"

namespace pgf
{
    DBFFieldType GetFieldType(pqxx::oid oid);
    void SerializeToDBF(const std::string& file, const pqxx::result& result);
}