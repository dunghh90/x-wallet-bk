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
 *  @brief  プロセス管理機能割付け
 *  @note   プロセス毎の初期設定を行う。シグナルの登録、各種リソースの確保を行う。
 *  @param  process_desc [in]   プロセス識別子
 *  @return result code
 *  @retval BPF_RM_SVRM_COMPLETE
 *  @retval BPF_RM_SVRM_ERR_DISC
 *  @retval BPF_RM_SVRM_ERR_EXEC
 *  @retval BPF_RM_SVRM_ERR_SYS
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 
 */
/********************************************************************************************************************/
int BPF_RM_SVRM_ASSIGN(unsigned int process_desc)
{
    int     ret;
	
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
    /* log */
    ret = bpf_com_log_assign(process_desc);
    if(ret != BPF_COM_LOG_INTER_OK){
         /* LOG(ERROR) */
         SVRM_DBGLOG_FMT_ERR(
              "LOG assign ret=%d", ret );
              
         if (ret == BPF_COM_LOG_INTER_ERR_SYS)
         {
            return BPF_RM_SVRM_ERR_SYS;
         }
         /* Internal error */
         return BPF_RM_SVRM_ERR_EXEC;
    }

    /* devc */
    ret = bpf_hm_devc_assign(process_desc);
    if(ret != BPF_HM_DEVC_INTER_OK){
         /* LOG(ERROR) */
         SVRM_DBGLOG_FMT_ERR(
              "DEVC assign ret=%d", ret );

         if (ret == BPF_HM_DEVC_INTER_ERR_SYS)
         {
            return BPF_RM_SVRM_ERR_SYS;
         }
         /* Internal error */
         return BPF_RM_SVRM_ERR_EXEC;
    }

    /* ipcm */
    ret = bpf_ru_ipcm_assign(process_desc);

    if(ret != BPF_RU_IPCM_OK){
         /* LOG(ERROR) */
         SVRM_DBGLOG_FMT_ERR(
              "IPCM assign ret=%d", ret );
         /* Internal error */
         return BPF_RM_SVRM_ERR_EXEC;
    }

    /* itcm */
    ret = bpf_ru_itcm_assign(process_desc);

    if(ret != BPF_RU_ITCM_OK){
         /* LOG(ERROR) */
         SVRM_DBGLOG_FMT_ERR(
              "ITCM assign ret=%d", ret );
         /* Internal error */
         return BPF_RM_SVRM_ERR_EXEC;
    }

    /* hrtm */
    ret = bpf_ru_hrtm_assign(process_desc);

    if(ret != BPF_RU_HRTM_OK){
         /* LOG(ERROR) */
         SVRM_DBGLOG_FMT_ERR(
              "HRTM assign ret=%d", ret );
         /* Internal error */
         return BPF_RM_SVRM_ERR_EXEC;
    }

    /* mblm */
    ret = bpf_ru_mblm_bufferassign(process_desc);

    if(ret != BPF_RU_MBLM_OK){
         /* LOG(ERROR) */
         SVRM_DBGLOG_FMT_ERR(
              "MBLM assign ret=%d", ret );
         /* Internal error */
         return BPF_RM_SVRM_ERR_EXEC;
    }

    return BPF_RM_SVRM_COMPLETE;
}

/* @} */
/********************************************************************************************************************/
/**
 *  @brief  SIGUSR1 handler
 *  @note   none
 *  @param  signal [in] signal number
 *  @param  siginfo [in] signal information
 *  @param  u_contxt [in] user context
 *  @return none
 *  @retval none
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 */
/********************************************************************************************************************/
void bpf_rm_svrm_usr_sigusr1_hdl( int signal, siginfo_t *siginfo, void *u_contxt  )
{

    pthread_exit(NULL);

    return;
}

/* @} */

/* RRH-001 MD alpha)matsuhashi add end */
