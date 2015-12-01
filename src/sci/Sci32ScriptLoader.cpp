//
//  Sci32ScriptLoader.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/25/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32ScriptLoader.hpp"
#include "Sci32Script.hpp"

#include "io/Reader.hpp"

#include <stdexcept>

using namespace scissum;

std::shared_ptr<Sci32Script> Sci32ScriptLoader::load(std::shared_ptr<Reader> const &reader, size_t length)
{
    uint8_t *buffer = new uint8_t [ length ];
    
    try {
        if (reader->read(buffer, length) != length) {
            throw std::runtime_error("resource seems corrupt, could not load the full script");
        }
        
        // Script will own the pointer.
        return std::make_shared<Sci32Script>(buffer, length);
    } catch (...) {
        delete [] buffer;
        throw;
    }
}
