/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   f_mpsw_msi.c
 *  @brief  MSI function library
 *  @date   2018/11/19 FJT)Taniguchi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2018
 */
/********************************************************************************************************************/
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#define _GNU_SOURCE					/* pgr0333 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sched.h>
#include "f_mpsw_msi.h"
#include "f_mpsw_srv_types.h"
#include "f_mpsw_msg_subscribe_def.h"
#include "f_mpsw_msg_ready_def.h"

/** @addtogroup MPSW_MSI
 * @{ */
 
#define MPSW_MSI_SUBSCRIBE_WAITTIME	3000	/* 3sec */

typedef union {
	T_MSG_MSI_SUBSCRIBE_REJ	rej;
	T_MSG_MSI_SUBSCRIBE_CFM	cfm;
} T_MPSW_MSG_MSI_FUNCIF;


static void (*msi_l_logfunc)(int, const char* , ...);
static void (*msi_l_sndLogFunc)(int, char*);
static void (*msi_l_rcvlogFunc)(int, const char* format, ...);

#define FUNC_DEBUG(...)	if(msi_l_logfunc){ msi_l_logfunc(E_MPSW_SRV_LOG_DEBUG,__VA_ARGS__); } 
#define FUNC_ABN(...)	if(msi_l_logfunc){ msi_l_logfunc(E_MPSW_SRV_LOG_ABN,__VA_ARGS__); }
#define FUNC_INFO(...)	if(msi_l_logfunc){ msi_l_logfunc(E_MPSW_SRV_LOG_INFO,__VA_ARGS__); }
#define FUNC_ERROR(...)	if(msi_l_logfunc){ msi_l_logfunc(E_MPSW_SRV_LOG_CRITICAL,__VA_ARGS__); }
#define FUNC_BUG(...)	if(msi_l_logfunc){ msi_l_logfunc(E_MPSW_SRV_LOG_BUG,__VA_ARGS__); }

static const char* msi_l_loglevel[] = 
{
	"UNKNOWN",
	"BUG",
	"UNKNOWN2",
	"CRITICAL",
	"UNKNOWN3",
	"ABN",
	"INFO",
	"DEBUG"
};

/********************************************************************************************************************/
/*!
 *  @brief  msi default log function(internal)
 *  @note   msi default log function
 *  @param  loglevel            [in]  loglevel
 *  @param  format              [in]  format
 *  @param  ...                 [in]  ...
 *  @return N/A
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static void msi_l_default_log_func( INT loglevel,const char* format, ...)
{
	char	text_str[256];

	va_list args;
	va_start(args, format);	/* pgr0039 */
	vsnprintf(text_str,sizeof(text_str),format,args);
	vprintf(format,args);
	va_end(args);

	fprintf(stdout,"[%s]%s",msi_l_loglevel[loglevel & 0x7],text_str);

	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  msi application initialize function(internal)
 *  @note   msi application initialize
 *  @param  app_pList            [in]  application start list
 *  @return process id
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static pid_t msi_l_app_initialize( T_MSI_APP_LIST* app_pList )
{
	pid_t app_pid;
	struct sched_param s_param;
	INT	rtn;

	app_pid = fork();
	if(app_pid == 0){
		if(( rtn = execvp(app_pList->startPrg, app_pList->startOpt)) == -1){
			FUNC_ERROR("execvp %s error(%s) \n",app_pList->startPrg,strerror(errno));
		}
		_exit(EXIT_SUCCESS);

	} else if(app_pid > 0){
		s_param.sched_priority = app_pList->priority;
		if(( rtn = sched_setscheduler(app_pid, app_pList->policy,&s_param)) != 0){
			FUNC_ABN("sched_setscheduler policy(0x%x) and priority(%d) error(%s) \n",app_pList->policy,app_pList->priority,strerror(errno));
		}

	} else{
		FUNC_ERROR("fork error(%s) \n",strerror(errno));
	}
	return app_pid;
}

/********************************************************************************************************************/
/*!
 *  @brief  msi udp socket initialize function(internal)
 *  @note   msi default log function
 *  @param  portNo            [in]  portNo
 *  @param  ipAddr            [in]  IP Address
 *  @return N/A
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static INT msi_l_udp_socket_initialize( in_port_t portNo, in_addr_t ipAddr )
{

	INT					msiHandle;
	INT					onoff = 1;
	struct sockaddr_in	ssaddr;
	INT					rtn;

	if((msiHandle = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		FUNC_DEBUG("socket error(%s) portNo:%d Ipaddr:%s\n",strerror(errno),portNo,ipAddr);
		return -1;
	}
	setsockopt( msiHandle, SOL_SOCKET, SO_REUSEADDR, (char *)&onoff, sizeof(onoff) );
	if((portNo != 0) || (ipAddr != 0)){
		memset(&ssaddr,0,sizeof(ssaddr));
		ssaddr.sin_family      = AF_INET;
		ssaddr.sin_port        = portNo;
		ssaddr.sin_addr.s_addr = ipAddr;
		if(( rtn = bind(msiHandle, (struct sockaddr *)&ssaddr, sizeof(ssaddr))) != 0){
			FUNC_ERROR("bind error(%s) portNo:%d Ipaddr:0x%x\n",strerror(errno),portNo,ipAddr);
			close(msiHandle);
			return -1;
		}
	}
	return msiHandle;
}

/********************************************************************************************************************/
/*!
 *  @brief  msi udp socket initialize function(internal)
 *  @note   msi default log function
 *  @param  portNo            [in]  portNo
 *  @param  ipAddr            [in]  IP Address
 *  @return N/A
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static INT msi_l_udp_send(
	INT msiHandle ,
	VOID *sbuf,
	ssize_t slen,
	in_port_t ipPortNo,
	in_addr_t ipAddr
)
{
	struct sockaddr_in	ssaddr;
	INT					hlen;
	ssize_t				rlen;
	
	hlen= sizeof(ssaddr);
	ssaddr.sin_family      = AF_INET;
	ssaddr.sin_port        = ipPortNo;
	ssaddr.sin_addr.s_addr = ipAddr;

	FUNC_DEBUG("sendto start(%d 0x%x %dbytes) \n",ntohs(ipPortNo),ntohl(ipAddr),slen);

	if(( rlen = sendto(msiHandle,sbuf, slen , 0, (const struct sockaddr *)&ssaddr,hlen)) <= 0){
		FUNC_ERROR("sendto error(%s) \n",strerror(errno));
		return MSI_FUNCIF_NG;
	}
	
	if(slen != rlen){
		FUNC_ABN("sendto abnormal(s:%d e:%d) \n",slen,rlen);
		return MSI_FUNCIF_NG;
	}
	
	if(msi_l_sndLogFunc != NULL){
		msi_l_sndLogFunc(slen,sbuf);
	}
	
	return MSI_FUNCIF_OK;
}

/********************************************************************************************************************/
/*!
 *  @brief  msi udp socket initialize function(internal)
 *  @note   msi default log function
 *  @param  portNo            [in]  portNo
 *  @param  ipAddr            [in]  IP Address
 *  @return N/A
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static ssize_t msi_l_udp_receive_timed(
	INT msiHandle ,
	VOID *buf,
	INT len,
	INT wait_time,
	in_addr_t* ipAddr,
	in_port_t* ipPortNo,
	UINT* messageId
)
{
	fd_set				fds;
	int					maxfd;
	ssize_t				rlen;
	struct sockaddr_in	rsaddr;
	int					hlen;
	int					rtn;
	struct timeval		timeout;
	INT 				wait_time_del;

	wait_time_del = wait_time;

	do {
		FD_ZERO(&fds);
		FD_SET(msiHandle, &fds);
		maxfd = msiHandle + 1;

		memset(&timeout,0,sizeof(timeout));
		if(wait_time == 0){
			rtn = select(maxfd, &fds, NULL, NULL, NULL);
		}else{
			timeout.tv_sec = wait_time_del / 1000;
			timeout.tv_usec = (wait_time_del % 1000)*1000;	
			rtn = select(maxfd, &fds, NULL, NULL, &timeout);
		}
		switch(rtn){
			case 0:
				FUNC_ERROR("select timeout(%s)\n",strerror(errno));
				rtn = -1;
				break;
			case -1:
				rtn = -1;
				if(EINTR == errno){
					FUNC_DEBUG("select signal intr(%s)\n",strerror(errno));
					rtn = -2;
					if(wait_time > 0){
						wait_time_del = (timeout.tv_sec * 1000) + ( timeout.tv_usec / 1000 );
						if(( wait_time > wait_time_del ) && ( wait_time_del > 0 )){
							FUNC_ABN("select continue(%d)\n",wait_time_del);
						}else{
							rtn = -1;
						}
					}
				}else{
					FUNC_ERROR("select error(%s)\n",strerror(errno));
				}
				break;
			default:
				if (FD_ISSET(msiHandle, &fds)) {
					FUNC_DEBUG("recvfrom start \n");
					hlen= sizeof(rsaddr);
					if((rlen = recvfrom(msiHandle, buf, len , 0 , (struct sockaddr *)&rsaddr , (socklen_t *)&hlen )) <= 0){
						FUNC_ERROR("recvfrom error(%s)\n",strerror(errno));
						rtn = -1;
					}else{
						if(rlen >= (ssize_t)sizeof(T_MPSW_SRV_MSGHEAD)){
							*messageId = ((T_MPSW_SRV_MSGHEAD*)buf)->msgId;
						}else{
							FUNC_ERROR("recvfrom length(%d)\n",rlen);
						}
						*ipAddr = rsaddr.sin_addr.s_addr;
						*ipPortNo = rsaddr.sin_port;
						rtn = rlen;

						if(msi_l_rcvlogFunc != NULL){
							msi_l_rcvlogFunc(len,buf);
						}
						FUNC_DEBUG("recvfrom OK \n");
					}
				}else{
					rtn = -1;
				}
				break;
		}
		if(rtn == -2){	/* EINTR */
			continue;
		}else{
			break;
		}
	}while(1);
	return rtn;
}


/********************************************************************************************************************/
/*!
 *  @brief  MSI application initialize
 *  @note   NSA and SAA process initialize
 *  @param  NpList              [in]  NSA program list
 *  @param  SpList              [in]  SAA program list
 *  @param  readyIndWaitTime    [in]  MSI_READY_IND wait time(msec)
 *  @param  logFunc             [in]  Log function pointer
 *  @param  NPid                [out] NSA Process ID
 *  @param  SPid                [out] SAA Process ID
 *  @return result code
 *  @retval MSI_FUNCIF_OK : result OK
 *  @retval MSI_FUNCIF_NG : result NG
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
INT msi_app_initialize(
	T_MSI_APP_LIST*	NpList,
	T_MSI_APP_LIST*	SpList,
	INT				readyIndWaitTime,
	VOID*			logFunc,
	pid_t*			NPid,
	pid_t*			SPid
){

	INT						msiHandle = -1;
	pid_t					app_pid = -1;
	T_MPSW_SRV_MSGHEAD		msg;
	INT						rtn = MSI_FUNCIF_NG;
	in_addr_t				ipAddr;
	in_port_t				ipPortNo;
	UINT					messageId = 0;
	ssize_t					rlen;
	INT						nsaOnly = D_MPSW_OFF;
	union sigval			value;
	cpu_set_t				cpu_mask;

	if(logFunc != NULL){
		msi_l_logfunc = logFunc;
	}
	else{
		msi_l_logfunc = msi_l_default_log_func;
	}

	if(
		((NpList != NULL) && (NPid == NULL)) ||
		((SpList != NULL) && (SPid == NULL))
	){
		FUNC_ERROR("Input Param is mismatch Np:0x%lx,0x%lx Sp:0x%lx,0x%lx\n",
			(ULONG)NpList,(ULONG)NPid,(ULONG)SpList,(ULONG)SPid);
		return rtn;
	}

	if(SpList != NULL){
		FUNC_INFO("Starting M-planeSW Server (SAA and NSA)... \n");
		ipAddr = inet_addr(D_MPSW_SRV_IPADDR);
		ipPortNo = htons(MSI_PORT_NO_READY_IND);

		if(( msiHandle = msi_l_udp_socket_initialize(ipPortNo,ipAddr)) == -1){
			FUNC_ERROR("Internal initialize error\n");
			return rtn;
		}

		*SPid = -1;
		if((app_pid = msi_l_app_initialize(SpList)) <= 0){
			FUNC_ERROR("SAA initialize error\n");
		}
		else{
			FUNC_INFO("SAA(pid:%d) executed OK\n",app_pid);
			*SPid = app_pid;
			if(NpList != NULL){
				*NPid = -1;
				if((app_pid = msi_l_app_initialize(NpList)) <= 0){
					FUNC_ERROR("NSA initialize error and kill SAA\n");
					value.sival_int = 0;
					sigqueue(*SPid,SIGTERM,value );	/* SAA kill */
				}
				else{
					FUNC_INFO("NSA(pid:%d) executed OK\n",app_pid);
					rtn = MSI_FUNCIF_OK;
					*NPid = app_pid;
				}
			}else{
				rtn = MSI_FUNCIF_OK;
			}
		}
	}else{
		/* NSA’P“Æƒ‚[ƒh */
		if(NpList != NULL){
			FUNC_INFO("Starting M-planeSW Server (NSA only)... \n");
			*NPid = -1;
			if((app_pid = msi_l_app_initialize(NpList)) <= 0){
				FUNC_ERROR("NSA initialize error\n");
			}
			else{
				FUNC_INFO("NSA(pid:%d) executed OK\n",app_pid);
				nsaOnly = D_MPSW_ON;
				rtn = MSI_FUNCIF_OK;
				*NPid = app_pid;
			}
		}
	}

	if(rtn == MSI_FUNCIF_OK){
		if(nsaOnly == D_MPSW_OFF){
			FUNC_INFO("M-planeSW Server Initialize OK and wait for MSI_READY_IND ... \n");
			rtn = MSI_FUNCIF_NG;
			if((rlen = msi_l_udp_receive_timed(msiHandle,&msg,sizeof(msg),readyIndWaitTime,&ipAddr,&ipPortNo,&messageId)) <= 0 ){
				FUNC_ERROR("M-planeSW Server Ready IND NG \n");
			}else{
				if(messageId == D_MSG_MSI_READY_IND){
					FUNC_INFO("MSI_READY_IND was received from 0x%x,0x%x\n",ntohl(ipAddr),ntohs(ipPortNo));
					rtn = MSI_FUNCIF_OK;
				}else{
					FUNC_ERROR("Unknown message was received (0x%08x 0x%x) \n",messageId,(INT)rlen);
				}
			}
			close(msiHandle);
			
			if(rtn == MSI_FUNCIF_OK){
				__CPU_ZERO_S(sizeof(cpu_mask),&cpu_mask);
				__CPU_SET_S(SpList->cpus,sizeof(cpu_mask),&cpu_mask);
				if((rtn = sched_setaffinity(*SPid, sizeof(cpu_mask), &cpu_mask)) != 0){
					FUNC_ABN("sched_setaffinity cpus:0x%x error(%s)\n",SpList->cpus,strerror(errno));	/* Œ‹‰Ê‚Í–³Ž‹ */
				}
			}
		}

		if(NpList != NULL){
			__CPU_ZERO_S(sizeof(cpu_mask),&cpu_mask);
			__CPU_SET_S(NpList->cpus,sizeof(cpu_mask),&cpu_mask);
			if((rtn = sched_setaffinity(*NPid, sizeof(cpu_mask), &cpu_mask)) != 0){
				FUNC_ABN("sched_setaffinity cpus:0x%x error(%s)\n",NpList->cpus,strerror(errno));	/* Œ‹‰Ê‚Í–³Ž‹ */
			}
		}
		rtn = MSI_FUNCIF_OK;

	}
	return rtn;
}


/********************************************************************************************************************/
/*!
 *  @brief  MSI subscriber entry funciton
 *  @note   MSI subscriber entry
 *  @param  portNo              [in]  MSI Port Number
 *  @param  serverIpAdder       [in]  Server Address
 *  @param  modNum              [in]  Entry module Number
 *  @param  modInfo             [in]  Entry module Information
 *  @param  logFunc             [in]  Log function pointer
 *  @param  sndLogFunc          [in]  Send log function pointer
 *  @param  rcvlogFunc          [in]  Receive Log function pointer
 *  @param  msiHandle           [out]  MSI handle
 *  @param  subscribe ID        [out]  MSI subscriberId
 *  @return result code
 *  @retval MSI_FUNCIF_OK : subscribe result OK
 *  @retval MSI_FUNCIF_NG : subscribe result NG
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
INT msi_subscribe(
	in_port_t					portNo,
	CHAR*						serverIpAdder,
	UINT						modNum,
	T_MSI_SUBSCRIBE_MOD_INFO**	modInfo,
	VOID*						logFunc,
	VOID*						sndLogFunc,
	VOID*						rcvlogFunc,
	T_MSI_HANDLE*				msiHandle	)
{
	INT							handle;
	UINT						messageId = 0;
	INT							rtn = MSI_FUNCIF_NG;
	T_MSG_MSI_SUBSCRIBE_REQ		req;
	INT							req_size;
	T_MPSW_MSG_MSI_FUNCIF		msg;
	
	in_addr_t					ipAddr;
	in_port_t					ipPortNo;
	ssize_t						rlen;
	UINT						count;
	T_MSI_SUBSCRIBE_MOD_INFO*	modInfo_p;
	
	if(logFunc != NULL){
		msi_l_logfunc = logFunc;
	}
	else{
		msi_l_logfunc = msi_l_default_log_func;
	}

	if(sndLogFunc != NULL){
		msi_l_sndLogFunc = sndLogFunc;
	}

	if(rcvlogFunc != NULL){
		msi_l_rcvlogFunc = rcvlogFunc;
	}
	
	if(serverIpAdder == NULL){
		ipAddr = inet_addr(D_MPSW_SRV_IPADDR);
	}else{
		ipAddr = inet_addr(serverIpAdder);
	}
	ipPortNo = htons(portNo);
	
	if(modNum > (E_MSI_MSGOPE_ALL + 1)){
		FUNC_ERROR("subscribe module number range error %d \n",modNum);
		return rtn;
	}

	if(( handle = msi_l_udp_socket_initialize(ipPortNo,ipAddr)) == -1){
		FUNC_ERROR("subscribe socket NG \n");
		msi_l_logfunc = NULL;
		msi_l_sndLogFunc = NULL;
		msi_l_rcvlogFunc = NULL;
		return rtn;
	}
	
	memset(&req,0,sizeof(req));
	req.msgHead.msgId = D_MSG_MSI_SUBSCRIBE_REQ;
	req.msgHead.senderId = getpid();
	req.data.moduleNum = (INT)modNum;
	req.msgHead.msgLength = sizeof(req.data.moduleNum);
	modInfo_p = *modInfo;
	for(count=0;count < modNum ; count++,modInfo_p++){
		memcpy((void*)(&req.data.msi_module_entry[count]),
				(void*)(modInfo_p),
					sizeof(T_MPSW_MSI_MODULE_ENTRY));		
		req.msgHead.msgLength += sizeof(T_MPSW_MSI_MODULE_ENTRY);
	}
	req_size = sizeof(req.msgHead) + req.msgHead.msgLength;
	
	FUNC_INFO("MSI_SUBSCRIBE_REQ sent(p:%d,ip:0x%x) ... \n",MSI_PORT_NO,ntohl(ipAddr));
	if( ( rtn = msi_l_udp_send(handle, &req, req_size, htons(MSI_PORT_NO) ,ipAddr )) == MSI_FUNCIF_OK ){
		rtn = MSI_FUNCIF_NG;

		if((rlen = msi_l_udp_receive_timed(handle,&msg,sizeof(msg),MPSW_MSI_SUBSCRIBE_WAITTIME,&ipAddr,&ipPortNo,&messageId)) <= (ssize_t)sizeof(messageId) ){
			FUNC_ERROR("receive NG \n");
		}
		else{
			switch( messageId ){
				case D_MSG_MSI_SUBSCRIBE_CFM:
					FUNC_INFO("MSI_SUBSCRIBE_CFM received\n");
					msiHandle->socketFd = handle;
					msiHandle->subscriberId = msg.cfm.data.subscriberId;
					msiHandle->receiverId = msg.cfm.msgHead.senderId;
					msiHandle->senderId = req.msgHead.senderId ;
					msiHandle->addr = ipAddr;
					msiHandle->port = ipPortNo;
					rtn = MSI_FUNCIF_OK;
					break;
				case D_MSG_MSI_SUBSCRIBE_REJ:
					FUNC_ERROR("MSI_SUBSCRIBE_REJ received\n");
					break;
				default:
					FUNC_ERROR("ABN message received(0x%08x)\n",messageId);
					break;
			}
		}
	}
	if(rtn != MSI_FUNCIF_OK){
		close(handle);
	}
	return rtn;
}

/********************************************************************************************************************/
/*!
 *  @brief  MSI subscriber delete funciton
 *  @note   MSI subscriber delete
 *  @param  msiHandle           [out]  MSI handle
 *  @return result code
 *  @retval MSI_FUNCIF_OK : subscribe result OK
 *  @retval MSI_FUNCIF_NG : subscribe result NG
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
INT msi_desubscribe(T_MSI_HANDLE* msiHandle)
{
	T_MSG_MSI_DESUBSCRIBE_IND	ind;
	INT							rtn = MSI_FUNCIF_NG;

	memset(&ind,0,sizeof(ind));

	ind.msgHead.msgId = D_MSG_MSI_DESUBSCRIBE_IND;
	ind.msgHead.subscriberId = msiHandle->subscriberId;
	ind.msgHead.senderId = msiHandle->senderId;

	rtn = msi_l_udp_send(msiHandle->socketFd , &ind, sizeof(ind), msiHandle->port ,msiHandle->addr);
	FUNC_INFO("MSI_DESUBSCRIBE_IND sent %d\n",rtn);
	close(msiHandle->socketFd);
	return rtn;	
}

/********************************************************************************************************************/
/*!
 *  @brief  MSI subscriber delete funciton
 *  @note   MSI subscriber delete
 *  @param  msiHandle           [out]  MSI handle
 *  @return result code
 *  @retval MSI_FUNCIF_OK : subscribe result OK
 *  @retval MSI_FUNCIF_NG : subscribe result NG
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
INT msi_send_to_agent(T_MSI_HANDLE* msiHandle ,INT msiLen, VOID* msiMsg)
{
	T_MPSW_SRV_MSGHEAD*		msiheadr;
	
	msiheadr = (T_MPSW_SRV_MSGHEAD*)msiMsg;
	msiheadr->senderId = msiHandle->senderId;
	msiheadr->subscriberId = msiHandle->subscriberId;
	return msi_l_udp_send(msiHandle->socketFd, msiMsg, msiLen, msiHandle->port ,msiHandle->addr );	
}


/********************************************************************************************************************/
/*!
 *  @brief  MSI subscriber delete funciton
 *  @note   MSI subscriber delete
 *  @param  msiHandle           [out]  MSI handle
 *  @return result code
 *  @retval MSI_FUNCIF_OK : subscribe result OK
 *  @retval MSI_FUNCIF_NG : subscribe result NG
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
INT msi_receive_from_agent(T_MSI_HANDLE* msiHandle, INT msiMaxLen ,INT* msiLen,VOID* msiMsg)
{
	INT							rtn = MSI_FUNCIF_NG;
	in_addr_t					ipAddr;
	in_port_t					ipPortNo;
	ssize_t						rlen;
	UINT						messageId = 0;

	if((rlen = msi_l_udp_receive_timed(msiHandle->socketFd,msiMsg,msiMaxLen,0,&ipAddr,&ipPortNo,&messageId)) >= (ssize_t)sizeof(T_MPSW_SRV_MSGHEAD) ){
		FUNC_DEBUG("MSGID(0x%08x) received len:%ld addr:0x%08x port:%d \n",messageId,rlen,ntohl(ipAddr),ntohs(ipPortNo));
		*msiLen = (INT)rlen;
		rtn = MSI_FUNCIF_OK;
	}
	return rtn;	
}

/********************************************************************************************************************/
/*!
 *  @brief  MSI receive funciton for timewait
 *  @note   MSI subscriber delete
 *  @param  msiHandle           [out]  MSI handle
 *  @return result code
 *  @retval MSI_FUNCIF_OK : subscribe result OK
 *  @retval MSI_FUNCIF_NG : subscribe result NG
 *  @date   2018/11/23 FJT)Taniguchi create
 */
/********************************************************************************************************************/
INT msi_receive_timed_from_agent(T_MSI_HANDLE* msiHandle,INT msiMaxLen ,INT  msiWaitTime ,INT*  msiLen,VOID* msiMsg)
{
	INT							rtn = MSI_FUNCIF_NG;
	in_addr_t					ipAddr;
	in_port_t					ipPortNo;
	ssize_t						rlen;
	UINT						messageId = 0;

	if((rlen = msi_l_udp_receive_timed(msiHandle->socketFd,msiMsg,msiMaxLen,msiWaitTime,&ipAddr,&ipPortNo,&messageId)) >= (ssize_t)sizeof(T_MPSW_SRV_MSGHEAD) ){
		FUNC_DEBUG("MSGID(0x%08x) received len:%ld addr:0x%08x port:%d \n",messageId,rlen,ipAddr,ipPortNo);
		*msiLen = (INT)rlen;
		rtn = MSI_FUNCIF_OK;
	}
	return rtn;	
}


/* @} */
