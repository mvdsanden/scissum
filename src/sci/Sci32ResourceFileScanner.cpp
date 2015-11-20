//
//  Sci32ResourceFileScanner.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/19/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32ResourceFileScanner.hpp"

using namespace scissum;


Sci32ResourceFileScanner::Sci32ResourceFileScanner(std::istream &stream)
: d_stream(stream), d_resourceOffset(0)
{
    memset(&d_entryHeader, 0, sizeof(d_entryHeader));
}

Sci32ResourceFileScanner::~Sci32ResourceFileScanner()
{
    
}

bool Sci32ResourceFileScanner::next()
{
    if (!d_stream) {
        return false;
    }
    
    memset(&d_entryHeader, 0, sizeof(d_entryHeader));
    
    d_stream.read(reinterpret_cast<char *>(&d_entryHeader), sizeof(d_entryHeader));
    
    d_resourceOffset = d_stream.tellg();
    
    d_stream.seekg(d_entryHeader.resourceCompressedSize, std::ios::cur);
    
    return !!d_stream;
}

size_t Sci32ResourceFileScanner::resourceOffset() const
{
    return d_resourceOffset;
}

uint8_t Sci32ResourceFileScanner::resourceType() const
{
    return d_entryHeader.resourceType;
}

uint16_t Sci32ResourceFileScanner::resourceId() const
{
    return d_entryHeader.resourceId;
}

uint32_t Sci32ResourceFileScanner::resourceCompressedSize() const
{
    return d_entryHeader.resourceCompressedSize;
}

uint32_t Sci32ResourceFileScanner::resourceUncompressedSize() const
{
    return d_entryHeader.resourceUncompressedSize;
}

uint16_t Sci32ResourceFileScanner::resourceCompressionMethod() const
{
    return d_entryHeader.resourceCompressionMethod;
}
