#include <gtest/gtest.h>
#include "TagHelper.h"

// IsParentOf 테스트
TEST(TagHelperTest, IsParentOf_BasicRelationship)
{
    Tag parent("weapon");
    Tag child("weapon.sword");

    EXPECT_TRUE(TagHelper::IsParentOf(parent, child));
}

TEST(TagHelperTest, IsParentOf_NotParent)
{
    Tag tag1("weapon");
    Tag tag2("armor");

    EXPECT_FALSE(TagHelper::IsParentOf(tag1, tag2));
}

TEST(TagHelperTest, IsParentOf_SameTag)
{
    Tag tag("weapon");

    EXPECT_TRUE(TagHelper::IsParentOf(tag, tag));
}

TEST(TagHelperTest, IsParentOf_WithNpos)
{
    Tag tag("weapon");
    Tag npos;

    EXPECT_FALSE(TagHelper::IsParentOf(tag, npos));
    EXPECT_FALSE(TagHelper::IsParentOf(npos, tag));
}

// IsChildOf 테스트
TEST(TagHelperTest, IsChildOf_BasicRelationship)
{
    Tag parent("weapon");
    Tag child("weapon.sword");

    EXPECT_TRUE(TagHelper::IsChildOf(child, parent));
}

TEST(TagHelperTest, IsChildOf_NotChild)
{
    Tag tag1("weapon");
    Tag tag2("armor");

    EXPECT_FALSE(TagHelper::IsChildOf(tag2, tag1));
}

TEST(TagHelperTest, IsChildOf_MultiLevel)
{
    Tag root("weapon");
    Tag grandchild("weapon.sword.legendary");

    EXPECT_TRUE(TagHelper::IsChildOf(grandchild, root));
}

// GetCommonParent 테스트
TEST(TagHelperTest, GetCommonParent_SameRoot)
{
    Tag tag1("weapon.sword.legendary");
    Tag tag2("weapon.bow.epic");

    auto common = TagHelper::GetCommonParent(tag1, tag2);
    ASSERT_TRUE(common.has_value());
    EXPECT_EQ(common->ToString(), "weapon");
}

TEST(TagHelperTest, GetCommonParent_DeeperCommonParent)
{
    Tag tag1("weapon.sword.legendary.excalibur");
    Tag tag2("weapon.sword.epic.durandal");

    auto common = TagHelper::GetCommonParent(tag1, tag2);
    ASSERT_TRUE(common.has_value());
    EXPECT_EQ(common->ToString(), "weapon.sword");
}

TEST(TagHelperTest, GetCommonParent_NoCommonParent)
{
    Tag tag1("weapon.sword");
    Tag tag2("armor.helmet");

    auto common = TagHelper::GetCommonParent(tag1, tag2);
    EXPECT_FALSE(common.has_value());
}

TEST(TagHelperTest, GetCommonParent_SameTags)
{
    Tag tag("weapon.sword");

    auto common = TagHelper::GetCommonParent(tag, tag);
    ASSERT_TRUE(common.has_value());
    EXPECT_EQ(common->ToString(), "weapon.sword");
}

TEST(TagHelperTest, GetCommonParent_OneIsParent)
{
    Tag parent("weapon");
    Tag child("weapon.sword.legendary");

    auto common = TagHelper::GetCommonParent(parent, child);
    ASSERT_TRUE(common.has_value());
    EXPECT_EQ(common->ToString(), "weapon");
}

TEST(TagHelperTest, GetCommonParent_WithNpos)
{
    Tag tag("weapon");
    Tag npos;

    EXPECT_FALSE(TagHelper::GetCommonParent(tag, npos).has_value());
    EXPECT_FALSE(TagHelper::GetCommonParent(npos, tag).has_value());
}

TEST(TagHelperTest, GetCommonParent_SingleLevel)
{
    Tag tag1("weapon");
    Tag tag2("armor");

    auto common = TagHelper::GetCommonParent(tag1, tag2);
    EXPECT_FALSE(common.has_value());
}
