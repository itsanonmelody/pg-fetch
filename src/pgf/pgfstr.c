#include "pgf/pgfstr.h"
#include "pgf/pgfmem.h"

#include <string.h>

// Create a string.
const struct PGF_String* PGF_CreateString(const char* value)
{
    struct PGF_String* str = PGF_Malloc(sizeof(struct PGF_String));
    memset(str, 0, sizeof(struct PGF_String));

    size_t size = strlen(value);
    str->size = size;

    str->value = PGF_Malloc(size+1);
    strncpy(str->value, value, size);
    str->value[size+1] = '\0';

    return str;
}

// Free a string.
void PGF_FreeString(const struct PGF_String* str)
{
    PGF_Free(str->value, str->size + 1);
    PGF_Free(str, sizeof(struct PGF_String));
}

// Compare two strings.
_Bool PGF_CompareString(const struct PGF_String* lhs, const struct PGF_String* rhs)
{
    return (lhs->size == rhs->size) && (strncmp(lhs->value, rhs->value, lhs->size) == 0);
}

// Compare two strings.
_Bool PGF_CompareStringC(const struct PGF_String* lhs, const char* rhs)
{
    return (lhs->size == strlen(rhs)) && (strncmp(lhs->value, rhs, lhs->size) == 0);
}

// Check if a string starts with a substring.
_Bool  PGF_StringStartsWith(const struct PGF_String* source, const struct PGF_String* substr)
{
    return (source->size >= substr->size) && (strncmp(source->value, substr->value, substr->size) == 0);
}

// Check if a string starts with a substring.
_Bool  PGF_StringStartsWithC(const struct PGF_String* source, const char* substr)
{
    size_t size = strlen(substr);
    return (source->size >= size) && (strncmp(source->value, substr, size) == 0);
}

// Transform a string to lower case.
void PGF_StringToLower(struct PGF_String* str)
{
    for (size_t i = 0; i < str->size; ++i)
    {
        str->value[i] = tolower(str->value[i]);
    }
}

// Transform a string to lower case.
struct PGF_String* PGF_StringToLowerC(const char* str)
{
    struct PGF_String* res = PGF_CreateString(str);
    for (size_t i = 0; i < res->size; ++i)
    {
        res->value[i] = tolower(res->value[i]);
    }

    return res;
}