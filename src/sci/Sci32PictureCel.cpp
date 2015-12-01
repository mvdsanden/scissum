//
//  Sci32PictureCel.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32PictureCel.hpp"

using namespace scissum;

Sci32PictureCel::Sci32PictureCel(size_t width, size_t height,
                size_t horzDisplacement, size_t vertDisplacement,
                size_t priority,
                size_t relativeXPos,
                size_t relativeYPos)
:
Sci32Cel(width, height, horzDisplacement, vertDisplacement),
d_priority(priority),
d_relativeXPos(relativeXPos),
d_relativeYPos(relativeYPos)
{
    
}

Sci32PictureCel::~Sci32PictureCel()
{
    
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
