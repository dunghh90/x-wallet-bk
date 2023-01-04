/*!
 * @skip  $ld:$
 * @file  f_fdl_filewrite_m.c
 * @brief FDLタスク　ファイル更新書込み応答(マスタ)
 * @date  2019/02/23 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */

#include "f_fdl_inc.h"


/*!
 * @brief 関数機能概要:マスタ側のファイル更新応答を受信し処理する
 * @note  関数処理内容.
 *		-# 全更新ファイルの応答チェック
 *		-# ファイル更新が終わればNotificationを通知する
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/02/23 KCN)上本 Create
 */
INT	f_fdl_filewrite_m(VOID* rcvMsg_p)
{
	INT				rtn = D_SYS_OK ;
	INT				ret = D_SYS_OK ;
	struct stat		stat_buf;
	T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND		sndMsg ;
	T_SYS_NMA_FILE_WRITE_RSP *								Rcv_p ;

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN, "f_fdl_filewrite_m start.") ;

	memset(&sndMsg , D_RRH_ZERO , sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND));
	memset(&stat_buf , D_FDL_ACT_STATE_RUN , sizeof(stat_buf)) ;

	if(f_fdl_swact_data[D_DFL_SLAVE_ACSS] == D_FDL_ACT_STATE_ERROR)
	{
		/* 異常終了(返却済) */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Install(Slave) Error") ;
		f_fdl_thdInfo.thdState = D_FDL_TASK_STS_ACT ;
		ret = D_SYS_NG ;
		return ret ;
	}
	Rcv_p = (T_SYS_NMA_FILE_WRITE_RSP *)rcvMsg_p ;
	switch(Rcv_p->kind)
	{
		case	E_RRH_FILE_KIND_KERNEL0 :						/* OSカーネルのファイル更新(0面)	*/
		case	E_RRH_FILE_KIND_KERNEL1 :						/* OSカーネルのファイル更新(1面)	*/
			/* OSファイルの更新結果を確認 */
			ret = f_fdl_install_check(rcvMsg_p , E_FDL_FLSH_OSPF) ;
			if(ret != D_SYS_OK)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Install(OS) Error") ;
				f_fdl_swact_data[D_DFL_MASTR_ACSS] = D_FDL_ACT_STATE_ERROR ;
				rtn = D_SYS_NG ;
			}else {
				ret = f_fdl_flash_write( E_FDL_FLSH_FPGAM , f_fdl_inst_state_data.slot ) ;
				if( ret != D_SYS_OK )
				{
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Install(PL-FPGA) Write Error") ;
					f_fdl_swact_data[D_DFL_MASTR_ACSS] = D_FDL_ACT_STATE_ERROR ;
					rtn = D_SYS_NG ;
				}else {
					f_fdl_inst_state_data.install_State[E_FDL_FLSH_FPGAM] = D_FDL_INST_START ;
				}
			}
			break ;
		case	E_RRH_FILE_KIND_PLTFRM0 :						/* FPGAのファイル更新(0面)			*/
		case	E_RRH_FILE_KIND_PLTFRM1 :						/* FPGAのファイル更新(1面)			*/
			/* FPGA(PLL)ファイルの更新結果を確認 */
			ret = f_fdl_install_check(rcvMsg_p , E_FDL_FLSH_FPGAM) ;
			if(ret != D_SYS_OK)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Install(PL-FPGA) Error") ;
				f_fdl_swact_data[D_DFL_MASTR_ACSS] = D_FDL_ACT_STATE_ERROR ;
				rtn = D_SYS_NG ;
			}else {
				if(f_cmw_tra_inv_tbl->cmx_device_typ == E_RRH_RRHKIND_5GDU_LLS_SUB6)
				{
					ret = f_fdl_flash_write( E_FDL_FLSH_RFIC_BOOT , f_fdl_inst_state_data.slot ) ;
					if( ret != D_SYS_OK )
					{
						BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Install(RFIC-Boot) Write Error") ;
						f_fdl_swact_data[D_DFL_MASTR_ACSS] = D_FDL_ACT_STATE_ERROR ;
						rtn = D_SYS_NG ;
					}else {
						f_fdl_inst_state_data.install_State[E_FDL_FLSH_RFIC_BOOT] = D_FDL_INST_START ;
					}
				}else {
					ret = f_fdl_flash_write( E_FDL_FLSH_PTP_PARAM , f_fdl_inst_state_data.slot ) ;
					if( ret != D_SYS_OK )
					{
						BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Install(PTP-PLL) Write Error") ;
						f_fdl_swact_data[D_DFL_MASTR_ACSS] = D_FDL_ACT_STATE_ERROR ;
						rtn = D_SYS_NG ;
					}else {
						f_fdl_inst_state_data.install_State[E_FDL_FLSH_PTP_PARAM] = D_FDL_INST_START ;
					}
				}
			}
			break ;
		case	E_RRH_FILE_KIND_RFICARM0 :						/* RFICのファイル(Boot)(0面)		*/
		case	E_RRH_FILE_KIND_RFICARM1 :						/* RFICのファイル(Boot)(1面)		*/
			/* RFIC-ARMファイルの更新結果を確認 */
			ret = f_fdl_install_check(rcvMsg_p , E_FDL_FLSH_RFIC_BOOT) ;
			if(ret != D_SYS_OK)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Install(RFIC-BOOT) Error") ;
				f_fdl_swact_data[D_DFL_MASTR_ACSS] = D_FDL_ACT_STATE_ERROR ;
				rtn = D_SYS_NG ;
			}else {
				ret = f_fdl_flash_write( E_FDL_FLSH_RFIC_ARM , f_fdl_inst_state_data.slot ) ;
				if( ret != D_SYS_OK )
				{
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Install(RFIC-ARM) Write Error") ;
					f_fdl_swact_data[D_DFL_MASTR_ACSS] = D_FDL_ACT_STATE_ERROR ;
					rtn = D_SYS_NG ;
				}else {
					f_fdl_inst_state_data.install_State[E_FDL_FLSH_RFIC_ARM] = D_FDL_INST_START ;
				}
			}
			break ;
		case	E_RRH_FILE_KIND_RFICFRM0 :						/* RFICのファイル(Firm)(0面)		*/
		case	E_RRH_FILE_KIND_RFICFRM1 :						/* RFICのファイル(Firm)(1面)		*/
			/* RFIC-ARMファイルの更新結果を確認 */
			ret = f_fdl_install_check(rcvMsg_p , E_FDL_FLSH_RFIC_ARM) ;
			if(ret != D_SYS_OK)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Install(RFIC-ARM) Error") ;
				f_fdl_swact_data[D_DFL_MASTR_ACSS] = D_FDL_ACT_STATE_ERROR ;
				rtn = D_SYS_NG ;
			}else {
				ret = f_fdl_flash_write( E_FDL_FLSH_PTP_PARAM , f_fdl_inst_state_data.slot ) ;
				if( ret != D_SYS_OK )
				{
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Install(PTP-PLL) Write Error") ;
					f_fdl_swact_data[D_DFL_MASTR_ACSS] = D_FDL_ACT_STATE_ERROR ;
					rtn = D_SYS_NG ;
				}else {
					f_fdl_inst_state_data.install_State[E_FDL_FLSH_PTP_PARAM] = D_FDL_INST_START ;
				}
			}
			break ;
		case	E_RRH_FILE_KIND_PTPPLL0 :						/* PTP-PLL初期設定ファイル(1面)		*/
		case	E_RRH_FILE_KIND_PTPPLL1 :						/* PTP-PLL初期設定ファイル(1面)		*/
			/* PTP-PLLファイルの更新結果を確認 */
			ret = f_fdl_install_check(rcvMsg_p , E_FDL_FLSH_PTP_PARAM) ;
			if(ret != D_SYS_OK)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Install(PTP-PLL) Error") ;
				f_fdl_swact_data[D_DFL_MASTR_ACSS] = D_FDL_ACT_STATE_ERROR ;
				rtn = D_SYS_NG ;
			}else {
				/* ファイル更新終了チェック確認 */
				ret = f_fdl_file_check() ;
				if(ret == D_SYS_OK)
				{
					/* 総合情報(ベンダコード/ビルドファイル名/バージョン)の更新 */
					ret = f_fdl_install_data_save() ;
					if(ret != D_SYS_OK)
					{
						BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR , "Install Data Set Error") ;
						f_fdl_send_MSInotif(E_FDL_ERR_BUFERR , f_fdl_inst_state_data.slot ) ;
						rtn = D_SYS_NG ;
						f_fdl_thdInfo.thdState = D_FDL_TASK_STS_ACT ;
					}
					else {
						/* OK応答(Notification通知)の送信 */
						f_fdl_send_MSInotif(E_FDL_ERR_OK , f_fdl_inst_state_data.slot ) ;
						f_fdl_thdInfo.thdState = D_FDL_TASK_STS_ACT ;
					}
				}
			}
			break ;
		default :												/* その他							*/
			/* ファイル更新終了チェック確認 */
			rtn = D_SYS_NG ;
			break ;
	}
	if(rtn == D_SYS_NG)
	{
		f_fdl_thdInfo.thdState = D_FDL_TASK_STS_ACT ;
	}

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN, "f_fdl_filewrite_m end.") ;
	return rtn ;
}
/* @} */


