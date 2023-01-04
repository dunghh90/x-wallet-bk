/*!
 * @skip  $ld:$
 * @file  f_hdl_main_state_fheth1.c
 * @brief pf_hdl 子スレッド メイン処理(f_hdl_main_state_fheth1)
 * @date  2018/08/20 Fujitsu) 吉田 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */
 
/*!
 * @addtogroup RRH_PF_HDL
 * @{
 */

#include "f_hdl_inc.h"
#include "f_com_inc.h"

/*!
 * @brief 関数機能概要: pf_hdl子スレッドメイン(f_hdl_main_state_fheth1)。スレッド起動からMSG受信待ちのLoop処理を実施
 * @note  関数処理内容.
 *       -# 割り込み通知先管理テーブルより、割り込み待ち種別を取得
 *       -# 割り込み待ち無限Loop処理へ移行する
 *          -# 
 *          -# 
 * @param	-
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2018/09/04 Fujitsu) Taniguchi Create
 */
VOID f_hdl_state_fheth_log(T_RRH_LOG_ETH_ERR_HIS*	his_p,UINT linkupdown , UINT state , UINT info ,UINT sfp_state,UINT his_state, UINT jcpll_state )
{
	UINT	index;
	
	index = his_p->uiWriteIdx;
	if(index != 0){
		index %= D_RRH_LOG_ETHERR_RCD_NUM;
	}
	BPF_HM_DEVC_GETTIME(&(his_p->tRecord[index].sysTime));
	his_p->tRecord[index].regdata       = state;
	his_p->tRecord[index].regdata_info  = info;
	his_p->tRecord[index].linkupdown    = linkupdown;
	his_p->tRecord[index].regdata_sfp   = sfp_state;
	his_p->tRecord[index].regdata_his   = his_state;
	his_p->tRecord[index].regdata_jcpll = jcpll_state;
	his_p->tRecord[index].validCode     = 0x11111111;
	index++;
	index %= D_RRH_LOG_ETHERR_RCD_NUM;
	his_p->uiWriteIdx = index;
	return;
}

/*!
 * @brief 関数機能概要: pf_hdl子スレッドメイン(f_hdl_main_state_fheth1)。スレッド起動からMSG受信待ちのLoop処理を実施
 * @note  関数処理内容.
 *       -# 割り込み通知先管理テーブルより、割り込み待ち種別を取得
 *       -# 割り込み待ち無限Loop処理へ移行する
 *          -# 
 *          -# 
 * @param	-
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2018/09/04 Fujitsu) Taniguchi Create
 */
VOID f_hdl_main_state_fheth(UINT portno)
{
	UINT	fid;
	UINT	mask;
	UINT	sts;
	UINT	info;
	UINT	irq;
	UINT	regval;
	UINT	regval_info;
	UINT	regval_sfp;
	UINT	regval_his;
	UINT	regval_jcpll;
	INT		endcd;
	UINT	faultflg;
	T_RRH_LOG_ETH_ERR_HIS*	his_p;
	INT		ret;
	UINT	already_restart = D_RRH_OFF;
	UINT	count;
	UINT	sfpsts;
	UINT	hissts;
	UINT	jcpllsts;

	switch(portno){
		case 0: 
			fid = D_DU_FLTID_L1_LDOWN_FH1;
			mask = M_DU_REG_ETH_PORT0(D_DU_REG_ETH_IRQMACSTM);
			sts = M_DU_REG_ETH_PORT0(D_DU_REG_ETH_IRQMACST);
			sfpsts = D_DU_REG_CTB_SFPSTA;
			hissts = D_DU_REG_ETH_HISMACST;
			jcpllsts = D_DU_REG_CTB_JCPLLALM ;
			info = M_DU_REG_ETH_PORT0(D_DU_REG_ETH_IRQMACSTI);
			irq = BPF_IRQ_FHETHMAC0;
			ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_MAC_STAT_ETH0,(VOID**)&his_p, &endcd);
			break;
		default:
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "port error = 0x%x ",portno);
			return;
			break;
	}

	if(ret != BPF_RU_IPCM_OK){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "port%d shmadd error = 0x%x ",portno,endcd);
		return;
	}

	while(1){
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,info,&regval_info);
		f_com_taskDelay(200,0,0,0);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,info,&regval);
		if(regval == regval_info){
			break;
		}
	}

	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,sfpsts,&regval_sfp);
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,hissts,&regval_his);
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,jcpllsts,&regval_jcpll);

	if((regval_info & 1) != 0){
		f_com_SVCtl(E_DU_SVCTL_OPE_FID05_ALM, D_DU_SVCTL_ON, fid);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "FH-Eth%d MAC status start LOS info:0x%08x sfp:0x%08x his:0x%08x jc:0x%08x",portno+1,regval_info,regval_sfp,regval_his,regval_jcpll);
		faultflg = D_RRH_ON;
	}else{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "FH-Eth%d MAC status start info:0x%08x sfp:0x%08x his:0x%08x jc:0x%08x",portno+1,regval_info,regval_sfp,regval_his,regval_jcpll);
		faultflg = D_RRH_OFF;
	}

	f_hdl_state_fheth_log(his_p, D_RRH_ON ,0xFFFFFFFF , regval_info ,regval_sfp,regval_his,regval_jcpll);

	BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, mask, 0xFF );
	while(1){
		endcd = BPF_HM_DEVC_RESERVE(irq);			/* IRQ割り込み待ち処理				*/
		if( endcd != BPF_HM_DEVC_COMPLETE )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "error = 0x%x ", endcd);
			break;
		}

		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,sfpsts,&regval_sfp);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,hissts,&regval_his);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,jcpllsts,&regval_jcpll);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,sts,&regval);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,info,&regval_info);
		if((regval_info & D_RRH_ON) == 0){
			if(faultflg == D_RRH_ON){
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "FH-Eth%d MAC status change LOS Cease sts:0x%08x info:0x%08x sfp:0x%08x his:0x%08x jc:0x%08x",portno+1,regval,regval_info,regval_sfp,regval_his,regval_jcpll);
				f_com_SVCtl(E_DU_SVCTL_OPE_FID05_ALM, D_DU_SVCTL_OFF, fid);
				faultflg = D_RRH_OFF;
				BPF_COM_LOG_DMESG("FH-Eth%d LINK-UP sts:0x%08x info:0x%08x sfp:0x%08x his:0x%08x jc:0x%08x\n",portno+1,regval,regval_info,regval_sfp,regval_his,regval_jcpll);
			}
		}
		else{
			if(faultflg == D_RRH_OFF){
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "FH-Eth%d MAC status change LOS sts:0x%08x info:0x%08x sfp:0x%08x his:0x%08x jc:0x%08x",portno+1,regval,regval_info,regval_sfp,regval_his,regval_jcpll);
				f_com_SVCtl(E_DU_SVCTL_OPE_FID05_ALM, D_DU_SVCTL_ON, fid);
				faultflg = D_RRH_ON;
				BPF_COM_LOG_DMESG( "FH-Eth%d LINK-DOWN sts:0x%08x info:0x%08x sfp:0x%08x his:0x%08x jc:0x%08x\n",portno+1,regval,regval_info,regval_sfp,regval_his,regval_jcpll);
			}
		}
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,sts,&regval);
		f_hdl_state_fheth_log(his_p, faultflg, regval , regval_info,regval_sfp,regval_his,regval_jcpll);
		if(already_restart == D_RRH_OFF){
			if(faultflg == D_RRH_ON){
				if((f_comw_du_connection_tbl->mp.port_no == portno) && (f_comw_mp_supervison_flg == D_RRH_OFF)){
					/* DHCP確立済かつMplane supervion未状態で90秒間LOS継続してる場合は自律Reset */
					for(count=0; count < D_HDL_MP_RECOVERY_COUNT ;count++){
						f_com_taskDelay(200,0,0,0);

						BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,sfpsts,&regval_sfp);
						BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,hissts,&regval_his);
						BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,jcpllsts,&regval_jcpll);
						BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,sts,&regval);
						BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,info,&regval_info);
						BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,sts,&regval);
						if((regval_info & D_RRH_ON) == 0){
							BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "FH-Eth%d MAC status LOS recovery sts:0x%08x info:0x%08x sfp:0x%08x his:0x%08x jc:0x%08x",portno+1,regval,regval_info,regval_sfp,regval_his,regval_jcpll);
							f_com_SVCtl(E_DU_SVCTL_OPE_FID05_ALM, D_DU_SVCTL_OFF, fid);
							faultflg = D_RRH_OFF;
							f_hdl_state_fheth_log(his_p, faultflg, regval , regval_info,regval_sfp,regval_his,regval_jcpll);
							BPF_COM_LOG_DMESG("FH-Eth%d LINK-UP recovery sts:0x%08x info:0x%08x sfp:0x%08x his:0x%08x jc:0x%08x\n",portno+1,regval,regval_info,regval_sfp,regval_his,regval_jcpll);
							/* recovery */
							break;
						}
					}
					if(count >= D_HDL_MP_RECOVERY_COUNT){
						BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "FH-Eth%d LINK-DOWN Not recovery sts:0x%08x info:0x%08x sfp:0x%08x his:0x%08x jc:0x%08x",portno+1,regval,regval_info,regval_sfp,regval_his,regval_jcpll);
						BPF_COM_LOG_DMESG("FH-Eth%d LINK-DOWN sts:0x%08x info:0x%08x sfp:0x%08x jc:0x%08x and Self Reset ...\n",portno+1,regval,regval_info,regval_sfp,regval_his,regval_jcpll);
						BPF_RM_SVRM_SIGNAL_SEND(D_RRH_PROCID_INI, SIGUSR2, D_RRH_SIGUSR2_OPT_CPRIL1RST );
						already_restart = D_RRH_ON;
					}
				}
			}
		}
	}
	return;
}

/*!
 * @brief 関数機能概要: pf_hdl子スレッドメイン(f_hdl_main_state_fheth1)。スレッド起動からMSG受信待ちのLoop処理を実施
 * @note  関数処理内容.
 *       -# 割り込み通知先管理テーブルより、割り込み待ち種別を取得
 *       -# 割り込み待ち無限Loop処理へ移行する
 *          -# 
 *          -# 
 * @param	-
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2018/08/20 Fujitsu) 吉田 Create
 */
VOID f_hdl_main_state_fheth1()
{
	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "pf_hdms1", 0, 0, 0);


	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_hdl_main_state_fheth1] ENTER" );

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET( D_RRH_PROCID_PF,					/* ProcessID						*/
								 D_SYS_THDID_PF_HDMSTA1,			/* ThreadID							*/
								 D_SYS_THDQID_PF_HDMSTA1,			/* ThreadQueueID					*/
								 (unsigned long int)pthread_self());	/* pThreadID					*/

	f_hdl_main_state_fheth(0);
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_hdl_main_state_fheth1] RETURN" );

	return;
}

/*!
 * @brief 関数機能概要: pf_hdl子スレッドメイン(f_hdl_main_state_fheth1)。スレッド起動からMSG受信待ちのLoop処理を実施
 * @note  関数処理内容.
 *       -# 割り込み通知先管理テーブルより、割り込み待ち種別を取得
 *       -# 割り込み待ち無限Loop処理へ移行する
 *          -# 
 *          -# 
 * @param	-
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2018/08/20 Fujitsu) 吉田 Create
 */
VOID f_hdl_main_state_fheth2()
{
	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "pf_hdms2", 0, 0, 0);


	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_hdl_main_state_fheth2] ENTER" );

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET( D_RRH_PROCID_PF,					/* ProcessID						*/
								 D_SYS_THDID_PF_HDMSTA2,			/* ThreadID							*/
								 D_SYS_THDQID_PF_HDMSTA2,			/* ThreadQueueID					*/
								 (unsigned long int)pthread_self());	/* pThreadID					*/


	f_hdl_main_state_fheth(1);
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_hdl_main_state_fheth2] RETURN" );
	return;
}


/* @} */
