#ifndef PGF_HEADER_CLOPTS
#define PGF_HEADER_CLOPTS

#include "pgfstr.h"

struct PGF_CommandArguments
{
    int argc;
    const char** argv;
};

enum PGF_CommandOptionValueType
{
    Bool, String, Int
};

struct PGF_CommandOptionValue
{
    enum PGF_CommandOptionValueType type;
    union {
        _Bool bool;
        int num;
        struct PGF_String* string;
    } value;
};

struct PGF_CommandOption
{
    struct PGF_String* shortName;
    struct PGF_String* longName;
};

struct PGF_CommandOptions
{
    struct PGF_CommandOption** options;
    struct PGF_CommandOptionValue** values;
    size_t size;
    size_t reserved;
};

// Wraps argc and argv into a single struct.
struct PGF_CommandArguments PGF_WrapArguments(int argc, const char** argv);

// Create a PGF_CommandOption struct.
struct PGF_CommandOption* PGF_CreateCommandOption(const char* shortName, const char* longName);

// Free a PGF_CommandOption struct.
void PGF_FreeCommandOption(struct PGF_CommandOption* option);

// Create a PGF_CommandOptions struct.
struct PGF_CommandOptions* PGF_CreateCommandOptions();

// Free a PGF_CommandOptions struct.
void PGF_FreeCommandOptions(struct PGF_CommandOptions* copts);

// Add options to a PGF_CommandOptions struct.
void PGF_AddOption(struct PGF_CommandOptions* copts, struct PGF_CommandOption* option, enum PGF_CommandOptionValueType type);

// Get the value of an option from PGF_CommandOptions struct.
struct PGF_CommandOptionValue* PGF_GetOption(struct PGF_CommandOptions* copts, const char* option);

// Set a boolean value to an option.
void PGF_SetOptionValueBool(struct PGF_CommandOptions* copts, const char* option, _Bool value);

// Set a string value to an option.
void PGF_SetOptionValueString(struct PGF_CommandOptions* copts, const char* option, const char* value);

// Set a boolean value to an option.
void PGF_SetOptionValueInt(struct PGF_CommandOptions* copts, const char* option, int value);

// Bind arguments to the options.
void PGF_ProcessArguments(struct PGF_CommandOptions* copts, const struct PGF_CommandArguments* args);

#endif