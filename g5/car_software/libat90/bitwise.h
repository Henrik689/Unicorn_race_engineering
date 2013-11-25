/**
* @file bitwise.h
* @brief
*	Provides some abstraction for
*	some bitwise operations.
*	Useage should help code readability.
*/

#ifndef BITWISE_H
#define BITWISE_H

/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define BIT_CHECK(var,pos) ((var) & (1<<(pos)))

/* x=target variable, y=mask */
#define BITMASK_SET(x,y) ((x) |= (y))
#define BITMASK_CLEAR(x,y) ((x) &= (~(y)))
#define BITMASK_FLIP(x,y) ((x) ^= (y))
#define BITMASK_CHECK(x,y) ((x) & (y))


#define IS_POW2(x) ((x) && !((x) & ((x) - 1)))

#endif /* BITWISE_H */
