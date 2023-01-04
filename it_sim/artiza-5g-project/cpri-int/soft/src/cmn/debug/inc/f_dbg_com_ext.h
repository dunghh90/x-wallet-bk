/*!
 * @skip  $ld:$
 * @file  f_dbg_com_ext.h
 * @brief Function prototype definition for Debug Function
 * @date  2013/12/26 ALPHA)宮崎
 * @date  2014/12/15 ALPHA)桑村
 *	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2014
 */

/* アラームヒストリー出力	*/
extern int cmd_alm_hist_log_main( int argc, char *argv[], FILE *ofp );

/* */
extern int	cmd_eeprom_access_main( int argc,	char *argv[], FILE *outfp	);

/* EEPROM情報表示	*/
extern int cmd_eeprom_disp_main( int argc, char *argv[], FILE *outfp );

/* LM書き込み	*/
extern int cmd_lm_write_main(int argc, char *argv[], FILE* outfp );

/* レジスターアクセス処理	*/
extern int cmd_reg_access_main( int argc, char *argv[], FILE *outfp );


/* RRHデバッグコマンド*/
extern int cmd_rrhdbg_main( int argc, char *argv[], FILE* outfp );

/* バージョン出力 */
extern int cmd_version_main(int argc, char *argv[], FILE *outfp )	;

/* メモリー表示 */
extern int pm_main( int argc, char *argv[], FILE *outfp );

/* メモリー設定	*/
extern int sm_main(int argc, char *argv[], FILE *outfp );

/* TRA読み込み */
extern int trar_main( int argc, char *argv[], FILE *outfp  );

/* TRA書き込み	*/
extern int	traw_main( int argc,	char *argv[], FILE *outfp	);

