/*!
 * @skip		$ld:$
 * @file		f_mpproxy_recv.c
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
#include "f_mpsw_msg_debug_def.h"
#include "f_mpsw_msg_debug_typ.h"
#include "BPF_RU_ITCM.h"


typedef enum{
	E_MPPOROXY_NORMAL	= 0,
	E_MPPOROXY_MODE_REJ	= 1
}E_MPPOROXY_DEBUG_MODE;


static 	T_MSI_HANDLE			f_mpproxyw_msiHandle;
static 	CHAR 					f_mpproxyw_recvBuffer[0x10000];
static 	UINT 					f_mpproxyw_modId_mng[0x100];
static T_MSI_SUBSCRIBE_MOD_INFO	f_mpproxyw_modInfo[D_CMU_MPTABLE_MAX];
static 	UINT					f_mpproxyw_msidebug_mode = 0;
static 	T_MPSW_SRV_MSGERROR		f_mpproxyw_msidebug_error;

#define D_CMU_MSGID_MSI_RCV_IND	0x10020001

/*!
 * @brief		f_mpproxy_recv
 * @note		f_mpproxy_recv関数.
 * @param		tRcvMsg					[in]	受信メッセージ
 * @return		N/A
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/11/23 Taniguchi Create
 */

VOID f_mpproxy_recv(T_CMU_CREATE_THREAD_MP *tRcvMsg)
{
	VOID*	rcvMsg_p;
	T_MSI_SUBSCRIBE_MOD_INFO* modInfo_p;
	T_MPSW_SRV_MSGHEAD*	mpheader;
	T_MPSW_SRV_MSGHEAD*	msiheader;
	UINT	count;
	unsigned long int		threadId	= 0;		/*	スレッドID格納変数				*/
	INT						endcd		= 0;
	INT						errcd		= 0;		/*	詳細エラーコード				*/
	INT msiLen;
	UINT					modInfoNum = 0;
	UINT					errorcount = 0;
	UINT					sendFlg;
	sigset_t 				sigSetVal;

	if(tRcvMsg == NULL){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"Error input tRcvMsg is NULL");
		return;
	}

	/* スレッド名表示	*/
	prctl(PR_SET_NAME, tRcvMsg->uiThreadName_recv, 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	tRcvMsg->uiProcID,				/* ProcessID		*/
									tRcvMsg->uiThreadID_recv,				/* ThreadID			*/
									tRcvMsg->uiThreadQID_recv,			/* ThreadQueueID	*/
									(unsigned long int)pthread_self());

	/* assert log */
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, 
						(const char*)"ThreadName:%s, ProcessID :0x%08lX, ProcessQID:0x%08lX, ThreadID:0x%08lX, ThreadQID:0x%08lX.",
						tRcvMsg->uiThreadName_recv,
						tRcvMsg->uiProcID,
						tRcvMsg->uiProcQID,
						tRcvMsg->uiThreadID_recv,
						tRcvMsg->uiThreadQID_recv);
	

	/* PTP用のシグナルが大量発生するため、本スレッドではBLOCKする  */
	sigemptyset( &sigSetVal );
	(void)sigaddset( &sigSetVal, SIGRTMAX - 1  );	/* ZLTICKTIMERSIG */
	(void)sigaddset( &sigSetVal, SIGRTMAX - 2  );	/* ZLAPRTIMERSIG */
	(void)sigaddset( &sigSetVal, SIGRTMAX - 3  );	/* ZLWDTIMERSIG */
	(void)sigaddset( &sigSetVal, SIGRTMAX - 10  );	/* ZLCSTTIMERSIG */
	(void)pthread_sigmask(SIG_BLOCK, &sigSetVal, NULL);

	memset((void*)f_mpproxyw_modId_mng,0,sizeof(f_mpproxyw_modId_mng));
	memset((void*)f_mpproxyw_modInfo,0,sizeof(f_mpproxyw_modInfo));

	for( count = 0 ;count < tRcvMsg->tRouteInfoNum ; count++){
		f_mpproxyw_modId_mng[ (UINT)((tRcvMsg->tRouteInfo[count].msgId >> 16) & 0xFF) ]
			 |= D_MPSW_ON << ((tRcvMsg->tRouteInfo[count].msgId >> 8) & 0xFF);
	}

	for(count=0;count < ( sizeof(f_mpproxyw_modId_mng) / sizeof(UINT) ) ; count++){
		if(f_mpproxyw_modId_mng[count] == D_RRH_OFF){
			continue;	
		}
		f_mpproxyw_modInfo[modInfoNum].modId     = count;
		f_mpproxyw_modInfo[modInfoNum].modOptBit = f_mpproxyw_modId_mng[count];
		modInfoNum++;
	}

	modInfo_p = f_mpproxyw_modInfo;
	
	memset(&f_mpproxyw_msiHandle,0,sizeof(f_mpproxyw_msiHandle));
	while(1){
		endcd = msi_subscribe(tRcvMsg->portNo,tRcvMsg->ipAddr,modInfoNum,&modInfo_p,bpf_com_plog_trace_mp,NULL,NULL,&f_mpproxyw_msiHandle);
		if(endcd != MSI_FUNCIF_OK){
			errorcount++;
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MSI subscribe error(%d) ec:%d",modInfoNum,errorcount);
			usleep(100000);	/* 100msec */
		}else{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"MSI subscribe OK(%d) ec:%d",modInfoNum,errorcount);
			break;
		}
	}
	
	tRcvMsg->msiHandle = &f_mpproxyw_msiHandle;

	/*	スレッドを生成		*/
	endcd = BPF_RU_ITCM_THREAD_CREATE(
					(unsigned long int *)&threadId,
					BPF_RU_ITCM_SCHED_FIFO,	
					D_RRH_PRIORITY_MPSEND,	
					D_RRH_STACKSIZE_DEFAULT,
					f_mpproxy_send,
					(VOID*)tRcvMsg,
					&errcd );

	if(BPF_RM_SVRM_COMPLETE != endcd){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MSI recv thread create error %d %d",	endcd,errcd);
		return;
	}

	mpheader = (T_MPSW_SRV_MSGHEAD*)(f_mpproxyw_recvBuffer);
	/********************************************************************************/
	/* MSG受信処理の無限Loop														*/
	/********************************************************************************/
	for(;;)
	{
		msiLen = 0;
		rcvMsg_p = NULL;
		sendFlg = 0;
	
		endcd = msi_receive_from_agent(&f_mpproxyw_msiHandle, sizeof(f_mpproxyw_recvBuffer) ,&msiLen,f_mpproxyw_recvBuffer);
		if( endcd != MSI_FUNCIF_OK ){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " MSI msi_receive_from_agent NG!!! ");
			return;
		}

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, " MSI recv 0x%08x ", mpheader->msgId );

		if(mpheader->msgId == D_MSG_MSI_DEBUG_IND){
			switch(((T_MSG_MSI_DEBUG_IND*)f_mpproxyw_recvBuffer)->data.opeCode){
				case E_MPPOROXY_NORMAL:
					memset(&f_mpproxyw_msidebug_error,0,sizeof(f_mpproxyw_msidebug_error));
					break;
				case E_MPPOROXY_MODE_REJ:
					memset(&f_mpproxyw_msidebug_error,0,sizeof(f_mpproxyw_msidebug_error));

					if(((T_MSG_MSI_DEBUG_IND*)f_mpproxyw_recvBuffer)->data.optLen > 0){
						memcpy(&f_mpproxyw_msidebug_error,
							(const void*)&(((T_MSG_MSI_DEBUG_IND*)f_mpproxyw_recvBuffer)->data.msgErr),
							((T_MSG_MSI_DEBUG_IND*)f_mpproxyw_recvBuffer)->data.optLen);
					}else{
						f_mpproxyw_msidebug_error.errorCode = -99;
						strcpy(f_mpproxyw_msidebug_error.errorDescription,"MSI REJ MODE");
					}
					break;
				default:
					break;
			}
			f_mpproxyw_msidebug_mode = ((T_MSG_MSI_DEBUG_IND*)f_mpproxyw_recvBuffer)->data.opeCode;
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, " DEBUG MSI recv 0x%08x mode:%d", mpheader->msgId ,f_mpproxyw_msidebug_mode);
			continue;
		}

		/* 強制Rejモード有効かつREQの場合 */
		if((f_mpproxyw_msidebug_mode == E_MPPOROXY_MODE_REJ) && ((mpheader->msgId & 0xF) == E_MSI_MSGTYPE_REQ)){
			CHAR*				msidata_p;

			mpheader->msgId += 2;	/* REJ */
			mpheader->msgLength = sizeof(f_mpproxyw_msidebug_error);
			msiLen = sizeof(T_MPSW_SRV_MSGHEAD)+ sizeof(f_mpproxyw_msidebug_error);

			msidata_p = ((CHAR*)mpheader) + sizeof(T_MPSW_SRV_MSGHEAD);
			memcpy(msidata_p,(const void*)&f_mpproxyw_msidebug_error,sizeof(f_mpproxyw_msidebug_error));
			
			if(( endcd = msi_send_to_agent(&f_mpproxyw_msiHandle ,msiLen, f_mpproxyw_recvBuffer)) != MSI_FUNCIF_OK){
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " DEBUG MSI REJ sent NG 0x%08x ", mpheader->msgId );

			}else{
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, " DEBUG MSI REJ sent OK 0x%08x code:0x%08x des:%s", 
						mpheader->msgId ,
						f_mpproxyw_msidebug_error.errorCode ,
						f_mpproxyw_msidebug_error.errorDescription
						 );
			}
			continue;
		}

		for(count=0;count < tRcvMsg->tRouteInfoNum ; count++){
			if(tRcvMsg->tRouteInfo[count].msgId != mpheader->msgId){
				continue;
			}

			endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			
									sizeof( T_RRH_HEAD ) + msiLen,		
									(VOID **)&rcvMsg_p,					
									&errcd );								

			if( endcd != D_RRH_OK ){
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " ADDRGET NG %d %d ", endcd,errcd );
				return;
			}
			
			((T_RRH_HEAD*)rcvMsg_p)->uiEventNo = D_CMU_MSGID_MSI_RCV_IND;
			((T_RRH_HEAD*)rcvMsg_p)->uiDstPQueueID	= tRcvMsg->uiProcQID;
			((T_RRH_HEAD*)rcvMsg_p)->uiDstTQueueID	= tRcvMsg->tRouteInfo[count].dst_que_id;
			((T_RRH_HEAD*)rcvMsg_p)->uiSrcPQueueID	= tRcvMsg->uiProcQID;
			((T_RRH_HEAD*)rcvMsg_p)->uiSrcTQueueID	= tRcvMsg->uiThreadQID_recv;
			
			msiheader = (T_MPSW_SRV_MSGHEAD*)(((CHAR*)rcvMsg_p) + sizeof(T_RRH_HEAD));
			memcpy((void*)msiheader ,(void*)mpheader,msiLen);

			endcd = BPF_RU_IPCM_MSGQ_SEND(	tRcvMsg->tRouteInfo[count].dst_que_id , (VOID *)rcvMsg_p );
			if( endcd != D_RRH_OK )
			{
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " MSGQ_SEND NG %d ", endcd );
				BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)rcvMsg_p);
				return;
			}
			sendFlg++;
		}
		if(sendFlg == 0){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " MSGQ Not Send 0x%08x ", mpheader->msgId );
		}
	}	
	
}

/* @} */
