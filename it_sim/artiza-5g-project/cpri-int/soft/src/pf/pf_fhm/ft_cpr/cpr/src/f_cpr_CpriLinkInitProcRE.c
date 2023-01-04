/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_cpr_CpriLinkInitProcRE.c
 *  @brief  re  link start indication function
 *  @date 2015/08/05 TDIPS)ikeda create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_CPR
* @{ */

#include "f_sys_inc.h"
#include "f_cpr_inc.h"
#include "BPF_RU_ITCM.h"
#include "BPF_RU_HRTM.h"
#include "rrhApi_Cpri.h"

/**
* @brief 	re cpri link init indication function
* @note 	re cpri link init indication function.\n
* @param 	buff_adr [in] the buffer address pointer of received message
* @return 	None
* @date 	2015/08/04 TDIPS)ikeda
* @date     2015/8/6 TDIPS)ikeda rev.27879 L3 CPRI 追加分
* @date     2015.09.03 TDIPS)ikeda SFPコード読み込み修正
* @date     2015/09/03 TDI)satou レビュー指摘修正
* @date     2015/10/10 TDI)satou ハソ101-013 「CPM#n L1, L2, L3 ERR ON」追加
* @date     2015/11/02 TDI)satou その他未実装-011 SFPERR2 -> SFPERR1
* @date     2015/11/05 FPT)Yen fix bug IT3 No51
* @date     2015/11/15 TDI)satou M-RRU-ZSYS-01993 問処番号No.184 SFP ERR ON制御追加
* @date     2015/11/17 TDI)satou ハソ-QA-047
* @date     2015/12/02 TDI)satou IT3問処番号No.110
* @date     2016/11/18 FJT)ohashi 16B スロット数制限機能追加
* @warning	N/A
* @FeatureID	PF_Cpri-002-001-001
* @Bug_No	M-RRU-ZSYS-01993
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_cpr_CpriLinkInitProcRE(UINT * buff_adr)
{
	T_API_CPRILINK_STARTUP_IND	*apiInd;
	UINT regVal, regVal2;
	UINT		num;
	USHORT linkno;
	T_RRH_CONN_DISC conn;
	UCHAR sfpcode[D_CPR_EEPROM_SFPSTR_LENGTH+1];
	T_RRH_CPRI_STATE	ltCpriSta;			/* CPRIState */

	apiInd = (T_API_CPRILINK_STARTUP_IND *)buff_adr;
	linkno = apiInd->link_num;

	/* 配下REの使用/未使用 */
	f_cmn_com_conn_disk_get(&conn);
	if ( !conn.conn_disc[linkno-1] )
	{
		/* CPM#n ERR ALL OFF */
		f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_L1ERR(linkno)             , D_SYS_OFF);
	    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_L2ERR(linkno)             , D_SYS_OFF);
	    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_L3ERR(linkno)          , D_SYS_OFF);
	    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_L3ERR(linkno)         , D_SYS_OFF);
	    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_SFPERR1(linkno)           , D_SYS_OFF);
	    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_SFPERR2(linkno)           , D_SYS_OFF);
	    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_SLOT_ERR(linkno)          , D_SYS_OFF);
	    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_DELAY_ERR(linkno)      , D_SYS_OFF);
	    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_DELAY_ERR(linkno)     , D_SYS_OFF);
	    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_RESTARTUP_ERR(linkno)  , D_SYS_OFF);
	    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_DL_NG_ERR(linkno)         , D_SYS_OFF);
	    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_CARR_RESET_ERR(linkno) , D_SYS_OFF);
	    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_CARR_RESET_ERR(linkno), D_SYS_OFF);
	    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_RESTARTUP_ERR(linkno) , D_SYS_OFF);

	    /* SFPログ取得 */
	    f_cpr_sfp_log(linkno, E_RRH_SFPLOG_TRIGGER_CPRI_UNUSE);
	    
		/* Master port 起動停止 */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "re cpri stop %d", linkno );

		return;
	}

	/* CPM#n L1, L2, L3 ERR ON */
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_L1ERR(linkno)    , D_SYS_ON);
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_L2ERR(linkno)    , D_SYS_ON);
	/* 共用メモリCPRI状態の呼び出し 取得用 */
	f_cmn_com_cpri_state_get( &ltCpriSta );
	if(CMD_CONN_S3G_SINGLE != ltCpriSta.cpri_linktype )
	{
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_L3ERR(linkno) , D_SYS_ON);
	}
    f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_L3ERR(linkno), D_SYS_ON);

//16B スロット数制限機能追加 start
//16B L1-ERR時は下記ERRを落とす
	f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_SLOT_ERR(linkno)         , D_SYS_OFF);
	f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_DELAY_ERR(linkno)      , D_SYS_OFF);
	f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_DELAY_ERR(linkno)     , D_SYS_OFF);
	f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_RESTARTUP_ERR(linkno)  , D_SYS_OFF);
	f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_DL_NG_ERR(linkno)         , D_SYS_OFF);
	f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_CARR_RESET_ERR(linkno) , D_SYS_OFF);
	f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_CARR_RESET_ERR(linkno), D_SYS_OFF);
	f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_RESTARTUP_ERR(linkno) , D_SYS_OFF);
//16B スロット数制限機能追加 end

	/* CPM#n SFP状態取得 */
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CNTS_CPSFPSTA(linkno), &regVal);
	/* CPM#n SFP実装の確認 */
	if(regVal & D_RRH_REG_CNTS_CPSFPSTA_OPTMNT)
	{
		/*	SFP電源状態取得	*/
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, D_RRH_REG_CNTS_SFPLDODIS, &regVal2);
		regVal2 = (regVal2 & (0x00010000 << (linkno - 1)));
		
		/*	増設時も他のAPIで電源ONするため、ここは動作しない	*/
		/*	電源OFFの場合										*/
		if(regVal2 != 0)
		{
			BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE_CPRI,
					D_RRH_REG_CNTS_SFPLDODIS, 0x00010000 << (linkno - 1));
			
			usleep(500*1000);
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "SFP#%d power on & wait 500msec", linkno);
		}
	}
	/*	CPM#n SFP未実装の場合	*/
	else
	{
		BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE_CPRI,
				D_RRH_REG_CNTS_SFPLDODIS, 0x00010000 << (linkno - 1));
		
		/* Master port 起動停止 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "SFP#%d power off", linkno);
		/* SFP ERR on */
		f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_SFPERR1(linkno), D_SYS_ON);

		return;
	}

    /* SFPログ取得 */
    f_cpr_sfp_log(linkno, (E_RRH_SFPLOG_TRIGGER)apiInd->sfpLogTrigger);
    
	/*	共有メモリのアドレス取得できていない場合	*/
	if(f_cprw_shm_sfplog == NULL)
	{
		/*	誤実装として動作する	*/
		f_support_sfp[linkno] = D_RRH_OFF;
		f_support_98g[linkno] = D_RRH_OFF;
		f_cprw_bitrate_real = D_COM_LINE_BITRATE_98G;
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "re#%d sfpcode not get", linkno);
	}
	else
	{
		/* CPS SFP 型番確認 サポート品種の場合 */
		if(f_cprw_shm_sfplog->cpr_latest[linkno].log.sfpSupport == E_RRH_SFPLOG_SUPPORT_YES)
		{
			f_support_sfp[linkno] = D_RRH_ON;
			if(f_cprw_shm_sfplog->cpr_latest[linkno].log.sfpSpeed == E_RRH_SFPLOG_SPEED_98G_49G)
			{
				f_support_98g[linkno] = D_RRH_ON;
			}
			else
			{
				f_support_98g[linkno] = D_RRH_OFF;
			}
		}
		/* CPS SFP 型番確認 未サポート品種の場合 */
		else
		{
			f_support_sfp[linkno] = D_RRH_OFF;
			f_support_98g[linkno] = D_RRH_OFF;
		}
		memset(sfpcode, 0, sizeof(sfpcode));
		memcpy(&sfpcode, &f_cprw_shm_sfplog->cpr_latest[linkno].log.sfpInf[0][0x0060], D_CPR_EEPROM_SFPSTR_LENGTH);
		
		/* SFPコードログ */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "RE%d sfpcode [%s]", linkno, sfpcode );
	}

	/*CPM# SFP 型番確認 サポート品種*/
	if (f_support_sfp[linkno] == D_RRH_ON)
	{
		/* SFP ERR off */
		f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_SFPERR1(linkno), D_SYS_OFF);
		/* LED制御用に誤実装の場合はSFP_ERR2もOFFする */
		f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_SFPERR2(linkno), D_SYS_OFF);

		/* CPS伝送レート対CPM#n SFP対応レート整合 */
		if (f_support_98g[linkno] != f_support_98g[D_RRH_CPRINO_REC])
		{
			/* Master port 起動停止 */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "no support sfp", linkno, sfpcode );

			return ;
		}
	}
	else
	{
		/* SFP ERR on */
		f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_SFPERR1(linkno), D_SYS_ON);
		/* LED制御用に誤実装の場合はSFP_ERR2もONする */
		f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_SFPERR2(linkno), D_SYS_ON);
	}

	/* CPM#x/SFP & CPRI Disable設定 */
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_SFPCNT(linkno), &regVal);
	regVal &= ~0x00000007;
	regVal |=  0x00000001;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPRI_SFPCNT(linkno), &regVal);

	/* CPS 9.8G or 4.9G or 2.4G PHY設定 */
	if(f_cprw_bitrate_real == D_COM_LINE_BITRATE_24G)
	{
		/* CPP PHY set 2.4G */
		regVal = 0x00001861;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE1(linkno), &regVal);
		regVal = 0x00003038;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE2(linkno), &regVal);
		regVal = 0x000080C2;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE3(linkno), &regVal);
		regVal = 0x00002BFF;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE4(linkno), &regVal);
		regVal = 0x00000746;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE5(linkno), &regVal);
		regVal = 0x00001800;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE6(linkno), &regVal);
		regVal = 0x00004C03;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE7(linkno), &regVal);
		regVal = 0x0000017C;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE8(linkno), &regVal);
		regVal = 0x00000260;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE9(linkno), &regVal);

		/* CPS VOD/Emphasis設定 */
		regVal = CPM_VOD_EMPHASIS_data24g[linkno-1];
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPRI_PHYCNT(linkno), &regVal);
	}
	else if(f_cprw_bitrate_real == D_COM_LINE_BITRATE_49G)
	{
		/* CPP PHY set 4.9G */
		regVal = 0x00001861;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE1(linkno), &regVal);
		regVal = 0x00003038;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE2(linkno), &regVal);
		regVal = 0x000080C1;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE3(linkno), &regVal);
		regVal = 0x00004BFF;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE4(linkno), &regVal);
		regVal = 0x00000756;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE5(linkno), &regVal);
		regVal = 0x00001800;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE6(linkno), &regVal);
		regVal = 0x00004C03;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE7(linkno), &regVal);
		regVal = 0x0000017C;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE8(linkno), &regVal);
		regVal = 0x00000160;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE9(linkno), &regVal);

		/* CPS VOD/Emphasis設定 */
		regVal = CPM_VOD_EMPHASIS_data49g[linkno-1];
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPRI_PHYCNT(linkno), &regVal);
	}
	else
	{
		/* CPP PHY set 9.8G */
		regVal = 0x000018A1;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE1(linkno), &regVal);
		regVal = 0x0000303D;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE2(linkno), &regVal);
		regVal = 0x000080C0;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE3(linkno), &regVal);
		regVal = 0x00008BFF;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE4(linkno), &regVal);
		regVal = 0x00000766;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE5(linkno), &regVal);
		regVal = 0x00001E00;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE6(linkno), &regVal);
		regVal = 0x00004C05;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE7(linkno), &regVal);
		regVal = 0x0000057C;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE8(linkno), &regVal);
		regVal = 0x00000060;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPHYM_RATE9(linkno), &regVal);

		/* CPS VOD/Emphasis設定 */
		regVal = CPM_VOD_EMPHASIS_data98g[linkno-1];
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPRI_PHYCNT(linkno), &regVal);
	}

	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPTXCW(linkno), &regVal);
	regVal &= ~( M_RRH_REG_CPRI_CPRXCW_VER | M_RRH_REG_CPRI_CPTXCW_STUP );
	regVal |= f_cprw_SlaveCPPVer_HDLCRate;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPRI_CPTXCW(linkno), &regVal);

	/* CPM PHYリセットON */
	BPF_HM_DEVC_REG_WRITE_BITON(	D_RRH_LOG_REG_LV_READ_WRITE_CPRI,
									D_RRH_REG_CNTS_CPMSERRST,
									(0x00000010 << (linkno - D_RRH_CPRINO_RE_MIN)));

	/* CPM PHY Done確認 */
	for(num = 0; num < (D_CPM_PHYDONE_TIME_100MSEC / D_CPM_PHYDONE_TIME_POLLING); num++)
	{
		usleep(D_CPM_PHYDONE_TIME_POLLING * 1000);

		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI,
									D_RRH_REG_CNTS_STA1, &regVal);

		if((regVal & (0x00001000 << (linkno - D_RRH_CPRINO_RE_MIN))) > 0)
		{
			break;
		}
	}

	/* CPM PHY Done NG */
	if(num >= (D_CPM_PHYDONE_TIME_100MSEC / D_CPM_PHYDONE_TIME_POLLING) )
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "CPM PHY Done confirm NG!CNTS_STA1:0x%x", regVal);

		/* ALM */
		f_com_abort(D_SYS_THDID_PF_F_MAIN, D_RRH_ALMCD_STNG);

		return;
	}

	/* master port resync */
	/* 光ON＆タイマー起動 */
	f_cpr_CpriLinkStartupProcRE(linkno);

	return;
}
/* @} */
/**
* @brief 	Master SFP power ON notification function
* @note 	Master SFP power ON notification function.\n
* @param 	buff_adr [in] the buffer address pointer of received message
* @return 	None
* @date 	2015/12/24 FJT)koshida
* @warning	N/A
* @FeatureID	PF_Cpri-002-001-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_cpr_cpmPwrNoProc(UINT * buff_adr)
{
	T_API_MSGID_CPRI_CPMPOWERON_IND	*apiInd;
	UINT regVal;
	USHORT							link_num;
	
	apiInd = (T_API_MSGID_CPRI_CPMPOWERON_IND*)buff_adr;
	link_num = apiInd->link_num;
	
	/* CPM#n SFP状態取得 */
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CNTS_CPSFPSTA(link_num), &regVal);
	/* CPM#n SFP実装の確認 */
	if(regVal & D_RRH_REG_CNTS_CPSFPSTA_OPTMNT)
	{
		/*	SPF電源ON設定	*/
		BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE_CPRI,
				D_RRH_REG_CNTS_SFPLDODIS, 0x00010000 << (link_num - 1));
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "SFP#%d inclease & power on", link_num);
	}
}
/* @} */

