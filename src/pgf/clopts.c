#include "pgf/clopts.h"
#include "pgf/pgfmem.h"

#include <assert.h>

#define RESERVE_SIZE(a, b) (##a->reserved * sizeof(##b))

// Wraps argc and argv into a single struct.
struct PGF_CommandArguments PGF_WrapArguments(int argc, const char** argv)
{
    return (struct PGF_CommandArguments){ argc, argv };
}

// Create a PGF_CommandOption struct.
struct PGF_CommandOption* PGF_CreateCommandOption(const char* shortName, const char* longName)
{
    struct PGF_CommandOption* option = PGF_Malloc(sizeof(struct PGF_CommandOption));
    memset(option, 0, sizeof(struct PGF_CommandOption));

    option->shortName = PGF_CreateString(shortName);
    option->longName  = PGF_CreateString(longName);

    return option;
}

// Free a PGF_CommandOption struct.
void PGF_FreeCommandOption(struct PGF_CommandOption* option)
{
    PGF_FreeString(option->shortName);
    PGF_FreeString(option->longName);
    PGF_Free(option, sizeof(struct PGF_CommandOption));
}

// Creates a PGF_CommandOptions struct.
struct PGF_CommandOptions* PGF_CreateCommandOptions()
{
    struct PGF_CommandOptions* copts = PGF_Malloc(sizeof(struct PGF_CommandOptions));
    memset(copts, 0, sizeof(struct PGF_CommandOptions));

    copts->reserved = 1;
    copts->options  = PGF_Malloc(RESERVE_SIZE(copts, struct PGF_CommandOption*));
    copts->values   = PGF_Malloc(RESERVE_SIZE(copts, struct PGF_CommandOptionValue*));

    return copts;
}

// Frees a PGF_CommandOptions struct.
void PGF_FreeCommandOptions(struct PGF_CommandOptions* copts)
{
    for (int i = 0; i < copts->size; ++i)
    {
        PGF_FreeCommandOption(copts->options[i]);
        
        if (copts->values[i]->type == String)
        {
            PGF_FreeString(copts->values[i]->value.string);
        }

        PGF_Free(copts->values[i], sizeof(struct PGF_CommandOptionValue));
    }

    PGF_Free(copts, sizeof(struct PGF_CommandOptions));
}

// Add options to a PGF_CommandOptions struct.
void PGF_AddOption(struct PGF_CommandOptions* copts, struct PGF_CommandOption* option, enum PGF_CommandOptionValueType type)
{
    if (copts->reserved < copts->size + 1)
    {
        copts->options   = PGF_Realloc(copts->options, RESERVE_SIZE(copts, struct PGF_CommandOption*), 2 * RESERVE_SIZE(copts, struct PGF_CommandOption*));
        copts->values    = PGF_Realloc(copts->options, RESERVE_SIZE(copts, struct PGF_CommandOptionValue*), 2 * RESERVE_SIZE(copts, struct PGF_CommandOptionValue*));
        copts->reserved *= 2;
    }

    struct PGF_CommandOptionValue* value = PGF_Malloc(sizeof(struct PGF_CommandOptionValue));
    memset(value, 0, sizeof(struct PGF_CommandOptionValue));
    value->type = type;

    copts->options[copts->size] = option;
    copts->values[copts->size++] = value;
}

// Get the value of an option from PGF_CommandOptions struct.
struct PGF_CommandOptionValue* PGF_GetOption(struct PGF_CommandOptions* copts, const char* option)
{
    struct PGF_String* opt = PGF_StringToLowerC(option);
    struct PGF_CommandOptionValue* value = NULL;

    for (size_t i = 0; i < copts->size; ++i)
    {
        if (PGF_CompareString(copts->options[i]->shortName, opt) ||
            PGF_CompareString(copts->options[i]->longName, opt))
        {
            value = copts->values[i];
            break;
        }
    }

    PGF_FreeString(opt);
    return value;
}

// Set a boolean value to an option.
void PGF_SetOptionValueBool(struct PGF_CommandOptions* copts, const char* option, _Bool value)
{
    struct PGF_CommandOptionValue* optValue = PGF_GetOption(copts, option);
    assert(optValue->type == Bool);

    optValue->value.bool = value;
}

// Set a string value to an option.
void PGF_SetOptionValueString(struct PGF_CommandOptions* copts, const char* option, const char* value)
{
    struct PGF_CommandOptionValue* optValue = PGF_GetOption(copts, option);
    assert(optValue->type == String);

    size_t valueSize = strlen(value);
    if (valueSize > optValue->value.string->size)
    {
        optValue->value.string->value = PGF_Realloc(optValue->value.string->value, optValue->value.string->size, valueSize+1);
        optValue->value.string->size = valueSize;
    }

    strncpy(optValue->value.string->value, value, valueSize);
    optValue->value.string->value[valueSize+1] = '\0';
}

// Set a boolean value to an option.
void PGF_SetOptionValueInt(struct PGF_CommandOptions* copts, const char* option, int value)
{
    struct PGF_CommandOptionValue* optValue = PGF_GetOption(copts, option);
    assert(optValue->type == Int);

    optValue->value.num = value;
}

// Bind arguments to the options.
void PGF_ProcessArguments(struct PGF_CommandOptions* copts, const struct PGF_CommandArguments* args)
{
    // ...
}