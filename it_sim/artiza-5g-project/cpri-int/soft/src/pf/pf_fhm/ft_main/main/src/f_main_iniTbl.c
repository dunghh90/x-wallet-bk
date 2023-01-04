/*!
 * @skip		$ld:$
 * @file		f_main_iniTbl.c
 * @brief		pf_main テーブルの初期化処理
 * @author		alpha)中村
 * @date		2013/12/12 alpha)中村 Create
 * @date  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"

#define BigLittleSwap16(V)  ((((USHORT)(V) & 0xff00) >> 8) | \
                            (((USHORT)(V) & 0x00ff) << 8))
/*!
 * @brief		f_main_iniTbl
 * @note		PFプロセステーブルの初期化処理を行う.
 * @param		-
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/12/12 alpha)中村
 * @date  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date        2015/08/08 TDI)satou トレーニングデータの共有メモリアドレス取得を削除
 * @date  		2015/08/18 TDIPS) maruyama 装置種別で配列の切り分けを取りやめ（データを直接セットする）
 */
VOID f_main_iniTbl()
{
	INT ret;
	INT errcd;

	const T_RRH_REDEVCOMPTBL re_dev_tbl_FHM = {
			D_RRH_IND_MAKER_NAME,		/* Maker name						*/
			D_SYS_BTS_SRE_NO,			/* BTS type							*/
			D_SYS_REKIND_OFTRX,			/* RE type							*/
			D_SYS_DEVKND_RRE,			/* device kind						*/
			D_SYS_NUM1,					/* device number					*/
			D_SYS_DIV_OFF,				/* Diversity type (3G use)			*/
			D_SYS_PTYP_COM,				/* power type						*/
			D_SYS_POWERUNIT_MW,			/* power class unit					*/
			D_SYS_PWR_CLASS_120,		/* power class (Br0)				*/
		 	D_SYS_PWR_CLASS_120,		/* power class (Br1) 				*/
			D_SYS_PWR_CLASS_120,		/* power class (Br2)				*/
		 	D_SYS_PWR_CLASS_120,		/* power class (Br3) 				*/
			D_SYS_CRP_FRQ_35G_S3G,		/* corresponding frequency area		*/
			D_SYS_BANDW_MAX_40M,		/* Max bandwidth					*/
			D_SYS_SND_ANTE_MAX_4,		/* Max TX antenna number			*/
			D_SYS_NUM0,					/* 800MHz filter type				*/
			D_SYS_NUM0,					/* 2GHz frequency carrier number	*/
			D_SYS_NUM0,					/* 800MHz frequency carrier number	*/
			D_SYS_NUM0,					/* 1.7GHz frequency carrier number	*/
			D_SYS_NUM0,					/* RE side TRX-INF type				*/
			D_SYS_NUM0,					/* 1.5GHz frequency carrier number	*/
			D_SYS_NUM2,					/* 3.5GHz frequency carrier number	*/
			D_SYS_NUM0					/* MCA Filter						*/
	};
	const T_RRH_REDEVCOMPTBL re_dev_tbl = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_iniTbl] ENTER" );

	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_DEVICE_MNG_TBL,(VOID**)&f_cmw_re_dev_tbl,&errcd);
	ret |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY,(VOID**)&f_cmw_tra_inv_tbl,&errcd);
	ret |= BPF_RU_IPCM_PROCSHM_ADDRGET(D_RRH_SHMID_APL_LMC_DATA,(VOID**)&f_cmw_rcvfirm_tbl,&errcd);

	if(ret != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x", errcd);
	}
	
	memset((VOID *)f_mainw_thdIniMng, 0, sizeof(f_mainw_thdIniMng));

	/************************************************************************/
	/* device information setting											*/
	/************************************************************************/
	/* ソフト埋め込みとして関数内で定義しているデータを共有メモリに展開 */
	switch(f_comw_reKind )
	{
		case E_RRH_REKIND_FHM:
			memcpy( f_cmw_re_dev_tbl, &re_dev_tbl_FHM, sizeof(T_RRH_REDEVCOMPTBL));
			break;
		default:
			memcpy( f_cmw_re_dev_tbl, &re_dev_tbl, sizeof(T_RRH_REDEVCOMPTBL));
			break;
	}
	/************************************************************************/
	/* システムパラメータTypical値設定										*/
	/************************************************************************/
	f_com_iniSysParaTblFHM();
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_iniTbl] RETURN" );

	return;
}

/* @} */
