#include "rrunitlib.h"
#include "f_mpsw_types.h"

#define DPRINT(...) f_rrulib_dmsg("MP", __func__ ,__VA_ARGS__ )

/************************************************/
/* テスト対象関数のテストコードを登録しています */
/************************************************/
static VOID saa_init_0101(VOID)
{	/* 期待値をチェック */
	UINT	ret = 0;
	CU_ASSERT(ret == 0);
	return;
}

CU_TestInfo	test_array_saa[] =
{
	{"0101"         ,saa_init_0101},
	CU_TEST_INFO_NULL
};
/****************************************************/
/* テスト対象タスクの全テストコードを登録しています */
/****************************************************/
CU_SuiteInfo test_suites_array_saa[] = {

	/* suite name            ,    ,    ,  test array name            */
	{"saa"            ,NULL,NULL,  test_array_saa      },	/* <--- 関数毎もしくはスレッド毎に1つ登録する */
	CU_SUITE_INFO_NULL				/* <-- 最後はお決まりでこれ */
};


