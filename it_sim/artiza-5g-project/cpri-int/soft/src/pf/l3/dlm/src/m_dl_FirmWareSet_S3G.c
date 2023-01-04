/*!
 * @skip   $Id$
 * @file   m_dl_FirmWareSet_S3G.c
 * @brief  the processing for firmware setting for S3G.
 * @date   2008/07/29 FFCS)Wuh Create for eNB-008-001.
 * @date   2009/03/05  FFCS)Wuh modify for M-S3G-eNBPF-01471
 *                          stop writing FLASH after receiving CPRI message
 * @date   2009/04/13  FFCS)Wuh modify for S3G PRC CR-xxx-xxx (CPRI spec V1.07)
 * @date   2009/04/30  FFCS)Wuh modify for CR-xxx-xxx
 *                      move the process of FLASH erasing to file data sending
 * @date   2010/05/06  FJT)Otani  CR-00091-001:[DCM向け]REハード・ソフトインタフェース仕様書v2.18版リリース
 * @date   2011/02/25  FJT)Tokunaga  modify for ECO-800M-RRE / 1.5G-RRE対応
 * @date   2011/09/16  CMS)Uchida    modify for 11B-02-005：VA提案の2G/800MのSLCアーキ適用対応
 * @date   2015/09/29  TDIPS)sasaki Update for FHM
 * @date   2015/09/30  TDIPS)sasaki Update for FHM
 * @date   2015/10/26  TDIPS)sasaki Update
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */	
#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */
/*!
 *  @brief  the function processing for firmware setting.
 *  @note   This function is processed as follows.
 *          1) write soft version to common table
 *          2) write firmware to FLASH
 *          3) write firmware's year,month,day to EEPROM
 *          4) write FPGA to FLASH
 *          5) write FPGA's year,month,day to EEPROM 
 *          6) write APD to FLASH if REC is S3G
 *  @param  a_down_typ			[in]	the download type(startup download or USE download)
 *  @param  checksum            [in]    check sum
 *  @param  time                [in]    wait time
 *  @return Result code is returned.
 *  @retval D_OK	0:success 
 *  @retval CMD_REFW_NG	1:error
 *  @date   2008/07/30 FFCS)Wuh Create.
 *  @date   2009/01/16 FFCS)Wuh M-S3G-eNBPF-01078 if LRE 1TX,don't write the branch 1 of VUPU
 *  @date	2009/01/28 FFCS)wuh M-S3G-eNBPF-01235
 *                          change the rule of mkj judgment
 *  @date   2010/05/06 FJT)Otani  CR-00091-001:[DCM向け]REハード・ソフトインタフェース仕様書v2.18版リリース
 *  @date   2011/02/25 FJT)Tokunaga  modify for ECO-800M-RRE / 1.5G-RRE対応
 *  @date   2011/09/16 CMS)Uchida    modify for 11B-02-005：VA提案の2G/800MのSLCアーキ適用対応
 *  @date   2012/05/01 FJT)Tokunaga  modify for CR-00060-000;TRA CPRI CDカード対応
 *  @date   2015/09/29 TDIPS)sasaki Update MKレビュー指摘No.197対応
 *  @date   2015/09/30 TDIPS)sasaki Update REファイル書込み位置算出ロジック実装
 *  @date   2015/10/26 TDIPS)sasaki Update IT2問処No.145対処
 */
UINT m_dl_FirmWareSet_S3G( UINT a_down_typ ,USHORT checksum, UINT time)
{
	T_RRH_FW_INFO	fwInfo;
	UINT			ret;
	UINT			idx;
	USHORT			fwtype = D_API_FILE_FW_TYPE_FHM;
	USHORT 			savepos = 0;
	USHORT 			emptypos = 0;
	USHORT 			replacepos = 0;
	ULONG			savedate = (9999 * 10000) + (99 * 100) + 99;
	ULONG			date;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_FirmWareSet_S3G] ENTER" );

	memcpy(&fwInfo, &gt_dlw_mnt_reinf[CMD_NUM0], sizeof(T_RRH_FW_INFO));

	if(gw_dl_FileType == DLD_FILENAME_RE)
	{
		fwtype = D_API_FILE_FW_TYPE_RE;

		for(idx = D_RRH_CPRINO_RE_MAX; idx >= D_RRH_CPRINO_RE_MIN; idx--)
		{
			/* FHM保持ファイル情報に該当メーカ識別あり */
			if(gt_dlw_mnt_reinf[idx].mk_jdg == fwInfo.maker_id)
			{
				savepos = idx;
				break;
			}

			/* 空のFHM保持ファイル情報領域 */
			if(gt_dlw_mnt_reinf[idx].mk_jdg == 0)
			{
				emptypos = idx;
			}
			/* 配下に接続されていないメーカ識別のFWファイル情報 */
			else if(gt_dlw_mnt_reinf[idx].re_conn_exist == CMD_OFF)
			{
				date = (gt_dlw_mnt_reinf[idx].year * 10000) + (gt_dlw_mnt_reinf[idx].month * 100) + (gt_dlw_mnt_reinf[idx].day);
				if(date <= savedate)
				{
					replacepos = idx;
					savedate = date;
				}
			}
//			printf("[%d]%s gt_dlw_mnt_reinf[%d].mk_jdg[%d] savepos[%d] emptypos[%d] replacepos[%d]\n", 
//				__LINE__,__FUNCTION__, idx, gt_dlw_mnt_reinf[idx].mk_jdg, savepos, emptypos, replacepos);
		}

		/* FHM保持ファイル情報に該当メーカ識別なし */
		if(idx == 0)
		{
			/* 空の領域か、配下に接続されていないメーカ識別のFWファイル情報は必ず1つは存在する筈 */
			savepos = emptypos ? emptypos : replacepos;
		}
	}

	/** DCM Zynq: call API to pf_fmng for erasing/writing **/
	ret = rrhApi_File_Mnr_SaveFirm( D_RRH_PROCQUE_L3, a_down_typ, checksum, fwtype, savepos, &fwInfo);

	if(ret == D_RRH_OK)
	{
		/* flash is writing */
		gt_dlw_dlm_tbl.write_flg = CMD_NUM1;
		gt_dlw_dlm_tbl.dlrefile_inf = savepos;
	}

	cm_Assert( CMD_ASL_RETURN, ret,	"[m_dl_FirmWareSet_S3G] RETURN" );
		
	return ret;
}
/* @} */

