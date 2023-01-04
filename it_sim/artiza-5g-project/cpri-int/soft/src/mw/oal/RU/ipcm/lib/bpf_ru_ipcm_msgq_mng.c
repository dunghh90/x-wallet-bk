/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	f_bpf_ru_ipcm_msgq_mng.c
 *	@brief	Inter Process Communication Management internal Library(MSGQ).
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_IPCM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "bpf_l_com.h"
#include "bpf_i_ru_ipcm_msgq.h"

/** @addtogroup BPF_RU_IPCM
 * @{ */


/* 管理データ用構造体宣言 */
struct bpf_ru_ipcm_msg_data_t {
  int qid;                       /* メッセージキュー識別子 */
  unsigned int maxmsglen;        /* 最大メッセージサイズ */
  enum ipcm_msgq_used used;      /* 使用中フラグ */
  struct bpf_ru_ipcm_msg_databuf_t rcvbuf; /* 受信用データ領域 */
  struct bpf_ru_ipcm_msg_databuf_t sndbuf; /* 送信用データ領域 */
};
/* 管理テーブル用構造体宣言 */ 
struct bpf_ru_ipcm_msgq_tbl_t {     
  enum ipcm_msgq_type type;      /* メッセージキュータイプ */
  int  max;                      /* メッセージキュー最大数 */
  int keymask;                   /* IPCキーマスク */
  int flg;                       /* メッセージキュー作成フラグ */
  struct bpf_ru_ipcm_msg_data_t *msgq;   /* 管理データ領域 */
};

/* 管理テーブル変数宣言 */
static struct bpf_ru_ipcm_msgq_tbl_t bpf_ru_icmp_msg[IPCM_MSGQ_TYPE_NUM] = {

/* RRH-001 MD alpha)matsuhashi add start */
  { /* ユーザ用メッセージキュー情報 */
    IPCM_MSGQ_USER,    /* type */
    0,                 /* max  */
    IPCM_KEYMASK_USER, /* keymask */
    IPCM_MSGFLG,       /* flg  */
    NULL               /* msgq */
  },
  { /* メッセージキュー情報(未使用) */
    IPCM_MSGQ_DUMMY,   /* type */
    0,                 /* max  */
    IPCM_KEYMASK_DUMMY,/* keymask */
    IPCM_MSGFLG,       /* flg  */
    NULL               /* msgq */
  }
/* RRH-001 MD alpha)matsuhashi add end */

};

/* Function Prototypr */
int bpf_ru_ipcm_msgq_init_tbl_set(enum ipcm_msgq_type q_type, bpf_ru_ipcm_sg_data_t *sgdata);
/* RRH-001 MD alpha)matsuhashi add start */
int bpf_ru_ipcm_msgq_uninit_tbl_set(enum ipcm_msgq_type q_type, bpf_ru_ipcm_sg_data_t *sgdata);
/* RRH-001 MD alpha)matsuhashi add end */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Message Queue Number Check.
 *  @note   IPCM internal function. 
 *  @param  q_type       [in]  queue type
 *                                   IPCM_MSGQ_USER
 *                                   IPCM_MSGQ_MDL
 *  @param  q_num        [in]  queue number
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_msgq_numcheck(enum ipcm_msgq_type q_type, 
			   unsigned int q_num)
{
    /********************************************
    * message queue number check processing.    *
    ********************************************/
    if( q_num >= bpf_ru_icmp_msg[q_type].max ){
      return BPF_RU_IPCM_NG;
    }

    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Message Size Check.
 *  @note   IPCM internal function. 
 *  @param  q_type       [in]  queue type
 *                                   IPCM_MSGQ_USER
 *                                   IPCM_MSGQ_MDL
 *  @param  q_num        [in]  queue number
 *  @param  msgsz        [in]  message size
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_msgq_msgszcheck(enum ipcm_msgq_type q_type,
			     unsigned int q_num,
			     unsigned int msgsz)
{
    /********************************************
    * message size check processing.            *
    ********************************************/
    if( msgsz > bpf_ru_icmp_msg[q_type].msgq[q_num].maxmsglen ){
      return BPF_RU_IPCM_NG;
    }

    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Message Queue Management Data Write.
 *  @note   IPCM internal function. 
 *  @param  q_type       [in]  queue type
 *                                   IPCM_MSGQ_USER
 *                                   IPCM_MSGQ_MDL
 *  @param  q_num        [in]  queue number
 *  @param  qid          [in]  queue id
 *  @param  used         [in]  used flag
 *                                   IPCM_MSGQ_UNUSED
 *                                   IPCM_MSGQ_USED
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_msgq_write_data(enum ipcm_msgq_type q_type, unsigned int q_num,
			    int qid, int used)
{
    /********************************************
    * message queue data write processing.      *
    ********************************************/
    bpf_ru_icmp_msg[q_type].msgq[q_num].qid = qid;
    bpf_ru_icmp_msg[q_type].msgq[q_num].used = used;

    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Message Queue Management Data Read.
 *  @note   IPCM internal function. 
 *  @param  q_type       [in]  queue type
 *                                   IPCM_MSGQ_USER
 *                                   IPCM_MSGQ_MDL
 *  @param  q_num        [in]  queue number
 *  @param  qid          [out] queue id
 *  @param  maxmsglen    [out] max message length
 *  @param  used         [out] used flag
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_msgq_read_data(enum ipcm_msgq_type q_type, unsigned int q_num,
			    int *qid, unsigned int *maxmsglen, int *used)
{
    /********************************************
    * message queue data read processing.       *
    ********************************************/
    *qid = bpf_ru_icmp_msg[q_type].msgq[q_num].qid;
    *maxmsglen = bpf_ru_icmp_msg[q_type].msgq[q_num].maxmsglen;
    *used = bpf_ru_icmp_msg[q_type].msgq[q_num].used;

    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Message Queue Management Data Read.
 *  @note   IPCM internal function. 
 *  @param  q_type       [in]  queue type
 *                                   IPCM_MSGQ_USER
 *                                   IPCM_MSGQ_MDL
 *  @param  q_num        [in]  queue number
 *  @param  rcvbuf       [out] receive buffer
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_msgq_read_rdatabuf(enum ipcm_msgq_type q_type, unsigned int q_num,
			    struct bpf_ru_ipcm_msg_databuf_t **rcvbuf)
{
    /*********************************************
    * message queue data buffer read processing. *
    *********************************************/
    *rcvbuf = &bpf_ru_icmp_msg[q_type].msgq[q_num].rcvbuf;

    return BPF_RU_IPCM_OK;
}
/* @} */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Message Queue Management Data Read.
 *  @note   IPCM internal function. 
 *  @param  q_type       [in]  queue type
 *                                   IPCM_MSGQ_USER
 *                                   IPCM_MSGQ_MDL
 *  @param  q_num        [in]  queue number
 *  @param  sndbuf       [out] send buffer
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_msgq_read_sdatabuf(enum ipcm_msgq_type q_type, unsigned int q_num,
			    struct bpf_ru_ipcm_msg_databuf_t **sndbuf)
{
    /*********************************************
    * message queue data buffer read processing. *
    *********************************************/
    *sndbuf = &bpf_ru_icmp_msg[q_type].msgq[q_num].sndbuf;

    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Message Queue Table Read.
 *  @note   IPCM internal function. 
 *  @param  msg_type_num [in]  message queue type number
 *                                 IPCM_MSGQ_MDL
 *                                 IPCM_MSGQ_USER
 *  @param  type         [out] message queue type
 *  @param  max          [out] max number of queue
 *  @param  keymask      [out] IPC key mask
 *  @param  flg          [out] message queue create flag
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_msgq_read_tbl(int msgq_type_num, enum ipcm_msgq_type *type, 
			   int *max, int *keymask, int *flg)
{
    /********************************************
    * message queue table read processing.      *
    ********************************************/
    *type = bpf_ru_icmp_msg[msgq_type_num].type;
    *max = bpf_ru_icmp_msg[msgq_type_num].max;
    *keymask = bpf_ru_icmp_msg[msgq_type_num].keymask;
    *flg = bpf_ru_icmp_msg[msgq_type_num].flg;

    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Message Queue Function Initialize.
 *  @note   IPCM internal function. 
 *  @param  process_desc [in] process descriptor
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_msgq_init(unsigned int process_desc)
{
    unsigned int q_num;
    int errcd = 0, ret;
    struct{
        int    sg_num;
        bpf_ru_ipcm_sg_data_t *sgdata[3];
    } sg_proc_data;
    enum ipcm_msgq_type q_type;
    struct bpf_ru_ipcm_msg_data_t *msgdata_tmp;

    int cnt;
/* RRH-001 MD alpha)matsuhashi add start */
    bpf_ru_ipcm_sg_data_t sgdata;
/* RRH-001 MD alpha)matsuhashi add end */


    sg_proc_data.sg_num = BPF_COM_NUM0;

    /************************************************
    * 初期化Queue Type の決定                       *
    ************************************************/
    (void)bpf_com_get_proc_tkind(process_desc);

/* RRH-001 MD alpha)matsuhashi add start */
    q_type = IPCM_MSGQ_USER;
    
    sg_proc_data.sg_num++;

    sg_proc_data.sgdata[BPF_COM_NUM0] = &sgdata;

    /* プロセス間メッセージQueue数の設定*/
    sg_proc_data.sgdata[BPF_COM_NUM0]->que_max = IPCM_QUEUE_NUM_MAX;
    sg_proc_data.sgdata[BPF_COM_NUM0]->que_top_no = 0;

    for(cnt=0;cnt<sg_proc_data.sgdata[BPF_COM_NUM0]->que_max;cnt++)
    {
        /* プロセス間メッセージQueue長の設定 */
        sg_proc_data.sgdata[BPF_COM_NUM0]->que_size[cnt] = IPCM_QUEUE_MSGSIZE;
    }

    bpf_ru_icmp_msg[q_type].max = sg_proc_data.sgdata[BPF_COM_NUM0]->que_max;

/* RRH-001 MD alpha)matsuhashi add end */

    /* Trace log */
    IPCM_DBGLOG_FMT_TRC(
        "MSGQ init. type=%d max=%d", 
        q_type, bpf_ru_icmp_msg[q_type].max);

    /* キュー数が0の場合は ここで終了 */
    if( bpf_ru_icmp_msg[q_type].max == 0){
        IPCM_DBGLOG_FMT_ERR(
          "Queue Num is 0. pdesc=%d q_type=%d", process_desc, q_type);
		return BPF_RU_IPCM_OK;
	}

    /********************************************
    * message queue table initialize processing.*
    ********************************************/
    /* 初期化 */

    /* データ用領域確保 */
    bpf_ru_icmp_msg[q_type].msgq = malloc(sizeof(struct bpf_ru_ipcm_msg_data_t) *
               bpf_ru_icmp_msg[q_type].max);
    errcd = errno;
    if( bpf_ru_icmp_msg[q_type].msgq == NULL ){
        /* メモリ確保失敗 */
        IPCM_DBGLOG_FMT_ERR(
          "malloc NG. errno = %d", errcd);
        return BPF_RU_IPCM_NG;
    }

    /* データ用領域を一旦サイズ0でクリア */
    for(q_num = 0; q_num < bpf_ru_icmp_msg[q_type].max; q_num++ ){

		msgdata_tmp = &bpf_ru_icmp_msg[q_type].msgq[q_num];
        msgdata_tmp->qid = 0;
        msgdata_tmp->maxmsglen = 0;
        msgdata_tmp->used = IPCM_MSGQ_UNUSED;
    }

    /* データ領域設定 */
    for(cnt=0; cnt<sg_proc_data.sg_num; cnt++){
        if(sg_proc_data.sgdata[cnt]->que_max != 0){
            ret = bpf_ru_ipcm_msgq_init_tbl_set(q_type, sg_proc_data.sgdata[cnt]);
            if( ret != BPF_RU_IPCM_OK ){
                return BPF_RU_IPCM_NG;
            }
        }
    }

    /********************************************
    * message queue create processing.          *
    ********************************************/
    /* メッセージキューを作る */
    for( q_num = 0; q_num < bpf_ru_icmp_msg[q_type].max; q_num++ ){

        if(bpf_ru_icmp_msg[q_type].msgq[q_num].maxmsglen != 0){
        /* 最大メッセージ長が0以外の場合のみ作成 */

            ret = bpf_ru_ipcm_procmsgq_create(q_type, q_num, &errcd);
            if( ret != BPF_RU_IPCM_OK ){
                /* NG処理 */
                IPCM_DBGLOG_FMT_ERR(
                  "bpf_ru_ipcm_procmsgq_create NG. ret = %d, type = %d, num = %d", 
                   ret, q_type, q_num);
                /* 上位にてエラー処理するので作成済みのメッセージキューを削除はしない*/
                return BPF_RU_IPCM_NG;
            }
        }
    }
    return BPF_RU_IPCM_OK;
}
/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Message Queue Table set Initialize.
 *  @note   IPCM internal table setting
 *  @param  sgdata [in] sg data table pointer
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_msgq_init_tbl_set(enum ipcm_msgq_type q_type, bpf_ru_ipcm_sg_data_t *sgdata)
{
    unsigned int q_num;
    int cnt;
    int errcd = 0;
    struct bpf_ru_ipcm_msg_data_t *msgdata_tmp;
    int bufsize=0;  /* For log */

    /* データ用領域設定 */

    for(cnt = 0, q_num = sgdata->que_top_no; cnt < sgdata->que_max; cnt++, q_num++ ){

		msgdata_tmp = &bpf_ru_icmp_msg[q_type].msgq[q_num];
        msgdata_tmp->qid = 0;

        /* SGよりメッセージ辺りの最大サイズ設定 */
        msgdata_tmp->maxmsglen = sgdata->que_size[cnt];
        msgdata_tmp->used = IPCM_MSGQ_UNUSED;
        bufsize += msgdata_tmp->maxmsglen;/*For log*/

        if(msgdata_tmp->maxmsglen == 0){
            /* 最大メッセージ長が0の場合以下の設定は実施しない */
            continue;
        }

        /* 受信用データ領域設定 */
        msgdata_tmp->rcvbuf.use_flg = BPF_RU_IPCM_FLG_OFF;
        pthread_mutex_init( &(msgdata_tmp->rcvbuf.mutex), NULL);
        /* 最大メッセージ長+1を確保している理由はサイズオーバーをチェックすため */
        msgdata_tmp->rcvbuf.data_buf = 
            malloc(sizeof(struct ipcm_msgbuf) + msgdata_tmp->maxmsglen + 1);
        if( msgdata_tmp->rcvbuf.data_buf == NULL ){
	        /* メモリ確保失敗 */
	        IPCM_DBGLOG_FMT_ERR( "malloc NG. errno = %d", errcd);
	        return BPF_RU_IPCM_NG;
	    }

        /* 送信用データ領域設定 */
        msgdata_tmp->sndbuf.use_flg = BPF_RU_IPCM_FLG_OFF;
        pthread_mutex_init( &(msgdata_tmp->sndbuf.mutex), NULL);
        msgdata_tmp->sndbuf.data_buf = 
            malloc(sizeof(struct ipcm_msgbuf) + msgdata_tmp->maxmsglen);
        if( msgdata_tmp->sndbuf.data_buf == NULL ){

            free(msgdata_tmp->rcvbuf.data_buf);
            msgdata_tmp->rcvbuf.data_buf = NULL;

	        /* メモリ確保失敗 */
	        IPCM_DBGLOG_FMT_ERR( "malloc NG. errno = %d", errcd);
	        return BPF_RU_IPCM_NG;
	    }
    }

    /* Trace log */
    IPCM_DBGLOG_FMT_TRC(
        "Total max message size. q_type=%d rcv/snd_buf=%d*2", 
        q_type, bufsize);

    return BPF_RU_IPCM_OK;
}
/* @} */

/* RRH-001 MD alpha)matsuhashi add start */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Message Queue Function UnInitialize.
 *  @note   IPCM internal function. 
 *  @param  process_desc [in] process descriptor
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_msgq_uninit(unsigned int process_desc)
{
    int ret;
    struct{
        int    sg_num;
        bpf_ru_ipcm_sg_data_t *sgdata[3];
    } sg_proc_data;
    enum ipcm_msgq_type q_type;
    int cnt;
    bpf_ru_ipcm_sg_data_t sgdata;


    sg_proc_data.sg_num = BPF_COM_NUM0;
    q_type = IPCM_MSGQ_USER;
    
    sg_proc_data.sg_num++;

    sg_proc_data.sgdata[BPF_COM_NUM0] = &sgdata;

    /* プロセス間メッセージQueue数の設定*/
    sg_proc_data.sgdata[BPF_COM_NUM0]->que_max = IPCM_QUEUE_NUM_MAX;
    sg_proc_data.sgdata[BPF_COM_NUM0]->que_top_no = 0;

    /* データ領域設定 */
    for(cnt=0; cnt<sg_proc_data.sg_num; cnt++){
        if(sg_proc_data.sgdata[cnt]->que_max != 0){
            ret = bpf_ru_ipcm_msgq_uninit_tbl_set(q_type, sg_proc_data.sgdata[cnt]);
            if( ret != BPF_RU_IPCM_OK ){
                return BPF_RU_IPCM_NG;
            }
        }
    }

    /* 捕捉したバッファを解放する */
    if (bpf_ru_icmp_msg[q_type].msgq != NULL)
    {
        free(bpf_ru_icmp_msg[q_type].msgq);
        bpf_ru_icmp_msg[q_type].msgq = NULL;
    }

    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Message Queue Table set UnInitialize.
 *  @note   IPCM internal table unsetting
 *  @param  sgdata [in] sg data table pointer
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_NG        abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_msgq_uninit_tbl_set(enum ipcm_msgq_type q_type, bpf_ru_ipcm_sg_data_t *sgdata)
{
    unsigned int q_num;
    int cnt;
    struct bpf_ru_ipcm_msg_data_t *msgdata_tmp;

    /* データ用領域設定 */

    for(cnt = 0, q_num = sgdata->que_top_no; cnt < sgdata->que_max; cnt++, q_num++ ){

        msgdata_tmp = &bpf_ru_icmp_msg[q_type].msgq[q_num];

        if (msgdata_tmp->rcvbuf.data_buf != NULL)
        {
            free(msgdata_tmp->rcvbuf.data_buf);
            msgdata_tmp->rcvbuf.data_buf = NULL;
        }
        if (msgdata_tmp->sndbuf.data_buf != NULL)
        {
            free(msgdata_tmp->sndbuf.data_buf); 
            msgdata_tmp->sndbuf.data_buf = NULL;
        }
    }

    return BPF_RU_IPCM_OK;
}
/* @} */
/* RRH-001 MD alpha)matsuhashi add end */

/* @} */

