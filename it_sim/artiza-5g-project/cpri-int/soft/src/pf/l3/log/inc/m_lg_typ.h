/*!
 * @skip  $ld:$
 * @file  M_lg_typ.h
 * @brief Structure definition for LOG thread.
 * @date  2008/07/30 FFCS)Wuh Create
 *	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
 */

#ifndef M_LG_TYP_HEAD
#define M_LG_TYP_HEAD

/****************************************************************************/
/* 障害ログ管理テーブル(LGT_ALMLOGESC_T  lgw_almlogmng_tbl)					*/
/****************************************************************************/
/*! 
 * @brief trouble log mangement table 
 */
typedef struct
{
	USHORT	write_men ;						/* 書込み面指定					*/
	USHORT	get_log_sta ;					/* ログ取得状態(TSK状態)		*/
	UINT	snd_logsize ;					/* 送信ログサイズ				*/
	USHORT	div_snd_num ;					/* 分割送信数					*/
	USHORT	snd_cmplt_num ;					/* 送信完了数					*/
	USHORT	in_parm1 ;						/* 入力パラメータ1				*/
	USHORT	in_parm2 ;						/* 入力パラメータ2				*/
	CHAR	logfile_name[256] ;				/* ログファイル名				*/
	CHAR	logfile_name_path[256];			/* ログファイル名(Full Path)	*/
	UINT	realsize;						/* 実size						*/
}LGT_ALMLOGESC_T ;

/****************************************************************************/
/* 応答送信先管理テーブル													*/
/****************************************************************************/
/*! 
 * @brief response manage table 
 */
typedef struct
{
	UINT	dstPQID ;						/* 送信先PQID					*/
	UINT	dstTQID ;						/* 送信先TQID					*/
}LGT_RSPMNG_T ;

/****************************************************************************/
/* RE障害ログ取得応答管理テーブル													*/
/****************************************************************************/
/*! 
 * @brief RE Get TroubleLog response manage table 
 */
typedef struct
{
	USHORT				result;		/**< 応答結果 */
	UCHAR				filename[256];	/**< ファイル名	 */
	UINT				datsize;	/**< トータルデータサイズ */
}LGT_REOBSLOGRESMNG_T ;

#endif /* M_LG_TYP_HEAD */
