//
//  VM.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/25/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "VM.hpp"
#include "MemoryManager.hpp"
#include "HeapPointer.hpp"
#include "HeapAllocator.hpp"

#include <stdexcept>
#include <iostream>

namespace scissum {
    
    // TODO: Kernel function handler.
    // TODO: Debug hooks.
    // TODO: Sign extention, by using ssize_t instead of size_t!!!
    class VM32
    : public VM
    {

        typedef HeapPointer<uint16_t> LocalVariableHeapPointer;
        typedef HeapPointer<uint16_t> GlobalVariableHeapPointer;
        typedef HeapPointer<size_t> TempVariableHeapPointer;
        typedef HeapPointer<size_t> ArgumentVariableHeapPointer;
        
        enum {
            CONST_VARIABLE_STACK_DEPTH = 128,
        };
        
        // TODO: move registers to a Register struct so they can easily
        //       be passed to Kernel Functions.
        
        // The stack pointer.
        size_t *d_st;
        
        // The program counter.
        uint8_t const *d_pc;
        
        // The accumelator register.
        size_t d_acc;
        
        // The previous register.
        size_t d_prev;
        
        // Global, local, temporary and parameter variable list pointers.
        
        // The stack.
        HeapPointer<size_t> d_stackPointer;
        // TODO: This pointer needs to be updated in case of an MemoryManager Relocation event.
        size_t *d_stck;//[8 * 1024];
        
        // Put the variable pointers in a struct so that they are always
        // located close to each other in memory.
        // TODO: Use HeapPointer instead of size_t?
        struct Vars {
            HeapPointerBase local;
            HeapPointerBase temp;
            HeapPointerBase arg;
        };
        
        // Global vars are always the local vars of script 0.
        GlobalVariableHeapPointer d_globalVars;
        
        // The variable frame stack.
        Vars d_varStack[CONST_VARIABLE_STACK_DEPTH];
        
        // Separate pointers to the variables stack, so that they can be
        // easily indexed.
        HeapPointerBase *d_vars[4];

        // The kernel function table.
        KernelFunction d_kernelFunctions[256];
        
        // Opcode tracer hook.
        OpTraceHandler d_opTracer;
        
    public:
        
        VM32()
        {
            // Allocate the stack.
            d_stackPointer = HeapAllocator<size_t>().allocate(8 * 1024);
            
            // This needs to be updated on the case of a MemoryManager heap relocation event.
            d_stck = d_stackPointer.operator->();
            
            reset();
        }
        
        virtual void reset()
        {
            d_st = d_stck;
            d_acc = 0;
            d_prev = 0;
            
            memset(d_varStack, 0, sizeof(d_varStack));
            
            d_vars[0] = &d_globalVars;
            d_vars[1] = &d_varStack->local;
            d_vars[2] = &d_varStack->temp;
            d_vars[3] = &d_varStack->arg;
        }
        
        virtual void registerKernelFunction(uint16_t id, KernelFunction const &function)
        {
            d_kernelFunctions[id] = function;
        }
        
        virtual size_t execute(uint8_t const *bytecode, size_t length, HeapPointerBase args, size_t argc)
        {
            // Point initial args to the arguments provided.
            *d_vars[3] = args;
            
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

#define SCISSUM_VM_COMPLEX_OP(OpCode, TotalSize, Name, Alias, ArgCount, Arg0, Arg1, Arg2, Desc, VarType, Operand, AccIndex, Operation)\
case OP_##Name##_0: ++d_pc, op##Name(); break;
                        
                        // All one byte operators.
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
                        
                        // All simple one argument operators.
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(LdI);
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(PushI);
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(Link);
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(LOfsA);
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(LOfsS);
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(Bt);
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(BNt);
                        SCISSUM_VM_SIMPLE_ONE_ARG_OP(Jmp);
                        
                        SCISSUM_SCI_COMPLEX_INSTRUCTIONS(SCISSUM_VM_COMPLEX_OP);
                        
                        // The push operators.
                    case OP_Push0_0: case OP_Push0_1: push(0); ++d_pc; break;
                    case OP_Push1_0: case OP_Push1_1: push(1); ++d_pc; break;
                    case OP_Push2_0: case OP_Push2_1: push(2); ++d_pc; break;
                        
                        // The return operator (for now).
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
        
        // Pushes a call frame.
        inline void pushFrame()
        {
            // Update (local, temp, arg) variable pointers.
            // Start with 1 because global variables always point to the same place.
            for (size_t i = 1; i < 4; ++i) {
                size_t tmp = *d_vars[i];
                d_vars[i] += 3;
                *d_vars[i] = tmp;
            }
        }
        
        // Pops a call frame.
        inline void popFrame()
        {
            // Update (local, temp, arg) variable pointers.
            // Start with 1 because global variables always point to the same place.
            for (size_t i = 1; i < 4; ++i) {
                d_vars[i] -= 3;
            }
        }
        
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
        
        inline void opLink(size_t value)
        {
            // Get a HeapPointer to the current stack pointer.
            *d_vars[2] = MemoryManager::instance().pointerTo(d_st);
            // Increase the stack pointer to allow for space for the temporary variables.
            d_st += value;
        }
        
        inline void opLOfsA(size_t value) { d_acc = reinterpret_cast<size_t>(d_pc) + value; }
        
        inline void opLOfsS(size_t value) { push(reinterpret_cast<size_t>(d_pc) + value); }
        
        inline void opBt(ssize_t value) { d_pc += (d_acc?value:0); }
        
        inline void opBNt(ssize_t value) { d_pc += (!d_acc?value:0); }
        
        inline void opJmp(ssize_t value) { d_pc += value; }

        template <size_t VarType>
        struct VariableTraits {
            typedef HeapPointerBase HeapPointerType;
        };

        // TODO: this could all be done with only the opcode as a template argument...
        template <class ImmType, size_t Operand, size_t Operation, size_t VarType, bool AccIndex>
        inline void opComplex()
        {
            //std::cout << "Complex(" << Operand << ", " << Operation << ", " << VarType << ", " << AccIndex << ").\n";
            
            // Get the index immediate value.
            ImmType index = *reinterpret_cast<ImmType const *>(d_pc); d_pc += sizeof(ImmType);
            
            // Add the accumulator the the index if necessary.
            index += (AccIndex?d_acc:0);

            //std::cout << "- Index=" << static_cast<size_t>(index) << ".\n";
            
            // Get the heap pointer to the value.
            typename VariableTraits<VarType>::HeapPointerType v = *d_vars[VarType];
            
            // Go to the right index.
            v += index;
            
            //std::cout << "- value=" << *v << ".\n";
            
            // Run the operation.
            switch (Operation) {
                case 0:
                    if (Operand == 0) {
                        d_acc = *v;
                    } else {
                        push(*v);
                    }
                    break;
                    
                case 1:
                    if (Operand == 0) {
                        *v = d_acc;
                    } else {
                        *v = pop();
                    }
                    break;
                    
                case 2:
                    if (Operand == 0) {
                        d_acc = ++*v;
                    } else {
                        push(++*v);
                    }
                    break;
                    
                case 3:
                    if (Operand == 0) {
                        d_acc = --*v;
                    } else {
                        push(--*v);
                    }
                    break;

            };
        }
        
#define SCISSUM_VM_OP_NAME(Name) op ## Name
        
#define SCISSUM_VM_COMPLEX_OP_TYPEDEF(OpCode, TotalSize, Name, Alias, ArgCount, Arg0, Arg1, Arg2, Desc, VarType, Operand, AccIndex, Operation)\
void op##Name() { opComplex<Arg0, Operand, Operation, VarType, AccIndex>(); }
        
        SCISSUM_SCI_COMPLEX_INSTRUCTIONS(SCISSUM_VM_COMPLEX_OP_TYPEDEF);
        
        // TODO: make aliases of opComplex with the right opname so we can add this in the switch!
        
#define SCISSUM_VM_OP_NAMES(OpCode, TotalSize, Name, Alias, ArgCount, Arg0, Arg1, Arg2, Desc, ...)\
#Name,
        
        static char const *s_opNames[];
        
    };
    
    template <>
    struct VM32::VariableTraits<0> {
        typedef GlobalVariableHeapPointer HeapPointerType;
    };
    
    template <>
    struct VM32::VariableTraits<1> {
        typedef LocalVariableHeapPointer HeapPointerType;
    };
    
    template <>
    struct VM32::VariableTraits<2> {
        typedef TempVariableHeapPointer HeapPointerType;
    };
    
    template <>
    struct VM32::VariableTraits<3> {
        typedef ArgumentVariableHeapPointer HeapPointerType;
    };
    
    char const *VM32::s_opNames[] = {SCISSUM_SCI_INSTRUCTIONS(SCISSUM_VM_OP_NAMES)};
    
    VM &VM::instance()
    {
        static VM32 s_instance;
        return s_instance;
    }
    
}