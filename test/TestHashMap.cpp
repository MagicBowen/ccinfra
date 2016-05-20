#include "gtest/gtest.h"
#include <ccinfra/container/map/HashMap.h>

TEST(HashMapTest, should_be_empty_when_init)
{
    HashMap<int, int> map;

    ASSERT_TRUE(map.empty());
    ASSERT_FALSE(map.full());
    ASSERT_EQ(0, map.size());
}

TEST(HashMapTest, should_get_the_value_from_key_when_inserted_success)
{
    HashMap<int, int> map;

    map.put(1, 2);
    map[2] = 4;

    ASSERT_FALSE(map.empty());
    ASSERT_FALSE(map.full());
    ASSERT_EQ(2, map.size());
    ASSERT_EQ(2, map[1]);
}

TEST(HashMapTest, should_erase_the_map_correctly_when_only_on_node_in_map)
{
    HashMap<int, int> map;

    map.put(1, 2);

    map.erase(1);

    ASSERT_TRUE(map.empty());
    ASSERT_EQ(0, map.size());
    ASSERT_EQ(__null_ptr__, map.get(1));
}

TEST(HashMapTest, should_erase_the_map_correctly_when_muti_node_in_map)
{
    HashMap<int, int> map;

    map.put(1, 2);
    map.put(2, 4);

    map.erase(2);

    ASSERT_FALSE(map.empty());
    ASSERT_EQ(1, map.size());
    ASSERT_EQ(2, *map.get(1));
    ASSERT_EQ(__null_ptr__, map.get(2));
}

TEST(HashMapTest, should_clear_the_map_correctly)
{
    HashMap<int, int> map;

    map.put(1, 2);
    map.put(2, 4);

    map.clear();

    ASSERT_TRUE(map.empty());
    ASSERT_EQ(0, map.size());
    ASSERT_EQ(__null_ptr__, map.get(1));
}

TEST(HashMapTest, should_not_change_the_value_when_same_key_already_in)
{
    HashMap<int, int> map;

    map.put(1, 2);
    map.put(1, 4);

    ASSERT_EQ(1, map.size());
    ASSERT_EQ(4, map[1]);
}

TEST(HashMapTest, should_be_full_when_reach_the_max_node_num)
{
    HashMap<int, int, 1> map;

    ASSERT_TRUE(__CCINFRA_SUCCESS(map.put(1, 2)));

    ASSERT_TRUE(map.full());
}

TEST(HashMapTest, should_put_fail_when_map_is_full)
{
    HashMap<int, int, 1> map;

    map.put(1, 2);
    ASSERT_TRUE(__CCINFRA_FAILED(map.put(2, 2)));

    ASSERT_EQ(1, map.size());
    ASSERT_EQ(2, map[1]);
}

TEST(HashMapTest, should_put_already_in_success_when_map_is_full)
{
    HashMap<int, int, 1> map;

    map.put(1, 2);
    ASSERT_TRUE(__CCINFRA_SUCCESS(map.put(1, 4)));

    ASSERT_EQ(1, map.size());
    ASSERT_EQ(4, map[1]);
}


