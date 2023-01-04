/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cm_ErrCntRegSet.c
 *  @brief	TRA common function - Err control register set function 
 *  @date   2008/12/18 FFCS)Wangjuan create
 *  @date   2013/11/18 FFCS)Xut modify for ZYNQ.
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*****************************************************************************/

/** @addtogroup RRH_DCM
 *  @{
 */
 
/**************************************************************************/
/**
 *  @brief  TRA common function - Err control register set function
 *  @note   Err control register set function  \n
 *  @param  linkno [in] cpri link number
 *  @param  action_flg [in] set or clear flag, 0:clear 1:set
 *  @param  writeAdr [in] register address
 *  @param  writeDat [in] register set data
 *  @param  chkalm_flg [in] card ALARM check flag
 *  @return none 
 *  @retval none
 *  @date   2008/12/18		FFCS)Wangjuan new create for M-S3G-eNBSYS-00023
 *  @date   2013/11/18 	FFCS)Xut modify for ZYNQ.
 *  @date   2015/8/3 TDIPS)ikeda rev.27733 17リンク対応、対RE実装（実装中）
 *  @date   2015/09/04 TDI)satou 「writeAdr」の引数を追加。「writeDat」の型をUINTに変更
 */
/**************************************************************************/

#include "f_rrh_def.h"
#include "m_cm_header.h" /* TRA common header file*/
#include "rrhApi_Svp.h"

VOID	m_cmR_ErrCntRegSetRE(USHORT linkno, USHORT action_flg, UINT writeAdr, UINT writeDat, USHORT chkalm_flg, const VOID* filename, UINT line)
{
	INT			ret;
	INT			wtime;						/*  応答メッセージ待ち時間(msec単位)*/
	VOID *		data_p;						/*  取得データ用ポインタ		*/

	/* Initialization */
	wtime = CMD_NUM0;
	data_p = NULL;
	
	if(chkalm_flg == CMD_CHKALM_NCY)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "lnk=%d,act=%d,add=%x,dat=%x[%s(%d)]", linkno, action_flg, writeAdr, writeDat, (const char*)filename, line);
	}
	/* Call API: Error Setting Notification */
	ret = rrhApi_Svp_Mnt_ErrSet(CMD_NUM1, wtime, data_p, action_flg, writeAdr, writeDat, chkalm_flg);
	if(ret != E_API_RCD_OK)	
	{
		cm_Assert(CMD_ASL_DBGLOW,ret,"rrhApi_Svp_Mnt_ErrSet NG");
	}

	return;
}
/* @} */
