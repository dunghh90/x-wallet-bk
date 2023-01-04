/*!
 * @skip  $ld:$
 * @file  BPF_COM_LOG_RST_REASON.c
 * @brief L3 Message log function.
 * @date  2013/04/05 ALPHA)fujiiy Create\n
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "bpf_i_com_log.h"

/*!
 *  @brief  This fuction will be called to write reset reason log.
 *  @note   This function is processed as follows.\n
 *          - Write Error log
 *  @param  version			[in]   version
 *  @param  resetReason		[in]   resetReason
 *  @param  addInfo         [in]   addInfo
 *  @return VOID
 *  @retval none
 *  @date   2013/04/05 ALPHA)fujiiy create
 *  @date   2013/12/06 ALPHA)yokoyama modify for ZYNQ
 */
VOID  BPF_COM_LOG_RST_REASON(	UINT				version,
								UINT				rstReason,
								UINT				addInfo)
{
	/****************************************************************************/
	/* Local parameter declaration												*/
	/****************************************************************************/
	UINT					logIndex;
	T_RRH_SYSTEM_TIME		tDateTime;
	T_RRH_LOG_RST_REASON*	rstReasonLog;
	INT						endcd;				/* 終了コード					*/
	INT						errcd;				/* 詳細NGコード					*/


	/****************************************************************************/
	/* Local parameter initialization											*/
	/****************************************************************************/
	logIndex			= D_RRH_ZERO;
	memset(&tDateTime, D_RRH_ZERO, sizeof(tDateTime));

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
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_RST_REASON]: Attach fault(Shared Memory) %d", errcd);
				break;
			case BPF_RU_IPCM_EPARAM:			/* パラメータ異常				*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_RST_REASON]: Invalid parameter(Shared Memory) %d", errcd);
				break;
			case BPF_RU_IPCM_ENOSHMNUM:			/* 指定の共有メモリ番号がない	*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_RST_REASON]: No Memory number(Shared Memory) %d", errcd);
				break;
			default:							/* 上記以外						*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_RST_REASON]: BPF_RU_IPCM_PROCSHM_ADDRGET error) %d", errcd);
		}

		return;
	}
	/****************************************************************************/
	/* Take Semaphore(Reset Reason log used)									*/
	/****************************************************************************/
	struct bpf_timespec		dtime;	/* sem waittime */	
	dtime.tv_sec = 0;
	dtime.tv_nsec = BPF_RU_IPCM_WAITTIME_LOG;
	endcd = BPF_RU_IPCM_PROCSEM_TAKE(
		E_RRH_SEMID_APL_RST_REASON_LOG,			/* セマフォ番号					*/
		BPF_RU_IPCM_LOCK_RW,					/* R/Wロック種					*/
		BPF_RU_IPCM_WAIT,						/* ウェイトフラグ				*/
		&dtime,									/* タイマ時間					*/
		&errcd									/* 詳細NGコード					*/
	);
	if( endcd != BPF_RU_IPCM_OK )
	{
		return;
	}

	/****************************************************************************/
	/* Get & update log index													*/
	/****************************************************************************/
	logIndex = rstReasonLog->uiWriteIdx;

	rstReasonLog->uiWriteIdx++;
	if(rstReasonLog->uiWriteIdx >= D_RRH_LOG_RSTRSN_RCD_NUM)
	{
		rstReasonLog->uiWriteIdx = D_RRH_ZERO;
		rstReasonLog->uiCycleFlg = D_RRH_ON;
	}

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
				"[BPF_COM_LOG_RST_REASON]: give semaphore fault %d", errcd);
				break;
			case BPF_RU_IPCM_EPARAM:			/* パラメータ異常				*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_RST_REASON]: Invalid parameter(give Semaphore) %d", errcd);
				break;
			case BPF_RU_IPCM_ENOSEMNUM:			/* 指定のセマフォ番号がない		*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_RST_REASON]: No semaphore number(give Semaphore) %d", errcd);
				break;
			default:							/* 上記以外						*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
				"[BPF_COM_LOG_RST_REASON]: BPF_RU_IPCM_PROCSEM_GIVE error %d", errcd);
		}

		return;
	}

	/****************************************************************************/
	/*  Write log																*/
	/****************************************************************************/
	/* Get System time		*/
	(VOID)BPF_HM_DEVC_GETTIME(&tDateTime);

	/* Set timestamp		*/
	memcpy((VOID*)(&(rstReasonLog->tRecord[logIndex].tDate)), (VOID *)&tDateTime, sizeof(tDateTime));

	/* Set reset reason		*/
	rstReasonLog->tRecord[logIndex].version		= version;
	rstReasonLog->tRecord[logIndex].resetReason	= rstReason;
	rstReasonLog->tRecord[logIndex].addInfo	= addInfo;


	return;
}

/* @} */

