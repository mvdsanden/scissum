//
//  test_vm.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/25/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "vm/VM.hpp"

#include <iostream>

using namespace scissum;

void opTracer(uint8_t const *pc, size_t const *st, size_t acc)
{
    std::cout << "0x" << std::hex << (pc - static_cast<uint8_t const *>(0)) << ": " << VM::instance().lookupOpName(*pc) << "\t\t(acc=" << acc << ", *(st-1)=" << *(st-1) << ")\n";
}

void test0()
{
    VM &vm = VM::instance();
    vm.reset();
    
    uint8_t program[] = {VM::OP_Push0_0, VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Add_0, VM::OP_Return_0};
    
    size_t result = vm.execute(program, sizeof(program));
    
    std::cout << "Result: " << result << ".\n";
    std::cout << "Stack top: " << *vm.stackPointer() << ".\n";
}

void testProg(std::string const &testName, uint8_t const *program, size_t length, size_t result)
{
    VM &vm = VM::instance();
    vm.reset();
    if (vm.execute(program, length) != result) {
        std::cout << "test " << testName << ": FAILED (expected " << result << ").\n";
        exit(1);
    }
    std::cout << "test " << testName << ": SUCCESS.\n";
}

#define TEST_PROG(TestName, Result, Program)\
uint8_t program[] = Program;\
testProg(TestName, program, sizeof(program), Result);

void testPush0()
{
    testProg("Push0", (uint8_t[]){VM::OP_Push0_0, VM::OP_Add_0, VM::OP_Return_0}, 3, 0);
}

void testPush1()
{
    testProg("Push1", (uint8_t[]){VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Return_0}, 3, 1);
}

void testPush2()
{
    testProg("Push2", (uint8_t[]){VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Return_0}, 3, 2);
}

void testBNot()
{
    testProg("BNot", (uint8_t[]){VM::OP_Push2_0, VM::OP_Add_0, VM::OP_BNot_0, VM::OP_Return_0}, 4, static_cast<uint16_t>(2) ^ 0xffff);
}

void testAdd()
{
    testProg("Add", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Add_0, VM::OP_Return_0}, 5, 4);
}

void testSub()
{
    testProg("Sub", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Sub_0, VM::OP_Return_0}, 5, -1);
}

void testMul()
{
    testProg("Mul", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Mul_0, VM::OP_Return_0}, 5, 4);
}

void testDiv()
{
    testProg("Div", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Div_0, VM::OP_Return_0}, 5, 1);
}

void testMod()
{
    testProg("Mod", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Mod_0, VM::OP_Return_0}, 5, 0);
}

void testShr()
{
    testProg("Shr", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Shr_0, VM::OP_Return_0}, 5, 1);
}

void testShl()
{
    testProg("Shl", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Shl_0, VM::OP_Return_0}, 5, 8);
}

void testXor()
{
    testProg("Xor", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Xor_0, VM::OP_Return_0}, 5, 2 ^ 1);
}

void testAnd()
{
    testProg("And0", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_And_0, VM::OP_Return_0}, 5, 0);
    testProg("And1", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_And_0, VM::OP_Return_0}, 5, 2);
}

void testOr()
{
    testProg("Or", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Or_0, VM::OP_Return_0}, 5, 3);
}

void testNeg()
{
    testProg("Neg", (uint8_t[]){VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_Return_0}, 4, -2);
}

void testNot()
{
    testProg("Not0", (uint8_t[]){VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Not_0, VM::OP_Return_0}, 4, 0);
    testProg("Not1", (uint8_t[]){VM::OP_Push0_0, VM::OP_Add_0, VM::OP_Not_0, VM::OP_Return_0}, 4, 1);
}

void testEQ()
{
    testProg("EQ0", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Eq_0, VM::OP_Return_0}, 5, 1);
    testProg("EQ1", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Eq_0, VM::OP_Return_0}, 5, 0);
}

void testNE()
{
    testProg("NE0", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Ne_0, VM::OP_Return_0}, 5, 0);
    testProg("NE1", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Ne_0, VM::OP_Return_0}, 5, 1);
}

void testGT()
{
    testProg("GT0", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Gt_0, VM::OP_Return_0}, 5, 0);
    testProg("GT1", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Gt_0, VM::OP_Return_0}, 5, 0);
    testProg("GT2", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Gt_0, VM::OP_Return_0}, 5, 1);
    testProg("GT3", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_Gt_0, VM::OP_Return_0}, 6, 1);
    testProg("GT4", (uint8_t[]){VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_Push_0, VM::OP_Neg_0, VM::OP_Gt_0, VM::OP_Return_0}, 7, 0);
}

void testGE()
{
    testProg("GE0", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Ge_0, VM::OP_Return_0}, 5, 0);
    testProg("GE1", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Ge_0, VM::OP_Return_0}, 5, 1);
    testProg("GE2", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Ge_0, VM::OP_Return_0}, 5, 1);
    testProg("GE3", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_Ge_0, VM::OP_Return_0}, 6, 1);
    testProg("GE4", (uint8_t[]){VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_Push_0, VM::OP_Neg_0, VM::OP_Ge_0, VM::OP_Return_0}, 7, 0);
}

void testLT()
{
    testProg("LT0", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Lt_0, VM::OP_Return_0}, 5, 1);
    testProg("LT1", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Lt_0, VM::OP_Return_0}, 5, 0);
    testProg("LT2", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Lt_0, VM::OP_Return_0}, 5, 0);
    testProg("LT3", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_Lt_0, VM::OP_Return_0}, 6, 0);
    testProg("LT4", (uint8_t[]){VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_Push_0, VM::OP_Neg_0, VM::OP_Lt_0, VM::OP_Return_0}, 7, 1);
}

void testLE()
{
    testProg("LE0", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Le_0, VM::OP_Return_0}, 5, 1);
    testProg("LE1", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Le_0, VM::OP_Return_0}, 5, 1);
    testProg("LE2", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Le_0, VM::OP_Return_0}, 5, 0);
    testProg("LE3", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_Le_0, VM::OP_Return_0}, 6, 0);
    testProg("LE4", (uint8_t[]){VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_Push_0, VM::OP_Neg_0, VM::OP_Le_0, VM::OP_Return_0}, 7, 1);
}

void testUGT()
{
    testProg("UGT0", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_UGt_0, VM::OP_Return_0}, 5, 0);
    testProg("UGT1", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_UGt_0, VM::OP_Return_0}, 5, 0);
    testProg("UGT2", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_UGt_0, VM::OP_Return_0}, 5, 1);
    testProg("UGT3", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_UGt_0, VM::OP_Return_0}, 6, 0);
    testProg("UGT4", (uint8_t[]){VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_Push_0, VM::OP_Neg_0, VM::OP_UGt_0, VM::OP_Return_0}, 7, 1);
}

void testUGE()
{
    testProg("UGE0", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_UGe_0, VM::OP_Return_0}, 5, 0);
    testProg("UGE1", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_UGe_0, VM::OP_Return_0}, 5, 1);
    testProg("UGE2", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_UGe_0, VM::OP_Return_0}, 5, 1);
    testProg("UGE3", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_UGe_0, VM::OP_Return_0}, 6, 0);
    testProg("UGE4", (uint8_t[]){VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_Push_0, VM::OP_Neg_0, VM::OP_UGe_0, VM::OP_Return_0}, 7, 1);
}

void testULT()
{
    testProg("ULT0", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_ULt_0, VM::OP_Return_0}, 5, 1);
    testProg("ULT1", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_ULt_0, VM::OP_Return_0}, 5, 0);
    testProg("ULT2", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_ULt_0, VM::OP_Return_0}, 5, 0);
    testProg("ULT3", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_ULt_0, VM::OP_Return_0}, 6, 1);
    testProg("ULT4", (uint8_t[]){VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_Push_0, VM::OP_Neg_0, VM::OP_ULt_0, VM::OP_Return_0}, 7, 0);
}

void testULE()
{
    testProg("ULE0", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_ULe_0, VM::OP_Return_0}, 5, 1);
    testProg("ULE1", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_ULe_0, VM::OP_Return_0}, 5, 1);
    testProg("ULE2", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_ULe_0, VM::OP_Return_0}, 5, 0);
    testProg("ULE3", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_ULe_0, VM::OP_Return_0}, 6, 1);
    testProg("ULE4", (uint8_t[]){VM::OP_Push1_0, VM::OP_Add_0, VM::OP_Neg_0, VM::OP_Push_0, VM::OP_Neg_0, VM::OP_ULe_0, VM::OP_Return_0}, 7, 0);
}

void testLdI()
{
    testProg("LDI0", (uint8_t[]){VM::OP_LdI_1, 0x02, VM::OP_Push2_0, VM::OP_Eq_0, VM::OP_Return_0}, 5, 1);
    testProg("LDI1", (uint8_t[]){VM::OP_LdI_0, 0x02, 0x00, VM::OP_Push2_0, VM::OP_Eq_0, VM::OP_Return_0}, 6, 1);
    testProg("LDI2", (uint8_t[]){VM::OP_LdI_0, 0x03, 0x00, VM::OP_Push2_0, VM::OP_Eq_0, VM::OP_Return_0}, 6, 0);
}

void testPushI()
{
    testProg("PushI0", (uint8_t[]){VM::OP_LdI_1, 0x02, VM::OP_PushI_0, 0x02, 0x00, VM::OP_Eq_0, VM::OP_Return_0}, 7, 1);
    testProg("PushI1", (uint8_t[]){VM::OP_LdI_0, 0x02, 0x00, VM::OP_PushI_1, 0x02, VM::OP_Eq_0, VM::OP_Return_0}, 7, 1);
    testProg("PushI2", (uint8_t[]){VM::OP_LdI_0, 0x03, 0x00, VM::OP_PushI_0, 0x02, 0x02, VM::OP_Eq_0, VM::OP_Return_0}, 8, 0);
}

void testToss()
{
    testProg("Toss", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push1_0, VM::OP_Toss_0, VM::OP_Add_0, VM::OP_Return_0}, 5, 2);
}

void testDup()
{
    testProg("Dup0", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push1_0, VM::OP_Dup_0, VM::OP_LdI_1, 0x01, VM::OP_Eq_0, VM::OP_Return_0}, 7, 1);
    testProg("Dup1", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push1_0, VM::OP_Dup_0, VM::OP_Toss_0, VM::OP_LdI_1, 0x01, VM::OP_Eq_0, VM::OP_Return_0}, 8, 1);
    testProg("Dup2", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push1_0, VM::OP_Dup_0, VM::OP_Toss_0, VM::OP_Toss_0, VM::OP_LdI_1, 0x01, VM::OP_Eq_0, VM::OP_Return_0}, 9, 0);
}

void testLink()
{
    testProg("Link0", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Link_0, 0x03, 0x00, VM::OP_Toss_0, VM::OP_Toss_0, VM::OP_Toss_0, VM::OP_LdI_1, 0x02, VM::OP_Eq_0, VM::OP_Return_0}, 12, 1);
    testProg("Link1", (uint8_t[]){VM::OP_Push1_0, VM::OP_Push2_0, VM::OP_Link_0, 0x03, 0x00, VM::OP_Toss_0, VM::OP_Toss_0, VM::OP_Toss_0, VM::OP_Toss_0, VM::OP_LdI_1, 0x01, VM::OP_Eq_0, VM::OP_Return_0}, 13, 1);
}

void testPPrev()
{
    testProg("PPrev", (uint8_t[]){VM::OP_Push2_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_Eq_0, VM::OP_Push2_0, VM::OP_Add_0, VM::OP_PPrev_0, VM::OP_Add_0, VM::OP_Return_0}, 9, 5);
}

void testLOfsA()
{
    uint8_t code0[] = {VM::OP_LOfsA_0, 0x02, 0x00, VM::OP_Return_0};
    testProg("LOfsA0", code0, 4, reinterpret_cast<size_t>(code0) + 5);
    
    uint8_t code1[] = {VM::OP_LOfsA_1, 0x02, VM::OP_Return_0};
    testProg("LOfsA1", code1, 3, reinterpret_cast<size_t>(code1) + 4);
}

void testLOfsS()
{
    uint8_t code0[] = {VM::OP_LOfsS_0, 0x02, 0x00, VM::OP_Add_0, VM::OP_Return_0};
    testProg("LOfsS0", code0, 5, reinterpret_cast<size_t>(code0) + 5);
    
    uint8_t code1[] = {VM::OP_LOfsS_1, 0x02, VM::OP_Add_0, VM::OP_Return_0};
    testProg("LOfsS1", code1, 4, reinterpret_cast<size_t>(code1) + 4);
}

void testJmp()
{
    testProg("Jmp", (uint8_t[]){VM::OP_LdI_0,0x20,0x01,VM::OP_Jmp_1,0x03,VM::OP_LdI_0,0x11,0x11,VM::OP_Return_0}, 9, 0x0120);
}

void testBt()
{
    testProg("Bt0", (uint8_t[]){VM::OP_LdI_1,0x01,VM::OP_Bt_1,0x03,VM::OP_LdI_0,0x11,0x11,VM::OP_Return_0}, 8, 1);
    testProg("Bt1", (uint8_t[]){VM::OP_LdI_1,0x00,VM::OP_Bt_1,0x03,VM::OP_LdI_0,0x11,0x11,VM::OP_Return_0}, 8, 0x1111);
    testProg("Bt2", (uint8_t[]){
        VM::OP_LdI_1,0x05,
        VM::OP_Push_0,
        VM::OP_LdI_1,0x10,

        VM::OP_BNt_1,0x03,
        VM::OP_LdI_0,0x11,0x11,
        VM::OP_Return_0
    }, 8, 0);
}

void testBNt()
{
    testProg("BNt0", (uint8_t[]){VM::OP_LdI_1,0x01,VM::OP_BNt_1,0x03,VM::OP_LdI_0,0x11,0x11,VM::OP_Return_0}, 8, 0x1111);
    testProg("BNt1", (uint8_t[]){VM::OP_LdI_1,0x00,VM::OP_BNt_1,0x03,VM::OP_LdI_0,0x11,0x11,VM::OP_Return_0}, 8, 0);
}

int main()
{
    VM::instance().setOpTraceHook(opTracer);
    
    test0();
    
    testPush0();
    testPush1();
    testPush2();
    testBNot();
    testAdd();
    testSub();
    testMul();
    testDiv();
    testMod();
    testShr();
    testShl();
    testXor();
    testOr();
    testNeg();
    testNot();
    testEQ();
    testNE();
    testGT();
    testGE();
    testLT();
    testLE();
    testUGT();
    testUGE();
    testULT();
    testULE();
    testLdI();
    testPushI();
    testToss();
    testDup();
    testPPrev();
    testLink();
    testLOfsA();
    testLOfsS();
    testJmp();
    testBt();
    testBNt();
    
    return 0;
}
