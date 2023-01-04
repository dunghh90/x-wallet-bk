#ifndef RRUPKTDUMP_H
#define RRUPKTDUMP_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>
#include <sched.h>

#define D_RRUPKTDUMP_TYPE_UNKNOWN 0
#define D_RRUPKTDUMP_TYPE_EXTERNL 1
#define D_RRUPKTDUMP_TYPE_INTERNL 2
#define D_RRUPKTDSP_MSIZE		128
#define D_RRUPLOG_IP_MAX		0x7FFFFFFF
#define D_RRUPLOG_IP_MIN		0x7F000000
#define D_RRUPLOG_IP_OFS		0x7F010000
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
	int tv_sec;
	int tv_usec;
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

struct vlan_ethhdr {
	unsigned char	h_dest[ETH_ALEN];
	unsigned char	h_source[ETH_ALEN];
	unsigned short	h_vlan_proto;
	unsigned short	h_vlan_TCI;
	unsigned short	h_vlan_encapsulated_proto;
};

typedef struct
{
	struct vlan_ethhdr h_vlaneth;
}pcap_rrupf_vlanethheadr;

typedef struct
{
	struct	ip	h_ip;
	char	dummy[4];
}pcap_rrupf_ipheader;

typedef struct
{
	char	data[1514];
}pcap_rrupf_userheader;

typedef struct
{
	pcap_rrupf_ipheader		pcap_ip;
	pcap_rrupf_userheader	pcap_user;
}pcap_rrupf_msg_t;

typedef struct
{
	int sec;
	int usec;
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
	pcap_ptif_name*  ptif;
}pcap_proc_name;

extern int gmode;
extern void func_gmsg(const char* format, ...);

extern int func_userdata_dsp(
	FILE* fp,
	pcap_rrupf_userheader* user_p,
	int len,
	unsigned int index
);


extern unsigned int func_head_dsp(
	FILE* fp ,
	pcap_data_timeoffs_t* time_p,
	char* data
);

extern void func_help_fil_list( void );
extern void func_mirror_send( int len , char* data );

#endif
