#pragma once

#include <string>

// 문자열로 변환 가능한 객체의 인터페이스
class Formattable
{
public:
    virtual ~Formattable() = default;
    [[nodiscard]] virtual std::string ToString() const = 0;
};
