//
//  test_bitwisetrie.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/1/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "utils/BitWiseTrie2.hpp"

#include <iostream>
#include <stack>
#include <random>
#include <limits>

using namespace scissum;

template <class Key, class Value>
void printTree(BitWiseTrie<Key, Value> const &trie)
{
    std::stack<std::pair<typename BitWiseTrie<Key, Value>::Node const *, size_t>> st;
    
    for (size_t i = 0; i < sizeof(Key)*8+1; ++i) {
        st.push(std::make_pair(trie.roots(i), i));
    }
    
    std::cout << "digraph G {\n";
    
    while (!st.empty()) {
        auto const *n = st.top().first;
        size_t level = st.top().second;
        st.pop();
        
        if (level < 32) {
            std::cout << "\t\"node" << reinterpret_cast<size_t>(n) << "\"[label=\"" << level << "\"];\n";
        } else {
            std::cout << "\t\"node" << reinterpret_cast<size_t>(n) << "\"[label=\"=" << n->value << "\"];\n";
        }
        
        
        if (n->childs[0]) {
            auto const *c = trie.node(n->childs[0]);
            std::cout << "\t\"node" << reinterpret_cast<size_t>(n) << "\" -> \"node" << reinterpret_cast<size_t>(c) << "\"[label=\"0\"];\n";
            st.push(std::make_pair(c, level+1));
        }
        
        if (n->childs[1]) {
            auto const *c = trie.node(n->childs[1]);
            std::cout << "\t\"node" << reinterpret_cast<size_t>(n) << "\" -> \"node" << reinterpret_cast<size_t>(c) << "\"[label=\"1\"];\n";
            st.push(std::make_pair(c, level+1));
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
    
    for (size_t i = 0; i < 50000000; ++i) {
        trie.insert(uniform_dist(rnd));
    }
    
    //    printTree(trie);
}

int main()
{
    //test0();
    test1();
    
    return 0;
}
