/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eio_rom.c
 *  @brief  supervision and control external IO ROM table
 *  @date   2019/03/18 FJT)Koshida create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_EIO
* @{ */

/****************************************************************************/
/*		define(s)/include(s)												*/
/****************************************************************************/
#include "f_eio_header.h"			/* 外部IOタスクヘッダファイル			*/

VOID (*const f_eior_matrix[E_EIO_STAT_MAX][E_EIO_EVT_MAX])(VOID* pRcvMsg) = 
{
	/* E_EIO_STAT_INI	*/
	{
		f_eio_nop /* f_eio_initProc */,	/**< Thread Create								*/
		f_eio_nop,						/**< External IO SV timer timeout indication	*/
		f_eio_nop,						/**< FAN SV & Cntrol timer timeout indication	*/
		f_eio_nop,						/**< FAN debug mode indication					*/
		f_eio_nop,						/**< FAN DAC chage indication					*/
		f_eio_nop						/**< MSI message receive notify					*/
	},
	
	/* E_EIO_STAT_RUN	*/
	{
		f_eio_nop,						/**< Thread Create								*/
		f_eio_msgExtTimOutInd,			/**< External IO SV timer timeout indication	*/
		f_eio_msgFanTimOutInd,			/**< FAN SV & Cntrol timer timeout indication	*/
		f_eio_apiFanDbgInd,				/**< FAN debug mode indication					*/
		f_eio_apiFanDacInd,				/**< FAN DAC chage indication					*/
		f_eio_MSIanlz					/**< MSI message receive notify					*/
	}
};

/** @name EIO FANUNIT SV INFO */
/* @{ */
const	T_EIOR_FANUNIT_SV_INFO		f_eior_fanUnitSvInfo[E_EIO_FANITEM_MAXNO] = {
	{
		D_DU_REGBIT_CTB_STA1_FANUNMOUNT,					/*	register chkbit			*/
		D_DU_BIT_FAN_UNMOUNT,								/*	faultSts bit			*/
		D_EIO_PROTECTCNT_FANUNMOUNT,						/*	fault Protect Number	*/
		D_EIO_PROTECTCNT_FANMOUNT,							/*	normal Protect Number	*/
		E_DU_SVCTL_OPE_FID40_ALM,							/*	fault ID				*/
		D_DU_FLTID_NO_FAN_DETECT,							/*	fault factor			*/
		&f_eiow_fanMngTbl.fanSv.fanUnit.unMountCnt,			/*	fault counter			*/
		&f_eiow_fanMngTbl.fanSv.fanUnit.mountCnt,			/*	normal counter			*/
		"FAN UNMOUNT Occur",								/*	fault log message		*/
		"FAN MOUNT Occur",									/*	normal log message		*/
	},
	{
		D_DU_REGBIT_CTB_STA1_FANPSALM,						/*	register chkbit			*/
		D_DU_BIT_FAN_PSALM,									/*	faultSts bit			*/
		D_EIO_PROTECTCNT_FANPSFLT,							/*	fault Protect Number	*/
		D_EIO_PROTECTCNT_FANPSNOR,							/*	normal Protect Number	*/
		E_DU_SVCTL_OPE_FID41_ALM,							/*	fault ID				*/
		D_DU_FLTID_FAN_PS_ALM,								/*	fault factor			*/
		&f_eiow_fanMngTbl.fanSv.fanUnit.psAbnCnt,			/*	fault counter			*/
		&f_eiow_fanMngTbl.fanSv.fanUnit.psNorCnt,			/*	normal counter			*/
		"FAN PS Fault Occur",								/*	fault log message		*/
		"FAN PS Fault Clear",								/*	normal log message		*/
	}
};

/* @} */

/** @name EIO FAN DAC Vaule */
/* @{ */
const	UINT	f_eior_fanDacVal[E_DU_FANSPEEDSTS_MAXNO] = {
		D_DU_FANDACVAL_STOP,	/**< FAN停止状態DAC値	*/
		D_DU_FANDACVAL_LOW,		/**< FAN低電圧状態DAC値	*/
/*		D_DU_FANDACVAL_HIGH,	*//**< FAN高電圧状態DAC値	*//*	ハソv1.05削除	*/
/*		D_DU_FANDACVAL_MAX		*//**< FAN MAX状態DAC値	*//*	ハソv1.05削除	*/
};
/* @} */

/** @name EIO FAN speed sv info */
/* @{ */
const	T_EIOR_FANSPEED_INFO	f_eior_fanSpeedInfo[E_DU_FAN_MAXNO] = {
	{
		D_DU_REG_CTB_TAC1CNT,		/**< FAN#1:0xA000_03C0			*/
		D_DU_BIT_FAN1,				/**< FAN#1 bit					*/
		D_DU_BIT_FAN1,				/**< FAN#1 bit					*/
		E_DU_SVCTL_OPE_FID42_ALM,	/**< faultID#42					*/
		D_DU_FLTID_COOL_FAN1_BROK	/**< Cooling fan broken FAN#1	*/
	},
	{
		D_DU_REG_CTB_TAC2CNT,		/**< FAN#2:0xA000_03C4			*/
		D_DU_BIT_FAN2,				/**< FAN#2 bit					*/
		D_DU_BIT_FAN2,				/**< FAN#2 bit					*/
		E_DU_SVCTL_OPE_FID43_ALM,	/**< faultID#43					*/
		D_DU_FLTID_COOL_FAN2_BROK	/**< Cooling fan broken FAN#2	*/
	},
	{
		D_DU_REG_CTB_TAC3CNT,		/**< FAN#3:0xA000_03C8			*/
		D_DU_BIT_FAN3,				/**< FAN#3 bit					*/
		D_DU_BIT_FAN3,				/**< FAN#3 bit					*/
		E_DU_SVCTL_OPE_FID44_ALM,	/**< faultID#44					*/
		D_DU_FLTID_COOL_FAN3_BROK	/**< Cooling fan broken FAN#3	*/
	}
};
/* @} */

/* @} */
