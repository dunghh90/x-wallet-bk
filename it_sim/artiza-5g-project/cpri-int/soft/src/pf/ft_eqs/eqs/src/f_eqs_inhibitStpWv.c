/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_inhibitStpWv.c
 *  @brief  Inhibit Stop Wave function
 *  @date   2019/03/28 FJT)Koshida create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_EQS
* @{ */
#include "f_eqs_header.h"			/* 5GDU-LLS�Ď��^�X�N�w�b�_�t�@�C��			*/

/* @{ */
/**
* @brief API Inhibit Stop Wave Notify process function
* @note Inhibit Stop Wave control regiseter.\n
* @param none
* @return None
* @date   2019/03/27 FJT)Koshida create
* @warning	N/A
* @FeatureID	
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/

VOID f_eqs_inhibitStpWv( VOID )
{
	
	/*	��g�}�~�t���O��OFF�̏ꍇ	*/
	if(f_eqs_StpWv_inhbitFlg == E_EQS_STPWVMSK_OFF)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "Stop wave mask ON(%d)", f_eqs_StpWv_inhbitFlg);
		/*	��g�}�~�t���O��ON����	*/
		f_eqs_StpWv_inhbitFlg = E_EQS_STPWVMSK_ON;
		/*	��g��}�~����	*/
		f_eqs_stpWvProc( f_eqsw_tra_sv_newsta.sv_inf );
	}
	else
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "Stop wave mask double error(%d)", f_eqs_StpWv_inhbitFlg);
	}
	return;
}

/* @} */

/* @} */

