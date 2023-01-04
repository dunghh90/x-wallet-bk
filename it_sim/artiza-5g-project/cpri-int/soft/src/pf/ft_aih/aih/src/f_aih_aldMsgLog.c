/*!
 * @skip  $ld:$
 * @file  f_aih_aldMsgLog.c
 * @brief log  ALD-send/receive message.
 * @date 2019/08/08 FJT)Takahashi create
 * All Rights Reserved, Copyright FUJITSU LIMITED 2019
 */
    
/*!
 * @addtogroup RRH_PF_AIH
 * @{
 */
#include "f_aih_inc.h"			/* ALD(AISG)タスクヘッダファイル		*/

static	struct timespec	f_aih_aldMsgLog_lastSnd[D_DU_AIH_ALD_CTRL_DEV_MAX];
static	struct timespec	f_aih_aldMsgLog_lastRcv[D_DU_AIH_ALD_CTRL_DEV_MAX];


/*!
 *  @brief  ALD状態LOGの時刻更新
 *  @note   ALD状態LOGの時刻更新
 *  @return N/A
 *  @date 2019/12/27 FJT)Taniguch create
 *  @warning	N/A
 *  @FeatureID	5GDU-M-002
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
static VOID f_aih_aldMsgLog_timeupdate(UINT dir, UCHAR devAddr ,UCHAR frameType )
{
	struct timespec					boottime;
	UCHAR							updateFlg;

	(void)clock_gettime( CLOCK_MONOTONIC_RAW   , &boottime);
	updateFlg = D_RRH_OFF;

	if(dir == D_DU_AIH_ALD_CTRL_DIR_TYPE_SND){
		if( f_aih_aldMsgLog_lastSnd[devAddr].tv_sec > 0 ){
			if(f_aih_aldMsgLog_lastSnd[devAddr].tv_nsec > boottime.tv_nsec){
				f_aih_aldCtrlLogp->devTdiffCR[devAddr].lastTimes.tv_sec  = boottime.tv_sec - f_aih_aldMsgLog_lastSnd[devAddr].tv_sec - 1;
				f_aih_aldCtrlLogp->devTdiffCR[devAddr].lastTimes.tv_nsec = (boottime.tv_nsec + 1000000000) - f_aih_aldMsgLog_lastSnd[devAddr].tv_nsec;
			}else{
				f_aih_aldCtrlLogp->devTdiffCR[devAddr].lastTimes.tv_sec  = boottime.tv_sec - f_aih_aldMsgLog_lastSnd[devAddr].tv_sec;
				f_aih_aldCtrlLogp->devTdiffCR[devAddr].lastTimes.tv_nsec = boottime.tv_nsec - f_aih_aldMsgLog_lastSnd[devAddr].tv_nsec;
			}

			if( f_aih_aldCtrlLogp->devTdiffCR[devAddr].lastTimes.tv_sec > f_aih_aldCtrlLogp->devTdiffCR[devAddr].maxTimes.tv_sec ){
				updateFlg = D_RRH_ON;
			}else{
				if(f_aih_aldCtrlLogp->devTdiffCR[devAddr].lastTimes.tv_sec == f_aih_aldCtrlLogp->devTdiffCR[devAddr].maxTimes.tv_sec ){
					if( f_aih_aldCtrlLogp->devTdiffCR[devAddr].lastTimes.tv_nsec > f_aih_aldCtrlLogp->devTdiffCR[devAddr].maxTimes.tv_nsec){
						updateFlg = D_RRH_ON;
					}
				}
			}
			if(updateFlg == D_RRH_ON){
				memcpy( &(f_aih_aldCtrlLogp->devTdiffCR[devAddr].maxTimes), 
					&(f_aih_aldCtrlLogp->devTdiffCR[devAddr].lastTimes),
					sizeof(f_aih_aldCtrlLogp->devTdiffCR[devAddr].maxTimes));
				updateFlg = D_RRH_OFF;
			}

			if((f_aih_aldCtrlLogp->devTdiffCR[devAddr].minTimes.tv_sec == 0) && (f_aih_aldCtrlLogp->devTdiffCR[devAddr].minTimes.tv_nsec == 0)){
				updateFlg = D_RRH_ON;
			}else{
				if( f_aih_aldCtrlLogp->devTdiffCR[devAddr].lastTimes.tv_sec < f_aih_aldCtrlLogp->devTdiffCR[devAddr].minTimes.tv_sec ){
					updateFlg = D_RRH_ON;
				}else{
					if(f_aih_aldCtrlLogp->devTdiffCR[devAddr].lastTimes.tv_sec == f_aih_aldCtrlLogp->devTdiffCR[devAddr].minTimes.tv_sec ){
						if( f_aih_aldCtrlLogp->devTdiffCR[devAddr].lastTimes.tv_nsec < f_aih_aldCtrlLogp->devTdiffCR[devAddr].minTimes.tv_nsec){
							updateFlg = D_RRH_ON;
						}
					}
				}
			}
			if(updateFlg == D_RRH_ON){
				memcpy( &(f_aih_aldCtrlLogp->devTdiffCR[devAddr].minTimes), 
					&(f_aih_aldCtrlLogp->devTdiffCR[devAddr].lastTimes),
					sizeof(f_aih_aldCtrlLogp->devTdiffCR[devAddr].minTimes));
				updateFlg = D_RRH_OFF;
			}

		}
		memcpy(&f_aih_aldMsgLog_lastSnd[devAddr],&boottime,sizeof(boottime));
	}else{
		memcpy(&f_aih_aldMsgLog_lastRcv[devAddr],&boottime,sizeof(boottime));
		if(frameType != D_DU_AIH_ALD_CTRL_FRAME_TYPE_U){
			if(f_aih_aldMsgLog_lastSnd[devAddr].tv_sec > 0){
				if(f_aih_aldMsgLog_lastRcv[devAddr].tv_nsec < f_aih_aldMsgLog_lastSnd[devAddr].tv_nsec){
					f_aih_aldCtrlLogp->devTdiffRR[devAddr].lastTimes.tv_sec  = f_aih_aldMsgLog_lastRcv[devAddr].tv_sec - f_aih_aldMsgLog_lastSnd[devAddr].tv_sec - 1;
					f_aih_aldCtrlLogp->devTdiffRR[devAddr].lastTimes.tv_nsec = ( f_aih_aldMsgLog_lastRcv[devAddr].tv_nsec + 1000000000 )- f_aih_aldMsgLog_lastSnd[devAddr].tv_nsec;
				}else{
					f_aih_aldCtrlLogp->devTdiffRR[devAddr].lastTimes.tv_sec  = f_aih_aldMsgLog_lastRcv[devAddr].tv_sec - f_aih_aldMsgLog_lastSnd[devAddr].tv_sec;
					f_aih_aldCtrlLogp->devTdiffRR[devAddr].lastTimes.tv_nsec = f_aih_aldMsgLog_lastRcv[devAddr].tv_nsec - f_aih_aldMsgLog_lastSnd[devAddr].tv_nsec;
				}

				if( f_aih_aldCtrlLogp->devTdiffRR[devAddr].lastTimes.tv_sec > f_aih_aldCtrlLogp->devTdiffRR[devAddr].maxTimes.tv_sec ){
					updateFlg = D_RRH_ON;
				}else{
					if(f_aih_aldCtrlLogp->devTdiffRR[devAddr].lastTimes.tv_sec == f_aih_aldCtrlLogp->devTdiffRR[devAddr].maxTimes.tv_sec ){
						if( f_aih_aldCtrlLogp->devTdiffRR[devAddr].lastTimes.tv_nsec > f_aih_aldCtrlLogp->devTdiffRR[devAddr].maxTimes.tv_nsec){
							updateFlg = D_RRH_ON;
						}
					}
				}
				if(updateFlg == D_RRH_ON){
					memcpy( &(f_aih_aldCtrlLogp->devTdiffRR[devAddr].maxTimes),&(f_aih_aldCtrlLogp->devTdiffRR[devAddr].lastTimes),sizeof(f_aih_aldCtrlLogp->devTdiffRR[devAddr].maxTimes));
					updateFlg = D_RRH_OFF;
				}

				if((f_aih_aldCtrlLogp->devTdiffRR[devAddr].minTimes.tv_sec == 0) && (f_aih_aldCtrlLogp->devTdiffRR[devAddr].minTimes.tv_nsec == 0)){
					updateFlg = D_RRH_ON;
				}else{
					if( f_aih_aldCtrlLogp->devTdiffRR[devAddr].lastTimes.tv_sec < f_aih_aldCtrlLogp->devTdiffRR[devAddr].minTimes.tv_sec ){
						updateFlg = D_RRH_ON;
					}else{
						if(f_aih_aldCtrlLogp->devTdiffRR[devAddr].lastTimes.tv_sec == f_aih_aldCtrlLogp->devTdiffRR[devAddr].minTimes.tv_sec ){
							if( f_aih_aldCtrlLogp->devTdiffRR[devAddr].lastTimes.tv_nsec < f_aih_aldCtrlLogp->devTdiffRR[devAddr].minTimes.tv_nsec){
								updateFlg = D_RRH_ON;
							}
						}
					}
				}
				if(updateFlg == D_RRH_ON){
					memcpy( &(f_aih_aldCtrlLogp->devTdiffRR[devAddr].minTimes),&(f_aih_aldCtrlLogp->devTdiffRR[devAddr].lastTimes),sizeof(f_aih_aldCtrlLogp->devTdiffRR[devAddr].minTimes));
					updateFlg = D_RRH_OFF;
				}
			}
		}
	}
	return;
}

/*!
 *  @brief  ALD電源制御履歴ログを取得する
 *  @note   This function is processed as follows.\n
 *          - log ALD-send/receive message
 *  @param  msgSize   [in]    log message size
 *  @param  *logMsg   [in]   log message buffer
 *  @return VOID
 *  @retval none
 *  @date 2019/08/08 FJT)Takahashi create
 *  @warning	N/A
 *  @FeatureID	5GDU-M-002
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_aih_aldMsgLog( UINT dir , UINT msgSize, VOID *logMsg )
{
	UCHAR*							hdlcMsg;
	T_RRH_AIH_ALD_CTRL_STAT			*logp;
	UCHAR							devAddr = 0;
	UCHAR							devCtl = 0;
	UCHAR							frameType = 0;
	T_AIH_XID_MSG*					xid_p;
	T_AIH_PI_PL_PV*					piplpv_p;
	UCHAR*							tmp_p;
	UCHAR							count;

	if( (logMsg == NULL) || (f_aih_aldCtrlLogp == NULL)){
		return;
	}

	switch(dir){
		case D_DU_AIH_ALD_CTRL_DIR_TYPE_SND:
		case D_DU_AIH_ALD_CTRL_DIR_TYPE_RCV:
			if(msgSize < D_AIH_MSG_MIN_SIZE){
				return;
			}
			hdlcMsg = (UCHAR*)logMsg;
			devAddr = *hdlcMsg;
			hdlcMsg++;
			devCtl  = *hdlcMsg;
			if( f_aih_aldCtrlLogp->devValid[devAddr] == D_RRH_OFF){
				memset(&f_aih_aldMsgLog_lastSnd[devAddr],0,sizeof(f_aih_aldMsgLog_lastSnd[devAddr]));
				memset(&f_aih_aldMsgLog_lastRcv[devAddr],0,sizeof(f_aih_aldMsgLog_lastRcv[devAddr]));
			}
			if((devCtl & 0x1) != 0){
				if((devCtl & 0x2) != 0){
					frameType = D_DU_AIH_ALD_CTRL_FRAME_TYPE_U;	/* U-frame */
				}else{
					frameType = D_DU_AIH_ALD_CTRL_FRAME_TYPE_S;	/* S-frame */
				}
			}else{
				frameType = D_DU_AIH_ALD_CTRL_FRAME_TYPE_I;	/* I-frame */
			}
			if(dir == D_DU_AIH_ALD_CTRL_DIR_TYPE_SND){
				logp = &f_aih_aldCtrlLogp->devAddrSnd[devAddr][frameType];
			}else{
				logp = &f_aih_aldCtrlLogp->devAddrRcv[devAddr][frameType];
				if( (devCtl & 0xEF) == 0xAF){
					if( msgSize >= (sizeof(devAddr) + sizeof(devCtl) + sizeof(T_AIH_XID_MSG))){
						hdlcMsg++;
						xid_p = (T_AIH_XID_MSG*)hdlcMsg;
						if(( xid_p->fI == 0x81) && (xid_p->gI == 0xF0 ) && (xid_p->gL >= sizeof(T_AIH_PI_PL_PV))){
							piplpv_p = xid_p->pI_pL_pV;
							tmp_p = (UCHAR*)piplpv_p;
							for( count = 0 ; count < xid_p->gL ; ){
								if(( piplpv_p->pI == 0x01 ) && (piplpv_p->pL <= D_DU_AIH_ALD_CTRL_UIDLEN)){
									/* UniqueID */
									memset(&f_aih_aldCtrlLogp->devName[devAddr][0],0,D_DU_AIH_ALD_CTRL_UIDLEN);
									memcpy(&f_aih_aldCtrlLogp->devName[devAddr][0],piplpv_p->pV,piplpv_p->pL);
									break;
								}
								count += sizeof(piplpv_p->pI) + sizeof(piplpv_p->pL) + piplpv_p->pL;
								if((count + 7) >= msgSize){	/* count + ADDR+CTRL+FI+GI+GL */
									break;
								}
								tmp_p += count;
								piplpv_p = (T_AIH_PI_PL_PV*)tmp_p;
							}
						}
					}
				}
			}
			f_aih_aldMsgLog_timeupdate(dir,devAddr,frameType);
			break;
		case D_DU_AIH_ALD_CTRL_DIR_TYPE_TO:
			devAddr = f_aih_aldCtrlLogp->last_devAddr;
			devCtl = 0;
			frameType = D_DU_AIH_ALD_CTRL_FRAME_TYPE_T;	/* Timeout */
			logp = &f_aih_aldCtrlLogp->devAddrRcv[devAddr][frameType];
			break;
		default:
			devAddr = f_aih_aldCtrlLogp->last_devAddr;
			devCtl = *(UCHAR*)logMsg;
			frameType = D_DU_AIH_ALD_CTRL_FRAME_TYPE_E;	/* Error */
			logp = &f_aih_aldCtrlLogp->devAddrRcv[devAddr][frameType];
			break;
	}

	if(logp->validFlg == D_RRH_OFF){
		(VOID)BPF_HM_DEVC_GETTIME(&(logp->s_date));  /* Set System time */
		logp->firstCtl = devCtl;
		logp->lastCtl = devCtl;
		logp->count = 0;
		logp->size = 0;
	}else{
		if(logp->lastCtl != devCtl){
			logp->firstCtl = logp->lastCtl;
			logp->lastCtl = devCtl;
		}
	}
	(VOID)BPF_HM_DEVC_GETTIME(&(logp->e_date));  /* Set System time */
	logp->count++;
	logp->size += msgSize;
	logp->validFlg = D_RRH_ON;
	f_aih_aldCtrlLogp->devValid[devAddr] = D_RRH_ON;
	return;
}
/* @} */

