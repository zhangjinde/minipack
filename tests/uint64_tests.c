#include <minipack.h>
#include <msgpack.h>

#include "minunit.h"
#include "memdump.h"

//==============================================================================
//
// Test Cases
//
//==============================================================================

int test_uint64_read() {
    mu_assert(minipack_unpack_uint64("\xCF\x00\x00\x00\x00\x00\x00\x00\x00") == 0);
    mu_assert(minipack_unpack_uint64("\xCF\x00\x00\x00\x00\x00\x00\x00\x02") == 2);
    mu_assert(minipack_unpack_uint64("\xCF\x00\x00\x00\x00\x00\x00\x40\x23") == 16419);
    mu_assert(minipack_unpack_uint64("\xCF\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF") == 281474976710655LL);
    mu_assert_msgpack_uint64(281474976710655LL, 9, "\xCF\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF");
    return 0;
}

int test_uint64_write() {
    uint8_t data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    minipack_pack_uint64(data, 0);
    mu_assert_mem(data, 9, "\xCF\x00\x00\x00\x00\x00\x00\x00\x00");

    minipack_pack_uint64(data, 2);
    mu_assert_mem(data, 9, "\xCF\x00\x00\x00\x00\x00\x00\x00\x02");

    minipack_pack_uint64(data, 16419);
    mu_assert_mem(data, 9, "\xCF\x00\x00\x00\x00\x00\x00\x40\x23");

    minipack_pack_uint64(data, 281474976710655LL);
    mu_assert_mem(data, 9, "\xCF\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF");

    return 0;
}



//==============================================================================
//
// Setup
//
//==============================================================================

int all_tests() {
    mu_run_test(test_uint64_read);
    mu_run_test(test_uint64_write);
    return 0;
}

RUN_TESTS()