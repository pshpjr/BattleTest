#pragma once

#include "Tag.h"
#include <optional>

class TagHelper {
public:
    // parent가 child의 부모인지 확인
    // npos 태그는 nullopt 리턴, 같은 태그는 true 리턴
    static std::optional<bool> IsParentOf(const Tag& parent, const Tag& child);

    // child가 parent의 자식인지 확인
    // npos 태그는 nullopt 리턴, 같은 태그는 true 리턴
    static std::optional<bool> IsChildOf(const Tag& child, const Tag& parent);

    // 두 태그의 공통 부모 반환
    // npos 태그나 공통 부모가 없으면 nullopt 리턴
    static std::optional<Tag> GetCommonParent(const Tag& tag1, const Tag& tag2);
};
