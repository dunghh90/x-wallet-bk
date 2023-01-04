/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_eth.h
 * @brief レジスタアドレス定義ヘッダ(ETH)
 * @date  2018/04/17	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_ETH_H
#define F_RRH_REG_ETH_H

/*!
 * @name ETHレジスタアドレス
 * @note ETHレジスタアドレス
 * @{
 */
#define	D_RRH_REG_ETH_MACSYSRST					(D_RRH_REG_ETH + 0x00000000)	/* MACシステムリセット						*/
#define	D_RRH_REG_ETH_MACRST					(D_RRH_REG_ETH + 0x00000004)	/* MACリセット								*/
#define	D_RRH_REG_ETH_RSTDONE					(D_RRH_REG_ETH + 0x00000010)	/* MACリセット完了							*/
#define	D_RRH_REG_ETH_RXCTL						(D_RRH_REG_ETH + 0x00000020)	/* MAC受信制御								*/
#define	D_RRH_REG_ETH_TXCTL						(D_RRH_REG_ETH + 0x00000040)	/* MAC送信制御								*/
#define	D_RRH_REG_ETH_UTXEN						(D_RRH_REG_ETH + 0x00000044)	/* U-Plane送信制御							*/
#define	D_RRH_REG_ETH_RSFECCTL					(D_RRH_REG_ETH + 0x00000060)	/* RS FEC制御								*/
#define	D_RRH_REG_ETH_FECCTL					(D_RRH_REG_ETH + 0x00000064)	/* FEC制御									*/
#define	D_RRH_REG_ETH_PTPCTL					(D_RRH_REG_ETH + 0x00000080)	/* PTP制御									*/
#define	D_RRH_REG_ETH_UPDCHKSUM					(D_RRH_REG_ETH + 0x00000084)	/* PTP UPDチェックサム設定					*/
#define	D_RRH_REG_ETH_TSTAMPOFFSET				(D_RRH_REG_ETH + 0x00000088)	/* PTPタイムスタンプオフセット設定			*/
#define	D_RRH_REG_ETH_CHKSUMOFFSET				(D_RRH_REG_ETH + 0x0000008C)	/* PTPチェックサムオフセット設定			*/
#define	D_RRH_REG_ETH_SFPCNT					(D_RRH_REG_ETH + 0x000000E0)	/* SFP制御 (SFP TX Enable)					*/
#define	D_RRH_REG_ETH_SFPRATE					(D_RRH_REG_ETH + 0x000000E4)	/* SFP制御 (Driver Swing制御)				*/
#define	D_RRH_REG_ETH_PHYCNT					(D_RRH_REG_ETH + 0x000000F0)	/* Ether PHY制御							*/
#define	D_RRH_REG_ETH_IRQMACST					(D_RRH_REG_ETH + 0x00000100)	/* 割込み MAC Status						*/
#define	D_RRH_REG_ETH_IRQMACSTM					(D_RRH_REG_ETH + 0x00000104)	/* 割込みマスク MAC Status					*/
#define	D_RRH_REG_ETH_IRQMACSTI					(D_RRH_REG_ETH + 0x00000108)	/* 割込み(生) MAC Status					*/
#define	D_RRH_REG_ETH_IRQMP						(D_RRH_REG_ETH + 0x00000110)	/* 割込み M-plane送信完了/受信完了			*/
#define	D_RRH_REG_ETH_IRQMPM					(D_RRH_REG_ETH + 0x00000114)	/* 割込みマスク M-plane送信完了/受信完了	*/
#define	D_RRH_REG_ETH_IRQSP						(D_RRH_REG_ETH + 0x00000120)	/* 割込み S-plane送信完了/受信完了			*/
#define	D_RRH_REG_ETH_IRQSPM					(D_RRH_REG_ETH + 0x00000124)	/* 割込みマスク S-plane送信完了/受信完了	*/
#define	D_RRH_REG_ETH_RXSTATUS					(D_RRH_REG_ETH + 0x00000180)	/* MAC受信ステータス1						*/
#define	D_RRH_REG_ETH_RXPAUSEREQ				(D_RRH_REG_ETH + 0x00000184)	/* MAC受信ステータス2						*/
#define	D_RRH_REG_ETH_RXVALIDCODE				(D_RRH_REG_ETH + 0x00000188)	/* MAC受信ステータス3						*/
#define	D_RRH_REG_ETH_RXBLOCKLOCK				(D_RRH_REG_ETH + 0x0000018C)	/* MAC受信ステータス4						*/
#define	D_RRH_REG_ETH_TXSTATUS					(D_RRH_REG_ETH + 0x000001A0)	/* MAC送信ステータス						*/
#define	D_RRH_REG_ETH_RSFECSTATUS				(D_RRH_REG_ETH + 0x000001C0)	/* RS-FECステータス							*/
#define	D_RRH_REG_ETH_FECSTATUS					(D_RRH_REG_ETH + 0x000001C4)	/* FECステータス							*/
#define	D_RRH_REG_ETH_BANKSTATUSM				(D_RRH_REG_ETH + 0x00000200)	/* 受信Bankステータス(M-Plane)				*/
#define	D_RRH_REG_ETH_BANKNOM					(D_RRH_REG_ETH + 0x00000204)	/* 受信Bank番号通知(M-Plane)				*/
#define	D_RRH_REG_ETH_BANKSTATUSS				(D_RRH_REG_ETH + 0x00000210)	/* 受信Bankステータス(S-Plane)				*/
#define	D_RRH_REG_ETH_BANKNOS					(D_RRH_REG_ETH + 0x00000214)	/* 受信Bank番号通知(S-Plane)				*/
#define	D_RRH_REG_ETH_FILTDA11					(D_RRH_REG_ETH + 0x00000280)	/* 受信Filter用DA設定11						*/
#define	D_RRH_REG_ETH_FILTDA12					(D_RRH_REG_ETH + 0x00000284)	/* 受信Filter用DA設定12						*/
#define	D_RRH_REG_ETH_FILTDA21					(D_RRH_REG_ETH + 0x00000288)	/* 受信Filter用DA設定21						*/
#define	D_RRH_REG_ETH_FILTDA22					(D_RRH_REG_ETH + 0x0000028C)	/* 受信Filter用DA設定22						*/
#define	D_RRH_REG_ETH_FILTDA31					(D_RRH_REG_ETH + 0x00000290)	/* 受信Filter用DA設定31						*/
#define	D_RRH_REG_ETH_FILTDA32					(D_RRH_REG_ETH + 0x00000294)	/* 受信Filter用DA設定32						*/
#define	D_RRH_REG_ETH_FILTDA41					(D_RRH_REG_ETH + 0x00000298)	/* 受信Filter用DA設定41						*/
#define	D_RRH_REG_ETH_FILTDA42					(D_RRH_REG_ETH + 0x0000029C)	/* 受信Filter用DA設定42						*/
#define	D_RRH_REG_ETH_FILTDA51					(D_RRH_REG_ETH + 0x000002A0)	/* 受信Filter用DA設定51						*/
#define	D_RRH_REG_ETH_FILTDA52					(D_RRH_REG_ETH + 0x000002A4)	/* 受信Filter用DA設定52						*/
#define	D_RRH_REG_ETH_FILTDA61					(D_RRH_REG_ETH + 0x000002A8)	/* 受信Filter用DA設定61						*/
#define	D_RRH_REG_ETH_FILTDA62					(D_RRH_REG_ETH + 0x000002AC)	/* 受信Filter用DA設定62						*/
#define	D_RRH_REG_ETH_FILTDA71					(D_RRH_REG_ETH + 0x000002B0)	/* 受信Filter用DA設定71						*/
#define	D_RRH_REG_ETH_FILTDA72					(D_RRH_REG_ETH + 0x000002B4)	/* 受信Filter用DA設定72						*/
#define	D_RRH_REG_ETH_FILTDA81					(D_RRH_REG_ETH + 0x000002B8)	/* 受信Filter用DA設定81						*/
#define	D_RRH_REG_ETH_FILTDA82					(D_RRH_REG_ETH + 0x000002BC)	/* 受信Filter用DA設定82						*/
#define	D_RRH_REG_ETH_FILTVID1					(D_RRH_REG_ETH + 0x00000300)	/* U/FC-Plane受信判定用VID設定1				*/
#define	D_RRH_REG_ETH_FILTVID2					(D_RRH_REG_ETH + 0x00000304)	/* U/FC-Plane受信判定用VID設定2				*/
#define	D_RRH_REG_ETH_FILTVID3					(D_RRH_REG_ETH + 0x00000308)	/* U/FC-Plane受信判定用VID設定3				*/
#define	D_RRH_REG_ETH_FILTVID4					(D_RRH_REG_ETH + 0x0000030C)	/* U/FC-Plane受信判定用VID設定4				*/
#define	D_RRH_REG_ETH_FILTVID5					(D_RRH_REG_ETH + 0x00000310)	/* U/FC-Plane受信判定用VID設定5				*/
#define	D_RRH_REG_ETH_FILTVID6					(D_RRH_REG_ETH + 0x00000314)	/* U/FC-Plane受信判定用VID設定6				*/
#define	D_RRH_REG_ETH_FILTVID7					(D_RRH_REG_ETH + 0x00000318)	/* U/FC-Plane受信判定用VID設定7				*/
#define	D_RRH_REG_ETH_FILTVID8					(D_RRH_REG_ETH + 0x0000031C)	/* U/FC-Plane受信判定用VID設定8				*/
#define	D_RRH_REG_ETH_FILTVID9					(D_RRH_REG_ETH + 0x00000320)	/* U/FC-Plane受信判定用VID設定9				*/
#define	D_RRH_REG_ETH_FILTVID10					(D_RRH_REG_ETH + 0x00000324)	/* U/FC-Plane受信判定用VID設定10			*/
#define	D_RRH_REG_ETH_FILTVID11					(D_RRH_REG_ETH + 0x00000328)	/* U/FC-Plane受信判定用VID設定11			*/
#define	D_RRH_REG_ETH_FILTVID12					(D_RRH_REG_ETH + 0x0000032C)	/* U/FC-Plane受信判定用VID設定12			*/
#define	D_RRH_REG_ETH_FILTVID13					(D_RRH_REG_ETH + 0x00000330)	/* U/FC-Plane受信判定用VID設定13			*/
#define	D_RRH_REG_ETH_FILTVID14					(D_RRH_REG_ETH + 0x00000334)	/* U/FC-Plane受信判定用VID設定14			*/
#define	D_RRH_REG_ETH_FILTVID15					(D_RRH_REG_ETH + 0x00000338)	/* U/FC-Plane受信判定用VID設定15			*/
#define	D_RRH_REG_ETH_FILTVID16					(D_RRH_REG_ETH + 0x0000033C)	/* U/FC-Plane受信判定用VID設定16			*/
#define	D_RRH_REG_ETH_FILTETYPE1				(D_RRH_REG_ETH + 0x00000380)	/* S-Plane受信Filter用E-Type設定1			*/
#define	D_RRH_REG_ETH_FILTETYPE2				(D_RRH_REG_ETH + 0x00000384)	/* S-Plane受信Filter用E-Type設定2			*/
#define	D_RRH_REG_ETH_TXTRIGM					(D_RRH_REG_ETH + 0x00000400)	/* 送信トリガ(M-Plane)						*/
#define	D_RRH_REG_ETH_TXLENGTHM					(D_RRH_REG_ETH + 0x00000404)	/* 送信フレーム長(M-Plane)					*/
#define	D_RRH_REG_ETH_TXTRIGS					(D_RRH_REG_ETH + 0x00000410)	/* 送信トリガ(S-Plane)						*/
#define	D_RRH_REG_ETH_TXLENGTHS					(D_RRH_REG_ETH + 0x00000414)	/* 送信フレーム長(S-Plane)					*/
#define	D_RRH_REG_ETH_TXPTPTAGS					(D_RRH_REG_ETH + 0x00000418)	/* 送信PTPタグ設定							*/
#define	D_RRH_REG_ETH_TXPTPTAG					(D_RRH_REG_ETH + 0x00000420)	/* 送信PTPタイムスタンプタグフィールド		*/
#define	D_RRH_REG_ETH_TXTSTAMP1					(D_RRH_REG_ETH + 0x00000424)	/* 送信PTPタイムスタンプ1					*/
#define	D_RRH_REG_ETH_TXTSTAMP2					(D_RRH_REG_ETH + 0x00000428)	/* 送信PTPタイムスタンプ2					*/
#define	D_RRH_REG_ETH_TXTSTAMP3					(D_RRH_REG_ETH + 0x0000042C)	/* 送信PTPタイムスタンプ3					*/
#define	D_RRH_REG_ETH_MTIMER					(D_RRH_REG_ETH + 0x00000470)	/* M-Plane送信優先タイマ					*/
#define	D_RRH_REG_ETH_CAPDA11					(D_RRH_REG_ETH + 0x00000480)	/* U-Plane 送信カプセル用DA設定1			*/
#define	D_RRH_REG_ETH_CAPDA12					(D_RRH_REG_ETH + 0x00000484)	/* U-Plane 送信カプセル用DA設定2			*/
#define	D_RRH_REG_ETH_CAPSA11					(D_RRH_REG_ETH + 0x00000488)	/* U-Plane 送信カプセル用SA設定1			*/
#define	D_RRH_REG_ETH_CAPSA12					(D_RRH_REG_ETH + 0x0000048C)	/* U-Plane 送信カプセル用SA設定2			*/
#define	D_RRH_REG_ETH_CAPTPID					(D_RRH_REG_ETH + 0x00000490)	/* U-Plane 送信カプセル用TPID				*/
#define	D_RRH_REG_ETH_CAPTAG					(D_RRH_REG_ETH + 0x00000494)	/* U-Plane 送信カプセル用TAG				*/
#define	D_RRH_REG_ETH_SYSTIMLDSH				(D_RRH_REG_ETH + 0x00000600)	/* SystemTimer LD値設定1					*/
#define	D_RRH_REG_ETH_SYSTIMLDSL				(D_RRH_REG_ETH + 0x00000604)	/* SystemTimer LD値設定2					*/
#define	D_RRH_REG_ETH_SYSTIMLDN					(D_RRH_REG_ETH + 0x00000608)	/* SystemTimer LD値設定3					*/
#define	D_RRH_REG_ETH_SYSTIMLDEN				(D_RRH_REG_ETH + 0x0000060C)	/* SystemTimer LD EN設定					*/
#define	D_RRH_REG_ETH_SYSTIMVALLT				(D_RRH_REG_ETH + 0x00000610)	/* SystemTimerラッチトリガ					*/
#define	D_RRH_REG_ETH_SYSTIMVALSH				(D_RRH_REG_ETH + 0x00000614)	/* SystemTimer値 1							*/
#define	D_RRH_REG_ETH_SYSTIMVALSL				(D_RRH_REG_ETH + 0x00000618)	/* SystemTimer値 2							*/
#define	D_RRH_REG_ETH_SYSTIMVALN				(D_RRH_REG_ETH + 0x0000061C)	/* SystemTimer値 3							*/
#define	D_RRH_REG_ETH_STATLAT					(D_RRH_REG_ETH + 0x00000800)	/* 統計カウンタラッチ&クリアトリガ(一括)	*/
#define	D_RRH_REG_ETH_STATLAT1					(D_RRH_REG_ETH + 0x00000804)	/* 統計カウンタラッチ&クリアトリガ1(個別)	*/
#define	D_RRH_REG_ETH_STATLAT2					(D_RRH_REG_ETH + 0x00000808)	/* 統計カウンタラッチ&クリアトリガ2(個別)	*/
#define	D_RRH_REG_ETH_RXUPLANE					(D_RRH_REG_ETH + 0x00000810)	/* DRP統計(受信)1							*/
#define	D_RRH_REG_ETH_RXMPLANE					(D_RRH_REG_ETH + 0x00000814)	/* DRP統計(受信)2							*/
#define	D_RRH_REG_ETH_RXSPLANE					(D_RRH_REG_ETH + 0x00000818)	/* DRP統計(受信)3							*/
#define	D_RRH_REG_ETH_RXDAERR					(D_RRH_REG_ETH + 0x0000081C)	/* DRP統計(受信)4							*/
#define	D_RRH_REG_ETH_RXETYPEERR				(D_RRH_REG_ETH + 0x00000820)	/* DRP統計(受信)5							*/
#define	D_RRH_REG_ETH_RXVERERR					(D_RRH_REG_ETH + 0x00000824)	/* DRP統計(受信)6							*/
#define	D_RRH_REG_ETH_STATRXFRERR				(D_RRH_REG_ETH + 0x00000828)	/* MAC統計_RX1								*/
#define	D_RRH_REG_ETH_STATRXSMALL				(D_RRH_REG_ETH + 0x0000082C)	/* MAC統計_RX2								*/
#define	D_RRH_REG_ETH_STATRXJABER				(D_RRH_REG_ETH + 0x00000830)	/* MAC統計_RX3								*/
#define	D_RRH_REG_ETH_STATRXLARGE				(D_RRH_REG_ETH + 0x00000834)	/* MAC統計_RX4								*/
#define	D_RRH_REG_ETH_STATRXOVRSIZE				(D_RRH_REG_ETH + 0x00000838)	/* MAC統計_RX5								*/
#define	D_RRH_REG_ETH_STATRXUDRSIZE				(D_RRH_REG_ETH + 0x0000083C)	/* MAC統計_RX6								*/
#define	D_RRH_REG_ETH_STATRXTOOLONG				(D_RRH_REG_ETH + 0x00000840)	/* MAC統計_RX7								*/
#define	D_RRH_REG_ETH_STATRXFRAGMNT				(D_RRH_REG_ETH + 0x00000844)	/* MAC統計_RX8								*/
#define	D_RRH_REG_ETH_STATRXBADCODE				(D_RRH_REG_ETH + 0x00000848)	/* MAC統計_RX9								*/
#define	D_RRH_REG_ETH_STATRXTOTALPACKET			(D_RRH_REG_ETH + 0x0000084C)	/* MAC統計_RX10								*/
#define	D_RRH_REG_ETH_STATRXBADFCS				(D_RRH_REG_ETH + 0x00000850)	/* MAC統計_RX11								*/
#define	D_RRH_REG_ETH_STATRXSTOMPEDFCS			(D_RRH_REG_ETH + 0x00000854)	/* MAC統計_RX12								*/
#define	D_RRH_REG_ETH_STATRXUNICAST				(D_RRH_REG_ETH + 0x00000858)	/* MAC統計_RX13								*/
#define	D_RRH_REG_ETH_STATRXMULTICAST			(D_RRH_REG_ETH + 0x0000085C)	/* MAC統計_RX14								*/
#define	D_RRH_REG_ETH_STATRXBROADCAST			(D_RRH_REG_ETH + 0x00000860)	/* MAC統計_RX15								*/
#define	D_RRH_REG_ETH_STATRXPAUSE				(D_RRH_REG_ETH + 0x00000864)	/* MAC統計_RX16								*/
#define	D_RRH_REG_ETH_OVFFRAMES					(D_RRH_REG_ETH + 0x00000868)	/* DRP統計(S-Plane)1						*/
#define	D_RRH_REG_ETH_AXIERRFRAMES				(D_RRH_REG_ETH + 0x0000086C)	/* DRP統計(S-Plane)2						*/
#define	D_RRH_REG_ETH_OVFFRAMEM					(D_RRH_REG_ETH + 0x00000870)	/* DRP統計(M-Plane)1						*/
#define	D_RRH_REG_ETH_AXIERRFRAMEM				(D_RRH_REG_ETH + 0x00000874)	/* DRP統計(M-Plane)2						*/
#define	D_RRH_REG_ETH_TXUPLANE					(D_RRH_REG_ETH + 0x00000878)	/* INS統計(送信)1							*/
#define	D_RRH_REG_ETH_TXMPLANE					(D_RRH_REG_ETH + 0x0000087C)	/* INS統計(送信)2							*/
#define	D_RRH_REG_ETH_TXSPLANE					(D_RRH_REG_ETH + 0x00000880)	/* INS統計(送信)3							*/
#define	D_RRH_REG_ETH_STATTXPACKETS				(D_RRH_REG_ETH + 0x00000884)	/* MAC統計_TX1								*/
#define	D_RRH_REG_ETH_STATTXBADFCS				(D_RRH_REG_ETH + 0x00000888)	/* MAC統計_TX2								*/
#define	D_RRH_REG_ETH_STATTXFRAMEERROR			(D_RRH_REG_ETH + 0x0000088C)	/* MAC統計_TX3								*/
#define	D_RRH_REG_ETH_FECCOR					(D_RRH_REG_ETH + 0x00000890)	/* FEC統計1									*/
#define	D_RRH_REG_ETH_FECCANTCOR				(D_RRH_REG_ETH + 0x00000894)	/* FEC統計2									*/
#define	D_RRH_REG_ETH_RSFECCOR					(D_RRH_REG_ETH + 0x00000898)	/* RS-FEC統計1								*/
#define	D_RRH_REG_ETH_RSFECUNCOR				(D_RRH_REG_ETH + 0x0000089C)	/* RS-FEC統計2								*/
#define	D_RRH_REG_ETH_RSFECERR					(D_RRH_REG_ETH + 0x000008A0)	/* RS-FEC統計3								*/
#define	D_RRH_REG_ETH_GTLOOPBACK				(D_RRH_REG_ETH + 0x00000C00)	/* ループバック								*/
#define	D_RRH_REG_ETH_IBERTSEL					(D_RRH_REG_ETH + 0x00000C10)	/* InsystemIBERT用DRP切替					*/
#define	D_RRH_REG_ETH_EYESCAN					(D_RRH_REG_ETH + 0x00000C20)	/* GTデバッグ (eyescantriggerイベント)		*/
#define	D_RRH_REG_ETH_PCSRSVDIN					(D_RRH_REG_ETH + 0x00000C24)	/* GTデバッグ (PCSRSVDIN)					*/
#define	D_RRH_REG_ETH_GTRXCTL					(D_RRH_REG_ETH + 0x00000C28)	/* GTデバッグ (rx出力クロック選択)			*/
#define	D_RRH_REG_ETH_GTTXCTL1					(D_RRH_REG_ETH + 0x00000C2C)	/* GTデバッグ (メインカーソル係数)			*/
#define	D_RRH_REG_ETH_GTTXCTL2					(D_RRH_REG_ETH + 0x00000C30)	/* GTデバッグ (Tx出力クロック選択)			*/
#define	D_RRH_REG_ETH_RXLEN						(D_RRH_REG_ETH + 0x00000C80)	/* RXコントロール (受信MAXパケット長)			*/
#define	D_RRH_REG_ETH_RXCTL2					(D_RRH_REG_ETH + 0x00000C84)	/* RXコントロール (Custom Preambleイネーブル)	*/
#define	D_RRH_REG_ETH_RXFORCERESYNC				(D_RRH_REG_ETH + 0x00000C88)	/* RXコントロール (RX強制再同期)				*/
#define	D_RRH_REG_ETH_RXFCCTL1					(D_RRH_REG_ETH + 0x00000CC0)	/* RX PAUSEコントロール (受信ポーズイネーブル)							*/
#define	D_RRH_REG_ETH_RXFCCTL2					(D_RRH_REG_ETH + 0x00000CC4)	/* RX PAUSEコントロール (グローバルPause OpeCode処理有効)				*/
#define	D_RRH_REG_ETH_RXETYPE					(D_RRH_REG_ETH + 0x00000CC8)	/* RX PAUSEコントロール (グローバルポーズ処理用のEtherTypeフィールド)	*/
#define	D_RRH_REG_ETH_RXOCGCP					(D_RRH_REG_ETH + 0x00000CCC)	/* RX PAUSEコントロール (グローバル制御パケットオペコード(Max))			*/
#define	D_RRH_REG_ETH_RXOCGPP					(D_RRH_REG_ETH + 0x00000CD0)	/* RX PAUSEコントロール (グローバルポーズパケットオペコード)			*/
#define	D_RRH_REG_ETH_PAUSEDAMC1				(D_RRH_REG_ETH + 0x00000CD4)	/* RX PAUSEコントロール (Pause 処理用MC DA(MSB))						*/
#define	D_RRH_REG_ETH_PAUSEDAMC2				(D_RRH_REG_ETH + 0x00000CD8)	/* RX PAUSEコントロール (Pause 処理用MC DA(LSB))						*/
#define	D_RRH_REG_ETH_RSTMASK					(D_RRH_REG_ETH + 0x00000D00)	/* ハード自律リセットマスク					*/
#define	D_RRH_REG_ETH_RSTTIMER					(D_RRH_REG_ETH + 0x00000D04)	/* ハード自律リセット保護時間				*/
#define	D_RRH_REG_ETH_STATEMONS					(D_RRH_REG_ETH + 0x00000E00)	/* 受信フレーム転送Stateモニタ (S-Plane)	*/
#define	D_RRH_REG_ETH_STATEMONM					(D_RRH_REG_ETH + 0x00000E04)	/* 受信フレーム転送Stateモニタ (M-Plane)	*/
#define	D_RRH_REG_ETH_FIFOSTATUS				(D_RRH_REG_ETH + 0x00000E80)	/* INS部FIFO Statusモニタ					*/
#define	D_RRH_REG_ETH_PAUSESTATUS				(D_RRH_REG_ETH + 0x00000F00)	/* RX Pauseステータス						*/
#define	D_RRH_REG_ETH_GTSTATUS					(D_RRH_REG_ETH + 0x00000F04)	/* GTステータス								*/
#define	D_RRH_REG_ETH_DBSYTIMS					(D_RRH_REG_ETH + 0x00000F80)	/* SystemTimer Debug Set					*/
#define	D_RRH_REG_ETH_DBSYTIMRD					(D_RRH_REG_ETH + 0x00000F84)	/* SystemTimer Debug Rd						*/
/* @} */

/*!
 * @name ETHレジスタbit & Shift
 * @note ETHレジスタbit & Shift
 * @{
 */
/* @} */
#endif
/* @} */
