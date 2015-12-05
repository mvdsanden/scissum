//
//  HeapAllocator.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/3/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef HeapAllocator_hpp
#define HeapAllocator_hpp

#include "MemoryManager.hpp"
#include "HeapPointer.hpp"

namespace scissum {
    
    template <class _T>
    class HeapAllocator
    {
    public:
        
        typedef typename HeapPointer<_T>::value_type value_type;
        typedef HeapPointer<value_type> pointer;
        typedef HeapPointer<value_type> const const_pointer;
        typedef HeapPointer<value_type> reference;
        typedef HeapPointer<value_type> const const_reference;
        typedef HeapPointer<void> void_pointer;
        typedef HeapPointer<void> const const_void_pointer;
        typedef typename HeapPointer<value_type>::difference_type difference_type;
        typedef typename HeapPointer<value_type>::size_type size_type;
        
        pointer allocate(std::size_t n)
        {
            return MemoryManager::instance().allocate(n * sizeof(value_type));
        }
        
        // TODO: should U be pointer?
        template< class... Args >
        void construct( pointer p, Args&&... args )
        {
            new (p.operator->()) value_type (std::forward(args)...);
        }
        
        template< class U >
        void destroy( U* p )
        {
            p->~U();
        }
        
        void deallocate(pointer p, std::size_t n = 0)
        {
            MemoryManager::instance().free(p.d_index);
        }
        
        pointer address( reference x ) const
        {
            return x;
        }
        
    };

    template <>
    class HeapAllocator<void>
    {
    public:
        
        typedef void value_type;
        typedef HeapPointerBase pointer;
        typedef HeapPointerBase const const_pointer;
        typedef HeapPointerBase reference;
        typedef HeapPointerBase const const_reference;
        typedef HeapPointerBase void_pointer;
        typedef HeapPointerBase const const_void_pointer;
        typedef typename HeapPointerBase::difference_type difference_type;
        typedef typename HeapPointerBase::size_type size_type;
        
        pointer allocate(std::size_t n)
        {
            return MemoryManager::instance().allocate(n);
        }
        
        // TODO: should U be pointer?
        template< class U, class... Args >
        void construct( U* p, Args&&... args )
        {
            new (p) U (args...);
        }
        
        template< class U >
        void destroy( U* p )
        {
            p->~U();
        }
        
        void deallocate(pointer p, std::size_t n = 0)
        {
            MemoryManager::instance().free(p.d_index);
        }
        
        pointer address( reference x ) const
        {
            return x;
        }
        
    };

    
}

#endif /* HeapAllocator_hpp */
