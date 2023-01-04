/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	f_BPF_RU_FACC_DFGET.c
 *	@brief	File Access Control Get filesystem disk space usage.
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_FACC /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_i_ru_facc.h"
#include <sys/statvfs.h>

/** @addtogroup BPF_RU_FACC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  File Access Control Get filesystem disk space usage.
 *  @note   API function. 
 *  @param  dir_path_name   [in]  directory path name
 *  @param  total_size      [out] total space  (Kbyte)
 *  @param  use_size        [out] use space (Kbyte)
 *  @param  errcd           [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_DPATHNAME  parameter error
 *  @retval BPF_RU_FACC_ERR_DFGET      failed
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_FACC_DFGET(const bpf_ru_facc_dpathname *dir_path_name, unsigned int *total_size, unsigned int *use_size, int *errcd)
{
	int ret;
	struct statvfs buf;
	
	char dir_pthname[BPF_RU_FACC_DPATHNAME_LEN];
	
	if (dir_path_name == NULL){
		return BPF_RU_FACC_ERR_DPATHNAME;
	}
	
	memset(dir_pthname,0,sizeof(dir_pthname));
	memset(&buf,0,sizeof(buf));
	
	/************************************************
	* Get  filesystem disk space usage processing.  *
	************************************************/
	/* dirフルパス生成を行う */
	ret = bpf_ru_facc_makepathname_dir(	dir_path_name,     /* ディレクトリパス名 (I) */
										dir_pthname);      /* pathname           (O) */
	
	/* 生成結果を判定する */
	if (ret != BPF_RU_FACC_OK){
		return ret;
	}


/*
struct statvfs {
	unsigned long  f_bsize;     ファイルシステムのブロックサイズ 
	unsigned long  f_frsize;    フラグメントサイズ 
	fsblkcnt_t     f_blocks;    ファイルシステムのサイズ (f_frsize 単位) 
	fsblkcnt_t     f_bfree;     解放されているブロック数 
	fsblkcnt_t     f_bavail;    ルート以外の解放されているブロック数 
	fsfilcnt_t     f_files;     inode 数 
	fsfilcnt_t     f_ffree;     解放されている inode の数 
	fsfilcnt_t     f_favail;    ルート以外の解放されている inode の数 
	unsigned long  f_fsid;      ファイルシステム ID 
	unsigned long  f_flag;      マウントフラグ 
	unsigned long  f_namemax;   ファイル名の長さの最大値 
};
*/
	
	ret = statvfs(dir_pthname,&buf);
	if( ret == BPF_RU_FACC_SYS_NG){
		*errcd = errno;
		switch (errno) {
			case ENOENT:
			case ENOTDIR:
				return BPF_RU_FACC_ERR_DPATHNAME;
			
			default:
				return BPF_RU_FACC_ERR_DFGET;
		}
	}
	
	const int KB = 1024;
	const double ROUND = 0.5; /* 四捨五入 */
	
	/* オーバーフローしないよう、doubleでキャスト */
	/* 全容量(KB) */
	*total_size = (unsigned int)((double) buf.f_blocks * buf.f_frsize / KB + ROUND);
	
	/* 使用量(KB) */
	*use_size   = (unsigned int)((double) (buf.f_blocks - buf.f_bfree) * buf.f_frsize / KB + ROUND);
	
    return BPF_RU_FACC_COMPLETE;
}
/* @} */

/* @} */
