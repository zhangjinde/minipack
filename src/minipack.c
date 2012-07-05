// minipack v0.2.0

#include "minipack.h"
#include "string.h"

//==============================================================================
//
// Constants
//
//==============================================================================

//--------------------------------------
// Fixnum
//--------------------------------------

#define POS_FIXNUM_MIN          0
#define POS_FIXNUM_MAX          127
#define POS_FIXNUM_TYPE         0x00
#define POS_FIXNUM_TYPE_MASK    0x80
#define POS_FIXNUM_VALUE_MASK   0x7F
#define POS_FIXNUM_SIZE         1

#define NEG_FIXNUM_MIN          -32
#define NEG_FIXNUM_MAX          -1
#define NEG_FIXNUM_TYPE         0xE0
#define NEG_FIXNUM_TYPE_MASK    0xE0
#define NEG_FIXNUM_VALUE_MASK   0x1F
#define NEG_FIXNUM_SIZE         1


//--------------------------------------
// Unsigned integers
//--------------------------------------

#define UINT8_TYPE              0xCC
#define UINT8_SIZE              2

#define UINT16_TYPE             0xCD
#define UINT16_SIZE             3

#define UINT32_TYPE             0xCE
#define UINT32_SIZE             5

#define UINT64_TYPE             0xCF
#define UINT64_SIZE             9


//--------------------------------------
// Signed integers
//--------------------------------------

#define INT8_TYPE               0xD0
#define INT8_SIZE               2

#define INT16_TYPE              0xD1
#define INT16_SIZE              3

#define INT32_TYPE              0xD2
#define INT32_SIZE              5

#define INT64_TYPE              0xD3
#define INT64_SIZE              9


//--------------------------------------
// Nil
//--------------------------------------

#define NIL_TYPE                0xC0
#define NIL_SIZE                1


//--------------------------------------
// Boolean
//--------------------------------------

#define TRUE_TYPE                0xC3
#define FALSE_TYPE               0xC2
#define BOOL_SIZE                1


//--------------------------------------
// Floating point
//--------------------------------------

#define FLOAT_TYPE              0xCA
#define FLOAT_SIZE              5

#define DOUBLE_TYPE             0xCB
#define DOUBLE_SIZE             9


//--------------------------------------
// Raw bytes
//--------------------------------------

#define FIXRAW_TYPE             0xA0
#define FIXRAW_TYPE_MASK        0xE0
#define FIXRAW_VALUE_MASK       0x1F
#define FIXRAW_SIZE             1
#define FIXRAW_MAXSIZE          31

#define RAW16_TYPE              0xDA
#define RAW16_SIZE              3
#define RAW16_MAXSIZE           65535

#define RAW32_TYPE              0xDB
#define RAW32_SIZE              5
#define RAW32_MAXSIZE           4294967295


//--------------------------------------
// Array
//--------------------------------------

#define FIXARRAY_TYPE           0x90
#define FIXARRAY_TYPE_MASK      0xF0
#define FIXARRAY_VALUE_MASK     0x0F
#define FIXARRAY_SIZE           1
#define FIXARRAY_MAXSIZE        15

#define ARRAY16_TYPE            0xDC
#define ARRAY16_SIZE            3
#define ARRAY16_MAXSIZE         65535

#define ARRAY32_TYPE            0xDD
#define ARRAY32_SIZE            5
#define ARRAY32_MAXSIZE         4294967295


//--------------------------------------
// Map
//--------------------------------------

#define FIXMAP_TYPE             0x80
#define FIXMAP_TYPE_MASK        0xF0
#define FIXMAP_VALUE_MASK       0x0F
#define FIXMAP_SIZE             1
#define FIXMAP_MAXSIZE          15

#define MAP16_TYPE              0xDE
#define MAP16_SIZE              3
#define MAP16_MAXSIZE           65535

#define MAP32_TYPE              0xDF
#define MAP32_SIZE              5
#define MAP32_MAXSIZE           4294967295


//==============================================================================
//
// Byte Order
//
//==============================================================================

#include <sys/types.h>

#ifndef BYTE_ORDER
#if defined(linux) || defined(__linux__)
# include <endian.h>
#else
# include <machine/endian.h>
#endif
#endif

#if !defined(BYTE_ORDER)
#error "Undefined byte order"
#endif

uint64_t bswap64(uint64_t value)
{
    return (
        ((value & 0x00000000000000FF) << 56) |
        ((value & 0x000000000000FF00) << 40) |
        ((value & 0x0000000000FF0000) << 24) |
        ((value & 0x00000000FF000000) << 8) |
        ((value & 0x000000FF00000000) >> 8) |
        ((value & 0x0000FF0000000000) >> 24) |
        ((value & 0x00FF000000000000) >> 40) |
        ((value & 0xFF00000000000000) >> 56)
    );
}

#if (BYTE_ORDER == LITTLE_ENDIAN)
#define htonll(x) bswap64(x)
#define ntohll(x) bswap64(x)
#else
#define htonll(x) x
#define ntohll(x) x
#endif


//==============================================================================
//
// Fixnum
//
//==============================================================================

//--------------------------------------
// Positive Fixnum
//--------------------------------------

// Checks if an element is a positive fixnum.
//
// ptr - A pointer to the element.
//
// Returns true if the element is a positive fixnum, otherwise returns false.
bool minipack_is_pos_fixnum(void *ptr)
{
    return (*((uint8_t*)ptr) & POS_FIXNUM_TYPE_MASK) == POS_FIXNUM_TYPE;
}

// Reads a positive fixnum from a given memory address.
//
// ptr - A pointer to where the fixnum should be read from.
// sz  - A pointer to where the size of the element should be stored.
//
// Returns an unsigned 8-bit integer value for the fixnum.
uint8_t minipack_unpack_pos_fixnum(void *ptr, size_t *sz)
{
    *sz = POS_FIXNUM_SIZE;
    uint8_t value = *((uint8_t*)ptr);
    return value & POS_FIXNUM_VALUE_MASK;
}

// Writes a positive fixnum to a given memory address.
//
// ptr - A pointer to where the fixnum should be written to.
// sz  - A pointer to where the size of the element should be stored.
void minipack_pack_pos_fixnum(void *ptr, uint8_t value, size_t *sz)
{
    *sz = POS_FIXNUM_SIZE;
    *((uint8_t*)ptr) = value & POS_FIXNUM_VALUE_MASK;
}


//--------------------------------------
// Negative Fixnum
//--------------------------------------

// Checks if an element is a negative fixnum.
//
// ptr - A pointer to the element.
//
// Returns true if the element is a negative fixnum, otherwise returns false.
bool minipack_is_neg_fixnum(void *ptr)
{
    return (*((uint8_t*)ptr) & NEG_FIXNUM_TYPE_MASK) == NEG_FIXNUM_TYPE;
}

// Reads a negative fixnum from a given memory address.
//
// ptr - A pointer to where the fixnum should be read from.
// sz  - A pointer to where the size of the element should be stored.
//
// Returns a signed 8-bit integer value for the fixnum.
int8_t minipack_unpack_neg_fixnum(void *ptr, size_t *sz)
{
    *sz = NEG_FIXNUM_SIZE;
    int8_t value = *((int8_t*)ptr) & NEG_FIXNUM_VALUE_MASK;
    return (32-value) * -1;
}

// Writes a negative fixnum from a given memory address.
//
// ptr - A pointer to where the fixnum should be written to.
// sz  - A pointer to where the size of the element should be stored.
void minipack_pack_neg_fixnum(void *ptr, int8_t value, size_t *sz)
{
    *sz = NEG_FIXNUM_SIZE;
    *((int8_t*)ptr) = (32 + value) | NEG_FIXNUM_TYPE;
}



//==============================================================================
//
// Unsigned Integers
//
//==============================================================================

//--------------------------------------
// Unsigned Int
//--------------------------------------

// Retrieves the size, in bytes, of how large an element will be.
//
// value - The value to calculate the size of.
//
// Returns the number of bytes needed for the element.
size_t minipack_sizeof_uint(uint64_t value)
{
    if(value <= POS_FIXNUM_MAX) {
        return POS_FIXNUM_SIZE;
    }
    else if(value <= UINT8_MAX) {
        return UINT8_SIZE;
    }
    else if(value <= UINT16_MAX) {
        return UINT16_SIZE;
    }
    else if(value <= UINT32_MAX) {
        return UINT32_SIZE;
    }
    else if(value <= UINT64_MAX) {
        return UINT64_SIZE;
    }

    return 0;
}

// Reads an unsigned integer from a given memory address.
//
// ptr - A pointer to where the unsigned int should be read from.
// sz  - A pointer to where the size of the element should be stored.
//
// Returns the value of the element.
uint64_t minipack_unpack_uint(void *ptr, size_t *sz)
{
    if(minipack_is_pos_fixnum(ptr)) {
        return (uint64_t)minipack_unpack_pos_fixnum(ptr, sz);
    }
    else if(minipack_is_uint8(ptr)) {
        return (uint64_t)minipack_unpack_uint8(ptr, sz);
    }
    else if(minipack_is_uint16(ptr)) {
        *sz = UINT16_SIZE;
        return (uint64_t)minipack_unpack_uint16(ptr, sz);
    }
    else if(minipack_is_uint32(ptr)) {
        *sz = UINT32_SIZE;
        return (uint64_t)minipack_unpack_uint32(ptr, sz);
    }
    else if(minipack_is_uint64(ptr)) {
        *sz = UINT64_SIZE;
        return minipack_unpack_uint64(ptr, sz);
    }
    else {
        *sz = 0;
        return 0;
    }
}

// Writes an unsigned integer to a given memory address.
//
// ptr   - A pointer to where the integer should be written to.
// value - The value to write.
// sz    - A pointer to where the size of the element will be returned.
void minipack_pack_uint(void *ptr, uint64_t value, size_t *sz)
{
    if(value <= POS_FIXNUM_MAX) {
        minipack_pack_pos_fixnum(ptr, (uint8_t)value, sz);
    }
    else if(value <= UINT8_MAX) {
        minipack_pack_uint8(ptr, (uint8_t)value, sz);
    }
    else if(value <= UINT16_MAX) {
        minipack_pack_uint16(ptr, (uint16_t)value, sz);
    }
    else if(value <= UINT32_MAX) {
        minipack_pack_uint32(ptr, (uint32_t)value, sz);
    }
    else if(value <= UINT64_MAX) {
        minipack_pack_uint64(ptr, value, sz);
    }
    else {
        *sz = 0;
    }
}


//--------------------------------------
// Unsigned Int (8-bit)
//--------------------------------------

// Checks if an element is an unsigned 8-bit integer.
//
// ptr - A pointer to the element.
//
// Returns true if the element is an 8-bit integer, otherwise returns false.
bool minipack_is_uint8(void *ptr)
{
    return (*((uint8_t*)ptr) == UINT8_TYPE);
}

// Reads an unsigned 8-bit integer from a given memory address.
//
// ptr - A pointer to where the unsigned int should be read from.
// sz  - A pointer to where the size of the element should be stored.
//
// Returns an unsigned 8-bit integer value.
uint8_t minipack_unpack_uint8(void *ptr, size_t *sz)
{
    *sz = UINT8_SIZE;
    return *((uint8_t*)(ptr+1));
}

// Writes an unsigned 8-bit integer to a given memory address.
//
// ptr - A pointer to where the integer should be written to.
// sz  - A pointer to where the size of the element should be stored.
void minipack_pack_uint8(void *ptr, uint8_t value, size_t *sz)
{
    *sz = UINT8_SIZE;
    *((uint8_t*)ptr)     = UINT8_TYPE;
    *((uint8_t*)(ptr+1)) = value;
}


//--------------------------------------
// Unsigned Int (16-bit)
//--------------------------------------

// Checks if an element is an unsigned 16-bit integer.
//
// ptr - A pointer to the element.
//
// Returns true if the element is an 16-bit integer, otherwise returns false.
bool minipack_is_uint16(void *ptr)
{
    return (*((uint8_t*)ptr) == UINT16_TYPE);
}

// Reads an unsigned 16-bit integer from a given memory address.
//
// ptr - A pointer to where the unsigned int should be read from.
// sz  - A pointer to where the size of the element should be stored.
//
// Returns an unsigned 16-bit integer value.
uint16_t minipack_unpack_uint16(void *ptr, size_t *sz)
{
    *sz = UINT16_SIZE;
    uint16_t value = *((uint16_t*)(ptr+1));
    return ntohs(value);
}

// Writes an unsigned 16-bit integer to a given memory address.
//
// ptr - A pointer to where the integer should be written to.
// sz  - A pointer to where the size of the element should be stored.
void minipack_pack_uint16(void *ptr, uint16_t value, size_t *sz)
{
    *sz = UINT16_SIZE;
    *((uint8_t*)ptr)      = UINT16_TYPE;
    *((uint16_t*)(ptr+1)) = htons(value);
}


//--------------------------------------
// Unsigned Int (32-bit)
//--------------------------------------

// Checks if an element is an unsigned 32-bit integer.
//
// ptr - A pointer to the element.
//
// Returns true if the element is an 32-bit integer, otherwise returns false.
bool minipack_is_uint32(void *ptr)
{
    return (*((uint8_t*)ptr) == UINT32_TYPE);
}

// Reads an unsigned 32-bit integer from a given memory address.
//
// ptr - A pointer to where the unsigned int should be read from.
// sz  - A pointer to where the size of the element should be stored.
//
// Returns an unsigned 32-bit integer value.
uint32_t minipack_unpack_uint32(void *ptr, size_t *sz)
{
    *sz = UINT32_SIZE;
    uint32_t value = *((uint32_t*)(ptr+1));
    return ntohl(value);
}

// Writes an unsigned 32-bit integer to a given memory address.
//
// ptr - A pointer to where the integer should be written to.
// sz  - A pointer to where the size of the element should be stored.
void minipack_pack_uint32(void *ptr, uint32_t value, size_t *sz)
{
    *sz = UINT32_SIZE;
    *((uint8_t*)ptr)      = UINT32_TYPE;
    *((uint32_t*)(ptr+1)) = htonl(value);
}


//--------------------------------------
// Unsigned Int (64-bit)
//--------------------------------------

// Checks if an element is an unsigned 64-bit integer.
//
// ptr - A pointer to the element.
//
// Returns true if the element is an 64-bit integer, otherwise returns false.
bool minipack_is_uint64(void *ptr)
{
    return (*((uint8_t*)ptr) == UINT64_TYPE);
}

// Reads an unsigned 64-bit integer from a given memory address.
//
// ptr - A pointer to where the unsigned int should be read from.
// sz  - A pointer to where the size of the element should be stored.
//
// Returns an unsigned 64-bit integer value.
uint64_t minipack_unpack_uint64(void *ptr, size_t *sz)
{
    *sz = UINT64_SIZE;
    uint64_t value = *((uint64_t*)(ptr+1));
    return ntohll(value);
}

// Writes an unsigned 64-bit integer to a given memory address.
//
// ptr - A pointer to where the integer should be written to.
// sz  - A pointer to where the size of the element should be stored.
void minipack_pack_uint64(void *ptr, uint64_t value, size_t *sz)
{
    *sz = UINT64_SIZE;
    *((uint8_t*)ptr)      = UINT64_TYPE;
    *((uint64_t*)(ptr+1)) = htonll(value);
}


//==============================================================================
//
// Signed Integers
//
//==============================================================================

//--------------------------------------
// Signed Int
//--------------------------------------

// Retrieves the size, in bytes, of how large an element will be.
//
// value - The value to calculate the size of.
//
// Returns the number of bytes needed for the element.
size_t minipack_int_sizeof(int64_t value)
{
    if(value >= POS_FIXNUM_MIN && value <= POS_FIXNUM_MAX) {
        return POS_FIXNUM_SIZE;
    }
    else if(value >= NEG_FIXNUM_MIN && value <= NEG_FIXNUM_MAX) {
        return NEG_FIXNUM_SIZE;
    }
    else if(value >= INT8_MIN && value <= INT8_MAX) {
        return INT8_SIZE;
    }
    else if(value >= INT16_MIN && value <= INT16_MAX) {
        return INT16_SIZE;
    }
    else if(value >= INT32_MIN && value <= INT32_MAX) {
        return INT32_SIZE;
    }
    else if(value >= INT64_MIN && value <= INT64_MAX) {
        return INT64_SIZE;
    }

    return 0;
}

// Reads a signed integer from a given memory address.
//
// ptr - A pointer to where the signed int should be read from.
// sz  - A pointer to where the size of the element should be stored.
//
// Returns the value of the element
int64_t minipack_unpack_int(void *ptr, size_t *sz)
{
    if(minipack_is_pos_fixnum(ptr)) {
        return (int64_t)minipack_unpack_pos_fixnum(ptr, sz);
    }
    if(minipack_is_neg_fixnum(ptr)) {
        return (int64_t)minipack_unpack_neg_fixnum(ptr, sz);
    }
    else if(minipack_is_int8(ptr)) {
        return (int64_t)minipack_unpack_int8(ptr, sz);
    }
    else if(minipack_is_int16(ptr)) {
        *sz = INT16_SIZE;
        return (int64_t)minipack_unpack_int16(ptr, sz);
    }
    else if(minipack_is_int32(ptr)) {
        *sz = INT32_SIZE;
        return (int64_t)minipack_unpack_int32(ptr, sz);
    }
    else if(minipack_is_int64(ptr)) {
        *sz = INT64_SIZE;
        return minipack_unpack_int64(ptr, sz);
    }
    else {
        *sz = 0;
        return 0;
    }
}

// Writes a signed integer to a given memory address.
//
// ptr   - A pointer to where the integer should be written to.
// value - The value to write.
// sz    - A pointer to where the size of the element will be returned.
void minipack_pack_int(void *ptr, int64_t value, size_t *sz)
{
    if(value >= POS_FIXNUM_MIN && value <= POS_FIXNUM_MAX) {
        minipack_pack_pos_fixnum(ptr, (int8_t)value, sz);
    }
    else if(value >= NEG_FIXNUM_MIN && value <= NEG_FIXNUM_MAX) {
        minipack_pack_pos_fixnum(ptr, (int8_t)value, sz);
    }
    else if(value >= INT8_MIN && value <= INT8_MAX) {
        minipack_pack_int8(ptr, (int8_t)value, sz);
    }
    else if(value >= INT16_MIN && value <= INT16_MAX) {
        minipack_pack_int16(ptr, (int16_t)value, sz);
    }
    else if(value >= INT32_MIN && value <= INT32_MAX) {
        minipack_pack_int32(ptr, (int32_t)value, sz);
    }
    else if(value >= INT64_MIN && value <= INT64_MAX) {
        minipack_pack_int64(ptr, value, sz);
    }
    else {
        *sz = 0;
    }
}


//--------------------------------------
// Signed Int (8-bit)
//--------------------------------------

// Checks if an element is a signed 8-bit integer.
//
// ptr - A pointer to the element.
//
// Returns true if the element is an 8-bit integer, otherwise returns false.
bool minipack_is_int8(void *ptr)
{
    return (*((uint8_t*)ptr) == INT8_TYPE);
}

// Reads an signed 8-bit integer from a given memory address.
//
// ptr - A pointer to where the signed int should be read from.
// sz  - A pointer to where the size of the element should be stored.
//
// Returns an signed 8-bit integer value.
int8_t minipack_unpack_int8(void *ptr, size_t *sz)
{
    *sz = INT8_SIZE;
    return *((int8_t*)(ptr+1));
}

// Writes an signed 8-bit integer to a given memory address.
//
// ptr - A pointer to where the integer should be written to.
// sz  - A pointer to where the size of the element should be stored.
void minipack_pack_int8(void *ptr, int8_t value, size_t *sz)
{
    *sz = INT8_SIZE;
    *((uint8_t*)ptr)    = INT8_TYPE;
    *((int8_t*)(ptr+1)) = value;
}


//--------------------------------------
// Signed Int (16-bit)
//--------------------------------------

// Checks if an element is a signed 16-bit integer.
//
// ptr - A pointer to the element.
//
// Returns true if the element is an 16-bit integer, otherwise returns false.
bool minipack_is_int16(void *ptr)
{
    return (*((uint8_t*)ptr) == INT16_TYPE);
}

// Reads an signed 16-bit integer from a given memory address.
//
// ptr - A pointer to where the signed int should be read from.
// sz  - A pointer to where the size of the element should be stored.
//
// Returns an signed 16-bit integer value.
int16_t minipack_unpack_int16(void *ptr, size_t *sz)
{
    *sz = INT16_SIZE;
    return ntohs(*((int16_t*)(ptr+1)));
}

// Writes an signed 16-bit integer to a given memory address.
//
// ptr - A pointer to where the integer should be written to.
// sz  - A pointer to where the size of the element should be stored.
void minipack_pack_int16(void *ptr, int16_t value, size_t *sz)
{
    *sz = INT16_SIZE;
    *((uint8_t*)ptr)     = INT16_TYPE;
    *((int16_t*)(ptr+1)) = htons(value);
}


//--------------------------------------
// Signed Int (32-bit)
//--------------------------------------

// Checks if an element is a signed 32-bit integer.
//
// ptr - A pointer to the element.
//
// Returns true if the element is an 32-bit integer, otherwise returns false.
bool minipack_is_int32(void *ptr)
{
    return (*((uint8_t*)ptr) == INT32_TYPE);
}

// Reads an signed 32-bit integer from a given memory address.
//
// ptr - A pointer to where the signed int should be read from.
// sz  - A pointer to where the size of the element should be stored.
//
// Returns an signed 32-bit integer value.
int32_t minipack_unpack_int32(void *ptr, size_t *sz)
{
    *sz = INT32_SIZE;
    return ntohl(*((int32_t*)(ptr+1)));
}

// Writes an signed 32-bit integer to a given memory address.
//
// ptr - A pointer to where the integer should be written to.
// sz  - A pointer to where the size of the element should be stored.
void minipack_pack_int32(void *ptr, int32_t value, size_t *sz)
{
    *sz = INT32_SIZE;
    *((uint8_t*)ptr)     = INT32_TYPE;
    *((int32_t*)(ptr+1)) = htonl(value);
}


//--------------------------------------
// Signed Int (64-bit)
//--------------------------------------

// Checks if an element is a signed 64-bit integer.
//
// ptr - A pointer to the element.
//
// Returns true if the element is an 64-bit integer, otherwise returns false.
bool minipack_is_int64(void *ptr)
{
    return (*((uint8_t*)ptr) == INT64_TYPE);
}

// Reads an signed 64-bit integer from a given memory address.
//
// ptr - A pointer to where the signed int should be read from.
// sz  - A pointer to where the size of the element should be stored.
//
// Returns an signed 64-bit integer value.
int64_t minipack_unpack_int64(void *ptr, size_t *sz)
{
    *sz = INT64_SIZE;
    return ntohll(*((int64_t*)(ptr+1)));
}

// Writes an signed 64-bit integer to a given memory address.
//
// ptr - A pointer to where the integer should be written to.
// sz  - A pointer to where the size of the element should be stored.
void minipack_pack_int64(void *ptr, int64_t value, size_t *sz)
{
    *sz = INT64_SIZE;
    *((uint8_t*)ptr)     = INT64_TYPE;
    *((int64_t*)(ptr+1)) = htonll(value);
}


//==============================================================================
//
// Nil
//
//==============================================================================

//--------------------------------------
// Nil
//--------------------------------------

// Checks if an element is a nil.
//
// ptr - A pointer to the element.
//
// Returns true if the element is a nil, otherwise returns false.
bool minipack_is_nil(void *ptr)
{
    return (*((uint8_t*)ptr) == NIL_TYPE);
}

// Reads a nil to a given memory address.
//
// ptr - A pointer to where the nil should be read from.
void minipack_unpack_nil(void *ptr, size_t *sz)
{
    if(minipack_is_nil(ptr)) {
        *sz = NIL_SIZE;
    }
    else {
        *sz = 0;
    }
}

// Writes a nil to a given memory address.
//
// ptr - A pointer to where the nil should be written to.
void minipack_pack_nil(void *ptr, size_t *sz)
{
    *sz = NIL_SIZE;
    *((uint8_t*)ptr) = NIL_TYPE;
}


//==============================================================================
//
// Boolean
//
//==============================================================================

//--------------------------------------
// Boolean
//--------------------------------------

// Checks if an element is a bool.
//
// ptr - A pointer to the element.
//
// Returns true if the element is a bool, otherwise returns false.
bool minipack_is_bool(void *ptr)
{
    return minipack_is_true(ptr) || minipack_is_false(ptr);
}

// Checks if an element is a bool true.
//
// ptr - A pointer to the element.
//
// Returns true if the element is a bool true, otherwise returns false.
bool minipack_is_true(void *ptr)
{
    return (*((uint8_t*)ptr) == TRUE_TYPE);
}

// Checks if an element is a bool false.
//
// ptr - A pointer to the element.
//
// Returns true if the element is a bool false, otherwise returns false.
bool minipack_is_false(void *ptr)
{
    return (*((uint8_t*)ptr) == FALSE_TYPE);
}

// Reads a boolean from a given memory address.
//
// ptr - A pointer to where the boolean should be read from.
//
// Returns a boolean value.
bool minipack_unpack_bool(void *ptr, size_t *sz)
{
    if(minipack_is_true(ptr)) {
        *sz = BOOL_SIZE;
        return true;
    }
    else if(minipack_is_false(ptr)) {
        *sz = BOOL_SIZE;
        return false;
    }
    else {
        *sz = 0;
        return false;
    }
}

// Writes a boolean to a given memory address.
//
// ptr - A pointer to where the boolean should be written to.
void minipack_pack_bool(void *ptr, bool value, size_t *sz)
{
    *sz = BOOL_SIZE;
    
    if(value) {
        *((uint8_t*)ptr) = TRUE_TYPE;
    }
    else {
        *((uint8_t*)ptr) = FALSE_TYPE;
    }
}


//==============================================================================
//
// Floating-point
//
//==============================================================================

//--------------------------------------
// Float
//--------------------------------------

// Checks if an element is a float.
//
// ptr - A pointer to the element.
//
// Returns true if the element is a float, otherwise returns false.
bool minipack_is_float(void *ptr)
{
    return (*((uint8_t*)ptr) == FLOAT_TYPE);
}

// Reads a float from a given memory address.
//
// ptr - A pointer to where the float should be read from.
//
// Returns a float value.
float minipack_unpack_float(void *ptr, size_t *sz)
{
    *sz = FLOAT_SIZE;
    
    // Cast bytes to int32 to use ntohl.
    uint32_t value = *((uint32_t*)(ptr+1));
    value = ntohl(value);
    return *((float*)&value);
}

// Writes a float to a given memory address.
//
// ptr - A pointer to where the float should be written to.
void minipack_pack_float(void *ptr, float value, size_t *sz)
{
    *sz = FLOAT_SIZE;
    
    uint32_t bytes = htonl(*((uint32_t*)&value));
    *((uint8_t*)ptr)   = FLOAT_TYPE;
    *((float*)(ptr+1)) = *((float*)&bytes);
}


//--------------------------------------
// Double
//--------------------------------------

// Checks if an element is a double.
//
// ptr - A pointer to the element.
//
// Returns true if the element is a double, otherwise returns false.
bool minipack_is_double(void *ptr)
{
    return (*((uint8_t*)ptr) == DOUBLE_TYPE);
}

// Reads a double from a given memory address.
//
// ptr - A pointer to where the double should be read from.
//
// Returns a double value.
double minipack_unpack_double(void *ptr, size_t *sz)
{
    *sz = DOUBLE_SIZE;
    
    // Cast bytes to int64 to use ntohll.
    uint64_t value = *((uint64_t*)(ptr+1));
    value = ntohll(value);
    return *((double*)&value);
}

// Writes a double to a given memory address.
//
// ptr - A pointer to where the double should be written to.
void minipack_pack_double(void *ptr, double value, size_t *sz)
{
    *sz = DOUBLE_SIZE;
    
    uint64_t bytes = htonll(*((uint64_t*)&value));
    *((uint8_t*)ptr)    = DOUBLE_TYPE;
    *((double*)(ptr+1)) = *((double*)&bytes);
}



//==============================================================================
//
// Raw Bytes
//
//==============================================================================

//--------------------------------------
// Raw bytes
//--------------------------------------

// Checks if an element is raw bytes.
//
// ptr - A pointer to the element.
//
// Returns true if the element is raw bytes, otherwise returns false.
bool minipack_is_raw(void *ptr)
{
    return minipack_is_fixraw(ptr) || minipack_is_raw16(ptr) || minipack_is_raw32(ptr);
}

// Retrieves the size, in bytes, of how large an element header will be.
//
// length - The length of the raw bytes.
//
// Returns the number of bytes needed for the header.
size_t minipack_raw_sizeof(uint32_t length)
{
    if(length <= FIXRAW_MAXSIZE) {
        return FIXRAW_SIZE;
    }
    else if(length <= RAW16_MAXSIZE) {
        return RAW16_SIZE;
    }
    else if(length <= RAW32_MAXSIZE) {
        return RAW32_SIZE;
    }

    return 0;
}

// Reads the header for raw bytes from a given memory address.
//
// ptr - A pointer to where the unsigned int should be read from.
// sz  - A pointer to where the size of the header will be returned to.
//
// Returns the number of bytes in the raw bytes.
uint32_t minipack_unpack_raw(void *ptr, size_t *sz)
{
    if(minipack_is_fixraw(ptr)) {
        return (uint32_t)minipack_unpack_fixraw(ptr, sz);
    }
    else if(minipack_is_raw16(ptr)) {
        return (uint32_t)minipack_unpack_raw16(ptr, sz);
    }
    else if(minipack_is_raw32(ptr)) {
        return minipack_unpack_raw32(ptr, sz);
    }
    else {
        *sz = 0;
        return 0;
    }
}

// Writes raw bytes to a given memory address.
//
// ptr    - A pointer to where the integer should be written to.
// length - The number of bytes to write.
// sz     - A pointer to where the size of the header will be returned.
void minipack_pack_raw(void *ptr, uint32_t length, size_t *sz)
{
    if(length <= FIXRAW_MAXSIZE) {
        minipack_pack_fixraw(ptr, (uint8_t)length, sz);
    }
    else if(length <= RAW16_MAXSIZE) {
        minipack_pack_raw16(ptr, (uint16_t)length, sz);
    }
    else if(length <= RAW32_MAXSIZE) {
        minipack_pack_raw32(ptr, length, sz);
    }
    else {
        *sz = 0;
    }
}


//--------------------------------------
// Fix raw
//--------------------------------------

// Checks if an element is a fixraw type.
//
// ptr - A pointer to the element.
//
// Returns true if the element is a fixraw, otherwise returns false.
bool minipack_is_fixraw(void *ptr)
{
    return (*((uint8_t*)ptr) & FIXRAW_TYPE_MASK) == FIXRAW_TYPE;
}

// Reads the number of bytes in a fix raw from a given memory address.
//
// ptr - A pointer to where the fix raw should be read from.
//
// Returns the length of the bytes.
uint8_t minipack_unpack_fixraw(void *ptr, size_t *sz)
{
    *sz = FIXRAW_SIZE;
    return *((uint8_t*)ptr) & FIXRAW_VALUE_MASK;
}

// Writes a fix raw byte array to a given memory address.
//
// ptr - A pointer to where the bytes should be written to.
void minipack_pack_fixraw(void *ptr, uint8_t length, size_t *sz)
{
    *sz = FIXRAW_SIZE;
    *((uint8_t*)ptr) = (length & FIXRAW_VALUE_MASK) | FIXRAW_TYPE;
}


//--------------------------------------
// Raw 16
//--------------------------------------

// Checks if an element is a raw16 type.
//
// ptr - A pointer to the element.
//
// Returns true if the element is a raw 16, otherwise returns false.
bool minipack_is_raw16(void *ptr)
{
    return (*((uint8_t*)ptr) == RAW16_TYPE);
}

// Reads the number of bytes in a raw 16 from a given memory address.
//
// ptr - A pointer to where the raw 16 should be read from.
//
// Returns the length of the bytes.
uint16_t minipack_unpack_raw16(void *ptr, size_t *sz)
{
    *sz = RAW16_SIZE;
    return ntohs(*((uint16_t*)(ptr+1)));
}

// Writes a raw 16 byte array to a given memory address.
//
// ptr - A pointer to where the bytes should be written to.
void minipack_pack_raw16(void *ptr, uint16_t length, size_t *sz)
{
    *sz = RAW16_SIZE;
    *((uint8_t*)ptr)      = RAW16_TYPE;
    *((uint16_t*)(ptr+1)) = htons(length);
}


//--------------------------------------
// Raw 32
//--------------------------------------

// Checks if an element is a raw32 type.
//
// ptr - A pointer to the element.
//
// Returns true if the element is a raw 32, otherwise returns false.
bool minipack_is_raw32(void *ptr)
{
    return (*((uint8_t*)ptr) == RAW32_TYPE);
}

// Reads the number of bytes in a raw 32 from a given memory address.
//
// ptr - A pointer to where the raw 32 should be read from.
//
// Returns the length of the bytes.
uint32_t minipack_unpack_raw32(void *ptr, size_t *sz)
{
    *sz = RAW32_SIZE;
    return ntohl(*((uint32_t*)(ptr+1)));
}

// Writes a raw 32 byte array to a given memory address.
//
// ptr - A pointer to where the bytes should be written to.
void minipack_pack_raw32(void *ptr, uint32_t length, size_t *sz)
{
    *sz = RAW32_SIZE;
    *((uint8_t*)ptr)      = RAW32_TYPE;
    *((uint32_t*)(ptr+1)) = htonl(length);
}



//==============================================================================
//
// Array
//
//==============================================================================

//--------------------------------------
// General
//--------------------------------------

// Checks if an element is an array.
//
// ptr - A pointer to the element.
//
// Returns true if the element is an array, otherwise returns false.
bool minipack_is_array(void *ptr)
{
    return minipack_is_fixarray(ptr) || minipack_is_array16(ptr) || minipack_is_array32(ptr);
}

// Retrieves the size, in bytes, of how large an element header will be.
//
// count - The number of elements in the array.
//
// Returns the number of bytes needed for the header.
size_t minipack_array_sizeof(uint32_t count)
{
    if(count <= FIXARRAY_MAXSIZE) {
        return FIXARRAY_SIZE;
    }
    else if(count <= ARRAY16_MAXSIZE) {
        return ARRAY16_SIZE;
    }
    else if(count <= ARRAY32_MAXSIZE) {
        return ARRAY32_SIZE;
    }

    return 0;
}

// Reads the header for an array from a given memory address.
//
// ptr - A pointer to where the array header should be read from.
// sz  - A pointer to where the size of the header will be returned to.
//
// Returns the number of elements in the array.
uint32_t minipack_unpack_array(void *ptr, size_t *sz)
{
    if(minipack_is_fixarray(ptr)) {
        return (uint32_t)minipack_unpack_fixarray(ptr, sz);
    }
    else if(minipack_is_array16(ptr)) {
        return (uint32_t)minipack_unpack_array16(ptr, sz);
    }
    else if(minipack_is_array32(ptr)) {
        return minipack_unpack_array32(ptr, sz);
    }
    else {
        *sz = 0;
        return 0;
    }
}

// Writes an array header to a given memory address.
//
// ptr   - A pointer to where the integer should be written to.
// count - The number of elements in the array.
// sz    - A pointer to where the size of the header will be returned.
void minipack_pack_array(void *ptr, uint32_t count, size_t *sz)
{
    if(count <= FIXARRAY_MAXSIZE) {
        minipack_pack_fixarray(ptr, (uint8_t)count, sz);
    }
    else if(count <= ARRAY16_MAXSIZE) {
        minipack_pack_array16(ptr, (uint16_t)count, sz);
    }
    else if(count <= ARRAY32_MAXSIZE) {
        minipack_pack_array32(ptr, count, sz);
    }
    else {
        *sz = 0;
    }
}


//--------------------------------------
// Fix array
//--------------------------------------

// Checks if an element is a fixarray type.
//
// ptr - A pointer to the element.
//
// Returns true if the element is a fixarray, otherwise returns false.
bool minipack_is_fixarray(void *ptr)
{
    return (*((uint8_t*)ptr) & FIXARRAY_TYPE_MASK) == FIXARRAY_TYPE;
}

// Reads the number of elements in a fix array from a given memory address.
//
// ptr - A pointer to where the fix array should be read from.
//
// Returns the number of elements in the array.
uint8_t minipack_unpack_fixarray(void *ptr, size_t *sz)
{
    *sz = FIXARRAY_SIZE;
    return *((uint8_t*)ptr) & FIXARRAY_VALUE_MASK;
}

// Writes a fix array header to a given memory address.
//
// ptr   - A pointer to where the header should be written to.
// count - The number of elements in the array.
void minipack_pack_fixarray(void *ptr, uint8_t count, size_t *sz)
{
    *sz = FIXARRAY_SIZE;
    *((uint8_t*)ptr) = (count & FIXARRAY_VALUE_MASK) | FIXARRAY_TYPE;
}


//--------------------------------------
// Array 16
//--------------------------------------

// Checks if an element is an array16 type.
//
// ptr - A pointer to the element.
//
// Returns true if the element is an array16, otherwise returns false.
bool minipack_is_array16(void *ptr)
{
    return (*((uint8_t*)ptr) == ARRAY16_TYPE);
}

// Reads the number of elements in an array 16 from a given memory address.
//
// ptr - A pointer to where the array 16 should be read from.
//
// Returns the number of elements in the array
uint16_t minipack_unpack_array16(void *ptr, size_t *sz)
{
    *sz = ARRAY16_SIZE;
    return ntohs(*((uint16_t*)(ptr+1)));
}

// Writes an array 16 header to a given memory address.
//
// ptr - A pointer to where the header should be written to.
void minipack_pack_array16(void *ptr, uint16_t count, size_t *sz)
{
    *sz = ARRAY16_SIZE;
    
    // Write header.
    *((uint8_t*)ptr)      = ARRAY16_TYPE;
    *((uint16_t*)(ptr+1)) = htons(count);
}


//--------------------------------------
// Array 32
//--------------------------------------

// Checks if an element is an array32 type.
//
// ptr - A pointer to the element.
//
// Returns true if the element is an array32, otherwise returns false.
bool minipack_is_array32(void *ptr)
{
    return (*((uint8_t*)ptr) == ARRAY32_TYPE);
}

// Reads the number of elements in an array 32 from a given memory address.
//
// ptr - A pointer to where the array 32 should be read from.
//
// Returns the number of elements in the array
uint32_t minipack_unpack_array32(void *ptr, size_t *sz)
{
    *sz = ARRAY32_SIZE;
    return ntohl(*((uint32_t*)(ptr+1)));
}

// Writes an array 32 header to a given memory address.
//
// ptr - A pointer to where the header should be written to.
void minipack_pack_array32(void *ptr, uint32_t count, size_t *sz)
{
    *sz = ARRAY32_SIZE;
    
    // Write header.
    *((uint8_t*)ptr)      = ARRAY32_TYPE;
    *((uint32_t*)(ptr+1)) = htonl(count);
}


//==============================================================================
//
// Map
//
//==============================================================================

//--------------------------------------
// General
//--------------------------------------

// Checks if an element is a map.
//
// ptr - A pointer to the element.
//
// Returns true if the element is a map, otherwise returns false.
bool minipack_is_map(void *ptr)
{
    return minipack_is_fixmap(ptr) || minipack_is_map16(ptr) || minipack_is_map32(ptr);
}

// Retrieves the size, in bytes, of how large an element header will be.
//
// count - The number of elements in the map.
//
// Returns the number of bytes needed for the header.
size_t minipack_map_sizeof(uint32_t count)
{
    if(count <= FIXMAP_MAXSIZE) {
        return FIXMAP_SIZE;
    }
    else if(count <= MAP16_MAXSIZE) {
        return MAP16_SIZE;
    }
    else if(count <= MAP32_MAXSIZE) {
        return MAP32_SIZE;
    }

    return 0;
}

// Reads the header for an map from a given memory address.
//
// ptr - A pointer to where the map header should be read from.
// sz  - A pointer to where the size of the header will be returned to.
//
// Returns the number of elements in the map.
uint32_t minipack_unpack_map(void *ptr, size_t *sz)
{
    if(minipack_is_fixmap(ptr)) {
        return (uint32_t)minipack_unpack_fixmap(ptr, sz);
    }
    else if(minipack_is_map16(ptr)) {
        return (uint32_t)minipack_unpack_map16(ptr, sz);
    }
    else if(minipack_is_map32(ptr)) {
        return minipack_unpack_map32(ptr, sz);
    }
    else {
        *sz = 0;
        return 0;
    }
}

// Writes an map header to a given memory address.
//
// ptr   - A pointer to where the integer should be written to.
// count - The number of elements in the map.
// sz    - A pointer to where the size of the header will be returned.
void minipack_pack_map(void *ptr, uint32_t count, size_t *sz)
{
    if(count <= FIXMAP_MAXSIZE) {
        minipack_pack_fixmap(ptr, (uint8_t)count, sz);
    }
    else if(count <= MAP16_MAXSIZE) {
        minipack_pack_map16(ptr, (uint16_t)count, sz);
    }
    else if(count <= MAP32_MAXSIZE) {
        minipack_pack_map32(ptr, count, sz);
    }
    else {
        *sz = 0;
    }
}


//--------------------------------------
// Fix map
//--------------------------------------

// Checks if an element is a fixmap type.
//
// ptr - A pointer to the element.
//
// Returns true if the element is a fixmap, otherwise returns false.
bool minipack_is_fixmap(void *ptr)
{
    return (*((uint8_t*)ptr) & FIXMAP_TYPE_MASK) == FIXMAP_TYPE;
}

// Reads the number of elements in a fix map from a given memory address.
//
// ptr - A pointer to where the fix map should be read from.
//
// Returns the number of elements in the map.
uint8_t minipack_unpack_fixmap(void *ptr, size_t *sz)
{
    *sz = FIXMAP_SIZE;
    return *((uint8_t*)ptr) & FIXMAP_VALUE_MASK;
}

// Writes a fix map header to a given memory address.
//
// ptr   - A pointer to where the header should be written to.
// count - The number of elements in the map.
void minipack_pack_fixmap(void *ptr, uint8_t count, size_t *sz)
{
    *sz = FIXMAP_SIZE;
    *((uint8_t*)ptr) = (count & FIXMAP_VALUE_MASK) | FIXMAP_TYPE;
}


//--------------------------------------
// Map 16
//--------------------------------------

// Checks if an element is an map16 type.
//
// ptr - A pointer to the element.
//
// Returns true if the element is an map16, otherwise returns false.
bool minipack_is_map16(void *ptr)
{
    return (*((uint8_t*)ptr) == MAP16_TYPE);
}

// Reads the number of elements in an map 16 from a given memory address.
//
// ptr - A pointer to where the map 16 should be read from.
//
// Returns the number of elements in the map.
uint16_t minipack_unpack_map16(void *ptr, size_t *sz)
{
    *sz = MAP16_SIZE;
    return ntohs(*((uint16_t*)(ptr+1)));
}

// Writes an map 16 header to a given memory address.
//
// ptr - A pointer to where the header should be written to.
void minipack_pack_map16(void *ptr, uint16_t count, size_t *sz)
{
    *sz = MAP16_SIZE;
    
    // Write header.
    *((uint8_t*)ptr)      = MAP16_TYPE;
    *((uint16_t*)(ptr+1)) = htons(count);
}


//--------------------------------------
// Map 32
//--------------------------------------

// Checks if an element is an map32 type.
//
// ptr - A pointer to the element.
//
// Returns true if the element is an map32, otherwise returns false.
bool minipack_is_map32(void *ptr)
{
    return (*((uint8_t*)ptr) == MAP32_TYPE);
}

// Reads the number of elements in an map 32 from a given memory address.
//
// ptr - A pointer to where the map 32 should be read from.
//
// Returns the number of elements in the map
uint32_t minipack_unpack_map32(void *ptr, size_t *sz)
{
    *sz = MAP32_SIZE;
    return ntohl(*((uint32_t*)(ptr+1)));
}

// Writes an map 32 header to a given memory address.
//
// ptr - A pointer to where the header should be written to.
void minipack_pack_map32(void *ptr, uint32_t count, size_t *sz)
{
    *sz = MAP32_SIZE;
    
    // Write header.
    *((uint8_t*)ptr)      = MAP32_TYPE;
    *((uint32_t*)(ptr+1)) = htonl(count);
}



