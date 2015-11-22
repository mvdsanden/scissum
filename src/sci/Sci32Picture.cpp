//
//  Sci32Picture.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32Picture.hpp"
#include "Sci32PictureCel.hpp"

#include <cassert>

using namespace scissum;

Sci32Picture::Sci32Picture(size_t width, size_t height)
: d_width(width), d_height(height)
{    
}

Sci32Picture::~Sci32Picture()
{
}

void Sci32Picture::appendCel(std::unique_ptr<Sci32PictureCel> &&cel)
{
    d_cels.push_back(std::move(cel));
}

size_t Sci32Picture::celCount() const
{
    return d_cels.size();
}

Sci32PictureCel const &Sci32Picture::getCel(size_t index) const
{
    assert(index < d_cels.size());
    return *d_cels[index].get();
}

Sci32PictureCel &Sci32Picture::getCel(size_t index)
{
    assert(index < d_cels.size());
    return *d_cels[index].get();
}

std::vector<std::unique_ptr<Sci32PictureCel>> const &Sci32Picture::cels() const
{
    return d_cels;
}

std::vector<std::unique_ptr<Sci32PictureCel>> &Sci32Picture::cels()
{
    return d_cels;
}
