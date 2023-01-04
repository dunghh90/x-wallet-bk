/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_RM_SVRM_CPUAFFINITY.c
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
#include <sched.h>
#include "bpf_i_rm_svrm.h"

extern int sched_setaffinity(pid_t pid, size_t size, cpu_set_t * mask );
extern int sched_getaffinity(pid_t pid, size_t size, cpu_set_t * mask );

/** @addtogroup BPF_RM_SVRM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  プロセスCPU割付
 *  @note   自プロセスを特定CPU(1つ)に割り付ける。
 *  @param  process_desc [in]   プロセス識別子
 *  @param  cpu          [in]   割付CPU
 *  @param  errcd        [out]  詳細NGコード
  *  @return result code
 *  @retval BPF_RM_SVRM_COMPLETE
 *  @retval BPF_RM_SVRM_ERR_DISC
 *  @retval BPF_RM_SVRM_ERR_SYS
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 */
/********************************************************************************************************************/
int BPF_RM_SVRM_CPUAFFINITY (unsigned int process_desc, int cpu, int* errcd)
{
    cpu_set_t cpu_mask;                /* CPU affinity mask */
    int ret;

    /* Process descriptor check */
    if((bpf_com_get_proc_tkind(process_desc) != BPF_COM_PTKIND_APL) &&
       ((bpf_com_get_proc_tkind(process_desc) != BPF_COM_PTKIND_PF) ||
       (BPF_COM_GET_MY_PROCESS_DESC() != process_desc))){

        /* LOG(ERROR) */
        SVRM_DBGLOG_FMT_ERR(
              "Process_descriptor error process_desc=%d(%d)",
              process_desc, BPF_COM_GET_MY_PROCESS_DESC());

        /* invalid process descriptor */
        return BPF_RM_SVRM_ERR_DISC;
    }

    /* --------------------------------------------------------- */
    /* SET CPU affinity                                          */
    /* --------------------------------------------------------- */
    __CPU_ZERO_S(sizeof(cpu_mask),&cpu_mask);			/* pgr0695:修正方法不明	*/
    __CPU_SET_S(cpu,sizeof(cpu_mask),&cpu_mask);		/* pgr0695 */

    ret = sched_setaffinity(BPF_COM_NUM0, sizeof(cpu_mask), &cpu_mask);

    if( ret == BPF_COM_SYSCALL_NG ){

        *errcd = errno;

        /* LOG(ERROR) */
        SVRM_DBGLOG_FMT_ERR(
              "Cpu affinity error cpu=%d errno=%d", cpu, *errcd);

        /* invalid process descriptor */
        return BPF_RM_SVRM_ERR_SYS;
    }

    return BPF_RM_SVRM_COMPLETE;

}
/* @} */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  プロセスCPU割付追加
 *  @note   自プロセスに特定CPUを追加する。
 *  @param  process_desc [in]   プロセス識別子
 *  @param  cpu          [in]   割付CPU
 *  @param  errcd        [out]  詳細NGコード
  *  @return result code
 *  @retval BPF_RM_SVRM_COMPLETE
 *  @retval BPF_RM_SVRM_ERR_DISC
 *  @retval BPF_RM_SVRM_ERR_SYS
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 */
/********************************************************************************************************************/
int BPF_RM_SVRM_CPUAFFINITY_ADD(unsigned int process_desc, int cpu, int* errcd)
{
    cpu_set_t cpu_mask;                /* CPU affinity mask */
    int ret;

    /* Process descriptor check */
    if((bpf_com_get_proc_tkind(process_desc) != BPF_COM_PTKIND_APL) &&
       ((bpf_com_get_proc_tkind(process_desc) != BPF_COM_PTKIND_PF) ||
       (BPF_COM_GET_MY_PROCESS_DESC() != process_desc))){

        /* LOG(ERROR) */
        SVRM_DBGLOG_FMT_ERR(
              "Process_descriptor error process_desc=%d(%d)",
              process_desc, BPF_COM_GET_MY_PROCESS_DESC());

        /* invalid process descriptor */
        return BPF_RM_SVRM_ERR_DISC;
    }
    
	ret = sched_getaffinity(BPF_COM_NUM0,sizeof(cpu_mask), &cpu_mask);
    if( ret == BPF_COM_SYSCALL_NG ){
        *errcd = errno;
        /* LOG(ERROR) */
        SVRM_DBGLOG_FMT_ERR(
              "Cpu affinity add error cpu=%d errno=%d", cpu, *errcd);
        /* invalid process descriptor */
        return BPF_RM_SVRM_ERR_SYS;
    }

    /* --------------------------------------------------------- */
    /* SET CPU affinity                                          */
    /* --------------------------------------------------------- */
    __CPU_SET_S(cpu,sizeof(cpu_mask),&cpu_mask);		/* pgr0695 */

    ret = sched_setaffinity(BPF_COM_NUM0, sizeof(cpu_mask), &cpu_mask);

    if( ret == BPF_COM_SYSCALL_NG ){

        *errcd = errno;

        /* LOG(ERROR) */
        SVRM_DBGLOG_FMT_ERR(
              "Cpu affinity error cpu=%d errno=%d", cpu, *errcd);

        /* invalid process descriptor */
        return BPF_RM_SVRM_ERR_SYS;
    }

    return BPF_RM_SVRM_COMPLETE;

}
/* @} */


/* @} */

/* RRH-001 MD alpha)matsuhashi add end */
