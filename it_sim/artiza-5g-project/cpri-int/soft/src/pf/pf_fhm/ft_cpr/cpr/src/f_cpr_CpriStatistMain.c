/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_cpr_CpriStatistMain.c
 *  @brief  HDLC Statistics Collect  function
 *  @date 2013/11/14 FFCS)niemsh create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_CPR
* @{ */

#include "f_sys_inc.h"
#include "f_cpr_inc.h"

/**
* @brief 	HDLC Statistics Collect Control process function
* @note 	HDLC Statistics Collect Control process function.\n
*
* @return 	None
* @date 	2013/11/14 FFCS)niemsh create
* @date		2015/07/01 ALPHA)ueda RRH-007-000 TDD-SRE(Zynq)
* @date     2015/07/15 ALPHA)ueda M-RRU-ZSYS-01645
* @date   2015/8/12 TDIPS)ikeda rev.28126 BPF�d�l�ύX�Ή�
* @date     2015/11/12 TDI)satou ハソ102103104-013
* @warning	N/A
* @FeatureID	PF_Cpri-001-001-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_cpr_CpriStatistMain()
{
	INT ret;
	INT errcd;
	UINT regVal;
	UINT count10ms[D_RRH_CPRINO_NUM];	
	UINT cpriStat_old[D_RRH_CPRINO_NUM], cpriStat;
	UINT cpriRegCPERR;
	UINT cpriRegLOSNUM;
	UINT cpriRegRL1MNT;
	UINT cpriRegLAERR1;
	UINT cpriRegLAERR2;
	UINT cpriRegLAERR3;
	UINT regPllSt;
	UINT regSfpSt;
	UINT cpriRegCpstChg;
	UINT cpriRegCpSync;
	UCHAR sfp[64];
	UINT hfnsyc;
	T_RRH_SYSTEM_TIME  logtime;
	T_RRH_L1ERR_INFO* ctbl_p;
	T_RRH_L1ERR_CRNTINFO* itbl_p;
	UINT count_index[D_RRH_CPRINO_NUM];
	UINT info_index[D_RRH_CPRINO_NUM];
	USHORT linkno;
	T_RRH_CONN_DISC conn;
	T_RRH_CONN_DISC conn_old;
	UINT rxBuff;	/*	★調査用ログ強化★	*/
	UINT txBuff;	/*	★調査用ログ強化★	*/

	prctl(PR_SET_NAME, "pf_cprstat", 0, 0, 0);

	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_CPRI_L1_LINK_LOG,
										(VOID**)&f_cprw_l1ErrTbl, &errcd);
	if(ret != BPF_RU_IPCM_OK)
	{
		f_com_assert(ret, "BPF_RU_IPCM_PROCSHM_ADDRGET error");
		return;
	}

	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++ )
	{
		f_cprw_l1ErrTbl[linkno].count_Idx++;
		f_cprw_l1ErrTbl[linkno].count_Idx %= D_RRH_L1_ERR_COUNT_SIZE;
		f_cprw_l1ErrTbl[linkno].Info_Idx %= D_RRH_L1_ERR_INFO_SIZE;
		count_index[linkno] = f_cprw_l1ErrTbl[linkno].count_Idx;
		info_index[linkno] = f_cprw_l1ErrTbl[linkno].Info_Idx;
		ctbl_p = &(f_cprw_l1ErrTbl[linkno].countTbl[count_index[linkno]]);
		memset( ctbl_p , 0 ,sizeof(*ctbl_p));

		BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPSTAT(linkno), &cpriStat);

		BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPERR(linkno), &cpriRegCPERR);
		BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_LOSNUM(linkno), &cpriRegLOSNUM);
		BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_RL1MNT(linkno), &cpriRegRL1MNT);

		BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CM_LAERR1(linkno), &cpriRegLAERR1);
		BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CM_LAERR2(linkno), &cpriRegLAERR2);
		BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CM_LAERR3(linkno), &cpriRegLAERR3);
		BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPST(linkno), &hfnsyc);

		BPF_COM_PLOG_TRACE_CPRI(E_TRC_LV_INFO,
			(const char*)"CPRI INIT ERR s:%08X,f1:%08X,f2:%08X,f3:%08X,h1:%08X,h2:%08X,h3:%08X,hs:%08X",
			cpriStat,cpriRegCPERR,cpriRegLOSNUM,cpriRegRL1MNT,cpriRegLAERR1,cpriRegLAERR2,cpriRegLAERR3,hfnsyc );

		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPMSK(linkno), &regVal);
		regVal &= 0xFFFFFE00;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPRI_CPMSK(linkno), &regVal);

		regVal = 1;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CM_LAPMES(linkno), &regVal);

		cpriStat_old[linkno] = cpriStat;
		count10ms[linkno] = 0;
	}
	/*	旧CPRI使用/未使用を初期化する	*/
	f_cmn_com_conn_disk_get(&conn_old);
#if 0
	/* UP Buffer phase initial */
//	BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE_CPRI,
//									D_RRH_REG_UP_INIJB,
//									0x00000001);
#endif
	BPF_HM_DEVC_GETTIME(&(ctbl_p->firstTime));
	for(;;)
	{
		/*	現CPRI使用/未使用を取得する・*/
		f_cmn_com_conn_disk_get(&conn);
		
		for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++ )
		{
			cpriStat = 0;
			cpriRegCPERR = 0;
			cpriRegLOSNUM = 0;
			cpriRegRL1MNT = 0;
			cpriRegLAERR1 = 0;
			cpriRegLAERR2 = 0;
			cpriRegLAERR3 = 0;
			regPllSt = 0;
			regSfpSt = 0;
			cpriRegCpstChg = 0;
			cpriRegCpSync = 0;
			memset(&logtime,0,sizeof(logtime));
			ctbl_p = &(f_cprw_l1ErrTbl[linkno].countTbl[count_index[linkno]]);
			itbl_p = &(f_cprw_l1ErrTbl[linkno].infoTbl[info_index[linkno]]);

			/*	MasterPortの場合	*/
			if(linkno != D_RRH_CPRINO_REC)
			{
				/*	既に減設済み(CPRI未使用->未使用)の場合	*/
				if((conn.conn_disc[linkno-1] == CMD_OFF)&&(conn_old.conn_disc[linkno-1] == CMD_OFF))
				{
					/*	対象CPRIリンクのログ取得しない	*/
					continue;
				}
				else if((conn.conn_disc[linkno-1] == CMD_OFF)&&(conn_old.conn_disc[linkno-1] == CMD_ON))
				{
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "log stop link=%d index=%d", linkno, info_index[linkno] );
				}
				else if((conn.conn_disc[linkno-1] == CMD_ON)&&(conn_old.conn_disc[linkno-1] == CMD_OFF))
				{
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "log start link=%d index=%d", linkno, info_index[linkno] );
				}
				/*	旧CPRI使用/未使用を現CPRI使用/未使用で更新する	*/
				conn_old.conn_disc[linkno-1] = conn.conn_disc[linkno-1];
			}
			BPF_HM_DEVC_GETTIME(&logtime);
			/*      0x8100_0004 0x8100_XX00+0x004 XX=0x10~0x88 */
			BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPSTAT(linkno), &cpriStat);
			/* CHK1 0x8100_0100 0x8100_XX00+0x100 XX=0x10~0x88 */
			BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPERR(linkno), &cpriRegCPERR);
			/* CHK2 0x8100_0108 0x8100_XX00+0x108 XX=0x10~0x88 */
			BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_LOSNUM(linkno), &cpriRegLOSNUM);
			/* CHK3 0x8100_0110 0x8100_XX00+0x110 XX=0x10~0x88 */
			BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_RL1MNT(linkno), &cpriRegRL1MNT);
			/* CHK4 0x8101_0090 0x8101_XX00+0x090 XX=0x10~0x88 */
			BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CM_LAERR1(linkno), &cpriRegLAERR1);
			/* CHK4 0x8101_0094 0x8101_XX00+0x094 XX=0x10~0x88 */
			BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CM_LAERR2(linkno), &cpriRegLAERR2);
			/* CHK4 0x8101_0098 0x8101_XX00+0x098 XX=0x10~0x88 */
			BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CM_LAERR3(linkno), &cpriRegLAERR3);
			/*      0x8100_0120 0x8100_XX00+0x120 XX=0x10~0x88 */
			BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPST(linkno), &hfnsyc);

			/*state E or now is Edown, preivous is stateE*/
			if( (cpriStat >= D_RRH_CPRISTAT_E) ||
				((cpriStat < D_RRH_CPRISTAT_E) && (cpriStat_old[linkno] >= D_RRH_CPRISTAT_E)) )
			{
				/* check register changed*/
				if( (cpriRegCPERR != 0)  || (cpriRegLOSNUM != 0) || (cpriRegRL1MNT != 0) ||
					(cpriRegLAERR1 != 0) || (cpriRegLAERR2 != 0) || (cpriRegLAERR3 != 0) )
				{
				    /* 0x4000_0200 */
				    BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, D_RRH_REG_CNT_STA1, &regPllSt);
				    /* 0x8000_0500 0x8000_0XX0 XX=0x51~0x60 */
//				    BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CNT_SFP_ST(linkno), &regSfpSt);
					BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CNTS_CPSFPSTA(linkno), &regSfpSt);

					memset(sfp,0,sizeof(sfp));
					/* CPSおよびCPM#n SFPが実装の場合 */
					if (regSfpSt & D_RRH_REG_CNTS_CPSFPSTA_OPTMNT)
					{
						/*	SlavePortまたは使用中MasterPortの場合	*/
						if( (linkno == D_RRH_CPRINO_REC) ||
							((linkno != D_RRH_CPRINO_REC)&&( conn.conn_disc[linkno-1] == CMD_ON)) )
						{
							/*	SFP情報を取得	*/
							if(( BPF_HM_DEVC_SFP_READ_BUFFER(
									(unsigned int)BPF_HM_DEVC_SFP_A2, (unsigned short)0x38, sizeof(sfp), sfp) )
								 != BPF_HM_DEVC_COMPLETE)
							{
								memset(sfp,0xFF,sizeof(sfp));	/* NG */
							}
						}
						/*	未使用MasterPortの場合	*/
						else
						{
							memset(sfp,0xAA,sizeof(sfp));	/* 減設 */
						}
					}
					/*	未実装	*/
					else
					{
							/* nop(All"0") */
					}
					
					/* 0x8100_0118 0x8100_XX00+0x118 */
					BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPSTCHG(linkno), &cpriRegCpstChg);
					/* 0x8100_0124 0x8100_XX00+0x124 */
					BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPSYNC(linkno), &cpriRegCpSync);
					/* 0x8101_0310 0x8100_XX00+0x310★調査用ログ強化★ */
					BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPRXJB(linkno), &rxBuff);
					/* 0x8101_0314 0x8100_XX00+0x314★調査用ログ強化★ */
					BPF_HM_DEVC_REG_READ_NOMUTEX(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPTXJB(linkno), &txBuff);

					/* save old/new cpri state */
					itbl_p->state = (UINT)((UINT)(( cpriStat_old[linkno] << 16) & 0x00FF0000 ) | (UINT)( cpriStat & 0x000000FF));
					itbl_p->cpeErr = cpriRegCPERR;
					itbl_p->los = cpriRegLOSNUM;
					itbl_p->rl1Inb = cpriRegRL1MNT;
					itbl_p->hdlcErr1= cpriRegLAERR1;
					itbl_p->hdlcErr2 = cpriRegLAERR2;
					itbl_p->hdlcErr3 = cpriRegLAERR3;
					itbl_p->hfnsync = hfnsyc;
					itbl_p->pllSt   = regPllSt;
					itbl_p->sfpSt   = regSfpSt;
					itbl_p->cpstChg = cpriRegCpstChg;
					itbl_p->cpSync  = cpriRegCpSync;
					itbl_p->rxBuff  = rxBuff;	/*	★調査用ログ強化★	*/
					itbl_p->txBuff  = txBuff;	/*	★調査用ログ強化★	*/
					memcpy( itbl_p->sfp,sfp,sizeof(sfp));
					memcpy( &(itbl_p->time),&logtime,sizeof(logtime));
					info_index[linkno]++;
					info_index[linkno] %= D_RRH_L1_ERR_INFO_SIZE;
					f_cprw_l1ErrTbl[linkno].Info_Idx = info_index[linkno];
				}
			}
			/*	CPRIステータスがStateF以下	*/
			if( cpriStat <= D_RRH_CPRISTAT_F )
			{
				/*	CPRIステータスがStateE以上で継続の場合	*/
				if(
					(( cpriStat == D_RRH_CPRISTAT_E ) || ( cpriStat == D_RRH_CPRISTAT_F)) &&
					( cpriStat_old[linkno] >= D_RRH_CPRISTAT_E )
				)
				{
					ctbl_p->cpeErr     |= cpriRegCPERR ;        /* bit */
					ctbl_p->los        += cpriRegLOSNUM;
					ctbl_p->rl1Inb     |= cpriRegRL1MNT;        /* bit */
					ctbl_p->hdlcErr1   += cpriRegLAERR1;
					ctbl_p->hdlcErr2   += cpriRegLAERR2;
					ctbl_p->hdlcErr3   += cpriRegLAERR3;
				}
				ctbl_p->stateErr[cpriStat].count++;
				memcpy(&(ctbl_p->stateErr[cpriStat].lastTime),&logtime,sizeof(logtime));
			}

			cpriStat_old[linkno] = cpriStat;
			/* 10min overまたは減設MasterPortの場合 */
			if(( count10ms[linkno] >= D_RRH_L1_ERR_INFO_SIZE ) ||
				((linkno != D_RRH_CPRINO_REC)&&( conn.conn_disc[linkno-1] == CMD_OFF)) )
			{
				count_index[linkno]++;
				count_index[linkno] %= D_RRH_L1_ERR_COUNT_SIZE;
				ctbl_p = &(f_cprw_l1ErrTbl[linkno].countTbl[count_index[linkno]]);
				memset( ctbl_p , 0 ,sizeof(*ctbl_p));
				memcpy( &(ctbl_p->firstTime),&logtime,sizeof(logtime));
				f_cprw_l1ErrTbl[linkno].count_Idx = count_index[linkno];
				count10ms[linkno] = 0;
			}
			else
			{
				count10ms[linkno]++;
			}
		}

		/* sleep 1000ms*/
		f_com_taskDelay(1000,D_SYS_THDID_PF_CPRI,0,0);
	}
	
	return;
}

/* @} */

