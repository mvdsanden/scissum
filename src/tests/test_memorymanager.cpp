//
//  test_memorymanager.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/30/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "vm/MemoryManager.hpp"

#include <iostream>
#include <list>
#include <random>

using namespace scissum;

void test0()
{
    MemoryManager &mm = MemoryManager::instance();
    
    auto p0 = mm.allocate(10);
    
//    std::cout << "p0.size = " << p0.size() << ".\n";
    
    if (p0.size() < 10) {
        std::cout << "test0: FAILED (allocation to small).\n";
        exit(1);
    }
    
    p0.as<uint8_t>() = 0x12;
    
    if (p0.as<uint8_t>() != 0x12) {
        std::cout << "test0: FAILED (wrong value).\n";
        exit(1);
    }
    
    std::cout << "test0: SUCCESS.\n";
}

void test1()
{
    MemoryManager &mm = MemoryManager::instance();

    std::list<MemoryManager::Ptr> l;
    
    for (size_t i = 0; i < 1000; ++i) {
        auto p0 = mm.allocate(10);
        
//        std::cout << reinterpret_cast<void*>(p0.nativePtr()) << ": p0.size = " << p0.size() << ".\n";
        
        if (p0.size() < 10) {
            std::cout << "test1: FAILED (allocation to small).\n";
            exit(1);
        }
        
        l.push_back(p0);
    }
    
    for (auto &p : l) {
        mm.free(p);
    }
    
    std::cout << "test1: SUCCESS.\n";
}

void test2()
{
    MemoryManager &mm = MemoryManager::instance();
    
    std::list<std::pair<MemoryManager::Ptr, size_t>> l;
    
    std::random_device rd;
    std::default_random_engine rnd(rd());
    
    try {
        for (size_t i = 0; i < 10024*1024; ++i) {
            size_t v = rnd();
            
//            std::cout << v << "\n";
            
            auto p0 = mm.allocate(sizeof(size_t));
            
            //std::cout << reinterpret_cast<void*>(p0.nativePtr()) << ": p0.size = " << p0.size() << ".\n";
            
            if (p0.size() < sizeof(size_t)) {
                std::cout << "test2: FAILED (allocation to small).\n";
                exit(1);
            }
            
            p0.as<size_t>() = v;
            
            l.push_back(std::make_pair(p0, v));
        }

    } catch (...) {
        for (auto &p : l) {
            
            if (p.first.as<size_t>() != p.second) {
                std::cout << "test2: FAILED (values do not correspond).\n";
                exit(1);
            }
            
//            std::cout << "+";
            
            mm.free(p.first);
        }
        std::cout << "test2: SUCCESS.\n";
        return;
    }
    
    std::cout << "test2: FAILED (allocation to small).\n";
    exit(1);
}

void test3()
{
    MemoryManager &mm = MemoryManager::instance();
    
    for (size_t i = 0; i < 1000000; ++i) {
        auto p = mm.allocate(1234);
        mm.free(p);
    }
    
    std::cout << "test3: SUCCESS.\n";
}

void test4()
{
    MemoryManager &mm = MemoryManager::instance();
    
    std::random_device rd;
    std::default_random_engine rnd(rd());
    std::uniform_int_distribution<int> uniform_dist(1, 1234);
    
    for (size_t i = 0; i < 1000000; ++i) {
        auto p = mm.allocate(uniform_dist(rnd));
        mm.free(p);
    }
    
    std::cout << "test4: SUCCESS.\n";
}

int main()
{
    MemoryManager::instance().initialize(100 * 1024 * 1024);
    
    test0();
    
    for (size_t i = 0; i < 1000; ++i) {
        test1();
    }
    
    test2();
    test3();
    test4();
    
    return 0;
}
