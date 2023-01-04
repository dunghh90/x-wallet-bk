#include "rrunitlib.h"
#include "f_main_inc.h"
#include "f_dpda_inc.h"

#define DPRINT(...) f_rrulib_dmsg("PF", __func__ ,__VA_ARGS__ )

/************************************************/
/* テスト対象関数のテストコードを登録しています */
/************************************************/
static VOID f_main_init_0101(VOID)
{	/* 期待値をチェック */
	UINT	ret = 0;
	ret = f_main_init(NULL);
	CU_ASSERT(ret == 0);
	return;
}

static VOID f_check_dpda(VOID)
{	/* 期待値をチェック */
	UINT	ret = 0;

	DPRINT("T_DPDA_INFO_ALL size:0x%x(%d), max:0x2000\n",sizeof(T_DPDA_INFO_ALL),sizeof(T_DPDA_INFO_ALL));
	DPRINT("T_DPDA_ALM_LOG_DEBUG size:0x%x(%d), max:0x00150000\n",sizeof(T_DPDA_ALM_LOG_DEBUG),sizeof(T_DPDA_ALM_LOG_DEBUG));

	if( sizeof(T_DPDA_INFO_ALL) > 0x2000 ){
		ret = 1;
	}
	if( sizeof(T_DPDA_ALM_LOG_DEBUG) > 0x00150000 ){
		ret = 1;
	}
	CU_ASSERT(ret == 0);
	return;
}

CU_TestInfo	test_array_ft_main[] =
{
	{"0101"         ,f_main_init_0101},
	CU_TEST_INFO_NULL
};

CU_TestInfo	test_array_ft_check[] =
{
	{"0101"         ,f_check_dpda},
	CU_TEST_INFO_NULL
};

/****************************************************/
/* テスト対象タスクの全テストコードを登録しています */
/****************************************************/
CU_SuiteInfo test_suites_array_ft_main[] = {

	/* suite name            ,    ,    ,  test array name            */
	{"ft_main"            ,NULL,NULL,  test_array_ft_main      },	/* <--- 関数毎もしくはスレッド毎に1つ登録する */
	CU_SUITE_INFO_NULL				/* <-- 最後はお決まりでこれ */
};

CU_SuiteInfo test_suites_array_ft_check[] = {

	/* suite name            ,    ,    ,  test array name            */
	{"ft_check"            ,NULL,NULL,  test_array_ft_check      },	/* <--- 関数毎もしくはスレッド毎に1つ登録する */
	CU_SUITE_INFO_NULL				/* <-- 最後はお決まりでこれ */
};


