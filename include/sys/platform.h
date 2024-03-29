/*
 * $QNXLicenseC:
 * Copyright 2007, 2009, QNX Software Systems. All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software.   Free development
 * licenses are available for evaluation and non-commercial purposes.  For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others.  Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 * 
 *  Original Path  \home\miki\qnx700\target\qnx7\usr\include\sys
 */

/*
 *  The only symbols made visible by this header are
 *  OS/compiler reserved symbols.
 */

#ifndef __PLATFORM_H_INCLUDED
#define __PLATFORM_H_INCLUDED

#define _NTO_HDR_PIECE_(x) x


/*<STAN_MACRO1,STAN_MACRO5>*/
#if defined(__X86__)
    #define _NTO_CPU_HDR_DIR_(h)   x86/h
#elif defined(__PPC__)
    #define _NTO_CPU_HDR_DIR_(h)   ppc/h
#elif defined(__MIPS__)
    #define _NTO_CPU_HDR_DIR_(h)   mips/h
#elif defined(__SH__)
    #define _NTO_CPU_HDR_DIR_(h)   sh/h
#elif defined(__ARM__)
    #define _NTO_CPU_HDR_DIR_(h)   arm/h
#elif defined(__X86_64__)
	#define _NTO_CPU_HDR_DIR_(h)   x86_64/h
#elif defined(__aarch64__)
	#define _NTO_CPU_HDR_DIR_(h)   aarch64/h

/* New CPU types go here */

#elif defined(__QNXNTO__)
    #error not configured for CPU
#else
    /* partial support only - cross hosted development targeting qnx6 */
    #define _NTO_CPU_HDR_DIR_(h)   unknown_cpu/h
#endif
/*</STAN_MACRO1,STAN_MACRO5>*/

#ifndef _NTO_HDR_
    #ifdef _NTO_HDR_DIR_
        #define _NTO_HDR_(hdr)  <_NTO_HDR_PIECE_(_NTO_HDR_DIR_)_NTO_HDR_PIECE_(hdr)>
    #else
        #define _NTO_HDR_(hdr)  <hdr>
    #endif
#endif

#ifndef _NTO_CPU_HDR_
    #ifdef _NTO_HDR_DIR_
		#define _NTO_CPU_HDR_(hdr)  <_NTO_HDR_PIECE_(_NTO_HDR_DIR_)_NTO_CPU_HDR_DIR_(hdr)>
    #else
		#define _NTO_CPU_HDR_(hdr)  <_NTO_CPU_HDR_DIR_(hdr)>
    #endif
#endif

#if defined(__MWERKS__)
    #include _NTO_HDR_(sys/compiler_mwerks.h)
#elif defined(__GNUC__)
    #include _NTO_HDR_(sys/compiler_gnu.h)
#elif defined(__INTEL_COMPILER)
    #include _NTO_HDR_(sys/compiler_intel.h)
#else
    #error not configured for compiler
#endif

#if !defined(__LINT)
	#define __LINT(__expr)
#endif

#if !defined(__FLEXARY)
	#define __FLEXARY(__type, __name) __type __name[1]
#endif

#if defined(__QNXNTO__)
    #include _NTO_CPU_HDR_(platform.h)
#else
    /* partial support only - cross hosted development targeting qnx6 */
#endif

#if __INT_BITS__==16 && __LONG_BITS__==32 && __PTR_BITS__==32
#define __LP32__ 1
#define __LP32 1
#define _LP32 1
#error Unsupported data model
#elif __INT_BITS__==32 && __LONG_BITS__==32 && __PTR_BITS__==32
#define __ILP32__ 1
#define __ILP32 1
#define _ILP32 1
#elif __INT_BITS__==32 && __LONG_BITS__==32 && __PTR_BITS__==64
#define __LLP64__ 1
#define __LLP64 1
#define _LLP64 1
#elif __INT_BITS__==64 && __LONG_BITS__==64 && __PTR_BITS__==64
#define __ILP64__ 1
#define __ILP64 1
#define _ILP64 1
#error Unsupported data model
#elif __INT_BITS__==32 && __LONG_BITS__==64 && __PTR_BITS__==64
/* Symbols __LP64__ and _LP64 are defined to be 1 by gcc internally */
#else
#warning Uncommon model for int, long, pointer sizes
#endif

/*
 * PPC numbers its bits backwards from the rest of the known universe
 * (except for the 370, of course :-).
 */
#define _BITFIELD64B(__start_bit,__value)	((_Uint64t)(__value) << (63-(__start_bit)))
#define _BITFIELD32B(__start_bit,__value)	((__value) << (31-(__start_bit)))
#define _BITFIELD16B(__start_bit,__value)	((__value) << (15-(__start_bit)))
#define _BITFIELD8B(__start_bit,__value)	((__value) << ( 7-(__start_bit)))

#define _BITFIELD64L(__start_bit,__value)	((_Uint64t)(__value) << (__start_bit))
#define _BITFIELD32L(__start_bit,__value)	((__value) << (__start_bit))
#define _BITFIELD16L(__start_bit,__value)	((__value) << (__start_bit))
#define _BITFIELD8L(__start_bit,__value)	((__value) << (__start_bit))

#define	_ONEBIT64B(__start_bit)				_BITFIELD64B(__start_bit,1U)
#define	_ONEBIT32B(__start_bit)				_BITFIELD32B(__start_bit,1U)
#define	_ONEBIT16B(__start_bit)				_BITFIELD16B(__start_bit,1U)
#define	_ONEBIT8B(__start_bit)				_BITFIELD8B(__start_bit,1U)

#define	_ONEBIT64L(__start_bit)				_BITFIELD64L(__start_bit,1U)
#define	_ONEBIT32L(__start_bit)				_BITFIELD32L(__start_bit,1U)
#define	_ONEBIT16L(__start_bit)				_BITFIELD16L(__start_bit,1U)
#define	_ONEBIT8L(__start_bit)				_BITFIELD8L(__start_bit,1U)

#undef __EXT_ANSIC_199012	/* Additional ANSI C bindings */
#undef __EXT_ANSIC_199409	/* Ansi C Amendment 1 bindings */
#undef __EXT_ANSIC_199901	/* Ansi C99 bindings */
#undef __EXT_ANSIC_201112      /* Ansi C11 bindings */
#undef __EXT_MATH_199001	/* IEEE 754-1985(R1990) IEEE std for Binary Floating-Point Arithmetic */
#undef __EXT_POSIX1_198808	/* Posix 1003.1 */
#undef __EXT_POSIX1_199009	/* Posix 1003.1 */
#undef __EXT_POSIX1_199309	/* Posix 1003.1b, XOPEN_RT */
#undef __EXT_POSIX1_199506	/* Posix 1003.1c, XOPEN_RTT */
#undef __EXT_POSIX1_200112	/* Posix 1003.1-2001 */
#undef __EXT_POSIX2			/* Posix 1003.2 */
#undef __EXT_QNX			/* QNX extentions */
#undef __EXT_PCDOS			/* Common DOS/Watcom Extentions */
#undef __EXT_XOPEN_EX		/* XOPEN extentions */
#undef __EXT_UNIX_MISC		/* UNIX definitions not defined by XOPEN */
#undef __EXT_UNIX_HIST		/* incompatible historic unix definitions */
#undef __EXT_BSD			/* BSD extensions */
#undef __EXT_LF64SRC		/* expose func64() and type64_t in header file namespace */
#undef __EXT_FUNCALIAS64	/* standard func() will be an alias to func64() (acts like #define func func64) */
#undef __EXT_LF64ALIAS		/* func64() will be an alias to standard func() (acts like #define func64 func) */

#if defined(_XOPEN_SOURCE)
	#if _XOPEN_SOURCE-0 == 500
		#if _POSIX_C_SOURCE-0 > 199506
			#error This POSIX_C_SOURCE is unsuported with XOPEN_SOURCE
		#endif
	#elif _XOPEN_SOURCE-0 == 600
		#if _POSIX_C_SOURCE-0 > 200112
			#error This POSIX_C_SOURCE is unsuported with XOPEN_SOURCE
		#endif
	#elif _XOPEN_SOURCE-0 == 700
		#if _POSIX_C_SOURCE-0 > 200809L
			#error This POSIX_C_SOURCE is unsuported with XOPEN_SOURCE
		#endif
	#else
		#error This version of XOPEN_SOURCE is not supported
	#endif
#endif

#if __STDC__-0 > 0 || defined(_QNX_SOURCE) || defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE) || (_POSIX_C_SOURCE+0 > 0) || defined(__EXT)
#define __EXT_ANSIC_199012
#endif

#if __STDC_VERSION__-0 >= 199409
#define __EXT_ANSIC_199409
#endif

#if __STDC_VERSION__-0 >= 199901  || __cplusplus-0 >= 201103L
#define __EXT_ANSIC_199901			/* C99 */

#ifndef __STDC_HOSTED__				/* Hosted implementation */
#define __STDC_HOSTED__		1
#endif

#ifndef __STDC_ISO_10646__			/* ISO/IEC 10646 characters for wchar_t */
#define __STDC_ISO_10646__	200009L	/* this matches glibc, previous was 199712L */
#endif

#ifndef __STDC_IEC_559__			/* IEC 60559 floating-point arithmetic */
#define __STDC_IEC_559__	1
#endif

#ifndef __STDC_IEC_559_COMPLEX__	/* IEC 60559 compatible complex arithmetic */
#define __STDC_IEC_559_COMPLEX__	1
#endif

#endif

#if __STDC_VERSION__-0 >= 201112
#define __EXT_ANSIC_201112
#endif

#if defined(_QNX_SOURCE) || defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE) || (_POSIX_C_SOURCE+0 > 0) || defined(__EXT)
#define __EXT_POSIX1_198808
#define __EXT_POSIX1_199009
#endif

#if defined(_QNX_SOURCE) || defined(_XOPEN_SOURCE) || (_POSIX_C_SOURCE+0 >= 199309) || (defined(__EXT) && !defined(_POSIX_SOURCE) && (_POSIX_C_SOURCE+0 <= 0))
#define __EXT_POSIX1_199309
#endif

#if defined(_QNX_SOURCE) || defined(_XOPEN_SOURCE) || (_POSIX_C_SOURCE+0 >= 199506) || (defined(__EXT) && !defined(_POSIX_SOURCE) && (_POSIX_C_SOURCE+0 <= 0))
#define __EXT_POSIX1_199506
#endif

#if defined(_QNX_SOURCE) || (_XOPEN_SOURCE+0 >= 600) || (_POSIX_C_SOURCE+0 >= 200112) || (defined(__EXT) && !defined(_POSIX_SOURCE) && (_POSIX_C_SOURCE+0 <= 0) && !defined(_XOPEN_SOURCE))
#define __EXT_POSIX1_200112
#endif

#if defined(_QNX_SOURCE) || (_XOPEN_SOURCE+0 >= 700) || (_POSIX_C_SOURCE+0 >= 200809L) || (defined(__EXT) && !defined(_POSIX_SOURCE) && (_POSIX_C_SOURCE+0 <= 0) && !defined(_XOPEN_SOURCE))
#define __EXT_POSIX1_200809
#endif

#if defined(_QNX_SOURCE) || defined(_XOPEN_SOURCE) || (_POSIX_C_SOURCE+0 == 2) || (_POSIX_C_SOURCE+0 >= 199506) || (defined(__EXT) && !defined(_POSIX_SOURCE) && (_POSIX_C_SOURCE+0 <= 0))
#define __EXT_POSIX2
#endif

#if defined(_QNX_SOURCE) || \
    (defined(__EXT) && !defined(__NO_EXT_PCDOS) && !defined(_POSIX_SOURCE) && !defined(_XOPEN_SOURCE) && (_POSIX_C_SOURCE+0 <= 0))
#define __EXT_PCDOS
#endif

#if defined(_QNX_SOURCE) || \
    (defined(__EXT) && !defined(__NO_EXT_QNX) && !defined(_POSIX_SOURCE) && !defined(_XOPEN_SOURCE) && (_POSIX_C_SOURCE+0 <= 0))
#define __EXT_QNX
#endif

#if defined(_QNX_SOURCE) || (defined(__EXT) && !defined(_POSIX_SOURCE) && !defined(_XOPEN_SOURCE) && (_POSIX_C_SOURCE+0 <= 0))
#define __EXT_BSD
#endif

#if defined(_QNX_SOURCE) || defined(_XOPEN_SOURCE) || (defined(__EXT) && !defined(_POSIX_SOURCE) && (_POSIX_C_SOURCE+0 <= 0))
#define __EXT_XOPEN_EX
#endif

#if defined(_QNX_SOURCE) || (_XOPEN_SOURCE+0 == 500) || (defined(__EXT) && !defined(_POSIX_SOURCE) && (_POSIX_C_SOURCE+0 <= 0))
#define __EXT_UNIX_MISC
#endif

#if defined(_QNX_SOURCE) || defined(_XOPEN_SOURCE) || (defined(__EXT) && !defined(_POSIX_SOURCE) && (_POSIX_C_SOURCE+0 <= 0))
#define __EXT_MATH_199001
#endif

#if defined(__UNIX_SOURCE)
#define __EXT_UNIX_HIST
#endif


#ifdef __ALIAS_ATTRIBUTE
#define __ALIASATTR(__n)	__ALIAS_ATTRIBUTE(__n)
#else
#define __ALIASATTR(__n)
#endif

/*
  As part of the transition to a 64-bit world,
  set the value of __OFF_BITS__ based on the input definitions
  For now, the default will remain 32 bits as in 6.5
*/

#if _FILE_OFFSET_BITS+0 == 32 || (!defined(_FILE_OFFSET_BITS) && (__LONG_BITS__ < 64))
#define __OFF_BITS__	32
#elif _FILE_OFFSET_BITS+0 == 64 || (!defined(_FILE_OFFSET_BITS))
#define __OFF_BITS__	64
#else
#error _FILE_OFFSET_BITS value is unsupported
#endif
#if __OFF_BITS__ < __LONG_BITS__
#error _FILE_OFFSET_BITS set too small
#endif


/*
  This causes _LARGEFILE64_SOURCE to be defined all the time.
  This will be removed as soon as all the lib/c source files are modified appropriately
*/
#if !defined(_LARGEFILE64_SOURCE) && (defined(_QNX_SOURCE) || defined(__EXT)) && (!defined(_FILE_OFFSET_BITS) || _FILE_OFFSET_BITS == 32) && !defined(_POSIX_SOURCE) && (_POSIX_C_SOURCE+0 <= 0)
#define _LARGEFILE64_SOURCE 1
#endif

/*

lib/c truth table

These are the 'inputs', to be defined in source or on the command line:
  __LONG_BITS__         32   32   32   32   32   32   32   32   64   64   64
  _LARGEFILE64_SOURCE   n|0  n|0  n|0  n|0  1    1    1    1    n|0  1    ?
  _FILE_OFFSET_BITS     n    n    32   64   n    n    32   64   !32  !32  32

  [0|1|32|64] means set to that numeric value, 'n' means not defined

These are the 'outputs', to be used internally for lib/c feature testing:
  __OFF_BITS__          32   32   32   64   32   32   32   64   64   64   ERR
  __EXT_LF64SRC         N*   N    N    N    Y    Y    Y    Y    N    Y    ERR
  __EXT_FUNCALIAS64     Y*   N    N    Y    Y*   N    N    Y    N    N    ERR
  __EXT_LF64ALIAS       N    N    N    N    N    N    N    N    Y    Y    ERR

  Y means set, N means not set

*   QNX6.5 and earlier defaulted to 32-bit offsets and exposed func64() functions.

In system headers:
__EXT_LF64SRC      expose func64() and type64_t in header file namespace
__EXT_FUNCALIAS64  standard func() will be an alias to func64() (acts like #define func func64 using __ALIAS64)
__EXT_LF64ALIAS    func64() will be an alias to standard func() (acts like #define func64 func using __ALIASOFF)

In library functions (usually #define _LARGEFILE64_SOURCE before including anything)
    if __OFF_BITS__ == 32 you need a 32-bit func() and 64-bit func64()
    else only func() is needed and it is always 64-bit

*/

#if __OFF_BITS__ != __LONG_BITS__
#define __EXT_FUNCALIAS64
#define __ALIAS64(__n)	__ALIASATTR(__n)
#else
#define __ALIAS64(__n)
#endif

#if __LONG_BITS__ == 64
#define __EXT_LF64ALIAS
#define __ALIASOFF(__n)	__ALIASATTR(__n)
#else
#define __ALIASOFF(__n)
#endif

#if _LARGEFILE64_SOURCE+0 || (defined(__EXT_ALIAS64) && !defined(__ALIAS_ATTRIBUTE))
#define __EXT_LF64SRC
#endif


#include _NTO_HDR_(_pack64.h)

#if __INT_BITS__ == 32
#define _INT32		int
#define _UINT32		unsigned
#else
#define _INT32		_Int32t
#define _UINT32		_Uint32t
#endif

#undef __INTPTR_T
#define __INTPTR_T		_Intptrt

#undef __PTRDIFF_T
#define __PTRDIFF_T		_Ptrdifft

#undef __SIZE_T
#define __SIZE_T		_Sizet

#undef __RSIZE_T
#define __RSIZE_T		_Sizet

#undef __SSIZE_T
#define __SSIZE_T		_Ssizet

#undef __ERRNO_T
#define __ERRNO_T		int

#undef __MAX_ALIGN_T
#define __MAX_ALIGN_T		_Maxalignt /*</STAN_MACRO1>*/

#define _LLONG_MAX      0x7fffffffffffffffLL
#define _ULLONG_MAX     0xffffffffffffffffULL


typedef _Uint64t				_Uintptr64t;
typedef _Uint32t				_Uintptr32t;
typedef _Int64t					_Intptr64t;
typedef _Int32t					_Intptr32t;

#if defined(__QNXNTO__)
	#include _NTO_HDR_(sys/target_nto.h)
#elif defined(__QNX__)
	#include _NTO_HDR_(sys/target_qnx.h)
#elif defined(__SOLARIS__) || defined(__NT__) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__LINUX__) || defined(__APPLE__)
	/* partial support only, solaris/win32/linux/darwin hosted targetting qnx6 */
#else
	#error not configured for target
#endif


#ifdef __WCHAR_T
#undef  __WCHAR_T
#define __WCHAR_T		wchar_t
#undef __WCHAR_T
#if !defined(__cplusplus)
#define __WCHAR_T		_Wchart
#endif
#endif

#ifdef __WINT_T
typedef __WINT_T		_Wintt;
#undef __WINT_T
#define __WINT_T		_Wintt
#endif

#ifdef __MBSTATE_T
typedef __MBSTATE_T		_Mbstatet;
#undef __MBSTATE_T
#define __MBSTATE_T		_Mbstatet
#endif

#ifdef __OFF_T
typedef __OFF_T			_Offt;
#undef __OFF_T
#define __OFF_T			_Offt
#endif

#ifdef __OFF64_T
typedef __OFF64_T		_Off64t;
#undef __OFF64_T
#define __OFF64_T		_Off64t
#endif

/* _Fpost must be after _Offt and _Off64t */
#ifdef __FPOS_T
typedef __FPOS_T		_Fpost;
#undef __FPOS_T
#define __FPOS_T		_Fpost
#endif

#ifdef __EXT_QNX	/* Compatible with old headers */
#ifndef __INTEL_COMPILER
typedef _Int8t					_int8;
typedef _Int16t					_int16;
typedef _Int32t					_int32;
typedef _Int64t					_int64;
#endif /* __INTEL_COMPILER */
typedef _Intptrt				_intptr;
typedef _Uint8t					_uint8;
typedef _Uint16t				_uint16;
typedef _Uint32t				_uint32;
typedef _Uint64t				_uint64;
typedef _Uintptrt				_uintptr;
#endif

typedef _Uint64t				_Paddr64t;
typedef _Uint32t				_Paddr32t;

#undef __PADDR_BITS__
#if (!defined(_PADDR_BITS) && __PTR_BITS__ > 32) || _PADDR_BITS - 0 == 64
typedef _Paddr64t				_Paddrt;
#define __PADDR_BITS__			64
#elif !defined(_PADDR_BITS) || _PADDR_BITS - 0 == 32
typedef _Paddr32t				_Paddrt;
#define __PADDR_BITS__			32
#else
#error _PADDR_BITS value is unsupported
#endif
#if __PADDR_BITS__ < __PTR_BITS__
#error PADDR_BITS must be at least the size of a pointer
#endif

#define __CLOCKADJUST	\
		{	\
            _Uint32t        tick_count; \
            _Int32t         tick_nsec_inc;  \
		}

#define __ITIMER \
		{	\
			_Uint64t					nsec;	\
			_Uint64t					interval_nsec;	\
		}

/* <STAN_MACRO1/> */
#define __TIMER_INFO_PREFIX  		\
		struct _itimer		itime;	\
		struct _itimer		otime;	\
		_Uint32t			flags;	\
		_Int32t				tid;	\
		_Int32t				notify;	\
		clockid_t			clockid;\
		_Uint32t			overruns;	

#define __TIMER_INFO32						\
		{									\
			__TIMER_INFO_PREFIX				\
			union {							\
				struct __sigevent32 event32;\
				struct __sigevent32 event;	\
			};								\
		}

#define __TIMER_INFO						\
		{									\
			__TIMER_INFO_PREFIX				\
			union {							\
				struct __sigevent32 event32;\
				struct sigevent 	event;	\
			};								\
		}

#define __TIMER_INFO64						\
		{									\
			__TIMER_INFO_PREFIX				\
			union {							\
				struct __sigevent32 event32;\
				struct sigevent 	event;	\
				struct __sigevent64 event64;\
			};								\
		}


/* <STAN_MACRO1/> */
#define __SA_ACTION_FUNC_T	\
		struct _siginfo;	\
		typedef void __sa_action_func_t(int, struct _siginfo *, void *); \

#include _NTO_HDR_(_packpop.h)

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.0.0/trunk/lib/c/public/sys/platform.h $ $Rev: 806926 $")
#endif
