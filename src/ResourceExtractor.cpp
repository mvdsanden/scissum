//
//  ResourceExtractor.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/19/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "sci/ResourceManager.hpp"
#include "sci/Sci32Picture.hpp"
#include "sci/Sci32PictureCel.hpp"
#include "sci/Sci32PictureLoader.hpp"
#include "io/Reader.hpp"
#include "graphics/ImageSaver.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>

const bool g_savePictures = true;

void loadPicture(std::shared_ptr<scissum::Reader> const &reader)
{
    scissum::Sci32PictureLoader loader;
    auto picture = loader.loadPicture(reader);

    auto &saver = scissum::ImageSaver::getPnmSaver();
    
    size_t index = 0;
    for (auto &i : picture->cels()) {
        std::cout << "Saving cel " << index
        << " (prio=" << i->priority()
        << " " << i->width() << "x" << i->height()
        << " +" << i->relativeXPos() << "x" << i->relativeYPos() << ").\n";
        
        std::stringstream s; s << "pixture_" << (index++) << ".pnm";
        saver.saveImage(i->image(), s.str());
    }
}

void copyToStream(std::shared_ptr<scissum::Reader> const &reader, std::ostream &stream)
{
    uint8_t readBuffer[2048];
    
    while (true) {
        size_t len = reader->read(readBuffer, 2048);
        if (len == 0) break;
        stream.write(reinterpret_cast<char*>(readBuffer), len);
    }
}

void extract(size_t type, size_t id)
{
    auto resourceReader = scissum::ResourceManager::instance().openResource(type, id);
    
    if (type == 1 && g_savePictures) {
        loadPicture(resourceReader);
    } else {
        copyToStream(resourceReader, std::cout);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 4) {
        std::cerr << argv[0] << " <filename> <resourceType> <resourceId>\n";
    } else {
        scissum::ResourceManager::instance().loadResourceFile(argv[1]);
        extract(std::atoi(argv[2]), std::atoi(argv[3]));
    }
    
    return 0;
}

