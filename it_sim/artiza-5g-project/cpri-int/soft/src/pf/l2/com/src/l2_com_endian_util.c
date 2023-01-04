/*!
 * @skip  $ld:$
 * @file  l2_com_endian_util.c
 * @brief L2 endian convert functions
 * @date  2013/11/20 FFCS)hongj Create for zynq\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
 
/*!
 * @addtogroup RRH_L2
 * @{
 */

#include "l2_com_inc.h"

/********************************************************************************************************************/
/*!
 *  @brief  Big endian to little endian
 *  @note   Big endian to little endiann for USHORT
 *  @param  target [in]  needed to convert
 *  @retval be converted value
 *  @date   2013/11/18 FFCS)hong Create for zynq
 */
 /********************************************************************************************************************/
USHORT l2_com_swap_USHORT(USHORT target)
{
    return BigLittleSwap16(target);
}

/********************************************************************************************************************/
/*!
 *  @brief  Little endian to big endian
 *  @note   Little endian to big endian for UINT
 *  @param  target [in]  needed to convert
 *  @retval be converted value
 *  @date   2013/11/18 FFCS)hong Create for zynq
 */
 /********************************************************************************************************************/
UINT l2_com_swap_UINT(UINT target)
{
    return BigLittleSwap32(target);
}

#define L2_SWAP_USHORT(a) (a) = (l2_com_swap_USHORT((a)))
#define L2_SWAP_UINT(a)   (a) = (l2_com_swap_UINT((a)))
/********************************************************************************************************************/
/*!
 *  @brief  Little endian to big endian
 *  @note   Little endian to big endian subroutine
 *  @param  linkno [in]  CPRI link number
 *  @param  signal_kind [in]  cpri signal kind
 *  @param  *body [in]  Pointer of cpri message body
 *  @param  direction [in]  direction
 *  @param  l3_len [in] L3 message length
 *  @date   2013/11/18 FFCS)hongj Create for zynq
 *  @date   2015/8/6 TDIPS)ikeda rev.27892 L2 LPB
 *  @date   2015/09/11 TDI)satou ファームウェアファイル報告応答2を追加
 *  @date   2015/09/11 TDI)satou フォーマットが3G/LTEで異なるため処理を分岐
 *                               - ファイルデータ送信完了報告応答
 *                               - TRX設定要求
 *                               - 無変調キャリアON/OFF指定要求
 *                               - 無変調キャリアON/OFF指定応答
 *                               - RE装置構成情報報告応答
 *                               - REカード状態報告応答
 *                               - FLD-MTデータファイル転送送信要求
 * @date    2015/09/19 TDI)satou 可変パラメータのエンディアン変換見直し
 * @date    2015/09/20 TDI)satou REスロット情報報告応答の応答結果以降のパラメータについて変換処理を追加
 * @date    2015/10/24 TDIPS)sasaki Warning対処
 */
 /********************************************************************************************************************/

void l2_com_swap_convert(USHORT linkno, USHORT signal_kind, VOID *body, CHAR direction, UINT l3_len)
{
	int num;
	USHORT* pdat;
	USHORT lng;
	if(body == D_RRH_NULL)
	{
		cm_Assert( CMD_ASL_DBGLOW, (UINT)body,
			(CHAR*)"[l2_com_swap_convert] body is NULL" );
		return;
	}

	USHORT system_type;
	system_type = signal_kind & 0x000F;
	T_RRH_CPRI_STATE 			ltCpriSta;   /* CPRIState */

	switch(signal_kind & 0xFFF0)
	{
	/* ヘルスチェック要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_HCREQ:
		((CMT_CPRIF_HCREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_HCREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
/*	性能改善のため一旦止血	*/
//		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_HCREQ, CMD_NML);
		break;

	/* ヘルスチェック応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_HCRES:
		((CMT_CPRIF_HCRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_HCRES *)body)->signal_kind);
		/* lblap_statis_for_l3 */
/*	性能改善のため一旦止血	*/
//		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_HCRES, CMD_NML);
		break;

	/* REリセット要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_RERSTREQ:
		((CMT_CPRIF_RERSTREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_RERSTREQ *)body)->signal_kind);
		((CMT_CPRIF_RERSTREQ *)body)->kind = l2_com_swap_USHORT(((CMT_CPRIF_RERSTREQ *)body)->kind);
		((CMT_CPRIF_RERSTREQ *)body)->recomapp = l2_com_swap_USHORT(((CMT_CPRIF_RERSTREQ *)body)->recomapp);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_RERSTREQ, CMD_NML);
		break;

	/* RE時刻設定要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_RETIMSETREQ:
		((CMT_CPRIF_RETIMSETREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_RETIMSETREQ *)body)->signal_kind);
		((CMT_CPRIF_RETIMSETREQ *)body)->year = l2_com_swap_USHORT(((CMT_CPRIF_RETIMSETREQ *)body)->year);
		((CMT_CPRIF_RETIMSETREQ *)body)->month_day = l2_com_swap_USHORT(((CMT_CPRIF_RETIMSETREQ *)body)->month_day);
		((CMT_CPRIF_RETIMSETREQ *)body)->hour = l2_com_swap_USHORT(((CMT_CPRIF_RETIMSETREQ *)body)->hour);
		((CMT_CPRIF_RETIMSETREQ *)body)->minute_sec = l2_com_swap_USHORT(((CMT_CPRIF_RETIMSETREQ *)body)->minute_sec);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_RETIMSETREQ, CMD_NML);
		break;

	/* RE共通部リンク確立要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_LINKSTSREQ:
		((CMT_CPRIF_LINKSTSREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_LINKSTSREQ *)body)->signal_kind);
		((CMT_CPRIF_LINKSTSREQ *)body)->link = l2_com_swap_USHORT(((CMT_CPRIF_LINKSTSREQ *)body)->link);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_LINKSTSREQ, CMD_NML);
		break;

	/* RE共通部リンク確立応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_LINKSTSRES:
		((CMT_CPRIF_LINKSTSRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_LINKSTSRES *)body)->signal_kind);
		((CMT_CPRIF_LINKSTSRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_LINKSTSRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_LINKSTSRES, ((CMT_CPRIF_LINKSTSRES *)body)->result);
		break;

	/* ファームウェアファイル報告要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FIRMFILEREQ:
		((CMT_CPRIF_FIRMFILEREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FIRMFILEREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FIRMFILEREQ, CMD_NML);
		break;

	/* ファームウェアファイル報告応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FIRMFILERES:
		((CMT_CPRIF_FIRMFILERES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FIRMFILERES *)body)->signal_kind);
		((CMT_CPRIF_FIRMFILERES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_FIRMFILERES *)body)->result);
		((CMT_CPRIF_FIRMFILERES *)body)->firmfile.makername = l2_com_swap_USHORT(((CMT_CPRIF_FIRMFILERES *)body)->firmfile.makername);
		((CMT_CPRIF_FIRMFILERES *)body)->firmfile.mjr_ver = l2_com_swap_USHORT(((CMT_CPRIF_FIRMFILERES *)body)->firmfile.mjr_ver);
		((CMT_CPRIF_FIRMFILERES *)body)->firmfile.mir_ver = l2_com_swap_USHORT(((CMT_CPRIF_FIRMFILERES *)body)->firmfile.mir_ver);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FIRMFILERES, ((CMT_CPRIF_FIRMFILERES *)body)->result);
		break;

	/* ファームウェアファイル報告応答2 : LTEのみ */
	case CMD_CPRID_FIRMFILERES2:
		/* 可変長 */
	    if ('S' == direction) {
	        lng = ((CMT_CPRIF_FIRMFILERES2*)body)->verinf_num;
	    } else {
	        lng = l2_com_swap_USHORT(((CMT_CPRIF_FIRMFILERES2*)body)->verinf_num);
	    }

		for (num = 0; num < lng; num++)
		{
			L2_SWAP_USHORT(((CMT_CPRIF_FIRMFILERES2*)body)->firmfile[num].makername);
			L2_SWAP_USHORT(((CMT_CPRIF_FIRMFILERES2*)body)->firmfile[num].mir_ver);
			L2_SWAP_USHORT(((CMT_CPRIF_FIRMFILERES2*)body)->firmfile[num].mjr_ver);
		}

		L2_SWAP_USHORT(((CMT_CPRIF_FIRMFILERES2*)body)->signal_kind);
		L2_SWAP_USHORT(((CMT_CPRIF_FIRMFILERES2*)body)->result     );
		L2_SWAP_USHORT(((CMT_CPRIF_FIRMFILERES2*)body)->verinf_num );
        /* lblap_statis_for_l3 */
        lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FIRMFILERES2, ((CMT_CPRIF_FIRMFILERES2 *)body)->result);
		break;

	/* ファイル情報報告要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FILEINFREQ:
		((CMT_CPRIF_FILEINFREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FILEINFREQ *)body)->signal_kind);
		((CMT_CPRIF_FILEINFREQ *)body)->data = l2_com_swap_USHORT(((CMT_CPRIF_FILEINFREQ *)body)->data);
		((CMT_CPRIF_FILEINFREQ *)body)->size = l2_com_swap_UINT(((CMT_CPRIF_FILEINFREQ *)body)->size);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FILEINFREQ, CMD_NML);
		break;

	/* ファイル情報報告応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FILEINFRES:
		((CMT_CPRIF_FILEINFRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FILEINFRES *)body)->signal_kind);
		((CMT_CPRIF_FILEINFRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_FILEINFRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FILEINFRES, ((CMT_CPRIF_FILEINFRES *)body)->result);
		break;

	/* ファイルデータ送信 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FILEDATSND:
		((CMT_CPRIF_FILEDATSND *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FILEDATSND *)body)->signal_kind);
		((CMT_CPRIF_FILEDATSND *)body)->sgmtno1 = l2_com_swap_USHORT(((CMT_CPRIF_FILEDATSND *)body)->sgmtno1);
		((CMT_CPRIF_FILEDATSND *)body)->sgmtno2 = l2_com_swap_USHORT(((CMT_CPRIF_FILEDATSND *)body)->sgmtno2);
		((CMT_CPRIF_FILEDATSND *)body)->datsize1 = l2_com_swap_USHORT(((CMT_CPRIF_FILEDATSND *)body)->datsize1);
		((CMT_CPRIF_FILEDATSND *)body)->datsize2 = l2_com_swap_USHORT(((CMT_CPRIF_FILEDATSND *)body)->datsize2);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FILEDATSND, CMD_NML);
		break;

	/* ファイルデータ送信完了報告通知 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FILSNDFINNTC:
		((CMT_CPRIF_FILSNDFINNTC *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FILSNDFINNTC *)body)->signal_kind);
		((CMT_CPRIF_FILSNDFINNTC *)body)->tgtdat = l2_com_swap_USHORT(((CMT_CPRIF_FILSNDFINNTC *)body)->tgtdat);
		((CMT_CPRIF_FILSNDFINNTC *)body)->chksum = l2_com_swap_USHORT(((CMT_CPRIF_FILSNDFINNTC *)body)->chksum);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FILSNDFINNTC, CMD_NML);
		break;

	/* ファイルデータ送信完了報告応答 : 3GとLTEでフォーマットが異なる */
    case CMD_CPRID_FILSNDFINRES:
        if (CMD_SYS_3G == system_type) {
            L2_SWAP_USHORT(((CMT_CPRIF_FILSNDFINRES*)body)->signal_kind);
            L2_SWAP_USHORT(((CMT_CPRIF_FILSNDFINRES*)body)->result     );
            /* lblap_statis_for_l3 */
            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FILSNDFINRES, ((CMT_CPRIF_FILSNDFINRES*)body)->result);
        } else {
            L2_SWAP_USHORT(((CMT_CPRIF_FILSNDFINRES_S3G *)body)->signal_kind);
            L2_SWAP_USHORT(((CMT_CPRIF_FILSNDFINRES_S3G *)body)->result     );
            L2_SWAP_USHORT(((CMT_CPRIF_FILSNDFINRES_S3G *)body)->primary_sys);
            /* lblap_statis_for_l3 */
            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FILSNDFINRES, ((CMT_CPRIF_FILSNDFINRES_S3G *)body)->result);
	    }
		break;

    /* 運用中ファームウェアファイル報告要求 : LTEのみ */
	case CMD_CPRID_UFIRMFILEREQ:
		((CMT_CPRIF_UFIRMFILEREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_UFIRMFILEREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_UFIRMFILEREQ, CMD_NML);
		break;

    /* 運用中ファームウェアファイル報告応答 : LTEのみ */
	case CMD_CPRID_UFIRMFILERES:
		((CMT_CPRIF_UFIRMFILERES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_UFIRMFILERES *)body)->signal_kind);
		((CMT_CPRIF_UFIRMFILERES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_UFIRMFILERES *)body)->result);
		((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.makername = l2_com_swap_USHORT(((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.makername);
		((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.cur_mjr_ver = l2_com_swap_USHORT(((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.cur_mjr_ver);
		((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.cur_mir_ver = l2_com_swap_USHORT(((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.cur_mir_ver);
		((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.nes_mjr_ver = l2_com_swap_USHORT(((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.nes_mjr_ver);
		((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.nes_mir_ver = l2_com_swap_USHORT(((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.nes_mir_ver);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_UFIRMFILERES, ((CMT_CPRIF_UFIRMFILERES *)body)->result);
		break;

	/* 運用中ファームウェアファイル報告応答2 : LTEのみ */
	case CMD_CPRID_UFIRMFILERES2:
	    pdat = (USHORT*)body;
	    for (num = 0; num < (l3_len / sizeof(USHORT)); num++, pdat++) {
	        L2_SWAP_USHORT(*pdat);
	    }

		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_UFIRMFILERES2, ((CMT_CPRIF_UFIRMFILERES *)body)->result);
		break;

    /* 運用中ファイル情報報告要求 : LTEのみ */
	case CMD_CPRID_UFILEINFREQ:
		((CMT_CPRIF_UFILEINFREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_UFILEINFREQ *)body)->signal_kind);
		((CMT_CPRIF_UFILEINFREQ *)body)->data = l2_com_swap_USHORT(((CMT_CPRIF_UFILEINFREQ *)body)->data);
		((CMT_CPRIF_UFILEINFREQ *)body)->size = l2_com_swap_UINT(((CMT_CPRIF_UFILEINFREQ *)body)->size);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_UFILEINFREQ, CMD_NML);
		break;

    /* 運用中ファイル情報報告応答 : LTEのみ */
	case CMD_CPRID_UFILEINFRES:
		((CMT_CPRIF_UFILEINFRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_UFILEINFRES *)body)->signal_kind);
		((CMT_CPRIF_UFILEINFRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_UFILEINFRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_UFILEINFRES, ((CMT_CPRIF_UFILEINFRES *)body)->result);
		break;

    /* 運用中ファイルデータ送信 : LTEのみ */
	case CMD_CPRID_UFILEDATSND:
		((CMT_CPRIF_UFILEDATSND *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_UFILEDATSND *)body)->signal_kind);
		((CMT_CPRIF_UFILEDATSND *)body)->sgmtno1 = l2_com_swap_USHORT(((CMT_CPRIF_UFILEDATSND *)body)->sgmtno1);
		((CMT_CPRIF_UFILEDATSND *)body)->sgmtno2 = l2_com_swap_USHORT(((CMT_CPRIF_UFILEDATSND *)body)->sgmtno2);
		((CMT_CPRIF_UFILEDATSND *)body)->datsize1 = l2_com_swap_USHORT(((CMT_CPRIF_UFILEDATSND *)body)->datsize1);
		((CMT_CPRIF_UFILEDATSND *)body)->datsize2 = l2_com_swap_USHORT(((CMT_CPRIF_UFILEDATSND *)body)->datsize2);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_UFILEDATSND, CMD_NML);
		break;

    /* 運用中ファイルデータ送信完了報告通知 : LTEのみ */
	case CMD_CPRID_UFILSNDFINNTC:
		((CMT_CPRIF_UFILSNDFINNTC *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_UFILSNDFINNTC *)body)->signal_kind);
		((CMT_CPRIF_UFILSNDFINNTC *)body)->tgtdat = l2_com_swap_USHORT(((CMT_CPRIF_UFILSNDFINNTC *)body)->tgtdat);
		((CMT_CPRIF_UFILSNDFINNTC *)body)->chksum = l2_com_swap_USHORT(((CMT_CPRIF_UFILSNDFINNTC *)body)->chksum);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_UFILSNDFINNTC, CMD_NML);
		break;

    /* 運用中ファイルデータ送信完了報告応答 : LTEのみ */
	case CMD_CPRID_UFILSNDFINRES:
		((CMT_CPRIF_UFILSNDFINRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_UFILSNDFINRES *)body)->signal_kind);
		((CMT_CPRIF_UFILSNDFINRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_UFILSNDFINRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_UFILSNDFINRES, ((CMT_CPRIF_UFILSNDFINRES *)body)->result);
		break;

    /* TRX設定要求 : 3GとLTEで異なるフォーマット */
	case CMD_CPRID_TRXSETREQ:
        if (CMD_SYS_3G == system_type) {
            L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ*)body)->signal_kind);
            L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ*)body)->carno      );
            L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ*)body)->frqno      );
            /* lblap_statis_for_l3 */
            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_TRXSETREQ, CMD_NML);
        } else {
        	f_cmn_com_cpri_state_get( &ltCpriSta );
			if(D_RRH_LINE_BITRATE_24G == ltCpriSta.cpri_bitrate)
			{
				L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ_S3G_FDD*)body)->signal_kind);
	            L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ_S3G_FDD*)body)->carno      );
	            L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ_S3G_FDD*)body)->bandwidth  );
	            L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ_S3G_FDD*)body)->dl_freq    );
	            L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ_S3G_FDD*)body)->snd_br     );
			}
        	else
        	{
	            L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ_S3G*)body)->signal_kind      );
	            L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ_S3G*)body)->carno            );
	            L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ_S3G*)body)->bandwidth        );
	            L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ_S3G*)body)->dl_freq          );
	            L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ_S3G*)body)->snd_br           );
	            L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ_S3G*)body)->tdd_up_dwn_config);
	            L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ_S3G*)body)->tdd_spe_sf_config);
	            L2_SWAP_USHORT(((CMT_CPRIF_TRXSETREQ_S3G*)body)->tdd_dwnlk_cp_len );
        	}
            /* lblap_statis_for_l3 */
            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_TRXSETREQ, CMD_NML);
        }
        break;

    /* TRX設定応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_TRXSETRES:
		((CMT_CPRIF_TRXSETRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_TRXSETRES *)body)->signal_kind);
		((CMT_CPRIF_TRXSETRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_TRXSETRES *)body)->result);
		((CMT_CPRIF_TRXSETRES *)body)->carno = l2_com_swap_USHORT(((CMT_CPRIF_TRXSETRES *)body)->carno);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_TRXSETRES, ((CMT_CPRIF_TRXSETRES *)body)->result);
		break;

	/* TRX解放要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_TRXRELREQ:
		((CMT_CPRIF_TRXRELREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_TRXRELREQ *)body)->signal_kind);
		((CMT_CPRIF_TRXRELREQ *)body)->carno = l2_com_swap_USHORT(((CMT_CPRIF_TRXRELREQ *)body)->carno);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_TRXRELREQ, CMD_NML);
		break;

	/* TRX解放応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_TRXRELRES:
		((CMT_CPRIF_TRXRELRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_TRXRELRES *)body)->signal_kind);
		((CMT_CPRIF_TRXRELRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_TRXRELRES *)body)->result);
		((CMT_CPRIF_TRXRELRES *)body)->carno = l2_com_swap_USHORT(((CMT_CPRIF_TRXRELRES *)body)->carno);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_TRXRELRES, ((CMT_CPRIF_TRXRELRES *)body)->result);
		break;

	/* Toffset報告要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_TOFFSETINFREQ:
		((CMT_CPRIF_TOFFSETINFREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_TOFFSETINFREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
/*	性能改善のため一旦REC側を止血	*/
		if(linkno != D_RRH_CPRINO_REC)
		{
			lblap_statis_for_l3(linkno, system_type, D_L2_STATI_TOFFSETINFREQ, CMD_NML);
		}
		break;

	/* Toffset報告応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_TOFFSETINFRES:
		((CMT_CPRIF_TOFFSETINFRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_TOFFSETINFRES *)body)->signal_kind);
		((CMT_CPRIF_TOFFSETINFRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_TOFFSETINFRES *)body)->result);
		((CMT_CPRIF_TOFFSETINFRES *)body)->toffset_inf.toffset = l2_com_swap_USHORT(((CMT_CPRIF_TOFFSETINFRES *)body)->toffset_inf.toffset);
		((CMT_CPRIF_TOFFSETINFRES *)body)->toffset_inf.updeleqp = l2_com_swap_USHORT(((CMT_CPRIF_TOFFSETINFRES *)body)->toffset_inf.updeleqp);
		((CMT_CPRIF_TOFFSETINFRES *)body)->toffset_inf.downdeleqp = l2_com_swap_USHORT(((CMT_CPRIF_TOFFSETINFRES *)body)->toffset_inf.downdeleqp);
		/* lblap_statis_for_l3 */
/*	性能改善のため一旦REC側を止血	*/
		if(linkno != D_RRH_CPRINO_REC)
		{
			lblap_statis_for_l3(linkno, system_type, D_L2_STATI_TOFFSETINFRES, ((CMT_CPRIF_TOFFSETINFRES *)body)->result);
		}
		break;

    /* 無変調キャリアON/OFF指定要求 : 3GとLTEでフォーマットが異なる */
    case CMD_CPRID_CARONOFFREQ:
		printf("\n[INFO] [L2] [%s:%d] Enter case CMD_CPRID_CARONOFFREQ (0x4411)", __FILE__, __LINE__);
        if (CMD_SYS_3G == system_type) {
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFREQ*)body)->signal_kind);
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFREQ*)body)->carno      );
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFREQ*)body)->br_app     );
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFREQ*)body)->onoff_app  );
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFREQ*)body)->sndpwr     );
            /* lblap_statis_for_l3 */
            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_CARONOFFREQ, CMD_NML);
        } else {
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFREQ_S3G*)body)->signal_kind);
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFREQ_S3G*)body)->carno      );
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFREQ_S3G*)body)->onoff_app  );
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFREQ_S3G*)body)->sndpwr     );
            /* lblap_statis_for_l3 */
            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_CARONOFFREQ, CMD_NML);
        }
        break;

    /* 無変調キャリアON/OFF指定応答 : 3GとLTEでフォーマットが異なる */
    case CMD_CPRID_CARONOFFRES:
		printf("\n[INFO] [L2] [%s:%d] Enter case CMD_CPRID_CARONOFFRES (0x4421)", __FILE__, __LINE__);
        if (CMD_SYS_3G == system_type) {
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFRES*)body)->signal_kind);
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFRES*)body)->result     );
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFRES*)body)->carno      );
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFRES*)body)->br_app     );
            /* lblap_statis_for_l3 */
            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_CARONOFFRES, ((CMT_CPRIF_CARONOFFRES*)body)->result);
        } else {
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFRES_S3G*)body)->signal_kind);
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFRES_S3G*)body)->result     );
            L2_SWAP_USHORT(((CMT_CPRIF_CARONOFFRES_S3G*)body)->carno      );
            /* lblap_statis_for_l3 */
            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_CARONOFFRES, ((CMT_CPRIF_CARONOFFRES_S3G *)body)->result);
        }
        break;

    /* RE装置構成情報報告要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_EQPSTRREQ:
		((CMT_CPRIF_EQPSTRREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_EQPSTRREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
/*	性能改善のため一旦REC側を止血	*/
		if(linkno != D_RRH_CPRINO_REC)
		{
			lblap_statis_for_l3(linkno, system_type, D_L2_STATI_EQPSTRREQ, CMD_NML);
		}
		break;
		

    /* RE装置構成情報報告応答 : 3GとLTEでフォーマットが異なる */
    case CMD_CPRID_EQPSTRRES:
        if (CMD_SYS_3G == system_type) {
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->signal_kind               );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->result                    );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->cpristr_inf.maker         );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->cpristr_inf.bts_no        );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->cpristr_inf.rekind        );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->cpristr_inf.eqp_no        );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->cpristr_inf.powerkind     );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->cpristr_inf.powerunit     );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->cpristr_inf.powerclass_0  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->cpristr_inf.powerclass_1  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->cpristr_inf.frqband       );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->cpristr_inf.us800m_fil_ytp);
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->cpristr_inf.us2g_car_no   );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->cpristr_inf.us800m_car_no );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->cpristr_inf.us17g_car_no  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->cpristr_inf.trx_inf_type  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES*)body)->cpristr_inf.us15g_car_no  );
            /* lblap_statis_for_l3 */
/*	性能改善のため一旦REC側を止血	*/
			if(linkno != D_RRH_CPRINO_REC)
			{
	            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_EQPSTRRES, ((CMT_CPRIF_EQPSTRRES*)body)->result);
	        }
        } else {
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->signal_kind               );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->result                    );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.maker         );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.bts_no        );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.eqp_no        );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.rekind        );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.bts_share_type);
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.powerkind     );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.rcv_antena_no );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.snd_antena_no );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.powerunit     );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.powerclass_0  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.powerclass_1  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.powerclass_2  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.powerclass_3  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.frqband       );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.max_bandwidth );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.us2g_car_no   );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.us15g_car_no  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.us800m_car_no );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.us700m_car_no );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.us17g_car_no  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.dummy3[0]     );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.trx_inf_type  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.us35g_car_no  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.powerclass_4  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.powerclass_5  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.powerclass_6  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.powerclass_7  );
            L2_SWAP_USHORT(((CMT_CPRIF_EQPSTRRES_S3G*)body)->cpristr_inf.us34g_car_no  );
            /* lblap_statis_for_l3 */
/*	性能改善のため一旦REC側を止血	*/
			if(linkno != D_RRH_CPRINO_REC)
			{
	            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_EQPSTRRES, ((CMT_CPRIF_EQPSTRRES_S3G *)body)->result);
	        }
        }
        break;

    /* REスロット情報報告要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_SLOTINFREQ:
		((CMT_CPRIF_SLOTINFREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_SLOTINFREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_SLOTINFREQ, CMD_NML);
		break;

	/* REスロット情報報告応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_SLOTINFRES:
	    pdat = (USHORT*)body;
	    lng = l3_len / sizeof(USHORT);
	    for (num = 0; num < lng; num++, pdat++) {
	        L2_SWAP_USHORT(*pdat);
	    }
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_SLOTINFRES, ((CMT_CPRIF_SLOTINFRES *)body)->result);
		break;

	/* RE状態報告要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_RESTSREQ:
		((CMT_CPRIF_RESTSREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_RESTSREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
/*	性能改善のため一旦止血	*/
//		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_RESTSREQ, CMD_NML);
		break;

	/* RE状態報告応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_RESTSRES:
	    pdat = (USHORT*)body;
	    for (num = 0; num < (l3_len / sizeof(USHORT)); num++, pdat++) {
	        L2_SWAP_USHORT(*pdat);
	    }

		/* lblap_statis_for_l3 */
/*	性能改善のため一旦止血	*/
//		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_RESTSRES, ((CMT_CPRIF_RESTSRES *)body)->result);
		break;

	/* REカード情報報告要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_CARDINFREQ:
		((CMT_CPRIF_CARDINFREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_CARDINFREQ *)body)->signal_kind);
		((CMT_CPRIF_CARDINFREQ *)body)->slot_no = l2_com_swap_USHORT(((CMT_CPRIF_CARDINFREQ *)body)->slot_no);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_CARDINFREQ, CMD_NML);
		break;

	/* REカード情報報告応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_CARDINFRES:
		((CMT_CPRIF_CARDINFRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->signal_kind);
		((CMT_CPRIF_CARDINFRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->result);
		((CMT_CPRIF_CARDINFRES *)body)->slot_no = l2_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->slot_no);
		((CMT_CPRIF_CARDINFRES *)body)->up_resrc_num1 = l2_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->up_resrc_num1);
		((CMT_CPRIF_CARDINFRES *)body)->up_resrc_num2 = l2_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->up_resrc_num2);
		((CMT_CPRIF_CARDINFRES *)body)->down_resrc_num1 = l2_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->down_resrc_num1);
		((CMT_CPRIF_CARDINFRES *)body)->down_resrc_num2 = l2_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->down_resrc_num2);
		((CMT_CPRIF_CARDINFRES *)body)->card_svitem = l2_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->card_svitem);
		((CMT_CPRIF_CARDINFRES *)body)->card_extitem = l2_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->card_extitem);
		((CMT_CPRIF_CARDINFRES *)body)->card_cntitem = l2_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->card_cntitem);
		((CMT_CPRIF_CARDINFRES *)body)->funcitem = l2_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->funcitem);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_CARDINFRES, ((CMT_CPRIF_CARDINFRES *)body)->result);
		break;

	/* REバージョン情報報告要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_REVERREQ:
		((CMT_CPRIF_REVERREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_REVERREQ *)body)->signal_kind);
		((CMT_CPRIF_REVERREQ *)body)->tgtkind = l2_com_swap_USHORT(((CMT_CPRIF_REVERREQ *)body)->tgtkind);
		((CMT_CPRIF_REVERREQ *)body)->slot_no = l2_com_swap_USHORT(((CMT_CPRIF_REVERREQ *)body)->slot_no);
		((CMT_CPRIF_REVERREQ *)body)->id_kind = l2_com_swap_USHORT(((CMT_CPRIF_REVERREQ *)body)->id_kind);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_REVERREQ, CMD_NML);
		break;

	/* REバージョン情報報告応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_REVERRES:
		pdat = (USHORT*)&((char*)body)[sizeof(CMT_CPRIF_REVERRES)];

		if ('S' == direction) {
		    lng = ((CMT_CPRIF_REVERRES *)body)->rests_inf.inf_num;
		} else {
		    lng = l2_com_swap_USHORT(((CMT_CPRIF_REVERRES *)body)->rests_inf.inf_num);
		}

		/* 可変長 */
		for (num = 0; num < lng; num++)
		{
			L2_SWAP_USHORT(pdat[5]);
			L2_SWAP_USHORT(pdat[6]);
			L2_SWAP_USHORT(pdat[7]);
			L2_SWAP_USHORT(pdat[8]);
			pdat = &pdat[9];
		}

		((CMT_CPRIF_REVERRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_REVERRES *)body)->signal_kind);
		((CMT_CPRIF_REVERRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_REVERRES *)body)->result);
		((CMT_CPRIF_REVERRES *)body)->rests_inf.kind = l2_com_swap_USHORT(((CMT_CPRIF_REVERRES *)body)->rests_inf.kind);
		((CMT_CPRIF_REVERRES *)body)->rests_inf.slot_no = l2_com_swap_USHORT(((CMT_CPRIF_REVERRES *)body)->rests_inf.slot_no);
		((CMT_CPRIF_REVERRES *)body)->rests_inf.inf_num = l2_com_swap_USHORT(((CMT_CPRIF_REVERRES *)body)->rests_inf.inf_num);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_REVERRES, ((CMT_CPRIF_REVERRES *)body)->result);
		break;

    /* REカード制御要求 : 3GとLTEでフォーマットが異なる */
    case CMD_CPRID_RECARDCNTREQ:
        if (CMD_SYS_3G == system_type) {
            L2_SWAP_USHORT(((CMT_CPRIF_RECARDCNTREQ*)body)->signal_kind);
            L2_SWAP_USHORT(((CMT_CPRIF_RECARDCNTREQ*)body)->slot_no    );
            L2_SWAP_USHORT(((CMT_CPRIF_RECARDCNTREQ*)body)->cnt_kind   );
            L2_SWAP_USHORT(((CMT_CPRIF_RECARDCNTREQ*)body)->cnt_code   );
            /* lblap_statis_for_l3 */
            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_RECARDCNTREQ, CMD_NML);
        } else {
            L2_SWAP_USHORT(((CMT_CPRIF_RECARDCNTREQ_S3G*)body)->signal_kind);
            L2_SWAP_USHORT(((CMT_CPRIF_RECARDCNTREQ_S3G*)body)->slot_no    );
            L2_SWAP_USHORT(((CMT_CPRIF_RECARDCNTREQ_S3G*)body)->cnt_kind   );
            /* lblap_statis_for_l3 */
            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_RECARDCNTREQ, CMD_NML);
        }
        break;

    /* REカード制御応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_RECARDCNTRES:
		((CMT_CPRIF_RECARDCNTRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_RECARDCNTRES *)body)->signal_kind);
		((CMT_CPRIF_RECARDCNTRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_RECARDCNTRES *)body)->result);
		((CMT_CPRIF_RECARDCNTRES *)body)->slot_no = l2_com_swap_USHORT(((CMT_CPRIF_RECARDCNTRES *)body)->slot_no);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_RECARDCNTRES, ((CMT_CPRIF_RECARDCNTRES *)body)->result);
		break;

	/* REカード状態報告要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_CARDSTSREQ:
		((CMT_CPRIF_CARDSTSREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_CARDSTSREQ *)body)->signal_kind);
		((CMT_CPRIF_CARDSTSREQ *)body)->slot_no = l2_com_swap_USHORT(((CMT_CPRIF_CARDSTSREQ *)body)->slot_no);
		/* lblap_statis_for_l3 */
/*	性能改善のため一旦止血	*/
//		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_CARDSTSREQ, CMD_NML);
		break;

    /* REカード状態報告応答 : 3GとLTEでフォーマットが異なる */
    case CMD_CPRID_CARDSTSRES:
        if (CMD_SYS_3G == system_type) {
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES*)body)->signal_kind             );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES*)body)->result                  );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES*)body)->cardsts_inf.slot_no     );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES*)body)->cardsts_inf.year        );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES*)body)->cardsts_inf.month_day   );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES*)body)->cardsts_inf.hour        );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES*)body)->cardsts_inf.minsec      );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES*)body)->cardsts_inf.card_svitem );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES*)body)->cardsts_inf.card_extitem);
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES*)body)->cardsts_inf.card_cntitem);
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES*)body)->cardsts_inf.cnt_code    );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES*)body)->cardsts_inf.funcitem    );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES*)body)->cardsts_inf.car_inf     );

    		pdat = (USHORT*)&((char*)body)[sizeof(CMT_CPRIF_CARDSTSRES)];
    		lng = (l3_len - sizeof(CMT_CPRIF_CARDSTSRES)) / sizeof(USHORT);
    		for (num = 0; num < lng; num++, pdat++) {
    		    L2_SWAP_USHORT(*pdat);
    		}

            /* lblap_statis_for_l3 */
/*	性能改善のため一旦止血	*/
//          lblap_statis_for_l3(linkno, system_type, D_L2_STATI_CARDSTSRES, ((CMT_CPRIF_CARDSTSRES*)body)->result);
        } else {
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES_S3G*)body)->signal_kind                );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES_S3G*)body)->result                     );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES_S3G*)body)->cardsts_inf.slot_no        );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES_S3G*)body)->cardsts_inf.year           );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES_S3G*)body)->cardsts_inf.month_day      );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES_S3G*)body)->cardsts_inf.hour           );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES_S3G*)body)->cardsts_inf.minsec         );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES_S3G*)body)->cardsts_inf.card_svitem    );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES_S3G*)body)->cardsts_inf.card_extitem   );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES_S3G*)body)->cardsts_inf.card_cntitem   );
            L2_SWAP_USHORT(((CMT_CPRIF_CARDSTSRES_S3G*)body)->cardsts_inf.card_extcntitem);

            /* 機能部数 */
    		pdat = (USHORT*)&((char*)body)[sizeof(CMT_CPRIF_CARDSTSRES_S3G)];
    		lng = (l3_len - sizeof(CMT_CPRIF_CARDSTSRES_S3G)) / sizeof(USHORT);
    		for (num = 0; num < lng; num++, pdat++) {
    		    L2_SWAP_USHORT(*pdat);
    		}

    		/* lblap_statis_for_l3 */
/*	性能改善のため一旦止血	*/
//          lblap_statis_for_l3(linkno, system_type, D_L2_STATI_CARDSTSRES, ((CMT_CPRIF_CARDSTSRES_S3G *)body)->result);
        }
        break;

    /* REキャリア状態報告要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_CARLSTSREQ:
		((CMT_CPRIF_CARLSTSREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_CARLSTSREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
// /*	性能改善のため一旦止血	*/
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_CARLSTSREQ, CMD_NML);
		break;

	/* REキャリア状態報告応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_CARLSTSRES:
		((CMT_CPRIF_CARLSTSRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_CARLSTSRES *)body)->signal_kind);
		((CMT_CPRIF_CARLSTSRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_CARLSTSRES *)body)->result);
		((CMT_CPRIF_CARLSTSRES *)body)->car_num = l2_com_swap_USHORT(((CMT_CPRIF_CARLSTSRES *)body)->car_num);

        /* 機能部数 */
		pdat = (USHORT*)&((char*)body)[sizeof(CMT_CPRIF_CARLSTSRES)];
		lng = (l3_len - sizeof(CMT_CPRIF_CARLSTSRES)) / sizeof(USHORT);
		for (num = 0; num < lng; num++, pdat++) {
		    L2_SWAP_USHORT(*pdat);
		}
	   
		/* lblap_statis_for_l3 */
// /*	性能改善のため一旦止血	*/
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_CARLSTSRES, ((CMT_CPRIF_CARLSTSRES *)body)->result);
		break;

	/* RE PORT制御要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_REPORTCNTREQ:
		((CMT_CPRIF_REPORTCNTREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_REPORTCNTREQ *)body)->signal_kind);
		((CMT_CPRIF_REPORTCNTREQ *)body)->infbit = l2_com_swap_USHORT(((CMT_CPRIF_REPORTCNTREQ *)body)->infbit);
		((CMT_CPRIF_REPORTCNTREQ *)body)->control = l2_com_swap_USHORT(((CMT_CPRIF_REPORTCNTREQ *)body)->control);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_REPORTCNTREQ, CMD_NML);
		break;

	/* RE PORT制御応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_REPORTCNTRES:
		((CMT_CPRIF_REPORTCNTRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_REPORTCNTRES *)body)->signal_kind);
		((CMT_CPRIF_REPORTCNTRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_REPORTCNTRES *)body)->result);
		((CMT_CPRIF_REPORTCNTRES *)body)->portout_sts = l2_com_swap_USHORT(((CMT_CPRIF_REPORTCNTRES *)body)->portout_sts);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_REPORTCNTRES, ((CMT_CPRIF_REPORTCNTRES *)body)->result);
		break;

	/* RE PORT状態報告要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_REPORTSTSREQ:
		((CMT_CPRIF_REPORTSTSREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_REPORTSTSREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
/*	性能改善のため一旦止血	*/
//		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_REPORTSTSREQ, CMD_NML);
		break;

	/* RE PORT状態報告応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_REPORTSTSRES:
		((CMT_CPRIF_REPORTSTSRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_REPORTSTSRES *)body)->signal_kind);
		((CMT_CPRIF_REPORTSTSRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_REPORTSTSRES *)body)->result);
		((CMT_CPRIF_REPORTSTSRES *)body)->portsts_inf.port_out_sts = l2_com_swap_USHORT(((CMT_CPRIF_REPORTSTSRES *)body)->portsts_inf.port_out_sts);
		((CMT_CPRIF_REPORTSTSRES *)body)->portsts_inf.port_in_sts = l2_com_swap_USHORT(((CMT_CPRIF_REPORTSTSRES *)body)->portsts_inf.port_in_sts);
		/* lblap_statis_for_l3 */
/*	性能改善のため一旦REC側を止血	*/
//		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_REPORTSTSRES, ((CMT_CPRIF_REPORTSTSRES *)body)->result);
		break;

	/* REカード診断要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_RECRDDIAREQ:
		((CMT_CPRIF_RECRDDIAREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_RECRDDIAREQ *)body)->signal_kind);
		((CMT_CPRIF_RECRDDIAREQ *)body)->slot_no = l2_com_swap_USHORT(((CMT_CPRIF_RECRDDIAREQ *)body)->slot_no);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_RECRDDIAREQ, CMD_NML);
		break;

	/* REカード診断応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_RECRDDIARES:
		((CMT_CPRIF_RECRDDIARES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_RECRDDIARES *)body)->signal_kind);
		((CMT_CPRIF_RECRDDIARES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_RECRDDIARES *)body)->result);
		((CMT_CPRIF_RECRDDIARES *)body)->slot_no = l2_com_swap_USHORT(((CMT_CPRIF_RECRDDIARES *)body)->slot_no);
		((CMT_CPRIF_RECRDDIARES *)body)->dia_result = l2_com_swap_USHORT(((CMT_CPRIF_RECRDDIARES *)body)->dia_result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_RECRDDIARES, ((CMT_CPRIF_RECRDDIARES *)body)->result);
		break;

	/* RE障害ログ取得要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_REOBSLOGREQ:
		((CMT_CPRIF_REOBSLOGREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_REOBSLOGREQ *)body)->signal_kind);
		((CMT_CPRIF_REOBSLOGREQ *)body)->comsec = l2_com_swap_USHORT(((CMT_CPRIF_REOBSLOGREQ *)body)->comsec);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_REOBSLOGREQ, CMD_NML);
		break;

	/* RE障害ログ取得応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_REOBSLOGRES:
		((CMT_CPRIF_REOBSLOGRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_REOBSLOGRES *)body)->signal_kind);
		((CMT_CPRIF_REOBSLOGRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_REOBSLOGRES *)body)->result);
		((CMT_CPRIF_REOBSLOGRES *)body)->datsize = l2_com_swap_UINT(((CMT_CPRIF_REOBSLOGRES *)body)->datsize);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_REOBSLOGRES, ((CMT_CPRIF_REOBSLOGRES *)body)->result);
		break;

	/* RE障害ログ取得中止要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_REOBLOGSTPREQ:
		((CMT_CPRIF_REOBLOGSTPREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_REOBLOGSTPREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_REOBLOGSTPREQ, CMD_NML);
		break;

	/* RE障害ログ取得中止応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_REOBLOGSTPRES:
		((CMT_CPRIF_REOBLOGSTPRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_REOBLOGSTPRES *)body)->signal_kind);
		((CMT_CPRIF_REOBLOGSTPRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_REOBLOGSTPRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_REOBLOGSTPRES, ((CMT_CPRIF_REOBLOGSTPRES *)body)->result);
		break;

	/* 外部装置データ送信要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_EXTDATSNDREQ:
		((CMT_CPRIF_EXTDATSNDREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_EXTDATSNDREQ *)body)->signal_kind);
		((CMT_CPRIF_EXTDATSNDREQ *)body)->extdev_no = l2_com_swap_USHORT(((CMT_CPRIF_EXTDATSNDREQ *)body)->extdev_no);
		((CMT_CPRIF_EXTDATSNDREQ *)body)->snd_byte = l2_com_swap_USHORT(((CMT_CPRIF_EXTDATSNDREQ *)body)->snd_byte);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_EXTDATSNDREQ, CMD_NML);
		break;

	/* 外部装置データ送信応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_EXTDATSNDRES:
		((CMT_CPRIF_EXTDATSNDRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_EXTDATSNDRES *)body)->signal_kind);
		((CMT_CPRIF_EXTDATSNDRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_EXTDATSNDRES *)body)->result);
		((CMT_CPRIF_EXTDATSNDRES *)body)->extdev_no = l2_com_swap_USHORT(((CMT_CPRIF_EXTDATSNDRES *)body)->extdev_no);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_EXTDATSNDRES, ((CMT_CPRIF_EXTDATSNDRES *)body)->result);
		break;

	/* 外部装置データ受信要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_EXTDATRCVREQ:
		((CMT_CPRIF_EXTDATRCVREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_EXTDATRCVREQ *)body)->signal_kind);
		((CMT_CPRIF_EXTDATRCVREQ *)body)->extdev_no = l2_com_swap_USHORT(((CMT_CPRIF_EXTDATRCVREQ *)body)->extdev_no);
		((CMT_CPRIF_EXTDATRCVREQ *)body)->sndbyte_num = l2_com_swap_USHORT(((CMT_CPRIF_EXTDATRCVREQ *)body)->sndbyte_num);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_EXTDATRCVREQ, CMD_NML);
		break;

	/* 外部装置データ受信応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_EXTDATRCVRES:
		((CMT_CPRIF_EXTDATRCVRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_EXTDATRCVRES *)body)->signal_kind);
		((CMT_CPRIF_EXTDATRCVRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_EXTDATRCVRES *)body)->result);
		((CMT_CPRIF_EXTDATRCVRES *)body)->extdev_no = l2_com_swap_USHORT(((CMT_CPRIF_EXTDATRCVRES *)body)->extdev_no);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_EXTDATRCVRES, ((CMT_CPRIF_EXTDATRCVRES *)body)->result);
		break;
		
	/* MTアドレス設定要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_MTADDSETREQ:
		((CMT_CPRIF_MTADDSETREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_MTADDSETREQ *)body)->signal_kind);
		((CMT_CPRIF_MTADDSETREQ *)body)->tgtmt = l2_com_swap_USHORT(((CMT_CPRIF_MTADDSETREQ *)body)->tgtmt);
		((CMT_CPRIF_MTADDSETREQ *)body)->cmdtrsport = l2_com_swap_USHORT(((CMT_CPRIF_MTADDSETREQ *)body)->cmdtrsport);
		((CMT_CPRIF_MTADDSETREQ *)body)->filtrsport = l2_com_swap_USHORT(((CMT_CPRIF_MTADDSETREQ *)body)->filtrsport);
		((CMT_CPRIF_MTADDSETREQ *)body)->portno = l2_com_swap_USHORT(((CMT_CPRIF_MTADDSETREQ *)body)->portno);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_MTADDSETREQ, CMD_NML);
		break;

	/* MTアドレス設定応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_MTADDSETRES:
		((CMT_CPRIF_MTADDSETRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_MTADDSETRES *)body)->signal_kind);
		((CMT_CPRIF_MTADDSETRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_MTADDSETRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_MTADDSETRES, ((CMT_CPRIF_MTADDSETRES *)body)->result);
		break;
		
	/* FLD-MT接続要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FLDMTCNCREQ:
		((CMT_CPRIF_FLDMTCNCREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FLDMTCNCREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLDMTCNCREQ, CMD_NML);
		break;

	/* FLD-MT接続応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FLDMTCNCRES:
		((CMT_CPRIF_FLDMTCNCRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FLDMTCNCRES *)body)->signal_kind);
		((CMT_CPRIF_FLDMTCNCRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_FLDMTCNCRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLDMTCNCRES, ((CMT_CPRIF_FLDMTCNCRES *)body)->result);
		break;

	/* FLD-MTデータコマンド転送送信要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FLDCOMSNDREQ:
		((CMT_CPRIF_FLDCOMSNDREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDREQ *)body)->signal_kind);
		((CMT_CPRIF_FLDCOMSNDREQ *)body)->sndalldat_num = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDREQ *)body)->sndalldat_num);
		((CMT_CPRIF_FLDCOMSNDREQ *)body)->snddatseq_no = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDREQ *)body)->snddatseq_no);
		((CMT_CPRIF_FLDCOMSNDREQ *)body)->sndbyte_num = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDREQ *)body)->sndbyte_num);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLDCOMSNDREQ, CMD_NML);
		break;

	/* FLD-MTデータコマンド転送送信応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FLDCOMSNDRES:
		((CMT_CPRIF_FLDCOMSNDRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDRES *)body)->signal_kind);
		((CMT_CPRIF_FLDCOMSNDRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDRES *)body)->result);
		((CMT_CPRIF_FLDCOMSNDRES *)body)->sndalldat_num = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDRES *)body)->sndalldat_num);
		((CMT_CPRIF_FLDCOMSNDRES *)body)->snddatseq_no = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDRES *)body)->snddatseq_no);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLDCOMSNDRES, ((CMT_CPRIF_FLDCOMSNDRES *)body)->result);
		break;

	/* FLD-MTデータコマンド転送受信要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FLDCOMRCVREQ:
		((CMT_CPRIF_FLDCOMRCVREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVREQ *)body)->signal_kind);
		((CMT_CPRIF_FLDCOMRCVREQ *)body)->rcvalldat_num = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVREQ *)body)->rcvalldat_num);
		((CMT_CPRIF_FLDCOMRCVREQ *)body)->rcvdatseq_no = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVREQ *)body)->rcvdatseq_no);
		((CMT_CPRIF_FLDCOMRCVREQ *)body)->rcvbyte_num = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVREQ *)body)->rcvbyte_num);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLDCOMRCVREQ, CMD_NML);
		break;

	/* FLD-MTデータコマンド転送受信応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FLDCOMRCVRES:
		((CMT_CPRIF_FLDCOMRCVRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVRES *)body)->signal_kind);
		((CMT_CPRIF_FLDCOMRCVRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVRES *)body)->result);
		((CMT_CPRIF_FLDCOMRCVRES *)body)->rcvalldat_num = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVRES *)body)->rcvalldat_num);
		((CMT_CPRIF_FLDCOMRCVRES *)body)->rcvdatseq_no = l2_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVRES *)body)->rcvdatseq_no);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLDCOMRCVRES, ((CMT_CPRIF_FLDCOMRCVRES *)body)->result);
		break;

    /* FLD-MTデータファイル転送送信要求 : 3GとLTEでフォーマットが異なる */
    case CMD_CPRID_FLDFILSNDREQ:
        if (CMD_SYS_3G == system_type) {
            L2_SWAP_USHORT(((CMT_CPRIF_FLDFILSNDREQ*)body)->signal_kind);
            L2_SWAP_USHORT(((CMT_CPRIF_FLDFILSNDREQ*)body)->sndbyte_num);
            /* lblap_statis_for_l3 */
            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLDFILSNDREQ, CMD_NML);
        } else {
            L2_SWAP_USHORT(((CMT_CPRIF_FLDFILSNDREQ_S3G*)body)->signal_kind  );
            L2_SWAP_USHORT(((CMT_CPRIF_FLDFILSNDREQ_S3G*)body)->protocol_type);
            L2_SWAP_USHORT(((CMT_CPRIF_FLDFILSNDREQ_S3G*)body)->data_type    );
            L2_SWAP_USHORT(((CMT_CPRIF_FLDFILSNDREQ_S3G*)body)->sndbyte_num  );
            /* lblap_statis_for_l3 */
            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLDFILSNDREQ, CMD_NML);
        }
        break;

    /* FLD-MTデータファイル転送送信応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FLDFILSNDRES:
		((CMT_CPRIF_FLDFILSNDRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FLDFILSNDRES *)body)->signal_kind);
		((CMT_CPRIF_FLDFILSNDRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_FLDFILSNDRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLDFILSNDRES, ((CMT_CPRIF_FLDFILSNDRES *)body)->result);
		break;

    /* FLD-MTデータファイル転送受信要求 : 3GとLTEでフォーマットが異なる */
    case CMD_CPRID_FLDFILRCVREQ:
        if (CMD_SYS_3G == system_type) {
            L2_SWAP_USHORT(((CMT_CPRIF_FLDFILRCVREQ*)body)->signal_kind);
            L2_SWAP_USHORT(((CMT_CPRIF_FLDFILRCVREQ*)body)->rcvbyte_num);
            /* lblap_statis_for_l3 */
            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLDFILRCVREQ, CMD_NML);
        } else {
            L2_SWAP_USHORT(((CMT_CPRIF_FLDFILRCVREQ_S3G*)body)->signal_kind  );
            L2_SWAP_USHORT(((CMT_CPRIF_FLDFILRCVREQ_S3G*)body)->protocol_type);
            L2_SWAP_USHORT(((CMT_CPRIF_FLDFILRCVREQ_S3G*)body)->ftpdata_type );
            L2_SWAP_USHORT(((CMT_CPRIF_FLDFILRCVREQ_S3G*)body)->rcvbyte_num  );
            /* lblap_statis_for_l3 */
            lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLDFILRCVREQ, CMD_NML);
        }
        break;

    /* FLD-MTデータファイル転送受信応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FLDFILRCVRES:
		((CMT_CPRIF_FLDFILRCVRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FLDFILRCVRES *)body)->signal_kind);
		((CMT_CPRIF_FLDFILRCVRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_FLDFILRCVRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLDFILRCVRES, ((CMT_CPRIF_FLDFILRCVRES *)body)->result);
		break;

	/* FLD-MT接続解放要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FLMTCNCRELREQ:
		((CMT_CPRIF_FLMTCNCRELREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FLMTCNCRELREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLMTCNCRELREQ, CMD_NML);
		break;

	/* FLD-MT接続解放応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FLMTCNCRELRES:
		((CMT_CPRIF_FLMTCNCRELRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FLMTCNCRELRES *)body)->signal_kind);
		((CMT_CPRIF_FLMTCNCRELRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_FLMTCNCRELRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLMTCNCRELRES, ((CMT_CPRIF_FLMTCNCRELRES *)body)->result);
		break;

	/* FLD-MT接続強制解放要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FLMTCMPRELREQ:
		((CMT_CPRIF_FLMTCMPRELREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FLMTCMPRELREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLMTCMPRELREQ, CMD_NML);
		break;

    /* FLD-MT接続強制解放応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_FLMTCMPRELRES:
		((CMT_CPRIF_FLMTCMPRELRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FLMTCMPRELRES *)body)->signal_kind);
		((CMT_CPRIF_FLMTCMPRELRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_FLMTCMPRELRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FLMTCMPRELRES, ((CMT_CPRIF_FLMTCMPRELRES *)body)->result);
		break;

	/* SV-MT/eNB-MT接続要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_SVMTCNTREQ:
		((CMT_CPRIF_SVMTCNTREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_SVMTCNTREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_SVMTCNTREQ, CMD_NML);
		break;

	/* SV-MT/eNB-MT接続応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_SVMTCNTRES:
		((CMT_CPRIF_SVMTCNTRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_SVMTCNTRES *)body)->signal_kind);
		((CMT_CPRIF_SVMTCNTRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_SVMTCNTRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_SVMTCNTRES, ((CMT_CPRIF_SVMTCNTRES *)body)->result);
		break;

	/* SV-MT/eNB-MTデータ送信要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_SVMTDATSNDREQ:
		((CMT_CPRIF_SVMTDATSNDREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_SVMTDATSNDREQ *)body)->signal_kind);
		((CMT_CPRIF_SVMTDATSNDREQ *)body)->snd_bytnum = l2_com_swap_USHORT(((CMT_CPRIF_SVMTDATSNDREQ *)body)->snd_bytnum);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_SVMTDATSNDREQ, CMD_NML);
		break;

	/* SV-MT/eNB-MTデータ送信応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_SVMTDATSNDRES:
		((CMT_CPRIF_SVMTDATSNDRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_SVMTDATSNDRES *)body)->signal_kind);
		((CMT_CPRIF_SVMTDATSNDRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_SVMTDATSNDRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_SVMTDATSNDRES, ((CMT_CPRIF_SVMTDATSNDRES *)body)->result);
		break;

	/* SV-MT/eNB-MTデータ受信要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_SVMTDATRCVREQ:
		((CMT_CPRIF_SVMTDATRCVREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_SVMTDATRCVREQ *)body)->signal_kind);
		((CMT_CPRIF_SVMTDATRCVREQ *)body)->rcv_bytnum = l2_com_swap_USHORT(((CMT_CPRIF_SVMTDATRCVREQ *)body)->rcv_bytnum);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_SVMTDATRCVREQ, CMD_NML);
		break;

	/* SV-MT/eNB-MTデータ受信応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_SVMTDATRCVRES:
		((CMT_CPRIF_SVMTDATRCVRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_SVMTDATRCVRES *)body)->signal_kind);
		((CMT_CPRIF_SVMTDATRCVRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_SVMTDATRCVRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_SVMTDATRCVRES, ((CMT_CPRIF_SVMTDATRCVRES *)body)->result);
		break;

	/* SV-MT/eNB-MT接続解放要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_SVMTCNTRELREQ:
		((CMT_CPRIF_SVMTCNTRELREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_SVMTCNTRELREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_SVMTCNTRELREQ, CMD_NML);
		break;

	/* SV-MT/eNB-MT接続解放応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_SVMTCNTRELRES:
		((CMT_CPRIF_SVMTCNTRELRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_SVMTCNTRELRES *)body)->signal_kind);
		((CMT_CPRIF_SVMTCNTRELRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_SVMTCNTRELRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_SVMTCNTRELRES, ((CMT_CPRIF_SVMTCNTRELRES *)body)->result);
		break;

	/* SV-MT/eNB-MT接続強制解放要求 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_SVMTFORRELREQ:
		((CMT_CPRIF_SVMTFORRELREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_SVMTFORRELREQ *)body)->signal_kind);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_SVMTFORRELREQ, CMD_NML);
		break;

	/* SV-MT/eNB-MT接続強制解放応答 : 3GとLTEで共通フォーマット */
	case CMD_CPRID_SVMTFORRELRES:
		((CMT_CPRIF_SVMTFORRELRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_SVMTFORRELRES *)body)->signal_kind);
		((CMT_CPRIF_SVMTFORRELRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_SVMTFORRELRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_SVMTFORRELRES, ((CMT_CPRIF_SVMTFORRELRES *)body)->result);
		break;

	case CMD_CPRID_MKMTDATSND:
		((CMT_CPRIF_MKMTDATSND *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_MKMTDATSND *)body)->signal_kind);
		((CMT_CPRIF_MKMTDATSND *)body)->mkmt_kind = l2_com_swap_USHORT(((CMT_CPRIF_MKMTDATSND *)body)->mkmt_kind);
		((CMT_CPRIF_MKMTDATSND *)body)->mt_length = l2_com_swap_UINT(((CMT_CPRIF_MKMTDATSND *)body)->mt_length);
		((CMT_CPRIF_MKMTDATSND *)body)->mt_signal = l2_com_swap_UINT(((CMT_CPRIF_MKMTDATSND *)body)->mt_signal);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_MKMTDATSND, CMD_NML);
		break;

	case CMD_CPRID_MKMTCNTREQ:
		((CMT_CPRIF_MKMTCNTREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_MKMTCNTREQ *)body)->signal_kind);
		((CMT_CPRIF_MKMTCNTREQ *)body)->mkmt_kind = l2_com_swap_USHORT(((CMT_CPRIF_MKMTCNTREQ *)body)->mkmt_kind);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_MKMTCNTREQ, CMD_NML);
		break;

	case CMD_CPRID_MKMTCNTRES:
		((CMT_CPRIF_MKMTCNTRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_MKMTCNTRES *)body)->signal_kind);
		((CMT_CPRIF_MKMTCNTRES *)body)->mkmt_kind = l2_com_swap_USHORT(((CMT_CPRIF_MKMTCNTRES *)body)->mkmt_kind);
		((CMT_CPRIF_MKMTCNTRES *)body)->result = l2_com_swap_UINT(((CMT_CPRIF_MKMTCNTRES *)body)->result);
		((CMT_CPRIF_MKMTCNTRES *)body)->NG_code = l2_com_swap_UINT(((CMT_CPRIF_MKMTCNTRES *)body)->NG_code);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_MKMTCNTRES, ((CMT_CPRIF_MKMTCNTRES *)body)->result);
		break;

	case CMD_CPRID_MKMTCNTRELREQ:
		((CMT_CPRIF_MKMTCNTRELREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_MKMTCNTRELREQ *)body)->signal_kind);
		((CMT_CPRIF_MKMTCNTRELREQ *)body)->mkmt_kind = l2_com_swap_USHORT(((CMT_CPRIF_MKMTCNTRELREQ *)body)->mkmt_kind);
		((CMT_CPRIF_MKMTCNTRELREQ *)body)->cause = l2_com_swap_UINT(((CMT_CPRIF_MKMTCNTRELREQ *)body)->cause);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_MKMTCNTRELREQ, CMD_NML);
		break;

	case CMD_CPRID_MKMTCNTRELRES:
		((CMT_CPRIF_MKMTCNTRELRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_MKMTCNTRELRES *)body)->signal_kind);
		((CMT_CPRIF_MKMTCNTRELRES *)body)->mkmt_kind = l2_com_swap_USHORT(((CMT_CPRIF_MKMTCNTRELRES *)body)->mkmt_kind);
		((CMT_CPRIF_MKMTCNTRELRES *)body)->result = l2_com_swap_UINT(((CMT_CPRIF_MKMTCNTRELRES *)body)->result);
		((CMT_CPRIF_MKMTCNTRELRES *)body)->NG_code = l2_com_swap_UINT(((CMT_CPRIF_MKMTCNTRELRES *)body)->NG_code);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_MKMTCNTRELREQ, ((CMT_CPRIF_MKMTCNTRELRES *)body)->result);
		break;

	case CMD_CPRID_MKMTFORRELREQ:
		((CMT_CPRIF_MKMTFORRELREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_MKMTFORRELREQ *)body)->signal_kind);
		((CMT_CPRIF_MKMTFORRELREQ *)body)->mkmt_kind = l2_com_swap_USHORT(((CMT_CPRIF_MKMTFORRELREQ *)body)->mkmt_kind);
		((CMT_CPRIF_MKMTFORRELREQ *)body)->cause = l2_com_swap_UINT(((CMT_CPRIF_MKMTFORRELREQ *)body)->cause);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_MKMTFORRELREQ, CMD_NML);
		break;

	case CMD_CPRID_MKMTFORRELRES:
		((CMT_CPRIF_MKMTFORRELRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_MKMTFORRELRES *)body)->signal_kind);
		((CMT_CPRIF_MKMTFORRELRES *)body)->mkmt_kind = l2_com_swap_USHORT(((CMT_CPRIF_MKMTFORRELRES *)body)->mkmt_kind);
		((CMT_CPRIF_MKMTFORRELRES *)body)->result = l2_com_swap_UINT(((CMT_CPRIF_MKMTFORRELRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_MKMTFORRELRES, ((CMT_CPRIF_MKMTFORRELRES *)body)->result);
		break;

	case CMD_CPRID_FTPPORTSETREQ:
		((CMT_CPRIF_FTPPORTSETREQ *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FTPPORTSETREQ *)body)->signal_kind);
		((CMT_CPRIF_FTPPORTSETREQ *)body)->portnum = l2_com_swap_USHORT(((CMT_CPRIF_FTPPORTSETREQ *)body)->portnum);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FTPPORTSETREQ, CMD_NML);
		break;

	case CMD_CPRID_FTPPORTSETRES:
		((CMT_CPRIF_FTPPORTSETRES *)body)->signal_kind = l2_com_swap_USHORT(((CMT_CPRIF_FTPPORTSETRES *)body)->signal_kind);
		((CMT_CPRIF_FTPPORTSETRES *)body)->result = l2_com_swap_USHORT(((CMT_CPRIF_FTPPORTSETRES *)body)->result);
		/* lblap_statis_for_l3 */
		lblap_statis_for_l3(linkno, system_type, D_L2_STATI_FTPPORTSETRES, ((CMT_CPRIF_FTPPORTSETRES *)body)->result);
		break;

	default:
		cm_Assert( CMD_ASL_DBGLOW, signal_kind,
			(CHAR*)"[l2_com_swap_convert] signal_kind is wrong" );
		break;
	}
	return;
}
/* @} */



