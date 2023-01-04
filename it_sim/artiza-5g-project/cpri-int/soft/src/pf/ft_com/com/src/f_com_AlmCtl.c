/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cm_AlmCtl.c
 *  @brief	TRA common function - hardware alarm report & clear function 
 *  @date   2012/06/26 FFCS)Xut create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2012-
 */
/*****************************************************************************/

/** @addtogroup TRA_COM
 *  @{
 */
 /********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "f_sys_inc.h"			/* TRA common head file				*/
#include "f_com_inc.h"			/* TRA common head file				*/
#include <signal.h>
#include "f_com_rom.h"         /* 2021/01/19 M&C) Merge 4G FHM src (add) */

/*****************************************************************************/
/**
 *  @brief  TRA common function - Common process of Alarm report
 *  @note   Set alarm detailed factor and alarm register 
 *          according to alarm code \n
 *  @param  almcode			[in]		Alarm Code
 *  @return NONE 
 *  @retval NONE
 *  @date   2012/06/26		FFCS)Xut	new create
 *  @date 2021/01/19 M&C) Merge 4G FHM src
 *  @warning	N/A
 *  @FeatureID	RRH-011-011
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/*****************************************************************************/
UINT debug_halm = D_RRH_OFF;
VOID f_com_almReportFHM(UINT srcThrdId, UINT almcode )
{
	UINT loop;
	UINT factor;
	INT	rtn_bpf;
	INT	errcd;


#ifdef OPT_RRH_SIM
	printf("--------- Alarm Report----------\n");
	printf("almcode = %04x\n", almcode);
#endif

	/*  Control code = SET */
	f_comw_svStateLog.chgType = D_SYS_SVSTATE_SET;
	/* Initialize SV state log table */
	for(loop = D_SYS_NUM0; loop < (sizeof(f_comw_svStateLog.svState) / sizeof(f_comw_svStateLog.svState[0])); loop++)
	{
		f_comw_svStateLog.svState[loop] = D_SYS_NUM0;
	}

	/* Take semphore of SV control */
	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_SVCTL,
		BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, NULL, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
			"SV CTL sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}
	
	/* software alarm case */
	if(almcode <= D_RRH_ALMCD_REMOTEALM)
	{
		/************************/
		/* ソフトアラーム制御   */
		/************************/
		/* アラーム詳細要因格納テーブルにアラーム詳細要因設定 */
		/* SOFT_ALM番号設定レジスタ(0xD0000160)設定 */
		f_com_SVCtl_AlmCodeSet((UINT)almcode);
		/* Set SV state log table */
		f_comw_svStateLog.svState[D_SYS_NUM12] = (UINT)almcode;
	}
	else
	{
		for(loop = D_SYS_NUM0; loop < sizeof(f_comr_hardalm_idx_tbl) / sizeof(T_COM_HARDIDXTBL); loop++)
		{
			if(almcode == f_comr_hardalm_idx_tbl[loop].alm_no)
			{
				/* check the alarm is enable ? */
				if(*(UINT*)(f_comr_hardalm_idx_tbl[loop].alm_mask_addr) & f_comr_hardalm_idx_tbl[loop].svbit)
				{
					/*assert log*/
					f_com_assert(almcode , "alarm is masked");
					break;
				}

				/* write final factor into related register*/
				BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,
						f_comr_hardalm_idx_tbl[loop].alm_factor_addr, &factor);

                factor &= D_RRH_HWALM_HW_COM_MASK;
				factor |= f_comr_hardalm_idx_tbl[loop].svbit;

				BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,
						f_comr_hardalm_idx_tbl[loop].alm_factor_addr, &factor);

				/* Set SV state log table */
				f_comw_svStateLog.svState[D_SYS_NUM3] = f_comr_hardalm_idx_tbl[loop].svbit;

				break;
			}
		}
	}

	/* Give semphore of SV control */
	rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_SVCTL,
		BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
			"SV CTL sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}

	if(srcThrdId != D_SYS_THDID_PF_RSV)
	{
		/* pf_rsv以外のスレッドはSV状態変化履歴ログ取得対象外 */
		return;
	}

	/****************************/
	/* SV状態変化履歴ログ取得   */
	/****************************/
	f_com_SVLogSet(D_SYS_THDID_PF_RSV);
	
	return;
}

/*****************************************************************************/
/**
 *  @brief  TRA common function - Common process of Alarm report
 *  @note   Set alarm detailed factor and alarm register 
 *          according to alarm code \n
 *  @param  almcode			[in]		Alarm Code
 *  @return NONE 
 *  @retval NONE
 *  @date   2012/06/26		FFCS)Xut	new create
 *  @warning	N/A
 *  @FeatureID	RRH-011-011
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/*****************************************************************************/
VOID f_com_almReport(UINT srcThrdId, UINT almcode )
{
	f_com_abort( srcThrdId, almcode );
	return;
}

/* @} */
