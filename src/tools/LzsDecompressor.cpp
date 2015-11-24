//
//  LzsDecompressor.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/19/15.
//  Copyright © 2015 MVDS. All rights reserved.
//
//
//  This code is made using the following references:
//  - ANSI X3.241-1994: http://masters.donntu.org/2003/fvti/boykov/library/lzs.pdf
//  - RFC1967: PPP LZS-DCP Compression Protocol (LZS-DCP)(https://tools.ietf.org/html/rfc1967#section-2.5.7)
//

#include "LzsDecompressor.hpp"
#include "DecompressorSource.hpp"

#include <stdexcept>
#include <algorithm>

using namespace scissum;

LzsDecompressor::LzsDecompressor(std::unique_ptr<DecompressorSource> &&source)
:
d_source(std::move(source)),
d_historyPos(0),
d_historyFill(0),
d_historyMatchPos(0),
d_historyMatchLength(0),
d_endOfData(false),
d_offset(0)
{
    
}

LzsDecompressor::~LzsDecompressor()
{
    
}

size_t LzsDecompressor::read(uint8_t *buffer, size_t length)
{
    if (d_endOfData) {
        return 0;
    }
    
    size_t total = length;
    while (length != 0) {
        if (d_historyMatchLength != 0) {
            size_t count = copyFromHistory(buffer, length);
            buffer += count;
            length -= count;
        } else {
            if (!decompressByte(*buffer)) {
                d_offset += (total - length);
                return total - length;
            }
            
            ++buffer;
            --length;
        }
    }
    
    d_offset += total;
    
    return total;
}

size_t LzsDecompressor::skip(size_t length)
{
    if (d_endOfData) {
        return 0;
    }
    
    size_t total = length;
    while (length != 0) {
        if (d_historyMatchLength != 0) {
            size_t count = skipFromHistory(length);
            length -= count;
        } else {
            uint8_t tmp = 0;
            if (!decompressByte(tmp)) {
                d_offset += (total - length);
                return total - length;
            }
            
            --length;
        }
    }
    
    d_offset += total;
    
    return total;
}

size_t LzsDecompressor::readOffset() const
{
    return d_offset;
}

bool LzsDecompressor::decompressByte(uint8_t &dest)
{
    if (d_source->getBit()) {
        // Compressed string.
        size_t offset = parseOffset();
        
        if (offset == 0) {
            // We found an end marker.
            d_endOfData = true;
            return false;
        }
        
        size_t length = parseLength();
        
        if (offset > d_historyFill) {
            throw std::runtime_error("LzsDecompressor: input corrupt");
        }
        
        d_historyMatchPos = ((d_historyPos + 2048) - offset) % 2048;
        d_historyMatchLength = length;
        
        copyFromHistory(&dest, 1);
    } else {
        // Raw byte.
        dest = d_source->getByte();
        pushHistory(dest);
    }
    
    return true;
}

size_t LzsDecompressor::parseOffset()
{
    size_t result = 0;
    if (d_source->getBit()) {
        // 7-bit offset.
        result = d_source->getVariableBits(7);
    } else {
        // 11-bit offset.
        result = d_source->getVariableBits(11);
    }
    
    return result;
}

size_t LzsDecompressor::parseLength()
{
    switch (d_source->getVariableBits(2)) {
        case 0: return 2;
        case 1: return 3;
        case 2: return 4;
        default:
            switch (d_source->getVariableBits(2)) {
                case 0: return 5;
                case 1: return 6;
                case 2: return 7;
                default:
                {
                    size_t o = 8;
                    size_t v = 15;
                    while (v == 15) {
                        v = d_source->getVariableBits(4);
                        o += v;
                    }
                    return o;
                }
            }
            break;
    }
}

void LzsDecompressor::pushHistory(uint8_t value)
{
    d_history[d_historyPos++] = value;
    d_historyFill = std::max(d_historyFill, d_historyPos);
    d_historyPos %= 2048;
}

size_t LzsDecompressor::skipFromHistory(size_t length)
{
    // TODO: optimize!
    size_t count = 0;
    while (d_historyMatchLength > 0 && length > 0) {
        pushHistory(d_history[d_historyMatchPos++]);
        
        d_historyMatchPos %= d_historyFill;
        --d_historyMatchLength;
        --length;
        ++count;
    }
    
    return count;
}

size_t LzsDecompressor::copyFromHistory(uint8_t *buffer, size_t length)
{
    // TODO: optimize!
    size_t count = 0;
    while (d_historyMatchLength > 0 && length > 0) {
        *(buffer++) = d_history[d_historyMatchPos];
        pushHistory(d_history[d_historyMatchPos++]);
        
        d_historyMatchPos %= d_historyFill;
        --d_historyMatchLength;
        --length;
        ++count;
    }
    
    /*
    size_t count = std::min(length, std::min(d_historyMatchLength, d_historyFill-d_historyMatchPos));
    memcpy(buffer, d_history + d_historyMatchPos, count);
    d_historyMatchLength -= count;
    d_historyMatchPos = (d_historyMatchPos + count) % d_historyFill;
     */
    
    return count;
}
