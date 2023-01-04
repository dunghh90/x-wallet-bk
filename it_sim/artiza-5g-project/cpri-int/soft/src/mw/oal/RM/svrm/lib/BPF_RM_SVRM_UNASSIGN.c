/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_RM_SVRM_ASSIGN.c
 *  @brief  
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RM_SVRM /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_rm_svrm.h"

/** @addtogroup BPF_RM_SVRM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  プロセス管理機能割付け解放
 *  @note   プロセス毎の解放を行う。
 *  @param  process_desc [in]   プロセス識別子
 *  @return result code
 *  @retval BPF_RM_SVRM_COMPLETE
 *  @retval BPF_RM_SVRM_ERR_DISC
 *  @retval BPF_RM_SVRM_ERR_EXEC
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 
 */
/********************************************************************************************************************/
int BPF_RM_SVRM_UNASSIGN(unsigned int process_desc)
{
    int     ret;

    /* Process descriptor check */
    if(((bpf_com_get_proc_tkind(process_desc) != BPF_COM_PTKIND_APL) &&
       (bpf_com_get_proc_tkind(process_desc) != BPF_COM_PTKIND_PF)) ||
       (BPF_COM_GET_MY_PROCESS_DESC() != process_desc)){

        /* LOG(ERROR) */
        SVRM_DBGLOG_FMT_ERR(
              "Process_descriptor error process_desc=%d(%d)",
              process_desc, BPF_COM_GET_MY_PROCESS_DESC());

        /* invalid process descriptor */
        return BPF_RM_SVRM_ERR_DISC;
    }

    /* --------------------------------------------------------- */
    /* BPF Function UnAssign                                     */
    /* --------------------------------------------------------- */
    /* ipcm */
    ret = bpf_ru_ipcm_unassign(process_desc);

    if(ret != BPF_RU_IPCM_OK){
         /* LOG(ERROR) */
         SVRM_DBGLOG_FMT_ERR(
              "IPCM unassign ret=%d", ret );
         /* Internal error */
         return BPF_RM_SVRM_ERR_EXEC;
    }

    /* itcm */
    ret = bpf_ru_itcm_unassign(process_desc);

    if(ret != BPF_RU_ITCM_OK){
         /* LOG(ERROR) */
         SVRM_DBGLOG_FMT_ERR(
              "ITCM unassign ret=%d", ret );
         /* Internal error */
         return BPF_RM_SVRM_ERR_EXEC;
    }

    /* dev unassign */
    bpf_hm_devc_unassign();

    return BPF_RM_SVRM_COMPLETE;
}

/* @} */
/* @} */

/* RRH-001 MD alpha)matsuhashi add end */
