#ifndef PGF_HEADER_STRING
#define PGF_HEADER_STRING

#include <string.h>

struct PGF_String
{
    char*   value;
    size_t  size;
};

// Create a string.
const struct PGF_String*  PGF_CreateString(const char* value);

// Free a string.
void   PGF_FreeString(const struct PGF_String* str);

// Compare two strings.
_Bool  PGF_CompareString(const struct PGF_String* lhs, const struct PGF_String* rhs);
_Bool  PGF_CompareStringC(const struct PGF_String* lhs, const char* rhs);

#endif