/*!
 * @skip    $Id$
 * @file    m_lb_dstrbtl3msg.c
 * @brief   distribute L3 message処理関数(tm_lb_DstrbtL3Msg)
 * @date    2008.07.22 FFCS)Shihzh create.
 * @date    2013.11.18 FFCS)hongj modify for zynq.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/


/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "f_rrh_def.h"
#include "m_cm_header.h"
#include "tm_lb_header.h"
#include "m_lb_header.h"
#include "rrhApi_L2.h"
/*!
 *  @brief   distribute L3 message処理関数(tm_lb_DstrbtL3Msg)
 *  @note    Analyse L3 message code, and send received I frame to proper L3 task.
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date   2015/8/10 TDIPS)ikeda rev.27999 LPB 17リンク修正
 */
void	tm_lb_DstrbtL3Msg(struct LBT_MNGTBL * s_lblap_p)
{
	CMT_TSKIF_CPRIRCV_SIGNALGET	 *l3msgno_p;				/*!< */
	USHORT		signal_kind;															/*!< */
	UINT		loop;										/*!< */
	USHORT	linkno;

	l3msgno_p	= (CMT_TSKIF_CPRIRCV_SIGNALGET *)s_lblap_p->tx_buf_adr.lap_buf_p;
	signal_kind = (l3msgno_p->signal_kind) & CMD_SYS_MSK_CPRID;
	linkno		= s_lblap_p->link_num;
		
	for(loop = CMD_NUM0; loop < sizeof(lbw_L3DstrbtTbl[0]) / sizeof(lbw_L3DstrbtTbl[0][0]); loop++)
	{
		if(lbw_L3DstrbtTbl[linkno][loop][CMD_NUM0] == signal_kind)
		{
			s_lblap_p->tx_buf_adr.lap_buf_p->length -= CMD_NUM2;

			l3msgno_p->head.uiEventNo	  = D_API_MSGID_L2_CPRIMSG_RCV_NTC;
			l3msgno_p->head.uiSignalkind  = signal_kind;
			l3msgno_p->head.uiDstPQueueID = lbw_L3DstrbtTbl[linkno][loop][CMD_NUM1];
			l3msgno_p->head.uiDstTQueueID = 0;
			l3msgno_p->head.uiSrcPQueueID = D_RRH_PROCQUE_L2;
			l3msgno_p->head.uiSrcTQueueID = CMD_TSKID_LPB;
			l3msgno_p->head.uiLength	  = sizeof(LBT_TSKIF_CPRICOM) + s_lblap_p->tx_buf_adr.lap_buf_p->length;
													/* LAPB走行履歴書込み	*/
			tm_lb_RunHisSet(LBD_RUNHIS_FLG_SND, 
							lbw_L3DstrbtTbl[linkno][loop][CMD_NUM1],
							CMD_TSKIF_CPRIRCVNTC, 
							(UINT *)s_lblap_p->tx_buf_adr.lap_buf_p);
			l2_com_cpriMsgToApl(lbw_L3DstrbtTbl[linkno][loop][CMD_NUM1],
							(VOID *)l3msgno_p,
							l3msgno_p->head.uiLength);

			lbw_keepbufflg = CMD_ON;
													/*-----------------------*/
													/* 統計カウント処理 	 */
													/*	 (正常受信回数) 	 */
													/*-----------------------*/
//			lblap_statis(s_lblap_p->link_num, LBD_L2_DRV);
			return;
		}
	}
	/*error log imformation*/
	cm_Assert(CMD_ASL_USELOW,l3msgno_p->cprircv_inf.length, "CPRI_Sig_ABNORMAL");
	cm_Assert(CMD_ASL_USELOW,
				(UINT)(((UINT)(l3msgno_p->cprircv_inf.Address << 24) & 0xFF000000)|
					((UINT)(l3msgno_p->cprircv_inf.Control << 16) & 0x00FF0000)|
					((UINT)(l3msgno_p->signal_kind) & 0x0000FFFF)	),
					"Add /Ctl/L3 Sig Kind");
	return;

}
/* @} */


