//
//  test_bitwisetrie.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/1/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "utils/BitWiseTrie.hpp"

#include <iostream>
#include <stack>
#include <random>
#include <limits>

using namespace scissum;

template <class Key, class Value>
void printTree(BitWiseTrie<Key, Value> const &trie)
{
    std::stack<std::pair<typename BitWiseTrie<Key, Value>::Node const *, size_t>> st;
    
    /*
#ifdef __SCISSUM_BITWISE_TRIE_USE_ZERO_TABLE
    
    for (size_t i = 0; i < sizeof(Key)*8+1; ++i) {
        st.push(std::make_pair(trie.roots(i), i));
    }
    
#else
    */
    
    st.push(std::make_pair(trie.root(), 0));
    
//#endif
    
    std::cout << "digraph G {\n";
    
    while (!st.empty()) {
        auto const *n = st.top().first;
        size_t level = st.top().second;
        st.pop();
        
        if (level < 32) {
            std::cout << "\t\"node" << reinterpret_cast<size_t>(n) << "\"[label=\"" << level << "\"];\n";
        } else {
            std::cout << "\t\"node" << reinterpret_cast<size_t>(n) << "\"[label=\"=" << static_cast<typename BitWiseTrie<Key, Value>::Leaf const*>(n)->value << "\"];\n";
        }
        
        
        if (n->childs[0]) {
            std::cout << "\t\"node" << reinterpret_cast<size_t>(n) << "\" -> \"node" << reinterpret_cast<size_t>(n->childs[0]) << "\"[label=\"0\"];\n";
            st.push(std::make_pair(n->childs[0], level+1));
        }
        
        if (n->childs[1]) {
            std::cout << "\t\"node" << reinterpret_cast<size_t>(n) << "\" -> \"node" << reinterpret_cast<size_t>(n->childs[1]) << "\"[label=\"1\"];\n";
            st.push(std::make_pair(n->childs[1], level+1));
        }
    }
    
    std::cout << "}\n";
}

void test0()
{
    BitWiseTrie<uint32_t, int> trie;
    
    trie.insert(10)->value = 1;
    trie.insert(11)->value = 2;
//    trie.insert(11);
    trie.insert(1234)->value = 3;
    
    printTree(trie);
}

void test1()
{
    BitWiseTrie<uint32_t, int> trie;

    std::random_device rd;
    std::default_random_engine rnd(rd());
    std::uniform_int_distribution<uint32_t> uniform_dist(1, 1000);//std::numeric_limits<uint32_t>::max());
    
    for (size_t i = 0; i < 20000000; ++i) {
        trie.insert(uniform_dist(rnd))->value = i;
    }
    
//    printTree(trie);
    
    std::cout << "test1: SUCCESS.\n";
}

void test1b()
{
    BitWiseTrie<uint32_t, int> trie;
    
    for (size_t i = 0; i < 20000000; ++i) {
        trie.insert(i)->value = i;
    }

    for (size_t i = 0; i < 20000000; ++i) {
        auto *n = trie.find(i);
        
        if (n == nullptr || !n->leaf || reinterpret_cast<BitWiseTrie<uint32_t, int>::Leaf*>(n)->value != i) {
            std::cout << "test1b: FAILED.\n";
            exit(1);
        }
    }
    
    //    printTree(trie);
    
    std::cout << "test1b: SUCCESS.\n";
}


void test2()
{
    BitWiseTrie<uint32_t, int> trie;

    std::random_device rd;
    std::default_random_engine rnd(rd());
    std::uniform_int_distribution<uint32_t> uniform_dist(1, 1000);//std::numeric_limits<uint32_t>::max());
    
    int mi = 1000;
    int ma = 0;
    
    for (size_t i = 0; i < 1000; ++i) {
        int value = uniform_dist(rnd);
        auto *n = trie.insert(value);
        
        if (!n->leaf) {
            std::cout << "test2: FAILED (added Node is not a leaf).\n";
            exit(1);
        }
        
        n->value = value;
        
        mi = std::min(mi, value);
        ma = std::max(ma, value);
    }
    
    auto mif = trie.minimum(trie.root());
    
    if (!mif->leaf) {
        std::cout << "test2: FAILED (mif != leaf).\n";
        exit(1);
    }
    
    if (mif->value != mi) {
        std::cout << "test2: FAILED (minimum incorrect, mif=" << mif->value << ", mi=" << mi << ").\n";
        exit(1);
    }
    
    auto maf = trie.maximum(trie.root());
    
    if (!maf->leaf) {
        std::cout << "test2: FAILED (maf != leaf).\n";
        exit(1);
    }
    
    if (maf->value != ma) {
        std::cout << "test2: FAILED (maximum incorrect, maf=" << maf->value << ", ma=" << ma << ").\n";
        exit(1);
    }
    
    std::cout << "test2: SUCCESS.\n";
}

void test3()
{
    BitWiseTrie<uint32_t, int> trie;

    trie.insert(12)->value = 12;
    trie.insert(9)->value = 9;
    trie.insert(1)->value = 1;
    trie.insert(6)->value = 6;
    
    auto n = trie.minimum(trie.root());
    
    if (n->value != 1) {
        std::cout << "test3: FAILED (wrong minimum).\n";
        exit(1);
    }
    
    n = trie.successor(n);

    if (n->value != 6) {
        std::cout << "test3: FAILED (wrong 1st successor (" << n->value << ")).\n";
        exit(1);
    }

    n = trie.successor(n);
    
    if (n->value != 9) {
        std::cout << "test3: FAILED (wrong 2nd successor).\n";
        exit(1);
    }

    n = trie.successor(n);
    
    if (n->value != 12) {
        std::cout << "test3: FAILED (wrong 3nd successor).\n";
        exit(1);
    }
    
    std::cout << "test3: SUCCESS.\n";
}

int main()
{
//    test0();
    test1();
    test1b();
    test2();
    test3();
    
    return 0;
}
