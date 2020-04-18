#pragma once

#include <string>
#include <vector>
#include <variant>

namespace pgf
{
    struct CommandOptionName
    {
        std::string name;
        std::string shorName;
    };

    enum class CommandOptionValueType
    {
        Bool,
        Int,
        String
    };

    struct CommandOptionValue
    {
        CommandOptionValueType type;
        std::variant<bool, int, std::string> data;

        CommandOptionValue(const CommandOptionValueType& type);
    };

    struct CommandOptions
    {
        CommandOptions()  = default;
        ~CommandOptions() = default;

        CommandOptions(const CommandOptions&  other) = delete;
        CommandOptions(const CommandOptions&& other) = delete;

        CommandOptions& operator=(CommandOptions&  other) = delete;
        CommandOptions& operator=(CommandOptions&& other) = delete;

        void AddOption(const std::string& name, const std::string& shortName, const CommandOptionValueType& type);
        std::vector<CommandOptionValue>::iterator       FindOptionValue(const std::string& name);
        std::vector<CommandOptionValue>::const_iterator FindOptionValue(const std::string& name) const;

        void SetOptionValue(const std::string& name, bool value);
        void SetOptionValue(const std::string& name, int value);
        void SetOptionValue(const std::string& name, const std::string& value);

        void ProcessArguments(std::vector<std::string>& args);

        std::vector<CommandOptionName>  m_options;
        std::vector<CommandOptionValue> m_values;
    };
}