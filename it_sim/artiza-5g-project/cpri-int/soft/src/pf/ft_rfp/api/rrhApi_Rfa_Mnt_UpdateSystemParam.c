/*!
 * @skip  $ld:$
 * @file  rrhApi_Rfa_Mnt_UpdateSystemParam.c
 * @brief API : RF操作関連
 * @date  2013/11/22 ALPHA)ueda Create.
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2016
 */

/** @addtogroup RRH_PF_RFP_API
 *  @{
 */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "f_rrh_inc.h"
#include "f_sys_def.h"
#include "BPF_RU_IPCM.h"
#include "BPF_COM_LOG.h"
#include "rrhApi_Com.h"
#include "rrhApi_Rfp.h"

/********************************************************************************************************************/
/**
 *  @brief  API : SystemParamter更新要求
 *  @note   PF EVENT ID     : 0xA0010001
 *          Reponse  ID     : -
 *          TYPE            : MNT
 *  @param  qid             : response queue id (応答送信先QueueID)
 *  @param  wtime           : wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param  *data_p         : 取得データポインタ
 *  @param  dlul_flag       : DL/ULフラグ(0:DL 1:UL)
 *  @param  ant             : アンテナ番号 (0～7)
 *  @param  carrier         : キャリア番号(0～7)
 *  @param  frequency       : 設定周波数kHz単位
 *  @param  acell_id        : ACELL ID(0x00 - 0x3F)
 *  @param  cc_id           : CC_ID(0x00 - 0x0F)
 *  @param  port_id         : PORT ID(0 - 7)
 *  @param  car_gain        : キャリアGain(TBD)
 *  @param  car_delay       : キャリア遅延(TBD)
 *  @return E_RRHAPI_RCODE
 *  @retval E_API_RCD_ERR_BPF_CALL
 *  @retval E_API_RCD_EPARAM
 *  @retval D_API_OK
 *  @warning	N/A
 *  @FeatureID	PF-Rfp-007-001-001
 *  @Bug_No		N/A
 *  @CR_No		N/A
 *  @TBD_No		N/A
 *  @date   2013/11/22 ALPHA)ueda Create.
 *  @date   2014/12/02 ALPHA)nakamura ppc対応
 *  @date   2015/04/21 ALPHA)fujiwara TDD対応
 *  @date   2015/10/20 ALPHA)fujiwara TDD-RRE(zynq)対応
 *  @date   2016/02/19 ALPHA)fujiiy 16A対応
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Rfa_Mnt_UpdateSystemParam(INT qid, INT wtime, VOID *data_p, 
												USHORT dlul_flag,
												USHORT ant,
												USHORT carrier,
												UINT   frequency,
												USHORT acell_id,
												USHORT cc_id,
												USHORT port_id,
												USHORT car_gain,
												USHORT car_delay)
{
	
	return E_API_RCD_OK;
}

/** @} */
