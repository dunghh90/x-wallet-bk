/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_i_ru_mblm.h
 *  @brief  Definitions for BPF MBLM Internal
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_I_RU_MBLM_H_
#define _BPF_I_RU_MBLM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include <pthread.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "bpf_l_com.h"
#include "bpf_l_ru_mblm.h"

/* フラグ設定値 */
#define BPF_RU_MBLM_FLG_OFF  0
#define BPF_RU_MBLM_FLG_ON   1

/* 使用／未使用 */
#define BPF_RU_MBLM_BUFFER_USE     1
#define BPF_RU_MBLM_BUFFER_NOUSE   0

/* バッファ管理ブロック数 */
#define BPF_RU_MBLM_BUFTYPE_NUM  3

/* バッファサイズ */
#define BPF_RU_MBLM_BUFFER_NUM_128  128
#define BPF_RU_MBLM_BUFFER_SIZE_256 256
#define BPF_RU_MBLM_BUFFER_SIZE_4096 4096
#define BPF_RU_MBLM_BUFFER_SIZE_8192 8192

/* システムコールNG定義 */
#define BPF_RU_MBLM_SYSCALL_NG (-1)

/* メモリブロック先頭識別子 */
#define BPF_RU_MBLM_MBM_TOP_ID   0xCCBBAA00

/* メモリブロック末尾識別子 */
#define BPF_RU_MBLM_MBM_TAIL_ID  0xFFEEDD00

/* リスト使用中識別子 */
#define BPF_RU_MBLM_LISTUSE 0xFFFFFFFF
/* リスト最終識別子 */
#define BPF_RU_MBLM_LISTEND 0xFFFFFFFE

/* 共有メモリIPCキー */
#define BPF_RU_MBLM_MNG_IPCKEY(a)  BPF_COM_CREATE_INTERNAL_IPCKEY(BPF_COM_IPCKEY_MASK_MBLM, (a), 0)
#define BPF_RU_MBLM_MBM_IPCKEY(a)  BPF_COM_CREATE_INTERNAL_IPCKEY(BPF_COM_IPCKEY_MASK_MBLM, (a), 1)


/* 構造体定義 */
/* システムデータ情報 */
typedef struct {
    unsigned int buffer_size;        /* バッファサイズ */
    unsigned int buffer_num;         /* バッファ数     */
} t_bpf_ru_mblm_systeminfo;

/* SGデータテーブル */
typedef struct {

    /* For bpf */
    unsigned int proc_num;
    t_bpf_ru_mblm_systeminfo sysinf[BPF_COM_PNUM][BPF_RU_MBLM_BUFTYPE_NUM];

} t_bpf_ru_mblm_sg_data;

/* プロセス情報管理テーブル */
typedef struct {
    unsigned int process_desc;       /* プロセス識別子         */
    char         *mng_shmaddr;       /* 管理領域先頭アドレス   */
    unsigned int mng_shmsize;        /* 管理領域サイズ         */
    char         *shmaddr;           /* 共有メモリ先頭アドレス */
    unsigned int shmsize;            /* 共有メモリサイズ       */
} t_bpf_ru_mblm_prcinfo;

/* 空きメモリブロック管理リスト要素 */
typedef struct {
    unsigned int    next;            /* 次メモリブロック       */
} t_bpf_ru_mblm_free_blk_list_elm;

/* 空きブロック管理テーブル */
typedef struct {
    t_bpf_ru_mblm_free_blk_list_elm  *mbm_list; /* 空きメモリブロック管理リストアドレス */
    unsigned int free_num;           /* 空きメモリブロック数   */
    unsigned int head;               /* 空きメモリブロックHEAD */
    unsigned int tail;               /* 空きメモリブロックTAIL */
} t_bpf_ru_mblm_free_blk_mng;

/* バッファ管理情報 */
typedef struct {
    unsigned int    buffer_size;        /* バッファサイズ(未使用バッファタイプは本領域に0を設定すること) */
    unsigned int    mbm_size;           /* メモリブロックサイズ           */
    unsigned int    mbm_num;            /* メモリブロック個数             */
    char            *mbm_top;           /* メモリブロック領域先頭アドレス */
    char            *mbm_bottom;        /* メモリブロック領域最終アドレス */
    unsigned int    total_allocreq_num; /* 捕捉要求回数(統計用)           */
    unsigned int    total_allocng_num;  /* 捕捉失敗回数(統計用)           */
    unsigned int    minimam_nouse_num;  /* 最小空き数(統計用)             */
    unsigned int    reserve[2];         /* Reserve                        */
    pthread_mutex_t mutex;              /* mutexオブジェクト              */
    t_bpf_ru_mblm_free_blk_mng free_mng;   /* 空きブロック管理テーブル       */
} t_bpf_ru_mblm_buffer_mng;

/* メモリブロック管理情報 */
typedef struct {
    unsigned int  mbm_no;       /* 自メモリブロック番号 */
    unsigned int  buffer_type;  /* バッファタイプ       */
    unsigned int  use_flg;      /* 使用中フラグ         */
    unsigned long int  thread_id;    /* 取得スレッドID       */
    unsigned int  alloc_cnt;    /* 捕捉回数             */
} t_bpf_ru_mblm_mbm_info;

/* メモリブロック先頭情報 */
typedef struct {
    unsigned int        buftop_id;      /* メモリブロック先頭識別子 */
    t_bpf_ru_mblm_mbm_info mbm_info;    /* メモリブロック管理情報   */
} t_bpf_ru_mblm_mbm_top_info;

/* メモリブロック末尾情報 */
typedef struct {
    unsigned int        buftail_id;   /* メモリブロック末尾識別子 */
    char                reserve[4];   /* Reserve                  */
} t_bpf_ru_mblm_mbm_tail_info;



/* マクロ定義 */
/********************************************************************/
/* メモリブロックの先頭アドレスからバッファの先頭アドレスを取得する */
/*   (I)addr  メモリブロック先頭アドレス                            */
/*   戻り値   指定メモリブロック内のバッファ先頭アドレス            */
/********************************************************************/
#define MBML_BUFFER_TOP_ADDR(addr) \
       ( ((char*)(addr))+sizeof(t_bpf_ru_mblm_mbm_top_info) )

/********************************************************************/
/* メモリブロックの先頭アドレス、バッファサイズから                 */
/* メモリブロック末尾情報のアドレスを取得する                       */
/*   (I)addr     メモリブロック先頭アドレス                         */
/*   (I)bufsize  バッファのサイズ                                   */
/*   戻り値   メモリブロック末尾情報のアドレス                      */
/********************************************************************/
#define MBML_TAIL_INFO_ADDR(addr, bufsize) \
       ((t_bpf_ru_mblm_mbm_tail_info*)(((char*)(addr)) + \
        sizeof(t_bpf_ru_mblm_mbm_top_info)+(bufsize)))

/********************************************************************/
/* バッファ先頭アドレスからメモリブロックの先頭アドレスを求める     */
/*   (I)addr  バッファ先頭アドレス                                  */
/*   戻り値   メモリブロック先頭アドレス                            */
/********************************************************************/
#define MBML_BUFFER_TO_MBMTOP_ADDR(addr) \
       ( ((char*)(addr))-sizeof(t_bpf_ru_mblm_mbm_top_info) )


/********************************************************************/
/* バッファタイプ、メモリブロック番号からメモリブロックの先頭       */
/* アドレスを求める                                                 */
/*   (I)buftype     メモリブロック先頭アドレス                      */
/*   (I)mbmno       バッファのサイズ                                */
/*   戻り値   メモリブロックの先頭アドレス                          */
/********************************************************************/
#define MBLM_GET_MBM_ADDR(buftype, mbmno)  \
       ( bpf_ru_mblm_buffer_mng[(buftype)].mbm_top + \
        (bpf_ru_mblm_buffer_mng[(buftype)].mbm_size * (mbmno)) )


/********************************************************************/
/* バッファタイプ、メモリブロック番号からバッファの先頭アドレス     */
/* を求める                                                         */
/*   (I)buftype     メモリブロック先頭アドレス                      */
/*   (I)mbmno       バッファのサイズ                                */
/*   戻り値   バッファの先頭アドレス                                */
/********************************************************************/
#define MBLM_GET_BUFF_ADDR(buftype, mbmno)  \
       ( bpf_ru_mblm_buffer_mng[(buftype)].mbm_top + \
        (bpf_ru_mblm_buffer_mng[(buftype)].mbm_size * (mbmno)) + \
        sizeof(t_bpf_ru_mblm_mbm_top_info) )

/********************************************************************/
/* バッファタイプ、メモリブロック番号からメモリブロック末尾情報     */
/* のアドレスを求める                                               */
/*   (I)buftype     メモリブロック先頭アドレス                      */
/*   (I)mbmno       バッファのサイズ                                */
/*   戻り値   メモリブロック末尾情報アドレス                       */
/********************************************************************/
#define MBLM_GET_TAILINFO_ADDR(buftype, mbmno)  \
       ( bpf_ru_mblm_buffer_mng[(buftype)].mbm_top + \
        (bpf_ru_mblm_buffer_mng[(buftype)].mbm_size * (mbmno)) + \
        sizeof(t_bpf_ru_mblm_mbm_top_info) + \
         bpf_ru_mblm_buffer_mng[(buftype)].buffer_size )


/* 変数の外部参照 */
/* メモリブロック割付有無フラグ */
extern unsigned int  bpf_ru_mblm_assign_flag;
/* システムデータテーブル */
extern t_bpf_ru_mblm_systeminfo bpf_ru_mblm_system_data[BPF_RU_MBLM_BUFTYPE_NUM];
/* プロセス情報管理テーブル */
extern t_bpf_ru_mblm_prcinfo bpf_ru_mblm_prc_info;
/* バッファ管理情報 */
extern t_bpf_ru_mblm_buffer_mng *bpf_ru_mblm_buffer_mng;
/* 空きメモリブロック管理リスト */
extern t_bpf_ru_mblm_free_blk_list_elm bpf_ru_mblm_free_blk_list[BPF_RU_MBLM_BUFTYPE_NUM];
/* SGデータ */
extern t_bpf_ru_mblm_sg_data *bpf_ru_mblm_sg_data;
/* メモリ破壊検出フラグ */
extern unsigned int mblm_memory_broken_detect;

#endif /* _BPF_I_RU_MBLM_H_ */


