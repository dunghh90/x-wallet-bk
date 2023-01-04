/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   fheth_hw.h
 *  @brief  Definitions for 25G ethernet hw
 *  @date   2018/03/06 KCN)satou
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2018
 */
/********************************************************************************************************************/

#ifndef __FHETH_HW_H_
#define __FHETH_HW_H_


/* FPGA Regster */
#define FPGA_BASE                       (0xA0000000) /* FPGAレジスタベースアドレス */


#define CTB_RST                         (0x00000040) /* 起動(Reset)制御レジスタオフセット */
#define CTB_RST_ETHPLL                  (0x00000010) /* Ether PLL Reset */
#define CTB_RST_ETHLOG                  (0x00000020) /* Ether Logic Reset */

#define CTB_STA1                        (0x00000200) /* FPGA状態表示レジスタオフセット */
#define CTB_STA1_ETHPULCK               (0x00000400) /* ETH PLL UnLock */

#define CTB_SFPPSSD                     (0x00000028) /* SFP LDO ShutDown制御レジスタオフセット */

/* 25G Ethernet Regster BASE Adress */
#define E25G_PORT1_BASE                 (0xA1000000) /* ポート１レジスタベースアドレス */
#define E25G_PORT2_BASE                 (0xA1001000) /* ポート２レジスタベースアドレス */

/* 送信バッファ BASE Adress */
#define E25G_PORT1_SPLANE_TX_BUFF       (0xA1010000) /* ポート１ S-plane送信バッファ */
#define E25G_PORT1_MPLANE_TX_BUFF       (0xA1010800) /* ポート１ M-plane送信バッファ */
#define E25G_PORT2_SPLANE_TX_BUFF       (0xA1011000) /* ポート２ S-plane送信バッファ */
#define E25G_PORT2_MPLANE_TX_BUFF       (0xA1011800) /* ポート２ M-plane送信バッファ */

/* 受信バッファ BASE Adress */
#define E25G_PORT1_SPLANE_RX_BUFF       (0x7FF00000) /* ポート１ S-plane受信バッファ */
#define E25G_PORT2_SPLANE_RX_BUFF       (0x7FF80000) /* ポート２ S-plane受信バッファ */
#define E25G_SPLANE_RX_SEC1             (0x00000000) /* S-plane受信タイムスタンプ(秒上位) */
#define E25G_SPLANE_RX_SEC2             (0x00000004) /* S-plane受信タイムスタンプ(秒下位) */
#define E25G_SPLANE_RX_NSEC             (0x00000008) /* S-plane受信タイムスタンプ(n秒) */
#define E25G_SPLANE_RX_FLAG             (0x0000000C) /* S-plane受信フラグオフセット */

#define E25G_PORT1_MPLANE_RX_BUFF       (0x7FF40000) /* ポート１ M-plane受信バッファ */
#define E25G_PORT2_MPLANE_RX_BUFF       (0x7FFC0000) /* ポート２ M-plane受信バッファ */
#define E25G_MPLANE_RX_FLAG             (0x0000008C) /* M-plane受信フラグオフセット */

/* Regster  */
/* MACシステムリセットレジスタ */
#define E25G_MACSYSRST                  (0x00000000) /* レジスタオフセット */

#define E25G_MACSYSRST_RST              (0x00000001) /* システムリセット */

/* MACリセットレジスタ */
#define E25G_MACRST                     (0x00000004) /* レジスタオフセット */

#define E25G_MACRST_GTEYESCAN           (0x10000000) /* Eyescanリセット */
#define E25G_MACRST_GTRXBUF             (0x01000000) /* 受信バッファ(ES)リセット */
#define E25G_MACRST_GTRXDFELPM          (0x00100000) /* 受信DEFリセット */
#define E25G_MACRST_GTRXPCS             (0x00020000) /* 受信PCSリセット */
#define E25G_MACRST_GTRXPMA             (0x00010000) /* 受信PMAリセット */
#define E25G_MACRST_GTRXPRBSCNT         (0x00001000) /* PRBSエラーカウンタリセット */
#define E25G_MACRST_GTTXPCS             (0x00000200) /* 送信PCSリセット */
#define E25G_MACRST_GTTXPMA             (0x00000100) /* 送信PMAリセット */
#define E25G_MACRST_GTWIZTXDATAPATH     (0x00000020) /* 送信データパスリセット */
#define E25G_MACRST_GTWIZRXDATAPATH     (0x00000010) /* 受信データパスリセット */
#define E25G_MACRST_TX                  (0x00000002) /* 送信リセット */
#define E25G_MACRST_RX                  (0x00000001) /* 受信リセット */

/* MACリセット完了レジスタ */
#define E25G_RSTDONE                    (0x00000010) /* レジスタオフセット */

#define E25G_RSTDONE_RX                 (0x00000010) /* 受信リセット処理終了 */
#define E25G_RSTDONE_TX                 (0x00000001) /* 送信リセット処理終了 */

/* MAC受信制御レジスタ */
#define E25G_RXCTL                      (0x00000020) /* レジスタオフセット */

#define E25G_RXCTL_RXEN                 (0x00000001) /* 受信イネーブル */

/* MAC送信制御レジスタ */
#define E25G_TXCTL                      (0x00000040) /* レジスタオフセット */

#define E25G_TXCTL_SENDIDLE             (0x01000000) /* アイドルワード送信 */
#define E25G_TXCTL_SENDRFI              (0x00100000) /* RFIワード送信 */
#define E25G_TXCTL_SENDLFI              (0x00010000) /* LFIコードワード送信 */
#define E25G_TXCTL_FCSINS               (0x00000010) /* TXのFCS挿入イネーブル */
#define E25G_TXCTL_TXEN                 (0x00000001) /* 送信イネーブル */

/* U-Plane送信制御レジスタ */
#define E25G_UTXEN                      (0x00000044) /* レジスタオフセット */

#define E25G_UTXEN_UTXEN                (0x00000001) /* U-Plane送信イネーブル */

/* RS FEC制御レジスタ */
#define E25G_RSFECCTL                   (0x00000060) /* レジスタオフセット */

#define E25G_RSFECCTL_ERRIND            (0x00010000) /* RS-FECのIEEE　RS-FEC準拠設定 */
#define E25G_RSFECCTL_ENCOR             (0x00001000) /* RS-FEC訂正イネーブル */
#define E25G_RSFECCTL_ENIND             (0x00000100) /* RS-FEC訂正エラー表示イネーブル */
#define E25G_RSFECCTL_25GMD             (0x00000010) /* IEEE108/25G EthernetConsortium モードを切換 */
#define E25G_RSFECCTL_EN                (0x00000001) /* RS-FECイネーブル */

/* FEC制御レジスタ */
#define E25G_FECCTL                     (0x00000064) /* レジスタオフセット */

#define E25G_FECCTL_EERTOPCS            (0x00000100) /* clause 74 FEC PCS送信イネーブル */
#define E25G_FECCTL_TXEN                (0x00000010) /* clause 74 FEC送信イネーブル */
#define E25G_FECCTL_RXEN                (0x00000001) /* clause 74 FEC受信イネーブル */

/* PTP制御レジスタ */
#define E25G_PTPCTL                     (0x00000080) /* レジスタオフセット */

#define E25G_PTPCTL_LTCADJ              (0x03FF0000) /* 送信レイテンシー調整 */
#define E25G_PTPCTL_TRNSPCKMD           (0x00000100) /* タイムスタンプ透過クロックモード */
#define E25G_PTPCTL_1STPEN              (0x00000001) /* 1stepイネーブル */

/* PTP UPDチェックサム設定レジスタ */
#define E25G_UPDCHKSUM                  (0x00000084) /* レジスタオフセット */

/* PTPタイムスタンプオフセット設定レジスタ */
#define E25G_TSTAMPOFFSET               (0x00000088) /* レジスタオフセット */

/* PTPチェックサムオフセット設定レジスタ */
#define E25G_CHKSUMOFFSET               (0x0000008C) /* レジスタオフセット */

/* SFP制御レジスタ */
#define E25G_SFPCNT                     (0x000000E0) /* レジスタオフセット */

#define E25G_SFPCNT_TXEN                (0x00000001) /* SFP TX Enable */

/* 割込み MAC Statusレジスタ */
#define E25G_IRQMACST                   (0x00000100) /* レジスタオフセット */

#define E25G_IRQMACST_PTPFWE            (0x00000080) /* 送信PTP FIFO書き込みエラー発生 */
#define E25G_IRQMACST_PTPFRE            (0x00000040) /* 送信PTP FIFO読み出しエラー発生 */
#define E25G_IRQMACST_RFR               (0x00000020) /* RF発生 */
#define E25G_IRQMACST_RFF               (0x00000010) /* RF回復 */
#define E25G_IRQMACST_LFR               (0x00000008) /* LF発生 */
#define E25G_IRQMACST_LFF               (0x00000004) /* LF回復 */
#define E25G_IRQMACST_BLHBR             (0x00000002) /* (BlockLock=0 or HiBER=1)発生 */
#define E25G_IRQMACST_BLHBF             (0x00000001) /* (BlockLock=1 ａｎｄ HiBER=0)回復 */

/* 割込みマスク MAC Statusレジスタ */
#define E25G_IRQMACSTM                  (0x00000104) /* レジスタオフセット */

#define E25G_IRQMACSTM_PTPFWE           (0x00000080) /* 送信PTP FIFO書き込みエラー発生 */
#define E25G_IRQMACSTM_PTPFRE           (0x00000040) /* 送信PTP FIFO読み出しエラー発生 */
#define E25G_IRQMACSTM_RFR              (0x00000020) /* RF発生 */
#define E25G_IRQMACSTM_RFF              (0x00000010) /* RF回復 */
#define E25G_IRQMACSTM_LFR              (0x00000008) /* LF発生 */
#define E25G_IRQMACSTM_LFF              (0x00000004) /* LF回復 */
#define E25G_IRQMACSTM_BLHBR            (0x00000002) /* (BlockLock=0 or HiBER=1)発生 */
#define E25G_IRQMACSTM_BLHBF            (0x00000001) /* (BlockLock=1 ａｎｄ HiBER=0)回復 */

/* 割込み(生) MAC Statusレジスタ */
#define E25G_IRQMACSTI                  (0x00000108) /* レジスタオフセット */

#define E25G_IRQMACSTI_PTPFWE           (0x00000080) /* 送信PTP FIFO書き込みエラー発生 */
#define E25G_IRQMACSTI_PTPFRE           (0x00000040) /* 送信PTP FIFO読み出しエラー発生 */
#define E25G_IRQMACSTI_RFF              (0x00000010) /* RF回復 */
#define E25G_IRQMACSTI_LFF              (0x00000004) /* LF回復 */
#define E25G_IRQMACSTI_BLHBF            (0x00000001) /* (BlockLock=1 ａｎｄ HiBER=0)回復 */

/* 割込み M-plane送信完了/受信完了レジスタ */
#define E25G_IRQMP                      (0x00000110) /* レジスタオフセット */

#define E25G_IRQMP_TXDONE               (0x00000002) /* TX完了 */
#define E25G_IRQMP_RXDONE               (0x00000001) /* RX完了 */

/* 割込みマスク M-plane送信完了/受信完了レジスタ */
#define E25G_IRQMPM                     (0x00000114) /* レジスタオフセット */

#define E25G_IRQMPM_TXDONE              (0x00000002) /* TX完了 */
#define E25G_IRQMPM_RXDONE              (0x00000001) /* RX完了 */

/* 割込み S-plane送信完了/受信完了レジスタ */
#define E25G_IRQSP                      (0x00000120) /* レジスタオフセット */

#define E25G_IRQSP_PTUPD                (0x00000004) /* PTP TAG Update */
#define E25G_IRQSP_TXDONE               (0x00000002) /* TX完了 */
#define E25G_IRQSP_RXDONE               (0x00000001) /* RX完了 */

/* 割込みマスク S-plane送信完了/受信完了レジスタ */
#define E25G_IRQSPM                     (0x00000124) /* レジスタオフセット */

#define E25G_IRQSPM_PTUPD               (0x00000004) /* PTP TAG Update */
#define E25G_IRQSPM_TXDONE              (0x00000002) /* TX完了 */
#define E25G_IRQSPM_RXDONE              (0x00000001) /* RX完了 */

/* MAC受信ステータス1レジスタ */
#define E25G_RXSTATUS                   (0x00000180) /* レジスタオフセット */

#define E25G_RXSTATUS_SIGOS             (0x00000000) /* シグナルOSワード受信 */
#define E25G_RXSTATUS_BADSFD            (0x00000000) /* 不正SFDを受信 */
#define E25G_RXSTATUS_BADPR             (0x00000000) /* 不正Preambleを受信 */
#define E25G_RXSTATUS_RXLF              (0x00000000) /* Local Fault受信 */
#define E25G_RXSTATUS_INTLF             (0x00000000) /* 内部Local Fault */
#define E25G_RXSTATUS_LF                (0x00000000) /* Local Fault受信 or 内部Local Fault */
#define E25G_RXSTATUS_RF                (0x00000000) /* Remote Faultステータス */
#define E25G_RXSTATUS_HIBER             (0x00000000) /* HiBER */

/* MAC受信ステータス2レジスタ */
#define E25G_RXPAUSEREQ                 (0x00000184) /* レジスタオフセット */

/* MAC受信ステータス3レジスタ */
#define E25G_RXVALIDCODE                (0x00000188) /* レジスタオフセット */

/* MAC受信ステータス4レジスタ */
#define E25G_RXBLOCKLOCK                (0x0000018C) /* レジスタオフセット */

/* MAC送信ステータスレジスタ */
#define E25G_TXSTATUS                   (0x000001A0) /* レジスタオフセット */

#define E25G_TXSTATUS_LOCALFAULT        (0x00010000) /* the receive decoder state machine is in the TX_INIT state */
#define E25G_TXSTATUS_PTPFIFOWERR       (0x00000010) /* 送信PTP FIFO書き込みエラー */
#define E25G_TXSTATUS_PTPFIFORERR       (0x00000001) /* 送信PTP FIFO読み出しエラー */

/* RS-FECステータスレジスタ */
#define E25G_RSFECSTATUS                (0x000001C0) /* レジスタオフセット */

#define E25G_RSFECSTATUS_TXALIGN        (0x00010000) /* TX RS-FECブロックのアライメント完了 */
#define E25G_RSFECSTATUS_HISER          (0x00000010) /* シンボルエラー数閾値K=417超え */
#define E25G_RSFECSTATUS_RXALIGN        (0x00000001) /* RX RS-FECブロックのアライメント完了 */

/* FECステータスレジスタ */
#define E25G_FECSTATUS                  (0x000001C4) /* レジスタオフセット */

#define E25G_FECSTATUS_LOCKERR          (0x00000000) /* FEC Lockエラー */
#define E25G_FECSTATUS_RXLOCK           (0x00000000) /* FEC RX Lock */

/* 受信Bankステータス(M-Plane)レジスタ */
#define E25G_BANKSTATUSM                (0x00000200) /* レジスタオフセット */

/* 受信Bank番号通知(M-Plane)レジスタ */
#define E25G_BANKNOM                    (0x00000204) /* レジスタオフセット */

#define BANKNOM_LENGTH                  (0x07FF0000) /* 受信フレーム長 */
#define BANKNOM_LENGTH_SHIFT            (0x00000010) /* 受信フレーム長シフト */
#define BANKNOM_BANKNO                  (0x0000001F) /* Bank番号 */

/* 受信Bankステータス(S-Plane)レジスタ */
#define E25G_BANKSTATUSS                (0x00000210) /* レジスタオフセット */

/* 受信Bank番号通知(S-Plane)レジスタ */
#define E25G_BANKNOS                    (0x00000214) /* レジスタオフセット */

#define E25G_BANKNOS_LENGTH             (0x07FF0000) /* 受信フレーム長 */
#define E25G_BANKNOS_LENGTH_SHIFT       (0x00000010) /* 受信フレーム長シフト */
#define E25G_BANKNOS_BANKNO             (0x0000001F) /* Bank番号 */

/* 受信Filter用DA設定レジスタ */
#define E25G_FILTDA11                   (0x00000280) /* レジスタオフセット */
#define E25G_FILTDA12                   (0x00000284) /* レジスタオフセット */
#define E25G_FILTDA21                   (0x00000288) /* レジスタオフセット */
#define E25G_FILTDA22                   (0x0000028C) /* レジスタオフセット */
#define E25G_FILTDA31                   (0x00000290) /* レジスタオフセット */
#define E25G_FILTDA32                   (0x00000294) /* レジスタオフセット */
#define E25G_FILTDA41                   (0x00000298) /* レジスタオフセット */
#define E25G_FILTDA42                   (0x0000029C) /* レジスタオフセット */
#define E25G_FILTDA51                   (0x000002A0) /* レジスタオフセット */
#define E25G_FILTDA52                   (0x000002A4) /* レジスタオフセット */
#define E25G_FILTDA61                   (0x000002A8) /* レジスタオフセット */
#define E25G_FILTDA62                   (0x000002AC) /* レジスタオフセット */
#define E25G_FILTDA71                   (0x000002B0) /* レジスタオフセット */
#define E25G_FILTDA72                   (0x000002B4) /* レジスタオフセット */
#define E25G_FILTDA81                   (0x000002B8) /* レジスタオフセット */
#define E25G_FILTDA82                   (0x000002BC) /* レジスタオフセット */

/* U/FC-Plane受信判定用VID設定レジスタ */
#define E25G_FILTVID1                   (0x00000300) /* レジスタオフセット */
#define E25G_FILTVID2                   (0x00000304) /* レジスタオフセット */
#define E25G_FILTVID3                   (0x00000308) /* レジスタオフセット */
#define E25G_FILTVID4                   (0x0000030C) /* レジスタオフセット */
#define E25G_FILTVID5                   (0x00000310) /* レジスタオフセット */
#define E25G_FILTVID6                   (0x00000314) /* レジスタオフセット */
#define E25G_FILTVID7                   (0x00000318) /* レジスタオフセット */
#define E25G_FILTVID8                   (0x0000031C) /* レジスタオフセット */
#define E25G_FILTVID9                   (0x00000320) /* レジスタオフセット */
#define E25G_FILTVID10                  (0x00000324) /* レジスタオフセット */
#define E25G_FILTVID11                  (0x00000325) /* レジスタオフセット */
#define E25G_FILTVID12                  (0x0000032C) /* レジスタオフセット */
#define E25G_FILTVID13                  (0x00000330) /* レジスタオフセット */
#define E25G_FILTVID14                  (0x00000334) /* レジスタオフセット */
#define E25G_FILTVID15                  (0x00000338) /* レジスタオフセット */
#define E25G_FILTVID16                  (0x0000033C) /* レジスタオフセット */

#define E25G_FILTVID_EN                 (0x00010000) /* フィルターイネーブル */
#define E25G_FILTVID_VID                (0x00000FFF) /* VID */

/* S-Plane受信Filter用E-Type設定レジスタ */
#define E25G_FILTETYPE1                 (0x00000380) /* レジスタオフセット */
#define E25G_FILTETYPE2                 (0x00000384) /* レジスタオフセット */

/* 送信トリガ(M-Plane)レジスタ */
#define E25G_TXTRIGM                    (0x00000400) /* レジスタオフセット */

/* 送信フレーム長(M-Plane)レジスタ */
#define E25G_TXLENGTHM                  (0x00000404) /* レジスタオフセット */

/* 送信トリガ(S-Plane)レジスタ */
#define E25G_TXTRIGS                    (0x00000410) /* レジスタオフセット */

/* 送信フレーム長(S-Plane)レジスタ */
#define E25G_TXLENGTHS                  (0x00000414) /* レジスタオフセット */

/* 送信PTPタグ設定レジスタ */
#define E25G_TXPTPTAGS                  (0x00000418) /* レジスタオフセット */

/* 送信PTPタイムスタンプタグフィールドレジスタ */
#define E25G_TXPTPTAG                   (0x00000420) /* レジスタオフセット */

/* 送信PTPタイムスタンプレジスタ */
#define E25G_TXTSTAMP1                  (0x00000424) /* レジスタオフセット */
#define E25G_TXTSTAMP2                  (0x00000428) /* レジスタオフセット */
#define E25G_TXTSTAMP3                  (0x0000042C) /* レジスタオフセット */
#define E25G_TXTSTAMPDBG                (0x00000680) /* レジスタオフセット */


/* M-Plane送信優先タイマレジスタ */
#define E25G_MTIMER                     (0x00000470) /* レジスタオフセット */

/* U-Plane 送信カプセル用DA設定レジスタ */
#define E25G_CAPDA11                    (0x00000480) /* レジスタオフセット */
#define E25G_CAPDA12                    (0x00000484) /* レジスタオフセット */

/* U-Plane 送信カプセル用SA設定レジスタ */
#define E25G_CAPSA11                    (0x00000488) /* レジスタオフセット */
#define E25G_CAPSA12                    (0x0000048C) /* レジスタオフセット */

/* U-Plane 送信カプセル用TPIDレジスタ */
#define E25G_CAPTPID                    (0x00000490) /* レジスタオフセット */

/* U-Plane 送信カプセル用TAGレジスタ */
#define E25G_CAPTAG                     (0x00000494) /* レジスタオフセット */

#define E25G_CAPTAG_CAPPCP              (0x00030000) /* カプセル用PCP */
#define E25G_CAPTAG_CAPVID              (0x00000FFF) /* カプセル用VID */

/* SystemTimer LD値設定レジスタ */
#define E25G_SYSTIMLDSH                 (0x00000600) /* レジスタオフセット */
#define E25G_SYSTIMLDSL                 (0x00000604) /* レジスタオフセット */
#define E25G_SYSTIMLDN                  (0x00000608) /* レジスタオフセット */

/* SystemTimer LD EN設定レジスタ */
#define E25G_SYSTIMLDEN                 (0x0000060C) /* レジスタオフセット */

/* SystemTimerラッチトリガレジスタ */
#define E25G_SYSTIMVALLT                (0x00000610) /* レジスタオフセット */

/* SystemTimer値レジスタ */
#define E25G_SYSTIMVALSH                (0x00000614) /* レジスタオフセット */
#define E25G_SYSTIMVALSL                (0x00000618) /* レジスタオフセット */
#define E25G_SYSTIMVALN                 (0x0000061C) /* レジスタオフセット */

/* 統計カウンタラッチトリガレジスタ */
#define E25G_STATLAT                    (0x00000800) /* レジスタオフセット */

/* 統計カウンタクリアトリガレジスタ */
#define E25G_STATCLR1                   (0x00000804) /* レジスタオフセット */
#define E25G_STATCLR2                   (0x00000808) /* レジスタオフセット */

/* 統計カウンタレジスタ */
#define E25G_RXUPLANE                   (0x00000810) /* 受信U/FC-Planeフレーム数(正常のみ) */
#define E25G_RXMPLANE                   (0x00000814) /* 受信M-Planeフレーム数(正常のみ) */
#define E25G_RXSPLANE                   (0x00000818) /* 受信S-Planeフレーム数(正常のみ) */
#define E25G_RXDAERR                    (0x0000081C) /* DA不一致フレーム数 */
#define E25G_RXETYPEERR                 (0x00000820) /* U/FC-Plane判定用E-Type不一致フレーム数 */
#define E25G_RXVERERR                   (0x00000824) /* Version不一致フレーム数 */
#define E25G_STATRXFRERR                (0x00000828) /* フレーミングエラー */
#define E25G_STATRXSMALL                (0x0000082C) /* 64Byte未満パケット */
#define E25G_STATRXJABER                (0x00000830) /* ctl_rx_max_packet_lenを超える長さでFCS異常のパケット */
#define E25G_SYTATRXLARGE               (0x00000834) /* 9215Byte以上のパケット */
#define E25G_STATRXOVRSIZE              (0x00000838) /* ctl_rx_max_packet_lenを超える長さでFCS正常のパケット */
#define E25G_STATRXUDRSIZE              (0x0000083C) /* stat_rx_min_packet_lenより短い長さでFCS正常のパケット */
#define E25G_STATRXTOOLONG              (0x00000840) /* ctl_rx_max_packet_lenを超える長さのパケット */
#define E25G_STATRXFRAGMNT              (0x00000844) /* stat_rx_min_packet_lenより短い長さでFCS異常のパケット */
#define E25G_STATRXBADCODE              (0x00000848) /* 64B/66B符号違反 */
#define E25G_STATRXTOTALPACKET          (0x0000084C) /* 受信パケット */
#define E25G_STATRXBADFCS               (0x00000850) /* FCS異常 */
#define E25G_STATRXSTOMPEDFCS           (0x00000854) /* Stomped FCS(FCSが前反転しているもの) */
#define E25G_STATRXUNICAST              (0x00000858) /* ユニキャストフレームの受信 */
#define E25G_STATRXMULTICAST            (0x0000085C) /* マルチキャストフレームの受信 */
#define E25G_STATRRXBROADCAST           (0x00000860) /* ブロードキャストフレームの受信 */
#define E25G_STATRXPAUSE                (0x00000864) /* 802.3x MAC Pause パケット受信 */
#define E25G_OVFFRAMES                  (0x00000868) /* FIFO OverFlowによる廃棄フレーム数 */
#define E25G_AXIERRFRAMES               (0x0000086V) /* AXI4でのエラー発生による廃棄フレーム数 */
#define E25G_OVFFRAMEM                  (0x00000870) /* FIFO OverFlowによる廃棄フレーム数 */
#define E25G_AXIERRFRAMEM               (0x00000874) /* AXI4でのエラー発生による廃棄フレーム数 */
#define E25G_TXUPLANE                   (0x00000878) /* 送信U/FC-Planeフレーム数(正常のみ) */
#define E25G_TXMPLANE                   (0x0000087V) /* 送信M-Planeフレーム数(正常のみ) */
#define E25G_TXSPLANE                   (0x00000880) /* 送信S-Planeフレーム数(正常のみ) */
#define E25G_STATTXPACKETS              (0x00000884) /* 送信パケット */
#define E25G_STATTXBADFCS               (0x00000888) /* 64Byte以上のFCSエラー送信パケット */
#define E25G_STATTXFRAMEERROR           (0x0000088C) /* EOP (End of Packet) の中止を示す tx_errin がセットされた送信パケット */
#define E25G_FECCOR                     (0x00000890) /* FECエラーカウント(訂正) */
#define E25G_FECCANTCOR                 (0x00000894) /* FECエラーカウント(訂正不可) */
#define E25G_RSFECCOR                   (0x00000898) /* RS-FECエラーカウント(訂正) */
#define E25G_RSFECUNCOR                 (0x0000089C) /* RS-FECエラーカウント(訂正不可) */
#define E25G_RSFECERR                   (0x000008A0) /* RS-FECエラーカウント検出 */

/* ハード自律リセットマスク */
#define E25G_RSTMASK                    (0x00000D00) /* レジスタオフセット */

#define E25G_RSTMASK_MASK               (0x00000001) /* マスク */

#define E25G_USAGE_PLANE_M               (0)
#define E25G_USAGE_PLANE_S               (1)


#endif	/* __E25G_HW_H_ */
