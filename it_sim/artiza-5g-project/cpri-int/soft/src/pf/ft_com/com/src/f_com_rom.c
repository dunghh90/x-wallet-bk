/*******************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_cm_ram.c
 *  @brief  RAM Table for common function
 *  @date   2008/07/18 FFCS)Tangj create
 *  @date   2009/03/05  FFCS)Wuh modify for M-S3G-eNBPF-01471
 *					    stop writing FLASH after receiving CPRI message
 *  @date   2009/04/16  FFCS)Wuh modify for S3G PRC CR-xxx-xxx (CPRI spec V1.07)
 *  @date   2009/06/09  FFCS)Wuh modify for M-S3G-eNBPF-02047
 *                      RE version response incorrect
 *	@date	2009/06/10  FFCS)Wangjuan modify for M-S3G-eNBPF-02050
 *						BRE card can not connect in SV-MT/FLD-MT debug test
 *  @date   2009/06/16  FFCS)Wuh modify for M-S3G-eNBPF-02086,
 *                      modify for FLASH erase in file no new case faild
 *  @date   2009/08/10  FFCS)Niemsh modify for CR-00054-005,
 *                      add common variable:cmw_ftpulstpflg,cmw_ftpulmaxmsgq
 *  @date   2009/12/27 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
 *  @date   2013/12/10 ALPHA) 加藤 ZYNQ対応
 *  @date   2014/12/20 ALPHA) Fujiiy modify for ppcEOL
 *  @date   2015/06/19 ALPHA) tomioka modify for TDD Zynq
 *  @date   2015/06/23 ALPHA) ueda modify for TDD Zynq
 *  @date   2016/03/14 ALPHA) fujiiy modify for M-RRU-ZSYS-02088
 *  @date   2018/02/14 FJT)吉田 add 5G-DU対応
 *  @date   2018/02/23 FJT)吉田 modify
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008 - 2018
 */
/*******************************************************************************/


/** @addtogroup RRH_PF_COM
 *  @{
 */

/******************************************************************************/
/* include file
 */
/******************************************************************************/
#include	"f_com_inc.h"
#include	"bpf_i_ru_ipcm_shm.h"

/* 2021/01/19 M&C) Merge 4G FHM src (start add) */
/** hard alarm index table */
const	T_COM_HARDIDXTBL		f_comr_hardalm_idx_tbl[] =
{
	/* Alarm Code				SV Control type			SV register address		SV register mask address		SV bit				alarm text*/
	// {D_RRH_ALMCD_PLLNG,		E_SYS_SVCTL_PLL_ALM,		D_RRH_REG_CNT_SV2INF,	(UINT)&f_comw_hardalm_mask,	D_RRH_ALMBIT_PLLUNLOCK,	"PLL UNLOCK ALM"	},
	// {D_RRH_ALMCD_FPGANG1,	E_SYS_SVCTL_CPRI_ALM,	    D_RRH_REG_CNT_SV2INF,	(UINT)&f_comw_hardalm_mask,	D_RRH_ALMBIT_FPGAUNLK1,	"FPGA-PLLUNLOCK1"	},
	// {D_RRH_ALMCD_FPGAIF1,	E_SYS_SVCTL_COM_ALM,		D_RRH_REG_CNT_SV2INF,	(UINT)&f_comw_hardalm_mask,	D_RRH_ALMBIT_FPGAIF1,	"FPGA IF ALM_1"		},
	// {D_RRH_ALMCD_FPGAIF2,	E_SYS_SVCTL_COM_ALM,		D_RRH_REG_CNT_SV2INF,	(UINT)&f_comw_hardalm_mask,	D_RRH_ALMBIT_FPGAIF2,	"FPGA IF ALM_2"		},
	// {D_RRH_ALMCD_CPLDIF1,	E_SYS_SVCTL_COM_ALM,		D_RRH_REG_CNT_SV2INF,	(UINT)&f_comw_hardalm_mask,	D_RRH_ALMBIT_CPLDIF1,	"CPLD IF ALM_1"		},
	// {D_RRH_ALMCD_TEMP,		E_SYS_SVCTL_COM_ALM,		D_RRH_REG_CNT_SV2INF,	(UINT)&f_comw_hardalm_mask,	D_RRH_ALMBIT_TEMP,		"TEMP ALM"			},
	// {D_RRH_ALMCD_PSALM,		E_SYS_SVCTL_COM_ALM,		D_RRH_REG_CNT_SV2INF,	(UINT)&f_comw_hardalm_mask,	D_RRH_ALMBIT_PSALM,		"PS ALM"			},
};

/* @} */

/** @name system parameter Typical info management table	 */
/* @{ */
/**
 * @brief  システムパラメータTypical値
 * @date   2016/03/25 TDI)satou M-RRU-ZSYS-02090 メーカ提出資料のシステムパラメータ暫定値の内容に合わせて修正
 */
const	USHORT		f_comr_syspara_typval_tbl[D_SYS_SYSPARA_MAX] =
{
    D_SYS_SYS_CSTATE,                       /**< CPRI State安定化時間(RE) */
    D_SYS_SYS_HCHECK,                       /**< ヘルスチェック要求確認タイマ(RE) */
    D_SYS_SYS_HREQUEST,                     /**< ヘルスチェック要求未確認保護段数(RE) */
    D_SYS_SYS_SYSSEND,                      /**< システムパラメータ送信確認タイマ */
    D_SYS_SYS_FIRMSEND,                     /**< ファームウェア送信確認タイマ */
    D_SYS_SYS_SECCHIP0,                     /**< セクタnチップ補正(0系) */
    D_SYS_SYS_SECCHIP1,                     /**< セクタnチップ補正(1系) */
    D_SYS_SYS_EXTDATA,                      /**< 外部装置データ受信確認タイマ */
    D_SYS_SYS_FLD_MT_CON_TIM,               /**< FLD-MT接続確認タイマ */
    D_SYS_SYS_FLD_MT_REL_TIM,               /**< FLD-MT接続解放確認タイマ */
    D_SYS_SYS_FLD_MT_DATA_C_RCV_TIM,        /**< FLD-MTデータコマンド受信確認タイマ */
    D_SYS_SYS_FLD_MT_DATA_F_RCV_TIM,        /**< FLD-MTデータファイル受信確認タイマ */
    D_SYS_SYS_SV_MT_CON_TIM,                /**< SV-MT接続確認タイマ */
    D_SYS_SYS_SV_MT_REL_TIM,                /**< SV-MT接続解放確認タイマ */
    D_SYS_SYS_SV_MT_DATA_RCV_TIM,           /**< SV-MTデータ受信確認タイマ */
    D_SYS_SYS_TX_DIV_VALUE_3G,              /**< 3G用送信ダイバーシチ設定 */
    D_SYS_SYS_PRISYSINFO,                   /**< primary system info */
    D_SYS_SYS_S3G_BANDWIDTH,                /**< S3G bandwidth */
    D_SYS_SYS_3G_CAR_WORKFLG,               /**< 3G carrier#1~4 working flag */
    D_SYS_SYS_TX_SYS_NUM,                   /**< TX system number */
    D_SYS_SYS_DEFAULT,                      /**< reserved area for parameterE */
    D_SYS_SYS_DEFAULT,                      /**< reserved area for parameterE */
	D_SYS_SYS_RESERVE,                      /**< reserved area for parameterE */
	D_SYS_SYS_RESERVE,                      /**< reserved area for parameterE */
    D_SYS_SYS_3G_CAR1_WORKFRQ,              /**< 3G carrier#1 working frequency */
    D_SYS_SYS_3G_CAR2_WORKFRQ,              /**< 3G carrier#2 working frequency */
    D_SYS_SYS_3G_CAR3_WORKFRQ,              /**< 3G carrier#3 working frequency */
    D_SYS_SYS_3G_CAR4_WORKFRQ,              /**< 3G carrier#4 working frequency */
    D_SYS_SYS_3G_MT_EXTD_FLG,               /**< 3G MT signaling/External equipment data corresponse flag (TBD)  */
    D_SYS_SYS_SYSPARA_WAIT_TIM,             /**< システムパラメータ転送待ちタイマ */
    D_SYS_SYS_FIRM_SND_TIM,                 /**< ファームウェア送信確認タイマ(更新用) */
	D_SYS_SYS_RESERVE                       /**< reserved area for parameterF */
};


/* Flashログ退避情報 */
const T_COM_LOG_SAVE_TBL		f_comr_flash_log_save_info_fhm[15] =
{
	/* (01)Reset reason ログ					*/
	{
		E_RRH_SHMID_APL_RST_REASON_LOG,										/* shared memory index	*/
		sizeof(T_RRH_LOG_RST_REASON)										/* log size				*/
	},
	/* (02)Assetログ							*/
	{
		E_RRH_SHMID_APL_ASSERT_LOG,											/* shared memory index	*/
		sizeof(T_RRH_LOG_ASSERT)											/* log size				*/
	},
	/* (03)Register Accessログ					*/
	{
		E_RRH_SHMID_APL_REG_ACCESS_LOG,										/* shared memory index	*/
		sizeof(T_RRH_LOG_REG_ACCESS)										/* log size				*/
	},
	/* (04)Alarm Historyログ					*/
	{
		E_RRH_SHMID_APL_ALM_HISTORY_LOG,									/* shared memory index	*/
		sizeof(T_RRH_ALARM_HISTORYMNG)										/* log size				*/
	},
	/* (05)CPRI L1復旧断履歴ログ				*/
	{
		E_RRH_SHMID_APL_CPRI_L1_LOSLOF_HIS,									/* shared memory index	*/
		sizeof(T_RRH_L1LOSLOF_TBL) * D_RRH_CPRINO_NUM						/* log size				*/
	},
	/* (06)CPRI L1リンク障害ログ				*/
	{
		E_RRH_SHMID_APL_CPRI_L1_LINK_LOG,									/* shared memory index	*/
		sizeof(T_RRH_L1ERR_TBL) * D_RRH_CPRINO_NUM							/* log size				*/
	},
	/* (07)CPRI L1統計情報ログ					*/
	{
		E_RRH_SHMID_APL_CPRI_L1_STAT_INFO,									/* shared memory index	*/
		sizeof(T_RRH_L1STAT_TBL) * D_RRH_CPRINO_NUM							/* log size				*/
	},
	/* (08)CPRI L2統計情報ログ					*/
	{
		E_RRH_SHMID_APL_CPRI_L2_STAT_INFO,									/* shared memory index	*/
		sizeof(T_RRH_L2_STATISTICS_L2TABLE) * D_RRH_CPRINO_NUM				/* log size				*/
	},
	/* (09)CPRI L3統計情報ログ					*/
	{
		E_RRH_SHMID_APL_CPRI_L3_STAT_INFO,									/* shared memory index	*/
		sizeof(T_RRH_L2_STATISTICS_L3TABLE) * D_RRH_L2_STATI_L3SIG_SYSTEM_TYPE * D_RRH_CPRINO_NUM				/* log size				*/
	},
	/* (10)CPRI L3復旧断履歴					*/
	{
		E_RRH_SHMID_APL_CPRI_L3_LOSLOF_HIS,									/* shared memory index	*/
		/* ★TODO ログ 提供待ち */
		0																	/* log size				*/
	},
	/* (11)CPRI LAPB/L3送受信Log				*/
	{
		E_RRH_SHMID_APL_LAPB_L3_SR_LOG,										/* shared memory index	*/
		sizeof(T_RRH_L2_LOG_INFO_ALL)										/* log size				*/
	},
	/* (12)SV状態変化履歴ログ					*/
	{
		E_RRH_SHMID_APL_SV_STATE_HISTORY,									/* shared memory index	*/
		sizeof(T_RRH_SVSTATECHG_HIS)										/* log size				*/
	},
	/* (13)L3送受信ログ 						*/
	{
		E_RRH_SHMID_APL_CPRI_L3_SR_LOG,										/* shared memory index	*/
		sizeof(T_RRH_L3_SR_LOG)												/* log size				*/
	},
	/* (14)SFPログ 								*/
	{
		E_RRH_SHMID_APL_SFPLOG,												/* shared memory index	*/
		sizeof(T_RRH_SFP_LOG_ALL)											/* log size				*/
	},
	/* (15)Assetログ							*/
	{
		E_RRH_SHMID_APL_ASSERT_DWL,											/* shared memory index	*/
		sizeof(T_RRH_LOG_ASSERT_DWL)										/* log size				*/
	},
};
/* 2021/01/19 M&C) Merge 4G FHM src (end add) */

const T_COM_LOG_SAVE_TBL		f_comr_flash_log_save_info[D_RRH_FLASH_LOG_MAX] =
{
	/* Reset reason ログ					*/
	{
		E_RRH_SHMID_APL_RST_REASON_LOG,										/* shared memory index	*/
		sizeof(T_RRH_LOG_RST_REASON)										/* log size				*/
	},
	/* Assetログ							*/
	{
		E_RRH_SHMID_APL_ASSERT_LOG,											/* shared memory index	*/
		sizeof(T_RRH_LOG_ASSERT)											/* log size				*/
	},
	/* Alarm Historyログ					*/
	{
		E_RRH_SHMID_APL_ALM_HISTORY_LOG,									/* shared memory index	*/
		sizeof(T_RRH_ALARM_HISTORYMNG)										/* log size				*/
	},
	/* SV状態変化履歴ログ					*/
	{
		E_RRH_SHMID_APL_SV_STATE_HISTORY,									/* shared memory index	*/
		sizeof(T_RRH_SVSTATECHG_HIS)										/* log size				*/
	},
	/* SFP#0管理情報履歴 					*/
	{
		E_RRH_SHMID_APL_SFP0_MNG_INFO_HIS,									/* shared memory index */
		sizeof(T_RRH_SFPINFO_HIS)											/* log size */
	},
	/* Ethernet リンク障害履歴ログ(ETH#1)					*/
	{
		E_RRH_SHMID_APL_MAC_STAT_ETH0,									/* shared memory index */
		sizeof(T_RRH_LOG_ETH_ERR_HIS)											/* log size */
	},
	/* PTP APR PARM					*/
	{
		E_RRH_SHMID_APL_PTP_TIME_ADJ,									/* shared memory index */
		sizeof(T_RRH_PTP_APR_PARM)											/* log size */
	},
	/* 外部IO制御履歴ログ					*//*	5G-商用DU	*/
	{
		E_RRH_SHMID_EIO_SVCTRL_LOG_TBL,										/* shared memory index */
		sizeof(T_RRH_DU_EIO_SVCTRL_LOG)										/* log size */
	},
	/* FH L1履歴ログ					*/
	{
		E_RRH_SHMID_APL_FH_L1_HIS,											/* shared memory index */
		sizeof(T_RRH_LOG_FH_L1_HIS)											/* log size */
	},
	/* DLF統計情報ログ					*/
	{
		E_RRH_SHMID_APL_DLF_LOG,											/* shared memory index */
		sizeof(T_RRH_LOG_DLF_HIS)											/* log size */
	}
};

/* proc create manage table */
const	T_RRH_SYS_CREATE_SHMMEM		f_comr_shmmemCreMng[E_RRH_SHMID_APL_MAX] = {
	/*==========================================*/
	/* 個数,			SIZE,					*/
	/*==========================================*/
/* BPFでの共有メモリ管理用メモリ(絶対に触らないこと)	*/
     {  1,              0x00001000                                              }   /*!< BPFでの共有メモリ管理用            */
    ,{  1,              0x00200000                                              }   /*!< BPFでの通信用共有メモリ管理テーブル*/
    ,{  1,              0x00001000                                              }   /*!< ログ取得レベル管理テーブル         */
    ,{  1,              0x00020000                                              }   /*!< タイマ管理テーブル                 */
    ,{  1,              0x00001000                                              }   /*!< プロセスID管理テーブル             */
    ,{  1,              0x00001000                                              }   /*!< スレッドID管理テーブル             */
/* 以降はユーザーが使いたい共有メモリを定義すること										*/
/* 追加する際は下に追加していくこと														*/
/* 同時にf_rrh_def.hにある[共有メモリアクセス定義]にも修正を行うこと					*/
    ,{  1,              sizeof(T_RRH_LOG_RST_REASON)                            }   /*!< reset reason log                   */
    ,{  1,              sizeof(T_RRH_LOG_ASSERT)                                }   /*!< Assert Log                         */
    ,{  1,              sizeof(T_RRH_LOG_REG_ACCESS)                            }   /*!< Register Access Log                */
    ,{  1,              sizeof(T_RRH_ALARM_HISTORYMNG)                          }   /*!< Alarm Hisotry Log                  */
    ,{  1,              sizeof(T_RRH_L1LOSLOF_TBL)          * D_RRH_CPRINO_NUM  }   /*!< CPRI L1復旧断履歴                  */
    ,{  1,              sizeof(T_RRH_L1ERR_TBL)             * D_RRH_CPRINO_NUM  }   /*!< CPRI L1リンク障害ログ              */
    ,{  1,              sizeof(T_RRH_L1STAT_TBL)            * D_RRH_CPRINO_NUM  }   /*!< CPRI L1統計情報                    */
    ,{  1,              sizeof(T_RRH_L2_STATISTICS_L2TABLE) * D_RRH_CPRINO_NUM  }   /*!< CPRI L2統計情報                    */
    ,{  1,              sizeof(T_RRH_L2_STATISTICS_L3TABLE) * D_RRH_L2_STATI_L3SIG_SYSTEM_TYPE * D_RRH_CPRINO_NUM  }   /*!< CPRI L3統計情報                    */
    ,{  1,              0x00010000                                              }   /*!< CPRI L3復旧断履歴                  */  /* 構造体未定義   */
    ,{  1,              sizeof(T_RRH_L2_LOG_INFO_ALL)                           }   /*!< CPRI LAPB/L3送受信ログ             */
    ,{  1,              sizeof(T_RRH_SVSTATECHG_HIS)                            }   /*!< SV状態変化履歴                     */
    ,{  1,              sizeof(T_RRH_LOG_BUFF_HIS)                              }   /*!< メッセージバッファ獲得履歴         */
    ,{  1,              sizeof(T_RRH_LOG_TIMER_HIS)                             }   /*!< タイマ履歴                         */
    ,{  1,              sizeof(T_RRH_LOG_SEM_HIS)                               }   /*!< セマフォ履歴                       */
    ,{  1,              0x00080000                                              }   /*!< dpda alarm log                     */  /* T_DPDA_ALM_LOG_DEBUGのsize以上であること */
    ,{  1,              0x00002000                                              }   /*!< dpda情報管理テーブル               */
    ,{  1,              sizeof(T_RRH_TRAINVTBL)                                 }   /*!< インベントリ情報                   */
    ,{  1,              0x00001000                                              }   /*!< shelft インベントリ情報            */  /* 構造体未定義   */
    ,{  1,              0x00001000                                              }   /*!< システムパラメータ管理テーブル     */  /* 構造体未定義   */
    ,{  1,              sizeof(T_RRH_REDEVCOMPTBL)                              }   /*!< デバイス管理テーブル               */
    ,{  1,              0x00000200                                              }   /*!< ソフトウェアバージョン             */  /* 構造体未定義   */
    ,{  1,              sizeof(T_RRH_ALM_LOG_FOR_HIST)                          }   /*!< Alarm History log収集準備Table     */
    ,{  1,              sizeof(T_RRH_LOG_TEMP_SLC)                              }   /*!< Temprature Log Table               */  /* T_RRH_LOG_TEMP と T_RRH_LOG_TEMP_SLC の大きいほうで定義する */
    ,{  1,              sizeof(UINT)                                            }   /*!< RRH装置情報                        */
    ,{  1,              sizeof(T_RRH_LOG_SPI_ACCESS)                            }   /*!< SPI Access Log                     */
    ,{  1,              sizeof(UINT)                                            }   /*!< RE種別                             */
    ,{  1,              sizeof(T_RRH_L3_SR_LOG)                                 }   /*!< L3送受信ログ                       */
    ,{  1,              sizeof(T_RRH_ALM_ABC) * CMD_MAX_SYSNUM                  }   /*!< ALM_ABC                            */
    ,{  1,              sizeof(T_RRH_CPRI_STATE)                                }   /*!< CPRI状態                           */
    ,{  1,              sizeof(T_RRH_FHM_STATE)                                 }   /*!< FHM状態                            */
    ,{  1,              sizeof(T_RRH_LAYER3) * CMD_MAX_SYSNUM                   }   /*!< Layer3                             */
    ,{  1,              sizeof(T_RRH_MTADDSET)  * CMD_MT_MAX * CMD_MAX_SYSNUM   }   /*!< MTアドレス設定                     */  /* [0]=3GFLD-MT[1]=3GSV-MT[2]=LTE/FLD-MT[3]=LTE/SV_SV-MT*/
    ,{  1,              sizeof(T_RRH_CARDST_S3G) * D_RRH_CPRINO_RE_MAX          }   /*!< (LTE)REカード状態                  */
    ,{  1,              sizeof(T_RRH_CARDST_3G) * D_RRH_CPRINO_RE_MAX           }   /*!< (3G)REカード状態                   */
    ,{  1,              sizeof(T_RRH_SLOT_S3G_RE) * D_RRH_CPRINO_RE_MAX         }   /*!< (LTE)スロット情報                  */
    ,{  1,              sizeof(T_RRH_SLOT_3G_RE) * D_RRH_CPRINO_RE_MAX          }   /*!< (3G)スロット情報                   */
    ,{  1,              sizeof(T_RRH_SIDE_USE)                                  }   /*!< FHM保持FWファイル運用面            */
    ,{  1,              sizeof(T_RRH_FW_INFO) * D_RRH_FLASH_WRT_MAX             }   /*!< FHM保持FWファイル情報(0.1面)       */
    ,{  1,              sizeof(T_RRH_FW_INFO) * D_RRH_CPRINO_RE_MAX             }   /*!< FHM保持FWファイル情報 (RE-FW)      */
    ,{  1,              sizeof(T_RRH_SYSPAR_LTE)                                }   /*!< シスパラ格納領域(LTE)              */
    ,{  1,              sizeof(T_RRH_SYSPAR_3G)                                 }   /*!< シスパラ格納領域(3G)               */
    ,{  1,              sizeof(T_RRH_REST_S3G) * D_RRH_CPRINO_RE_MAX            }   /*!< (LTE)RE状態                        */
    ,{  1,              sizeof(T_RRH_REST_3G) * D_RRH_CPRINO_RE_MAX             }   /*!< (3G)RE状態                         */
    ,{  1,              sizeof(T_RRH_TOFFSET_S3G) * D_RRH_CPRINO_RE_MAX         }   /*!< (LTE)Toffset値報告                 */
    ,{  1,              sizeof(T_RRH_TOFFSET_3G) * D_RRH_CPRINO_RE_MAX          }   /*!< (3G)Toffset値報告                  */
    ,{  1,              sizeof(T_TRXSET_PARA_LTE) * CMD_MAX_CARNUM_S3G          }   /*!< (LTE)TRX設定パラメータテーブル     */
    ,{  1,              sizeof(T_TRXSET_PARA_3G) * CMD_MAX_CARNUM_3G            }   /*!< (3G)TRX設定パラメータテーブル      */
    ,{  1,              sizeof(T_RRH_CARST_S3G) * D_RRH_CPRINO_RE_MAX           }   /*!< (LTE)キャリア状態                  */
    ,{  1,              sizeof(T_RRH_CARST_3G) * D_RRH_CPRINO_RE_MAX            }   /*!< (3G)キャリア状態                   */
    ,{  1,              sizeof(T_RRH_EQP_S3G) * D_RRH_CPRINO_RE_MAX             }   /*!< (LTE)装置構成情報                  */
    ,{  1,              sizeof(T_RRH_EQP_3G) * D_RRH_CPRINO_RE_MAX              }   /*!< (3G)装置構成情報                   */
    ,{  1,              sizeof(T_RRH_EQP_S3G)                                   }   /*!< (LTE)装置構成情報(起動時集約）     */
    ,{  1,              sizeof(T_RRH_EQP_3G)                                    }   /*!< (3G)装置構成情報(起動時集約）      */
    ,{  1,              sizeof(T_RRH_RE_DELAY_INF) * D_RRH_CPRINO_RE_MAX        }   /*!< RE単位遅延補正情報                 */
    ,{  1,              sizeof(T_RRH_DELAY_INF)                                 }   /*!< 遅延補正情報                       */
    ,{  1,              sizeof(T_RRH_TX_ONOFF)                                  }   /*!< 無線送信ONOFF状態                  */
    ,{  1,              sizeof(T_RRH_CONN_DISC) * CMD_MAX_SYSNUM                }   /*!< RE接続有無状態                     */
#ifdef OPT_RRH_ZYNQ_REC
    ,{  1,              sizeof(T_RRH_RETURN_MESSAGE)*D_RRH_CPRINO_RE_MAX        }   /*!< 擬似RE用                     */
#endif
    ,{  1,              sizeof(T_RRH_SFP_LOG_ALL)                               }   /*!< SFP履歴ログ                        */
    ,{  1,              sizeof(T_RRH_SLOT_S3G)                                  }   /*!< （LTE)スロット情報(起動時集約) */
    ,{  1,              sizeof(T_RRH_SLOT_3G)                                   }   /*!< (3G)スロット情報(起動時集約) */
    ,{  1,              sizeof(T_RRH_REPORTSTA_S3G)                             }   /*!< (LTE)RE PORT状態報告 */
/* 以降、16Bで追加 */
    ,{  1,              sizeof(T_RRH_CARDST_S3G) * D_RRH_CPRINO_RE_MAX * CMD_MAX_CARDNUM }   /*!< (LTE)REカード状態(ポート集約)      */
    ,{  1,              sizeof(T_RRH_CARDST_3G) * D_RRH_CPRINO_RE_MAX * CMD_MAX_CARDNUM  }   /*!< (3G)REカード状態(ポート集約)       */
    ,{  1,              sizeof(UCHAR) * 16                                      }   /*!< デバッグ用printf文出力Off(=0)/On(=1) */
    ,{  1,              sizeof(T_RRH_LOG_ASSERT_DWL)                            },   /*!< Assert Log for DWL				    */
	/*==========================================*/
	/* 個数,			SIZE,					*/
	/*==========================================*/
	/***** BPFでの共有メモリ管理用メモリ(絶対に触らないこと！)	*****/
	{	1,				sizeof(struct ipcm_shm_create_data_t)	},	/*!< 00 : BPFでの共有メモリ管理用			*/
	{	1,				sizeof(BPF_MNG_COM_MEMORY) * E_BPF_RU_IPCM_SHMKIND_MAX	},	/*!< 01 : BPFでの通信用共有メモリ管理テーブル*/
	{	1,				sizeof(T_RRH_LOG_LEVEL_MNG)				},	/*!< 02 : ログ取得レベル管理テーブル		*/
	{	1,				0x00001000								},	/*!< 03 : タイマ管理テーブル				*/
	{	1,				sizeof(T_RRH_PID_MNG)					},	/*!< 04 : プロセスID管理テーブル			*/
	{	1,				sizeof(T_RRH_TID_MNG)					},	/*!< 05 : スレッドID管理テーブル			*/
	/***** 以降はユーザーが使いたい共有メモリを定義すること						*****/
	/***** 追加する際は下に追加していくこと										*****/
	/***** 同時にf_rrh_def.hにある[共有メモリアクセス定義]にも修正を行うこと!	*****/
	{	1,				sizeof(T_RRH_LOG_RST_REASON)			},	/*!< 06 : reset reason log					*/
	{	1,				sizeof(T_RRH_LOG_ASSERT)				},	/*!< 07 : Assert Log						*/
	{	1,				sizeof(T_RRH_ALARM_HISTORYMNG)			},	/*!< 08 : Alarm Hisotry Log					*/
	{	1,				sizeof(T_RRH_LOG_BUFF_HIS)				},	/*!< 09 : メッセージバッファ獲得履歴		*/
	{	1,				sizeof(T_RRH_LOG_TIMER_HIS)				},	/*!< 10 : タイマ履歴						*/
	{	1,				sizeof(T_RRH_LOG_SEM_HIS)				},	/*!< 11 : セマフォ履歴						*/
	{	1,				0x00150000								},	/*!< 12 : dpda alarm log					*/	/* T_DPDA_ALM_LOG_DEBUGのsize以上であること	*/
	{	1,				0x00002000								},	/*!< 13 : dpda情報管理テーブル				*/  /* T_DPDA_INFO_ALLのsize以上であること */
	{	1,				sizeof(T_RRH_TRAINVTBL)					},	/*!< 14 : インベントリ情報					*/
	{	1,				0x00000004								},	/*!< 15 : DU種別							*/
	{	1,				sizeof(T_RRH_ALM_LOG_FOR_HIST)			},	/*!< 16 : Alarm History log収集準備Table	*/
	{	1,				sizeof(T_RRH_LOG_TEMP)					},	/*!< 17 : Temprature Log Table				*/
	{	1,				sizeof(T_RRH_SFPINFO_HIS)				},	/*!< 18 : SFP#0管理情報履歴					*/
	{	1,				sizeof(T_RRH_SFPINFO_HIS)				},	/*!< 19 : SFP#1管理情報履歴					*/
	{	1,				sizeof(T_RRH_LOG_CAL_HIS)				},	/*!< 20 : CAL履歴							*/
	{	1,				0x00001000								},	/*!< 21 : Alarm通知ログ						*/
	{	1,				0x00001000								},	/*!< 22 : 装置状態ログ						*/
	{	1,				0x00001000								},	/*!< 23 : Active-Alarm-list					*/
	{	1,				0x00009000								},	/*!< 24 : キャリア状態ログ					*/ /* T_DPDA_LOG_CAR_STATE のsize以上であること*/
	{	1,				sizeof(T_RRH_PTPCONF_MNG)				},	/*!< 25 : PTP起動時パラメータ				*/
	{	1,				sizeof(T_RRH_LOG_PTP_STATUS)			},	/*!< 26 : PTP状態ログ						*/
	{	1,				sizeof(T_RRH_PTP_APR_PARM)				},	/*!< 27 : PTP時刻補正						*/
	{	1,				sizeof(T_RRH_LOG_UPSIGSTOP_HIS)			},	/*!< 28 : 主信号停波履歴					*/
	{	1,				sizeof(T_RRH_LOG_FH_L1_HIS)				},	/*!< 29 : FH L1履歴							*/
	{	1,				sizeof(T_RRH_SFNFRM_CALB_TBL)			},	/*!< 30 : SFN/FRM補正用テーブル				*/
	{	1,				sizeof(T_RRH_TXPOW_DETECT_TBL)			},	/*!< 31 : Tx power detector テーブル		*/
	{	1,				sizeof(T_RRH_TXPOW_DETECT_LOG)			},	/*!< 32 : Tx power detector LOG				*/
	{	1,				sizeof(T_RRH_LUT_LOG)					},	/*!< 33 : LUT LOG							*/
	{	1,				sizeof(T_RRH_LOG_FHSTAT_HIS)			},	/*!< 34 : FH統計情報						*/
	{	1,				sizeof(T_RRH_LOG_ETH_ERR_HIS)			},	/*!< 35 : Ethernet リンク障害履歴ログ(ETH#1)*/
	{	1,				sizeof(T_RRH_LOG_ETH_ERR_HIS)			},	/*!< 36 : Ethernet リンク障害履歴ログ(ETH#2)*/
	{	1,				0x00001000								},	/*!< 37 : ネットワーク統計(M-Plane)			*/
	{	1,				sizeof(T_RRH_SVSTATECHG_HIS)			},	/*!< 38 : SV状態変化履歴ログ				*/
	{	1,				sizeof(T_RRH_TRAINING_DATA)				},	/*!< 39 : TPコマンド仮想メモリデータ		*/
	{	1,				sizeof(T_RRH_DU_INFORMATION_TBL)		},	/*!< 40 : DUネットワーク情報テーブル		*/
	{	1,				0x00002000								},	/*!< 41 : RFIC Configデータ					*/
	{	1,				sizeof(T_RRH_DU_PROCESS_INFO)			},	/*!< 42 : DU information Table				*/
	{	1,				sizeof(T_RRH_RF_TRAINING_DATA)			},	/*!< 43 : RF個別トレーニングデータ			*/
	{	1,				sizeof(T_RRH_DU_CMD_INFO)				},	/*!< 44 : DUコマンド管理情報				*/
	{	1,				sizeof(T_RRH_DU_FAULT_MNG)				},	/*!< 45 : Fault管理情報						*/
	{	1,				sizeof(T_RRH_NETCONF_INFO_TBL)			},	/*!< 46 : NETCONFユーザー情報テーブル		*/
	{	1,				sizeof(T_RRH_CAL_COEF_TBL)				},	/*!< 47 : Tx CAL係数						*/
	{	1,				sizeof(T_RRH_CAL_COEF_TBL)				},	/*!< 48 : Rx CAL係数						*/
	{	1,				sizeof(T_RRH_PFM_PERFORMANCE_TBL)		},	/*!< 49 : Performanceログテーブル			*/
	{	1,				sizeof(T_RRH_PFM_PERFORMANCE_MNG_TBL)	},	/*!< 50 : Performance情報管理テーブル		*/
	{	1,				sizeof(T_RRH_LBM_MNGMENT_TBL)			},	/*!< 51 : LBM/LBR管理テーブル				*/
	{	1,				sizeof(T_RRH_DU_EIO_SVCTRL_MNG)			},	/*!< 52 : 外部IO情報テーブル				*/
	{	1,				sizeof(T_RRH_DU_EIO_SVCTRL_LOG)			},	/*!< 53 : 外部IO制御履歴テーブル			*/
	{	1,				sizeof(T_RRH_DU_CMD_INFO)				},	/*!< 54 : TPコマンド管理情報				*/
	{	1,				sizeof(T_RRH_MT_INFO)					},	/*!< 55 : MT管理							*/
	{	1,				sizeof(T_RRH_BF_MODE_TBL)				},	/*!< 56 : Beamforming Mode Flag				*/
	{	1,				sizeof(UINT)							},	/*!< 57 : SFP Fail Safe ON/OFF Flag			*/
	{	1,				sizeof(T_RRH_LOG_DLF_HIS)				},	/*!< 58 : DLF統計情報						*/
	{	1,				sizeof(T_RRH_AIH_ALD_LOG_TBL)			},	/*!< 59 : ALD(AISG) message history			*/
	{	1,				sizeof(T_RRH_AIH_ALD_CTRL_TBL)			}	/*!< 60 : ALD(AISG) Latest information		*/
};
/* 2021/01/19 M&C) Merge 4G FHM src (start add) */
/** @} */

/** @} */

/** 運用ログパス */
const	UCHAR	f_comr_runLogPath[ D_SYS_MAX_SYSNUM ][24] = {
	"/var/log/sendlog_3g",								/* 3G用											*/
	"/var/log/sendlog"									/* S3G用										*/
};

/* @} */

/**
 * @name WDT Handler
 * @{
 */

/**
 * watch dog timer handler flag. (WDT(IP Macro)割り込みフラグ)
 * - f_hdl_main_wdt()で BPF_IRQ_WDT の割り込みを受信時にセットします
 * - f_wdt_main()のメインループで参照し、フラグがセットされている場合は
 *   SWDTがクリアされるのを防止するためにスレッドを終了します
 */
UINT f_comw_wdtHndlFlg;
/* 2021/01/19 M&C) Merge 4G FHM src (end add) */

/*!
 * @brief SV Control MAP change Table
 * @{
 */

/*	Admin State:(00)E_DU_SVCTL_OPE_ADM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptAdmTbl[D_DU_SVCTL_ON_ADM_NUM] =
	{
		{ D_DU_SVCTL_ON_ADM_LOCKED         , D_DU_SVCTL_BIT_ADM_LOCKED      , 0,  0,  0                                              , D_RRH_OFF, "Admin:Locked                   " },
		{ D_DU_SVCTL_ON_ADM_DOWN           , D_DU_SVCTL_BIT_ADM_DOWN        , 0,  0,  0                                              , D_RRH_OFF, "Admin:shutting down            " },
		{ D_DU_SVCTL_ON_ADM_UNLOCKED       , D_DU_SVCTL_BIT_ADM_UNLOCKED    , 0,  0,  0                                              , D_RRH_OFF, "Admin:Unlocked                 " },
		{ D_DU_SVCTL_ON_ADM_LOCKED_LO      , D_DU_SVCTL_BIT_ADM_LOCKED_LO   , 0,  0,  0                                              , D_RRH_OFF, "Admin:Locked(local)            " },
		{ D_DU_SVCTL_ON_ADM_UNLOCKED_LO    , D_DU_SVCTL_BIT_ADM_UNLOCKED_LO , 0,  0,  0                                              , D_RRH_OFF, "Admin:Unlocked(local)          " }
	};

/*	Energysaving State:(01)E_DU_SVCTL_OPE_ENG	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptEngTbl[D_DU_SVCTL_ON_ENG_NUM] =
	{
		{ D_DU_SVCTL_ON_ENG_SLEEP          , D_DU_SVCTL_BIT_ENG_SLEEP       , 0,  0,  0                                              , D_RRH_OFF, "Power:sleep                    " },
		{ D_DU_SVCTL_ON_ENG_AWAKE          , D_DU_SVCTL_BIT_ENG_AWAKE       , 0,  0,  0                                              , D_RRH_OFF, "Power:awake                    " }
	};

/*	Operational State:(02)E_DU_SVCTL_OPE_OPE	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptOpeTbl[D_DU_SVCTL_ON_OPE_NUM] =
	{
		{ D_DU_SVCTL_ON_OPE_ENABLE         , D_DU_SVCTL_BIT_OPE_ENABLE      , 0,  0,  0                                              , D_RRH_OFF, "Oper :enable                   " },
		{ D_DU_SVCTL_ON_OPE_DISABLE        , D_DU_SVCTL_BIT_OPE_DISABLE     , 0,  0,  0                                              , D_RRH_OFF, "Oper :disable                  " }
	};

/*	availability State:(03)E_DU_SVCTL_OPE_AVL	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptAvlTbl[D_DU_SVCTL_ON_AVL_NUM] =
	{
		{ D_DU_SVCTL_ON_AVL_FAULTY         , D_DU_SVCTL_BIT_AVL_FAULTY      , 0,  0,  0                                              , D_RRH_OFF, "Avail:faulty                   " },
		{ D_DU_SVCTL_ON_AVL_DEGRADED       , D_DU_SVCTL_BIT_AVL_DEGRADED    , 0,  0,  0                                              , D_RRH_OFF, "Avail:degraded                 " },
		{ D_DU_SVCTL_ON_AVL_NORMAL         , D_DU_SVCTL_BIT_AVL_NORMAL      , 0,  0,  0                                              , D_RRH_OFF, "Avail:normal                   " }
	};

/*	usage State:(04)E_DU_SVCTL_OPE_USA	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptUsaTbl[D_DU_SVCTL_ON_USA_NUM] =
	{
		{ D_DU_SVCTL_ON_USA_BUSY           , D_DU_SVCTL_BIT_USA_BUSY        , 0,  0,  0                                              , D_RRH_OFF, "Usage :busy                    " },
		{ D_DU_SVCTL_ON_USA_ACTIVE         , D_DU_SVCTL_BIT_USA_ACTIVE      , 0,  0,  0                                              , D_RRH_OFF, "Usage :active                  " },
		{ D_DU_SVCTL_ON_USA_IDLE           , D_DU_SVCTL_BIT_USA_IDLE        , 0,  0,  0                                              , D_RRH_OFF, "Usage :idle                    " }
	};

/*	USE Ant#A(sub6):(05)E_DU_SVCTL_OPE_USE_A	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptUseAsTbl[D_DU_SVCTL_ON_USE_NUM_SB6] =
	{
		{ D_DU_SVCTL_ON_USE_C0             , D_DU_SVCTL_BIT_USE_A_C0        , 0,  0,  0                                              , D_RRH_OFF, "AntA car0:use                  " },
		{ D_DU_SVCTL_ON_SLP_C0             , D_DU_SVCTL_BIT_SLP_A_C0        , 0,  0,  0                                              , D_RRH_OFF, "AntA car0:slp                  " }
	};

/*	USE Ant#A(mmW):(05)E_DU_SVCTL_OPE_USE_A	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptUseAmTbl[D_DU_SVCTL_ON_USE_NUM_MMW] =
	{
		{ D_DU_SVCTL_ON_USE_C0             , D_DU_SVCTL_BIT_USE_A_C0        , 0,  0,  0                                              , D_RRH_OFF, "AntA car0:use                  " },
		{ D_DU_SVCTL_ON_USE_C1             , D_DU_SVCTL_BIT_USE_A_C1        , 0,  0,  0                                              , D_RRH_OFF, "AntA car1:use                  " },
		{ D_DU_SVCTL_ON_USE_C2             , D_DU_SVCTL_BIT_USE_A_C2        , 0,  0,  0                                              , D_RRH_OFF, "AntA car2:use                  " },
		{ D_DU_SVCTL_ON_USE_C3             , D_DU_SVCTL_BIT_USE_A_C3        , 0,  0,  0                                              , D_RRH_OFF, "AntA car3:use                  " },
		{ D_DU_SVCTL_ON_SLP_C0             , D_DU_SVCTL_BIT_SLP_A_C0        , 0,  0,  0                                              , D_RRH_OFF, "AntA car0:slp                  " },
		{ D_DU_SVCTL_ON_SLP_C1             , D_DU_SVCTL_BIT_SLP_A_C1        , 0,  0,  0                                              , D_RRH_OFF, "AntA car1:slp                  " },
		{ D_DU_SVCTL_ON_SLP_C2             , D_DU_SVCTL_BIT_SLP_A_C2        , 0,  0,  0                                              , D_RRH_OFF, "AntA car2:slp                  " },
		{ D_DU_SVCTL_ON_SLP_C3             , D_DU_SVCTL_BIT_SLP_A_C3        , 0,  0,  0                                              , D_RRH_OFF, "AntA car3:slp                  " }
	};

/*	USE Ant#B(sub6):(06)E_DU_SVCTL_OPE_USE_B	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptUseBsTbl[D_DU_SVCTL_ON_USE_NUM_SB6] =
	{
		{ D_DU_SVCTL_ON_USE_C0             , D_DU_SVCTL_BIT_USE_B_C0        , 0,  0,  0                                              , D_RRH_OFF, "AntB car0:use                  " },
		{ D_DU_SVCTL_ON_SLP_C0             , D_DU_SVCTL_BIT_SLP_B_C0        , 0,  0,  0                                              , D_RRH_OFF, "AntB car0:slp                  " }
	};

/*	USE Ant#B(mmW):(06)E_DU_SVCTL_OPE_USE_B	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptUseBmTbl[D_DU_SVCTL_ON_USE_NUM_MMW] =
	{
		{ D_DU_SVCTL_ON_USE_C0             , D_DU_SVCTL_BIT_USE_B_C0        , 0,  0,  0                                              , D_RRH_OFF, "AntB car0:use                  " },
		{ D_DU_SVCTL_ON_USE_C1             , D_DU_SVCTL_BIT_USE_B_C1        , 0,  0,  0                                              , D_RRH_OFF, "AntB car1:use                  " },
		{ D_DU_SVCTL_ON_USE_C2             , D_DU_SVCTL_BIT_USE_B_C2        , 0,  0,  0                                              , D_RRH_OFF, "AntB car2:use                  " },
		{ D_DU_SVCTL_ON_USE_C3             , D_DU_SVCTL_BIT_USE_B_C3        , 0,  0,  0                                              , D_RRH_OFF, "AntB car3:use                  " },
		{ D_DU_SVCTL_ON_SLP_C0             , D_DU_SVCTL_BIT_SLP_B_C0        , 0,  0,  0                                              , D_RRH_OFF, "AntB car0:slp                  " },
		{ D_DU_SVCTL_ON_SLP_C1             , D_DU_SVCTL_BIT_SLP_B_C1        , 0,  0,  0                                              , D_RRH_OFF, "AntB car1:slp                  " },
		{ D_DU_SVCTL_ON_SLP_C2             , D_DU_SVCTL_BIT_SLP_B_C2        , 0,  0,  0                                              , D_RRH_OFF, "AntB car2:slp                  " },
		{ D_DU_SVCTL_ON_SLP_C3             , D_DU_SVCTL_BIT_SLP_B_C3        , 0,  0,  0                                              , D_RRH_OFF, "AntB car3:slp                  " }
	};

/*	USE Ant#C(sub6):(07)E_DU_SVCTL_OPE_USE_C	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptUseCTbl[D_DU_SVCTL_ON_USE_NUM_SB6] =
	{
		{ D_DU_SVCTL_ON_USE_C0             , D_DU_SVCTL_BIT_USE_C_C0        , 0,  0,  0                                              , D_RRH_OFF, "AntC car0:use                  " },
		{ D_DU_SVCTL_ON_SLP_C0             , D_DU_SVCTL_BIT_SLP_C_C0        , 0,  0,  0                                              , D_RRH_OFF, "AntC car0:slp                  " }
	};

/*	USE Ant#D(sub6):(08)E_DU_SVCTL_OPE_USE_D	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptUseDTbl[D_DU_SVCTL_ON_USE_NUM_SB6] =
	{
		{ D_DU_SVCTL_ON_USE_C0             , D_DU_SVCTL_BIT_USE_D_C0        , 0,  0,  0                                              , D_RRH_OFF, "AntD car0:use                  " },
		{ D_DU_SVCTL_ON_SLP_C0             , D_DU_SVCTL_BIT_SLP_D_C0        , 0,  0,  0                                              , D_RRH_OFF, "AntD car0:slp                  " }
	};

/*	Fault Level:(09)E_DU_SVCTL_OPE_FLV	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFlvTbl[E_DU_SVCTL_FLVNO_MAX] =
	{
		{ E_DU_SVCTL_FLVNO_MINOR           , D_DU_SVCTL_BIT_FLV_MINOR       , E_DU_SVCTL_OPE_FLV,  0,  0                             , D_RRH_OFF, "FltLvl:minor                   " },
		{ E_DU_SVCTL_FLVNO_MAJOR           , D_DU_SVCTL_BIT_FLV_MAJOR       , E_DU_SVCTL_OPE_FLV,  0,  0                             , D_RRH_OFF, "FltLvl:major                   " },
		{ E_DU_SVCTL_FLVNO_CRITICAL        , D_DU_SVCTL_BIT_FLV_CRITICAL    , E_DU_SVCTL_OPE_FLV,  0,  0                             , D_RRH_OFF, "FltLvl:critical                " }
	};

/*	Fault ID:(10)E_DU_SVCTL_OPE_FID	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFidTbl[E_DU_SVCTL_OCCUR_FIDNO_MAX] =
	{
		{ E_DU_SVCTL_OCCUR_FIDNO_01        , D_DU_SVCTL_BIT_OCCUR_FID_01    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :01                      " },		/*	bit [0]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_02        , D_DU_SVCTL_BIT_OCCUR_FID_02    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :02                      " },		/*	bit [1]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_03        , D_DU_SVCTL_BIT_OCCUR_FID_03    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :03                      " },		/*	bit [2]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_04        , D_DU_SVCTL_BIT_OCCUR_FID_04    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :04                      " },		/*	bit [3]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_05        , D_DU_SVCTL_BIT_OCCUR_FID_05    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :05                      " },		/*	bit [4]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_06        , D_DU_SVCTL_BIT_OCCUR_FID_06    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :06                      " },		/*	bit [5]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_07        , D_DU_SVCTL_BIT_OCCUR_FID_07    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :07                      " },		/*	bit [6]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_08        , D_DU_SVCTL_BIT_OCCUR_FID_08    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :08                      " },		/*	bit [7]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_10        , D_DU_SVCTL_BIT_OCCUR_FID_10    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :10                      " },		/*	bit [8]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_11        , D_DU_SVCTL_BIT_OCCUR_FID_11    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :11                      " },		/*	bit [9]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_12        , D_DU_SVCTL_BIT_OCCUR_FID_12    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :12                      " },		/*	bit[10]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_30        , D_DU_SVCTL_BIT_OCCUR_FID_30    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :30                      " },		/*	bit[12]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_31        , D_DU_SVCTL_BIT_OCCUR_FID_31    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :31                      " },		/*	bit[13]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_40        , D_DU_SVCTL_BIT_OCCUR_FID_40    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :40                      " },		/*	bit[16]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_41        , D_DU_SVCTL_BIT_OCCUR_FID_41    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :41                      " },		/*	bit[17]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_42        , D_DU_SVCTL_BIT_OCCUR_FID_42    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :42                      " },		/*	bit[18]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_43        , D_DU_SVCTL_BIT_OCCUR_FID_43    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :43                      " },		/*	bit[19]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_44        , D_DU_SVCTL_BIT_OCCUR_FID_44    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :44                      " },		/*	bit[20]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_50        , D_DU_SVCTL_BIT_OCCUR_FID_50    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :50                      " },		/*	bit[24]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_F0        , D_DU_SVCTL_BIT_OCCUR_FID_F0    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :F0                      " },		/*	bit[28]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_F1        , D_DU_SVCTL_BIT_OCCUR_FID_F1    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :F1                      " },		/*	bit[29]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_F2        , D_DU_SVCTL_BIT_OCCUR_FID_F2    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :F2                      " },		/*	bit[30]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_FF        , D_DU_SVCTL_BIT_OCCUR_FID_FF    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :FF                      " },		/*	bit[31]	*/
		{ E_DU_SVCTL_OCCUR_FIDNO_51        , D_DU_SVCTL_BIT_OCCUR_FID_51    , E_DU_SVCTL_OPE_FID,  0,  0                             , D_RRH_OFF, "FltID :51                      " }		/*	bit[26]	*/
	};

/*	FID=0x01 Soft ALM code:(11)E_DU_SVCTL_OPE_SFT_ALMCD		*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid01Tbl[D_DU_FLTID_01_NUM] =
	{
		{ 0                                , 0x00000FFF                     , 0, E_DU_SVCTL_OCCUR_FIDNO_01, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_OFF, "Soft ALM:code                  " }
	};

/*	FID=0x02 Initialization failure code:(12)E_DU_SVCTL_OPE_INI_ALMCD	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid02Tbl[D_DU_FLTID_02_NUM] =
	{
		{ 0                                , 0x0FFF0000                     , 0, E_DU_SVCTL_OCCUR_FIDNO_02, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_OFF, "Init ALM:code                  " }
	};

/*	FID=0x03 failure:(13)E_DU_SVCTL_OPE_FID03_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid03Tbl[D_DU_FLTID_03_NUM] =
	{
		{ D_DU_FLTID_PTP_UNLOCK            , D_DU_ALMBIT_PTP_UNLOCK         , 0, E_DU_SVCTL_OCCUR_FIDNO_03, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_OFF, "Sync Err:PTP unlock            " }
	};

/*	FID=0x04 failure(sub6):(14)E_DU_SVCTL_OPE_FID04_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid04sTbl[D_DU_FLTID_04_NUM_SB6] =
	{
		{ D_DU_FLTID_JC_PLL_UNLOCK         , D_DU_ALMBIT_JC_PLL_UNLOCK      , 0, E_DU_SVCTL_OCCUR_FIDNO_04, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "Clk flt:JCPLL Unlock           " }
	};

/*	FID=0x04 failure(mmW):(14)E_DU_SVCTL_OPE_FID04_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid04mTbl[D_DU_FLTID_04_NUM_MMW] =
	{
		{ D_DU_FLTID_JC_PLL_UNLOCK         , D_DU_ALMBIT_JC_PLL_UNLOCK      , 0, E_DU_SVCTL_OCCUR_FIDNO_04, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "Clk flt:JCPLL Unlock           " },
		{ D_DU_FLTID_RF_PLL_UNLOCK         , D_DU_ALMBIT_RF_PLL_UNLOCK      , 0, E_DU_SVCTL_OCCUR_FIDNO_04, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "Clk flt:RFPLL Unlock           " }
	};

/*	FID=0x05 failure:(15)E_DU_SVCTL_OPE_FID05_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid05Tbl[D_DU_FLTID_05_NUM] =
	{
		{ D_DU_FLTID_L1_LDOWN_FH1          , D_DU_ALMBIT_L1_LDOWN_FH1       , 0, E_DU_SVCTL_OCCUR_FIDNO_05, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "FH flt :L1 Down                " },
		{ D_DU_FLTID_BER_DET_FH1           , D_DU_ALMBIT_BER_DET_FH1        , 0, E_DU_SVCTL_OCCUR_FIDNO_05, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "FH flt :BER                    " },
		{ D_DU_FLTID_SFP_UNMOUNT_FH1       , D_DU_ALMBIT_SFP_UNMOUNT_FH1    , 0, E_DU_SVCTL_OCCUR_FIDNO_05, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "FH flt :SFP Unmount            " },
		{ D_DU_FLTID_SPLANE_ABNL_FH1       , D_DU_ALMBIT_SPALNE_ABNL_FH1    , 0, E_DU_SVCTL_OCCUR_FIDNO_05, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "FH flt :S-Plane abnormal       " },
		{ D_DU_FLTID_UPLANE_ABNL_FH1       , D_DU_ALMBIT_UPALNE_ABNL_FH1    , 0, E_DU_SVCTL_OCCUR_FIDNO_05, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "FH flt :U-Plane abnormal       " },
		{ D_DU_FLTID_CPLANE_ABNL_FH1       , D_DU_ALMBIT_CPALNE_ABNL_FH1    , 0, E_DU_SVCTL_OCCUR_FIDNO_05, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "FH flt :C-Plane abnormal       " },
	};

/*	FID=0x06 failure(sub6):(16)E_DU_SVCTL_OPE_FID06_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid06Tbl[D_DU_FLTID_06_NUM] =
	{
		{ D_DU_FLTID_PS_ALM                , D_DU_ALMBIT_PS_ALM             , 0, E_DU_SVCTL_OCCUR_FIDNO_06, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "PS ALM :RF PS ALM              " },
		{ D_DU_FLTID_POW_ERR1              , D_DU_ALMBIT_POW_ERR1           , 0, E_DU_SVCTL_OCCUR_FIDNO_06, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "PS ALM :Power flt1             " },
		{ D_DU_FLTID_POW_ERR2              , D_DU_ALMBIT_POW_ERR2           , 0, E_DU_SVCTL_OCCUR_FIDNO_06, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "PS ALM :Power flt2             " },
		{ D_DU_FLTID_SFP_PDOWN_FH1         , D_DU_ALMBIT_SFP_PDOWN_FH1      , 0, E_DU_SVCTL_OCCUR_FIDNO_06, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "PS ALM :SFP P-Down             " }
	};

/*	FID=0x07 failure(sub6):(17)E_DU_SVCTL_OPE_FID07_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid07sTbl[D_DU_FLTID_07_NUM_SB6] =
	{
		{ D_DU_FLTID_SYS_PLL_UNLOCK_LLB    , D_DU_ALMBIT_SYS_PLL_UNLOCK_LLB , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB SYS PLL Unlock      " },
		{ D_DU_FLTID_IC_PLL_UNLOCK_LLB     , D_DU_ALMBIT_IC_PLL_UNLOCK_LLB  , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB ICPLL Unlock        " },
		{ D_DU_FLTID_ETH_PLL_UNLOCK_LLB    , D_DU_ALMBIT_ETH_PLL_UNLOCK_LLB , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB ETHPLL Unlock       " },
		{ D_DU_FLTID_TRX1_PCIE_ERR1_LLB    , D_DU_ALMBIT_TRX1_PCIE_ERR1_LLB , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB_TRX#1 PCIE Root Err " },
		{ D_DU_FLTID_TRX1_PCIE_ERR2_LLB    , D_DU_ALMBIT_TRX1_PCIE_ERR2_LLB , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB_TRX#1 PCIE EndP Err " },
		{ D_DU_FLTID_TRX1_SV_FAIL_LLB      , D_DU_ALMBIT_TRX1_SV_FAIL_LLB   , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB_TRX#1 SV bus fail   " },
		{ D_DU_FLTID_ETH_ALM               , D_DU_ALMBIT_ETH_ALM            , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:Ether Alm               " },
		{ D_DU_FLTID_SRAM_CORRECT_ERR      , D_DU_ALMBIT_SRAM_CORRECT_ERR   , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:SRAM Correct Error      " },
		{ D_DU_FLTID_SRAM_UNCORRECT_ERR    , D_DU_ALMBIT_SRAM_UNCORRECT_ERR , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:SRAM UnCorrect Error    " },
		{ D_DU_FLTID_TRX1_IC_TX_ERR_LLB    , D_DU_ALMBIT_TRX1_IC_TX_ERR_LLB , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB_TRX IC TX ERR       " },
		{ D_DU_FLTID_TRX1_IC_RX_ERR_LLB    , D_DU_ALMBIT_TRX1_IC_RX_ERR_LLB , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB_TRX IC RX ERR       " },
		{ D_DU_FLTID_LLB_IC_TX_ERR_TRX1    , D_DU_ALMBIT_LLB_IC_TX_ERR_TRX1 , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX_LLB IC TX ERR       " },
		{ D_DU_FLTID_LLB_IC_RX_ERR_TRX1    , D_DU_ALMBIT_LLB_IC_RX_ERR_TRX1 , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX_LLB IC RX ERR       " },
		{ D_DU_FLTID_AUTO_CRUISE_HC_ERR    , D_DU_ALMBIT_AUTO_CRUISE_HC_ERR , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:Auto Cruise HC ERR      " },
		{ D_DU_FLTID_SYS_PLL_UNLOCK_TRX1   , D_DU_ALMBIT_SYS_PLL_UNLOCK_TRX1, 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX SYS PLL Unlock      " },
		{ D_DU_FLTID_IC_PLL_UNLOCK_TRX1    , D_DU_ALMBIT_IC_PLL_UNLOCK_TRX1 , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX ICPLL Unlock        " },
		{ D_DU_FLTID_PHYICPLLULCK1_TRX1    , D_DU_ALMBIT_PHYICPLLULCK1_TRX1 , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX PHYICPLLULCK1       " },
		{ D_DU_FLTID_PHYICPLLULCK2_TRX1    , D_DU_ALMBIT_PHYICPLLULCK2_TRX1 , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX PHYICPLLULCK2       " },
		{ D_DU_FLTID_PHY_POW_ALM_TRX1      , D_DU_ALMBIT_PHY_POW_ALM_TRX1   , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX PHYPOWALM           " },
		{ D_DU_FLTID_PHYJS1PLLULCK_TRX1    , D_DU_ALMBIT_PHYJS1PLLULCK_TRX1 , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX PHYJS1PLLULCK       " },
		{ D_DU_FLTID_PHYJS2PLLULCK_TRX1    , D_DU_ALMBIT_PHYJS2PLLULCK_TRX1 , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX PHYJS2PLLULCK       " },
		{ D_DU_FLTID_TRX1_SV_FAIL_TRX1     , D_DU_ALMBIT_TRX1_SV_FAIL_TRX1  , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX SV bus failure      " },
		{ D_DU_FLTID_R1_TX_JESD_ERR_TRX1   , D_DU_ALMBIT_R1_TX_JESD_ERR_TRX1, 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX/RFIC1 TX JESD IF ERR" },
		{ D_DU_FLTID_R1_FB_JESD_ERR_TRX1   , D_DU_ALMBIT_R1_FB_JESD_ERR_TRX1, 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX/RFIC1 FB JESD IF ERR" },
		{ D_DU_FLTID_R1_RX_JESD_ERR_TRX1   , D_DU_ALMBIT_R1_RX_JESD_ERR_TRX1, 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX/RFIC1 RX JESD IF ERR" },
		{ D_DU_FLTID_R2_TX_JESD_ERR_TRX1   , D_DU_ALMBIT_R2_TX_JESD_ERR_TRX1, 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX/RFIC2 TX JESD IF ERR" },
		{ D_DU_FLTID_R2_FB_JESD_ERR_TRX1   , D_DU_ALMBIT_R2_FB_JESD_ERR_TRX1, 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX/RFIC2 FB JESD IF ERR" },
		{ D_DU_FLTID_R2_RX_JESD_ERR_TRX1   , D_DU_ALMBIT_R2_RX_JESD_ERR_TRX1, 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX/RFIC2 RX JESD IF ERR" },
		{ D_DU_FLTID_LMB_CORRECT_ERR       , D_DU_ALMBIT_LMB_CORRECT_ERR    , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LMB Correct Error       " },
		{ D_DU_FLTID_LMB_UNCORRECT_ERR     , D_DU_ALMBIT_LMB_UNCORRECT_ERR  , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LMB UnCorrect Error     " }
	};

/*	FID=0x07 failure(mmW):(17)E_DU_SVCTL_OPE_FID07_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid07mTbl[D_DU_FLTID_07_NUM_MMW] =
	{
		{ D_DU_FLTID_SYS_PLL_UNLOCK_LLB    , D_DU_ALMBIT_SYS_PLL_UNLOCK_LLB , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB SYS PLL Unlock      " },
		{ D_DU_FLTID_IC_PLL_UNLOCK_LLB     , D_DU_ALMBIT_IC_PLL_UNLOCK_LLB  , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB ICPLL Unlock        " },
		{ D_DU_FLTID_ETH_PLL_UNLOCK_LLB    , D_DU_ALMBIT_ETH_PLL_UNLOCK_LLB , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB ETHPLL Unlock       " },
		{ D_DU_FLTID_TRX1_PCIE_ERR1_LLB    , D_DU_ALMBIT_TRX1_PCIE_ERR1_LLB , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB_TRX#1 PCIE Root Err " },
		{ D_DU_FLTID_TRX1_PCIE_ERR2_LLB    , D_DU_ALMBIT_TRX1_PCIE_ERR2_LLB , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB_TRX#1 PCIE EndP Err " },
		{ D_DU_FLTID_TRX1_SV_FAIL_LLB      , D_DU_ALMBIT_TRX1_SV_FAIL_LLB   , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB_TRX#1 SV bus fail   " },
		{ D_DU_FLTID_ETH_ALM               , D_DU_ALMBIT_ETH_ALM            , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:Ether Alm               " },
		{ D_DU_FLTID_SRAM_CORRECT_ERR      , D_DU_ALMBIT_SRAM_CORRECT_ERR   , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:SRAM Correct Error      " },
		{ D_DU_FLTID_SRAM_UNCORRECT_ERR    , D_DU_ALMBIT_SRAM_UNCORRECT_ERR , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:SRAM UnCorrect Error    " },
		{ D_DU_FLTID_TRX1_IC_TX_ERR_LLB    , D_DU_ALMBIT_TRX1_IC_TX_ERR_LLB , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB_TRX IC TX ERR       " },
		{ D_DU_FLTID_TRX1_IC_RX_ERR_LLB    , D_DU_ALMBIT_TRX1_IC_RX_ERR_LLB , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LLB_TRX IC RX ERR       " },
		{ D_DU_FLTID_LLB_IC_TX_ERR_TRX1    , D_DU_ALMBIT_LLB_IC_TX_ERR_TRX1 , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX_LLB IC TX ERR       " },
		{ D_DU_FLTID_LLB_IC_RX_ERR_TRX1    , D_DU_ALMBIT_LLB_IC_RX_ERR_TRX1 , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX_LLB IC RX ERR       " },
		{ D_DU_FLTID_SYS_PLL_UNLOCK_TRX1   , D_DU_ALMBIT_SYS_PLL_UNLOCK_TRX1, 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX SYS PLL Unlock      " },
		{ D_DU_FLTID_IC_PLL_UNLOCK_TRX1    , D_DU_ALMBIT_IC_PLL_UNLOCK_TRX1 , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX ICPLL Unlock        " },
		{ D_DU_FLTID_PHYICPLLULCK1_TRX1    , D_DU_ALMBIT_PHYICPLLULCK1_TRX1 , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX PHYICPLLULCK1       " },
		{ D_DU_FLTID_PHYICPLLULCK2_TRX1    , D_DU_ALMBIT_PHYICPLLULCK2_TRX1 , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX PHYICPLLULCK2       " },
		{ D_DU_FLTID_PHY_POW_ALM_TRX1      , D_DU_ALMBIT_PHY_POW_ALM_TRX1   , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX PHYPOWALM           " },
		{ D_DU_FLTID_PHYJS1PLLULCK_TRX1    , D_DU_ALMBIT_PHYJS1PLLULCK_TRX1 , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX PHYJS1PLLULCK       " },
		{ D_DU_FLTID_TRX1_SV_FAIL_TRX1     , D_DU_ALMBIT_TRX1_SV_FAIL_TRX1  , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX SV bus failure      " },
		{ D_DU_FLTID_R1_TX_JESD_ERR_TRX1   , D_DU_ALMBIT_R1_TX_JESD_ERR_TRX1, 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX/RFIC1 TX JESD IF ERR" },
		{ D_DU_FLTID_R1_RX_JESD_ERR_TRX1   , D_DU_ALMBIT_R1_RX_JESD_ERR_TRX1, 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:TRX/RFIC1 RX JESD IF ERR" },
		{ D_DU_FLTID_LMB_CORRECT_ERR       , D_DU_ALMBIT_LMB_CORRECT_ERR    , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LMB Correct Error       " },
		{ D_DU_FLTID_LMB_UNCORRECT_ERR     , D_DU_ALMBIT_LMB_UNCORRECT_ERR  , 0, E_DU_SVCTL_OCCUR_FIDNO_07, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "HW flt:LMB UnCorrect Error     " }
	};

/*	FID=0x08 failure(sub6):(18)E_DU_SVCTL_OPE_FID08_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid08Tbl[D_DU_FLTID_08_NUM_SB6] =
	{
		{ D_DU_FLTID_RFIC1_ALM             , D_DU_ALMBIT_RFIC1_ALM          , 0, E_DU_SVCTL_OCCUR_FIDNO_08, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "RF flt:RFIC1 ALM               " },
		{ D_DU_FLTID_RFIC2_ALM             , D_DU_ALMBIT_RFIC2_ALM          , 0, E_DU_SVCTL_OCCUR_FIDNO_08, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "RF flt:RFIC2 ALM               " }
	};

/*	FID=0x10 failure Ant#A(sub6):(19)E_DU_SVCTL_OPE_FID10_ALM_A	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF10AntAsTbl[D_DU_FLTID_10_NUM_SB6] =
	{
		{ D_DU_FLTID_TX_GAIN_ALM_ANT_A     , D_DU_ALMBIT_TX_GAIN_ALM_ANT_A  , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA GAIN ALM           " },
		{ D_DU_FLTID_TOVER_ALM_ANT_A       , D_DU_ALMBIT_TOVER_ALM_ANT_A    , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA TOVER ALM          " },
		{ D_DU_FLTID_TX_CAL_FAIL_ANT_A     , D_DU_ALMBIT_TX_CAL_FAIL_ANT_A  , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA CAL FAIL           " }
	};

/*	FID=0x10 failure Ant#B(sub6):(20)E_DU_SVCTL_OPE_FID10_ALM_B	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF10AntBsTbl[D_DU_FLTID_10_NUM_SB6] =
	{
		{ D_DU_FLTID_TX_GAIN_ALM_ANT_B     , D_DU_ALMBIT_TX_GAIN_ALM_ANT_B  , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB GAIN ALM           " },
		{ D_DU_FLTID_TOVER_ALM_ANT_B       , D_DU_ALMBIT_TOVER_ALM_ANT_B    , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB TOVER ALM          " },
		{ D_DU_FLTID_TX_CAL_FAIL_ANT_B     , D_DU_ALMBIT_TX_CAL_FAIL_ANT_B  , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB CAL FAIL           " }
	};

/*	FID=0x10 failure Ant#C(sub6):(21)E_DU_SVCTL_OPE_FID10_ALM_C	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF10AntCTbl[D_DU_FLTID_10_NUM_SB6] =
	{
		{ D_DU_FLTID_TX_GAIN_ALM_ANT_C     , D_DU_ALMBIT_TX_GAIN_ALM_ANT_A  , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntC GAIN ALM           " },
		{ D_DU_FLTID_TOVER_ALM_ANT_C       , D_DU_ALMBIT_TOVER_ALM_ANT_A    , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntC TOVER ALM          " },
		{ D_DU_FLTID_TX_CAL_FAIL_ANT_C     , D_DU_ALMBIT_TX_CAL_FAIL_ANT_A  , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntC CAL FAIL           " }
	};

/*	FID=0x10 failure Ant#D(sub6):(22)E_DU_SVCTL_OPE_FID10_ALM_D	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF10AntDTbl[D_DU_FLTID_10_NUM_SB6] =
	{
		{ D_DU_FLTID_TX_GAIN_ALM_ANT_D     , D_DU_ALMBIT_TX_GAIN_ALM_ANT_B  , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntD GAIN ALM           " },
		{ D_DU_FLTID_TOVER_ALM_ANT_D       , D_DU_ALMBIT_TOVER_ALM_ANT_B    , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntD TOVER ALM          " },
		{ D_DU_FLTID_TX_CAL_FAIL_ANT_D     , D_DU_ALMBIT_TX_CAL_FAIL_ANT_B  , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntD CAL FAIL           " }
	};

/*	FID=0x10 failure Ant#A(mmW):(19)E_DU_SVCTL_OPE_FID10_ALM_A	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF10AntAmTbl[D_DU_FLTID_10_AB_NUM_MMW] =
	{
		{ D_DU_FLTID_TOVER_ALM_ANT_A       , D_DU_ALMBIT_TOVER_ALM_ANT_A    , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA TOVER ALM          " }
	};

/*	FID=0x10 failure Ant#B(mmW):(20)E_DU_SVCTL_OPE_FID10_ALM_B	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF10AntBmTbl[D_DU_FLTID_10_AB_NUM_MMW] =
	{
		{ D_DU_FLTID_TOVER_ALM_ANT_B       , D_DU_ALMBIT_TOVER_ALM_ANT_B    , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB TOVER ALM          " }
	};

/*	FID=0x11 failure Ant#A(sub6):(23)E_DU_SVCTL_OPE_FID11_ALM_A	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF11AntAsTbl[D_DU_FLTID_11_NUM_SB6] =
	{
		{ D_DU_FLTID_RX_GAIN_ALM_ANT_A     , D_DU_ALMBIT_RX_GAIN_ALM_ANT_A  , 0, E_DU_SVCTL_OCCUR_FIDNO_11, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Rx flt:AntA GAIN ALM           " },
		{ D_DU_FLTID_RX_CAL_FAIL_ANT_A     , D_DU_ALMBIT_RX_CAL_FAIL_ANT_A  , 0, E_DU_SVCTL_OCCUR_FIDNO_11, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Rx flt:AntA CAL FAIL           " },
		{ D_DU_FLTID_LNA_ABNORMAL_ANT_A    , D_DU_ALMBIT_LNA_ABNORMAL_ANT_A , 0, E_DU_SVCTL_OCCUR_FIDNO_11, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Rx flt:AntA LNA abnormal       " }
	};

/*	FID=0x11 failure Ant#B(sub6):(24)E_DU_SVCTL_OPE_FID11_ALM_B	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF11AntBsTbl[D_DU_FLTID_11_NUM_SB6] =
	{
		{ D_DU_FLTID_RX_GAIN_ALM_ANT_B     , D_DU_ALMBIT_RX_GAIN_ALM_ANT_B  , 0, E_DU_SVCTL_OCCUR_FIDNO_11, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Rx flt:AntB GAIN ALM           " },
		{ D_DU_FLTID_RX_CAL_FAIL_ANT_B     , D_DU_ALMBIT_RX_CAL_FAIL_ANT_B  , 0, E_DU_SVCTL_OCCUR_FIDNO_11, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Rx flt:AntB CAL FAIL           " },
		{ D_DU_FLTID_LNA_ABNORMAL_ANT_B    , D_DU_ALMBIT_LNA_ABNORMAL_ANT_B , 0, E_DU_SVCTL_OCCUR_FIDNO_11, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Rx flt:AntB LNA abnormal       " }
	};

/*	FID=0x11 failure Ant#C(sub6):(25)E_DU_SVCTL_OPE_FID11_ALM_C	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF11AntCTbl[D_DU_FLTID_11_NUM_SB6] =
	{
		{ D_DU_FLTID_RX_GAIN_ALM_ANT_C     , D_DU_ALMBIT_RX_GAIN_ALM_ANT_A  , 0, E_DU_SVCTL_OCCUR_FIDNO_11, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Rx flt:AntC GAIN ALM           " },
		{ D_DU_FLTID_RX_CAL_FAIL_ANT_C     , D_DU_ALMBIT_RX_CAL_FAIL_ANT_A  , 0, E_DU_SVCTL_OCCUR_FIDNO_11, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Rx flt:AntC CAL FAIL           " },
		{ D_DU_FLTID_LNA_ABNORMAL_ANT_C    , D_DU_ALMBIT_LNA_ABNORMAL_ANT_A , 0, E_DU_SVCTL_OCCUR_FIDNO_11, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Rx flt:AntC LNA abnormal       " }
	};

/*	FID=0x11 failure Ant#D(sub6):(26)E_DU_SVCTL_OPE_FID11_ALM_D	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF11AntDTbl[D_DU_FLTID_11_NUM_SB6] =
	{
		{ D_DU_FLTID_RX_GAIN_ALM_ANT_D     , D_DU_ALMBIT_RX_GAIN_ALM_ANT_B  , 0, E_DU_SVCTL_OCCUR_FIDNO_11, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Rx flt:AntD GAIN ALM           " },
		{ D_DU_FLTID_RX_CAL_FAIL_ANT_D     , D_DU_ALMBIT_RX_CAL_FAIL_ANT_B  , 0, E_DU_SVCTL_OCCUR_FIDNO_11, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Rx flt:AntD CAL FAIL           " },
		{ D_DU_FLTID_LNA_ABNORMAL_ANT_D    , D_DU_ALMBIT_LNA_ABNORMAL_ANT_B , 0, E_DU_SVCTL_OCCUR_FIDNO_11, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Rx flt:AntD LNA abnormal       " }
	};

/*	FID=0x11 failure Ant#A(mmW):(23)E_DU_SVCTL_OPE_FID11_ALM_A	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF11AntAmTbl[D_DU_FLTID_11_AB_NUM_MMW] =
	{
		{ D_DU_FLTID_RX_GAIN_ALM_ANT_A     , D_DU_ALMBIT_RX_GAIN_ALM_ANT_A  , 0, E_DU_SVCTL_OCCUR_FIDNO_11, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Rx flt:AntA GAIN ALM           " }
	};

/*	FID=0x11 failure Ant#B(mmW):(24)E_DU_SVCTL_OPE_FID11_ALM_B	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF11AntBmTbl[D_DU_FLTID_11_AB_NUM_MMW] =
	{
		{ D_DU_FLTID_RX_GAIN_ALM_ANT_B     , D_DU_ALMBIT_RX_GAIN_ALM_ANT_B  , 0, E_DU_SVCTL_OCCUR_FIDNO_11, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Rx flt:AntB GAIN ALM           " }
	};

/*	FID=0x12 failure Ant#A(sub6):(27)E_DU_SVCTL_OPE_FID12_ALM_A	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF12AntATbl[D_DU_FLTID_12_NUM_SB6] =
	{
		{ D_DU_FLTID_VSWR_ALM_ANT_A        , D_DU_ALMBIT_VSWR_ALM_ANT_A     , 0, E_DU_SVCTL_OCCUR_FIDNO_12, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "ANTabnr:AntA VSWR ALM          " }
	};

/*	FID=0x12 failure Ant#B(sub6):(28)E_DU_SVCTL_OPE_FID12_ALM_B	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF12AntBTbl[D_DU_FLTID_12_NUM_SB6] =
	{
		{ D_DU_FLTID_VSWR_ALM_ANT_B        , D_DU_ALMBIT_VSWR_ALM_ANT_B     , 0, E_DU_SVCTL_OCCUR_FIDNO_12, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "ANTabnr:AntB VSWR ALM          " }
	};

/*	FID=0x12 failure Ant#C(sub6):(29)E_DU_SVCTL_OPE_FID12_ALM_C	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF12AntCTbl[D_DU_FLTID_12_NUM_SB6] =
	{
		{ D_DU_FLTID_VSWR_ALM_ANT_C        , D_DU_ALMBIT_VSWR_ALM_ANT_C     , 0, E_DU_SVCTL_OCCUR_FIDNO_12, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "ANTabnr:AntC VSWR ALM          " }
	};

/*	FID=0x12 failure Ant#D(sub6):(30)E_DU_SVCTL_OPE_FID12_ALM_D	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF12AntDTbl[D_DU_FLTID_12_NUM_SB6] =
	{
		{ D_DU_FLTID_VSWR_ALM_ANT_D        , D_DU_ALMBIT_VSWR_ALM_ANT_D     , 0, E_DU_SVCTL_OCCUR_FIDNO_12, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "ANTabnr:AntD VSWR ALM          " }
	};

/*	FID=0x10 failure power detector VH(mmW):(13)E_DU_SVCTL_OPE_FID10_VH	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF10VHTbl[D_DU_FLTID_10_VH_NUM_MMW] =
	{
		{ D_DU_FLTID_TX_PW_ALM_V1          , D_DU_ALMBIT_TX_PW_ALM_V1       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#01     " },
		{ D_DU_FLTID_TX_PW_ALM_V2          , D_DU_ALMBIT_TX_PW_ALM_V2       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#02     " },
		{ D_DU_FLTID_TX_PW_ALM_V3          , D_DU_ALMBIT_TX_PW_ALM_V3       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#03     " },
		{ D_DU_FLTID_TX_PW_ALM_V4          , D_DU_ALMBIT_TX_PW_ALM_V4       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#04     " },
		{ D_DU_FLTID_TX_PW_ALM_V5          , D_DU_ALMBIT_TX_PW_ALM_V5       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#05     " },
		{ D_DU_FLTID_TX_PW_ALM_V6          , D_DU_ALMBIT_TX_PW_ALM_V6       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#06     " },
		{ D_DU_FLTID_TX_PW_ALM_V7          , D_DU_ALMBIT_TX_PW_ALM_V7       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#07     " },
		{ D_DU_FLTID_TX_PW_ALM_V8          , D_DU_ALMBIT_TX_PW_ALM_V8       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#08     " },
		{ D_DU_FLTID_TX_PW_ALM_V9          , D_DU_ALMBIT_TX_PW_ALM_V9       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#09     " },
		{ D_DU_FLTID_TX_PW_ALM_V10         , D_DU_ALMBIT_TX_PW_ALM_V10      , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#10     " },
		{ D_DU_FLTID_TX_PW_ALM_V11         , D_DU_ALMBIT_TX_PW_ALM_V11      , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#11     " },
		{ D_DU_FLTID_TX_PW_ALM_V12         , D_DU_ALMBIT_TX_PW_ALM_V12      , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#12     " },
		{ D_DU_FLTID_TX_PW_ALM_V13         , D_DU_ALMBIT_TX_PW_ALM_V13      , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#13     " },
		{ D_DU_FLTID_TX_PW_ALM_V14         , D_DU_ALMBIT_TX_PW_ALM_V14      , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#14     " },
		{ D_DU_FLTID_TX_PW_ALM_V15         , D_DU_ALMBIT_TX_PW_ALM_V15      , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#15     " },
		{ D_DU_FLTID_TX_PW_ALM_V16         , D_DU_ALMBIT_TX_PW_ALM_V16      , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntB Power ALM V#16     " },
		{ D_DU_FLTID_TX_PW_ALM_H1          , D_DU_ALMBIT_TX_PW_ALM_H1       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#01     " },
		{ D_DU_FLTID_TX_PW_ALM_H2          , D_DU_ALMBIT_TX_PW_ALM_H2       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#02     " },
		{ D_DU_FLTID_TX_PW_ALM_H3          , D_DU_ALMBIT_TX_PW_ALM_H3       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#03     " },
		{ D_DU_FLTID_TX_PW_ALM_H4          , D_DU_ALMBIT_TX_PW_ALM_H4       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#04     " },
		{ D_DU_FLTID_TX_PW_ALM_H5          , D_DU_ALMBIT_TX_PW_ALM_H5       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#05     " },
		{ D_DU_FLTID_TX_PW_ALM_H6          , D_DU_ALMBIT_TX_PW_ALM_H6       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#06     " },
		{ D_DU_FLTID_TX_PW_ALM_H7          , D_DU_ALMBIT_TX_PW_ALM_H7       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#07     " },
		{ D_DU_FLTID_TX_PW_ALM_H8          , D_DU_ALMBIT_TX_PW_ALM_H8       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#08     " },
		{ D_DU_FLTID_TX_PW_ALM_H9          , D_DU_ALMBIT_TX_PW_ALM_H9       , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#09     " },
		{ D_DU_FLTID_TX_PW_ALM_H10         , D_DU_ALMBIT_TX_PW_ALM_H10      , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#10     " },
		{ D_DU_FLTID_TX_PW_ALM_H11         , D_DU_ALMBIT_TX_PW_ALM_H11      , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#11     " },
		{ D_DU_FLTID_TX_PW_ALM_H12         , D_DU_ALMBIT_TX_PW_ALM_H12      , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#12     " },
		{ D_DU_FLTID_TX_PW_ALM_H13         , D_DU_ALMBIT_TX_PW_ALM_H13      , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#13     " },
		{ D_DU_FLTID_TX_PW_ALM_H14         , D_DU_ALMBIT_TX_PW_ALM_H14      , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#14     " },
		{ D_DU_FLTID_TX_PW_ALM_H15         , D_DU_ALMBIT_TX_PW_ALM_H15      , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#15     " },
		{ D_DU_FLTID_TX_PW_ALM_H16         , D_DU_ALMBIT_TX_PW_ALM_H16      , 0, E_DU_SVCTL_OCCUR_FIDNO_10, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_ON , "Tx flt:AntA Power ALM H#16     " }
	};

/*	FID=0x30 failure:(32)E_DU_SVCTL_OPE_FID30_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid30Tbl[D_DU_FLTID_30_NUM] =
	{
		{ D_DU_FLTID_TEMP_ALM_LOW          , D_DU_ALMBIT_TEMP_ALM_LOW       , 0, E_DU_SVCTL_OCCUR_FIDNO_30, E_DU_SVCTL_FLVNO_MAJOR   , D_RRH_OFF, "TempLow:Temperature too low    " }
	};

/*	FID=0x31 failure:(33)E_DU_SVCTL_OPE_FID31_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid31Tbl[D_DU_FLTID_31_NUM] =
	{
		{ D_DU_FLTID_TEMP_ALM_HIGH         , D_DU_ALMBIT_TEMP_ALM_HIGH      , 0, E_DU_SVCTL_OCCUR_FIDNO_31, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "TempHi :Temperature too high   " }
	};

/*	FID=0x40 failure:(34)E_DU_SVCTL_OPE_FID40_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid40Tbl[D_DU_FLTID_40_NUM] =
	{
		{ D_DU_FLTID_NO_FAN_DETECT         , D_DU_ALMBIT_NO_FAN_DETECT      , 0, E_DU_SVCTL_OCCUR_FIDNO_40, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "FanUnM :No fan detected        " }
	};

/*	FID=0x41 failure:(35)E_DU_SVCTL_OPE_FID41_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid41Tbl[D_DU_FLTID_41_NUM] =
	{
		{ D_DU_FLTID_FAN_PS_ALM            , D_DU_ALMBIT_FAN_PS_ALM         , 0, E_DU_SVCTL_OCCUR_FIDNO_41, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "Fan PS :FAN PS ALM             " }
	};

/*	FID=0x42 failure:(36)E_DU_SVCTL_OPE_FID42_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid42Tbl[D_DU_FLTID_42_NUM] =
	{
		{ D_DU_FLTID_COOL_FAN1_BROK        , D_DU_ALMBIT_COOL_FAN1_BROK     , 0, E_DU_SVCTL_OCCUR_FIDNO_42, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "Fan Bk :Cooling fan#1 broken   " }
	};

/*	FID=0x43 failure:(37)E_DU_SVCTL_OPE_FID43_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid43Tbl[D_DU_FLTID_43_NUM] =
	{
		{ D_DU_FLTID_COOL_FAN2_BROK        , D_DU_ALMBIT_COOL_FAN2_BROK     , 0, E_DU_SVCTL_OCCUR_FIDNO_43, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "Fan Bk :Cooling fan#2 broken   " }
	};

/*	FID=0x44 failure:(38)E_DU_SVCTL_OPE_FID44_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid44Tbl[D_DU_FLTID_44_NUM] =
	{
		{ D_DU_FLTID_COOL_FAN3_BROK        , D_DU_ALMBIT_COOL_FAN3_BROK     , 0, E_DU_SVCTL_OCCUR_FIDNO_44, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "Fan Bk :Cooling fan#3 broken   " }
	};

/*	FID=0x50 failure:(39)E_DU_SVCTL_OPE_FID50_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid50Tbl[D_DU_FLTID_50_NUM] =
	{
		{ D_DU_FLTID_SFP_TYP_ERR           , D_DU_ALMBIT_SFP_TYP_ERR        , 0, E_DU_SVCTL_OCCUR_FIDNO_50, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "SFP Typ:SFP Type Error         " },
		{ D_DU_FLTID_SFP_ACCESS_ERR        , D_DU_ALMBIT_SFP_ACCESS_ERR     , 0, E_DU_SVCTL_OCCUR_FIDNO_50, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "SFP Typ:SFP Access Error       " }
	};

/*	FID=0x51 failure:(40)E_DU_SVCTL_OPE_FID51_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid51Tbl[D_DU_FLTID_51ALM_NUM] =
	{
		{ D_DU_FLTID_SFP_FAILSAFE_NTF       , D_DU_ALMBIT_SFP_FAILSAFE_NTF  , 0, E_DU_SVCTL_OCCUR_FIDNO_51, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "FH fail safe:Notify            " }
	};

/*	FID=0x51 failure:(41)E_DU_SVCTL_OPE_FID51_NOALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid51NoTbl[D_DU_FLTID_51NO_NUM] =
	{
		{ D_DU_FLTID_SFP_FAILSAFE           , D_DU_ALMBIT_SFP_FAILSAFE      , 0, 0, 0                                                , D_RRH_OFF, "FH fail safe:No Notify         " },
	};

/*	FID=0xF0 failure:(42)E_DU_SVCTL_OPE_FIDF0_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFidf0Tbl[D_DU_FLTID_F0_NUM] =
	{
		{ D_DU_FLTID_TX_SLEEP              , D_DU_ALMBIT_TX_SLEEP           , 0, E_DU_SVCTL_OCCUR_FIDNO_F0, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "sleep  :Tx Sleep               " }
	};

/*	FID=0xF1 failure:(43)E_DU_SVCTL_OPE_FIDF1_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFidf1Tbl[D_DU_FLTID_F1_NUM] =
	{
		{ D_DU_FLTID_TX_OFF                , D_DU_ALMBIT_TX_OFF             , 0, E_DU_SVCTL_OCCUR_FIDNO_F1, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "PA off :Tx off                 " }
	};

/*	FID=0xF2 failure:(44)E_DU_SVCTL_OPE_FIDF2_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFidf2Tbl[D_DU_FLTID_F2_NUM] =
	{
		{ D_DU_FLTID_BEAM_ID_FIX           , D_DU_ALMBIT_BEAM_ID_FIX        , 0, E_DU_SVCTL_OCCUR_FIDNO_F2, E_DU_SVCTL_FLVNO_MINOR   , D_RRH_OFF, "BeamID :TEST Mode              " }
	};

/*	FID=0xFF failure:(45)E_DU_SVCTL_OPE_FIDFF_ALM	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFidffTbl[D_DU_FLTID_FF_NUM] =
	{
		{ D_DU_FLTID_FORCE_ALM             , D_DU_ALMBIT_FORCE_ALM          , 0, E_DU_SVCTL_OCCUR_FIDNO_FF, E_DU_SVCTL_FLVNO_CRITICAL, D_RRH_ON , "FC ALM :Forced alarm           " }
	};

/*	Running file download:(46)E_DU_SVCTL_OPE_FLDL	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFlDwnldTbl[D_DU_SVCTL_FL_DWNLD_NUM] =
	{
		{ D_DU_SVCTL_ON_FL_DWNLD           , D_DU_SVCTL_FL_DWNLD            , 0,  0,  0                                              , D_RRH_OFF, "File Download                  " }
	};

/*	all ANT 停波:(47)E_DU_SVCTL_OPE_STP_WAV	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptStpWTbl[D_DU_SVCTL_ON_WAVE_STOP_NUM] =
	{
		{ D_DU_SVCTL_ON_WAVE_STOP_ALL             , D_DU_SVCTL_WAVE_STOP_ALL       , 0,  0,  0                                       , D_RRH_OFF, "Stop wave:syncE                " },
		{ D_DU_SVCTL_ON_WAVE_STOP_ALL_PTP         , D_DU_SVCTL_WAVE_STOP_ALL_PTP   , 0,  0,  0                                       , D_RRH_OFF, "Stop wave:PTP                  " },
		{ D_DU_SVCTL_ON_WAVE_STOP_ALL_PTP_UNLOCK  , D_DU_SVCTL_WAVE_STOP_ALL_PTP_UNLOCK   , 0,  0,  0                                , D_RRH_OFF, "Stop wave:PTP Unlock           " }
	};

/*	SyncE Status:(48)E_DU_SVCTL_OPE_SYNCE	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptSyncETbl[D_DU_SVCTL_ON_SYNCE_STATE_NUM] =
	{
		{ D_DU_SVCTL_ON_SYNCE_STATE_DISABLE, D_DU_SVCTL_SYNCE_STATE_DISABLE , 0,  0,  0                                              , D_RRH_OFF, "syncE sts:Disable              " },
		{ D_DU_SVCTL_ON_SYNCE_STATE_PARENT , D_DU_SVCTL_SYNCE_STATE_PARENT  , 0,  0,  0                                              , D_RRH_OFF, "syncE sts:parent               " },
		{ D_DU_SVCTL_ON_SYNCE_STATE_OK     , D_DU_SVCTL_SYNCE_STATE_OK      , 0,  0,  0                                              , D_RRH_OFF, "syncE sts:OK                   " },
		{ D_DU_SVCTL_ON_SYNCE_STATE_NOK    , D_DU_SVCTL_SYNCE_STATE_NOK     , 0,  0,  0                                              , D_RRH_OFF, "syncE sts:NOK                  " }
	};

/*	PTP/SyncE Locked Status:(49)E_DU_SVCTL_OPE_SPLAN	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptSplanTbl[D_DU_SVCTL_ON_SPLAN_NUM] =
	{
		{ D_DU_SVCTL_ON_PTP_STATE_LOCKED   , D_DU_SVCTL_PTP_STATE_LOCKED    , 0,  0,  0                                              , D_RRH_OFF, "sync lock:PTP Locked           " },
		{ D_DU_SVCTL_ON_SYNCE_STATE_LOCKED , D_DU_SVCTL_SYNCE_STATE_LOCKED  , 0,  0,  0                                              , D_RRH_OFF, "sync lock:syncE Locked         " }
	};

/*	Sync Status:(50)E_DU_SVCTL_OPE_SYNC	*/
const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptSyncTbl[D_DU_SVCTL_ON_SYNC_STATE_NUM] =
	{
		{ D_DU_SVCTL_ON_SYNC_STATE_FREERUN , D_DU_SVCTL_SYNC_STATE_FREERUN  , 0,  0,  0                                              , D_RRH_OFF, "sync sts:freerun               " },
		{ D_DU_SVCTL_ON_SYNC_STATE_LOCKED  , D_DU_SVCTL_SYNC_STATE_LOCKED   , 0,  0,  0                                              , D_RRH_OFF, "sync sts:locked                " },
		{ D_DU_SVCTL_ON_SYNC_STATE_HOLDOVER, D_DU_SVCTL_SYNC_STATE_HOLDOVER , 0,  0,  0                                              , D_RRH_OFF, "sync sts:holdover              " }
	};

/*	SV Bus set default register val table	*/
const T_COM_DU_SVOPT_DEF_TBL f_comr_svOptMapDefTbl[D_DU_SVINF_NUM] =
	{
		/*	SVバス情報#01	*/
		{
			D_DU_REG_CTB_SV1INF,
			D_DU_REG_CTB_SV1CMPM,
			D_DU_SVCTL_SV01_MAX,
			{ E_DU_SVCTL_OPE_ADM, E_DU_SVCTL_OPE_ENG, E_DU_SVCTL_OPE_OPE, E_DU_SVCTL_OPE_AVL, E_DU_SVCTL_OPE_USA, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX }
		},
		/*	SVバス情報#02	*/
		{
			D_DU_REG_CTB_SV2INF,
			D_DU_REG_CTB_SV2CMPM,
			D_DU_SVCTL_SV02_MAX,
			{ E_DU_SVCTL_OPE_USE_A, E_DU_SVCTL_OPE_USE_B, E_DU_SVCTL_OPE_USE_C, E_DU_SVCTL_OPE_USE_D, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX }
		},
		/*	SVバス情報#03	*/
		{
			D_DU_REG_CTB_SV3INF,
			D_DU_REG_CTB_SV3CMPM,
			D_DU_SVCTL_SV03_MAX,
			{ E_DU_SVCTL_OPE_FLV, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX }
		},
		/*	SVバス情報#04	*/
		{
			D_DU_REG_CTB_SV4INF,
			D_DU_REG_CTB_SV4CMPM,
			D_DU_SVCTL_SV04_MAX,
			{ E_DU_SVCTL_OPE_FID, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX }
		},
		/*	SVバス情報#05	*/
		{
			D_DU_REG_CTB_SV5INF,
			D_DU_REG_CTB_SV5CMPM,
			D_DU_SVCTL_SV05_MAX,
			{ E_DU_SVCTL_OPE_SFT_ALMCD, E_DU_SVCTL_OPE_INI_ALMCD, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX }
		},
		/*	SVバス情報#06	*/
		{
			D_DU_REG_CTB_SV6INF,
			D_DU_REG_CTB_SV6CMPM,
			D_DU_SVCTL_SV06_MAX,
			{ E_DU_SVCTL_OPE_FID03_ALM, E_DU_SVCTL_OPE_FID04_ALM, E_DU_SVCTL_OPE_FID05_ALM, E_DU_SVCTL_OPE_FID06_ALM, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX }
		},
		/*	SVバス情報#07	*/
		{
			D_DU_REG_CTB_SV7INF,
			D_DU_REG_CTB_SV7CMPM,
			D_DU_SVCTL_SV07_MAX,
			{ E_DU_SVCTL_OPE_FID07LL_ALM, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX }
		},
		/*	SVバス情報#08	*/
		{
			D_DU_REG_CTB_SV8INF,
			D_DU_REG_CTB_SV8CMPM,
			D_DU_SVCTL_SV08_MAX,
			{ E_DU_SVCTL_OPE_FID08_ALM, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX }
		},
		/*	SVバス情報#09	*/
		{
			D_DU_REG_CTB_SV9INF,
			D_DU_REG_CTB_SV9CMPM,
			D_DU_SVCTL_SV09_MAX,
			{ E_DU_SVCTL_OPE_FID10_ALM_A, E_DU_SVCTL_OPE_FID10_ALM_B, E_DU_SVCTL_OPE_FID11_ALM_A, E_DU_SVCTL_OPE_FID11_ALM_B, E_DU_SVCTL_OPE_FID12_ALM_A, E_DU_SVCTL_OPE_FID12_ALM_B, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX }
		},
		/*	SVバス情報#10	*/
		{
			D_DU_REG_CTB_SV10INF,
			D_DU_REG_CTB_SV10CMPM,
			D_DU_SVCTL_SV10_MAX,
			{ E_DU_SVCTL_OPE_FID10_ALM_C, E_DU_SVCTL_OPE_FID10_ALM_D, E_DU_SVCTL_OPE_FID11_ALM_C, E_DU_SVCTL_OPE_FID11_ALM_D, E_DU_SVCTL_OPE_FID12_ALM_C, E_DU_SVCTL_OPE_FID12_ALM_D, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX }
		},
		/*	SVバス情報#11	*/
		{
			D_DU_REG_CTB_SV11INF,
			D_DU_REG_CTB_SV11CMPM,
			D_DU_SVCTL_SV11_MAX,
			{ E_DU_SVCTL_OPE_FID10_ALM_VH, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX }
		},
		/*	SVバス情報#12	*/
		{
			D_DU_REG_CTB_SV12INF,
			D_DU_REG_CTB_SV12CMPM,
			D_DU_SVCTL_SV12_MAX,
			{ E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX }
		},
		/*	SVバス情報#13	*/
		{
			D_DU_REG_CTB_SV13INF,
			D_DU_REG_CTB_SV13CMPM,
			D_DU_SVCTL_SV13_MAX,
			{ E_DU_SVCTL_OPE_FID30_ALM, E_DU_SVCTL_OPE_FID31_ALM, E_DU_SVCTL_OPE_FID40_ALM, E_DU_SVCTL_OPE_FID41_ALM, E_DU_SVCTL_OPE_FID42_ALM, E_DU_SVCTL_OPE_FID43_ALM, E_DU_SVCTL_OPE_FID44_ALM, E_DU_SVCTL_OPE_FID50_ALM, E_DU_SVCTL_OPE_FID51_ALM, E_DU_SVCTL_OPE_FID51_NOALM }
		},
		/*	SVバス情報#14	*/
		{
			D_DU_REG_CTB_SV14INF,
			D_DU_REG_CTB_SV14CMPM,
			D_DU_SVCTL_SV14_MAX,
			{ E_DU_SVCTL_OPE_FIDF0_ALM, E_DU_SVCTL_OPE_FIDF1_ALM, E_DU_SVCTL_OPE_FIDF2_ALM, E_DU_SVCTL_OPE_FIDFF_ALM, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX }
		},
		/*	SVバス情報#15	*/
		{
			D_DU_REG_CTB_SV15INF,
			D_DU_REG_CTB_SV15CMPM,
			D_DU_SVCTL_SV15_MAX,
			{ E_DU_SVCTL_OPE_FLDL, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX }
		},
		/*	SVバス情報#16	*/
		{
			D_DU_REG_CTB_SV16INF,
			D_DU_REG_CTB_SV16CMPM,
			D_DU_SVCTL_SV16_MAX,
			{ E_DU_SVCTL_OPE_STP_WAV, E_DU_SVCTL_OPE_SYNCE, E_DU_SVCTL_OPE_SPLAN, E_DU_SVCTL_OPE_SYNC, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX, E_DU_SVCTL_OPE_MAX }
		}
	};

/** @} */
