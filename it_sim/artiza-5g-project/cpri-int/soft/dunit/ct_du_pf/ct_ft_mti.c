#include "rrunitlib.h"
#include "f_mti_inc.h"

#define DPRINT(...) f_rrulib_dmsg("PF", __func__ ,__VA_ARGS__ )

extern T_RRH_MT_INFO*			f_mtiw_InfoTbl;

/************************************************/
/* テスト対象関数のテストコードを登録しています */
/************************************************/
static VOID f_mti_printf(VOID)
{
	int ret = 0;
	UINT	count;
	
	if(f_mtiw_InfoTbl != NULL){
		DPRINT("f_mtiw_InfoTbl->logInfo.index : %d\n",f_mtiw_InfoTbl->logInfo.index);
		for(count=0;count < f_mtiw_InfoTbl->logInfo.index;count++){
			DPRINT("%s\n",f_mtiw_InfoTbl->logInfo.tRecord[count].message);
		}

	}else{
		ret = -1;
	}
	CU_ASSERT(ret == 0);
	return;
}

CU_TestInfo	test_array_ft_mti[] =
{
	{"0101"         ,f_mti_printf},
	CU_TEST_INFO_NULL
};
/****************************************************/
/* テスト対象タスクの全テストコードを登録しています */
/****************************************************/
CU_SuiteInfo test_suites_array_ft_mti[] = {

	/* suite name            ,    ,    ,  test array name            */
	{"ft_mti"            ,NULL,NULL,  test_array_ft_mti      },	/* <--- 関数毎もしくはスレッド毎に1つ登録する */
	CU_SUITE_INFO_NULL				/* <-- 最後はお決まりでこれ */
};


