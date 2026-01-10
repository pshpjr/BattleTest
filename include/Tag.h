#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include "base/Formattable.h"

class Tag : public Formattable {
public:
    // 빈 문자열을 나타내는 npos
    static const Tag npos;

    // 기본 생성자 - npos와 동일 (빈 벡터)
    Tag();

    // string_view로 받아서 '.'으로 분리하고 문자로 변환하여 저장
    explicit Tag(std::string_view name);

    // 동등 비교 연산자
    bool operator==(const Tag& other) const;
    bool operator!=(const Tag& other) const;

    // TagHelper에서 사용할 접근자
    [[nodiscard]] const std::vector<std::string>& GetParts() const;
    [[nodiscard]] bool IsNpos() const;

    // IStringify 구현
    [[nodiscard]] std::string ToString() const override;

    // 계층 구조 관련 메서드
    [[nodiscard]] std::optional<bool> IsParent(const Tag& child) const;
    [[nodiscard]] std::optional<bool> IsChild(const Tag& parent) const;
    [[nodiscard]] std::optional<bool> IsSibling(const Tag& other) const;

    // 태그 정보 메서드
    [[nodiscard]] size_t GetDepth() const;
    [[nodiscard]] Tag GetRoot() const;
    [[nodiscard]] Tag GetParent(size_t level) const;
    [[nodiscard]] Tag GetSubTag(size_t depth) const;

private:
    std::vector<std::string> parts_;
};

// 팩토리 함수 - 유효하지 않은 입력이면 nullopt 리턴
std::optional<Tag> MakeTag(std::string_view name);