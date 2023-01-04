/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cm_ext.h
 *  @brief	TRA process external common table declaration
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
 *  @date   2009/12/27 QNET)Kabasima MOD QNET)Kabasima MOD CR-00081-001:[DCM����]RE�n�[�h�\�t�g�C���^�t�F�[�X�d�l��v2.13�Ń����[�X
 *  @date   2010/01/04 QNET)Kabasima ADD M-S3G-eNBPF-02869:[�i�����P]RE�f�o�b�O�R�}���h�@�\�g�[
 *  @date   2010/06/01 FJT)Taniguchi CR-00091-001:[DCM����]RE�n�[�h�E�\�t�g�C���^�t�F�[�X�d�l��v2.18�Ń����[�X
 *  @date   2010/11/02 CMS)Takahashi CR-XXX
 *  @date   2012/10/18 FFCS)niemsh modify for M-S3G-eNBSYS-02684
 *  @date   2013/09/30 FFCS)Xut Modify for 13B-03-005-04
 *  @date   2013/10/10 FJT)Matsumoto 13B-03-005 13B���Ή�(TRA)
 *  @date   2015/04/25 FFCS)Zhangxq TDD
 *  @date   2015/09/08 ALP)Ueda modify for TDD-RRE(Zynq)
 *  @date   2015/10/22 ALP)Ueda modify for TDD-RRE(Zynq)
 *  @date   2021/01/19 M&C)Huan.dh add cmw_connectoperatemode
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/******************************************************************************************************************************/

/* For multiple include */

#ifndef    M_CM_EXT
#define    M_CM_EXT
/** @addtogroup RRH_DCM
* @{ */

/****************************************************************************/
/*																			*/
/*	Common table declaration												*/
/*																			*/
/****************************************************************************/
extern	CMT_SYSMNGTBL	cmw_sys_mng_tbl[CMD_MAX_SYSNUM];
								/**< System parameter management table  */
/** @name SV state management table */
/* @{ */

extern CMT_TRASVTBL		cmw_tra_sv_tbl; 		/**< TRA SV state management table	*/
/* @} */
/* TDD SRE  TRX setting S. */								
extern	CMT_TRXSETTBL	cmw_trxset_tbl;
									/**< TRX setting status management table  */
/* TDD SRE  TRX setting E. */
									
extern	UINT			cmw_card_car_onoff_tid[CMD_MAX_SYSNUM];
									/**< USE��ԕω��^�C�}�Ǘ��e�[�u��  */
									
extern CMT_CARDCNTTBL	cmw_cardcnt_tbl;
										/**< TRA card control table  */
									
extern	CMT_TOFFSETTBL	cmw_toffset_tbl[CMD_MAX_SYSNUM];		
						/**< Toffset management table  */
						
extern	CMT_REDEVCOMPTBL	cmw_re_dev_tbl;
						/**< RE device management table  */
						
										/**< task state management table  */
extern	CMT_RECLSMNGTBL	cmw_reclsmngtbl;
						/**< RE common link information management table  */

/* 2020/12/28 M&C) Merge 4G FHM src (start modify) */
// extern	CMT_CPRILNKSTATBL	cmw_cprilnkstatbl;
extern CMT_CPRILNKSTATBL    cmw_cprilnkstatbl[D_RRH_CPRINO_NUM];
				/**< CPRI state management table  */
/* 2020/12/28 M&C) Merge 4G FHM src (end modify) */

extern	CMT_CPRILNKSTATBL	cmw_cprilnkstatbl_cscd;
				/**< CPRI state(cps) management table  */
				
extern	UINT			cmw_mtaddr_reset;				/**< MT address reset flag for MTM*/
extern UINT					cmw_mkaddr_reset;			/**< MT address reset flag for MKM */
					
extern	CMT_TRAFILEINFTBL	cmw_trafile_inf_tbl;
					/**< TRA file information table  */
					
extern	CMT_CASCADEINFOTBL*	cmw_cascade_info_tbl;	/* cascade info table */
extern	T_RRH_LOG_WDT	* cmw_wdt_log;				/* wdt log table */

/****************************************************************************/
/*																			*/
/*							���ʃe�[�u���錾								*/
/*																			*/
/****************************************************************************/

/****************************************************************************/
/*																			*/
/*							�ʃe�[�u���錾								*/
/*																			*/
/****************************************************************************/

/* TRA extend SV info */
extern CMT_TRAEXTENDSV_TBL	cmw_tra_extendsv_tbl;

/*!
 * @name the flag of first startup and use download
 */

/* File information table */
extern	CMT_FILEINFREP_TBL	cmw_file_inf_tbl[CMD_MAX_SYSNUM];	/**< File information table   */

/* �ڑ��\�� info */
extern UINT cmw_connectinfo;

/* Connected Operation mode */
extern UINT cmw_connectoperatemode;

/* SV-MT/FLD-MT connect enable flag for BRE card */
extern UINT cmw_dbg_bremtconnflg;

/* FLD UINTtransfer stop flg for upload case */
extern UINT cmw_ftpulstpflg;

/* FLD ftp transfer stop flg for upload case */
extern INT cmw_ftpulmaxmsgq;

extern CMT_HWSETSYSTBL		cmw_hwsetsys_tbl;		/**< system parameter which is set to hardware management table  */

extern UINT cmw_SysparaSetting_flag;

extern UINT cmw_device_kind;				/* device kind */

extern UINT cmw_SysparaSetting_flag;
/* valid system info */
extern UINT cmw_validsysinfo;

/* 2020/12/28 M&C) Merge 4G FHM src (start modify) */
// extern UINT cmw_hcsta[CMD_MAX_SYSNUM];
extern UINT cmw_hcsta[D_RRH_CPRINO_NUM][CMD_MAX_SYSNUM];
/* 2020/12/28 M&C) Merge 4G FHM src (end modify) */

extern UINT cmw_hcsta_cscd[CMD_MAX_SYSNUM];
/* @} */

#endif	/* M_CM_EXT */

