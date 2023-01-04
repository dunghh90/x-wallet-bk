/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_cm.h
 * @brief レジスタアドレス定義ヘッダ(C&M)
 * @date  2015/3/27 FJT) Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_CM_H
#define F_RRH_REG_CM_H

/*!
 * @name C&Mレジスタアドレス
 * @note C&Mレジスタアドレス
 * @{
 */
#if defined (OPT_RRH_ARCH_A10)
#define	D_RRH_REG_CM_LTXB10						0xD1002000
#define	D_RRH_REG_CM_LTXB32						0xD1002004
#define	D_RRH_REG_CM_LTXB54						0xD1002008
#define	D_RRH_REG_CM_LTXB76						0xD100200C
#define	D_RRH_REG_CM_LTXCNT						0xD1002020
#define	D_RRH_REG_CM_LTXEN						0xD1002028
#define	D_RRH_REG_CM_LRXB10						0xD1002040
#define	D_RRH_REG_CM_LRXB32						0xD1002044
#define	D_RRH_REG_CM_LRXB54						0xD1002048
#define	D_RRH_REG_CM_LRXB76						0xD100204C
#define	D_RRH_REG_CM_LRXST						0xD1002060
#define	D_RRH_REG_CM_LRXBNK						0xD1002064
#define	D_RRH_REG_CM_LRXEN						0xD1002068
#define	D_RRH_REG_CM_LAXIST						0xD1002084
#define	D_RRH_REG_CM_LAERR1						0xD1002090
#define	D_RRH_REG_CM_LAERR2						0xD1002094
#define	D_RRH_REG_CM_LAERR3						0xD1002098
#define	D_RRH_REG_CM_LAPMES						0xD10020A0
#define	D_RRH_REG_CM_LAPLBK						0xD10020B0
#define	D_RRH_REG_CM_CMTEST						0xD10020F0

#elif defined  (OPT_RRH_ARCH_ZYNQ)
/* Primary(Slave) */
#define	D_RRH_REG_CM_LTXB10						0x81002000
#define	D_RRH_REG_CM_LTXB32						0x81002004
#define	D_RRH_REG_CM_LTXB54						0x81002008
#define	D_RRH_REG_CM_LTXB76						0x8100200C
#define	D_RRH_REG_CM_LTXCNT						0x81002020
#define	D_RRH_REG_CM_LTXEN						0x81002028
#define	D_RRH_REG_CM_LRXB10						0x81002040
#define	D_RRH_REG_CM_LRXB32						0x81002044
#define	D_RRH_REG_CM_LRXB54						0x81002048
#define	D_RRH_REG_CM_LRXB76						0x8100204C
#define	D_RRH_REG_CM_LRXST						0x81002060
#define	D_RRH_REG_CM_LRXBNK						0x81002064
#define	D_RRH_REG_CM_LRXEN						0x81002068
#define	D_RRH_REG_CM_LAXIST						0x81002084
#define	D_RRH_REG_CM_LAERR1						0x81002090
#define	D_RRH_REG_CM_LAERR2						0x81002094
#define	D_RRH_REG_CM_LAERR3						0x81002098
#define	D_RRH_REG_CM_LAPMES						0x810020A0
#define	D_RRH_REG_CM_LAPLBK						0x810020B0
#define	D_RRH_REG_CM_CMTEST						0x810020F0

/* Secondary(Master) */
#define	D_RRH_REG_CMS_LTXB10					0x81002800
#define	D_RRH_REG_CMS_LTXB32					0x81002804
#define	D_RRH_REG_CMS_LTXB54					0x81002808
#define	D_RRH_REG_CMS_LTXB76					0x8100280C
#define	D_RRH_REG_CMS_LTXCNT					0x81002820
#define	D_RRH_REG_CMS_LTXEN						0x81002828
#define	D_RRH_REG_CMS_LRXB10					0x81002840
#define	D_RRH_REG_CMS_LRXB32					0x81002844
#define	D_RRH_REG_CMS_LRXB54					0x81002848
#define	D_RRH_REG_CMS_LRXB76					0x8100284C
#define	D_RRH_REG_CMS_LRXST						0x81002860
#define	D_RRH_REG_CMS_LRXBNK					0x81002864
#define	D_RRH_REG_CMS_LRXEN						0x81002868
#define	D_RRH_REG_CMS_LAXIST					0x81002884
#define	D_RRH_REG_CMS_LAERR1					0x81002890
#define	D_RRH_REG_CMS_LAERR2					0x81002894
#define	D_RRH_REG_CMS_LAERR3					0x81002898
#define	D_RRH_REG_CMS_LAPMES					0x810028A0
#define	D_RRH_REG_CMS_LAPLBK					0x810028B0
#define	D_RRH_REG_CMS_CMTEST					0x810028F0

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_RRH_REG_CM_LTXB98             		0x81010010  /**< LAPB送信バッファデータ長 Bank8                   */
#define D_RRH_REG_CM_LTXBBA             		0x81010014  /**< LAPB送信バッファデータ長 Bank10                  */
#define D_RRH_REG_CM_LTXBDC             		0x81010018  /**< LAPB送信バッファデータ長 Bank12                  */
#define D_RRH_REG_CM_LTXBFE             		0x8101001C  /**< LAPB送信バッファデータ長 Bank14                  */
#define D_RRH_REG_CM_LTXST              		0x81010024  /**< LAPB送信中表示                                   */
#define D_RRH_REG_CM_LRXB98             		0x81010050  /**< LAPB受信バッファデータ長 Bank8                   */
#define D_RRH_REG_CM_LRXBBA             		0x81010054  /**< LAPB受信バッファデータ長 Bank10                  */
#define D_RRH_REG_CM_LRXBDC             		0x81010058  /**< LAPB受信バッファデータ長 Bank12                  */
#define D_RRH_REG_CM_LRXBFE             		0x8101005C  /**< LAPB受信バッファデータ長 Bank14                  */
#define D_RRH_REG_CM_T4TIM              		0x81010080  /**< LAP-B T4タイマ設定                               */
#define D_RRH_REG_CM_LATEST             		0x810100C0  /**< LAPBテスト設定                                   */
#define D_RRH_REG_CM_LRXDBGBNK          		0x810100C4  /**< LAPB受信BANK番号通知レジスタ用FIFO状態表示       */
#define D_RRH_REG_CM_LBUFNUM            		0x810100C8  /**< HDLC用バッファインプリバンク数                   */
#define D_RRH_REG_CM_CMRX2AXIST         		0x810107D0  /**< バッファ使用量履歴                               */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#else
#endif
/* @} */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/*!
 * @name C&Mレジスタアドレス算出マクロ
 * @note D_RRH_CPRINO_REC, D_RRH_CPRINO_RE1 ... のマクロを引数に渡してください
 * @{
 */
#define M_RRH_REG_CM_GETADR(cprino, ofs) (UINT)((D_RRH_CPRINO_REC==cprino) ? ofs : (ofs+0x1000+(0x800*(cprino-1))))
#define M_RRH_REG_CM_LTXB10( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LTXB10               )
#define M_RRH_REG_CM_LTXB32( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LTXB32               )
#define M_RRH_REG_CM_LTXB54( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LTXB54               )
#define M_RRH_REG_CM_LTXB76( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LTXB76               )
#define M_RRH_REG_CM_LTXB98( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LTXB98               )
#define M_RRH_REG_CM_LTXBBA( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LTXBBA               )
#define M_RRH_REG_CM_LTXBDC( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LTXBDC               )
#define M_RRH_REG_CM_LTXBFE( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LTXBFE               )
#define M_RRH_REG_CM_LTXCNT( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LTXCNT               )
#define M_RRH_REG_CM_LTXST( cprino )        M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LTXST                )
#define M_RRH_REG_CM_LTXEN( cprino )        M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LTXEN                )
#define M_RRH_REG_CM_LRXB10( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LRXB10               )
#define M_RRH_REG_CM_LRXB32( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LRXB32               )
#define M_RRH_REG_CM_LRXB54( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LRXB54               )
#define M_RRH_REG_CM_LRXB76( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LRXB76               )
#define M_RRH_REG_CM_LRXB98( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LRXB98               )
#define M_RRH_REG_CM_LRXBBA( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LRXBBA               )
#define M_RRH_REG_CM_LRXBDC( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LRXBDC               )
#define M_RRH_REG_CM_LRXBFE( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LRXBFE               )
#define M_RRH_REG_CM_LRXST( cprino )        M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LRXST                )
#define M_RRH_REG_CM_LRXBNK( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LRXBNK               )
#define M_RRH_REG_CM_LRXEN( cprino )        M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LRXEN                )
#define M_RRH_REG_CM_T4TIM( cprino )        M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_T4TIM                )
#define M_RRH_REG_CM_LAXIST( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LAXIST               )
#define M_RRH_REG_CM_LAERR1( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LAERR1               )
#define M_RRH_REG_CM_LAERR2( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LAERR2               )
#define M_RRH_REG_CM_LAERR3( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LAERR3               )
#define M_RRH_REG_CM_LAPMES( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LAPMES               )
#define M_RRH_REG_CM_LAPLBK( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LAPLBK               )
#define M_RRH_REG_CM_LATEST( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LATEST               )
#define M_RRH_REG_CM_LRXDBGBNK( cprino )    M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LRXDBGBNK            )
#define M_RRH_REG_CM_LBUFNUM( cprino )      M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_LBUFNUM              )
#define M_RRH_REG_CM_CMRX2AXIST( cprino )   M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_CMRX2AXIST           )
#define M_RRH_REG_CM_CMTEST( cprino )       M_RRH_REG_CM_GETADR( cprino, D_RRH_REG_CM_CMTEST               )
/* @} */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#endif
/* @} */
