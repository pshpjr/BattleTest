#include "TagHelper.h"

bool TagHelper::IsParentOf(const Tag& parent, const Tag& child)
{
    return parent.IsParent(child);
}

bool TagHelper::IsChildOf(const Tag& child, const Tag& parent)
{
    return child.IsChild(parent);
}

std::optional<Tag> TagHelper::GetCommonParent(const Tag& tag1, const Tag& tag2)
{
    return tag1.GetCommonParent(tag2);
}