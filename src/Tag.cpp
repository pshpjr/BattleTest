#include "Tag.h"
#include "base/string.h"

// static 멤버 정의
const Tag Tag::npos = Tag();

// 기본 생성자 - 빈 벡터로 초기화
Tag::Tag() : parts_()
{
}

// string_view 생성자 - '.'으로 분리하고 소문자로 변환하여 저장
Tag::Tag(std::string_view name) : parts_(Split(name,'.'))
{
}

// 동등 비교
bool Tag::operator==(const Tag& other) const
{
    return parts_ == other.parts_;
}

// 부등 비교
bool Tag::operator!=(const Tag& other) const
{
    return parts_ != other.parts_;
}

// ToString 구현 - parts_를 '.'로 연결
std::string Tag::ToString() const
{
    if (parts_.empty())
    {
        return "";
    }

    std::string result;
    for (size_t i = 0; i < parts_.size(); ++i)
    {
        if (i > 0)
        {
            result += '.';
        }
        result += parts_[i];
    }
    return result;
}

// parent가 child의 부모인지 확인
bool Tag::IsParent(const Tag& child) const
{
    // npos 처리
    if (*this == Tag::npos || child == Tag::npos)
    {
        return false;
    }

    // 같은 태그면 서로가 부모/자식
    if (*this == child)
    {
        return true;
    }

    // this가 child보다 길거나 같으면 부모가 될 수 없음
    if (parts_.size() >= child.parts_.size())
    {
        return false;
    }

    // this의 모든 부분이 child의 앞부분과 일치하는지 확인
    for (size_t i = 0; i < parts_.size(); ++i)
    {
        if (parts_[i] != child.parts_[i])
        {
            return false;
        }
    }

    return true;
}

// child가 parent의 자식인지 확인
bool Tag::IsChild(const Tag& parent) const
{
    return parent.IsParent(*this);
}

// 형제 관계 확인 (같은 부모를 가진 태그)
bool Tag::IsSibling(const Tag& other) const
{
    // npos 처리
    if (*this == Tag::npos || other == Tag::npos)
    {
        return false;
    }

    // 같은 태그는 형제가 아님
    if (*this == other)
    {
        return false;
    }

    // 깊이가 1이면 부모가 없으므로 형제 관계 아님
    if (parts_.size() == 1 || other.parts_.size() == 1)
    {
        return false;
    }

    // 깊이가 다르면 형제가 아님
    if (parts_.size() != other.parts_.size())
    {
        return false;
    }

    // 마지막 요소 제외하고 모두 같으면 형제
    for (size_t i = 0; i < parts_.size() - 1; ++i)
    {
        if (parts_[i] != other.parts_[i])
        {
            return false;
        }
    }

    return true;
}

// 깊이 반환
size_t Tag::GetDepth() const {
    return parts_.size();
}

// 루트 태그 반환
Tag Tag::GetRoot() const {
    // npos는 npos가 루트
    if (*this == Tag::npos) {
        return Tag::npos;
    }

    // 첫 번째 요소만 가진 Tag 생성
    return Tag(parts_[0]);
}

// level 깊이의 부모 반환 (parts_[0..level]까지)
Tag Tag::GetParent(size_t level) const {
    // level이 범위를 벗어나면 npos
    if (level >= parts_.size()) {
        return Tag::npos;
    }

    // parts_[0..level]까지 연결
    std::string result;
    for (size_t i = 0; i <= level; ++i) {
        if (i > 0) {
            result += '.';
        }
        result += parts_[i];
    }

    return Tag(result);
}

// 루트부터 depth 깊이까지의 태그 반환
Tag Tag::GetSubTag(size_t depth) const {
    // depth가 size보다 크면 자기 자신 반환
    if (depth >= parts_.size()) {
        return *this;
    }

    // parts_[0..depth-1]까지 연결
    std::string result;
    for (size_t i = 0; i < depth; ++i) {
        if (i > 0) {
            result += '.';
        }
        result += parts_[i];
    }

    return Tag(result);
}

// 두 태그의 공통 부모 반환
std::optional<Tag> Tag::GetCommonParent(const Tag& other) const
{
    // npos 처리
    if (*this == Tag::npos || other == Tag::npos)
    {
        return std::nullopt;
    }

    std::vector<std::string> common_parts;
    size_t min_size = std::min(parts_.size(), other.parts_.size());

    for (size_t i = 0; i < min_size; ++i)
    {
        if (parts_[i] == other.parts_[i])
        {
            common_parts.push_back(parts_[i]);
        }
        else
        {
            break;
        }
    }

    // 공통 부모가 없으면 nullopt
    if (common_parts.empty())
    {
        return std::nullopt;
    }

    // 공통 부분을 '.'로 연결해서 Tag 생성
    std::string common_str;
    for (size_t i = 0; i < common_parts.size(); ++i)
    {
        if (i > 0)
        {
            common_str += '.';
        }
        common_str += common_parts[i];
    }

    return Tag(common_str);
}

// 팩토리 함수 - 유효하지 않은 입력이면 nullopt 리턴
std::optional<Tag> MakeTag(std::string_view name) {
    // 빈 문자열
    if (name.empty()) {
        return std::nullopt;
    }

    // '.'으로 분리하여 각 부분이 비어있지 않은지 검사
    auto parts = Split(name, '.');
    for (const auto& part : parts) {
        if (part.empty()) {
            return std::nullopt;
        }
    }

    return Tag(name);
}