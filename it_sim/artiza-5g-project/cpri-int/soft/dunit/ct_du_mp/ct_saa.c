#include "rrunitlib.h"
#include "f_mpsw_types.h"

#define DPRINT(...) f_rrulib_dmsg("MP", __func__ ,__VA_ARGS__ )

/************************************************/
/* �e�X�g�Ώۊ֐��̃e�X�g�R�[�h��o�^���Ă��܂� */
/************************************************/
static VOID saa_init_0101(VOID)
{	/* ���Ғl���`�F�b�N */
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
/* �e�X�g�Ώۃ^�X�N�̑S�e�X�g�R�[�h��o�^���Ă��܂� */
/****************************************************/
CU_SuiteInfo test_suites_array_saa[] = {

	/* suite name            ,    ,    ,  test array name            */
	{"saa"            ,NULL,NULL,  test_array_saa      },	/* <--- �֐����������̓X���b�h����1�o�^���� */
	CU_SUITE_INFO_NULL				/* <-- �Ō�͂����܂�ł��� */
};


