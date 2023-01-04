#include "rrunitlib.h"
#include "f_ini_inc.h"

#define DPRINT(...) f_rrulib_dmsg("INIT", __func__ ,__VA_ARGS__ )

/************************************************/
/* テスト対象関数のテストコードを登録しています */
/************************************************/
static VOID f_ini_init_0101(VOID)
{	/* 期待値をチェック */
	f_iniw_sigchld = 0xFFFF;
	f_ini_iniTbl();
	CU_ASSERT(f_iniw_sigchld == 0);
	return;
}

CU_TestInfo	test_array_ft_init[] =
{
	{"0101"         ,f_ini_init_0101},
	CU_TEST_INFO_NULL
};
/****************************************************/
/* テスト対象タスクの全テストコードを登録しています */
/****************************************************/
CU_SuiteInfo test_suites_array_ft_init[] = {

	/* suite name            ,    ,    ,  test array name            */
	{"ft_init"            ,NULL,NULL,  test_array_ft_init        },	/* <--- 関数毎もしくはスレッド毎に1つ登録する */
	CU_SUITE_INFO_NULL				/* <-- 最後はお決まりでこれ */
};


