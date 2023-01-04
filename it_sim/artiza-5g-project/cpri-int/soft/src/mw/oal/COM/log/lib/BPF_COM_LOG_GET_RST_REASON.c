/*!
 * @skip  $ld:$
 * @file  BPF_COM_LOG_GET_RST_REASON.c
 * @brief L3 Message log function.
 * @date  2013/07/19 ALPHA)Nakajima Create\n
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
 */
 
/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "bpf_i_com_log.h"

/*!
 *  @brief  This fuction will be called to get reset reason log.
 *  @note   This function get reset reason log.
 *  @param  resetReason		[out]   resetReason
 *  @return UINT
 *  @retval BPF_HM_DEVC_COMPLETE：正常終了
 *  @retval BPF_HM_DEVC_NG      ：異常終了
 *  @date   2013/07/18 ALPHA)Nakajima create
 */
UINT BPF_COM_LOG_GET_RST_REASON( UINT *rstReason)
{
	/****************************************************************************/
	/* Local parameter declaration												*/
	/****************************************************************************/
	UINT					logIndex;
	T_RRH_LOG_RST_REASON*	rstReasonLog;
	INT						endcd;				/* 終了コード					*/
	INT						errcd;				/* 詳細NGコード					*/
	
	
	/****************************************************************************/
	/* Local parameter initialization											*/
	/****************************************************************************/
	logIndex			= D_RRH_ZERO;
	
	/****************************************************************************/
	/* Get Reset Reason log(from shared memory									*/
	/****************************************************************************/
	endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_RST_REASON_LOG,			/* 共有メモリ番号				*/
		(VOID **)&rstReasonLog,					/* 共有メモリアドレス			*/
		&errcd									/* 詳細NGコード					*/
	);
	
	if( endcd != BPF_RU_IPCM_OK )
	{
		/* Write assert log	*/
		switch( endcd )
		{
			case BPF_RU_IPCM_NG:				/* 共有メモリアタッチ失敗		*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[BPF_COM_LOG_GET_RST_REASON]: Attach fault(Shared Memory) %d", errcd);
				break;
			case BPF_RU_IPCM_EPARAM:			/* パラメータ異常				*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, 
				"[BPF_COM_LOG_GET_RST_REASON]: Invalid parameter(Shared Memory) %d", errcd);
				break;
			case BPF_RU_IPCM_ENOSHMNUM:			/* 指定の共有メモリ番号がない	*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_GET_RST_REASON]: No Memory number(Shared Memory) %d", errcd);
				break;
			default:							/* 上記以外						*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_GET_RST_REASON]: BPF_RU_IPCM_PROCSHM_ADDRGET error) %d", errcd);
		}
		
		return BPF_HM_DEVC_NG;
	}
	
	struct bpf_timespec		dtime;	/* sem waittime */	
	dtime.tv_sec = 0;
	dtime.tv_nsec = BPF_RU_IPCM_WAITTIME_LOG;

	/****************************************************************************/
	/* Take Semaphore(Reset Reason log used)									*/
	/****************************************************************************/
	endcd = BPF_RU_IPCM_PROCSEM_TAKE(
		E_RRH_SEMID_APL_RST_REASON_LOG,			/* セマフォ番号					*/
		BPF_RU_IPCM_LOCK_RW,					/* R/Wロック種					*/
		BPF_RU_IPCM_WAIT,						/* ウェイトフラグ				*/
		&dtime,									/* タイマ時間					*/
		&errcd									/* 詳細NGコード					*/
	);
	
	if( endcd != BPF_RU_IPCM_OK )
	{
		switch( endcd )
		{
			/* Write assert log		*/
			case BPF_RU_IPCM_NG:				/* セマフォ取得失敗				*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, 
				"[BPF_COM_LOG_GET_RST_REASON]: take semaphore fault %d", errcd);
				break;
			case BPF_RU_IPCM_EPARAM:			/* パラメータ異常				*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_GET_RST_REASON]: Invalid parameter(take Semaphore) %d", errcd);
				break;
			case BPF_RU_IPCM_ENOSEMNUM:			/* 指定のセマフォ番号がない		*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_GET_RST_REASON]: No semaphore number(take Semaphore) %d", errcd);
				break;
			case BPF_RU_IPCM_ENOSEMGET:			/* セマフォ取得失敗(他がロック)	*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_GET_RST_REASON]: Previously-locked(take Semaphore) %d", errcd);
				break;
			case BPF_RU_IPCM_ETIMEOUT:			/* タイマ満了					*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_GET_RST_REASON]: Time out(take Semaphore) %d", errcd);
				break;
			case BPF_RU_IPCM_EINTR:				/* シグナル受信により中断		*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_GET_RST_REASON]: abort by received the signal %d", errcd);
				break;
			default:							/* 上記以外						*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_GET_RST_REASON]: BPF_RU_IPCM_PROCSEM_TAKE error %d", errcd);
		}
		
		return BPF_HM_DEVC_NG;
	}
	
	/****************************************************************************/
	/* Get reset reason															*/
	/* 装置起動前のリセット要因を取得											*/
	/****************************************************************************/
	/* Get log index		*/
	/* サイクリックした場合 */
	if((rstReasonLog->uiCycleFlg == D_RRH_ON) && (rstReasonLog->uiWriteIdx == D_RRH_ZERO))
	{
		logIndex = D_RRH_LOG_RSTRSN_RCD_NUM - 1;
	}
	/* WriteIdxが異常値の場合 */
	else if(D_RRH_LOG_RSTRSN_RCD_NUM < rstReasonLog->uiWriteIdx)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
		"[BPF_COM_LOG_GET_RST_REASON]: WriteIndex error %d", errcd);
		/****************************************************************************/
		/* Give Semaphore(Error log used)											*/
		/****************************************************************************/
		endcd = BPF_RU_IPCM_PROCSEM_GIVE(
			E_RRH_SEMID_APL_RST_REASON_LOG,			/* セマフォ番号					*/
			BPF_RU_IPCM_LOCK_RW,					/* R/Wロック種					*/
			&errcd									/* 詳細NGコード					*/
		);
		
		return BPF_HM_DEVC_NG;
	}
	/* WriteIdxが正常値かつサイクリックしていない場合 */
	else{
		if( rstReasonLog->uiWriteIdx != 0 )
		{
			logIndex = (rstReasonLog->uiWriteIdx) - 1;
		}
	}
	
	/* Get reset reason		*/
	*rstReason = rstReasonLog->tRecord[logIndex].resetReason;
	
	/****************************************************************************/
	/* Give Semaphore(Error log used)											*/
	/****************************************************************************/
	endcd = BPF_RU_IPCM_PROCSEM_GIVE(
		E_RRH_SEMID_APL_RST_REASON_LOG,			/* セマフォ番号					*/
		BPF_RU_IPCM_LOCK_RW,					/* R/Wロック種					*/
		&errcd									/* 詳細NGコード					*/
	);
	
	if( endcd != BPF_RU_IPCM_OK )
	{
		/* Write assert log	*/
		switch( endcd )
		{
			case BPF_RU_IPCM_NG:				/* セマフォ解放失敗				*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_GET_RST_REASON]: give semaphore fault %d", errcd);
				break;
			case BPF_RU_IPCM_EPARAM:			/* パラメータ異常				*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_GET_RST_REASON]: Invalid parameter(give Semaphore) %d", errcd);
				break;
			case BPF_RU_IPCM_ENOSEMNUM:			/* 指定のセマフォ番号がない		*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_GET_RST_REASON]: No semaphore number(give Semaphore) %d", errcd);
				break;
			default:							/* 上記以外						*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_GET_RST_REASON]: BPF_RU_IPCM_PROCSEM_GIVE error %d", errcd);
		}
		return BPF_HM_DEVC_NG;
	}
	
	return BPF_HM_DEVC_COMPLETE;
}

/* @} */

