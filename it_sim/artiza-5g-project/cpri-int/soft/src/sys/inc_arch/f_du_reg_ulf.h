/*!
 * @skip  $ld:$
 * @file  f_du_reg_ulf.h
 * @brief レジスタアドレス定義ヘッダ(ULF)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_ULF_H
#define F_DU_REG_ULF_H

/*!
 * @name ULFレジスタアドレス
 * @note ULFレジスタアドレス, sub6とmmWでdesineが異なるので注意！
 * @{
 */
/*	sub6	*/
#define D_DU_REG_ULF_DGLB0_SUB6				0xA2204000		/* DGLB0			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_DGLB1_SUB6				0xA2204004		/* DGLB1			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_ULCPRMVSTPOINT_SUB6	0xA2204080		/* ULCPRMVSTPOINT	(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_ULCPRMV_SUB6			0xA2204080		/* ULCPRMV			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_ULCPRMVMODE_SUB6		0xA2204088		/* ULCPRMVMODE		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UTDGAIN0_SUB6			0xA2204100		/* UTDGAIN0			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UTDGAIN1_SUB6			0xA2204104		/* UTDGAIN1			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UTDGAINENB_SUB6		0xA2204108		/* UTDGAINENB		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UFDGAIN0_SUB6			0xA2204110		/* UFDGAIN0			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UFDGAIN1_SUB6			0xA2204114		/* UFDGAIN1			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UFDGAINENB_SUB6		0xA2204118		/* UFDGAINENB		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UCALENB_SUB6			0xA2204180		/* UCALENB			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UCALREGSEL_SUB6		0xA2204184		/* UCALREGSEL		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UBSTENB_SUB6			0xA2204200		/* UBSTENB			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_DLBSTSCSTR0_SUB6		0xA2204210		/* DLBSTSCSTR0		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_DLBSTSCLEN0_SUB6		0xA2204214		/* DLBSTSCLEN0		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_DLBSTSCSTR1_SUB6		0xA2204218		/* DLBSTSCSTR1		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_DLBSTSCLEN1_SUB6		0xA220421C		/* DLBSTSCLEN1		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UTBLSEL_SUB6			0xA2204220		/* UTBLSEL			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UPHASFTSEL_SUB6		0xA2204280		/* UPHASFTSEL		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UPHASFTENB_SUB6		0xA2204284		/* UPHASFTENB		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UTDGOVFLW_SUB6			0xA2204300		/* UTDGOVFLW		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UBSTOVFLW_SUB6			0xA2204304		/* UBSTOVFLW		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UCALOVFLW_SUB6			0xA2204308		/* UCALOVFLW		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UFFTOVFLW_SUB6			0xA220430C		/* UFFTOVFLW		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UFDGOVFLW_SUB6			0xA2204310		/* UFDGOVFLW		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UDBFCVFLW_SUB6			0xA2204314		/* UDBFCVFLW		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UESBUFSLP_SUB6			0xA2204320		/* UESBUFSLP		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UESBUFPINT_SUB6		0xA2204324		/* UESBUFPINT		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UFFTMUX_SUB6			0xA2204330		/* UFFTMUX			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UPLSSLP_SUB6			0xA2204340		/* UPLSSLP			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_ULFMONSEL_SUB6			0xA2204380		/* ULFMONSEL		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_DBFIDSET_SUB6			0xA2204388		/* DBFIDSET			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_ULSCS_SUB6				0xA2200014		/* ULSCS			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_ULBW_SUB6				0xA2200018		/* ULBW				(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_CR0PRM_SUB6			0xA2200020		/* CR0PRM			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_CR1PRM_SUB6			0xA2200024		/* CR1PRM			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UCALCOEF_SUB6			0xA2204800		/* UCALCOEF			(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UCALCOEFIMA_SUB6		0xA2204840		/* UCALCOEFIMA		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UCALCOEFIMB_SUB6		0xA2204860		/* UCALCOEFIMB		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UPCCOEF15KS0_SUB6		0xA2205000		/* UPCCOEF15KS0		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UPCCOEF30KS0_SUB6		0xA2205080		/* UPCCOEF30KS0		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UPCCOEF30KS1_SUB6		0xA22050C0		/* UPCCOEF30KS1		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UPCCOEF60KS0_SUB6		0xA2205100		/* UPCCOEF60KS0		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UPCCOEF60KS1_SUB6		0xA2205140		/* UPCCOEF60KS1		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UPCCOEF60KS2_SUB6		0xA2205180		/* UPCCOEF60KS2		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UPCCOEF60KS3_SUB6		0xA22051C0		/* UPCCOEF60KS3		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UBSTCOEFCC0A_SUB6		0xA2206000		/* UBSTCOEFCC0A		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UBSTCOEFCC0B_SUB6		0xA2206400		/* UBSTCOEFCC0B		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UBSTCOEFCC1A_SUB6		0xA2206800		/* UBSTCOEFCC1A		(sub6 : o / mmw : -)*/
#define D_DU_REG_ULF_UBSTCOEFCC1B_SUB6		0xA2206C00		/* UBSTCOEFCC1B		(sub6 : o / mmw : -)*/
/*	mmW	*/
#define D_DU_REG_ULF_DGLB0_MMW				0xA1004000		/* DGLB0			(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_DGLB1_MMW				0xA1004004		/* DGLB1			(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_ULCPRMV_MMW			0xA1004080		/* ULCPRMV			(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_ULCPRMVMODE_MMW		0xA1004088		/* ULCPRMVMODE		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UTDGAIN0_MMW			0xA1004100		/* UTDGAIN0			(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UTDGAINENB_MMW			0xA1004108		/* UTDGAINENB		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UFDGAIN0_MMW			0xA1004110		/* UFDGAIN0			(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UFDGAINENB_MMW			0xA1004118		/* UFDGAINENB		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UCALENB_MMW			0xA1004180		/* UCALENB			(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UCALREGSEL_MMW			0xA1004184		/* UCALREGSEL		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UBSTENB_MMW			0xA1004200		/* UBSTENB			(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_DLBSTSCSTR0_MMW		0xA1004210		/* DLBSTSCSTR0		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_DLBSTSCLEN0_MMW		0xA1004214		/* DLBSTSCLEN0		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_DLBSTSCSTR1_MMW		0xA1004218		/* DLBSTSCSTR1		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_DLBSTSCLEN1_MMW		0xA100421C		/* DLBSTSCLEN1		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UTBLSEL_MMW			0xA1004220		/* UTBLSEL			(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPHASFTSEL_MMW			0xA1004280		/* UPHASFTSEL		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPHASFTENB_MMW			0xA1004284		/* UPHASFTENB		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UTDGOVFLW_MMW			0xA1004300		/* UTDGOVFLW		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UBSTOVFLW_MMW			0xA1004304		/* UBSTOVFLW		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UCALOVFLW_MMW			0xA1004308		/* UCALOVFLW		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UFFTOVFLW_MMW			0xA100430C		/* UFFTOVFLW		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UFDGOVFLW_MMW			0xA1004310		/* UFDGOVFLW		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UDBFCVFLW_MMW			0xA1004314		/* UDBFCVFLW		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_DPSFTOVFLW_MMW			0xA1004318		/* DPSFTOVFLW		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UFFTMUX_MMW			0xA1004330		/* UFFTMUX			(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPLSSLP_MMW			0xA1004340		/* UPLSSLP			(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_ULFMONSEL_MMW			0xA1004380		/* ULFMONSEL		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_ULSCS_MMW				0xA1000014		/* ULSCS			(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_ULBW_MMW				0xA1000018		/* ULBW				(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_CRPRM_MMW				0xA1000020		/* CRPRM			(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UCALCOEFREA_MMW		0xA1004800		/* UCALCOEFREA		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UCALCOEFREB_MMW		0xA1004820		/* UCALCOEFREB		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UCALCOEFIMA_MMW		0xA1004840		/* UCALCOEFIMA		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UCALCOEFIMB_MMW		0xA1004860		/* UCALCOEFIMB		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPCCOEF60KS0_MMW		0xA1005000		/* UPCCOEF60KS0		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPCCOEF60KS1_MMW		0xA1005040		/* UPCCOEF60KS1		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPCCOEF120KS0_MMW		0xA1005100		/* UPCCOEF120KS0	(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPCCOEF120KS1_MMW		0xA1005140		/* UPCCOEF120KS1	(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPCCOEF120KS2_MMW		0xA1005180		/* UPCCOEF120KS2	(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPCCOEF120KS3_MMW		0xA10051C0		/* UPCCOEF120KS3	(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPCCOEF240KS0_MMW		0xA1005200		/* UPCCOEF240KS0	(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPCCOEF240KS1_MMW		0xA1005240		/* UPCCOEF240KS1	(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPCCOEF240KS2_MMW		0xA1005280		/* UPCCOEF240KS2	(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPCCOEF240KS3_MMW		0xA10052C0		/* UPCCOEF240KS3	(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPCCOEF240KS4_MMW		0xA1005300		/* UPCCOEF240KS4	(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPCCOEF240KS5_MMW		0xA1005340		/* UPCCOEF240KS5	(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPCCOEF240KS6_MMW		0xA1005380		/* UPCCOEF240KS6	(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UPCCOEF240KS7_MMW		0xA10053C0		/* UPCCOEF240KS7	(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UBSTCOEFCC_MMW			0xA1006000		/* UBSTCOEFCC		(sub6 : - / mmw : o)*/
#define D_DU_REG_ULF_UBSTCOEFCC1_MMW		0xA1006800		/* UBSTCOEFCC1		(sub6 : - / mmw : o)*/
/* @} */
#endif
/* @} */
