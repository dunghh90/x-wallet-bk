#ifndef __DULIB_H__
#define __DULIB_H__

/*!
 * @brief		rrhコマンド情報構造体
 *
 */
typedef struct
{
	FILE   *fp;			/*!< スクリプトファイルポインタ */
	char*  cmdline;		/*!< コマンド名 */
	int    argc;		/*!< スクリプトパラメータ数 */
	char** argv;		/*!< スクリプト引きパラメータ  */
} rrh_t_args;



void rrhlib_usage(char* argv);
unsigned int rrhlib_install(mrb_state* mrb);
void rrhlib_clean(mrb_state* mrb);
void rrhlib_print_error(mrb_state* mrb);
#endif
