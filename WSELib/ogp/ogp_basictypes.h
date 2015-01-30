#ifndef __OGP_BASICTYPES_H__
#define __OGP_BASICTYPES_H__

#ifdef _MSC_VER

typedef unsigned char ogp_uint8_t;
typedef unsigned short ogp_uint16_t;
typedef unsigned long ogp_uint32_t;
typedef unsigned __int64 ogp_uint64_t;

typedef signed char ogp_int8_t;
typedef signed short ogp_int16_t;
typedef signed long ogp_int32_t;
typedef signed __int64 ogp_int64_t;

#else

#include <stdint.h>

typedef uint8_t ogp_uint8_t;
typedef uint16_t ogp_uint16_t;
typedef uint32_t ogp_uint32_t;
typedef uint64_t ogp_uint64_t;

typedef int8_t ogp_int8_t;
typedef int16_t ogp_int16_t;
typedef int32_t ogp_int32_t;
typedef int64_t ogp_int64_t;

#endif

#endif
