/*!
 * @skip   $Id$
 * @file   m_dl_SysParaSet_3G.c
 * @brief  the processing for systemparameter setting for LTE.
 * @date   2008/07/30 FFCS)Wuh Create for eNB-008-001.
 * @date   2008/08/05 FFCS)Change the method of sector chip writing by the new HS.
 * @date   2009/04/08  FFCS)Wuh
 *                      add a new BTI setting flag to judge whether need do BTI setting
 * @date   2009/04/13  FFCS)Wuh modify for S3G PRC CR-00038-005 (CPRI spec V1.07)
 * @date   2009/07/20 FFCS)Tangj modify for S3G PRC 7th release function system parameter mechanism change
 * @date   2009/08/12 FFCS)Wangjuan modify for  M-S3G-eNBPF-02336 
 *						   TRA(RF)カードにおいて電源ON後一瞬ACTとなった直後にALM発生
 * @date   2009/08/08 FFCS)Wangjuan modify for 
 *							  CR-00062-005(CPRI spec V1.1.0)
 * @date   2010/01/13 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース追加修正
 * @date   2010/02/17 QNET)Kabasima MOD CR-00088-000:[DCM向け]REハードソフトインタフェース仕様書v2.16版リリース
 * @date   2010/02/19 QNET)Kabasima LREでTxDiv有無受信時のシスパラチェック外す件
 * @date   2010/03/17 QNET)Kabasima CR-00090-000 [DCM向け]LTE単独構成でTRX設定(LTE)有りのままでCPRI共用構成変更後のTRX送信波が異常となる場合がある件
 * @date   2010/04/02 FJT)Taniguchi  modify for CR-XXXXX-XXX
 * @date   2012/07/05  FFCS)niemsh modify for 1.5GVA
 * @date   2015/10/14 TDIPS)sasaki Update
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"			/* TRA common head file			*/
#include "m_dl_header.h"			/* download management task head file */
#include "f_rrh_reg_eep.h"
#include "rrhApi_Inv.h"
#include "rrhApi_Svp.h"
/*!
 *  @brief  the function processing for LTE system parameter download.
 *  @note   This function is processed as follows.
 *          1) write systemparameter to common table
 *          2) set sector chip revision and CPRI state stability timer
 *          3) CPRI-share or RF-share connection case, set S3G bandwidth,3G carrier working flag,BTI and RSSI
 *  @param  -
 *  @return Result code is returned.
 *  @retval D_OK	0:success 
 *  @retval CMD_INF_NG	3:情報NG(3G)
 *  @date   2008/07/30 FFCS)Wuh Create.
 *  @date   2008/12/26 FFCS)Wuh M-S3G-eNBPF-01078												
 *				            don't set branch1 if card type is LRE 1TX
 *  @date   2009/01/04 FFCS)Wuh M-S3G-eNBPF-01134												
 *				            modify the judging rule of RF type
 *  @date   2009/01/04 FFCS)wuh	H.S related, HS version V1R10
 *  @date   2009/01/04 FFCS)Wuh M-S3G-eNBPF-01150												
 *				            add sum for S3G bandwidth and 3G carrier frequency is not right
 *  @date   2009/01/04 FFCS)Wuh M-S3G-eNBPF-01153												
 *				            improve the setting of APD register "3G/S3G system enable"
 *  @date   2009/01/26 FFCS)Wuh M-S3G-eNBPF-01227												
 *				            delete the judgment after reading from EEPROM
 *  @date   2009/02/11  FFCS)wuh modify for M-S3G-eNBPF-01348
 * @date    2009/03/05  FFCS)Wuh modify for M-S3G-eNBPF-01471
 *                          stop writing FLASH after receiving CPRI message
 * @date    2009/06/03  FFCS)Wuh modify for M-S3G-eNBPF-02011
 *                          out of the range,give up the systemparameter
 * @date   2009/07/20 FFCS)Tangj modify for S3G PRC 7th release function system parameter mechanism change
 * @date   2009/08/12 FFCS)Wangjuan modify for  M-S3G-eNBPF-02336 
 *						   TRA(RF)カードにおいて電源ON後一瞬ACTとなった直後にALM発生
 * @date   2009/10/08 QNET)Kabasima MOD CR-00071-001(TRA-HS-V2R11 RE AUTO RESET and 3GRF-INF)
 * @date   2010/01/13 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース追加修正
 * @date   2010/02/17 QNET)Kabasima MOD CR-00088-000:[DCM向け]REハードソフトインタフェース仕様書v2.16版リリース
 * @date   2010/02/19 QNET)Kabasima LREでTxDiv有無受信時のシスパラチェック外す件
 * @date   2010/03/17 QNET)Kabasima CR-00090-000 [DCM向け]LTE単独構成でTRX設定(LTE)有りのままでCPRI共用構成変更後のTRX送信波が異常となる場合がある件
 * @date   2010/04/02 FJT)Taniguchi  modify for CR-XXXXX-XXX
 * @date   2011/04/06 FJT)Koshida modify CeNB-F-072-018(DCM)BRE(RF共用)装置関連仕様について
 * @date   2011/04/06 FJT)Koshida modify for M-S3G-eNBPF-04023
 * @date   2012/05/01 FJT)Tokunaga CeNB-F-082-007(DCM)(F-1382(DCM)：LTE帯域幅と3Gキャリア運用有無の反映について)
 * @date   2012/07/05  FFCS)niemsh modify for 1.5GVA
 * @date   2015/10/08 FPT)Duong update MKレビュー指摘No.198,226対応
 * @date   2015/10/14 TDIPS)sasaki update MKレビュー指摘No.298対応
 *
 */

UINT m_dl_SysParaSet_3G( VOID )
{
	UINT			a_rtn_code	= CMD_RES_OK;			/* 応答結果格納変数			*/
	UINT			a_cnt		= CMD_NUM0;				/* ループカウンタ			*/
	UINT			a_num		= CMD_NUM0; 			/* the number of the matrix */
    USHORT			a_sysprm_dt	= CMD_NUM0;				/* 取得したシスパラ値		*/
	USHORT			*efParam = NULL;
	E_RRHAPI_RCODE	apiRtn = E_API_RCD_OK;

	/*---------------------------------------------------------------------------------------------*/

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_SysParaSet_3G] ENTER" );

	/* for the 1st system parameter receiving card, RSV has set default system parameter E&F to EEPROM	*/
	/* so can read out the EEPROM value directly here													*/
	
	/********************/
	/* テーブル設定処理 */
	/********************/
	/* check the range of systemparameter */
	for( a_cnt = CMD_NUM0; a_cnt < DLD_SYSPRAD_NUM; a_cnt++ )
	{
		/* 取得したシスパラ値をSHORT型に変換 */
		a_sysprm_dt = ( dlw_rcvsyspa_tbl_3g[a_cnt * CMD_NUM2] << DLD_BIT8 ) | 
					  ( dlw_rcvsyspa_tbl_3g[a_cnt * CMD_NUM2 + CMD_NUM1] );
		/* the processing about systemparameterD */
		a_num = a_cnt;
		/* out of the range */

		/* シスパラD_No.16のTxDiv有無設定を判定する時 */
		/* シスパラD_No.16のTxDiv有無設定判定はLREだけ行う */
		if (a_cnt == CMD_NUM15)
		{
			/* 自装置がLREの場合 */
			if (cmw_re_dev_tbl.dev_kind == CMD_DEVKND_LRE2T2R)
			{
				if (( a_sysprm_dt < dlr_sysprmchk[a_num].min ) ||
					( a_sysprm_dt > dlr_sysprmchk[a_num].max ))
				{
					/* 応答結果を情報NGに設定する */

					/* シスパラD_No.16のTxdivが範囲チェックNGの場合の暫定対処 */
					/* ここから */
					/* CPRI信号応答はOKとする。 */
					a_rtn_code = CMD_RES_OK;

					BPF_COM_LOG_ASSERT_DWL( D_RRH_LOG_AST_LV_ERROR, "SYS_PARA NO16_OUTRANGE TYPICAL SET(0x%02x%02x)", dlw_rcvsyspa_tbl_3g[a_cnt * CMD_NUM2], dlw_rcvsyspa_tbl_3g[a_cnt * CMD_NUM2 + CMD_NUM1] );

					/* システムパラメータDのNo.16だけTypical値の2:送信ダイバーシチ無しを設定する */
					/* 範囲チェックOKとなり、システムパラメータ管理テーブル(cmw_sys_mng_tbl)に格納される */
					dlw_rcvsyspa_tbl_3g[a_cnt * CMD_NUM2] = CMD_NUM0;
					dlw_rcvsyspa_tbl_3g[a_cnt * CMD_NUM2 + CMD_NUM1] = CMD_NUM2;
					/* LTEの場合は、変数名EFも範囲チェックするためブレークさせない */

					/* 現状富士通回答はTypical値を設定する。今後発行されるドコモ殿コメントにより */
					/* LTEからのシスパラD_No.16が範囲外の時に、3GからのシスパラD_No.16を有効とする場合は、 */
					/* 上記テーブル設定処理の2行を削除する */
				}
			}
			/* 自装置がLRE以外の場合(RRE,TRA(CPRI),TRA(RF)) */
			else
			{
				/* チェックしない */
			}
		}
		/* シスパラD_No.16のTxDiv有無設定以外を判定する時 */
		else
		{
			if(( a_sysprm_dt < dlr_sysprmchk[a_num].min ) ||
			   ( a_sysprm_dt > dlr_sysprmchk[a_num].max ))
			{
				/* 応答結果を情報NGに設定する */
				a_rtn_code = CMD_INF_NG;
				BPF_COM_LOG_ASSERT_DWL( D_RRH_LOG_AST_LV_ERROR, "SYS_PARA OUTRANGE[%d]=0x%04x", a_cnt, a_sysprm_dt );
				break;
			}
		}
	}

	/* If one parameter is out of range, all parameters will set the default value to the common table */
	if(a_rtn_code == CMD_RES_OK)
	{
		/* システムパラメータのデータ分繰り返す */
		for( a_cnt = CMD_NUM0; a_cnt < DLD_SYSPRAD_NUM; a_cnt++ )
		{
			/* 取得したシスパラ値をSHORT型に変換 */
			a_sysprm_dt = ( dlw_rcvsyspa_tbl_3g[a_cnt * CMD_NUM2] << DLD_BIT8 ) | 
						  ( dlw_rcvsyspa_tbl_3g[a_cnt * CMD_NUM2 + CMD_NUM1] );
			/* the processing about systemparameterD */
			a_num = a_cnt;
			/* システムパラメータ情報管理テーブル設定 */
			cm_MemCpy((VOID*)((UINT)&cmw_sys_mng_tbl[CMD_SYS_3G] + (a_cnt * CMD_NUM2)),
													&a_sysprm_dt, CMD_NUM2);
		}
		
		/* save the system parameter to shared memory */
		efParam = (USHORT*)(&cmw_sys_mng_tbl[CMD_SYS_S3G])+CMD_NUM16;
		apiRtn = rrhApi_Inv_Fsm_SetSystemPara(cmw_sys_mng_tbl,sizeof(cmw_sys_mng_tbl),CMD_NUM0,efParam,CMD_NUM16);
		if(apiRtn != E_API_RCD_OK)
			cm_Assert( CMD_ASL_USELOW, apiRtn, "rrhApi_Inv_Fsm_SetSystemPara NG" ); 
		else
			cm_Assert( CMD_ASL_DBGLOW, apiRtn, "rrhApi_Inv_Fsm_SetSystemPara OK" ); 			
	}
	else
	{
		/* パラメータが範囲外であるとき */
		/* If one of received data is out of range, all received data will be discarded */
		 
		/* remove the system parameter E&F setting processing here				  */
		/* because the system parameter E&F seeting has been done in m_rs_InitNtc */
		/* LTE system paremter downloaded value previously						  */

		/* system paramete D setting processing 	*/
		if(cmw_connectinfo != CMD_CONN_CPRI_SHARED)
		{
			/* LTE single connection case and RF-share connection case		*/
			/* received system parameter is out of range, set the system	*/
			/* parameter D default value to common table					*/
				
				/* システムパラメータのデータ分繰り返す */
			for( a_cnt = CMD_NUM0; a_cnt < DLD_SYSPRAD_NUM; a_cnt++ )
			{
				/* the processing about systemparameterD */
					
					/* システムパラメータ情報管理テーブルにTypical設定値を設定 */
				cm_MemCpy((VOID*)((ULONG)&cmw_sys_mng_tbl[CMD_SYS_3G] + (a_cnt * CMD_NUM2)),
										&dlr_sysprmchk[a_cnt].typical, CMD_NUM2);
			}
		}
	}

	/* シスパラ(S3G)設定 */
	f_cmn_com_syspara_3g_set((T_RRH_SYSPAR_3G*)&cmw_sys_mng_tbl[CMD_SYS_3G]);
	/* set systemparameterD to Hardware */
	m_cm_SysParaDSet(CMD_SYS_3G);

	cmw_SysparaSetting_flag = CMD_OFF;
	
	cm_Assert( CMD_ASL_RETURN, CMD_NUM0,	"[m_dl_SysParaSet_3G] RETURN" );
	
	return a_rtn_code;
}


/* @} */

