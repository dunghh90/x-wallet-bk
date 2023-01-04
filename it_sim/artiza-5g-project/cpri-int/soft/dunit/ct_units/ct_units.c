#include "rrunitlib.h"

static const t_ct_CUerror ct_CUerror[] =
{
	/* basic errors */
	{CUE_SUCCESS           ,"No error condition."},
	{CUE_NOMEMORY          ,"Memory allocation failed."},
	/* Test Registry Level Errors */
	{CUE_NOREGISTRY        ,"Test registry not initialized."},
	{CUE_REGISTRY_EXISTS   ,"Attempt to CU_set_registry() without CU_cleanup_registry()."},
	/* Test Suite Level Errors */
	{CUE_NOSUITE           ,"A required CU_pSuite pointer was NULL."},
	{CUE_NO_SUITENAME      ,"Required CU_Suite name not provided."},
	{CUE_SINIT_FAILED      ,"Suite initialization failed."},
	{CUE_SCLEAN_FAILED     ,"Suite cleanup failed."},
	{CUE_DUP_SUITE         ,"Duplicate suite name not allowed."},
	/* Test Case Level Errors */
	{CUE_NOTEST            ,"A required CU_pTest pointer was NULL."},
	{CUE_NO_TESTNAME       ,"Required CU_Test name not provided."},
	{CUE_DUP_TEST          ,"Duplicate test case name not allowed."},
	{CUE_TEST_NOT_IN_SUITE ,"Test not registered in specified suite."},
	/* File handling errors */
	{CUE_FOPEN_FAILED      ,"An error occurred opening a file."},
	{CUE_FCLOSE_FAILED     ,"An error occurred closing a file."},
	{CUE_BAD_FILENAME      ,"A bad filename was requested (NULL, empty, nonexistent, etc.)."},
	{CUE_WRITE_ERROR       ,"An error occurred during a write to a file."},
	{0xFFFFFFFF            ,"error code No entry "},
};

/**
 *  @brief  CT CUnit error code get 
 *  @note   CT CUnit error code get
 *  @return INT
 */
static int	ct_CUerrorGet(CU_ErrorCode cu_error)
{
	unsigned int	count;
	unsigned int	max = (sizeof(ct_CUerror)/sizeof(ct_CUerror[0]));
	unsigned int	index;

	/* OK時は無処理 */
	if(cu_error == 0)
	{
		return(0);
	}
	for(count=0; count < max ; count++)
	{
		if(cu_error == ct_CUerror[count].cu_error)
		{
			index = count;
			break;
		}
	}
	if(cu_error >= max)
	{
		index = max - 1 ;
	}
	printf("Error Happen \n");
	printf(" error code = %d \n",(int)cu_error);
	printf(" error info = %s \n",(ct_CUerror[index].cu_txt));
	return(-1);
}

extern CU_ErrorCode CU_basic_run_tests(void);
/**
 *  @brief  CT main code 
 *  @note   CT main code
 *  @return INT
 */
/* int main(int argc, char**argv) */
int ct_units_main(void* test_suites_array, int mode)
{
	CU_ErrorCode	cu_error;
	int				ret=0;
	

	if(!test_suites_array){
		printf("** input table addr error *** \n");
		return(-1);
	}

	/* CUnit初期化 */
	CU_initialize_registry();
	/* テストコード登録 */
	cu_error = CU_register_suites(test_suites_array);
	if(cu_error != 0)
	{
		ret = ct_CUerrorGet(cu_error);
	}
	else{
		/* モード判定 */
		/* 0 */
		if (mode == 0){
			/* basicで起動(コマンドライン上から起動時に使用) */
			cu_error = CU_basic_run_tests();
			if(cu_error != 0)
			{
				ret = ct_CUerrorGet(cu_error);
			}
		}
		/* 1 */
		else {
			/* コンソールモードで起動 */
			CU_console_run_tests();
		}
	}
	/* CUnit解放 */
	CU_cleanup_registry();
	if(ret != 0){
		printf("** CUnit Quit(%d.%d)) *** \n",ret,cu_error);
	}
	return(ret);
}
/** @} */
