//
//  Sci32SelectorVocab.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/14/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32SelectorVocab.hpp"

#include <cassert>

using namespace scissum;

Sci32SelectorVocab::Sci32SelectorVocab(size_t count)
: d_vocab(count)
{    
}

std::string &Sci32SelectorVocab::get(size_t index)
{
    assert(index < d_vocab.size());
    return d_vocab[index];
}

std::string const &Sci32SelectorVocab::get(size_t index) const
{
    assert(index < d_vocab.size());
    return d_vocab[index];
}

size_t Sci32SelectorVocab::size() const
{
    return d_vocab.size();
}
