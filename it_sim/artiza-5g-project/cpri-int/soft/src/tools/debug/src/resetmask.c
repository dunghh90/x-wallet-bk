/*!
 * @skip   $Id$
 * @file   sm.c
 * @brief  print memory
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */
#include "f_dbg_cmd_inc.h"

/*!
 * @brief 関数機能概要:リセットマスク設定処理
 * @note  関数処理内容.
 *       -# リセットマスクを設定する
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  *argv		[in]char	コマンドアーギュメント
 * @return INT
 * @retval D_DBG_CMD_RST_SUCCESS	0:正常終了
 * @retval D_DBG_CMD_RST_ERROR		1:異常終了
 * @date 2014/01/15 ALPHA)宮崎 create
 */

int resetmask_main( int argc, char *argv[], FILE *outfp )	{
	int					in_sw	=	0;
	int					result	=	0;
	unsigned char		eeprom_dat		;
	T_RRH_TRAINVTBL*	invTbl;
	int					errcd;

	BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY, (VOID **)&invTbl, &errcd);

	if( argc == 2 ) {	/* パラメータにenable/disable設定が含まれている場合	*/

		in_sw = (char)atoi(argv[1]);	/* 変換不可も０ */
		if( in_sw == 0 || in_sw == 1 )	{

			eeprom_dat = in_sw ;
			/* EEPROM 書き込み	*/
			if(( result =  BPF_HM_DEVC_EEPROM_WRITE( D_DU_EEP_SOFT_RESET_MSK,  &eeprom_dat )) != BPF_HM_DEVC_COMPLETE ) {
				fprintf( stderr, "EEPROM write error(code:%d)\n", result );
				return( D_DBG_CMD_RST_ERROR ) ;
			}
			if( in_sw == 0 )	{
				fprintf(outfp, "set software reset enabled.\n");
			}
			else {
				fprintf(outfp, "set software reset disabled.\n");
			}
			/* 共有メモリにも値を設定	*/
			invTbl->softreset_mask[0] = eeprom_dat	;
			return D_DBG_CMD_RST_SUCCESS	;
		}
	}
	BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_SOFT_RESET_MSK,  &eeprom_dat ) ;

	fprintf( outfp, "usage: resetmask 0      enable software reset\n");
	fprintf( outfp, "       resetmask 1      disable software reset\n");
	fprintf( outfp, "now setting: software reset is %s.\n", ( eeprom_dat == 0 )? "enabled": "disabled" );

	return D_DBG_CMD_RST_ERROR ;

}
/* @} */

