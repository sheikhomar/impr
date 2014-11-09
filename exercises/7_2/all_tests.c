#include "CuTest.h"
#include "translate_seconds.h"
#include <stdio.h>

CuSuite* StrUtilGetSuite();

void testCase1(CuTest *tc);
void testCase2(CuTest *tc);
void testCase3(CuTest *tc);
void testCase4(CuTest *tc);
void testCase5(CuTest *tc);
CuSuite* translate_seconds_test_suite();
void run_all_tests(void);

int main(void) {
  run_all_tests();
  return 0;
}

void run_all_tests(void) {
  CuString *output = CuStringNew();
  CuSuite* suite = CuSuiteNew();

  CuSuiteAddSuite(suite, (CuSuite*)translate_seconds_test_suite());

  CuSuiteRun(suite);
  CuSuiteSummary(suite, output);
  CuSuiteDetails(suite, output);
  printf("%s\n", output->buffer);
}

CuSuite* translate_seconds_test_suite() {
  CuSuite* suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, testCase1);
  SUITE_ADD_TEST(suite, testCase2);
  SUITE_ADD_TEST(suite, testCase3);
  SUITE_ADD_TEST(suite, testCase4);
  SUITE_ADD_TEST(suite, testCase5);
  return suite;
}
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

/*
 * Input: 75 sekunder. Forventet output: 0 timer, 1 minutter og 15 sekunder
 */
void testCase2(CuTest *tc){
  int hours, minutes, seconds;
  translate_seconds(75, &hours, &minutes, &seconds);

  CuAssertIntEquals(tc, 0, hours);
  CuAssertIntEquals(tc, 1, minutes);
  CuAssertIntEquals(tc, 15, seconds);
}

/*
 * Input: 3700 sekunder. Forventet output: 1 timer, 1 minutter og 40 sekunder
 */
void testCase3(CuTest *tc){
  int hours, minutes, seconds;
  translate_seconds(3700, &hours, &minutes, &seconds);

  CuAssertIntEquals(tc, 1, hours);
  CuAssertIntEquals(tc, 1, minutes);
  CuAssertIntEquals(tc, 40, seconds);
}

/*
 * Input: 55 sekunder. Forventet output: 0 timer, 0 minutter og 55 sekunder
 */
void testCase4(CuTest *tc){
  int hours, minutes, seconds;
  translate_seconds(55, &hours, &minutes, &seconds);

  CuAssertIntEquals(tc, 0, hours);
  CuAssertIntEquals(tc, 0, minutes);
  CuAssertIntEquals(tc, 55, seconds);
}

/*
 * Input: 3661 sekunder. Forventet output: 1 timer, 1 minutter og 1 sekunder
 */
void testCase5(CuTest *tc){
  int hours, minutes, seconds;
  translate_seconds(3661, &hours, &minutes, &seconds);

  CuAssertIntEquals(tc, 1, hours);
  CuAssertIntEquals(tc, 1, minutes);
  CuAssertIntEquals(tc, 1, seconds);
}
