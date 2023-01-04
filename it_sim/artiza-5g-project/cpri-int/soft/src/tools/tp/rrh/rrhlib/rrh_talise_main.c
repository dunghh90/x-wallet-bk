
/************************************************************************************************/
/*
 * rrh_marconi_main.c
 *
 * marconi用 TPコマンド処理関数を定義
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */
/************************************************************************************************/

//#include <common.h>
#include "rrhlib_local.h"
//#include <stdint.h>
//#include <stddef.h>
#include <RFIC_TALISE.h>
#include "talise.h"
//#include <talise_rrh_extern.h>
#include <talise_config.h>
#include "f_rrh_reg_eep.h"

typedef unsigned int uint;

void talise_config_relink(void) {
//	talise_config_p->talDevHalInfo.spiSettings = &(talise_config_p->spiDev1);
//	talise_config_p->talDevice.devHalInfo = &(talise_config_p->talDevHalInfo);					/* デバイスの多重登録対応 */
	talise_config_p->talDevHalInfo[0].spiSettings = &(talise_config_p->spiDev1[0]);
	talise_config_p->talDevice[0].devHalInfo = &(talise_config_p->talDevHalInfo[0]);
	talise_config_p->talDevHalInfo[1].spiSettings = &(talise_config_p->spiDev1[1]);
	talise_config_p->talDevice[1].devHalInfo = &(talise_config_p->talDevHalInfo[1]);
	talise_config_p->talDevHalInfo[2].spiSettings = &(talise_config_p->spiDev1[2]);
	talise_config_p->talDevice[2].devHalInfo = &(talise_config_p->talDevHalInfo[2]);
	talise_config_p->talDevHalInfo[3].spiSettings = &(talise_config_p->spiDev1[3]);
	talise_config_p->talDevice[3].devHalInfo = &(talise_config_p->talDevHalInfo[3]);
	talise_config_p->talInit[0].rx.rxProfile.rxFir.coefs = &(talise_config_p->rxFirCoefs[0]);
	talise_config_p->talInit[0].tx.txProfile.txFir.coefs = &(talise_config_p->txFirCoefs[0]);
	talise_config_p->talInit[0].obsRx.orxProfile.rxFir.coefs = &(talise_config_p->obsrxFirCoefs[0]);
	talise_config_p->talInit[1].rx.rxProfile.rxFir.coefs = &(talise_config_p->rxFirCoefs[0]);
	talise_config_p->talInit[1].tx.txProfile.txFir.coefs = &(talise_config_p->txFirCoefs[0]);
	talise_config_p->talInit[1].obsRx.orxProfile.rxFir.coefs = &(talise_config_p->obsrxFirCoefs[0]);
	talise_config_p->talInit[2].rx.rxProfile.rxFir.coefs = &(talise_config_p->rxFirCoefs[0]);
	talise_config_p->talInit[2].tx.txProfile.txFir.coefs = &(talise_config_p->txFirCoefs[0]);
	talise_config_p->talInit[2].obsRx.orxProfile.rxFir.coefs = &(talise_config_p->obsrxFirCoefs[0]);
	talise_config_p->talInit[3].rx.rxProfile.rxFir.coefs = &(talise_config_p->rxFirCoefs[0]);
	talise_config_p->talInit[3].tx.txProfile.txFir.coefs = &(talise_config_p->txFirCoefs[0]);
	talise_config_p->talInit[3].obsRx.orxProfile.rxFir.coefs = &(talise_config_p->obsrxFirCoefs[0]);
	talise_config_p->talInit_p = &(talise_config_p->talInit[0]);
	talise_config_p->talTxAttSndData_p[0] = &(talise_config_p->talTxAttSndData[0]);
	talise_config_p->talTxAttSndData_p[1] = &(talise_config_p->talTxAttSndData[1]);

	return ;
}


/************************************************************************************************/
/*
 * Function		：	Talise用TPコマンド処理メイン
 * Symbol		：	talise_cmd_main
 *
 * Parameter	：	[I]int  argc      : 入力パラメータ数
 *                  [I]char * constargv[]   : 入力パラメータ
 *
 * Return		：	0:処理成功
 *              ：  1:処理異常
 *
 * Date			：	2015/11/30：初版
 *
 * Author		：	MTC)岡部(talise用に修正(土屋))
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */
/************************************************************************************************/
int talise_cmd_main(int argc, char * const argv[])
{
	uint offset = 3;// 第2引数にUNIT番号(all、0～3)が入るので、3個目から引数取得
	uint i;
	uint tmp[7];
	uint tmp10[7];
//    uint32_t countARM=114688;
//    uint8_t binaryARM[countARM];
//    uint32_t countboot=4096;
//    uint8_t binaryboot[countboot];
	int err_code = 0;
    unsigned char status = 0;
    unsigned char unit = 0;
	UINT					*shm_rrhKind;
	int						endcd;		/* 終了コード                   */
    int						errcd;		/* 詳細NGコード                 */
	
	/* 共有メモリの取得 */
    endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
            E_RRH_SHMID_APL_TALISE_CONF_INFO,   /* 共有メモリ番号               */
            (VOID **)&talise_config_p,          /* 共有メモリアドレス           */
            &errcd                              /* 詳細NGコード                 */
    );
    /* 共有メモリの取得失敗時 */
    if( endcd != BPF_RU_IPCM_OK )
    {
    	printf("Talise Get Memory NG:%x ERRCD:%x\n",endcd,errcd);
    	return 1;
    }

	/* 共有メモリの取得(装置種別) */
	endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
	        E_RRH_SHMID_APL_RRH_KIND,           /* 共有メモリ番号               */
	        (void**)&shm_rrhKind,               /* 共有メモリアドレス           */
	        &errcd                              /* 詳細NGコード                 */
	);
    /* 共有メモリの取得失敗時 */
	if( endcd != BPF_RU_IPCM_OK )
	{
		printf("Talise Get Memory NG(SHM_KIND):%x ERRCD:%x\n",endcd,errcd);
		return 1;
	}

	/* 装置種別がmmwなら */
	if( M_DU_RRHKIND_KIND_GET(*shm_rrhKind) == D_RRH_EEP_DEV_KIND_5GDU_MMW )
	{
		printf("Talise Device non Mount\n");
		return 1;
	}

	/* 共有メモリ内のリンク再設定 */
	talise_config_relink();

    /* 共有メモリ初期化コマンド */
	if (strcmp(argv[1], "-talise_ConfigInit") == 0) {
    	talise_config_init();
		printf("Talise Config Init Complete\n");
		return 0;
    }

	if (strcmp(argv[1], "-talise_print") == 0) {
    	offset = 2;		// 第2引数にUNIT番号が入らないコマンドは2個目から引数取得
    }
	for(i=0 ; i<7 ; i++){
		if((int)i < (int)(argc - offset) ){
			tmp[i] = (uint)strtoul (argv[offset +i], NULL, 16);
			tmp10[i] = (uint)strtoul (argv[offset +i], NULL, 10);
		}else{
			tmp[i] = 0;
			tmp10[i] = 0;
		}
	}

    if (strcmp(argv[1], "-talise_print") == 0) {
    	// 第2引数にUNIT番号が入らないコマンドはUNIT番号の処理スキップ
    } else
	{
		if (argc < 3) {		// UNIT番号まで入っているかチェック
	        printf ("format :  rrh -talise_XXX (all or 0-1) ...   \n");
	        return 1;
		}
		argc--;	// パラメータ数チェック対策で第2引数分コマンド数を引いておく
		unit = (uint8_t)(strtoul (argv[2], NULL, 16));//UNIT番号を抽出しておく

		/* UNIT番号をSPIのCSに設定 */
	    if (strcmp(argv[2], "all") == 0) {
			((zynqAdiDev_t*)((talise_config_p->talDevice[0]).devHalInfo))->spiSettings->chipSelectIndex =
	    		BPF_HM_DEVC_RFIC_UNIT_RFICALL;
	    	(talise_config_p->talInit_p) = 0;
	    	unit = BPF_HM_DEVC_RFIC_UNIT_RFICALL;
	    	talise_config_p->unit = 0 ;
	    }else
		if (unit < BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX) {
			((zynqAdiDev_t*)((talise_config_p->talDevice[unit]).devHalInfo))->spiSettings->chipSelectIndex =
				unit;
			talise_config_p->unit = unit ;
	    	(talise_config_p->talInit_p) = &(talise_config_p->talInit[unit]);
		}else
		{
	        printf ("format :  rrh -talise_XXX (all or 0-1) ...   \n");
	        return 1;
		}
	}

    if (strcmp(argv[1], "-talise_setparam_1") == 0) {
        if (argc != 7) {
            printf ("format :  rrh -talise_setparam_1 (rx1GainIndex) (rx2GainIndex) (rxChannels) (txChannels) (framerSel) \n");
            return 1;
        }
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
				err_code = rrh_cmd_talise_setparam_1( argc, &argv[1]);	// 第2引数分ポインタをずらして渡す
    		}
    	} else
    	{
			err_code = rrh_cmd_talise_setparam_1( argc, &argv[1]);	// 第2引数分ポインタをずらして渡す
    	}
    }else
    if (strcmp(argv[1], "-talise_getparam_1") == 0) {
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_getparam_1();
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_getparam_1();
    	}
    }else
    if (strcmp(argv[1], "-talise_getparam_fir") == 0) {
        if (argc != 3) {
            printf("format :  rrh -talise_getparam_fir  (0:tx/1:rx/2:orx)   : talise parameter get\n");
            return 1;
        }
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_getparam_fir(tmp[0]);
    		}
    	} else
    	{
        	err_code = rrh_cmd_talise_getparam_fir(tmp[0]);
    	}
    }else
    if (strcmp(argv[1], "-talise_setparam_jesd") == 0) {
        if (argc != 13) {
            printf ("format :  rrh -talise_setparam_jesd   (framerA.serializerLanesEnabled) (framerA.lmfcOffset)\n");
            printf ("                                 (framerB.serializerLanesEnabled) (framerB.lmfcOffset)\n");
            printf ("                                 (deframerA.deserializerLanesEnabled) (deframerA.lmfcOffset)\n");
            printf ("                                 (deframerB.deserializerLanesEnabled) (deframerB.lmfcOffset)\n");
            printf ("                                 (serPreEmphasis) (desEqSetting) (serAmplitude) \n");
            return 1;
        }
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
        		err_code = rrh_cmd_talise_setparam_jesd( argc, &argv[1]);	// 第2引数分ポインタをずらして渡す
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_setparam_jesd( argc, &argv[1]);	// 第2引数分ポインタをずらして渡す
    	}
    }else
    if (strcmp(argv[1], "-talise_getparam_jesd") == 0) {
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_getparam_jesd();
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_getparam_jesd();
    	}
    }else
    if (strcmp(argv[1], "-talise_setparam_frame") == 0) {
        if (argc != 8) {
            printf ("format :  rrh -talise_setparam_frame  (framerA.serializerLanesEnabled) (A.serializerLaneCrossbar) (A.enableManualLaneXbar)\n");
            printf ("                                 (framerB.serializerLanesEnabled) (B.serializerLaneCrossbar) (B.enableManualLaneXbar)\n");
            printf ("                                 [hex]\n");
            return 1;
        }
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_setparam_frame( argc, &argv[1]);	// 第2引数分ポインタをずらして渡す
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_setparam_frame( argc, &argv[1]);	// 第2引数分ポインタをずらして渡す
    	}
    }else
    if (strcmp(argv[1], "-talise_getparam_frame") == 0) {
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_getparam_frame();
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_getparam_frame();
    	}
    }else
    if (strcmp(argv[1], "-talise_setparam_other") == 0) {
        if (argc != 5) {
            printf ("format :  rrh -talise_setparam_other  (orx1gain) (orx2gain) (freq_khz)\n");
            return 1;
        }
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_setparam_other( tmp10[0], tmp10[1], tmp10[2]);
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_setparam_other( tmp10[0], tmp10[1], tmp10[2]);
    	}
    }else
    if (strcmp(argv[1], "-talise_getparam_other") == 0) {
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_getparam_other();
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_getparam_other();
    	}
    }else
    if (strcmp(argv[1], "-talise_setparam_txatt") == 0) {
        if (argc != 4) {
            printf ("format :  rrh -talise_setparam_txatt  (dac1) (dac2)\n");
            return 1;
        }
    	if (unit == BPF_HM_DEVC_RFIC_UNIT_RFICALL) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
	    		(talise_config_p->talTxAttSndData_p[0]) = &(talise_config_p->talTxAttSndData[(i * 2)    ]);
    			(talise_config_p->talTxAttSndData_p[1]) = &(talise_config_p->talTxAttSndData[(i * 2) + 1]);
		        err_code = rrh_cmd_talise_setparam_txatt( tmp[0], tmp[1]);
    		}
    	} else
    	{
    		(talise_config_p->talTxAttSndData_p[0]) = &(talise_config_p->talTxAttSndData[(unit * 2)    ]);
    		(talise_config_p->talTxAttSndData_p[1]) = &(talise_config_p->talTxAttSndData[(unit * 2) + 1]);
	        err_code = rrh_cmd_talise_setparam_txatt( tmp[0], tmp[1]);
    	}
    }else
    if (strcmp(argv[1], "-talise_getparam_txatt") == 0) {
    	if (unit == BPF_HM_DEVC_RFIC_UNIT_RFICALL) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
	    		(talise_config_p->talTxAttSndData_p[0]) = &(talise_config_p->talTxAttSndData[(i * 2)    ]);
    			(talise_config_p->talTxAttSndData_p[1]) = &(talise_config_p->talTxAttSndData[(i * 2) + 1]);
		        err_code = rrh_cmd_talise_getparam_txatt();
    		}
    	} else
    	{
    		(talise_config_p->talTxAttSndData_p[0]) = &(talise_config_p->talTxAttSndData[(unit * 2)    ]);
    		(talise_config_p->talTxAttSndData_p[1]) = &(talise_config_p->talTxAttSndData[(unit * 2) + 1]);
	        err_code = rrh_cmd_talise_getparam_txatt();
    	}
    }else
    if (strcmp(argv[1], "-talise_openHW") == 0) {
        err_code = rrh_cmd_talise_openHW();
    }else
    if (strcmp(argv[1], "-talise_resetDevice") == 0) {
        err_code = rrh_cmd_talise_resetDevice();
    }else
    if (strcmp(argv[1], "-talise_initialize") == 0) {
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_initialize();
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_initialize();
    	}
    }else
    if (strcmp(argv[1], "-talise_getPllsLockStatus") == 0) {
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_getPllsLockStatus(&status);
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_getPllsLockStatus(&status);
    	}
    }else
    if (strcmp(argv[1], "-talise_enableMultichipSync") == 0) {
        if (argc != 3) {
            printf ("format :  rrh -talise_enableMultichipSync (0 or 1)      \n");
            return 1;
        }
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_enableMultichipSync( tmp[0]);
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_enableMultichipSync( tmp[0]);
    	}
    }else
    if (strcmp(argv[1], "-talise_initArm") == 0) {
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_initArm();
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_initArm();
    	}
    }else
    if (strcmp(argv[1], "-talise_loadStreamFromBinary") == 0) {
//        err_code = rrh_cmd_talise_loadStreamFromBinary();
        err_code = rrh_cmd_talise_loadStream();
    }else
    if (strcmp(argv[1], "-talise_loadArmFromBinary") == 0) {
//        err_code = rrh_cmd_talise_loadArmFromBinary();
        err_code = rrh_cmd_talise_loadArm();
    }else
    if (strcmp(argv[1], "-talise_verifyArmChecksum") == 0) {
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_verifyArmChecksum();
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_verifyArmChecksum();
    	}
    }else
    if (strcmp(argv[1], "-talise_setRfPllFrequency") == 0) {
        if (argc != 4) {
            printf ("format :  rrh -talise_setRfPllFrequency (0:CLK/1:RF/2AUX) (周波数[kHz])      \n");
            return 1;
        }
        err_code = rrh_cmd_talise_setRfPllFrequency( tmp10[0],tmp10[1]);
    }else
    if (strcmp(argv[1], "-talise_runInitCals") == 0) {
        if (argc != 3) {
            printf ("format :  rrh -talise_runInitCals (cal_mask(16進))      \n");
            return 1;
        }
        err_code = rrh_cmd_talise_runInitCals(tmp[0]);
    }else
    if (strcmp(argv[1], "-talise_waitInitCals") == 0) {
        if (argc == 2) {
            tmp[0] = 2000;
        }else if (argc != 3) {
            printf ("format :  rrh -talise_waitInitCals (wait)       \n");
            return 1;
        }
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
	            err_code = rrh_cmd_talise_waitInitCals( tmp[0], &status);
    		}
    	} else
    	{
            err_code = rrh_cmd_talise_waitInitCals( tmp[0], &status);
    	}
    }else
    if (strcmp(argv[1], "-talise_enableFramerLink") == 0) {
        if (argc != 4) {
            printf ("format :  rrh -talise_enableFramerLink (0:A/1:B/2:A_B) (0 or 1)      \n");
            return 1;
        }
        err_code = rrh_cmd_talise_enableFramerLink( tmp[0], tmp[1]);
    }else
    if (strcmp(argv[1], "-talise_enableSysrefToFramer") == 0) {
        if (argc != 4) {
            printf ("format :  rrh -talise_enableSysrefToFramer (0:A/1:B/2:A_B) (0 or 1)      \n");
            return 1;
        }
        err_code = rrh_cmd_talise_enableSysrefToFramer( tmp[0], tmp[1]);
    }else
    if (strcmp(argv[1], "-talise_enableDeframerLink") == 0) {
        if (argc != 4) {
            printf ("format :  rrh -talise_enableDeframerLink (0:A/1:B/2:A_B) (0 or 1)      \n");
            return 1;
        }
        err_code = rrh_cmd_talise_enableDeframerLink( tmp[0], tmp[1]);
    }else
    if (strcmp(argv[1], "-talise_enableSysrefToDeframer") == 0) {
        if (argc != 4) {
            printf ("format :  rrh -talise_enableSysrefToDeframer (0:A/1:B/2:A_B) (0 or 1)      \n");
            return 1;
        }
        err_code = rrh_cmd_talise_enableSysrefToDeframer( tmp[0], tmp[1]);
    }else
    if (strcmp(argv[1], "-talise_readDeframerStatus") == 0) {
        if (argc != 3) {
            printf ("format :  rrh -talise_readDeframerStatus (0:A/1:B/2:A_B)      \n");
            return 1;
        }
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_readDeframerStatus( tmp[0]);
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_readDeframerStatus( tmp[0]);
    	}
    }else
    if (strcmp(argv[1], "-talise_readFramerStatus") == 0) {
        if (argc != 3) {
            printf ("format :  rrh -talise_readFramerStatus (0:A/1:B/2:A_B)      \n");
            return 1;
        }
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_readFramerStatus(  tmp[0]);
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_readFramerStatus(  tmp[0]);
    	}
    }else
    if (strcmp(argv[1], "-talise_enableTrackingCals") == 0) {
        if (argc != 3) {
            printf ("format :  rrh -talise_enableTrackingCals (cal_mask(hex))       \n");
            return 1;
        }
        err_code = rrh_cmd_talise_enableTrackingCals( tmp[0]);
    }else
    if (strcmp(argv[1], "-talise_radioOn") == 0) {
        err_code = rrh_cmd_talise_radioOn();
    }else
    if (strcmp(argv[1], "-talise_radioOff") == 0) {
        err_code = rrh_cmd_talise_radioOff();
    }else
    if (strcmp(argv[1], "-talise_shutdown") == 0) {
        err_code = rrh_cmd_talise_shutdown();
    }else
    if (strcmp(argv[1], "-talise_closeHw") == 0) {
        err_code = rrh_cmd_talise_closeHw();
    }else
    if (strcmp(argv[1], "-talise_setupAuxDacs") == 0) {
        err_code = rrh_cmd_talise_setupAuxDacs();
    }else
    if (strcmp(argv[1], "-talise_writeAuxDac") == 0) {
        if (argc != 4) {
            printf ("format :  rrh -talise_writeAuxDac (index) (code(hex))      \n");
            return 1;
        }
        err_code = rrh_cmd_talise_writeAuxDac( tmp10[0], tmp[1]);
    }else
    if (strcmp(argv[1], "-talise_setTxAttenuation") == 0) {
        if (argc != 4) {
            printf ("format :  rrh -talise_setTxAttenuation  (TAL_TX) (mdB)     \n");
            return 1;
        }
        err_code = rrh_cmd_talise_setTxAttenuation( tmp[0] , tmp10[1]);
    }else
    if (strcmp(argv[1], "-talise_getTxAttenuation") == 0) {
        if (argc != 3) {
            printf ("format :  rrh -talise_getTxAttenuation  (TAL_TX)      \n");
            return 1;
        }
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_getTxAttenuation( tmp[0]);
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_getTxAttenuation( tmp[0]);
    	}
    }else
    if (strcmp(argv[1], "-talise_getTemperature") == 0) {
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_getTemperature();
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_getTemperature();
    	}
    }else
    if (strcmp(argv[1], "-talise_setTxToOrxMapping") == 0) {
        if (argc != 5) {
            printf ("format :  rrh -talise_setTxToOrxMapping  (EN(0 or 1)) (oRx(0:none/2:Tx1/3:Tx2)) (oRx(0:none/2:Tx1/3:Tx2))\n");
            return 1;
        }
        err_code = rrh_cmd_talise_setTxToOrxMapping( tmp[0], tmp[1], tmp[2]);
    }else
    if (strcmp(argv[1], "-talise_setArmGpioPins") == 0) {
//        if (argc != 3) {
//            printf ("書式 rrh -talise_setArmGpioPins  \n");
//            return 1;
//        }
        err_code = rrh_cmd_talise_setArmGpioPins();
    }else
    if (strcmp(argv[1], "-talise_setRadioCtlPinMode") == 0) {
        if (argc != 4) {
            printf ("format :  rrh -talise_setRadioCtlPinMode (pinOptionsMask(hex)) (orxEnGpioPinSel(0-3)\n");
            return 1;
        }
        err_code = rrh_cmd_talise_setRadioCtlPinMode( tmp[0] , tmp[1]);
    }else
    if (strcmp(argv[1], "-talise_setRxTxEnable") == 0) {
        if (argc != 4) {
            printf ("format :  rrh -talise_setRxTxEnable (rxOrxChannel(0:RXoff/1:RX1/2:RX2/3RX12/4:ORX1/8:ORX2/C:ORX12) (txChannel(0:off/1:1/2:2/3:1and2)\n");
            return 1;
        }
        err_code = rrh_cmd_talise_setRxTxEnable( tmp[0] , tmp[1]);
    }else
    if (strcmp(argv[1], "-talise_getRxTxEnable") == 0) {
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_getRxTxEnable();
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_getRxTxEnable();
    	}
    }else
    if (strcmp(argv[1], "-talise_getRadioState") == 0) {
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_getRadioState();
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_getRadioState();
    	}
    }else
    if (strcmp(argv[1], "-talise_getInitCalStatus") == 0) {
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_getInitCalStatus();
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_getInitCalStatus();
    	}
    }else
    if (strcmp(argv[1], "-talise_checkInitCalComplete") == 0) {
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_checkInitCalComplete();
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_checkInitCalComplete();
    	}
    }else
    if (strcmp(argv[1], "-talise_setRxManualGain") == 0) {
        if (argc != 4) {
            printf ("format :  rrh -talise_setRxManualGain (0:off/1:rx1/2:rx2/3:rx12) (gainIndex)     \n");
            return 1;
        }
        err_code = rrh_cmd_talise_setRxManualGain( tmp[0] , tmp10[1]);
    }else
    if (strcmp(argv[1], "-talise_setObsRxManualGain") == 0) {
        if (argc != 4) {
            printf ("format :  rrh -talise_setObsRxManualGain (0:off/1:rx1/2:rx2/3:rx12) (gainIndex)     \n");
            return 1;
        }
        err_code = rrh_cmd_talise_setObsRxManualGain( tmp[0] , tmp10[1]);
    }else
    if (strcmp(argv[1], "-talise_enableFramerTestData") == 0) {
        if (argc != 5) {
            printf ("format :  rrh -talise_enableFramerTestData (0:A/1:B/2:AB) (DataSource(0-15)) (TestData(0-2))\n");
            return 1;
        }
        err_code = rrh_cmd_talise_enableFramerTestData( tmp10[0] , tmp10[1], tmp10[2]);
    }else
    if (strcmp(argv[1], "-talise_setupAdcSampleXbar") == 0) {
        if (argc != 11) {
            printf ("format :  rrh -talise_setupAdcSampleXbar (0:A/1:B/2:AB) (conv0) … (conv7)      \n");
            return 1;
        }
        rrh_cmd_talise_setupAdcSampleXbar( tmp[0], &tmp[1]);
        err_code = 0;
    }else
    if (strcmp(argv[1], "-talise_getErrCode") == 0) {
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_getErrCode();
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_getErrCode();
    	}
    }else
    if (strcmp(argv[1], "-talise_rescheduleTrackingCal") == 0) {
        if (argc != 3) {
            printf ("format :  rrh -talise_rescheduleTrackingCal (trackingCal)  \n");
            printf ("      trackingCal                                     \n");
            printf ("        TAL_TRACK_NONE              = 0x00000000    not use \n");
            printf ("        TAL_TRACK_RX1_QEC           = 0x00000001      \n");
            printf ("        TAL_TRACK_RX2_QEC           = 0x00000002      \n");
            printf ("        TAL_TRACK_ORX1_QEC          = 0x00000004      \n");
            printf ("        TAL_TRACK_ORX2_QEC          = 0x00000008      \n");
            printf ("        TAL_TRACK_TX1_LOL           = 0x00000010      \n");
            printf ("        TAL_TRACK_TX2_LOL           = 0x00000020      \n");
            printf ("        TAL_TRACK_TX1_QEC           = 0x00000040      \n");
            printf ("        TAL_TRACK_TX2_QEC           = 0x00000080      \n");
            printf ("        TAL_TRACK_RX1_HD2           = 0x00000100    not use \n");
            printf ("        TAL_TRACK_RX2_HD2           = 0x00000200    not use \n");
            printf ("        TAL_TRACK_ALL               = 0x000003FF    not use \n");
            return 1;
        }
        err_code = rrh_cmd_talise_rescheduleTrackingCal( tmp[0]);
    }else
    if (strcmp(argv[1], "-talise_enableDeframerPrbsChecker") == 0) {
        if (argc != 4) {
            printf ("format :  rrh -talise_enableDeframerPrbsChecker (polyOrder) (checkerLocation)     \n");
            printf ("        (polyOrder)                0:Disable/1:PRBS7/2:PRBS15/3:PRBS31     \n");
            printf ("        (checkerLocation)          0:LANE_DATA/1:SAMPLE_DATA \n");
            return 1;
        }
        err_code = rrh_cmd_talise_enableDeframerPrbsChecker( tmp[0] , tmp[1]);
    }else
    if (strcmp(argv[1], "-talise_readDeframerPrbsCounters") == 0) {
        if (argc != 3) {
            printf ("format :  rrh -talise_readDeframerPrbsCounters  (lane(0-3))      \n");
            return 1;
        }
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_readDeframerPrbsCounters( tmp[0]);
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_readDeframerPrbsCounters( tmp[0]);
    	}
    }else
    if (strcmp(argv[1], "-talise_setRfPllLoopFilter") == 0) {
        if (argc != 4) {
            printf ("format :  rrh -talise_setRfPllLoopFilter  (loopBandwidth_kHz(50-750)) (stability(3-15)     \n");
            return 1;
        }
        err_code = rrh_cmd_talise_setRfPllLoopFilter( tmp10[0] , tmp10[1]);
    }else
    if (strcmp(argv[1], "-talise_getRfPllLoopFilter") == 0) {
    	if ((talise_config_p->talInit_p) == 0) {
    		for (i=BPF_HM_DEVC_RFIC_UNIT_RFIC1; i<BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX; i++) {
				((zynqAdiDev_t*)((talise_config_p->talDevice[i]).devHalInfo))->spiSettings->chipSelectIndex = i;
				(talise_config_p->talInit_p) = &(talise_config_p->talInit[i]);
				talise_config_p->unit = i;
		        err_code = rrh_cmd_talise_getRfPllLoopFilter();
    		}
    	} else
    	{
	        err_code = rrh_cmd_talise_getRfPllLoopFilter();
    	}
    }else
    if (strcmp(argv[1], "-talise_boot") == 0) {
        if (argc != 4) {
            printf ("format :  rrh -talise_boot (StartBlock) (StopBlock)         \n");
            printf ("  Block番号  (StartBlock)から(StopBlock)まで処理を行う \n");
            printf ("    1:FPGA初期設定                                     \n");
            printf ("    2:RFIC初期設定                                     \n");
            printf ("    3:PLL-IC SYSREF発行(MCS)                           \n");
            printf ("    4:RFIC ARM起動                                     \n");
            printf ("    5:TX ATT設定 RFIC CAL実行                          \n");
            printf ("    6:RFICのJSFW,JSFB,JSRX設定                         \n");
            printf ("    7:PLL-IC SYSREF発行(JESD) JSFW,JSFB,JSRXの状態確認 \n");
            printf ("    8:送受信設定                                       \n");
            return 1;
        }
    	if ((tmp[0] <= 1) && (1 <= tmp[1])) {
			err_code = RFIC_TAL_FPGA_INITIALIZE(unit) ;
    		if (err_code != RFIC_TALISE_COMPLETE) {
				printf("FPGA INITIALIZE Error err_code : %08x\n",err_code);
				return 1;
			}
    	}
    	if ((tmp[0] <= 2) && (2 <= tmp[1])) {
			err_code = RFIC_TAL_RFIC_INITIALIZE(unit) ;
    		if (err_code != RFIC_TALISE_COMPLETE) {
				printf("RFIC INITIALIZE Error err_code : %08x\n",err_code);
				return 1;
			}
    	}
    	if ((tmp[0] <= 3) && (3 <= tmp[1])) {
			err_code = RFIC_TAL_PLLIC_MCS(unit) ;
    		if (err_code != RFIC_TALISE_COMPLETE) {
				printf("SET SYSREF(MCS) Error err_code : %08x\n",err_code);
				return 1;
			}
    	}
    	if ((tmp[0] <= 4) && (4 <= tmp[1])) {
			err_code = RFIC_TAL_ARM_START(unit) ;
    		if (err_code != RFIC_TALISE_COMPLETE) {
				printf("ARM START Error err_code : %08x\n",err_code);
				return 1;
			}
    	}
    	if ((tmp[0] <= 5) && (5 <= tmp[1])) {
			err_code = RFIC_TAL_RFIC_CAL(unit) ;
    		if (err_code != RFIC_TALISE_COMPLETE) {
				printf("RFIC CAL Error err_code : %08x.unit:%d\n",err_code,unit);
				return 1;
			}
    	}
    	if ((tmp[0] <= 6) && (6 <= tmp[1])) {
			err_code = RFIC_TAL_JESD_SET(unit) ;
    		if (err_code != RFIC_TALISE_COMPLETE) {
				printf("SET JESD Error err_code : %08x\n",err_code);
				return 1;
			}
    	}
    	if ((tmp[0] <= 7) && (7 <= tmp[1])) {
			err_code = RFIC_TAL_PLLIC_JESD(unit) ;
    		if (err_code != RFIC_TALISE_COMPLETE) {
				printf("SET SYSREF(JESD) Error err_code : %08x\n",err_code);
				return 1;
			}
    	}
    	if ((tmp[0] <= 8) && (8 <= tmp[1])) {
			err_code = RFIC_TAL_SENDRCV_SET(unit) ;
    		if (err_code != RFIC_TALISE_COMPLETE) {
				printf("SET SEND RECIEVE Error err_code : %08x\n",err_code);
				return 1;
			}
    	}
    	/***** RFIC初期化完了時のメッセージ出力 *****/
		if( strcmp(argv[2], "all") == 0 ) {
			printf( "ALL RFIC BOOT PROCESS(%d-%d) COMPLETE!! \n", tmp[0], tmp[1] );
		} else {
			printf( "RFIC#%s BOOT PROCESS(%d-%d) COMPLETE!! \n", argv[2], tmp[0], tmp[1] );
		}
        err_code = 0;
    }else

    if (strcmp(argv[1], "-talise_print") == 0) {
        if (argc != 3) {
            printf ("format :  rrh -talise_print (0:OFF/1:ON/2:now set disp)      \n");
            return 1;
        }
        rrh_talise_logprint( tmp[0]);
        err_code = 0;
    }else

    if (strcmp(argv[1], "-talise_getRxGain") == 0) {
        if (argc != 3){
            printf ("format :  rrh -talise_getRxGain (0:off/1:RX1/2:RX2/3:RX12)      \n");
            return 1;
        }

		UCHAR 					rxGainval;
		err_code = rrh_talise_getRxGain( tmp[0] , &rxGainval) ;
		/* TP svrx用 */
		if((err_code == 0) && ((tmp[0] == 1) || (tmp[0] == 2))){
			T_RRH_TRAINING_DATA* traData;
			endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
				E_RRH_SHMID_APL_TRAINING_DATA,      /* 共有メモリ番号               */
				(VOID **)&traData,                  /* 共有メモリアドレス           */
				 &errcd                              /* 詳細NGコード                 */
			);
			int						ant_ofs = 0xff;
			switch(	unit ){
				case BPF_HM_DEVC_RFIC_UNIT_RFIC1:
					ant_ofs = 0x300;
					break;
				case BPF_HM_DEVC_RFIC_UNIT_RFIC2:
					ant_ofs = 0x380;
					break;
				default:
					break;
			}
			if(ant_ofs != 0xff){
				if(tmp[0] == 2){
					ant_ofs += 0x40;
				}
			 	traData->traVal_vt[ant_ofs] = rxGainval;
			}
		}
    }else
    if (strcmp(argv[1], "-talise_getObsRxGain") == 0) {
        if (argc < 3){
            printf ("format :  rrh -talise_getObsRxGain (0:off/1:RX1/2:RX2/3:RX12)      \n");
            return 1;
        }
		UCHAR 					obsRxGainVal;

		err_code = rrh_talise_getObsRxGain(tmp[0], &obsRxGainVal) ;
		if((err_code == 0) && (tmp[0] == 1)){
			T_RRH_TRAINING_DATA* traData;
			endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
				E_RRH_SHMID_APL_TRAINING_DATA,      /* 共有メモリ番号               */
				(VOID **)&traData,                  /* 共有メモリアドレス           */
				 &errcd                              /* 詳細NGコード                 */
			);
			switch(	unit ){
				case BPF_HM_DEVC_RFIC_UNIT_RFIC1:
					traData->traVal_vt[0x00c] = obsRxGainVal;
					break;
				case BPF_HM_DEVC_RFIC_UNIT_RFIC2:
					traData->traVal_vt[0x00d] = obsRxGainVal;
					break;
				default:
					break;
			}
		}
    }else
    if (strcmp(argv[1], "-talise_setGpioOe") == 0) {
        if (argc != 3){
            printf ("format :  rrh -talise_setGpioOe (Mask)                       \n");
            return 1;
        }
        err_code = rrh_cmd_talise_setGpioOe( tmp[0] );
    }else
    if (strcmp(argv[1], "-talise_getGpioOe") == 0) {
        err_code = rrh_cmd_talise_getGpioOe();
    }else
    if (strcmp(argv[1], "-talise_setGpioSourceCtrl") == 0) {
        if (argc != 3){
            printf ("format :  rrh -talise_setGpioSourceCtrl (gpioSrcCtrl)        \n");
            return 1;
        }
        err_code = rrh_cmd_talise_setGpioSourceCtrl( tmp[0] );
    }else
    if (strcmp(argv[1], "-talise_getGpioSourceCtrl") == 0) {
        err_code = rrh_cmd_talise_getGpioSourceCtrl();
    }else
    if (strcmp(argv[1], "-talise_setGpioMonitorOut") == 0) {
        if (argc != 4){
            printf ("format :  rrh -talise_setGpioMonitorOut (MonitorIndex) (monitorMASK)\n");
            return 1;
        }
        err_code = rrh_cmd_talise_setGpioMonitorOut( tmp[0] , tmp[1] );
    }else
    if (strcmp(argv[1], "-talise_getGpioMonitorOut") == 0) {
        if (argc != 3){
            printf ("format :  rrh -talise_getGpioMonitorOut (MonitorIndex)        \n");
            return 1;
        }
        err_code = rrh_cmd_talise_getGpioMonitorOut( tmp[0] );
    }else
    if (strcmp(argv[1], "-talise_resetRxAgc") == 0) {
        err_code = rrh_cmd_talise_resetRxAgc( );
    }else
    if (strcmp(argv[1], "-talise_getSlicerPosition") == 0) {
        err_code = rrh_cmd_talise_getSlicerPosition( );
    }else
    if (strcmp(argv[1], "-talise_getEnabledTrackingCals") == 0) {
        uint32_t                enableMask;
        err_code = rrh_cmd_talise_getEnabledTrackingCals( &enableMask );
    } else if (strcmp(argv[1], "-talise_getOrxQecStatus") == 0) {
        taliseOrxQecStatus_t    orxQecStatus;
        if(argc != 3) {
            printf ("Format : rrh -talise_getOrxQecStatus (channelSel) \n");
            return 1;
        } else if( (tmp[0] != 1) && (tmp[0] != 2) ) {
            printf ("Paramter Error! (set to 1:Orx1/2:Orx2 only) \n");
            return 1;
        } else {
            err_code = rrh_cmd_talise_getOrxQecStatus( tmp[0], &orxQecStatus );
        }
    } else if (strcmp(argv[1], "-talise_getTxLolStatus") == 0) {
        taliseTxLolStatus_t     txLolStatus;
        if(argc != 3) {
            printf ("Format : rrh -talise_getTxLolStatus (channelSel) \n");
            return 1;
        } else if( (tmp[0] != 1) && (tmp[0] != 2) ) {
            printf ("Paramter Error! (set to 1:TX1/2:TX2 only) \n");
            return 1;
        } else {
            err_code = rrh_cmd_talise_getTxLolStatus( tmp[0], &txLolStatus );
        }
    } else if (strcmp(argv[1], "-talise_getTxQecStatus") == 0) {
        taliseTxQecStatus_t     txQecStatus;
        if(argc != 3) {
            printf ("Format : rrh -talise_getTxQecStatus (channelSel) \n");
            return 1;
        } else {
            err_code = rrh_cmd_talise_getTxQecStatus( tmp[0], &txQecStatus );
        }
    } else if (strcmp(argv[1], "-talise_getRxQecStatus") == 0) {
        taliseRxQecStatus_t     rxQecStatus;
        if(argc != 3) {
            printf ("Format : rrh -talise_getRxQecStatus (channelSel) \n");
            return 1;
        } else {
            err_code = rrh_cmd_talise_getRxQecStatus( tmp[0], &rxQecStatus );
        }
    } else if (strcmp(argv[1], "-talise_getRxHd2Status") == 0) {
        taliseRxHd2Status_t     rxHd2Status;
        if(argc != 3) {
            printf ("Format : rrh -talise_getRxHd2Status (channelSel) \n");
            return 1;
        } else {
            err_code = rrh_cmd_talise_getRxHd2Status( tmp[0], &rxHd2Status );
        }
    } else if (strcmp(argv[1], "-talise_getPendingTrackingCals") == 0) {
        uint32_t                pendingCalMask;
        err_code = rrh_cmd_talise_getPendingTrackingCals( &pendingCalMask );
    }else
    if (strcmp(argv[1], "-talise_setGpIntMask") == 0) {
        if (argc != 3){
            printf ("format :  rrh -talise_setGpIntMask (Mask) \n");
            return 1;
        }
        err_code = rrh_cmd_talise_setGpIntMask( tmp[0] );
    }else
    if (strcmp(argv[1], "-talise_getGpIntStatus") == 0) {
        uint16_t                gpIntStatus;
        err_code = rrh_cmd_talise_getGpIntStatus( &gpIntStatus );
    }else
    if (strcmp(argv[1], "-talise_gpIntHandler") == 0) {
        uint32_t                gpIntStatus;
        uint32_t                gpIntDiag;
        err_code = rrh_cmd_talise_gpIntHandler( &gpIntStatus, &gpIntDiag );
    }else
    if (strcmp(argv[1], "-talise_getGpIntMask") == 0) {
        uint16_t                gpIntMask;
        err_code = rrh_cmd_talise_getGpIntMask( &gpIntMask );
    }else
    {
        printf("rrh     - talise TPコマンドリスト\n");
        printf("\n");
        printf("[ parameter ]                                                        \n");
//        printf(" rrh -talise_setspi                         : spi parameter write    \n");
//        printf(" rrh -talise_dispspi                        : spi parameter read     \n");
        printf(" rrh -talise_print (0:OFF/1:ON/2:now set disp)                       \n");
        printf(" rrh -talise_setparam_1                        : talise parameter set\n");
        printf(" rrh -talise_setparam_jesd                     : talise parameter set\n");
        printf(" rrh -talise_setparam_frame              [hex] : talise parameter set\n");
        printf(" rrh -talise_getparam_1                        : talise parameter get\n");
        printf(" rrh -talise_getparam_jesd                     : talise parameter get\n");
        printf(" rrh -talise_getparam_frame                    : talise parameter get\n");
        printf(" rrh -talise_getparam_fir  (0:tx/1:rx/2:orx)   : talise parameter get\n");
        printf(" [ talise API command ] (taliseNo all:ALL 0-3:part)                  \n");
        printf(" rrh -talise_openHW                                                  \n");
        printf(" rrh -talise_resetDevice                                             \n");
        printf(" rrh -talise_initialize                                              \n");
        printf(" rrh -talise_getPllsLockStatus                                       \n");
        printf(" rrh -talise_enableMultichipSync (0 or 1)                            \n");
        printf(" rrh -talise_initArm                                                 \n");
        printf(" rrh -talise_loadStreamFromBinary                                    \n");
        printf(" rrh -talise_loadArmFromBinary                                       \n");
        printf(" rrh -talise_verifyArmChecksum                                       \n");
        printf(" rrh -talise_setRfPllFrequency (0:CLK/1:RF/2AUX) (周波数[kHz])       \n");
        printf(" rrh -talise_runInitCals (cal_mask(hex))                             \n");
        printf(" rrh -talise_waitInitCals (wait)                                     \n");
        printf(" rrh -talise_enableFramerLink (0:A/1:B/2:A_B) (0 or 1)               \n");
        printf(" rrh -talise_enableSysrefToFramer (0:A/1:B/2:A_B) (0 or 1)           \n");
        printf(" rrh -talise_enableDeframerLink (0:A/1:B/2:A_B) (0 or 1)             \n");
        printf(" rrh -talise_enableSysrefToDeframer (0:A/1:B/2:A_B) (0 or 1)         \n");
        printf(" rrh -talise_readDeframerStatus (0:A/1:B/2:A_B)                      \n");
        printf(" rrh -talise_readFramerStatus (0:A/1:B/2:A_B)                        \n");
        printf(" rrh -talise_enableTrackingCals                                      \n");
        printf(" rrh -talise_radioOn                                                 \n");
        printf(" rrh -talise_radioOff                                                \n");
        printf(" rrh -talise_shutdown                                                \n");
        printf(" rrh -talise_closeHw                                                 \n");
        printf(" rrh -talise_setupAuxDacs                                            \n");
        printf(" rrh -talise_writeAuxDac (index) (code(hex)                          \n");
        printf(" rrh -talise_setTxAttenuation  (1:TX1/2:TX2) (mdB)                   \n");
        printf(" rrh -talise_getTxAttenuation  (1:TX1/2:TX2)                         \n");
        printf(" rrh -talise_setRxManualGain (0:off/1:rx1/2:rx2/3:rx12) (gainIndex)  \n");
        printf(" rrh -talise_setObsRxManualGain (0:off/1:rx1/2:rx2/3:rx12) (gainIndex)\n");
        printf(" rrh -talise_getTemperature                                          \n");
        printf(" rrh -talise_setTxToOrxMapping  (EN(0 or 1)) (oRx(0:none/2:Tx1/3:Tx2)) (oRx(0:none/2:Tx1/3:Tx2))\n");
        printf(" rrh -talise_setArmGpioPins                                          \n");
        printf(" rrh -talise_setRadioCtlPinMode (pinOptionsMask(hex)) (orxEnGpioPinSel(0-3))\n");
        printf(" rrh -talise_setRxTxEnable (rxOrxChannel(0:RXoff/1:RX1/2:RX2/3RX12/4:ORX1/8:ORX2/C:ORX12) (txChannel(0:off/1:1/2:2/3:1and2))\n");
        printf(" rrh -talise_getRxTxEnable                                           \n");
        printf(" rrh -talise_getRadioState                                           \n");
        printf(" rrh -talise_getInitCalStatus                                        \n");
        printf(" rrh -talise_checkInitCalComplete                                    \n");
        printf(" rrh -talise_enableFramerTestData (0:A/1:B/2:AB) (DataSource(0-15)) (TestData(0-2))\n");
        printf(" rrh -talise_setupAdcSampleXbar (0:A/1:B/2:AB) (conv0) … (conv7)    \n");
        printf(" rrh -talise_getErrCode                                              \n");
        printf(" rrh -talise_rescheduleTrackingCal (trackingCal)                     \n");
        printf("    ->  rrh -talise_rescheduleTrackingCal  -> print trackingCal value\n");
        printf(" rrh -talise_enableDeframerPrbsChecker (polyOrder) (checkerLocation)     \n");
        printf("        (polyOrder)                0:Disable/1:PRBS7/2:PRBS15/3:PRBS31     \n");
        printf("        (checkerLocation)          0:LANE_DATA/1:SAMPLE_DATA \n");
        printf(" rrh -talise_readDeframerPrbsCounters  (lane(0-3))      \n");
        printf(" rrh -talise_setRfPllLoopFilter  (loopBandwidth_kHz(50-750)) (stability(3-15)     \n");
        printf(" rrh -talise_getRxGain (0:off/1:RX1/2:RX2/3:RX12)       \n");
        printf(" rrh -talise_getObsRxGain  (0:off/1:ORX1/2:ORX2/3:ORX12)\n");
        printf(" rrh -talise_boot (StartBlock) (StopBlock)              \n");
        printf(" rrh -talise_setGpioOe (Mask)                           \n");
        printf(" rrh -talise_getGpioOe                                  \n");
        printf(" rrh -talise_setGpioSourceCtrl (gpioSrcCtrl)            \n");
        printf(" rrh -talise_getGpioSourceCtrl                          \n");
        printf(" rrh -talise_setGpioMonitorOut (MonitorIndex) (monitorMASK)\n");
        printf(" rrh -talise_getGpioMonitorOut (MonitorIndex)           \n");
        printf(" rrh -talise_resetRxAgc                                 \n");
        printf(" rrh -talise_getSlicerPosition                          \n");
/*      printf(" rrh -talise_setupRxAgc  (rxAgcCtrl)                    \n");  */
/*      printf(" rrh -talise_setRxGainControlMode (mode)                \n");  */
/*      printf(" rrh -talise_setRxDataformat                            \n");  */
/*      printf(" rrh -talise_getRxDataformat                            \n");  */
        printf(" rrh -talise_getEnabledTrackingCals                     \n");
        printf(" rrh -talise_getOrxQecStatus (channelSel)               \n");
        printf(" rrh -talise_getTxLolStatus (channelSel)                \n");
        printf(" rrh -talise_getTxQecStatus (channelSel)                \n");
        printf(" rrh -talise_getRxQecStatus (channelSel)                \n");
        printf(" rrh -talise_getRxHd2Status (channelSel)                \n");
        printf(" rrh -talise_getPendingTrackingCals                     \n");
        printf(" rrh -talise_setGpIntMask (Mask)                        \n");
        printf(" rrh -talise_getGpIntMask                               \n");
        printf(" rrh -talise_getGpIntStatus                             \n");
        printf(" rrh -talise_gpIntHandler                               \n");
    }

    return err_code;

}

#if 0 //未使用
int Update_TXRFICparam(uint antnum)
{
    uint eep_offset = 0x0020;
    uint16_t  txAttenuation_mdB;
    uint8_t  obsRxGainIndex;
    int ret;
    unsigned long dat=0;
    
    ret = rrh_talise_getTxAttenuation( antnum + 1, &txAttenuation_mdB);
    if(ret){
        return 1;
    }

    printf("debug txAttenuation_mdB: 0x%04hx\n",txAttenuation_mdB);
    
    apde2pTX[APDE2P_TXVGA_A + (eep_offset * antnum)]     = (unsigned char)(0x00FF & (txAttenuation_mdB>>8));
    apde2pTX[APDE2P_TXVGA_A + (eep_offset * antnum) + 1] = (unsigned char)(0x00FF & txAttenuation_mdB);

//talise のSPIレジスタ直読みに変更
//    ret = rrh_talise_getObsRxGain( antnum, &obsRxGainIndex);
    ret = SPI_RFIC_regr( 0x5D3, &dat );
    obsRxGainIndex = (uint8_t)(0x000000FF & dat);
    if(ret){
        return 1;
    }
    apde2pTX[APDE2P_ORX1GAIN + (eep_offset * antnum)]     = obsRxGainIndex;
    
    return 0;
}


int Update_RXRFICparam(uint antnum)
{
    uint eep_offset = 0x0020;
    uint8_t  rxGainIndex;
    int ret;
    unsigned long add,dat=0;
    
//talise のSPIレジスタ直読みに変更
//    ret = rrh_talise_getRxGain( antnum, &rxGainIndex);
    if(antnum==0){
        add = 0x5D2;
    }else{
        add = 0x5E2;
    }

    ret = SPI_RFIC_regr( add, &dat );
    rxGainIndex = (uint8_t)(0x000000FF & dat);
    if(ret){
        return 1;
    }
    
    apde2pRX[APDE2P_RXGAIN_INDEX_A + (eep_offset * antnum)]     = rxGainIndex;

    return 0;
}

int Update_otherRFICparam(void)
{
    unsigned long freq_khz;
    int ret;
    

    ret = rrh_talise_getRfPllFrequency(&freq_khz);
    if(ret){
        return 1;
    }
    
    apde2pTX[APDE2P_PLLFREQ ]    = (unsigned char)(0x0000FF & (freq_khz>>24));
    apde2pTX[APDE2P_PLLFREQ + 1] = (unsigned char)(0x0000FF & (freq_khz>>16));
    apde2pTX[APDE2P_PLLFREQ + 2] = (unsigned char)(0x0000FF & (freq_khz>>8));
    apde2pTX[APDE2P_PLLFREQ + 3] = (unsigned char)(0x0000FF & freq_khz);

    return 0;
}

void rrh_udelay(uint32_t time_us)
{
uint32_t buf1,buf2;

    if(time_us >= 1000){
        buf1 = time_us / 1000;
        buf2 = time_us - 1000 * buf1;
//debug        printf("mdelay(buf1) buf1:%ld \n",buf1);
//debug        printf("udelay(buf2) buf2:%ld \n",buf2);
        udelay(buf2);
        mdelay(buf1);
    }else{
        udelay(time_us);
    }
}


void rrh_talise_reset(uint32_t time_us)
{
//unsigned long buf,buf2;
//アドレスまだ未定
    unsigned long add = FPGA_DBG_RFICON;

    *REG_32(add) = 0;
//1μsec待つだけみたい
    rrh_udelay(time_us);
    *REG_32(add) = 1;
}
#endif //未使用

