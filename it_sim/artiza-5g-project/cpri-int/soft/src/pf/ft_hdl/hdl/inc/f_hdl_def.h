/*!
 * @skip		$ld:$
 * @file		f_hdl_def.h
 * @brief		HDLスレッド デファイン定義.
 * @date		2010/05/24 ALPHA)横山 Create
 * @date		2013/03/28 ALPHA)松延 B25 対処
 * @date		2013/10/29 ALPHA)鮫島 docomo ZYNQ化 & 共通化対応
 * @date		2015/04/22 ALPHA)鎌田 DCN向け35G-SRE対応
 * @date		2015/09/16 ALPHA)鎌田 modify for RRE 3.5G TDD
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_HDL
 * @{
 */
#ifndef F_HDL_DEF_H
#define F_HDL_DEF_H


/* @} */

/*!
 * @name HDLスレッド状態
 * @note HDLスレッドの状態定義
 * @{
 */
#define	D_HDL_THD_STS_INIT						0x0000				/*!< 初期状態							*/
#define	D_HDL_THD_STS_ACT						0x0001				/*!< 通常運用状態						*/
#define	D_HDL_STATENUM							0x0002				/*!< 診断スレッド状態数					*/
/* @} */

/** @name PF_HDL thread name len
 *  @{
 */
#define	D_HDL_THDNAME_LEN	16										/**< thread name len 					*/
/** @} */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/*!
 * @name    割り込み待ち用define
 * @{
 */
#define D_HDL_BPF_IRQ_CPRI_REC		BPF_IRQ_CPRI
#define D_HDL_BPF_IRQ_CPRI_RE		BPF_IRQ_CPRI_MASTER
/* @} */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/** @name PF_HDL thread数
 *  @{
 */
/* 4TRxRRE用に、子thread数を7 -> 8に変更 */
#define	D_HDL_THDNAME_SRENUM	8									/**< SRE装置の子thread数(SFP追加)		*/
#define	D_HDL_THDNAME_RRENUM	11									/**< SRE装置の子thread数				*/
/** @} */

/** @name PF_HDL SVINDEX数
 *  @{
 */
enum
{
	D_HDL_SV1 = 0,
	D_HDL_SV2,
	D_HDL_SV3,
	D_HDL_SV4,
	D_HDL_SV5,
	D_HDL_SV6,
	D_HDL_SV7,
	D_HDL_SV8,
	D_HDL_SVMAX
};
/** @} */

/** @name PF_HDL SFP Mount status
 *  @{
 */
#define	D_HDL_SFP0			0x00000001								/**< SFP#0 Mountステータス変化有無bit	*/
#define	D_HDL_SFP1			0x00000002								/**< SFP#1 Mountステータス変化有無bit	*/
#define	D_HDL_SFPPMNT		0x00040000								/**< SFP#0 Primary SFP Mount Status bit		*/
#define	D_HDL_SFPSMNT		0x00080000								/**< SFP#1 Secondary SFP Mount Status bit 	*/
/** @} */

/** @name PF_HDL SFP Mount status
 *  @{
 */
#define	D_HDL_1PPS_CALC_CMP_WAIT	3									/**< 1pps SFN/FRM計算 ハード取り込み待ち	*/
#define	D_HDL_1PPS_CALC_WAIT		2									/**< 1pps SFN/FRM計算 待ち				*/
#define	D_HDL_1PPS_CALC				1									/**< 1pps SFN/FRM計算 実行				*/
#define	D_HDL_1PPS_CALC_STOP		0									/**< 1pps SFN/FRM計算 停止				*/
/** @} */

/** @name PF_HDL MPlane Recovery Reset count
 *  @{
 */
#define	D_HDL_MP_RECOVERY_COUNT	450							/**< 450*200msec=>90秒			*/
/** @} */

/** @name ALD(AISG) IRQ factor bit mask
 *  @{
 */
#define	D_HDLALD_IRQDISBL	0x0000001F							/**< AISG IRQ DISABLE				*/
#define	D_HDLALD_IRQENBL	0x00000000							/**< AISG IRQ ENABLE				*/
#define	D_HDLALD_IRQRXEND	0x00000001							/**< AISG 受信完了					*/
#define	D_HDLALD_IRQTXEND	0x00000002							/**< AISG 送信完了					*/
#define	D_HDLALD_IRQRXERR	0x00000004							/**< AISG 受信error					*/
#define	D_HDLALD_IRQRXTO	0x00000008							/**< AISG 受信timeout				*/
#define	D_HDLALD_IRQRXSTART	0x00000010							/**< AISG 受信開始					*/
/** @} */

#endif
/* @} */
