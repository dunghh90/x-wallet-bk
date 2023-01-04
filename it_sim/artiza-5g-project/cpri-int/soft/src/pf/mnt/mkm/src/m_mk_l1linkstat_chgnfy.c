/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_mk_L1linkstat_chgnfy.c
 *  @brief  Link status change notification process
 *  @date   2008/07/25 FFCS)zhengmh create
 *  @date   2013/11/25 FFCS)zhaodx modify for zynq
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_MKM
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "m_cm_header.h"			/*  TRA common head file							*/
#include "m_mk_header.h"			/*	Maker MT management task header 	*/
#include "rrhApi_Cpri.h"
/********************************************************************************************************************/
/**
 *  @brief     set system type
 *  @note     set system type according to bitrate
 *  @param  inf_p		[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2014/12/25 FFCS)wuqy modify for zynq
 */
/********************************************************************************************************************/
VOID m_mk_L1linkstat_setSysType(UINT *inf_p)
{
	//TDD SRE only support lte
	cmw_validsysinfo = CMD_SYS_S3G;
}

/********************************************************************************************************************/
/**
 *  @brief     Link status change notification process
 *  @note     link status change infomation to PC through LAN tx task
 *  @param  inf_p		[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2016/01/13 FJT)koshida create
 */
/********************************************************************************************************************/

VOID m_mk_cpristamsg_ntc(MNT_COM_TSKIF_CPRISTANTC_REC *cpristatTbl_p, USHORT link_num)
{
	CMT_TSKIF_TCPSNDREQ 	    *sndmsg_p;	/* Pointer of TCP data send request IF*/
	MNT_COM_TSKIF_CPRISTANTC_REC	cpristat;
	UINT					    ret_buf;		/* Return value of buffer request*/
	UINT					    msglen;			/* 送信メッセージの長さ			*/
	UINT 					    databyte_leng;	/* データ部バイト長				*/
	T_CMD_TCPCONN_NUM tcpconn;
	T_RRH_SYSTEM_TIME	logtime;
	USHORT						link_idx;
	
	tcpconn = mnt_com_get_tcpconn_monitor(link_num);
	/* If not in monitor connection		*/	
	if(cmw_tblClientInfo[tcpconn].conn != MKD_CONN_Y)
	{
//		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_mk_L2linkStat_ChgNfy] Connect mode NG");
		return;
	}
	sndmsg_p = NULL;
	ret_buf = CMD_NUM0;
	
	msglen = (UINT)(sizeof(MNT_COM_TSKIF_CPRISTANTC_REC) - sizeof(CMT_TSKIF_HEAD));
	databyte_leng = msglen + sizeof(VOID *) + sizeof(INT) + sizeof(UINT) + sizeof(UINT);

	ret_buf = cm_BReq(CMD_BUFCA_TSKIF, databyte_leng + sizeof(CMT_TSKIF_HEAD), (VOID **)&sndmsg_p);

	if(ret_buf != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, 
			"m_mk_L1linkstat_chgnfyRE", 
			"Buffer request error", 
			ret_buf, 
			databyte_leng + sizeof(CMT_TSKIF_HEAD), 
			CMD_NUM0);
	}

	sndmsg_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + databyte_leng;
	sndmsg_p->msgbody = NULL;
	sndmsg_p->linkhandle = cmw_tblClientInfo[tcpconn].tcplinkhdl;
	sndmsg_p->msglen = msglen;
	sndmsg_p->msgevent = MKD_CPRILNKSTANTC;	/* CPRIリンクstate通知			*/

	/* Copy time stamp and link state infomation	*/
	/* set timestamp */
	for(link_idx=0; link_idx<6; link_idx++)
	{
		cpristat.stainfo[link_idx].selfslotno = mnt_com_swap_USHORT(cpristatTbl_p->stainfo[link_idx].selfslotno);
		cpristat.stainfo[link_idx].retype = mnt_com_swap_USHORT(cpristatTbl_p->stainfo[link_idx].retype);
		cpristat.stainfo[link_idx].cpriconf = mnt_com_swap_USHORT(cpristatTbl_p->stainfo[link_idx].cpriconf);
		cpristat.stainfo[link_idx].cprista = mnt_com_swap_USHORT(cpristatTbl_p->stainfo[link_idx].cprista);
		cpristat.stainfo[link_idx].hdlcsta = mnt_com_swap_USHORT(cpristatTbl_p->stainfo[link_idx].hdlcsta);
		cpristat.stainfo[link_idx].L2abnminfo = mnt_com_swap_USHORT(cpristatTbl_p->stainfo[link_idx].L2abnminfo);
		cpristat.stainfo[link_idx].hcsta = mnt_com_swap_USHORT(cpristatTbl_p->stainfo[link_idx].hcsta);
		cpristat.stainfo[link_idx].traclsinfo = mnt_com_swap_USHORT(cpristatTbl_p->stainfo[link_idx].traclsinfo);
		cpristat.stainfo[link_idx].actswforbflg = mnt_com_swap_USHORT(cpristatTbl_p->stainfo[link_idx].actswforbflg);
		cpristat.stainfo[link_idx].dummy = 0;
	}
	cm_RtcGet(&logtime);
	cm_MemCpy(&cpristat.timestamp,&logtime,sizeof(cpristat.timestamp));	
	cpristat.timestamp.year = mnt_com_swap_USHORT(cpristat.timestamp.year);	
	cm_MemCpy(&(sndmsg_p->rtc_rsp), 
					&(cpristat.timestamp),
					msglen);
	
	/*Start Modify */
	m_mk_cm_MonDatProc(MKD_CPRILNKSTANTC, sndmsg_p, link_num);
}

/********************************************************************************************************************/
/**
 *  @brief    Master Link status change notification process
 *  @note     Master link status change infomation to PC through LAN tx task
 *  @param  inf_p		[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2016/01/13 FJT)koshida create
 */
/********************************************************************************************************************/

VOID m_mk_L1linkstat_chgnfyRE(UINT *inf_p)
{
	T_API_CPRILINK_STATE_NTC    *cpristantc_p;	/* Pointer of CPRIリンクstate通知 IF*/
	USHORT						link_idx;
	USHORT						mtIndx;
	USHORT						mLnkIndx;
	UINT					    ret_buf;		/* Return value of buffer request*/
	
	cpristantc_p = (T_API_CPRILINK_STATE_NTC *)inf_p;
	
	link_idx = cpristantc_p->link_num-1;
	mtIndx = link_idx / 6;
	mLnkIndx = link_idx % 6;
	/*	通知情報をテーブルに格納する	*/
	mkw_cpristatM[mtIndx].stainfo[mLnkIndx].cprista = cpristantc_p->cpriState;
	mkw_cpristatM[mtIndx].stainfo[mLnkIndx].hdlcsta = cpristantc_p->cpriHdlc;
	if((mkw_cpristatM[mtIndx].stainfo[mLnkIndx].cprista < D_RRH_CPRISTAT_E) ||
		 (mkw_cpristatM[mtIndx].stainfo[mLnkIndx].hdlcsta != CMD_ON))
	{
		mkw_cpristatM[mtIndx].stainfo[mLnkIndx].L2abnminfo = CMD_ON;
		mkw_cpristatM[mtIndx].stainfo[mLnkIndx].hcsta = CMD_OFF;
	}
	
	/*	CPRI-MTへの通知メッセージを作成	*/
	m_mk_cpristamsg_ntc(&mkw_cpristatM[mtIndx], cpristantc_p->link_num);
	
	/*	Lnsタスクに通知した場合はここでbuffer解放が必要	*/
	if(mkw_needfrmsg == MKD_FALSE)		/* If need free msg buffer and pointer not null	*/
	{
		ret_buf = cm_BFree(inf_p);

		if(ret_buf != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGLOW, ret_buf, "[m_mk_L1linkstat_chgnfyRE] cm_BFree NG");
		}
	}
	return;
}

/********************************************************************************************************************/
/**
 *  @brief     Link status change notification process
 *  @note     Send link status change infomation to PC through LAN tx task
 *  @param  inf_p		[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/25 FFCS)zhaodx modify for zynq
 */
/********************************************************************************************************************/

VOID m_mk_L1linkstat_chgnfy(UINT *inf_p)
{
	CMT_TSKIF_TCPSNDREQ 	    *sndmsg_p;	/* Pointer of TCP data send request IF*/
	T_API_CPRILINK_STATE_NTC    *cpristantc_p;	/* Pointer of CPRIリンクstate通知 IF*/
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
	
	sndmsg_p = NULL;
	ret_buf = CMD_NUM0;
	cpristantc_p = (T_API_CPRILINK_STATE_NTC *)inf_p;
	tcpconn = mnt_com_get_tcpconn_monitor(cpristantc_p->link_num);
	/*	現状は、Slave linkのみ対応のため	*/
	if(cpristantc_p->link_num != D_RRH_CPRINO_REC)
	{
#ifndef OPT_RRH_ZYNQ_REC
		m_mk_L1linkstat_chgnfyRE(inf_p);
#else
		cm_Assert(CMD_ASL_DBGLOW, cpristantc_p->link_num, "link no NG" );
#endif
		return;
	}
	/* set re type */
	mkw_cpristat.retype = CMD_REKIND_OFTRX;
	/* selfslotno is fixed to 0 */
	mkw_cpristat.selfslotno = 1;
	/* set cpri state */
	mkw_cpristat.cprista = cpristantc_p->cpriState;
	/* set hdlc state */
	mkw_cpristat.hdlcsta = cpristantc_p->cpriHdlc;
	if((mkw_cpristat.cprista < D_RRH_CPRISTAT_E) || (mkw_cpristat.hdlcsta != CMD_ON))
	{
		mkw_cpristat.L2abnminfo = CMD_ON;
		mkw_cpristat.hcsta = CMD_OFF;
	}
	/* TRA間active/negative情報を設定 */
	mkw_cpristat.traclsinfo = CMD_ACTIVE;
	m_mk_L1linkstat_setSysType(inf_p);
#ifndef OPT_RRH_ZYNQ_REC
	msglen = (UINT)(sizeof(MNT_COM_TSKIF_CPRISTANTC) - sizeof(CMT_TSKIF_HEAD));
#else
	msglen = (UINT)(sizeof(MNT_COM_TSKIF_CPRISTANTC_REC) - sizeof(CMT_TSKIF_HEAD));
	cm_MemClr(cpristat.linkdummy,sizeof(cpristat.linkdummy));
#endif
	databyte_leng = msglen + sizeof(VOID *) + sizeof(INT) + sizeof(UINT) + sizeof(UINT);
	
	/* If not in monitor connection		*/	
	if(cmw_tblClientInfo[tcpconn].conn != MKD_CONN_Y)
	{
		cm_Assert(CMD_ASL_NOLEVEL, CMD_NUM0, "[m_mk_L1linkstat_chgnfy] Connect mode NG");
		return;
	}

	ret_buf = cm_BReq(CMD_BUFCA_TSKIF, databyte_leng + sizeof(CMT_TSKIF_HEAD), (VOID **)&sndmsg_p);

	if(ret_buf != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, 
			"m_mk_L1linkstat_chgnfy", 
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
	/* set timestamp */
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
	
	cm_RtcGet(&logtime);
	cm_MemCpy(&cpristat.timestamp,&logtime,sizeof(cpristat.timestamp));	
	cpristat.timestamp.year = mnt_com_swap_USHORT(cpristat.timestamp.year);	
	cm_MemCpy(&(sndmsg_p->rtc_rsp), 
					&(cpristat.timestamp),
					msglen);

	sndmsg_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + databyte_leng;

	/*Start Modify */
	m_mk_cm_MonDatProc(MKD_CPRILNKSTANTC, sndmsg_p, cpristantc_p->link_num);

	ret_buf = cm_BFree(inf_p);

	if(ret_buf != CMD_RES_OK)
	{
		cm_MAbort( CMD_ALMCD_BUFREL, 
					"m_mk_L1linkstat_chgnfy", 
				   	"cm_BFree NG", 
				 	  ret_buf, 
				 	  (UINT)inf_p, 
				 	  CMD_NUM0);			
	}
}

/* @} */
