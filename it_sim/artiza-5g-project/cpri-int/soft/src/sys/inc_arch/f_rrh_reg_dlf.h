/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_dlf.h
 * @brief レジスタアドレス定義ヘッダ(DLF)
 * @date  2018/04/17	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_DLF_H
#define F_RRH_REG_DLF_H

/*!
 * @name DLFレジスタアドレス
 * @note DLFレジスタアドレス
 * @{
 */
#define	D_RRH_REG_DLF_DLFDGAIN0					(D_RRH_REG_DLF + 0x00004000)	/* DL側Gain制御（Cr0 - iFFT入力側）			*/
#define	D_RRH_REG_DLF_DLTDGAIN0					(D_RRH_REG_DLF + 0x00004004)	/* DL側Gain制御（Cr0 - iFFF出力側）			*/
#define	D_RRH_REG_DLF_DLFDGAIN1					(D_RRH_REG_DLF + 0x00004008)	/* DL側Gain制御（Cr1 - iFFT入力側）			*/
#define	D_RRH_REG_DLF_DLTDGAIN1					(D_RRH_REG_DLF + 0x0000400C)	/* DL側Gain制御（Cr1 - iFFF出力側）			*/
#define	D_RRH_REG_DLF_DLCAL						(D_RRH_REG_DLF + 0x00004010)	/* DL側キャリブレーションイネーブル			*/
#define	D_RRH_REG_DLF_DLBST						(D_RRH_REG_DLF + 0x00004014)	/* DL側補正処理イネーブル					*/
#define	D_RRH_REG_DLF_DLBBFSEL					(D_RRH_REG_DLF + 0x00004018)	/* DL側BBF補正テーブル制御					*/
#define	D_RRH_REG_DLF_DLBBFMON					(D_RRH_REG_DLF + 0x0000401C)	/* DL側BBF補正テーブル使用面通知			*/
#define	D_RRH_REG_DLF_DLCALTBL					(D_RRH_REG_DLF + 0x00004020)	/* TableRAMアクセス方路制御（CAL補正係数）	*/
#define	D_RRH_REG_DLF_DLCHFTBL					(D_RRH_REG_DLF + 0x00004024)	/* TableRAMアクセス方路制御（CHF補正係数）	*/
#define	D_RRH_REG_DLF_DLBBFTBL					(D_RRH_REG_DLF + 0x00004028)	/* TableRAMアクセス方路制御（DLB補正係数）	*/
#define	D_RRH_REG_DLF_DLWINTBL					(D_RRH_REG_DLF + 0x0000402C)	/* TableRAMアクセス方路制御（窓係数）		*/
#define	D_RRH_REG_DLF_DLTBL						(D_RRH_REG_DLF + 0x00004030)	/* TableRAMリードミスマッチ					*/
#define	D_RRH_REG_DLF_DLPLS						(D_RRH_REG_DLF + 0x00004034)	/* TDD受信パルス異常						*/
#define	D_RRH_REG_DLF_DFDG						(D_RRH_REG_DLF + 0x00004038)	/* 演算Flowモニタ(iFFT入力側Gain処理OverFlow)	*/
#define	D_RRH_REG_DLF_DBST						(D_RRH_REG_DLF + 0x0000403C)	/* 演算Flowモニタ(Boost演算のOverFlow)		*/
#define	D_RRH_REG_DLF_DTDG						(D_RRH_REG_DLF + 0x00004040)	/* 演算Flowモニタ(iFFT出力側Gain処理OverFlow)	*/
#define	D_RRH_REG_DLF_DFFT						(D_RRH_REG_DLF + 0x00004044)	/* 演算Flowモニタ(FFT演算処理OverFlow)		*/
#define	D_RRH_REG_DLF_DLMXQUE					(D_RRH_REG_DLF + 0x00004048)	/* データFlowモニタ							*/
#define	D_RRH_REG_DLF_DWINCOEF					(D_RRH_REG_DLF + 0x00004400)	/* Window処理係数テーブル					*/
#define	D_RRH_REG_DLF_DLCALREVAL				(D_RRH_REG_DLF + 0x00004800)	/* CAL補正係数（実部）						*/
#define	D_RRH_REG_DLF_DLCALIMVAL				(D_RRH_REG_DLF + 0x00004C00)	/* CAL補正係数（虚部）						*/
#define	D_RRH_REG_DLF_DLCHFVAL					(D_RRH_REG_DLF + 0x00005000)	/* CHF補正係数								*/
#define	D_RRH_REG_DLF_DLBBFVAL0					(D_RRH_REG_DLF + 0x00006000)	/* BBF補正係数（0面）						*/
#define	D_RRH_REG_DLF_DLBBFVAL1					(D_RRH_REG_DLF + 0x00007000)	/* BBF補正係数（1面）						*/
/* @} */

/*!
 * @name DLFレジスタbit & Shift
 * @note DLFレジスタbit & Shift
 * @{
 */
/* @} */
#endif
/* @} */
