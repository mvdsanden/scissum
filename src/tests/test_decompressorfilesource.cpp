//
//  test_decompressorfilesource.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include <iostream>
#include <iomanip>

#include <fcntl.h>

#include "tools/DecompressorSource.hpp"

char const *filenameTest0 = "/tmp/scissum_tests_decompressorfilesource_test0.dat";

void generateTestSet()
{
    uint8_t buffer[18] = {0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
    int fd = open(filenameTest0, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    if (write(fd, buffer, sizeof(buffer)) == -1) {
        std::cerr << "Error creating testset.\n";
    }
    close(fd);
}

void testGetByte()
{
    uint8_t buffer[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    
    int fd = open(filenameTest0, O_RDONLY);
    
    scissum::DecompressorFileSource s(fd, 1, 16);
    
    for (size_t i = 0; i < 16; ++i) {
        uint8_t v = s.getByte();
        
        //        std::cout << static_cast<size_t>(v) << "\n";
        
        if (v != buffer[i]) {
            std::cerr << "testGetByte(): FAILED!\n";
            exit(1);
        }
    }
    
    close(fd);
    
    std::cerr << "testGetByte(): SUCCESS!\n";
}


void testGetBit()
{
    uint8_t buffer[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    
    uint64_t t[2] = {};
    
    uint8_t buffer2[16] = {16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
    t[0] = *reinterpret_cast<uint64_t*>(buffer2 + 8);
    t[1] = *reinterpret_cast<uint64_t*>(buffer2);
    
    //    std::cout << "t[0]=0x" << std::hex << std::setw(16) << std::setfill('0') << t[0] << std::dec << ".\n";
    //    std::cout << "t[1]=0x" << std::hex << std::setw(16) << std::setfill('0') << t[1] << std::dec << ".\n";

    int fds[2];
    pipe(fds);
    write(fds[1], buffer, 16);
    
    scissum::DecompressorFileSource s(fds[0], 0, 16);
    
    
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 64; ++j) {
            bool v = s.getBit();
            
            //            std::cout << (i*64+j) << ": " << static_cast<size_t>(v) << "\n";
            
            size_t mask = (0x8000000000000000 >> j);
            
            //            std::cout << "Mask=" << std::hex << std::setw(16) << std::setfill('0') << mask << std::dec << ".\n";
            
            if (v != !!(t[i] & mask)) {
                std::cerr << "testGetBit(): FAILED!\n";
                exit(1);
            }
        }
    }
    
    std::cerr << "testGetBit(): SUCCESS!\n";
}

void testGetVariableBits()
{
    uint8_t buffer[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    
    uint64_t t[2] = {};
    
    uint8_t buffer2[16] = {16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
    t[0] = *reinterpret_cast<uint64_t*>(buffer2 + 8);
    t[1] = *reinterpret_cast<uint64_t*>(buffer2);
    
    //    std::cout << "t[0]=0x" << std::hex << std::setw(16) << std::setfill('0') << t[0] << std::dec << ".\n";
    //    std::cout << "t[1]=0x" << std::hex << std::setw(16) << std::setfill('0') << t[1] << std::dec << ".\n";
    
    for (size_t bits = 1; bits < 32; ++bits) {
        //        std::cout << "BITS=" << bits << ".\n";
        
        
        int fd = open(filenameTest0, O_RDONLY);
        
        scissum::DecompressorFileSource s(fd, 1, 16);
        
        
        for (size_t j = 0; j < 64-bits; j += bits) {
            
            
            size_t v = s.getVariableBits(bits);
            
            //            std::cout << (i*64+j) << ": " << static_cast<size_t>(v) << "\n";
            
            size_t r = (t[0] << j) >> (64-bits);
            
            //            std::cout << "Mask=" << std::hex << std::setw(16) << std::setfill('0') << mask << std::dec << ".\n";
            
            if (v != r) {
                std::cerr << "testGetVariableBits(): FAILED!\n";
                close(fd);
                exit(1);
            }
        }
        
        close(fd);
    }
    
    std::cerr << "testGetVariableBit(): SUCCESS!\n";
}


int main()
{
    generateTestSet();
    
    testGetByte();
    testGetBit();
    testGetVariableBits();
    
    return 0;
}
