/*!
 * @skip    $Id$
 * @file    tm_lb_trans.c
 * @brief   単一のリンクIF変換処理関数(tm_lb_trans)
 * @date    2008.07.22 FFCS)Shihzh create.
 * @date    2013.11.22 FFCS)hongj modify for zynq.
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
 *  @brief   単一のリンクIF変換処理関数(tm_lb_trans)
 *  @note    Transform single link task IF to single link internal event number
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p		[in]  受信メッセージアドレス
 *  @param   a_event		[in]  起動要因
 *  @return  L2-frame-kind
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
UINT 	tm_lb_trans(UINT a_event, struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
	UINT		cond;                                       /*!< */

	
	switch(a_event)
	{
														/* Layer 1 Data CNF	*/
		case CMD_TSKIF_L1DATCNF:
			cond = LBD_L1DATCNF;
			break;
														/* CPRI信号送信通知	*/
		case CMD_TSKIF_CPRISNDNTC:
			cond = LBD_CPRISNDNTC;
			break;
														/* L1データ受信通知	*/
		case CMD_TSKIF_L1DATRCVNTC:
			if(s_lblap_p->lap_dt.status != LBD_L2_TESTSTS)
			{
				s_lblap_p->tr_adr.lap_buf_p = (LBT_TSKIF_CPRICOM	*)a_inf_p;
/*				s_lblap_p->tr_adr.lap_buf_p->disp = LBD_L2CTL_OFFSET;*/
														/*-----------------------*/
														/* 受信 バッファ アドレス セーブ */
														/*-----------------------*/
				s_lblap_p->buf_adr.lap_buf_p = s_lblap_p->tr_adr.lap_buf_p;
													/*---------------------------*/
													/*L2用アドレス､コントロール フィールドセーブ*/
													/*---------------------------*/
				s_lblap_p->buf_adr.disp += s_lblap_p->buf_adr.lap_buf_p->disp;
				s_lblap_p->adr_wk = s_lblap_p->buf_adr.lap_l2_p->address;
				s_lblap_p->ctl1_sav.ctl1_all = s_lblap_p->buf_adr.lap_l2_p->l2_cntl1;
														/*-----------------------*/
														/* コントロール フィルド チェック 処理*/
														/*-----------------------*/
				lblap_chkctl(s_lblap_p);

				switch(s_lblap_p->ctl_sv)
				{
					case LBD_ICTL:
					case LBD_RRCTL:
					case LBD_RNRCTL:
					case LBD_REJCTL:
					case LBD_SBMCTL:
					case LBD_DISCCTL:
					case LBD_DMCTL:
					case LBD_UACTL:
					case LBD_FRMRCTL:
						cond = s_lblap_p->ctl_sv;
						break;
					case LBD_ERRORW	:
					case LBD_ERRORX:
					case LBD_ERRORY:
/*					case LBD_ERRORZ:		*/
						cond = LBD_ERROR;
						break;
					default:
						cond = LBD_OTHERS;
						/*abort or log error imformation*/
				}
			}
			else
			{
				cond=LBD_ICTL;
			}
			break;
														/* L2 DEACT依頼			*/
		case CMD_TSKIF_L2DEACTREQ:
			cond = LBD_L2DEACTREQ;
			break;
														/* データリンク設定要求	*/
		case CMD_TSKIF_ESTREQ:
			cond = LBD_ESTREQ;
			break;
														/* データリンク解放要求	*/
		case CMD_TSKIF_RELREQ:
			cond = LBD_RELREQ;
			break;
														/* 自受信ビジー			*/
		case CMD_TSKIF_L2BSY:
			cond = LBD_L2BSY;
			break;
														/* 自受信ビジーの解除	*/
		case CMD_TSKIF_L2BSYCLR:
			cond = LBD_L2BSYCLR;
			break;
														/* L2 STOP依頼			*/
		case CMD_TSKIF_L2STPREQ:
			cond = LBD_L2STPREQ;
			break;
														/* L3 test 開始通知		*/
		case CMD_TSKIF_L3MODESTRNTC:
			cond = LBD_L3TESTSTR;
			break;
														/* L3 test 停止通知		*/
		case CMD_TSKIF_L3MODESTPNTC:
			cond = LBD_L3TESTSTP;
			break;
		default:
			cm_Assert(CMD_ASL_USELOW, CMD_NUM0, "tm_lb_trans CPRI_Sig_ABNORMAL");
			cond = LBD_OTHERS;
	}
	
	return(cond);
	
}


/* @} */

