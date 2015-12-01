//
//  Instruction.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/24/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Instruction_hpp
#define Instruction_hpp

#include <cstdint>

#include "instructions.inc"

namespace scissum {
    
    class Instruction
    {
    public:
        
        Instruction(size_t sourceAddress)
        : d_sourceAddress(sourceAddress)
        {
        }
        
        size_t sourceAddress() const
        {
            return d_sourceAddress;
        }
        
        template <class T>
        static size_t parseArgument(T &result, char const *source)
        {
            
        }
        
    private:
        
        size_t d_sourceAddress;
        
    };

    template <uint8_t _opcode, size_t _size, char *_name, size_t alias, size_t _argCount, class _arg0, class _arg1, class _arg2>
    class InstructionTemplate
    : public Instruction
    {
    public:
        
        InstructionTemplate(size_t sourceAddress,
                            char const *source)
        {
            
        }
        
        uint8_t opcode() const { return _opcode; }
        
        size_t size() const { return _size; }
        
        char const *name() const { return _name; }
        
        size_t argCount() const { return _argCount; }
        
    private:
        
        _arg0 d_arg0;
        _arg1 d_arg1;
        _arg2 d_arg2;
        
    };
    
#define SCISSUM_INSTRUCTION_CLASS(opcode, size, name, alias, argCount, arg0, arg1, arg2, desc)\
    typedef InstructionTemplate<opcode, size, #name, alias, argCount, arg0, arg1, arg2> Inst##name;
    
}

#endif /* Instruction_hpp */
