/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_Init.c
 *  @brief  CPRI management task init function
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2009/08/08 FFCS)Wangjuan modify for 
 *							  CR-00054-005(CPRI spec V1.0.8) 
 *  @date   2012/07/05  FFCS)niemsh modify for 1.5GVA
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_CPR
* @{ */

#include "f_sys_inc.h"
#include "f_cpr_inc.h"

/**
* @brief 	CPRI management task init function
* @note 	init CPRI management task related common table.\n
* 
* @return None
* @date 		2013/11/14 FFCS)niemsh create
* @date    2015.08.11 TDIPS)ikeda 17リンク対応
* @date    2015.09.03 TDIPS)ikeda SFPコード読み込み修正
* @warning	N/A
* @FeatureID	PF_Cpri-001-001-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_cpr_Init()
{
	/**********************************************/
	/* Local parameter declaration				  */
	/**********************************************/
	INT ret;
	INT errcd;
	T_SYS_INIT_COMP_NTC tInitCompNTC;
	UINT index;
	USHORT linkno;

	/**********************************************/
	/* Local parameter initialization			  */
	/**********************************************/
	/* initialize the external event number*/
	f_cprw_extEventNo = D_SYS_NUM0;
	
	/* Change thread state into init */
	f_cprw_thrdState = D_CPR_THRDSTA_INIT;

	/* Auto Nego Complete flag init */
	f_cprw_autonego_complete_flag = D_SYS_OFF;

	/* real CPRI Bit Rate init */
	f_cprw_bitrate_real = D_COM_LINE_BITRATE_98G;

	memset(f_cprw_cprStatChgNtc_QID,0,sizeof(f_cprw_cprStatChgNtc_QID));
	memset(f_cprw_runhis_tbl,0,sizeof(f_cprw_runhis_tbl));
	f_cprw_l1StatTbl = 0;
	f_cprw_l1ErrTbl = 0;
	f_cprw_l1LosLofTbl = 0;
	/************************************************************/
	/* Task Init completion notice								*/
	/************************************************************/	
	memset(&tInitCompNTC,0,sizeof(tInitCompNTC));
	/* 1. Edit the message header		*/
	tInitCompNTC.head.uiEventNo = D_SYS_MSGID_INIT_COMP_NTC;
 	tInitCompNTC.head.uiLength = sizeof(tInitCompNTC);

	/* write running history */
	f_cpr_cm_runHisSet(D_CPR_SENDIEVEFLG, (UCHAR*)&tInitCompNTC);

	/* 2. Send Task Init completion notice	*/
	f_com_msgQSendFHM( D_SYS_THDQID_PF_F_MAIN, 
					(CHAR*)&tInitCompNTC,
					sizeof( T_SYS_INIT_COMP_NTC ), 
					D_SYS_THDQID_PF_CPRI, 
					f_cprw_thrdState, 
					tInitCompNTC.head.uiEventNo);
	
	/* Change thread state into running */
	f_cprw_thrdState = D_CPR_THRDSTA_RUN;	  

	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_CPRI_L1_STAT_INFO,
	(VOID**)&f_cprw_l1StatTbl,&errcd);
	if(ret != BPF_RU_IPCM_OK)
	{
		f_com_assert(ret, "BPF_RU_IPCM_PROCSHM_ADDRGET error");
		return;
	}

	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++)
	{
		f_cprw_l1StatTbl[linkno].index++;
		index = f_cprw_l1StatTbl[linkno].index % D_RRH_L1_STATI_L1BLK_NUM;
		memset(f_cprw_l1StatTbl[linkno].blks[index],0,sizeof(f_cprw_l1StatTbl[linkno].blks[index]));
		f_cprw_l1StatTbl[linkno].index = index;
	}

	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_CPRI_L1_LOSLOF_HIS,
	(VOID**)&f_cprw_l1LosLofTbl,&errcd);
	if(ret != BPF_RU_IPCM_OK)
	{
		f_com_assert(ret, "BPF_RU_IPCM_PROCSHM_ADDRGET error");
		return;
	}

	return;
}
/* @} */

