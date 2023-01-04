#include "rrunitlib.h"
#include "f_ini_inc.h"

#define DPRINT(...) f_rrulib_dmsg("INIT", __func__ ,__VA_ARGS__ )

/************************************************/
/* �e�X�g�Ώۊ֐��̃e�X�g�R�[�h��o�^���Ă��܂� */
/************************************************/
static VOID f_ini_init_0101(VOID)
{	/* ���Ғl���`�F�b�N */
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
/* �e�X�g�Ώۃ^�X�N�̑S�e�X�g�R�[�h��o�^���Ă��܂� */
/****************************************************/
CU_SuiteInfo test_suites_array_ft_init[] = {

	/* suite name            ,    ,    ,  test array name            */
	{"ft_init"            ,NULL,NULL,  test_array_ft_init        },	/* <--- �֐����������̓X���b�h����1�o�^���� */
	CU_SUITE_INFO_NULL				/* <-- �Ō�͂����܂�ł��� */
};


