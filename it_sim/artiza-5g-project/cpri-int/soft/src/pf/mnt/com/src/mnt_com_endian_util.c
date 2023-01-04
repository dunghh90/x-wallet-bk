/*!
 * @skip  $ld:$
 * @file  mnt_com_endian_util.c
 * @brief MNT endian convert functions
 * @date  2013/12/04 FFCS)zhaodx Create for zynq\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
 
/*!
 * @addtogroup RRH_MNT
 * @{
 */

#include "m_cm_header.h"
#include "mnt_com_inc.h"

/********************************************************************************************************************/
/*!
 *  @brief  Big endian to little endian
 *  @note   Big endian to little endiann for USHORT
 *  @param  target [in]  needed to convert
 *  @retval be converted value
 *  @date  2013/12/04 FFCS)zhaodx Create for zynq
 */
 /********************************************************************************************************************/
USHORT mnt_com_swap_USHORT(USHORT target)
{
	USHORT ret;

	ret = ((((USHORT)(target) & 0xff00) >> 8) | (((USHORT)(target) & 0x00ff) << 8));

    return ret;
}

/********************************************************************************************************************/
/*!
 *  @brief  Little endian to big endian
 *  @note   Little endian to big endian for UINT
 *  @param  target [in]  needed to convert
 *  @retval be converted value
 *  @date  2013/12/04 FFCS)zhaodx Create for zynq
 */
 /********************************************************************************************************************/
UINT mnt_com_swap_UINT(UINT target)
{
	UINT ret;

    ret = ((((UINT)(target) & 0xff000000) >> 24) | \
              (((UINT)(target) & 0x00ff0000) >> 8) | \
              (((UINT)(target) & 0x0000ff00) << 8) | \
              (((UINT)(target) & 0x000000ff) << 24));

    return ret;
}

/* matsumoto modify start */
#ifdef OPT_RRH_ZYNQ_REC
/********************************************************************************************************************/
/*!
 *  @brief  Little endian to big endian
 *  @note   Little endian to big endian subroutine
 *  @param  signal_kind [in]  cpri signal kind
 *  @param  *body [in]  Pointer of cpri message body
 *  @param  direction [in]  direction
 *  @retval -
 *  @date   2013/11/18 FFCS)hongj Create for zynq
 */
 /********************************************************************************************************************/

void m_mk_com_swap_convert(USHORT signal_kind, VOID *body)
{
	UINT num;
	UINT count;
	UINT count_in_blk;
	USHORT *cursor;
	T_RRH_CPRI_STATE 			ltCpriSta;   /* CPRIState */

	if(body == D_RRH_NULL)
	{
		cm_Assert( CMD_ASL_DBGLOW, (UINT)body,
			(UCHAR*)"[m_mk_com_swap_convert] body is NULL" );
		return;
	}

	switch(signal_kind & 0xFFFE)
	{
	case CMD_CPRID_HCREQ:
		((CMT_CPRIF_HCREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_HCREQ *)body)->signal_kind);
		break;

	case CMD_CPRID_HCRES:
		((CMT_CPRIF_HCRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_HCRES *)body)->signal_kind);
		break;

	case CMD_CPRID_RERSTREQ:
		((CMT_CPRIF_RERSTREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_RERSTREQ *)body)->signal_kind);
		((CMT_CPRIF_RERSTREQ *)body)->kind = mnt_com_swap_USHORT(((CMT_CPRIF_RERSTREQ *)body)->kind);
		((CMT_CPRIF_RERSTREQ *)body)->recomapp = mnt_com_swap_USHORT(((CMT_CPRIF_RERSTREQ *)body)->recomapp);
		break;

	case CMD_CPRID_RETIMSETREQ:
		((CMT_CPRIF_RETIMSETREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_RETIMSETREQ *)body)->signal_kind);
		((CMT_CPRIF_RETIMSETREQ *)body)->year = mnt_com_swap_USHORT(((CMT_CPRIF_RETIMSETREQ *)body)->year);
		((CMT_CPRIF_RETIMSETREQ *)body)->month_day = mnt_com_swap_USHORT(((CMT_CPRIF_RETIMSETREQ *)body)->month_day);
		((CMT_CPRIF_RETIMSETREQ *)body)->hour = mnt_com_swap_USHORT(((CMT_CPRIF_RETIMSETREQ *)body)->hour);
		((CMT_CPRIF_RETIMSETREQ *)body)->minute_sec = mnt_com_swap_USHORT(((CMT_CPRIF_RETIMSETREQ *)body)->minute_sec);
		break;

	case CMD_CPRID_LINKSTSREQ:
		((CMT_CPRIF_LINKSTSREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_LINKSTSREQ *)body)->signal_kind);
		((CMT_CPRIF_LINKSTSREQ *)body)->link = mnt_com_swap_USHORT(((CMT_CPRIF_LINKSTSREQ *)body)->link);
		break;

	case CMD_CPRID_LINKSTSRES:
		((CMT_CPRIF_LINKSTSRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_LINKSTSRES *)body)->signal_kind);
		((CMT_CPRIF_LINKSTSRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_LINKSTSRES *)body)->result);
		break;

	case CMD_CPRID_FIRMFILEREQ:
		((CMT_CPRIF_FIRMFILEREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FIRMFILEREQ *)body)->signal_kind);
		break;

	case CMD_CPRID_FIRMFILERES:
		((CMT_CPRIF_FIRMFILERES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FIRMFILERES *)body)->signal_kind);
		((CMT_CPRIF_FIRMFILERES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_FIRMFILERES *)body)->result);
		((CMT_CPRIF_FIRMFILERES *)body)->firmfile.makername = mnt_com_swap_USHORT(((CMT_CPRIF_FIRMFILERES *)body)->firmfile.makername);
		((CMT_CPRIF_FIRMFILERES *)body)->firmfile.mjr_ver = mnt_com_swap_USHORT(((CMT_CPRIF_FIRMFILERES *)body)->firmfile.mjr_ver);
		((CMT_CPRIF_FIRMFILERES *)body)->firmfile.mir_ver = mnt_com_swap_USHORT(((CMT_CPRIF_FIRMFILERES *)body)->firmfile.mir_ver);
		break;

	case CMD_CPRID_FILEINFREQ:
		((CMT_CPRIF_FILEINFREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FILEINFREQ *)body)->signal_kind);
		((CMT_CPRIF_FILEINFREQ *)body)->data = mnt_com_swap_USHORT(((CMT_CPRIF_FILEINFREQ *)body)->data);
		((CMT_CPRIF_FILEINFREQ *)body)->size = mnt_com_swap_UINT(((CMT_CPRIF_FILEINFREQ *)body)->size);
		break;

	case CMD_CPRID_FILEINFRES:
		((CMT_CPRIF_FILEINFRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FILEINFRES *)body)->signal_kind);
		((CMT_CPRIF_FILEINFRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_FILEINFRES *)body)->result);
		break;

	case CMD_CPRID_FILEDATSND:
		((CMT_CPRIF_FILEDATSND *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FILEDATSND *)body)->signal_kind);
		((CMT_CPRIF_FILEDATSND *)body)->sgmtno1 = mnt_com_swap_USHORT(((CMT_CPRIF_FILEDATSND *)body)->sgmtno1);
		((CMT_CPRIF_FILEDATSND *)body)->sgmtno2 = mnt_com_swap_USHORT(((CMT_CPRIF_FILEDATSND *)body)->sgmtno2);
		((CMT_CPRIF_FILEDATSND *)body)->datsize1 = mnt_com_swap_USHORT(((CMT_CPRIF_FILEDATSND *)body)->datsize1);
		((CMT_CPRIF_FILEDATSND *)body)->datsize2 = mnt_com_swap_USHORT(((CMT_CPRIF_FILEDATSND *)body)->datsize2);
		break;

	case CMD_CPRID_FILSNDFINNTC:
		((CMT_CPRIF_FILSNDFINNTC *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FILSNDFINNTC *)body)->signal_kind);
		((CMT_CPRIF_FILSNDFINNTC *)body)->tgtdat = mnt_com_swap_USHORT(((CMT_CPRIF_FILSNDFINNTC *)body)->tgtdat);
		((CMT_CPRIF_FILSNDFINNTC *)body)->chksum = mnt_com_swap_USHORT(((CMT_CPRIF_FILSNDFINNTC *)body)->chksum);
		break;

	case CMD_CPRID_FILSNDFINRES:
		((CMT_CPRIF_FILSNDFINRES_S3G *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FILSNDFINRES_S3G *)body)->signal_kind);
		((CMT_CPRIF_FILSNDFINRES_S3G *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_FILSNDFINRES_S3G *)body)->result);
		((CMT_CPRIF_FILSNDFINRES_S3G *)body)->primary_sys = mnt_com_swap_USHORT(((CMT_CPRIF_FILSNDFINRES_S3G *)body)->primary_sys);
		break;

	case CMD_CPRID_UFIRMFILEREQ:
		((CMT_CPRIF_UFIRMFILEREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_UFIRMFILEREQ *)body)->signal_kind);
		break;

	case CMD_CPRID_UFIRMFILERES:
		((CMT_CPRIF_UFIRMFILERES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_UFIRMFILERES *)body)->signal_kind);
		((CMT_CPRIF_UFIRMFILERES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_UFIRMFILERES *)body)->result);
		((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.makername = mnt_com_swap_USHORT(((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.makername);
		((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.cur_mjr_ver = mnt_com_swap_USHORT(((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.cur_mjr_ver);
		((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.cur_mir_ver = mnt_com_swap_USHORT(((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.cur_mir_ver);
		((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.nes_mjr_ver = mnt_com_swap_USHORT(((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.nes_mjr_ver);
		((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.nes_mir_ver = mnt_com_swap_USHORT(((CMT_CPRIF_UFIRMFILERES *)body)->firmfile.nes_mir_ver);
		break;

	case CMD_CPRID_UFILEINFREQ:
		((CMT_CPRIF_UFILEINFREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_UFILEINFREQ *)body)->signal_kind);
		((CMT_CPRIF_UFILEINFREQ *)body)->data = mnt_com_swap_USHORT(((CMT_CPRIF_UFILEINFREQ *)body)->data);
		((CMT_CPRIF_UFILEINFREQ *)body)->size = mnt_com_swap_UINT(((CMT_CPRIF_UFILEINFREQ *)body)->size);
		break;

	case CMD_CPRID_UFILEINFRES:
		((CMT_CPRIF_UFILEINFRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_UFILEINFRES *)body)->signal_kind);
		((CMT_CPRIF_UFILEINFRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_UFILEINFRES *)body)->result);
		break;

	case CMD_CPRID_UFILEDATSND:
		((CMT_CPRIF_UFILEDATSND *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_UFILEDATSND *)body)->signal_kind);
		((CMT_CPRIF_UFILEDATSND *)body)->sgmtno1 = mnt_com_swap_USHORT(((CMT_CPRIF_UFILEDATSND *)body)->sgmtno1);
		((CMT_CPRIF_UFILEDATSND *)body)->sgmtno2 = mnt_com_swap_USHORT(((CMT_CPRIF_UFILEDATSND *)body)->sgmtno2);
		((CMT_CPRIF_UFILEDATSND *)body)->datsize1 = mnt_com_swap_USHORT(((CMT_CPRIF_UFILEDATSND *)body)->datsize1);
		((CMT_CPRIF_UFILEDATSND *)body)->datsize2 = mnt_com_swap_USHORT(((CMT_CPRIF_UFILEDATSND *)body)->datsize2);
		break;

	case CMD_CPRID_UFILSNDFINNTC:
		((CMT_CPRIF_UFILSNDFINNTC *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_UFILSNDFINNTC *)body)->signal_kind);
		((CMT_CPRIF_UFILSNDFINNTC *)body)->tgtdat = mnt_com_swap_USHORT(((CMT_CPRIF_UFILSNDFINNTC *)body)->tgtdat);
		((CMT_CPRIF_UFILSNDFINNTC *)body)->chksum = mnt_com_swap_USHORT(((CMT_CPRIF_UFILSNDFINNTC *)body)->chksum);
		break;

	case CMD_CPRID_UFILSNDFINRES:
		((CMT_CPRIF_UFILSNDFINRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_UFILSNDFINRES *)body)->signal_kind);
		((CMT_CPRIF_UFILSNDFINRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_UFILSNDFINRES *)body)->result);
		break;

	case CMD_CPRID_TRXSETREQ:
		f_cmn_com_cpri_state_get( &ltCpriSta );
		if(D_RRH_LINE_BITRATE_24G == ltCpriSta.cpri_bitrate)
		{
			((CMT_CPRIF_TRXSETREQ_S3G_FDD *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETREQ_S3G *)body)->signal_kind);
			((CMT_CPRIF_TRXSETREQ_S3G_FDD *)body)->carno = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETREQ_S3G *)body)->carno);
			((CMT_CPRIF_TRXSETREQ_S3G_FDD *)body)->bandwidth = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETREQ_S3G *)body)->bandwidth);
			((CMT_CPRIF_TRXSETREQ_S3G_FDD *)body)->dl_freq = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETREQ_S3G *)body)->dl_freq);
			((CMT_CPRIF_TRXSETREQ_S3G_FDD *)body)->snd_br = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETREQ_S3G *)body)->snd_br);
		}
		else
		{
			((CMT_CPRIF_TRXSETREQ_S3G *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETREQ_S3G *)body)->signal_kind);
			((CMT_CPRIF_TRXSETREQ_S3G *)body)->carno = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETREQ_S3G *)body)->carno);
			((CMT_CPRIF_TRXSETREQ_S3G *)body)->bandwidth = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETREQ_S3G *)body)->bandwidth);
			((CMT_CPRIF_TRXSETREQ_S3G *)body)->dl_freq = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETREQ_S3G *)body)->dl_freq);
			((CMT_CPRIF_TRXSETREQ_S3G *)body)->snd_br = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETREQ_S3G *)body)->snd_br);
			((CMT_CPRIF_TRXSETREQ_S3G *)body)->tdd_up_dwn_config = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETREQ_S3G *)body)->tdd_up_dwn_config);
			((CMT_CPRIF_TRXSETREQ_S3G *)body)->tdd_spe_sf_config = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETREQ_S3G *)body)->tdd_spe_sf_config);
			((CMT_CPRIF_TRXSETREQ_S3G *)body)->tdd_dwnlk_cp_len = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETREQ_S3G *)body)->tdd_dwnlk_cp_len);
		}
		break;

	case CMD_CPRID_TRXSETRES:
		((CMT_CPRIF_TRXSETRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETRES *)body)->signal_kind);
		((CMT_CPRIF_TRXSETRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETRES *)body)->result);
		((CMT_CPRIF_TRXSETRES *)body)->carno = mnt_com_swap_USHORT(((CMT_CPRIF_TRXSETRES *)body)->carno);
		break;

	case CMD_CPRID_TRXRELREQ:
		((CMT_CPRIF_TRXRELREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_TRXRELREQ *)body)->signal_kind);
		((CMT_CPRIF_TRXRELREQ *)body)->carno = mnt_com_swap_USHORT(((CMT_CPRIF_TRXRELREQ *)body)->carno);
		break;

	case CMD_CPRID_TRXRELRES:
		((CMT_CPRIF_TRXRELRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_TRXRELRES *)body)->signal_kind);
		((CMT_CPRIF_TRXRELRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_TRXRELRES *)body)->result);
		((CMT_CPRIF_TRXRELRES *)body)->carno = mnt_com_swap_USHORT(((CMT_CPRIF_TRXRELRES *)body)->carno);
		break;

	case CMD_CPRID_TOFFSETINFREQ:
		((CMT_CPRIF_TOFFSETINFREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_TOFFSETINFREQ *)body)->signal_kind);
		break;

	case CMD_CPRID_TOFFSETINFRES:
		((CMT_CPRIF_TOFFSETINFRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_TOFFSETINFRES *)body)->signal_kind);
		((CMT_CPRIF_TOFFSETINFRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_TOFFSETINFRES *)body)->result);
		((CMT_CPRIF_TOFFSETINFRES *)body)->toffset_inf.toffset = mnt_com_swap_USHORT(((CMT_CPRIF_TOFFSETINFRES *)body)->toffset_inf.toffset);
		((CMT_CPRIF_TOFFSETINFRES *)body)->toffset_inf.updeleqp = mnt_com_swap_USHORT(((CMT_CPRIF_TOFFSETINFRES *)body)->toffset_inf.updeleqp);
		((CMT_CPRIF_TOFFSETINFRES *)body)->toffset_inf.downdeleqp = mnt_com_swap_USHORT(((CMT_CPRIF_TOFFSETINFRES *)body)->toffset_inf.downdeleqp);
		break;

	case CMD_CPRID_CARONOFFREQ:
		((CMT_CPRIF_CARONOFFREQ_S3G *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_CARONOFFREQ_S3G *)body)->signal_kind);
		((CMT_CPRIF_CARONOFFREQ_S3G *)body)->carno = mnt_com_swap_USHORT(((CMT_CPRIF_CARONOFFREQ_S3G *)body)->carno);
		((CMT_CPRIF_CARONOFFREQ_S3G *)body)->onoff_app = mnt_com_swap_USHORT(((CMT_CPRIF_CARONOFFREQ_S3G *)body)->onoff_app);
		((CMT_CPRIF_CARONOFFREQ_S3G *)body)->sndpwr = mnt_com_swap_USHORT(((CMT_CPRIF_CARONOFFREQ_S3G *)body)->sndpwr);
		break;

	case CMD_CPRID_CARONOFFRES:
		((CMT_CPRIF_CARONOFFRES_S3G *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_CARONOFFRES_S3G *)body)->signal_kind);
		((CMT_CPRIF_CARONOFFRES_S3G *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_CARONOFFRES_S3G *)body)->result);
		((CMT_CPRIF_CARONOFFRES_S3G *)body)->carno = mnt_com_swap_USHORT(((CMT_CPRIF_CARONOFFRES_S3G *)body)->carno);
		break;

	case CMD_CPRID_EQPSTRREQ:
		((CMT_CPRIF_EQPSTRREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRREQ *)body)->signal_kind);
		break;
		
	case CMD_CPRID_EQPSTRRES:
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->signal_kind);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->result);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.maker = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.maker);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.bts_no = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.bts_no);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.eqp_no = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.eqp_no);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.rekind = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.rekind);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.bts_share_type = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.bts_share_type);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerkind = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerkind);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.rcv_antena_no = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.rcv_antena_no);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.snd_antena_no = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.snd_antena_no);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerunit = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerunit);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_0 = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_0);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_1 = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_1);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_2 = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_2);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_3 = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_3);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.frqband = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.frqband);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.max_bandwidth = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.max_bandwidth);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.us2g_car_no = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.us2g_car_no);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.us15g_car_no = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.us15g_car_no);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.us800m_car_no = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.us800m_car_no);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.us700m_car_no = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.us700m_car_no);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.us17g_car_no = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.us17g_car_no);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.trx_inf_type = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.trx_inf_type);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.us35g_car_no = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.us35g_car_no);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_4 = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_4);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_5 = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_5);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_6 = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_6);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_7 = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.powerclass_7);
		((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.us34g_car_no = mnt_com_swap_USHORT(((CMT_CPRIF_EQPSTRRES_S3G *)body)->cpristr_inf.us34g_car_no);

		break;

	case CMD_CPRID_SLOTINFREQ:
		((CMT_CPRIF_SLOTINFREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_SLOTINFREQ *)body)->signal_kind);
		break;

	case CMD_CPRID_SLOTINFRES:
		((CMT_CPRIF_SLOTINFRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_SLOTINFRES *)body)->signal_kind);
		((CMT_CPRIF_SLOTINFRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_SLOTINFRES *)body)->result);
		break;

	case CMD_CPRID_RESTSREQ:
		((CMT_CPRIF_RESTSREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_RESTSREQ *)body)->signal_kind);
		break;

	case CMD_CPRID_RESTSRES:
		((CMT_CPRIF_RESTSRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_RESTSRES *)body)->signal_kind);
		((CMT_CPRIF_RESTSRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_RESTSRES *)body)->result);
		((CMT_CPRIF_RESTSRES *)body)->rests_inf.maker_name = mnt_com_swap_USHORT(((CMT_CPRIF_RESTSRES *)body)->rests_inf.maker_name);
		break;

	case CMD_CPRID_CARDINFREQ:
		((CMT_CPRIF_CARDINFREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_CARDINFREQ *)body)->signal_kind);
		((CMT_CPRIF_CARDINFREQ *)body)->slot_no = mnt_com_swap_USHORT(((CMT_CPRIF_CARDINFREQ *)body)->slot_no);
		break;

	case CMD_CPRID_CARDINFRES:
		((CMT_CPRIF_CARDINFRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->signal_kind);
		((CMT_CPRIF_CARDINFRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->result);
		((CMT_CPRIF_CARDINFRES *)body)->slot_no = mnt_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->slot_no);
		((CMT_CPRIF_CARDINFRES *)body)->up_resrc_num1 = mnt_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->up_resrc_num1);
		((CMT_CPRIF_CARDINFRES *)body)->up_resrc_num2 = mnt_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->up_resrc_num2);
		((CMT_CPRIF_CARDINFRES *)body)->down_resrc_num1 = mnt_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->down_resrc_num1);
		((CMT_CPRIF_CARDINFRES *)body)->down_resrc_num2 = mnt_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->down_resrc_num2);
		((CMT_CPRIF_CARDINFRES *)body)->card_svitem = mnt_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->card_svitem);
		((CMT_CPRIF_CARDINFRES *)body)->card_extitem = mnt_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->card_extitem);
		((CMT_CPRIF_CARDINFRES *)body)->card_cntitem = mnt_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->card_cntitem);
		((CMT_CPRIF_CARDINFRES *)body)->funcitem = mnt_com_swap_USHORT(((CMT_CPRIF_CARDINFRES *)body)->funcitem);
		
		break;

	case CMD_CPRID_REVERREQ:
		((CMT_CPRIF_REVERREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_REVERREQ *)body)->signal_kind);
		((CMT_CPRIF_REVERREQ *)body)->tgtkind = mnt_com_swap_USHORT(((CMT_CPRIF_REVERREQ *)body)->tgtkind);
		((CMT_CPRIF_REVERREQ *)body)->slot_no = mnt_com_swap_USHORT(((CMT_CPRIF_REVERREQ *)body)->slot_no);
		((CMT_CPRIF_REVERREQ *)body)->id_kind = mnt_com_swap_USHORT(((CMT_CPRIF_REVERREQ *)body)->id_kind);
		
		break;

	case CMD_CPRID_REVERRES:
		((CMT_CPRIF_REVERRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_REVERRES *)body)->signal_kind);
		((CMT_CPRIF_REVERRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_REVERRES *)body)->result);
		((CMT_CPRIF_REVERRES *)body)->rests_inf.kind = mnt_com_swap_USHORT(((CMT_CPRIF_REVERRES *)body)->rests_inf.kind);
		((CMT_CPRIF_REVERRES *)body)->rests_inf.slot_no = mnt_com_swap_USHORT(((CMT_CPRIF_REVERRES *)body)->rests_inf.slot_no);
		((CMT_CPRIF_REVERRES *)body)->rests_inf.inf_num = mnt_com_swap_USHORT(((CMT_CPRIF_REVERRES *)body)->rests_inf.inf_num);
		break;

	case CMD_CPRID_RECARDCNTREQ:
		((CMT_CPRIF_RECARDCNTREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_RECARDCNTREQ *)body)->signal_kind);
		((CMT_CPRIF_RECARDCNTREQ *)body)->slot_no = mnt_com_swap_USHORT(((CMT_CPRIF_RECARDCNTREQ *)body)->slot_no);
		((CMT_CPRIF_RECARDCNTREQ *)body)->cnt_kind = mnt_com_swap_USHORT(((CMT_CPRIF_RECARDCNTREQ *)body)->cnt_kind);
		((CMT_CPRIF_RECARDCNTREQ *)body)->cnt_code = mnt_com_swap_USHORT(((CMT_CPRIF_RECARDCNTREQ *)body)->cnt_code);
		break;

	case CMD_CPRID_RECARDCNTRES:
		((CMT_CPRIF_RECARDCNTRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_RECARDCNTRES *)body)->signal_kind);
		((CMT_CPRIF_RECARDCNTRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_RECARDCNTRES *)body)->result);
		((CMT_CPRIF_RECARDCNTRES *)body)->slot_no = mnt_com_swap_USHORT(((CMT_CPRIF_RECARDCNTRES *)body)->slot_no);
		break;

	case CMD_CPRID_CARDSTSREQ:
		((CMT_CPRIF_CARDSTSREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_CARDSTSREQ *)body)->signal_kind);
		((CMT_CPRIF_CARDSTSREQ *)body)->slot_no = mnt_com_swap_USHORT(((CMT_CPRIF_CARDSTSREQ *)body)->slot_no);
		break;

	case CMD_CPRID_CARDSTSRES:
		((CMT_CPRIF_CARDSTSRES_S3G *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_CARDSTSRES_S3G *)body)->signal_kind);
		((CMT_CPRIF_CARDSTSRES_S3G *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_CARDSTSRES_S3G *)body)->result);
		((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.slot_no = mnt_com_swap_USHORT(((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.slot_no);
		((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.year = mnt_com_swap_USHORT(((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.year);
		((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.month_day = mnt_com_swap_USHORT(((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.month_day);
		((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.hour = mnt_com_swap_USHORT(((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.hour);
		((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.minsec = mnt_com_swap_USHORT(((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.minsec);
		((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.card_svitem = mnt_com_swap_USHORT(((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.card_svitem);
		((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.card_extitem = mnt_com_swap_USHORT(((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.card_extitem);
		((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.card_cntitem = mnt_com_swap_USHORT(((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.card_cntitem);
		((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.card_extcntitem = mnt_com_swap_USHORT(((CMT_CPRIF_CARDSTSRES_S3G *)body)->cardsts_inf.card_extcntitem);
		
		break;
		
	case CMD_CPRID_CARLSTSREQ:
		((CMT_CPRIF_CARLSTSREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_CARLSTSREQ *)body)->signal_kind);
		break;

	case CMD_CPRID_CARLSTSRES:
		((CMT_CPRIF_CARLSTSRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_CARLSTSRES *)body)->signal_kind);
		((CMT_CPRIF_CARLSTSRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_CARLSTSRES *)body)->result);
		((CMT_CPRIF_CARLSTSRES *)body)->car_num = mnt_com_swap_USHORT(((CMT_CPRIF_CARLSTSRES *)body)->car_num);
		((CMT_CPRIF_CARLSTSRES *)body)->carstsinf = mnt_com_swap_USHORT(((CMT_CPRIF_CARLSTSRES *)body)->carstsinf);
		break;

	case CMD_CPRID_REPORTCNTREQ:
		((CMT_CPRIF_REPORTCNTREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_REPORTCNTREQ *)body)->signal_kind);
		((CMT_CPRIF_REPORTCNTREQ *)body)->infbit = mnt_com_swap_USHORT(((CMT_CPRIF_REPORTCNTREQ *)body)->infbit);
		((CMT_CPRIF_REPORTCNTREQ *)body)->control = mnt_com_swap_USHORT(((CMT_CPRIF_REPORTCNTREQ *)body)->control);
		break;

	case CMD_CPRID_REPORTCNTRES:
		((CMT_CPRIF_REPORTCNTRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_REPORTCNTRES *)body)->signal_kind);
		((CMT_CPRIF_REPORTCNTRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_REPORTCNTRES *)body)->result);
		((CMT_CPRIF_REPORTCNTRES *)body)->portout_sts = mnt_com_swap_USHORT(((CMT_CPRIF_REPORTCNTRES *)body)->portout_sts);
		break;

	case CMD_CPRID_REPORTSTSREQ:
		((CMT_CPRIF_REPORTSTSREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_REPORTSTSREQ *)body)->signal_kind);
		break;

	case CMD_CPRID_REPORTSTSRES:
		((CMT_CPRIF_REPORTSTSRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_REPORTSTSRES *)body)->signal_kind);
		((CMT_CPRIF_REPORTSTSRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_REPORTSTSRES *)body)->result);
		((CMT_CPRIF_REPORTSTSRES *)body)->portsts_inf.port_out_sts = mnt_com_swap_USHORT(((CMT_CPRIF_REPORTSTSRES *)body)->portsts_inf.port_out_sts);
		((CMT_CPRIF_REPORTSTSRES *)body)->portsts_inf.port_in_sts = mnt_com_swap_USHORT(((CMT_CPRIF_REPORTSTSRES *)body)->portsts_inf.port_in_sts);
		break;

	case CMD_CPRID_RECRDDIAREQ:
		((CMT_CPRIF_RECRDDIAREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_RECRDDIAREQ *)body)->signal_kind);
		((CMT_CPRIF_RECRDDIAREQ *)body)->slot_no = mnt_com_swap_USHORT(((CMT_CPRIF_RECRDDIAREQ *)body)->slot_no);
		break;

	case CMD_CPRID_RECRDDIARES:
		((CMT_CPRIF_RECRDDIARES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_RECRDDIARES *)body)->signal_kind);
		((CMT_CPRIF_RECRDDIARES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_RECRDDIARES *)body)->result);
		((CMT_CPRIF_RECRDDIARES *)body)->slot_no = mnt_com_swap_USHORT(((CMT_CPRIF_RECRDDIARES *)body)->slot_no);
		((CMT_CPRIF_RECRDDIARES *)body)->dia_result = mnt_com_swap_USHORT(((CMT_CPRIF_RECRDDIARES *)body)->dia_result);
		break;

	case CMD_CPRID_REOBSLOGREQ:
		((CMT_CPRIF_REOBSLOGREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_REOBSLOGREQ *)body)->signal_kind);
		((CMT_CPRIF_REOBSLOGREQ *)body)->comsec = mnt_com_swap_USHORT(((CMT_CPRIF_REOBSLOGREQ *)body)->comsec);
		break;

	case CMD_CPRID_REOBSLOGRES:
		((CMT_CPRIF_REOBSLOGRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_REOBSLOGRES *)body)->signal_kind);
		((CMT_CPRIF_REOBSLOGRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_REOBSLOGRES *)body)->result);
		((CMT_CPRIF_REOBSLOGRES *)body)->datsize = mnt_com_swap_UINT(((CMT_CPRIF_REOBSLOGRES *)body)->datsize);
		break;

	case CMD_CPRID_REOBLOGSTPREQ:
		((CMT_CPRIF_REOBLOGSTPREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_REOBLOGSTPREQ *)body)->signal_kind);
		break;

	case CMD_CPRID_REOBLOGSTPRES:
		((CMT_CPRIF_REOBLOGSTPRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_REOBLOGSTPRES *)body)->signal_kind);
		((CMT_CPRIF_REOBLOGSTPRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_REOBLOGSTPRES *)body)->result);
		break;

	case CMD_CPRID_EXTDATSNDREQ:
		((CMT_CPRIF_EXTDATSNDREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_EXTDATSNDREQ *)body)->signal_kind);
		((CMT_CPRIF_EXTDATSNDREQ *)body)->extdev_no = mnt_com_swap_USHORT(((CMT_CPRIF_EXTDATSNDREQ *)body)->extdev_no);
		((CMT_CPRIF_EXTDATSNDREQ *)body)->snd_byte = mnt_com_swap_USHORT(((CMT_CPRIF_EXTDATSNDREQ *)body)->snd_byte);
		break;

	case CMD_CPRID_EXTDATSNDRES:
		((CMT_CPRIF_EXTDATSNDRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_EXTDATSNDRES *)body)->signal_kind);
		((CMT_CPRIF_EXTDATSNDRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_EXTDATSNDRES *)body)->result);
		((CMT_CPRIF_EXTDATSNDRES *)body)->extdev_no = mnt_com_swap_USHORT(((CMT_CPRIF_EXTDATSNDRES *)body)->extdev_no);
		break;

	case CMD_CPRID_EXTDATRCVREQ:
		((CMT_CPRIF_EXTDATRCVREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_EXTDATRCVREQ *)body)->signal_kind);
		((CMT_CPRIF_EXTDATRCVREQ *)body)->extdev_no = mnt_com_swap_USHORT(((CMT_CPRIF_EXTDATRCVREQ *)body)->extdev_no);
		((CMT_CPRIF_EXTDATRCVREQ *)body)->sndbyte_num = mnt_com_swap_USHORT(((CMT_CPRIF_EXTDATRCVREQ *)body)->sndbyte_num);
		break;

	case CMD_CPRID_EXTDATRCVRES:
		((CMT_CPRIF_EXTDATRCVRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_EXTDATRCVRES *)body)->signal_kind);
		((CMT_CPRIF_EXTDATRCVRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_EXTDATRCVRES *)body)->result);
		((CMT_CPRIF_EXTDATRCVRES *)body)->extdev_no = mnt_com_swap_USHORT(((CMT_CPRIF_EXTDATRCVRES *)body)->extdev_no);
		break;
		
	case CMD_CPRID_MTADDSETREQ:
		((CMT_CPRIF_MTADDSETREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MTADDSETREQ *)body)->signal_kind);
		((CMT_CPRIF_MTADDSETREQ *)body)->tgtmt = mnt_com_swap_USHORT(((CMT_CPRIF_MTADDSETREQ *)body)->tgtmt);
		((CMT_CPRIF_MTADDSETREQ *)body)->cmdtrsport = mnt_com_swap_USHORT(((CMT_CPRIF_MTADDSETREQ *)body)->cmdtrsport);
		((CMT_CPRIF_MTADDSETREQ *)body)->filtrsport = mnt_com_swap_USHORT(((CMT_CPRIF_MTADDSETREQ *)body)->filtrsport);
		((CMT_CPRIF_MTADDSETREQ *)body)->portno = mnt_com_swap_USHORT(((CMT_CPRIF_MTADDSETREQ *)body)->portno);
		break;

	case CMD_CPRID_MTADDSETRES:
		((CMT_CPRIF_MTADDSETRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MTADDSETRES *)body)->signal_kind);
		((CMT_CPRIF_MTADDSETRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_MTADDSETRES *)body)->result);
		break;
		
	case CMD_CPRID_FLDMTCNCREQ:
		((CMT_CPRIF_FLDMTCNCREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FLDMTCNCREQ *)body)->signal_kind);
		break;

	case CMD_CPRID_FLDMTCNCRES:
		((CMT_CPRIF_FLDMTCNCRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FLDMTCNCRES *)body)->signal_kind);
		((CMT_CPRIF_FLDMTCNCRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_FLDMTCNCRES *)body)->result);
		break;

	case CMD_CPRID_FLDCOMSNDREQ:
		((CMT_CPRIF_FLDCOMSNDREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDREQ *)body)->signal_kind);
		((CMT_CPRIF_FLDCOMSNDREQ *)body)->sndalldat_num = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDREQ *)body)->sndalldat_num);
		((CMT_CPRIF_FLDCOMSNDREQ *)body)->snddatseq_no = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDREQ *)body)->snddatseq_no);
		((CMT_CPRIF_FLDCOMSNDREQ *)body)->sndbyte_num = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDREQ *)body)->sndbyte_num);
		break;

	case CMD_CPRID_FLDCOMSNDRES:
		((CMT_CPRIF_FLDCOMSNDRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDRES *)body)->signal_kind);
		((CMT_CPRIF_FLDCOMSNDRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDRES *)body)->result);
		((CMT_CPRIF_FLDCOMSNDRES *)body)->sndalldat_num = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDRES *)body)->sndalldat_num);
		((CMT_CPRIF_FLDCOMSNDRES *)body)->snddatseq_no = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMSNDRES *)body)->snddatseq_no);
		break;

	case CMD_CPRID_FLDCOMRCVREQ:
		((CMT_CPRIF_FLDCOMRCVREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVREQ *)body)->signal_kind);
		((CMT_CPRIF_FLDCOMRCVREQ *)body)->rcvalldat_num = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVREQ *)body)->rcvalldat_num);
		((CMT_CPRIF_FLDCOMRCVREQ *)body)->rcvdatseq_no = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVREQ *)body)->rcvdatseq_no);
		((CMT_CPRIF_FLDCOMRCVREQ *)body)->rcvbyte_num = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVREQ *)body)->rcvbyte_num);
		break;

	case CMD_CPRID_FLDCOMRCVRES:
		((CMT_CPRIF_FLDCOMRCVRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVRES *)body)->signal_kind);
		((CMT_CPRIF_FLDCOMRCVRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVRES *)body)->result);
		((CMT_CPRIF_FLDCOMRCVRES *)body)->rcvalldat_num = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVRES *)body)->rcvalldat_num);
		((CMT_CPRIF_FLDCOMRCVRES *)body)->rcvdatseq_no = mnt_com_swap_USHORT(((CMT_CPRIF_FLDCOMRCVRES *)body)->rcvdatseq_no);
		break;

	case CMD_CPRID_FLDFILSNDREQ:
		((CMT_CPRIF_FLDFILSNDREQ_S3G *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FLDFILSNDREQ_S3G *)body)->signal_kind);
		((CMT_CPRIF_FLDFILSNDREQ_S3G *)body)->protocol_type = mnt_com_swap_USHORT(((CMT_CPRIF_FLDFILSNDREQ_S3G *)body)->protocol_type);
		((CMT_CPRIF_FLDFILSNDREQ_S3G *)body)->data_type = mnt_com_swap_USHORT(((CMT_CPRIF_FLDFILSNDREQ_S3G *)body)->data_type);
		((CMT_CPRIF_FLDFILSNDREQ_S3G *)body)->sndbyte_num = mnt_com_swap_USHORT(((CMT_CPRIF_FLDFILSNDREQ_S3G *)body)->sndbyte_num);
		break;

	case CMD_CPRID_FLDFILSNDRES:
		((CMT_CPRIF_FLDFILSNDRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FLDFILSNDRES *)body)->signal_kind);
		((CMT_CPRIF_FLDFILSNDRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_FLDFILSNDRES *)body)->result);
		break;

	case CMD_CPRID_FLDFILRCVREQ:
		((CMT_CPRIF_FLDFILRCVREQ_S3G *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FLDFILRCVREQ_S3G *)body)->signal_kind);
		((CMT_CPRIF_FLDFILRCVREQ_S3G *)body)->protocol_type = mnt_com_swap_USHORT(((CMT_CPRIF_FLDFILRCVREQ_S3G *)body)->protocol_type);
		((CMT_CPRIF_FLDFILRCVREQ_S3G *)body)->ftpdata_type = mnt_com_swap_USHORT(((CMT_CPRIF_FLDFILRCVREQ_S3G *)body)->ftpdata_type);
		((CMT_CPRIF_FLDFILRCVREQ_S3G *)body)->rcvbyte_num = mnt_com_swap_USHORT(((CMT_CPRIF_FLDFILRCVREQ_S3G *)body)->rcvbyte_num);
		break;

	case CMD_CPRID_FLDFILRCVRES:
		((CMT_CPRIF_FLDFILRCVRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FLDFILRCVRES *)body)->signal_kind);
		((CMT_CPRIF_FLDFILRCVRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_FLDFILRCVRES *)body)->result);
		break;

	case CMD_CPRID_FLMTCNCRELREQ:
		((CMT_CPRIF_FLMTCNCRELREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FLMTCNCRELREQ *)body)->signal_kind);
		break;

	case CMD_CPRID_FLMTCNCRELRES:
		((CMT_CPRIF_FLMTCNCRELRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FLMTCNCRELRES *)body)->signal_kind);
		((CMT_CPRIF_FLMTCNCRELRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_FLMTCNCRELRES *)body)->result);
		break;

	case CMD_CPRID_FLMTCMPRELREQ:
		((CMT_CPRIF_FLMTCMPRELREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FLMTCMPRELREQ *)body)->signal_kind);
		break;

	case CMD_CPRID_FLMTCMPRELRES:
		((CMT_CPRIF_FLMTCMPRELRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FLMTCMPRELRES *)body)->signal_kind);
		((CMT_CPRIF_FLMTCMPRELRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_FLMTCMPRELRES *)body)->result);
		break;

	case CMD_CPRID_SVMTCNTREQ:
		((CMT_CPRIF_SVMTCNTREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTCNTREQ *)body)->signal_kind);
		break;

	case CMD_CPRID_SVMTCNTRES:
		((CMT_CPRIF_SVMTCNTRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTCNTRES *)body)->signal_kind);
		((CMT_CPRIF_SVMTCNTRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTCNTRES *)body)->result);
		break;

	case CMD_CPRID_SVMTDATSNDREQ:
		((CMT_CPRIF_SVMTDATSNDREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTDATSNDREQ *)body)->signal_kind);
		((CMT_CPRIF_SVMTDATSNDREQ *)body)->snd_bytnum = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTDATSNDREQ *)body)->snd_bytnum);
		break;

	case CMD_CPRID_SVMTDATSNDRES:
		((CMT_CPRIF_SVMTDATSNDRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTDATSNDRES *)body)->signal_kind);
		((CMT_CPRIF_SVMTDATSNDRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTDATSNDRES *)body)->result);
		break;

	case CMD_CPRID_SVMTDATRCVREQ:
		((CMT_CPRIF_SVMTDATRCVREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTDATRCVREQ *)body)->signal_kind);
		((CMT_CPRIF_SVMTDATRCVREQ *)body)->rcv_bytnum = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTDATRCVREQ *)body)->rcv_bytnum);
		break;

	case CMD_CPRID_SVMTDATRCVRES:
		((CMT_CPRIF_SVMTDATRCVRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTDATRCVRES *)body)->signal_kind);
		((CMT_CPRIF_SVMTDATRCVRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTDATRCVRES *)body)->result);
		break;

	case CMD_CPRID_SVMTCNTRELREQ:
		((CMT_CPRIF_SVMTCNTRELREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTCNTRELREQ *)body)->signal_kind);
		break;

	case CMD_CPRID_SVMTCNTRELRES:
		((CMT_CPRIF_SVMTCNTRELRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTCNTRELRES *)body)->signal_kind);
		((CMT_CPRIF_SVMTCNTRELRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTCNTRELRES *)body)->result);
		break;

	case CMD_CPRID_SVMTFORRELREQ:
		((CMT_CPRIF_SVMTFORRELREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTFORRELREQ *)body)->signal_kind);
		break;

	case CMD_CPRID_SVMTFORRELRES:
		((CMT_CPRIF_SVMTFORRELRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTFORRELRES *)body)->signal_kind);
		((CMT_CPRIF_SVMTFORRELRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_SVMTFORRELRES *)body)->result);
		break;

	case CMD_CPRID_MKMTDATSND:
		((CMT_CPRIF_MKMTDATSND *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MKMTDATSND *)body)->signal_kind);
		((CMT_CPRIF_MKMTDATSND *)body)->mkmt_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MKMTDATSND *)body)->mkmt_kind);
		((CMT_CPRIF_MKMTDATSND *)body)->mt_length = mnt_com_swap_UINT(((CMT_CPRIF_MKMTDATSND *)body)->mt_length);
		((CMT_CPRIF_MKMTDATSND *)body)->mt_signal = mnt_com_swap_UINT(((CMT_CPRIF_MKMTDATSND *)body)->mt_signal);
		break;

	case CMD_CPRID_MKMTCNTREQ:
		((CMT_CPRIF_MKMTCNTREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MKMTCNTREQ *)body)->signal_kind);
		((CMT_CPRIF_MKMTCNTREQ *)body)->mkmt_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MKMTCNTREQ *)body)->mkmt_kind);
		break;

	case CMD_CPRID_MKMTCNTRES:
		((CMT_CPRIF_MKMTCNTRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MKMTCNTRES *)body)->signal_kind);
		((CMT_CPRIF_MKMTCNTRES *)body)->mkmt_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MKMTCNTRES *)body)->mkmt_kind);
		((CMT_CPRIF_MKMTCNTRES *)body)->result = mnt_com_swap_UINT(((CMT_CPRIF_MKMTCNTRES *)body)->result);
		((CMT_CPRIF_MKMTCNTRES *)body)->NG_code = mnt_com_swap_UINT(((CMT_CPRIF_MKMTCNTRES *)body)->NG_code);
		break;

	case CMD_CPRID_MKMTCNTRELREQ:
		((CMT_CPRIF_MKMTCNTRELREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MKMTCNTRELREQ *)body)->signal_kind);
		((CMT_CPRIF_MKMTCNTRELREQ *)body)->mkmt_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MKMTCNTRELREQ *)body)->mkmt_kind);
		((CMT_CPRIF_MKMTCNTRELREQ *)body)->cause = mnt_com_swap_UINT(((CMT_CPRIF_MKMTCNTRELREQ *)body)->cause);
		break;

	case CMD_CPRID_MKMTCNTRELRES:
		((CMT_CPRIF_MKMTCNTRELRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MKMTCNTRELRES *)body)->signal_kind);
		((CMT_CPRIF_MKMTCNTRELRES *)body)->mkmt_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MKMTCNTRELRES *)body)->mkmt_kind);
		((CMT_CPRIF_MKMTCNTRELRES *)body)->result = mnt_com_swap_UINT(((CMT_CPRIF_MKMTCNTRELRES *)body)->result);
		((CMT_CPRIF_MKMTCNTRELRES *)body)->NG_code = mnt_com_swap_UINT(((CMT_CPRIF_MKMTCNTRELRES *)body)->NG_code);
		break;

	case CMD_CPRID_MKMTFORRELREQ:
		((CMT_CPRIF_MKMTFORRELREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MKMTFORRELREQ *)body)->signal_kind);
		((CMT_CPRIF_MKMTFORRELREQ *)body)->mkmt_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MKMTFORRELREQ *)body)->mkmt_kind);
		((CMT_CPRIF_MKMTFORRELREQ *)body)->cause = mnt_com_swap_UINT(((CMT_CPRIF_MKMTFORRELREQ *)body)->cause);
		break;

	case CMD_CPRID_MKMTFORRELRES:
		((CMT_CPRIF_MKMTFORRELRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MKMTFORRELRES *)body)->signal_kind);
		((CMT_CPRIF_MKMTFORRELRES *)body)->mkmt_kind = mnt_com_swap_USHORT(((CMT_CPRIF_MKMTFORRELRES *)body)->mkmt_kind);
		((CMT_CPRIF_MKMTFORRELRES *)body)->result = mnt_com_swap_UINT(((CMT_CPRIF_MKMTFORRELRES *)body)->result);
		break;

	case CMD_CPRID_FTPPORTSETREQ:
		((CMT_CPRIF_FTPPORTSETREQ *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FTPPORTSETREQ *)body)->signal_kind);
		((CMT_CPRIF_FTPPORTSETREQ *)body)->portnum = mnt_com_swap_USHORT(((CMT_CPRIF_FTPPORTSETREQ *)body)->portnum);
		break;

	case CMD_CPRID_FTPPORTSETRES:
		((CMT_CPRIF_FTPPORTSETRES *)body)->signal_kind = mnt_com_swap_USHORT(((CMT_CPRIF_FTPPORTSETRES *)body)->signal_kind);
		((CMT_CPRIF_FTPPORTSETRES *)body)->result = mnt_com_swap_USHORT(((CMT_CPRIF_FTPPORTSETRES *)body)->result);
		break;

	default:
		cm_Assert( CMD_ASL_DBGLOW, signal_kind,
			(UCHAR*)"[m_mk_com_swap_convert] signal_kind is wrong" );
		break;
	}
	return;
}
#endif
/* matsumoto modify end */

/* @} */



