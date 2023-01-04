/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   l3_com_msgCom.c
 *  @brief  the common function of thread
 *  @date   2015/07/09 FPT)Yen Create new
 *  @date   
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 
 */
/********************************************************************************************************************/

/** @addtogroup L3_COM
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "f_sys_inc.h"
#include "m_cm_header.h"
#include "l3_com_inc.h"
#include "rrhApi_Svp.h"

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  The function of sending message betheen process / thread
 *  @note 
 *  @param  awSrcTQueueID [in] Source thread ID
 *  @param  awDstPQueueID [in] Destination process ID
 *  @param  awDstTQueueID [in] Destination thread ID
 *  @param  apMsgBuff [in] Message buffer pointer
 *  @param  awMsgSize [in] Message size
 *  @return None
 *  @date 
 */
/********************************************************************************************************************/

INT l3_com_sendMsg( UINT awSrcTQueueID, UINT awDstPQueueID, UINT awDstTQueueID, VOID* apMsgBuff, UINT awMsgSize )
{
	INT		endcd;						/*!< 終了コー繝?					*/
	VOID	*shm_msgp;
	UINT	keyID;
	INT		ret __attribute__((unused));
	UINT	msgQid;

	/* パラメータチェック */
	if(( NULL == apMsgBuff ) || ( 0 == awMsgSize))
	{
		return D_RRH_NG;
	}



/* Communication Shared Memory Address Get. */
	/* buffer kind：E_BPF_RU_IPCM_BUFF_KIND_TASK */
	/* buffer size：Message size */

	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(
										E_BPF_RU_IPCM_BUFF_KIND_TASK,	/*!< Buffer種別					*/
										awMsgSize,							/*!< size						*/
										(void **)&shm_msgp,				/*!< 通信用共有メモリのポイン繧?*/
										&endcd);						/*!< エラーコード				*/

	if(shm_msgp == D_RRH_NULL)
	{
		/*access log*/
		f_com_assert(CMD_NUM1,"fail to BPF_RU_IPCM_PROCMSG_ADDRGET");
		return D_RRH_NG;
	}

	/*temp save key*/
	keyID = ((T_SYS_COMMON_THDIF*)shm_msgp)->uiKeyID;

	/* Update msg */

	/* SrcPQueueID：D_RRH_PROCQUE_L3 */
	/* SrcTQueueID：Source thread ID */
	/* DstPQueueID：Destination process ID */
	/* DstTQueueID：Destination thread ID */
	/* Length：Message size */
	memcpy(shm_msgp, apMsgBuff, awMsgSize);
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiDstTQueueID = awDstTQueueID;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiSrcTQueueID = awSrcTQueueID;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiDstPQueueID = awDstPQueueID;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiSrcPQueueID = D_RRH_PROCQUE_L3;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiKeyID = keyID;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiLength = awMsgSize;
	
	if( 
		( (((T_SYS_COMMON_THDIF*)shm_msgp)->uiEventNo == CMD_NUM0)
		  || (((T_SYS_COMMON_THDIF*)shm_msgp)->uiEventNo == CMD_TSKIF_CPRIRCVNTC))
			&&	( D_RRH_PROCQUE_RE == awDstPQueueID ))
	{
		/*EventNo for sending to re/rec*/
		((T_SYS_COMMON_THDIF*)shm_msgp)->uiEventNo	= CMD_TSKIF_RE_FWDNTC;
	}



	/* DstPQueueID がD_RRH_PROCQUE_L3の場合 */
	if( awDstPQueueID == D_RRH_PROCQUE_L3 )
	{
		/* queue number ：　DstTQueueID */
		msgQid = awDstTQueueID;
	}
	/* その他 */
	else
	{
		/* queue number ： DstPQueueID　 */
		msgQid = awDstPQueueID;
	}
	/************************************************************************/
	/* 他プロセスへメッセージを送信する										*/
	/************************************************************************/
	ret	 = BPF_RU_IPCM_MSGQ_SEND(
							msgQid,			/* 直近の送信先QueueID( Thread Queue ID or Process Queue ID )		*/
							(void *)shm_msgp	/* 送信メッセー繧?BPF_RU_IPCM_PROCMSG_ADDRGET)で取得したアドレ繧?*/
							);

	if(ret != BPF_RU_IPCM_OK)
		return D_RRH_NG;

	return D_RRH_OK;

}

/* @} */


/* @} */

