#include "base/string.h"
#include <algorithm>
#include <cctype>

std::string MakeLower(std::string str)
{
    std::ranges::transform(str, str.begin(),
                           [](unsigned char c) { return std::tolower(c); });
    return str;
}

std::vector<std::string> Split(std::string_view str, const std::vector<char>& delimiters)
{
    std::vector<std::string> result;
    std::string::size_type start = 0;

    for (std::string::size_type i = 0; i < str.length(); ++i)
    {
        // 현재 문자가 구분자인지 확인
        bool is_delimiter = false;
        for (char delimiter : delimiters)
        {
            if (str[i] == delimiter)
            {
                is_delimiter = true;
                break;
            }
        }

        if (is_delimiter)
        {
            // 구분자를 발견하면 현재까지의 문자열을 추가
            result.push_back(std::string(str.substr(start, i - start)));
            start = i + 1;
        }
    }

    // 마지막 부분 추가
    result.push_back(std::string(str.substr(start)));
    return result;
}

std::vector<std::string> Split(std::string_view str, std::string_view delimiters)
{
    std::vector<char> delimiter_vec(delimiters.begin(), delimiters.end());
    return Split(str, delimiter_vec);
}

std::vector<std::string> Split(std::string_view str, char delimiter)
{
    return Split(str, std::vector<char>{delimiter});
}
