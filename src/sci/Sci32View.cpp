//
//  Sci32View.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/24/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32View.hpp"

#include "Sci32ViewCel.hpp"

using namespace scissum;

Sci32View::Sci32View(size_t width, size_t height,
                     std::vector<Group> &&groups)
: d_width(width), d_height(height), d_groups(std::move(groups))
{
    
}

Sci32View::~Sci32View()
{
    
}

size_t Sci32View::width() const
{
    return d_width;
}

size_t Sci32View::height() const
{
    return d_height;
}

std::vector<Sci32View::Group> const &Sci32View::groups() const
{
    return d_groups;
}

std::vector<Sci32View::Group> &Sci32View::groups()
{
    return d_groups;
}
