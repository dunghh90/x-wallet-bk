/*!
 * @skip  $ld:$
 * @file  rrhApi_Log_Mnr_TroubleLogDelete_3G.c
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
 *  @brief  API : Trouble Log Delete Send(障害ログファイル削除)
 *  @note   PF EVENT ID : 0xA0020005
 *          Reponse  ID : -
 *          TYPE        : MNR
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  wtime       : wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param  *data_p     : 取得データポインタ
 *  @param  *filename   : Trouble Log filename
 *  @return E_RRHAPI_RCODE
 *  @retval ret
 *  @date   2014/12/23 ALPHA)fujii Create.
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Log_Mnr_TroubleLogDelete_3G(	INT				qid, 
													INT				wtime,
													VOID			*data_p,
													UCHAR			*filename )
{
	E_RRHAPI_RCODE		rCode;
	
	rCode = rrhApi_Log_TroubleLogDelete_com( qid, wtime, data_p, filename, D_SYS_3G );
	
	return rCode;
}

/** @} */

