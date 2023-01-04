/*!
 * @skip  $ld:$
 * @file  rrhApi_Log_Mnr_TroubleLogGet_3G.c
 * @brief API : Log関連
 * @date  2014/12/22 ALPHA)fujii Create.
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
 *  @brief  API : Trouble Log Get Send(障害ログファイル作成要求)(3G用)
 *  @note   PF EVENT ID : 0xA0020001
 *          Reponse  ID : 0xA0020002
 *          TYPE        : MNR
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  wtime       : wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param  *data_p     : 取得データポインタ
 *  @param  *filename   : Trouble Log filename
 *	@param  size        : ログファイルサイズ
 *  @return E_RRHAPI_RCODE
 *  @retval ret
 *  @date   2014/12/22 ALPHA)fujii Create
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Log_Mnr_TroubleLogGet_3G(	INT				qid, 
												INT				wtime,
												VOID			*data_p,
												UCHAR			*filename,
												UINT			size )
{
	E_RRHAPI_RCODE		rCode;
	
	rCode = rrhApi_Log_TroubleLogGet_com( qid, wtime, data_p, filename, size, D_SYS_3G );
	
	return rCode;
}

/** @} */

