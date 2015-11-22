//
//  DecompressorSource.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/19/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "DecompressorSource.hpp"

#include <cassert>

#include <iostream>
#include <iomanip>

using namespace scissum;

DecompressorSourceBasic::DecompressorSourceBasic()
: d_word(0), d_wordFill(0)
{
}

DecompressorSourceBasic::~DecompressorSourceBasic()
{
}

bool DecompressorSourceBasic::getBit()
{
    if (d_wordFill < 1) {
        if (!fillWord()) {
            throw std::runtime_error("end of source reached");
        }
    }
    
    bool result = d_word & (1ull << (sizeof(d_word) * 8 - 1));
    d_word <<= 1;
    --d_wordFill;
    
    return result;
}

uint8_t DecompressorSourceBasic::getByte()
{
    if (d_wordFill < 8) {
        if (!fillWord()) {
            throw std::runtime_error("end of source reached");
        }
    }
    
    const size_t s = (sizeof(d_word) * 8 - 8);
    
    size_t result = d_word >> s;
    d_word <<= 8;
    d_wordFill -= 8;
    
    return result;
}

size_t DecompressorSourceBasic::getVariableBits(size_t bits)
{
    assert(bits < sizeof(d_word) * 8 - 8 && "maximum variable bits extraction reached");
    
    if (d_wordFill < bits) {
        if (!fillWord() || d_wordFill < bits) {
            throw std::runtime_error("end of source reached");
        }
    }
    
    const size_t s = (sizeof(d_word) * 8 - bits);
    
    size_t result = d_word >> s;
    d_word <<= bits;
    d_wordFill -= bits;
    
    return result;
}


DecompressorMemorySource::DecompressorMemorySource(uint8_t const *buffer, size_t length)
: d_buffer(buffer), d_length(length)
{
    
}

DecompressorMemorySource::~DecompressorMemorySource()
{
    
}

bool DecompressorMemorySource::fillWord()
{
    if (d_length == 0) {
        return false;
    }
    
    const size_t wordSize = sizeof(d_word) * 8;
    
    while ((wordSize - d_wordFill > 8) && (d_length > 0)) {
        d_word |= static_cast<size_t>(*d_buffer) << (wordSize - d_wordFill - 8);
        d_wordFill += 8;
        ++d_buffer;
        --d_length;
    }
    
    //std::cout << "Word=0x" << std::hex << std::setw(16) << std::setfill('0') << d_word << std::dec << ".\n";
    return true;
}



DecompressorFileSource::DecompressorFileSource(int fileDescriptor,
                                               size_t offset,
                                               size_t length)
: d_fd(fileDescriptor), d_pipe(false), d_offset(offset), d_length(length)
{
    if (lseek(d_fd, 0, SEEK_CUR) == -1) {
        if (errno == ESPIPE ) {
            d_pipe = true;
            
            if (offset != 0) {
                throw std::runtime_error("file descriptor not seekable");
            }
        } else {
            throw std::runtime_error("invalid file descriptor");
        }
    }
}

DecompressorFileSource::~DecompressorFileSource()
{
}

bool DecompressorFileSource::fillWord()
{
    if (d_length == 0) {
        return false;
    }
    
    bool result = false;
    
    const size_t wordSize = sizeof(d_word) * 8;
    
    while ((wordSize - d_wordFill > 8) && (d_length > 0)) {
        
        uint8_t b = 0;
        
        while (true) {
            int res = 0;
            if (d_pipe) {
                res = read(d_fd, &b, 1);
            } else {
                res = pread(d_fd, &b, 1, d_offset);
            }
            if (res == -1) {
                if (errno == EAGAIN || errno == EINTR) {
                    continue;
                }
                return result;
            } else if (res == 0) {
                return result;
            }
            break;
        }
        
        d_word |= static_cast<size_t>(b) << (wordSize - d_wordFill - 8);
        d_wordFill += 8;
        
        ++d_offset;
        --d_length;
        result = true;
    }
    
    return result;
}
