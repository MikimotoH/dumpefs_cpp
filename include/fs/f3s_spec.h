/*
 * $QNXLicenseC:
 * Copyright 2014, QNX Software Systems.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */
#include <string>
#include <string_view>
#include <spdlog/fmt/bundled/compile.h>
using size_t = std::size_t;
/*
** File: f3s_spec.h
**
** Description:
**
** This file contains the structure specification definitions for the f3s
** resource manager for Neutrino
**
** Ident: $Id: f3s_spec.h 772206 2015-03-02 16:16:43Z dgreen@qnx.com $
 Original Path: \home\miki\qnx700\target\qnx7\usr\include\fs\f3s_spec.h
*/
#ifndef __F3S_SPEC_H_V3_INCLUDED
#define __F3S_SPEC_H_V3_INCLUDED

#ifdef __F3S_SPEC_H_INCLUDED
#error Cannot include old and new flash file system headers
#endif

#ifndef __PLATFORM_H_INCLUDED
#include "sys/platform.h"
#endif

/*
** Constant Definitions
*/

/* General Purpose Constants */
const int F3S_HEAD_POW2 = 5;   /* power of two shift for head size */
const int F3S_OFFSET_HI_POW2 = 16;  /* power of two shift for offset hi */
const int F3S_SYS_POW2_DEFAULT = 2;   /* power of two shift for system extents */
const int F3S_SYS_ALIGN_DEFAULT = 4;   /* alignment for system extents */
const int F3S_WS_SIZE_DEFAULT = 0;   /* no white space between data and header area */

/* For Spansion GL-S part, ECC page size is 32 byte 
 *   f3s library supports two ECC mode:
 *   - 64byte alignment: interleave 1 or 2
 *   - 32byte alignment: interleave must be 1.
 */
const int F3S_ECC_MODE_64 = 1;
const int F3S_ECC_MODE_32 = 2;
const int F3S_ECC64_SYS_ALIGN = 64;
const int F3S_ECC64_SYS_POW2 = 6;
const int F3S_ECC64_LOGI_SIZE = 80;  /* ECC page size * max interleave(2) + 16byte MD5 */
const int F3S_ECC32_SYS_ALIGN = 32;
const int F3S_ECC32_SYS_POW2 = 5;
const int F3S_ECC32_LOGI_SIZE = 48;  /* ECC page size + 16byte MD5 */

#if __cplusplus == 202002L 
// Signature Constants
constexpr std::string_view F3S_SIG_STRING = "QSSL_F3S";
std::size_t F3S_SIG_SIZE = F3S_SIG_STRING.size();
// Default Root Constants 
constexpr std::string_view F3S_ROOT_STRING = "";                           // root string
std::size_t F3S_ROOT_SIZE = F3S_ROOT_STRING.size()+1;
#else
char F3S_SIG_STRING[8] = {'Q','S','S','L','_','F','3','S',};
std::size_t F3S_SIG_SIZE = 8;
char F3S_ROOT_STRING[1] = {'\x00'};
std::size_t F3S_ROOT_SIZE = sizeof(F3S_ROOT_STRING);
#endif

/* Revision Constants */
const int F3S_REV_MAJOR = 3;  /* major revision number */
const int F3S_REV_MINOR = 0;  /* minor revision number */


/* Name Definitions */
const size_t F3S_NAME_MAX = 0xff;   /* max name size for dirents */
const size_t F3S_PATH_MAX = 0x400;   /* max path length*/

inline constexpr uint32_t F3S_NAME_ALIGN(uint32_t x){
    return (x+4-1) & (~(4-1));
}

// #define F3S_NAME_ALIGN(LENGTH)\
// \
//   (((LENGTH)+(sizeof(_Uint32t)-1))&~(sizeof(_Uint32t)-1))

// inline constexpr auto operator*(const std::string_view& sv, size_t times) -> std::array<char, sv.length() * times> {
//   std::array<char, sv.length() * times> result;
//   constexpr char* data = result.data();
//   for(int i=0; i<times; ++i){
//     for(int j=0; j<sv.length(); ++j, ++data){
//       *data = sv.data()[j];
//     }
//   }
//   return result;
// }


/* Unit Index Constants */
const int F3S_UNIT_INFO_INDEX = 0;  /* header index of unit info */
const int F3S_UNIT_LOGI_INDEX = 1;  /* header index of unit logical info */
const int F3S_BOOT_INDEX = 2;  /* header index of boot info */
const int F3S_ROOT_INDEX = 3;  /* header index of root directory entry */
const int F3S_FIRST_INDEX = 4;  /* header index of first directory entry */

/* Unit Logical Constants */
const int F3S_FIRST_LOGI = 1;  /* first logical unit */

/* Unit Status Constants */
const int F3S_UNIT_MASK = 0x003f;  /* mask for unit status */
const int F3S_UNIT_ERASE = 0x003f;  /* unit was freshly erased */
const int F3S_UNIT_COUNT = 0x001f;  /* erase count was updated */
const int F3S_UNIT_SPARE = 0x000f;  /* unit is a spare */
const int F3S_UNIT_RECLAIM = 0x0007;  /* unit is in reclaim process */
const int F3S_UNIT_READY = 0x0003;  /* unit is ready for use */
const int F3S_UNIT_QUEUE = 0x0001;  /* unit is queued for erase */
const int F3S_UNIT_RETIRE = 0x0000;  /* unit is retired */
const int F3S_UNIT_NO_LOGI = 0x0040;  /* unit has no logical */
const int F3S_UNIT_NO_ZERO = 0x0080;  /* unit is not pre-programmed */

/* Extent Header Status Constants */
const int F3S_EXT_NO_WRITE = 0x0001;  /* extent text not written yet */
const int F3S_EXT_NO_NEXT = 0x0002;  /* no next extent exists */
const int F3S_EXT_NO_SUPER = 0x0004;  /* no supersede extent exists */
const int F3S_EXT_NO_SPLIT = 0x0008;  /* extent is complete */
const int F3S_EXT_MASK = 0x0070;  /* mask for condition access */
const int F3S_EXT_FREE = 0x0070;  /* extent header is free */
const int F3S_EXT_ALLOC = 0x0030;  /* extent header is allocated */
const int F3S_EXT_DELETE = 0x0010;  /* extent header and text is deleted */ 
const int F3S_EXT_BAD = 0x0000;  /* extent is bad */
const int F3S_EXT_LAST = 0x0080;  /* extent header is last in unit */
const int F3S_EXT_TYPE = 0x0300;  /* mask for type access */
const int F3S_EXT_FILE = 0x0300;  /* extent is part of a file */
const int F3S_EXT_DIR = 0x0200;  /* extent is part of a directory */
const int F3S_EXT_SYS = 0x0100;  /* extent is for file system */
const int F3S_EXT_XIP = 0x0000;  /* extent is xip aligned */
const int F3S_EXT_BASIC = 0x0400;  /* extent is basic in supersede chain */

struct f3s_status_bitfield{
    uint32_t no_write :1;
    uint32_t no_next  :1;
    uint32_t no_super :1;
    uint32_t no_split :1;
    uint32_t MASK     :3;
    uint32_t last     :1;
    uint32_t TYPE     :2;
    uint32_t _0xF_    :4;
    uint32_t basic      :1;
    uint32_t _0x1_      :1;
    uint32_t __0x0000__: 16;
  
  constexpr uint32_t to_value()const{
    return *((uint32_t*)(this));
  }

  constexpr const char* TYPE_to_str()const{
    switch(this->TYPE){
      case 3:return "FILE";
      case 2:return "DIR";
      case 1:return "SYS";
      case 0:return "XIP";
      default:return "invalid TYPE";
    }
  }

  constexpr const char* MASK_to_str()const{
    switch(this->MASK){
      case 7:return "FREE";
      case 3:return "ALLOC";
      case 1:return "DELETE";
      case 0:return "BAD";
      default:return "invalid MASK";
    }
  }

  std::string to_str()const {
    return fmt::format("{}|{}|{}|{}|{}|{}|{}|{}",
        this->no_write ? "NO_WRITE": "WRITE",
        this->no_next  ? "NO_NEXT" : "NEXT",
        this->no_super ? "NO_SUPER": "SUPER",
        this->no_split ? "NO_SPLIT": "SPLIT",
        this->MASK_to_str(),
        this->last     ? "LAST" : "NO_LAST",
        this->TYPE_to_str(),
        this->basic    ?"BASIC" : "NO_BASIC"
        );
  }
};

inline constexpr bool operator==(f3s_status_bitfield lhs, f3s_status_bitfield rhs){
  return lhs.to_value() == rhs.to_value();
}

static_assert(sizeof(f3s_status_bitfield)==4, "must be 4 bytes length");

struct st_mode_t{
    uint16_t other_x   :1;
    uint16_t other_w   :1;
    uint16_t other_r   :1;
    uint16_t group_x   :1;
    uint16_t group_w   :1;
    uint16_t group_r   :1;
    uint16_t user_x    :1;
    uint16_t user_w    :1;
    uint16_t user_r    :1;
    uint16_t sticky    :1;
    uint16_t _nouse_   :2;
    uint16_t fifo      :1;
    uint16_t link      :1; // symbolic link
    uint16_t dir       :1; // Directory
    uint16_t ifreg     :1; // regular file
  constexpr uint16_t to_value()const{
    return *((uint16_t*)this);
  } 
  constexpr bool is_dir()const{
    return this->dir?true:false;
  }
  constexpr bool is_link()const{
    return this->link?true:false;
  }
  constexpr bool is_file()const{
    return this->ifreg?true:false;
  }

  constexpr auto _ftype() -> char{
    if (this->ifreg && this->link)
      return 'l'; // link
    else if (this->dir)
      return 'd'; // directory/folder
    else if (this->ifreg)
      return '-'; // regular file
    else
      return '-';
  }

#if  __cplusplus == 202002L  
  constexpr auto to_str(st_mode_t mode) -> std::array<char, 11>{
    auto result = std::array<char, 11>();
    fmt::format_to(result.data(), FMT_COMPILE("{}{}{}{}{}{}{}{}{}{}"), 
        this->_ftype(),
        this->user_r?'r':'-',   this->user_w?'w':'-',  this->user_x?'x':'-',
        this->group_r?'r':'-',  this->group_w?'w':'-', this->group_x?'x':'-',
        this->other_r?'r':'-',  this->other_w?'w':'-', this->other_x?'x':'-' 
        );
    return result;
  }
#else
  auto to_str(st_mode_t mode) -> std::string{
    return fmt::format("{}{}{}{}{}{}{}{}{}{}", 
        this->_ftype(),
        this->user_r?'r':'-',   this->user_w?'w':'-',  this->user_x?'x':'-',
        this->group_r?'r':'-',  this->group_w?'w':'-', this->group_x?'x':'-',
        this->other_r?'r':'-',  this->other_w?'w':'-', this->other_x?'x':'-' 
        );
  }
#endif // __cplusplus
};
static_assert(sizeof(st_mode_t)==2, "must be 4 bytes length");




/*
** Structure Definitions
*/

/* Extent Pointer Definition */
struct f3s_extptr_t
{
  _Uint16t logi_unit;  /* logical unit number */
  _Uint16t index;      /* header index in unit */  
  constexpr uint32_t to_value()const{
    return *((uint32_t*)this);
  }
  std::string to_str()const{
    return fmt::format("H[{:02x}] L[{:02x}]", this->index, this->logi_unit);
  }
  constexpr bool is_valid()const{
    return this->logi_unit!=0xFFFFu && this->index!=0xFFFu;
  }
};
static_assert(sizeof(f3s_extptr_t)==sizeof(uint32_t));

inline constexpr bool operator==(const f3s_extptr_t& lhs, const f3s_extptr_t& rhs){
  return lhs.logi_unit == rhs.logi_unit && lhs.index == rhs.index;
}
inline constexpr bool operator!=(const f3s_extptr_t& lhs, const f3s_extptr_t& rhs){
  return !(lhs==rhs);
}
inline constexpr bool operator>(const f3s_extptr_t& lhs, const f3s_extptr_t& rhs){
  return lhs.to_value() > rhs.to_value();
}
inline constexpr bool operator<(const f3s_extptr_t& lhs, const f3s_extptr_t& rhs){
  return lhs.to_value() < rhs.to_value();
}

// inline std::strong_ordering operator<=>(const f3s_extptr_t& lhs, const f3s_extptr_t& rhs){
//   return lhs.value <=> rhs.value ;
// }


/* File Stat Info Definition */
typedef struct f3s_stat_s
{
  _Uint16t struct_size;  /* size of this structure */
  st_mode_t mode;         /* mode of file */
  _Uint32t uid;          /* user id of file */
  _Uint32t gid;          /* group id of file */
  _Uint32t mtime;        /* time of last modification */
  _Uint32t ctime;        /* time of last status change */
}
f3s_stat_t;
static_assert(sizeof(f3s_stat_t)==0x14);

/* Directory Entry Info */
typedef struct f3s_dirent_s
{
  _Uint16t     struct_size;  /* size of this structure */
  _Uint8t      moves;        /* number of times file was moved */
  _Uint8t      namelen;      /* directory entry name length */
  f3s_extptr_t first;        /* first extent */
/*_Uint8t      name[];          directory entry name (aligned, max=255) */
/*f3s_stat_t   stat;            directory entry stat info (dword aligned) */
}
f3s_dirent_t;
static_assert(sizeof(f3s_dirent_t)==0x08);


/* Extent Header Info */
typedef struct f3s_head_s
{
  f3s_status_bitfield   status[3];      /* status of header and text */
  union
  {
    _Uint8t      ecc[6];       /* 3 ECC bytes followed by 3 pad bytes */
    _Uint16t     pad[3];
  } mix;
  _Uint8t      reserve;        /* this is reserved and should be 0xff */
  _Uint8t      text_offset_hi; /* high byte of text offset (shift dep. on align) */
  _Uint16t     text_offset_lo; /* low word of text offset (shift dep. on align) */
  _Uint16t     text_size;      /* text size max=65535 */
  f3s_extptr_t next;           /* next header in file or dir */
  f3s_extptr_t super;          /* superseding header for this header */
}
f3s_head_t;
static_assert(sizeof(f3s_head_t)==0x20);

/* Unit Header Info */
typedef struct f3s_unit_info_s
{
  _Uint16t     struct_size;  /* size of this structure */
  _Uint8t      endian;       /* endianness of file system: 'L' or 'B' */
  _Uint8t      pad;          /* pad */
  _Uint16t     unit_pow2;    /* pow2 size of unit */
  _Uint16t     reserve;      /* this is reserved and should be 0xffff */
  _Uint32t     erase_count;  /* erase counter of physical unit */
  f3s_extptr_t boot;         /* boot information pointer */
}
f3s_unit_info_t;
static_assert(sizeof(f3s_unit_info_t)==0x10);

typedef struct f3s_unit_logi_s
{
  _Uint16t     struct_size;  /* size of this structure */
  _Uint16t     logi;         /* logical unit number */
  _Uint32t     age;          /* age of logical unit */
  _Uint32t     md5[4];       /* md5 checksum */
}
f3s_unit_logi_t;
static_assert(sizeof(f3s_unit_logi_t)==0x18);


/* Partition Boot Info */
typedef struct f3s_boot_s
{
  _Uint16t     struct_size;  /* size of this structure */
  _Uint8t      rev_major;    /* major revision number */
  _Uint8t      rev_minor;    /* minor revision number */
  char         sig[8];       /* QSSL_F3S signature */
  _Uint16t     unit_index;   /* physical unit index */
  _Uint16t     unit_total;   /* total number of units */
  _Uint16t     unit_spare;   /* number of spare units */
  _Uint16t     align_pow2;   /* alignment of text for regular power of two */
  f3s_extptr_t root;         /* root directory entry extent pointer */
}
f3s_boot_t;
static_assert(sizeof(f3s_boot_t)==0x18);


#endif /* __F3S_SPEC_H_V3_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.0.0/trunk/lib/fs-flash3/public/fs/f3s_spec.h $ $Rev: 772206 $")
#endif
