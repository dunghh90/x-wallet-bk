/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_mk_L3linkStat_ChgNfy.c
 *  @brief  l3 Link status change notification process
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

/********************************************************************************************************************/
/**
 *  @brief    Master L3 link status change notification process
 *  @note     Master L3 link status
 *  @param    inf_p		[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2013/11/21 FFCS)zhaodx create for zynq
 */
/********************************************************************************************************************/

VOID m_mk_L3linkStat_ChgNfyRE(UINT *inf_p)
{
	CMT_TSKIF_L3STATENTC	 	*l3stantc_p;	/* Pointer of L3リンクstate通知 IF*/
	USHORT						link_idx;
	USHORT						mtIndx;
	USHORT						mLnkIndx;
	UINT					    ret_buf;		/* Return value of buffer request*/
	
	l3stantc_p = (CMT_TSKIF_L3STATENTC *)inf_p;
	link_idx = l3stantc_p->link_num-1;
	mtIndx = link_idx / 6;
	mLnkIndx = link_idx % 6;
	/*	通知情報をテーブルに格納する	*/
	if(l3stantc_p->system_type == CMD_SYS_S3G)
	{
		mkw_cpristatM[mtIndx].stainfo[mLnkIndx].hcsta = l3stantc_p->onOff;
	}
	else
	{
		BPF_COM_LOG_ASSERT(CMD_ASL_DBGLOW, "MasterCPRI HC STS CHG:sys=%d:lnk=%d:sts:%d/val=%d", 
		l3stantc_p->system_type, l3stantc_p->link_num, l3stantc_p->onOff);
		return;
	}
	/*	CPRI-MTへの通知メッセージを作成	*/
	m_mk_cpristamsg_ntc(&mkw_cpristatM[mtIndx], l3stantc_p->link_num);
	
	/*	Lnsタスクに通知した場合はここでbuffer解放が必要	*/
	if(mkw_needfrmsg == MKD_FALSE)		/* If need free msg buffer and pointer not null	*/
	{
		ret_buf = cm_BFree(inf_p);

		if(ret_buf != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGLOW, ret_buf, "[m_mk_L3linkStat_ChgNfyRE] cm_BFree NG");
		}
	}
}

/********************************************************************************************************************/
/**
 *  @brief    L3 link status change notification process
 *  @note     Save L3 link status
 *  @param    inf_p		[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2013/11/21 FFCS)zhaodx create for zynq
 */
/********************************************************************************************************************/

VOID m_mk_L3linkStat_ChgNfy(UINT *inf_p)
{
	CMT_TSKIF_L3STATENTC	 	*l3stantc_p;	/* Pointer of L3リンクstate通知 IF*/
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
	USHORT	hcget[2][2] = {{CMD_SYS_3G, CMD_SYS_S3G},{CMD_SYS_S3G, CMD_SYS_3G}};
	USHORT	hcstaOwn;
	USHORT	hcstaOther;

	l3stantc_p = (CMT_TSKIF_L3STATENTC *)inf_p;
	/*	現状は、Slave linkのみ対応のため	*/
	if(l3stantc_p->link_num != D_RRH_CPRINO_REC)
	{
#ifndef OPT_RRH_ZYNQ_REC
		m_mk_L3linkStat_ChgNfyRE(inf_p);
#else
		cm_Assert(CMD_ASL_DBGLOW, l3stantc_p->link_num, "link no NG" );
#endif
		return;
	}
	tcpconn = mnt_com_get_tcpconn_monitor(l3stantc_p->link_num);
	
	/*	自システムのチェックサム状態を作成	*/
	hcstaOwn   = l3stantc_p->onOff << hcget[l3stantc_p->system_type][0];
	/*	他システムのチェックサム状態を取得	*/
	hcstaOther = mkw_cpristat.hcsta & (CMD_ON<<hcget[l3stantc_p->system_type][1]);
	
	mkw_cpristat.hcsta = hcstaOwn | hcstaOther;
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
		/*cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_mk_L3linkStat_ChgNfy] Connect mode NG");*/
		return;
	}

	ret_buf = cm_BReq(CMD_BUFCA_TSKIF, databyte_leng + sizeof(CMT_TSKIF_HEAD), (VOID **)&sndmsg_p);

	if(ret_buf != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, 
			"m_mk_L3linkStat_ChgNfy", 
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
#endif

	/* set timestamp */
	cm_RtcGet(&logtime);
	cm_MemCpy(&cpristat.timestamp,&logtime,sizeof(cpristat.timestamp));
	cm_MemCpy(&(sndmsg_p->rtc_rsp), 
					&(cpristat.timestamp),
						msglen);

	sndmsg_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + databyte_leng;

	/*Start Modify */
	m_mk_cm_MonDatProc(MKD_CPRILNKSTANTC, sndmsg_p, l3stantc_p->link_num);

	ret_buf = cm_BFree(inf_p);

	if(ret_buf != CMD_RES_OK)
	{
		cm_MAbort( CMD_ALMCD_BUFREL, 
					"m_mk_L3linkStat_ChgNfy", 
				   	"cm_BFree NG", 
				 	  ret_buf, 
				 	  (UINT)inf_p, 
				 	  CMD_NUM0);			
	}
}

/* @} */
