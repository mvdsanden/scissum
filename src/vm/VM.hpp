//
//  VM.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/25/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef VM_hpp
#define VM_hpp

#include <functional>

#include <cstdint>

#include "HeapPointer.hpp"
#include "HeapAllocator.hpp"

#include "instructions.inc"

namespace scissum {
    
    class KernelFunction;
    
    class VM
    {
    public:
        
        typedef std::function<void (uint8_t const *pc, size_t const *st, size_t acc)> OpTraceHandler;
        
        // TODO: move VM state to separate struct and pass an address to that in a KernelFunction call.
        typedef std::function<void (size_t functionId, size_t argc, size_t *args)> KernelFunction;
        
        static VM &instance();
        
        virtual void reset() = 0;
        
        virtual void registerKernelFunction(uint16_t id, KernelFunction const &function) = 0;
        
        virtual size_t execute(uint8_t const *bytecode, size_t length, HeapPointerBase args = nullptr, size_t argc = 0) = 0;

        template <class ... Args>
        size_t execute(uint8_t const *bytecode, size_t length, Args ... args)
        {
            HeapAllocator<std::tuple<Args...>> allocator;
            
            HeapPointerBase a = allocator.allocate(1);
            allocator.construct(a, args...);
            
            size_t result = execute(bytecode, length, a, sizeof...(args));
            
            //allocator.destroy(a);
            allocator.deallocate(a);
            
            return result;
        }
        
        virtual size_t *stackPointer() const = 0;
        
        virtual void setOpTraceHook(OpTraceHandler const &handler) = 0;
        
        virtual char const *lookupOpName(uint8_t opCode) = 0;
        
        enum Instructions {
            
#define SCISSUM_VM_INSTRUCTION_ENUM(OpCode, TotalSize, Name, Alias, ArgCount, Arg0, Arg1, Arg2, Desc, ...)\
OP_##Name##_##Alias = OpCode,
            
            SCISSUM_SCI_INSTRUCTIONS(SCISSUM_VM_INSTRUCTION_ENUM)
            
        };
                
    };
    
}

#endif /* VM_hpp */
