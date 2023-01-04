/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_l3_msg_fw.c
 * @brief  Receive msg from l2/lpb, send msg to l3/rct
 * @date   2015/07/23 FPT)Yen create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"
#include "m_cm_header.h"
#include "f_sys_inc.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Receive msg from l2/lpb, send msg to l3/rct
 * @note   Receive msg from l2/lpb, send msg to l3/rct
 * @param  aBufp [in]  バッファポインタ
 * @param  awSize [in]  バッファサイズ
 * @return None
 * @date  2015/07/23 FPT)Yen create
 */
/****************************************************************************/
INT f_trp_rec_Fcom_t_l3_msg_fw(                                  /* なし:R */
    VOID*   aBufp,                                     /* バッファポインタ:I */
    USHORT  awSize
) {
    INT		endcd;						/*!< 終了コー繝?					*/
	VOID	*shm_msgp;
	UINT	keyID;
	INT		ret __attribute__((unused));
#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif

	/* パラメータチェック */
	if(( NULL == aBufp ) || ( 0 == awSize))
	{
		
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
		return D_RRH_NG;
	}


	/* Communication Shared Memory Address Get. */	
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(
										E_BPF_RU_IPCM_BUFF_KIND_TASK,	/*!< Buffer種別					*/
										awSize,							/*!< size						*/
										(void **)&shm_msgp,				/*!< 通信用共有メモリのポイン繧?*/
										&endcd);						/*!< エラーコード				*/

	if(shm_msgp == D_RRH_NULL)
	{
		/*access log*/
		f_com_assert(endcd,"fail to BPF_RU_IPCM_PROCMSG_ADDRGET");
		
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
		return D_RRH_NG;
	}

	/*temp save key*/
	keyID = ((T_SYS_COMMON_THDIF*)shm_msgp)->uiKeyID;

/* Update msg */

	/* SrcPQueueID：D_RRH_PROCQUE_RE */
	/* SrcTQueueID：Source thread ID */
	/* DstPQueueID：Destination process ID */
	/* DstTQueueID：Destination thread ID */
	/* Length ：Message size */
	memcpy(shm_msgp,aBufp,awSize);
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiDstTQueueID = 0;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiSrcTQueueID = CMD_TSKID_REC;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiDstPQueueID = D_RRH_PROCQUE_L3;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiSrcPQueueID = D_RRH_PROCQUE_RE;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiKeyID = keyID;
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiLength = awSize;		
	((T_SYS_COMMON_THDIF*)shm_msgp)->uiEventNo = CMD_TSKIF_REC_FWDNTC;


	/************************************************************************/
	/* 他プロセスへメッセージを送信する										*/
	/************************************************************************/
	ret = BPF_RU_IPCM_MSGQ_SEND(
							D_RRH_PROCQUE_L3,			/* 直近の送信先QueueID( Thread Queue ID or Process Queue ID )		*/
							(void *)shm_msgp	/* 送信メッセー繧?BPF_RU_IPCM_PROCMSG_ADDRGET)で取得したアドレ繧?*/
							);

	if(ret != BPF_RU_IPCM_OK)
	{
		f_com_assert(ret,"fail to send msg");
		
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
		return D_RRH_NG;
	}

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
	return D_RRH_OK;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
