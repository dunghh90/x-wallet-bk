/*!
 * @skip		$ld:$
 * @file		cmd_dbg_rom.c
 * @brief		DEBUGコマンド　ROM定義
 * @author		alpha)宮崎
 * @date		2013/11/26 alpha)宮崎 Create
 * @date		2014/12/15 alpha)桑村 docomo SLC/1CHIP対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2014
 */
#include	"f_dbg_cmd_inc.h"

/** @addtogroup RRH_DEBUG
 *  @{
 */

/**
 *
 *
 */
T_DBG_CMD_DEF_TBL	DebugCommandDef[]	=	{
/* ローカル実行コマンド */
	{ "cmd_eeprom_access",  	"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"cmd_eeprom_access_main"	}, /* EEPROMアクセス	*/
	{ "cmd_eeprom_disp",		"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"cmd_eeprom_disp_main"		},
	{ "cmd_alm_hist",			"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"cmd_alm_hist_log_main"		},
	{ "cmd_rrhdbg",				"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"cmd_rrhdbg_main"			},
	{ "cmd_reg_access",			"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"cmd_reg_access_main"		},
	{ "cmd_lm_write",			"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"cmd_lm_write_main"			},
	{ "cmd_spi_access",			"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"cmd_spi_access_main"		},
	{ "cmd_mmi_cpld_access",	"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"cmd_mmi_cpld_access_main"	},
	{ "cmd_version"	,			"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"cmd_version_main"			},
	{ "cmd_shm_dmp"	,			"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"cmd_shm_dmp_main"			},
	{ "cmd_shm_write"	,		"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"cmd_shm_write_main"		},
	{ "trar"		,			"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"trar_main"					},
	{ "traw"		,			"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"traw_main"					},
	{ "pm"			,			"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"pm_main"					},
	{ "sm"			,			"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"sm_main"					},
	{ "cmd_shm_getmon",			"",		"",		"",		"",		"",		0,		0,		"", 	"libDBGcmd.so",		"cmd_shm_getmon"			},
	{ "cmd_trouble_log_get",	"",		"",		"",		"",		"",		0,		0,		"", 	"libDBGcmd.so",		"cmd_trouble_log_get_main"	},
	{ "cmd_flash_read",			"",		"",		"",		"",		"",		0,		0,		"", 	"libDBGcmd.so",		"cmd_flash_read_main"		},
	{ "cmd_flash_slave_read",	"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"cmd_flash_slave_read_main"	}, 
	{ "resetmask",				"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"resetmask_main"			},
	{ "cmd_sfp_info",			"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"cmd_sfp_info_main"			},
	{ "cmd_sfp_log",			"",		"",		"",		"",		"",		0,		0,		"",		"libDBGcmd.so",		"cmd_sfp_log_disp_main"		},
	/* 各機能実行コマンド	*/
	{ "cmd_pfdbg",		"cca",	"dsp",		"almlog",	"",		"",		D_RRH_PROCQUE_PF,	D_SYS_THDQID_PF_DBG,	"cca", 	"",		 ""			}, /* 1秒周期ログの確認 */
	{ "cmd_pfdbg",		"cca",	"dsp",		"almall",	"",		"",		D_RRH_PROCQUE_PF,	D_SYS_THDQID_PF_DBG,	"cca", 	"",		 ""			}, /* 1秒周期ログの確認 */
	{ "cmd_pfdbg",		"cca",	"dsp",		"almlogall","",		"",		D_RRH_PROCQUE_PF,	D_SYS_THDQID_PF_DBG,	"cca", 	"",		 ""			}, /* 1秒周期ログの確認 */
	{ "cmd_pfdbg",		"cca",	"dsp",		"traall",	"",		"",		D_RRH_PROCQUE_PF,	D_SYS_THDQID_PF_DBG,	"cca", 	"",		 ""			}, /*  */
	{ "cmd_pfdbg",		"cca",	"dsp",		"car",		"",		"",		D_RRH_PROCQUE_PF,	D_SYS_THDQID_PF_DBG,	"cca", 	"",		 ""			}, /* キャリア状態確認 */
	{ "cmd_pfdbg",		"cca",	"dsp",		"temper",	"",		"",		D_RRH_PROCQUE_PF,	D_SYS_THDQID_PF_DBG,	"cca", 	"",		 ""			}, /* 温度の推移を表示する（Default：1分毎の情報を24時間分） */
	{ "cmd_pfdbg",		"cca",	"set",		"tra",		"",		"",		D_RRH_PROCQUE_PF,	D_SYS_THDQID_PF_DBG,	"cca", 	"",		 ""			}, /* トレーニングデータ設定(sub6/mmW共通) */
	{ "cmd_pfdbg",		"cca",	"set",		"trammw",	"",		"",		D_RRH_PROCQUE_PF,	D_SYS_THDQID_PF_DBG,	"cca", 	"",		 ""			}, /* トレーニングデータ設定(mmW用) */
	{ "cmd_pfdbg",		"cca",	"set",		"reg",		"",		"",		D_RRH_PROCQUE_PF,	D_SYS_THDQID_PF_DBG,	"cca", 	"",		 ""			}, /*  */
	{ "cmd_pfdbg",		"cca",	"set",		"vreg",		"",		"",		D_RRH_PROCQUE_PF,	D_SYS_THDQID_PF_DBG,	"cca", 	"",		 ""			}, /* 擬似レジスタ設定 */
	{ "cmd_pfdbg",		"cca",	"set",		"almth",	"",		"",		D_RRH_PROCQUE_PF,	D_SYS_THDQID_PF_DBG,	"cca", 	"",		 ""			}, /*  */
	{ "cmd_pfdbg",		"cca",	"set",		"1sec",		"",		"",		D_RRH_PROCQUE_PF,	D_SYS_THDQID_PF_DBG,	"cca", 	"",		 ""			}, /*  */
	{ "cmd_pfdbg",		"cca",	"set",		"temper",	"tim",	"",		D_RRH_PROCQUE_PF,	D_SYS_THDQID_PF_DBG,	"cca", 	"",		 ""			}, /* 温度集計のタイマ値を変更する */
	{ 0,				0,		0,			0,			0,		0,		0,					0,						0,		0,					0										} /* terminator */

};

/* @} */

