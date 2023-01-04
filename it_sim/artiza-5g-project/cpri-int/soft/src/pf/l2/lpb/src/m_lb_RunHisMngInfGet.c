/*!
 * @skip    $Id$
 * @file    m_lb_RunHisMngInfGet.c
 * @brief   Get LAPB management table information(LAPB走行履歴)
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

#include "m_lb_header.h"
/*!
 *  @brief   Get LAPB management table information(LAPB走行履歴)
 *  @note    Get link status,cpri link number,internal event number from managemenr table(LAPB走行履歴)
 *  @param   link_sta_p			[in]  link status pointer
 *  @param   in_cpri_no_p		[in]  CPRI link number pointer
 *  @param   inter_event_no_p	[in]  Internal event number pointer
 *  @param   a_inf_p			[in]  受信メッセージアドレス
 *  @param   a_event			[in]  起動要因
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 */
void	tm_lb_RunHisMngInfGet(UCHAR *link_sta_p, UCHAR *in_cpri_no_p, 
											UCHAR *inter_event_no_p, UINT a_event, 
											UINT *  a_inf_p)
{
	LBT_TSKIF_CPRICOM	*cpri_p;                            /*!< */
	UINT	length;                                         /*!< */
	UCHAR	adr;                                            /*!< */
	UCHAR	ctl;                                            /*!< */
	struct LBT_MNGTBL* a_mngtbl;

	a_mngtbl = get_mngtbl(a_inf_p);

	*in_cpri_no_p = (UCHAR)a_mngtbl->link_num;
	*link_sta_p = a_mngtbl->lap_dt.status;
	
	switch(a_event)
	{
														/* Layer 1 Data CNF	*/
		case CMD_TSKIF_L1DATCNF:
			*inter_event_no_p = LBD_L1DATCNF;
			break;
														/* L1データ受信通知	*/
		case CMD_TSKIF_L1DATRCVNTC:
			if(a_mngtbl->lap_dt.status != LBD_L2_TESTSTS)
			{
				cpri_p = (LBT_TSKIF_CPRICOM	*)a_inf_p;
				length = cpri_p->length;
				adr		= cpri_p->Address;
				ctl		= cpri_p->Control;
				*inter_event_no_p = tm_lb_RunHisInterEvGet(length,adr,ctl,
						a_mngtbl->lap_dt.own_adr,
						a_mngtbl->lap_dt.per_adr,
						a_mngtbl->lap_dt.n1);
			}
			else
			{
				*inter_event_no_p = LBD_ICTL;
			}
			break;
														/* L1データ送信要求		*/
		case CMD_TSKIF_L1DATTXREQ:
			if(a_mngtbl->lap_dt.status != LBD_L2_TESTSTS)
			{
				cpri_p = (LBT_TSKIF_CPRICOM	*)a_inf_p;
				length = cpri_p->length;
				adr		= cpri_p->Address;
				ctl		= cpri_p->Control;
				*inter_event_no_p = tm_lb_RunHisInterEvGet(length,adr,ctl,
						a_mngtbl->lap_dt.per_adr,
						a_mngtbl->lap_dt.own_adr,
						a_mngtbl->lap_dt.n1);
			}
			else
			{
				*inter_event_no_p = LBD_ICTL;
			}
			break;
														/* CPRI信号送信通知	*/
		case CMD_TSKIF_CPRISNDNTC:
			*inter_event_no_p = LBD_CPRISNDNTC;
			break;
														/* L2 DEACT依頼			*/
		case CMD_TSKIF_L2DEACTREQ:
			*inter_event_no_p = LBD_L2DEACTREQ;
			break;
														/* データリンク設定確認	*/
		case CMD_TSKIF_ESTCNF:
														/* データリンク設定表示	*/
		case CMD_TSKIF_ESTIND:
														/* データリンク設定中表示	*/
		case CMD_TSKIF_ESTINGIND:
														/* データリンク解放確認	*/
		case CMD_TSKIF_RELCNF:
														/* データリンク解放表示	*/
		case CMD_TSKIF_RELIND:
														/* CPRI信号受信通知		*/
		case CMD_TSKIF_CPRIRCVNTC:
			return;
														/* データリンク設定要求	*/
		case CMD_TSKIF_ESTREQ:
			*inter_event_no_p = LBD_ESTREQ;
			break;
														/* データリンク解放要求	*/
		case CMD_TSKIF_RELREQ:
			*inter_event_no_p = LBD_RELREQ;
			break;
														/* 自受信ビジー			*/
		case CMD_TSKIF_L2BSY:
			*inter_event_no_p = LBD_L2BSY;
			break;
														/* 自受信ビジーの解除	*/
		case CMD_TSKIF_L2BSYCLR:
			*inter_event_no_p = LBD_L2BSYCLR;
			break;
														/* L2 STOP依頼			*/
		case CMD_TSKIF_L2STPREQ:
			*inter_event_no_p = LBD_L2STPREQ;
			break;
														/* L3 test 開始通知		*/
		case CMD_TSKIF_L3MODESTRNTC:
			*inter_event_no_p = LBD_L3TESTSTR;
			break;
														/* L3 test 停止通知		*/
		case CMD_TSKIF_L3MODESTPNTC:
			*inter_event_no_p = LBD_L3TESTSTP;
			break;
		default:
			*inter_event_no_p = LBD_OTHERS;
			break;
	}
	return;
}

/* @} */

