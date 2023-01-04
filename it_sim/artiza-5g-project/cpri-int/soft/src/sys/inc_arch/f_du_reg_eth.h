/*!
 * @skip  $ld:$
 * @file  f_du_reg_eth.h
 * @brief レジスタアドレス定義ヘッダ(ETH)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_ETH_H
#define F_DU_REG_ETH_H

/*!
 * @name ETHレジスタアドレス
 * @note ETHレジスタアドレス
 * @{
 */
#define	D_DU_REG_ETH_MACSYSRST				0xA1000000		/* MACSYSRST	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_MACRST					0xA1000004		/* MACRST		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RSTDONE				0xA1000010		/* RSTDONE		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXCTL					0xA1000020		/* RXCTL		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_TXCTL					0xA1000040		/* TXCTL		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_UTXEN					0xA1000044		/* UTXEN		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RSFECCTL				0xA1000060		/* RSFECCTL		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FECCTL					0xA1000064		/* FECCTL		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_PTPCTL					0xA1000080		/* PTPCTL		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_UPDCHKSUM				0xA1000084		/* UPDCHKSUM	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_TSTAMPOFFSET			0xA1000088		/* TSTAMPOFFSET	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_CHKSUMOFFSET			0xA100008C		/* CHKSUMOFFSET	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_SFPCNT					0xA10000E0		/* SFPCNT		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_SFPRATE				0xA10000E4		/* SFPRATE		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_PHYCNT					0xA10000F0		/* PHYCNT		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_IRQMACST				0xA1000100		/* IRQMACST		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_IRQMACSTM				0xA1000104		/* IRQMACSTM	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_IRQMACSTI				0xA1000108		/* IRQMACSTI	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_IRQMP					0xA1000110		/* IRQMP		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_IRQMPM					0xA1000114		/* IRQMPM		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_IRQSP					0xA1000120		/* IRQSP		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_IRQSPM					0xA1000124		/* IRQSPM		(sub6 : o / mmw : o)*/
#define D_DU_REG_ETH_HISMACST				(0xa1000160)
#define D_DU_REG_ETH_HISRXST				(0xa1000164)
#define	D_DU_REG_ETH_RXSTATUS				0xA1000180		/* RXSTATUS		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXPAUSEREQ				0xA1000184		/* RXPAUSEREQ	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXVALIDCODE			0xA1000188		/* RXVALIDCODE	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXBLOCKLOCK			0xA100018C		/* RXBLOCKLOCK	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_TXSTATUS				0xA10001A0		/* TXSTATUS		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RSFECSTATUS			0xA10001C0		/* RSFECSTATUS	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FECSTATUS				0xA10001C4		/* FECSTATUS	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_BANKSTATUSM			0xA1000200		/* BANKSTATUSM	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_BANKNOM				0xA1000204		/* BANKNOM		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_BANKSTATUSS			0xA1000210		/* BANKSTATUSS	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_BANKNOS				0xA1000214		/* BANKNOS		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA11				0xA1000280		/* FILTDA11		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA12				0xA1000284		/* FILTDA12		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA21				0xA1000288		/* FILTDA21		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA22				0xA100028C		/* FILTDA22		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA31				0xA1000290		/* FILTDA31		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA32				0xA1000294		/* FILTDA32		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA41				0xA1000298		/* FILTDA41		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA42				0xA100029C		/* FILTDA42		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA51				0xA10002A0		/* FILTDA51		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA52				0xA10002A4		/* FILTDA52		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA61				0xA10002A8		/* FILTDA61		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA62				0xA10002AC		/* FILTDA62		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA71				0xA10002B0		/* FILTDA71		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA72				0xA10002B4		/* FILTDA72		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA81				0xA10002B8		/* FILTDA81		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTDA82				0xA10002BC		/* FILTDA82		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID1				0xA1000300		/* FILTVID1		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID2				0xA1000304		/* FILTVID2		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID3				0xA1000308		/* FILTVID3		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID4				0xA100030C		/* FILTVID4		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID5				0xA1000310		/* FILTVID5		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID6				0xA1000314		/* FILTVID6		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID7				0xA1000318		/* FILTVID7		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID8				0xA100031C		/* FILTVID8		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID9				0xA1000320		/* FILTVID9		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID10				0xA1000324		/* FILTVID10	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID11				0xA1000328		/* FILTVID11	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID12				0xA100032C		/* FILTVID12	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID13				0xA1000330		/* FILTVID13	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID14				0xA1000334		/* FILTVID14	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID15				0xA1000338		/* FILTVID15	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTVID16				0xA100033C		/* FILTVID16	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTETYPE1				0xA1000380		/* FILTETYPE1	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FILTETYPE2				0xA1000384		/* FILTETYPE2	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_TXTRIGM				0xA1000400		/* TXTRIGM		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_TXLENGTHM				0xA1000404		/* TXLENGTHM	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_TXTRIGS				0xA1000410		/* TXTRIGS		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_TXLENGTHS				0xA1000414		/* TXLENGTHS	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_TXPTPTAGS				0xA1000418		/* TXPTPTAGS	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_TXPTPTAG				0xA1000420		/* TXPTPTAG		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_TXTSTAMP1				0xA1000424		/* TXTSTAMP1	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_TXTSTAMP2				0xA1000428		/* TXTSTAMP2	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_TXTSTAMP3				0xA100042C		/* TXTSTAMP3	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_MTIMER					0xA1000470		/* MTIMER		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_CAPDA11				0xA1000480		/* CAPDA11		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_CAPDA12				0xA1000484		/* CAPDA12		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_CAPSA11				0xA1000488		/* CAPSA11		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_CAPSA12				0xA100048C		/* CAPSA12		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_CAPTPID				0xA1000490		/* CAPTPID		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_CAPTAG					0xA1000494		/* CAPTAG		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_SYSTIMLDSH				0xA1000600		/* SYSTIMLDSH	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_SYSTIMLDSL				0xA1000604		/* SYSTIMLDSL	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_SYSTIMLDN				0xA1000608		/* SYSTIMLDN	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_SYSTIMLDEN				0xA100060C		/* SYSTIMLDEN	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_SYSTIMVALLT			0xA1000610		/* SYSTIMVALLT	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_SYSTIMVALSH			0xA1000614		/* SYSTIMVALSH	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_SYSTIMVALSL			0xA1000618		/* SYSTIMVALSL	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_SYSTIMVALN				0xA100061C		/* SYSTIMVALN	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATLAT				0xA1000800		/* STATLAT		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATLAT1				0xA1000804		/* STATLAT1		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATLAT2				0xA1000808		/* STATLAT2		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATLATONLY			0xA100080C		/* STATLATONLY	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXUPLANE				0xA1000810		/* RXUPLANE		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXMPLANE				0xA1000814		/* RXMPLANE		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXSPLANE				0xA1000818		/* RXSPLANE		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXDAERR				0xA100081C		/* RXDAERR		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXETYPEERR				0xA1000820		/* RXETYPEERR	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXVERERR				0xA1000824		/* RXVERERR		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXFRERR			0xA1000828		/* STATRXFRERR	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXSMALL			0xA100082C		/* STATRXSMALL	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXJABER			0xA1000830		/* STATRXJABER	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXLARGE			0xA1000834		/* STATRXLARGE	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXOVRSIZE			0xA1000838		/* STATRXOVRSIZE	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXUDRSIZE			0xA100083C		/* STATRXUDRSIZE	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXTOOLONG			0xA1000840		/* STATRXTOOLONG	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXFRAGMNT			0xA1000844		/* STATRXFRAGMNT	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXBADCODE			0xA1000848		/* STATRXBADCODE	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXTOTALPACKET		0xA100084C		/* STATRXTOTALPACKET	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXBADFCS			0xA1000850		/* STATRXBADFCS	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXSTOMPEDFCS		0xA1000854		/* STATRXSTOMPEDFCS	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXUNICAST			0xA1000858		/* STATRXUNICAST	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXMULTICAST		0xA100085C		/* STATRXMULTICAST	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXBROADCAST		0xA1000860		/* STATRXBROADCAST	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATRXPAUSE			0xA1000864		/* STATRXPAUSE	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_OVFFRAMES				0xA1000868		/* OVFFRAMES	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_AXIERRFRAMES			0xA100086C		/* AXIERRFRAMES	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_OVFFRAMEM				0xA1000870		/* OVFFRAMEM	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_AXIERRFRAMEM			0xA1000874		/* AXIERRFRAMEM	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_TXUPLANE				0xA1000878		/* TXUPLANE		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_TXMPLANE				0xA100087C		/* TXMPLANE		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_TXSPLANE				0xA1000880		/* TXSPLANE		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATTXPACKETS			0xA1000884		/* STATTXPACKETS	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATTXBADFCS			0xA1000888		/* STATTXBADFCS	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATTXFRAMEERROR		0xA100088C		/* STATTXFRAMEERROR		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FECCOR					0xA1000890		/* FECCOR		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FECCANTCOR				0xA1000894		/* FECCANTCOR	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RSFECCOR				0xA1000898		/* RSFECCOR		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RSFECUNCOR				0xA100089C		/* RSFECUNCOR	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RSFECERR				0xA10008A0		/* RSFECERR		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_GTLOOPBACK				0xA1000C00		/* GTLOOPBACK	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_IBERTSEL				0xA1000C10		/* IBERTSEL		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_EYESCAN				0xA1000C20		/* EYESCAN		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_PCSRSVDIN				0xA1000C24		/* PCSRSVDIN	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_GTRXCTL				0xA1000C28		/* GTRXCTL		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_GTTXCTL1				0xA1000C2C		/* GTTXCTL1		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_GTTXCTL2				0xA1000C30		/* GTTXCTL2		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXLEN					0xA1000C80		/* RXLEN		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXCTL2					0xA1000C84		/* RXCTL2		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXFORCERESYNC			0xA1000C88		/* RXFORCERESYNC	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXFCCTL1				0xA1000CC0		/* RXFCCTL1		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXFCCTL2				0xA1000CC4		/* RXFCCTL2		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXETYPE				0xA1000CC8		/* RXETYPE		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXOCGCP				0xA1000CCC		/* RXOCGCP		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RXOCGPP				0xA1000CD0		/* RXOCGPP		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_PAUSEDAMC1				0xA1000CD4		/* PAUSEDAMC1	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_PAUSEDAMC2				0xA1000CD8		/* PAUSEDAMC2	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RSTMASK				0xA1000D00		/* RSTMASK		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_RSTTIMER				0xA1000D04		/* RSTTIMER		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATEMONS				0xA1000E00		/* STATEMONS	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_STATEMONM				0xA1000E04		/* STATEMONM	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_FIFOSTATUS				0xA1000E80		/* FIFOSTATUS	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_PAUSESTATUS			0xA1000F00		/* PAUSESTATUS	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_GTSTATUS				0xA1000F04		/* GTSTATUS		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_ERRDA11				0xA1000F40		/* ERRDA11		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_ERRDA12				0xA1000F44		/* ERRDA12		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_ERRDA21				0xA1000F48		/* ERRDA21		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_ERRDA22				0xA1000F4C		/* ERRDA22		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_ERRDA31				0xA1000F50		/* ERRDA31		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_ERRDA32				0xA1000F54		/* ERRDA32		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_ERRDA41				0xA1000F58		/* ERRDA41		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_ERRDA42				0xA1000F5C		/* ERRDA42		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_ERRETYPE1				0xA1000F60		/* ERRETYPE1	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_ERRETYPE2				0xA1000F64		/* ERRETYPE2	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_ERRETYPE3				0xA1000F68		/* ERRETYPE3	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_ERRETYPE4				0xA1000F6C		/* ERRETYPE4	(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_DBSYTIMS				0xA1000F80		/* DBSYTIMS		(sub6 : o / mmw : o)*/
#define	D_DU_REG_ETH_DBSYTIMRD				0xA1000F84		/* DBSYTIMRD	(sub6 : o / mmw : o)*/
#define	M_DU_REG_ETH_PORT0(addr)			(addr)
#define	M_DU_REG_ETH_PORT1(addr)			(addr)
/* @} */

/*!
 * @name ETHレジスタ マスク値
 * @note ETHレジスタ マスク値
 * @{
 */
#define	D_DU_ETH_MASK_FILTVID_VID			0x00000FFF		/* FILTVIDx VIDマスク値				*/
#define	D_DU_ETH_MASK_FILTVID_EN			0x00010000		/* FILTVIDx Enable bit				*/
#define	D_DU_ETH_MASK_CAPTAG_PCP			0x00070000		/* CAPTAG   PCPマスク値				*/
#define	D_DU_ETH_MASK_CAPTAG_VID			0x00000FFF		/* CAPTAG   VIDマスク値				*/
/* @} */

/*!
 * @name ETHレジスタ シフト値
 * @note ETHレジスタ シフト値
 * @{
 */
#define	D_DU_ETH_SHFT_CAPTAG_PCP			16				/* CAPTAG   PCPシフト値				*/
/* @} */
#endif
/* @} */
