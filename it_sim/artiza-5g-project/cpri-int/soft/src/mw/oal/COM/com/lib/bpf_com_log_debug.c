/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_com_log_debug.c
 *  @brief  Common log Function.
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013-2015
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_COM /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_l_com.h"
#include "bpf_i_com.h"
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extern unsigned int  di_log_assign_flag;
extern unsigned int  di_devc_assign_flag;


static int bpf_com_log_l_proctid_pre = -1;
static int bpf_com_log_l_procname_pre_len = 0;
static char bpf_com_log_l_procname_pre[32];



/** @addtogroup BPF_COM_LOG
 * @{ */
 
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  スレッド名取得処理
 *  @note   スレッド名取得処理
 */
/********************************************************************************************************************/
 static void bpf_com_log_l_get_prcname(char* name)
{
	char filename[32];
	int fd;
	int selftid = (int)(syscall(SYS_gettid));
	
	if(bpf_com_log_l_proctid_pre != selftid){
		memset(filename,0,sizeof(filename));
		snprintf((char*)filename, sizeof(filename),"/proc/%d/comm", selftid);
		if ((fd = open(filename, O_RDONLY)) > 0){
			bpf_com_log_l_procname_pre_len = read(fd,bpf_com_log_l_procname_pre,sizeof(bpf_com_log_l_procname_pre));
			close(fd);
			bpf_com_log_l_procname_pre[bpf_com_log_l_procname_pre_len - 1] = '\0';
			bpf_com_log_l_proctid_pre = selftid;
		}
		else{
			bpf_com_log_l_proctid_pre = -1;
			return;
		}
	}
	memcpy(name,bpf_com_log_l_procname_pre,bpf_com_log_l_procname_pre_len);

	return ;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  デバッグログ収集
 *  @note   デバッグログの収集を行う。収集したデバッグログは共有メモリに格納される。
 *  @param  filename        [in]  ファイル名
 *  @param  funcname        [in]  関数名
 *  @param  line            [in]  行番号
 *  @param  eventadd        [in]  イベント名
 *  @param  logsize         [in]  ログ情報サイズ
 *  @param  logdata         [in]  ログ情報
 *  @return result code
 *  @retval BPF_COM_LOG_INTER_OK
 *  @retval BPF_COM_LOG_ERR_ASSIGN
 *  @retval BPF_COM_LOG_INTER_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2015/08/04 ALPHA)Ueda M-RRU-ZSYS-01654対応
 */
/********************************************************************************************************************/
int bpf_com_log_debug(const char* filename, const char* funcname,unsigned int line,const char* event,unsigned int logsize,void* logdata)
{   
    int ret;                                            /* 戻り値                 */
    t_bpf_ru_log_data_tbl*		log_data_tbl;           /* ログ管理情報           */
    struct timeval tv;
    struct tm ltm;
    struct tm *dmy_tm;
    struct timespec	boottime;

    /* 初期化処理 */
    ret = 0;
    
    
    /* アサイン未の場合は処理終了 */
    if(di_log_assign_flag == LOG_FLG_OFF){
		FILE* fp = NULL;
		if(( fp = fopen("/dev/kmsg","w") ) != NULL ){
			fprintf(fp,"<0>(%s(%d)%s)%s",filename,line,funcname,event);
			fflush(fp);
			fclose(fp);
		}
        return BPF_COM_LOG_ERR_ASSIGN;
    }
    
    /* ログ情報が規定数以上の場合は終了 */
    if( logsize > LOG_DATA_LOG_DATA_MAX)
    {
        return BPF_COM_LOG_INTER_NG;
    }

    /* Mutex lock */
    ret = pthread_mutex_lock(&di_log_data_p->mutex);
    if( ret != 0 ){
        return BPF_COM_LOG_INTER_NG;
    }
    
    /* ログ情報の設定 */
    log_data_tbl = &(di_log_data_p->data_info[di_log_data_p->index]);

    /* ログ情報が規定数を越していた場合は先頭を上書きする */
    di_log_data_p->index++;
    
    if (di_log_data_p->index >= LOG_DATA_LINE)
    {
        di_log_data_p->index = 0;
    }

    /* 時間情報の取得 */
    ret = gettimeofday(&tv,NULL);
    if( ret != 0 )
    {
		/* Mutex Unlock */
		pthread_mutex_unlock(&di_log_data_p->mutex);
        return BPF_COM_LOG_INTER_ERR_SYS;
    }
    
    /* localtime_rがNULLで復帰したらNG終了 */
    dmy_tm = localtime_r(&tv.tv_sec, &ltm);
    if( dmy_tm == NULL )
    {
		/* Mutex Unlock */
		pthread_mutex_unlock(&di_log_data_p->mutex);
        return BPF_COM_LOG_INTER_ERR_SYS;
    }
	
	(void)clock_gettime( CLOCK_MONOTONIC_COARSE   , &boottime);	/* 時刻取得(bootup時刻) */

    di_log_data_p->pre_no++;

    memcpy(log_data_tbl->filename,filename,LOG_STRING_FILE_MAX);  /* ファイル名               */
    log_data_tbl->time.year  = (ltm.tm_year + 1900);              /* 年                       */
    log_data_tbl->time.month = ltm.tm_mon + 1;                    /* 月(1～12)                */
    log_data_tbl->time.day   = ltm.tm_mday;                       /* 日(1～31)                */
    log_data_tbl->time.hour  = ltm.tm_hour;                       /* 時(0～23)                */
    log_data_tbl->time.min   = ltm.tm_min;                        /* 分(0～59)                */
    log_data_tbl->time.sec   = ltm.tm_sec;                        /* 秒(0～59)                */
    log_data_tbl->time.msec  = (tv.tv_usec / 10000);              /* ミリ秒(0～99)(10ms単位)  */
    log_data_tbl->time.ucount= (UINT)(boottime.tv_sec);           /* bootup時刻 */
    log_data_tbl->no  = di_log_data_p->pre_no;                    /* ログNo                   */
    memcpy(log_data_tbl->funcname,funcname,LOG_STRING_FUNC_MAX);  /* 関数名                   */
    log_data_tbl->line = line;                                    /* 行番号                   */
    memcpy(log_data_tbl->event,event,LOG_STRING_EVENT_MAX);       /* イベント名               */
	bpf_com_log_l_get_prcname(log_data_tbl->threadname);			/* スレッド名 */
    log_data_tbl->logsize = logsize;                              /* ログ情報サイズ           */

	
    /* ログ情報の初期化 */
    memset(log_data_tbl->logdata,0,LOG_DATA_LOG_DATA_MAX);

    /* ログ情報サイズが0以上の場合はログ情報を設定 */
    if (logsize > 0){
        memcpy(log_data_tbl->logdata,logdata,logsize);            /* ログ情報       */
    }

#ifdef  BPF_DEBUG_CONSOLE   /* デバッグオプション */
    printf("==================================================\n");
    printf("NO = %d  "            ,log_data_tbl->no);
    printf("%04d%02d%02d"         ,log_data_tbl->time.year,log_data_tbl->time.month,log_data_tbl->time.day);
    printf("[%02d:%02d:%02d:%02d]",log_data_tbl->time.hour,log_data_tbl->time.min,log_data_tbl->time.sec,log_data_tbl->time.msec);
    printf(" %s:%d(%s)"           ,log_data_tbl->filename,log_data_tbl->line,log_data_tbl->funcname);
    printf(" event = %s"          ,log_data_tbl->event);
    printf(" logsize = %d  "      ,log_data_tbl->logsize);
    if (log_data_tbl->logsize > 0)
    {
        printf("logdata  = %s  " ,log_data_tbl->logdata);
    }
    printf("\n");
    printf("==================================================\n");
#endif
    /* Mutex Unlock */
    pthread_mutex_unlock(&di_log_data_p->mutex);

    return BPF_COM_LOG_INTER_OK;
}
/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
