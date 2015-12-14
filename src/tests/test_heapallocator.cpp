//
//  test_heapallocator.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/3/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "vm/HeapAllocator.hpp"

#include <iostream>

#include <vector>
#include <list>

using namespace scissum;

void test0()
{
    HeapAllocator<int> allocator;
    
    auto p = allocator.allocate(1);
    
    *p = 12345;
    
    if (*p != 12345) {
        std::cout << "test0: FAILED.\n";
        exit(1);
    }
    
    std::cout << "test0: SUCCESS.\n";
}

template <class T>
using HeapVector = std::vector<T, HeapAllocator<T>>;

HeapPointer<HeapVector<int>> g_vpointer(0x48);

void test1()
{
    HeapAllocator<HeapVector<int>> allocator;
    
//    auto v = allocator.construct();
    
    auto v = g_vpointer = allocator.allocate(1);
    allocator.construct(v);
    
//    HeapVector<int> v;
    
    v->push_back(-1);
    v->push_back(100);
    v->push_back(1234);
    
    std::cout << "0: " << (*v)[0] << ".\n";
    
    for (auto i : *v) {
        std::cout << i << ".\n";
    }
    
    MemoryManager::instance().save("test.heap");
    
    std::cout << "vpointer=" << g_vpointer << ".\n";
    
}

void test2()
{
    MemoryManager::instance().load("test.heap");
    
    auto v = g_vpointer;
    
    for (auto &i : *v) {
        std::cout << i << ".\n";
    }
}

template <class T>
using HeapList = std::list<T, HeapAllocator<T>>;


void test3()
{
    HeapAllocator<HeapList<int>> allocator;
    
    //    auto v = allocator.construct();
    
    auto v = allocator.allocate(1);
    allocator.construct(v);
    
    //    HeapVector<int> v;
    
    v->push_back(-1);
    v->push_back(100);
    v->push_back(1234);
    
    for (auto i : *v) {
        std::cout << i << ".\n";
    }
    
    std::cout << v->back() << ".\n";
    
}

void test4()
{
    HeapAllocator<std::tuple<size_t, size_t, size_t>> allocator;
    
    auto v = allocator.allocate(1);
    allocator.construct(v, 1, 2, 3);
    
    std::cout << "Size of heap tuple: " << v.size() << ".\n";
    
    HeapPointer<size_t> other = v;
    
    for (size_t i = 0; i < 3; ++i) {
        std::cout << other[i] << ".\n";
    }
}

int main()
{
    MemoryManager::instance().initialize(25 * 1024 * 1024);

    test2();
    
    test0();
    
    test1();
    
    test2();
    
    test3();
    
    test4();
    
    return 0;
}
