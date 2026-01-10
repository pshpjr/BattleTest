#include "TagHelper.h"
#include <algorithm>

std::optional<bool> TagHelper::IsParentOf(const Tag& parent, const Tag& child) {
    return parent.IsParent(child);
}

std::optional<bool> TagHelper::IsChildOf(const Tag& child, const Tag& parent) {
    return child.IsChild(parent);
}

std::optional<Tag> TagHelper::GetCommonParent(const Tag& tag1, const Tag& tag2) {
    // npos 처리
    if (tag1.IsNpos() || tag2.IsNpos()) {
        return std::nullopt;
    }

    const auto& parts1 = tag1.GetParts();
    const auto& parts2 = tag2.GetParts();

    std::vector<std::string> common_parts;
    size_t min_size = std::min(parts1.size(), parts2.size());

    for (size_t i = 0; i < min_size; ++i) {
        if (parts1[i] == parts2[i]) {
            common_parts.push_back(parts1[i]);
        } else {
            break;
        }
    }

    // 공통 부모가 없으면 nullopt
    if (common_parts.empty()) {
        return std::nullopt;
    }

    // 공통 부분을 '.'로 연결해서 Tag 생성
    std::string common_str;
    for (size_t i = 0; i < common_parts.size(); ++i) {
        if (i > 0) {
            common_str += '.';
        }
        common_str += common_parts[i];
    }

    return Tag(common_str);
}