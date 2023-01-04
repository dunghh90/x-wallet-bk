/*!
 * @skip  $ld:$
 * @file  rrhApi_Log_Mnt_TroubleLogGetStop_3G.c
 * @brief API : Log関連
 * @date  2014/12/23 ALPHA)fujii Create.
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014
 */

/** @addtogroup RRH_PF_LOG_API
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
#include "rrhApi_Log.h"

/********************************************************************************************************************/
/**
 *  @brief  API : Trouble Log Get Stop Send(障害ログファイル作成中止)(3G用)
 *  @note   PF EVENT ID : 0xA0020003
 *          Reponse  ID : -
 *          TYPE        : MNT
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  wtime       : wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param  *data_p     : 取得データポインタ
 *	
 *  @return E_RRHAPI_RCODE
 *  @retval ret
 *  @date   2014/12/23 ALPHA)fujii Create.
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Log_Mnt_TroubleLogGetStop_3G(	INT				qid, 
													INT				wtime,
													VOID			*data_p )
{
	E_RRHAPI_RCODE		rCode;
	
	rCode = rrhApi_Log_TroubleLogGetStop_com( qid, wtime, data_p, D_SYS_3G );
	
	return rCode;
}

/** @} */

