/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file  f_cpr_sfp_log.c
 *  @brief SFP履歴ログ
 *  @date  2015/11/16 TDI)satou ハソ-QA-047 create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-
 */
/*********************************************************************************/
/**
 * @addtogroup RRH_PF_CPR
 * @{
 */
/*********************************************************************************/
/* include                                                                       */
/*********************************************************************************/
#include "f_sys_inc.h"
#include "f_cpr_inc.h"
#include "BPF_RU_ITCM.h"
#include "BPF_RU_HRTM.h"
#include "rrhApi_Cpri.h"
/*********************************************************************************/
/* variables                                                                     */
/*********************************************************************************/
/*********************************************************************************/
/* functions                                                                     */
/*********************************************************************************/
/******************************************************************************//**
 *
 * @brief   SFP速度変換
 * @note    内部で使用するD_COM_LINE_BITRATE_*の速度情報をSFP履歴ログ用の値に変換
 * @param   com_line_bitrate [in] 変換元
 * @return  変換結果
 * @date    2015/11/17 TDI)satou ハソ-QA-047 create
 *
 *********************************************************************************/
E_RRH_SFPLOG_SPEED f_cpr_sfp_log_conv_sfp_speed(INT com_line_bitrate)
{
    E_RRH_SFPLOG_SPEED speed;

    switch (com_line_bitrate)
    {
    case D_COM_LINE_BITRATE_24G:
        speed = E_RRH_SFPLOG_SPEED_24G;
        break;
    case D_COM_LINE_BITRATE_49G:
        speed = E_RRH_SFPLOG_SPEED_98G_49G;
        break;
    case D_COM_LINE_BITRATE_98G:
        speed = E_RRH_SFPLOG_SPEED_98G_49G;
        break;
    default:
        speed = E_RRH_SFPLOG_SPEED_UNKNOWN;
        break;
    }

    return speed;
}
/******************************************************************************//**
 *
 * @brief   サポートするSFPかチェックする
 * @note    SFPからSFPコードを取得し、サポートするSFPコードと一致するか確認
 * @param   cpr_num [in ] CPRIリンク番号
 * @param   sfpcode [in ] SFPコード
 * @param   bitrate [out] SFP速度(ビットレート)
 * @return  チェック結果
 * @date    2015/11/17 TDI)satou ハソ-QA-047 create
 *
 *********************************************************************************/
E_RRH_SFPLOG_SUPPORT f_cpr_sfp_log_check_sfp_support(USHORT cpr_num, UCHAR *sfpcode, E_RRH_SFPLOG_SPEED *bitrate)
{
    INT idx;

    for (idx = 0; idx < D_CPR_EEPROM_SFPITEM_MAX; idx++)
    {
        if (strncmp((const char*) sfpcode, f_cpreeprom_sfp[idx], D_CPR_EEPROM_SFPSTR_LENGTH) == 0)
        {
            if ((f_sfp_typerec[idx] == 0) && (D_RRH_CPRINO_REC == cpr_num))
            {
                *bitrate = f_cpr_sfp_log_conv_sfp_speed(f_sfp_bitrate[idx]);
                return E_RRH_SFPLOG_SUPPORT_YES;
            }

            if ((f_sfp_typerec[idx] == 1) && (D_RRH_CPRINO_RE_MIN <= cpr_num))
            {
                *bitrate = f_cpr_sfp_log_conv_sfp_speed(f_sfp_bitrate[idx]);
                return E_RRH_SFPLOG_SUPPORT_YES;
            }

            break;
        }
    }

    *bitrate = E_RRH_SFPLOG_SPEED_UNKNOWN;
    return E_RRH_SFPLOG_SUPPORT_NO;
}
/******************************************************************************//**
 *
 * @brief   SFPログ情報1件作成
 * @param   cpr_num [in ] CPRIリンク番号
 * @param   trigger [in ] 取得契機
 * @param   logDat  [out] 作成ログ
 * @date    2015/11/17 TDI)satou ハソ-QA-047 create
 *
 *********************************************************************************/
UINT f_cpr_sfp_log_make(USHORT cpr_num, E_RRH_SFPLOG_TRIGGER trigger, T_RRH_SFP_LOG *logDat, T_RRH_SFP_LOG *logDat_rec)
{
    T_RRH_CONN_DISC shmConnDisc;
    UINT regAdr, regDat;
    E_RRH_SFPLOG_SUPPORT sfpSupport;
    E_RRH_SFPLOG_SPEED sfpSpeed, sfpSpeedSlave;
    UCHAR sfpVendor[17];
    UCHAR sfpSN[17];
    UCHAR sfpReadSlave[D_CPR_EEPROM_SFPSTR_LENGTH+1] = {0};
    INT ret;
    UINT cnt;
	
	if(logDat == NULL)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "logDat = NULL NG.");
		return(CMD_NG);
	}
	/************************/
	/*	ログ取得有無判定	*/
	/************************/
	/*	SPFログ取得triggerが未設定の場合	*/
	if(trigger == 0) /* pgr0838 */
	{
		return(CMD_NG);
	}

    /* SFP状態 */
    regAdr = M_RRH_REG_CNTS_CPSFPSTA(cpr_num);
    BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, regAdr, &regDat);
    logDat->sfpSt = regDat;
    
	/********************/
	/*	変化有無チェック	*/
	/********************/
    /*	SFPが実装の場合	*/
    if((logDat->sfpSt & D_RRH_REG_CNTS_CPSFPSTA_OPTMNT) == D_RRH_REG_CNTS_CPSFPSTA_OPTMNT)
    {
/* 2020/12/28 M&C) Merge 4G FHM src (start modify) */
		// ret = BPF_HM_DEVC_SFP_READ_BUFFER(cpr_num, (UINT)BPF_HM_DEVC_SFP_A0, 0x014, 16, sfpVendor);
		ret = BPF_HM_DEVC_SFP_READ_BUFFER((UINT)BPF_HM_DEVC_SFP_A0, 0x014, 16, sfpVendor);
/* 2020/12/28 M&C) Merge 4G FHM src (end modify) */
		sfpVendor[16] = 0;
/* 2020/12/28 M&C) Merge 4G FHM src (start modify) */
		// ret |= BPF_HM_DEVC_SFP_READ_BUFFER(cpr_num, (UINT)BPF_HM_DEVC_SFP_A0, 0x044, 16, sfpSN);
		ret |= BPF_HM_DEVC_SFP_READ_BUFFER((UINT)BPF_HM_DEVC_SFP_A0, 0x044, 16, sfpSN);
/* 2020/12/28 M&C) Merge 4G FHM src (end modify) */
		sfpSN[16] = 0;
		
		/*	ベンダ名またはシリアル番号どちらかの取得が失敗した場合	*/
		if(ret != BPF_HM_DEVC_COMPLETE)
		{
			memset(sfpVendor, 0x55, sizeof(sfpVendor));
			memset(sfpSN, 0x55, sizeof(sfpSN));
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_HM_DEVC_SFP_READ_BUFFER() NG. ret=0x%08x", ret);
		}
	}
    /*	SFPが未実装の場合	*/
	else
	{
		/*	未実装の場合は0xFFで初期化する	*/
		memset(sfpVendor, 0xFF, sizeof(sfpVendor));
		memset(sfpSN, 0xFF, sizeof(sfpSN));
		ret = BPF_HM_DEVC_NG;
	}
	/*	ベンダ名とシリアル番号が変化無い(一致の)場合	*/
	if((strncmp((const char *)&logDat->sfpInf[0][0x014], (const char *)sfpVendor, 16) == 0) &&
		(strncmp((const char *)&logDat->sfpInf[0][0x044], (const char *)sfpSN, 16) == 0))
	{
		return(CMD_NG);
	}
    /************************************************************************/
    /* ログ情報作成                                                         */
    /************************************************************************/
    memset(logDat, 0, sizeof(logDat)); /* pgr0834 */
	/*	ベンダ名またはシリアル番号どちらかの取得が失敗した場合	*/
    if(ret != BPF_HM_DEVC_COMPLETE)
    {
		/*	ベンダ名またはシリアル番号どちらかの取得が失敗したことが判るように	*/
		memcpy(&logDat->sfpInf[0][0x014], sfpVendor, sizeof(sfpVendor));
		memcpy(&logDat->sfpInf[0][0x044], sfpSN, sizeof(sfpSN));
		memset(&logDat->sfpInf[0][0x060], 0x00, 12); /* pgr0584 */
	}
	/*	ベンダ名またはシリアル番号両方とも取得が成功した場合	*/
	else
	{
		/* SFP read */
/* 2020/12/28 M&C) Merge 4G FHM src (start modify) */
		// (VOID)BPF_HM_DEVC_SFP_READ_BUFFER(cpr_num, (UINT)BPF_HM_DEVC_SFP_A0, 0x003,  8, &logDat->sfpInf[0][0x003]);
		// (VOID)BPF_HM_DEVC_SFP_READ_BUFFER(cpr_num, (UINT)BPF_HM_DEVC_SFP_A0, 0x00C,  2, &logDat->sfpInf[0][0x00C]);
		(VOID)BPF_HM_DEVC_SFP_READ_BUFFER((UINT)BPF_HM_DEVC_SFP_A0, 0x003,  8, &logDat->sfpInf[0][0x003]);
		(VOID)BPF_HM_DEVC_SFP_READ_BUFFER((UINT)BPF_HM_DEVC_SFP_A0, 0x00C,  2, &logDat->sfpInf[0][0x00C]);
/* 2020/12/28 M&C) Merge 4G FHM src (end modify) */
		memcpy(&logDat->sfpInf[0][0x014], sfpVendor, sizeof(sfpVendor));
/* 2020/12/28 M&C) Merge 4G FHM src (start modify) */
		// (VOID)BPF_HM_DEVC_SFP_READ_BUFFER(cpr_num, (UINT)BPF_HM_DEVC_SFP_A0, 0x024, 24, &logDat->sfpInf[0][0x024]);
		(VOID)BPF_HM_DEVC_SFP_READ_BUFFER((UINT)BPF_HM_DEVC_SFP_A0, 0x024, 24, &logDat->sfpInf[0][0x024]);
/* 2020/12/28 M&C) Merge 4G FHM src (end modify) */
		memcpy(&logDat->sfpInf[0][0x044], sfpSN, sizeof(sfpSN));
/* 2020/12/28 M&C) Merge 4G FHM src (start modify) */
		// (VOID)BPF_HM_DEVC_SFP_READ_BUFFER(cpr_num, (UINT)BPF_HM_DEVC_SFP_A0, 0x054, 10, &logDat->sfpInf[0][0x054]);
		(VOID)BPF_HM_DEVC_SFP_READ_BUFFER((UINT)BPF_HM_DEVC_SFP_A0, 0x054, 10, &logDat->sfpInf[0][0x054]);
/* 2020/12/28 M&C) Merge 4G FHM src (end modify) */
		for(cnt=0; cnt<2; cnt++)
		{
/* 2020/12/28 M&C) Merge 4G FHM src (start modify) */
			// ret = BPF_HM_DEVC_SFP_READ_BUFFER(cpr_num, (UINT)BPF_HM_DEVC_SFP_A0, 0x060, 32, &logDat->sfpInf[0][0x060]);
			ret = BPF_HM_DEVC_SFP_READ_BUFFER((UINT)BPF_HM_DEVC_SFP_A0, 0x060, 32, &logDat->sfpInf[0][0x060]);
/* 2020/12/28 M&C) Merge 4G FHM src (end modify) */
			if(ret == BPF_HM_DEVC_COMPLETE)
			{
				break;
			}
			else
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_HM_DEVC_SFP_READ_BUFFER() NG. link=%d, cnt=%d, ret=0x%08x", cpr_num, cnt, ret);
				memset(&logDat->sfpInf[0][0x060], 0x55, 12); /* pgr0584 */
			}
		}
/* 2020/12/28 M&C) Merge 4G FHM src (start modify) */
		// (VOID)BPF_HM_DEVC_SFP_READ_BUFFER(cpr_num, (UINT)BPF_HM_DEVC_SFP_A2, 0x000, 40, &logDat->sfpInf[1][0x000]);
		// (VOID)BPF_HM_DEVC_SFP_READ_BUFFER(cpr_num, (UINT)BPF_HM_DEVC_SFP_A2, 0x060, 24, &logDat->sfpInf[1][0x060]);
		(VOID)BPF_HM_DEVC_SFP_READ_BUFFER((UINT)BPF_HM_DEVC_SFP_A2, 0x000, 40, &logDat->sfpInf[1][0x000]);
		(VOID)BPF_HM_DEVC_SFP_READ_BUFFER((UINT)BPF_HM_DEVC_SFP_A2, 0x060, 24, &logDat->sfpInf[1][0x060]);
/* 2020/12/28 M&C) Merge 4G FHM src (end modify) */
	}
    /* 時刻取得 */
    BPF_HM_DEVC_GETTIME_BCD(&logDat->time);

    /* 取得契機 */
    logDat->trigger = (UINT)trigger;

    /* SFP対応 */
    sfpSupport = f_cpr_sfp_log_check_sfp_support(cpr_num, &logDat->sfpInf[0][0x060], &sfpSpeed);
    logDat->sfpSupport = (UINT)sfpSupport;
    logDat->sfpSpeed = (UINT)sfpSpeed;

    /* Slaveリンクの場合 */
    if (D_RRH_CPRINO_REC == cpr_num)
    {
	    /* 有効/無効 */
        logDat->enable_disable = 1;
	    /* Slave SFP速度 */
	    logDat->sfpSlaveSpeed = (UINT)sfpSpeed;
    }
    /* Masterリンクの場合 */
    else
    {
	    /* 有効/無効 */
	    f_cmn_com_conn_disk_get(&shmConnDisc);
        logDat->enable_disable = (D_RRH_ON == shmConnDisc.conn_disc[cpr_num-1]) ? 1 : 2;
        
        /*	引数がNULLポインタでない場合	*/
        if(logDat_rec != NULL)
        {
		    /* slave SFP read */
		    memcpy(sfpReadSlave, &logDat_rec->sfpInf[0][0x060], D_CPR_EEPROM_SFPSTR_LENGTH);
		    sfpReadSlave[12] = 0;
		    
		    /* Slave SFP速度 */
		    (VOID)f_cpr_sfp_log_check_sfp_support(D_RRH_CPRINO_REC, sfpReadSlave, &sfpSpeedSlave);
		    logDat->sfpSlaveSpeed = (UINT)sfpSpeedSlave;
		}
        /*	引数がNULLポインタの場合	*/
		else
		{
		    /* Slave SFP速度 */
			logDat->sfpSlaveSpeed = E_RRH_SFPLOG_SPEED_UNKNOWN;
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "logDat_rec = NULL.");
		}
    }
    return(CMD_OK);
}
/******************************************************************************//**
 *
 * @brief   SFPログ保存
 * @param   cpr_num [in] CPRIリンク番号
 * @param   trigger [in] 取得契機
 * @date    2015/11/17 TDI)satou ハソ-QA-047 create
 *
 *********************************************************************************/
VOID f_cpr_sfp_log(USHORT cpr_num, E_RRH_SFPLOG_TRIGGER trigger)
{
    INT ret, errcd;
    INT idxLogThisTime;
    INT idxLogNextTime;
    USHORT cpr_idx = 0;
	UINT rtn;
    /* 共有メモリのアドレス取得 */
    if (NULL == f_cprw_shm_sfplog)
    {
        ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_SFPLOG ,(VOID**)&f_cprw_shm_sfplog, &errcd);
        if(ret != BPF_RU_IPCM_OK)
        {
            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET() NG. shm_num=%d errcd=0x%08x", E_RRH_SHMID_APL_SFPLOG, errcd);
            return;
        }
    }

    /* 今回の書き込み位置を特定 */
    if (D_RRH_CPRINO_REC == cpr_num)
    {
        idxLogThisTime = f_cprw_shm_sfplog->header.idxNextTime_slave;
    }
    else
    {
        cpr_idx = cpr_num - 1;
        idxLogThisTime = f_cprw_shm_sfplog->header.idxNextTime_master[cpr_idx];
    }

    /* ログ情報1件作成 */
    rtn = f_cpr_sfp_log_make(cpr_num, trigger, &f_cprw_shm_sfplog->cpr_latest[cpr_num].log, &f_cprw_shm_sfplog->cpr_latest[D_RRH_CPRINO_REC].log);
	/*	SFPログ履歴保存が必要な場合	*/
    if(rtn == CMD_OK)
    {
	    /* 今回ログ保存 */
	    memcpy(&f_cprw_shm_sfplog->cpr[cpr_num].log[idxLogThisTime], &f_cprw_shm_sfplog->cpr_latest[cpr_num].log, sizeof(T_RRH_SFP_LOG));

	    /* 次回の書き込み位置を特定 */
	    idxLogNextTime = (idxLogThisTime + 1) % D_RRH_SFPLOG_TOTAL_NUM;
	    if (D_RRH_CPRINO_REC == cpr_num)
	    {
	        f_cprw_shm_sfplog->header.idxNextTime_slave = idxLogNextTime;
	    }
	    else
	    {
	        f_cprw_shm_sfplog->header.idxNextTime_master[cpr_idx] = idxLogNextTime;
	    }
	}
}
/* @} */
