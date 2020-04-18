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

    void CommandOptions::AddOption(const std::string& name, char shortName, const CommandOptionValueType& type)
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
            if (opt.name == name)
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
            if (opt.name == name)
                break;
        }

        return m_values.begin() + index;
    }

    std::vector<CommandOptionValue>::iterator CommandOptions::FindOptionValue(char name)
    {
        unsigned int index = 0;
        while (index < m_options.size())
        {
            const auto& opt = m_options[index];
            if (opt.shortName == name)
                break;
            ++index;
        }

        return m_values.begin() + index;
    }
    std::vector<CommandOptionValue>::const_iterator CommandOptions::FindOptionValue(char name) const
    {
        unsigned int index = 0;
        while (index < m_options.size())
        {
            const auto& opt = m_options[index];
            if (opt.shortName == name)
                break;
            ++index;
        }

        return m_values.begin() + index;
    }

    bool CommandOptions::HasOption(const std::string& name)
    {
        return std::find_if(
                m_options.begin(),
                m_options.end(),
                [&name](const CommandOptionName& opt) {
                    return opt.name == name;
                }
            ) != m_options.end();
    }

    bool CommandOptions::HasOption(char name)
    {
        return std::find_if(
                m_options.begin(),
                m_options.end(),
                [&name](const CommandOptionName& opt) {
                    return opt.shortName == name;
                }
            ) != m_options.end();
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
        for (unsigned int i = 0; i < args.size(); ++i)
        {
            std::string arg = args[i];

            if (util::StartsWith(arg, "--"))
            {
                args.erase(args.begin() + (i--));
                arg = util::ToLower(arg.substr(2));

                if (!HasOption(arg))
                {
                    continue;
                }

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

            else if (util::StartsWith(arg, "-"))
            {
                args.erase(args.begin() + (i--));
                arg = util::ToLower(arg.substr(1));

                for (char c : arg)
                {
                    if (!HasOption(c))
                        continue;
                    
                    auto& optValue = this->FindOptionValue(c);
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
            }
        }
    }
}