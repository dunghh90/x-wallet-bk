/*!
 * @skip		$ld:$
 * @file		f_mpproxy_send.c
 * @brief		cmu メイン処理
 * @author		Taniguchi
 * @date		2018/11/23 Taniguchi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_CMU
 * @{
 */

#include "f_cmn_cmu_inc.h"
#include "f_mpsw_msi.h"
#include "f_mpsw_msg_MSI.h"


/*!
 * @brief		f_mpproxy_send
 * @note		f_mpproxy_send関数.
 * @param		tRcvMsg					[in]	受信メッセージ
 * @return		N/A
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/11/23 Taniguchi Create
 */

VOID f_mpproxy_send(T_CMU_CREATE_THREAD_MP *tRcvMsg)
{
	UINT					uiSize;
	VOID*					rcvMsg_p;
	INT						ret;
	INT						msiLen;
	VOID*					msiMsg;
	T_MSI_HANDLE*			msiHandle;
	
	/* スレッド名表示	*/
	prctl(PR_SET_NAME, tRcvMsg->uiThreadName_send, 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	tRcvMsg->uiProcID,				/* ProcessID		*/
									tRcvMsg->uiThreadID_send,			/* ThreadID			*/
									tRcvMsg->uiThreadQID_send,			/* ThreadQueueID	*/
									(unsigned long int)pthread_self());

	/* assert log */
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, 
						(const char*)"ThreadName:%s, ProcessID:0x%08X, ProcessQID:0x%08X, ThreadID:0x%08X, ThreadQID:0x%08X.",
						tRcvMsg->uiThreadName_send,
						tRcvMsg->uiProcID,
						tRcvMsg->uiProcQID,
						tRcvMsg->uiThreadID_send,
						tRcvMsg->uiThreadQID_send);
	
	
	
	msiHandle = (T_MSI_HANDLE*)(tRcvMsg->msiHandle);
	/********************************************************************************/
	/* MSG受信処理の無限Loop														*/
	/********************************************************************************/
	for(;;)
	{
		/* スレッド間MSG受信待ち	*/
		ret = BPF_RU_IPCM_MSGQ_RECV(	tRcvMsg->uiThreadQID_send,	/* スレッドQID					*/
										0,							/* 応答待ち時間					*/
										(void**)&rcvMsg_p,			/* 受信MSGポインタ				*/
										&uiSize						/* 受信MSGサイズ				*/
		);
		
		if(BPF_RU_IPCM_OK == ret){
			msiLen = uiSize - sizeof(T_RRH_HEAD);
			if(msiLen >= sizeof(T_MPSW_SRV_MSGHEAD)){
				msiMsg = (VOID*)(((CHAR*)rcvMsg_p) + sizeof(T_RRH_HEAD));
				if(( ret = msi_send_to_agent(msiHandle ,msiLen, msiMsg)) != MSI_FUNCIF_OK){
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"MSI send error id:0x%08x req::0x%08x len:%d",
								((T_MPSW_SRV_MSGHEAD*)msiMsg)->msgId,
								((T_MPSW_SRV_MSGHEAD*)msiMsg)->requestId,
								msiLen
								);
				}
			}else{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"MSI send length error %d",msiLen);
			}
			BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)rcvMsg_p);
		}
	}
}

/* @} */
