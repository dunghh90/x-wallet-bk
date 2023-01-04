/*!
 * @skip  $ld:$
 * @file  mnt_com_ext.h
 * @brief L3 extent
 * @date  2013/11/28
 * @date  2015/04/25 FFCS)Zhangxq TDD
 * @date  2021/01/19 M&C)Huan.dh add cmw_connectoperatemode
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

/*!
 * @addtogroup RRH_L3
 * @{
 */
#ifndef L3_COM_EXT_H
#define L3_COM_EXT_H

extern CMT_CARDCNTTBL       cmw_cardcnt_tbl;
extern T_RRH_TRAINVTBL      cmw_tra_inv_tbl;
extern CMT_SHELFINVTBL      cmw_slf_inv_tbl;
extern CMT_TOFFSETTBL       cmw_toffset_tbl[CMD_MAX_SYSNUM];
extern CMT_REDEVCOMPTBL     cmw_re_dev_tbl;
extern CMT_RECLSMNGTBL      cmw_reclsmngtbl;
extern CMT_CPRILNKSTATBL    cmw_cprilnkstatbl[D_RRH_CPRINO_NUM];
extern UINT                 cmw_validsysinfo;
extern CMT_FILEINFREP_TBL   cmw_file_inf_tbl[CMD_MAX_SYSNUM];
extern UINT                 cmw_connectinfo;
extern UINT                 cmw_connectoperatemode;
extern CMT_HWSETSYSTBL      cmw_hwsetsys_tbl;
extern UINT                 cmw_device_kind;
extern UINT                 cmw_reKind;
extern UINT                 cmw_hcsta[D_RRH_CPRINO_NUM][CMD_MAX_SYSNUM];
extern UINT const           l3_comr_cprimsg_subTbl[D_L3_CPRIMSGNUM];
extern CMT_VERSION_DATA     cmw_versionData;
extern UINT                 cmw_linebitrate;
extern UINT                 cmw_SysparaSetting_flag;
extern UINT cmw_filedatsnd_flag;

#endif
/* @} */
