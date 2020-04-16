#include "pgf/pgfstr.h"

#include <stdlib.h>

// Create a string.
const struct PGF_String* PGF_CreateString(const char* value)
{
    struct PGF_String* str = malloc(sizeof(struct PGF_String));
    memset(str, 0, sizeof(str));

    size_t size = strlen(value);
    str->size = size;

    str->value = malloc(size+1);
    strncpy(str->value, value, size);
    str->value[size+1] = '\0';

    return str;
}

// Free a string.
void PGF_FreeString(const struct PGF_String* str)
{
    free(str->value);
    free(str);
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