/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eio_typ.h
 *  @brief  5GDU-LLS external IO supervision & cntrol common structure head file defination
 *  @date   2019/03/19 FJT)Koshida create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019-
 */
/*********************************************************************************/

/* ìÒèdéÊçûÇ›ñhé~ */
#ifndef		F_EIO_TYP
#define		F_EIO_TYP

/** @addtogroup RRH_PF_EIO
* @{ */


/**
 * @brief	external IO manage table structure
 * @note	Contents:  external IO manage table structure \m
 * 
 */

/* @{ */
typedef struct{
	UINT					svStart;
	UINT					lineInReg;
	UINT					lineInRep;
	UINT					lineOutReg;
	UINT					lineInDefLog;
	UINT					lineInDefCnt;
}T_EIOW_EXTIO;
/* @} */

/**
 * @brief	EIO FAN Cntorl manage table structure
 * @note	Contents:  EIO FAN Cntorl manage table structure \m
 * 
 */

/* @{ */
typedef struct{
	UINT					fanMaxCnt;
	UINT					statusNow;
	UINT					statusNxt;
	UINT					fanCtrlReserve;
}T_EIOW_FANCTRL;
/* @} */

/**
 * @brief	EIO FAN SV manage table structure
 * @note	Contents:  EIO FAN SV manage table structure \m
 * 
 */

/* @{ */
typedef struct{
	UINT					abnormalCnt;
	UINT					normalCnt;
	UINT					faultCnt;
	UINT					fanSpeedReserv;
}T_EIOW_FANSV_SPEED;

typedef struct{
	UINT					unMountCnt;
	UINT					mountCnt;
	UINT					psAbnCnt;
	UINT					psNorCnt;
}T_EIOW_FANSV_UNIT;

typedef struct{
	T_EIOW_FANSV_UNIT		fanUnit;
	T_EIOW_FANSV_SPEED		fanSpeed[E_DU_FAN_MAXNO];
}T_EIOW_FANSV;

typedef struct{
	UINT					fanToCnt;
	T_EIOW_FANCTRL			fanCtrl;
	T_EIOW_FANSV			fanSv;
}T_EIOW_FANMNG;

/* @} */


/**
 * @brief	EIO FAN Unit SV Info structure
 * @note	Contents:  EIO FAN Unit SV Info structure \m
 * 
 */

/* @{ */
typedef struct{
	UINT					regChkBit;
	UINT					stsBit;
	UINT					faultProtect;
	UINT					normalProtect;
	UINT					faultID;
	UINT					faultFact;
	UINT					*faultCnt;
	UINT					*normalCnt;
	UCHAR					fltLogMsg[32];
	UCHAR					norLogMsg[32];
}T_EIOR_FANUNIT_SV_INFO;

/* @} */

/**
 * @brief	EIO FAN Unit SV Info structure
 * @note	Contents:  EIO FAN Unit SV Info structure \m
 * 
 */

/* @{ */
typedef struct{
	UINT					regAddr;
	UINT					stsBit;
	UINT					resetBit;
	UINT					fltId;
	UINT					fltFct;
}T_EIOR_FANSPEED_INFO;

/* @} */

/**
 * @brief	EIO Ant Info structure
 * @note	Contents:  EEIO Ant Info structure \m
 * 
 */

/* @{ */
typedef struct{
	UINT					powref;
	UINT					change_cnt;
}T_EIO_ANT_INFO;

/* @} */

/* @} */

#endif	/* F_EIO_TYP */

