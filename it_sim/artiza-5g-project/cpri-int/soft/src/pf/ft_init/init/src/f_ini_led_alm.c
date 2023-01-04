/*!
 * @skip  $ld:$
 * @file  f_ini_led_alm.c
 * @brief ALM-LED点灯
 * @data  2014/12/18 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  2015/09/08 ALPHA) kamada add＆mod RRH-006-000 対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"
#include "BPF_COM_END.h"
#include <sys/wait.h> 

/*!
 * @brief	f_ini_led_alm
 * @note 	関数処理内容.
 *			-# ALM-LEDを点灯する。
 * @param  
 * @warning		N/A
 * @FeatureID	PF-Init-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2014/12/18 ALPHA) okabe Create RRH-007-000 共通ppc対応
 */
VOID f_ini_led_alm(VOID* buf_p)
{
	UINT	regdat;
	UINT	setdat;

	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SYSLED, &regdat);
	setdat = regdat & 0x000000F0;
	setdat |= 0x1;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_DU_REG_CTB_SYSLED, &setdat);
	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_CRITICAL, "LED RED ON(0x%08x->0x%08x)",regdat,setdat );
	return;
}

/* @} */
