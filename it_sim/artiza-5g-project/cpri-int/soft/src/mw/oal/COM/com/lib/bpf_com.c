/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_com.c
 *  @brief  BPF Common function Library.
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_COM /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/prctl.h>
#include <syslog.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/syscall.h>

#include "bpf_l_com.h"
#include "bpf_i_com.h"

/** @addtogroup BPF_COM
 * @{ */

/* @{ */
/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @brief  Debug Log output
 *  @note   Common function library.
 *          This function is not signal safe.
 *  @param  loglevel       [in]  Log level
 *                                BPF_COM_LOG_TRACE
 *                                BPF_COM_LOG_ERR_L1
 *                                BPF_COM_LOG_ERR_L2
 *                                BPF_COM_LOG_ERR_L3
 *  @param  filename       [in]  File name     (valid 40 words)
 *  @param  funcname       [in]  Function name (valid 40 words)
 *  @param  line           [in]  Line number
 *  @param  event          [in]  Logging event (valid 128words)
 *  @param  outdata        [in]  Output data
 *  @param  data_len       [in]  Output data length (0-300)
 *  @return none
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 */
/********************************************************************************************************************/
void bpf_com_dbglog_func(unsigned int loglevel, const char* filename,const char* funcname,
                unsigned int line , char* event, void* outdata, unsigned int data_len)
{

#ifdef BPF_DEBUG
    bpf_com_log_debug(filename,funcname,line,event,data_len,outdata);

#elif BPF_COM_LOG_ERR
    if (loglevel != BPF_COM_LOG_TRACE)
    {
        bpf_com_log_debug(filename,funcname,line,event,data_len,outdata);
    }
#endif /* BPF_DEBUG */

    return;
}

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */

/* @{ */
/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @brief  Debug Log format output
 *  @note   Common function library.
 *          This function is not signal safe.
 *  @param  loglevel       [in]  Log level
 *  @param  filename       [in]  File name    (valid 40 words)
 *  @param  funcname       [in]  Function name(Valid 40 words)
 *  @param  line           [in]  Line number
 *  @param  event_fmt      [in]  Logging event format
 *  @return none
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2021/02/18 M&C)Tri.hn Update for intermediate device
 */
/********************************************************************************************************************/
void bpf_com_dbglog_fmt_func(unsigned int loglevel, const char* filename,const char* funcname, unsigned int line, char *event_fmt, ...)
{
    // char    event[BPF_COM_LOG_EVENT_MAX];
    // va_list args;
    // int  len;
	// int	 selfpid = getpid();
	// int  selftid = (int)(syscall(SYS_gettid));

    // va_start(args, event_fmt);	/* pgr0039 */
	// memset(event,0,sizeof(event));
    // len = vsnprintf(event, BPF_COM_LOG_EVENT_MAX, event_fmt, args);
    // va_end(args);
    // if(len < BPF_COM_LOG_EVENT_MAX ){
    //  (void)snprintf( &event[len], ( BPF_COM_LOG_EVENT_MAX - len ), " Pid:%d Tid:%d", selfpid, selftid);
    // }
    // bpf_com_dbglog_func(loglevel,
    //                     filename,
    //                     funcname,
    //                     line,
    //                     event,
    //                     NULL, 0);

    return;
}
/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
/* @} */
