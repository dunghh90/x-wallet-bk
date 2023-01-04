/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_aih_rom.c
 *  @brief  supervision and control external IO ROM table
 *  @date   2019/08/01 FJT)Takahashi create
 *  @date   2019/09/13 FJT)Takahashi modify for IT2
 * 
 *  ALL Rights Reserved, Copyright FUJITSU Limited 2019-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_AIH
* @{ */

/****************************************************************************/
/*		define(s)/include(s)												*/
/****************************************************************************/
#include "f_aih_inc.h"			/* ALD(AISG)タスクヘッダファイル		*/

VOID (*const f_aihr_matrix[E_AIH_STAT_NUM][E_AIH_EVT_NUM])(VOID* pRcvMsg) = 
{
	/* E_AIH_STAT_INI */
	{
		f_aih_nop /* f_aih_init */, /**< Thread Create							*/
		f_aih_nop1,					/**< MSI data transmit req					*/
		f_aih_nop2,					/**< ALD transmission completion interrupt	*/
		f_aih_nop3,					/**< ALD reception completion interrupt		*/
		f_aih_nop4,					/**< ALD receive error interrupt			*/
		f_aih_nop5,					/**< ALD receive start interrupt			*/
		f_aih_nop6,					/**< ALD wait timer timeout interrupt		*/
		f_aih_msiAldPwrCtrl,		/**< ALD pwr ctrl							*/
		f_aih_msiAldPwrGet			/**< ALD pwr get							*/
	},
	
	/* E_AIH_STAT_IDLE */
	{
		f_aih_abort,				/**< Thread Create							*/
		f_aih_msiAldTransmit,		/**< MSI message receive notify				*/
		f_aih_nop2,					/**< ALD transmission completion interrupt	*/
		f_aih_nop3,					/**< ALD reception completion interrupt		*/
		f_aih_nop4,					/**< ALD receive error interrupt			*/
		f_aih_nop5,					/**< ALD receive start interrupt			*/
		f_aih_nop6,					/**< ALD wait timer timeout interrupt		*/
		f_aih_msiAldPwrCtrl,		/**< ALD pwr ctrl							*/
		f_aih_msiAldPwrGet			/**< ALD pwr get							*/
	},
	
	/* E_AIH_STAT_ALDSEND */
	{
		f_aih_abort,				/**< Thread Create							*/
		f_aih_msiAldTransmit,		/**< MSI message receive notify				*/
		f_aih_aldSendEnd,			/**< ALD transmission completion interrupt	*/
		f_aih_nop3,					/**< ALD reception completion interrupt		*/
		f_aih_nop4,					/**< ALD receive error interrupt			*/
		f_aih_aldRcvStart,			/**< ALD receive start interrupt			*/
		f_aih_aldBusyRcvTO,			/**< ALD wait timer timeout interrupt		*/
		f_aih_msiAldPwrCtrl,		/**< ALD pwr ctrl							*/
		f_aih_msiAldPwrGet			/**< ALD pwr get							*/
	},
	
	/* E_AIH_STAT_ALDRCV */
	{
		f_aih_abort,				/**< Thread Create							*/
		f_aih_msiAldTransmit,		/**< MSI message receive notify				*/
		f_aih_nop2,					/**< ALD transmission completion interrupt	*/
		f_aih_aldRcv,				/**< ALD reception completion interrupt		*/
		f_aih_nop4,					/**< ALD receive error interrupt			*/
		f_aih_aldRcvStart,			/**< ALD receive start interrupt			*/
		f_aih_aldRcvTO,				/**< ALD wait timer timeout interrupt		*/
		f_aih_msiAldPwrCtrl,		/**< ALD pwr ctrl							*/
		f_aih_msiAldPwrGet			/**< ALD pwr get							*/
	}
};



/* @} */
