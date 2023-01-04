/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_it_mtsetfreq_snd.c
 * @brief  スレッド間(MTアドレス設定(FLD-MT)要求)送信
 * @date   2009/01/19 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  スレッド間(MTアドレス設定(FLD-MT)要求)送信
 * @note   スレッド間(MTアドレス設定(FLD-MT)要求)を送信する
 *         - バッファ設定(データ)
 *         - スレッド間送信
 * @param  -
 * @return None
 * @date   2015/07/30 FPT)Hieu create.
 * @date   2015/09/19 TDI)satou 共有メモリから設定値を取得して設定するように
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_it_mtsetfreq_snd(                           /* なし:R */
){
    CMT_TSKIF_CPRISND_MTADDSETREQ *bufp;
    UINT   msglen = sizeof(CMT_CPRIF_MTADDSETREQ);//CMD_NUM4;                               /* メッセージ長 */
    UINT   msgid  = CMD_TSKIF_CPRISNDIND;                   /* メッセージID */
    USHORT thrino = D_TCM_THRINO_LPB;                   /* スレッド内部番号 */
    INT    sit_ret;                                           /* 終了コード */
    CHAR  errdata[CMD_NUM48];
    USHORT cpr_no =  D_REC_C02_CPR_NO(); 
    T_RRH_MTADDSET shm_mtaddr_set;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    /* 共有メモリから設定値を取得(RECから受信したデータ。未受信の場合は初期値 */
    if (D_RRH_OK != f_cmn_com_mtaddset_get(CMD_SYS_S3G, CMD_FLD_MT, &shm_mtaddr_set)) {
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d]f_cmn_com_mtaddset_get NG.", cpr_no);
        D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
        return;
    }

    /****************/
    /* バッファ取得 */
    /****************/
    sit_ret = f_trp_com_Falloc_buf(                         /* 終了コード:R */
        sizeof(*bufp),                                  /* バッファサイズ:I */
        (VOID*)&bufp                                  /* バッファアドレス:O */
    );
    /* NGの場合 */
    if (sit_ret != CMD_OK) {
#ifndef D_OPT_TRIF_BUFNGLOG_INVALID
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][siz:0x%08x][ret:0x%02x]Alloc buffer NG.", cpr_no, sizeof(*bufp), sit_ret); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
#endif /* D_OPT_TRIF_BUFNGLOG_INVALID */
        return ;
    }

    /****************************/
    /* バッファ設定(CPRIヘッダ) */
    /****************************/
    bufp->cprisnd_inf.link_num            = cpr_no;                                 /* CPRI番号                 */
    bufp->cprisnd_inf.length              = sizeof( CMT_CPRIF_MTADDSETREQ );        /* buffer length            */
    bufp->cpridat_mtaddsetreq.signal_kind = CMD_CPRID_MTADDSETREQ + CMD_SYS_LTE;    /* 信号種別                 */
    bufp->cpridat_mtaddsetreq.tgtmt       = CMD_FLD_MT;                             /* 対象MT(FLD-MT)           */
    bufp->cpridat_mtaddsetreq.ipadd_1     = shm_mtaddr_set.ipadd_1;                 /* IPアドレス#1             */
    bufp->cpridat_mtaddsetreq.ipadd_2     = shm_mtaddr_set.ipadd_2;                 /* IPアドレス#2             */
    bufp->cpridat_mtaddsetreq.ipadd_3     = shm_mtaddr_set.ipadd_3;                 /* IPアドレス#3             */
    bufp->cpridat_mtaddsetreq.ipadd_4     = shm_mtaddr_set.ipadd_4;                 /* IPアドレス#4             */
    bufp->cpridat_mtaddsetreq.sbnetmsk_1  = shm_mtaddr_set.sbnetmsk_1;              /* サブネットマスク#1       */
    bufp->cpridat_mtaddsetreq.sbnetmsk_2  = shm_mtaddr_set.sbnetmsk_2;              /* サブネットマスク#2       */
    bufp->cpridat_mtaddsetreq.sbnetmsk_3  = shm_mtaddr_set.sbnetmsk_3;              /* サブネットマスク#3       */
    bufp->cpridat_mtaddsetreq.sbnetmsk_4  = shm_mtaddr_set.sbnetmsk_4;              /* サブネットマスク#4       */
    bufp->cpridat_mtaddsetreq.cmdtrsport  = shm_mtaddr_set.cmdtrsport;              /* コマンド転送用ポート番号 */
    bufp->cpridat_mtaddsetreq.filtrsport  = shm_mtaddr_set.filtrsport;              /* ファイル転送用ポート番号 */
    bufp->cpridat_mtaddsetreq.portno      = shm_mtaddr_set.portno;                  /* ポート番号               */

    /******************/
    /* スレッド間送信 */
    /******************/
    sit_ret = f_trp_rec_Fcom_t_it_snd(                      /* 終了コード:R */
        //sndbf_p,                                      /* バッファポインタ:I */
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

        return ;
    }

    return ;
}
/****************************************************************************/
/*!
 * @brief  スレッド間(MTアドレス設定(FLD-MT)要求)送信
 * @note   スレッド間(MTアドレス設定(FLD-MT)要求)を送信する
 *         - バッファ設定(データ)
 *         - スレッド間送信
 * @param  -
 * @return None
 * @date   2015/07/30 FPT)Hieu create.
 * @date   2015/09/19 TDI)satou 共有メモリから設定値を取得して設定するように
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_it_mtsetsreq_snd(                           /* なし:R */
){
    CMT_TSKIF_CPRISND_MTADDSETREQ *bufp;
    UINT   msglen = sizeof(CMT_CPRIF_MTADDSETREQ);//CMD_NUM4;                               /* メッセージ長 */
    UINT   msgid  = CMD_TSKIF_CPRISNDIND;                   /* メッセージID */
    USHORT thrino = D_TCM_THRINO_LPB;                   /* スレッド内部番号 */
    INT    sit_ret;                                           /* 終了コード */
    CHAR  errdata[CMD_NUM48];
    USHORT cpr_no =  D_REC_C02_CPR_NO(); 
    T_RRH_MTADDSET shm_mtaddr_set;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    /* 共有メモリから設定値を取得(RECから受信したデータ。未受信の場合は初期値 */
    if (D_RRH_OK != f_cmn_com_mtaddset_get(CMD_SYS_S3G, CMD_SV_MT, &shm_mtaddr_set)) {
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d]f_cmn_com_mtaddset_get NG.", cpr_no);
        D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
        return;
    }

    /****************/
    /* バッファ取得 */
    /****************/
    sit_ret = f_trp_com_Falloc_buf(                         /* 終了コード:R */
        sizeof(*bufp),                                  /* バッファサイズ:I */
        (VOID*)&bufp                                  /* バッファアドレス:O */
    );
    /* NGの場合 */
    if (sit_ret != CMD_OK) {
#ifndef D_OPT_TRIF_BUFNGLOG_INVALID
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][siz:0x%08x][ret:0x%02x]Alloc buffer NG.", cpr_no, sizeof(*bufp), sit_ret); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
#endif /* D_OPT_TRIF_BUFNGLOG_INVALID */
        return ;
    }

    /****************************/
    /* バッファ設定(CPRIヘッダ) */
    /****************************/
    bufp->cprisnd_inf.link_num            = cpr_no;                                 /* CPRI番号                 */
    bufp->cprisnd_inf.length              = sizeof( CMT_CPRIF_MTADDSETREQ );        /* buffer length            */
    bufp->cpridat_mtaddsetreq.signal_kind = CMD_CPRID_MTADDSETREQ + CMD_SYS_LTE;    /* 信号種別                 */
    bufp->cpridat_mtaddsetreq.tgtmt       = CMD_SV_MT;                              /* 対象MT(SV-MT)            */
    bufp->cpridat_mtaddsetreq.ipadd_1     = shm_mtaddr_set.ipadd_1;                 /* IPアドレス#1             */
    bufp->cpridat_mtaddsetreq.ipadd_2     = shm_mtaddr_set.ipadd_2;                 /* IPアドレス#2             */
    bufp->cpridat_mtaddsetreq.ipadd_3     = shm_mtaddr_set.ipadd_3;                 /* IPアドレス#3             */
    bufp->cpridat_mtaddsetreq.ipadd_4     = shm_mtaddr_set.ipadd_4;                 /* IPアドレス#4             */
    bufp->cpridat_mtaddsetreq.sbnetmsk_1  = shm_mtaddr_set.sbnetmsk_1;              /* サブネットマスク#1       */
    bufp->cpridat_mtaddsetreq.sbnetmsk_2  = shm_mtaddr_set.sbnetmsk_2;              /* サブネットマスク#2       */
    bufp->cpridat_mtaddsetreq.sbnetmsk_3  = shm_mtaddr_set.sbnetmsk_3;              /* サブネットマスク#3       */
    bufp->cpridat_mtaddsetreq.sbnetmsk_4  = shm_mtaddr_set.sbnetmsk_4;              /* サブネットマスク#4       */
    bufp->cpridat_mtaddsetreq.cmdtrsport  = shm_mtaddr_set.cmdtrsport;              /* コマンド転送用ポート番号 */
    bufp->cpridat_mtaddsetreq.filtrsport  = shm_mtaddr_set.filtrsport;              /* ファイル転送用ポート番号 */
    bufp->cpridat_mtaddsetreq.portno      = shm_mtaddr_set.portno;                  /* ポート番号               */

    /******************/
    /* スレッド間送信 */
    /******************/
    sit_ret = f_trp_rec_Fcom_t_it_snd(                      /* 終了コード:R */
        //sndbf_p,                                      /* バッファポインタ:I */
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

        return ;
    }

    return ;
}
/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
