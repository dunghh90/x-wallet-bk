/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_RU_FACC.h
 *  @brief  Definitions for BPF_RU_FACC API
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#ifndef _BPF_RU_FACC_H_
#define _BPF_RU_FACC_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_COM.h"
#include <fcntl.h>


/********************************************************************************************************************/
/* External Definition                                                                                              */
/********************************************************************************************************************/
/********************************************************************************************************************/
/* define                                                                                                           */
/********************************************************************************************************************/
/* Return code:終了コード */
#define BPF_RU_FACC_COMPLETE			0			/* 正常終了						*/
#define BPF_RU_FACC_ERR_DPATHNAME		1			/* ディレクトリパス名異常		*/
#define BPF_RU_FACC_ERR_FNAME			2			/* ファイル名異常				*/
#define BPF_RU_FACC_ERR_OMODE			3			/* オープンモード異常			*/
#define BPF_RU_FACC_ERR_RSIZE			4			/* 読み込みサイズ異常			*/
#define BPF_RU_FACC_ERR_WSIZE			5			/* 書き込みサイズ異常			*/
#define BPF_RU_FACC_ERR_WDATA			6			/* 書き込みデータ格納領域異常	*/
#define BPF_RU_FACC_ERR_SMODE			7			/* シークモード異常				*/
#define BPF_RU_FACC_ERR_LISTNUM			8			/* 最大読み出し可能数異常		*/
#define BPF_RU_FACC_ERR_LIST			9			/* ディレクトリ内一覧取得失敗	*/
#define BPF_RU_FACC_ERR_DMODE			10			/* ディレクトリ許可属性異常		*/
#define BPF_RU_FACC_ERR_ENOENT			11			/* No such file or directory    */
#define BPF_RU_FACC_ERR_PARAM			12			/* parameter check failed		*/
#define	BPF_RU_FACC_ERR_XML				13			/* XMLフォーマットエラー		*/

#define BPF_RU_FACC_ERR_EXC				-1			/* 排他失敗						*/
#define BPF_RU_FACC_ERR_FOPEN			-2			/* ファイルオープン失敗			*/
#define BPF_RU_FACC_ERR_FREAD			-3			/* ファイル読み込み失敗			*/
#define BPF_RU_FACC_ERR_FWRITE			-4			/* ファイル書き込み失敗			*/
#define BPF_RU_FACC_ERR_FSEEK			-5			/* ファイルシーク失敗			*/
#define BPF_RU_FACC_ERR_FCLOSE			-6			/* ファイルクローズ失敗			*/
#define BPF_RU_FACC_ERR_FCOPY			-7			/* ファイルコピー失敗			*/
#define BPF_RU_FACC_ERR_FMOVE			-8			/* ファイル移動失敗				*/
#define BPF_RU_FACC_ERR_FREMOVE			-9			/* ファイル削除失敗				*/
#define BPF_RU_FACC_ERR_FREMOVEALL		-10			/* ディレクトリ内ファイル一括消去失敗	*/
#define BPF_RU_FACC_ERR_MKDIR			-11			/* ディレクトリ作成失敗			*/
#define BPF_RU_FACC_ERR_DREMOVE			-12			/* ディレクトリ削除失敗			*/
#define BPF_RU_FACC_ERR_DMOVE			-13			/* ディレクトリ名変更失敗		*/
#define BPF_RU_FACC_ERR_DFGET			-14			/* ディスク使用量取得失敗		*/
#define BPF_RU_FACC_ERR_FSIZE			-15			/* ファイルサイズ取得失敗		*/



#define BPF_RU_FACC_DPATHNAME_LEN		256			/* ディレクトリパス長			*/
#define BPF_RU_FACC_FNAME_LEN		    64			/* ファイル名長 				*/
#define BPF_RU_FACC_FILIST_MAX			1024		/* ファイル一覧最大				*/
#define BPF_RU_FACC_RDATA_MIN			1			/* 読み込みサイズ最小			*/
#define BPF_RU_FACC_RDATA_MAX			0x2600000	/* 読み込みサイズ最大(38M)		*/
#define BPF_RU_FACC_WDATA_MIN			1			/* 書き込みサイズ最小			*/
#define BPF_RU_FACC_WDATA_MAX			32767		/* 書き込みサイズ最大			*/
#define BPF_RU_FACC_CREATE				O_CREAT		/* ファイル作成有り 			*/
#define BPF_RU_FACC_FTYPE_FILE			0			/* ファイルタイプ				*/
#define BPF_RU_FACC_FTYPE_DIR			1			/* ファイルタイプ				*/

#define BPF_RU_FACC_NO_CREATE 			0			/* ファイル作成無し				*/
#define BPF_RU_FACC_CREATE				O_CREAT		/* ファイル作成有り 			*/
#define BPF_RU_FACC_APPEND				O_APPEND	/* append mode					*/
#define BPF_RU_FACC_TRUNC				O_TRUNC		/* truncate mode				*/
#define BPF_RU_FACC_FTYPE_FILE			0			/* ファイルタイプ				*/
#define BPF_RU_FACC_FTYPE_DIR			1			/* ファイルタイプ				*/

/********************************************************************************************************************/
/* enum                                                                                                             */
/********************************************************************************************************************/
/* オープンモード */
typedef enum {
	FACC_READ				= 0x00000000,			/* 読み込み専用			*/
	FACC_WRITE				= 0x00000001,			/* 書き込み専用			*/
	FACC_READWRITE			= 0x00000002			/* 読み込み、書き込み	*/
} bpf_ru_facc_openmode;

/* シークモード */
typedef enum {
	FACC_SEEK_SET			= 0x00000000,			/* オフセットをファイル先頭からoffsetバイトに設定します */
	FACC_SEEK_CUR			= 0x00000001,			/* オフセットを現在位置にoffsetバイトを足した位置に設定します */
	FACC_SEEK_END			= 0x00000002			/* オフセットをファイルサイズにoffsetバイトを足した位置に設定します */
} bpf_ru_facc_seekmode;

typedef enum {
	FACC_ADV				= 0x00000000,			/* Advisory Lock		*/
	FACC_MAN				= 0x00000001			/* Mandatory Lock		*/
} bpf_ru_facc_excmode;

/********************************************************************************************************************/
/* typedef                                                                                                          */
/********************************************************************************************************************/
/* ディレクトリパス名 */
typedef struct {
	char dpathname[BPF_RU_FACC_DPATHNAME_LEN];
} bpf_ru_facc_dpathname;

/* ファイル名 */
typedef struct {
	char fname[BPF_RU_FACC_FNAME_LEN+1];
} bpf_ru_facc_fname;

/* ディレクトリファイル一覧構造体 */
typedef struct {
    char file_name[BPF_RU_FACC_DPATHNAME_LEN];
    unsigned int  file_type;
    unsigned int  file_size; 
} bpf_ru_facc_dirinfo;

typedef struct {
	unsigned int	file_num;								/* 取得ファイル数	*/
	bpf_ru_facc_dirinfo	dirinfo[BPF_RU_FACC_FILIST_MAX];	/* 取得ファイル情報	*/
} bpf_ru_facc_dirlist;

/* ファイル一覧構造体 */
typedef struct {
    char file_name[BPF_RU_FACC_FNAME_LEN+1];
    unsigned int  file_size;
} bpf_ru_facc_fileinfo;

typedef struct {
    unsigned int file_num;									/* 取得ファイル数 */
    bpf_ru_facc_fileinfo file_info[BPF_RU_FACC_FILIST_MAX];	/* 取得ファイル情報 */
} bpf_ru_facc_filelist;


/* オフセット */
typedef int bpf_ru_facc_off_t ;


/********************************************************************************************************************/
/* Function Prototype                                                                                               */
/********************************************************************************************************************/
/* ファイルオープン */
int BPF_RU_FACC_FOPEN(
			const bpf_ru_facc_dpathname *dir_path_name,		/* ディレクトリパス名				*/
			const bpf_ru_facc_fname *file_name,				/* ファイル名						*/
			bpf_ru_facc_openmode open_mode,					/* オープンモード					*/
			unsigned int create_mode,						/* ファイル作成モード				*/
			int *file_descriptor,							/* ファイルディスクリプタ			*/
			int *errcd);									/* 詳細NGコード						*/

/* ファイル読み込み */
int BPF_RU_FACC_FREAD(
			int file_descriptor,							/* ファイルディスクリプタ			*/
			unsigned int read_size,							/* 読み込みサイズ					*/
			char *read_data,								/* 読み込みデータ格納領域			*/
			unsigned int *act_read_size,					/* 実読み出しサイズ					*/
			int *errcd);									/* 詳細NGコード						*/

/* ファイル書き込み */
int BPF_RU_FACC_FWRITE(
			int file_descriptor,							/* ファイルディスクリプタ			*/
			unsigned int write_size,						/* 書き込みサイズ					*/
			const char *write_data,							/* 書き込みデータ格納領域			*/
			unsigned int *act_write_size,					/* 実書き込みデータサイズ			*/
			int *errcd);									/* 詳細NGコード						*/

/* ファイルシーク */
int BPF_RU_FACC_FSEEK(
			int file_descriptor, 							/* ファイルディスクリプタ			*/
			bpf_ru_facc_off_t offset,						/* オフセット						*/
			bpf_ru_facc_seekmode seek_mode,					/* シークモード						*/
			int *errcd);									/* 詳細NGコード						*/

/* ファイルクローズ */
int BPF_RU_FACC_FCLOSE(
			int file_descriptor, 							/* ファイルディスクリプタ			*/
			int *errcd);									/* 詳細NGコード						*/

/* ファイルコピー */
int BPF_RU_FACC_FCOPY(
			const bpf_ru_facc_dpathname *src_dir_path_name,	/* コピー元ディレクトリパス名		*/
			const bpf_ru_facc_dpathname *dst_dir_path_name,	/* コピー先ディレクトリパス名		*/
			const bpf_ru_facc_fname *src_file_name,			/* sourceファイル名					*/
			const bpf_ru_facc_fname *dst_file_name,			/* destinationファイル名			*/
			int *errcd);									/* 詳細NGコード						*/

/* ファイル移動 */
int BPF_RU_FACC_FMOVE(
			const bpf_ru_facc_dpathname *src_dir_path_name,	/* 移動元ディレクトリパス名			*/
			const bpf_ru_facc_dpathname *dst_dir_path_name,	/* 移動先ディレクトリパス名			*/
			const bpf_ru_facc_fname *src_file_name,			/* sourceファイル名					*/
			const bpf_ru_facc_fname *dst_file_name,			/* destinationファイル名			*/
			int *errcd);									/* 詳細NGコード						*/

/* ファイル削除 */
int BPF_RU_FACC_FREMOVE(
			const bpf_ru_facc_dpathname *dir_path_name,		/* ディレクトリパス名				*/
			const bpf_ru_facc_fname *file_name,				/* ファイル名						*/
			int *errcd);									/* 詳細NGコード						*/

/* ファイルサイズ取得 */
int BPF_RU_FACC_FSIZE(
			const bpf_ru_facc_dpathname *dir_path_name,		/* ディレクトリパス名				*/
			const bpf_ru_facc_fname *file_name,				/* ファイル名						*/
			long *file_sz,									/* ファイルサイズ					*/
			int *errcd);									/* 詳細NGコード						*/


/* ディレクトリ内一覧取得 */
int BPF_RU_FACC_LIST(
			const bpf_ru_facc_dpathname *dir_path_name,		/* ディレクトリパス名				*/
			unsigned int max_file_list_num,					/* 最大読み出し可能数				*/
			bpf_ru_facc_dirlist *dir_list,					/* ディレクトリファイル一覧構造体	*/
			unsigned int *list_num,							/* 実読み出しファイル数				*/
			int *errcd);									/* 詳細NGコード						*/

/* ディレクトリ内ファイル一覧取得 */
int BPF_RU_FACC_FLIST(
			const bpf_ru_facc_dpathname *dir_path_name,		/* ディレクトリパス名				*/
			unsigned int max_file_list_num,					/* 最大読み出し可能ファイル数		*/
			bpf_ru_facc_filelist *file_list,				/* ファイル一覧構造体				*/
			unsigned int *act_file_list_num,				/* 実読み出しファイル数				*/
			int *errcd);									/* 詳細NGコード						*/

/* ディレクトリ内ファイル一括消去 */
int BPF_RU_FACC_FREMOVEALL(
			const bpf_ru_facc_dpathname * dir_path_name,	/* ディレクトリパス名				*/
			int *errcd);									/* 詳細NGコード						*/

/* ディレクトリ作成 */
int BPF_RU_FACC_MKDIR(
			const bpf_ru_facc_dpathname * dir_path_name,	/* ディレクトリパス名				*/
			int dir_mode,									/* ディレクトリ許可属性				*/
			int *errcd);									/* 詳細NGコード						*/

/* ディレクトリ削除 */
int BPF_RU_FACC_DREMOVE(
			const bpf_ru_facc_dpathname *dir_path_name,		/* ディレクトリパス名				*/
			int *errcd);									/* 詳細NGコード						*/

/* ディレクトリ名変更 */
int BPF_RU_FACC_DMOVE(
			const bpf_ru_facc_dpathname *src_dir_path_name,	/* 変更元ディレクトリパス名			*/
			const bpf_ru_facc_dpathname *dst_dir_path_name,	/* 変更ディレクトリパス名			*/
			int *errcd);									/* 詳細NGコード						*/

/* ディスク使用量取得 */
int BPF_RU_FACC_DFGET(
			const bpf_ru_facc_dpathname *dir_path_name,		/* 対象ディレクトリパス名			*/
			unsigned int *total_size,						/* トータルサイズ					*/
			unsigned int *use_size,							/* 使用中サイズ						*/
			int *errcd);									/* 詳細NGコード						*/

/********************************************* EOF ******************************************************************/
#endif /* _BPF_RU_FACC_H_ */

