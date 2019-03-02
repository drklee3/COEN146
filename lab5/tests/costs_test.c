#include <stdio.h>
#include <stdlib.h>
#include "CuTest.h"
#include "../src/costs.h"

void TestParseCosts(CuTest *tc) {
    FILE* fp = fopen("../costs", "r");
    CuAssertTrue(tc, fp != 0);

    parse_costs(fp);
}

CuSuite* CuGetSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestParseCosts);

	return suite;
}

