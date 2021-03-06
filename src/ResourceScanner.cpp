//
//  ResourceScanner.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/19/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "sci/Sci32ResourceFileScanner.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>

#include <fcntl.h>

void scan(int fd)
{
    scissum::Sci32ResourceFileScanner scanner(fd);
    
    while (scanner.next()) {
        std::cout
        << "0x" << std::hex << std::setw(8) << std::setfill('0') << scanner.resourceOffset() << std::dec
        << ": TYPE=" << static_cast<size_t>(scanner.resourceType())
        << ", ID=" << scanner.resourceId()
        << ", CSIZE=" << scanner.resourceCompressedSize()
        << ", DSIZE=" << scanner.resourceUncompressedSize()
        << ", CMETHOD=" << std::hex << std::setw(4) << std::setfill('0') << scanner.resourceCompressionMethod() << std::dec
        << ".\n";
    }
}

int main(int argc, char *argv[])
{
    int fd = -1;
    
    if (argc < 2) {
        fd = STDIN_FILENO;
    } else {
        fd = open(argv[1], O_RDONLY);
    }

    scan(fd);

    close(fd);
    
    return 0;
}
