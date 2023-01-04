/* RRH-001 MD alpha)yokoyama add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_RM_SVRM_SIGNAL.c
 *  @brief  
 *  @date   2013/11/10 ALPHA)yokoyama create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RM_SVRM /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_rm_svrm.h"
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <execinfo.h>
#include <byteswap.h>
#include <errno.h>
#include <signal.h>

/** @addtogroup BPF_RM_SVRM
 * @{ */

/*!
 *	@brief	シグナルハンドリング対象テーブル
 *	@note
 *	このテーブルに登録したシグナル番号の中でsighdlflgが1の場合ハンドリング対象となる． <BR>
 *
 */
typedef struct {
	INT 	signo;				/*!< signal no */
	CHAR*	signame;			/*!< signal name */
	INT		sighdlflg;			/*!< handle signal 0:default 1:handle 2:ignore */
	INT		almcode;			/*!< almcode */
}BPF_RM_SVRM_SIG_LIST_T;

const BPF_RM_SVRM_SIG_LIST_T	bpf_rm_svrm_sig_list[] = {
	/* ハンドラ登録するシグナルは1を立てる	*/
	{	0,			"RESERVE"	,	0,	0},					/*!<  reserve										 0 */
	{	SIGHUP,		"SIGHUP"	,	1,	D_DU_ALMCD_OS},	/*!<  -		Hangup (POSIX)							 1 */
	{	SIGINT,		"SIGINT"	,	0,	0},					/*!<  Term	キーボード Interrupt (ANSI).			 2 */
	{	SIGQUIT,	"SIGQUIT"	,	0,	0},					/*!<  Core	キーボード Quit (POSIX).				 3 */
	{	SIGILL,		"SIGILL"	,	1,	D_DU_ALMCD_ISI},	/*!<  Core	Illegal instruction (ANSI).				 4 */
	{	SIGTRAP,	"SIGTRAP"	,	0,	0},					/*!<  Core	Trace trap (POSIX).						 5 */
	{	SIGABRT,	"SIGABRT"	,	1,	D_DU_ALMCD_PRO},	/*!<  SIGIOT		Abort (ANSI).					 6 */
	{	SIGBUS,		"SIGBUS"	,	1,	D_DU_ALMCD_EXC},	/*!<  Core	BUS error (4.2 BSD).					 7 */
	{	SIGFPE,		"SIGFPE"	,	1,	D_DU_ALMCD_ARAI},	/*!<  Core	Floating-point exception (ANSI).		 8 */
	{	SIGKILL,	"SIGKILL"	,	0,	0},					/*!<  Term	Kill, unblockable (POSIX).				 9 */
	{	SIGUSR1,	"SIGUSR1"	,	0,	0},					/*!<  Term	User-defined signal 1 (POSIX).			10 */
	{	SIGSEGV,	"SIGSEGV"	,	1,	D_DU_ALMCD_DSI},	/*!<  Core	Segmentation violation (ANSI).			11 */
	{	SIGUSR2,	"SIGUSR2"	,	0,	0},					/*!<  Term	User-defined signal 2 (POSIX).			12 */
	{	SIGPIPE,	"SIGPIPE"	,	2,	0},					/*!<  Term	Broken pipe (POSIX).					13 */
	{	SIGALRM,	"SIGALRM"	,	0,	0},					/*!<  Term	Alarm clock (POSIX).					14 */
	{	SIGTERM,	"SIGTERM"	,	1,	D_DU_ALMCD_ANG},	/*!<  Term	Termination (ANSI).						15 */
	{	SIGSTKFLT,	"SIGSTKFLT"	,	1,	D_DU_ALMCD_ANNG2},	/*!<  												16 */
	{	SIGCHLD,	"SIGCHLD"	,	0,	0},					/*!<  Ign	Child status has changed (POSIX).		17 */
	{	SIGCONT,	"SIGCONT"	,	0,	0},					/*!<  Cont	Continue (POSIX).						18 */
	{	SIGSTOP,	"SIGSTOP"	,	0,	0},					/*!<  Stop	Stop, unblockable (POSIX).				19 */
	{	SIGTSTP,	"SIGTSTP"	,	0,	0},					/*!<  Stop	Keyboard stop (POSIX).					20 */
	{	SIGTTIN,	"SIGTTIN"	,	0,	0},					/*!<  Stop	Background read from tty (POSIX).		21 */
	{	SIGTTOU,	"SIGTTOU"	,	0,	0},					/*!<  Stop	Background write to tty (POSIX).		22 */
	{	SIGURG,		"SIGURG"	,	0,	0},					/*!<  Ign	Urgent condition on socket (4.2 BSD)	23 */
	{	SIGXCPU,	"SIGXCPU"	,	1,	D_DU_ALMCD_ANNG2},	/*!<  Core	CPU limit exceeded (4.2 BSD).			24 */
	{	SIGXFSZ,	"SIGXFSZ"	,	1,	D_DU_ALMCD_ANNG2},	/*!<  Core	File size limit exceeded (4.2 BSD).		25 */
	{	SIGVTALRM,	"SIGVTALRM"	,	0,	0},					/*!<  Term	Virtual alarm clock (4.2 BSD).			26 */
	{	SIGPROF,	"SIGPROF"	,	0,	0},					/*!<  Term	Profiling alarm clock (4.2 BSD).		27 */
	{	SIGWINCH,	"SIGWINCH"	,	0,	0},					/*!<  Ign	Window size change (4.3 BSD, Sun).		28 */
	{	SIGPOLL,	"SIGPOLL"	,	0,	0},					/*!<  SIGIO	Pollable event occurred (System V)		29 */
	{	SIGPWR,		"SIGPWR"	,	0,	0},					/*!<  Term	Power failure restart (System V).		30 */
	{	SIGSYS,		"SIGSYS"	,	1,	D_DU_ALMCD_ANOT}	/*!<  Core	ルーチンへの引き数が不正 (SVr4)			31 */
};


/*		typedef struct siginfo {									*/
/*			int si_signo;											*/
/*			int si_errno;											*/
/*			int si_code;											*/
/*			union {													*/
/*			-	SIGCHLD												*/
/*				struct {											*/
/*					__kernel_pid_t _pid;							*/
/*					__ARCH_SI_UID_T _uid;							*/
/*					int _status;									*/
/*					__ARCH_SI_CLOCK_T _utime;						*/
/*					__ARCH_SI_CLOCK_T _stime;						*/
/*				} _sigchld;											*/
/*																	*/
/*			-	SIGILL, SIGFPE, SIGSEGV, SIGBUS						*/
/*				struct {											*/
/*					void *_addr;									*/					
/*		#ifdef __ARCH_SI_TRAPNO										*/
/*					int _trapno;									*/				
/*		#endif														*/
/*					short _addr_lsb;								*/									
/*					union {											*/
/*						struct {									*/
/*							void *_lower;							*/
/*							void *_upper;							*/
/*						} _addr_bnd;								*/
/*						__u32 _pkey;								*/
/*					};												*/
/*				} _sigfault;										*/
/*			} _sifields;											*/
/*		} __ARCH_SI_ATTRIBUTES siginfo_t;							*/
/*																	*/
/*		siginfo_t {													*/
/*		    int      si_signo;  									*/
/*		    int      si_errno;  									*/
/*		    int      si_code;   									*/
/*		    int      si_trapno; 									*/
/*		                        									*/
/*		                        									*/
/*		    pid_t    si_pid;    									*/
/*		    uid_t    si_uid;    									*/
/*		    int      si_status; 									*/
/*		    clock_t  si_utime;  									*/
/*		    clock_t  si_stime;  									*/
/*		    sigval_t si_value;  									*/
/*		    int      si_int;    									*/
/*		    void    *si_ptr;    									*/
/*		    int      si_overrun;									*/
/*		                        									*/
/*		    int      si_timerid;									*/
/*		    void    *si_addr;   									*/
/*		    long     si_band;   									*/
/*		                        									*/
/*		    int      si_fd;     									*/
/*		    short    si_addr_lsb									*/
/*		                        									*/
/*		    void    *si_call_add									*/
/*		                        									*/
/*		    int      si_syscall;									*/
/*		                        									*/
/*		    unsigned int si_arch									*/
/*		}															*/
/*																	*/
/*		typedef struct sigaltstack {								*/
/*			void *ss_sp;											*/
/*			int ss_flags;											*/
/*			size_t ss_size;											*/
/*		} stack_t;													*/
/*																	*/
/*		struct sigcontext {											*/
/*			__u64 fault_address;									*/
/*			__u64 regs[31];											*/
/*			__u64 sp;												*/
/*			__u64 pc;												*/
/*			__u64 pstate;											*/
/*			__u8 __reserved[4096] __attribute__((__aligned__(16)));	*/
/*		};															*/
/*																	*/
/*	struct ucontext {												*/
/*		unsigned long	  uc_flags;									*/
/*		struct ucontext	 *uc_link;									*/
/*		stack_t		  uc_stack;										*/
/*		sigset_t	  uc_sigmask;									*/
/*		__u8		  __unused[1024 / 8 - sizeof(sigset_t)];		*/
/*		struct sigcontext uc_mcontext;								*/
/*	};																*/
/*																	*/




typedef struct {
	ULONG fault_address;
	ULONG arm64_reg[31];
	ULONG arm64_sp;
	ULONG arm64_pc;
	ULONG arm64_pstate;
}bpf_rm_svrm_sigget_cpureg_t;

enum {
	D_BPF_RM_SVRM_PRCFILE_PATH = 0,
	D_BPF_RM_SVRM_PRCFILE_STATUS,	
	D_BPF_RM_SVRM_PRCFILE_MAPS,
	D_BPF_RM_SVRM_PRCFILE_MAXNUM
};

typedef struct {
	CHAR						filename[256];
	INT							data_len;
	CHAR						data[32*1024];
}bpf_rm_svrm_sigget_prc_t;

typedef struct {
	CHAR						pname[32];		/*!< process name	 */
	INT							tid;			/*!< thread id		 */
	INT							pid;			/*!< process id		 */
	INT							ppid;			/*!< Pprocess id	 */
	INT							si_signo;		/*!< signo			 */
	INT							si_code;		/*!< code			 */
	INT							si_errcode;		/*!< error code		 */
	INT							sig_pid;		/*!< pid			 */
	INT							sig_uid;		/*!< uid			 */
	INT							sig_status;		/*!< status			 */
	INT							sig_utime;		/*!< utime			 */
	INT							sig_stime;		/*!< utime			 */
	INT							sig_value;		/*!< utime			 */
	ULONG						sig_addr;		/*!< addr			 */
	ULONG						sig_lowaddr;	/*!< lowaddr		 */
	INT							prcfilenum;		/*!< proc file		 */
	INT							tblnum;			/*!< tblnum			 */
	struct timespec				almtime;		/*!< alm happentime	 */
	struct timespec				boottime;				/*!< bootup time */
	INT							bktrc_num;				/*!< backtrace num	*/
	char						bktrc_txt[2][1024];		/*!< backtrace		 */
	ULONG						stack_size;				/*!< stack size	*/
	ULONG						dump_addr;				/*!< stack size	*/
	ULONG						stack_addr;				/*!< stack size	*/
	ULONG						stack_info[0x1000];
	bpf_rm_svrm_sigget_cpureg_t	cpu_reg;
	bpf_rm_svrm_sigget_prc_t	procfile_info[D_BPF_RM_SVRM_PRCFILE_MAXNUM];
}bpf_rm_svrm_sigget_reg_t;


typedef struct {
	CHAR*						name;
	INT							size;
}bpf_rm_svrm_sigget_filename_t;


static const bpf_rm_svrm_sigget_filename_t bpf_rm_svrm_sigget_filename[D_BPF_RM_SVRM_PRCFILE_MAXNUM] = 
{
	{"/proc/self/task/",sizeof("/proc/self/task/")},
	{"/status",sizeof("/status")},
	{"/maps" ,sizeof("/maps")}
};

static bpf_rm_svrm_sigget_reg_t	bpf_rm_svrm_sigget_reg;
static bpf_rm_svrm_sigget_prc_t	bpf_rm_svrm_sigget_file[D_BPF_RM_SVRM_PRCFILE_MAXNUM];
static char bpf_rm_svrm_sigget_logfilename[256];
static char bpf_rm_svrm_sigget_txtfilename[256];
static char bpf_rm_svrm_sigget_logfilecmd[1024];
static char bpf_rm_svrm_sigget_workbuf[4096];
int (*bpf_rm_svrm_sigget_proc_func_p)(int, siginfo_t *, void * ,int);
static int bpf_rm_svrm_sig_sndonoff;

/* @{ */
/**************************************************************************************************/
/*!
 *  @brief  f_cmn_mem_swap
 *  @note   メモリのswap関数
 *  @return 
 *  @date   
 */
/**************************************************************************************************/
static VOID bpf_rm_svrm_mem_swap(CHAR* topbuf_p ,INT size ,UINT mode)
{
	UINT	count;
	
	if((mode == 2) || (mode == 4) || (mode == 8))
	{
		for(count = 0; count < (size/mode);count++ , topbuf_p += mode)
		{
			switch(mode)
			{
				case 2:
					*(USHORT*)topbuf_p = bswap_16(*(USHORT*)topbuf_p);
					break;
				case 4:
					*(UINT*)topbuf_p = bswap_32(*(UINT*)topbuf_p);
					break;
				case 8:
					*(UINT*)topbuf_p = bswap_64(*(ULONG*)topbuf_p);
					break;
			}
		}
		if((size % mode) > 0)
		{
			/* あまりはなにもしない */
		}
	}	
	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  メモリ表示処理
 *  @note   メモリ表示処理
 *  @param  topbuf_p		[in]  バッファアドレス
 *  @param  size			[in]  バッファサイズ
 *  @return バッファアドレス
 *  @retval バッファアドレス
 *  @date   2013/12/26 FJT)Taniguchi create
 */
/********************************************************************************************************************/
#define D_BPF_RM_SVRM_SIGTBL_DISPSIZE2 32
#define D_BPF_RM_SVRM_SIGTBL_DISPSIZE 16
static CHAR* bpf_rm_svrm_mem_to_print2(FILE* fp ,ULONG addr ,CHAR* topbuf_p ,ULONG size )
{
	CHAR*	buf_p;
	ULONG*	buf_l_p;
	UINT	count1;
	UINT	count2;
	INT		tmp_size;
	INT		asc_size;

	buf_p = topbuf_p;
	tmp_size = size/D_BPF_RM_SVRM_SIGTBL_DISPSIZE2;
	asc_size = 0;
	memset(bpf_rm_svrm_sigget_workbuf,0,sizeof(bpf_rm_svrm_sigget_workbuf));

	fprintf(fp,"ADDRESS           +0                +8                +10               +18                0 2 4 6 8 A C E 0 2 4 6 8 A C E\n");
	fprintf(fp,"----------------   -------------------------------------------------------------------------------------------------------\n");

	for(count1 = 0 ; count1 < tmp_size ; count1++)
	{
		asc_size += sprintf(&bpf_rm_svrm_sigget_workbuf[asc_size],"%016lX   ",addr + (ULONG)(buf_p - topbuf_p));
		for(count2 = 0 ,buf_l_p = (ULONG*)buf_p ; count2 < 4 ; count2++,buf_l_p++)
		{
			asc_size += sprintf(&bpf_rm_svrm_sigget_workbuf[asc_size] ,"%016lX  ",*buf_l_p);
		}
		for(count2 = 0;count2 < D_BPF_RM_SVRM_SIGTBL_DISPSIZE2 ; count2++)
		{
			bpf_rm_svrm_sigget_workbuf[asc_size+count2] = (CHAR)*(buf_p+count2);
			if((bpf_rm_svrm_sigget_workbuf[asc_size+count2] > 0x7e)||((bpf_rm_svrm_sigget_workbuf[asc_size+count2] < 0x20)))
			{
				bpf_rm_svrm_sigget_workbuf[asc_size+count2] = 0x2e;
			}
		}
		asc_size += D_BPF_RM_SVRM_SIGTBL_DISPSIZE2;
		asc_size += sprintf(&bpf_rm_svrm_sigget_workbuf[asc_size],"\n");
		buf_p += D_BPF_RM_SVRM_SIGTBL_DISPSIZE2;
		if((sizeof(bpf_rm_svrm_sigget_workbuf) - 256) < asc_size)
		{
			fprintf(fp,"%s",bpf_rm_svrm_sigget_workbuf);
			asc_size = 0;
			memset(bpf_rm_svrm_sigget_workbuf,0,sizeof(bpf_rm_svrm_sigget_workbuf));
		}
	}
	if(asc_size != 0){
		fprintf(fp,"%s",bpf_rm_svrm_sigget_workbuf);
	}
	return(buf_p);
}

static CHAR* bpf_rm_svrm_mem_to_print(FILE* fp ,CHAR* topbuf_p ,INT size )
{
	CHAR*	buf_p;
	UINT	count1;
	UINT	count2;
	INT		tmp_size;
	INT		asc_size;

	buf_p = topbuf_p;
	tmp_size = size/D_BPF_RM_SVRM_SIGTBL_DISPSIZE;
	asc_size = 0;
	memset(bpf_rm_svrm_sigget_workbuf,0,sizeof(bpf_rm_svrm_sigget_workbuf));
	fprintf(fp,"ADDRESS            +0    +2    +4    +6    +8    +A    +C    +E    0 2 4 6 8 A C E\n");
	fprintf(fp,"----------------   ---------------------------------------------------------------\n");
	for(count1 = 0 ; count1 < tmp_size ; count1++)
	{
		asc_size += sprintf(&bpf_rm_svrm_sigget_workbuf[asc_size],"%016lx   ",(ULONG)(buf_p - topbuf_p));
		for(count2 = 0 ; count2 < D_BPF_RM_SVRM_SIGTBL_DISPSIZE ; count2++)
		{
			asc_size += sprintf(&bpf_rm_svrm_sigget_workbuf[asc_size] ,"%02X",(UCHAR)*(buf_p+count2));
			if(count2%2 > 0)
			{
				asc_size += sprintf(&bpf_rm_svrm_sigget_workbuf[asc_size] ,"  ");
			}
		}
		for(count2 = 0;count2 < D_BPF_RM_SVRM_SIGTBL_DISPSIZE ; count2++)
		{
			bpf_rm_svrm_sigget_workbuf[asc_size+count2] = (CHAR)*(buf_p+count2);
			if((bpf_rm_svrm_sigget_workbuf[asc_size+count2] > 0x7e)||((bpf_rm_svrm_sigget_workbuf[asc_size+count2] < 0x20)))
			{
				bpf_rm_svrm_sigget_workbuf[asc_size+count2] = 0x2e;
			}
		}
		asc_size += D_BPF_RM_SVRM_SIGTBL_DISPSIZE;
		asc_size += sprintf(&bpf_rm_svrm_sigget_workbuf[asc_size],"\n");
		buf_p += D_BPF_RM_SVRM_SIGTBL_DISPSIZE;
		if((sizeof(bpf_rm_svrm_sigget_workbuf) - 256) < asc_size)
		{
			fprintf(fp,"%s",bpf_rm_svrm_sigget_workbuf);
			asc_size = 0;
			memset(bpf_rm_svrm_sigget_workbuf,0,sizeof(bpf_rm_svrm_sigget_workbuf));
		}
	}
	tmp_size = size% D_BPF_RM_SVRM_SIGTBL_DISPSIZE;
	if(tmp_size > 0)
	{
		asc_size += sprintf(&bpf_rm_svrm_sigget_workbuf[asc_size],"%016lx   ",(ULONG)(buf_p - topbuf_p));
		for(count2 = 0 ; count2 < tmp_size ; count2++)
		{
			asc_size += sprintf(&bpf_rm_svrm_sigget_workbuf[asc_size] ,"%02X",(UCHAR)*(buf_p+count2));
			if(count2%2 > 0)
			{
				asc_size += sprintf(&bpf_rm_svrm_sigget_workbuf[asc_size] ,"  ");
			}
		}
		for(count2 = 0 ; count2 < (D_BPF_RM_SVRM_SIGTBL_DISPSIZE - tmp_size) ; count2++)
		{
			asc_size += sprintf(&bpf_rm_svrm_sigget_workbuf[asc_size] ,"  ");
			if(count2%2 > 0)
			{
				asc_size += sprintf(&bpf_rm_svrm_sigget_workbuf[asc_size] ,"  ");
			}
		}
		for(count2 = 0;count2 < tmp_size ; count2++)
		{
			bpf_rm_svrm_sigget_workbuf[asc_size+count2] = (CHAR)*(buf_p+count2);
			if((bpf_rm_svrm_sigget_workbuf[asc_size+count2] > 0x7e)||((bpf_rm_svrm_sigget_workbuf[asc_size+count2] < 0x20)))
			{
				bpf_rm_svrm_sigget_workbuf[asc_size+count2] = 0x2e;
			}
		}
		asc_size += tmp_size;
		buf_p += tmp_size;
		fprintf(fp,"%s\n",bpf_rm_svrm_sigget_workbuf);
	}
	else
	{
		fprintf(fp,"%s",bpf_rm_svrm_sigget_workbuf);
	}
	return(buf_p);
}

/* @} */

static void bpf_rm_svrm_l_get_prcname(int selftid ,int maxlen , char* proc_name)
{
	char filename[32];
	int fd;
	int len = 0;

	memset(filename,0,sizeof(filename));
	snprintf((char*)filename, sizeof(filename),"/proc/%d/comm", selftid);
	if((fd = open(filename, O_RDONLY)) > 0){
		len = read(fd,proc_name,maxlen);
		close(fd);
	}
	if(len < 1){
		strncpy(proc_name,"UNKNOWN",strlen("UNKNOWN"));
	}else{
		proc_name[len - 1] = '\0';
	}
	return;
}

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  コア情報表示処理
 *  @note   コア情報表示処理
 *  @param  topbuf_p		[in]  バッファアドレス
 *  @param  size			[in]  バッファサイズ
 *  @return バッファアドレス
 *  @retval バッファアドレス
 *  @date   2013/12/26 FJT)Taniguchi create
 */
/********************************************************************************************************************/
char* BPF_RM_SVRM_SIGNAL_COREDUMP(FILE* fp, char* topbuf_p, unsigned int size )
{
	UINT count;
	bpf_rm_svrm_sigget_tbl_t* tbl_p;
	CHAR*	buf_p;
	CHAR*	tbl_buf_p;
	struct tm lctimep;

	buf_p = topbuf_p;
	fprintf(fp,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	fprintf(fp,"Process Log Information \n");
	fprintf(fp,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	memset(&lctimep,0,sizeof(lctimep));
	localtime_r((const time_t *)&(((bpf_rm_svrm_sigget_reg_t*)buf_p)->almtime.tv_sec),&lctimep);
	fprintf(fp,"Time:%04d-%02d-%02d %02d:%02d:%02d.%09ld / Bootup:%ld.%09ld sec\n",
			lctimep.tm_year + 1900,
			lctimep.tm_mon+1,
			lctimep.tm_mday,
			lctimep.tm_hour,
			lctimep.tm_min,
			lctimep.tm_sec,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->almtime.tv_nsec,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->boottime.tv_sec,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->boottime.tv_nsec
	);
	fprintf(fp,"Name : <%s> / tid : <%d(0x%08X)> / pid : <%d(0x%08X)> / ppid : <%d(0x%08X)> \n",
		((bpf_rm_svrm_sigget_reg_t*)buf_p)->pname,
		((bpf_rm_svrm_sigget_reg_t*)buf_p)->tid,
		((bpf_rm_svrm_sigget_reg_t*)buf_p)->tid,
		((bpf_rm_svrm_sigget_reg_t*)buf_p)->pid,
		((bpf_rm_svrm_sigget_reg_t*)buf_p)->pid,
		((bpf_rm_svrm_sigget_reg_t*)buf_p)->ppid,
		((bpf_rm_svrm_sigget_reg_t*)buf_p)->ppid);
	fprintf(fp,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	for (count = 0; count < (sizeof(bpf_rm_svrm_sig_list) / sizeof(bpf_rm_svrm_sig_list[0])); count++)
	{
		if(bpf_rm_svrm_sig_list[count].signo == ((bpf_rm_svrm_sigget_reg_t*)buf_p)->si_signo)
		{
			fprintf(fp,"sig name : %s(%d),  code : %d , errcode : %d \n",
			bpf_rm_svrm_sig_list[count].signame,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->si_signo,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->si_code,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->si_errcode);
			break;
		}
	}
	if(count >= (sizeof(bpf_rm_svrm_sig_list) / sizeof(bpf_rm_svrm_sig_list[0])))
	{
			fprintf(fp,"sig nal : %d(%08X),  code : %d , errcode : %d \n",
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->si_signo,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->si_signo,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->si_code,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->si_errcode);
	}
			fprintf(fp,"pid : %d  , uid : %d , status : %d(%08X) , utime %d(%08X) , stime %d(%08X)\n",
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->sig_pid,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->sig_uid,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->sig_status,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->sig_status,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->sig_utime,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->sig_utime,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->sig_stime,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->sig_stime);
			fprintf(fp,"value : %d(%08X),  adr : 0x%p , sig_lowaddr :0x%p \n",
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->sig_value,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->sig_value,
			(void*)(((bpf_rm_svrm_sigget_reg_t*)buf_p)->sig_addr),
			(void*)(((bpf_rm_svrm_sigget_reg_t*)buf_p)->sig_lowaddr));

	fprintf(fp,"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	fprintf(fp,"Back Trace \n");
	for(count=0 ; count < (((bpf_rm_svrm_sigget_reg_t*)buf_p)->bktrc_num); count++ ){
	fprintf(fp,"[%02d] %s\n",count,(UCHAR*)&(((bpf_rm_svrm_sigget_reg_t*)buf_p)->bktrc_txt[count][0]));
	}
	fprintf(fp,"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	fprintf(fp,"Stack Addr : %016lX Stack Dump size: %016ldbytes \n\n",((bpf_rm_svrm_sigget_reg_t*)buf_p)->stack_addr ,((bpf_rm_svrm_sigget_reg_t*)buf_p)->stack_size);
	bpf_rm_svrm_mem_to_print2(fp, ((bpf_rm_svrm_sigget_reg_t*)buf_p)->dump_addr,(CHAR*)(((bpf_rm_svrm_sigget_reg_t*)buf_p)->stack_info) ,((bpf_rm_svrm_sigget_reg_t*)buf_p)->stack_size);
	fprintf(fp,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	fprintf(fp,"CPU Reg   fault_address :%016lX PC :%016lX SP :%016lX LR :%016lX PSR:%016lX\n\n"\
			   "r0 :%016lX r1 :%016lX r2 :%016lX r3 :%016lX r4 :%016lX r5 :%016lX \n"\
			   "r6 :%016lX r7 :%016lX r8 :%016lX r9 :%016lX r10:%016lX r11:%016lX \n"\
			   "r12:%016lX r13:%016lX r14:%016lX r15:%016lX r16:%016lX r17:%016lX \n"\
			   "r18:%016lX r19:%016lX r20:%016lX r21:%016lX r22:%016lX r23:%016lX \n"\
			   "r24:%016lX r25:%016lX r26:%016lX r27:%016lX r28:%016lX r29:%016lX \n",
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.fault_address,
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_pc,
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_sp,
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[30],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_pstate,
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[0],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[1],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[2],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[3],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[4],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[5],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[6],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[7],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[8],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[9],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[10],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[11],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[12],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[13],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[14],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[15],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[16],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[17],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[18],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[19],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[20],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[21],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[22],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[23],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[24],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[25],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[26],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[27],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[28],
				((bpf_rm_svrm_sigget_reg_t*)buf_p)->cpu_reg.arm64_reg[29]
				);

	fprintf(fp,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	fprintf(fp,"Status :%s(%dbytes)\n",
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->procfile_info[D_BPF_RM_SVRM_PRCFILE_STATUS].filename,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->procfile_info[D_BPF_RM_SVRM_PRCFILE_STATUS].data_len);
	fprintf(fp,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	fprintf(fp,"%s",((bpf_rm_svrm_sigget_reg_t*)buf_p)->procfile_info[D_BPF_RM_SVRM_PRCFILE_STATUS].data);
	fprintf(fp,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	fprintf(fp,"Maps :%s(%dbytes)\n",
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->procfile_info[D_BPF_RM_SVRM_PRCFILE_MAPS].filename,
			((bpf_rm_svrm_sigget_reg_t*)buf_p)->procfile_info[D_BPF_RM_SVRM_PRCFILE_MAPS].data_len);
	fprintf(fp,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	fprintf(fp,"%s",((bpf_rm_svrm_sigget_reg_t*)buf_p)->procfile_info[D_BPF_RM_SVRM_PRCFILE_MAPS].data);
	/* table情報先頭アドレスを取得 */
	tbl_p = (bpf_rm_svrm_sigget_tbl_t*)(buf_p + sizeof(bpf_rm_svrm_sigget_reg_t));
	fprintf(fp,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	fprintf(fp,"Process internal table information (table num is %d)\n",((bpf_rm_svrm_sigget_reg_t*)buf_p)->tblnum);
	fprintf(fp,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	for(count = 0 ;
		count < ((bpf_rm_svrm_sigget_reg_t*)buf_p)->tblnum ;
		count++)
	{
		if((tbl_p == NULL) || (tbl_p == (void*)-1))		/* pgr0203 */
		{
			fprintf(fp,"tbl pointer error \n");
			break;
		}
		if((tbl_p->size == 0)||(tbl_p->size == 0xFFFFFFFF))
		{
			fprintf(fp,"tbl size ng %d \n",tbl_p->size);
			break;
		}
		if( strlen( (CHAR*)tbl_p->name ) > sizeof(tbl_p->name) )
		{
			fprintf(fp,"tbl name ng \n");
			break;
		}
		fprintf(fp,"===========================================================================\n");
		fprintf(fp,"No.%d table name = %s , size = 0x%08X(%d) , addr = 0x%p \n",
				count,
				tbl_p->name,
				tbl_p->size,
				tbl_p->size,
				(tbl_p->addr));
		fprintf(fp,"===========================================================================\n");
		/* テーブルの実体アドレスの取得 */
		tbl_buf_p = (CHAR*)(((CHAR*)tbl_p) + sizeof(bpf_rm_svrm_sigget_tbl_t));
		/* swap */
		bpf_rm_svrm_mem_swap(tbl_buf_p ,tbl_p->size,tbl_p->mode);
		tbl_p = (bpf_rm_svrm_sigget_tbl_t*)bpf_rm_svrm_mem_to_print(fp,tbl_buf_p ,tbl_p->size);
	}
	fprintf(fp,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	buf_p = (CHAR*)tbl_p;
	fflush(fp);
	return(buf_p);
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  プロセスID変換処理(シグナルハンドラ専用処理)
 *  @note   プロセスID変換処理(シグナルハンドラ専用処理)
 *  @param  pid		[in]  プロセスID(int)
 *  @param  pre*	[in]  プロセスID(文字列)
 *  @return int
 *  @retval size
 *  @date   2013/12/26 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static const CHAR bpf_rm_svrm_pidtoAsc[] = {'0','1','2','3','4','5','6','7','8','9'};
static INT bpf_rm_svrm_func_pidtoAsc(INT pid ,CHAR* pre)
{
	char decstr[10];
	int wpid;
	char* next;
	int cnt;
	int cnt2;

	wpid = pid;
	next = pre;

	if(pid ==  0)
	{
		*next = '0';
		next++;
		return (next - pre);
		
	}
	memset(decstr,'0',sizeof(decstr));
	if(pid >= 1000000000)
	{
		decstr[0]  = bpf_rm_svrm_pidtoAsc[(wpid/1000000000)];
		wpid %= 1000000000;
	}
	if(pid >= 100000000)
	{
		decstr[1]  = bpf_rm_svrm_pidtoAsc[(wpid/100000000)];
		wpid %= 100000000;
	}
	if(pid >= 10000000)
	{
		decstr[2]  = bpf_rm_svrm_pidtoAsc[(wpid/10000000)];
		wpid %= 10000000;
	}
	if(pid >= 1000000)
	{
		decstr[3]  = bpf_rm_svrm_pidtoAsc[(wpid/1000000)];
		wpid %= 1000000;
	}
	if(pid >= 100000)
	{
		decstr[4]  = bpf_rm_svrm_pidtoAsc[(wpid/100000)];
		wpid %= 100000;
	}
	if(pid >= 10000)
	{
		decstr[5]  = bpf_rm_svrm_pidtoAsc[(wpid/10000)];
		wpid %= 10000;
	}
	if(pid >= 1000)
	{
		decstr[6]  = bpf_rm_svrm_pidtoAsc[(wpid/1000)];
		wpid %= 1000;
	}
	if(pid >= 100)
	{
		decstr[7]  = bpf_rm_svrm_pidtoAsc[(wpid/100)];
		wpid %= 100;
	}
	if(pid >= 10)
	{
		decstr[8]  = bpf_rm_svrm_pidtoAsc[(wpid/10)];
		wpid %= 10;
	}
	decstr[9] = bpf_rm_svrm_pidtoAsc[(wpid)];

	for(cnt = 0;cnt < 10; cnt++)
	{
		if(decstr[cnt] != '0')
		{	
			for(cnt2 = cnt;cnt2 < 10; cnt2++)
			{
				*next = decstr[cnt2];
				next++;
			}
			break;
		}
	}
	return (next - pre);
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  ローカルテーブル保存処理
 *  @note   ローカルテーブル保存処理
 *  @param  fd			[in]ファイルディスクリプタ
 *  @param  tblnum		[in]テーブル登録数
 *  @param  tbl_top_p	[in]テーブル先頭アドレス
 *  @date   2013/11/10 FJT)Taniguchi create
 */
/********************************************************************************************************************/
void BPF_RM_SVRM_SIGNAL_TBLSAVE( int fd , int tblnum , char* tbl_top_p)
{
#ifndef OPT_RRE_ZYNQ_SIM
	unsigned int count;
	bpf_rm_svrm_sigget_tbl_t* tbl_p = (bpf_rm_svrm_sigget_tbl_t*)tbl_top_p;
	ssize_t  ret;

	if( tbl_p == NULL ) 
	{
		return;
	}
	
	for(count = 0 ; count < tblnum ; count++ , tbl_p++ ) /* pgr0689 */
	{
		if((tbl_p == NULL) || (tbl_p->size == 0) || (tbl_p->addr == 0))
		{
			break;
		}
		ret = write(fd,tbl_p,sizeof(*tbl_p));
		ret = write(fd,tbl_p->addr,tbl_p->size);
		ret = ret;
	}
#else
	printf("SIGNAL TBLSAVE %d \n",tblnum);
#endif
	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Signal Handler共通関数
 *  @note   Signal Handler共通待ち受け処理
 *  @param  signum					[in]  Signal番号
 *  @param  info*					[in]  Signal情報
 *  @param  ctx*					[in]  Contex情報
 *  @return int
 *  @retval BPF_RM_SVRM_COMPLETE
 *  @retval BPF_RM_SVRM_ERR_EXEC
 *  @date   2013/11/10 ALPHA)yokoyama create
 */
/********************************************************************************************************************/
void BPF_RM_SVRM_SIGNAL_HANDLE_COMMON( int signum, siginfo_t *siginfo, void *ctx )
{
#ifndef OPT_RRE_ZYNQ_SIM
	ULONG		start_ofs = 0xeeeeeeeeeeeeeeee;
	/* スタックは最低限とすること */
	INT			status;	/* CHILDになる要因を設定 */
	INT			fd;
	INT			procfd;
	UINT		count;
	INT			size;
	INT			len;
	ssize_t  ret;

	/* 本関数では非同期スレッドセーフな関数のみ使用可能 */
	status = -1;
	/* 全シグナル共通情報を取得 */
	(void)clock_gettime( CLOCK_REALTIME_COARSE , &(bpf_rm_svrm_sigget_reg.almtime));	/* 時刻取得(system時刻) */
	(void)clock_gettime( CLOCK_MONOTONIC_RAW   , &(bpf_rm_svrm_sigget_reg.boottime));	/* 時刻取得(bootup時刻) */
	bpf_rm_svrm_sigget_reg.tid = (int)syscall(SYS_gettid);
	bpf_rm_svrm_sigget_reg.pid = getpid();	
	bpf_rm_svrm_sigget_reg.ppid = getppid();
	bpf_rm_svrm_sigget_reg.si_signo = 0xFF;

	/* シグナル固有情報を取得 */
	if(siginfo != NULL )
	{
		status = siginfo->si_signo;
		bpf_rm_svrm_sigget_reg.si_signo = siginfo->si_signo;
		bpf_rm_svrm_sigget_reg.si_code  = siginfo->si_code;
		bpf_rm_svrm_sigget_reg.si_errcode = siginfo->si_errno;

		/* signalによって、OSが設定するsiginfoは異なる */
		switch (siginfo->si_signo)
		{	
			case SIGCHLD:
				bpf_rm_svrm_sigget_reg.sig_pid   = siginfo->si_pid;
				bpf_rm_svrm_sigget_reg.sig_uid   = siginfo->si_uid;
				bpf_rm_svrm_sigget_reg.sig_status  = siginfo->si_status;
				bpf_rm_svrm_sigget_reg.sig_utime  = (INT)(siginfo->si_utime);
				bpf_rm_svrm_sigget_reg.sig_stime  = (INT)(siginfo->si_stime);
				break;
			case SIGTERM:
				bpf_rm_svrm_sigget_reg.sig_pid   = siginfo->si_pid;
				bpf_rm_svrm_sigget_reg.sig_uid   = siginfo->si_uid;
				bpf_rm_svrm_sigget_reg.sig_value = siginfo->si_value.sival_int;
				break;
			case SIGILL:
			case SIGFPE:
			case SIGSEGV:
			case SIGBUS:
				bpf_rm_svrm_sigget_reg.sig_addr = (ULONG)(siginfo->si_addr);
				break;
			default:
				break;
		}
	}
	/* context情報を取得 */
	if(ctx != NULL)
	{
		void *             bktrc_ptr[2];
		char **            bk_txt = NULL;
		ULONG*			   stack_value;
		ULONG			   stack_size;
		ULONG			   total_stack_size;


		bpf_rm_svrm_sigget_reg.bktrc_num = 0;
		bktrc_ptr[0] = (void*)(((ucontext_t *)ctx)->uc_mcontext.pc);		/* pc */
		bpf_rm_svrm_sigget_reg.bktrc_num++;
		bktrc_ptr[1] = (void*)(((ucontext_t *)ctx)->uc_mcontext.regs[30]);	/* lr */
		bpf_rm_svrm_sigget_reg.bktrc_num++;
		bk_txt = backtrace_symbols(bktrc_ptr, bpf_rm_svrm_sigget_reg.bktrc_num);
		if(bk_txt != NULL){
			for(count = 0;count < bpf_rm_svrm_sigget_reg.bktrc_num ; count++){
				if((size = strlen(bk_txt[count])) >= 1024){
					size = 1023;
				}
				memcpy(&(bpf_rm_svrm_sigget_reg.bktrc_txt[count][0]),bk_txt[count],size);
			}
			free(bk_txt);
		}

		bpf_rm_svrm_sigget_reg.stack_addr = ((ucontext_t *)ctx)->uc_mcontext.sp;
		stack_value = (ULONG*)(((ucontext_t *)ctx)->uc_mcontext.sp);
		for(count=0 ,stack_size = 0 , total_stack_size = 0; *stack_value != 0 ; count++){
			stack_size = *stack_value - (ULONG)stack_value;
			total_stack_size += stack_size;
			if(total_stack_size >= 0x1000){
				break;
			}
			stack_value += stack_size / sizeof(ULONG);
		}

		bpf_rm_svrm_sigget_reg.dump_addr = (ULONG)&start_ofs;
		total_stack_size += bpf_rm_svrm_sigget_reg.stack_addr - (ULONG)&start_ofs;
		memcpy((void*)&(bpf_rm_svrm_sigget_reg.stack_info[0]),
				(void*)(bpf_rm_svrm_sigget_reg.stack_addr - (bpf_rm_svrm_sigget_reg.stack_addr - (ULONG)&start_ofs)),
				total_stack_size);

		total_stack_size += 32 - (total_stack_size % 32);
		bpf_rm_svrm_sigget_reg.stack_size = total_stack_size;

		memcpy(&(bpf_rm_svrm_sigget_reg.cpu_reg),&(((ucontext_t *)ctx)->uc_mcontext),sizeof(bpf_rm_svrm_sigget_cpureg_t));	/* pgr0548 */


	}

	/* procログファイル作成開始 */
	bpf_rm_svrm_sigget_reg.prcfilenum = 0;
	for(count = D_BPF_RM_SVRM_PRCFILE_STATUS ;count < D_BPF_RM_SVRM_PRCFILE_MAXNUM ; count++)
	{		
		/* proc fileの名前を作成 */
		len = bpf_rm_svrm_sigget_filename[D_BPF_RM_SVRM_PRCFILE_PATH].size - 1;

		len += bpf_rm_svrm_func_pidtoAsc(
				bpf_rm_svrm_sigget_reg.tid,
				&bpf_rm_svrm_sigget_file[count].filename[len]);

		memcpy(&bpf_rm_svrm_sigget_file[count].filename[len],
				bpf_rm_svrm_sigget_filename[count].name,
				bpf_rm_svrm_sigget_filename[count].size);

		/* proc file情報を収集 */
		if ((procfd = open(bpf_rm_svrm_sigget_file[count].filename, O_RDONLY)) > 0){
			len = 0;
			/* procfileから4096byte単位でデータ取得  */
			while((size = read(procfd,
							bpf_rm_svrm_sigget_reg.procfile_info[count].data + len,
							sizeof(bpf_rm_svrm_sigget_reg.procfile_info[count].data) - len))
				 > 0)
			{
				len += size;
			}
			close(procfd);

			strncpy(bpf_rm_svrm_sigget_reg.procfile_info[count].filename,
					bpf_rm_svrm_sigget_file[count].filename,
					sizeof(bpf_rm_svrm_sigget_reg.procfile_info[count].filename) - 1);

			bpf_rm_svrm_sigget_reg.procfile_info[count].data_len = len;
			bpf_rm_svrm_sigget_reg.prcfilenum++;
		}
	}

	/* coreファイル作成開始 */
	if ((fd = open((const char*)bpf_rm_svrm_sigget_logfilename, O_WRONLY | O_CREAT | O_TRUNC , 0666)) <= 0)
	{
		printf("Failure signal happen !!!!!! But coredump file open error ... \n");
		(void)BPF_RM_SVRM_SIGNAL_COREDUMP(stdout,(char*)&bpf_rm_svrm_sigget_reg,sizeof(bpf_rm_svrm_sigget_reg));
		exit(status);
	}
	bpf_rm_svrm_sigget_reg.tblnum = 0;
	if( bpf_rm_svrm_sigget_proc_func_p != NULL )
	{	
		/* ログテーブル数取得 */
		bpf_rm_svrm_sigget_reg.tblnum = bpf_rm_svrm_sigget_proc_func_p(0,NULL,NULL,0);
	}
	ret = write(fd,&bpf_rm_svrm_sigget_reg,sizeof(bpf_rm_svrm_sigget_reg));
	if( bpf_rm_svrm_sigget_reg.tblnum > 0 )
	{
		/* UserのSignal Handler関数をCall	*/
		(void)bpf_rm_svrm_sigget_proc_func_p(signum, siginfo, ctx ,fd);
	}
	ret = ret;	/* warning対策 */
	/* coreファイル作成終了 */
	fsync(fd);
	close(fd);

	int sys_ret = system(bpf_rm_svrm_sigget_logfilecmd);
	if(sys_ret == -1){
		printf("cmd error %s\n",bpf_rm_svrm_sigget_logfilecmd);
	}

	/* bpf_rm_svrm_sigget_reg.sig_valueの値によって判断する */
	/* 値が0の場合は例外発生と判断し,0以外の場合は、userabortと判断する */
	union sigval	value;
	if ( bpf_rm_svrm_sigget_reg.sig_value == 0)
	{
		value.sival_int = D_RRH_SIGUSR_OPT_RSET_SWALM_NTC;
		if( bpf_rm_svrm_sigget_reg.si_signo < (sizeof(bpf_rm_svrm_sig_list)/sizeof(bpf_rm_svrm_sig_list[0])))
		{
			value.sival_int |= bpf_rm_svrm_sig_list[ bpf_rm_svrm_sigget_reg.si_signo ].almcode;
		}
		else
		{
			value.sival_int |= D_DU_ALMCD_ANNG2;
		}
	}
	else
	{
		value.sival_int = bpf_rm_svrm_sigget_reg.sig_value ;
	}
	
	/* mutex lock強制解放	*/
	BPF_HM_DEVC_MUTEX_LOCK_FREE();
	
	/* send to init */
	if( bpf_rm_svrm_sig_sndonoff > 0)
	{
		sigqueue(bpf_rm_svrm_sigget_reg.ppid,	 /* 親(init)のPID		*/
			 SIGUSR1,						 /* Signal番号			*/
			 value);						 /* オプション			*/
	}
	/* no send */
	else
	{
		printf("Failure signal happen !!!!!! \n");
		bpf_rm_svrm_sigget_reg.tblnum = 0;
		(void)BPF_RM_SVRM_SIGNAL_COREDUMP(stdout,(char*)&bpf_rm_svrm_sigget_reg,sizeof(bpf_rm_svrm_sigget_reg));
	}

	/* exitの下位1byteは指定した値がそのまま送信される */
	exit(status & 0xFF);
#else
	printf("SIGNAL HAPPEN %d \n",siginfo->si_signo);
	exit((siginfo->si_signo) & 0xFF);
#endif
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Signal Handler登録
 *  @note   Signal Handler登録及びSignal待ち受けのための初期設定を行う
 *  @param  void*					[in]  Signal登録する関数のポインタ
 *  @param  sigset_t*				[out] Signal受信のための情報
 *  @return int
 *  @retval BPF_RM_SVRM_COMPLETE
 *  @retval BPF_RM_SVRM_ERR_EXEC
 *  @date   2013/11/10 ALPHA)yokoyama create
 */
/********************************************************************************************************************/
int BPF_RM_SVRM_SIGNAL_HANDLE_ENTRY( int* funcP, sigset_t* sigSetVal , int sig_sndonoff )
{
/* Signal登録すると、そのプロセスで何かが起こったとき、登録した関数が呼ばれます				*/
/* 以下コールされる例																		*/
/* void signal_term(int signum, siginfo_t *info, void *ctx)									*/
/*		signumまたはinfo->si_signo = Signal番号(10=SIGUSR1)									*/
/*		info->si_code = 4byteのオプション情報												*/
/*		ctx = コンテキスト情報																*/
/*																							*/
/* 各プロセスはプロセス固有の情報とコンテキスト情報をファイルに出力すること					*/
/* 出力先 = /var/log/troublelog_rst															*/
/* ファイル名 = プロセス名.log																*/
/* なお、ハンドラ関数でfopen/fwriteは使用できないのでOPEN/WRITEを使用すること				*/
#ifndef OPT_RRE_ZYNQ_SIM
	struct sigaction	signalInfo;
	UINT				loop;
	char				prcname[128];
	int					fd;
	int					len;

	/* 各プロセス毎に取得する情報を設定する */
	memset(&bpf_rm_svrm_sigget_reg,0,sizeof(bpf_rm_svrm_sigget_reg));
	/* ログ対象のprocファイルシステムを登録 */
	memset(bpf_rm_svrm_sigget_file,0,sizeof(bpf_rm_svrm_sigget_prc_t) * D_BPF_RM_SVRM_PRCFILE_MAXNUM);
	for(loop = D_BPF_RM_SVRM_PRCFILE_STATUS ; loop < D_BPF_RM_SVRM_PRCFILE_MAXNUM ; loop++ )
	{
		memcpy(&bpf_rm_svrm_sigget_file[loop].filename[0],
			   bpf_rm_svrm_sigget_filename[D_BPF_RM_SVRM_PRCFILE_PATH].name,
			   bpf_rm_svrm_sigget_filename[D_BPF_RM_SVRM_PRCFILE_PATH].size);
	}
	/* ユーザログ取得関数登録	*/
	bpf_rm_svrm_sigget_proc_func_p = NULL;
	if(funcP != NULL)
	{
		bpf_rm_svrm_sigget_proc_func_p = (int (*)(int, siginfo_t *, void * ,int))funcP;
	}
	/* logファイル名を作成 */
	/* ログ出力例) /var/log/troublelog_rst/coredump_rrh_pf.bin */
	memset((void*)bpf_rm_svrm_sigget_logfilename,0,sizeof(bpf_rm_svrm_sigget_logfilename));
	memset((void*)bpf_rm_svrm_sigget_txtfilename,0,sizeof(bpf_rm_svrm_sigget_txtfilename));

	if ((fd = open("/proc/self/comm", O_RDONLY)) > 0)
	{
		len = read(fd,prcname,sizeof(prcname));
		close(fd);
		prcname[len - 1] = '\0';
	}
	else
	{
		snprintf(prcname,sizeof(prcname),"unknown");
	}
	/* テンポラリ領域設定 */
	snprintf( (void*)bpf_rm_svrm_sigget_logfilename,
			  sizeof(bpf_rm_svrm_sigget_logfilename),
			  "/tmp/coredump_%s.bin",
			  prcname
			);
	/* ログ領域設定 */
	snprintf( (void*)bpf_rm_svrm_sigget_txtfilename,
			  sizeof(bpf_rm_svrm_sigget_txtfilename),
			  "/var/log/troublelog_rst/coredump_%s.txt",
			  prcname
			);
	/* プロセス名設定 */
	snprintf((void*)bpf_rm_svrm_sigget_reg.pname,
			 sizeof(bpf_rm_svrm_sigget_reg.pname),
			 "%s",
			  prcname
			);

	memset(bpf_rm_svrm_sigget_logfilecmd,0,sizeof(bpf_rm_svrm_sigget_logfilecmd));
	snprintf(bpf_rm_svrm_sigget_logfilecmd,sizeof(bpf_rm_svrm_sigget_logfilecmd),"/opt/debug/cmd_coredump %s %s",
		bpf_rm_svrm_sigget_logfilename,bpf_rm_svrm_sigget_txtfilename);
	
	/* sigqueue有無設定 */
	bpf_rm_svrm_sig_sndonoff = sig_sndonoff;

	/* シグナルハンドラ設定 */
	memset(&signalInfo, 0, sizeof(signalInfo));
	signalInfo.sa_sigaction = BPF_RM_SVRM_SIGNAL_HANDLE_COMMON;
	signalInfo.sa_flags = SA_SIGINFO;
	sigemptyset(&signalInfo.sa_mask);
	for (loop = 0; loop < (sizeof(bpf_rm_svrm_sig_list) / sizeof(bpf_rm_svrm_sig_list[0])); loop++)
	{
		if(bpf_rm_svrm_sig_list[loop].sighdlflg == 1){
			(void)sigaddset(&signalInfo.sa_mask,bpf_rm_svrm_sig_list[loop].signo);
		}
	}
	for (loop = 0 ; loop < (sizeof(bpf_rm_svrm_sig_list) / sizeof(bpf_rm_svrm_sig_list[0])) ; loop++)
	{
		if(bpf_rm_svrm_sig_list[loop].sighdlflg == 0){
			continue;
		}else if(bpf_rm_svrm_sig_list[loop].sighdlflg == 2){
			/* signal無視 */
			(void)signal(bpf_rm_svrm_sig_list[loop].signo, SIG_IGN);
		}else{
			(void)sigaction(bpf_rm_svrm_sig_list[loop].signo, &signalInfo, NULL);
		}
	}

	/* 受信用に各種設定を行い、ユーザ側にデータを返す	*/
	sigemptyset( sigSetVal );	
	/* SIGUSR1/SIGCHLDを受信できるようにする	*/
	(void)sigaddset( sigSetVal, SIGUSR1 );
	(void)sigaddset( sigSetVal, SIGUSR2 );
	(void)sigaddset( sigSetVal, SIGCHLD );
	(void)sigprocmask(SIG_BLOCK, sigSetVal, NULL);
#else
	printf("SIGNAL HANDLE ENTRY is OK\n");
#endif
	return BPF_RM_SVRM_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Signal送信関数
 *  @note   Signal送信関数
 *  @param  process ID				[in] 送信先プロセスID
 *  @param  signal number			[in] Signal番号
 *  @param  signal option			[in] Signalオプション
 *  @return int
 *  @retval BPF_RM_SVRM_COMPLETE
 *  @date   2013/11/10 ALPHA)yokoyama create
 */
/********************************************************************************************************************/
void BPF_RM_SVRM_SIGNAL_SEND(int pid, int signo, int option)
{
	union sigval		value ;
	T_RRH_PID_MNG		*procIdTbl;
	int					realPID;
	int					errcd;
	unsigned int		count;
	
	/* オプションを設定	*/
	value.sival_int = option;
	
	/* プロセスID→実プロセスIDに変換	*/
	/************************************************************/
	/* プロセスID管理テーブルを共有メモリから取得               */
	/************************************************************/
	/* 共有メモリ取得 */
	(void)BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_PROC_ID_MNG_INFO,
		(VOID **)&procIdTbl,
		&errcd
	);
	
	realPID = 254;
	for( count = 0; count < procIdTbl->count; count++ )
	{
		if( pid == procIdTbl->info[ count ].aplProcessID )
		{
			realPID = procIdTbl->info[ count ].realProcessID;
			break;
		}
	}
	if(count >= procIdTbl->count){
		for( count = 0; count < procIdTbl->mpsw_count; count++ ){
			if(
				(procIdTbl->mpsw_info[ count ].aplProcessID != 0) &&
				(pid == procIdTbl->mpsw_info[ count ].aplProcessID)
			){
				realPID = procIdTbl->mpsw_info[ count ].realProcessID;
				break;
			}
		}
	}	
	
	/* ガード	*/
	if( realPID != 254 )
	{
		/* signalを送信 */
		sigqueue(	realPID, 			/* 送信先プロセス番号	*/
					signo,				/* Signal番号			*/
					value );			/* オプション			*/
	}
	else
	{
		SVRM_DBGLOG_FMT_ERR("Unknown process! pid:%d",pid);
	}
	
	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Signal待ち受け関数
 *  @note   Signal待ち受け関数
 *  @param  sigSetVal				[in] Signal受信用データ
 *  @param  signal_pid				[out]Signal送信元PID
 *  @param  signal_no				[out]Signal番号
 *  @param  signal_option			[out]Signalオプション
 *  @return int
 *  @retval BPF_RM_SVRM_COMPLETE
 *  @retval BPF_RM_SVRM_ERR_EXEC
 *  @date   2013/11/10 ALPHA)yokoyama create
 */
/********************************************************************************************************************/
int BPF_RM_SVRM_SIGNAL_WAIT( sigset_t* sigSetVal, int* signal_pid, int* signal_no, int* signal_option )
{
	siginfo_t			sigInfo;
	T_RRH_PID_MNG		*procIdTbl;
	int					errcd;
	unsigned int		count;
	
	/* sigwaitによる受信処理 */
	if ( sigwaitinfo( sigSetVal, &sigInfo) == -1 )
	{
		SVRM_DBGLOG_FMT_ERR("sigwaitinfo errno=%d",errno);
		return BPF_RM_SVRM_ERR_EXEC;
	}
	
	/* Signal送信元プロセスIDを返す	*/
	/* 実プロセスID→プロセスIDに変換	*/
	/************************************************************/
	/* プロセスID管理テーブルを共有メモリから取得               */
	/************************************************************/
	/* 共有メモリ取得 */
	(void)BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_PROC_ID_MNG_INFO,
		(VOID **)&procIdTbl,
		&errcd
	);
	
	*signal_pid = -1;
	for( count = 0; count < procIdTbl->count; count++ )
	{
		if( sigInfo.si_pid == procIdTbl->info[ count ].realProcessID )
		{
			*signal_pid = procIdTbl->info[ count ].aplProcessID;
			break;
		}
	}

	if(count >= procIdTbl->count){
		for( count = 0; count < procIdTbl->mpsw_count; count++ ){
			if( sigInfo.si_pid == procIdTbl->mpsw_info[ count ].realProcessID ){
				*signal_pid = procIdTbl->mpsw_info[ count ].aplProcessID;
				break;
			}
		}
	}

	/* Signal番号を返す	*/
	*signal_no = sigInfo.si_signo;
	/* Signalのオプションを返す	*/
	*signal_option = sigInfo.si_status;

	if((*signal_pid == -1) && (*signal_no == SIGCHLD)){
		/* 無視 */
		return BPF_RM_SVRM_ERR_EXEC;
	}
	
	return BPF_RM_SVRM_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Signal待ち受け関数(時間指定)
 *  @note   Signal待ち受け関数(時間指定)
 *  @param  sigSetVal				[in] Signal受信用データ
 *  @param  time					[in] 時間
 *  @param  signal_pid				[out]Signal送信元PID
 *  @param  signal_no				[out]Signal番号
 *  @param  signal_option			[out]Signalオプション
 *  @return int
 *  @retval BPF_RM_SVRM_COMPLETE
 *  @retval BPF_RM_SVRM_ERR_EXEC
 *  @date   2013/11/10 ALPHA)yokoyama create
 */
/********************************************************************************************************************/
int BPF_RM_SVRM_SIGNAL_WAIT_TIME( sigset_t* sigSetVal, struct timespec *time, int* signal_pid, int* signal_no, int* signal_option )
{
	siginfo_t			sigInfo;
	T_RRH_PID_MNG		*procIdTbl;
	int					errcd;
	unsigned int		count;
	
	/* sigwaitによる受信処理 */
	if ( sigtimedwait( sigSetVal, &sigInfo, time ) == -1 )
	{
		return BPF_RM_SVRM_ERR_EXEC;
	}
	
	/* Signal送信元プロセスIDを返す	*/
	/* 実プロセスID→プロセスIDに変換	*/
	/************************************************************/
	/* プロセスID管理テーブルを共有メモリから取得               */
	/************************************************************/
	/* 共有メモリ取得 */
	(VOID)BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_PROC_ID_MNG_INFO,
		(VOID **)&procIdTbl,
		&errcd
	);
	
	/* 一旦実プロセスIDを保持する	*/
	*signal_pid = -1;
	for( count = 0; count < procIdTbl->count; count++ )
	{
		if( sigInfo.si_pid == procIdTbl->info[ count ].realProcessID )
		{
			*signal_pid = procIdTbl->info[ count ].aplProcessID;
			break;
		}
	}

	/* Signal番号を返す	*/
	*signal_no = sigInfo.si_signo;
	/* Signalのオプションを返す	*/
	*signal_option = sigInfo.si_status;

	if((*signal_pid == -1) && (*signal_no == SIGCHLD)){
		char proc_name[32];
		memset(proc_name,0,sizeof(proc_name));
		bpf_rm_svrm_l_get_prcname(sigInfo.si_pid,sizeof(proc_name) - 1, proc_name);
		SVRM_DBGLOG_FMT_ERR("Unknown process! pid:%d(%s) status:%d",sigInfo.si_pid ,proc_name,  sigInfo.si_status);
		return BPF_RM_SVRM_ERR_EXEC;
	}
	
	return BPF_RM_SVRM_COMPLETE;
}
/* @} */
/* @} */
/* RRH-001 MD alpha)yokoyama add end */
