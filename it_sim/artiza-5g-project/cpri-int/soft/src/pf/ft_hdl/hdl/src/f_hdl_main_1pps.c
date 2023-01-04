/*!
 * @skip  $ld:$
 * @file  f_hdl_main_1pps.c
 * @brief pf_hdl 子スレッド メイン処理(f_hdl_main_1pps)
 * @date  2018/06/22 Fujitsu) 吉田 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */
 
/*!
 * @addtogroup RRH_PF_HDL
 * @{
 */

#include "f_hdl_inc.h"
#include "f_com_inc.h"


/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/
/* Warning対策 */
int BPF_HM_DEVC_REG_READ(unsigned int loglevel, unsigned int offset, unsigned int *data_p);
int BPF_HM_DEVC_REG_WRITE(unsigned int loglevel, unsigned int offset, unsigned int *data_p);

/*!
 * @brief 関数機能概要: 1pps割り込み通知処理
 * @note  関数処理内容: 1pps割り込み通知処理
 * @param	-
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		FJT)Taniguchi
 */
static VOID f_hdl_1pps_ntc(VOID)
{
	UINT							endcd;
	INT								errcd;
	T_RRH_HEAD						*msgp;
	
	endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer種別				*/
										sizeof(T_RRH_HEAD),				/* Size						*/
										(void**)&msgp,					/* msgP						*/
										&errcd );
	if( endcd != D_RRH_OK ){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " ADDRGET ret=%08x, errcd=%08x ", endcd, errcd );
		return;
	}

	msgp->uiEventNo		= D_SYS_MSGID_TIMOUTNTC;					/* 1秒周期割り込み通知(0x10020003)	*/
	msgp->uiSignalkind	= 0;										/* IFログ有り						*/
	msgp->uiDstPQueueID = D_RRH_PROCQUE_PF;							/* 送信先Process Queue ID			*/
	msgp->uiDstTQueueID = D_SYS_THDQID_PF_CCA;						/* 送信先Thread Queue ID			*/
	msgp->uiSrcPQueueID = D_RRH_PROCQUE_PF;							/* 送信元Process Queue ID			*/
	msgp->uiSrcTQueueID = D_SYS_THDQID_PF_HD1PPS;					/* 送信元Thread Queue ID			*/
	msgp->uiLength = sizeof(T_RRH_HEAD);							/* Length(Head部のみ)				*/

	endcd = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_CCA,			/* 送信先Process Queue ID		*/
									(void*)msgp );					/* 送信Message					*/

	if( endcd != BPF_HM_DEVC_COMPLETE ){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " MSGQ_SEND %d ", endcd );
	}
	return;
}


/*!
 * @brief 関数機能概要: pf_hdl子スレッドメイン(f_hdl_main_1pps)。スレッド起動からMSG受信待ちのLoop処理を実施
 * @note  関数処理内容.
 *       -# 割り込み通知先管理テーブルより、割り込み待ち種別を取得
 *       -# 割り込み待ち無限Loop処理へ移行する
 *          -# 割り込み受信関数をCallする(BPF_HM_DEVC_RESERVE)
 *          -# SFN/FRMの補正計算を行う
 * @param	-
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2018/06/22 Fujitsu) 吉田 Create
 *        2018/08/24 kcn)Suehiro f_hdl_sfnfrm_calc()を切り出さず本関数内で対応
 */
VOID f_hdl_main_1pps()
{
	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "pf_hd1pps", 0, 0, 0);

	UINT					endcd;

	double 					cps_a;
	double 					cps_b;
	
	int 					sfn_offset_1;
	int 					sfn_offset_2;
	int 					sfn_offset_3 = 148; 		/* 固定値 */
	int 					clk_offset_1;

	int 					offset_a;					/* Offset_α読出し値格納用(単位: 1/1.2288*10^9) */
	int 					offset_b;					/* Offset_β読出し値格納用 */

	int 					tmp;						/* ハソ仕様書のX */
	unsigned int			sfn;						/* SFN補正値 */
	unsigned int			frm;						/* FRM補正値 */
	unsigned int 			val;
	unsigned int 			val2;

	/* 時刻取得用定義 */
	unsigned int reg = 1;
	unsigned int cpuTime_hi;
	unsigned int cpuTime_lo;
	long cpuTime = 0;


	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_hdl_main_1pps] ENTER" );

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET( D_RRH_PROCID_PF,					/* ProcessID						*/
								 D_SYS_THDID_PF_HD1PPS,				/* ThreadID							*/
								 D_SYS_THDQID_PF_HD1PPS,			/* ThreadQueueID					*/
								 (unsigned long int)pthread_self());	/* pThreadID	*/

	/****************/
	/* 補正計算開始 */
	/****************/
	cps_a = 1 / (1.2288 * pow(10.0, 9.0));								/* 1.2288^9 */
	cps_b = 491.52 * pow(10.0, 6.0);									/* 491.52^6 */
	
	/****************************************************************************************************/
	/* 割り込み受信待ち																					*/
	/****************************************************************************************************/
	while(1)
	{
		/* 割り込み受信		*/
		endcd	= BPF_HM_DEVC_RESERVE(BPF_IRQ_1PPS);				/* IRQ割り込み待ち処理				*/
		
		/* エラーログ取得	*/
		if( endcd != BPF_HM_DEVC_COMPLETE )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_hdl_main_1pps : error = 0x%x ", endcd);
			break;
		}

		if(f_comw_sfnfrm_tbl->ns_flag == D_HDL_1PPS_CALC)
		{
			offset_a = f_comw_sfnfrm_tbl->offset_a;
			offset_b = f_comw_sfnfrm_tbl->offset_b;


			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_hdl_sfnfrm_calc] ENTER" );
			
			/* SYSTimer取得 */
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xa1000610, &reg);
			BPF_HM_DEVC_REG_READ (D_RRH_LOG_REG_LV_READ,  0xa1000614, &cpuTime_hi); 
			BPF_HM_DEVC_REG_READ (D_RRH_LOG_REG_LV_READ,  0xa1000618, &cpuTime_lo); 

			cpuTime = cpuTime_hi * pow(2.0, 32.0) + cpuTime_lo;
			
			/* sfn_offset_1: FLOOR(MOD(256 - X * 100/(491.52^6),256),1) */
			tmp = round(offset_a * cps_a * cps_b);									/* ハソ仕様書のX */
			
			sfn_offset_1 = ((int)(256 - (double)(tmp * 100)/ cps_b ) % 256);
			
			/* CLK_Offset1: MOD(4915200 - ROUND( (X * 0.011) * 491.52^6,0),4915200) */
			/* CLK_Offset1=MOD((4915200-ROUND(X,0)),4915200) */
			/* (4915200 - round( tmp * 0.011 * cps_b)) が負となる可能性がある */
			/* clk_offset_1 = (int)(4915200 - round( tmp * 0.011 * cps_b)) % 4915200 ; */
			/* clk_offset_1 = (int) (( (int)(4915200 - round( tmp * 0.011 * cps_b)) % 4915200 ) + 4915200 ) % 4915200 ; */
			clk_offset_1 = (int)(4915200 - tmp) % 4915200 ;
		
			/* SFN_Offset2: MOD (256 + (-1)* MOD([Offset_β],256)),256) */
			sfn_offset_2 = (256 + (-1)* (offset_b % 256)) % 256;
			
			/* SFN補正: MOD( MOD((SYS_Timer[47:0] + 1)*100,256) + [SFN_Offset1] + [SFN_Offset2] + [SFN_Offset3]) */
			sfn = ((cpuTime + 1)*100 % 256 + sfn_offset_1 + sfn_offset_2 + sfn_offset_3 ) % 256;
			
			/* FRM補正 ROUND([CLK_Offset1],0) */
			frm = clk_offset_1;

			/* FPGAへの書込み */
			/* SFN補正値は、A200_0000[31:24]へ、FRM補正値はA200_0000[22:0] */
			val = ((sfn << 24) & 0xFF000000) + ( frm & 0x7FFFFF);
			(void)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xA2000000, &val);

			/* 更新トリガ設定 */
			val2 = 0x00000001;
			(void)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xA2000004, &val2);

			/* NS補正フラグ完了待ち */
			f_comw_sfnfrm_tbl->ns_flag = D_HDL_1PPS_CALC_CMP_WAIT;
			
			/* 結果ログ出力 */
			BPF_COM_PLOG_TRACE_PTPLOG(E_TRC_LV_INFO,"1PPS IRQ adjust set cpuTime:%ld Hi:%u Lo:%u SFN:0x%x FRM:0x%x wVal:0x%x a:0x%x b:0x%x", cpuTime,cpuTime_hi,cpuTime_lo,sfn,frm,val,val2,f_comw_sfnfrm_tbl->offset_a,f_comw_sfnfrm_tbl->offset_b);
	
		} else if(f_comw_sfnfrm_tbl->ns_flag == D_HDL_1PPS_CALC_WAIT){

			f_comw_sfnfrm_tbl->ns_flag = D_HDL_1PPS_CALC;
			BPF_COM_PLOG_TRACE_PTPLOG(E_TRC_LV_INFO,"1PPS next IRQ wait offset a:0x%x b:0x%x",f_comw_sfnfrm_tbl->offset_a,f_comw_sfnfrm_tbl->offset_b);

		} else if(f_comw_sfnfrm_tbl->ns_flag == D_HDL_1PPS_CALC_CMP_WAIT){

			f_comw_sfnfrm_tbl->ns_flag = D_HDL_1PPS_CALC_STOP;
			BPF_COM_PLOG_TRACE_PTPLOG(E_TRC_LV_INFO,"1PPS IRQ SFN load compleated a:0x%x b:0x%x",f_comw_sfnfrm_tbl->offset_a,f_comw_sfnfrm_tbl->offset_b);

			if ( M_DU_RRHKIND_KIND_GET(f_comw_rrhKind) == D_RRH_EEP_DEV_KIND_5GDU_SUB6 ){
				f_hdl_1pps_ntc();
			}

		} else {

			f_comw_sfnfrm_tbl->ns_flag = D_HDL_1PPS_CALC_STOP;
		}
		
/**/	/* BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "call of f_hdl_main_1s"); */
	}
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_hdl_main_1sec] RETURN" );
	return;
}

/* @} */
