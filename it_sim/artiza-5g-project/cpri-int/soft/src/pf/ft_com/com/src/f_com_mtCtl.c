/*!
 * @skip  $ld:$
 * @file  f_com_mtCtl.c
 * @brief MT制御処理
 * @date  2019/05/13 FJT)Taniguchi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include <stdlib.h>
#include "f_com_inc.h"


static T_RRH_MT_INFO*	f_comw_mtInfoTbl = NULL;

/*!
 * @brief 関数機能概要:MT情報更新
 * @note  関数処理内容.
 * @param  none
 * @return VOID
 * @retval none
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2019/05/13 FJT)Taniguchi
 */
VOID f_com_mtCtl_init(VOID )
{

	INT							errcd;
	INT							rtn_bpf;

	T_RRH_TRAINVTBL				*invInfo;

	BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_MT_INFO, (VOID **)&f_comw_mtInfoTbl, &errcd);
	BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY, (VOID **)&invInfo, &errcd);

	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_MT_INFO, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, NULL, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MT CTL sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
		return;
	}

	f_comw_mtInfoTbl->comInfo.server.ipAddr = (UINT)((((UINT)(invInfo->cmx_ipup_mt) << 16) & 0xFFFF0000) | (UINT)(invInfo->cmx_ipdwn_mt));
	f_comw_mtInfoTbl->comInfo.server.portNo = D_DU_MTPORT_NO;
	f_comw_mtInfoTbl->comInfo.client.ipAddr = 0;
	f_comw_mtInfoTbl->comInfo.client.portNo = 0;
	f_comw_mtInfoTbl->comInfo.beamId = 0;
	f_comw_mtInfoTbl->comInfo.mt_con_state = D_RRH_OFF;
	f_comw_mtInfoTbl->comInfo.cu_con_state = D_RRH_OFF;
	/* 明示的にAISG-INF OFF(0)にしておく */
	f_comw_mtInfoTbl->comInfo.aisg_state = D_RRH_OFF;

	strncpy((char*)(f_comw_mtInfoTbl->sfpInfo.sfpType)   ,"-",strlen("-"));
	strncpy((char*)(f_comw_mtInfoTbl->sfpInfo.vendorCode),"-",strlen("-"));
	strncpy((char*)(f_comw_mtInfoTbl->sfpInfo.serialNo)  ,"-",strlen("-"));
	memset(&(f_comw_mtInfoTbl->carInfo_tx),0,sizeof(f_comw_mtInfoTbl->carInfo_tx));
	memset(&(f_comw_mtInfoTbl->carInfo_rx),0,sizeof(f_comw_mtInfoTbl->carInfo_rx));
	memset(&(f_comw_mtInfoTbl->logInfo),0,sizeof(f_comw_mtInfoTbl->logInfo));

	rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_MT_INFO, BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MT CTL sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}
	return;
}
/* @} */

/*!
 * @brief 関数機能概要:MT情報更新
 * @note  関数処理内容.
 * @param  none
 * @return VOID
 * @retval none
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2019/05/13 FJT)Taniguchi
 */
VOID f_com_mtCtl_con(UCHAR onoff)
{
	INT							errcd;
	INT							rtn_bpf;

	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_MT_INFO, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, NULL, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MT CTL sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
		return;
	}

	f_comw_mtInfoTbl->comInfo.cu_con_state = onoff;

	rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_MT_INFO, BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MT CTL sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}

	return;
}
/* @} */


/*!
 * @brief 関数機能概要:MT情報更新
 * @note  関数処理内容.
 * @param  none
 * @return VOID
 * @retval none
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2019/05/13 FJT)Taniguchi
 */
VOID f_com_mtCtl_carInfo(UINT antno, UINT carno, UINT onoff, UINT freq , UINT bandwith, UINT dir)
{
	INT							errcd;
	INT							rtn_bpf;
	UINT						antCnt;

	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_MT_INFO, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, NULL, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MT CTL sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
		return;
	}

	/* TX */
	if(dir == D_RRH_ON){
		f_comw_mtInfoTbl->carInfo_tx.carrierState[antno][carno] = onoff;
		if(onoff == D_RRH_ON){
			f_comw_mtInfoTbl->carInfo_tx.FreqNo[carno] = freq;
			f_comw_mtInfoTbl->carInfo_tx.bandWidth[carno] = bandwith;
		}
		else{
			for(antCnt=0; antCnt < D_RRH_ANT_MAX ; antCnt++){
				if(f_comw_mtInfoTbl->carInfo_tx.carrierState[antCnt][carno] == D_RRH_ON){
					/* Onがまだ存在する場合 */
					break;
				}
			}
			if(antCnt >= D_RRH_ANT_MAX){
				/* 全Ant分 Offの場合のみ 0クリア */
				f_comw_mtInfoTbl->carInfo_tx.FreqNo[carno] = 0;
				f_comw_mtInfoTbl->carInfo_tx.bandWidth[carno] = 0;
			}
		}
	}else{
	/* RX */
		f_comw_mtInfoTbl->carInfo_rx.carrierState[antno][carno] = onoff;
		if(onoff == D_RRH_ON){
			f_comw_mtInfoTbl->carInfo_rx.FreqNo[carno] = freq;
			f_comw_mtInfoTbl->carInfo_rx.bandWidth[carno] = bandwith;
		}
		else{
			for(antCnt=0; antCnt < D_RRH_ANT_MAX ; antCnt++){
				if(f_comw_mtInfoTbl->carInfo_rx.carrierState[antCnt][carno] == D_RRH_ON){
					/* Onがまだ存在する場合 */
					break;
				}
			}
			if(antCnt >= D_RRH_ANT_MAX){
				/* 全Ant分 Offの場合のみ 0クリア */
				f_comw_mtInfoTbl->carInfo_rx.FreqNo[carno] = 0;
				f_comw_mtInfoTbl->carInfo_rx.bandWidth[carno] = 0;
			}
		}
	}

	rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_MT_INFO, BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MT CTL sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}

	return;
}
/* @} */


/*!
 * @brief 関数機能概要:MT情報更新
 * @note  関数処理内容.
 * @param  none
 * @return VOID
 * @retval none
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2019/05/13 FJT)Taniguchi
 */
VOID f_com_mtCtl_sfpInfo(UINT sfp_type, UCHAR* name , UCHAR* serial)
{
	INT							errcd;
	INT							rtn_bpf;

	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_MT_INFO, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, NULL, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MT CTL sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
		return;
	}

	memset(&(f_comw_mtInfoTbl->sfpInfo),0,sizeof(f_comw_mtInfoTbl->sfpInfo));

	switch(sfp_type){
		case 0xE:
		case 0xF:
		case 0:
			if(sfp_type == 0xE){
				strncpy((char*)(f_comw_mtInfoTbl->sfpInfo.sfpType),"TYPE-E",strlen("TYPE-E"));
			}else if(sfp_type == 0xF){
				strncpy((char*)(f_comw_mtInfoTbl->sfpInfo.sfpType),"TYPE-F",strlen("TYPE-F"));
			}else{
				strncpy((char*)(f_comw_mtInfoTbl->sfpInfo.sfpType),"ERROR",strlen("ERROR"));
			}
			strncpy((char*)(f_comw_mtInfoTbl->sfpInfo.vendorCode),(const char*)name,strlen((const char*)name));
			strncpy((char*)(f_comw_mtInfoTbl->sfpInfo.serialNo)  ,(const char*)serial,strlen((const char*)serial));
			break;
		default:
			strncpy((char*)(f_comw_mtInfoTbl->sfpInfo.sfpType)   ,"-",strlen("-"));
			strncpy((char*)(f_comw_mtInfoTbl->sfpInfo.vendorCode),"-",strlen("-"));
			strncpy((char*)(f_comw_mtInfoTbl->sfpInfo.serialNo)  ,"-",strlen("-"));
			break;
	}

	rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_MT_INFO, BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MT CTL sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}

	return;
}
/* @} */


/*!
 * @brief 関数機能概要:AISG電源状態更新
 * @note  関数処理内容.
 * @param  none
 * @return VOID
 * @retval none
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2019/12/26 FJT)Taniguchi
 */
VOID f_com_mtCtl_aisgCtl(UINT onoff)
{
	INT							errcd;
	INT							rtn_bpf;

	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_MT_INFO, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, NULL, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MT CTL sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
		return;
	}

	f_comw_mtInfoTbl->comInfo.aisg_state = onoff;

	rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_MT_INFO, BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MT CTL sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}
	return;
}
/* @} */
