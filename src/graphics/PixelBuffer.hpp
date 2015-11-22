//
//  PixelBuffer.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef PixelBuffer_hpp
#define PixelBuffer_hpp

#include "Color.hpp"

namespace scissum {
    
    /**
     *  This is an interface so it can later be extended with an OpenGL (or other)
     *  Pixel Buffer.
     */
    class PixelBuffer
    {
    public:
        
        typedef Color *MappedType;
        typedef Color const *ConstMappedType;
        
        virtual void setSize(size_t pixelCount) = 0;
        
        virtual size_t size() const = 0;
        
        virtual Color *mapReadWrite() = 0;
        
        virtual Color const *mapReadOnly() const = 0;
        
        virtual void unmap() const = 0;
        
    };
    
    template <class T>
    class ScopedMapReadWrite
    {
        T &d_mapable;
        
        typename T::MappedType d_ptr;
        
    public:
        
        ScopedMapReadWrite(T &mapable)
        : d_mapable(mapable)
        {
            d_ptr = d_mapable.mapReadWrite();
        }
        
        ~ScopedMapReadWrite()
        {
            d_ptr = nullptr;
            d_mapable.unmap();
        }
        
        typename T::MappedType ptr()
        {
            return d_ptr;
        }
        
    };

    template <class T>
    class ScopedMapReadOnly
    {
        T const &d_mapable;
        
        typename T::ConstMappedType d_ptr;
        
    public:
        
        ScopedMapReadOnly(T const &mapable)
        : d_mapable(mapable)
        {
            d_ptr = d_mapable.mapReadOnly();
        }
        
        ~ScopedMapReadOnly()
        {
            d_ptr = nullptr;
            d_mapable.unmap();
        }
        
        typename T::ConstMappedType ptr()
        {
            return d_ptr;
        }
        
    };

    
    class MemoryPixelBuffer
    : public PixelBuffer
    {
        
        MemoryPixelBuffer(MemoryPixelBuffer const &); // NI
        MemoryPixelBuffer &operator=(MemoryPixelBuffer const &); // NI
        
    public:
        
        MemoryPixelBuffer(size_t pixelCount);
        
        ~MemoryPixelBuffer();
        
        virtual void setSize(size_t pixelCount) ;
        
        virtual size_t size() const;
        
        virtual Color *mapReadWrite();
        
        virtual Color const *mapReadOnly() const;
        
        virtual void unmap() const;
        
    private:
        
        size_t d_size;
        
        Color *d_pixels;
        
    };
    
}

#endif /* PixelBuffer_hpp */
