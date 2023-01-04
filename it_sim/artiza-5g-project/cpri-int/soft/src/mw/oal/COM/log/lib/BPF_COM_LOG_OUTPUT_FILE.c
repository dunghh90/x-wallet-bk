/*!
 * @skip  $ld:$
 * @file  BPF_COM_LOG_OUTPUT_FILE.c
 * @brief command file open function.
 * @date  2013/10/28 ALPHA)Yokoyama create
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
 */
 
/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "bpf_i_com_log.h"

/*!
 *  @brief  This fuction will be called to get log.
 *  @note   This function is processed as follows.\n
 *          - 種別に指定された値に応じてファイルを開く
 *  @param  -
 *  @return file discriptor
 *  @retval 0x0000：正常終了
 *  @retval 0x0001：異常終了
 *  @date   2013/11/04 ALPHA)Yokoyama Create
 */

FILE* BPF_COM_LOG_OUTPUT_FILE_OPEN( CHAR* open_filename, UINT kind, UCHAR* output_filename )
{
	FILE						*fp;
	CHAR						a_date[256];
	T_RRH_SYSTEM_TIME			tDateTime;
	CHAR						filename_date[256];
	CHAR						a_outdirfname[256];
	
	/* ファイル名生成									*/
	/* INパラのファイル名に日時と拡張子を付与する		*/
	/* Get System time									*/
	memset(&tDateTime, D_RRH_ZERO, sizeof(tDateTime));
	(VOID)BPF_HM_DEVC_GETTIME(&tDateTime);
	
	memset(&a_date, D_RRH_ZERO, sizeof(a_date));
	/* ファイル名に時刻情報を付与	*/
	snprintf(a_date, sizeof(a_date),"%04u%02u%02u%02u%02u%02u%02u",
						tDateTime.year, tDateTime.month, tDateTime.day,
						tDateTime.hour, tDateTime.min,   tDateTime.sec, tDateTime.msec );
	
	/* ファイル名に時刻情報を付与	*/
	memset( filename_date, 0 , sizeof( filename_date ) );
	strcpy( filename_date, open_filename );
	strcat( filename_date, "_");
	strcat(	filename_date, a_date);
	
	/* 拡張子を付与	*/
	strcat(	filename_date, ".log" );
	
	/* 出力先ディレクトリ決定				*/
	/* 種別によって出力先を変える			*/
	switch( kind )
	{
	case E_BPF_LOG_CMDLOG_OUTDIR_NORMAL:
		/* 通常debugの場合	*/
		strcpy(a_outdirfname,D_BPF_LOG_CMDLOG_OUTDIR_NORMAL);
		break;
		
	case E_BPF_LOG_CMDLOG_OUTDIR_ALM:
		/* 障害発生時の場合	*/
		strcpy(a_outdirfname,D_BPF_LOG_CMDLOG_OUTDIR_ALM);
		break;
		
	case E_BPF_LOG_CMDLOG_OUTDIR_SEND:
		/* 運用ログ出力(S3G用)の場合	*/
		strcpy(a_outdirfname,D_BPF_LOG_CMDLOG_OUTDIR_SEND);
		break;
		
	case E_BPF_LOG_CMDLOG_OUTDIR_ALM_HEALTCHK:
		/* 障害発生時(Health Check断)の場合	*/
		strcpy(a_outdirfname,D_BPF_LOG_CMDLOG_OUTDIR_ALM_HEALTCHK);
		break;
		
	case E_BPF_LOG_CMDLOG_OUTDIR_SEND_3G:
		/* 運用ログ出力(3G用)の場合	*/
		strcpy(a_outdirfname,D_BPF_LOG_CMDLOG_OUTDIR_SEND_3G);
		break;
		
	case E_BPF_LOG_CMDLOG_OUTDIR_ALM_RST:
		/* 障害発生(RESET有)の場合	*/
		strcpy(a_outdirfname,D_BPF_LOG_CMDLOG_OUTDIR_ALM_RST);
		break;
		
	default:
		break;
	}
	
	/* ディレクトリとファイル名を合体する	*/
	strcat(a_outdirfname, filename_date);
	
	/* Full Pathを通知	*/
	memcpy( output_filename, a_outdirfname, sizeof(a_outdirfname) );
	
	/* 対象ファイルを開いてユーザに通知する	*/
	fp = fopen( a_outdirfname, "wb" );
	
	return fp;
}

/*!
 *  @brief  This fuction will be called to get log.
 *  @note   This function is processed as follows.\n
 *          - 引数に指定されたfileをcloseする。
 *  @param  -
 *  @return UINT
 *  @retval 0x0000：正常終了
 *  @retval 0x0001：異常終了
 *  @date   2013/11/04 ALPHA)Yokoyama Create
 */

UINT  BPF_COM_LOG_OUTPUT_FILE_CLOSE( FILE *fp )
{
	/* file close */
	fclose( fp );
	
	return 0x00000000;
}

/* @} */
