/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_i_ru_facc.h
 *  @brief  Definitions for BPF FACC Local header
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_I_RU_FACC_H_
#define _BPF_I_RU_FACC_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_RU_FACC.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <assert.h>
#include <errno.h>

/********************************************************************************************************************/
/* Internal Definition                                                                                              */
/********************************************************************************************************************/
#define BPF_RU_FACC_OK        0 /* FACC内部処理OK */
#define BPF_RU_FACC_NG        1 /* FACC内部処理NG */
#define BPF_RU_FACC_SYS_NG   -1 /* FACCシステムコールNG */
#define BPF_RU_FACC_SIZE_ZERO 0 /* FACC書き込み・読み込みサイズ0 */
#define BPF_RU_FACC_NOTABLE   2 /* テーブル設定なし */
#define BPF_RU_FACC_FLAG_ON    1 /* FLG ON  */
#define BPF_RU_FACC_FLAG_OFF   0 /* FLG OFF */

/* For Command execute table free mngtbl */
#define BPF_RU_FACC_LISTEND  0xFFFF
#define BPF_RU_FACC_LISTUSE  0xFFFE
#define BPF_RU_FACC_EXTBL_MAX  100
typedef struct{
	int free_num;
	int free_head;
	int free_tail;
} bpf_ru_facc_extbl_free_mngtable_head_t;


#define BPF_RU_CMD_LENGTH_MAX        512  /* str command length */
#define BPF_RU_CMD_NICE_ADJ          20

extern unsigned int facc_buf_log_flg;
extern unsigned int facc_buf_get_count;
extern unsigned int facc_buf_rel_count;

/********************************************************************************************************************/
/* Internal Structure                                                                                              */
/********************************************************************************************************************/


/********************************************************************************************************************/
/* Internal enum                                                                                              */
/********************************************************************************************************************/


/********************************************************************************************************************/
/* Internal Function Routine Prototype                                                                                              */
/********************************************************************************************************************/
/* フルパス生成 */
int bpf_ru_facc_makepathname(
	const bpf_ru_facc_dpathname *dir_path_name,    /* ディレクトリパス名    (I) */
	const bpf_ru_facc_fname *file_name        ,    /* ファイル名            (I) */
	char  *pattname);                              /* フルパス名            (O) */

/* 排他制御(ロック設定) */
int bpf_ru_facc_filelock(
	int fdescriptor        ,                       /* ファイルディスクリプタ (I) */
	bpf_ru_facc_openmode open_mode,                /* オープンモード         (I) */
	bpf_ru_facc_excmode  exc_mode);                /* 排他制御モード         (I) */

/* 排他制御(ロック解除) */
int bpf_ru_facc_fileunlock(
	int fdescriptor);                              /* ファイルディスクリプタ (I) */

/* ディレクトリパス生成 */
int bpf_ru_facc_makepathname_dir(
	const bpf_ru_facc_dpathname *dir_path_name,    /* ディレクトリパス名    (I) */
	char  *pattname);                              /* フルパス名            (O) */

/* For command execute table */
void bpf_ru_facc_cmn_init_exetbl( void );
int bpf_ru_facc_cmn_set_exetbl( pid_t pid, unsigned int *exetbl_id );
pid_t bpf_ru_facc_cmn_delete_exetbl( unsigned int exetbl_id );
pid_t bpf_ru_facc_cmn_chk_exetbl( unsigned int exetbl_id );

/********************************************* EOF ******************************************************************/
#endif /* _BPF_I_RU_FACC_H_ */

