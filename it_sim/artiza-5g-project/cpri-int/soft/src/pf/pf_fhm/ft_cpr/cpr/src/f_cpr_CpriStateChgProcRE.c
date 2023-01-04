/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_cpr_CpriStateChgProcRE.c
 *  @brief  CPRI state change interruption handle function
 *  @date   2015/07/30 TDIPS)ikeda create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_CPR
* @{ */

#include "f_sys_inc.h"
#include "f_cpr_inc.h"
#include "rrhApi_Cpri.h"
#include "BPF_RU_HRTM.h"

/**
* @brief CPRI state change interruption handle function
* @note CPRI state change interruption handle function.\n
* @param buff_adr [in] the buffer address pointer of received message
* @return None
* @date 2015/07/30 TDIPS)ikeda create
* @date 2015/11/15 TDI)satou M-RRU-ZSYS-01993 問処番号No.184 L1ERR ON制御追加
* @date 2015/11/28 TDI)satou IT3問処番号No.97
* @date 2015/12/03 TDI)satou IT3問処番号No.117
* @date 2016/11/18 FJT)ohashi 16B スロット数制限機能追加
* @warning	N/A
* @FeatureID	PF_Cpri-003-003-001
* @Bug_No	M-RRU-ZSYS-01993
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_cpr_CpriStateChgProcRE(UINT * buff_adr)
 {
	T_SYS_CPRIST_IRQNTC*		a_cpristachgntc_p;	/* input IF pointer */
	T_API_CPRILINK_STATE_NTC	apiNtc;
	UINT						a_signal;
	UINT						a_num;
	UINT						a_cprista;	/* CPRI state */
	UINT						a_index;
	UINT						bChange = D_SYS_OFF;
	T_RRH_SYSTEM_TIME			logtime;
	USHORT						a_link_num;
	UINT						regVal;
	T_RRH_L1STAT_TBL *a_cprw_l1StatTbl;
	T_RRH_L1LOSLOF_TBL	*a_cprw_l1LosLofTbl;

	/* initial */
	memset(&apiNtc, 0, sizeof(apiNtc));

	/* Get CPRI state from IF */
	a_cpristachgntc_p = (T_SYS_CPRIST_IRQNTC*)buff_adr;
	a_cprista = a_cpristachgntc_p->data1;
 
 	a_link_num = a_cpristachgntc_p->link_num;

	/* if HDLC ACT or (HDLC Idle and E Down off) or Status E */
	if( ((a_cprista & D_CPR_CPRISTA_BIT_HDLCACT) != D_SYS_NUM0) || 
		(((a_cprista & D_CPR_CPRISTA_BIT_HDLCIDLE) != D_SYS_NUM0) && 
		((a_cprista & D_CPR_CPRISTA_BIT_EDWN) == D_SYS_NUM0)) || 
		((a_cprista & D_CPR_CPRISTA_BIT_E) != D_SYS_NUM0) )
	{
		apiNtc.cpriEDown = D_RRH_OFF;
		apiNtc.cpriState = D_RRH_CPRISTAT_E;

		bChange = D_SYS_ON;

		/* stop timer */
		f_cprLinkUpTimerCtrl(a_link_num, D_RRH_OFF);

		/* L1 ERR off */
		f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_L1ERR(a_link_num), D_SYS_OFF);

		if ((a_cprista & D_CPR_CPRISTA_BIT_HDLCIDLE) == D_SYS_NUM0)
		{
			/* CPRI リンク障害表示クリア */
//			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPERR(a_link_num), &regVal);
//			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_RL1MNT(a_link_num), &regVal);
//			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPSTCHG(a_link_num), &regVal);
//			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPSYNC(a_link_num), &regVal);

			/* SFP状態表示クリア */
//			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CNTS_CPSFPSTA(a_link_num), &regVal);
		}

		a_signal = apiNtc.cpriState;

		/* bitrate 取得 */
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI,
				M_RRH_REG_CPRI_CPTXCW(a_link_num), &apiNtc.cpriHDLCBitrate);
		apiNtc.cpriHDLCBitrate &= D_RRH_HDLC_RATE_MASK;

		/* HDLC ACT */
		if((a_cprista & D_CPR_CPRISTA_BIT_HDLCACT)    != D_SYS_NUM0)
		{
			apiNtc.cpriHdlc = D_RRH_ON;
			a_signal = D_RRH_CPRISTAT_HDLCACT;
		}
		/* HDLC IDLE */
		else if( ((a_cprista & D_CPR_CPRISTA_BIT_HDLCIDLE) != D_SYS_NUM0) && 
				((a_cprista & D_CPR_CPRISTA_BIT_EDWN) == D_SYS_NUM0) )
		{
			a_signal = D_RRH_CPRISTAT_HDLCIDLE;
		}

	}
	else
	{
		if((a_cprista & D_CPR_CPRISTA_BIT_HDLCIDLE) == D_SYS_NUM0)
		{
			bChange = D_SYS_ON;
			apiNtc.cpriHdlc = D_RRH_OFF;

			if ( (a_cprista & D_CPR_CPRISTA_BIT_B)   != D_SYS_NUM0  )
			{
				apiNtc.cpriState = D_RRH_CPRISTAT_B;
			}

			if ( (a_cprista & D_CPR_CPRISTA_BIT_C)   != D_SYS_NUM0  )
			{
				apiNtc.cpriState = D_RRH_CPRISTAT_C;
			}

			if ( (a_cprista & D_CPR_CPRISTA_BIT_D)   != D_SYS_NUM0  )
			{
				apiNtc.cpriState = D_RRH_CPRISTAT_D;
			}
		}

		if((a_cprista & D_CPR_CPRISTA_BIT_EDWN) != D_SYS_NUM0)
		{
			bChange = D_SYS_ON;
			apiNtc.cpriEDown = D_RRH_ON;

            /* 送信系切替制御解除 */
            (VOID)BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_RRH_REG_DIS_LSWAP0, 1 << (a_link_num-1));
            (VOID)BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_RRH_REG_DIS_LSWAP1, 1 << (a_link_num-1));
            (VOID)BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_RRH_REG_DIS_WSWAP,  1 << (a_link_num-1));

			if ( (a_cprista & D_CPR_CPRISTA_BIT_D)   != D_SYS_NUM0  )
			{
				apiNtc.cpriState = D_RRH_CPRISTAT_D;
			}

			if ( (a_cprista & D_CPR_CPRISTA_BIT_C)   != D_SYS_NUM0  )
			{
				apiNtc.cpriState = D_RRH_CPRISTAT_C;
			}

			if ( (a_cprista & D_CPR_CPRISTA_BIT_B)   != D_SYS_NUM0  )
			{
				apiNtc.cpriState = D_RRH_CPRISTAT_B;
			}

			/* 配下REの使用/未使用 */
			T_RRH_CONN_DISC conn;
			f_cmn_com_conn_disk_get(&conn);
			if ( conn.conn_disc[a_link_num-1] )
			{
				/* slave State */
				BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, D_RRH_REG_CPRI_CPSTAT, &regVal);
                /* slave State がE以上 かつ オートネゴ完了済みなら */
                if ((regVal >= D_RRH_CPRISTAT_E) && (D_SYS_ON == f_cprw_autonego_complete_flag))
				{
					/* master port resync */
					/* 光ON＆タイマー起動 */
					f_cpr_CpriLinkStartupProcRE(a_link_num);
				}

	            /* L1 ERR on */
	            f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_L1ERR(a_link_num), D_SYS_ON);
//16B スロット数制限機能追加 start
			    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_SLOT_ERR(a_link_num)          , D_SYS_OFF);
			    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_DELAY_ERR(a_link_num)      , D_SYS_OFF);
			    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_DELAY_ERR(a_link_num)     , D_SYS_OFF);
			    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_RESTARTUP_ERR(a_link_num)  , D_SYS_OFF);
			    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_DL_NG_ERR(a_link_num)         , D_SYS_OFF);
			    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_CARR_RESET_ERR(a_link_num) , D_SYS_OFF);
			    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_CARR_RESET_ERR(a_link_num), D_SYS_OFF);
			    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_RESTARTUP_ERR(a_link_num) , D_SYS_OFF);
//16B スロット数制限機能追加 end
			}
			else
			{
				/* L1 ERR off */
				f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_L1ERR(a_link_num), D_SYS_OFF);
			}
		}

		a_signal = apiNtc.cpriState;
	}
	
	/*add log*/
	if(f_cprw_l1StatTbl)
	{
		a_cprw_l1StatTbl = &f_cprw_l1StatTbl[a_link_num];

		BPF_HM_DEVC_GETTIME(&logtime);
		a_index = a_cprw_l1StatTbl->index % D_RRH_L1_STATI_L1BLK_NUM;
		a_signal %= D_RRH_L1_STATI_L1SIG_NUM;
		if(a_cprw_l1StatTbl->blks[a_index][a_signal].firstTime_flag  == D_SYS_OFF)
		{
			a_cprw_l1StatTbl->blks[a_index][a_signal].singal_kind = a_signal;
			a_cprw_l1StatTbl->blks[a_index][a_signal].firstTime_flag = D_SYS_ON;
			memcpy(&a_cprw_l1StatTbl->blks[a_index][a_signal].firstTime,&logtime,sizeof(logtime));
		}
		memcpy(&a_cprw_l1StatTbl->blks[a_index][a_signal].lastTime,&logtime,sizeof(logtime));
		a_cprw_l1StatTbl->blks[a_index][a_signal].occur_counter++;
		if(a_cprw_l1StatTbl->blks[a_index][a_signal].occur_counter == 0)
		{
			a_cprw_l1StatTbl->blks[a_index][a_signal].overflow_counter++;
		}
	}

	/*add log*/
	if(f_cprw_l1LosLofTbl)
	{
		a_cprw_l1LosLofTbl = &f_cprw_l1LosLofTbl[a_link_num];

		BPF_HM_DEVC_GETTIME(&a_cprw_l1LosLofTbl->hisInfo[a_cprw_l1LosLofTbl->index % D_RRH_L1_LOSLOF_SIZE].time);
		a_cprw_l1LosLofTbl->hisInfo[a_cprw_l1LosLofTbl->index % D_RRH_L1_LOSLOF_SIZE].cpristat = a_cpristachgntc_p->data2;
		a_cprw_l1LosLofTbl->hisInfo[a_cprw_l1LosLofTbl->index % D_RRH_L1_LOSLOF_SIZE].cptrans  = a_cpristachgntc_p->data3;
		a_cprw_l1LosLofTbl->hisInfo[a_cprw_l1LosLofTbl->index % D_RRH_L1_LOSLOF_SIZE].irqvalue = a_cprista;
		a_cprw_l1LosLofTbl->index ++;
		a_cprw_l1LosLofTbl->index %= D_RRH_L1_LOSLOF_SIZE;
	}

	if(bChange)
	{
		/* send notice to apl */
		apiNtc.header.uiEventNo = D_API_MSGID_CPRI_STATE_STATE_NTC;
		apiNtc.header.uiLength = sizeof(apiNtc);
		apiNtc.link_num = a_link_num;

		for(a_num = 0; a_num < sizeof(f_cprw_cprStatChgNtc_QID) / sizeof(UINT);a_num++)
		{
			if(f_cprw_cprStatChgNtc_QID[a_num])
			{
				apiNtc.header.uiDstPQueueID = f_cprw_cprStatChgNtc_QID[a_num];
			
				/* write running history */
				f_cpr_cm_runHisSet(D_CPR_SENDIEVEFLG, (UCHAR*)&apiNtc);

				f_com_sendMsgToAplFHM(f_cprw_cprStatChgNtc_QID[ a_num], &apiNtc, sizeof(apiNtc),
					apiNtc.header.uiEventNo, D_SYS_THDQID_PF_CPRI);
			}
		}
	}

	return;
 }

/* @} */

