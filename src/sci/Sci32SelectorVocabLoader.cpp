//
//  Sci32SelectorVocabLoader.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/14/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32SelectorVocabLoader.hpp"
#include "Sci32SelectorVocab.hpp"

#include "io/Reader.hpp"

#include <stdexcept>

#include <iostream>

using namespace scissum;

std::shared_ptr<Sci32SelectorVocab> Sci32SelectorVocabLoader::load(std::shared_ptr<Reader> const &reader, size_t length)
{
    std::unique_ptr<uint8_t[]> data(new uint8_t [ length ]);
    
    size_t len = reader->read(data.get(), length);
    
    if (len != length) {
        throw std::runtime_error("vocab resource seems corrupt, could not read data");
    }
    
    uint16_t count = *reinterpret_cast<uint16_t const*>(data.get());
    
    std::cout << "Vocab count: " << count << ".\n";
    
    uint16_t const *offsetTable = reinterpret_cast<uint16_t const *>(data.get() + sizeof(count));
    
    std::shared_ptr<Sci32SelectorVocab> result = std::make_shared<Sci32SelectorVocab>(count);
    
    for (size_t i = 0; i < count; ++i) {

        std::cout << "Vocab entry offset: " << offsetTable[i] << ".\n";
        
        uint16_t length = *reinterpret_cast<uint16_t*>(data.get() + offsetTable[i]);
        
        result->get(i) = std::string(reinterpret_cast<char const *>(data.get() + offsetTable[i] + 2), length);
    }
    
    return result;
}
