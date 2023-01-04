/********************************************************************************************************************/
/*!
 * @skip  $ld:$
 * @file  f_aih_typ.h
 * @brief AIH  Type Define Header
 * @date  2019/08/09 FJ)Takahashi Create
 *
 * All Rights Reserved, Copyright FUJITSU LIMITED 2019
 */
/********************************************************************************************************************/

/*!
 * @addtogroup RRH_PF_AIH
 * @{
 */

#ifndef F_AIH_TYP
#define F_AIH_TYP



typedef struct 
{
	UINT	regAddr;  /* addr of ALD power control reg */
	UINT	regDat;   /* value of ALD pwr ctrl */
}AIH_PWRCTRL_IF;


typedef struct {
	UCHAR		pI;
	UCHAR		pL;
	UCHAR		pV[1];
}T_AIH_PI_PL_PV;


typedef struct {
	UCHAR		pI;
	UCHAR		pL;
}T_AIH_PI_PL;

typedef struct {
	UCHAR			addr;
	UCHAR			ctl;
	UCHAR			fI;
	UCHAR			gI;
	UCHAR			gL;
}T_AIH_HDLC_XID_MSG;

typedef struct {
	UCHAR			fI;
	UCHAR			gI;
	UCHAR			gL;
	T_AIH_PI_PL_PV	pI_pL_pV[1];
}T_AIH_XID_MSG;

#endif
/* @} */
