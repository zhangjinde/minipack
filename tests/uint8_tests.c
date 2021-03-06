#include <minipack.h>
#include <msgpack.h>

#include "minunit.h"
#include "memdump.h"

//==============================================================================
//
// Test Cases
//
//==============================================================================

int test_sizeof_uint8() {
    mu_assert(minipack_sizeof_elem_and_data("\xCC\x14") == 2);
    return 0;
}

int test_unpack_uint8() {
    size_t sz;
    mu_assert(minipack_unpack_uint8("\xCC\x00", &sz) == 0);
    mu_assert(sz == 2);
    mu_assert(minipack_unpack_uint8("\xCC\x02", &sz) == 2);
    mu_assert(sz == 2);
    mu_assert(minipack_unpack_uint8("\xCC\x14", &sz) == 20);
    mu_assert(sz == 2);
    mu_assert(minipack_unpack_uint8("\xCC\x7F", &sz) == 127);
    mu_assert_msgpack_uint8(128, 2, "\xCC\x80");
    mu_assert(sz == 2);
    mu_assert(minipack_unpack_uint8("\xCC\xFF", &sz) == 255);
    mu_assert_msgpack_uint8(255, 2, "\xCC\xFF");
    mu_assert(sz == 2);
    return 0;
}

int test_pack_uint8() {
    size_t sz;
    uint8_t data[] = {0x00, 0x00};

    minipack_pack_uint8(data, 0, &sz);
    mu_assert_mem(data, 2, "\xCC\x00");
    mu_assert(sz == 2);

    minipack_pack_uint8(data, 2, &sz);
    mu_assert_mem(data, 2, "\xCC\x02");
    mu_assert(sz == 2);

    minipack_pack_uint8(data, 20, &sz);
    mu_assert_mem(data, 2, "\xCC\x14");
    mu_assert(sz == 2);

    minipack_pack_uint8(data, 127, &sz);
    mu_assert_mem(data, 2, "\xCC\x7F");
    mu_assert(sz == 2);

    minipack_pack_uint8(data, 255, &sz);
    mu_assert_mem(data, 2, "\xCC\xFF");
    mu_assert(sz == 2);

    return 0;
}



//==============================================================================
//
// Setup
//
//==============================================================================

int all_tests() {
    mu_run_test(test_sizeof_uint8);
    mu_run_test(test_unpack_uint8);
    mu_run_test(test_pack_uint8);
    return 0;
}

RUN_TESTS()