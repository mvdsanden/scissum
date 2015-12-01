//
//  VM.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/25/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "VM.hpp"
//#include "KernelFunction.hpp"

#include <stdexcept>
#include <iostream>

namespace scissum {
    
    // TODO: Kernel function handler.
    // TODO: Debug hooks.
    class VM32
    : public VM
    {

        // The stack pointer.
        size_t *d_st;
        
        // The program counter.
        uint8_t const *d_pc;
        
        // The accumelator register.
        size_t d_acc;
        
        // The previous register.
        size_t d_prev;
        
        // Global, local, temporary and parameter variable list pointers.
        uint16_t *d_vars[4];
        
        // The stack.
        size_t d_stck[8 * 1024];

        // The kernel function table.
        KernelFunction d_kernelFunctions[0xffff];
        
        // Opcode tracer hook.
        OpTraceHandler d_opTracer;
        
    public:
        
        VM32()
        {
            reset();
        }
        
        virtual void reset()
        {
            d_st = d_stck;
            d_acc = 0;
            d_prev = 0;
            d_vars[0] = d_vars[1] = d_vars[2] = d_vars[3] = nullptr;
        }
        
        virtual void registerKernelFunction(uint16_t id, KernelFunction const &function)
        {
            d_kernelFunctions[id] = function;
        }
        
        virtual size_t execute(uint8_t const *bytecode, size_t length)
        {
            // Program counter.
            d_pc = bytecode;
            uint8_t const *pcEnd = d_pc + length;
            
            while (d_pc < pcEnd) {
                
                if (d_opTracer) {
                    d_opTracer(d_pc, d_st, d_acc);
                }
                
                switch (*d_pc) {
                        
#define SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Name)\
case OP_##Name##_0: case OP_##Name##_1: ++d_pc, op##Name(); break

#define SCISSUM_VM_SIMPLE_ONE_ARG_OP(Name)\
case OP_##Name##_0: d_pc += 3, op##Name(*reinterpret_cast<uint16_t const*>(d_pc - 2)); break;\
case OP_##Name##_1: d_pc += 2, op##Name(d_pc[-1]); break\


                        
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(BNot);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Add);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Sub);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Mul);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Div);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Mod);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Shr);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Shl);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Xor);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(And);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Or);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Neg);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Not);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Push);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Eq);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Ne);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Gt);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Ge);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Lt);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Le);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(UGt);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(UGe);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(ULt);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(ULe);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Toss);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(Dup);
                        SCISSUM_VM_SIMPLE_ONE_BYTE_OP(PPrev);
                        
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(LdI);
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(PushI);
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(Link);
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(LOfsA);
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(LOfsS);
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(Bt);
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(BNt);
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(Jmp);
                        
                    case OP_Push0_0: case OP_Push0_1: push(0); ++d_pc; break;
                    case OP_Push1_0: case OP_Push1_1: push(1); ++d_pc; break;
                    case OP_Push2_0: case OP_Push2_1: push(2); ++d_pc; break;
                        
                    case OP_Return_0: case OP_Return_1:
                        std::cout << "Returning " << d_acc << ".\n";
                        return d_acc;
                        
                    default:
                        throw std::runtime_error("unimplemented op code");
                }
                
            }
            
            return -1;
        }
      
        size_t *stackPointer() const
        {
            return d_st;
        }
        
        void setOpTraceHook(OpTraceHandler const &handler)
        {
            d_opTracer = handler;
        }
        
        char const *lookupOpName(uint8_t opCode)
        {
            return s_opNames[opCode];
        }
        
    private:
        
        inline void push(size_t value) { *(d_st++) = value; }
        
        inline size_t pop() { return *(--d_st); }

        inline void opBNot() { d_acc ^= 0xffff; }
        
        inline void opAdd() { d_acc += pop(); }
        
        inline void opSub() { d_acc = pop() - d_acc; }
        
        inline void opMul() { d_acc *= pop(); }
        
        inline void opDiv() { d_acc = pop() / d_acc; }
        
        inline void opMod() { d_acc = pop() % d_acc; }
        
        inline void opShr() { d_acc = pop() >> d_acc; }
        
        inline void opShl() { d_acc = pop() << d_acc; }
        
        inline void opXor() { d_acc ^= pop(); }
        
        inline void opAnd() { d_acc &= pop(); }
        
        inline void opOr() { d_acc |= pop(); }
        
        inline void opNeg() { *reinterpret_cast<ssize_t*>(&d_acc) = -*reinterpret_cast<ssize_t*>(&d_acc); }
        
        inline void opNot() { d_acc = !d_acc; }
        
        inline void opPush() { push(d_acc); }
        
        inline void opEq() { d_prev = d_acc, d_acc = (d_acc == pop()); }
        
        inline void opNe() { d_prev = d_acc, d_acc = (d_acc != pop()); }
        
        inline void opGt() { d_prev = d_acc, d_acc = (static_cast<ssize_t>(pop()) > static_cast<ssize_t>(d_acc)); }
        
        inline void opGe() { d_prev = d_acc, d_acc = (static_cast<ssize_t>(pop()) >= static_cast<ssize_t>(d_acc)); }
        
        inline void opLt() { d_prev = d_acc, d_acc = (static_cast<ssize_t>(pop()) < static_cast<ssize_t>(d_acc)); }
        
        inline void opLe() { d_prev = d_acc, d_acc = (static_cast<ssize_t>(pop()) <= static_cast<ssize_t>(d_acc)); }

        inline void opUGt() { d_prev = d_acc, d_acc = (pop() > d_acc); }
        
        inline void opUGe() { d_prev = d_acc, d_acc = (pop() >= d_acc); }
        
        inline void opULt() { d_prev = d_acc, d_acc = (pop() < d_acc); }
        
        inline void opULe() { d_prev = d_acc, d_acc = (pop() <= d_acc); }
        
        inline void opToss() { pop(); }
        
        inline void opDup() { push(*(d_st-1)); }
        
        inline void opPPrev() { push(d_prev); }
        
        inline void opLdI(size_t value) { d_acc = value; }
        
        inline void opPushI(size_t value) { push(value); }
        
        inline void opLink(size_t value) { d_st += value; }
        
        inline void opLOfsA(size_t value) { d_acc = reinterpret_cast<size_t>(d_pc) + value; }
        
        inline void opLOfsS(size_t value) { push(reinterpret_cast<size_t>(d_pc) + value); }
        
        inline void opBt(ssize_t value) { d_pc += (d_acc?value:0); }
        
        inline void opBNt(ssize_t value) { d_pc += (!d_acc?value:0); }
        
        inline void opJmp(ssize_t value) { d_pc += value; }


        
#define SCISSUM_VM_OP_NAMES(OpCode, TotalSize, Name, Alias, ArgCount, Arg0, Arg1, Arg2, Desc)\
#Name,
        
        static char const *s_opNames[];
        
    };
    
    char const *VM32::s_opNames[] = {SCISSUM_SCI_INSTRUCTIONS(SCISSUM_VM_OP_NAMES)};
    
    VM &VM::instance()
    {
        static VM32 s_instance;
        return s_instance;
    }
    
}