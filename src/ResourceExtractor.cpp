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
#include "sci/Sci32View.hpp"
#include "sci/Sci32ViewCel.hpp"
#include "sci/Sci32ViewLoader.hpp"
#include "sci/Sci32Script.hpp"
#include "sci/Sci32ScriptLoader.hpp"
#include "sci/Sci32ScriptWriter.hpp"
#include "sci/Sci32SelectorVocab.hpp"
#include "sci/Sci32SelectorVocabLoader.hpp"
#include "sci/Sci32SelectorVocabWriter.hpp"
#include "graphics/Palette.hpp"
#include "io/Reader.hpp"
#include "graphics/ImageSaver.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <memory>

const bool g_savePictures = true;
const bool g_saveViews = true;
const bool g_saveScripts = true;
const bool g_saveVocabs = true;

void loadVocab(std::shared_ptr<scissum::Reader> const &reader, size_t length, size_t id)
{
    scissum::Sci32SelectorVocabLoader loader;
    scissum::Sci32SelectorVocabWriter writer;
    
    auto vocab = loader.load(reader, length);
    
    std::stringstream s; s << "vocab_" << id << ".lst";
    
    std::ofstream stream(s.str());
    
    writer.write(stream, vocab);
}

void loadScript(std::shared_ptr<scissum::Reader> const &reader, size_t length, size_t id)
{
    scissum::Sci32ScriptLoader loader;
    scissum::Sci32ScriptWriter writer;
    
    auto script = loader.load(reader, length);
    
    std::stringstream s; s << "script_" << id << ".asm";
    
    std::ofstream stream(s.str());
    
    writer.writeAssembly(stream, script);
}

void loadView(std::shared_ptr<scissum::Reader> const &reader, size_t id)
{
    scissum::Palette palette;
    scissum::Sci32ViewLoader loader;
    
    palette.setSafetyPalette();
    
    auto view = loader.load(reader, palette);
    
    auto &saver = scissum::ImageSaver::getPnmSaver();
    
    size_t groupIndex = 0;
    for (auto &group : view->groups()) {
        size_t celIndex = 0;
        for (auto &cel : group) {
            std::cout << "Saving group " << groupIndex << " cel " << celIndex
            << " (" << cel->width() << "x" << cel->height()
            << " +" << cel->horzDisplacement() << "x" << cel->vertDisplacement() << ").\n";
            
            std::stringstream s; s << "view_" << id << "_" << groupIndex << "_" << celIndex << ".pnm";
            saver.saveImage(cel->image(), s.str());
            
            ++celIndex;
        }
        ++groupIndex;
    }
}

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
    } else if (type == 0 && g_saveViews) {
        loadView(resourceReader, id);
    } else if (type == 2 && g_saveScripts) {
        loadScript(resourceReader, scissum::ResourceManager::instance().getResourceSize(type, id), id);
    } else if (type == 6 && g_saveVocabs) {
        loadVocab(resourceReader, scissum::ResourceManager::instance().getResourceSize(type, id), id);
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

