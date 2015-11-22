//
//  Sci32PictureCel.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32PictureCel.hpp"
#include "graphics/Image.hpp"

using namespace scissum;

Sci32PictureCel::Sci32PictureCel(size_t width, size_t height,
                size_t horzDisplacement, size_t vertDisplacement,
                size_t priority,
                size_t relativeXPos,
                size_t relativeYPos)
:
d_width(width),
d_height(height),
d_horzDisplacement(horzDisplacement),
d_vertDisplacement(vertDisplacement),
d_priority(priority),
d_relativeXPos(relativeXPos),
d_relativeYPos(relativeYPos)
{
    
}

Sci32PictureCel::~Sci32PictureCel()
{
    
}

void Sci32PictureCel::setImage(std::unique_ptr<Image> &&image)
{
    d_image = std::move(image);
}

size_t Sci32PictureCel::width() const
{
    return d_width;
}

size_t Sci32PictureCel::height() const
{
    return d_height;
}

size_t Sci32PictureCel::horzDisplacement() const
{
    return d_horzDisplacement;
}

size_t Sci32PictureCel::vertDisplacement() const
{
    return d_vertDisplacement;
}

size_t Sci32PictureCel::priority() const
{
    return d_priority;
}

size_t Sci32PictureCel::relativeXPos() const
{
    return d_relativeXPos;
}

size_t Sci32PictureCel::relativeYPos() const
{
    return d_relativeYPos;
}

Image const &Sci32PictureCel::image() const
{
    return *d_image.get();
}

Image &Sci32PictureCel::image()
{
    return *d_image.get();
}
