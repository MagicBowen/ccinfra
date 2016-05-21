#ifndef HF41FC0CC_15A0_4F16_92AE_15D0759215A8
#define HF41FC0CC_15A0_4F16_92AE_15D0759215A8

#include "ccinfra/container/map/HashFn.h"
#include "ccinfra/container/map/EqualFn.h"
#include "ccinfra/container/list/ListElem.h"
#include "ccinfra/container/list/List.h"
#include "ccinfra/core/static_assert.h"
#include "ccinfra/memory/ObjectAllocator.h"
#include "ccinfra/algo/foreach.h"
#include "ccinfra/container/map/MapVisitor.h"

template < typename KEY
         , typename VALUE
         , size_t   ELEM_SIZE = 1024
         , size_t   HASH_SIZE = 1024
         , typename HASH_FN = HashFn<KEY>
         , typename EQUAL_FN = EqualFn<KEY> >
struct HashMap
{
    template <typename K, typename V>
    struct HashNode : ListElem< HashNode<K, V> >
    {
        HashNode(const K& k, const V& v)
        : key(k), value(v)
        {
        }

        K key;
        V value;
    };

    typedef HashNode<KEY, VALUE> Node;

    HashMap() : num(0)
    {
        STATIC_ASSERT(ELEM_SIZE > 0);
        STATIC_ASSERT(HASH_SIZE > 0);
    }

    size_t size() const
    {
        return num;
    }

    size_t maxSize() const
    {
        return ELEM_SIZE;
    }

    bool empty() const
    {
        return num == 0;
    }

    bool full() const
    {
        return !allocator.hasFreeSlot();
    }

    const VALUE* get(const KEY& key) const
    {
        Node* node = find(key);
        if(__null__(node))
        {
            return __null_ptr__;
        }

        return &(node->value);
    }

    Status put(const KEY& key, const VALUE& value)
    {
        const VALUE* v = get(key);

        if(__notnull__(v))
        {
            *const_cast<VALUE*>(v) = value;
            return CCINFRA_SUCCESS;
        }

        return __null__(insert(key ,value)) ? CCINFRA_FAILURE : CCINFRA_SUCCESS;
    }

    VALUE& operator[](const KEY& key)
    {
        const VALUE* v = get(key);

        if(__notnull__(v))
        {
            return *const_cast<VALUE*>(v);
        }

        return insert(key, VALUE())->value;
    }

    void erase(const KEY& key)
    {
        Node* node = find(key);

        if(__notnull__(node))
        {
            buckets[getIndex(key)].remove(*node);
            allocator.free(node);
            num--;
        }
    }

    void clear()
    {
        FOR_EACH_0(i, HASH_SIZE)
        {
            while(Node* p = buckets[i].popFront())
            {
                allocator.free(p);
            }
        }

        num = 0;
    }

    Status visit(MapVisitor<KEY, VALUE>& visitor)
    {
        return access(visitor);
    }

    Status visit(ConstMapVisitor<KEY, VALUE>& visitor) const
    {
        return const_cast<HashMap&>(*this).access(visitor);
    }

private:
    template<typename VISITOR>
    Status access(VISITOR& visitor)
    {
        FOR_EACH_0(i, HASH_SIZE)
        {
            LIST_FOREACH(Node, node, buckets[i])
            {
                CCINFRA_ASSERT_SUCC_CALL(visitor.visit(node->key, node->value));
            }
        }

        return CCINFRA_SUCCESS;
    }

    size_t getIndex(const KEY &key) const
    {
        return hashFn(key) % HASH_SIZE;
    }

    Node* insert(const KEY& key, const VALUE& value)
    {
        void* p = allocator.alloc();
        if(__null__(p)) return __null_ptr__;

        Node* node = new (p) Node(key, value);
        buckets[getIndex(key)].pushBack(*node);

        num++;

        return node;
    }

    Node* find(const KEY& key) const
    {
        Node* node = buckets[getIndex(key)].search([&](const Node& node){ return equalFn(node.key, key);});
        if(__null__(node))
        {
            return __null_ptr__;
        }

        return node;
    }

private:
    typedef List<Node> Bucket;
    Bucket buckets[HASH_SIZE];
    unsigned int num;

    ObjectAllocator<Node, ELEM_SIZE> allocator;

private:
    HASH_FN  hashFn;
    EQUAL_FN equalFn;
};

#endif
