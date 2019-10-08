#ifndef __STRING_TO_ARRAY_H
#define __STRING_TO_ARRAY_H

#define STR_TO_ARRAY_1(str)     str[0]
#define STR_TO_ARRAY_2(str)     STR_TO_ARRAY_1(str), str[1]
#define STR_TO_ARRAY_3(str)     STR_TO_ARRAY_2(str), str[2]
#define STR_TO_ARRAY_4(str)     STR_TO_ARRAY_3(str), str[3]
#define STR_TO_ARRAY_5(str)     STR_TO_ARRAY_4(str), str[4]
#define STR_TO_ARRAY_6(str)     STR_TO_ARRAY_5(str), str[5]
#define STR_TO_ARRAY_7(str)     STR_TO_ARRAY_6(str), str[6]
#define STR_TO_ARRAY_8(str)     STR_TO_ARRAY_7(str), str[7]
#define STR_TO_ARRAY_9(str)     STR_TO_ARRAY_8(str), str[8]
#define STR_TO_ARRAY_10(str)    STR_TO_ARRAY_9(str), str[9]
#define STR_TO_ARRAY_11(str)    STR_TO_ARRAY_10(str), str[10]
#define STR_TO_ARRAY_12(str)    STR_TO_ARRAY_11(str), str[11]
#define STR_TO_ARRAY_13(str)    STR_TO_ARRAY_12(str), str[12]
#define STR_TO_ARRAY_14(str)    STR_TO_ARRAY_13(str), str[13]
#define STR_TO_ARRAY_15(str)    STR_TO_ARRAY_14(str), str[14]
#define STR_TO_ARRAY_16(str)    STR_TO_ARRAY_15(str), str[15]
#define STR_TO_ARRAY_17(str)    STR_TO_ARRAY_16(str), str[16]
#define STR_TO_ARRAY_18(str)    STR_TO_ARRAY_17(str), str[17]
#define STR_TO_ARRAY_19(str)    STR_TO_ARRAY_18(str), str[18]
#define STR_TO_ARRAY_20(str)    STR_TO_ARRAY_19(str), str[19]
#define STR_TO_ARRAY_21(str)    STR_TO_ARRAY_20(str), str[20]
#define STR_TO_ARRAY_22(str)    STR_TO_ARRAY_21(str), str[21]
#define STR_TO_ARRAY_23(str)    STR_TO_ARRAY_22(str), str[22]
#define STR_TO_ARRAY_24(str)    STR_TO_ARRAY_23(str), str[23]
#define STR_TO_ARRAY_25(str)    STR_TO_ARRAY_24(str), str[24]
#define STR_TO_ARRAY_26(str)    STR_TO_ARRAY_25(str), str[25]
#define STR_TO_ARRAY_27(str)    STR_TO_ARRAY_26(str), str[26]
#define STR_TO_ARRAY_28(str)    STR_TO_ARRAY_27(str), str[27]
#define STR_TO_ARRAY_29(str)    STR_TO_ARRAY_28(str), str[28]
#define STR_TO_ARRAY_30(str)    STR_TO_ARRAY_29(str), str[29]

#define STR_TO_ARRAY_MAX_LENGTH 30

#define STRING_TO_ARRAY(str,length)         STR_TO_ARRAY_ ## length (str)

#endif


