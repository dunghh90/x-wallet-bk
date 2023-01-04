/*!
 *  @skip   $Id:$
 *  @file   f_trp_com_rom.c
 *  @brief  TRIF common RAM table define
 *  @date   2007/03/16  FFCS)linjunhua create
 *  @date   2008/07/28  FFCS)tangj  S3G PRC eNB-008-002
 *  @date   2009/01/16  FFCS)tangj  Add MTR thread
 *  @date   2010/08/12 CMS)Takahashi modify for M-S3G-eNBSYS-01722
 *  @date   2011/02/10 FJT)Tokunaga CeNB-F-062-033(DCM) 運用パラメータの反映元と反映タイミング
 *  @date   2011/12/09 FJT)K.Koshida D-MICRO-008
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2011
 */
 
#include "f_trp_com.h"
#include "f_trp_com_ext.h"

/*! @addtogroup TRIF_TCM
 * @{ */

///* スレッド起動情報テーブル */
//T_TCM_THRSUPINF_TBL f_trp_com_Rthrsupinf_tbl = {{
//  /* スケジューリングポリシー */
//                                 /* スレッド優先度 */
//                                                     /* 起動関数 */
//    {BPF_RU_ITCM_SCHED_FIFO,    D_TCM_THRPRI_TRM,   f_trp_trm_Fr_main       },  /* TRIFメイン */
//    {BPF_RU_ITCM_SCHED_FIFO,    D_TCM_THRPRI_TRO,   f_trp_tro_Fr_main       },  /* TRIF OCM受信 */
//    {BPF_RU_ITCM_SCHED_FIFO,    D_TCM_THRPRI_CPC,   t_cr_main               },  /* CPRI制御 */
//    {BPF_RU_ITCM_SCHED_FIFO,    D_TCM_THRPRI_REC,   f_trp_rec_Fcom_r_main   },  /* RE制御 */
//    {BPF_RU_ITCM_SCHED_FIFO,    D_TCM_THRPRI_RED,   f_trp_red_F_r_main      },  /* REダウンロード */
//    {BPF_RU_ITCM_SCHED_FIFO,    D_TCM_THRPRI_EDT,   t_er_Main               },  /* 外部データ転送 */    
//    {BPF_RU_ITCM_SCHED_FIFO,    D_TCM_THRPRI_CMC,   t_mk_main               },  /* CPRI-MT制御 */
//    {BPF_RU_ITCM_SCHED_FIFO,    D_TCM_THRPRI_LPB,   f_trp_lpb_Fcom_r_main   },  /* LAPB */
//    {BPF_RU_ITCM_SCHED_FIFO,    D_TCM_THRPRI_TXS,   t_tx_Main               },  /* TXIOCS */
//    {BPF_RU_ITCM_SCHED_FIFO,    D_TCM_THRPRI_RXS,   t_rx_Main               },  /* RXIOCS */
//    {BPF_RU_ITCM_SCHED_FIFO,    D_TCM_THRPRI_MTR,   f_trp_mtr_Fr_main       }   /* MTR  */
//}};
//
//
/*!< timeout entry function list. 
 * the order is same to スレッド情報テーブル. Don't to change the array order.
 */
//T_TCM_TIMEOUT_ENTRY f_trp_com_Rtimout_ent[D_TCM_THRINO_MAX] = {
//    f_trp_com_Ftimeoutfor_trm,  /* TRIFメイン　timeout エントリ関数     */
//    f_trp_com_Ftimeoutfor_tro,  /* TRIF OCM　　timeout エントリ関数     */
//    f_trp_com_Ftimeoutfor_cpc,  /* CPRI制御　　timeout エントリ関数     */
//    f_trp_com_Ftimeoutfor_rec,  /* RE制御　timeout エントリ関数         */
//    f_trp_com_Ftimeoutfor_red,  /* REダウンロード　timeout エントリ関数 */
//    f_trp_com_Ftimeoutfor_edt,  /* 外部データ転送　timeout エントリ関数 */
//    f_trp_com_Ftimeoutfor_cmc,  /* CPRI-MT制御　timeout エントリ関数    */
//    f_trp_com_Ftimeoutfor_lpb,  /* LAPB　timeout エントリ関数           */
//    f_trp_com_Ftimeoutfor_txs,  /* TXIOCS　timeout エントリ関数         */
//    f_trp_com_Ftimeoutfor_rxs,  /* RXIOCS　timeout エントリ関数         */
//	f_trp_com_Ftimeoutfor_mtr   /* MTR task　timeout エントリ関数 		*/
//};

///**************************************/
///* スレッド別ログコマンド関数テーブル */
///**************************************/
//T_TCM_LOGFNC_TBL f_trp_com_Rlogfnc_tbl = {{
//            /* ステータスログ出力関数 */    /* リソースログ出力関数 */
//    {"TRM", f_trp_trm_Ft_sta_dsp    ,       NULL                        },  /* TRM */
//    {"TRO", NULL                    ,       NULL                        },  /* TRO */
//    {"CPC", f_trp_cpc_Fcom_t_sta_dsp,       NULL                        },  /* CPC */
//    {"REC", f_trp_rec_Fcom_t_sta_dsp,       NULL                        },  /* REC */
//    {"RED", f_trp_red_F_t_sta_dsp   ,       NULL                        },  /* RED */
//    {"EDT", f_trp_edt_Ft_sta_dsp    ,       NULL                        },  /* EDT */
//    {"CMC", f_trp_cmc_Fcom_t_sta_dsp,       NULL                        },  /* CMC */
//    {"LPB", f_trp_lpb_Fcom_t_sta_dsp,       NULL                        },  /* LPB */
//    {"TXS", f_trp_txs_Ft_sta_dsp    ,       NULL                        },  /* TXS */
//    {"RXS", f_trp_rxs_Ft_sta_dsp    ,       NULL                        },  /* RXS */
//    {"MTR", f_trp_mtr_Ft_sta_dsp,       NULL                        },  /* MTR */
//    {"TCM", f_trp_com_Flogout_sta   ,       f_trp_com_Flogout_res       }   /* TCM */
//}};
//
/*!< buffer infomation table */
/* for 16M buffer, no buffer busy control threshold */
//CMT_BUFFINF_TBL f_trp_Rbuffinf_tbl[CMD_BUFFTYP_MAX] = 
//{
//    /* buffer type */     /* buffer count */    /* busy on threshold */ /* busy off threshold */ 
//    {BPF_RU_MBLM_BUFTYPE_001, CMD_BUFF64_NUM  ,  CMD_BUFF64_NUM    / 10,  CMD_BUFF64_NUM   *3 / 20},
//    {BPF_RU_MBLM_BUFTYPE_002, CMD_BUFF256_NUM ,  CMD_BUFF256_NUM   / 10,  CMD_BUFF256_NUM  *3 / 20},
//    {BPF_RU_MBLM_BUFTYPE_003, CMD_BUFF1024_NUM,  CMD_BUFF1024_NUM  / 10,  CMD_BUFF1024_NUM *3 / 20},
//    {BPF_RU_MBLM_BUFTYPE_004, CMD_BUFF4096_NUM,  CMD_BUFF4096_NUM  / 10,  CMD_BUFF4096_NUM *3 / 20},
//    {BPF_RU_MBLM_BUFTYPE_005, CMD_BUFF16384_NUM,  CMD_BUFF16384_NUM / 10,  CMD_BUFF16384_NUM*3 / 20},
//    {BPF_RU_MBLM_BUFTYPE_006, CMD_BUFF16M_NUM ,  CMD_NUM0       ,  CMD_NUM0         }
//};

///* ローカルレジスタ間接アクセス制御 */
//UCHAR const f_trp_com_local_ctrl[CMD_CPRINO_MAX/CMD_NUM2] = 
//{
//	D_TCM_CPRIMUX_REGADR_LK1_LCR_CTRL,		/* LINK#1　ローカルレジスタ間接アクセス制御 */
//	D_TCM_CPRIMUX_REGADR_LK2_LCR_CTRL,		/* LINK#2　ローカルレジスタ間接アクセス制御 */
//	D_TCM_CPRIMUX_REGADR_LK3_LCR_CTRL		/* LINK#3　ローカルレジスタ間接アクセス制御 */
//};
//
///* ローカルレジスタ間接アクセスアドレス */
//UCHAR const f_trp_com_local_adr[CMD_CPRINO_MAX/CMD_NUM2] = 
//{
//	D_TCM_CPRIMUX_REGADR_LK1_LCR_ADR,		/* LINK#1　ローカルレジスタ間接アクセスアドレス */
//	D_TCM_CPRIMUX_REGADR_LK2_LCR_ADR,		/* LINK#2　ローカルレジスタ間接アクセスアドレス */
//	D_TCM_CPRIMUX_REGADR_LK3_LCR_ADR		/* LINK#3　ローカルレジスタ間接アクセスアドレス */
//};
//
///* ローカルレジスタ間接アクセスデータ */
//UCHAR const f_trp_com_local_data[CMD_CPRINO_MAX/CMD_NUM2] = 
//{
//	D_TCM_CPRIMUX_REGADR_LK1_LCR_DATA,		/* LINK#1　ローカルレジスタ間接アクセスデータ */
//	D_TCM_CPRIMUX_REGADR_LK2_LCR_DATA,		/* LINK#2　ローカルレジスタ間接アクセスデータ */
//	D_TCM_CPRIMUX_REGADR_LK3_LCR_DATA		/* LINK#3　ローカルレジスタ間接アクセスデータ */
//};
//
///* Semophone ID for CPRIMUX local register access */
//UINT const f_trp_com_semphone_id[CMD_NUM2][CMD_CPRINO_MAX/CMD_NUM2] = 
//{
//	{
//		D_SEM_CPU_PROC_CPMX1R1,		
//		D_SEM_CPU_PROC_CPMX1R2,
//		D_SEM_CPU_PROC_CPMX1R3
//	},
//	
//	{
//		D_SEM_CPU_PROC_CPMX2R1,
//		D_SEM_CPU_PROC_CPMX2R2,
//		D_SEM_CPU_PROC_CPMX2R3
//	}	
//};	
//
//
//const UINT f_trp_com_l1reglog[CMD_LINK_NUM_T][CMD_NUM16] = {
//	{
//		CMD_REGADR_CPRISTA_CPR01,		/* 0:CPRIステート */
//		CMD_REGADR_RCVSTA_CPR01,		/* 1:HDLC受信状態 */
//		CMD_REGADR_RCVDES_CPR01,		/* 2:HDLC受信Discripter */
//		CMD_REGADR_RCVORD_CPR01,		/* 3:HDLC受信Order */
//		CMD_REGADR_RCVIOSTA_CPR01,		/* 4:HDLC受信IO状態 */
//		CMD_REGADR_RCVCSW0_CPR01,		/* 5:HDLC受信CSW0 */
//		CMD_REGADR_SNDDES_CPR01,		/* 6:HDLC送信Discripter */
//		CMD_REGADR_SNDORD_CPR01,		/* 7:HDLC送信Order */
//		CMD_REGADR_SNDIOSTA_CPR01,		/* 8:HDLC送信IO状態 */
//		CMD_REGADR_SNDCSW0_CPR01,		/* 9:HDLC送信CSW0 */
//		CMD_REGADR_SNDDESDATA0_CPR01,	/* 10:HDLC送信DiscripterData0 */
//		CMD_REGADR_SNDDESDATA1_CPR01,	/* 11:HDLC送信DiscripterData1 */
//		CMD_REGADR_SNDDESDATA2_CPR01,	/* 12:HDLC送信DiscripterData2 */
//		CMD_REGADR_SNDDESDATA3_CPR01,	/* 13:HDLC送信DiscripterData3 */
//		CMD_REGADR_SENDDMASINTCOUNT_CPR01,	/* 14:CPRI#1送信DMAソフト割り込み数カウンタ */
//		CMD_REGADR_SENDDMAHINTCOUNT_CPR01	/* 15:CPRI#1送信DMAハード割り込み数カウンタ */
//	},
//	{
//		CMD_REGADR_CPRISTA_CPR02,		/* 0:CPRIステート */
//		CMD_REGADR_RCVSTA_CPR02,		/* 1:HDLC受信状態 */
//		CMD_REGADR_RCVDES_CPR02,		/* 2:HDLC受信Discripter */
//		CMD_REGADR_RCVORD_CPR02,		/* 3:HDLC受信Order */
//		CMD_REGADR_RCVIOSTA_CPR02,		/* 4:HDLC受信IO状態 */
//		CMD_REGADR_RCVCSW0_CPR02,		/* 5:HDLC受信CSW0 */
//		CMD_REGADR_SNDDES_CPR02,		/* 6:HDLC送信Discripter */
//		CMD_REGADR_SNDORD_CPR02,		/* 7:HDLC送信Order */
//		CMD_REGADR_SNDIOSTA_CPR02,		/* 8:HDLC送信IO状態 */
//		CMD_REGADR_SNDCSW0_CPR02,		/* 9:HDLC送信CSW0 */
//		CMD_REGADR_SNDDESDATA0_CPR02,	/* 10:HDLC送信DiscripterData0 */
//		CMD_REGADR_SNDDESDATA1_CPR02,	/* 11:HDLC送信DiscripterData1 */
//		CMD_REGADR_SNDDESDATA2_CPR02,	/* 12:HDLC送信DiscripterData2 */
//		CMD_REGADR_SNDDESDATA3_CPR02,	/* 13:HDLC送信DiscripterData3 */
//		CMD_REGADR_SENDDMASINTCOUNT_CPR02,	/* 14:CPRI#2送信DMAソフト割り込み数カウンタ */
//		CMD_REGADR_SENDDMAHINTCOUNT_CPR02	/* 15:CPRI#2送信DMAハード割り込み数カウンタ */
//	},
//	{
//		CMD_REGADR_CPRISTA_CPR03,		/* 0:CPRIステート */
//		CMD_REGADR_RCVSTA_CPR03,		/* 1:HDLC受信状態 */
//		CMD_REGADR_RCVDES_CPR03,		/* 2:HDLC受信Discripter */
//		CMD_REGADR_RCVORD_CPR03,		/* 3:HDLC受信Order */
//		CMD_REGADR_RCVIOSTA_CPR03,		/* 4:HDLC受信IO状態 */
//		CMD_REGADR_RCVCSW0_CPR03,		/* 5:HDLC受信CSW0 */
//		CMD_REGADR_SNDDES_CPR03,		/* 6:HDLC送信Discripter */
//		CMD_REGADR_SNDORD_CPR03,		/* 7:HDLC送信Order */
//		CMD_REGADR_SNDIOSTA_CPR03,		/* 8:HDLC送信IO状態 */
//		CMD_REGADR_SNDCSW0_CPR03,		/* 9:HDLC送信CSW0 */
//		CMD_REGADR_SNDDESDATA0_CPR03,	/* 10:HDLC送信DiscripterData0 */
//		CMD_REGADR_SNDDESDATA1_CPR03,	/* 11:HDLC送信DiscripterData1 */
//		CMD_REGADR_SNDDESDATA2_CPR03,	/* 12:HDLC送信DiscripterData2 */
//		CMD_REGADR_SNDDESDATA3_CPR03,	/* 13:HDLC送信DiscripterData3 */
//		CMD_REGADR_SENDDMASINTCOUNT_CPR03,	/* 14:CPRI#3送信DMAソフト割り込み数カウンタ */
//		CMD_REGADR_SENDDMAHINTCOUNT_CPR03	/* 15:CPRI#3送信DMAハード割り込み数カウンタ */
//	},
//	{
//		CMD_REGADR_CPRISTA_CPR04,		/* 0:CPRIステート */
//		CMD_REGADR_RCVSTA_CPR04,		/* 1:HDLC受信状態 */
//		CMD_REGADR_RCVDES_CPR04,		/* 2:HDLC受信Discripter */
//		CMD_REGADR_RCVORD_CPR04,		/* 3:HDLC受信Order */
//		CMD_REGADR_RCVIOSTA_CPR04,		/* 4:HDLC受信IO状態 */
//		CMD_REGADR_RCVCSW0_CPR04,		/* 5:HDLC受信CSW0 */
//		CMD_REGADR_SNDDES_CPR04,		/* 6:HDLC送信Discripter */
//		CMD_REGADR_SNDORD_CPR04,		/* 7:HDLC送信Order */
//		CMD_REGADR_SNDIOSTA_CPR04,		/* 8:HDLC送信IO状態 */
//		CMD_REGADR_SNDCSW0_CPR04,		/* 9:HDLC送信CSW0 */
//		CMD_REGADR_SNDDESDATA0_CPR04,	/* 10:HDLC送信DiscripterData0 */
//		CMD_REGADR_SNDDESDATA1_CPR04,	/* 11:HDLC送信DiscripterData1 */
//		CMD_REGADR_SNDDESDATA2_CPR04,	/* 12:HDLC送信DiscripterData2 */
//		CMD_REGADR_SNDDESDATA3_CPR04,	/* 13:HDLC送信DiscripterData3 */
//		CMD_REGADR_SENDDMASINTCOUNT_CPR04,	/* 14:CPRI#4送信DMAソフト割り込み数カウンタ */
//		CMD_REGADR_SENDDMAHINTCOUNT_CPR04	/* 15:CPRI#4送信DMAハード割り込み数カウンタ */
//	},
//	{
//		CMD_REGADR_CPRISTA_CPR05,		/* 0:CPRIステート */
//		CMD_REGADR_RCVSTA_CPR05,		/* 1:HDLC受信状態 */
//		CMD_REGADR_RCVDES_CPR05,		/* 2:HDLC受信Discripter */
//		CMD_REGADR_RCVORD_CPR05,		/* 3:HDLC受信Order */
//		CMD_REGADR_RCVIOSTA_CPR05,		/* 4:HDLC受信IO状態 */
//		CMD_REGADR_RCVCSW0_CPR05,		/* 5:HDLC受信CSW0 */
//		CMD_REGADR_SNDDES_CPR05,		/* 6:HDLC送信Discripter */
//		CMD_REGADR_SNDORD_CPR05,		/* 7:HDLC送信Order */
//		CMD_REGADR_SNDIOSTA_CPR05,		/* 8:HDLC送信IO状態 */
//		CMD_REGADR_SNDCSW0_CPR05,		/* 9:HDLC送信CSW0 */
//		CMD_REGADR_SNDDESDATA0_CPR05,	/* 10:HDLC送信DiscripterData0 */
//		CMD_REGADR_SNDDESDATA1_CPR05,	/* 11:HDLC送信DiscripterData1 */
//		CMD_REGADR_SNDDESDATA2_CPR05,	/* 12:HDLC送信DiscripterData2 */
//		CMD_REGADR_SNDDESDATA3_CPR05,	/* 13:HDLC送信DiscripterData3 */
//		CMD_REGADR_SENDDMASINTCOUNT_CPR05,	/* 14:CPRI#5送信DMAソフト割り込み数カウンタ */
//		CMD_REGADR_SENDDMAHINTCOUNT_CPR05	/* 15:CPRI#5送信DMAハード割り込み数カウンタ */
//	},
//	{
//		CMD_REGADR_CPRISTA_CPR06,		/* 0:CPRIステート */
//		CMD_REGADR_RCVSTA_CPR06,		/* 1:HDLC受信状態 */
//		CMD_REGADR_RCVDES_CPR06,		/* 2:HDLC受信Discripter */
//		CMD_REGADR_RCVORD_CPR06,		/* 3:HDLC受信Order */
//		CMD_REGADR_RCVIOSTA_CPR06,		/* 4:HDLC受信IO状態 */
//		CMD_REGADR_RCVCSW0_CPR06,		/* 5:HDLC受信CSW0 */
//		CMD_REGADR_SNDDES_CPR06,		/* 6:HDLC送信Discripter */
//		CMD_REGADR_SNDORD_CPR06,		/* 7:HDLC送信Order */
//		CMD_REGADR_SNDIOSTA_CPR06,		/* 8:HDLC送信IO状態 */
//		CMD_REGADR_SNDCSW0_CPR06,		/* 9:HDLC送信CSW0 */
//		CMD_REGADR_SNDDESDATA0_CPR06,	/* 10:HDLC送信DiscripterData0 */
//		CMD_REGADR_SNDDESDATA1_CPR06,	/* 11:HDLC送信DiscripterData1 */
//		CMD_REGADR_SNDDESDATA2_CPR06,	/* 12:HDLC送信DiscripterData2 */
//		CMD_REGADR_SNDDESDATA3_CPR06,	/* 13:HDLC送信DiscripterData3 */
//		CMD_REGADR_SENDDMASINTCOUNT_CPR06,	/* 14:CPRI#6送信DMAソフト割り込み数カウンタ */
//		CMD_REGADR_SENDDMAHINTCOUNT_CPR06	/* 15:CPRI#6送信DMAハード割り込み数カウンタ */
//	}
//};
//
//UINT const f_trp_com_BandWidth_reg[CMD_CPRINO_MAX] =
//{
//#if !defined(OPT_CPRI_L2HARD)
//	D_TCM_CPRIMUX_REGADR_LK1_BAND,			/* LINK#1 帯域情報設定 */
//	D_TCM_CPRIMUX_REGADR_LK2_BAND,			/* LINK#2 帯域情報設定 */
//	D_TCM_CPRIMUX_REGADR_LK3_BAND,			/* LINK#3 帯域情報設定 */
//	D_TCM_CPRIMUX_REGADR_LK1_BAND,			/* LINK#1 帯域情報設定 */
//	D_TCM_CPRIMUX_REGADR_LK2_BAND,			/* LINK#2 帯域情報設定 */
//	D_TCM_CPRIMUX_REGADR_LK3_BAND			/* LINK#3 帯域情報設定 */
//#else
//	0x00000000,								/* LINK#1 帯域情報設定 */
//	0x00000000,								/* LINK#2 帯域情報設定 */
//	0x00000000,								/* LINK#3 帯域情報設定 */
//	0x00000000,								/* LINK#1 帯域情報設定 */
//	0x00000000,								/* LINK#2 帯域情報設定 */
//	0x00000000								/* LINK#3 帯域情報設定 */
//#endif
//};
//
//UINT const f_trp_com_CarrOperate_reg[CMD_CPRINO_MAX] =
//{
//#if !defined(OPT_CPRI_L2HARD)
//	D_TCM_CPRIMUX_REGADR_LK1_3G_CA,			/* LINK#1 3Gキャリア#X運用有無 */
//	D_TCM_CPRIMUX_REGADR_LK2_3G_CA,			/* LINK#2 3Gキャリア#X運用有無 */
//	D_TCM_CPRIMUX_REGADR_LK3_3G_CA,			/* LINK#3 3Gキャリア#X運用有無 */
//	D_TCM_CPRIMUX_REGADR_LK1_3G_CA,			/* LINK#1 3Gキャリア#X運用有無 */
//	D_TCM_CPRIMUX_REGADR_LK2_3G_CA,			/* LINK#2 3Gキャリア#X運用有無 */
//	D_TCM_CPRIMUX_REGADR_LK3_3G_CA 			/* LINK#3 3Gキャリア#X運用有無 */
//#else
//	CMD_REGADR_CPRI_1_3G_CARRIER,			/* LINK#1 3Gキャリア#X運用有無 */
//	CMD_REGADR_CPRI_2_3G_CARRIER,			/* LINK#2 3Gキャリア#X運用有無 */
//	0x00000000,								/* LINK#3 3Gキャリア#X運用有無 */
//	0x00000000,								/* LINK#1 3Gキャリア#X運用有無 */
//	0x00000000,								/* LINK#2 3Gキャリア#X運用有無 */
//	0x00000000								/* LINK#3 3Gキャリア#X運用有無 */
//#endif
//};
//
//UINT const f_trp_com_S3G_CpriSteadyTime_reg[CMD_CPRINO_MAX] = 
//{
//#if !defined(OPT_CPRI_L2HARD)
//	D_TCM_CPRIMUX_REGADR_LK1_CPRI_S3_T1,	/* CPRI安定化時間パラメータ(S3G) */
//	D_TCM_CPRIMUX_REGADR_LK2_CPRI_S3_T1,	/* CPRI安定化時間パラメータ(S3G) */
//	D_TCM_CPRIMUX_REGADR_LK3_CPRI_S3_T1,	/* CPRI安定化時間パラメータ(S3G) */
//	D_TCM_CPRIMUX_REGADR_LK1_CPRI_S3_T1,	/* CPRI安定化時間パラメータ(S3G) */
//	D_TCM_CPRIMUX_REGADR_LK2_CPRI_S3_T1,	/* CPRI安定化時間パラメータ(S3G) */
//	D_TCM_CPRIMUX_REGADR_LK3_CPRI_S3_T1		/* CPRI安定化時間パラメータ(S3G) */
//#else
//	D_CMM_FPGA_ADDR_CPRI_STB_TIME,			/* CPRI安定化時間パラメータ(S3G) */
//	D_CMM_FPGA_ADDR_CPRI_STB_TIME,			/* CPRI安定化時間パラメータ(S3G) */
//	0x00000000,								/* CPRI安定化時間パラメータ(S3G) */
//	0x00000000,								/* CPRI安定化時間パラメータ(S3G) */
//	0x00000000,								/* CPRI安定化時間パラメータ(S3G) */
//	0x00000000								/* CPRI安定化時間パラメータ(S3G) */
//#endif
//};                                          
//
//UINT const f_trp_com_3G_CpriSteadyTime_reg[CMD_CPRINO_MAX] = 
//{
//#if !defined(OPT_CPRI_L2HARD)
//	D_TCM_CPRIMUX_REGADR_LK1_CPRI_3G_T1,	/* CPRI安定化時間パラメータ(3G) */
//	D_TCM_CPRIMUX_REGADR_LK2_CPRI_3G_T1,	/* CPRI安定化時間パラメータ(3G) */
//	D_TCM_CPRIMUX_REGADR_LK3_CPRI_3G_T1,	/* CPRI安定化時間パラメータ(3G) */
//	D_TCM_CPRIMUX_REGADR_LK1_CPRI_3G_T1,	/* CPRI安定化時間パラメータ(3G) */
//	D_TCM_CPRIMUX_REGADR_LK2_CPRI_3G_T1,	/* CPRI安定化時間パラメータ(3G) */
//	D_TCM_CPRIMUX_REGADR_LK3_CPRI_3G_T1		/* CPRI安定化時間パラメータ(3G) */
//#else
//	0x00000000,								/* CPRI安定化時間パラメータ(3G) */
//	0x00000000,								/* CPRI安定化時間パラメータ(3G) */
//	0x00000000,								/* CPRI安定化時間パラメータ(3G) */
//	0x00000000,								/* CPRI安定化時間パラメータ(3G) */
//	0x00000000,								/* CPRI安定化時間パラメータ(3G) */
//	0x00000000								/* CPRI安定化時間パラメータ(3G) */
//#endif
//};
//
//#if !defined(OPT_CPRI_L2HARD)
//UINT const f_trp_com_DlReDly_reg[CMD_NUM3][CMD_CPRINO_MAX] = {
//	{
//		D_TCM_CPRIMUX_REGADR_LK1_DLRE_032,		/* RE 送信フレーム位相調整値設定 1/32chip単位 */
//		D_TCM_CPRIMUX_REGADR_LK2_DLRE_032,		/* RE 送信フレーム位相調整値設定 1/32chip単位 */
//		D_TCM_CPRIMUX_REGADR_LK3_DLRE_032,		/* RE 送信フレーム位相調整値設定 1/32chip単位 */
//		D_TCM_CPRIMUX_REGADR_LK1_DLRE_032,		/* RE 送信フレーム位相調整値設定 1/32chip単位 */
//		D_TCM_CPRIMUX_REGADR_LK2_DLRE_032,		/* RE 送信フレーム位相調整値設定 1/32chip単位 */
//		D_TCM_CPRIMUX_REGADR_LK3_DLRE_032		/* RE 送信フレーム位相調整値設定 1/32chip単位 */
//	},
//	{
//		D_TCM_CPRIMUX_REGADR_LK1_DLRE_640,		/* RE 送信フレーム位相調整値設定 1/640chip単位 */
//		D_TCM_CPRIMUX_REGADR_LK2_DLRE_640,		/* RE 送信フレーム位相調整値設定 1/640chip単位 */
//		D_TCM_CPRIMUX_REGADR_LK3_DLRE_640,		/* RE 送信フレーム位相調整値設定 1/640chip単位 */
//		D_TCM_CPRIMUX_REGADR_LK1_DLRE_640,		/* RE 送信フレーム位相調整値設定 1/640chip単位 */
//		D_TCM_CPRIMUX_REGADR_LK2_DLRE_640,		/* RE 送信フレーム位相調整値設定 1/640chip単位 */
//		D_TCM_CPRIMUX_REGADR_LK3_DLRE_640		/* RE 送信フレーム位相調整値設定 1/640chip単位 */
//	},
//	{
//		D_TCM_CPRIMUX_REGADR_LK1_DLRE_FLG,		/* RE 送信フレーム位相調整値設定有効フラグ */
//		D_TCM_CPRIMUX_REGADR_LK2_DLRE_FLG,		/* RE 送信フレーム位相調整値設定有効フラグ */
//		D_TCM_CPRIMUX_REGADR_LK3_DLRE_FLG,		/* RE 送信フレーム位相調整値設定有効フラグ */
//		D_TCM_CPRIMUX_REGADR_LK1_DLRE_FLG,		/* RE 送信フレーム位相調整値設定有効フラグ */
//		D_TCM_CPRIMUX_REGADR_LK2_DLRE_FLG,		/* RE 送信フレーム位相調整値設定有効フラグ */
//		D_TCM_CPRIMUX_REGADR_LK3_DLRE_FLG		/* RE 送信フレーム位相調整値設定有効フラグ */
//	}
//};
//#endif
//
//#if defined(OPT_CPRI_L2HARD)
///* CPRIMUX init setting(syspara setting register address) */
//UINT const f_trp_com_syspara_setreg[D_TCM_SYSPARATBL_NUM] = 
//{
//    D_CMM_FPGA_ADDR_CPRI_STB_TIME	,		/* CPRI安定化時間パラメータ(RE)  */
//    D_CMM_FPGA_ADDR_CPRI_1_RERESET_TIMER,	/* CPRI#1 RE 強制リセット確認タイマ   */
//    D_CMM_FPGA_ADDR_CPRI_1_RERESET_RETRY,	/* CPRI#1 RE 強制リセット再送回数   */
//    D_CMM_FPGA_ADDR_CPRI_2_RERESET_TIMER,	/* CPRI#2 RE 強制リセット確認タイマ   */
//    D_CMM_FPGA_ADDR_CPRI_2_RERESET_RETRY	/* CPRI#2 RE 強制リセット再送回数   */
//};
//
///************************************/
///*	DMA関連情報格納テーブル			*/
///************************************/
///*	L3データ受信用マスクOFF設定データ	*/
//CMT_DMADATINFO	const cmr_dmaRcvInfo[CMD_DMAKIND_RCV_NUM][CMD_LINK_NUM_T] =
//
//{{	/*	RXS LTEデータ用	*/
//	{	/*	CPRI#1	*/
//		D_CMM_FPGA_DAT_DMA_INT_LTE_DMAC_3,					/*	割込みbit(0x00400000)		*/
//		CMD_DMA3_SHIFTNUM,									/*	ビットシフト数(22)			*/
//		D_CMM_FPGA_ADDR_DMAC3_DSCADD						/*	制御アドレス(0x00103300)	*/
//	},
//	{	/*	CPRI#2	*/
//		D_CMM_FPGA_DAT_DMA_INT_LTE_DMAC_4,					/*	割込みbit(0x00200000)		*/
//		CMD_DMA4_SHIFTNUM,									/*	ビットシフト数(21)			*/
//		D_CMM_FPGA_ADDR_DMAC4_DSCADD						/*	制御アドレス(0x00103400)	*/
//	},
//	{	/*	CPRI#3	*/
//		CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//		CMD_NUM0,											/*	ビットシフト数(0)			*/
//		CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//	},
//	{	/*	CPRI#4	*/
//		CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//		CMD_NUM0,											/*	ビットシフト数(0)			*/
//		CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//	},
//	{	/*	CPRI#5	*/
//		CMD_NUM0,											/*	ビットシフト数(0)			*/
//		CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//		CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//	},
//	{	/*	CPRI#6	*/
//		CMD_NUM0,											/*	ビットシフト数(0)			*/
//		CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//		CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//	}
//},
//{	/*	MKM SENDデータ用	*/
//	{	/*	CPRI#1	*/
//		D_CMM_FPGA_DAT_DMA_INT_LTE_DMAC_5,					/*	割込みbit(0x00100000)		*/
//		CMD_DMA5_SHIFTNUM,									/*	ビットシフト数(20)			*/
//		D_CMM_FPGA_ADDR_DMAC5_DSCADD						/*	制御アドレス(0x00103500)	*/
//	},
//	{	/*	CPRI#2	*/
//		D_CMM_FPGA_DAT_DMA_INT_LTE_DMAC_6,					/*	割込みbit(0x00080000)		*/
//		CMD_DMA6_SHIFTNUM,									/*	ビットシフト数(19)			*/
//		D_CMM_FPGA_ADDR_DMAC6_DSCADD						/*	制御アドレス(0x00103600)	*/
//	},
//	{	/*	CPRI#3	*/
//		CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//		CMD_NUM0,											/*	ビットシフト数(0)			*/
//		CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//	},
//	{	/*	CPRI#4	*/
//		CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//		CMD_NUM0,											/*	ビットシフト数(0)			*/
//		CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//	},
//	{	/*	CPRI#5	*/
//		CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//		CMD_NUM0,											/*	ビットシフト数(0)			*/
//		CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//	},
//	{	/*	CPRI#6	*/
//		CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//		CMD_NUM0,											/*	ビットシフト数(0)			*/
//		CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//	}
//},
//{	/*	MKM RECVデータ用	*/
//	{	/*	CPRI#1	*/
//		D_CMM_FPGA_DAT_DMA_INT_LTE_DMAC_7,					/*	割込みbit(0x00040000)		*/
//		CMD_DMA7_SHIFTNUM,									/*	ビットシフト数(18)			*/
//		D_CMM_FPGA_ADDR_DMAC7_DSCADD						/*	制御アドレス(0x00103700)	*/
//	},
//	{	/*	CPRI#2	*/
//		D_CMM_FPGA_DAT_DMA_INT_LTE_DMAC_8,					/*	割込みbit(0x00020000)		*/
//		CMD_DMA8_SHIFTNUM,									/*	ビットシフト数(17)			*/
//		D_CMM_FPGA_ADDR_DMAC8_DSCADD						/*	制御アドレス(0x00103800)	*/
//	},
//	{	/*	CPRI#3	*/
//		CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//		CMD_NUM0,											/*	ビットシフト数(0)			*/
//		CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//	},
//	{	/*	CPRI#4	*/
//		CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//		CMD_NUM0,											/*	ビットシフト数(0)			*/
//		CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//	},
//	{	/*	CPRI#5	*/
//		CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//		CMD_NUM0,											/*	ビットシフト数(0)			*/
//		CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//	},
//	{	/*	CPRI#6	*/
//		CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//		CMD_NUM0,											/*	ビットシフト数(0)			*/
//		CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//	}
//}};
//
//
///*	L3データ送信用マスクOFF設定データ	*/
//CMT_DMADATINFO	const cmr_dmaSndInfo[CMD_LINK_NUM_T]=
//
//{{	/*	CPRI#1	*/
//	D_CMM_FPGA_DAT_DMA_INT_LTE_DMAC_1,					/*	割込みbit(0x01000000)		*/
//	CMD_DMA1_SHIFTNUM,									/*	ビットシフト数(24)			*/
//	D_CMM_FPGA_ADDR_DMAC1_DSCADD						/*	制御アドレス(0x00103100)	*/
//},
//{	/*	CPRI#2	*/
//	D_CMM_FPGA_DAT_DMA_INT_LTE_DMAC_2,					/*	割込みbit(0x00800000)		*/
//	CMD_DMA2_SHIFTNUM,									/*	ビットシフト数(23)			*/
//	D_CMM_FPGA_ADDR_DMAC2_DSCADD						/*	制御アドレス(0x00103200)	*/
//},
//{	/*	CPRI#3	*/
//	CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//	CMD_NUM0,											/*	ビットシフト数(0)			*/
//	CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//},
//{	/*	CPRI#4	*/
//	CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//	CMD_NUM0,											/*	ビットシフト数(0)			*/
//	CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//},
//{	/*	CPRI#5	*/
//	CMD_NUM0,											/*	ビットシフト数(0)			*/
//	CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//	CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//},
//{	/*	CPRI#6	*/
//	CMD_NUM0,											/*	ビットシフト数(0)			*/
//	CMD_NUM0,											/*	割込みbit(0x00000000)		*/
//	CMD_NUM0											/*	制御アドレス(0x00000000)	*/
//}};
//#endif


/* @} */  /* group TRIF_TCM */
