#ifndef __RRHLIB_H__
#define __RRHLIB_H__

/*!
 * @brief		rrh�R�}���h���\����
 *
 */
typedef struct
{
	FILE   *fp;			/*!< �X�N���v�g�t�@�C���|�C���^ */
	char*  cmdline;		/*!< �R�}���h�� */
	int    argc;		/*!< �X�N���v�g�p�����[�^�� */
	char** argv;		/*!< �X�N���v�g�����p�����[�^  */
} rrh_t_args;



void rrhlib_usage(char* argv);
void rrhlib_install(mrb_state* mrb);
void rrhlib_clean(mrb_state* mrb);
void rrhlib_print_error(mrb_state* mrb);
int talise_cmd_main(int argc, char * const argv[]);
#endif
