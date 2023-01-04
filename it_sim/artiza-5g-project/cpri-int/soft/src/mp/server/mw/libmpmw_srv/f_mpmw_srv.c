/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   f_mpsw_msi.c
 *  @brief  Mplane Middleware function library for Server
 *  @date   2018/11/19 FJT)Taniguchi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2018
 */
/********************************************************************************************************************/
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/syscall.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "log.h"
#include "log_vendor.h"
#include "log_vendor_extern.h"

#include "f_mpsw_srv_types.h"
#include "f_mpmw_srv.h"
#include "f_mpmw_com.h"

/** @addtogroup MPMW_SRV
 * @{ */

static int mpmw_l_loglevel = (int)E_MPSW_SRV_LOG_ALL;	/* 試験中はすべて出力　*/
/* static int mpmw_l_loglevel = (int)E_MPSW_SRV_LOG_INFO;	*//* リリース時はINFO　*/

#define OPT_MPMW_DEBUG	0
#define OPT_MPMW_DEBUG_SEND_DONTWAIT	0

#define D_MPMW_SRV_COM_DATA			4096
#define D_MPMW_SRV_LOGSEND_IPOFS	0x7F020000

typedef	struct{
	int				loglevel;							/* log level */
	char			text[D_MPMW_SRV_COM_DATA];			/* text data		 */
}t_log_com;


typedef struct {
	char				magicno[4];		/* magicno */
	int					len;			/* info length */
	struct timespec		tcount;			/* bootup time */
	union {
		t_log_com		com;			/* com data log */
	}info;
} t_mpmw_srv_com_data;

typedef struct {
								/* pcap headr   16byte */
								/* ether headr  12byte */
	struct	ip				head;	/* ip heade r   20byte */
	char					dummy[4];
	t_mpmw_srv_com_data		data;	/* user data   4096byte */
} t_mpmw_srv_com_msg;

/********************************************************************************************************************/
/*!
 *  @brief  set log header
 *  @note   set log header
 *  @param  dst                [in]  dst address
 *  @param  len                [in]  length
 *  @param  data_p             [in]  data pointer
 *  @return header size
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static inline int mpmw_srv_com_l_set_headr(
	unsigned int				dst,
	int							len,
	void*						data_p
)
{
	struct ip* header = (struct ip*)data_p;
	int self = (int)syscall(SYS_gettid);	
	header->ip_v          = 4;				/* ipV4 */
	header->ip_hl         = sizeof(*header)/4;
	header->ip_tos        = 0;
	header->ip_len        = htons(len);
	header->ip_id         = 0;				/* freeID */
	header->ip_off        = 0;				/* dont flagment */
	header->ip_ttl        = 255;
	header->ip_p          = IPPROTO_RAW;
	header->ip_sum        = 0;
	header->ip_src.s_addr = htonl(self);
	header->ip_dst.s_addr = dst;
	return (sizeof(*header));
}

/********************************************************************************************************************/
/*!
 *  @brief  Log send
 *  @note   SLog send
 *  @param  app                [in]  application no
 *  @param  data_p             [in]  log data
 *  @param  ipaddr_ofs         [in]  log send address offset
 *  @return N/A
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static void mpmw_l_send_to_lo(
	int							app,
	t_mpmw_srv_com_msg*			data_p,
	unsigned int				ipaddr_ofs
)
{
	struct sockaddr_in	sin;
	int					soc;
	int					rtn;
	int					onoff=1;
	int					datalen;
	int					offset=0;

	if(data_p == NULL) {
		return;
	}
	if((data_p->data.len) > (int)sizeof(t_mpmw_srv_com_msg)) {	
		return;
	}
	if((soc = socket(PF_INET, SOCK_RAW, IPPROTO_RAW) )==-1) {
		return;
	}
	if((rtn = setsockopt(soc, IPPROTO_IP, IP_HDRINCL, &onoff, sizeof(int))) == -1) {
		close(soc);
		return;
	}
	/* Raw socket Setting */
	memset(&sin, 0, sizeof(struct sockaddr_in));
	offset = f_mpmw_get_offset(data_p->data.info.com.text);
	sin.sin_family      = PF_INET;
	sin.sin_addr.s_addr = htonl((unsigned int)(ipaddr_ofs | (unsigned char)(app + 1 + offset)));
	sin.sin_port        = 0;
	(void)clock_gettime( CLOCK_MONOTONIC_COARSE , &(data_p->data.tcount) );
	/* msg setting */
	memcpy(data_p->data.magicno,(const char*)"MPMW",4);	/* pgr0832 */
	datalen = sizeof(data_p->data.magicno)+sizeof(data_p->data.len)+sizeof(data_p->data.tcount);
	datalen += data_p->data.len;
	datalen += mpmw_srv_com_l_set_headr((int)(sin.sin_addr.s_addr),datalen,data_p);
	datalen += 4;	/* alignment */
#if OPT_MPMW_DEBUG_SEND_DONTWAIT
	(void)sendto(soc, data_p, datalen, MSG_DONTWAIT , (struct sockaddr *) &sin, sizeof(sin));
#else
	(void)sendto(soc, data_p, datalen, 0 , (struct sockaddr *) &sin, sizeof(sin));
#endif
	close(soc);
	return;
}


/********************************************************************************************************************/
/*!
 *  @brief  SERVER LOG LEVEL SET
 *  @note   SERVER LOG LEVEL SET
 *  @param  loglevel           [in]  log level
 *  @return N/A
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
void MPMW_SRV_LOG_SET_LEVEL(int loglevel)
{
	mpmw_l_loglevel = loglevel;
	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  SERVER LOG LEVEL GET
 *  @note   SERVER LOG LEVEL GET
 *  @param  N/A
 *  @return loglevel
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
int MPMW_SRV_LOG_GET_LEVEL(void)
{
	return mpmw_l_loglevel;
}


/********************************************************************************************************************/
/*!
 *  @brief  SERVER LOG 
 *  @note   SERVER LOG 
 *  @param  app              [in]  NSA/SAA
 *  @param  loglevel         [in]  log level
 *  @param  format           [in]  format
 *  @param  ...              [in]  ...
 *  @return N/A
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
void MPMW_SRV_LOG(int app , int loglevel,const char *format, va_list args)
{
	
	if(mpmw_l_loglevel < loglevel){
		return;
	}

	t_mpmw_srv_com_msg    msg;
	int textlen;

	msg.data.info.com.loglevel = loglevel;
	vsnprintf( msg.data.info.com.text , sizeof(msg.data.info.com.text) , format, args );
	textlen = strlen(msg.data.info.com.text);
	textlen++;
	if(textlen >= D_MPMW_SRV_COM_DATA){
		msg.data.info.com.text[D_MPMW_SRV_COM_DATA - 1] = '\0';
		textlen = D_MPMW_SRV_COM_DATA;
	}
#if OPT_MPMW_DEBUG
	if(app != 4){
		printf("<app:%d lvl:%d pid(%d)>:%*s \n",app,loglevel,getpid(),textlen,msg.data.info.com.text );
	}
#endif
	msg.data.len = sizeof(msg.data.info.com.loglevel) + textlen;
	mpmw_l_send_to_lo(app ,&msg,D_MPMW_SRV_LOGSEND_IPOFS );
	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  register vendor log function 
 *  @note   register vendor log function  
 *  @date   2018/12/4 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static void
    fj_log_vendor_send_fn (log_debug_app_t app, log_debug_t level,
            const char *fstr, va_list args)
{
	MPMW_SRV_LOG(app , level,fstr,args);
	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  LOG INIT
 *  @note   LOG INIT 
 *  @param  N/A
 *  @return N/A
 *  @date   2018/12/4 FJT)Taniguchi create
 */
/********************************************************************************************************************/
void MPMW_SRV_LOG_INIT( void ){
	log_vendor_extern_register_send_fn(fj_log_vendor_send_fn);
	return;
}

/* @} */
