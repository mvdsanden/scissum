//
//  Sci32SelectorVocabWriter.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/14/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32SelectorVocabWriter.hpp"
#include "Sci32SelectorVocab.hpp"

#include <iostream>

using namespace scissum;

void Sci32SelectorVocabWriter::write(std::ostream &stream, std::shared_ptr<Sci32SelectorVocab> const &vocab)
{
    for (size_t i = 0; i < vocab->size(); ++i) {
        std::cout << i << "\t" << vocab->get(i) << ".\n";
    }
}
