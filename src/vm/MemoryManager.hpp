//
//  MemoryManager.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/30/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef MemoryManager_hpp
#define MemoryManager_hpp

#include <string>

#include <cstdint>
#include <cassert>

namespace scissum {
    
    class MemoryManager
    {
    public:
        
        typedef size_t HeapIndex;
        
        static MemoryManager &instance();
        
        virtual void initialize(size_t size) = 0;
        
        virtual void reset() = 0;
        
        virtual void load(std::string const &filename) = 0;
        
        virtual void save(std::string const &filename) = 0;
        
        virtual HeapIndex allocate(size_t bytes) = 0;
        
        virtual void free(HeapIndex p) = 0;
        
        virtual bool validate(HeapIndex p, size_t bytes) const = 0;
        
        virtual size_t sizeOf(HeapIndex index) const = 0;
        
        virtual uint8_t const *nativePtr(HeapIndex index) const = 0;
        
        virtual uint8_t *nativePtr(HeapIndex index) = 0;
        
        virtual HeapIndex pointerTo(void *p) = 0;
        
        virtual void printHeapChunks() = 0;
        
        virtual bool checkHeap() = 0;
        
    };
    
}

#endif /* MemoryManager_hpp */
