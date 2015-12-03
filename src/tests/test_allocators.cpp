//
//  test_allocators.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

template <class Tp> struct Pointer;

struct Heapish
{
    uint8_t *ptr;
    size_t size;
    size_t position;
    
    Heapish()
    : ptr(nullptr), size(0), position(0)
    {
    }
    
    void initialize(size_t _size)
    {
        std::cout << "Initializing heapish...\n";
        position = 4;
        size = _size;
        ptr = new uint8_t [ size ];
    }
    
    void loadFromFile(std::string const &filename)
    {
        std::ifstream s(filename);
        s.seekg(0, s.end);
        size = s.tellg();
        s.seekg(0, s.beg);
        
        ptr = new uint8_t [ size ];
        position = size;
        
        s.read(reinterpret_cast<char *>(ptr), size);
        
        std::cout << "Read " << s.gcount() << " bytes of heap from file.\n";
    }
    
    ~Heapish()
    {
        delete [] ptr;
    }
    
    template <class Tp>
    Pointer<Tp> allocate(size_t n);
    
    template <class Tp, class... Args>
    Pointer<Tp> construct(Args & ... args);

    template <class Tp>
    Pointer<Tp> get(size_t index)
    {
        return Pointer<Tp>(index);
    }
    
    void saveToFile(std::string const &filename)
    {
        std::ofstream(filename).write(reinterpret_cast<char const *>(ptr), size);
    }
    
    static Heapish &instance()
    {
        static Heapish s_instance;
        return s_instance;
    }
    
};

struct PointerBase
{
    
};

template <class Tp>
struct Pointer
{
//    typedef Tp element_type;
    typedef Tp element_type;
    typedef std::ptrdiff_t difference_type;
//    typedef Tp &reference;
//    typedef Tp const &const_reference;
    
    size_t d_index;
    
    Pointer()
    : d_index(0)
    {
        std::cout << "A Pointer (" << d_index << ").\n";
    }
    
    Pointer(std::nullptr_t)
    : d_index(0)
    {
        std::cout << "B Pointer (" << d_index << ").\n";
    }
    
    Pointer(size_t index)
    : d_index(index)
    {
        std::cout << "C Pointer (" << d_index << ").\n";
    }
    
    Pointer(Pointer<Tp> const &other)
    : d_index(other.d_index)
    {
        std::cout << "D Pointer (" << d_index << ").\n";
    }
    
    Pointer<Tp> &operator=(Pointer<Tp> const &other)
    {
        std::cout << "Copy\n";
        d_index = other.d_index;
        return *this;
    }
    
    operator bool() const
    {
        return d_index == 0;
    }
    
    operator size_t() const
    {
        return d_index;
    }
    
    element_type *operator->()
    {
        std::cout << d_index << ": operator-> (" << reinterpret_cast<void*>(Heapish::instance().ptr + d_index) << ").\n";
//        std::cout << d_index << ": operator-> (=" << (d_heapish?*reinterpret_cast<element_type*>(Heapish::instance().ptr + d_index):0) << ").\n";
        return (d_index != 0?reinterpret_cast<element_type*>(Heapish::instance().ptr + d_index):nullptr);
    }

    element_type const *operator->() const
    {
        std::cout << d_index << ": operator-> (" << reinterpret_cast<void*>(Heapish::instance().ptr + d_index) << ").\n";
//        std::cout << d_index << ": operator-> (=" << (d_heapish?*reinterpret_cast<element_type*>(Heapish::instance().ptr + d_index):0) << ").\n";
        return (d_index != 0?reinterpret_cast<element_type const*>(Heapish::instance().ptr + d_index):nullptr);
    }

    element_type &operator*()
    {
        std::cout << d_index << ": operator*\n";
//        std::cout << d_index << ": operator* (=" << *reinterpret_cast<element_type*>(Heapish::instance().ptr + d_index) << ").\n";
        return *reinterpret_cast<element_type*>(Heapish::instance().ptr + d_index);
    }
    
    element_type const &operator*() const
    {
        std::cout << d_index << ": operator*\n";
//        std::cout << d_index << ": operator* (=" << *reinterpret_cast<element_type*>(Heapish::instance().ptr + d_index) << ").\n";
        return *reinterpret_cast<element_type const*>(Heapish::instance().ptr + d_index);
    }
    
    element_type &operator[](size_t index)
    {
        std::cout << d_index << ": operator[" << index << "]\n";
        return *(reinterpret_cast<element_type*>(Heapish::instance().ptr + d_index) + index);
    }
    
    element_type const &operator[](size_t index) const
    {
        std::cout << d_index << ": operator[" << index << "]\n";
        return *(reinterpret_cast<element_type const*>(Heapish::instance().ptr + d_index) + index);
    }
    
    std::ptrdiff_t operator-(Pointer<Tp> const &other) const
    {
        return (d_index - other.d_index) / sizeof(Tp);
    }

    Pointer<Tp> operator-(int n) const
    {
        return Pointer(d_index - n*sizeof(Tp));
    }

    Pointer<Tp> operator+(size_t n) const
    {
        return Pointer(d_index + n*sizeof(Tp));
    }
    
    Pointer<Tp> &operator+=(size_t n)
    {
        d_index += n*sizeof(Tp);
        return *this;
    }

    Pointer<Tp> &operator-=(size_t n)
    {
        d_index -= n*sizeof(Tp);
        return *this;
    }
    
    Pointer<Tp> &operator--()
    {
        d_index -= sizeof(Tp);
        return *this;
    }

    Pointer<Tp> &operator--(int)
    {
        d_index -= sizeof(Tp);
        return *this;
    }

    Pointer<Tp> &operator++()
    {
        d_index += sizeof(Tp);
        return *this;
    }
    
    Pointer<Tp> &operator++(int)
    {
        d_index += sizeof(Tp);
        return *this;
    }
    
    bool operator==(std::nullptr_t) const
    {
        return d_index == 0;
    }
    
    bool operator==(Pointer<Tp> const &other) const
    {
        return d_index == other.d_index;
    }

    bool operator!=(Pointer<Tp> const &other) const
    {
        return d_index != other.d_index;
    }
    
    bool operator<(Pointer<Tp> const &other) const
    {
        std::cout << d_index << ": operator<\n";
        return d_index < other.d_index;
    }
    
    bool operator>(Pointer<Tp> const &other) const
    {
        std::cout << d_index << ": operator>\n";
        return d_index > other.d_index;
    }
    
};



template <>
struct Pointer<void>
{
    //    typedef Tp element_type;
    typedef void element_type;
    typedef std::ptrdiff_t difference_type;
    //    typedef Tp &reference;
    //    typedef Tp const &const_reference;
    
    size_t d_index;
    
    Pointer()
    : d_index(0)
    {
        std::cout << "A Pointer (" << d_index << ").\n";
    }
    
    Pointer(std::nullptr_t)
    : d_index(0)
    {
        std::cout << "B Pointer (" << d_index << ").\n";
    }
    
    Pointer(size_t index)
    : d_index(index)
    {
        std::cout << "C Pointer (" << d_index << ").\n";
    }
    
    template <class Tp>
    Pointer(Pointer<Tp> const &other)
    : d_index(other.d_index)
    {
        std::cout << "D Pointer (" << d_index << ").\n";
    }

    template <class Tp>
    Pointer<void> &operator=(Pointer<Tp> const &other)
    {
        std::cout << "Copy\n";
        d_index = other.d_index;
        return *this;
    }
    
    operator bool() const
    {
        return d_index == 0;
    }
    
    operator size_t() const
    {
        return d_index;
    }
    
    element_type *operator->()
    {
        std::cout << d_index << ": operator-> (" << reinterpret_cast<void*>(Heapish::instance().ptr + d_index) << ").\n";
        //        std::cout << d_index << ": operator-> (=" << (d_heapish?*reinterpret_cast<element_type*>(Heapish::instance().ptr + d_index):0) << ").\n";
        return (d_index != 0?reinterpret_cast<element_type*>(Heapish::instance().ptr + d_index):nullptr);
    }
    
    element_type const *operator->() const
    {
        std::cout << d_index << ": operator-> (" << reinterpret_cast<void*>(Heapish::instance().ptr + d_index) << ").\n";
        //        std::cout << d_index << ": operator-> (=" << (d_heapish?*reinterpret_cast<element_type*>(Heapish::instance().ptr + d_index):0) << ").\n";
        return (d_index != 0?reinterpret_cast<element_type const*>(Heapish::instance().ptr + d_index):nullptr);
    }
    
    template <class Tp>
    std::ptrdiff_t operator-(Pointer<Tp> const &other) const
    {
        return (d_index - other.d_index) / sizeof(Tp);
    }
    
    bool operator==(std::nullptr_t) const
    {
        return d_index == 0;
    }

    template <class Tp>
    bool operator==(Pointer<Tp> const &other) const
    {
        return d_index == other.d_index;
    }
    
    template <class Tp>
    bool operator!=(Pointer<Tp> const &other) const
    {
        return d_index != other.d_index;
    }
    
    template <class Tp>
    bool operator<(Pointer<Tp> const &other) const
    {
        std::cout << d_index << ": operator<\n";
        return d_index < other.d_index;
    }
    
    template <class Tp>
    bool operator>(Pointer<Tp> const &other) const
    {
        std::cout << d_index << ": operator>\n";
        return d_index > other.d_index;
    }
    
};



/*
template <> struct Pointer<void>
{
    typedef int element_type;
//    struct element_type {};
    size_t d_index;
//    typedef void element_type;
//    typedef size_t difference_type;
};
 */

//template <class Tp> struct Pointer

template <class Tp>
Pointer<Tp> Heapish::allocate(size_t n)
{
    std::cout << "Heap: " << this << ".\n";
    size_t p = position;
    position += sizeof(Tp) * n;
    std::cout << "Allocating... (p=" << p << ", position=" << position << ").\n";
    Pointer<Tp> result(p);
    return result;
}

template <class Tp, class... Args>
Pointer<Tp> Heapish::construct(Args & ... args)
{
    std::cout << "Heap: " << this << ".\n";
    size_t p = position;
    position += sizeof(Tp);
    std::cout << "Constructing... (p=" << p << ", position=" << position << ", size=" << sizeof(Tp) << ").\n";
    auto tmp = new (ptr + p) Tp (args...);
    std::cout << "- position=" << position << " (" << tmp << ").\n";
    Pointer<Tp> result(p);
    return result;
}


//template <class Tp>
//template <>
template <typename Tp>
struct std::iterator_traits<Pointer<Tp>>
{
    typedef std::ptrdiff_t difference_type;
    typedef typename Pointer<Tp>::element_type value_type;
    typedef typename Pointer<Tp>::element_type *pointer;
    typedef typename Pointer<Tp>::element_type &reference;
    typedef std::random_access_iterator_tag iterator_category;
};

template <typename Tp>
struct std::iterator_traits<Pointer<Tp> const>
{
    typedef std::ptrdiff_t difference_type;
    typedef typename Pointer<Tp>::element_type value_type;
    typedef typename Pointer<Tp>::element_type const *pointer;
    typedef typename Pointer<Tp>::element_type const &reference;
    typedef std::random_access_iterator_tag iterator_category;
};

template <typename Tp>
struct std::pointer_traits<Pointer<Tp>>
{
    typedef Pointer<Tp> pointer;
    typedef typename Pointer<Tp>::element_type element_type;
    typedef std::ptrdiff_t difference_type;
    
    template<class U>
    using rebind = Pointer<U>;
    
    static pointer pointer_to(element_type &obj)
    {
        return Pointer<Tp>(reinterpret_cast<uint8_t*>(&obj) - Heapish::instance().ptr);
    }
};

template <>
struct std::pointer_traits<Pointer<void>>
{
    typedef Pointer<void> pointer;
    typedef typename Pointer<void>::element_type element_type;
    typedef std::ptrdiff_t difference_type;
    
    template<class U>
    using rebind = Pointer<U>;
};

template <typename Tp>
struct std::pointer_traits<Pointer<Tp> const>
{
    typedef Pointer<Tp> const pointer;
    typedef typename Pointer<Tp>::element_type element_type;
    typedef std::ptrdiff_t difference_type;
    
    template<class U>
    using rebind = Pointer<U>;
    
    static pointer pointer_to(element_type &obj)
    {
        return Pointer<Tp>(reinterpret_cast<uint8_t*>(&obj) - Heapish::instance().ptr);
    }
};


template <class Tp>
struct TestAllocator
{
    typedef Tp value_type;
    
//    typedef Tp *pointer;
    
//    typedef Tp const *const_pointer;
    
    typedef Pointer<Tp> pointer;
    typedef Pointer<Tp> const const_pointer;

    typedef Pointer<Tp> reference;
    typedef Pointer<Tp> const const_reference;

//    typedef size_t void_pointer;
//    typedef size_t const const_void_pointer;
    
    typedef std::ptrdiff_t difference_type;
    
    typedef size_t size_type;
    
    TestAllocator()
    {
        
    }
    
    template <class T> TestAllocator(TestAllocator<T> const &other)
    {
        
    }
    
    pointer allocate(std::size_t n)
    {
        //        std::cout << "Allocating " << n*sizeof(Tp) << " starting from " << d_heapish.position << ".\n";;
        //return new Tp [ n ];
        //return reinterpret_cast<pointer>(d_heapish.allocate(n));
        return Heapish::instance().allocate<value_type>(n);
    }
    
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
    
    void deallocate(pointer p, std::size_t n)
    {
        std::cout << "Deallocating " << n << ".\n";
        //        delete [] p;
    }
    
    pointer address( reference x ) const
    {
        return x;
    }
    
    /*
    const_pointer address( const_reference x ) const
    {
        return x;
    }*/
};

typedef std::vector<int, TestAllocator<int>> Vector;
typedef std::list<int, TestAllocator<int>> List;

int main(int argc, char *argv[])
{
    if (argc == 1) {
        Heapish &h = Heapish::instance();
        h.initialize(1024);
        
        auto pv = h.construct<Vector>();
        auto pl = h.construct<List>();
        
        pv->push_back(10);
        pv->push_back(11);
        pv->push_back(12);

        std::cout << (*pv)[0] << "!\n";
        std::cout << (*pv)[1] << "!\n";
        
        pl->push_back(1);
        pl->push_back(2);
        pl->push_back(3);

        std::cout << pl->size() << ".\n";
        std::cout << pl->back() << ".\n";
        
        h.saveToFile("heapish.dump");
    } else {
        Heapish &h = Heapish::instance();
        h.loadFromFile("heapish.dump");
        
        auto pv = h.get<Vector>(4);
        auto pl = h.get<List>(4 + sizeof(Vector));
        
        std::cout << "Count=" << pv->size() << ".\n";
        
        std::cout << (*pv)[0] << "!\n";
        std::cout << (*pv)[1] << "!\n";
        
        std::cout << pl->size() << ".\n";
        std::cout << pl->back() << ".\n";
    }

#if 0
    Heapish h(1024);
    
    auto pv = h.construct<Vector>(h);
    
//    Vector v(h);
    
//    v.reserve(10);
    
    pv->push_back(10);
    pv->push_back(11);
//    v.push_back(12);
  
//    std::cout << (*pv)[0] << "!\n";
//    std::cout << (*pv)[1] << "!\n";
    
//    for (auto i = v.begin(); i != v.end(); ++i) {
//        std::cout << *i.base() << "\n";
//    }
    
    /*
    for (auto &i : v) {
        std::cout << i << "\n";
    }*/
#endif
    
    return 0;
}
