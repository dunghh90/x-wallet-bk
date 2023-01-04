/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_L2linkStat_ChgNfy.c
 *  @brief   Link status change notification process
 *  @date   2013/11/21 FFCS)zhaodx create for zynq
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "m_cm_header.h"			/*  RRH common head file							*/
#include "m_mk_header.h"			/*	Maker MT management task header 	*/
#include "rrhApi_L2.h"

/********************************************************************************************************************/
/**
 *  @brief    Master L2 link status change notification process
 *  @note     Master L2 link status
 *  @param  inf_p		[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2013/11/21 FFCS)zhaodx create for zynq
 */
/********************************************************************************************************************/

VOID m_mk_L2linkStat_ChgNfyRE(UINT *inf_p)
{
    T_API_L2_LINKSTATE_CHG_NTC  *l2stantc_p;	/* Pointer of L2リンクstate通知 IF*/
	USHORT						link_idx;
	USHORT						mtIndx;
	USHORT						mLnkIndx;
	UINT					    ret_buf;		/* Return value of buffer request*/
	
	l2stantc_p = (T_API_L2_LINKSTATE_CHG_NTC *)inf_p;
	
	link_idx = l2stantc_p->data.link_num-1;
	mtIndx = link_idx / 6;
	mLnkIndx = link_idx % 6;
	/*	通知情報をテーブルに格納する	*/
	if (l2stantc_p->data.state== D_API_L2_STACHG_EST)
	{
		/* normal */
		mkw_cpristatM[mtIndx].stainfo[mLnkIndx].L2abnminfo = CMD_OFF;
		mkw_cpristatM[mtIndx].stainfo[mLnkIndx].cprista = D_RRH_CPRISTAT_F;
	}
	else
	{
		/* abnormal */
		mkw_cpristatM[mtIndx].stainfo[mLnkIndx].L2abnminfo = CMD_ON;
	}
	
	/*	CPRI-MTへの通知メッセージを作成	*/
	m_mk_cpristamsg_ntc(&mkw_cpristatM[mtIndx], l2stantc_p->data.link_num);
	
	/*	Lnsタスクに通知した場合はここでbuffer解放が必要	*/
	if(mkw_needfrmsg == MKD_FALSE)		/* If need free msg buffer and pointer not null	*/
	{
		ret_buf = cm_BFree(inf_p);

		if(ret_buf != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGLOW, ret_buf, "[m_mk_L2linkStat_ChgNfyRE] cm_BFree NG");
		}
	}
}

/********************************************************************************************************************/
/**
 *  @brief    L2 link status change notification process
 *  @note     Save L2 link status
 *  @param  inf_p		[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2013/11/21 FFCS)zhaodx create for zynq
 */
/********************************************************************************************************************/

VOID m_mk_L2linkStat_ChgNfy(UINT *inf_p)
{
    T_API_L2_LINKSTATE_CHG_NTC  *l2stantc_p;	/* Pointer of L2リンクstate通知 IF*/
	CMT_TSKIF_TCPSNDREQ 	    *sndmsg_p;	/* Pointer of TCP data send request IF*/
	UINT					    ret_buf;		/* Return value of buffer request*/
	UINT					    msglen;			/* 送信メッセージの長さ			*/
	UINT 					    databyte_leng;	/* データ部バイト長				*/
#ifndef OPT_RRH_ZYNQ_REC
	MNT_COM_TSKIF_CPRISTANTC	cpristat;
#else
	MNT_COM_TSKIF_CPRISTANTC_REC	cpristat;
#endif
	T_RRH_SYSTEM_TIME	logtime;
	T_CMD_TCPCONN_NUM tcpconn;
	
	l2stantc_p = (T_API_L2_LINKSTATE_CHG_NTC *)inf_p;
	/*	現状は、Slave linkのみ対応のため	*/
	if(l2stantc_p->data.link_num != D_RRH_CPRINO_REC)
	{
#ifndef OPT_RRH_ZYNQ_REC
		m_mk_L2linkStat_ChgNfyRE(inf_p);
#else
		cm_Assert(CMD_ASL_DBGLOW, l2stantc_p->data.link_num, "link no NG" );
#endif
		return;
	}
	if (l2stantc_p->data.state== D_API_L2_STACHG_EST)
	{
		/* normal */
		mkw_cpristat.L2abnminfo = CMD_OFF;
		mkw_cpristat.cprista = D_RRH_CPRISTAT_F;
	}
	else
	{
		/* abnormal */
		mkw_cpristat.L2abnminfo = CMD_ON;
	}

	tcpconn = mnt_com_get_tcpconn_monitor(l2stantc_p->data.link_num);

	/* set re type */
	mkw_cpristat.retype = CMD_REKIND_OFTRX;
	/* selfslotno is fixed to 0 */
	mkw_cpristat.selfslotno = 1;
	/* TRA間active/negative情報を設定 */
	mkw_cpristat.traclsinfo = CMD_ACTIVE;
	
#ifndef OPT_RRH_ZYNQ_REC
	msglen = (UINT)(sizeof(MNT_COM_TSKIF_CPRISTANTC) - sizeof(CMT_TSKIF_HEAD));
#else
	msglen = (UINT)(sizeof(MNT_COM_TSKIF_CPRISTANTC_REC) - sizeof(CMT_TSKIF_HEAD));
#endif
	databyte_leng = msglen + sizeof(VOID *) + sizeof(INT) + sizeof(UINT) + sizeof(UINT);
	
	/* If not in monitor connection		*/	
	if(cmw_tblClientInfo[tcpconn].conn != MKD_CONN_Y)
	{
		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_mk_L2linkStat_ChgNfy] Connect mode NG");
		return;
	}

	ret_buf = cm_BReq(CMD_BUFCA_TSKIF, databyte_leng + sizeof(CMT_TSKIF_HEAD), (VOID **)&sndmsg_p);

	if(ret_buf != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, 
			"m_mk_L2linkStat_ChgNfy", 
			"Buffer request error", 
			ret_buf, 
			databyte_leng + sizeof(CMT_TSKIF_HEAD), 
			CMD_NUM0);
	}

	sndmsg_p->msgbody = NULL;
	sndmsg_p->linkhandle = cmw_tblClientInfo[tcpconn].tcplinkhdl;
	sndmsg_p->msglen = msglen;
	sndmsg_p->msgevent = MKD_CPRILNKSTANTC;	/* CPRIリンクstate通知			*/

	/* Copy time stamp and link state infomation	*/
	cm_MemCpy(&cpristat,&mkw_cpristat,sizeof(mkw_cpristat));
	cpristat.retype = mnt_com_swap_USHORT(cpristat.retype);
	cpristat.selfslotno = mnt_com_swap_USHORT(cpristat.selfslotno);
	cpristat.cprista = mnt_com_swap_USHORT(cpristat.cprista);
	cpristat.hdlcsta = mnt_com_swap_USHORT(cpristat.hdlcsta);
	cpristat.L2abnminfo = mnt_com_swap_USHORT(cpristat.L2abnminfo);
	cpristat.hcsta = mnt_com_swap_USHORT(cpristat.hcsta);
	cpristat.traclsinfo = mnt_com_swap_USHORT(cpristat.traclsinfo);
#ifdef OPT_RRH_ZYNQ_REC
	cpristat.cpriconf = mnt_com_swap_USHORT(1);
	cm_MemClr(cpristat.linkdummy,sizeof(cpristat.linkdummy));
#endif

	/* set timestamp */
	cm_RtcGet(&logtime);
	cm_MemCpy(&cpristat.timestamp,&logtime,sizeof(cpristat.timestamp));
	cpristat.timestamp.year = mnt_com_swap_USHORT(cpristat.timestamp.year);	
	cm_MemCpy(&(sndmsg_p->rtc_rsp), 
					&(cpristat.timestamp),
						msglen);

	sndmsg_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + databyte_leng;

	/*Start Modify */
	m_mk_cm_MonDatProc(MKD_CPRILNKSTANTC, sndmsg_p, l2stantc_p->data.link_num);

	ret_buf = cm_BFree(inf_p);

	if(ret_buf != CMD_RES_OK)
	{
		cm_MAbort( CMD_ALMCD_BUFREL, 
					"m_mk_L2linkStat_ChgNfy", 
				   	"cm_BFree NG", 
				 	  ret_buf, 
				 	  (UINT)inf_p, 
				 	  CMD_NUM0);			
	}
}

/* @} */
