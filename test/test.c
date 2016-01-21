#include "CUnit/Basic.h"
#include "CUnit/Console.h"
#include "CUnit/Automated.h"

#include "../typedefs.h"
#include "../system/fileio.h"
#include "../output/outbuf.h"

int init_suite1(void) { return 0; }
int clean_suite1(void) { return 0; }

static int fatal_call_count = 0;

void fatal(int errnum, ...)
{
    ++fatal_call_count;
}

void test_write_out_buf(void)
{
    OUTBUF test;
    SetupOutbuf(&test, 10);
    CU_ASSERT_EQUAL(test.end, test.buffer + 10);

    fatal_call_count = 0;
    WriteOutbuf(&test, "Hello", sizeof("Hello"));
    CU_ASSERT_EQUAL(fatal_call_count, 0);
    CU_ASSERT_EQUAL(OB_SIZE(test), sizeof("Hello"));

    WriteOutbuf(&test, "Hello World in other language", 11);
    CU_ASSERT_EQUAL(fatal_call_count, 1);
}

void test_simple(void)
{
    OUTBUF test;
    SetupOutbuf(&test, 10);
    CU_ASSERT_EQUAL(OB_SIZE(test), 0);

    SeekFwdOutbuf(&test, 2);
    CU_ASSERT_EQUAL(OB_SIZE(test), 2);

    fatal_call_count = 0;
    SeekFwdOutbuf(&test, 10);
    CU_ASSERT_EQUAL(fatal_call_count, 1);
    CU_ASSERT_EQUAL(OB_SIZE(test), 2);
}

int main()
{
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if (NULL == CU_add_test(pSuite, "test SeekFwdOutbuf", test_simple))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test WriteOutbuf", test_write_out_buf))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }



    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
