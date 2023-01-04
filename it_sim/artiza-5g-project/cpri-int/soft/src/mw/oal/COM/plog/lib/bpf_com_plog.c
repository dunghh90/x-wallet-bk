/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_com_plog.c
 *  @brief  BPF Common packet log Library.
 *  @date   2014/03/28 FJT)Taniguchi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2014
 */
/********************************************************************************************************************/
#define UNIT_ID BPF_COM_PLOG /* Pre-define */
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
#include <net/if.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>
#include "f_rrh_def.h"
#include "BPF_COM_PLOG.h"
#include "bpf_com_plog.h"

#if defined( PLOG_TIME_RAW )
#define D_BPF_COM_PLOG_CLOCK_ID	CLOCK_MONOTONIC_RAW				/* 低速だけど高精度 */
#else
#define D_BPF_COM_PLOG_CLOCK_ID	CLOCK_MONOTONIC_COARSE			/* 高速だけど低精度 */
#endif

unsigned int bpf_com_plog_tracekind = 0xFFFFFFFF;				/* トレース種別 */

/** @addtogroup BPF_COM
 * @{ */

/********************************************************************************************************************/
/**
 *  @brief  pakect log header set
 *  @note   Common function packet log library.
 *          This function is thread safe.
 *  @param  dst       [in]  dst ip addr (hex)
 *  @param  len       [in]  data length
 *  @param  data_p    [in]  data pointer
 *  @return int
 *  @retval header length
 *  @date   2014/03/28 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static inline int bpf_com_plog_set_headr(
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
/**
 *  @brief  pakect log send 
 *  @note   Common function packet log library.
 *          This function is thread safe.
 *  @param  level       [in]  log level
 *  @param  data_p      [in]  data pointer
 *  @param  ipaddr_ofs  [in]  ip address offset
 *  @return none
 *  @retval header length
 *  @date   2014/03/28 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static void bpf_com_plog_send_to_lo(
	e_bpf_com_plog_level		level,
	t_bpf_com_plog_msg*			data_p,
	unsigned int				ipaddr_ofs
)
{
	struct sockaddr_in	sin;
	int					soc;
	int					rtn;
	int					onoff=1;
	int					datalen;

	if(data_p == NULL) {
		return;
	}
	if((data_p->data.len) > sizeof(t_bpf_com_plog_msg)) {	
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
	sin.sin_family      = PF_INET;
	sin.sin_addr.s_addr = htonl((unsigned int)(ipaddr_ofs | (unsigned short)level));
	sin.sin_port        = 0;
	(void)clock_gettime( D_BPF_COM_PLOG_CLOCK_ID , &(data_p->data.tcount) );
	/* msg setting */
	memcpy(data_p->data.magicno,(char*)"PLOG",4);	/* pgr0832 */
	datalen = sizeof(data_p->data.magicno)+sizeof(data_p->data.len)+sizeof(data_p->data.tcount);
	datalen += data_p->data.len;
	datalen += bpf_com_plog_set_headr((int)(sin.sin_addr.s_addr),datalen,data_p);
	datalen += 4;	/* alignment */
	(void)sendto(soc, data_p, datalen, 0 , (struct sockaddr *) &sin, sizeof(sin));
	close(soc);
	return;
}
/********************************************************************************************************************/
/**
 *  @brief  get pakect log level
 *  @note   Common function packet log library.
 *          This function is thread safe.
 *  @param  name       [in]  log level file name
 *  @return e_bpf_com_plog_level
 *  @retval level
 *  @date   2014/03/28 FJT)Taniguchi create
 */
/********************************************************************************************************************/
int bpf_com_plog_get_level( char* name )
{
	int		fd;
	char	buf[16];
	int		val;
	
	if((fd = open(name,O_RDONLY)) <= 0)
	{
		return((int)E_DEF_LV);
	}
	if( read(fd,buf,sizeof(buf)) <= 0)
	{
		close(fd);
		return((int)E_DEF_LV);
	}
	if(buf[0] == '0')
	{
		val = 0;
	}
	else if((val = strtoul(buf,NULL,0)) == 0)
	{
		val = (int)E_DEF_LV;
	}
	else
	{
		;	/* ok */
	}
	close(fd);
	return (val);
}

/********************************************************************************************************************/
/**
 *  @brief  set pakect log level
 *  @note   Common function packet log library.
 *          This function is thread safe.
 *  @param  name       [in]  log level file name
 *  @return e_bpf_com_plog_level
 *  @retval level
 *  @date   2014/03/28 FJT)Taniguchi create
 */
/********************************************************************************************************************/
void bpf_com_plog_set_level( char* name , int level )
{
	int		fd;
	char	buf[10];
	int		count;
	ssize_t wsize __attribute__((unused));

	if((fd = open(name,O_RDWR)) <= 0)
	{
		return;
	}
	count = snprintf(buf,sizeof(buf),"%d",level);
	wsize =  write(fd,buf,count);
	close(fd);
	return;
}

/********************************************************************************************************************/
/**
 *  @brief  get pakect log for assert log
 *  @note   Common function packet log library.
 *          This function is thread safe.
 *  @param  lname      [in]  log level file name
 *  @param  level      [in]  log level
 *  @param  code       [in]  assert code 
 *  @param  text_p     [in]  assert text
 *  @return none
 *  @date   2014/03/28 FJT)Taniguchi create
 */
/********************************************************************************************************************/
void bpf_com_plog_ast(
	char*			lname,
	e_bpf_com_plog_level level ,
	unsigned int	 code ,
	void*			 text_p )
{
	t_bpf_com_plog_msg msg;
	int				  len;
	
	if((level&(0xff)) < bpf_com_plog_get_level(lname))
	{
		return;
	}
	msg.data.info.ast.code = code;
	len = snprintf(msg.data.info.ast.text, sizeof(msg.data.info.ast.text),"%s",(char*)text_p);
	len++;
	if(len > sizeof(msg.data.info.ast.text))
	{
		msg.data.info.ast.text[ sizeof(msg.data.info.ast.text) - 1] = '\0';
		len = sizeof(msg.data.info.ast.text);
	}
	msg.data.len = sizeof(msg.data.info.ast.code) + len;
	bpf_com_plog_send_to_lo(level,&msg,D_BPF_COM_PLOG_IPOFS);
	return;
}

/********************************************************************************************************************/
/**
 *  @brief  get pakect log for device log
 *  @note   Common function packet log library.
 *          This function is thread safe.
 *  @param  lname      [in]  log level file name
 *  @param  level      [in]  log level
 *  @param  addr       [in]  addr
 *  @param  data       [in]  data
 *  @param  opt1       [in]  opt1
 *  @param  opt2       [in]  opt2
 *  @return none
 *  @date   2014/03/28 FJT)Taniguchi create
 */
/********************************************************************************************************************/
void bpf_com_plog_device(
	char*			lname,
	e_bpf_com_plog_level level ,
	unsigned int     addr ,
	unsigned int     data ,
	unsigned int     opt1 ,
	unsigned int     opt2	
)
{
	t_bpf_com_plog_msg msg;
	if((level&(0xff)) < bpf_com_plog_get_level(lname))
	{
		return;
	}
	switch(level & 0x00000F00)
	{
		case E_REG_LV_OFS:
			msg.data.info.reg.addr = addr;
			msg.data.info.reg.data = data;
			msg.data.len = sizeof(t_log_reg);
			break;
		case E_SPI_LV_OFS:
			msg.data.info.spi.addr = addr;
			msg.data.info.spi.data = data;
			msg.data.info.spi.no   = opt1;
			msg.data.info.spi.ssno = opt2;
			msg.data.len = sizeof(t_log_spi);
			break;
		case E_I2C_LV_OFS:
			msg.data.info.i2c.addr = addr;
			msg.data.info.i2c.data = data;
			msg.data.info.i2c.no   = opt1;
			msg.data.info.i2c.ssno = opt2;
			msg.data.len = sizeof(t_log_i2c);
			break;
		case E_QSPI_LV_OFS:
			msg.data.info.qspi.addr = addr;
			msg.data.info.qspi.size = data;
			msg.data.info.qspi.no   = opt1;
			msg.data.info.qspi.face = opt2;
			msg.data.len = sizeof(t_log_qspi);
			break;
		default:
			return;
			break;
	}
	bpf_com_plog_send_to_lo(level,&msg,D_BPF_COM_PLOG_IPOFS);
	return;
}

/********************************************************************************************************************/
/**
 *  @brief  get pakect log for trace log
 *  @note   Common function packet log library.
 *          This function is thread safe.
 *  @param  lname      [in]  log level file name
 *  @param  level      [in]  log level
 *  @param  file       [in]  file
 *  @param  line       [in]  line
 *  @param  format     [in]  format ...
 *  @return none
 *  @date   2014/03/28 FJT)Taniguchi create
 */
/********************************************************************************************************************/
void bpf_com_plog_trace(
	char*			     lname,
	e_bpf_com_plog_level level ,
	const char*		 file,
	int				 line,
	const char*		 format, ...)
{
	t_bpf_com_plog_msg msg;
	va_list args;
	int textlen;
	const char*	out_p;

	if((level&(0xff)) < bpf_com_plog_get_level(lname))
	{
		return;
	}
	va_start(args, format);	/* pgr0039 */
	vsnprintf( msg.data.info.trace.text , sizeof(msg.data.info.trace.text) , format, args );
	va_end(args);
	textlen = strlen(msg.data.info.trace.text);
	textlen++;
	if(textlen >= sizeof(msg.data.info.trace.text))
	{
		msg.data.info.trace.text[sizeof(msg.data.info.trace.text) - 1] = '\0';
		textlen = sizeof(msg.data.info.trace.text);
	}
	if((out_p = strrchr(file, '/')) == NULL)
	{
		out_p = file;
	}
	else
	{
		out_p++;
	}
	snprintf(msg.data.info.trace.file, sizeof(msg.data.info.trace.file) ,"%s",out_p);
	msg.data.info.trace.line = line;
	msg.data.len = sizeof(msg.data.info.trace.file) + sizeof(msg.data.info.trace.line) + textlen;
	bpf_com_plog_send_to_lo(level,&msg,D_BPF_COM_PLOG_IPOFS);
	return;
}

/********************************************************************************************************************/
/**
 *  @brief  get pakect log for rsc log
 *  @note   Common function packet log library.
 *          This function is thread safe.
 *  @param  lname      [in]  log level file name
 *  @param  level      [in]  log level
 *  @param  id         [in]  id
 *  @param  opt1       [in]  opt1 
 *  @param  opt2       [in]  opt2 
 *  @param  opt3       [in]  opt2 
 *  @return none
 *  @date   2014/12/09 FJT)Taniguchi create
 */
/********************************************************************************************************************/
void bpf_com_plog_rsc(
	char*			     lname,
	e_bpf_com_plog_level level ,
	unsigned int		 id,
	unsigned int		 opt1,
	unsigned int		 opt2,
	unsigned int		 opt3
)
{
	t_bpf_com_plog_msg msg;

	if((level&(0xff)) < bpf_com_plog_get_level(lname))
	{
		return;
	}
	switch(level & 0x00000F00)
	{
		case 	E_SEM_LV_OFS:
			msg.data.info.sem.id = id;
			msg.data.len = sizeof(t_log_sem);
			break;
		case 	E_TIM_LV_OFS:
			msg.data.info.tim.id     = id;
			msg.data.info.tim.id_bpf = opt1;
			msg.data.info.tim.id_os  = opt2;
			msg.data.info.tim.value  = opt3;
			msg.data.len = sizeof(t_log_tim);
			break;
		case 	E_BUF_LV_OFS:
			msg.data.info.buf.id     = id;
			msg.data.info.buf.shmid = opt1;
			msg.data.info.buf.addr  = opt2;
			msg.data.info.buf.size  = opt3;
			msg.data.len = sizeof(t_log_buf);
			break;
		default:
			return;
			break;
	}
	bpf_com_plog_send_to_lo(level,&msg,D_BPF_COM_PLOG_IPOFS);
	return;
}


/********************************************************************************************************************/
/**
 *  @brief  get pakect log for rsc log 2
 *  @note   Common function packet log library.
 *          This function is thread safe.
 *  @param  lname      [in]  log level file name
 *  @param  level      [in]  log level
 *  @param  id         [in]  id
 *  @param  opt1       [in]  opt1 
 *  @param  opt2       [in]  opt2 
 *  @return none
 *  @date   2018/02/19 FJT)Taniguchi create
 */
/********************************************************************************************************************/
void bpf_com_plog_rsc2(
	char*			     lname,
	e_bpf_com_plog_level level ,
	unsigned long		 id,
	unsigned int		 opt1,
	unsigned int		 opt2
)
{
	t_bpf_com_plog_msg msg;

	if((level&(0xff)) < bpf_com_plog_get_level(lname))
	{
		return;
	}
	switch(level & 0x00000F00)
	{
		case E_TIM_LV_OFS:
			msg.data.info.tim2.id      = id;
			msg.data.info.tim2.latency = opt1;
			msg.data.info.tim2.period  = opt2;
			msg.data.len = sizeof(t_log_tim2);
			break;
		case E_SEM_LV_OFS:
			msg.data.info.sem.id      = id;
			msg.data.info.sem.val1    = opt1;
			msg.data.info.sem.val2    = opt2;
			msg.data.len = sizeof(t_log_sem);
			break;
		default:
			return;
			break;
	}
	bpf_com_plog_send_to_lo(level,&msg,D_BPF_COM_PLOG_IPOFS);
	return;
}



/********************************************************************************************************************/
/**
 *  @brief  get pakect log for thread interface log
 *  @note   Common function packet log library.
 *          This function is thread safe.
 *  @param  level      [in]  log level
 *  @param  len        [in]  length
 *  @param  data_p     [in]  data pointer
 *  @param  ipaddr_ofs [in]  ip address offset
 *  @return none
 *  @date   2014/03/28 FJT)Taniguchi create
 */
/********************************************************************************************************************/
void bpf_com_plog_com(
	char*			     lname,
	e_bpf_com_plog_level level ,
	int					 len,
	void*				 data_p,
	unsigned int		 ipaddr_ofs)
{
	t_bpf_com_plog_msg msg;
	if(data_p == NULL)
	{
		return;
	}	
	if((level&(0xff)) < bpf_com_plog_get_level(lname))
	{
		return;
	}
	if( (unsigned int)len > sizeof(t_log_com))
	{
		len = sizeof(t_log_com);
	}
	msg.data.len = len;
	memcpy(msg.data.info.com.data,data_p,len);
	bpf_com_plog_send_to_lo(level,&msg,ipaddr_ofs);
	return;
}


/********************************************************************************************************************/
/**
 *  @brief  get pakect log for trace log
 *  @note   Common function packet log library.
 *          This function is thread safe.
 *  @param  lname      [in]  log level file name
 *  @param  level      [in]  log level
 *  @param  file       [in]  file
 *  @param  line       [in]  line
 *  @param  format     [in]  format ...
 *  @return none
 *  @date   2014/03/28 FJT)Taniguchi create
 */
/********************************************************************************************************************/
void bpf_com_plog_trace_mp(
	int				 level ,
	const char*		 format, ...)
{
	char*			     lname;
	t_bpf_com_plog_msg msg;
	va_list args;
	int textlen;
	const char*	out_p;
	char*			 file;
	int				 line =0;
	e_bpf_com_plog_level	plevel;

	switch(level){
		case 3:
			plevel = E_TRC_LV_CRITICAL;
			break;
		case 5:
			plevel = E_TRC_LV_SERIOUS;
			break;
		case 6:
			plevel = E_TRC_LV_INFO;
			break;
		case 7:
			plevel = E_TRC_LV_DEBUG;
			break;
		default:
			plevel = E_TRC_LV_BUG;
			break;
	}

	switch(bpf_com_plog_tracekind){
		case D_RRH_PROCID_PF:
			lname = "/proc/rru/trc_pf";
			file = "DU_pf";
			plevel = M_BPF_COM_PLOG_LEVEL(plevel,E_TRC_NO_PF);
;			break;											
		case D_RRH_PROCID_PTP:								
			lname = "/proc/rru/trc_ptp";		
			file = "DU_ptp";
			plevel = M_BPF_COM_PLOG_LEVEL(plevel,E_TRC_NO_PTP);
			break;											
		case D_RRH_PROCID_INI:								
			lname = "/proc/rru/trc_init";		
			file = "DU_init";
			plevel = M_BPF_COM_PLOG_LEVEL(plevel,E_TRC_NO_INIT);
			break;											
		default:		
			lname = "/proc/rru/trc";
			file = "DU_other";
			break;											
	}

	if((plevel&(0xff)) < bpf_com_plog_get_level(lname))
	{
		return;
	}
	va_start(args, format);	/* pgr0039 */
	vsnprintf( msg.data.info.trace.text , sizeof(msg.data.info.trace.text) , format, args );
	va_end(args);
	textlen = strlen(msg.data.info.trace.text);
	textlen++;
	if(textlen >= sizeof(msg.data.info.trace.text))
	{
		msg.data.info.trace.text[sizeof(msg.data.info.trace.text) - 1] = '\0';
		textlen = sizeof(msg.data.info.trace.text);
	}
	if((out_p = strrchr(file, '/')) == NULL)
	{
		out_p = file;
	}
	else
	{
		out_p++;
	}
	snprintf(msg.data.info.trace.file, sizeof(msg.data.info.trace.file) ,"%s",out_p);
	msg.data.info.trace.line = line;
	msg.data.len = sizeof(msg.data.info.trace.file) + sizeof(msg.data.info.trace.line) + textlen;
	bpf_com_plog_send_to_lo(plevel,&msg,D_BPF_COM_PLOG_IPOFS);
	return;
}

/********************************************************************************************************************/
/**
 *  @brief  get pakect log for trace log
 *  @note   Common function packet log library.
 *          This function is thread safe.
 *  @param  plane_sd        [in]  socket no
 *  @param  dir             [in]  0:rx 1:tx
 *  @param  dp_header_p     [in]  header
 *  @param  dp_type         [in]  type
 *  @param  dp_length       [in]  length
 *  @param  dp_payload      [in]  payload
 *  @return result
 *  @date   2019/11/11 FJT)Taniguchi create
 */
/********************************************************************************************************************/
int bpf_com_plog_dp_to_eth0(
	int				plane_sd,
	int				dir,
	void*			dp_header_p,
	int				dp_type,
	int				dp_length,
	char*			dp_payload
)
{
	int						ret;
	char					eth_frame[1514];
	size_t					plane_pos;
	int						value;
	struct ether_header*	header;
	struct ether_header*	header_In;

	header_In = (struct ether_header*)dp_header_p;
	header    = (struct ether_header*)eth_frame;
	if(dir == BPF_COM_DPLOG_DIR_TX){
		memcpy(header->ether_dhost,header_In->ether_dhost,6);
		memcpy(header->ether_shost,header_In->ether_shost,6);
	}else{
		memcpy(header->ether_dhost,header_In->ether_shost,6);
		memcpy(header->ether_shost,header_In->ether_dhost,6);
	}
	header->ether_type = header_In->ether_type;

	plane_pos = sizeof(*header);
	value = htonl(dp_type);
	memcpy(&eth_frame[plane_pos], &value, sizeof(value));
	plane_pos += sizeof(value);

	if( dp_length > ( sizeof(eth_frame) - ( sizeof(*header) + sizeof(dp_type) + sizeof(dp_length))) ){
		dp_length = sizeof(eth_frame) - ( sizeof(*header) + sizeof(dp_type) + sizeof(dp_length));
	}

	value = htonl(dp_length);
	memcpy(&eth_frame[plane_pos], &value, sizeof(value));
	plane_pos += sizeof(value);

	memcpy(&eth_frame[plane_pos], dp_payload, dp_length );
	plane_pos += dp_length;
	if(( ret = send(plane_sd, eth_frame, plane_pos, 0)) < 0){
		return ret;
	}
	return 0;
}

/********************************************************************************************************************/
/**
 *  @brief  get pakect log for trace log
 *  @note   Common function packet log library.
 *          This function is thread safe.
 *  @param  ether_type      [in]  ether header type
 *  @param  src_macaddr     [in]  src mac address
 *  @param  dst_macaddr     [in]  dst mac address
 *  @param  dp_header_p     [in]  header
 *  @return int socket no
 *  @date   2019/11/11 FJT)Taniguchi create
 */
/********************************************************************************************************************/
int bpf_com_plog_dp_init(
	unsigned short	ether_type,
	unsigned char*	src_macaddr,
	unsigned char*	dst_macaddr,
	void*			dp_header_p
)
{
	int						plane_sd;
	struct ifreq			ifrq;
	struct sockaddr_ll		sd_bif;
	int						ret;
	struct ether_header*	header;

	if((plane_sd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0){
		return -1;
	}

	memset(&ifrq, 0, sizeof(ifrq));
	snprintf(ifrq.ifr_name,sizeof(ifrq.ifr_name),"eth0");
	if((ret = ioctl(plane_sd, SIOCGIFINDEX, &ifrq)) < 0){
		close(plane_sd);
		return -2;
	}

	memset(&sd_bif, 0, sizeof(sd_bif));
	sd_bif.sll_family = AF_PACKET;
	sd_bif.sll_ifindex = ifrq.ifr_ifindex;
	sd_bif.sll_protocol = htons(ETH_P_ALL);
	sd_bif.sll_halen = ETHER_ADDR_LEN;
	if ((ret = bind(plane_sd, (const struct sockaddr *)&sd_bif, sizeof(sd_bif))) < 0) {
		close(plane_sd);
		return -3;
	}

	header = (struct ether_header*)dp_header_p;
	memcpy(header->ether_dhost,dst_macaddr,6);
	memcpy(header->ether_shost,src_macaddr,6);
	header->ether_type = htons(ether_type);

	return plane_sd;
}
/* @} */
