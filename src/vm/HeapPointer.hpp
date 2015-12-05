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

#include <iosfwd>

namespace scissum {    
    
    template <class _T> class HeapPointer;
    
    class HeapPointerBase
    {
        
        template <class _T> friend class HeapAllocator;
        
        friend std::ostream &operator<<(std::ostream &s, HeapPointerBase const &p);
        
    public:
        
        typedef MemoryManager::HeapIndex HeapIndex;
        
        typedef std::ptrdiff_t difference_type;
        typedef size_t size_type;
        
    protected:
        
        HeapIndex d_index;
        
    private:
        
    public:
        
        HeapPointerBase(HeapIndex index)
        : d_index(index)
        {
            //          std::cout << this << ": HPB(" << index << ")\n";
        }
        
        HeapPointerBase()
        : d_index(0)
        {
//            std::cout << this << ": HPB() -> " << d_index << "\n";
        }
        
        HeapPointerBase(std::nullptr_t)
        : d_index(0)
        {
//            std::cout << this << ": HPB(nullptr) -> " << d_index << "\n";
        }
        
        HeapPointerBase(HeapPointerBase const &other)
        : d_index(other.d_index)
        {
//            std::cout << this << ": HPB-COPY(" << &other << ") -> " << d_index << "\n";
        }

        HeapPointerBase(HeapPointerBase &&other)
        : d_index(other.d_index)
        {
//            std::cout << this << ": HPB-&&COPY(" << &other << ") -> " << d_index << "\n";
        }
        
        HeapPointerBase &operator=(HeapPointerBase const &other)
        {
            d_index = other.d_index;
//            std::cout << this << ": HPB = -> " << d_index << "\n";
            return *this;
        }

        HeapPointerBase &operator=(HeapPointerBase &&other)
        {
            d_index = other.d_index;
//            std::cout << this << ": HPB &&= -> " << d_index << "\n";
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
        
        void *operator->() const;
        
//        void const *operator->() const;
        
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
        
    public:

        /*
        template <class U>
        friend HeapPointer<U> reinterpret_heap_pointer_cast(HeapIndex index)
        {
            return HeapPointer<U>(index);
        }
         */
        
        typedef _T value_type;
        typedef _T *pointer;
        typedef _T const *const_pointer;
        typedef _T &reference;
        typedef _T const &const_reference;
        
        HeapPointer(HeapIndex index)
        : HeapPointerBase(index)
        {
        }
        
        HeapPointer()
        : HeapPointerBase()
        {
//            std::cout << this << ": HP() -> " << d_index << "\n";
        }

        HeapPointer(std::nullptr_t)
        : HeapPointerBase()
        {
//            std::cout << this << ": HP(nullptr) -> " << d_index << "\n";
        }
        
        HeapPointer(HeapPointerBase const &other)
        : HeapPointerBase(other)
        {
//            std::cout << this << ": HP-COPY(" << &other << ") -> " << d_index << "\n";
        }

        HeapPointer(HeapPointerBase &&other)
        : HeapPointerBase(other)
        {
//            std::cout << this << ": HP-&&COPY(" << &other << ") -> " << d_index << "\n";
        }

        pointer operator->() const
        {
            return reinterpret_cast<pointer>(HeapPointerBase::operator->());
        }

        /*
        const_pointer operator->() const
        {
            return reinterpret_cast<const_pointer>(HeapPointerBase::operator->());
        }
        */
        
        value_type &operator*() const
        {
            return *operator->();
        }

        /*
        value_type const &operator*() const
        {
            return *operator->();
        }
        */
        
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
            incrementIndex<value_type>(n);
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
    
    template <>
    class HeapPointer<void>
    : public HeapPointerBase
    {
    public:
        
        typedef void value_type;
        typedef void *pointer;
        typedef void const *const_pointer;
        
        HeapPointer(HeapIndex index)
        : HeapPointerBase(index)
        {
        }
        
        HeapPointer()
        : HeapPointerBase()
        {
            //            std::cout << this << ": HP() -> " << d_index << "\n";
        }
        
        HeapPointer(std::nullptr_t)
        : HeapPointerBase()
        {
            //            std::cout << this << ": HP(nullptr) -> " << d_index << "\n";
        }
        
        HeapPointer(HeapPointerBase const &other)
        : HeapPointerBase(other)
        {
            //            std::cout << this << ": HP-COPY(" << &other << ") -> " << d_index << "\n";
        }
        
        HeapPointer(HeapPointerBase &&other)
        : HeapPointerBase(other)
        {
            //            std::cout << this << ": HP-&&COPY(" << &other << ") -> " << d_index << "\n";
        }
        
    };
    
}

template <typename Tp>
struct std::iterator_traits<scissum::HeapPointer<Tp>>
{
    typedef typename scissum::HeapPointer<Tp>::difference_type difference_type;
    typedef typename scissum::HeapPointer<Tp>::value_type value_type;
    typedef typename scissum::HeapPointer<Tp>::pointer pointer;
    typedef typename scissum::HeapPointer<Tp>::const_pointer const_pointer;
    typedef typename scissum::HeapPointer<Tp>::reference reference;
    typedef typename scissum::HeapPointer<Tp>::const_reference const_reference;
    typedef std::random_access_iterator_tag iterator_category;
};

template <typename Tp>
struct std::iterator_traits<const scissum::HeapPointer<Tp>>
{
    typedef typename scissum::HeapPointer<Tp>::difference_type difference_type;
    typedef typename scissum::HeapPointer<Tp>::value_type const value_type;
    typedef typename scissum::HeapPointer<Tp>::const_pointer pointer;
    typedef typename scissum::HeapPointer<Tp>::const_reference reference;
    typedef std::random_access_iterator_tag iterator_category;
};



template <typename Tp>
struct std::pointer_traits<scissum::HeapPointer<Tp>>
{
    typedef scissum::HeapPointer<Tp> pointer;
    typedef typename scissum::HeapPointer<Tp>::value_type element_type;
    typedef std::ptrdiff_t difference_type;
    
    template<class U>
    using rebind = scissum::HeapPointer<U>;
    
    static pointer pointer_to(element_type &obj)
    {
        return scissum::MemoryManager::instance().pointerTo(&obj);
    }
};

template <>
struct std::pointer_traits<scissum::HeapPointer<void>>
{
    typedef scissum::HeapPointer<void> pointer;
    typedef typename scissum::HeapPointer<void>::value_type element_type;
    typedef std::ptrdiff_t difference_type;
    
    template<class U>
    using rebind = scissum::HeapPointer<U>;
};


#endif /* HeapPointer_hpp */
