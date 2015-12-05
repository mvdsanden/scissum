//
//  HeapPointer.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/3/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef HeapPointer_hpp
#define HeapPointer_hpp

#include "MemoryManager.hpp"

#include <utility>

#include <cstdint>
#include <cstddef>

#include <iostream>

namespace scissum {    
    
    template <class _T> class HeapPointer;
    
    class HeapPointerBase
    {
        
        template <class _T> friend class HeapAllocator;
        
    public:
        
        typedef MemoryManager::HeapIndex HeapIndex;
        
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;
        
    protected:
        
        HeapIndex d_index;
        
        HeapPointerBase(HeapIndex index)
        : d_index(index)
        {
            std::cout << this << ": HPB(" << index << ")\n";
        }
        
    public:
        
        HeapPointerBase()
        : d_index(0)
        {
            std::cout << this << ": HPB()\n";
        }
        
        HeapPointerBase(std::nullptr_t)
        : d_index(0)
        {
            std::cout << this << ": HPB(nullptr)\n";
        }
        
        HeapPointerBase(HeapPointerBase const &other)
        : d_index(other.d_index)
        {
            std::cout << this << ": HPB-COPY()\n";
        }

        HeapPointerBase(HeapPointerBase &&other)
        : d_index(other.d_index)
        {
            std::cout << this << ": HPB-&&COPY()\n";
        }
        
        HeapPointerBase &operator=(HeapPointerBase const &other)
        {
            std::cout << this << ": HPB =\n";
            d_index = other.d_index;
            return *this;
        }

        HeapPointerBase &operator=(HeapPointerBase &&other)
        {
            std::cout << this << ": HPB &&=\n";
            
            d_index = other.d_index;
            return *this;
        }

        bool operator==(std::nullptr_t) const
        {
            return d_index == 0;
        }
        
        bool operator==(HeapPointerBase const &other) const
        {
            return d_index == other.d_index;
        }

        bool operator!=(HeapPointerBase const &other) const
        {
            return d_index != other.d_index;
        }
        
        bool operator<(HeapPointerBase const &other) const
        {
            return d_index < other.d_index;
        }
        
        bool operator>(HeapPointerBase const &other) const
        {
            return d_index > other.d_index;
        }

        bool operator<=(HeapPointerBase const &other) const
        {
            return d_index <= other.d_index;
        }
        
        bool operator>=(HeapPointerBase const &other) const
        {
            return d_index >= other.d_index;
        }

        /*
        template <class T>
        HeapPointerBase(HeapPointer<T> const &other);

        template <class T>
        HeapPointerBase(HeapPointer<T> &&other);
        
        template <class T>
        HeapPointerBase &operator=(HeapPointer<T> const &other);

        template <class T>
        HeapPointerBase &operator=(HeapPointer<T> &&other);
         */
        
        operator bool() const
        {
            return d_index != 0;
        }
        
        void *operator->();
        
        void const *operator->() const;
        
        size_type size() const;
        
        template <class T>
        void incrementIndex(size_t n)
        {
            d_index += n * sizeof(T);
        }

        template <class T>
        void decrementIndex(size_t n)
        {
            d_index -= n * sizeof(T);
        }
        
        template <class T>
        difference_type difference(HeapPointer<T> const &other) const
        {
            return (d_index - other.d_index) / sizeof(T);
        }
        
    };
    
    template <class _T>
    class HeapPointer
    : public HeapPointerBase
    {
        template <class U> friend class HeapAllocator;
        
        HeapPointer(HeapIndex index)
        : HeapPointerBase(index)
        {
        }
        
    public:
        
        typedef _T value_type;
        typedef _T *pointer;
        typedef _T const *const_pointer;
        typedef _T &reference;
        typedef _T const &const_reference;
        
        HeapPointer()
        : HeapPointerBase()
        {
            std::cout << this << ": HP()\n";
        }

        HeapPointer(std::nullptr_t)
        : HeapPointerBase()
        {
            std::cout << this << ": HP(nullptr)\n";
        }
        
        HeapPointer(HeapPointerBase const &other)
        : HeapPointerBase(other)
        {
            std::cout << this << ": HP-COPY()\n";
        }

        HeapPointer(HeapPointerBase &&other)
        : HeapPointerBase(other)
        {
            std::cout << this << ": HP-&&COPY()\n";
        }

        pointer operator->()
        {
            return reinterpret_cast<pointer>(HeapPointerBase::operator->());
        }

        const_pointer operator->() const
        {
            return reinterpret_cast<const_pointer>(HeapPointerBase::operator->());
        }
        
        reference operator*()
        {
            return *operator->();
        }

        const_reference operator*() const
        {
            return *operator->();
        }
        
        HeapPointer<value_type> &operator++()
        {
            incrementIndex<value_type>(1);
            return *this;
        }

        HeapPointer<value_type> &operator++(int)
        {
            incrementIndex<value_type>(1);
            return *this;
        }

        HeapPointer<value_type> &operator+=(size_t n)
        {
            incrementIndex(n);
            return *this;
        }

        HeapPointer<value_type> &operator--()
        {
            decrementIndex<value_type>(1);
            return *this;
        }
        
        HeapPointer<value_type> &operator--(int)
        {
            decrementIndex<value_type>(1);
            return *this;
        }
        
        HeapPointer<value_type> &operator-=(size_t n)
        {
            decrementIndex<value_type>(n);
            return *this;
        }
        
        difference_type operator-(HeapPointer<value_type> const other) const
        {
            return HeapPointerBase::difference<value_type>(other);
        }
        
        HeapPointer<value_type> operator-(int n) const
        {
            return HeapPointer<value_type>(d_index - n*sizeof(value_type));
        }

        HeapPointer<value_type> operator+(size_t n) const
        {
            return HeapPointer<value_type>(d_index + n*sizeof(value_type));
        }
        
        reference operator[](size_t index)
        {
            return *(operator->() + index);
        }

        const_reference operator[](size_t index) const
        {
            return *(operator->() + index);
        }
        
    };
    
}

#endif /* HeapPointer_hpp */
