#include "pgf/util.hpp"

#include <locale>
#include <algorithm>

bool util::StartsWith(const std::string& source, const std::string& front)
{
    return !(front.empty() || front.size() > source.size()) &&
            (source.substr(0, front.size()) == front);
}

std::string util::ToLower(const std::string& source)
{
    std::string lower;
    std::transform(
        source.begin(),
        source.end(),
        std::back_inserter(lower),
        [](char c) {
            return std::tolower(c);
        }
    );

    return lower;
}

// int util::GetEndianess()
// {
//     union Data
//     {
//         int value;
//         char bytes[4];
//     };
    
//     static int endianess = (Data{0x01020304}.bytes[0] == 0x01) ? BIG_ENDIAN : LITTLE_ENDIAN;
//     return endianess;
// }