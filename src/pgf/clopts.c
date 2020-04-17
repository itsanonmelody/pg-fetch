#include "pgf/clopts.h"
#include "pgf/pgfmem.h"

#include <assert.h>
#include <stdlib.h>

#define RESERVE_SIZE(a, b) (##a->reserved * sizeof(##b))

// Create CommandArguments from argc and argv.
struct PGF_CommandArguments* PGF_CreateArguments(int argc, const char** argv)
{
    struct PGF_CommandArguments* args = PGF_Malloc(sizeof(struct PGF_CommandArguments));
    args->argc = argc;

    args->argv = PGF_Malloc(argc * sizeof(struct PGF_String*));
    for (int i = 0; i < argc; ++i)
    {
        args->argv[i] = PGF_CreateString(argv[i]);
    }

    return args;
}

// Pop an argument from a CommandArguments struct.
struct PGF_String* PGF_PopArgument(struct PGF_CommandArguments* args, size_t index)
{
    assert(index < args->argc);

    struct PGF_String* arg = args->argv[index];
    for (; index < args->argc - 1; ++index)
    {
        args->argv[index] = args->argv[index + 1];
    }

    args->argv[args->argc--] = NULL;
    return arg;
}

// Free a CommandArguments struct.
void PGF_FreeArguments(struct PGF_CommandArguments* args)
{
    for (int i = 0; i < args->argc; ++i)
    {
        PGF_FreeString(args->argv[i]);
    }

    PGF_Free(args->argv, args->argc * sizeof(struct PGF_String*));
    PGF_Free(args, sizeof(struct PGF_CommandArguments));
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
    int prefixLength = 0;
    for (int i = 0; i < args->argc;)
    {
        struct PGF_String arg = *args->argv[i];
        prefixLength = (PGF_StringStartsWithC(&arg, "--") ? 2 : (PGF_StringStartsWithC(&arg, "-") ? 1 : 0));

        if (prefixLength)
        {
            arg.value += prefixLength;
            struct PGF_CommandOptionValue* optValue = PGF_GetOption(copts, arg.value);

            if (optValue)
            {
                switch (optValue->type)
                {
                case String:
                    if (optValue->value.string)
                    {
                        PGF_FreeString(optValue->value.string);
                    }

                    optValue->value.string = PGF_PopArgument(args, i + 1);
                    break;

                case Int:
                    struct PGF_String* str = PGF_PopArgument(args, i + 1);
                    optValue->value.num    = atoi(str->value);

                    PGF_FreeString(str);
                    break;

                case Bool: default:
                    optValue->value.bool = 1;
                    break;
                }
            }

            PGF_FreeString(PGF_PopArgument(args, i));
        }

        ++i;
    }
}