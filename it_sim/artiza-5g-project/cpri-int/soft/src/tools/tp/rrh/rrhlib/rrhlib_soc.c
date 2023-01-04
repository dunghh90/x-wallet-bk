/*!
 *  @skip	$Id:$
 *  @file	rrhlib_soc.c
 *  @brief	rrh command socket library routine
 *  @author	FJT)Taniguchi
 *  @date	2013/11/25 FJT)Taniguchi create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup	RRH_TP_CMD_LIB
 * @{
 */

#include "rrhlib_local.h"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
/*!
 * @brief		メッセージヘッダ構造体
 */
typedef struct
{
	int  type;	/*!< データタイプ */
	int  len;	/*!< データ長 */
}t_msg_head;

/*!
 * @brief		メッセージデータ構造体
 */
typedef union
{
	int   value;	/*!< int型データ */
	char  str[128];	/*!< string型データ */
}t_msg_data;

/*!
 * @brief		メッセージ構造体
 */
typedef struct
{
	t_msg_head  head;	/*!< メッセージヘッダ情報 */
	t_msg_data  data;	/*!< メッセージデータ情報 */
}t_msg;

/*!
 * @brief		メッセージ構造体
 */
typedef struct
{
	unsigned int data[4*1024];
}t_msg2;


/*!
 * @brief		TFTP読み書きフォーマット
 */
typedef struct
{
	unsigned short opecode;		/*!< operation code */
	unsigned char  filename;	/*!< file name(可変長) */
}t_tftp_wr_rq;


/*!
 * @brief		TFTPデータ転送フォーマット
 */
typedef struct
{
	unsigned short  opecode;	/*!< operation code 				*/
	unsigned short  blkno;		/*!< block no						*/
	unsigned char   data;		/*!< データ(可変長) max is 512byte	*/
}t_tftp_tr_rq;

/*!
 * @brief		TFTPデータ応答フォーマット
 */
typedef struct
{
	unsigned short opecode;		/*!< operation code 				*/
	unsigned short  blkno;		/*!< block no						*/
}t_tftp_rq_rs;

/*!
 * @brief		TFTPエラー応答フォーマット
 */
typedef struct
{
	unsigned short opecode;		/*!< operation code 				*/
	unsigned short error;		/*!< error code 					*/
	unsigned char  errorstr;	/*!< error text						*/
}t_tftp_err_rs;

/*!
 * @brief		LBM format
 */
typedef struct {
	char				dst_mac_addr[6];				/* Destination address					*/
	char				src_mac_addr[6];				/* Source address						*/
	char				vlan_eth_type[2];				/* Ethertype (VLAN:0xxxxx)				*/
	char				vlan_tag[2];					/* vlan-tag								*/
	char				eth_type[2];					/* EtherType (CFM :0x8902)				*/
} T_LBM_HEADER;

typedef struct {
	char				dst_mac_addr[6];				/* Destination address					*/
	char				src_mac_addr[6];				/* Source address						*/
	char				eth_type[2];					/* EtherType (CFM :0x8902)				*/
} T_LBM_HEADER_NONVLAN;


typedef struct {
	char				version;						/* version (MA-Level:3bit + Ver:5bit)	*/
	char				OpCode;							/* OpCode (LBM=0x03/LBR=2)				*/
	char				Flag;							/* Flag (0固定)							*/
	char				TLVoffset;						/* TLV_Offset (4固定)					*/
	char				TransactionID[4];				/* TransactionID						*/
	char				PatternData_type;
	char				PatternData_len[2];
	char				PatternData_id[128];			/* パターンデータ						*/
	char				TLVtype;						/* TVLType								*/
} T_LBM_PAYLOAD;

typedef struct {
	T_LBM_HEADER		head;							/* Etherヘッダ							*/
	T_LBM_PAYLOAD		body;							/* Payload								*/
} T_LBM_FRAME;

typedef struct {
	T_LBM_HEADER_NONVLAN		head;							/* Etherヘッダ							*/
	T_LBM_PAYLOAD		body;							/* Payload								*/
} T_LBM_FRAME_SND;

typedef struct {
	T_LBM_HEADER_NONVLAN	head;						/* Etherヘッダ							*/
	T_LBM_PAYLOAD			body;						/* Payload								*/
} T_LBM_FRAME_RCV;




/*!
 * @brief		アドレスオブジェクト設定処理
 * @note
<pre>
	受信したアドレス情報からオブジェクトを設定する処理である.
	オブジェクトには、IPアドレス("xxx.xxx.xxx.xxx")、ポート番号(dddd)の順で設定する.
</pre>
 * @param		mrb		[in]	mrb status
 * @param		head	[in]	アドレス情報
 * @param		obj		[in]	mrb value
 * @return		N/A
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
static void rrhlib_soc_head_to_obj( mrb_state* mrb ,struct sockaddr_in* head ,mrb_value obj )
{
	char		 str[32];
	mrb_int 	 num;
	mrb_value 	 *ptr;
	mrb_int		 count;
	int 		 len;

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	for(count=0;count < num ; count++)
	{
		switch(count)
		{
			case 0:
				memset(str,0,sizeof(str));
				len = snprintf(str,sizeof(str),"%s",inet_ntoa(head->sin_addr));
				ptr[count] = mrb_str_new(mrb , str , len);
				break;
			case 1:
				ptr[count] = mrb_fixnum_value(ntohs(head->sin_port));
				break;
			default:
				rrhlib_dmsg("Recv Head Unkown : %d\n",count);
				break;
		}
	}
	return;
}

/*!
 * @brief		オブジェクトアドレス設定処理
 * @note
<pre>
	オブジェクトから送信先のアドレス情報を設定する処理である.
	オブジェクトには、IPアドレス("xxx.xxx.xxx.xxx")、ポート番号(dddd)の順で設定する.
</pre>
 * @param		mrb		[in]	mrb status
 * @param		obj		[in]	mrb value
 * @param		head	[in]	アドレス情報
 * @return		N/A
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
static void rrhlib_soc_obj_to_head( mrb_state* mrb ,mrb_value obj , struct sockaddr_in* head )
{
	char		 ip[32];
	mrb_int 	 num;
	mrb_value 	 *ptr;
	mrb_int		 count;

	memset(ip,0,sizeof(ip));

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	head->sin_family = AF_INET;
	head->sin_port = 0;
	head->sin_addr.s_addr = 0;
	for(count=0;count < num ; count++)
	{
		switch (count)
		{
			case 0:
				head->sin_addr.s_addr = inet_addr(RSTRING_PTR(ptr[count]));
				break;
			case 1:
				head->sin_port = htons(mrb_fixnum(ptr[count]));
				break;
			default:
				rrhlib_dmsg("Send Head Unkown : %d\n",count);
				break;
		}
	}
	return;
}

/*!
 * @brief		TCP用ソケットオープン処理
 * @note
<pre>
	TCP通信用ソケットをオープンする処理である.
</pre>
 * @tparam		N/A
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		TCPソケット番号
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_tsocket(mrb_state* mrb, mrb_value self)
{
	int sd;
	mrb_value  msd;
	int onoff = 1;

	errno = 0;
	sd = socket(AF_INET, SOCK_STREAM, 0);
	rrhlib_dmsg("Socket Socket:%u result:%s \n",sd,strerror(errno));
	errno = 0;
	setsockopt( sd, IPPROTO_TCP, TCP_NODELAY, (char *)&onoff, sizeof(onoff) );
	rrhlib_dmsg("Socket Opt result:%s \n",strerror(errno));
	msd = mrb_fixnum_value( sd );
	return msd;
}

/*!
 * @brief		TCP用コネクト処理
 * @note
<pre>
	TCPサーバ(IPアドレス、ポート番号)に対し、コネクション要求(SYN)を行う処理である.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		IPアドレス				[in]	String
 * @tparam		IPポート番号			[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		エラー番号
 * @retval		0		:正常
 * @retval		0以外	:異常
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.tconnect()として使用できる.
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_tconnect(mrb_state* mrb, mrb_value self)
{
	mrb_value  network;
	mrb_int    port;
	mrb_int    sd;
	struct sockaddr_in ssaddr;

	mrb_get_args(mrb, "iSi", &sd,&network,&port);
	ssaddr.sin_family = AF_INET;
	ssaddr.sin_port = htons(port);
	ssaddr.sin_addr.s_addr = inet_addr(RSTRING_PTR(network));
	errno = 0;
	connect(sd,(struct sockaddr *)&ssaddr, sizeof(ssaddr));
	rrhlib_dmsg("Connect Socket:%u Host:\"%s\" Port:%u result:%s \n", sd,RSTRING_PTR(network),port,strerror(errno));
	return mrb_fixnum_value( errno);
}

/*!
 * @brief		TCP用リッスン処理
 * @note
<pre>
	TCPコネクションの最大同時接続数を設定する処理である.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		最大同時接続数			[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		エラー番号
 * @retval		0		:正常
 * @retval		0以外	:異常
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.tlisten()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_tlisten(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;
	mrb_int 	 lis_num;

	mrb_get_args(mrb, "ii", &sd,&lis_num);
	errno = 0;
	listen(sd, lis_num);
	rrhlib_dmsg("Listen Socket:%u Num:%u result:%s \n",sd,lis_num,strerror(errno));
	return mrb_fixnum_value( errno);
}

/*!
 * @brief		TCP用アクセプト処理
 * @note
<pre>
	TCPクライアントからのコネクション要求(SYN)を待つ受け処理である.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		IPヘッダ情報			[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		アクセプトしたソケットディスクリプタ
 * @retval		0		:異常
 * @retval		0以外	:正常
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.taccept()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_taccept(mrb_state* mrb, mrb_value self)
{
	mrb_int 			 sd;
	int 				 ac_sd;
	int					 ac_len;
	struct sockaddr_in 	acaddr;
	mrb_value			msd;
	char				src[16];
	int					onoff= 1;
	mrb_value 			headobj;

	mrb_get_args(mrb, "io", &sd , &headobj);
	memset(src,0,sizeof(src));
	ac_len = sizeof(acaddr);

	rrhlib_dmsg("Accept Start... \n");
	errno = 0;
	ac_sd = accept(sd, (struct sockaddr *)&acaddr , (socklen_t *)&ac_len);
	strncpy(src,inet_ntoa(acaddr.sin_addr),(sizeof(src) - 1));
	rrhlib_dmsg("Accept OK Socket:%u ClinetSocket:%u IP:%s Port:%u result:%s \n",
		sd,
		ac_sd,
		src,
		ntohs(acaddr.sin_port),
		strerror(errno));

	errno = 0;
	setsockopt( ac_sd, IPPROTO_TCP, TCP_NODELAY, (char *)&onoff, sizeof(onoff) );
	rrhlib_dmsg("Accept Opt result:%s \n",strerror(errno));

	rrhlib_soc_head_to_obj(mrb,&acaddr,headobj);

	msd = mrb_fixnum_value( ac_sd );
	return msd;
}

/*!
 * @brief		TCP用データ送信処理
 * @note
<pre>
	TCPデータ送信を行う処理である.
	TCP送信データはint型(値)かstring型(文字列)を許容する.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		送信データ情報			[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		送信回数
 * @retval		0		:送信失敗
 * @retval		0以外	:送信成功
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.tsend()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_tsend(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;
	mrb_int 	 num;
	mrb_value 	 obj;
	mrb_value 	 *ptr;
	mrb_int		 count;
	t_msg 		 msg;
	int  		 mlen;

	mrb_get_args(mrb, "io",&sd,&obj);

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	rrhlib_dmsg("Send Socket:%u Num:%u \n",sd,num);
	if(num <= 0)
	{
		return mrb_fixnum_value(0);
	}
	for(count=0; count< num ; count++)
	{
		memset(&msg,0,sizeof(msg));
		msg.head.type = mrb_type(ptr[count]);
		switch(msg.head.type)
		{
			case MRB_TT_FIXNUM:
				msg.head.len = 4;
				msg.data.value = mrb_fixnum(ptr[count]);
				break;
			case MRB_TT_STRING:
				msg.head.len = strlen(RSTRING_PTR(ptr[count]));
				strncpy(msg.data.str,RSTRING_PTR(ptr[count]),msg.head.len);
				break;
			default:
				break;
		}
		errno = 0;
		if((mlen = sendto(sd, &msg, sizeof(msg.head) + msg.head.len, 0, NULL, 0)) <= 0)
		{
			rrhlib_dmsg("Send Rlen:%d result:%s \n",mlen,strerror(errno));
			break;
		}
		rrhlib_dmsg("Send Type:%d Len:%d SLen:%d \n",msg.head.type,msg.head.len,mlen);
	}
	rrhlib_dmsg("Send End:%u\n",count);
	return mrb_fixnum_value(count);
}

/*!
 * @brief		TCP用データ送信処理
 * @note
<pre>
	TCPデータ送信を行う処理である.
	TCP送信データはint型(値)かstring型(文字列)を許容する.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		送信データ情報			[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		送信回数
 * @retval		0		:送信失敗
 * @retval		0以外	:送信成功
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.tsend()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_tsend2(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;
	mrb_int 	 num;
	mrb_value 	 obj;
	mrb_value 	 *ptr;
	mrb_int		 count;
	t_msg2 		 msg;
	int  		 mlen;
	int  		 string_len;

	mrb_get_args(mrb, "io",&sd,&obj);

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	rrhlib_dmsg("Send Socket:%u Num:%u \n",sd,num);
	if(num <= 0)
	{
		return mrb_fixnum_value(0);
	}

	for(count=0; count< num ; ){
		switch(mrb_type(ptr[count])){
			case MRB_TT_FIXNUM:
				msg.data[count]  = htonl(mrb_fixnum(ptr[count]));
				count++;
				break;
			case MRB_TT_STRING:
				string_len = (int)strlen(RSTRING_PTR(ptr[count]));
				memcpy((char*)&(msg.data[count]),RSTRING_PTR(ptr[count]),string_len);
				count += string_len / 4;
				if((string_len % 4) > 0){
					count++;
				}
				break;
			default:
				count++;
				break;
		}
	}
	errno = 0;
	if((mlen = sendto(sd, (const void*)&msg, num * 4, 0, NULL, 0)) <= 0){
		rrhlib_dmsg("Send Rlen:%d result:%s \n",mlen,strerror(errno));
	}else{
		rrhlib_dmsg("Send %d Len:%d SLen:%d \n",num,num * 4,mlen);
	}
	return mrb_fixnum_value(count);
}

/*!
 * @brief		TCP用データ受信処理
 * @note
<pre>
	TCPデータ受信を行う処理である.
	TCP受信データはint型(値)かstring型(文字列)を許容する.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		受信データ情報			[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		受信回数
 * @retval		0		:受信失敗
 * @retval		0以外	:受信成功
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.trecv()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_trecv(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;
	int			 rlen;
	mrb_value 	 obj;
	mrb_value 	 *ptr;
	mrb_int 	 num;
	mrb_int      count;
	t_msg 		 msg;
	char*		 data_p;

	mrb_get_args(mrb, "io", &sd,&obj);
	
	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	if(num <= 0)
	{
		return mrb_fixnum_value(0);
	}
	rrhlib_dmsg("Recv Socket:%u \n",sd);
	rlen = 0;
	for(count = 0 ; count < num ; count++)
	{
		if(rlen == 0)
		{
			memset(&msg,0,sizeof(msg));
			errno = 0;
			if((rlen = recv(sd, &msg, sizeof(msg), 0)) <= 0)
			{
				rrhlib_dmsg("Recv Rlen:%d result:%s \n",rlen,strerror(errno));
				break;
			}
		}
		else
		{
			data_p = ((char*)&msg) + (sizeof(msg.head) + msg.head.len) ;
			memcpy(&msg,data_p, rlen);
			rrhlib_dmsg("Recv Nagle Rlen:%d\n",rlen);
		}
		rrhlib_dmsg("Recv Type:%d Len:%d RLen:%d \n",msg.head.type,msg.head.len,rlen);
		switch(msg.head.type)
		{
			case MRB_TT_FIXNUM:
				ptr[count] = mrb_fixnum_value(msg.data.value);
				break;
			case MRB_TT_STRING:
				ptr[count] = mrb_str_new(mrb , msg.data.str , msg.head.len);
				break;
			default:
				break;
		}
		rlen -= sizeof(msg.head) + msg.head.len;
	}
	rrhlib_dmsg("Recv End:%u\n",count);
	return mrb_fixnum_value(count);
}

/*!
 * @brief		TCP用データ受信処理
 * @note
<pre>
	TCPデータ受信を行う処理である.
	TCP受信データはint型(値)かstring型(文字列)を許容する.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		受信データ情報			[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		受信回数
 * @retval		0		:受信失敗
 * @retval		0以外	:受信成功
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.trecv()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_trecv2(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;
	int			 rlen;
	mrb_value 	 obj;
	mrb_value 	 *ptr;
	mrb_int 	 num;
	mrb_int      count;
	t_msg2 		 msg;

	mrb_get_args(mrb, "io", &sd,&obj);
	
	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	if(num <= 0)
	{
		return mrb_fixnum_value(0);
	}
	rrhlib_dmsg("Recv Socket:%u \n",sd);
	errno = 0;
	if((rlen = recv(sd, &msg, sizeof(msg), 0)) <= 0){
		rrhlib_dmsg("Recv Rlen:%d result:%s \n",rlen,strerror(errno));
		return mrb_fixnum_value(0);
	}
	rrhlib_dmsg("Recv RLen:%d \n",rlen);
	for(count = 0 ; count < num ; count++){
		ptr[count] = mrb_fixnum_value(ntohl(msg.data[count]));
		rlen -= 4;
		if(rlen <= 0){
			break;
		}
	}
	rrhlib_dmsg("Recv End:%u\n",count);
	return mrb_fixnum_value(count);
}

/*!
 * @brief		UDP用ソケットオープン処理
 * @note
<pre>
	UDP通信用ソケットをオープンする処理である.
</pre>
 * @tparam		N/A
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		TCPソケット番号
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.usocket()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_usocket(mrb_state* mrb, mrb_value self)
{
	int sd;
	mrb_value  msd;

	errno = 0;
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	rrhlib_dmsg("Socket Socket:%u result:%s \n",sd,strerror(errno));
	msd = mrb_fixnum_value( sd );
	return msd;
}


/*!
 * @brief		UDP用データ送信処理
 * @note
<pre>
	UDPデータ送信を行う処理である.
	UDP送信データはint型(値)かstring型(文字列)を許容する.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		送信データ情報			[in]	Object
 * @tparam		IPヘッダ情報			[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		送信回数
 * @retval		0		:送信失敗
 * @retval		0以外	:送信成功
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.usend()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_usend(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;
	mrb_int 	 num;
	mrb_value 	 obj;
	mrb_value 	 headobj;
	mrb_value 	 *ptr;
	mrb_int		 count;
	t_msg 		 msg;
	int  		 mlen;
	struct sockaddr_in ssaddr;

	mrb_get_args(mrb, "ioo",&sd,&obj,&headobj);

	rrhlib_soc_obj_to_head(mrb,headobj,&ssaddr);

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	rrhlib_dmsg("Send Socket:%u Num:%u Ip:%08x Port:%d \n",sd,num,ntohl(ssaddr.sin_addr.s_addr),ntohs(ssaddr.sin_port));
	if(num <= 0)
	{
		return mrb_fixnum_value(0);
	}
	for(count=0; count< num ; count++)
	{
		memset(&msg,0,sizeof(msg));
		msg.head.type = mrb_type(ptr[count]);
		switch(msg.head.type)
		{
			case MRB_TT_FIXNUM:
				msg.head.len = 4;
				msg.data.value = mrb_fixnum(ptr[count]);
				break;
			case MRB_TT_STRING:
				msg.head.len = strlen(RSTRING_PTR(ptr[count]));
				strncpy(msg.data.str,RSTRING_PTR(ptr[count]),msg.head.len);
				break;
			default:
				break;
		}
		errno = 0;
		if((mlen = sendto(sd, &msg, sizeof(msg.head) + msg.head.len, 0, (const struct sockaddr *)&ssaddr, sizeof(ssaddr))) <= 0)
		{
			rrhlib_dmsg("Send Rlen:%d result:%s \n",mlen,strerror(errno));
			break;
		}
		rrhlib_dmsg("Send Type:%d Len:%d SLen:%d \n",msg.head.type,msg.head.len,mlen);
	}
	rrhlib_dmsg("Send End:%u\n",count);
	return mrb_fixnum_value(count);
}



/*!
 * @brief		UDP用データ受信処理
 * @note
<pre>
	UDPデータ受信を行う処理である.
	UDP受信データはint型(値)かstring型(文字列)を許容する.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		受信データ情報			[in]	Object
 * @tparam		IPヘッダ情報			[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		受信回数
 * @retval		0		:受信失敗
 * @retval		0以外	:受信成功
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.urecv()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_urecv(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;
	int			 rlen;
	mrb_value 	 obj;
	mrb_value 	 headobj;
	mrb_value 	 *ptr;
	mrb_int 	 num;
	mrb_int          count;
	t_msg 		 msg;
	char*		 data_p;
	struct sockaddr_in rsaddr;
	int			 hlen;

	mrb_get_args(mrb, "ioo",&sd,&obj,&headobj);

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	if(num <= 0)
	{
		return mrb_fixnum_value(0);
	}
	rrhlib_dmsg("Recv Socket:%u \n",sd);
	rlen = 0;
	hlen = sizeof(rsaddr);
	for(count = 0 ; count < num ; count++)
	{
		if(rlen == 0)
		{
			memset(&rsaddr,0,sizeof(rsaddr));			
			memset(&msg,0,sizeof(msg));
			errno = 0;
			if((rlen = recvfrom(sd, &msg, sizeof(msg), 0 , (struct sockaddr *)&rsaddr ,(socklen_t *)&hlen )) <= 0)
			{
				rrhlib_dmsg("Recv Rlen:%d result:%s \n",rlen,strerror(errno));
				break;
			}
			rrhlib_soc_head_to_obj(mrb,&rsaddr,headobj);
		}
		else
		{
			data_p = ((char*)&msg) + (sizeof(msg.head) + msg.head.len) ;
			memcpy(&msg,data_p, rlen);
			rrhlib_dmsg("Recv Nagle Rlen:%d\n",rlen);
		}
		rrhlib_dmsg("Recv Type:%d Len:%d RLen:%d \n",msg.head.type,msg.head.len,rlen);
		switch(msg.head.type)
		{
			case MRB_TT_FIXNUM:
				ptr[count] = mrb_fixnum_value(msg.data.value);
				break;
			case MRB_TT_STRING:
				ptr[count] = mrb_str_new(mrb , msg.data.str , msg.head.len);
				break;
			default:
				break;
		}
		rlen -= sizeof(msg.head) + msg.head.len;
	}
	rrhlib_dmsg("Recv End:%u\n",count);
	return mrb_fixnum_value(count);
}

/*!
 * @brief		バインド処理
 * @note
<pre>
	オープンしたソケットとIPアドレス、IPポート番号をバインドする処理である.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		IPアドレス				[in]	String
 * @tparam		IPポート番号			[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		エラー番号
 * @retval		0		:正常
 * @retval		0以外	:異常
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.tbind(),Rrh.ubind()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_bind(mrb_state* mrb, mrb_value self)
{
	mrb_value  host;
	mrb_int    port;
	mrb_int    sd;
	struct sockaddr_in ssaddr;
	int onoff = 1;

	mrb_get_args(mrb, "iSi", &sd,&host,&port);
	ssaddr.sin_family = AF_INET;
	ssaddr.sin_port = htons(port);
	ssaddr.sin_addr.s_addr = inet_addr(RSTRING_PTR(host));
	errno = 0;
	setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, (char *)&onoff, sizeof(onoff) );
	rrhlib_dmsg("Bind Opt result:%s \n",strerror(errno));
	errno = 0;
	bind(sd, (struct sockaddr *)&ssaddr, sizeof(ssaddr));
	rrhlib_dmsg("Bind Socket:%u Host:\"%s\" Port:%u result:%s \n", sd,RSTRING_PTR(host),port,strerror(errno));
	return mrb_fixnum_value( errno);
}


/*!
 * @brief		recv処理
 * @note
<pre>
	recv関数のカバー関数である.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		受信バッファアドレス	[in]	Integer
 * @tparam		受信バッファサイズ		[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		受信サイズ
 * @retval		0		:受信失敗
 * @retval		0以外	:受信成功
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.recv()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_recv(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;
	int			 rlen;
	mrb_int		 buf;
	mrb_int		 size;

	mrb_get_args(mrb, "iii",&sd,&buf,&size);

	rrhlib_dmsg("Recv Socket:%u Buf:%08x Size:%u \n",sd,buf,size);
	rlen = 0;
	errno = 0;
	rlen = recv(sd, (void*)buf, size, 0);
	rrhlib_dmsg("Recv Rlen:%d result:%s \n",rlen,strerror(errno));
	if(rlen <= 0)
	{
		return mrb_fixnum_value(0);
	}
	return mrb_fixnum_value(rlen);
}


/*!
 * @brief		recvfrom処理
 * @note
<pre>
	recvfrom関数のカバー関数である.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		受信バッファアドレス	[in]	Integer
 * @tparam		受信バッファサイズ		[in]	Integer
 * @tparam		IPヘッダ情報			[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		受信サイズ
 * @retval		0		:受信失敗
 * @retval		0以外	:受信成功
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.recvfrom()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_recvfrom(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;
	int			 rlen;
	mrb_int		 buf;
	mrb_int		 size;
	mrb_value 	 headobj;
	struct sockaddr_in rsaddr;
	int			 hlen;

	mrb_get_args(mrb, "iiio",&sd,&buf,&size,&headobj);

	rrhlib_dmsg("Recvfrom Socket:%u Buf:%08x Size:%u \n",sd,buf,size);
	rlen = 0;
	hlen = sizeof(rsaddr);
	memset(&rsaddr,0,sizeof(rsaddr));
	errno = 0;
	if((rlen = recvfrom(sd, (void*)buf, size, 0 , (struct sockaddr *)&rsaddr ,(socklen_t *)&hlen )) <= 0)
	{
		rrhlib_dmsg("Recv Rlen:%d result:%s \n",rlen,strerror(errno));
		return mrb_fixnum_value(0);
	}
	rrhlib_soc_head_to_obj(mrb,&rsaddr,headobj);
	rrhlib_dmsg("Recvfrom Socket:%u Size:%d Ip:%08x Port:%d \n",
				sd,rlen,ntohl(rsaddr.sin_addr.s_addr),ntohs(rsaddr.sin_port));

	return mrb_fixnum_value(rlen);
}

/*!
 * @brief		send処理
 * @note
<pre>
	send関数のカバー関数である.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		送信バッファアドレス	[in]	Integer
 * @tparam		送信バッファサイズ		[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		送信サイズ
 * @retval		0		:送信失敗
 * @retval		0以外	:送信成功
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.send()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_send(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;
	mrb_int 	 buf;
	mrb_int 	 size;
	int  		 mlen;

	mrb_get_args(mrb, "iii",&sd,&buf,&size);

	rrhlib_dmsg("Send Socket:%u Buf:0x%08x Size:%u \n",sd,buf,size);
	errno = 0;
	mlen = send(sd, (void*)buf, size , 0);
	rrhlib_dmsg("Send Slen:%d result:%s \n",mlen,strerror(errno));
	if(mlen <= 0)
	{
		return mrb_fixnum_value(0);
	}
	return mrb_fixnum_value(mlen);
}


/*!
 * @brief		sendto処理
 * @note
<pre>
	sendto関数のカバー関数である.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		送信バッファアドレス	[in]	Integer
 * @tparam		送信バッファサイズ		[in]	Integer
 * @tparam		IPヘッダ情報			[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		送信サイズ
 * @retval		0		:送信失敗
 * @retval		0以外	:送信成功
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.sendto()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_sendto(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;
	mrb_int 	 buf;
	mrb_int 	 size;
	mrb_value 	 headobj;
	int  		 mlen;
	struct sockaddr_in ssaddr;

	mrb_get_args(mrb, "iiio",&sd,&buf,&size,&headobj);

	rrhlib_soc_obj_to_head(mrb,headobj,&ssaddr);

	rrhlib_dmsg("Sendto Socket:%u Buf:0x%08x Size:%u Ip:%08x Port:%d \n",
				sd,buf,size,ntohl(ssaddr.sin_addr.s_addr),ntohs(ssaddr.sin_port));
	errno = 0;
	if((mlen = sendto(sd, (void*)buf, size , 0, (const struct sockaddr *)&ssaddr, sizeof(ssaddr))) <= 0)
	{
		rrhlib_dmsg("Sendto Slen:%d result:%s \n",mlen,strerror(errno));
		return mrb_fixnum_value(0);
	}
	rrhlib_dmsg("Sendto End:%u\n",mlen);
	return mrb_fixnum_value(mlen);
}

/*!
 * @brief		select処理
 * @note
<pre>
	select関数のカバー関数である.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @tparam		セレクト待ち秒			[in]	Integer
 * @tparam		セレクト待ちマイクロ秒	[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		self
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.select()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_select(mrb_state* mrb, mrb_value self)
{
	mrb_int 		 sd;
	mrb_int 		 sec;
	mrb_int 		 usec;
	fd_set 			 fds;
	struct timeval	 wtime;
	int				 rtn;

	mrb_get_args(mrb, "iii", &sd,&sec,&usec);

	wtime.tv_sec = sec;
	wtime.tv_usec = usec;
	
	FD_ZERO(&fds);
	FD_SET(sd,&fds);

	errno = 0;
	rrhlib_dmsg("Select Socket:%u Time:%d.%d sec \n",sd,sec,usec);
	rtn = select(sd + 1, &fds, NULL,NULL, &wtime);
	rrhlib_dmsg("Select result:(%d)%s \n",rtn,strerror(errno));
	return mrb_fixnum_value(rtn);
}

/*!
 * @brief		複数ソケット用select処理
 * @note
<pre>
	複数ソケットをselectするための関数である.
</pre>
 * @tparam		ソケット情報			[in]	Object
 * @tparam		セレクト待ち秒			[in]	Integer
 * @tparam		マイクロセレクト待ち秒	[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		self
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.nselect()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_nselect(mrb_state* mrb, mrb_value self)
{
	mrb_int 		 sd;
	mrb_int 		 sec;
	mrb_int 		 usec;
	fd_set 			 fds;
	struct timeval	 wtime;
	int				 rtn;
	mrb_value 		 obj;
	mrb_value 		 *ptr;
	mrb_int 		 num;
	mrb_int			 count;
	int				 sdnum;

	mrb_get_args(mrb, "oii", &obj,&sec,&usec);

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);


	wtime.tv_sec = sec;
	wtime.tv_usec = usec;
	
	FD_ZERO(&fds);
	sdnum = 0;
	for(count=0;count < num ; count++)
	{
		if(mrb_type(ptr[count]) != MRB_TT_FIXNUM)
		{
			return mrb_fixnum_value(-1);
		}
		sd = mrb_fixnum(ptr[count]);
		FD_SET(sd,&fds);
		if(sd > sdnum)
		{
			sdnum = sd;
		}
	}
	sdnum++;
	rrhlib_dmsg("NSelect Socket NUM :%d Time:%d.%d sec \n",sdnum,sec,usec);
	errno = 0;
	rtn = select(sdnum, &fds, NULL,NULL, &wtime);
	rrhlib_dmsg("NSelect result:(%d)%s \n",rtn,strerror(errno));
	return mrb_fixnum_value(rtn);
}

/*!
 * @brief		ソケットクローズ処理
 * @note
<pre>
	オープンしたソケットをクローズする処理である.
	全てのプロトコルソケットをクローズできる.
</pre>
 * @tparam		ソケットディスクリプタ	[in]	Integer
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		self
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.tclose(),Rrh.uclose()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_close(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;

	mrb_get_args(mrb, "i", &sd);
	rrhlib_dmsg("Close Socket:%u \n",sd);
	close(sd);
	return self;
}

/*!
 * @brief		tftpデータ獲得処理
 * @note
<pre>
	tftp受信データをオブジェクトに設定する処理である.
</pre>
 * @tparam		受信バッファアドレス	[in]	Integer
 * @tparam		tftp情報				[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		self
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.tftpget()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_tftpget(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 buf;
	void*		 buf_p;
	mrb_value 	 obj;
	unsigned short opecode;
	int			 len;
	char*		 data_p;
	int			 blkno;
	int			 errorcd;
	mrb_int 	 num;
	mrb_value 	 *ptr;
	unsigned long wbuf;

	mrb_get_args(mrb, "io", &buf,&obj);
	buf_p = (void*)buf;

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	if(num == 0)
	{
		return mrb_fixnum_value(0);
	}
	opecode = ntohs(*(unsigned short*)buf_p);
	ptr[0] = mrb_fixnum_value(opecode);
	switch(opecode)
	{
		case 1: /* 読み出し要求 */
		case 2: /* 書き込み要求 */
			data_p = (char*)&(((t_tftp_wr_rq *)buf_p)->filename);
			len = strlen(data_p);		/* filename */
			ptr[1] = mrb_str_new(mrb , data_p , len);
			data_p += len + 1;
			len = strlen(data_p);	/* mode */ 
			ptr[2] = mrb_str_new(mrb , data_p ,len);
			break;
		case 3: /* 転送 */
			blkno = ntohs(((t_tftp_tr_rq *)buf_p)->blkno);
			ptr[3] = mrb_fixnum_value(blkno);
			wbuf = ( unsigned long )&(((t_tftp_tr_rq *)buf_p)->data);
			ptr[5] = mrb_fixnum_value(wbuf);
			break;
		case 4: /* 応答 */
			blkno = ntohs(((t_tftp_rq_rs *)buf_p)->blkno);
			ptr[3] = mrb_fixnum_value(blkno);
			break;
		case 5: /* エラー */
			errorcd = ntohs(((t_tftp_err_rs *)buf_p)->error);
			ptr[6] = mrb_fixnum_value(errorcd);
			data_p = (char*)&(((t_tftp_err_rs *)buf_p)->errorstr);
			len = strlen(data_p);		/* errstring */
			ptr[7] = mrb_str_new(mrb , data_p , len);
			break;
		default:
			opecode = 0;
			break;
	}
	rrhlib_dmsg("tftpget ope:%d \n",opecode);
	return mrb_fixnum_value( opecode );
}


/*!
 * @brief		tftpデータ設定処理
 * @note
<pre>
	オブジェクトからtftp送信データを設定する処理である.
</pre>
 * @tparam		送信バッファアドレス	[in]	Integer
 * @tparam		tftp情報				[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		self
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.tftpset()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_tftpset(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 buf;
	void*		 buf_p;
	mrb_value 	 obj;
	unsigned short opecode;
	int			 len;
	char*		 data_p;
	mrb_int 	 num;
	mrb_value 	 *ptr;
	unsigned long			 rdata;

	mrb_get_args(mrb, "io", &buf,&obj);
	buf_p = (void*)buf;

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	if(num == 0)
	{
		return mrb_fixnum_value(0);
	}
	opecode = (unsigned short)mrb_fixnum(ptr[0]);
	((t_tftp_wr_rq *)buf_p)->opecode = htons(opecode);
	switch(opecode)
	{
		case 1: /* 読み出し要求 */
		case 2: /* 書き込み要求 */
			data_p = (char*)&(((t_tftp_wr_rq *)buf_p)->filename);
			len = strlen(RSTRING_PTR(ptr[1]));		/* filename */
			strncpy(data_p,RSTRING_PTR(ptr[1]),len);
			data_p[len] = '\0';
			data_p += len + 1;
			len = strlen(RSTRING_PTR(ptr[2]));		/* mode */
			strncpy(data_p,RSTRING_PTR(ptr[2]),len);
			data_p[len] = '\0';
			break;
		case 3: /* 転送 */
			((t_tftp_tr_rq *)buf_p)->blkno = htons((unsigned short)mrb_fixnum(ptr[3]));
			data_p = (char*)&(((t_tftp_tr_rq *)buf_p)->data);
			len = mrb_fixnum(ptr[4]);
			rdata = mrb_fixnum(ptr[5]);
			memcpy(data_p,(void*)rdata,len);
			break;
		case 4: /* 応答 */
			((t_tftp_rq_rs *)buf_p)->blkno = htons((unsigned short)mrb_fixnum(ptr[3]));
			break;
		case 5: /* エラー */
			((t_tftp_err_rs *)buf_p)->error = htons((unsigned short)mrb_fixnum(ptr[6]));
			data_p = (char*)&(((t_tftp_err_rs *)buf_p)->errorstr);
			len = strlen(RSTRING_PTR(ptr[7]));		/* errorstring */
			strncpy(data_p,RSTRING_PTR(ptr[7]),len);
			data_p[len] = '\0';
			break;
		default:
			opecode = 0;
			break;
	}
	rrhlib_dmsg("tftpset ope:%d \n",opecode);
	return mrb_fixnum_value( opecode );
}

/*!
 * @brief		httpデータ獲得処理
 * @note
<pre>
	http受信データをオブジェクトに設定する処理である.
</pre>
 * @tparam		受信バッファアドレス	[in]	Integer
 * @tparam		http情報				[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		self
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.httpget()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_httpget(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 buf;
	mrb_value 	 obj;
	char*		 str_p;
	mrb_int 	 num;
	mrb_value 	 *ptr;
	mrb_int		 count;

	mrb_get_args(mrb, "io", &buf,&obj);

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	if(num == 0)
	{
		return mrb_fixnum_value(0);
	}
	str_p = strtok((char*)buf,"\n");
	for(count = 0; count < num ;count++)
	{
		if( str_p == NULL )
		{
			break;
		}
		ptr[count] = mrb_str_new(mrb , str_p , strlen(str_p));
		str_p = strtok(NULL,"\n");
	}
	rrhlib_dmsg("hftpget count:%d \n",count);
	return mrb_fixnum_value( count );
}

/*!
 * @brief		httpデータ設定処理
 * @note
<pre>
	オブジェクトからhttp送信データを設定する処理である.
</pre>
 * @tparam		送信バッファアドレス	[in]	Integer
 * @tparam		http情報				[in]	Object
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		self
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.httpset()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_httpset(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 buf;
	mrb_value 	 obj;
	int			 len;
	int			 alllen = 0;
	char*		 data_p;
	mrb_int 	 num;
	mrb_value 	 *ptr;
	mrb_int		 count;

	mrb_get_args(mrb, "io", &buf,&obj);

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	if(num == 0)
	{
		return mrb_fixnum_value(0);
	}
	rrhlib_dmsg("httpset num:%d buf 0x%08x \n",num,buf);
	data_p = (char*)buf;
	for(count=0 ; count < num ; count++ )
	{
		if(mrb_type(ptr[count]) != MRB_TT_STRING )
		{
			return mrb_fixnum_value(0);
		}
		len = strlen( RSTRING_PTR(ptr[count]) );
		strncpy(data_p , RSTRING_PTR(ptr[count]) ,len );
		data_p += len;
		alllen += len;
	}
	rrhlib_dmsg("httpset len:%d \n",alllen);
	return mrb_fixnum_value( alllen );
}

/*!
 * @brief		ioctl処理
 * @note
<pre>
	ソケットioctl処理のカバー関数である.
	現在、考え中
</pre>
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		エラー番号
 * @retval		0		:正常
 * @retval		0以外	:異常
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_ioctl(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 sd;
	mrb_value	 mcmd;
	int			 cmd;
	struct ifreq cmdinfo;

	mrb_get_args(mrb, "iS", &sd,&mcmd);
	
	if(strncmp("SIOCGIFADDR",RSTRING_PTR(mcmd),sizeof("SIOCGIFADDR")) == 0)
	{
		cmd = SIOCGIFADDR;
	}
	else
	{
		return mrb_fixnum_value( -1 );
	}
	errno = 0;
	ioctl(sd,cmd,&cmdinfo);
	rrhlib_dmsg("Ioctl Socket:%u command:\"%s\" result:%s \n",sd,RSTRING_PTR(mcmd),strerror(errno));
	return mrb_fixnum_value( errno);
}


/*!
 * @brief		nota処理
 * @note		nota処理
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		値をドット表記で返す
 * @retval		0		:正常
 * @retval		0以外	:異常
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_ntoa(mrb_state* mrb, mrb_value self)
{
	mrb_int 	 nwip;
	mrb_int 	 ntwkbyte;
	char		 str[16];
	unsigned int len;
	struct in_addr n_addr;

	mrb_get_args(mrb, "ii", &nwip,&ntwkbyte);
	if(ntwkbyte == 1){
		nwip = ntohl(nwip);
	}
	n_addr.s_addr = (in_addr_t)nwip;
	memset(str,0,sizeof(str));
	len = snprintf(str,sizeof(str),"%s",inet_ntoa(n_addr));
	rrhlib_dmsg("ntoa 0x%08x -> %s \n",(int)nwip,str);
	return mrb_str_new(mrb, str, len);
}


/*!
 * @brief		aotn処理
 * @note		aotn処理
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		ドット表記を値で返す
 * @retval		0		:正常
 * @retval		0以外	:異常
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_aton(mrb_state* mrb, mrb_value self)
{
	mrb_value 	 dtip;
	mrb_int 	 ntwkbyte;
	in_addr_t	 nwip;

	mrb_get_args(mrb, "Si", &dtip,&ntwkbyte);
	nwip = inet_addr((const char*)RSTRING_PTR(dtip));
	if(ntwkbyte == 1){
		nwip = htonl(nwip);
	}
	rrhlib_dmsg("aton %s -> 0x%08x \n",RSTRING_PTR(dtip),nwip);
	return mrb_fixnum_value(nwip);
}

/*!
 * @brief		LBM send処理
 * @note		LBM send処理
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		ドット表記を値で返す
 * @retval		0		:正常
 * @retval		0以外	:異常
 * @date		2019/6/7 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_lbmsend(mrb_state* mrb, mrb_value self)
{
	mrb_value						ethname;
	mrb_value						dsthwaddr;
	mrb_int							vlanid;
	mrb_int							vlanpcp;
	mrb_int							level;
	mrb_int							ope;
	mrb_int							trid;
	mrb_value						ma_value;
	int								dsthwaddr_i[6];
	int								sock;
	int								rtn;
	int								settrid;
	struct ifreq					ifr;
	struct sockaddr_ll				sll;
	T_LBM_FRAME						sndData;
	T_LBM_FRAME_SND					sndData_vlan;
	unsigned int					count;
	const char*						ename;
	VOID*							sndData_p;
	size_t							sndDataLen;
	

	mrb_get_args(mrb, "SSiiiiiS", &ethname,&dsthwaddr,&vlanid,&vlanpcp,&level,&ope,&trid,&ma_value);

	rrhlib_dmsg("LBM send start %s dst:%s vid:%d pcp:%d lvl:%d \n",RSTRING_PTR(ethname),RSTRING_PTR(dsthwaddr),vlanid,vlanpcp,level);

	if( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		rrhlib_dmsg("socket error %d %s\n",sock,strerror(errno));
		return mrb_false_value();
	}

	memset( &ifr, 0, sizeof(struct ifreq) );
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, RSTRING_PTR(ethname), IFNAMSIZ-1);
	if( (rtn = ioctl(sock, SIOCGIFHWADDR, &ifr)) < 0 ) {
		rrhlib_dmsg("ioctl error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}
	close(sock);
	memset(dsthwaddr_i,0,sizeof(dsthwaddr_i));
	if( sscanf( RSTRING_PTR(dsthwaddr), "%x:%x:%x:%x:%x:%x", 
			&dsthwaddr_i[0],&dsthwaddr_i[1],&dsthwaddr_i[2],&dsthwaddr_i[3],&dsthwaddr_i[4],&dsthwaddr_i[5]) == EOF )
	{
		rrhlib_dmsg("sscanf error %s\n",RSTRING_PTR(dsthwaddr));
		return mrb_false_value();
	}

	ename = strrchr(RSTRING_PTR(ethname), '.');
	if(ename == NULL){
		/* 通常のeth*/
		memset( (UCHAR *)&sndData, 0, sizeof(sndData) );
		for(count=0;count < 6;count++){
			sndData.head.dst_mac_addr[count] = (char)dsthwaddr_i[count];
		}
		memcpy( sndData.head.src_mac_addr, &ifr.ifr_hwaddr.sa_data[0], D_RRH_MAC_ADDR_SIZE );
		sndData.head.vlan_eth_type[0] = 0x81;
		sndData.head.vlan_eth_type[1] = 0x00;
		sndData.head.vlan_tag[0]      = (((char)vlanpcp & 0x7) << 5);	/* pcp */
		sndData.head.vlan_tag[1]      = (char)(vlanid & 0xFFF);			/* VID */
		sndData.head.eth_type[0]      = 0x89;
		sndData.head.eth_type[1]      = 0x02;
		sndData.body.version   = level << 5;
		sndData.body.OpCode    = (char)ope;
		sndData.body.Flag      = 0;
		sndData.body.TLVoffset = 4;
		settrid = htonl((int)trid);
		memcpy(sndData.body.TransactionID,(const char*)&settrid,sizeof(sndData.body.TransactionID));
		sndData.body.PatternData_type   = 3;
		sndData.body.PatternData_len[0] = 0;
		sndData.body.PatternData_len[1] = 128;
		strncpy( sndData.body.PatternData_id,   RSTRING_PTR(ma_value), sizeof(sndData.body.PatternData_id) - 1);
		sndData_p = (VOID*)&sndData;
		sndDataLen = sizeof(sndData);
	}
	else{
		/* vlan eth向け */
		memset( (UCHAR *)&sndData_vlan, 0, sizeof(sndData_vlan) );
		for(count=0;count < 6;count++){
			sndData_vlan.head.dst_mac_addr[count] = (char)dsthwaddr_i[count];
		}
		memcpy( sndData_vlan.head.src_mac_addr, &ifr.ifr_hwaddr.sa_data[0], D_RRH_MAC_ADDR_SIZE );
		sndData_vlan.head.eth_type[0]      = 0x89;
		sndData_vlan.head.eth_type[1]      = 0x02;
		sndData_vlan.body.version   = level << 5;
		sndData_vlan.body.OpCode    = (char)ope;
		sndData_vlan.body.Flag      = 0;
		sndData_vlan.body.TLVoffset = 4;
		settrid = htonl((int)trid);
		memcpy(sndData_vlan.body.TransactionID,(const char*)&settrid,sizeof(sndData_vlan.body.TransactionID));
		sndData_vlan.body.PatternData_type   = 3;
		sndData_vlan.body.PatternData_len[0] = 0;
		sndData_vlan.body.PatternData_len[1] = 128;
		strncpy( sndData_vlan.body.PatternData_id,   RSTRING_PTR(ma_value), sizeof(sndData_vlan.body.PatternData_id) - 1);
		sndData_p = (VOID*)&sndData_vlan;
		sndDataLen = sizeof(sndData_vlan);
	}

	sock = -1;
	if( (sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0 ) {
		rrhlib_dmsg("socket error %d %s\n",sock,strerror(errno));
		return mrb_false_value();
	}
	memset( &ifr, 0, sizeof(struct ifreq) );
	strncpy(ifr.ifr_name, RSTRING_PTR(ethname), IFNAMSIZ-1);
	if( (rtn = ioctl(sock, SIOCGIFINDEX, &ifr)) < 0 ) {
		rrhlib_dmsg("ioctl error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}

	memset( &sll, 0, sizeof(struct sockaddr_ll) );
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(ETH_P_ALL);
	sll.sll_halen = ETHER_ADDR_LEN;

	if( (rtn = bind(sock, (struct sockaddr *)&sll, sizeof(sll))) < 0 ) {
		rrhlib_dmsg("bind error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}

	if((rtn = send(sock, (VOID *)sndData_p, sndDataLen, 0)) <= 0) {
		rrhlib_dmsg("send error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}
	close( sock );
	rrhlib_dmsg("LBM send OK %d \n",rtn);
	return mrb_true_value();

}


/*!
 * @brief		LBM recv処理
 * @note		LBM recv処理
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		ドット表記を値で返す
 * @retval		0		:正常
 * @retval		0以外	:異常
 * @date		2019/6/7 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_lbmrecv(mrb_state* mrb, mrb_value self)
{
	mrb_value						ethname;
	mrb_int							level;
	int								sock;
	int								rtn;
	struct ifreq					ifr;
	struct sockaddr_ll				sll;
	T_LBM_FRAME_RCV					rcvData;

	mrb_get_args(mrb, "Si", &ethname,&level);

	sock = -1;
	if( (sock = socket(PF_PACKET, SOCK_RAW, htons(0x8902))) < 0 ) {
		rrhlib_dmsg("socket error %d %s\n",sock,strerror(errno));
		return mrb_false_value();
	}
	memset( &ifr, 0, sizeof(struct ifreq) );
	strncpy(ifr.ifr_name, RSTRING_PTR(ethname), IFNAMSIZ-1);
	if( (rtn = ioctl(sock, SIOCGIFINDEX, &ifr)) < 0 ) {
		rrhlib_dmsg("ioctl error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}

	memset( &sll, 0, sizeof(struct sockaddr_ll) );
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(0x8902);
	sll.sll_halen = ETHER_ADDR_LEN;

	if( (rtn = bind(sock, (struct sockaddr *)&sll, sizeof(sll))) < 0 ) {
		rrhlib_dmsg("bind error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}

	memset((void*)&rcvData,0,sizeof(rcvData));
	if((rtn = recv(sock, (VOID *)&rcvData, sizeof(rcvData), 0)) <= 0) {
		rrhlib_dmsg("recv error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}
	close( sock );

	rrhlib_dmsg("LBM recv OK %d \n",rtn);
	rrhlib_dmsg("Dst    %02x:%02x:%02x:%02x:%02x:%02x \n",rcvData.head.dst_mac_addr[0],rcvData.head.dst_mac_addr[1],rcvData.head.dst_mac_addr[2],rcvData.head.dst_mac_addr[3],rcvData.head.dst_mac_addr[4],rcvData.head.dst_mac_addr[5]);
	rrhlib_dmsg("Src    %02x:%02x:%02x:%02x:%02x:%02x \n",rcvData.head.src_mac_addr[0],rcvData.head.src_mac_addr[1],rcvData.head.src_mac_addr[2],rcvData.head.src_mac_addr[3],rcvData.head.src_mac_addr[4],rcvData.head.src_mac_addr[5]);
	rrhlib_dmsg("Type   0x%02x%02x \n",rcvData.head.eth_type[0],rcvData.head.eth_type[1]);
	rrhlib_dmsg(" CFM   v  :0x%x op:0x%x F:0x%x Of:%x\n",rcvData.body.version,rcvData.body.OpCode,rcvData.body.Flag ,rcvData.body.TLVoffset);
	rrhlib_dmsg("       id :0x%02x%02x%02x%02x \n",rcvData.body.TransactionID[0],rcvData.body.TransactionID[1],rcvData.body.TransactionID[2],rcvData.body.TransactionID[3]);
	rrhlib_dmsg("       typ:0x%x len:0x%02x%02x\n",rcvData.body.PatternData_type,rcvData.body.PatternData_len[0],rcvData.body.PatternData_len[1] );

	if(level != ((rcvData.body.version >> 5) & 0x7)){
		rrhlib_dmsg("LBM recv level mismatch %d expected %d\n",((rcvData.body.version >> 5) & 0x7),level);
		return mrb_false_value();
	}
	return mrb_true_value();

}


/*!
 * @brief		RAW send処理
 * @note		RAW send処理
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		ドット表記を値で返す
 * @retval		0		:正常
 * @retval		0以外	:異常
 * @date		2019/6/7 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_rawsend(mrb_state* mrb, mrb_value self)
{
	mrb_value						ethname;
	mrb_int							length;
	mrb_value						buffer_s;
	int								sock;
	int								rtn;
	struct ifreq					ifr;
	struct sockaddr_ll				sll;
	long							buffer_addr;

	mrb_get_args(mrb, "SiS", &ethname,&length,&buffer_s);

	rrhlib_dmsg("Raw send start %s len:%d buff:%s \n",RSTRING_PTR(ethname),length,RSTRING_PTR(buffer_s));

	if( sscanf( RSTRING_PTR(buffer_s), "%lx", &buffer_addr) == EOF){
		rrhlib_dmsg("buff error %s\n",RSTRING_PTR(buffer_s));
		return mrb_false_value();
	}

	sock = -1;
	if( (sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0 ) {
		rrhlib_dmsg("socket error %d %s\n",sock,strerror(errno));
		return mrb_false_value();
	}
	memset( &ifr, 0, sizeof(struct ifreq) );
	strncpy(ifr.ifr_name, RSTRING_PTR(ethname), IFNAMSIZ-1);
	if( (rtn = ioctl(sock, SIOCGIFINDEX, &ifr)) < 0 ) {
		rrhlib_dmsg("ioctl error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}

	memset( &sll, 0, sizeof(struct sockaddr_ll) );
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(ETH_P_ALL);
	sll.sll_halen = ETHER_ADDR_LEN;

	if( (rtn = bind(sock, (struct sockaddr *)&sll, sizeof(sll))) < 0 ) {
		rrhlib_dmsg("bind error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}

	if((rtn = send(sock, (VOID *)buffer_addr, length, 0)) <= 0) {
		rrhlib_dmsg("send error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}
	close( sock );
	rrhlib_dmsg("Raw send OK %d \n",rtn);
	return mrb_true_value();

}


/*!
 * @brief		RAW recv処理
 * @note		RAW recv処理
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		ドット表記を値で返す
 * @retval		0		:正常
 * @retval		0以外	:異常
 * @date		2019/6/7 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_rawrecv(mrb_state* mrb, mrb_value self)
{
	mrb_value						ethname;
	mrb_int							length;
	mrb_value						buffer_s;
	int								sock;
	int								rtn;
	struct ifreq					ifr;
	struct sockaddr_ll				sll;
	long							buffer_addr;

	mrb_get_args(mrb, "SiS", &ethname,&length,&buffer_s);

	rrhlib_dmsg("Raw recv start %s len:%d buff:%s \n",RSTRING_PTR(ethname),length,RSTRING_PTR(buffer_s));

	if( sscanf( RSTRING_PTR(buffer_s), "%lx", &buffer_addr) == EOF){
		rrhlib_dmsg("buff error %s\n",RSTRING_PTR(buffer_s));
		return mrb_false_value();
	}

	sock = -1;
	if( (sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0 ) {
		rrhlib_dmsg("socket error %d %s\n",sock,strerror(errno));
		return mrb_false_value();
	}
	memset( &ifr, 0, sizeof(struct ifreq) );
	strncpy(ifr.ifr_name, RSTRING_PTR(ethname), IFNAMSIZ-1);
	if( (rtn = ioctl(sock, SIOCGIFINDEX, &ifr)) < 0 ) {
		rrhlib_dmsg("ioctl error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}

	memset( &sll, 0, sizeof(struct sockaddr_ll) );
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(ETH_P_ALL);
	sll.sll_halen = ETHER_ADDR_LEN;

	if( (rtn = bind(sock, (struct sockaddr *)&sll, sizeof(sll))) < 0 ) {
		rrhlib_dmsg("bind error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}

	memset((void*)buffer_addr,0,length);
	if((rtn = recv(sock, (VOID *)buffer_addr, length, 0)) <= 0) {
		rrhlib_dmsg("recv error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}
	close( sock );
	rrhlib_dmsg("Raw redv OK %d \n",rtn);
	return mrb_true_value();

}

/*!
 * @brief		RAW send処理
 * @note		RAW send処理
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		ドット表記を値で返す
 * @retval		0		:正常
 * @retval		0以外	:異常
 * @date		2019/6/7 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_rawsend_f(mrb_state* mrb, mrb_value self)
{
	mrb_value						ethname;
	mrb_int							send_size;
	mrb_value						buffer_file;
	int								sock;
	int								rtn;
	struct ifreq					ifr;
	struct sockaddr_ll				sll;
	char							buffer[4096];
	char							linebuff[256];
	FILE*							fp;
	int								value[16];
	int								ofs;
	unsigned int 					count;
	int								ngflg;
	size_t							buffer_size;

	mrb_get_args(mrb, "SiS", &ethname,&send_size,&buffer_file);

	rrhlib_dmsg("Raw file send start %s buff:%s \n",RSTRING_PTR(ethname),RSTRING_PTR(buffer_file));

	if( ( fp = fopen(RSTRING_PTR(buffer_file),"r")) == NULL ){
		rrhlib_dmsg("file open error %s \n",RSTRING_PTR(buffer_file));
		return mrb_false_value();
	}

	memset(buffer,0,sizeof(buffer));
	ngflg = 0;
	buffer_size = 0;
	while ( fgets(linebuff,sizeof(linebuff), fp) != NULL ) {
		memset(value,0,sizeof(value));
		if((rtn = sscanf( linebuff,"%x  %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",
				&ofs,&value[0],&value[1],&value[2],&value[3],&value[4],&value[5],&value[6],&value[7],
					 &value[8],&value[9],&value[10],&value[11],&value[12],&value[13],&value[14],&value[15]))
				 != EOF)
		{
			for(count=0; count < 16 ;count++){
				buffer[buffer_size] = (char)value[count];
				buffer_size++;
				if(buffer_size >= send_size){
					ngflg = -1;
					break;
				}
			}
			if(ngflg != 0){
				break;
			}
		}
	}
	fclose(fp);

	sock = -1;
	if( (sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0 ) {
		rrhlib_dmsg("socket error %d %s\n",sock,strerror(errno));
		return mrb_false_value();
	}
	memset( &ifr, 0, sizeof(struct ifreq) );
	strncpy(ifr.ifr_name, RSTRING_PTR(ethname), IFNAMSIZ-1);
	if( (rtn = ioctl(sock, SIOCGIFINDEX, &ifr)) < 0 ) {
		rrhlib_dmsg("ioctl error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}

	memset( &sll, 0, sizeof(struct sockaddr_ll) );
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(ETH_P_ALL);
	sll.sll_halen = ETHER_ADDR_LEN;

	if( (rtn = bind(sock, (struct sockaddr *)&sll, sizeof(sll))) < 0 ) {
		rrhlib_dmsg("bind error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}

	if((rtn = send(sock, (VOID *)buffer, buffer_size, 0)) <= 0) {
		rrhlib_dmsg("send error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}

	close( sock );
	rrhlib_dmsg("Raw file send OK %d %d \n",rtn,buffer_size);
	return mrb_true_value();

}

/*!
 * @brief		RAW recv処理
 * @note		RAW recv処理
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		ドット表記を値で返す
 * @retval		0		:正常
 * @retval		0以外	:異常
 * @date		2019/6/7 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_rawrecv_f(mrb_state* mrb, mrb_value self)
{
	mrb_value						ethname;
	mrb_int							eth_type;
	mrb_value						buffer_file;
	int								sock;
	int								rtn;
	struct ifreq					ifr;
	struct sockaddr_ll				sll;
	char							buffer[4096];
	size_t							recv_size;
	FILE*							fp;
	unsigned int 					count;
	unsigned int 					index;

	mrb_get_args(mrb, "SiS", &ethname,&eth_type,&buffer_file);

	rrhlib_dmsg("Raw recv start %s type:0x%x buff:%s \n",RSTRING_PTR(ethname),eth_type,RSTRING_PTR(buffer_file));

	sock = -1;
	if( (sock = socket(PF_PACKET, SOCK_RAW, htons((ushort)eth_type))) < 0 ) {
		rrhlib_dmsg("socket error %d %s\n",sock,strerror(errno));
		return mrb_false_value();
	}
	memset( &ifr, 0, sizeof(struct ifreq) );
	strncpy(ifr.ifr_name, RSTRING_PTR(ethname), IFNAMSIZ-1);
	if( (rtn = ioctl(sock, SIOCGIFINDEX, &ifr)) < 0 ) {
		rrhlib_dmsg("ioctl error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}

	memset( &sll, 0, sizeof(struct sockaddr_ll) );
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons((ushort)eth_type);
	sll.sll_halen = ETHER_ADDR_LEN;

	if( (rtn = bind(sock, (struct sockaddr *)&sll, sizeof(sll))) < 0 ) {
		rrhlib_dmsg("bind error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}

	memset(buffer,0,sizeof(buffer));
	if((recv_size = recv(sock, (VOID *)buffer, sizeof(buffer), 0)) <= 0) {
		rrhlib_dmsg("recv error %d %s\n",rtn,strerror(errno));
		close( sock );
		return mrb_false_value();
	}
	
	close( sock );

	if( ( fp = fopen(RSTRING_PTR(buffer_file),"w")) == NULL ){
		rrhlib_dmsg("file open error %s \n",RSTRING_PTR(buffer_file));
		return mrb_false_value();
	}

	for(count=0 ,index=0; count < (recv_size/16) ; count++,index+=16){
		fprintf(fp,"%04X  %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
			index,
			buffer[ count*16],
			buffer[(count*16) + 1],
			buffer[(count*16) + 2],
			buffer[(count*16) + 3],
			buffer[(count*16) + 4],
			buffer[(count*16) + 5],
			buffer[(count*16) + 6],
			buffer[(count*16) + 7],
			buffer[(count*16) + 8],
			buffer[(count*16) + 9],
			buffer[(count*16) + 10],
			buffer[(count*16) + 11],
			buffer[(count*16) + 12],
			buffer[(count*16) + 13],
			buffer[(count*16) + 14],
			buffer[(count*16) + 15]
		);
	}
	if((recv_size%16) > 0){
		fprintf(fp,"%04X  ",index);
		for(count=0 ; count < (recv_size%16) ; count++,index++){
			fprintf(fp,"%02X ",buffer[index]);
		}
		fprintf(fp,"\n");
	}
	fflush(fp);
	fclose(fp);
	rrhlib_dmsg("Raw file recv OK %d \n",recv_size);
	return mrb_true_value();

}


/*!
 * @brief		ソケット用モジュール登録処理
 * @note
<pre>
	スクリプト上で使用するソケットモジュールを登録する関数である.
</pre>
 * @param		mrb				[in]	mrb status
 * @param		module_class	[in]	module_class
 * @return		N/A
 * @date		2013/11/25 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
void rrhlib_soc_install(mrb_state* mrb ,struct RClass * module_class)
{
	rrhlib_dmsg("rrh lib soc install start \n");
	
	/* scriptで使用できるモジュールを登録する */
	/* tcp */
	mrb_define_module_function(mrb, module_class, "tsocket",  rrhlib_tsocket         , MRB_ARGS_NONE());
	mrb_define_module_function(mrb, module_class, "tbind",    rrhlib_bind            , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "tlisten",  rrhlib_tlisten         , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "taccept",  rrhlib_taccept         , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "tconnect", rrhlib_tconnect        , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "tsend",    rrhlib_tsend           , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "tsend2",   rrhlib_tsend2          , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "trecv",    rrhlib_trecv           , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "trecv2",    rrhlib_trecv2         , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "tclose",   rrhlib_close          , MRB_ARGS_REQ(1));
	/* udp */
	mrb_define_module_function(mrb, module_class, "usocket",  rrhlib_usocket         , MRB_ARGS_NONE());
	mrb_define_module_function(mrb, module_class, "ubind",    rrhlib_bind           , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "usend",    rrhlib_usend           , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "urecv",    rrhlib_urecv           , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "uclose",   rrhlib_close           , MRB_ARGS_REQ(1));

	/* basic */
	mrb_define_module_function(mrb, module_class, "recv",       rrhlib_recv          , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "recvfrom",   rrhlib_recvfrom      , MRB_ARGS_REQ(4));
	mrb_define_module_function(mrb, module_class, "send",       rrhlib_send          , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "sendto",     rrhlib_sendto        , MRB_ARGS_REQ(4));
	mrb_define_module_function(mrb, module_class, "select",     rrhlib_select        , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "nselect",    rrhlib_nselect       , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "ioctl",      rrhlib_ioctl         , MRB_ARGS_REQ(3));

	/* tftp */
	mrb_define_module_function(mrb, module_class, "tftpget",    rrhlib_tftpget        , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "tftpset",    rrhlib_tftpset        , MRB_ARGS_REQ(2));

	/* http */
	mrb_define_module_function(mrb, module_class, "httpget",    rrhlib_httpget        , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "httpset",    rrhlib_httpset        , MRB_ARGS_REQ(2));


	mrb_define_module_function(mrb, module_class, "ntoa",    rrhlib_ntoa              , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "aton",    rrhlib_aton              , MRB_ARGS_REQ(2));

	mrb_define_module_function(mrb, module_class, "lbmsend",    rrhlib_lbmsend              , MRB_ARGS_REQ(8));
	mrb_define_module_function(mrb, module_class, "lbmrecv",    rrhlib_lbmrecv              , MRB_ARGS_REQ(2));


	mrb_define_module_function(mrb, module_class, "rawsend",    rrhlib_rawsend              , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "rawrecv",    rrhlib_rawrecv              , MRB_ARGS_REQ(3));

	mrb_define_module_function(mrb, module_class, "rawsend_f",    rrhlib_rawsend_f          , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "rawrecv_f",     rrhlib_rawrecv_f          , MRB_ARGS_REQ(3));

	rrhlib_dmsg("rrh lib soc install OK \n");
	return;
}
/* @} */
