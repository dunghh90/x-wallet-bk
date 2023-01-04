/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	ROM Table
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also TableSpecifications(dpda)_rom.xls
 * 
 *  @date 2014/03/03 ALPHA)藤井 Create
 *  @date 2015/04/22 ALPHA)近間 RRH-013-000 DCM_3.5G対応
 *  @date 2015/06/19 ALPHA)近間 RRH-007-000 DCM_3.5G対応
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2014-2015
 */
/******************************************************************************************************************************/

#include	"f_dpda_inc.h"

/** @addtogroup RRH_PF_RFP
 *  @{
 */
/********************************************************************************************************************/
/* アラーム関連																										*/
/********************************************************************************************************************/
/*!
 * @name ALM Threshold初期設定値
 * @note ALM Threshold初期設定値
 * @date 2015/08/06 TDI)satou 「High Temparature 発生閾値」以外削除
 * @{
 */
const	T_DPDA_ALM_THRESHOLD_INFO	f_dpdar_almThreshold[E_DPDA_ALM_THRESH_NUM_MAX] = {
	/* High Temperature 発生閾値 */
	{
		0,								/* アンテナ数	*/
		0,								/* キャリア数	*/
		"High Temperature",				/* 閾値名称		*/
		100								/* 閾値(初期値)	*/
	}
};
/** @} */

/*!
 * @name DPDA Alarm Information Table
 * @note DPDA Alarm Information Table
 * @{
 */
const	T_DPDA_ALM_INFO	f_dpdar_almInfo[D_DPDA_ALM_NUM] = {
	/* TEMP ALM(温度異常) */
	{
		"TEMP ALM",
		{
			{																	/* アラーム要因1		*/
				E_DPDA_ALM_FACTOR_OPE_OR,
				(UINT *) &f_dpdaw_statusInfo.Temperature,
				(UINT *) &f_dpdaw_almThreshold[E_DPDA_ALM_THRESH_HIGH_TEMP][D_DPDA_ANTA],
				E_DPDA_ALM_OPE_GE_SIGNED
			}
		},
		1,																		/* アラーム要因数		*/
		5,																		/* アラーム発生回数		*/
		0,																		/* アラーム回復回数		*/
		D_SYS_ON,																/* 回復なしフラグ		*/
		D_DPDA_ALM_INST_HIGH_TEMP												/* Instance Id			*/
	}
};
/** @} */

/************************************************************************************************/
/* register address table																		*/
/************************************************************************************************/
/*!
 * @name register address Table
 * @note register address Table
 * @date 2015/08/06 TDI)satou FHM用に全面見直し
 * @{
 */
const UINT f_dpdar_reg_Tbl[E_RRH_REG_NUM_MAX] = 
{
    D_RRH_REG_CNT_SV1INF,                               /* SV情報1 */
    D_RRH_REG_CNT_SV2INF,                               /* SV情報2 */
    D_RRH_REG_CNT_SV3INF,                               /* SV情報3 */
    D_RRH_REG_CNT_SV4INF,                               /* SV情報4 */
    D_RRH_REG_CNT_SV5INF,                               /* SV情報5 */
    D_RRH_REG_CNT_SV6INF,                               /* SV情報6 */
    D_RRH_REG_CNT_SV7INF,                               /* SV情報7 */
    D_RRH_REG_CNT_SV8INF,                               /* SV情報8 */
    D_RRH_REG_CNT_SV9INF,                               /* SV情報9 */
    D_RRH_REG_CNT_SV10INF,                              /* SV情報10 */
    D_RRH_REG_CNT_SV11INF,                              /* SV情報11 */
    D_RRH_REG_CNT_SV12INF,                              /* SV情報12 */
    D_RRH_REG_CNT_SV13INF,                              /* SV情報13 */
    D_RRH_REG_CNT_SV14INF,                              /* SV情報14 */
    D_RRH_REG_CNT_SV15INF,                              /* SV情報15 */
    D_RRH_REG_CNT_SV16INF,                              /* SV情報16 */
    M_RRH_REG_CPRI_CPSTAT(0),                           /* CPRIステート表示(port0) */
    M_RRH_REG_CPRI_CPSTAT(1),                           /* CPRIステート表示(port1) */
    M_RRH_REG_CPRI_CPSTAT(2),                           /* CPRIステート表示(port2) */
    M_RRH_REG_CPRI_CPSTAT(3),                           /* CPRIステート表示(port3) */
    M_RRH_REG_CPRI_CPSTAT(4),                           /* CPRIステート表示(port4) */
    M_RRH_REG_CPRI_CPSTAT(5),                           /* CPRIステート表示(port5) */
    M_RRH_REG_CPRI_CPSTAT(6),                           /* CPRIステート表示(port6) */
    M_RRH_REG_CPRI_CPSTAT(7),                           /* CPRIステート表示(port7) */
    M_RRH_REG_CPRI_CPSTAT(8),                           /* CPRIステート表示(port8) */
    M_RRH_REG_CPRI_CPSTAT(9),                           /* CPRIステート表示(port9) */
    M_RRH_REG_CPRI_CPSTAT(10),                          /* CPRIステート表示(port10) */
    M_RRH_REG_CPRI_CPSTAT(11),                          /* CPRIステート表示(port11) */
    M_RRH_REG_CPRI_CPSTAT(12),                          /* CPRIステート表示(port12) */
    M_RRH_REG_CPRI_CPSTAT(13),                          /* CPRIステート表示(port13) */
    M_RRH_REG_CPRI_CPSTAT(14),                          /* CPRIステート表示(port14) */
    M_RRH_REG_CPRI_CPSTAT(15),                          /* CPRIステート表示(port15) */
    M_RRH_REG_CPRI_CPSTAT(16),                          /* CPRIステート表示(port16) */
    D_RRH_REG_PCI_BRDG_STS_CTL,                         /* Bridge Status and Control        */
    D_RRH_REG_PCI_PHY_STS_CTL,                          /* PHY Status/Control               */
    D_RRH_REG_PCI_ROOTPORT_STS_CTL,                     /* Root Port Status/Control         */
    D_RRH_REG_PCI_ROOTPORT_ERR_FIFO,                    /* Root Port Error FIFO Read        */
    D_RRH_REG_PCI_ROOTPORT_INT_FIFO1,                   /* Root Port Interrupt FIFO Read1   */
    D_RRH_REG_PCI_ROOTPORT_INT_FIFO2                    /* Root Port Interrupt FIFO Read2   */
};

/*!
 * @name Virtual register address Table
 * @note Virtual register address Table
 * @{
 */
const   UINT    f_dpdar_virtualRegAddr[D_DPDA_DBG_VIRTUAL_REG_CNT] = {
    D_RRH_REG_CNT_SV1INF,                               /* SV情報1 */
    D_RRH_REG_CNT_SV2INF,                               /* SV情報2 */
    D_RRH_REG_CNT_SV3INF,                               /* SV情報3 */
    D_RRH_REG_CNT_SV4INF,                               /* SV情報4 */
    D_RRH_REG_CNT_SV5INF,                               /* SV情報5 */
    D_RRH_REG_CNT_SV6INF,                               /* SV情報6 */
    D_RRH_REG_CNT_SV7INF,                               /* SV情報7 */
    D_RRH_REG_CNT_SV8INF,                               /* SV情報8 */
    D_RRH_REG_CNT_SV9INF,                               /* SV情報9 */
    D_RRH_REG_CNT_SV10INF,                              /* SV情報10 */
    D_RRH_REG_CNT_SV11INF,                              /* SV情報11 */
    D_RRH_REG_CNT_SV12INF,                              /* SV情報12 */
    D_RRH_REG_CNT_SV13INF,                              /* SV情報13 */
    D_RRH_REG_CNT_SV14INF,                              /* SV情報14 */
    D_RRH_REG_CNT_SV15INF,                              /* SV情報15 */
    D_RRH_REG_CNT_SV16INF,                              /* SV情報16 */
    M_RRH_REG_CPRI_CPSTAT(0),                           /* CPRIステート表示(port0) */
    M_RRH_REG_CPRI_CPSTAT(1),                           /* CPRIステート表示(port1) */
    M_RRH_REG_CPRI_CPSTAT(2),                           /* CPRIステート表示(port2) */
    M_RRH_REG_CPRI_CPSTAT(3),                           /* CPRIステート表示(port3) */
    M_RRH_REG_CPRI_CPSTAT(4),                           /* CPRIステート表示(port4) */
    M_RRH_REG_CPRI_CPSTAT(5),                           /* CPRIステート表示(port5) */
    M_RRH_REG_CPRI_CPSTAT(6),                           /* CPRIステート表示(port6) */
    M_RRH_REG_CPRI_CPSTAT(7),                           /* CPRIステート表示(port7) */
    M_RRH_REG_CPRI_CPSTAT(8),                           /* CPRIステート表示(port8) */
    M_RRH_REG_CPRI_CPSTAT(9),                           /* CPRIステート表示(port9) */
    M_RRH_REG_CPRI_CPSTAT(10),                          /* CPRIステート表示(port10) */
    M_RRH_REG_CPRI_CPSTAT(11),                          /* CPRIステート表示(port11) */
    M_RRH_REG_CPRI_CPSTAT(12),                          /* CPRIステート表示(port12) */
    M_RRH_REG_CPRI_CPSTAT(13),                          /* CPRIステート表示(port13) */
    M_RRH_REG_CPRI_CPSTAT(14),                          /* CPRIステート表示(port14) */
    M_RRH_REG_CPRI_CPSTAT(15),                          /* CPRIステート表示(port15) */
    M_RRH_REG_CPRI_CPSTAT(16),                          /* CPRIステート表示(port16) */
    D_RRH_REG_PCI_BRDG_STS_CTL,                         /* Bridge Status and Control        */
    D_RRH_REG_PCI_PHY_STS_CTL,                          /* PHY Status/Control               */
    D_RRH_REG_PCI_ROOTPORT_STS_CTL,                     /* Root Port Status/Control         */
    D_RRH_REG_PCI_ROOTPORT_ERR_FIFO,                    /* Root Port Error FIFO Read        */
    D_RRH_REG_PCI_ROOTPORT_INT_FIFO1,                   /* Root Port Interrupt FIFO Read1   */
    D_RRH_REG_PCI_ROOTPORT_INT_FIFO2,                   /* Root Port Interrupt FIFO Read2   */
    D_DPDA_DBG_VREG_ADDR_OFFSET_TEMP                    /* 温度情報(疑似レジスタのための offset) */
};
/** @} */

/*!
 * @name Virtual register info value table
 * @note Virtual register info value table
 * @{
 */
const   UINT    f_dpdar_virtualRegData[D_DPDA_DBG_VIRTUAL_REG_CNT] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000020                                  /* 温度情報                             */
};
/** @} */

/*!
 * @name alm mask check table
 * @note alm mask check table
 * @date 2015/08/07 TDI)satou 「温度異常アラーム」以外を削除
 * @{
 */
const T_DPDA_ALM_MSK_CHECK	f_dpdar_almmsk_check[ D_DPDA_ALM_NUM ] = 
{
	{1,0}				/*!< 温度異常アラーム					*/
};
/** @} */

/** @} */

/** @} */
