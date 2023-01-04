/*******************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_cm_ram.c
 *  @brief  RAM Table for common function
 *  @date   2008/07/18 FFCS)Tangj create
 *  @date   2009/03/05  FFCS)Wuh modify for M-S3G-eNBPF-01471
 *					    stop writing FLASH after receiving CPRI message
 *  @date   2009/04/16  FFCS)Wuh modify for S3G PRC CR-xxx-xxx (CPRI spec V1.07)
 *  @date   2009/06/09  FFCS)Wuh modify for M-S3G-eNBPF-02047
 *                      RE version response incorrect
 *	@date	2009/06/10  FFCS)Wangjuan modify for M-S3G-eNBPF-02050
 *						BRE card can not connect in SV-MT/FLD-MT debug test
 *  @date   2009/06/16  FFCS)Wuh modify for M-S3G-eNBPF-02086, 
 *                      modify for FLASH erase in file no new case faild
 *  @date   2009/08/10  FFCS)Niemsh modify for CR-00054-005, 
 *                      add common variable:cmw_ftpulstpflg,cmw_ftpulmaxmsgq
 *  @date   2009/12/27 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
 *  @date   2015/10/29 ALPHA)Fujiwara TDD-RRE(Zynq)対応
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */
/*******************************************************************************/


/** @addtogroup RRH_PF_COM
 *  @{
 */
#ifndef F_COM_ROM_H
#define F_COM_ROM_H

/* 2021/01/19 M&C) Merge 4G FHM src (start add) */
extern const	T_COM_HARDIDXTBL		f_comr_hardalm_idx_tbl	[D_COM_HARDALM_MAX];
extern const	USHORT		f_comr_syspara_typval_tbl[D_SYS_SYSPARA_MAX];
extern const	T_COM_LOG_SAVE_TBL			f_comr_flash_log_save_info_fhm[15];
extern const	UCHAR	f_comr_runLogPath[ D_SYS_MAX_SYSNUM ][24];
/* 2021/01/19 M&C) Merge 4G FHM src (end add) */

extern const	T_COM_LOG_SAVE_TBL			f_comr_flash_log_save_info[D_RRH_FLASH_LOG_MAX];

extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptAdmTbl[D_DU_SVCTL_ON_ADM_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptEngTbl[D_DU_SVCTL_ON_ENG_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptOpeTbl[D_DU_SVCTL_ON_OPE_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptAvlTbl[D_DU_SVCTL_ON_AVL_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptUsaTbl[D_DU_SVCTL_ON_USA_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptUseAsTbl[D_DU_SVCTL_ON_USE_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptUseAmTbl[D_DU_SVCTL_ON_USE_NUM_MMW];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptUseBsTbl[D_DU_SVCTL_ON_USE_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptUseBmTbl[D_DU_SVCTL_ON_USE_NUM_MMW];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptUseCTbl[D_DU_SVCTL_ON_USE_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptUseDTbl[D_DU_SVCTL_ON_USE_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFlvTbl[E_DU_SVCTL_FLVNO_MAX];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFidTbl[E_DU_SVCTL_OCCUR_FIDNO_MAX];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid01Tbl[D_DU_FLTID_01_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid02Tbl[D_DU_FLTID_02_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid03Tbl[D_DU_FLTID_03_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid04sTbl[D_DU_FLTID_04_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid04mTbl[D_DU_FLTID_04_NUM_MMW];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid05Tbl[D_DU_FLTID_05_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid06Tbl[D_DU_FLTID_06_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid07sTbl[D_DU_FLTID_07_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid07mTbl[D_DU_FLTID_07_NUM_MMW];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid08Tbl[D_DU_FLTID_08_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF10AntAsTbl[D_DU_FLTID_10_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF10AntAmTbl[D_DU_FLTID_10_AB_NUM_MMW];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF10AntBsTbl[D_DU_FLTID_10_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF10AntBmTbl[D_DU_FLTID_10_AB_NUM_MMW];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF10AntCTbl[D_DU_FLTID_10_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF10AntDTbl[D_DU_FLTID_10_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF11AntAsTbl[D_DU_FLTID_11_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF11AntAmTbl[D_DU_FLTID_11_AB_NUM_MMW];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF11AntBsTbl[D_DU_FLTID_11_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF11AntBmTbl[D_DU_FLTID_11_AB_NUM_MMW];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF11AntCTbl[D_DU_FLTID_11_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF11AntDTbl[D_DU_FLTID_11_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF12AntATbl[D_DU_FLTID_12_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF12AntBTbl[D_DU_FLTID_12_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF12AntCTbl[D_DU_FLTID_12_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF12AntDTbl[D_DU_FLTID_12_NUM_SB6];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptF10VHTbl[D_DU_FLTID_10_VH_NUM_MMW];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid30Tbl[D_DU_FLTID_30_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid31Tbl[D_DU_FLTID_31_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid40Tbl[D_DU_FLTID_40_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid41Tbl[D_DU_FLTID_41_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid42Tbl[D_DU_FLTID_42_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid43Tbl[D_DU_FLTID_43_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid44Tbl[D_DU_FLTID_44_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid50Tbl[D_DU_FLTID_50_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid51Tbl[D_DU_FLTID_51ALM_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFid51NoTbl[D_DU_FLTID_51NO_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFidf0Tbl[D_DU_FLTID_F0_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFidf1Tbl[D_DU_FLTID_F1_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFidf2Tbl[D_DU_FLTID_F2_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFidffTbl[D_DU_FLTID_FF_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptFlDwnldTbl[D_DU_SVCTL_FL_DWNLD_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptStpWTbl[D_DU_SVCTL_ON_WAVE_STOP_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptSyncETbl[D_DU_SVCTL_ON_SYNCE_STATE_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptSplanTbl[D_DU_SVCTL_ON_SPLAN_NUM];
extern const T_COM_DU_SVBUS_CTRL_INFO_TBL f_comr_svOptSyncTbl[D_DU_SVCTL_ON_SYNC_STATE_NUM];

extern const T_COM_DU_SVOPT_DEF_TBL f_comr_svOptMapDefTbl[D_DU_SVINF_NUM];


/* @} */

#endif
/* @} */
