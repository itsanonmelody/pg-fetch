#include "pgf/cli.hpp"
#include "pgf/util.hpp"

#include <cassert>

namespace pgf
{
    CommandOptionValue::CommandOptionValue(const CommandOptionValueType& type)
        : type(type)
    {
        switch (type)
        {
        case CommandOptionValueType::Int:
            data = 0;
            break;
        
        case CommandOptionValueType::String:
            data = std::string();
            break;

        case CommandOptionValueType::Bool:
        default:
            data = false;
            break;
        }
    }

    void CommandOptions::AddOption(const std::string& name, const std::string& shortName, const CommandOptionValueType& type)
    {
        m_options.push_back(CommandOptionName{ name, shortName });
        m_values.push_back(CommandOptionValue{ type });
    }

    std::vector<CommandOptionValue>::iterator CommandOptions::FindOptionValue(const std::string& name)
    {
        if (name.empty())
            return m_values.end();
        
        unsigned int index = 0;
        while (index < m_options.size())
        {
            const auto& opt = m_options[index];
            if (opt.name == name || opt.shorName == name)
                break;
            ++index;
        }

        return m_values.begin() + index;
    }

    std::vector<CommandOptionValue>::const_iterator CommandOptions::FindOptionValue(const std::string& name) const
    {
        if (name.empty())
            return m_values.end();
        
        unsigned int index = 0;
        while (index < m_options.size())
        {
            const auto& opt = m_options[index++];
            if (opt.name == name || opt.shorName == name)
                break;
        }

        return m_values.begin() + index;
    }

    void CommandOptions::SetOptionValue(const std::string& name, bool value)
    {
        auto optValue = this->FindOptionValue(name);
        if (optValue == m_values.end())
            return;

        if (optValue->type != CommandOptionValueType::Bool)
            return;
        
        optValue->data = value;
    }

    void CommandOptions::SetOptionValue(const std::string& name, int value)
    {
        auto optValue = this->FindOptionValue(name);
        if (optValue == m_values.end())
            return;

        if (optValue->type != CommandOptionValueType::Int)
            return;
        
        optValue->data = value;
    }

    void CommandOptions::SetOptionValue(const std::string& name, const std::string& value)
    {
        auto optValue = this->FindOptionValue(name);
        if (optValue == m_values.end())
            return;

        if (optValue->type != CommandOptionValueType::String)
            return;
        
        optValue->data = value;
    }

    void CommandOptions::ProcessArguments(std::vector<std::string>& args)
    {
        for (unsigned int i = 0; i < args.size();)
        {
            std::string arg = args[i];
            int prefixLength = util::StartsWith(arg, "--") ? 2 : (util::StartsWith(arg, "-") ? 1 : 0);

            if (prefixLength)
            {
                args.erase(args.begin() + i);
                arg = util::ToLower(arg.substr(prefixLength));

                auto& optValue = this->FindOptionValue(arg);
                if (optValue == m_values.end())
                    continue;
                
                switch (optValue->type)
                {
                case CommandOptionValueType::String:
                    if (i + 1 < args.size())
                    {
                        optValue->data = args[i + 1];
                        args.erase(args.begin() + i + 1);
                    }
                    break;

                case CommandOptionValueType::Int:
                    if (i + 1 < args.size())
                    {
                        optValue->data = std::stoi(args[i + 1]);
                        args.erase(args.begin() + i + 1);
                    }
                    break;

                case CommandOptionValueType::Bool:
                default:
                    optValue->data = true;
                    break;
                }
            }

            ++i;
        }
    }
}