/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_ulf.h
 * @brief レジスタアドレス定義ヘッダ(ULF)
 * @date  2018/04/17	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_ULF_H
#define F_RRH_REG_ULF_H

/*!
 * @name ULFレジスタアドレス
 * @note ULFレジスタアドレス
 * @{
 */
#define	D_RRH_REG_ULF_ULFDGAIN0					(D_RRH_REG_ULF + 0x00004000)	/* UL側Gain制御（Cr0 - iFFT入力側）			*/
#define	D_RRH_REG_ULF_ULTDGAIN0					(D_RRH_REG_ULF + 0x00004004)	/* UL側Gain制御（Cr0 - iFFF出力側）			*/
#define	D_RRH_REG_ULF_ULFDGAIN1					(D_RRH_REG_ULF + 0x00004008)	/* UL側Gain制御（Cr1 - iFFT入力側）			*/
#define	D_RRH_REG_ULF_ULTDGAIN1					(D_RRH_REG_ULF + 0x0000400C)	/* UL側Gain制御（Cr1 - iFFF出力側）			*/
#define	D_RRH_REG_ULF_ULCAL						(D_RRH_REG_ULF + 0x00004010)	/* UL側キャリブレーションイネーブル			*/
#define	D_RRH_REG_ULF_ULBST						(D_RRH_REG_ULF + 0x00004014)	/* UL側補正処理イネーブル					*/
#define	D_RRH_REG_ULF_ULBBFSEL					(D_RRH_REG_ULF + 0x00004018)	/* UL側BBF補正テーブル制御					*/
#define	D_RRH_REG_ULF_ULBBFMON					(D_RRH_REG_ULF + 0x0000401C)	/* UL側BBF補正テーブル使用面通知			*/
#define	D_RRH_REG_ULF_ULCPRMV					(D_RRH_REG_ULF + 0x00004020)	/* CP Remove位置							*/
#define	D_RRH_REG_ULF_ULCALTBL					(D_RRH_REG_ULF + 0x00004024)	/* TableRAMアクセス方路制御（CAL補正係数）	*/
#define	D_RRH_REG_ULF_ULCHFTBL					(D_RRH_REG_ULF + 0x00004028)	/* TableRAMアクセス方路制御（CHF補正係数）	*/
#define	D_RRH_REG_ULF_ULBBFTBL					(D_RRH_REG_ULF + 0x0000402C)	/* TableRAMアクセス方路制御（BBF補正係数）	*/
#define	D_RRH_REG_ULF_ULTBL						(D_RRH_REG_ULF + 0x00004030)	/* TableRAMリードミスマッチ					*/
#define	D_RRH_REG_ULF_UPLS						(D_RRH_REG_ULF + 0x00004034)	/* TDD受信パルス異常						*/
#define	D_RRH_REG_ULF_ULFDG						(D_RRH_REG_ULF + 0x00004038)	/* 演算Flowモニタ(FFT入力側Gain処理OverFlow)*/
#define	D_RRH_REG_ULF_ULBST						(D_RRH_REG_ULF + 0x0000403C)	/* 演算Flowモニタ(Boost演算のOverFlow)		*/
#define	D_RRH_REG_ULF_ULTDG						(D_RRH_REG_ULF + 0x00004040)	/* 演算Flowモニタ(FFT出力側Gain処理OverFlow)*/
#define	D_RRH_REG_ULF_ULFFT						(D_RRH_REG_ULF + 0x00004044)	/* 演算Flowモニタ(FFT演算処理OverFlow)		*/
#define	D_RRH_REG_ULF_ULMXQUE					(D_RRH_REG_ULF + 0x00004048)	/* データFlowモニタ							*/
#define	D_RRH_REG_ULF_ULICES					(D_RRH_REG_ULF + 0x0000404C)	/* ULデータ受信エラーモニタ					*/
#define	D_RRH_REG_ULF_ULCALREVAL				(D_RRH_REG_ULF + 0x00004800)	/* CAL補正係数 (実部)						*/
#define	D_RRH_REG_ULF_ULCALIMVAL				(D_RRH_REG_ULF + 0x00004C00)	/* CAL補正係数 (虚部)						*/
#define	D_RRH_REG_ULF_ULCHFVAL					(D_RRH_REG_ULF + 0x00005000)	/* CHF補正係数								*/
#define	D_RRH_REG_ULF_ULBBFVAL0					(D_RRH_REG_ULF + 0x00006000)	/* BBF補正係数（0面）						*/
#define	D_RRH_REG_ULF_ULBBFVAL1					(D_RRH_REG_ULF + 0x00007000)	/* BBF補正係数（1面）						*/
/* @} */

/*!
 * @name ULFレジスタbit & Shift
 * @note ULFレジスタbit & Shift
 * @{
 */
/* @} */
#endif
/* @} */
