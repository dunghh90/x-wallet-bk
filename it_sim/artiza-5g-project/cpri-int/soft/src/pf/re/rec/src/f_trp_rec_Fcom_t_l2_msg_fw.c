/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_l2_msg_fw.c
 * @brief  Receive msg from l3/rct, send msg to l2/lpb
 * @date   2015/07/23 FPT)Yen create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"
#include "m_cm_header.h"
#include "rrhApi_Svp.h"
#include "rrhApi_L2.h"



/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Receive msg from l3/rct, send msg to l2/lpb
 * @note   Receive msg from l3/rct, send msg to l2/lpb
 * @param  aBufp [in]  バッファポインタ
 * @param  awSize [in]  バッファサイズ
 * @return None
 * @date  2015/07/23 FPT)Yen create
 */
/****************************************************************************/
INT f_trp_rec_Fcom_t_l2_msg_fw(                                  /* なし:R */
    VOID*   aBufp,                                     /* バッファポインタ:I */
    USHORT  awSize
) {

    VOID*  bufp = NULL;
    UINT   msglen = CMD_NUM2;/* メッセージ長 */
    UINT   msgid  = CMD_TSKIF_CPRISNDIND;                   /* メッセージID */
    USHORT thrino = D_TCM_THRINO_LPB;                   /* スレッド内部番号 */
    INT        sit_ret;                                       /* 終了コード */
    CHAR  errdata[CMD_NUM48];
    USHORT	cpr_no;
    UINT	keyID;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    if( awSize >= (sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF)) ) {
        msglen = awSize - sizeof(CMT_TSKIF_HEAD) - sizeof(CMT_CPRISIGSND_INF); 
    } else {
        return CMD_NG;
    }
#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "%s %s %d <msglen>%d\n", __FILE__, __FUNCTION__, __LINE__, msglen);
#endif

    cpr_no = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)aBufp)->cprircv_inf.link_num;

    /****************/
    /* バッファ取得 */
    /****************/
    sit_ret = f_trp_com_Falloc_buf(                         /* 終了コード:R */
        awSize, /* バッファサイズ:I */
        (VOID*)&bufp                                  /* バッファアドレス:O */
    );
    /* NGの場合 */
    if (sit_ret != CMD_OK) {
#ifndef D_OPT_TRIF_BUFNGLOG_INVALID
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][siz:%u][ret:0x%02x]Alloc buffer NG.", cpr_no, sizeof(*bufp), sit_ret);
        D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
#endif /* D_OPT_TRIF_BUFNGLOG_INVALID */
        return CMD_NG;
    }

    /*temp save key*/
    keyID = ((T_RRH_HEAD*)bufp)->uiKeyID;

    /****************/
    /* バッファにコピー */
    /****************/
    memcpy( bufp, aBufp, awSize );
    ((T_RRH_HEAD*)bufp)->uiKeyID = keyID;

    /******************/
    /* スレッド間送信 */
    /******************/
    sit_ret = f_trp_rec_Fcom_t_it_snd(                      /* 終了コード:R */
        (VOID*)bufp,
        msgid,                                            /* メッセージID:I */
        thrino,                                       /* スレッド内部番号:I */
        msglen                                            /* メッセージ長:I */
    );
    /* NGの場合 */
    if (sit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][ret:0x%02x]IT send NG.", cpr_no, sit_ret);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);

        /****************/
        /* バッファ解放 */
        /****************/
        f_trp_com_Ffree_buf(                                      /* なし:R */
            bufp                                      /* バッファアドレス:I */
        );

        return CMD_NG;
    }

    return CMD_OK;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
