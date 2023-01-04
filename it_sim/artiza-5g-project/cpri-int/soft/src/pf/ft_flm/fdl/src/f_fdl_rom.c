/*!
 * @skip $Id:$
 * @file f_fdl_rom.c
 * @brief FDLタスク 内部テーブル
 * @date  2019/01/28 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */

#include "f_fdl_inc.h"

/* FDL State Mng */
const	T_FDL_CALLSETRELL_MTXTBL f_fdl_callSetRell_mtxtbl[D_FDL_STATENUM][E_FDL_EVE_EVENTNUM] __attribute__ ((aligned (16))) =
{
	{
		/* init */
		{f_fdl_init							},					/**< スレッド起動							*/
		{f_fdl_abort						},					/**< インベントリ要求						*/
		{f_fdl_abort						},					/**< ダウンロード要求						*/
		{f_fdl_abort						},					/**< インストール要求						*/
		{f_fdl_abort						},					/**< アクティベート要求						*/
		{f_fdl_abort						},					/**< マスタ側ファイル更新書込み応答要求		*/
		{f_fdl_abort						}					/**< スレーブ側ファイル更新書込み応答要求	*/
	},
	{
		/* 運用状態 */
		{f_fdl_abort						},					/**< スレッド起動							*/
		{f_fdl_inventry_rcv					},					/**< インベントリ要求						*/
		{f_fdl_download_rcv					},					/**< ダウンロード要求						*/
		{f_fdl_install_rcv					},					/**< インストール要求						*/
		{f_fdl_activate_rcv					},					/**< アクティベート要求						*/
		{f_fdl_filewrite_m					},					/**< マスタ側ファイル更新書込み応答要求		*/
		{f_fdl_filewrite_s					}					/**< スレーブ側ファイル更新書込み応答要求	*/
	},
	{
		/* ローディング中状態 */
		{f_fdl_abort						},					/**< スレッド起動							*/
		{f_fdl_inventry_rcv					},					/**< インベントリ要求						*/
		{f_fdl_msi_err						},					/**< ダウンロード要求						*/
		{f_fdl_msi_err						},					/**< インストール要求						*/
		{f_fdl_msi_err						},					/**< アクティベート要求						*/
		{f_fdl_filewrite_m					},					/**< マスタ側ファイル更新書込み応答要求		*/
		{f_fdl_filewrite_s					}					/**< スレーブ側ファイル更新書込み応答要求	*/
	}
};

/* FDL Slot Name */
const	char	f_fdl_Slot_nametbl[D_SYS_NUM2][D_SYS_NUM16] =
{
	{"slot0"	},												/**< スロット0		*/
	{"slot1"	}												/**< スロット1		*/
};

const char f_fdl_install_errtbl[E_FDL_ERR_ERRMAX][D_SYS_NUM32] =
{
 {"Install OK"},
 {"Unzip FIle Open Error"},       /* ファイルオープンエラー */
 {"Unzip EXE Error"},             /* unzip実行エラー */
 {"Install File Check"},          /* 更新ファイル名一致 */
 {"Flash File Erase Error"},      /* Flash消去失敗 */
 {"Flash File Write Error"},      /* Flash書込み失敗 */
 {"Write Buffer Hunt Error"},     /* バッファエラー */
 {"Install File Not Found"},      /* 更新ファイルなし */
 {"Install File CheckSum Error"}, /* 更新ファイルチェックサムエラー */
 {"Install File Invalid"},        /* 更新ファイル無効 */
 {"Input Parameter Error"}        /* パラメータエラー */
};

const	char			f_fdl_type_nametbl[D_SYS_NUM3][D_SYS_NUM10] =
{
	{"37G-LLSDU"	},										/* 37G(SUB6)装置名			*/
	{"45G-LLSDU"	},										/* 45G(SUB6)装置名			*/
	{"28G-LLSDU"	}										/* 28G(mmW)装置名			*/
};

const	T_FDL_EEP_PNT	f_fdl_eepwrit_point[E_FDL_FLSH_FILEMAX] =
{
	{ D_DU_EEP_SOFT_FILE_SIZE_0		, D_DU_EEP_SOFT_FILE_SIZE_1		, D_DU_EEP_SOFT_VER0	, D_DU_EEP_SOFT_VER1	,"SW_VER"			},
	{ D_DU_EEP_PLFPGA_FILE_SIZE_0	, D_DU_EEP_PLFPGA_FILE_SIZE_1	, D_DU_EEP_PLFPGA_VER0	, D_DU_EEP_PLFPGA_VER1	, "PLFPGA_VER"		},
	{ D_DU_EEP_TRXFPGA_FILE_SIZE_0	, D_DU_EEP_TRXFPGA_FILE_SIZE_1	, D_DU_EEP_TRXFPGA_VER0	, D_DU_EEP_TRXFPGA_VER1	, "TRXFPGA_VER"		},
	{ D_DU_EEP_RFICBOT_FILE_SIZE_0	, D_DU_EEP_RFICBOT_FILE_SIZE_1	, D_DU_EEP_RFICBOT_VER0	, D_DU_EEP_RFICBOT_VER1	, "RFIC_BOOT_VER"	},
	{ D_DU_EEP_RFICARM_FILE_SIZE_0	, D_DU_EEP_RFICARM_FILE_SIZE_1	, D_DU_EEP_RFICARM_VER0	, D_DU_EEP_RFICARM_VER1	, "RFIC_ARM_VER"	},
	{ D_DU_EEP_PTPCOND_FILE_SIZE_0	, D_DU_EEP_PTPCOND_FILE_SIZE_1	, D_DU_EEP_PTPCOND_VER0	, D_DU_EEP_PTPCOND_VER1	, "PTPCNF_VER"		}
};
	
