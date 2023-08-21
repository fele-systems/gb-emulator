#pragma once

#include <string>
#include <iterator>
#include <type_traits>

namespace fele
{

    template<typename _CharT, typename _Itr>
    std::basic_string<_CharT> join(const std::basic_string<_CharT> delim, _Itr begin, _Itr end)
    {
        if (begin == end) return "";
        std::string output;
        for(; begin != end; std::advance(begin, 1))
        {
            if constexpr (std::is_convertible_v<decltype(*begin), std::basic_string<_CharT>>)
            {
                output.append(*begin);
            }
            else {
                output.append(std::to_string(*begin));
            }
            
            output.append(delim);
        }
        
        output.resize(output.size() - delim.size());
        return output;
    }

    template<typename _CharT, typename _Itr, typename _Transf>
    std::basic_string<_CharT> join(const std::basic_string<_CharT> delim, _Transf transformer, _Itr begin, _Itr end)
    {
        static_assert(std::is_invocable_v<decltype(transformer), decltype(*begin)>, "transformer must be a function");
        if (begin == end) return "";
        std::string output;
        for(; begin != end; std::advance(begin, 1))
        {
            output.append(transformer(*begin));
            output.append(delim);
        }
        output.resize(output.size() - delim.size());
        return output;
    }

}