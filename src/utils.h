#pragma once

#ifndef __utils_h__
#define __utils_h__
#include <time.h>
#include <string.h>
#include <assert.h>

#if __cplusplus == 202002L
using BYTE = std::byte;
#else
using BYTE = unsigned char;
#endif

typedef struct StrBuf_s{
    char str[80];
}StrBuf;

static inline StrBuf u32toTimeStr(time_t seconds){
    StrBuf timeStr= {0};
    static struct tm tmBuf;
    memcpy(&tmBuf, localtime(&seconds), sizeof(struct tm));
    strftime((char *__restrict)&timeStr.str, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tmBuf);
    return timeStr;
}


inline constexpr bool is_power_of_2(long n){
    return (n>0 && ((n & (n-1)) == 0));
}


inline constexpr uint32_t log2_floor(uint32_t x) {
    uint32_t res = -1u;
    assert(x!=0);
    while (x > 0) {
        res++;
        x >>= 1;
    }
    return res;
}

inline constexpr uint32_t log2_ceil(uint32_t x){
    uint32_t res = -1u;
    uint32_t is_exact_power_of_2 = 1;
    assert(x!=0);
    while (x > 0) {
        if( x&1  && x!=1){
            is_exact_power_of_2=0;
        }
        res++;
        x >>= 1;
    }
    return res + (is_exact_power_of_2?0:1);
}

inline constexpr uint32_t alignup(uint32_t x, uint32_t align){
    //static_assert(is_power_of_2(align));
    return (x+align-1) & (~(align-1));
}


std::string ToHexString(const void* pData, size_t byteLen){
    int i=0;
    const BYTE* pByte = (const BYTE*)pData;
    std::string strBuf;
    for(i=0; pByte != pByte+byteLen; ++pByte, i+=2){
        std::array<char,3> buf={0};
        fmt::format_to(buf.data(), "{:02x}", *pByte);
        strBuf.append(buf.data(), 2);
    }
    return strBuf;
}



static inline constexpr const char* MASK_TO_STR(uint32_t mask){
    switch(mask){
        case 7: return "FREE";
        case 3: return "ALLOC";
        case 1: return "DELETE";
        default: return "BAD";
    };
}

static inline constexpr const char* TYPE_TO_STR(uint32_t type){
    switch(type){
        case 3: return "FILE";
        case 2: return "DIR";
        case 1: return "SYS";
        case 0: return "XIP";
        default: return "error! Invalid type value";
    }
}


static inline bool f3s_head_check(const f3s_head_t* pHead){
    return ((pHead->status[0].__0x0000__ & 0xFFFF0000)==0) && 
        pHead->status[0] == pHead->status[1] &&
         pHead->status[1] == pHead->status[2] &&
         pHead->reserve == 0xFF;
}

static inline constexpr const char* f3s_ext_type_to_str(uint32_t t){
    switch(t){
        case F3S_EXT_FILE:return"FILE";
        case F3S_EXT_DIR:return "DIR";
        case F3S_EXT_SYS:return "SYS";
        case F3S_EXT_XIP:return "XIP";
        default:return "invalid f3s_ext_type";
    }
}

static inline long f3s_head_get_next_logi(const f3s_head_t* head){
    if(head->next.logi_unit==0xFFFF){
        return -1;
    }else{
        return head->next.logi_unit;
    }
}

#endif // __utils_h__