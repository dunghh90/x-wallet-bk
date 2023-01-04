/*!
 *  @skip   $Id:$
 *  @file   re_main_systemInit.c
 *  @brief  system init 
 *  @date   2015/08/04 TDIPS) maruyama create(他のスレッドを参考に作成）
 *  @date   2015/08/05 TDIPS) maruyama modify includeファイル追加、ワーニング対策
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */


 /*!
 * @addtogroup RRH_RE_MAIN
 * @{
 */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "re_com_inc.h"
#include "re_main_inc.h"
#include "f_com_aplCom.h"
#include "rrhApi_L2.h"

/********************************************************************************************************************/
/*!
 *  @brief  system init.
 *  @note   send system init complete notice to init process and init system
 *  @param  -      [in]  -
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date  2015/08/04 TDIPS) maruyama create(他のスレッドを参考に作成）
 */
/********************************************************************************************************************/
VOID re_main_systemInit(void)
{
	INT 					ret;							/* MSG受信成否				*/
	INT						errcd;							/* エラーコード				*/
	UINT					rcvmsgsz;						/* 受信メッセージサイズ		*/
	UCHAR               	rcvBuf[512];					/* 受信バッファ				*/
	USHORT linkno;

	/* init processからの(0x10010007)All thread initialize 受信 */
	ret = apl_com_msgQReceive(
			CMD_TSKID_INI,						/* 送信元プロセス 			*/
			sizeof(rcvBuf),		 				/* 受信バッファサイズ		*/
			(VOID *)rcvBuf, 					/* 受信バッファ				*/
			(INT*)&rcvmsgsz, 					/* 受信メッセージサイズ		*/
			&errcd								/* エラーコード				*/
		);
	/* 受信成否チェック */
	if( ret != D_RRH_OK ) {
		cm_Assert( CMD_ASL_DBGLOW, ret,
		    "[re_main] Receive system init from init process NG" );

		return;
	}


	for (linkno = D_RRH_CPRINO_RE1; linkno < D_RRH_CPRINO_NUM; linkno++ )
	{
		/* call api to subscribe RE message */
		rrhApi_L2_Mpr_SubscribeCpriMsg(linkno, D_RRH_PROCQUE_RE, 0, D_RRH_NULL, (UINT *)mnt_comr_remsg_subTbl,MBD_REMSGNUM);
	}
	
	return;
}


/* @} */

