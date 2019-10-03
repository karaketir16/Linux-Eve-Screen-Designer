/*
 * panl_util.h
 *
 *  Created on: Sep 8, 2017
 *      Author: govind.mukundan
 */

#ifndef INCLUDES_UTILS_PANL_UTIL_H_
#define INCLUDES_UTILS_PANL_UTIL_H_

/************************************************************************/
/* Some Useful MACROS                                                   */
/************************************************************************/

// align pointer to x bytes boundary
#define align(ptr, bytes) \
((typeof(ptr))(((uintptr_t)(ptr) + (bytes)-1) & ~((bytes)-1)))


// check is pointer is memory aligned to x bytes
#define isaligned(ptr, bytes) \
(((uintptr_t)(ptr) & ((bytes)-1)) == 0)

// size of variable without sizeof operator
// size_t is unsigned int
#define size(var) \
((size_t)(&(var)+1) - (size_t)(&(var)))

// size of type without sizeof operator
#define sizetype(type) \
((size_t)(((type *)0) + 1)

// find if integer is power of 2
// power of two means only one bit is set. So if we zero it out, we get 0
#define power2(x) \
(x != 0 && (x & (x-1) == 0)))

/*
#define max(a,b) \
({ typeof (a) _a = (a); \
	typeof (b) _b = (b); \
_a > _b ? _a : _b; })
*/

#define INT_MAX         ((int)(~0U>>1))
#define INT_MIN         (-INT_MAX - 1)
#define UINT_MAX        (~0U)
#define LONG_MAX        ((long)(~0UL>>1))
#define LONG_MIN        (-LONG_MAX - 1)
#define ULONG_MAX       (~0UL)

#define IS_ALIGNED(x, a)            (((x) & ((typeof(x))(a) - 1)) == 0)
#define ARRAY_SIZE(arr)				(sizeof(arr) / sizeof((arr)[0]))
// Find the sizeof struct element
#define Z_SIZEOF(type, member)          sizeof(((type *)0)->member)

#define roundup(x, y) (                                 \
{                                                       \
	const typeof(y) __y = y;							\
	(((x) + (__y - 1)) / __y) * __y;					\
}                                                       \
)
#define rounddown(x, y) (                               \
{                                                       \
	typeof(x) __x = (x);								\
	__x - (__x % (y));									\
}                                                       \
)


// Given a Little Endian byte stream, convert to:
#define Z_MAKEU32(p)            ((uint32_t)(*((uint8_t*)p+0) << 0) | (uint32_t)(*((uint8_t*)p+1) << 8) | (uint32_t)(*((uint8_t*)p+2) << 16) | (uint32_t)(*((uint8_t*)p+3) << 24))
#define Z_MAKEU16(p)            ((uint32_t)(*((uint8_t*)p+0) << 0) | (uint32_t)(*((uint8_t*)p+1) << 8))

#define Z_BitClear(val, mask)			((val) = (val) & (~(mask)))
#define Z_BitSet(val, mask)				((val) = (val) | (mask))
#define Z_BitIsSet(val, mask)           (((val) & (mask)) == (mask))
#define Z_BitIsClear(val, mask) 		(!(((val) & (mask)) == (mask)))

/************************************************************************/
/* Some useful GCC attributes                                          */
/************************************************************************/

#endif /* INCLUDES_UTILS_PANL_UTIL_H_ */
