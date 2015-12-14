//
//  Sci32ScriptWriter.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/25/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32ScriptWriter.hpp"
#include "Sci32Script.hpp"

#include "vm/instructions.inc"

#include <iostream>
#include <iomanip>


using namespace scissum;

void Sci32ScriptWriter::writeAssembly(std::ostream &stream, std::shared_ptr<Sci32Script> const &script)
{
    uint8_t const *base = script->data();
    uint8_t const *code = script->bytecodeArea();
    uint8_t const *codeEnd = code + script->bytecodeLength();
    
    while (code < codeEnd) {
        
        stream << "0x" << std::hex << std::setfill('0') << std::setw(2) << code-base << ":\t";
        
        uint8_t op = *code++;
        
        stream << "(0x" << static_cast<size_t>(op) << ") ";
        
        switch (op) {
          
#define SCISSUM_ASM_WRITER_CASES(OpCode, TotalBytes, InternalName, Alias, ArgCount, Arg0, Arg1, Arg2, Desc, ...)\
        case OpCode:\
                stream << #InternalName << "\t";\
                if (ArgCount > 0) {\
                    stream << "\t" << std::dec << std::setw(8) << std::setfill('0') << static_cast<int>(*reinterpret_cast<Arg0 const*>(code));\
                    code += sizeof(Arg0);\
                }\
                if (ArgCount > 1) {\
                    stream << "\t" << std::dec << std::setw(8) << std::setfill('0') << static_cast<int>(*reinterpret_cast<Arg1 const*>(code));\
                    code += sizeof(Arg1);\
                }\
                if (ArgCount > 2) {\
                    stream << "\t" << std::dec << std::setw(8) << std::setfill('0') << static_cast<int>(*reinterpret_cast<Arg2 const*>(code));\
                    code += sizeof(Arg2);\
                }\
                stream << "\n";\
                break;\
                
                SCISSUM_SCI_INSTRUCTIONS(SCISSUM_ASM_WRITER_CASES);
                
        };
        
        // Return OpCode.
        if (op == 0x48) {
            break;
        }
        
    }
    
}
