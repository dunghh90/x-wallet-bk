/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_rsv_ErrSet_zynq.c
 *  @brief  API LED Setting Request process function
 *  @date   2013/11/08 FFCS)Xut create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_RSV
* @{ */
#include "f_rsv_header.h"			/* RE監視タスクヘッダファイル			*/

/**
* @brief Err control register set function
* @param  action_flg [in] set or clear flag, 0:clear 1:set
* @param  writeAdr [in] writing register address 
* @param  writeDat [in] register set data
* @return None
* @date   2013/11/25 FFCS)Xut create
* @date   2015/11/15 TDI)satou M-RRU-ZSYS-01993 問処番号No.184 未使用REのSV制御レジスタON設定を無効に
* @warning	N/A
* @FeatureID	PF-Svp-012-001-001
* @Bug_No	M-RRU-ZSYS-01993
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_cm_ErrRegSet( USHORT action_flg, UINT writeAdr, UINT writeDat )
{
	/* save register value to common table */
	if (action_flg == D_SYS_SET)
	{
		/* error set ON */
	    f_com_SVCtl_OtherProc(D_SYS_THDID_PF_RSV, writeAdr, 0, writeDat);
	}
	else
	{
		/* error clear */
	    f_com_SVCtl_OtherProc(D_SYS_THDID_PF_RSV, writeAdr, ~writeDat, 0);
	}

	return;
}

/**
* @brief API Error Setting Request process function
* @note Update Error control regiseter.\n
* @param errset_p [in] the buffer address pointer of received message
* @return None
* @date   2013/11/25 FFCS)Xut create
* @date   2015/11/15 TDI)satou M-RRU-ZSYS-01993 問処番号No.184 未使用REのSV制御レジスタON設定を無効に
* @warning	N/A
* @FeatureID	PF-Svp-012-001-001
* @Bug_No	M-RRU-ZSYS-01993
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_ErrSet( T_API_SVP_ERRSET_NTC* errset_p )
{
	USHORT action_flg;

	/* if need to check card status ALARM */
	if(errset_p->chkalm_flg == D_RRH_CHKALM_NCY)
	{
		/* if card status is ALARM, do not set ERR control register */
		if ((( tra_sv_newsta[D_SYS_NUM0] & D_RRH_ACT_BIT) == D_SYS_NUM0) &&
			(( tra_sv_newsta[D_SYS_NUM0] & D_RRH_BLK_BIT) == D_SYS_NUM0) &&
			(( tra_sv_newsta[D_SYS_NUM0] & D_RRH_ALM_BIT_ON) != D_SYS_NUM0))
		{
			return;
		}
	}

	/* Get parameters from API */
	action_flg = errset_p->act_flag;

	/* Error register setting */
	f_rsv_cm_ErrRegSet(action_flg, errset_p->writeAdr, errset_p->writeDat);

	return;
}

/* @} */
