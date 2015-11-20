//
//  ResourceExtractor.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/19/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "sci/Sci32ResourceFileScanner.hpp"
#include "tools/LzsDecompressor.hpp"
#include "tools/DecompressorSource.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>

void extract(std::istream &stream, size_t type, size_t id)
{
    scissum::Sci32ResourceFileScanner scanner(stream);
    
    while (scanner.next()) {
        
        if (scanner.resourceType() == type && scanner.resourceId() == id) {

            stream.seekg(scanner.resourceOffset(), std::ios::beg);
            
            uint8_t *compressedData = new uint8_t [ scanner.resourceCompressedSize() ];
            
            stream.read(reinterpret_cast<char *>(compressedData), scanner.resourceCompressedSize());
            
            std::unique_ptr<scissum::DecompressorSource> source(new scissum::DecompressorMemorySource(compressedData,
                                                                                                      scanner.resourceCompressedSize()));
            scissum::LzsDecompressor decompressor(std::move(source));
            
            uint8_t readBuffer[2048];
            
            while (true) {
                size_t len = decompressor.read(readBuffer, 2048);
                if (len == 0) break;
                std::cout.write(reinterpret_cast<char*>(readBuffer), len);
            }
            
            break;
        }

    }
}

int main(int argc, char *argv[])
{
    
    if (argc < 4) {
        std::cerr << argv[0] << " <filename> <resourceType> <resourceId>\n";
    } else {
        std::ifstream stream(argv[1]);
        extract(stream, std::atoi(argv[2]), std::atoi(argv[3]));
    }
    
    return 0;
}

