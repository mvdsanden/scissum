//
//  createops.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/24/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>


int main()
{
    
    for (uint8_t i = 0x80; i >= 0x80; ++i) {
        
        std::string name;
        
        switch (i & 0x60) {
            case 0x00: // 00
                name += "L"; break;
            case 0x20: // 01
                name += "S"; break;
            case 0x40: // 10
                name += "I"; break;
            case 0x60: // 11
                name += "D"; break;
        };

        name += ((i & 0x10)?"I":"N");
        name += ((i & 0x08)?"S":"A");
        
        switch (i & 0x06) {
            case 0x00: // 00
                name += "G"; break;
            case 0x02: // 01
                name += "L"; break;
            case 0x04: // 10
                name += "T"; break;
            case 0x06: // 11
                name += "P";
        };
        
        size_t size = (i & 0x01?2:3);
        size_t argCount = 1;
        std::string arg0 = (i & 0x01?"uint8_t":"uint16_t");
        
        name += (i & 0x01?"B":"W");
        
        std::cout
        << "_(0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<size_t>(i)
        << ", " << std::dec << size
        << ", " << name
        << ", 0, 1, " << arg0
        << ", void, void, \"" << name << "\")\\\n";
        
    }
    
    return 0;
}
