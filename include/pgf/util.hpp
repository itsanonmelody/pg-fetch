#pragma once

#include <string>

namespace util
{
    bool StartsWith(const std::string& source, const std::string& front);
    std::string ToLower(const std::string& source);
}