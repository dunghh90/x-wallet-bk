/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_RM_SVRM_INIT.c
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
 *  @brief  共通プロセス管理機能割付け
 *  @note   プロセス共通の初期化設定を行う。各種リソースの確保を行う。
 *  @param  process_desc [in]   プロセス識別子
 *  @return result code
 *  @retval BPF_RM_SVRM_COMPLETE
 *  @retval BPF_RM_SVRM_ERR_DISC
 *  @retval BPF_RM_SVRM_ERR_EXEC
 *  @retval BPF_RM_SVRM_ERR_SYS
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 */
/********************************************************************************************************************/
int BPF_RM_SVRM_INIT(unsigned int process_desc)
{
    int msgqid;
    int ret;

    /* Process descriptor check */
    if((bpf_com_get_proc_tkind(process_desc) != BPF_COM_PTKIND_APL) &&
       (bpf_com_get_proc_tkind(process_desc) != BPF_COM_PTKIND_PF)) {

        /* LOG(ERROR) */
        SVRM_DBGLOG_FMT_ERR(
              "Process_descriptor error process_desc=%d",process_desc);

        /* invalid process descriptor */
        return BPF_RM_SVRM_ERR_DISC;
    }

    /* --------------------------------------------------------- */
    /* Save My process descriptor                                */
    /* --------------------------------------------------------- */
    BPF_COM_SET_MY_PROCESS_DESC( process_desc );

    /* --------------------------------------------------------- */
    /* BPF Function Assign                                       */
    /* --------------------------------------------------------- */
    /* devc */
    ret = bpf_hm_devc_init(process_desc);
    if(ret != BPF_HM_DEVC_INTER_OK){
         /* LOG(ERROR) */
         SVRM_DBGLOG_FMT_ERR("DEVC assign ret=%d", ret );

         if (ret == BPF_HM_DEVC_INTER_ERR_SYS)
         {
            return BPF_RM_SVRM_ERR_SYS;
         }
         /* Internal error */
         return BPF_RM_SVRM_ERR_EXEC;
    }
	/* bpf_hm_devc_initの失敗要因が分からないため、logを後にする */
    /* log */
    ret = bpf_com_log_init(process_desc);
    if(ret != BPF_COM_LOG_INTER_OK){
         /* LOG(ERROR) */
         SVRM_DBGLOG_FMT_ERR("LOG assign ret=%d", ret );

         if (ret == BPF_COM_LOG_INTER_ERR_SYS)
         {
            return BPF_RM_SVRM_ERR_SYS;
         }
         /* Internal error */
         return BPF_RM_SVRM_ERR_EXEC;
    }

    /* メッセージキュー作成 */
    msgqid = msgget( BPF_COM_IPCKEY_MASK_INTERNAL,SVRM_MSGFLG);

    if( msgqid == SVRM_MSGGET_NG ){ /* 戻り値-1で作成失敗 */
        SVRM_DBGLOG_FMT_ERR(
            "msgget failed. msgqid = %08x, flg = %d, errno = %d",
            msgqid, SVRM_MSGFLG, errno);
        return BPF_RM_SVRM_ERR_SYS;
    }

    /* LOG(TRACE) */
    SVRM_DBGLOG_FMT_TRC("BPF_RM_SVRM_INIT msgget Success msgqid = 0x%08x" , msgqid);


    return BPF_RM_SVRM_COMPLETE;
}

/* @} */
/* @} */

/* RRH-001 MD alpha)matsuhashi add end */
