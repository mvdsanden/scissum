//
//  MemoryManager.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/30/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef MemoryManager_hpp
#define MemoryManager_hpp

#include <cstdint>
#include <cassert>

namespace scissum {
    
    class MemoryManager
    {
    public:
        
        class Ptr
        {
            
            size_t d_handle;
            
        public:
            
            Ptr(size_t handle);
            
            size_t handle() const;
            size_t size() const;
            uint8_t const *nativePtr() const;
            uint8_t *nativePtr();
            
            template <class T> T const &as() const;
            template <class T> T &as();
            
        };
        
        static MemoryManager &instance();
        
        virtual void initialize(size_t size) = 0;
        
        virtual void reset() = 0;
        
        virtual void import(uint8_t *buffer, size_t size) = 0;
        
        virtual Ptr allocate(size_t bytes) = 0;
        
        virtual void free(Ptr const &p) = 0;
        
        virtual size_t sizeOf(size_t handle) const = 0;
        
        virtual uint8_t const *nativePtr(size_t handle) const = 0;
        
        virtual uint8_t *nativePtr(size_t handle) = 0;
        
    };
    
    inline MemoryManager::Ptr::Ptr(size_t handle)
    : d_handle(handle)
    {
    }    
    
    inline size_t MemoryManager::Ptr::handle() const
    {
        return d_handle;
    }
    
    inline size_t MemoryManager::Ptr::size() const
    {
        return MemoryManager::instance().sizeOf(d_handle);
    }
    
    inline uint8_t const *MemoryManager::Ptr::nativePtr() const
    {
        return MemoryManager::instance().nativePtr(d_handle);
    }
    
    inline uint8_t *MemoryManager::Ptr::nativePtr()
    {
        return MemoryManager::instance().nativePtr(d_handle);
    }
    
    template <class T>
    inline T const &MemoryManager::Ptr::as() const
    {
        assert(sizeof(T) <= size());
        return *reinterpret_cast<T const *>(nativePtr());
    }
    
    template <class T>
    inline T &MemoryManager::Ptr::as()
    {
        assert(sizeof(T) <= size());
        return *reinterpret_cast<T*>(nativePtr());
    }
    
}

#endif /* MemoryManager_hpp */
