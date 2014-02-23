/**
 * @file ringbuffer.h
 * @brief
 * 	A simple ring buffer of size RB_BUFFER_SIZE.
 * 	The buffer size is determined at compile time
 * 	on a per module basis. That is a.c can use a
 * 	buffer of size 32 while b.c can use a buffer
 * 	size of 64 but c.c cannot have both a 32 and 64
 * 	length buffer. The buffer size can be altered
 * 	by defining RB_BUFFER_SIZE as the desired buffer
 * 	size before including ringbuffer.h
 * 	The default value for RB_BUFFER_SIZE is 32
 *
 * 	The datatype of the buffer is uint8_t by default
 * 	but this can also be overridden by defining RB_DATA_t
 * 	to another type before including ringbuffer.h
 *
 * 	The buffer size is recommended to a power of 2
 * 	value as this allows for faster calculating the
 * 	next index values. However non power of 2 values
 * 	can be used.
 */

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>
#include <string.h> // memset()

#ifndef RB_BUFFER_SIZE
	#define RB_BUFFER_SIZE 32
#endif

#define RB_BUFFER_MASK	(RB_BUFFER_SIZE-1)

#if RB_BUFFER_SIZE < (1<<8)
 	//#define rb_index_t	uint8_t
 	typedef uint8_t rb_index_t;
#elif RB_BUFFER_SIZE < (1<<16)
 	typedef uint16_t rb_index_t;
#elif RB_BUFFER_SIZE < (1<<32)
 	typedef uint32_t rb_index_t;
#else
 	#error RB_BUFFER_SIZE is too big
#endif

// Determine if the buffer size is a power of 2
#if RB_BUFFER_SIZE  == (1<<2)
	#define RB_BUFFER_SIZE_IS_POW2
#elif RB_BUFFER_SIZE  == (1<<3)
	#define RB_BUFFER_SIZE_IS_POW2
#elif RB_BUFFER_SIZE  == (1<<4)
	#define RB_BUFFER_SIZE_IS_POW2
#elif RB_BUFFER_SIZE  == (1<<5)
	#define RB_BUFFER_SIZE_IS_POW2
#elif RB_BUFFER_SIZE  == (1<<6)
	#define RB_BUFFER_SIZE_IS_POW2
#elif RB_BUFFER_SIZE  == (1<<7)
	#define RB_BUFFER_SIZE_IS_POW2
#elif RB_BUFFER_SIZE  == (1<<8)
	#define RB_BUFFER_SIZE_IS_POW2
#elif RB_BUFFER_SIZE  == (1<<9)
	#define RB_BUFFER_SIZE_IS_POW2
#elif RB_BUFFER_SIZE  == (1<<10)
	#define RB_BUFFER_SIZE_IS_POW2
#elif RB_BUFFER_SIZE  == (1<<11)
	#define RB_BUFFER_SIZE_IS_POW2
#elif RB_BUFFER_SIZE  == (1<<12)
	#define RB_BUFFER_SIZE_IS_POW2
#elif RB_BUFFER_SIZE  == (1<<13)
	#define RB_BUFFER_SIZE_IS_POW2
#elif RB_BUFFER_SIZE  == (1<<14)
	#define RB_BUFFER_SIZE_IS_POW2
#elif RB_BUFFER_SIZE  == (1<<15)
	#define RB_BUFFER_SIZE_IS_POW2
#elif RB_BUFFER_SIZE  == (1<<16)
	#define RB_BUFFER_SIZE_IS_POW2
#endif


#ifndef RB_DATA_t
#define RB_DATA_t uint8_t
#endif


typedef struct ringbuffer_t{
	RB_DATA_t buffer[RB_BUFFER_SIZE]; //!< The actual buffer
	rb_index_t start; //!< Index of the start of valid data in the buffer
	rb_index_t end; //!< Index of the end of valid data in the buffer
} ringbuffer_t;


#ifdef RB_BUFFER_SIZE_IS_POW2
	#define rb_nextStart(B)		(((B)->start+1) & RB_BUFFER_MASK)
	#define rb_nextEnd(B)		(((B)->end+1) & RB_BUFFER_MASK)
#else
	#define rb_nextStart(B)		(((B)->start+1) % RB_BUFFER_MASK)
	#define rb_nextEnd(B)		(((B)->end+1) % RB_BUFFER_MASK)
#endif

#define rb_isEmpty(B)			((B)->end == (B)->start) //!< Returns a boolean whether ring buffer is empty
#define rb_isFull(B)			(rb_nextEnd((B)) == (B)->start) //!< Returns a boolean whether ring buffer is full

#define rb_reset(B)				{(B)->end = (B)->start = 0;}

/**
 * Initializes the ring buffer to it's
 * initial values
 * @param buffer The buffer to initialize
 */
static inline void rb_init(ringbuffer_t* const buffer){
	rb_reset(buffer);
	memset(buffer->buffer, 0, sizeof(buffer->buffer));
}

/**
 * Inserts a byte to the ring buffer.
 * If RB_OVERWRITE_ON_PUSH is defined rb_push
 * will overwrite the last value in the buffer.
 * If not defined it will return 1 if buffer is full
 * and not commit the write
 *
 * @param  buffer The ring buffer
 * @param  data   The data point that is added to buffer
 * @return        1 on overflow error and 0 on success
 */
static inline int rb_push(ringbuffer_t *buffer, RB_DATA_t data) {
	rb_index_t nextStart =  rb_nextStart(buffer);

#ifndef RB_OVERWRITE_ON_PUSH
	// if buffer is full. We cant use the macro as we
	// want to cache the nextStart value
	if (nextStart == buffer->end) {
		return 1; // ERROR: buffer overflow
	}
#endif

	// Commit the write
	buffer->start = nextStart; // Set the new index
	buffer->buffer[nextStart] = data;

	return 0; // Success
}

/**
 * Get a byte from ring buffer.
 * @param  buffer The ring buffer
 * @param  data   pointer where returned byte is stored
 * @return        1 if no data is available and 0 on success
 */
static inline int rb_pop(ringbuffer_t *buffer, RB_DATA_t *data) {
	if (rb_isEmpty(buffer)) {
		return 1; // No data available
	}

	rb_index_t nextEnd = rb_nextEnd(buffer);
	buffer->end = nextEnd; // Set the new index

	*data = buffer->buffer[nextEnd];
	return 0; // Success
}

/**
 * Get the next byte in the ring buffer without
 * removing it from the buffer. This will work
 * the same as rb_pop except successive calls
 * to rb_peek will return the same byte.
 * @param  buffer The ring buffer
 * @param  data   pointer where returned byte is stored
 * @return        1 if no data is available and 0 on success
 */
static inline int rb_peek(ringbuffer_t *buffer, RB_DATA_t *data) {
	if (rb_isEmpty(buffer)) {
		return 1; // No data available
	}

	rb_index_t nextEnd = rb_nextEnd(buffer);

	*data = buffer->buffer[nextEnd];
	return 0; // Success
}

#endif /* RINGBUFFER_H */
