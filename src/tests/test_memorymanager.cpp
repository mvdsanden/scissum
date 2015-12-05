//
//  test_memorymanager.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/30/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "vm/HeapAllocator.hpp"

#include <iostream>
#include <list>
#include <random>

using namespace scissum;

void testa()
{
    HeapAllocator<uint32_t> a;
    
    auto p0 = a.allocate(1);
    auto p1 = a.allocate(1);
    auto p2 = a.allocate(2);
    
    a.deallocate(p1);
    
    auto p3 = a.allocate(3);
}

void test0()
{
    auto p0 = HeapAllocator<uint8_t>().allocate(10);
    
//    std::cout << "p0.size = " << p0.size() << ".\n";
    
    if (p0.size() < 10) {
        std::cout << "test0: FAILED (allocation to small).\n";
        exit(1);
    }
    
    *p0 = 0x12;
    
    if (*p0 != 0x12) {
        std::cout << "test0: FAILED (wrong value).\n";
        exit(1);
    }
    
    std::cout << "test0: SUCCESS.\n";
}


void test1()
{
    HeapAllocator<uint8_t> allocator;
    
    std::list<HeapPointerBase> l;
    
    if (!MemoryManager::instance().checkHeap()) {
        exit(2);
    }
    
    for (size_t i = 0; i < 5000; ++i) {
        
        
        HeapPointerBase p0;
        
        try {
            p0 = allocator.allocate(10);
        } catch (std::bad_alloc &e) {
            std::cerr << "Out of memory!\n";
            MemoryManager::instance().printHeapChunks();
            MemoryManager::instance().checkHeap();
            break;
            
        }
        
//        std::cout << reinterpret_cast<void*>(p0.nativePtr()) << ": p0.size = " << p0.size() << ".\n";
        
        if (p0.size() < 10) {
            std::cout << "test1: FAILED (allocation to small).\n";
            exit(1);
        }
        
        l.push_back(p0);
    }
    
    
    for (auto &p : l) {
        allocator.deallocate(p);
    }
    
    
    if (!MemoryManager::instance().checkHeap()) {
        exit(2);
    }
    
    std::cout << "test1: SUCCESS.\n";
}

void test2()
{
    HeapAllocator<size_t> allocator;
    
    std::list<std::pair<HeapPointer<size_t>, size_t>> l;
    
    std::random_device rd;
    std::default_random_engine rnd(rd());
    
    try {
        for (size_t i = 0; i < 10024*1024; ++i) {
            
            size_t v = rnd();
            
//            std::cout << v << "\n";
            
            auto p0 = allocator.allocate(1);
            
            //std::cout << reinterpret_cast<void*>(p0.nativePtr()) << ": p0.size = " << p0.size() << ".\n";
            
            if (p0.size() < sizeof(size_t)) {
                std::cout << "test2: FAILED (allocation to small).\n";
                exit(1);
            }
            
            *p0 = v;
            
            l.push_back(std::make_pair(p0, v));
        }

    } catch (...) {
        for (auto &p : l) {
            
            if (*(p.first) != p.second) {
                std::cout << "test2: FAILED (values do not correspond).\n";
                exit(1);
            }
            
//            std::cout << "+";
            
            allocator.deallocate(p.first);
        }
        std::cout << "test2: SUCCESS.\n";
        return;
    }
    
    std::cout << "test2: FAILED (allocation to small).\n";
    exit(1);
}

void test3()
{
    HeapAllocator<void> allocator;
    
    for (size_t i = 0; i < 1000000; ++i) {
        auto p = allocator.allocate(1234);
        allocator.deallocate(p);
    }
    
    std::cout << "test3: SUCCESS.\n";
}

void test4()
{
    HeapAllocator<void> allocator;
    
    std::random_device rd;
    std::default_random_engine rnd(rd());
    std::uniform_int_distribution<int> uniform_dist(1, 1234);
    
    for (size_t i = 0; i < 1000000; ++i) {
        auto p = allocator.allocate(uniform_dist(rnd));
        allocator.deallocate(p);
    }
    
    std::cout << "test4: SUCCESS.\n";
}

int main()
{
    MemoryManager::instance().initialize(100 * 1024 * 1024);
    
    testa();
    
    test0();
    
    for (size_t i = 0; i < 1000; ++i) {
        std::cout << "TEST1 #" << i << ".\n";
        
        //MemoryManager::instance().printHeapChunks();
        
        test1();
    }

    
    test2();
    
    test3();
    
    test4();
    
    return 0;
}
