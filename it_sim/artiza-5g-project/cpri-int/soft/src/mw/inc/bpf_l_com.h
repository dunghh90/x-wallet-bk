/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_l_com.h
 *  @brief  Definitions for BPF Common library
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_L_COM_H_
#define _BPF_L_COM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include "BPF_COM.h"
#include "bpf_l_com.h"
#include "bpf_l_ru_hrtm.h"
#include "bpf_l_ru_itcm.h"
#include "bpf_l_ru_ipcm.h"
#include "bpf_l_ru_mblm.h"
#include "bpf_l_hm_devc.h"

/* Magic Number */
#define BPF_COM_NUM0   0
#define BPF_COM_NUM1   1
#define BPF_COM_NUM2   2
#define BPF_COM_NUM3   3
#define BPF_COM_NUM4   4
#define BPF_COM_NUM5   5
#define BPF_COM_NUM6   6
#define BPF_COM_NUM7   7
#define BPF_COM_NUM8   8
#define BPF_COM_NUM9   9
#define BPF_COM_NUM10  10
#define BPF_COM_NUM20  20
#define BPF_COM_NUM30  30
#define BPF_COM_NUM40  40
#define BPF_COM_NUM50  50
#define BPF_COM_NUM100 100
#define BPF_COM_NUM16384 16384
#define BPF_COM_NUM_MINUS1   (-1)

#define BPF_COM_SYSCALL_NG (-1)

/* BPF  Shared Memory Mode*/
#define BPF_COM_SEMFLG    IPC_CREAT|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH

/* Flag ON/OFF */
#define BPF_COM_FLAG_OFF        0
#define BPF_COM_FLAG_ON         1

/* LOG Level */
#define BPF_COM_LOG_TRACE       0
#define BPF_COM_LOG_ERR_L3      1
#define BPF_COM_LOG_ERR_L2      2
#define BPF_COM_LOG_ERR_L1      3
#define BPF_COM_LOGLEVEL_MAX    3

/* LOG */
#define BPF_COM_LOG_EVENT_MAX           128   /* イベント長最大 */
#define BPF_COM_LOG_DATALEN_MAX         128   /* データ長最大   */

#define BPF_COM_IPCKEY_MASK_ITCM        0x00000000
#define BPF_COM_IPCKEY_MASK_HRTM        0x00000001
#define BPF_COM_IPCKEY_MASK_MBLM        0x00000002
#define BPF_COM_IPCKEY_MASK_LOG         0x00000003
#define BPF_COM_IPCKEY_MASK_DEVC        0x00000004

#define BPF_COM_IPCKEY_MASK_IPCM_USER    0x10000000  /* プロセス間通信用ユーザ  */
#define BPF_COM_IPCKEY_MASK_IPCM_DUMMY   0x20000000  /* プロセス間通信用Dummy   */
#define BPF_COM_IPCKEY_MASK_INTERNAL     0x30000000  /* BPF内部用               */

/* BPF内部用 IPCキー生成マクロ */
/*  構成  [3|xx|yy|zzz](Hex)   */
/*  xx(6bit)   : disc1  BPF内部キー生成用FB識別    0～0x3F  */
/*  yy(12bit)  : disc2  プロセス識別子(ユーザ)     0～0xFFF */
/*  zz(10bit) : seq    通し番号                    0～0x3FF */
/*  yy はProcess descriptorと同じ                           */
#define BPF_COM_CREATE_INTERNAL_IPCKEY(disc1,disc2,seq) \
  ( (key_t)((BPF_COM_IPCKEY_MASK_INTERNAL)|(((disc1)&0x0000003F)<<22)|\
  (((disc2)&0x00000FFF)<<10)|(((seq)&0x00000FFF))) )

/* ---------------------------------------------------- */
/* Process descriptor                                   */
/* 31                     16         8           0      */
/* +-----------------------+-----------+-----------+    */
/* |         (1)           |    (2)    |    (3)    |    */
/* +-----------------------+-----------+------ ----+    */
/* (1) temporary                       0x0000(fix)      */
/* (2) process table kind      Initial 0                */
/*                             RM      1                */
/*                             BPF     2                */
/*                             USER    3                */
/*                             APL     4                */
/* (3) process number                  0-63             */
/*                                                      */
/*                                                      */
/* get process table kind (a:process descriptor)   */
#define bpf_com_get_proc_tkind(a) ( ((a)&0x0000FF00)>>8 )
/* get process number (a:process descriptor) */
#define bpf_com_get_proc_num(a)  ( (a)&0x000000FF )
/* make process descriptor (a:process table kind, b:process number) */
#define bpf_com_make_proc_desc(a,b) ( (((a)&0x000000FF)<<8)|((b)&0x000000FF) )
/* Max process number */
#define BPF_COM_PNUM         64
/* process table kind */
#define BPF_COM_PTKIND_APL   0
#define BPF_COM_PTKIND_PF    1

/* 自プロセスのプロセス識別子取得 */
#define BPF_COM_GET_MY_PROCESS_DESC() (bpf_com_my_proc_descriptor)
/* 自プロセスのプロセス識別子設定 */
#define BPF_COM_SET_MY_PROCESS_DESC(a) (bpf_com_my_proc_descriptor=(a))

#define BPF_COM_LOG_INTER_OK	      0    /* OK */
#define BPF_COM_LOG_INTER_NG	      1    /* NG */
#define BPF_COM_LOG_INTER_ERR_SYS     2    /* システムコール失敗 */
#define BPF_COM_LOG_ERR_ASSIGN        3    /* ASSIGN NG */

/* BPF内部用 KEYID	*/
/* 2021/03/04 M&C) Update for intermediate device - Add start */
// #define BPF_RU_IPCM_SHMID_TASK_S			0x10010000						/* 一般(タスク間通信)用		KEYID TOP	*/
// #define BPF_RU_IPCM_SHMID_TASK_M			0x10020000						/* 一般(タスク間通信)用		KEYID TOP	*/
// #define BPF_RU_IPCM_SHMID_TASK_L			0x10030000						/* 一般(タスク間通信)用		KEYID TOP	*/
/* 2021/03/04 M&C) Update for intermediate device - End start */

/* 2021/03/04 M&C) Update for intermediate device - Add start */
#define BPF_RU_IPCM_SHMID_LAPBS_S			0x10010000						/* LAPB通信(LAPB用)-送信用	KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_LAPBS_M			0x10020000						/* LAPB通信(LAPB用)-送信用	KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_LAPBS_L			0x10030000						/* LAPB通信(LAPB用)-送信用	KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_LAPBS_LL			0x10040000						/* LAPB通信(LAPB用)-送信用	KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_LAPBR_S			0x10050000						/* LAPB通信(LAPB用)-受信用	KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_LAPBR_M			0x10060000						/* LAPB通信(LAPB用)-受信用	KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_LAPBR_L			0x10070000						/* LAPB通信(LAPB用)-受信用	KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_LAPBR_LL			0x10080000						/* LAPB通信(LAPB用)-受信用	KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_MONITOR_S			0x10090000						/* モニタ用					KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_MONITOR_M			0x100A0000						/* モニタ用					KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_MONITOR_L			0x100B0000						/* モニタ用					KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_MONITOR_LL		0x100C0000						/* モニタ用					KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_TASK_S			0x100D0000						/* 一般(タスク間通信)用		KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_TASK_M			0x100E0000						/* 一般(タスク間通信)用		KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_TASK_L			0x100F0000						/* 一般(タスク間通信)用		KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_TASK_LL			0x10100000						/* 一般(タスク間通信)用		KEYID TOP	*/
#define BPF_RU_IPCM_SHMID_MTMMLR_BUFF		0x10110000						/* MTM/MLR用				KEYID TOP	*/
/* 2021/03/04 M&C) Update for intermediate device - End start */

#define BPF_RU_IPCM_SHMID_START				BPF_RU_IPCM_SHMID_LAPBS_S		/* OFFSET								*/
#define BPF_RU_IPCM_SHMID_OFS				0x00010000						/* OFFSET								*/

/* @} */

/* BPF内部用 MessageSize	*/
#define BPF_MESSAGE_SIZE_S					0x00001000
#define BPF_MESSAGE_SIZE_M					0x00010000
#define BPF_MESSAGE_SIZE_L					0x00100000

/* -------------------------------- */
/* Debug log MACRO                  */
/* -------------------------------- */
/* Collect LOG */
#define bpf_com_dbglog(a, b, c, d) \
    do{\
		bpf_com_dbglog_func(a,__FILE__,__func__, __LINE__, b, c, d);\
	}while(0)

/* caution : ## is only GCC */
#define bpf_com_dbglog_fmt(a, b, ...) \
    do{\
	    bpf_com_dbglog_fmt_func(a,__FILE__,__func__, __LINE__, b, ##__VA_ARGS__);\
	}while(0)

/* -------------------------------- */
/* Function Prototype               */
/* -------------------------------- */
/* LOG */
void bpf_com_dbglog_func(unsigned int, const char*, const char*, unsigned int, char*, void*, unsigned int);
void bpf_com_dbglog_fmt_func(unsigned int, const char*,const char*, unsigned int, char*, ...);
int  bpf_com_log_assign(unsigned int);
int  bpf_com_log_init(unsigned int);
int  bpf_com_log_debug(const char* filename, const char* funcname,unsigned int line,const char* event,unsigned int logsize,void* logdata);

/* Extern */

/* My process descriptor */
/* This is COM internal value.  Must not use this value direct. */
unsigned int bpf_com_my_proc_descriptor;

/********************************************* EOF ******************************************************************/
#endif /* _BPF_L_COM_H_ */

                                           

