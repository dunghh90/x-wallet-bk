/*!
 * @skip  $ld:$
 * @file  f_du_reg_dlf.h
 * @brief レジスタアドレス定義ヘッダ(DLF)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_DLF_H
#define F_DU_REG_DLF_H

/*!
 * @name DLFレジスタアドレス
 * @note DLFレジスタアドレス, sub6とmmWでdesineが異なるので注意！
 * @{
 */
/*	sub6	*/
#define	D_DU_REG_DLF_DGLB0_SUB6			0xA1004000		/* DGLB0		(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DGLB1_SUB6			0xA1004004		/* DGLB1		(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DFDGAIN0_SUB6		0xA1004100		/* DFDGAIN0		(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DFDGAIN1_SUB6		0xA1004104		/* DFDGAIN1		(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DFDGAINENB_SUB6	0xA1004108		/* DFDGAINENB	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DCALENB_SUB6		0xA1004180		/* DCALENB		(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DCALREGSEL_SUB6	0xA1004184		/* DCALREGSEL	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DBSTENB_SUB6		0xA1004200		/* DBSTENB		(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DLBSTSCSTR0_SUB6	0xA1004210		/* DLBSTSCSTR0	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DLBSTSCLEN0_SUB6	0xA1004214		/* DLBSTSCLEN0	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DLBSTSCSTR1_SUB6	0xA1004218		/* DLBSTSCSTR1	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DLBSTSCLEN1_SUB6	0xA100421C		/* DLBSTSCLEN1	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DTBLSEL_SUB6		0xA1004220		/* DTBLSEL		(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DPHASFTSEL_SUB6	0xA1004280		/* DPHASFTSEL	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DPHASFTENB_SUB6	0xA1004284		/* DPHASFTENB	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DFDGOVFLW_SUB6		0xA1004300		/* DFDGOVFLW	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DBSTOVFLW_SUB6		0xA1004304		/* DBSTOVFLW	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DCALOVFLW_SUB6		0xA1004308		/* DCALOVFLW	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DFFTOVFLW_SUB6		0xA100430C		/* DFFTOVFLW	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DOVLOVFLW_SUB6		0xA1004310		/* DOVLOVFLW	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DDBFOVFLW_SUB6		0xA1004314		/* DDBFOVFLW	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DPSFTOVFLW_SUB6	0xA1004318		/* DPSFTOVFLW	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DFFTMUX_SUB6		0xA1004330		/* DFFTMUX		(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DPLSSLP_SUB6		0xA1004340		/* DPLSSLP		(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DLFMONSEL_SUB6		0xA1004380		/* DLFMONSEL	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DBFIDSET_SUB6		0xA1004388		/* DBFIDSET		(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DLSCS_SUB6			0xA1000018		/* DLSCS		(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DUBW_SUB6			0xA100001C		/* DUBW			(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DR0PRM_SUB6		0xA1000020		/* DR0PRM		(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DR1PRM_SUB6		0xA1000024		/* DR1PRM		(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DCALCOEF_SUB6		0xA1004800		/* DCALCOEF		(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DCALCOEFIMA_SUB6	0xA1004840		/* DCALCOEFIMA	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DCALCOEFIMB_SUB6	0xA1004860		/* DCALCOEFIMB	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DPCCOEF15KS0_SUB6	0xA1005000		/* DPCCOEF15KS0	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DPCCOEF30KS0_SUB6	0xA1005080		/* DPCCOEF30KS0	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DPCCOEF30KS1_SUB6	0xA10050C0		/* DPCCOEF30KS1	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DPCCOEF60KS0_SUB6	0xA1005100		/* DPCCOEF60KS0	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DPCCOEF60KS1_SUB6	0xA1005140		/* DPCCOEF60KS1	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DPCCOEF60KS2_SUB6	0xA1005180		/* DPCCOEF60KS2	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DPCCOEF60KS3_SUB6	0xA10051C0		/* DPCCOEF60KS3	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DBSTCOEFCC0A_SUB6	0xA1006000		/* DBSTCOEFCC0A	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DBSTCOEFCC0B_SUB6	0xA1006400		/* DBSTCOEFCC0B	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DBSTCOEFCC1A_SUB6	0xA1006800		/* DBSTCOEFCC1A	(sub6 : o / mmw : -)*/
#define	D_DU_REG_DLF_DBSTCOEFCC1B_SUB6	0xA1006C00		/* DBSTCOEFCC1B	(sub6 : o / mmw : -)*/
/*	mmW	*/
#define	D_DU_REG_DLF_DGLB0_MMW			0xA2104000		/* DGLB0		(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DFDGAIN0_MMW		0xA2104100		/* DFDGAIN0		(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DFDGAINENB_MMW		0xA2104104		/* DFDGAINENB	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DCALENB_MMW		0xA2104100		/* DCALENB		(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DCALREGSEL_MMW		0xA2104104		/* DCALREGSEL	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DBSTENB_MMW		0xA2104200		/* DBSTENB		(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DLBSTSCSTR0_MMW	0xA2104210		/* DLBSTSCSTR0	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DLBSTSCLEN0_MMW	0xA2104214		/* DLBSTSCLEN0	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DLBSTSCSTR1_MMW	0xA2104218		/* DLBSTSCSTR1	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DLBSTSCLEN1_MMW	0xA210421C		/* DLBSTSCLEN1	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DTBLSEL_MMW		0xA2104220		/* DTBLSEL		(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPHASFTSEL_MMW		0xA2104280		/* DPHASFTSEL	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPHASFTENB_MMW		0xA2104284		/* DPHASFTENB	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DFDGCVFLW_MMW		0xA2104300		/* DFDGCVFLW	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DBSTCVFLW_MMW		0xA2104304		/* DBSTCVFLW	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DCALCVFLW_MMW		0xA2104308		/* DCALCVFLW	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DFFTCVFLW_MMW		0xA210430C		/* DFFTCVFLW	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DOVLCVFLW_MMW		0xA2104310		/* DOVLCVFLW	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPSFTOVFLW_MMW		0xA2104318		/* DPSFTOVFLW	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPLSSLP_MMW		0xA2104340		/* DPLSSLP		(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DLFMONSEL_MMW		0xA2104380		/* DLFMONSEL	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DLSCS_MMW			0xA2100014		/* DLSCS		(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DUBW_MMW			0xA2100018		/* DUBW			(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_CRPRM_MMW			0xA2100020		/* CRPRM		(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DCALCOEFREA_MMW	0xA2104800		/* DCALCOEFREA	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DCALCOEFREB_MMW	0xA2104820		/* DCALCOEFREB	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DCALCOEFIMA_MMW	0xA2104840		/* DCALCOEFIMA	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DCALCOEFIMB_MMW	0xA2104860		/* DCALCOEFIMB	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPCCOEF60KS0_MMW	0xA2105000		/* DPCCOEF60KS0	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPCCOEF60KS1_MMW	0xA2105040		/* DPCCOEF60KS1	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPCCOEF120KS0_MMW	0xA2105100		/* DPCCOEF120KS0(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPCCOEF120KS1_MMW	0xA2105140		/* DPCCOEF120KS1(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPCCOEF120KS2_MMW	0xA2105180		/* DPCCOEF120KS2(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPCCOEF120KS3_MMW	0xA21051C0		/* DPCCOEF120KS3(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPCCOEF240KS0_MMW	0xA2105200		/* DPCCOEF240KS0(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPCCOEF240KS1_MMW	0xA2105240		/* DPCCOEF240KS1(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPCCOEF240KS2_MMW	0xA2105280		/* DPCCOEF240KS2(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPCCOEF240KS3_MMW	0xA21052C0		/* DPCCOEF240KS3(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPCCOEF240KS4_MMW	0xA2105300		/* DPCCOEF240KS4(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPCCOEF240KS5_MMW	0xA2105340		/* DPCCOEF240KS5(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPCCOEF240KS6_MMW	0xA2105380		/* DPCCOEF240KS6(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DPCCOEF240KS7_MMW	0xA21053C0		/* DPCCOEF240KS7(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DBSTCOEFCA_MMW		0xA2106000		/* DBSTCOEFCA	(sub6 : - / mmw : o)*/
#define	D_DU_REG_DLF_DBSTCOEFCB_MMW		0xA2106200		/* DBSTCOEFCB	(sub6 : - / mmw : o)*/
/* @} */
#endif
/* @} */
