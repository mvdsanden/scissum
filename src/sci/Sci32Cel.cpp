//
//  Sci32Cel.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/24/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32Cel.hpp"

#include "graphics/Image.hpp"

#include <cassert>

using namespace scissum;


Sci32Cel::Sci32Cel(size_t width,
                   size_t height,
                   ssize_t horzDisplacement,
                   ssize_t vertDisplacement)
:
d_width(width), d_height(height),
d_horzDisplacement(horzDisplacement), d_vertDisplacement(vertDisplacement)
{    
}

Sci32Cel::~Sci32Cel()
{    
}

void Sci32Cel::setImage(std::unique_ptr<Image> &&image)
{
    d_image = std::move(image);
}

size_t Sci32Cel::width() const
{
    return d_width;
}

size_t Sci32Cel::height() const
{
    return d_height;
}

ssize_t Sci32Cel::horzDisplacement() const
{
    return d_horzDisplacement;
}

ssize_t Sci32Cel::vertDisplacement() const
{
    return d_vertDisplacement;
}

Image const &Sci32Cel::image() const
{
    assert(d_image);
    return *d_image.get();
}

Image &Sci32Cel::image()
{
    assert(d_image);
    return *d_image.get();
}
