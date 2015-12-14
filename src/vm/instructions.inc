#define SCISSUM_SCI_COMPLEX_INSTRUCTIONS(_)\
_(0x80, 3, LNAGW, 0, 1, uint16_t, int, int, "LNAGW", 0, 0, 0, 0)\
_(0x81, 2, LNAGB, 0, 1, uint8_t, int, int, "LNAGB", 0, 0, 0, 0)\
_(0x82, 3, LNALW, 0, 1, uint16_t, int, int, "LNALW", 1, 0, 0, 0)\
_(0x83, 2, LNALB, 0, 1, uint8_t, int, int, "LNALB", 1, 0, 0, 0)\
_(0x84, 3, LNATW, 0, 1, uint16_t, int, int, "LNATW", 2, 0, 0, 0)\
_(0x85, 2, LNATB, 0, 1, uint8_t, int, int, "LNATB", 2, 0, 0, 0)\
_(0x86, 3, LNAPW, 0, 1, uint16_t, int, int, "LNAPW", 3, 0, 0, 0)\
_(0x87, 2, LNAPB, 0, 1, uint8_t, int, int, "LNAPB", 3, 0, 0, 0)\
_(0x88, 3, LNSGW, 0, 1, uint16_t, int, int, "LNSGW", 0, 1, 0, 0)\
_(0x89, 2, LNSGB, 0, 1, uint8_t, int, int, "LNSGB", 0, 1, 0, 0)\
_(0x8a, 3, LNSLW, 0, 1, uint16_t, int, int, "LNSLW", 1, 1, 0, 0)\
_(0x8b, 2, LNSLB, 0, 1, uint8_t, int, int, "LNSLB", 1, 1, 0, 0)\
_(0x8c, 3, LNSTW, 0, 1, uint16_t, int, int, "LNSTW", 2, 1, 0, 0)\
_(0x8d, 2, LNSTB, 0, 1, uint8_t, int, int, "LNSTB", 2, 1, 0, 0)\
_(0x8e, 3, LNSPW, 0, 1, uint16_t, int, int, "LNSPW", 3, 1, 0, 0)\
_(0x8f, 2, LNSPB, 0, 1, uint8_t, int, int, "LNSPB", 3, 1, 0, 0)\
_(0x90, 3, LIAGW, 0, 1, uint16_t, int, int, "LIAGW", 0, 0, 1, 0)\
_(0x91, 2, LIAGB, 0, 1, uint8_t, int, int, "LIAGB", 0, 0, 1, 0)\
_(0x92, 3, LIALW, 0, 1, uint16_t, int, int, "LIALW", 1, 0, 1, 0)\
_(0x93, 2, LIALB, 0, 1, uint8_t, int, int, "LIALB", 1, 0, 1, 0)\
_(0x94, 3, LIATW, 0, 1, uint16_t, int, int, "LIATW", 2, 0, 1, 0)\
_(0x95, 2, LIATB, 0, 1, uint8_t, int, int, "LIATB", 2, 0, 1, 0)\
_(0x96, 3, LIAPW, 0, 1, uint16_t, int, int, "LIAPW", 3, 0, 1, 0)\
_(0x97, 2, LIAPB, 0, 1, uint8_t, int, int, "LIAPB", 3, 0, 1, 0)\
_(0x98, 3, LISGW, 0, 1, uint16_t, int, int, "LISGW", 0, 1, 1, 0)\
_(0x99, 2, LISGB, 0, 1, uint8_t, int, int, "LISGB", 0, 1, 1, 0)\
_(0x9a, 3, LISLW, 0, 1, uint16_t, int, int, "LISLW", 1, 1, 1, 0)\
_(0x9b, 2, LISLB, 0, 1, uint8_t, int, int, "LISLB", 1, 1, 1, 0)\
_(0x9c, 3, LISTW, 0, 1, uint16_t, int, int, "LISTW", 2, 1, 1, 0)\
_(0x9d, 2, LISTB, 0, 1, uint8_t, int, int, "LISTB", 2, 1, 1, 0)\
_(0x9e, 3, LISPW, 0, 1, uint16_t, int, int, "LISPW", 3, 1, 1, 0)\
_(0x9f, 2, LISPB, 0, 1, uint8_t, int, int, "LISPB", 3, 1, 1, 0)\
_(0xa0, 3, SNAGW, 0, 1, uint16_t, int, int, "SNAGW", 0, 0, 0, 1)\
_(0xa1, 2, SNAGB, 0, 1, uint8_t, int, int, "SNAGB", 0, 0, 0, 1)\
_(0xa2, 3, SNALW, 0, 1, uint16_t, int, int, "SNALW", 1, 0, 0, 1)\
_(0xa3, 2, SNALB, 0, 1, uint8_t, int, int, "SNALB", 1, 0, 0, 1)\
_(0xa4, 3, SNATW, 0, 1, uint16_t, int, int, "SNATW", 2, 0, 0, 1)\
_(0xa5, 2, SNATB, 0, 1, uint8_t, int, int, "SNATB", 2, 0, 0, 1)\
_(0xa6, 3, SNAPW, 0, 1, uint16_t, int, int, "SNAPW", 3, 0, 0, 1)\
_(0xa7, 2, SNAPB, 0, 1, uint8_t, int, int, "SNAPB", 3, 0, 0, 1)\
_(0xa8, 3, SNSGW, 0, 1, uint16_t, int, int, "SNSGW", 0, 1, 0, 1)\
_(0xa9, 2, SNSGB, 0, 1, uint8_t, int, int, "SNSGB", 0, 1, 0, 1)\
_(0xaa, 3, SNSLW, 0, 1, uint16_t, int, int, "SNSLW", 1, 1, 0, 1)\
_(0xab, 2, SNSLB, 0, 1, uint8_t, int, int, "SNSLB", 1, 1, 0, 1)\
_(0xac, 3, SNSTW, 0, 1, uint16_t, int, int, "SNSTW", 2, 1, 0, 1)\
_(0xad, 2, SNSTB, 0, 1, uint8_t, int, int, "SNSTB", 2, 1, 0, 1)\
_(0xae, 3, SNSPW, 0, 1, uint16_t, int, int, "SNSPW", 3, 1, 0, 1)\
_(0xaf, 2, SNSPB, 0, 1, uint8_t, int, int, "SNSPB", 3, 1, 0, 1)\
_(0xb0, 3, SIAGW, 0, 1, uint16_t, int, int, "SIAGW", 0, 0, 1, 1)\
_(0xb1, 2, SIAGB, 0, 1, uint8_t, int, int, "SIAGB", 0, 0, 1, 1)\
_(0xb2, 3, SIALW, 0, 1, uint16_t, int, int, "SIALW", 1, 0, 1, 1)\
_(0xb3, 2, SIALB, 0, 1, uint8_t, int, int, "SIALB", 1, 0, 1, 1)\
_(0xb4, 3, SIATW, 0, 1, uint16_t, int, int, "SIATW", 2, 0, 1, 1)\
_(0xb5, 2, SIATB, 0, 1, uint8_t, int, int, "SIATB", 2, 0, 1, 1)\
_(0xb6, 3, SIAPW, 0, 1, uint16_t, int, int, "SIAPW", 3, 0, 1, 1)\
_(0xb7, 2, SIAPB, 0, 1, uint8_t, int, int, "SIAPB", 3, 0, 1, 1)\
_(0xb8, 3, SISGW, 0, 1, uint16_t, int, int, "SISGW", 0, 1, 1, 1)\
_(0xb9, 2, SISGB, 0, 1, uint8_t, int, int, "SISGB", 0, 1, 1, 1)\
_(0xba, 3, SISLW, 0, 1, uint16_t, int, int, "SISLW", 1, 1, 1, 1)\
_(0xbb, 2, SISLB, 0, 1, uint8_t, int, int, "SISLB", 1, 1, 1, 1)\
_(0xbc, 3, SISTW, 0, 1, uint16_t, int, int, "SISTW", 2, 1, 1, 1)\
_(0xbd, 2, SISTB, 0, 1, uint8_t, int, int, "SISTB", 2, 1, 1, 1)\
_(0xbe, 3, SISPW, 0, 1, uint16_t, int, int, "SISPW", 3, 1, 1, 1)\
_(0xbf, 2, SISPB, 0, 1, uint8_t, int, int, "SISPB", 3, 1, 1, 1)\
_(0xc0, 3, INAGW, 0, 1, uint16_t, int, int, "INAGW", 0, 0, 0, 2)\
_(0xc1, 2, INAGB, 0, 1, uint8_t, int, int, "INAGB", 0, 0, 0, 2)\
_(0xc2, 3, INALW, 0, 1, uint16_t, int, int, "INALW", 1, 0, 0, 2)\
_(0xc3, 2, INALB, 0, 1, uint8_t, int, int, "INALB", 1, 0, 0, 2)\
_(0xc4, 3, INATW, 0, 1, uint16_t, int, int, "INATW", 2, 0, 0, 2)\
_(0xc5, 2, INATB, 0, 1, uint8_t, int, int, "INATB", 2, 0, 0, 2)\
_(0xc6, 3, INAPW, 0, 1, uint16_t, int, int, "INAPW", 3, 0, 0, 2)\
_(0xc7, 2, INAPB, 0, 1, uint8_t, int, int, "INAPB", 3, 0, 0, 2)\
_(0xc8, 3, INSGW, 0, 1, uint16_t, int, int, "INSGW", 0, 1, 0, 2)\
_(0xc9, 2, INSGB, 0, 1, uint8_t, int, int, "INSGB", 0, 1, 0, 2)\
_(0xca, 3, INSLW, 0, 1, uint16_t, int, int, "INSLW", 1, 1, 0, 2)\
_(0xcb, 2, INSLB, 0, 1, uint8_t, int, int, "INSLB", 1, 1, 0, 2)\
_(0xcc, 3, INSTW, 0, 1, uint16_t, int, int, "INSTW", 2, 1, 0, 2)\
_(0xcd, 2, INSTB, 0, 1, uint8_t, int, int, "INSTB", 2, 1, 0, 2)\
_(0xce, 3, INSPW, 0, 1, uint16_t, int, int, "INSPW", 3, 1, 0, 2)\
_(0xcf, 2, INSPB, 0, 1, uint8_t, int, int, "INSPB", 3, 1, 0, 2)\
_(0xd0, 3, IIAGW, 0, 1, uint16_t, int, int, "IIAGW", 0, 0, 1, 2)\
_(0xd1, 2, IIAGB, 0, 1, uint8_t, int, int, "IIAGB", 0, 0, 1, 2)\
_(0xd2, 3, IIALW, 0, 1, uint16_t, int, int, "IIALW", 1, 0, 1, 2)\
_(0xd3, 2, IIALB, 0, 1, uint8_t, int, int, "IIALB", 1, 0, 1, 2)\
_(0xd4, 3, IIATW, 0, 1, uint16_t, int, int, "IIATW", 2, 0, 1, 2)\
_(0xd5, 2, IIATB, 0, 1, uint8_t, int, int, "IIATB", 2, 0, 1, 2)\
_(0xd6, 3, IIAPW, 0, 1, uint16_t, int, int, "IIAPW", 3, 0, 1, 2)\
_(0xd7, 2, IIAPB, 0, 1, uint8_t, int, int, "IIAPB", 3, 0, 1, 2)\
_(0xd8, 3, IISGW, 0, 1, uint16_t, int, int, "IISGW", 0, 1, 1, 2)\
_(0xd9, 2, IISGB, 0, 1, uint8_t, int, int, "IISGB", 0, 1, 1, 2)\
_(0xda, 3, IISLW, 0, 1, uint16_t, int, int, "IISLW", 1, 1, 1, 2)\
_(0xdb, 2, IISLB, 0, 1, uint8_t, int, int, "IISLB", 1, 1, 1, 2)\
_(0xdc, 3, IISTW, 0, 1, uint16_t, int, int, "IISTW", 2, 1, 1, 2)\
_(0xdd, 2, IISTB, 0, 1, uint8_t, int, int, "IISTB", 2, 1, 1, 2)\
_(0xde, 3, IISPW, 0, 1, uint16_t, int, int, "IISPW", 3, 1, 1, 2)\
_(0xdf, 2, IISPB, 0, 1, uint8_t, int, int, "IISPB", 3, 1, 1, 2)\
_(0xe0, 3, DNAGW, 0, 1, uint16_t, int, int, "DNAGW", 0, 0, 0, 3)\
_(0xe1, 2, DNAGB, 0, 1, uint8_t, int, int, "DNAGB", 0, 0, 0, 3)\
_(0xe2, 3, DNALW, 0, 1, uint16_t, int, int, "DNALW", 1, 0, 0, 3)\
_(0xe3, 2, DNALB, 0, 1, uint8_t, int, int, "DNALB", 1, 0, 0, 3)\
_(0xe4, 3, DNATW, 0, 1, uint16_t, int, int, "DNATW", 2, 0, 0, 3)\
_(0xe5, 2, DNATB, 0, 1, uint8_t, int, int, "DNATB", 2, 0, 0, 3)\
_(0xe6, 3, DNAPW, 0, 1, uint16_t, int, int, "DNAPW", 3, 0, 0, 3)\
_(0xe7, 2, DNAPB, 0, 1, uint8_t, int, int, "DNAPB", 3, 0, 0, 3)\
_(0xe8, 3, DNSGW, 0, 1, uint16_t, int, int, "DNSGW", 0, 1, 0, 3)\
_(0xe9, 2, DNSGB, 0, 1, uint8_t, int, int, "DNSGB", 0, 1, 0, 3)\
_(0xea, 3, DNSLW, 0, 1, uint16_t, int, int, "DNSLW", 1, 1, 0, 3)\
_(0xeb, 2, DNSLB, 0, 1, uint8_t, int, int, "DNSLB", 1, 1, 0, 3)\
_(0xec, 3, DNSTW, 0, 1, uint16_t, int, int, "DNSTW", 2, 1, 0, 3)\
_(0xed, 2, DNSTB, 0, 1, uint8_t, int, int, "DNSTB", 2, 1, 0, 3)\
_(0xee, 3, DNSPW, 0, 1, uint16_t, int, int, "DNSPW", 3, 1, 0, 3)\
_(0xef, 2, DNSPB, 0, 1, uint8_t, int, int, "DNSPB", 3, 1, 0, 3)\
_(0xf0, 3, DIAGW, 0, 1, uint16_t, int, int, "DIAGW", 0, 0, 1, 3)\
_(0xf1, 2, DIAGB, 0, 1, uint8_t, int, int, "DIAGB", 0, 0, 1, 3)\
_(0xf2, 3, DIALW, 0, 1, uint16_t, int, int, "DIALW", 1, 0, 1, 3)\
_(0xf3, 2, DIALB, 0, 1, uint8_t, int, int, "DIALB", 1, 0, 1, 3)\
_(0xf4, 3, DIATW, 0, 1, uint16_t, int, int, "DIATW", 2, 0, 1, 3)\
_(0xf5, 2, DIATB, 0, 1, uint8_t, int, int, "DIATB", 2, 0, 1, 3)\
_(0xf6, 3, DIAPW, 0, 1, uint16_t, int, int, "DIAPW", 3, 0, 1, 3)\
_(0xf7, 2, DIAPB, 0, 1, uint8_t, int, int, "DIAPB", 3, 0, 1, 3)\
_(0xf8, 3, DISGW, 0, 1, uint16_t, int, int, "DISGW", 0, 1, 1, 3)\
_(0xf9, 2, DISGB, 0, 1, uint8_t, int, int, "DISGB", 0, 1, 1, 3)\
_(0xfa, 3, DISLW, 0, 1, uint16_t, int, int, "DISLW", 1, 1, 1, 3)\
_(0xfb, 2, DISLB, 0, 1, uint8_t, int, int, "DISLB", 1, 1, 1, 3)\
_(0xfc, 3, DISTW, 0, 1, uint16_t, int, int, "DISTW", 2, 1, 1, 3)\
_(0xfd, 2, DISTB, 0, 1, uint8_t, int, int, "DISTB", 2, 1, 1, 3)\
_(0xfe, 3, DISPW, 0, 1, uint16_t, int, int, "DISPW", 3, 1, 1, 3)\
_(0xff, 2, DISPB, 0, 1, uint8_t, int, int, "DISPB", 3, 1, 1, 3)\

// OpCode, Total Bytes, Internal Name, Alias#, #Arguments, Arg0 Type, Arg1 Typw, Arg2 Type, Description
#define SCISSUM_SCI_INSTRUCTIONS(_)\
_(0x00, 1, BNot     , 0, 0, int, int, int, "Binary not")\
_(0x01, 1, BNot     , 1, 0, int, int, int, "Binary not")\
_(0x02, 1, Add      , 0, 0, int, int, int, "Addition")\
_(0x03, 1, Add      , 1, 0, int, int, int, "Addition")\
_(0x04, 1, Sub      , 0, 0, int, int, int, "Subtraction")\
_(0x05, 1, Sub      , 1, 0, int, int, int, "Subtraction")\
_(0x06, 1, Mul      , 0, 0, int, int, int, "Multiplication")\
_(0x07, 1, Mul      , 1, 0, int, int, int, "Multiplication")\
_(0x08, 1, Div      , 0, 0, int, int, int, "Division")\
_(0x09, 1, Div      , 1, 0, int, int, int, "Division")\
_(0x0a, 1, Mod      , 0, 0, int, int, int, "Modulo")\
_(0x0b, 1, Mod      , 1, 0, int, int, int, "Modulo")\
_(0x0c, 1, Shr      , 0, 0, int, int, int, "Shift right")\
_(0x0d, 1, Shr      , 1, 0, int, int, int, "Shift right")\
_(0x0e, 1, Shl      , 0, 0, int, int, int, "Shift left")\
_(0x0f, 1, Shl      , 1, 0, int, int, int, "Shift left")\
_(0x10, 1, Xor      , 0, 0, int, int, int, "Xor")\
_(0x11, 1, Xor      , 1, 0, int, int, int, "Xor")\
_(0x12, 1, And      , 0, 0, int, int, int, "And")\
_(0x13, 1, And      , 1, 0, int, int, int, "And")\
_(0x14, 1, Or       , 0, 0, int, int, int, "Or")\
_(0x15, 1, Or       , 1, 0, int, int, int, "Or")\
_(0x16, 1, Neg      , 0, 0, int, int, int, "Neg")\
_(0x17, 1, Neg      , 1, 0, int, int, int, "Neg")\
_(0x18, 1, Not      , 0, 0, int, int, int, "Not")\
_(0x19, 1, Not      , 1, 0, int, int, int, "Not")\
_(0x1a, 1, Eq       , 0, 0, int, int, int, "Equals")\
_(0x1b, 1, Eq       , 1, 0, int, int, int, "Equals")\
_(0x1c, 1, Ne       , 0, 0, int, int, int, "Not equals")\
_(0x1d, 1, Ne       , 1, 0, int, int, int, "Not equals")\
_(0x1e, 1, Gt       , 0, 0, int, int, int, "Greater than")\
_(0x1f, 1, Gt       , 1, 0, int, int, int, "Greater than")\
_(0x20, 1, Ge       , 0, 0, int, int, int, "Greater or equal")\
_(0x21, 1, Ge       , 1, 0, int, int, int, "Greater or equal")\
_(0x22, 1, Lt       , 0, 0, int, int, int, "Less than")\
_(0x23, 1, Lt       , 1, 0, int, int, int, "Less than")\
_(0x24, 1, Le       , 0, 0, int, int, int, "Less or equal")\
_(0x25, 1, Le       , 1, 0, int, int, int, "Less or equal")\
_(0x26, 1, UGt      , 0, 0, int, int, int, "")\
_(0x27, 1, UGt      , 1, 0, int, int, int, "")\
_(0x28, 1, UGe      , 0, 0, int, int, int, "")\
_(0x29, 1, UGe      , 1, 0, int, int, int, "")\
_(0x2a, 1, ULt      , 0, 0, int, int, int, "")\
_(0x2b, 1, ULt      , 1, 0, int, int, int, "")\
_(0x2c, 1, ULe      , 0, 0, int, int, int, "")\
_(0x2d, 1, ULe      , 1, 0, int, int, int, "")\
_(0x2e, 3, Bt       , 0, 1, int16_t, int, int, "")\
_(0x2f, 2, Bt       , 1, 1, int8_t , int, int, "")\
_(0x30, 3, BNt      , 0, 1, int16_t, int, int, "")\
_(0x31, 2, BNt      , 1, 1, int8_t , int, int, "")\
_(0x32, 3, Jmp      , 0, 1, int16_t, int, int, "")\
_(0x33, 2, Jmp      , 1, 1, int8_t , int, int, "")\
_(0x34, 3, LdI      , 0, 1, int16_t, int, int, "")\
_(0x35, 2, LdI      , 1, 1, int8_t , int, int, "")\
_(0x36, 1, Push     , 0, 0, int, int, int, "")\
_(0x37, 1, Push     , 1, 0, int, int, int, "")\
_(0x38, 3, PushI    , 0, 1, int16_t, int, int, "")\
_(0x39, 2, PushI    , 1, 1, int8_t , int, int, "")\
_(0x3a, 1, Toss     , 0, 0, int, int, int, "")\
_(0x3b, 1, Toss     , 1, 0, int, int, int, "")\
_(0x3c, 1, Dup      , 0, 0, int, int, int, "")\
_(0x3d, 1, Dup      , 1, 0, int, int, int, "")\
_(0x3e, 3, Link     , 0, 1, int16_t, int, int, "")\
_(0x3f, 2, Link     , 1, 1, int8_t , int, int, "")\
_(0x40, 4, Call     , 0, 2, int16_t, int8_t, int, "")\
_(0x41, 3, Call     , 1, 2, int8_t , int8_t, int, "")\
_(0x42, 4, CallKern , 0, 2, int16_t, int8_t, int, "")\
_(0x43, 3, CallKern , 1, 2, int8_t , int8_t, int, "")\
_(0x44, 4, CallBase , 0, 2, int16_t, int8_t, int, "")\
_(0x45, 3, CallBase , 1, 2, int8_t , int8_t, int, "")\
_(0x46, 6, CallExt  , 0, 3, uint16_t, uint16_t, uint8_t, "")\
_(0x47, 4, CallExt  , 1, 3, uint8_t , uint8_t, uint8_t, "")\
_(0x48, 1, Return   , 0, 0, int, int, int, "")\
_(0x49, 1, Return   , 1, 0, int, int, int, "")\
_(0x4a, 2, Send     , 0, 1, int8_t, int, int, "")\
_(0x4b, 2, Send     , 1, 1, int8_t , int, int, "")\
_(0x4c, 1, Nop      , 0, 0, int, int, int, "")\
_(0x4d, 1, Nop      , 1, 0, int, int, int, "")\
_(0x4e, 1, Nop      , 2, 0, int, int, int, "")\
_(0x4f, 1, Nop      , 3, 0, int, int, int, "")\
_(0x50, 3, Class    , 0, 1, int16_t, int, int, "")\
_(0x51, 2, Class    , 1, 1, int8_t , int, int, "")\
_(0x52, 1, Nop      , 4, 0, int, int, int, "")\
_(0x53, 1, Nop      , 5, 0, int, int, int, "")\
_(0x54, 2, Self     , 0, 1, int8_t, int, int, "")\
_(0x55, 2, Self     , 1, 1, int8_t , int, int, "")\
_(0x56, 4, Super    , 0, 2, int16_t, int8_t, int, "")\
_(0x57, 3, Super    , 1, 2, int8_t , int8_t, int, "")\
_(0x58, 3, Rest     , 0, 1, int16_t, int, int, "")\
_(0x59, 2, Rest     , 1, 1, int8_t , int, int, "")\
_(0x5a, 4, Lea      , 0, 2, int16_t, int16_t, int, "")\
_(0x5b, 2, Lea      , 1, 2, int8_t , int8_t, int, "")\
_(0x5c, 1, SelfId   , 0, 0, int, int, int, "")\
_(0x5d, 1, SelfId   , 1, 0, int, int, int, "")\
_(0x5e, 1, Nop      , 6, 0, int, int, int, "")\
_(0x5f, 1, Nop      , 7, 0, int, int, int, "")\
_(0x60, 1, PPrev    , 0, 0, int, int, int, "")\
_(0x61, 1, PPrev    , 1, 0, int, int, int, "")\
_(0x62, 3, PToA     , 0, 1, int16_t, int, int, "")\
_(0x63, 2, PToA     , 1, 1, int8_t , int, int, "")\
_(0x64, 3, AToP     , 0, 1, int16_t, int, int, "")\
_(0x65, 2, AToP     , 1, 1, int8_t , int, int, "")\
_(0x66, 3, PToS     , 0, 1, int16_t, int, int, "")\
_(0x67, 2, PToS     , 1, 1, int8_t , int, int, "")\
_(0x68, 3, SToP     , 0, 1, int16_t, int, int, "")\
_(0x69, 2, SToP     , 1, 1, int8_t , int, int, "")\
_(0x6a, 3, IPToA    , 0, 1, int16_t, int, int, "")\
_(0x6b, 2, IPToA    , 1, 1, int8_t , int, int, "")\
_(0x6c, 3, DPToA    , 0, 1, int16_t, int, int, "")\
_(0x6d, 2, DPToA    , 1, 1, int8_t , int, int, "")\
_(0x6e, 3, IPToS    , 0, 1, int16_t, int, int, "")\
_(0x6f, 2, IPToS    , 1, 1, int8_t , int, int, "")\
_(0x70, 3, DPToS    , 0, 1, int16_t, int, int, "")\
_(0x71, 2, DPToS    , 1, 1, int8_t , int, int, "")\
_(0x72, 3, LOfsA    , 0, 1, int16_t, int, int, "")\
_(0x73, 2, LOfsA    , 1, 1, int8_t , int, int, "")\
_(0x74, 3, LOfsS    , 0, 1, int16_t, int, int, "")\
_(0x75, 2, LOfsS    , 1, 1, int8_t , int, int, "")\
_(0x76, 1, Push0    , 0, 0, int, int, int, "")\
_(0x77, 1, Push0    , 1, 0, int, int, int, "")\
_(0x78, 1, Push1    , 0, 0, int, int, int, "")\
_(0x79, 1, Push1    , 1, 0, int, int, int, "")\
_(0x7a, 1, Push2    , 0, 0, int, int, int, "")\
_(0x7b, 1, Push2    , 1, 0, int, int, int, "")\
_(0x7c, 1, PushSelf , 0, 0, int, int, int, "")\
_(0x7d, 1, PushSelf , 1, 0, int, int, int, "")\
_(0x7e, 3, Line     , 0, 1, uint16_t, int, int, "Line Number")\
_(0x7f, 3, Line     , 1, 1, uint16_t, int, int, "Line Number")\
SCISSUM_SCI_COMPLEX_INSTRUCTIONS(_)\

