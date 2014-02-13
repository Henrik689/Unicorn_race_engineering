/**
* @file bitwise.h
* @brief
*	Provides some abstraction for
*	some bitwise operations.
*	Useage should help code readability.
*/

#ifndef BITWISE_H
#define BITWISE_H

/** @name Bit operations
* Does operation on the specified bit
* where a=target, b=bit number to act on
* @{
*/
#define BIT_SET(a,b) 		( (a) |= (1<<(b))	) 
#define BIT_CLEAR(a,b) 		( (a) &= ~(1<<(b))	)
#define BIT_FLIP(a,b) 		( (a) ^= (1<<(b))	)
#define BIT_CHECK(a,b) 		( (a) & (1<<(b))	)
/**@}*/

/** @name Bit mask operations
* Does operation on the specified target
* according to the mask where
* x=target, y=mask
* @{
*/
#define BITMASK_SET(x,y) 	( (x) |= (y)	)
#define BITMASK_CLEAR(x,y) 	( (x) &= (~(y))	)
#define BITMASK_FLIP(x,y) 	( (x) ^= (y)	)
#define BITMASK_CHECK(x,y) 	( (x) & (y)		)

/**
* @brief
*	Conditionally set or clear bit without branching
*
* @param[out] w
*	The word to modify:  if (f) w |= m; else w &= ~m;
* @param[in] m
*	The bit mask
* @param[in] f
*	Conditional flag
*/
#define BITMASK_SET_OR_CLEAR(w, m, f) ( (w) ^= (-(f) ^ (w)) & (m) )
/**@}*/

#define IS_POW2(x) 				( (x) && !((x) & ((x) - 1))	) //!< Check if x is a power of 2
#define HAS_OPPSITE_SIGN(x, y)	( (((x) ^ (y)) < 0) ) //!< check if x and y has opposite sign

#define LOW_BYTE(w) 		( (uint8_t) ((w) & 0xFF)		) //!< Extracts the low-order (rightmost) byte of a variable.
#define HIGH_BYTE(w) 		( (uint8_t) ((w) >> 8)			) //!< Extracts the high-order (leftmost) byte of a word (or the second lowest byte of a larger data type).
#define MERGE_BYTE(h, l) 	( (uint16_t)(((h) << 8) | (l)) 	) //!< Merges two 8 bit bytes into one 16 bit byte where h is the High byte and l is the Low byte

#endif /* BITWISE_H */
