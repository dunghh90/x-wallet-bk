/*!
 * @skip  $ld:$
 * @file  f_du_reg_fbatc.h
 * @brief レジスタアドレス定義ヘッダ(FBATC)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_FBATC_H
#define F_DU_REG_FBATC_H

/*!
 * @name FBATCレジスタアドレス
 * @note FBATCレジスタアドレス
 * @{
 */
#define D_DU_REG_FBATC_SWCNT				0xB1202000		/* SWCNT		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_SWCMON				0xB1202004		/* SWCMON		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_UNCMON				0xB1202008		/* UNCMON		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACCMON				0xB120200C		/* ACCMON		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_SWTIME				0xB1202010		/* SWTIME		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_SWINTVL				0xB1202014		/* SWINTVL		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_SWGUARD				0xB1202018		/* SWGUARD		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACMODE				0xB1202020		/* ACMODE		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACLEN				0xB1202024		/* ACLEN		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_HSMODE				0xB1202028		/* HSMODE		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_HSLEN				0xB120202C		/* HSLEN		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_HSCONTINUED			0xB1202100		/* HSCONTINUED	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_AFLGMON				0xB1202104		/* AFLGMON		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACSUBDCO				0xB1202140		/* ACSUBDCO		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACSUBDLY				0xB1202144		/* ACSUBDLY		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACSUBOLP				0xB1202148		/* ACSUBOLP		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACSUBUPD				0xB120214C		/* ACSUBUPD		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACSUBRST				0xB1202150		/* ACSUBRST		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACDCO				0xB1202200		/* ACDCO		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACDLY				0xB1202204		/* ACDLY		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACOLP				0xB1202208		/* ACOLP		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACDLYMBA				0xB1202240		/* ACDLYMBA		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACOLPMBA				0xB1202244		/* ACOLPMBA		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACDLYMBP				0xB1202248		/* ACDLYMBP		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACOLPMBP				0xB120224C		/* ACOLPMBP		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_AKRMDLY				0xB1202260		/* AKRMDLY		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_AKRMOLP				0xB1202264		/* AKRMOLP		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_AKRMUPD				0xB1202268		/* AKRMUPD		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACWARNA				0xB1202280		/* ACWARNA		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_AKRMLNDMON			0xB1202300		/* AKRMLNDMON	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_AKRMDLYMON			0xB1202304		/* AKRMDLYMON	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_AKRMOLPMON			0xB1202308		/* AKRMOLPMON	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_AKRMUPDMON			0xB120230C		/* AKRMUPDMON	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_PACINFO				0xB1202400		/* PACINFO		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_PACVER				0xB1202404		/* PACVER		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_DEVICE				0xB1202408		/* DEVICE		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ABT					0xB1202440		/* ABT			(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ABTMON				0xB1202444		/* ABTMON		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_SEQMON				0xB1202448		/* SEQMON		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_SWTIMEMON			0xB120244C		/* SWTIMEMON	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACTESTOLP			0xB1202700		/* ACTESTOLP	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACTEST				0xB1202704		/* ACTEST		(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_ACTESTTRG			0xB1202708		/* ACTESTTRG	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_FBLASTMODI			0xB12027F8		/* FBLASTMODI	(sub6 : o / mmw : -)*/
#define D_DU_REG_FBATC_UNMONSEL				0xB12027FC		/* UNMONSEL		(sub6 : o / mmw : -)*/
/* @} */
#endif
/* @} */
