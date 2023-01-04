/*!
 * @skip  $ld:$
 * @file  f_ieo_msiExtInNtfyStartInd.c
 * @brief Satrt external Input Notification(SMI).
 * @date 2019/03/18 FJT)Koshida create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019-
 */

/*!
** @addtogroup RRH_PF_EIO
* @{ */

#include "f_eio_header.h"			/* 5GDU-LLS外部IOタスクヘッダファイル			*/

/* @{ */

/*!
 *  @brief  Satrt external Input Notification(SMI)
 *  @note   This function is Satrting external Input Notification.\n
 *  @param  none
 *  @return VOID
 *  @retval none
 *  @date 2019/03/18 FJT)Koshida create
 *  @warning	N/A
 *  @FeatureID	5GDU-002-007
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_eio_msiExtInNtfyStartInd()
{
	UINT					regVal;
	INT						errcd;
	INT						rtn_bpf;
	
	/*	外部IO状態を初期化	*/
	f_eiow_extIo.svStart    = D_RRH_ON;		/*	外部IO入力監視開始	*/
	f_eiow_extIo.lineInRep  = D_EIO_EXTIO_INVALID;	/*	外部IO入力未報告	*/
	
	/*	外部IO入力状態を読出	*/
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_OTH_GPIO_DATA_2_RO, &regVal);
	
	/*	外部IO入力値がオープン(ON)	*/
	if((regVal & D_DU_REGBIT_OTH_GPIO_DATA_2_RO_LINEIN) != 0)
	{
		f_eiow_extIo.lineInReg  = D_EIO_EXTIO_OPEN;	/*	オープン(OFF)	*/
	}
	/*	外部IO入力値がショート(OFF)	*/
	else
	{
		f_eiow_extIo.lineInReg  = D_EIO_EXTIO_SHORT;	/*	ショート(ON)	*/
	}
	/*	FAN監視制御周期タイマ(100ms)生成	*/
	rtn_bpf = BPF_RU_HRTM_REGISTER( BPF_RU_HRTM_REG_PERIODIC,						/* Timer種別						*/
									E_RRH_TIMID_EIO_EXTIOSV,						/* TimerID							*/
									D_EIO_EXTOUTSV_TIMVAL,							/* Timer値							*/
									D_SYS_MSGID_EIO_EXTOUTSV_TO,					/* TimeOut時通知Event番号			*/
									D_SYS_THDQID_PF_EIO,							/* TimeOut時送信先Queue番号			*/
									&errcd );
	
	if( rtn_bpf != 0 )
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "EXT LINE IN SV TIMER START NG 0x%08x(0x%08x)", rtn_bpf, errcd );
	}
	
	return ;
}

/* @} */

