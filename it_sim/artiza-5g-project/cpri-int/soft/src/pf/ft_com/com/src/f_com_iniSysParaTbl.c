/*!
 * @skip		$ld:$
 * @file		f_com_iniSysParaTbl.c
 * @brief		pf_main テーブルのsystem parameter初期化処理
 * @author		FFCS)linb
 * @date		2014/12/16 FFCS)linb Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_com_inc.h"
/*!
 * @brief		f_com_iniSysParaTblFHM
 * @note		PFプロセステーブルのsystem parameter初期化処理を行う.
 * @param		-
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2021/01/19 M&C) Merge 4G FHM src
 */
 #define BigLittleSwap16(V)  ((((USHORT)(V) & 0xff00) >> 8) | \
                            (((USHORT)(V) & 0x00ff) << 8))
     
VOID f_com_iniSysParaTblFHM( VOID )
{
	INT 		ret = D_SYS_NUM0;
	INT 		errcd = D_SYS_NUM0;
	UCHAR* 	 	sysPara_p = NULL;
	UCHAR*		sysParaS3G_p = NULL;
	USHORT		sysset_flg = D_SYS_OFF;			/* Systemparameter EEPROM setting Flag */
	UINT		eepSize = D_SYS_NUM0;
	USHORT		sysData[D_SYS_NUM16];
	UCHAR*		data_p = NULL;
	UCHAR*		data2_p = NULL;
	UINT		sys_idx = D_SYS_NUM0;        	/* system parameter index value 		*/
	UINT 		num = D_SYS_NUM0;
	UCHAR		old_eep = D_SYS_NUM0;			/* the old systmparameterEF number from EEPROM */

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_com_iniSysParaTbl] ENTER" );

	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_SYSPARA_INFO, (VOID **)&sysPara_p, &errcd);
	if(ret != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x", errcd);
	}

	/************************************************************************/
	/* システムパラメータTypical値設定										*/
	/************************************************************************/
	sysParaS3G_p = sysPara_p + D_SYS_SYSPARA_MAX * sizeof( USHORT );
	
	f_cmw_sys_mng_tbl[D_SYS_3G] = (T_RRH_SYSMNGTBL*)sysPara_p;
	f_cmw_sys_mng_tbl[D_SYS_S3G] = (T_RRH_SYSMNGTBL*)sysParaS3G_p;

	memcpy((void*)sysPara_p,    f_comr_syspara_typval_tbl, (sizeof( USHORT ) * D_SYS_SYSPARA_MAX) );
	memcpy((void*)sysParaS3G_p, f_comr_syspara_typval_tbl, (sizeof( USHORT ) * D_SYS_SYSPARA_MAX) );

	/* set the initial data for system parameter E and F */
	/* read out system parameter EEPROM setting flag			*/

	data_p = (UCHAR*)(&sysset_flg);
	eepSize = sizeof(USHORT);
	for( sys_idx = D_SYS_NUM0; sys_idx < eepSize; sys_idx++ )
	{
		BPF_HM_DEVC_EEPROM_READ( D_RRH_EEP_SYSPARA_FLG0 + sys_idx, data_p);
		data_p++;
	}

	sysset_flg = BigLittleSwap16(sysset_flg);

	if ( sysset_flg != D_SYS_ON )
	{	/* SREでは、システムパラメータ受信フラグの読み出し値をLSBでマスクした結果が0x0000であれば
		 * 本ルートを実行し、変数E/FをEEPROMに書き込み、システムパラメータ受信フラグを受信済みに更新していた.
		 * FHMではすでにリリース済みの装置で、システムパラメータ受信フラグの値は0xFFFFになっており、同じ処理を行うと、
		 * 変数E/Fの内容が0xFFFFでFHMとREの起動手順を行ってしまう.
		 *
		 * そのためFHMでは、システムパラメータ受信フラグの値が受信済み(0x0001)以外の場合は何も処理せず、
		 * f_comr_syspara_typval_tblに格納されているシステムパラメータ暫定値で動作し、EEPROMには
		 * LTE-RECからシステムパラメータを受信した時のみ書き込む. */
#if 0
		eepSize = sizeof(USHORT) * (D_RRH_SYSPRARCV_MAX - D_RRH_SYSPRAD_MAX);
		memcpy((VOID*)sysData, (VOID*)(&f_comr_syspara_typval_tbl[D_RRH_SYSPRAD_MAX]), eepSize);
		/* Endian switch */
		for(num = D_SYS_NUM0; num < D_SYS_NUM16; num++)
		{
			sysData[num] = BigLittleSwap16(sysData[num]);
		}
		/* Writing data pointer */
		data_p = (UCHAR*)sysData;

		for( sys_idx = D_SYS_NUM0; sys_idx < eepSize; sys_idx++ )
		{
			/* in EEPROM, there are no system parameter E&F value 	*/
			/* set the typical value to EEPROM						*/
			/* parameter E& F in cmw_sys_mng_tbl has been updated	*/
			BPF_HM_DEVC_EEPROM_WRITE( D_RRH_EEP_RE_PRIORITY_SYS_INFO0 + sys_idx, data_p);
			data_p++;
		}
		/* set the system parameter EEPROM setting flag ON */
		sysset_flg = D_SYS_ON;
		sysset_flg = BigLittleSwap16(sysset_flg);

		data_p = (UCHAR*)(&sysset_flg);
		eepSize = sizeof(USHORT);
		for( sys_idx = D_SYS_NUM0; sys_idx < eepSize; sys_idx++ )
		{
			BPF_HM_DEVC_EEPROM_WRITE( D_RRH_EEP_SYSPARA_FLG0 + sys_idx, data_p);
			data_p++;
		}
#endif
	}
	else
	{
		data_p = (UCHAR*)sysData;
		eepSize = sizeof(USHORT) * (D_RRH_SYSPRARCV_MAX - D_RRH_SYSPRAD_MAX);

		for( sys_idx = D_SYS_NUM0; sys_idx < eepSize; sys_idx++ )
		{
			/* get the original system parameter E&F from EEPROM	*/
			BPF_HM_DEVC_EEPROM_READ(D_RRH_EEP_RE_PRIORITY_SYS_INFO0 + sys_idx, &old_eep);

			/* set the system parameter E&F in EEPROM to common table */
			*data_p = old_eep;
			data_p++;
		}
		/* Endian switch */
		for(num = D_SYS_NUM0; num < D_SYS_NUM16; num++)
		{
			sysData[num] = BigLittleSwap16(sysData[num]);
		}
		/* Copy system parameter data to share memory */
		data_p = sysPara_p + D_RRH_SYSPRAD_MAX * D_SYS_NUM2;
		data2_p = sysParaS3G_p + D_RRH_SYSPRAD_MAX * D_SYS_NUM2;
		memcpy(data_p, (VOID*)sysData, eepSize);
		memcpy(data2_p, (VOID*)sysData, eepSize);
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_com_iniSysParaTbl] RETURN" );

	return;
}

/*!
 * @brief		f_com_iniSysParaTbl
 * @note		PFプロセステーブルのsystem parameter初期化処理を行う.
 * @param		-
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2014/12/16 FFCS)linb
 */    
VOID f_com_iniSysParaTbl( VOID )
{
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_com_iniSysParaTbl] ENTER" );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_com_iniSysParaTbl] RETURN" );

	return;
}
/* @} */
