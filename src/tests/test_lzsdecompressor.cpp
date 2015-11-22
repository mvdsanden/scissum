//
//  test_lzsdecompressor.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/19/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "tools/lzsdecompressor.hpp"
#include "tools/decompressorsource.hpp"

using namespace scissum;

void test0()
{    
    std::unique_ptr<DecompressorSource> source(new DecompressorMemorySource(0,0));
    LzsDecompressor d(std::move(source));
    
    uint8_t buffer[16];
    
    try {
        d.read(buffer, 16);
    } catch (...) {
        std::cerr << "test0: SUCCESS.\n";
        return;
    }

    std::cerr << "test0: FAILED.\n";
    exit(1);
}

void test1()
{
    uint8_t compressedData[3] = {0x00, 0xe0, 0x00};
    
    std::unique_ptr<DecompressorSource> source(new DecompressorMemorySource(compressedData, 3));
    LzsDecompressor d(std::move(source));
    
    uint8_t buffer[16];
    
    try {
        size_t len = d.read(buffer, 16);
        
        std::cout << "Read " << len << " bytes.\n";
        
        if (len != 1) {
            std::cerr << "test1: FAILED (output length incorrect).\n";
            exit(1);
        }
        
        if (buffer[0] != 1) {
            std::cerr << "test1: FAILED (output incorrect).\n";
            exit(1);
        }
        
    } catch (std::runtime_error &e) {
        std::cerr << "test1: FAILED (" << e.what() << ").\n";
        exit(1);
    }
    
    std::cerr << "test1: SUCESS.\n";
}

void test2()
{
    uint8_t compressedData[4] = {0x00, 0xE0, 0x6c, 0x00};
    
    std::unique_ptr<DecompressorSource> source(new DecompressorMemorySource(compressedData, 4));
    LzsDecompressor d(std::move(source));
    
    uint8_t buffer[16];
    
    try {
        size_t len = d.read(buffer, 16);
        
        std::cout << "Read " << len << " bytes.\n";
        
        if (len != 5) {
            std::cerr << "test2: FAILED (output length incorrect).\n";
            exit(1);
        }
        
        if (buffer[0] != 1 || buffer[1] != 1 || buffer[2] != 1 || buffer[3] != 1 || buffer[4] != 1) {
            std::cerr << "test2: FAILED (output incorrect).\n";
            exit(1);
        }
        
    } catch (std::runtime_error &e) {
        std::cerr << "test2: FAILED (" << e.what() << ").\n";
        exit(1);
    }
    
    std::cerr << "test2: SUCESS.\n";
}

void test3()
{
    uint8_t compressedData[4] = {0x00, 0xE0, 0x6c, 0x00};
    
    std::unique_ptr<DecompressorSource> source(new DecompressorMemorySource(compressedData, 4));
    LzsDecompressor d(std::move(source));
    
    uint8_t buffer[16];
    
    try {
        
        
        size_t len = d.skip(2);
        
        std::cout << "Skipped " << len << " bytes.\n";
        
        if (len != 2) {
            std::cerr << "test3: FAILED (skipped incorrect number of bytes)\n";
            exit(1);
        }
        
        len = d.read(buffer, 16);
        
        std::cout << "Read " << len << " bytes.\n";
        
        if (len != 3) {
            std::cerr << "test3: FAILED (output length incorrect).\n";
            exit(1);
        }
        
        if (buffer[0] != 1 || buffer[1] != 1 || buffer[2] != 1) {
            std::cerr << "test3: FAILED (output incorrect).\n";
            exit(1);
        }
        
    } catch (std::runtime_error &e) {
        std::cerr << "test3: FAILED (" << e.what() << ").\n";
        exit(1);
    }
    
    std::cerr << "test3: SUCESS.\n";
}

void testInputOutput(uint8_t const *compressedData, size_t compressedLength,
                     uint8_t const *testData, size_t testLength)
{
    std::unique_ptr<DecompressorSource> source(new DecompressorMemorySource(compressedData, compressedLength));
    LzsDecompressor d(std::move(source));
    
    uint8_t buffer[testLength];
    
    try {
        size_t length = 0;
        
        while (true) {
            size_t len = d.read(buffer, testLength);
            
            if (len == 0) {
                break;
            }
            
            length += len;
        }
        
        std::cout << "Read " << length << " bytes.\n";
        
        if (length != testLength) {
            std::cerr << "testInputOutput: FAILED (output length incorrect).\n";
            exit(1);
        }
        
        for (size_t i = 0; i < length; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<size_t>(buffer[i]) << std::dec << " ";
        }
        std::cout << "\n";
        
        if (memcmp(buffer, testData, length) != 0) {
            std::cerr << "testInputOutput: FAILED (output incorrect).\n";
            exit(1);
        }
        
    } catch (std::runtime_error &e) {
        std::cerr << "testInputOutput: FAILED (" << e.what() << ").\n";
        exit(1);
    }
    
    std::cerr << "testInputOutput: SUCESS.\n";
}


int main()
{
    test0();
    test1();
    test2();
    test3();
    
    uint8_t input0[] = {0x00, 0xe0, 0x6c, 0x00};
    uint8_t output0[] = {0x01, 0x01, 0x01, 0x01, 0x01};

    testInputOutput(input0, sizeof(input0), output0, sizeof(output0));
    
    uint8_t input1[] = {0x00, 0xe0, 0x7c, 0x30, 0x00};
    uint8_t output1[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    
    testInputOutput(input1, sizeof(input1), output1, sizeof(output1));

    uint8_t input2[] = {0x00, 0xe0, 0x7c, 0xf0, 0x00};
    uint8_t output2[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    
    testInputOutput(input2, sizeof(input2), output2, sizeof(output2));

    uint8_t input3[] = {0x00, 0xe0, 0x7f, 0xc7, 0x00};
    uint8_t output3[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    
    testInputOutput(input3, sizeof(input3), output3, sizeof(output3));

    uint8_t input4[] = {0x00, 0xc0, 0x07, 0xc3, 0x00};
    uint8_t output4[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};

    testInputOutput(input4, sizeof(input4), output4, sizeof(output4));

    // Example from http://masters.donntu.org/2003/fvti/boykov/library/lzs.pdf
    uint8_t input5[] = {0x20, 0x90, 0x88, 0x38, 0x1C, 0x21, 0xE2, 0x5C, 0x15, 0x80};
    uint8_t output5[] = {0x41, 0x42, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x43, 0x41, 0x42, 0x41, 0x42, 0x41, 0x42, 0x41};
    
    testInputOutput(input5, sizeof(input5), output5, sizeof(output5));

    return 0;
}
