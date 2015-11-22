//
//  Sci32ResourceFileScanner.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/19/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32ResourceFileScanner.hpp"

#include <stdexcept>

#include <cstring>

#include <cstddef>
#include <cerrno>

using namespace scissum;


Sci32ResourceFileScanner::Sci32ResourceFileScanner(int fileDescriptor)
: d_fd(fileDescriptor), d_resourceOffset(0)
{
    memset(&d_entryHeader, 0, sizeof(d_entryHeader));
}

Sci32ResourceFileScanner::~Sci32ResourceFileScanner()
{
    
}

bool Sci32ResourceFileScanner::next()
{
//    if (!d_stream) {
//        return false;
//    }
    
    d_resourceOffset += d_entryHeader.resourceCompressedSize;
    
    memset(&d_entryHeader, 0, sizeof(d_entryHeader));
    
    while (true) {
        int res = pread(d_fd, &d_entryHeader, sizeof(d_entryHeader), d_resourceOffset);
        if (res == -1) {
            if (res == EAGAIN || res == EINTR) {
                continue;
            }
            throw std::runtime_error("error reading from file");
        } else if (res == 0) {
            return false;
        }
        d_resourceOffset += res;
        break;
    }
    
//    d_stream.read(reinterpret_cast<char *>(&d_entryHeader), sizeof(d_entryHeader));
    
//    d_resourceOffset = d_stream.tellg();
    
//    d_stream.seekg(d_entryHeader.resourceCompressedSize, std::ios::cur);
    
    return true;//!!d_stream;
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
