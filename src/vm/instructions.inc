// OpCode, Total Bytes, Internal Name, Alias#, #Arguments, Arg0 Type, Arg1 Typw, Arg2 Type, Description
#define SCISSUM_SCI_INSTRUCTIONS(_)\
_(0x00, 1, BinaryNot, 0, 0, void, void, void, "Binary not")\
_(0x01, 1, BinaryNot, 1, 0, void, void, void, "Binary not")\
_(0x02, 1, Add      , 0, 0, void, void, void, "Addition")\
_(0x03, 1, Add      , 1, 0, void, void, void, "Addition")\
_(0x04, 1, Sub      , 0, 0, void, void, void, "Subtraction")\
_(0x05, 1, Sub      , 1, 0, void, void, void, "Subtraction")\
_(0x06, 1, Mul      , 0, 0, void, void, void, "Multiplication")\
_(0x07, 1, Mul      , 1, 0, void, void, void, "Multiplication")\
_(0x08, 1, Div      , 0, 0, void, void, void, "Division")\
_(0x09, 1, Div      , 1, 0, void, void, void, "Division")\
_(0x0a, 1, Mod      , 0, 0, void, void, void, "Modulo")\
_(0x0b, 1, Mod      , 1, 0, void, void, void, "Modulo")\
_(0x0c, 1, Shr      , 0, 0, void, void, void, "Shift right")\
_(0x0d, 1, Shr      , 1, 0, void, void, void, "Shift right")\
_(0x0e, 1, Shl      , 0, 0, void, void, void, "Shift left")\
_(0x0f, 1, Shl      , 1, 0, void, void, void, "Shift left")\
_(0x10, 1, Xor      , 0, 0, void, void, void, "Xor")\
_(0x11, 1, Xor      , 1, 0, void, void, void, "Xor")\
_(0x12, 1, And      , 0, 0, void, void, void, "And")\
_(0x13, 1, And      , 1, 0, void, void, void, "And")\
_(0x14, 1, Or       , 0, 0, void, void, void, "Or")\
_(0x15, 1, Or       , 1, 0, void, void, void, "Or")\
_(0x16, 1, Neg      , 0, 0, void, void, void, "Neg")\
_(0x17, 1, Neg      , 1, 0, void, void, void, "Neg")\
_(0x18, 1, Not      , 0, 0, void, void, void, "Not")\
_(0x19, 1, Not      , 1, 0, void, void, void, "Not")\
_(0x1a, 1, Eq       , 0, 0, void, void, void, "Equals")\
_(0x1b, 1, Eq       , 1, 0, void, void, void, "Equals")\
_(0x1c, 1, Ne       , 0, 0, void, void, void, "Not equals")\
_(0x1d, 1, Ne       , 1, 0, void, void, void, "Not equals")\
_(0x1e, 1, Gt       , 0, 0, void, void, void, "Greater than")\
_(0x1f, 1, Gt       , 1, 0, void, void, void, "Greater than")\
_(0x20, 1, Ge       , 0, 0, void, void, void, "Greater or equal")\
_(0x21, 1, Ge       , 1, 0, void, void, void, "Greater or equal")\
_(0x22, 1, Lt       , 0, 0, void, void, void, "Less than")\
_(0x23, 1, Lt       , 1, 0, void, void, void, "Less than")\
_(0x24, 1, Le       , 0, 0, void, void, void, "Less or equal")\
_(0x25, 1, Le       , 1, 0, void, void, void, "Less or equal")\
_(0x26, 1, UGt      , 0, 0, void, void, void, "")\
_(0x27, 1, UGt      , 1, 0, void, void, void, "")\
_(0x28, 1, UEt      , 0, 0, void, void, void, "")\
_(0x29, 1, UEt      , 1, 0, void, void, void, "")\
_(0x2a, 1, ULt      , 0, 0, void, void, void, "")\
_(0x2b, 1, ULt      , 1, 0, void, void, void, "")\
_(0x2c, 1, ULe      , 0, 0, void, void, void, "")\
_(0x2d, 1, ULe      , 1, 0, void, void, void, "")\
_(0x2e, 3, Bt       , 0, 1, int16_t, void, void, "")\
_(0x2f, 2, Bt       , 1, 1, int8_t , void, void, "")\
_(0x30, 3, BNt      , 0, 1, int16_t, void, void, "")\
_(0x31, 2, BNt      , 1, 1, int8_t , void, void, "")\
_(0x32, 3, Jmp      , 0, 1, int16_t, void, void, "")\
_(0x33, 2, Jmp      , 1, 1, int8_t , void, void, "")\
_(0x34, 3, LdImm    , 0, 1, int16_t, void, void, "")\
_(0x35, 2, LdImm    , 1, 1, int8_t , void, void, "")\
_(0x36, 1, Push     , 0, 0, void, void, void, "")\
_(0x37, 1, Push     , 1, 0, void, void, void, "")\
_(0x38, 3, PushImm  , 0, 1, int16_t, void, void, "")\
_(0x39, 2, PushImm  , 1, 1, int8_t , void, void, "")\
_(0x3a, 1, Toss     , 0, 0, void, void, void, "")\
_(0x3b, 1, Toss     , 1, 0, void, void, void, "")\
_(0x3c, 1, Dup      , 0, 0, void, void, void, "")\
_(0x3d, 1, Dup      , 1, 0, void, void, void, "")\
_(0x3e, 3, Link     , 0, 1, int16_t, void, void, "")\
_(0x3f, 2, Link     , 1, 1, int8_t , void, void, "")\
_(0x40, 4, Call     , 0, 2, int16_t, int8_t, void, "")\
_(0x41, 3, Call     , 1, 2, int8_t , int8_t, void, "")\
_(0x42, 4, CallKern , 0, 2, int16_t, int8_t, void, "")\
_(0x43, 3, CallKern , 1, 2, int8_t , int8_t, void, "")\
_(0x44, 4, CallBase , 0, 2, int16_t, int8_t, void, "")\
_(0x45, 3, CallBase , 1, 2, int8_t , int8_t, void, "")\
_(0x46, 5, CallExt  , 0, 2, int16_t, int16_t, int8_t, "")\
_(0x47, 4, CallExt  , 1, 2, int8_t , int8_t, int8_t, "")\
_(0x48, 1, Return   , 0, 0, void, void, void, "")\
_(0x49, 1, Return   , 1, 0, void, void, void, "")\
_(0x4a, 2, Send     , 0, 1, int8_t, void, void, "")\
_(0x4b, 2, Send     , 1, 1, int8_t , void, void, "")\
_(0x50, 3, Class    , 0, 1, int16_t, void, void, "")\
_(0x51, 2, Class    , 1, 1, int8_t , void, void, "")\
_(0x54, 2, Self     , 0, 1, int8_t, void, void, "")\
_(0x55, 2, Self     , 1, 1, int8_t , void, void, "")\
_(0x56, 4, Super    , 0, 2, int16_t, int8_t, void, "")\
_(0x57, 3, Super    , 1, 2, int8_t , int8_t, void, "")\
_(0x58, 3, Rest     , 0, 1, int16_t, void, void, "")\
_(0x59, 2, Rest     , 1, 1, int8_t , void, void, "")\
_(0x5a, 4, Lea      , 0, 2, int16_t, int16_t, void, "")\
_(0x5b, 2, Lea      , 1, 2, int8_t , int8_t, void, "")\
_(0x5c, 1, SelfId   , 0, 0, void, void, void, "")\
_(0x5d, 1, SelfId   , 1, 0, void, void, void, "")\
_(0x60, 1, PPrev    , 0, 0, void, void, void, "")\
_(0x61, 1, PPrev    , 1, 0, void, void, void, "")\
_(0x62, 3, PToA     , 0, 1, int16_t, void, void, "")\
_(0x63, 2, PToA     , 1, 1, int8_t , void, void, "")\
_(0x64, 3, AToP     , 0, 1, int16_t, void, void, "")\
_(0x65, 2, AToP     , 1, 1, int8_t , void, void, "")\
_(0x66, 3, PToS     , 0, 1, int16_t, void, void, "")\
_(0x67, 2, PToS     , 1, 1, int8_t , void, void, "")\
_(0x68, 3, SToP     , 0, 1, int16_t, void, void, "")\
_(0x69, 2, SToP     , 1, 1, int8_t , void, void, "")\
_(0x6a, 3, IPToA    , 0, 1, int16_t, void, void, "")\
_(0x6b, 2, IPToA    , 1, 1, int8_t , void, void, "")\
_(0x6c, 3, DPToA    , 0, 1, int16_t, void, void, "")\
_(0x6d, 2, DPToA    , 1, 1, int8_t , void, void, "")\
_(0x6e, 3, IPToS    , 0, 1, int16_t, void, void, "")\
_(0x6f, 2, IPToS    , 1, 1, int8_t , void, void, "")\
_(0x70, 3, DPToS    , 0, 1, int16_t, void, void, "")\
_(0x71, 2, DPToS    , 1, 1, int8_t , void, void, "")\
_(0x72, 3, LOfsA    , 0, 1, int16_t, void, void, "")\
_(0x73, 2, LOfsA    , 1, 1, int8_t , void, void, "")\
_(0x74, 3, LOfsS    , 0, 1, int16_t, void, void, "")\
_(0x75, 2, LOfsS    , 1, 1, int8_t , void, void, "")\
_(0x76, 1, Push0    , 0, 0, void, void, void, "")\
_(0x77, 1, Push0    , 1, 0, void, void, void, "")\
_(0x78, 1, Push1    , 0, 0, void, void, void, "")\
_(0x79, 1, Push1    , 1, 0, void, void, void, "")\
_(0x7a, 1, Push2    , 0, 0, void, void, void, "")\
_(0x7b, 1, Push2    , 1, 0, void, void, void, "")\
_(0x7c, 1, PushSelf , 0, 0, void, void, void, "")\
_(0x7d, 1, PushSelf , 1, 0, void, void, void, "")\



