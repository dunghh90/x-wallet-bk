/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_up.h
 * @brief レジスタアドレス定義ヘッダ(UP)
 * @date  2015/3/27 FJT) Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_UP_H
#define F_RRH_REG_UP_H

/*!
 * @name UPレジスタアドレス
 * @note UPレジスタアドレス
 * @{
 */
#define	D_RRH_REG_UP_BR						0x00000100

#if defined (OPT_RRH_ARCH_A10)
#define	D_RRH_REG_UP_DLA0UPP				0xD1003000
#define	D_RRH_REG_UP_DLA0LOW				0xD1003004
#define	D_RRH_REG_UP_DLA1UPP				0xD1003008
#define	D_RRH_REG_UP_DLA1LOW				0xD100300C
#define	D_RRH_REG_UP_DLACARI				0xD1003020
#define	D_RRH_REG_UP_ULA0UPP				0xD1003030
#define	D_RRH_REG_UP_ULA0LOW				0xD1003034
#define	D_RRH_REG_UP_ULA1UPP				0xD1003038
#define	D_RRH_REG_UP_ULA1LOW				0xD100303C
#define	D_RRH_REG_UP_ULACARI				0xD1003050
#define	D_RRH_REG_UP_RXGAINA				0xD1003060
#define	D_RRH_REG_UP_DLB0UPP				0xD1003100
#define	D_RRH_REG_UP_DLB0LOW				0xD1003104
#define	D_RRH_REG_UP_DLB1UPP				0xD1003108
#define	D_RRH_REG_UP_DLB1LOW				0xD100310C
#define	D_RRH_REG_UP_DLBCARI				0xD1003120
#define	D_RRH_REG_UP_ULB0UPP				0xD1003130
#define	D_RRH_REG_UP_ULB0LOW				0xD1003134
#define	D_RRH_REG_UP_ULB1UPP				0xD1003138
#define	D_RRH_REG_UP_ULB1LOW				0xD100313C
#define	D_RRH_REG_UP_ULBCARI				0xD1003150
#define	D_RRH_REG_UP_RXGAINB				0xD1003160
#define	D_RRH_REG_UP_DLC0UPP				0xD1003200
#define	D_RRH_REG_UP_DLC0LOW				0xD1003204
#define	D_RRH_REG_UP_DLC1UPP				0xD1003208
#define	D_RRH_REG_UP_DLC1LOW				0xD100320C
#define	D_RRH_REG_UP_DLCCARI				0xD1003220
#define	D_RRH_REG_UP_ULC0UPP				0xD1003230
#define	D_RRH_REG_UP_ULC0LOW				0xD1003234
#define	D_RRH_REG_UP_ULC1UPP				0xD1003238
#define	D_RRH_REG_UP_ULC1LOW				0xD100323C
#define	D_RRH_REG_UP_ULCCARI				0xD1003250
#define	D_RRH_REG_UP_RXGAINC				0xD1003260
#define	D_RRH_REG_UP_DLD0UPP				0xD1003300
#define	D_RRH_REG_UP_DLD0LOW				0xD1003304
#define	D_RRH_REG_UP_DLD1UPP				0xD1003308
#define	D_RRH_REG_UP_DLD1LOW				0xD100330C
#define	D_RRH_REG_UP_DLDCARI				0xD1003320
#define	D_RRH_REG_UP_ULD0UPP				0xD1003330
#define	D_RRH_REG_UP_ULD0LOW				0xD1003334
#define	D_RRH_REG_UP_ULD1UPP				0xD1003338
#define	D_RRH_REG_UP_ULD1LOW				0xD100333C
#define	D_RRH_REG_UP_ULDCARI				0xD1003350
#define	D_RRH_REG_UP_RXGAIND				0xD1003360
#define	D_RRH_REG_UP_INIJB					0xD1003400
#define	D_RRH_REG_UP_IQEN					0xD1003410
#define	D_RRH_REG_UP_TMERR					0xD1003420
#define	D_RRH_REG_UP_TMMSK					0xD1003424
#define	D_RRH_REG_UP_CASS					0xD1003500
#define	D_RRH_REG_UP_CASP					0xD1003504
#define	D_RRH_REG_UP_TDDLFIX				0xD1003510
#define	D_RRH_REG_UP_TDDLGAP				0xD1003514
#define	D_RRH_REG_UP_TDULFIX				0xD1003518
#define	D_RRH_REG_UP_TMGMON					0xD1003580
#define	D_RRH_REG_UP_DLYCPPRPH				0xD1003600
#define	D_RRH_REG_UP_DLYCPPRES				0xD1003610
#define	D_RRH_REG_UP_DLYCPPTES				0xD1003614
#define	D_RRH_REG_UP_DLYCPSRPH				0xD1003620
#define	D_RRH_REG_UP_DLYCPSRES				0xD1003630
#define	D_RRH_REG_UP_DLYCPSTES				0xD1003634
#define	D_RRH_REG_UP_CPPHASE				0xD1003640
#define	D_RRH_REG_UP_CPSLNUM				0xD1003644
#define	D_RRH_REG_UP_CPSLCNT				0xD1003648
#define	D_RRH_REG_UP_DGEN					0xD1003650
#define	D_RRH_REG_UP_DGDT					0xD1003654
#define	D_RRH_REG_UP_DGCYC					0xD1003658
#define	D_RRH_REG_UP_MONSEL					0xD10037F0
#define	D_RRH_REG_UP_UPTEST					0xD10037FC

#elif defined (OPT_RRH_ARCH_ZYNQ)
#define	D_RRH_REG_UP_DLA0UPP				0x81003000
#define	D_RRH_REG_UP_DLA0LOW				0x81003004
#define	D_RRH_REG_UP_DLA1UPP				0x81003008
#define	D_RRH_REG_UP_DLA1LOW				0x8100300C
#define	D_RRH_REG_UP_DLACARI				0x81003020
#define	D_RRH_REG_UP_ULA0UPP				0x81003030
#define	D_RRH_REG_UP_ULA0LOW				0x81003034
#define	D_RRH_REG_UP_ULA1UPP				0x81003038
#define	D_RRH_REG_UP_ULA1LOW				0x8100303C
#define	D_RRH_REG_UP_ULACARI				0x81003050
#define	D_RRH_REG_UP_RXGAINA				0x81003060
#define	D_RRH_REG_UP_DLB0UPP				0x81003100
#define	D_RRH_REG_UP_DLB0LOW				0x81003104
#define	D_RRH_REG_UP_DLB1UPP				0x81003108
#define	D_RRH_REG_UP_DLB1LOW				0x8100310C
#define	D_RRH_REG_UP_DLBCARI				0x81003120
#define	D_RRH_REG_UP_ULB0UPP				0x81003130
#define	D_RRH_REG_UP_ULB0LOW				0x81003134
#define	D_RRH_REG_UP_ULB1UPP				0x81003138
#define	D_RRH_REG_UP_ULB1LOW				0x8100313C
#define	D_RRH_REG_UP_ULBCARI				0x81003150
#define	D_RRH_REG_UP_RXGAINB				0x81003160
#define	D_RRH_REG_UP_DLC0UPP				0x81003200
#define	D_RRH_REG_UP_DLC0LOW				0x81003204
#define	D_RRH_REG_UP_DLC1UPP				0x81003208
#define	D_RRH_REG_UP_DLC1LOW				0x8100320C
#define	D_RRH_REG_UP_DLCCARI				0x81003220
#define	D_RRH_REG_UP_ULC0UPP				0x81003230
#define	D_RRH_REG_UP_ULC0LOW				0x81003234
#define	D_RRH_REG_UP_ULC1UPP				0x81003238
#define	D_RRH_REG_UP_ULC1LOW				0x8100323C
#define	D_RRH_REG_UP_ULCCARI				0x81003250
#define	D_RRH_REG_UP_RXGAINC				0x81003260
#define	D_RRH_REG_UP_DLD0UPP				0x81003300
#define	D_RRH_REG_UP_DLD0LOW				0x81003304
#define	D_RRH_REG_UP_DLD1UPP				0x81003308
#define	D_RRH_REG_UP_DLD1LOW				0x8100330C
#define	D_RRH_REG_UP_DLDCARI				0x81003320
#define	D_RRH_REG_UP_ULD0UPP				0x81003330
#define	D_RRH_REG_UP_ULD0LOW				0x81003334
#define	D_RRH_REG_UP_ULD1UPP				0x81003338
#define	D_RRH_REG_UP_ULD1LOW				0x8100333C
#define	D_RRH_REG_UP_ULDCARI				0x81003350
#define	D_RRH_REG_UP_RXGAIND				0x81003360
#define	D_RRH_REG_UP_INIJB					0x81003400
#define	D_RRH_REG_UP_IQEN					0x81003410
#define	D_RRH_REG_UP_TMERR					0x81003420
#define	D_RRH_REG_UP_TMMSK					0x81003424
#define	D_RRH_REG_UP_CASS					0x81003500
#define	D_RRH_REG_UP_CASP					0x81003504
#define	D_RRH_REG_UP_TDDLFIX				0x81003510
#define	D_RRH_REG_UP_TDDLGAP				0x81003514
#define	D_RRH_REG_UP_TDULFIX				0x81003518
#define	D_RRH_REG_UP_TMGMON					0x81003580
#define	D_RRH_REG_UP_DLYCPPRPH				0x81003600
#define	D_RRH_REG_UP_DLYCPPRES				0x81003610
#define	D_RRH_REG_UP_DLYCPPTES				0x81003614
#define	D_RRH_REG_UP_DLYCPSRPH				0x81003620
#define	D_RRH_REG_UP_DLYCPSRES				0x81003630
#define	D_RRH_REG_UP_DLYCPSTES				0x81003634
#define	D_RRH_REG_UP_CPPHASE				0x81003640
#define	D_RRH_REG_UP_CPSLNUM				0x81003644
#define	D_RRH_REG_UP_CPSLCNT				0x81003648
#define	D_RRH_REG_UP_DGEN					0x81003650
#define	D_RRH_REG_UP_DGDT					0x81003654
#define	D_RRH_REG_UP_DGCYC					0x81003658
#define	D_RRH_REG_UP_MONSEL					0x810037F0
#define	D_RRH_REG_UP_UPTEST					0x810037FC

#else

#endif

/* T14測定値 閾値 */
#define D_RRH_UP_T14_THRESHOLD				0x184

/* @} */
#endif
/* @} */
