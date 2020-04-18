#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>

#include "pgf/cli.hpp"

void DisplayHelp()
{
    std::cout << "Usage: pg-fetch [options] message..." << std::endl
              << "  -h, --help          displays this message" << std::endl
              << "      --max           sets max word of message" << std::endl
              << "      --prefix        sets prefix of message" << std::endl;
}

int main(int argc, const char* argv[])
{
    pgf::CommandOptions options;
    options.AddOption("help",   'h',  pgf::CommandOptionValueType::Bool);
    options.AddOption("max",    '\0', pgf::CommandOptionValueType::Int);
    options.AddOption("prefix", '\0',  pgf::CommandOptionValueType::String);

    std::vector<std::string> args;
    args.reserve(argc - 1);

    std::transform(
        argv + 1,
        argv + argc,
        std::back_inserter(args),
        [](const char* arg) {
            return std::string(arg);
        }
    );

    options.ProcessArguments(args);

    if (argc == 1 ||
        std::get<bool>(options.FindOptionValue("help")->data))
    {
        DisplayHelp();
        return 0;
    }

    std::string prefix = std::get<std::string>(options.FindOptionValue("prefix")->data);
    if (!prefix.empty())
    {
        args.insert(args.begin(), prefix);
    }

    std::copy(
        args.begin(),
        args.end(),
        std::ostream_iterator<std::string>(std::cout, " ")
    );

    return 0;
}