#ifndef RRUPKTDUMP_H
#define RRUPKTDUMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "BPF_COM_PLOG.h"
#include "bpf_com_plog.h"

#define D_RRUPKTDUMP_TYPE_UNKNOWN 0
#define D_RRUPKTDUMP_TYPE_EXTERNL 1
#define D_RRUPKTDUMP_TYPE_INTERNL 2
#define D_RRUPKTDSP_MSIZE		80
#define D_RRUPLOG_IP_MAX		0x7FFFFFFF
#define D_RRUPLOG_IP_MIN		0x7F000000
#define D_RRUPLOG_IP_OFS		0x7F010000
#define D_RRUPLOG_IP_OFS_MP		0x7F020000
#define D_RRUPKTDUMP_MAXFILESIZE	256	/* maxcount*256 */
#define D_RRUPKTDSP_FILNUM		5

typedef struct
{
   int      magic_no;
   short    version_major_no;
   short    version_minor_no;
   int      thiszone;
   int      sigfigs;
   int      snaplen;
   int      linktype;
}pcap_header_t;


typedef struct {
	unsigned int tv_sec;
	unsigned int tv_usec;
}pcap_data_header_time_t;

typedef struct
{
   pcap_data_header_time_t ts;
   int incl_len;  
   int orig_len;  
}pcap_data_header_t;

typedef struct
{
	struct ether_header h_eth;
}pcap_rrupf_ethheader;

typedef struct
{
	struct	ip	h_ip;
	char	dummy[4];
}pcap_rrupf_ipheader;

typedef struct
{
	t_bpf_com_plog_data	data;
}pcap_rrupf_userheader;

typedef struct
{
	unsigned int	eventNo;
	unsigned int	sigKind;
	unsigned int	dPrcQid;
	unsigned int	dThrdQid;
	unsigned int	sPrcQid;
	unsigned int	sThrdQid;
	unsigned int	len;	
	unsigned int	shmKeyid;
}pcap_rrupf_userheader_if;


typedef	struct{
	int				loglevel;							/* log level */
	char			text[4096];							/* text data		 */
}t_mplog_com;

typedef struct {
	char				magicno[4];		/* magicno */
	int					len;			/* info length */
	struct timespec		tcount;			/* bootup time */
	union {
		t_mplog_com		com;			/* com data log */
	}info;
} t_rrupf_mp_com_data;

typedef struct
{
	t_rrupf_mp_com_data	data;
}pcap_rrupf_userheader_mp;


typedef struct
{
	pcap_rrupf_ipheader		pcap_ip;
	pcap_rrupf_userheader	pcap_user;
}pcap_rrupf_msg_t;

typedef struct
{
	long sec;
	long usec;
}pcap_data_timeoffs_t;

typedef struct
{
	unsigned int eventNo;
	unsigned int sigKind;
	char*        signame;
	int (*func)(FILE* fp , char* data_p ,int datalen , unsigned int kind);
}pcap_ptif_name;

typedef struct
{
	char*            name;
}pcap_thrd_name;

typedef struct
{
	char*            name;
	pcap_thrd_name*  thrd;
	pcap_ptif_name*  ptif;
}pcap_proc_name;

extern int gmode;
extern void func_gmsg(const char* format, ...);

extern int func_userdata_dsp(
	FILE* fp,
	pcap_rrupf_userheader* user_p,
	int len,
	unsigned int procindex,
	unsigned int subindex,
	int srcthdno,
	unsigned int kind
);

extern int func_userdata_dsp_mp(
	FILE* fp,
	pcap_rrupf_userheader* user_p,
	int len,
	unsigned int procindex,
	unsigned int subindex,
	int srcthdno,
	unsigned int kind
);


extern int func_userdata_dsp_if(
	FILE* fp,
	pcap_rrupf_userheader_if* user_p,
	int len,
	unsigned int procindex
);


extern unsigned int func_iphead_dsp(
	FILE* fp ,
	pcap_data_timeoffs_t* time_p,
	pcap_rrupf_ipheader* data,
	unsigned int* subindex,
	unsigned int* kind
);

extern unsigned int func_iphead_dsp_if(
	FILE* fp ,
	pcap_data_timeoffs_t* time_p,
	pcap_rrupf_ipheader* data
);

extern void func_help_fil_list( void );
extern void func_help_fil_list_if( void );


#endif
