/*!
 * @skip    $Id$
 * @file    m_lb_c_if.c
 * @brief   Task IF変換処理関数(tm_lb_Anlz)
 * @date    2008.07.22 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/


/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "m_cm_header.h"
#include "tm_lb_header.h"





/*!
 *  @brief   Task IF変換処理関数(tm_lb_Anlz)
 *  @note    Transform task IF to internal event number
 *  @param   a_event		[in]  a_event
 *  @return  inter_event
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2015.08.11 TDIPS)ikeda 状態番号変更
 */
UINT tm_lb_Anlz(UINT a_event)
{
	UINT	ret;
	
	ret = CMD_OK;

	switch(a_event)
	{
		case CMD_TSKIF_L2BSY:                           /* 自受信ビジー         */
		case CMD_TSKIF_L2BSYCLR:                        /* 自受信ビジーの解除   */
		case CMD_TSKIF_L1DATRCVNTC:                     /* L1データ受信通知     */
		case CMD_TSKIF_L1DATCNF:                        /* Layer 1 Data CNF     */
		case CMD_TSKIF_CPRISNDNTC:                      /* CPRI信号送信通知     */
		case CMD_TSKIF_L2STPREQ:                        /* L2 STOP依頼          */
		case CMD_TSKIF_L3MODESTRNTC:                    /* L3 test 開始通知     */
		case CMD_TSKIF_L3MODESTPNTC:                    /* L3 test 停止通知     */
			lbw_tskevent = CMD_NUM1;
			break;

		case CMD_TSKIF_TIMOUTNTC :                      /* タイムアウト発生通知 */
			lbw_tskevent = CMD_NUM2;
			break;

		case CMD_TSKIF_ESTREQ:                          /* データリンク設定要求 */
			lbw_tskevent = CMD_NUM3;
			break;

		case CMD_TSKIF_L2DEACTREQ:                      /* L2 DEACT依頼         */
		case CMD_TSKIF_RELREQ:                          /* データリンク解放要求 */
			lbw_tskevent = CMD_NUM4;
			break;

		case CMD_TSKIF_INIREQ:                          /* 初期化要求           */
			lbw_tskevent = CMD_NUM0;
			break;

		default:                                        /* Others               */
			ret = CMD_NG;
			break;
	}
	return(ret);
}

/* @} */

