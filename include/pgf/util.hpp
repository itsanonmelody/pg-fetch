#pragma once

#include <string>
#include <type_traits>

#define LITTLE_ENDIAN 1
#define BIG_ENDIAN    2

namespace util
{
    bool StartsWith(const std::string& source, const std::string& front);
    std::string ToLower(const std::string& source);

    // int GetEndianess();

    // template<typename _Ty>
    // _Ty SwitchByteOrder(_Ty value)
    // {
    //     static_assert(std::is_integral<_Ty>::value, "_Ty must be an integral type!!");
    //     for (char* start = (char*)&value,
    //              * end   = (char*)&value + (sizeof(_Ty)-1);
    //          start < end; ++start, --end)
    //     {
    //         *start ^= *end ^= *start ^= *end;
    //     }

    //     return value;
    // }
}