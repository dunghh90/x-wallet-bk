/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_rom.c
 * @brief  ROMテーブル
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2007/06/07 FJT)Nagasima CR-00019-001
 * @date   2008/07/21 FFCS)Shihzh modify for S3G PRC
 * @date   2009/01/17 FFCS)Shihzh modify for BI-TBD-557
 * @date   2009/01/19 FFCS)Shihzh modify for eNB-004-003 BD-TBD-11
 * @date   2009/10/16 FFCS)Niemsh M-S3G-eNBPF-02674
 * @date   2010/10/26 FJT)Koshida modify for eNB-004 of TS656案5対応
 * @date   2011/12/11 FJT)K.Koshida  DMICRO-008
 * @date   2021/02/04 M&C)Huan.dh add CARONOFF handle
 * @date   2021/01/18 M&C)Tri.hn Add CARLSTS event
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007-2011
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

#if 0
VOID (*const f_trp_rec_Rc01_pre_m_fnc[EC_REC_C01_EVT_MAX][EC_REC_C01_STN_MAX])(VOID*) = {
    /* (00)スレッド初期化要求                                                         */
    {
        f_trp_rec_Fc01_pre_m_ini_init_exe,          /* 初期                       */
        f_trp_rec_Fc01_pre_m_err,                   /* 運用中                     */
        f_trp_rec_Fc01_pre_m_err                    /* N/E切替中                  */
    },
    /* (01)全スレッド初期化完了通知                                                   */
    {
        f_trp_rec_Fc01_pre_m_err,                   /* 初期                       */
        f_trp_rec_Fc01_pre_m_nop,                   /* 運用中                     */
        f_trp_rec_Fc01_pre_m_nop                    /* N/E切替中                  */
    },
    /* (02)全プロセス初期化完了通知                                                   */
    {
        f_trp_rec_Fc01_pre_m_err,                   /* 初期                       */
        f_trp_rec_Fc01_pre_m_nop,                   /* 運用中                     */
        f_trp_rec_Fc01_pre_m_nop                    /* N/E切替中                  */
    },
    /* (03)AP起動条件完了通知                                                         */
    {
        f_trp_rec_Fc01_pre_m_err,                   /* 初期                       */
        f_trp_rec_Fc01_pre_m_aps_cmp,               /* 運用中                     */
        f_trp_rec_Fc01_pre_m_err                    /* N/E切替中                  */
    },
    /* (04)N/E切替開始要求                                                            */
    {
        f_trp_rec_Fc01_pre_m_err,                   /* 初期                       */
        f_trp_rec_Fc01_pre_m_nes_sta,               /* 運用中                     */
        f_trp_rec_Fc01_pre_m_err                    /* N/E切替中                  */
    },
    /* (05)N/E切替完了通知                                                            */
    {
        f_trp_rec_Fc01_pre_m_err,                   /* 初期                       */
        f_trp_rec_Fc01_pre_m_err,                   /* 運用中                     */
        f_trp_rec_Fc01_pre_m_nes_end                /* N/E切替中                  */
    },
    /* (06)N/E間データ引継ぎ開始通知                                                  */
    {
        f_trp_rec_Fc01_pre_m_err,                   /* 初期                       */
        f_trp_rec_Fc01_pre_m_suc_sta,               /* 運用中                     */
        f_trp_rec_Fc01_pre_m_nop                    /* N/E切替中                  */
    },
    /* (07)N/E間データ引継ぎ完了通知                                                  */
    {
        f_trp_rec_Fc01_pre_m_err,                   /* 初期                       */
        f_trp_rec_Fc01_pre_m_err,                   /* 運用中                     */
        f_trp_rec_Fc01_pre_m_suc_end                /* N/E切替中                  */
    },
    /* (08)自律信号停止完了通知                                                       */
    {
        f_trp_rec_Fc01_pre_m_err,                   /* 初期                       */
        f_trp_rec_Fc01_pre_m_err,                   /* 運用中                     */
        f_trp_rec_Fc01_pre_m_sigstp_end             /* N/E切替中                  */
    }
};

VOID (*const f_trp_rec_Rc01_pst_m_fnc[EC_REC_C01_EVT_MAX][EC_REC_C01_STN_MAX])(VOID*) = {
    /* (00)スレッド初期化要求                                                         */
    {
        f_trp_rec_Fc01_pst_m_ini_init_exe,          /* 初期                       */
        f_trp_rec_Fc01_pst_m_err,                   /* 運用中                     */
        f_trp_rec_Fc01_pst_m_err                    /* N/E切替中                  */
    },
    /* (01)全スレッド初期化完了通知                                                   */
    {
        f_trp_rec_Fc01_pst_m_err,                   /* 初期                       */
        f_trp_rec_Fc01_pst_m_nop,                   /* 運用中                     */
        f_trp_rec_Fc01_pst_m_nop                    /* N/E切替中                  */
    },
    /* (02)全プロセス初期化完了通知                                                   */
    {
        f_trp_rec_Fc01_pst_m_nop,                   /* 初期                       */
        f_trp_rec_Fc01_pst_m_nop,                   /* 運用中                     */
        f_trp_rec_Fc01_pst_m_nop                    /* N/E切替中                  */
    }
};
#endif

/**
 * @brief   Rc02 matrix table (S3G)
 * @Bug_No  M-RRU-ZSYS-01793
 * @Bug_No  M-RRU-ZSYS-01986
 * @date    2015/10/22 TDIPS)ikeda M-RRU-ZSYS-01793 
 * @date    2015/11/14 TDIPS)sasaki M-RRU-ZSYS-01986 ヘルスチェックリトライオーバーで強制停止にならない不具合修正
 */
VOID (*const f_trp_rec_Rc02_m_fnc[EC_REC_C02_EVT_MAX][EC_REC_C02_STN_MAX])(VOID*) = {
    /* (00)スレッド初期化要求                                             */
    {
        f_trp_rec_Fc02_m_ini_init_exe,              /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (01)全スレッド初期化完了通知                                       */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (02)全プロセス初期化完了通知                                       */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (03)CPRIリンク状態変化通知(StateB)                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_rst_rerst_cmp,             /* REリセット中                             */
        f_trp_rec_Fc02_m_rst_refrcrst_cmp,          /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (04)CPRIリンク状態変化通知(StateE)                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_2nd_rdy,                   /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (05)CPRIリンク状態変化通知(StateF)                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_2nd_rdy,                   /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_res_rdy,                   /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                  /* N/E切替中                                */
    },
    /* (06)CPRIリンク状態変化通知(L3確立)                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_res_sta,                   /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_idl_restsuse_exe,          /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                 /* N/E切替中                                */
    },
    /* (07)CPRIリンク断通知(StateF未満)                                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_dsc_useini_exe,            /* 運用中                                   */
        f_trp_rec_Fc02_m_dsc_resini_exe,            /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop              /* N/E切替中                                */
    },
    /* (08)CPRIリンク断通知(StateF)                                       */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_dsc_resrst_exe,            /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_dsc_useidl_exe,            /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop              /* N/E切替中                                */
    },
    /* (09)割込(T14値変化)通知                                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,            /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,            /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (10)Active設定応答                                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop             /* N/E切替中                                */
    },
    /* (11)Active切替結果通知(OK)                                         */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,             /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,             /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,             /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,             /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,             /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                /* N/E切替中                                */
    },
    /* (12)Active切替結果通知(NG)                                         */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                   /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                   /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                   /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                   /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                   /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                   /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (13)Negative通知(Negative)                                         */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (14)Negative通知(Negative TO)                                      */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,         /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,         /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,         /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,         /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,         /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,         /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,         /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop          /* N/E切替中                                */
    },
    /* (15)二次リセット要求                                               */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                   	/* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_2nd_sta,                   /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (16)REシステムパラメータ更新応答                                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_res_reprm_end,             /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (17)REファイル更新応答                                             */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_res_refil_end,             /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (18)MTアドレス設定(FLD-MT/SV-MT)応答                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_res_mtsetf_end,            /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_res_mtsets_end,            /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop             /* N/E切替中                                */
    },
    /* (19)MTアドレス設定(SV-MT)応答                                      */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,            			/* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop             /* N/E切替中                                */
    },
    /* (20)二次リセットプロシージャ実行指示                               */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (21)RE時刻設定プロシージャ実行指示                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_res_retimset_exe,          /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (22)REシステムパラメータ更新プロシージャ実行指示                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_res_reprm_sta,             /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (23)REファイル更新プロシージャ実行指示                             */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_res_refil_sta,             /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (24)Toffset値報告プロシージャ実行指示                              */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_res_tofval_sta,            /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (25)Active設定プロシージャ実行指示                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (26)RE装置構成情報報告プロシージャ実行指示                         */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_res_reeqpinf_sta,          /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (27)REスロット情報報告プロシージャ実行指示                         */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_res_resltinf_sta,          /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (28)RE状態報告確プロシージャ実行指示                               */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_res_rests_sta,             /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (29)RE PORT状態報告プロシージャ実行指示                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_res_reprtsts_sta,          /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (30)REカード状態報告(セクタ部)プロシージャ実行指示                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_res_recrdssc_sta,          /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (31)REカード状態報告(共通部)プロシージャ実行指示                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (32)REキャリア状態報告プロシージャ実行指示                         */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_res_recrrsts_sta,          /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (33)MTアドレス設定(FLD-MT)プロシージャ実行指示                     */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_res_mtsetf_sta,            /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (34)MTアドレス設定(SV-MT)プロシージャ実行指示                      */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_res_mtsets_sta,            /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (35)RE起動完了プロシージャ実行指示                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_res_cmp,                   /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (36)REリセットプロシージャ実行指示                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_rst_rerst_sta,             /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_rst_rerst_sta,             /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (37)RE強制リセット設置プロシージャ実行指示                         */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (38)RE強制リセットプロシージャ実行指示                             */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_res_refrcrst_sta,          /* REリセット中                             */
        f_trp_rec_Fc02_m_rst_refrcrst_sta,          /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (39)強制停止プロシージャ実行指示                                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* 運用中                                   */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* REリセット中                             */
        f_trp_rec_Fc02_m_rst_frcstp_exe,            /* RE強制リセット中                         */
		f_trp_rec_Fc02_m_rst_frcstp_exe,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (40)CPRI信号受信通知(Toffset値報告応答)                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_res_tofval_end,            /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (41)CPRI信号受信通知(RE装置構成情報報告応答)                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_res_reeqpinf_end,          /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        //f_trp_rec_Fc02_m_use_reeqpinf_end,          /* 運用中                                   */
    	f_trp_rec_Fc02_m_err,          /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (42)CPRI信号受信通知(REスロット情報報告)                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_res_resltinf_end,          /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (43)CPRI信号受信通知(RE状態報告応答)                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_res_rests_end,             /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_rests_end,             /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (44)CPRI信号受信通知(RE PORT状態報告応答)                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_res_reprtsts_end,          /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
		f_trp_rec_Fc02_m_use_reprtsts_end,          /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (45)CPRI信号受信通知(REカード状態報告(セクタ部)応答)               */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_res_recrdssc_end,          /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_recrdssc_end,          /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (46)CPRI信号受信通知(REカード状態報告(共通部)応答)                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (47)CPRI信号受信通知(REキャリア状態報告応答)                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_res_recrrsts_end,          /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_recrrsts_end,          /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (48)タイムアウト発生通知(Toffset値報告確認タイマ)                  */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_res_tofval_stp,            /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (49)タイムアウト発生通知(RE装置構成情報報告確認タイマ)                  */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_res_reeqpinf_stp,          /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,          				/* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (50)タイムアウト発生通知(REスロット情報報告確認タイマ)                  */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_res_resltinf_stp,          /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,          				/* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (51)タイムアウト発生通知(RE状態報告確認タイマ)                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_res_rests_stp,             /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_rests_stp,             /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (52)タイムアウト発生通知(RE PORT状態報告確認タイマ)                  */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_res_reprtsts_stp,          /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
		f_trp_rec_Fc02_m_use_reprtsts_stp,          /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (53)タイムアウト発生通知(REカード状態報告(セクタ部)確認タイマ)     */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_res_recrdssc_stp,          /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_recrdssc_stp,          /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (54)タイムアウト発生通知(REカード状態報告(共通部)確認タイマ)       */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (55)タイムアウト発生通知(REキャリア状態報告確認タイマ)       */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_res_recrrsts_stp,          /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_recrrsts_stp,          /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (56)タイムアウト発生通知(REリセット確認タイマ)                     */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_rst_rerst_stp,             /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (57)タイムアウト発生通知(RE強制リセット確認タイマ)                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_rst_refrcrst_stp,          /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (58)タイムアウト発生通知(RE時刻設定送信間隔タイマ)                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,          /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (59)タイムアウト発生通知(Toffset値報告送信間隔タイマ)              */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,            /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (60)タイムアウト発生通知(RE装置構成情報報告送信間隔タイマ)              */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        //f_trp_rec_Fc02_m_use_reeqpinf_sta,          /* 運用中                                   */
    	f_trp_rec_Fc02_m_err,          /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (61)タイムアウト発生通知(RE状態報告送信間隔タイマ)              */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_rests_sta,             /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (62)タイムアウト発生通知(RE PORT状態報告送信間隔タイマ)              */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
		f_trp_rec_Fc02_m_use_reprtsts_sta,         	/* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (63)タイムアウト発生通知(REカード状態報告(セクタ部)送信間隔タイマ) */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_recrdssc_sta,          /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (64)タイムアウト発生通知(REカード状態報告(共通部)送信間隔タイマ)   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (65)タイムアウト発生通知(REキャリア状態報告送信間隔タイマ)             */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_recrrsts_sta,          /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (66)タイムアウト発生通知(二次リセット待ちタイマ)                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                   	/* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_2nd_exe,                   /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (67)タイムアウト発生通知(Active切替指示送信待ちタイマ)             */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                   /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                   /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                   /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                   /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                   /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                   /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (68)タイムアウト発生通知(遅延値設定待ちタイマ)                          */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,            /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,            /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (69)タイムアウト発生通知(REauto Reset 待ちタイマ)           */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_reautrst_exe,              /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_reautrst_exe,              /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_reautrst_exe,              /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_reautrst_exe,              /* 運用中                                   */
        f_trp_rec_Fc02_m_reautrst_exe,              /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_reautrst_exe,              /* REリセット中                             */
        f_trp_rec_Fc02_m_reautrst_exe,              /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_reautrst_exe,              /* 強制停止                                 */
        f_trp_rec_Fc02_m_reautrst_exe               /* N/E切替中                                */
    },
    /* (70)N/E切替開始要求                                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                   /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                   /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                   /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                   /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                   /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                   /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (71)N/E切替完了通知                                                */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                    /* N/E切替中                                */
    },
    /* (72)N/E間データ引継ぎ開始通知                                      */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                   /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                   /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                   /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                   /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                   /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                   /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (73)N/E間データ引継ぎ完了通知                                      */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                    /* N/E切替中                                */
    },
    /* (74)CPRI link Recovery Notification                                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                   /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (75)(0x86050000)RE file download permission request                                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,           /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (76)(0x8A020000)MT data sending permission request                                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,         /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (77)データリンク解放表示                                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,             /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
//	/*!< (78)(0x4001)RE Forwarding Notice( MTアドレス設定要求 )			 						*/
//    {
//        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
//        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
//        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
//        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
//        f_trp_rec_Fc02_m_err,            	/* RE起動中(REキャリア状態報告完了)         */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
//        f_trp_rec_Fc02_m_err,            	/* 運用中                                   */
//        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
//        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
//        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
//        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
//        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
//    },
	/* (78)タイムアウト発生通知(遅延値設定待ちタイマ FHM)                          */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,            			/* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_mtsetf_fhm_to,             /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_mtsets_fhm_to,             /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,            			/* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
//	/*!< (80)(0x4001)RE Forwarding Notice( 「MTアドレス設定確認タイマ」タイムアウト(REC) )		*/
//    {
//        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
//        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
//        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
//        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
//        f_trp_rec_Fc02_m_err,            			/* RE起動中(REキャリア状態報告完了)         */
//        f_trp_rec_Fc02_m_mtsetf_rec_to,             /* RE起動中(MTアドレス設定(FLD-MT)中)       */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
//        f_trp_rec_Fc02_m_err,            			/* 運用中                                   */
//        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
//        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
//        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
//        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
//        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
//    }
};

/**
 * @brief   Rc02 matrix table (3G)
 * @Bug_No  M-RRU-ZSYS-01793
 * @Bug_No  M-RRU-ZSYS-01986
 * @date    2015/10/22 TDIPS)ikeda M-RRU-ZSYS-01793 
 * @date    2015/11/14 TDIPS)sasaki M-RRU-ZSYS-01986 ヘルスチェックリトライオーバーで強制停止にならない不具合修正
 */
VOID (*const f_trp_rec_Rc02_m_fnc_3g[EC_REC_C02_EVT_MAX][EC_REC_C02_STN_MAX])(VOID*) = {
    /* (00)スレッド初期化要求                                             */
    {
        f_trp_rec_Fc02_m_ini_init_exe_3g,
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (01)全スレッド初期化完了通知                                       */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (02)全プロセス初期化完了通知                                       */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (03)CPRIリンク状態変化通知(StateB)                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_rst_rerst_cmp_3g,          /* REリセット中                             */
        f_trp_rec_Fc02_m_rst_refrcrst_cmp_3g,       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (04)CPRIリンク状態変化通知(StateE)                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_2nd_rdy_3g,                /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (05)CPRIリンク状態変化通知(StateF)                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_2nd_rdy_3g,                /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_res_rdy_3g,                /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                  /* N/E切替中                                */
    },
    /* (06)CPRIリンク状態変化通知(L3確立)                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_res_sta_3g,                /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_idl_restsuse_exe_3g,       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                 /* N/E切替中                                */
    },
    /* (07)CPRIリンク断通知(StateF未満)                                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_dsc_useini_exe_3g,         /* 運用中                                   */
        f_trp_rec_Fc02_m_dsc_resini_exe_3g,         /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop              /* N/E切替中                                */
    },
    /* (08)CPRIリンク断通知(StateF)                                       */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_dsc_resrst_exe_3g,         /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_dsc_useidl_exe_3g,         /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop              /* N/E切替中                                */
    },
    /* (09)割込(T14値変化)通知                                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,            /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,            /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (10)Active設定応答                                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop             /* N/E切替中                                */
    },
    /* (11)Active切替結果通知(OK)                                         */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,             /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,             /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,             /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,             /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,             /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,             /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                /* N/E切替中                                */
    },
    /* (12)Active切替結果通知(NG)                                         */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                   /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                   /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                   /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                   /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                   /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                   /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (13)Negative通知(Negative)                                         */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                   /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                   /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                   /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                   /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                   /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                   /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                /* N/E切替中                                */
    },
    /* (14)Negative通知(Negative TO)                                      */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,         /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,         /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,         /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,         /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,         /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,         /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,         /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,         /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop          /* N/E切替中                                */
    },
    /* (15)二次リセット要求                                               */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                   	/* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_2nd_sta_3g,                /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (16)REシステムパラメータ更新応答                                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_res_reprm_end_3g,          /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (17)REファイル更新応答                                             */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_res_refil_end_3g,          /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
   /* (18)MTアドレス設定(FLD-MT)応答                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_res_mtsetf_end_3g,         /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_res_mtsets_end_3g,         /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,            			/* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop             /* N/E切替中                                */
    },
    /* (19)MTアドレス設定(SV-MT)応答                                      */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,            			/* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop             /* N/E切替中                                */
    },
    /* (20)二次リセットプロシージャ実行指示                               */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (21)RE時刻設定プロシージャ実行指示                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_res_retimset_exe_3g,       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (22)REシステムパラメータ更新プロシージャ実行指示                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_res_reprm_sta_3g,          /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (23)REファイル更新プロシージャ実行指示                             */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_res_refil_sta_3g,          /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (24)Toffset値報告プロシージャ実行指示                              */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_res_tofval_sta_3g,         /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (25)Active設定プロシージャ実行指示                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,            /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (26)RE装置構成情報報告プロシージャ実行指示                         */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_res_reeqpinf_sta_3g,       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (27)REスロット情報報告プロシージャ実行指示                         */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_res_resltinf_sta_3g,       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (28)RE状態報告確プロシージャ実行指示                               */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_res_rests_sta_3g,          /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (29)RE PORT状態報告プロシージャ実行指示                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_res_reprtsts_sta_3g,       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (30)REカード状態報告(セクタ部)プロシージャ実行指示                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_res_recrdssc_sta_3g,       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (31)REカード状態報告(共通部)プロシージャ実行指示                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (32)REキャリア状態報告プロシージャ実行指示                         */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_res_recrrsts_sta_3g,       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (33)MTアドレス設定(FLD-MT)プロシージャ実行指示                     */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_res_mtsetf_sta_3g,         /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (34)MTアドレス設定(SV-MT)プロシージャ実行指示                      */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_res_mtsets_sta_3g,         /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,            			/* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (35)RE起動完了プロシージャ実行指示                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_res_cmp_3g,                /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (36)REリセットプロシージャ実行指示                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_rst_rerst_sta_3g,          /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_rst_rerst_sta_3g,          /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (37)RE強制リセット設置プロシージャ実行指示                         */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (38)RE強制リセットプロシージャ実行指示                             */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_res_refrcrst_sta_3g,       /* REリセット中                             */
        f_trp_rec_Fc02_m_rst_refrcrst_sta_3g,       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (39)強制停止プロシージャ実行指示                                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* 運用中                                   */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* REリセット中                             */
        f_trp_rec_Fc02_m_rst_frcstp_exe_3g,         /* RE強制リセット中                         */
		f_trp_rec_Fc02_m_rst_frcstp_exe_3g,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (40)CPRI信号受信通知(Toffset値報告応答)                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_res_tofval_end_3g,         /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                      /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (41)CPRI信号受信通知(RE装置構成情報報告応答)                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_res_reeqpinf_end_3g,       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (42)CPRI信号受信通知(REスロット情報報告)                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_res_resltinf_end_3g,       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop           /* N/E切替中                                */
    },
    /* (43)CPRI信号受信通知(RE状態報告応答)                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_res_rests_end_3g,          /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_rests_end_3g,             /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (44)CPRI信号受信通知(RE PORT状態報告応答)                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_res_reprtsts_end_3g,       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (45)CPRI信号受信通知(REカード状態報告(セクタ部)応答)               */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_res_recrdssc_end_3g,       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_recrdssc_end_3g,       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (46)CPRI信号受信通知(REカード状態報告(共通部)応答)                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (47)CPRI信号受信通知(REキャリア状態報告応答)                            */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_res_recrrsts_end_3g,       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_recrrsts_end_3g,       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (48)タイムアウト発生通知(Toffset値報告確認タイマ)                  */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_res_tofval_stp_3g,         /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (49)タイムアウト発生通知(RE装置構成情報報告確認タイマ)                  */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_res_reeqpinf_stp_3g,       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,       				/* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (50)タイムアウト発生通知(REスロット情報報告確認タイマ)                  */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_res_resltinf_stp_3g,       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (51)タイムアウト発生通知(RE状態報告確認タイマ)                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_res_rests_stp_3g,          /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_rests_stp_3g,          /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (52)タイムアウト発生通知(RE PORT状態報告確認タイマ)                  */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_res_reprtsts_stp_3g,       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (53)タイムアウト発生通知(REカード状態報告(セクタ部)確認タイマ)     */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_res_recrdssc_stp_3g,       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_recrdssc_stp_3g,       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (54)タイムアウト発生通知(REカード状態報告(共通部)確認タイマ)       */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (55)タイムアウト発生通知(REキャリア状態報告確認タイマ)       */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_res_recrrsts_stp_3g,       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_recrrsts_stp_3g,       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (56)タイムアウト発生通知(REリセット確認タイマ)                     */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_rst_rerst_stp_3g,          /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (57)タイムアウト発生通知(RE強制リセット確認タイマ)                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_rst_refrcrst_stp_3g,       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (58)タイムアウト発生通知(RE時刻設定送信間隔タイマ)                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,          /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (59)タイムアウト発生通知(Toffset値報告送信間隔タイマ)              */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,            /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (60)タイムアウト発生通知(RE装置構成情報報告送信間隔タイマ)              */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,      					/* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (61)タイムアウト発生通知(RE状態報告送信間隔タイマ)              */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_rests_sta_3g,          /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },    
    /* (62)タイムアウト発生通知(RE PORT状態報告送信間隔タイマ)              */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (63)タイムアウト発生通知(REカード状態報告(セクタ部)送信間隔タイマ) */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_recrdssc_sta_3g,       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (64)タイムアウト発生通知(REカード状態報告(共通部)送信間隔タイマ)   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (65)タイムアウト発生通知(REキャリア状態報告送信間隔タイマ)             */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_use_recrrsts_sta_3g,       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (66)タイムアウト発生通知(二次リセット待ちタイマ)                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                   	/* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_2nd_exe_3g,                /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (67)タイムアウト発生通知(Active切替指示送信待ちタイマ)             */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                   /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                   /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                   /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                   /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                   /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                   /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (68)タイムアウト発生通知(遅延値設定待ちタイマ)                          */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (69)タイムアウト発生通知(REauto Reset 待ちタイマ)           */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* 運用中                                   */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* REリセット中                             */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_reautrst_exe_3g,           /* 強制停止                                 */
        f_trp_rec_Fc02_m_reautrst_exe_3g            /* N/E切替中                                */
    },
    /* (70)N/E切替開始要求                                                 */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                   /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                   /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                   /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                   /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                   /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                   /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (71)N/E切替完了通知                                                */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                    /* N/E切替中                                */
    },
    /* (72)N/E間データ引継ぎ開始通知                                      */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                   /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                   /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                   /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_nop,                   /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                   /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                   /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
    /* (73)N/E間データ引継ぎ完了通知                                      */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,                       /* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                    /* N/E切替中                                */
    },
    /* (74)CPRI link Recovery Notification                                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_nop,                   /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_nop,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_nop,                   /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,                   /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (75)(0x86050000)RE file download permission request                                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,           /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (76)(0x8A020000)MT data sending permission request                                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,         /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
    /* (77)データリンク解放表示                                   */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_err,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_nop,             /* 運用中                                   */
        f_trp_rec_Fc02_m_nop,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_err,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_err,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_err                        /* N/E切替中                                */
    },
//	/*!< (78)(0x4001)RE Forwarding Notice( MTアドレス設定要求 )			 						*/
//    {
//        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
//        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
//        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
//        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(L3確立完了)                     */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(RE時刻設定完了)                 */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(REシステムパラメータ更新中)     */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(REシステムパラメータ更新完了)   */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(REファイル更新中)               */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(REファイル更新完了)             */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(Toffset値報告中)                */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(Toffset値報告完了)              */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(Active設定中)                   */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(Active設定完了)                 */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(RE装置構成情報報告中)           */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(RE装置構成情報報告完了)         */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(REスロット情報報告中)           */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(REスロット情報報告完了)         */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(RE状態報告中)                   */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(RE状態報告完了)                 */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(RE PORT状態報告中)              */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(RE PORT状態報告完了)            */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(REカード状態報告(共通部)中)     */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(REカード状態報告(共通部)完了)   */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(REキャリア状態報告中)           */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,            	/* RE起動中(REキャリア状態報告完了)         */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(MTアドレス設定(FLD-MT)中)       */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
//        f_trp_rec_Fc02_m_mtsetf_rcv_3g,            	/* 運用中                                   */
//        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
//        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
//        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
//        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
//        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
//    },
	/* (79)タイムアウト発生通知(遅延値設定待ちタイマ FHM)                          */
    {
        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
        f_trp_rec_Fc02_m_err,            			/* RE起動中(REキャリア状態報告完了)         */
        f_trp_rec_Fc02_m_mtsetf_fhm_to_3g,          /* RE起動中(MTアドレス設定(FLD-MT)中)       */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
        f_trp_rec_Fc02_m_mtsets_fhm_to_3g,          /* RE起動中(MTアドレス設定(SV-MT)中)        */
        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
        f_trp_rec_Fc02_m_err,            			/* 運用中                                   */
        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
    },
//	/*!< (80)(0x4001)RE Forwarding Notice( 「MTアドレス設定確認タイマ」タイムアウト(REC) )		*/
//    {
//        f_trp_rec_Fc02_m_err,                       /* 初期                                     */
//        f_trp_rec_Fc02_m_nop,                       /* LAPBリンク確立前                         */
//        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備中                       */
//        f_trp_rec_Fc02_m_err,                       /* 二次リセット準備完了                     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動待ちアイドル                       */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(L3確立完了)                     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE時刻設定完了)                 */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新中)     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REシステムパラメータ更新完了)   */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新中)               */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REファイル更新完了)             */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告中)                */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(Toffset値報告完了)              */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定中)                   */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(Active設定完了)                 */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告中)           */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE装置構成情報報告完了)         */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告中)           */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REスロット情報報告完了)         */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告中)                   */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE状態報告完了)                 */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告中)              */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(RE PORT状態報告完了)            */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)中)   */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(セクタ部)完了) */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)中)     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REカード状態報告(共通部)完了)   */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(REキャリア状態報告中)           */
//        f_trp_rec_Fc02_m_err,            			/* RE起動中(REキャリア状態報告完了)         */
//        f_trp_rec_Fc02_m_mtsetf_rec_to_3g,          /* RE起動中(MTアドレス設定(FLD-MT)中)       */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(FLD-MT)完了)     */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)中)        */
//        f_trp_rec_Fc02_m_err,                       /* RE起動中(MTアドレス設定(SV-MT)完了)      */
//        f_trp_rec_Fc02_m_err,            			/* 運用中                                   */
//        f_trp_rec_Fc02_m_err,                       /* 運用中アイドル                           */
//        f_trp_rec_Fc02_m_err,                       /* REリセット中                             */
//        f_trp_rec_Fc02_m_nop,                       /* RE強制リセット中                         */
//        f_trp_rec_Fc02_m_nop,                       /* 強制停止                                 */
//        f_trp_rec_Fc02_m_nop                        /* N/E切替中                                */
//    },
};


VOID (*const f_trp_rec_Rc03_m_fnc[EC_REC_C03_EVT_MAX][EC_REC_C03_STN_MAX])(VOID*) = {
    /* (00)REからキャリア状態報告応答受信                                  */
    {
        f_trp_rec_Rc03_m_idle_sts,                  /* アイドル            */
        f_trp_rec_Fc03_m_nop,                       /* 自律TRX解放中       */
        f_trp_rec_Fc03_m_nop                        /* 自律TRX設定中       */
    },
    /* (01)REからTRX解放応答受信                                           */
    {
        f_trp_rec_Fc03_m_txr_idl_rcv,               /* アイドル            */
        f_trp_rec_Rc03_m_txr_txr,                   /* 自律TRX解放中       */
        f_trp_rec_Fc03_m_nop                        /* 自律TRX設定中       */
    },
    /* (02)REからTRX設定応答受信                                           */
    {
        f_trp_rec_Fc03_m_txs_idl_rcv,               /* アイドル            */
        f_trp_rec_Fc03_m_nop,                       /* 自律TRX解放中       */
        f_trp_rec_Rc03_m_txs_txs                    /* 自律TRX設定中       */
    },
    /* (03)REからのTRX解放応答待ちタイムアウト                             */
    {
        f_trp_rec_Fc03_m_err,                       /* アイドル            */
        f_trp_rec_Rc03_m_txr_to,                    /* 自律TRX解放中       */
        f_trp_rec_Fc03_m_err                        /* 自律TRX設定中       */
    },
    /* (04)RECからのTRX設定応答待ちタイムアウト                            */
    {
        f_trp_rec_Fc03_m_err,                       /* アイドル            */
        f_trp_rec_Fc03_m_err,                       /* 自律TRX解放中       */
        f_trp_rec_Rc03_m_txs_to                     /* 自律TRX設定中       */
    },
    /* (05)RECからのTRX設定要求受信                                        */
    {
        f_trp_rec_Fc03_m_txs_idl_fw,                /* アイドル            */
        f_trp_rec_Rc03_m_txs_fw,                    /* 自律TRX解放中       */
        f_trp_rec_Fc03_m_err                        /* 自律TRX設定中       */
    },
    /* (06)RECからのTRX解放要求受信                                        */
    {
        f_trp_rec_Fc03_m_txr_idl_fw,                /* アイドル            */
        f_trp_rec_Fc03_m_err,                       /* 自律TRX解放中       */
        f_trp_rec_Rc03_m_rst                        /* 自律TRX設定中       */
    },
    /* (07)Receive CARONOFF request from REC                                        */
    {
        f_trp_rec_Fc03_m_caronoff_idl_fw,           /* アイドル            */
        f_trp_rec_Fc03_m_nop,                       /* 自律TRX解放中       */
        f_trp_rec_Fc03_m_nop                        /* 自律TRX設定中       */
    },
    /* (08)Receive CARONOFF response from RE                                        */
    {
        f_trp_rec_Fc03_m_caronoff_idl_rcv,          /* アイドル            */
        f_trp_rec_Fc03_m_nop,                       /* 自律TRX解放中       */
        f_trp_rec_Fc03_m_nop                        /* 自律TRX設定中       */
    },
    /* (09)REキャリア状態報告応答                                     */
    {
        f_trp_rec_Fc03_m_carstsrep_idl_fw,          /* アイドル */
        f_trp_rec_Fc03_m_nop,                       /* 自律TRX解放中 */
        f_trp_rec_Fc03_m_nop                        /* 自律TRX解放中 */
    },
    /* (10)REキャリア状態報告要求                                     */
    {
        f_trp_rec_Fc03_m_carstsrep_idl_rcv,         /* アイドル */
        f_trp_rec_Fc03_m_nop,                       /* 自律TRX解放中 */
        f_trp_rec_Fc03_m_nop                        /* 自律TRX解放中 */
    }
};

VOID (*const f_trp_rec_Rc03_m_3G_fnc[EC_REC_C03_EVT_MAX][EC_REC_C03_STN_MAX])(VOID*) = {
    /* (00)REからキャリア状態報告応答受信                                  */
    {
        f_trp_rec_Rc03_m_idle_sts_3g,               /* アイドル            */
        f_trp_rec_Fc03_m_nop,                       /* 自律TRX解放中       */
        f_trp_rec_Fc03_m_nop                        /* 自律TRX設定中       */
    },
    /* (01)REからTRX解放応答受信                                           */
    {
        f_trp_rec_Fc03_m_txr_idl_rcv_3g,            /* アイドル            */
        f_trp_rec_Rc03_m_txr_txr_3g,                /* 自律TRX解放中       */
        f_trp_rec_Fc03_m_nop                        /* 自律TRX設定中       */
    },
    /* (02)REからTRX設定応答受信                                           */
    {
        f_trp_rec_Fc03_m_txs_idl_rcv_3g,            /* アイドル            */
        f_trp_rec_Fc03_m_nop,                       /* 自律TRX解放中       */
        f_trp_rec_Rc03_m_txs_txs_3g                 /* 自律TRX設定中       */
    },
    /* (03)REからのTRX解放応答待ちタイムアウト                             */
    {
        f_trp_rec_Fc03_m_err,                       /* アイドル            */
        f_trp_rec_Rc03_m_txr_to_3g,                 /* 自律TRX解放中       */
        f_trp_rec_Fc03_m_err                        /* 自律TRX設定中       */
    },
    /* (04)RECからのTRX設定応答待ちタイムアウト                            */
    {
        f_trp_rec_Fc03_m_err,                       /* アイドル            */
        f_trp_rec_Fc03_m_err,                       /* 自律TRX解放中       */
        f_trp_rec_Rc03_m_txs_to_3g                  /* 自律TRX設定中       */
    },
    /* (05)RECからのTRX設定要求受信                                        */
    {
        f_trp_rec_Fc03_m_txs_idl_fw_3g,             /* アイドル            */
        f_trp_rec_Rc03_m_txs_fw_3g,                 /* 自律TRX解放中       */
        f_trp_rec_Fc03_m_err                        /* 自律TRX設定中       */
    },
    /* (06)RECからのTRX解放要求受信                                        */
    {
        f_trp_rec_Fc03_m_txr_idl_fw,                /* アイドル            */
        f_trp_rec_Fc03_m_err,                       /* 自律TRX解放中       */
        f_trp_rec_Rc03_m_rst_3g                     /* 自律TRX設定中       */
    }
};

#if 0
VOID (*const f_trp_rec_Rc04_m_fnc[EC_REC_C04_EVT_MAX][EC_REC_C04_STN_MAX])(VOID*) = {
    /* (00)スレッド初期化要求                                             */
    {
        f_trp_rec_Fc04_m_ini_init_exe,              /* 初期                         */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF完了        */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON準備中       */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON中           */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON(自律)準備中 */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON(自律)中     */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON完了         */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF準備中      */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF中          */
        f_trp_rec_Fc04_m_err                        /* N/E切替中                    */
    },
    /* (01)全スレッド初期化完了通知                                       */
    {
        f_trp_rec_Fc04_m_err,                       /* 初期                         */
        f_trp_rec_Fc04_m_nop,                       /* 無変調キャリアOFF完了        */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON準備中       */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON中           */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON(自律)準備中 */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON(自律)中     */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON完了         */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF準備中      */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF中          */
        f_trp_rec_Fc04_m_err                        /* N/E切替中                    */
    },
    /* (02)全プロセス初期化完了通知                                       */
    {
        f_trp_rec_Fc04_m_err,                       /* 初期                         */
        f_trp_rec_Fc04_m_nop,                       /* 無変調キャリアOFF完了        */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON準備中       */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON中           */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON(自律)準備中 */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON(自律)中     */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON完了         */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF準備中      */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF中          */
        f_trp_rec_Fc04_m_err                        /* N/E切替中                    */
    },
    /* (03)無変調キャリアON/OFF指定(自律)(ON)プロシージャ実行指示         */
    {
        f_trp_rec_Fc04_m_err,                       /* 初期                         */
        f_trp_rec_Fc04_m_nop,                       /* 無変調キャリアOFF完了        */
        f_trp_rec_Fc04_m_nop,                       /* 無変調キャリアON準備中       */
        f_trp_rec_Fc04_m_nop,                       /* 無変調キャリアON中           */
        f_trp_rec_Fc04_m_non_a_sta,                 /* 無変調キャリアON(自律)準備中 */
        f_trp_rec_Fc04_m_nop,                       /* 無変調キャリアON(自律)中     */
        f_trp_rec_Fc04_m_non_a_sta,                 /* 無変調キャリアON完了         */
        f_trp_rec_Fc04_m_nop,                       /* 無変調キャリアOFF準備中      */
        f_trp_rec_Fc04_m_nop,                       /* 無変調キャリアOFF中          */
        f_trp_rec_Fc04_m_nop                        /* N/E切替中                    */
    },
    /* (04)無変調キャリアON/OFF指定(自律)(ON)プロシージャ実行指示         */
    {
        f_trp_rec_Fc04_m_err,                       /* 初期                         */
        f_trp_rec_Fc04_m_nop,                       /* 無変調キャリアOFF完了        */
        f_trp_rec_Fc04_m_nof_a_sta,                 /* 無変調キャリアON準備中       */
        f_trp_rec_Fc04_m_nof_a_sta,                 /* 無変調キャリアON中           */
        f_trp_rec_Fc04_m_nof_a_sta,                 /* 無変調キャリアON(自律)準備中 */
        f_trp_rec_Fc04_m_nof_a_sta,                 /* 無変調キャリアON(自律)中     */
        f_trp_rec_Fc04_m_nof_a_sta,                 /* 無変調キャリアON完了         */
        f_trp_rec_Fc04_m_nof_a_sta,                 /* 無変調キャリアOFF準備中      */
        f_trp_rec_Fc04_m_nof_a_sta,                 /* 無変調キャリアOFF中          */
        f_trp_rec_Fc04_m_nop                        /* N/E切替中                    */
    },
    /* (05)CPRI信号受信通知(無変調キャリアON/OFF指定応答)                 */
    {
        f_trp_rec_Fc04_m_err,                       /* 初期                         */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF完了        */
        f_trp_rec_Fc04_m_non_r_sta,                 /* 無変調キャリアON準備中       */
        f_trp_rec_Fc04_m_non_m_end,                 /* 無変調キャリアON中           */
        f_trp_rec_Fc04_m_nop,                       /* 無変調キャリアON(自律)準備中 */
        f_trp_rec_Fc04_m_non_a_end,                 /* 無変調キャリアON(自律)中     */
        f_trp_rec_Fc04_m_nop,                       /* 無変調キャリアON完了         */
        f_trp_rec_Fc04_m_nof_r_sta,                 /* 無変調キャリアOFF準備中      */
        f_trp_rec_Fc04_m_nof_m_end,                 /* 無変調キャリアOFF中          */
        f_trp_rec_Fc04_m_nnf_n_end                  /* N/E切替中                    */
    },
    /* (06)タイムアウト発生通知(無変調キャリアON/OFF指定確認タイマ)       */
    {
        f_trp_rec_Fc04_m_err,                       /* 初期                         */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF完了        */
        f_trp_rec_Fc04_m_non_r_sta,                 /* 無変調キャリアON準備中       */
        f_trp_rec_Fc04_m_non_m_stp,                 /* 無変調キャリアON中           */
        f_trp_rec_Fc04_m_nop,                       /* 無変調キャリアON(自律)準備中 */
        f_trp_rec_Fc04_m_non_a_stp,                 /* 無変調キャリアON(自律)中     */
        f_trp_rec_Fc04_m_nop,                       /* 無変調キャリアON完了         */
        f_trp_rec_Fc04_m_nof_r_sta,                 /* 無変調キャリアOFF準備中      */
        f_trp_rec_Fc04_m_nof_m_stp,                 /* 無変調キャリアOFF中          */
        f_trp_rec_Fc04_m_nnf_n_stp                  /* N/E切替中                    */
    },
    /* (07)無変調キャリアON/OFF指定(ON)要求                               */
    {
        f_trp_rec_Fc04_m_err,                       /* 初期                         */
        f_trp_rec_Fc04_m_non_m_sta,                 /* 無変調キャリアOFF完了        */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON準備中       */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON中           */
        f_trp_rec_Fc04_m_non_m_rac,                 /* 無変調キャリアON(自律)準備中 */
        f_trp_rec_Fc04_m_non_m_rac,                 /* 無変調キャリアON(自律)中     */
        f_trp_rec_Fc04_m_non_m_sta,                 /* 無変調キャリアON完了         */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF準備中      */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF中          */
        f_trp_rec_Fc04_m_err                        /* N/E切替中                    */
    },
    /* (08)無変調キャリアON/OFF指定(OFF)要求                              */
    {
        f_trp_rec_Fc04_m_err,                       /* 初期                         */
        f_trp_rec_Fc04_m_nof_m_sta,                 /* 無変調キャリアOFF完了        */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON準備中       */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON中           */
        f_trp_rec_Fc04_m_nof_m_rac,                 /* 無変調キャリアON(自律)準備中 */
        f_trp_rec_Fc04_m_nof_m_rac,                 /* 無変調キャリアON(自律)中     */
        f_trp_rec_Fc04_m_nof_m_sta,                 /* 無変調キャリアON完了         */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF準備中      */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF中          */
        f_trp_rec_Fc04_m_err                        /* N/E切替中                    */
    },
    /* (09)N/E切替開始要求                                               */
    {
        f_trp_rec_Fc04_m_err,                       /* 初期                         */
        f_trp_rec_Fc04_m_nes_sta,                   /* 無変調キャリアOFF完了        */
        f_trp_rec_Fc04_m_nes_sta,                   /* 無変調キャリアON準備中       */
        f_trp_rec_Fc04_m_nes_sta,                   /* 無変調キャリアON中           */
        f_trp_rec_Fc04_m_nes_sta,                   /* 無変調キャリアON(自律)準備中 */
        f_trp_rec_Fc04_m_nes_sta,                   /* 無変調キャリアON(自律)中     */
        f_trp_rec_Fc04_m_nes_sta,                   /* 無変調キャリアON完了         */
        f_trp_rec_Fc04_m_nes_sta,                   /* 無変調キャリアOFF準備中      */
        f_trp_rec_Fc04_m_nes_sta,                   /* 無変調キャリアOFF中          */
        f_trp_rec_Fc04_m_err                        /* N/E切替中                    */
    },
    /* (10)N/E切替完了通知                                               */
    {
        f_trp_rec_Fc04_m_err,                       /* 初期                         */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF完了        */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON準備中       */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON中           */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON(自律)準備中 */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON(自律)中     */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON完了         */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF準備中      */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF中          */
        f_trp_rec_Fc04_m_nes_end                    /* N/E切替中                    */
    },
    /* (11)N/E間データ引継ぎ開始通知                                     */
    {
        f_trp_rec_Fc04_m_err,                       /* 初期                         */
        f_trp_rec_Fc04_m_suc_sta,                   /* 無変調キャリアOFF完了        */
        f_trp_rec_Fc04_m_suc_sta,                   /* 無変調キャリアON準備中       */
        f_trp_rec_Fc04_m_suc_sta,                   /* 無変調キャリアON中           */
        f_trp_rec_Fc04_m_suc_sta,                   /* 無変調キャリアON(自律)準備中 */
        f_trp_rec_Fc04_m_suc_sta,                   /* 無変調キャリアON(自律)中     */
        f_trp_rec_Fc04_m_suc_sta,                   /* 無変調キャリアON完了         */
        f_trp_rec_Fc04_m_suc_sta,                   /* 無変調キャリアOFF準備中      */
        f_trp_rec_Fc04_m_suc_sta,                   /* 無変調キャリアOFF中          */
        f_trp_rec_Fc04_m_nop                        /* N/E切替中                    */
    },
    /* (12)N/E間データ引継ぎ完了通知                                     */
    {
        f_trp_rec_Fc04_m_err,                       /* 初期                         */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF完了        */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON準備中       */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON中           */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON(自律)準備中 */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON(自律)中     */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON完了         */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF準備中      */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF中          */
        f_trp_rec_Fc04_m_suc_end                    /* N/E切替中                    */
    },
    /* (13)CPRI link Recovery Notice                                     */
    {
        f_trp_rec_Fc04_m_err,                       /* 初期                         */
        f_trp_rec_Fc04_m_nop,                       /* 無変調キャリアOFF完了        */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON準備中       */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON中           */
        f_trp_rec_Fc04_m_rcv_exe,                   /* 無変調キャリアON(自律)準備中 */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアON(自律)中     */
        f_trp_rec_Fc04_m_nop,                       /* 無変調キャリアON完了         */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF準備中      */
        f_trp_rec_Fc04_m_err,                       /* 無変調キャリアOFF中          */
        f_trp_rec_Fc04_m_err                        /* N/E切替中                    */
    }
};

VOID (*const f_trp_rec_Rc05_m_fnc[EC_REC_C05_EVT_MAX][EC_REC_C05_STN_MAX])(VOID*) = {
    /* (00)スレッド初期化要求                                             */
    {
        f_trp_rec_Fc05_m_ini_init_exe,              /* 初期               */
        f_trp_rec_Fc05_m_err,                       /* REリセット不可     */
        f_trp_rec_Fc05_m_err,                       /* RE強制リセット可能 */
        f_trp_rec_Fc05_m_err,                       /* REリセット可能     */
        f_trp_rec_Fc05_m_err,                       /* RE強制リセット中   */
        f_trp_rec_Fc05_m_err,                       /* REリセット中       */
        f_trp_rec_Fc05_m_err                        /* N/E切替中          */
    },
    /* (01)全スレッド初期化完了通知                                       */
    {
        f_trp_rec_Fc05_m_err,                       /* 初期               */
        f_trp_rec_Fc05_m_nop,                       /* REリセット不可     */
        f_trp_rec_Fc05_m_err,                       /* RE強制リセット可能 */
        f_trp_rec_Fc05_m_err,                       /* REリセット可能     */
        f_trp_rec_Fc05_m_err,                       /* REリセット中       */
        f_trp_rec_Fc05_m_err,                       /* RE強制リセット中   */
        f_trp_rec_Fc05_m_err                        /* N/E切替中          */
    },
    /* (02)全プロセス初期化完了通知                                       */
    {
        f_trp_rec_Fc05_m_err,                       /* 初期               */
        f_trp_rec_Fc05_m_nop,                       /* REリセット不可     */
        f_trp_rec_Fc05_m_err,                       /* RE強制リセット可能 */
        f_trp_rec_Fc05_m_err,                       /* REリセット可能     */
        f_trp_rec_Fc05_m_err,                       /* REリセット中       */
        f_trp_rec_Fc05_m_err,                       /* RE強制リセット中   */
        f_trp_rec_Fc05_m_err                        /* N/E切替中          */
    },
    /* (03)CPRIリンク状態変化通知(StateB)                                 */
    {
        f_trp_rec_Fc05_m_err,                       /* 初期               */
        f_trp_rec_Fc05_m_rst_imp_exe,               /* REリセット不可     */
        f_trp_rec_Fc05_m_rst_imp_exe,               /* RE強制リセット可能 */
        f_trp_rec_Fc05_m_rst_imp_exe,               /* REリセット可能     */
        f_trp_rec_Fc05_m_rst_end,                   /* REリセット中       */
        f_trp_rec_Fc05_m_frt_end,                   /* RE強制リセット中   */
        f_trp_rec_Fc05_m_nop                        /* N/E切替中          */
    },
    /* (04)CPRIリンク状態変化通知(StateC)                                 */
    {
        f_trp_rec_Fc05_m_err,                       /* 初期               */
        f_trp_rec_Fc05_m_frt_pos_exe,               /* REリセット不可     */
        f_trp_rec_Fc05_m_frt_pos_exe,               /* RE強制リセット可能 */
        f_trp_rec_Fc05_m_frt_pos_exe,               /* REリセット可能     */
        f_trp_rec_Fc05_m_frt_pos_rdy,               /* REリセット中       */
        f_trp_rec_Fc05_m_frt_pos_rdy,               /* RE強制リセット中   */
        f_trp_rec_Fc05_m_nop                        /* N/E切替中          */
    },
    /* (05)CPRIリンク状態変化通知(StateF)                                 */
    {
        f_trp_rec_Fc05_m_err,                       /* 初期               */
        f_trp_rec_Fc05_m_rst_pos_exe,               /* REリセット不可     */
        f_trp_rec_Fc05_m_rst_pos_exe,               /* RE強制リセット可能 */
        f_trp_rec_Fc05_m_rst_pos_exe,               /* REリセット可能     */
        f_trp_rec_Fc05_m_rst_pos_rdy,               /* REリセット中       */
        f_trp_rec_Fc05_m_rst_pos_rdy,               /* RE強制リセット中   */
        f_trp_rec_Fc05_m_nop                        /* N/E切替中          */
    },
    /* (06)タイムアウト発生通知(REリセット確認タイマ)                     */
    {
        f_trp_rec_Fc05_m_err,                       /* 初期               */
        f_trp_rec_Fc05_m_err,                       /* REリセット不可     */
        f_trp_rec_Fc05_m_err,                       /* RE強制リセット可能 */
        f_trp_rec_Fc05_m_err,                       /* REリセット可能     */
        f_trp_rec_Fc05_m_rst_stp,                   /* REリセット中       */
        f_trp_rec_Fc05_m_err,                       /* RE強制リセット中   */
        f_trp_rec_Fc05_m_nop                        /* N/E切替中          */
    },
    /* (07)タイムアウト発生通知(RE強制リセット確認タイマ)                 */
    {
        f_trp_rec_Fc05_m_err,                       /* 初期               */
        f_trp_rec_Fc05_m_err,                       /* REリセット不可     */
        f_trp_rec_Fc05_m_err,                       /* RE強制リセット可能 */
        f_trp_rec_Fc05_m_err,                       /* REリセット可能     */
        f_trp_rec_Fc05_m_err,                       /* REリセット中       */
        f_trp_rec_Fc05_m_frt_stp,                   /* RE強制リセット中   */
        f_trp_rec_Fc05_m_nop                        /* N/E切替中          */
    },
    /* (08)CPRIリンクリセット要求(ソフトウェア)                           */
    {
        f_trp_rec_Fc05_m_err,                       /* 初期               */
        f_trp_rec_Fc05_m_rst_sta,                   /* REリセット不可     */
        f_trp_rec_Fc05_m_rst_sta,                   /* RE強制リセット可能 */
        f_trp_rec_Fc05_m_rst_sta,                   /* REリセット可能     */
        f_trp_rec_Fc05_m_rst_ing_rej,               /* REリセット中       */
        f_trp_rec_Fc05_m_rst_ing_rej,               /* RE強制リセット中   */
        f_trp_rec_Fc05_m_nop                        /* N/E切替中          */
    },
    /* (09)CPRIリンクリセット要求(ハードウェア)                           */
    {
        f_trp_rec_Fc05_m_err,                       /* 初期               */
        f_trp_rec_Fc05_m_frt_sta,                   /* REリセット不可     */
        f_trp_rec_Fc05_m_frt_sta,                   /* RE強制リセット可能 */
        f_trp_rec_Fc05_m_frt_sta,                   /* REリセット可能     */
        f_trp_rec_Fc05_m_frt_ing_rej,               /* REリセット中       */
        f_trp_rec_Fc05_m_frt_ing_rej,               /* RE強制リセット中   */
        f_trp_rec_Fc05_m_nop                        /* N/E切替中          */
    },
    /* (10)N/E切替開始要求                                                */
    {
        f_trp_rec_Fc05_m_err,                       /* 初期               */
        f_trp_rec_Fc05_m_nes_sta,                   /* REリセット不可     */
        f_trp_rec_Fc05_m_nes_sta,                   /* RE強制リセット可能 */
        f_trp_rec_Fc05_m_nes_sta,                   /* REリセット可能     */
        f_trp_rec_Fc05_m_nes_sta,                   /* REリセット中       */
        f_trp_rec_Fc05_m_nes_sta,                   /* RE強制リセット中   */
        f_trp_rec_Fc05_m_nop                        /* N/E切替中          */
    },
    /* (11)N/E切替完了通知                                                */
    {
        f_trp_rec_Fc05_m_err,                       /* 初期               */
        f_trp_rec_Fc05_m_err,                       /* REリセット不可     */
        f_trp_rec_Fc05_m_err,                       /* RE強制リセット可能 */
        f_trp_rec_Fc05_m_err,                       /* REリセット可能     */
        f_trp_rec_Fc05_m_err,                       /* REリセット中       */
        f_trp_rec_Fc05_m_err,                       /* RE強制リセット中   */
        f_trp_rec_Fc05_m_nes_end                    /* N/E切替中          */
    },
    /* (12)N/E間データ引継ぎ開始通知                                      */
    {
        f_trp_rec_Fc05_m_err,                       /* 初期               */
        f_trp_rec_Fc05_m_suc_sta,                   /* REリセット不可     */
        f_trp_rec_Fc05_m_suc_sta,                   /* RE強制リセット可能 */
        f_trp_rec_Fc05_m_suc_sta,                   /* REリセット可能     */
        f_trp_rec_Fc05_m_suc_sta,                   /* REリセット中       */
        f_trp_rec_Fc05_m_suc_sta,                   /* RE強制リセット中   */
        f_trp_rec_Fc05_m_nop                        /* N/E切替中          */
    },
    /* (13)N/E間データ引継ぎ完了通知                                       */
    {
        f_trp_rec_Fc05_m_err,                       /* 初期               */
        f_trp_rec_Fc05_m_err,                       /* REリセット不可     */
        f_trp_rec_Fc05_m_err,                       /* RE強制リセット可能 */
        f_trp_rec_Fc05_m_err,                       /* REリセット可能     */
        f_trp_rec_Fc05_m_err,                       /* REリセット中       */
        f_trp_rec_Fc05_m_err,                       /* RE強制リセット中   */
        f_trp_rec_Fc05_m_suc_end                    /* N/E切替中          */
    }
};

VOID (*const f_trp_rec_Rc06_m_fnc[EC_REC_C06_EVT_MAX][EC_REC_C06_STN_MAX])(VOID*) = {
    /* (00)スレッド初期化要求                                             */
    {
        f_trp_rec_Fc06_m_ini_init_exe,              /* 初期                 */
        f_trp_rec_Fc06_m_nop,                       /* アイドル             */
        f_trp_rec_Fc06_m_err,                       /* REファイル更新中     */
        f_trp_rec_Fc06_m_err                        /* N/E切替中            */
    },
    /* (01)全スレッド初期化完了通知                                       */
    {
        f_trp_rec_Fc06_m_err,                       /* 初期                 */
        f_trp_rec_Fc06_m_nop,                       /* アイドル             */
        f_trp_rec_Fc06_m_err,                       /* REファイル更新中     */
        f_trp_rec_Fc06_m_err                        /* N/E切替中            */
    },
    /* (02)全プロセス初期化完了通知                                       */
    {
        f_trp_rec_Fc06_m_err,                       /* 初期                 */
        f_trp_rec_Fc06_m_nop,                       /* アイドル             */
        f_trp_rec_Fc06_m_err,                       /* REファイル更新中     */
        f_trp_rec_Fc06_m_err                        /* N/E切替中            */
    },
    /* (03)CPRIリンク断通知(StateF未満)                                   */
    {
        f_trp_rec_Fc06_m_err,                       /* 初期                 */
        f_trp_rec_Fc06_m_nop,                       /* アイドル             */
        f_trp_rec_Fc06_m_refil_stp,                 /* REファイル更新中     */
        f_trp_rec_Fc06_m_err                        /* N/E切替中            */
    },
    /*  (04)REファイル更新指示                                           */
    {
        f_trp_rec_Fc06_m_err,                       /* 初期                 */
        f_trp_rec_Fc06_m_refil_sta,                 /* アイドル             */
        f_trp_rec_Fc06_m_err,                       /* REファイル更新中     */
        f_trp_rec_Fc06_m_err                        /* N/E切替中            */
    },
    /*  (05)REファイル更新応答                                           */
    {
        f_trp_rec_Fc06_m_err,                       /* 初期                 */
        f_trp_rec_Fc06_m_nop,                       /* アイドル             */
        f_trp_rec_Fc06_m_refil_end,                 /* REファイル更新中     */
        f_trp_rec_Fc06_m_err                        /* N/E切替中            */
    },
    /* (06)タイムアウト発生通知(REリセット確認タイマ)                     */
    {
        f_trp_rec_Fc06_m_err,                       /* 初期                 */
        f_trp_rec_Fc06_m_nop,                       /* アイドル             */
        f_trp_rec_Fc06_m_refil_to,                  /* REファイル更新中     */
        f_trp_rec_Fc06_m_err                        /* N/E切替中            */
    },
    /* (07)N/E切替開始要求                                                */
    {
        f_trp_rec_Fc06_m_err,                       /* 初期                 */
        f_trp_rec_Fc06_m_nes_sta,                   /* アイドル             */
        f_trp_rec_Fc06_m_nes_sta,                   /* REファイル更新中     */
        f_trp_rec_Fc06_m_err                        /* N/E切替中            */
    },
    /* (08)N/E切替完了通知                                                */
    {
        f_trp_rec_Fc06_m_err,                       /* 初期                 */
        f_trp_rec_Fc06_m_err,                       /* アイドル             */
        f_trp_rec_Fc06_m_err,                       /* REファイル更新中     */
        f_trp_rec_Fc06_m_nes_end                    /* N/E切替中            */
    },
    /* (09)N/E間データ引継ぎ開始通知                                      */
    {
        f_trp_rec_Fc06_m_err,                       /* 初期                 */
        f_trp_rec_Fc06_m_suc_sta,                   /* アイドル             */
        f_trp_rec_Fc06_m_suc_sta,                   /* REファイル更新中     */
        f_trp_rec_Fc06_m_nop                        /* N/E切替中            */
    },
    /* (10)N/E間データ引継ぎ完了通知                                      */
    {
        f_trp_rec_Fc06_m_err,                       /* 初期                 */
        f_trp_rec_Fc06_m_err,                       /* アイドル             */
        f_trp_rec_Fc06_m_err,                       /* REファイル更新中     */
        f_trp_rec_Fc06_m_suc_end                    /* N/E切替中            */
    }
};

VOID (*const f_trp_rec_Rc07_m_fnc[EC_REC_C07_EVT_MAX][EC_REC_C07_STN_MAX])(VOID*) = {
    /* (00)スレッド初期化要求                                             */
    {
        f_trp_rec_Fc07_m_ini_init_exe,              /* 初期                 */
        f_trp_rec_Fc07_m_nop,                       /* アイドル             */
        f_trp_rec_Fc07_m_err,                       /* REリセット中         */
        f_trp_rec_Fc07_m_err                        /* N/E切替中            */
    },
    /* (01)全スレッド初期化完了通知                                       */
    {
        f_trp_rec_Fc07_m_err,                       /* 初期                 */
        f_trp_rec_Fc07_m_nop,                       /* アイドル             */
        f_trp_rec_Fc07_m_err,                       /* REリセット中         */
        f_trp_rec_Fc07_m_nop                        /* N/E切替中            */
    },
    /* (02)全プロセス初期化完了通知                                       */
    {
        f_trp_rec_Fc07_m_err,                       /* 初期                 */
        f_trp_rec_Fc07_m_nop,                       /* アイドル             */
        f_trp_rec_Fc07_m_err,                       /* REリセット中         */
        f_trp_rec_Fc07_m_nop                        /* N/E切替中            */
    },
    /* (03)CPRIリンク状態変化通知(StateB)                                 */
    {
        f_trp_rec_Fc07_m_err,                       /* 初期                 */
        f_trp_rec_Fc07_m_nop,                       /* アイドル             */
        f_trp_rec_Fc07_m_rst_end,                   /* REリセット中         */
        f_trp_rec_Fc07_m_nop                        /* N/E切替中            */
    },
    /* (04)タイムアウト発生通知(REリセット確認タイマ)                     */
    {
        f_trp_rec_Fc07_m_err,                       /* 初期                 */
        f_trp_rec_Fc07_m_nop,                       /* アイドル             */
        f_trp_rec_Fc07_m_rst_stp,                   /* REリセット中         */
        f_trp_rec_Fc07_m_nop                        /* N/E切替中            */
    },
    /* (05))REリセット要求                                                */
    {
        f_trp_rec_Fc07_m_err,                       /* 初期                 */
        f_trp_rec_Fc07_m_rst_sta,                   /* アイドル             */
        f_trp_rec_Fc07_m_rst_ing_rej,               /* REリセット中         */
        f_trp_rec_Fc07_m_nop                        /* N/E切替中            */
    },
    /* (06)N/E切替開始要求                                                */
    {
        f_trp_rec_Fc07_m_err,                       /* 初期                 */
        f_trp_rec_Fc07_m_nes_sta,                   /* アイドル             */
        f_trp_rec_Fc07_m_nes_sta,                   /* REリセット中         */
        f_trp_rec_Fc07_m_err                        /* N/E切替中            */
    },
    /* (07)N/E切替完了通知                                                */
    {
        f_trp_rec_Fc07_m_err,                       /* 初期                 */
        f_trp_rec_Fc07_m_err,                       /* アイドル             */
        f_trp_rec_Fc07_m_err,                       /* REリセット中         */
        f_trp_rec_Fc07_m_nes_end                    /* N/E切替中            */
    },
    /* (08)N/E間データ引継ぎ開始通知                                      */
    {
        f_trp_rec_Fc07_m_err,                       /* 初期                 */
        f_trp_rec_Fc07_m_suc_sta,                   /* アイドル             */
        f_trp_rec_Fc07_m_suc_sta,                   /* REリセット中         */
        f_trp_rec_Fc07_m_nop                        /* N/E切替中            */
    },
    /* (09)N/E間データ引継ぎ完了通知                                      */
    {
        f_trp_rec_Fc07_m_err,                       /* 初期                 */
        f_trp_rec_Fc07_m_err,                       /* アイドル             */
        f_trp_rec_Fc07_m_err,                       /* REリセット中         */
        f_trp_rec_Fc07_m_suc_end                    /* N/E切替中            */
    }
};

VOID (*const f_trp_rec_Rc08_m_fnc[EC_REC_C08_EVT_MAX][EC_REC_C08_STN_MAX])(VOID*) = {
    /* (00)スレッド初期化要求                                             */
    {
        f_trp_rec_Fc08_m_ini_init_exe,              /* 初期                 */
        f_trp_rec_Fc08_m_nop,                       /* アイドル             */
        f_trp_rec_Fc08_m_err,                       /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_err                        /* N/E切替中            */
    },
    /* (01)全スレッド初期化完了通知                                       */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_nop,                       /* アイドル             */
        f_trp_rec_Fc08_m_err,                       /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_nop                        /* N/E切替中            */
    },
    /* (02)全プロセス初期化完了通知                                       */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_nop,                       /* アイドル             */
        f_trp_rec_Fc08_m_err,                       /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_nop                        /* N/E切替中            */
    },
    /* (03))CPRIリンク断通知                                                   */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_nop,                       /* アイドル             */
        f_trp_rec_Fc08_m_use_dsc_stp,               /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_nop                        /* N/E切替中            */
    },
    /* (04)Negative通知                                                   */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_nop,                       /* アイドル             */
        f_trp_rec_Fc08_m_use_neg_stp,               /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_nop                        /* N/E切替中            */
    },
    /* (05)RE 障害ログ取得要求                                            */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_use_relogget_sta,          /* アイドル             */
        f_trp_rec_Fc08_m_use_relogget_sta,          /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_err                        /* N/E切替中            */
    },
    /* (06)RE 障害ログ中止要求                                            */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_use_relogstp_sta,          /* アイドル             */
        f_trp_rec_Fc08_m_use_relogstp_sta,          /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_err                        /* N/E切替中            */
    },
    /* (07)CPRI信号受信通知(RE 障害ログ取得応答)                          */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_use_relogget_end,          /* アイドル             */
        f_trp_rec_Fc08_m_err,                       /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_nop                        /* N/E切替中            */
    },
    /* (08)CPRI信号受信通知(RE 障害ログ中止応答)                          */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_use_relogstp_end,          /* アイドル             */
        f_trp_rec_Fc08_m_use_relogstp_end,          /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_nop                        /* N/E切替中            */
    },
    /* (09)CPRI信号受信通知(ファイルデータ送信)                           */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_nop,                       /* アイドル             */
        f_trp_rec_Fc08_m_use_relogsnd_end,          /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_nop                        /* N/E切替中            */
    },
    /* (10)CPRI信号受信通知(ファイルデータ送信完了)                       */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_nop,                       /* アイドル             */
        f_trp_rec_Fc08_m_use_relogcmp_end,          /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_nop                        /* N/E切替中            */
    },
    /* (11)タイムアウト発生通知(RE 障害ログ取得確認タイマ)                */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_use_relogget_stp,          /* アイドル             */
        f_trp_rec_Fc08_m_err,                       /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_nop                        /* N/E切替中            */
    },
    /* (12)タイムアウト発生通知(RE 障害ログ中止確認タイマ)                */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_use_relogstp_stp,          /* アイドル             */
        f_trp_rec_Fc08_m_use_relogstp_stp,          /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_nop                        /* N/E切替中            */
    },
    /* (13)N/E切替開始要求                                                */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_nes_sta,                   /* アイドル             */
        f_trp_rec_Fc08_m_nes_sta,                   /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_err                        /* N/E切替中            */
    },
    /* (14)N/E切替完了通知                                                */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_err,                       /* アイドル             */
        f_trp_rec_Fc08_m_err,                       /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_nes_end                    /* N/E切替中            */
    },
    /* (15)N/E間データ引継ぎ開始通知                                      */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_suc_sta,                   /* アイドル             */
        f_trp_rec_Fc08_m_suc_sta,                   /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_nop                        /* N/E切替中            */
    },
    /* (16)N/E間データ引継ぎ完了通知                                      */
    {
        f_trp_rec_Fc08_m_err,                       /* 初期                 */
        f_trp_rec_Fc08_m_err,                       /* アイドル             */
        f_trp_rec_Fc08_m_err,                       /* RE 障害ログ取得中    */
        f_trp_rec_Fc08_m_suc_end                    /* N/E切替中            */
    }
};


VOID (*const f_trp_rec_RcFF_m_fnc[EC_REC_CFF_EVT_MAX][EC_REC_CFF_STN_MAX])(VOID*) = {
    /* (00)スレッド初期化要求                                             */
    {
        f_trp_rec_FcFF_m_ini_init_exe,              /* 初期      */
        f_trp_rec_FcFF_m_err,                       /* 運用中    */
        f_trp_rec_FcFF_m_err                        /* N/E切替中 */
    },
    /* (01)全スレッド初期化完了通知                                       */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_nop,                       /* 運用中    */
        f_trp_rec_FcFF_m_err                        /* N/E切替中 */
    },
    /* (02)全プロセス初期化完了通知                                       */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_nop,                       /* 運用中    */
        f_trp_rec_FcFF_m_err                        /* N/E切替中 */
    },
    /* (03)時刻変更通知                                                   */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_use_retimset_exe,          /* 運用中    */
        f_trp_rec_FcFF_m_err                        /* N/E切替中 */
    },
    /* (04)REカード制御要求                                               */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_use_recrdctl_sta,          /* 運用中    */
        f_trp_rec_FcFF_m_err                        /* N/E切替中 */
    },
    /* (05)REカード診断要求                                               */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_use_recrddia_sta,          /* 運用中    */
        f_trp_rec_FcFF_m_err                        /* N/E切替中 */
    },
    /* (06)RE PORT 制御要求                                               */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_use_reprtctl_sta,          /* 運用中    */
        f_trp_rec_FcFF_m_err                        /* N/E切替中 */
    },
    /* (07)バージョン報告要求                                             */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_use_rever_sta,             /* 運用中    */
        f_trp_rec_FcFF_m_err                        /* N/E切替中 */
    },
    /* (08)CPRI信号受信通知(REカード制御応答)                             */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_use_recrdctl_end,          /* 運用中    */
        f_trp_rec_FcFF_m_nop                        /* N/E切替中 */
    },
    /* (09)CPRI信号受信通知(REカード診断応答)                             */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_use_recrddia_end,          /* 運用中    */
        f_trp_rec_FcFF_m_nop                        /* N/E切替中 */
    },
    /* (10)CPRI信号受信通知(RE PORT 制御応答)                             */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_use_reprtctl_end,          /* 運用中    */
        f_trp_rec_FcFF_m_nop                        /* N/E切替中 */
    },
    /* (11)CPRI信号受信通知(REバージョン報告応答)                         */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_use_rever_end,             /* 運用中    */
        f_trp_rec_FcFF_m_nop                        /* N/E切替中 */
    },
    /* (12)タイムアウト発生通知(REカード制御確認タイマ)                   */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_use_recrdctl_stp,          /* 運用中    */
        f_trp_rec_FcFF_m_nop                        /* N/E切替中 */
    },
    /* (13)タイムアウト発生通知(REカード診断確認タイマ)                   */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_use_recrddia_stp,          /* 運用中    */
        f_trp_rec_FcFF_m_nop                        /* N/E切替中 */
    },
    /* (14)タイムアウト発生通知(RE PORT 制御確認タイマ)                   */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_use_reprtctl_stp,          /* 運用中    */
        f_trp_rec_FcFF_m_nop                        /* N/E切替中 */
    },
    /* (15)タイムアウト発生通知(REバージョン報告確認タイマ)               */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_use_rever_stp,             /* 運用中    */
        f_trp_rec_FcFF_m_nop                        /* N/E切替中 */
    },
    /* (16)N/E切替開始要求                                                */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_nes_sta,                   /* 運用中    */
        f_trp_rec_FcFF_m_err                        /* N/E切替中 */
    },
    /* (17)N/E切替完了通知                                                */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_err,                       /* 運用中    */
        f_trp_rec_FcFF_m_nes_end                    /* N/E切替中 */
    },
    /* (18)N/E間データ引継ぎ開始通知                                      */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_suc_sta,                   /* 運用中    */
        f_trp_rec_FcFF_m_nop                        /* N/E切替中 */
    },
    /* (19)N/E間データ引継ぎ完了通知                                      */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_err,                       /* 運用中    */
        f_trp_rec_FcFF_m_suc_end                    /* N/E切替中 */
    },
    /* (20)RE status collect request (N/E Switch)                         */
    {
        f_trp_rec_FcFF_m_err,                       /* 初期      */
        f_trp_rec_FcFF_m_use_restsclt_exe,          /* 運用中    */
        f_trp_rec_FcFF_m_nes_restsclt_exe,          /* N/E切替中 */
     }
};
#endif

/* レジスタアドレステーブル */
T_REC_CRA_TBL f_trp_rec_Rcom_cra_tbl = {{
//    {
//        (UINT*)D_REC_CRA_DOF_CPR01, /* (00)上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI01) */
//        (UINT*)D_REC_CRA_DOF_CPR02, /* (00)上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI02) */
//        (UINT*)D_REC_CRA_DOF_CPR03, /* (00)上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI03) */
//        (UINT*)D_REC_CRA_DOF_CPR04, /* (00)上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI04) */
//        (UINT*)D_REC_CRA_DOF_CPR05, /* (00)上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI05) */
//        (UINT*)D_REC_CRA_DOF_CPR06  /* (00)上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI06) */
//    },
//    {
//        (UINT*)D_REC_CRA_T14_CPR01, /* (01)T14測定値(CPRI01) */
//        (UINT*)D_REC_CRA_T14_CPR02, /* (01)T14測定値(CPRI02) */
//        (UINT*)D_REC_CRA_T14_CPR03, /* (01)T14測定値(CPRI03) */
//        (UINT*)D_REC_CRA_T14_CPR04, /* (01)T14測定値(CPRI04) */
//        (UINT*)D_REC_CRA_T14_CPR05, /* (01)T14測定値(CPRI05) */
//        (UINT*)D_REC_CRA_T14_CPR06  /* (01)T14測定値(CPRI06) */
//    },
//    {
//        (UINT*)D_REC_CRA_UDL_CPR01, /* (02)上り遅延補正値(CPRI01) */
//        (UINT*)D_REC_CRA_UDL_CPR02, /* (02)上り遅延補正値(CPRI02) */
//        (UINT*)D_REC_CRA_UDL_CPR03, /* (02)上り遅延補正値(CPRI03) */
//        (UINT*)D_REC_CRA_UDL_CPR04, /* (02)上り遅延補正値(CPRI04) */
//        (UINT*)D_REC_CRA_UDL_CPR05, /* (02)上り遅延補正値(CPRI05) */
//        (UINT*)D_REC_CRA_UDL_CPR06  /* (02)上り遅延補正値(CPRI06) */
//    },
//    {
//        (UINT*)D_REC_CRA_DDL_CPR01, /* (03)下り遅延補正値(CPRI01) */
//        (UINT*)D_REC_CRA_DDL_CPR02, /* (03)下り遅延補正値(CPRI02) */
//        (UINT*)D_REC_CRA_DDL_CPR03, /* (03)下り遅延補正値(CPRI03) */
//        (UINT*)D_REC_CRA_DDL_CPR04, /* (03)下り遅延補正値(CPRI04) */
//        (UINT*)D_REC_CRA_DDL_CPR05, /* (03)下り遅延補正値(CPRI05) */
//        (UINT*)D_REC_CRA_DDL_CPR06  /* (03)下り遅延補正値(CPRI06) */
//    },
//    {
//        (UINT*)D_REC_CRA_CRR_CPR01, /* (04)キャリアON/OFF設定(CPRI01) */
//        (UINT*)D_REC_CRA_CRR_CPR02, /* (04)キャリアON/OFF設定(CPRI02) */
//        (UINT*)D_REC_CRA_CRR_CPR03, /* (04)キャリアON/OFF設定(CPRI03) */
//        (UINT*)D_REC_CRA_CRR_CPR04, /* (04)キャリアON/OFF設定(CPRI04) */
//        (UINT*)D_REC_CRA_CRR_CPR05, /* (04)キャリアON/OFF設定(CPRI05) */
//        (UINT*)D_REC_CRA_CRR_CPR06  /* (04)キャリアON/OFF設定(CPRI06) */
//    },
    {
          (UINT*)D_REC_CRA_RFR_CPR01, /* (05)RE強制リセット(CPRI01) */
          (UINT*)D_REC_CRA_RFR_CPR02, /* (05)RE強制リセット(CPRI02) */
          (UINT*)D_REC_CRA_RFR_CPR03, /* (05)RE強制リセット(CPRI03) */
          (UINT*)D_REC_CRA_RFR_CPR04, /* (05)RE強制リセット(CPRI04) */
          (UINT*)D_REC_CRA_RFR_CPR05, /* (05)RE強制リセット(CPRI05) */
          (UINT*)D_REC_CRA_RFR_CPR06,  /* (05)RE強制リセット(CPRI06) */
    	  (UINT*)D_REC_CRA_RFR_CPR07, /* (05)RE強制リセット(CPRI07) */
          (UINT*)D_REC_CRA_RFR_CPR08, /* (05)RE強制リセット(CPRI08) */
          (UINT*)D_REC_CRA_RFR_CPR09, /* (05)RE強制リセット(CPRI09) */
          (UINT*)D_REC_CRA_RFR_CPR10, /* (05)RE強制リセット(CPRI10) */
          (UINT*)D_REC_CRA_RFR_CPR11, /* (05)RE強制リセット(CPRI11) */
          (UINT*)D_REC_CRA_RFR_CPR12, /* (05)RE強制リセット(CPRI12) */
    	  (UINT*)D_REC_CRA_RFR_CPR13, /* (05)RE強制リセット(CPRI13) */
          (UINT*)D_REC_CRA_RFR_CPR14, /* (05)RE強制リセット(CPRI14) */
          (UINT*)D_REC_CRA_RFR_CPR15, /* (05)RE強制リセット(CPRI15) */
          (UINT*)D_REC_CRA_RFR_CPR16 /* (05)RE強制リセット(CPRI16) */
    }//,
//    {
//        (UINT*)D_REC_CRA_BAB_CPR01, /* (06)帯域／系統数設定(CPRI01) */
//        (UINT*)D_REC_CRA_BAB_CPR02, /* (06)帯域／系統数設定(CPRI02) */
//        (UINT*)D_REC_CRA_BAB_CPR03, /* (06)帯域／系統数設定(CPRI03) */
//        (UINT*)D_REC_CRA_BAB_CPR04, /* (06)帯域／系統数設定(CPRI04) */
//        (UINT*)D_REC_CRA_BAB_CPR05, /* (06)帯域／系統数設定(CPRI05) */
//        (UINT*)D_REC_CRA_BAB_CPR06  /* (06)帯域／系統数設定(CPRI06) */
//#if !defined(OPT_CPRI_L2HARD)
//    },
//        {
//        (UINT*)D_REC_CRA_DLY_CPR01, /* (07)CA対応 RE送信フレーム位相調整値設定(CPRI01) */
//        (UINT*)D_REC_CRA_DLY_CPR02, /* (07)CA対応 RE送信フレーム位相調整値設定(CPRI02) */
//        (UINT*)D_REC_CRA_DLY_CPR03, /* (07)CA対応 RE送信フレーム位相調整値設定(CPRI03) */
//        (UINT*)D_REC_CRA_DLY_CPR04, /* (07)CA対応 RE送信フレーム位相調整値設定(CPRI04) */
//        (UINT*)D_REC_CRA_DLY_CPR05, /* (07)CA対応 RE送信フレーム位相調整値設定(CPRI05) */
//        (UINT*)D_REC_CRA_DLY_CPR06  /* (07)CA対応 RE送信フレーム位相調整値設定(CPRI06) */
//#endif
//    }
}};


/* レジスタアドレステーブル */
T_REC_NRA_TBL f_trp_rec_Rcom_nra_tbl = {
    {
        (UINT*)D_REC_NRA_ERS,       /* ERR設定               */
        (UINT*)D_REC_NRA_ERC,       /* ERR解除               */
        (UINT*)D_REC_NRA_CRD,       /* カード実装新状態表示  */
        (UINT*)D_REC_NRA_MON,       /* T14割り込みマスクON   */
#if !defined(OPT_CPRI_L2HARD)
        (UINT*)D_REC_NRA_MOF        /* T14割り込みマスクOFF  */
#else
        (UINT*)D_REC_NRA_MOF,       /* T14割り込みマスクOFF  */
        (UINT*)D_CMM_FPGA_ADDR_T14_INT_LTE,					/**< NO.76 CPRIリンク#1-#2 T14測定値変化通知レジスタ（LTE）               [0x00101100]	*/
        (UINT*)D_CMM_FPGA_ADDR_MAIN_INT_MASK_OFF_4_2_ENB	/**< NO.75 SLOT4_2(TRXIF(LTE))代表割込み要因マスクOFF レジスタ(eNB-CNT用) [0x00101008]	*/
#endif
    }
};

#if 0
/* ERR要因テーブル */
T_REC_ERF_TBL f_trp_rec_Rcom_erf_tbl = {{
    {
        D_REC_ERF_DLY_CPR01,
        D_REC_ERF_DLY_CPR02,
        D_REC_ERF_DLY_CPR03,
        D_REC_ERF_DLY_CPR04,
        D_REC_ERF_DLY_CPR05,
        D_REC_ERF_DLY_CPR06
    },
    {
        D_REC_ERF_STP_CPR01,
        D_REC_ERF_STP_CPR02,
        D_REC_ERF_STP_CPR03,
        D_REC_ERF_STP_CPR04,
        D_REC_ERF_STP_CPR05,
        D_REC_ERF_STP_CPR06
    }
}};

#endif /* #if 0 */

/**
 * @brief タイマ種別→通知種別変換テーブル
 * @date  2015/09/20 TDI)satou ET_REC_TMKの種類に合わせて見直し
 */
T_REC_TMK_NTC_TBL f_trp_rec_Rcom_tmk_ntc_tbl = {{
    {BPF_RU_HRTM_REG_PERIODIC}, /*!< (00)RE時刻設定送信間隔タイマ                 */
    {BPF_RU_HRTM_REG_PERIODIC}, /*!< (01)Toffset値報告送信間隔タイマ              */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (02)Toffset値報告確認タイマ                  */
    {BPF_RU_HRTM_REG_PERIODIC}, /*!< (03)RE装置構成情報報告送信間隔タイマ         */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (04)RE装置構成情報報告確認タイマ             */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (05)REスロット情報報告確認タイマ             */
    {BPF_RU_HRTM_REG_PERIODIC}, /*!< (06)RE状態報告送信間隔タイマ                 */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (07)RE状態報告確認タイマ                     */
    {BPF_RU_HRTM_REG_PERIODIC}, /*!< (08)RE PORT状態報告送信間隔タイマ            */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (09)RE PORT状態報告確認タイマ                */
    {BPF_RU_HRTM_REG_PERIODIC}, /*!< (10)REカード状態報告(セクタ部)送信間隔タイマ */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (11)REカード状態報告(セクタ部)確認タイマ     */
    {BPF_RU_HRTM_REG_PERIODIC}, /*!< (12)REカード状態報告(共通部)送信間隔タイマ   */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (13)REカード状態報告(共通部)確認タイマ       */
    {BPF_RU_HRTM_REG_PERIODIC}, /*!< (14)REキャリア状態報告送信間隔タイマ         */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (15)REキャリア状態報告確認タイマ             */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (16)REリセット確認タイマ                     */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (17)RE強制リセット確認タイマ                 */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (18)二次リセット待ちタイマ                   */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (19)Active切替指示送信待ちタイマ             */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (20)遅延補正値設定待ちタイマ                 */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (21)TRX設定確認タイマ                        */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (22)TRX解放確認タイマ                        */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (23)無変調キャリアON/OFF指定確認タイマ       */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (24)REバージョン報告確認タイマ               */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (25)REカード制御確認タイマ                   */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (26)REカード診断確認タイマ                   */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (27)RE PORT制御確認タイマ                    */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (28)RE 障害ログ取得確認タイマ                */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (29)RE 障害ログ中止確認タイマ                */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (30)RE 障害ログ送信確認タイマ                */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (31)RE File update Confirm     */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (32)RE auto reset waiting     */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (33)Active切替指示送信待ちタイマ(M-LRE2)     */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (34)Active切替指示送信待ちタイマ(M-LRE3)     */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (35)Active切替指示送信待ちタイマ(M-LRE4)     */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (36)Active切替指示送信待ちタイマ(M-LRE5)     */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (37)Active切替指示送信待ちタイマ(M-LRE6)     */
    {BPF_RU_HRTM_REG_ONCE    }, /*!< (38)Active切替指示送信待ちタイマ(M-LRE7)     */
    {BPF_RU_HRTM_REG_ONCE    }  /*!< (39)MTアドレス設定確認タイマ                 */
}};

#if 0
T_REC_RESLTFB_TBL f_trp_rec_Rcom_sltfbinf_tbl ={{
    {D_REC_FBTYP_DUP,       CMD_CRDFNBIT_DUP   },
    {D_REC_FBTYP_LNA,       CMD_CRDFNBIT_LNA   },
    {D_REC_FBTYP_TPA,       CMD_CRDFNBIT_TPA   },
    {D_REC_FBTYP_TRX,       CMD_CRDFNBIT_TRX   },
    {D_REC_FBTYP_TRXINF,    CMD_CRDFNBIT_TRXIF },
    {D_REC_FBTYP_OARAINF,   CMD_CRDFNBIT_OARA  },
    {D_REC_FBTYP_TILTINF,   CMD_CRDFNBIT_TILT  },
    {D_REC_FBTYP_PORTINF,   CMD_CRDFNBIT_PROT  },
    {D_REC_FBTYP_MTINF,     CMD_CRDFNBIT_MT    },
    {D_REC_FBTYP_3GRFINF,   CMD_CRDFNBIT_3GRF  },
    {D_REC_FBTYP_EXOARAINF, CMD_CRDFNBIT_EXOARA},
    {D_REC_FBTYP_CLKINF,    CMD_CRDFNBIT_CLK   }
}};

#endif

/* 機能部の故障状態の組み合テーブル */
T_REC_REFBTRB_TBL f_trp_rec_Rcom_fbtrbsts_tbl = {{
    CMD_NODATA         ,      /*!< (00) Uninstall   Normal   High-ALM   SYS-ALM    */
    D_REC_FBTRB_SYSALM ,      /*!< (01) Uninstall:0 Normal:0 High-ALM:0 SYS-ALM:1  */
    D_REC_FBTRB_HIGHALM,      /*!< (02) Uninstall:0 Normal:0 High-ALM:1 SYS-ALM:0  */
    D_REC_FBTRB_SYSALM ,      /*!< (03) Uninstall:0 Normal:0 High-ALM:1 SYS-ALM:1  */
    D_REC_FBTRB_NORMAL ,      /*!< (04) Uninstall:0 Normal:1 High-ALM:0 SYS-ALM:0  */
    D_REC_FBTRB_SYSALM ,      /*!< (05) Uninstall:0 Normal:1 High-ALM:0 SYS-ALM:1  */
    D_REC_FBTRB_HIGHALM,      /*!< (06) Uninstall:0 Normal:1 High-ALM:1 SYS-ALM:0  */
    D_REC_FBTRB_SYSALM ,      /*!< (07) Uninstall:0 Normal:1 High-ALM:1 SYS-ALM:1  */
    D_REC_FBTRB_UNMOUNT,      /*!< (08) Uninstall:1 Normal:0 High-ALM:0 SYS-ALM:0  */
    D_REC_FBTRB_SYSALM ,      /*!< (09) Uninstall:1 Normal:0 High-ALM:0 SYS-ALM:1  */
    D_REC_FBTRB_SYSALM ,      /*!< (10) Uninstall:1 Normal:0 High-ALM:1 SYS-ALM:0  */
    D_REC_FBTRB_SYSALM ,      /*!< (11) Uninstall:1 Normal:0 High-ALM:1 SYS-ALM:1  */
    D_REC_FBTRB_SYSALM ,      /*!< (12) Uninstall:1 Normal:1 High-ALM:0 SYS-ALM:0  */
    D_REC_FBTRB_SYSALM ,      /*!< (13) Uninstall:1 Normal:1 High-ALM:0 SYS-ALM:1  */
    D_REC_FBTRB_SYSALM ,      /*!< (14) Uninstall:1 Normal:1 High-ALM:1 SYS-ALM:0  */
    D_REC_FBTRB_SYSALM        /*!< (15) Uninstall:1 Normal:1 High-ALM:1 SYS-ALM:1  */
}};

#if 0

T_REC_CRRBRCSTS_TBL f_trp_rec_Rcom_crrbrcsts_tbl = {{
    {
        {CMD_ON, CMD_OFF, CMD_OFF, CMD_OFF},
        {CMD_ON, CMD_ON,  CMD_OFF, CMD_OFF},
        {CMD_ON, CMD_ON,  CMD_OFF, CMD_OFF},
        {CMD_ON, CMD_ON,  CMD_OFF, CMD_OFF}
    },
    {
        {CMD_ON, CMD_OFF, CMD_OFF, CMD_OFF},
        {CMD_ON, CMD_ON,  CMD_OFF, CMD_OFF},
        {CMD_ON, CMD_ON,  CMD_ON,  CMD_OFF},
        {CMD_ON, CMD_ON,  CMD_ON,  CMD_ON }
    }
}};
#endif

/* ログテーブルテーブル */
T_REC_LOGTBL_TBL f_trp_rec_Rcom_logtbl_tbl = {{
    {"DMP00", "f_trp_rec_Wcom_remsc_tbl",      (VOID*)&f_trp_rec_Wcom_remsc_tbl,      sizeof(f_trp_rec_Wcom_remsc_tbl)},
    {"DMP01", "f_trp_rec_Wcom_remcm_tbl",      (VOID*)&f_trp_rec_Wcom_remcm_tbl,      sizeof(f_trp_rec_Wcom_remcm_tbl)},
    {"DMP03", "f_trp_rec_Wc02_stsmng_tbl",     (VOID*)&f_trp_rec_Wc02_stsmng_tbl,     sizeof(f_trp_rec_Wc02_stsmng_tbl)},
    {"DMP04", "f_trp_rec_Wc03_stsmng_tbl",     (VOID*)&f_trp_rec_Wc03_stsmng_tbl,     sizeof(f_trp_rec_Wc03_stsmng_tbl)},
    {"DMP09", "f_trp_rec_Wc02_prm_tbl",        (VOID*)&f_trp_rec_Wc02_prm_tbl,        sizeof(f_trp_rec_Wc02_prm_tbl)},
    {"DMP10", "f_trp_rec_Wc03_prm_tbl",        (VOID*)&f_trp_rec_Wc03_prm_tbl,        sizeof(f_trp_rec_Wc03_prm_tbl)},
//    {"DMP14", "f_trp_rec_Wcom_nestsmng_tbl",   (VOID*)&f_trp_rec_Wcom_nestsmng_tbl,   sizeof(f_trp_rec_Wcom_nestsmng_tbl)},
//    {"DMP15", "f_trp_rec_Wcom_txstsmng_tbl",   (VOID*)&f_trp_rec_Wcom_txstsmng_tbl,   sizeof(f_trp_rec_Wcom_txstsmng_tbl)},
//    {"DMP16", "f_trp_rec_Wcom_ampslt_cpr_tbl", (VOID*)&f_trp_rec_Wcom_ampslt_cpr_tbl, sizeof(f_trp_rec_Wcom_ampslt_cpr_tbl)},
//    {"DMP17", "f_trp_rec_Wcom_cpr_amp_tbl",    (VOID*)&f_trp_rec_Wcom_cpr_amp_tbl,    sizeof(f_trp_rec_Wcom_cpr_amp_tbl)},
    {"DMP18", "f_trp_rec_Wcom_tmk_val_tbl",    (VOID*)&f_trp_rec_Wcom_tmk_val_tbl,    sizeof(f_trp_rec_Wcom_tmk_val_tbl)},
    {"DMP19", "f_trp_rec_Wcom_msgrcv_tbl",     (VOID*)&f_trp_rec_Wcom_msgrcv_tbl,     sizeof(f_trp_rec_Wcom_msgrcv_tbl)},
    {"DMP20", "f_trp_rec_Wcom_timmng_tbl",     (VOID*)&f_trp_rec_Wcom_timmng_tbl,     sizeof(f_trp_rec_Wcom_timmng_tbl)},
    {"SDP21", "f_trp_rec_Wcom_runhis_tbl",     (VOID*)&f_trp_rec_Wcom_runhis_tbl,     sizeof(f_trp_rec_Wcom_runhis_tbl)},/* SDP = Simple Dump */
    {"DMP22", "f_trp_rec_Wcom_reghis_tbl",     (VOID*)&f_trp_rec_Wcom_reghis_tbl,     sizeof(f_trp_rec_Wcom_reghis_tbl)},
    {"DMP23", "f_trp_rec_Wcom_ivlprm_tbl",     (VOID*)&f_trp_rec_Wcom_ivlprm_tbl,     sizeof(f_trp_rec_Wcom_ivlprm_tbl)}
}};
#if 0
#if defined(OPT_CPRI_L2HARD)
UINT crr_reAutoCntReg[CMD_LINK_NUM_T][CMD_SLOTINF_MAX] =
{
	{	/* CPRI#1 */
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE1,
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE2,
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE3,
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE4,
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE5,
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE6,
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE7,
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE8,
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE9,
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE10,
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE11,
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE12,
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE13,
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE14,
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE15,
		D_CMM_FPGA_ADDR_CPRI_1_RE_CTR_LTE16,
	},
	{	/* CPRI#2 */
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE1,
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE2,
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE3,
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE4,
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE5,
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE6,
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE7,
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE8,
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE9,
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE10,
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE11,
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE12,
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE13,
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE14,
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE15,
		D_CMM_FPGA_ADDR_CPRI_2_RE_CTR_LTE16,
	},
	{	/* CPRI#3 */
		CMD_NUM0,CMD_NUM0,CMD_NUM0,CMD_NUM0,CMD_NUM0,CMD_NUM0
	},
	{	/* CPRI#4 */
		CMD_NUM0,CMD_NUM0,CMD_NUM0,CMD_NUM0,CMD_NUM0,CMD_NUM0
	},
	{	/* CPRI#5 */
		CMD_NUM0,CMD_NUM0,CMD_NUM0,CMD_NUM0,CMD_NUM0,CMD_NUM0
	},
	{	/* CPRI#6 */
		CMD_NUM0,CMD_NUM0,CMD_NUM0,CMD_NUM0,CMD_NUM0,CMD_NUM0
	},
};
#endif

///* f_trp_rec_Rcom_fbType2CmnSecInfo_tbl */
//const	T_REC_FBTYPE2CMNSECINFO_TBL	f_trp_rec_Rcom_fbType2CmnSecInfo_tbl[29] = {
//	/* 0：DUP */
//	{
//		D_REC_FBTYP_SEC,									/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_DUP,									/**< Function Block Index  */
//	},
//	/* 1：LNA */
//	{
//		D_REC_FBTYP_SEC,									/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_LNA,									/**< Function Block Index  */
//	},
//	/* 2：T-PA */
//	{
//		D_REC_FBTYP_SEC,									/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_TPA,									/**< Function Block Index  */
//	},
//	/* 3：TRX */
//	{
//		D_REC_FBTYP_SEC,									/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_TRX,									/**< Function Block Index  */
//	},
//	/* 4：TRX-INF */
//	{
//		D_REC_FBTYP_SEC,									/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_TRXINF,									/**< Function Block Index  */
//	},
//	/* 5：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 6：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 7：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 8：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 9：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 10：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 11：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 12：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 13：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 14：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 15：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 16：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 17：OA-RA-INF */
//	{
//		D_REC_FBTYP_CMN,									/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_OARAINF,								/**< Function Block Index  */
//	},
//	/* 18：TILT-INF */
//	{
//		D_REC_FBTYP_CMN,									/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_TILTINF,								/**< Function Block Index  */
//	},
//	/* 19：PORT-INF */
//	{
//		D_REC_FBTYP_CMN,									/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_PORTINF,								/**< Function Block Index  */
//	},
//	/* 20：MT-INF */
//	{
//		D_REC_FBTYP_CMN,									/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_MTIF,									/**< Function Block Index  */
//	},
//	/* 21：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 22：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 23：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 24：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//	/* 25：3GRF-INF */
//	{
//		D_REC_FBTYP_SEC,									/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_3GRFINF,								/**< Function Block Index  */
//	},
//	/* 26：EX-OA-RA-INF */
//	{
//		D_REC_FBTYP_CMN,									/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_EXOARAINF,								/**< Function Block Index  */
//	},
//	/* 27：CLK-INF */
//	{
//		D_REC_FBTYP_CMN,									/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_CLKINF,									/**< Function Block Index  */
//	},
//	/* 28：- */
//	{
//		D_REC_FBTYP_INVALID,								/**< Function Block Common Sector Type   */
//		D_REC_FBIDX_INVALID,								/**< Function Block Index  */
//	},
//};

/* f_trp_rec_Rcom_reFbTrbStsCnv_tbl */
const	USHORT	f_trp_rec_Rcom_reFbTrbStsCnv_tbl[32] = {
	D_REC_FBTRB_NORMAL,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:OFF  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:ON  */
	D_REC_FBTRB_NORMAL,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:OFF  */
	D_REC_FBTRB_NORMAL,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:ON  */
	D_REC_FBTRB_HIGHALM,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:OFF  */
	D_REC_FBTRB_HIGHALM,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:ON  */
	D_REC_FBTRB_HIGHALM,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:OFF  */
	D_REC_FBTRB_HIGHALM,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:ON  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:OFF  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:ON  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:OFF  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:ON  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:OFF  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:ON  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:OFF  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:ON  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:OFF  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:ON  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:OFF  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:ON  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:OFF  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:ON  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:OFF  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:ON  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:OFF  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:ON  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:OFF  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:ON  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:OFF  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:ON  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:OFF  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:ON  */
};

/* f_trp_rec_Rcom_reSecFbTrbStsCnv_tbl */
const	USHORT	f_trp_rec_Rcom_reSecFbTrbStsCnv_tbl[32] = {
	D_REC_FBTRB_INVALID,									/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:OFF  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:ON  */
	D_REC_FBTRB_NORMAL,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:OFF  */
	D_REC_FBTRB_NORMAL,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:ON  */
	D_REC_FBTRB_HIGHALM,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:OFF  */
	D_REC_FBTRB_HIGHALM,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:ON  */
	D_REC_FBTRB_HIGHALM,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:OFF  */
	D_REC_FBTRB_HIGHALM,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:ON  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:OFF  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:ON  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:OFF  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:ON  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:OFF  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:ON  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:OFF  */
	D_REC_FBTRB_SYSALM,										/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:ON  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:OFF  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:ON  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:OFF  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:ON  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:OFF  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:ON  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:OFF  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:ON  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:OFF  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:OFF Normal:OFF NG:ON  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:OFF  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:OFF Normal:ON NG:ON  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:OFF  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:OFF NG:ON  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:OFF  */
	D_REC_FBTRB_UNMOUNT,									/**< Unmount: SYS-ALM: High-ALM:ON Normal:ON NG:ON  */
};

/* f_trp_rec_Rcom_fbTrbSts2Bit_tbl */
const	USHORT	f_trp_rec_Rcom_fbTrbSts2Bit_tbl[5] = {
	D_REC_FBTRB_BIT_UNMOUNT,								/**< Unmount  */
	D_REC_FBTRB_BIT_NORMAL,									/**< Normal  */
	D_REC_FBTRB_BIT_HIGHALM,								/**< High-ALM  */
	D_REC_FBTRB_BIT_SYSALM,									/**< SYS-ALM  */
	D_REC_FBTRB_BIT_NG,										/**< NG  */
};

/* 機能部種別→Bit変換テーブル */
const USHORT f_trp_rec_Rcom_fbinfbit_tbl[28] = {
    CMD_CRDFNBIT_DUP,         /*  0：DUP */
    CMD_CRDFNBIT_LNA,         /*  1：LNA */
    CMD_CRDFNBIT_TPA,         /*  2：T-PA */
    CMD_CRDFNBIT_TRX,         /*  3：TRX */
    CMD_CRDFNBIT_TRXIF,       /*  4：TRX-INF */
    CMD_NUM0,                 /*  5：- */
    CMD_NUM0,                 /*  6：- */
    CMD_NUM0,                 /*  7：- */
    CMD_NUM0,                 /*  8：- */
    CMD_NUM0,                 /*  9：- */
    CMD_NUM0,                 /* 10：- */
    CMD_NUM0,                 /* 11：- */
    CMD_NUM0,                 /* 12：- */
    CMD_NUM0,                 /* 13：- */
    CMD_NUM0,                 /* 14：- */
    CMD_NUM0,                 /* 15：- */
    CMD_NUM0,                 /* 16：- */
    CMD_CRDFNBIT_OARA,        /* 17：OA-RA-INF */
    CMD_CRDFNBIT_TILT,        /* 18：TILT-INF */
    CMD_CRDFNBIT_PROT,        /* 19：PORT-INF */
    CMD_CRDFNBIT_MT,          /* 20：MT-INF */
    CMD_NUM0,                 /* 21：- */
    CMD_NUM0,                 /* 22：- */
    CMD_NUM0,                 /* 23：- */
    CMD_NUM0,                 /* 24：- */
    CMD_CRDFNBIT_3GRF,        /* 25：3GRF-INF */
    CMD_CRDFNBIT_EXOARA,      /* 26：EX-OA-RA-INF */
    CMD_CRDFNBIT_CLK,         /* 27：CLK-INF */
};

/* ブランチ番号→Bit変換テーブル */
const	USHORT	f_trp_rec_Rcom_brcinf_tbl[4] = {
	D_REC_BRCINF_BIT_0,										/**< Brc No.0  */
	D_REC_BRCINF_BIT_1,										/**< Brc No.1  */
	D_REC_BRCINF_BIT_2,										/**< Brc No.2  */
	D_REC_BRCINF_BIT_3,										/**< Brc No.3  */
};


/****************************************************************************/
/*
 */
/****************************************************************************/
#endif /* #if 0 */
/* @} */  /* group TRIF_REC */
