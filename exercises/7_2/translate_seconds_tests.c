#include "CuTest.h"
#include "translate_seconds.h"
#include <stdio.h>

/* 
 * Input: 4000 sekunder. Forventet output: 1 timer, 6 minutter og 40 sekunder
 */
void testCase1(CuTest *tc){
  int hours, minutes, seconds;
  translate_seconds(4000, &hours, &minutes, &seconds);

  CuAssertIntEquals(tc, 1, hours);
  CuAssertIntEquals(tc, 6, minutes);
  CuAssertIntEquals(tc, 40, seconds);
}

/* Test case management: Adding the test case to a test suite */
CuSuite* translate_seconds_test_suite() {
  CuSuite* suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, testCase1);
  return suite;
}
