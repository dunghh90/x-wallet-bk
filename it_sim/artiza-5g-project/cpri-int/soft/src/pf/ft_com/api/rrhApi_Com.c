/*!
 * @skip  $ld:$
 * @file  rrhApi_Com.c
 * @brief API : API関連
 *  @date   2013/11/13 FFCS)niemsh create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/** @addtogroup RRH_API_COM
 *  @{
 */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "f_rrh_inc.h"
#include "BPF_COM_LOG.h"
#include "BPF_RU_IPCM.h"
#include "rrhApi_Com.h"

/********************************************************************************************************************/
/*!
 *  @brief
 *  @note 他プロセスへメッセージを送信し、プロセス間IFログを取得する。
 *  @param  q_num      [in]   キュー番号
 *  @param  msgp       [in]   送信メッセージ格納アドレス
 *  @param  msgsz(Byte)[in]   送信メッセージサイズ
 *  @param  q_flag     [in]   ウェイトフラグ
 *  @param  errcd      [out]  詳細NGコード
 *  @return INT return code
 *  @retval E_API_RCD_OK
 *  @retval E_API_RCD_NG
 *  @retval E_API_RCD_EPARAM
 *  @retval E_API_RCD_ESIZEOVER
 *  @retval E_API_RCD_ENOQNUM
 *  @retval E_API_RCD_ENOSPACE
 *  @retval E_API_RCD_EADDR
 *  @retval E_API_RCD_EINTR
 *  @date   2013/11/13 FFCS)niemsh create
 *  @FeatureID	RRH-011-011
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
INT rrhApi_com_msgQSend( UINT	q_num,
						 const VOID			*msgp,
						 UINT				msgsz,
						 INT				*errcd
){
	/************************************************************************/
	/* Local parameter declaration											*/
	/************************************************************************/
	INT		retcd;						/* 終了コード						*/
	INT		endcd;						/* 終了コード						*/
	VOID	*shm_msgp;
	UINT	keyID;
	
	if(msgp == D_RRH_NULL)
	{
		if(errcd) *errcd = E_API_RCD_EPARAM;
		return E_API_RCD_EPARAM;
	}

	if((((T_API_COMMON_HEADER*)msgp)->uiDstPQueueID < 1)
		|| (((T_API_COMMON_HEADER*)msgp)->uiSrcPQueueID < 1))
	{
		if(errcd) *errcd = E_API_RCD_EPARAM;
		return E_API_RCD_EPARAM;
	}

	shm_msgp = D_RRH_NULL;
	retcd = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK,/* Buffer種別					*/
										msgsz,						/* size						*/
										(void **)&shm_msgp,			/* 通信用共有メモリのポインタ	*/
										&endcd);					/* エラーコード				*/
	if(retcd == BPF_RU_IPCM_OK)
	{
		/*temp save key*/
		keyID = ((T_RRH_HEAD*)shm_msgp)->uiKeyID;
		memcpy(shm_msgp,msgp,msgsz);
		((T_API_COMMON_HEADER*)shm_msgp)->uiKeyID = keyID;
		/************************************************************************/
		/* 他プロセスへメッセージを送信する										*/
		/************************************************************************/
		retcd = BPF_RU_IPCM_MSGQ_SEND(	q_num,			/* 直近の送信先QueueID( Thread Queue ID or Process Queue ID )		*/
										(void *)shm_msgp	/* 送信メッセージ(BPF_RU_IPCM_PROCMSG_ADDRGET)で取得したアドレス	*/
									);
		
		if(retcd == BPF_RU_IPCM_OK)
		return E_API_RCD_OK;
	}
	else
	{
		switch (endcd)
		{
		case BPF_RU_IPCM_EPARAM:    
			endcd = E_API_RCD_EPARAM;
			break;
		case BPF_RU_IPCM_ESIZEOVER: 
			endcd = E_API_RCD_ESIZEOVER;
			break;
		case BPF_RU_IPCM_ENOQNUM:	
			endcd = E_API_RCD_ENOQNUM;
			break;
		case BPF_RU_IPCM_ENOSPACE:	
			endcd = E_API_RCD_ENOSPACE;
			break;
		case BPF_RU_IPCM_EADDR:		
			endcd = E_API_RCD_EADDR;
			break;
		case BPF_RU_IPCM_EINTR:		
			endcd = E_API_RCD_EINTR ;
			break;
		default :
			endcd = E_API_RCD_OTHER ;
			break;
		}
	}
	
	if(errcd) *errcd = endcd;

	return E_API_RCD_NG;
}

/**************************************************************************************************/
/*!
 *  @brief
 *  @note 他プロセスからのメッセージを受信し、L3ログを取得する。
 *  @param  q_num    [in]   キュー番号
 *  @param  q_flag   [in]   ウェイトフラグ
 *  @param  msgsz    [in]   メッセージ格納先サイズ
 *  @param  msgp     [OUT]  メッセージ格納先アドレス
 *  @param  rcvmsgsz [OUT]  受信メッセージサイズ
 *  @param  errcd    [out]  詳細NGコード
 *  @return result code
 *  @retval E_API_RCD_OK
 *  @retval E_API_RCD_NG
 *  @retval E_API_RCD_EPARAM
 *  @retval E_API_RCD_ENOQNUM
 *  @retval E_API_RCD_ESIZEOVER
 *  @retval E_API_RCD_ENOMSG
 *  @retval E_API_RCD_EADDR
 *  @retval E_API_RCD_EINTR
 *  @date   2013/11/13 FFCS)niemsh create
 *  @FeatureID	RRH-011-011
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/**************************************************************************************************/
INT rrhApi_com_msgQReceive( UINT			q_num,
							INT				timeout,
							UINT			msgsz,
							VOID			*msgp,
							UINT			*rcvmsgsz,
							INT				*errcd
){
	/************************************************************************/
	/* Local parameter declaration											*/
	/************************************************************************/
	INT		endcd;						/* 終了コード						*/
	VOID	*shmmsgp;
	INT		rcvLen;

	if((msgp == D_RRH_NULL) || (rcvmsgsz == D_RRH_NULL) )
		return E_API_RCD_EPARAM;
	
	/************************************************************************/
	/* 他プロセスからメッセージを受信する									*/
	/************************************************************************/
	*rcvmsgsz = 0;
	shmmsgp = D_RRH_NULL;
	endcd = BPF_RU_IPCM_MSGQ_RECV(	q_num,				/* 自分のQueueID								*/
									timeout,			/* 応答待ち時間(Process間Message受信時のみ有効)	*/
									(void **)&shmmsgp,	/* 通信用共有メモリのポインタ					*/
									rcvmsgsz );			/* Receiveしたメッセージのサイズ	*/

	if(endcd == BPF_RU_IPCM_OK)
	{
		rcvLen = ((T_API_COMMON_HEADER*)shmmsgp)->uiLength;
		if(msgsz >= rcvLen)
		memcpy(msgp,shmmsgp,rcvLen);
		else
		memcpy(msgp,shmmsgp,msgsz);

		BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)shmmsgp );
			
		*rcvmsgsz = rcvLen;

		return E_API_RCD_OK;
	}
	
	switch (endcd)
	{
	case BPF_RU_IPCM_EPARAM:    endcd = E_API_RCD_EPARAM;break;
	case BPF_RU_IPCM_ESIZEOVER: endcd = E_API_RCD_ESIZEOVER;break;
	case BPF_RU_IPCM_ENOQNUM:	endcd = E_API_RCD_ENOQNUM;break;
	case BPF_RU_IPCM_ENOSPACE:	endcd = E_API_RCD_ENOSPACE;break;
	case BPF_RU_IPCM_EADDR:		endcd = E_API_RCD_EADDR;break;
	case BPF_RU_IPCM_EINTR:		endcd = E_API_RCD_EINTR ;break;
	case BPF_RU_IPCM_ENOMSG:	endcd = E_API_RCD_ENOMSG ;break;
	
	default :
		endcd = E_API_RCD_NG ;break;
	}
	if(errcd) 
		*errcd = endcd;
	
	return E_API_RCD_NG;
}

void rrhApi_com_log_real(UINT logLevel , const char* log , INT err ,const VOID * fileName, UINT lineNo )
{
	bpf_com_log_assert_real(logLevel, (const char*)fileName , lineNo ,"%s api error(%d)",log, err );
	BPF_COM_PLOG_TRACE_PROC( D_RRH_LOG_AST_LV_INFO , "%s api error(%d)", log, err);
	return;
}

INT rrhApi_com_checkRespQ(UINT Qid)
{	
	return E_API_RCD_OK;
}
/** @} */

