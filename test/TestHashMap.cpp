#include "gtest/gtest.h"
#include <ccinfra/ctnr/map/HashMap.h>
#include <ccinfra/base/Keywords.h>
#include <string>

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
    ASSERT_EQ(__null_ptr, map.get(1));
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
    ASSERT_EQ(__null_ptr, map.get(2));
}

TEST(HashMapTest, should_clear_the_map_correctly)
{
    HashMap<int, int> map;

    map.put(1, 2);
    map.put(2, 4);

    map.clear();

    ASSERT_TRUE(map.empty());
    ASSERT_EQ(0, map.size());
    ASSERT_EQ(__null_ptr, map.get(1));
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

TEST(HashMapTest, should_point_to_correct_node_by_iterator_when_map_is_empty)
{
    HashMap<int, int> map;

    ASSERT_TRUE(map.begin().isNull());
    ASSERT_TRUE(map.end().isNull());
}

TEST(HashMapTest, should_point_to_correct_node_by_iterator_when_map_has_valid_elem)
{
    HashMap<int, int> map;

    map.put(1, 2);

    ASSERT_FALSE(map.begin().isNull());
    ASSERT_EQ(1, map.begin()->key);
    ASSERT_EQ(2, map.begin()->value);
    ASSERT_TRUE(map.end().isNull());
}

TEST(HashMapTest, should_point_to_correct_node_when_iterator_forward)
{
    typedef HashMap<int, int> ThisMap;
    ThisMap map;

    map.put(1, 2);
    map.put(3, 6);

    ThisMap::Iterator p(map.begin());
    ASSERT_EQ(map.begin(), p++);
    ASSERT_EQ(3, p.getValue()->key);
    ASSERT_EQ(6, p.getValue()->value);
    ASSERT_EQ(map.end(), ++p);
}

TEST(HashMapTest, should_travel_the_map_by_iterator)
{
    typedef HashMap<int, int, 5, 5> ThisMap;
    ThisMap map;

    map.put(1, 2);
    map.put(3, 6);
    map.put(8, 16);
    map.put(5, 10);

    int keySum = 0;
    int valueSum = 0;

    MAP_FOREACH(ThisMap, i, map)
    {
        keySum += i->key;
        valueSum += i->value;
    }

    ASSERT_EQ(1 + 3 + 8 + 5, keySum);
    ASSERT_EQ(2 + 6 + 16 + 10, valueSum);
}

TEST(HashMapTest, should_put_and_get_when_hash_string)
{
    HashMap<const char*, int> map;

    map["hello"] = 5;
    map["ni hao ma"] = 9;

    ASSERT_EQ(2, map.size());
    ASSERT_EQ(5, map["hello"]);
    ASSERT_EQ(9, map["ni hao ma"]);
}

namespace
{
    struct MultipleValueMapVisitor : MapVisitor<int, int>
    {
        MultipleValueMapVisitor(int times) : times(times)
        {
        }

    private:
        OVERRIDE(Status visit(const int& key, int& value))
        {
            value = times * key;
            return CCINFRA_SUCCESS;
        }

    private:
        const int times;
    }multipleVisitor(3);
}

TEST(HashMapTest, shoud_double_all_value_through_visitor)
{
    HashMap<int, int> map;

    map[1] = 1;
    map[2] = 2;

    map.visit(multipleVisitor);

    ASSERT_EQ(3, map[1]);
    ASSERT_EQ(6, map[2]);
}


namespace
{
    struct PrintMapVisitor : ConstMapVisitor<int, const char*>
    {
        std::string result;

    private:
        OVERRIDE(Status visit(const int& key, const char* const& value))
        {
            std::stringstream ss;
            ss << "map[" << key <<"] = " << value << " \n";
            result += ss.str();
            return CCINFRA_SUCCESS;
        }
    }printVisitor;
}

TEST(HashMapTest, should_dump_map_through_const_visitor)
{
    HashMap<int, const char*> map;

    map[1] = "one";
    map[2] = "two";
    map[3] = "three";

    map.visit(printVisitor);

    ASSERT_EQ("map[1] = one \nmap[2] = two \nmap[3] = three \n", printVisitor.result);
}

namespace
{
    struct Key
    {
        Key(int x, int y)
        : x(x), y(y)
        {
        }

        size_t hash() const
        {
            return (x + y);
        }

        __INLINE_EQUALS(Key)
        {
            return (x == rhs.x) && (y == rhs.y);
        }

    private:
        int x;
        int y;
    };

    struct Value
    {
        Value() : value(__null_ptr){}
        Value(const char* v) : value(v)
        {
        }

        const char* getValue() const
        {
            return value;
        }

        bool operator==(const Value& rhs) const
        {
            return strcmp(value, rhs.value) == 0;
        }

    private:
        const char* value;
    };
}

template<size_t HASH_SIZE>
struct HashFn<Key, HASH_SIZE>
{
    size_t operator()(const Key& key) const
    {
        return key.hash();
    }
};

TEST(HashMapTest, should_work_with_user_defined_hash_and_equal_function)
{
    HashMap<Key, Value> map;

    map.put(Key(1, 3), Value("four"));
    map[Key(2, 3)] = Value("five");

    ASSERT_EQ(Value("four"), map[Key(1, 3)]);
    ASSERT_EQ(Value("five"), *map.get(Key(2, 3)));
    ASSERT_EQ(__null_ptr,  map.get(Key(2, 4)));
}

TEST(HashMapTest, should_store_the_pointer_to_value)
{
    Value v1("one");
    Value v2("two");

    HashMap<Key, Value*> map;

    map.put(Key(0, 1), &v1);
    map[Key(1, 1)] = &v2;

    ASSERT_EQ(&v1, map[Key(0, 1)]);
    ASSERT_EQ(&v2, *map.get(Key(1, 1)));
    ASSERT_EQ(__null_ptr,  map.get(Key(2, 4)));
}




