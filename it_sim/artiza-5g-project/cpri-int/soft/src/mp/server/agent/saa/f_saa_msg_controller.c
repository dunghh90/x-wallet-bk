/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   f_saa_msg_controller.c
 *  @brief  Message controller for Sub-Agent 
 *  @date   2018/11/19 FJT)Taniguchi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2018
 */
/********************************************************************************************************************/
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
/* standard header */
#define _GNU_SOURCE			/* pgr0333 */
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

/* fujitsu header */
#include "f_saa_msg_controller.h"
#include "f_saa_msg_buffer.h"
#include "f_mpsw_msg_subscribe_def.h"
#include "f_mpsw_msg_subscribe_typ.h"
#include "f_mpsw_msg_ready_def.h"

/* YumaSDKpro header */
#include "ncx.h"

/** @addtogroup SAA_MSG_CONTROLLER
 * @{ */

#define D_SAA_MSG_BUFF_SIZE			0x10000				/*!< 最大送受信用バッファサイズ:64Kbyte */
#define D_SAA_MSG_SUBSCRIBER_MAX	32					/*!< 最大同時接続数(32) */
#define D_SAA_MSG_MOD_MAX			0x100				/*!< Module数 */

#define SAA_MSG_LOG_ERR(...)				log_dev0( __VA_ARGS__ )
#define SAA_MSG_LOG_WARN(...)				log_warn( __VA_ARGS__ )
#define SAA_MSG_LOG_INFO(...)				log_info( __VA_ARGS__ )
#define SAA_MSG_LOG_DEBUG(...)				log_dev1( __VA_ARGS__ )
#define SAA_MSG_MNG_LOCK(locks) \
do { \
	int	lock_result; \
	if((lock_result = pthread_mutex_lock(locks)) != D_MPSW_OK){ \
		SAA_MSG_LOG_ERR("\n%s Lock error(%d)",__func__,lock_result); \
		return -1; \
	} \
}while(0)

#define SAA_MSG_MNG_UNLOCK(locks)	pthread_mutex_unlock(locks)
#define SAA_MSG_MSGIDTOMOD(msgids)	(UINT)(( msgids >> 16) & 0xFF)
#define SAA_MSG_MSGIDTOOPE(msgids)	(UINT)(( msgids >> 8) & 0xFF)
#define SAA_MSG_MSGIDTOTYPE(msgids)	(UINT)( msgids & 0xF )

#define SAA_MSG_MOD(mods)			(UINT)(mods & 0xFF)

typedef struct {
	INT								initflg;
	INT								socFd;
	in_addr_t						addr;
	in_port_t						port;
}T_SAA_MSG_CTRL_INFO;

typedef struct {
	pid_t							pid;
	in_addr_t						addr;
	in_port_t						port;
}T_SAA_MSG_CTRL_ADDR;

typedef struct {
	T_SAA_MSG_CTRL_ADDR				senderInfo;
	T_MPSW_MSI_SUBSCRIBE_REQ		subReqInfo;
}T_SAA_MSG_CTRL_SUBSCRIBER_INFO;

typedef struct {
	T_SAA_MSG_CTRL_INFO				msgCtrl;
	UINT							entrySubscriberUseMap;
	UINT							entryModInfoMap[D_SAA_MSG_MOD_MAX];
	T_SAA_MSG_CTRL_SUBSCRIBER_INFO	info[D_SAA_MSG_SUBSCRIBER_MAX];
}T_SAA_MSG_CTRL_SUBSCRIBER_MNG;

static CHAR f_saaw_msg_sender_buff[D_SAA_MSG_BUFF_SIZE];
static CHAR f_saaw_msg_receiver_buff[D_SAA_MSG_BUFF_SIZE];
static T_SAA_MSG_CTRL_SUBSCRIBER_MNG f_saaw_msg_ctrl_mng;
static pthread_mutex_t f_saaw_msg_ctrl_mutex = PTHREAD_MUTEX_INITIALIZER;


/********************************************************************************************************************/
/*!
 *  @brief  message subscriber init function
 *  @note   init subscriber manage table
 *  @param  N/A
 *  @return result
 *  @retval D_MPSW_OK	: OK
 *  @retval -1			: NG
 *  @date   2018/11/20 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static INT f_saa_msg_l_chkInitMsgCtrl( VOID )
{
	INT	okng;
	while(1){
		okng = D_MPSW_NG;
		SAA_MSG_MNG_LOCK(&f_saaw_msg_ctrl_mutex);
		if(f_saaw_msg_ctrl_mng.msgCtrl.initflg == D_MPSW_ON){
			okng = D_MPSW_OK;
		}
		SAA_MSG_MNG_UNLOCK(&f_saaw_msg_ctrl_mutex);	
		if(okng == D_MPSW_OK){
			break;
		}
		usleep(1000);
	}
	return D_MPSW_OK;
}


/********************************************************************************************************************/
/*!
 *  @brief  message subscriber init function
 *  @note   init subscriber manage table
 *  @param  N/A
 *  @return result
 *  @retval D_MPSW_OK	: OK
 *  @retval -1			: NG
 *  @date   2018/11/20 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static INT f_saa_msg_l_initSubscriber( VOID )
{
	SAA_MSG_MNG_LOCK(&f_saaw_msg_ctrl_mutex);

	memset(&f_saaw_msg_ctrl_mng,0,sizeof(f_saaw_msg_ctrl_mng));
	f_saaw_msg_ctrl_mng.msgCtrl.socFd = -1;

	SAA_MSG_MNG_UNLOCK(&f_saaw_msg_ctrl_mutex);	
	return D_MPSW_OK;
}

/********************************************************************************************************************/
/*!
 *  @brief  message subscriber add function
 *  @note   add subscriber
 *  @param  pid 		[in]	process ID
 *  @param  port		[in]	suscriber portno
 *  @param  addr		[in]	suscriber ipaddress
 *  @param  req 		[in]	subscriber information
 *  @retval D_MPSW_OK	: OK
 *  @retval -1			: NG
 *  @date   2018/11/20 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static INT f_saa_msg_l_addSubscriber(pid_t pid,in_port_t port ,in_addr_t addr ,T_MPSW_MSI_SUBSCRIBE_REQ* req)
{
	T_SAA_MSG_CTRL_SUBSCRIBER_MNG* mng;
	T_SAA_MSG_CTRL_SUBSCRIBER_INFO* info = NULL;
	INT			count;
	INT			subscriberID = 0;
	INT			result = -1;


	SAA_MSG_MNG_LOCK(&f_saaw_msg_ctrl_mutex);

	mng = &f_saaw_msg_ctrl_mng;
	
	if(mng->entrySubscriberUseMap != 0){
		for(count=0 ; count < D_SAA_MSG_SUBSCRIBER_MAX ; count++){	
			if((mng->entrySubscriberUseMap & (D_MPSW_ON << count )) != 0 ){
				if( mng->info[count].senderInfo.pid != pid ){
					continue;
				}
			}
			info = &(mng->info[count]);
			subscriberID = (INT)count;
			result = D_MPSW_OK;
			break;
		}
	}else{
		info = &(mng->info[0]);
		subscriberID = 0;
		result = D_MPSW_OK;
	}

	if(result == D_MPSW_OK){
		info->senderInfo.pid  = pid;		/* pgr0060 */
		info->senderInfo.addr = addr;
		info->senderInfo.port = port;
		if(req != NULL){
			info->subReqInfo.moduleNum = req->moduleNum;
			for(count=0 ; count < (info->subReqInfo.moduleNum) ; count++){	

				memcpy(&(info->subReqInfo.msi_module_entry[count]),
					   &(req->msi_module_entry[count]),
					   sizeof(info->subReqInfo.msi_module_entry[count]));
				
				mng->entryModInfoMap[ SAA_MSG_MOD(info->subReqInfo.msi_module_entry[count].moduleNo) ]
					 |= D_MPSW_ON << subscriberID;
			}
		}
		mng->entrySubscriberUseMap |= D_MPSW_ON << subscriberID;
		result = subscriberID;
		SAA_MSG_LOG_INFO("\naddSubscriber OK pid:%d add:0x%08x port:%d subId:%d",
						info->senderInfo.pid,
						ntohl(info->senderInfo.addr),
						ntohs(info->senderInfo.port),
						subscriberID);

	}else{
		SAA_MSG_LOG_ERR("\naddSubscriber error 0x%x 0x%x",mng->entrySubscriberUseMap,subscriberID);
	}
	SAA_MSG_MNG_UNLOCK(&f_saaw_msg_ctrl_mutex);
	return result;
}

/********************************************************************************************************************/
/*!
 *  @brief  message subscriber delete function
 *  @note   delete subscriber
 *  @param  subscriberID 	[in] subscriber ID
 *  @retval D_MPSW_OK	: OK
 *  @retval -1			: NG
 *  @date   2018/11/20 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static INT f_saa_msg_l_delSubscriber( INT	subscriberID )
{
	T_SAA_MSG_CTRL_SUBSCRIBER_MNG* mng;
	T_SAA_MSG_CTRL_SUBSCRIBER_INFO* info;
	INT				count;
	
	SAA_MSG_MNG_LOCK(&f_saaw_msg_ctrl_mutex);

	mng = &f_saaw_msg_ctrl_mng;
	info = &(mng->info[subscriberID]);
	
	for(count=0 ; count < (info->subReqInfo.moduleNum) ; count++){
		mng->entryModInfoMap[ SAA_MSG_MOD(info->subReqInfo.msi_module_entry[count].moduleNo) ]
			 &= ~(D_MPSW_ON << subscriberID);
	}
	memset((void*)info,0,sizeof(*info));
	mng->entrySubscriberUseMap &= ~(D_MPSW_ON << subscriberID);
	
	SAA_MSG_MNG_UNLOCK(&f_saaw_msg_ctrl_mutex);

	return D_MPSW_OK;
}

/********************************************************************************************************************/
/*!
 *  @brief  message subscriber check function
 *  @note   check subscriber
 *  @param  subscriberID	[in] subscriber ID
 *  @param  sender_pid		[in] subscriber process ID
 *  @retval D_MPSW_OK	: OK
 *  @retval -1			: NG
 *  @date   2018/11/20 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static INT f_saa_msg_l_chkSubscriber( INT	subscriberID ,pid_t sender_pid )
{
	T_SAA_MSG_CTRL_SUBSCRIBER_MNG* mng;
	T_SAA_MSG_CTRL_SUBSCRIBER_INFO* info;
	INT							rtn = -1;

	if(subscriberID >= D_SAA_MSG_SUBSCRIBER_MAX){
		return rtn;
	}

	SAA_MSG_MNG_LOCK(&f_saaw_msg_ctrl_mutex);
	mng = &f_saaw_msg_ctrl_mng;

	if(( mng->entrySubscriberUseMap & (D_MPSW_ON << subscriberID) ) != D_MPSW_OFF ){
		info = &(mng->info[subscriberID]);
		if( info->senderInfo.pid == sender_pid ){
			rtn = D_MPSW_OK;
		}
	}
	SAA_MSG_MNG_UNLOCK(&f_saaw_msg_ctrl_mutex);

	return rtn;
}

/********************************************************************************************************************/
/*!
 *  @brief  message socket create function
 *  @note   create socket
 *  @param  src_port	[in]	server portno
 *  @param  src_addr	[in]	server ipaddress
 *  @retval D_MPSW_OK	: OK
 *  @retval -1			: NG
 *  @date   2018/11/20 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static INT f_saa_msg_l_socket( in_port_t src_port ,in_addr_t src_addr , INT* msg_socFd )
{
	INT					onoff = 1;
	struct sockaddr_in	ssaddr;
	INT					rtn;
	INT					socFd;
	T_SAA_MSG_CTRL_SUBSCRIBER_MNG* mng;


	SAA_MSG_LOG_INFO("\nSocket open Ipaddr:0x%08x portNo:%d",ntohl(src_addr),ntohs(src_port));


	if((socFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		SAA_MSG_LOG_ERR("\nsocket error(%s) Ipaddr:0x%08x portNo:%d",strerror(errno),ntohl(src_addr),ntohs(src_port));
		return -1;
	}
	memset(&ssaddr,0,sizeof(ssaddr));
	ssaddr.sin_family      = AF_INET;
	ssaddr.sin_port        = src_port;
	ssaddr.sin_addr.s_addr = src_addr;
	setsockopt( socFd, SOL_SOCKET, SO_REUSEADDR, (char *)&onoff, sizeof(onoff) );
	if(( rtn = bind(socFd, (struct sockaddr *)&ssaddr, sizeof(ssaddr))) != 0){
		SAA_MSG_LOG_ERR("\nbind error(%s) Ipaddr:0x%08x portNo:%d",strerror(errno),ntohl(src_addr),ntohs(src_port));
		close(socFd);
		return -1;
	}

	SAA_MSG_MNG_UNLOCK(&f_saaw_msg_ctrl_mutex);
	mng = &f_saaw_msg_ctrl_mng;
	mng->msgCtrl.socFd = socFd;
	mng->msgCtrl.addr = src_addr;
	mng->msgCtrl.port = src_port;
	mng->msgCtrl.initflg = D_MPSW_ON;
	*msg_socFd = socFd;
	SAA_MSG_MNG_UNLOCK(&f_saaw_msg_ctrl_mutex);

	return D_MPSW_OK;
}

/********************************************************************************************************************/
/*!
 *  @brief  message send function
 *  @note   send message
 *  @param  message_id		[in]	message ID
 *  @param  request_id		[in]	request ID
 *  @param  msgdata_size	[in]	message data size
 *  @param  msg_p			[in]	message pointer
 *  @retval D_MPSW_OK	: OK
 *  @retval -1			: NG
 *  @date   2018/11/20 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static INT f_saa_msg_l_send( uint32_t message_id,uint32_t request_id,uint32_t msgdata_size,void* msg_p )
{
	T_SAA_MSG_CTRL_SUBSCRIBER_MNG* mng;
	T_SAA_MSG_CTRL_SUBSCRIBER_INFO* info;
	T_MPSW_MSI_MODULE_ENTRY* entryMod;
	struct sockaddr_in	ssaddr;
	INT					hlen = sizeof(ssaddr);
	ssize_t				slen;
	ssize_t				msg_size;
	UINT				count;
	INT					count2;
	UINT				entrySubscriberUseMap;
	UINT				moduleNo;
	UINT				moduleOpeBit;
	UINT				sendflg = D_MPSW_OFF;
	INT					rtn = -1;
	INT					errorcounter;
	INT					sentcounter;

	moduleNo = SAA_MSG_MSGIDTOMOD(message_id);

	SAA_MSG_MNG_LOCK(&f_saaw_msg_ctrl_mutex);
	mng = &f_saaw_msg_ctrl_mng;
	if( mng->entryModInfoMap[moduleNo] == 0 ){
		SAA_MSG_LOG_ERR("\nNot Found Subscriber id:0x%08x req:0x%08x",message_id,request_id);
		SAA_MSG_MNG_UNLOCK(&f_saaw_msg_ctrl_mutex);
		return rtn;
	}
	
	memset(msg_p,0,sizeof(T_MPSW_SRV_MSGHEAD));
	((T_MPSW_SRV_MSGHEAD*)msg_p)->msgId     = message_id;
	((T_MPSW_SRV_MSGHEAD*)msg_p)->requestId = request_id;
	((T_MPSW_SRV_MSGHEAD*)msg_p)->msgLength = msgdata_size;
	((T_MPSW_SRV_MSGHEAD*)msg_p)->senderId  = (INT)getpid();
	msg_size = sizeof(T_MPSW_SRV_MSGHEAD) + msgdata_size;

	entrySubscriberUseMap = mng->entryModInfoMap[moduleNo];
	moduleOpeBit = SAA_MSG_MSGIDTOOPE(message_id);
	errorcounter = 0;
	sentcounter = 0;
	for(count=0 ; count < D_SAA_MSG_SUBSCRIBER_MAX ; count++){	
		sendflg = D_MPSW_OFF;

		if(( entrySubscriberUseMap & ( D_MPSW_ON << count )) == 0){
			continue;
		}
		
		info = &(mng->info[count]);
		for( count2 = 0 ; count2 < (info->subReqInfo.moduleNum) ; count2++ ){
			entryMod = &(info->subReqInfo.msi_module_entry[count2]);
			if(entryMod->moduleNo != moduleNo){
				continue;
			}
			if( entryMod->moduleOpe == 0){
				sendflg = D_MPSW_ON;	/* 無条件送信 */
				break;	
			}
			
			if(( entryMod->moduleOpe & ( D_MPSW_ON << moduleOpeBit ) ) != 0){
				sendflg = D_MPSW_ON;
				break;
			}
		}

		if( sendflg == D_MPSW_ON ){
			((T_MPSW_SRV_MSGHEAD*)msg_p)->subscriberId = count;
			memset((void*)&ssaddr,0,sizeof(ssaddr));
			ssaddr.sin_port        = info->senderInfo.port;
			ssaddr.sin_addr.s_addr = info->senderInfo.addr;

			if(( slen = sendto(mng->msgCtrl.socFd,msg_p,msg_size,0,(const struct sockaddr *)&ssaddr,hlen)) <= 0){
				SAA_MSG_LOG_ERR("\nSend Error(%s) pid:%d sub:%d id:0x%08x req:0x%08x addr:0x%08x port:%d",
								strerror(errno),
								info->senderInfo.pid,
								count,message_id,request_id,
								ntohl(ssaddr.sin_addr.s_addr),
								ntohs(ssaddr.sin_port));
				errorcounter++;
				continue;
			}

			if(slen != msg_size){
				SAA_MSG_LOG_WARN("\nMismatch size send:%ldbyte expected:%ldbyte pid:%d sub:%d id:0x%08x req:0x%08x addr:0x%08x port:%d",
								slen,
								msg_size,
								info->senderInfo.pid,
								count,message_id,request_id,
								ntohl(info->senderInfo.addr),
								ntohs(info->senderInfo.port));
				errorcounter++;
			}
			sentcounter++;
		}else{
			SAA_MSG_LOG_WARN("\nNot Send id:%d sub:%d id:0x%08x req:0x%08x addr:0x%08x port:%d",
								info->senderInfo.pid,
								count,message_id,request_id,
								ntohl(info->senderInfo.addr),
								ntohs(info->senderInfo.port));
			errorcounter++;
		}
	}
	SAA_MSG_MNG_UNLOCK(&f_saaw_msg_ctrl_mutex);

	if((sentcounter == 0) || (errorcounter > 0)){
		rtn = -1;
	}else{
		rtn = D_MPSW_OK;
	}
	return rtn;
}

/********************************************************************************************************************/
/*!
 *  @brief  message receive function
 *  @note   receive message
 *  @param  msg_socFd		[in]	socket number
 *  @param  message_id		[out]	message ID
 *  @param  request_id		[out]	request ID
 *  @param  msgdata_size	[out]	message data size
 *  @param  msg_p			[out]	message pointer
 *  @param  msgmax_size		[in]	message max size
 *  @param  addrInfo		[in]	receive message address information
 *  @retval D_MPSW_OK	: OK
 *  @retval -1			: NG
 *  @date   2018/11/20 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static INT f_saa_msg_l_recv(
	INT		  msg_socFd,
	uint32_t* message_id,
	uint32_t* request_id,
	uint32_t* msgdata_size,
	void* msg_p ,
	uint32_t msgmax_size,
	T_SAA_MSG_CTRL_ADDR* addrInfo)
{
	ssize_t				rlen;
	struct sockaddr_in	rsaddr;
	int					hlen= sizeof(rsaddr);

	while(1){
		if((rlen = recvfrom(msg_socFd, msg_p, msgmax_size , 0 , (struct sockaddr *)&rsaddr , (socklen_t *)&hlen )) <= 0){
			if(errno == EINTR){
				continue;
			}
			SAA_MSG_LOG_ERR("\nrecv error(%s)",strerror(errno));
			return -1;
		}
		break;
	}
	if(rlen >= (ssize_t)sizeof(T_MPSW_SRV_MSGHEAD)){
		*msgdata_size = rlen - sizeof(T_MPSW_SRV_MSGHEAD);
		*message_id    = ((T_MPSW_SRV_MSGHEAD*)msg_p)->msgId;
		*request_id    = ((T_MPSW_SRV_MSGHEAD*)msg_p)->requestId;
		addrInfo->pid  = (pid_t)(((T_MPSW_SRV_MSGHEAD*)msg_p)->senderId);
		addrInfo->addr = rsaddr.sin_addr.s_addr;	/* ntohlはあえて実施しない */
		addrInfo->port = rsaddr.sin_port;			/* ntohsはあえて実施しない */

		SAA_MSG_LOG_DEBUG("\nrecvfrom %s MsgId:0x%08x ReqId:0x%08x size:%d",strerror(errno),*message_id,*request_id,*msgdata_size);

	}else{
		SAA_MSG_LOG_WARN("\nrecv size error(%s) %ld",strerror(errno),rlen);
		return -1;
	}
	return D_MPSW_OK;
}

/********************************************************************************************************************/
/*!
 *  @brief  inddication subscribe check function
 *  @note   receive message

 *  @retval D_MPSW_OK	: OK
 *  @retval -1			: NG
 *  @date   2018/11/20 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static void f_saa_msg_l_sendStartSvInd(	INT msg_socFd,const struct sockaddr *addr,T_MPSW_MSI_SUBSCRIBE_REQ* req)
{
    (void)msg_socFd;
    (void)addr;
    (void)req;

	return;
}


/********************************************************************************************************************/
/*!
 *  @brief  message sender thread function
 *  @note   
 *  @param  arg            [in]  thread information
 *  @return NULL
 *  @date   2018/11/20 FJT)Taniguchi create
 */
/********************************************************************************************************************/
void* f_saa_msg_sender(void* arg)
{
	T_SAA_MSG_CTRL_THREAD_ARG*	thread_arg;
	uint32_t					request_id;
	uint32_t					message_id;
	uint32_t					read_size;
	uint32_t					max_read_size;
	int							rtn;
	void*						data_p;
	void*						head_p;
	pid_t						indPid;

	T_MPSW_MSI_SUBSCRIBE_REQ 	req;
	INT			subscriberID;

	if(arg == NULL){
		SAA_MSG_LOG_ERR("\nsaa_msg_sender:Input arg error");
		return NULL;
	}

	pthread_setname_np( pthread_self() , (const char *)"saa_msgs");

	SAA_MSG_LOG_INFO("\nStarting %s ...",__func__);

	if((rtn = f_saa_msg_l_chkInitMsgCtrl()) != D_MPSW_OK){
		SAA_MSG_LOG_ERR("\nsaa_msg_sender:check init error");
		return NULL;
	}

	thread_arg = (T_SAA_MSG_CTRL_THREAD_ARG*)arg;
	head_p = f_saaw_msg_sender_buff;
	data_p = f_saaw_msg_sender_buff + sizeof(T_MPSW_SRV_MSGHEAD);
	max_read_size = sizeof(f_saaw_msg_sender_buff) - sizeof(T_MPSW_SRV_MSGHEAD);

	while(1){
		rtn = f_saa_com_get_request(max_read_size,&request_id,&message_id,&read_size,data_p);
		if( rtn != D_MSG_BUFFER_GET_OK ){
			SAA_MSG_LOG_WARN("\nsaa_msg_sender:f_saa_com_get_request NG(0x%x)",rtn);
			continue;	
		}

		if( D_MSG_MSI_READY_IND == message_id ){
			req.moduleNum = 1;
			req.msi_module_entry[0].moduleNo = SAA_MSG_MSGIDTOMOD(message_id);
			req.msi_module_entry[0].moduleOpe = 0;
			indPid = getppid();
			subscriberID = f_saa_msg_l_addSubscriber( indPid,thread_arg->dst_port,thread_arg->dst_addr,&req );
			SAA_MSG_LOG_INFO("\nsaa_msg_sender:MSI_READY_IND sent pid:%d addr:0x%08x port:%d sub:%d",
								indPid,ntohl(thread_arg->dst_addr),ntohs(thread_arg->dst_port),subscriberID);
		}

		if((rtn = f_saa_msg_l_send(message_id,request_id,read_size,head_p)) != D_MPSW_OK){
			SAA_MSG_LOG_WARN("\n%s: f_saa_msg_l_send error id:0x%08x req:0x%08x",__func__,message_id,request_id);
			break;
		}else{
			SAA_MSG_LOG_DEBUG("\nMSI send OK(msgID:0x%08x reqId:0x%08x size:%d)",message_id,request_id,read_size);
		}
	}
	SAA_MSG_LOG_ERR("\nf_saa_msg_sender exit");
	return NULL;
}

/********************************************************************************************************************/
/*!
 *  @brief  msi_message receive thread function
 *  @note   
 *  @param  arg            [in]  thread information
 *  @return NULL
 *  @date   2018/11/20 FJT)Taniguchi create
 */
/********************************************************************************************************************/
void* f_saa_msg_receiver(void* arg)
{

	T_SAA_MSG_CTRL_THREAD_ARG*	thread_arg;
	INT							rtn;
	INT							cmrtn;
	T_SAA_MSG_CTRL_ADDR			addrInfo;
	uint32_t					request_id;
	uint32_t					message_id;
	void*						msg_p;
	void*						data_p;
	uint32_t					msg_max_size;
	uint32_t					msg_data_size;
	INT							msg_socFd;
	INT							subscriberID;
	T_MSG_MSI_SUBSCRIBE_CFM		cfm;
	T_MSG_MSI_SUBSCRIBE_REJ		rej;
	struct sockaddr_in			ssaddr;
	int							hlen= sizeof(ssaddr);
	pid_t						senderId;
	ssize_t						sent_result;
	
	if(arg == NULL){
		SAA_MSG_LOG_ERR("\nsaa_msg_receiver:Input arg error");
		return NULL;
	}

	pthread_setname_np( pthread_self() , (const char *)"saa_msgr");

	SAA_MSG_LOG_INFO("\nStarting %s ...",__func__);

	if((rtn = f_saa_msg_l_initSubscriber()) != D_MPSW_OK){
		return NULL;
	}

	thread_arg = (T_SAA_MSG_CTRL_THREAD_ARG*)arg;
	if((rtn = f_saa_msg_l_socket(thread_arg->src_port,thread_arg->src_addr,&msg_socFd)) != D_MPSW_OK){
		SAA_MSG_LOG_ERR("\nsaa_msg_receiver:socket error IPaddr:0x%08x port:%d",
							ntohl(thread_arg->src_addr),ntohs(thread_arg->src_port));
		return NULL;	
	}
	
	msg_p = f_saaw_msg_receiver_buff;
	data_p = f_saaw_msg_receiver_buff + sizeof(T_MPSW_SRV_MSGHEAD);
	msg_max_size = sizeof(f_saaw_msg_receiver_buff);
	
	while(1){
		if(( rtn = f_saa_msg_l_recv(msg_socFd, &message_id, &request_id, &msg_data_size,msg_p,msg_max_size,&addrInfo))
			 != D_MPSW_OK){
			break;
		}
		if( D_MSG_MSI_SUBSCRIBE_REQ == message_id ){
			SAA_MSG_LOG_INFO("\nMSI_SUBSCRIBE_REQ received from pid:%d IPaddr:0x%08x port:%d",addrInfo.pid,ntohl(addrInfo.addr),ntohs(addrInfo.port));

			if((subscriberID = f_saa_msg_l_addSubscriber( addrInfo.pid,addrInfo.port,addrInfo.addr,(T_MPSW_MSI_SUBSCRIBE_REQ*)data_p ))
				 == -1 ){

				memset(&rej,0,sizeof(rej));
				rej.msgHead.msgId = D_MSG_MSI_SUBSCRIBE_REJ;
				rej.msgHead.senderId  = (INT)getpid();
				rej.msgHead.msgLength = sizeof(T_MPSW_MSI_SUBSCRIBE_REJ);
				rej.data.errorInfo.errorCode = -1;
				snprintf(rej.data.errorInfo.errorDescription,sizeof(rej.data.errorInfo.errorDescription),
						"addSubscriber error");
				memset((void*)&ssaddr,0,sizeof(ssaddr));
				ssaddr.sin_port        = addrInfo.port;
				ssaddr.sin_addr.s_addr = addrInfo.addr;
				
				(void)sendto(msg_socFd,(VOID*)&rej,sizeof(rej),0,(const struct sockaddr *)&ssaddr,hlen);

				SAA_MSG_LOG_WARN("\nMSI_SUBSCRIBE_REJ sent to pid:%d IPaddr:0x%08x port:%d",addrInfo.pid,ntohl(addrInfo.addr),ntohs(addrInfo.port));

			}else{
				memset(&cfm,0,sizeof(cfm));

				cfm.msgHead.msgId = D_MSG_MSI_SUBSCRIBE_CFM;
				cfm.msgHead.senderId  = (INT)getpid();
				cfm.msgHead.msgLength = sizeof(T_MPSW_MSI_SUBSCRIBE_CFM) ;
				cfm.data.subscriberId = subscriberID;

				memset((void*)&ssaddr,0,sizeof(ssaddr));
				ssaddr.sin_port        = addrInfo.port;
				ssaddr.sin_addr.s_addr = addrInfo.addr;

				if(( sent_result = sendto(msg_socFd,(VOID*)&cfm,sizeof(cfm),0,(const struct sockaddr *)&ssaddr,hlen) ) == -1){
					SAA_MSG_LOG_WARN("\nMSI_SUBSCRIBE_CFM sent ERR(%s) l:%ld pid:%d IPaddr:0x%08x port:%d",strerror(errno),sent_result,addrInfo.pid,ntohl(addrInfo.addr),ntohs(addrInfo.port));
				}else{
					SAA_MSG_LOG_INFO("\nMSI_SUBSCRIBE_CFM sent to pid:%d IPaddr:0x%08x port:%d",addrInfo.pid,ntohl(addrInfo.addr),ntohs(addrInfo.port));

					f_saa_msg_l_sendStartSvInd(msg_socFd,(const struct sockaddr *)&ssaddr,(T_MPSW_MSI_SUBSCRIBE_REQ*)data_p);

				}
			}
		} 
		else if( D_MSG_MSI_DESUBSCRIBE_IND == message_id ){
			SAA_MSG_LOG_INFO("\nMSI_DESUBSCRIBE_IND received from pid:%d IPaddr:0x%08x port:%d",addrInfo.pid,ntohl(addrInfo.addr),ntohs(addrInfo.port));
			subscriberID = ((T_MPSW_SRV_MSGHEAD*)msg_p)->subscriberId;
			(void)f_saa_msg_l_delSubscriber(subscriberID);
		}
		else{

			subscriberID = ((T_MPSW_SRV_MSGHEAD*)msg_p)->subscriberId;
			senderId = ((T_MPSW_SRV_MSGHEAD*)msg_p)->senderId;

			SAA_MSG_LOG_DEBUG("\nMSI(0x%08x) ReqId:0x%08x size:%d subId:%d sender:%d received from pid:%d IPaddr:0x%08x port:%d",
							message_id,request_id,msg_data_size,subscriberID,senderId,addrInfo.pid,ntohl(addrInfo.addr),ntohs(addrInfo.port));

			if( ( rtn = f_saa_msg_l_chkSubscriber(subscriberID,senderId)) == D_MPSW_OK ){
				cmrtn = D_MSG_BUFFER_GET_OK;

				switch( SAA_MSG_MSGIDTOTYPE( message_id ) ){
					case E_MSI_MSGTYPE_IND:
						cmrtn = f_saa_com_add_notification(message_id,msg_data_size ,data_p);
						break;
					case E_MSI_MSGTYPE_CFM:
					case E_MSI_MSGTYPE_REJ:
						cmrtn = f_saa_com_add_response(request_id, message_id, msg_data_size,data_p);
						break;
					default:
						SAA_MSG_LOG_ERR("\nUnknow received Message 0x%x",message_id);
						break;
				}
				if(cmrtn != D_MSG_BUFFER_GET_OK){
					SAA_MSG_LOG_ERR("\nf_saa_com_add Error MSI:0x%08x Req:0x%08x",message_id,request_id);
				}
			}else{
				SAA_MSG_LOG_WARN("\nNot entry subscriber subId:%d sender:%d",subscriberID,senderId);
			}
		}
	}
	SAA_MSG_LOG_ERR("\nf_saa_msg_receiver exit");
	return NULL;
}

/* @} */
