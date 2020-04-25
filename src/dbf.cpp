#include "pgf/dbf.hpp"

#include "pqxx/pqxx"
#include "postgres.h"
#include "libpq-fe.h"
#include "catalog/pg_type.h"

#include <vector>
#include <climits>

DBFFieldType pgf::GetFieldType(pqxx::oid oid)
{
    DBFFieldType type = FTInvalid;
    switch(oid)
    {
    case BOOLOID:
        type = FTLogical;
        break;

    case INT2OID:
    case INT4OID:
        type = FTInteger;
        break;
    
    case FLOAT4OID:
    case FLOAT8OID:
        type = FTDouble;
        break;
    
    case CHAROID:
    case VARCHAROID:
    case TEXTOID:
        type = FTString;
        break;
    }

    return type;
}

void pgf::SerializeToDBF(const std::string& file, const pqxx::result& result)
{
    DBFHandle dbfFile = DBFCreate(file.c_str());
    
    std::vector<int> fields;
    fields.reserve(result.columns());

    for (pqxx::row_size_type i = 0; i < result.columns(); ++i)
    {
        DBFFieldType type = GetFieldType(result.column_type(i));
        fields.push_back(
            DBFAddField(
                dbfFile,
                result.column_name(i),
                type,
                (type == FTString) ? 0x20 : INT_MAX,
                (type == FTDouble) ? 2 : 0
            )
        );
    }

    for (int i = 0; i < result.size(); ++i)
    {
        pqxx::row& row = result[i];
        for (pqxx::row_size_type j = 0; j < result.columns(); ++j)
        {
            int dbfField = fields[j];
            if (dbfField == -1)
                continue;
            
            auto& field = row[j];

            DBFFieldType type = GetFieldType(field.type());
            switch (type)
            {
            case FTLogical:
                DBFWriteLogicalAttribute(
                    dbfFile,
                    i,
                    dbfField,
                    *field.get<bool>()
                );
                break;
            case FTInteger:
                DBFWriteIntegerAttribute(
                    dbfFile,
                    i,
                    dbfField,
                    *field.get<int>()
                );
                break;
            case FTDouble:
                DBFWriteDoubleAttribute(
                    dbfFile,
                    i,
                    dbfField,
                    *field.get<double>()
                );
                break;
            case FTString:
                DBFWriteStringAttribute(
                    dbfFile,
                    i,
                    dbfField,
                    field.c_str()
                );
                break;
            case FTInvalid:
            default:
                DBFWriteNULLAttribute(
                    dbfFile,
                    i,
                    dbfField
                );
                break;
            }
        }

        DBFClose(dbfFile);
    }
}