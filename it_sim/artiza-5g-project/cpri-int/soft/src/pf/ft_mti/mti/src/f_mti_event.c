/*!
 * @skip  $ld:$
 * @file  f_mti_main.c
 * @brief pf_mti メイン処理
 * @date  2019/05/13 Fujitsu)Taniguchi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_MTI
 * @{
 */

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<errno.h>
#include	"f_mti_inc.h"
#include	"f_mti_typ.h"									/*!< 定数定義			*/
#include	"f_mnt_msg_interfaces_def.h"					/*!< MT定義				*/
#include	"f_mnt_msg_interfaces_typ.h"					/*!< MT定義				*/
#include	"rrhApi_Rfp.h"
#include	"f_mpsw_msg_o-ran_software_management_def.h"
#include	"f_mpsw_msg_o-ran_software_management_typ.h"

enum{

	E_MTI_LED1_OFF = 0,
	E_MTI_LED1_RED_ON,
	E_MTI_LED1_GREEN_ON,
	E_MTI_LED1_RED_BLINK,
	E_MTI_LED1_GREEN_BLINK,
	E_MTI_LED1_RED_GREEN_BLINK,

	E_MTI_LED2_OFF = 0,
	E_MTI_LED2_ORANGE_ON,
	E_MTI_LED2_GREEN_ON,
	E_MTI_LED2_ORANGE_BLINK,
	E_MTI_LED2_GREEN_BLINK,
	E_MTI_LED2_ORANGE_GREEN_BLINK,

	E_MTI_OPE_ENB = 0,
	E_MTI_OPE_DIS,

	E_MTI_ABL_NML = 0,
	E_MTI_ABL_MALM,
	E_MTI_ABL_CALM,

	E_MTI_ADM_UNLOCK = 0,
	E_MTI_ADM_LOCK,

	E_MTI_USG_IDLE = 0,
	E_MTI_USG_ACT,
	E_MTI_USG_BSY,

	E_MTI_PWR_AWAKE = 0,
	E_MTI_PWR_SLEEP,

	E_MTI_FAN_NML = 0,
	E_MTI_FAN_ALM_H,
	E_MTI_FAN_ALM_L,

	E_MTI_PTIN_OFF = 0,
	E_MTI_PTIN_ON,
	
	E_MTI_PTP_FREE = 0,
	E_MTI_PTP_LOCK,
	E_MTI_PTP_HOLD,

	E_MTI_LINK_UP = 0,
	E_MTI_LINK_DOWN,
	
	E_MTI_SFP_NML = 0,
	E_MTI_SFP_ALM,

	E_MTI_RESET_NML = 0,
	E_MTI_RESET_CHG


};

enum{
	E_MTI_OK          =    0,
	E_MTI_NG_CONN     = 0x01,
	E_MTI_NG_PARAM    = 0x11,
	E_MTI_NG_FUNC     = 0x12,
	E_MTI_NG_UNMNT    = 0x13,
	E_MTI_NG_KIND     = 0x14,
	E_MTI_NG_TIMEOUT  = 0x15,
	E_MTI_NG_PTERROR  = 0x16,
	E_MTI_NG_RW       = 0x17,
	E_MTI_NG_FSIZE    = 0x1A,
	E_MTI_NG_CHKSUM   = 0x1B,
	E_MTI_NG_SEQNO    = 0x1C,
	E_MTI_NG_STATE    = 0x1D,
	E_MTI_NG_DL       = 0x1E
};

typedef struct {
	UINT		num;
	UINT		size;
}T_MTI_MSG_INFO;


typedef struct {
	UINT				msgId;
	UINT				paraNum;
	T_MTI_MSG_INFO		msgInfo[32];
}T_MTI_MSG_ENDIAN;


static const T_MTI_MSG_ENDIAN f_mtir_msg_endian_tbl[] = {
	{ D_MSG_BODY_OPT_CONNECT_STT_REQ         , 4  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 4 }, { 1 , 4 }} },
	{ D_MSG_BODY_OPT_CONNECT_STT_RSP         , 2  , {{ 1 , 4 }, { 1 , 4 }}                       },
	{ D_MSG_BODY_OPT_CONNECT_STP_REQ         , 1  , {{ 1 , 4 }}                                  },
	{ D_MSG_BODY_OPT_CONNECT_STP_RSP         , 2  , {{ 1 , 4 }, { 1 , 4 }}                       },
	{ D_MSG_BODY_EEPROM_REG_READ_REQ         , 2  , {{ 1 , 4 }, { 1 , 4 }}                       },
	{ D_MSG_BODY_EEPROM_REG_READ_RSP         , 12 , {
										  			{ 1 , 4 }, { 1 , 4 },{ 1 , 4 }, { 1 , 4 },{ 1 , 4 }, { 12 , 1 },{ 2 ,1 } ,{ 2 , 1 },
										  			{ 1 , 4 }, { 1 , 2 },{ 32, 2 }, { 1536 , 1 }
										  		  }},
	{ D_MSG_BODY_DUSTATUS_GET_REQ            , 2  , {{ 1 , 4 }, { 1 , 4 }}                       },
	{ D_MSG_BODY_DUSTATUS_GET_RSP            , 29  ,{
													{ 1 , 4 }, { 1 , 4 },{ 1 , 4 }, { 1 , 4 },{ 1  , 1 }, { 1  , 1 },{ 1  ,1 } ,{ 1 , 1 },
													{ 1 , 1 }, { 1 , 1 },{ 1 , 1 }, { 1 , 1 },{ 1  , 1 }, { 1 ,  1 },{ 32 ,2 } ,{ 1 , 1 },
													{ 1 , 1 }, { 1 , 1 },{ 1 , 1 }, { 16 ,1 },{ 16 , 1 }, { 16 , 1 },{ 16 ,1 } ,{ 4 , 4 },
													{ 4 , 4 }, { 4 , 4 },{ 1 , 1 }, { 1 , 1 },{ 1  , 4 },
												   }},
	{ D_MSG_BODY_FORCEALM_REQ                , 2  , {{ 1 , 4 }, { 1 , 4 }}                       },
	{ D_MSG_BODY_FORCEALM_RSP                , 4  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 4 }, { 1 , 4 }} },
	{ D_MSG_BODY_CARDBLK_REQ                 , 3  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 4 }}            },
	{ D_MSG_BODY_CARDBLK_RSP                 , 4  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 4 }, { 1 , 4 }} },
	{ D_MSG_BODY_CARDRST_REQ                 , 3  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 4 }}            },
	{ D_MSG_BODY_CARDRST_RSP                 , 4  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 4 }, { 1 , 4 }} },
	{ D_MSG_BODY_PAONOFF_REQ                 , 4  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 4 }, { 1 , 4 }} },
	{ D_MSG_BODY_PAONOFF_RSP                 , 4  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 4 }, { 1 , 4 }} },
	{ D_MSG_BODY_NETCONF_AUTH_RESET_REQ      , 2  , {{ 1 , 4 }, { 1 , 4 }}                       },
	{ D_MSG_BODY_NETCONF_AUTH_RESET_RSP      , 4  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 4 }, { 1 , 4 }} },
	{ D_MSG_BODY_POWADJ_WRITE_REQ            , 3  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 4 }}            },
	{ D_MSG_BODY_POWADJ_WRITE_RSP            , 4  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 4 }, { 1 , 4 }} },
	{ D_MSG_BODY_BEAMID_SET_REQ              , 3  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 4 }}            },
	{ D_MSG_BODY_BEAMID_SET_RSP              , 4  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 4 }, { 1 , 4 }} },
	{ D_MSG_BODY_SW_INSTALL_REQ              , 4  , {{ 1 , 4 }, { 1 , 4 }, { 32 ,1 }, { 1 , 4 }} },
	{ D_MSG_BODY_SW_INSTALL_RSP              , 4  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 4 }, { 1 , 4 }} },
	{ D_MSG_BODY_EEPWRITE_REQ                , 5  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 1 }, { 1 , 1 }, { 16 , 1 }} },
	{ D_MSG_BODY_EEPWRITE_RSP                , 4  , {{ 1 , 4 }, { 1 , 4 }, { 1 , 4 }, { 1 , 4 }, { 1 , 2 }} },
	{ 0                                      , 1  , {{ 0 , 0 }}                                  }

};


typedef struct {
	USHORT				faultId;
	UINT				faultBit;
	UCHAR				ucFaultName[48];
}T_MTI_MSG_FAULT;

static const T_MTI_MSG_FAULT f_mtir_msg_fault_tbl[] = {
										  /* 0123456789ABCDEF012456789ABCDEF */
	{ 0x0001, D_DU_SVCTL_BIT_OCCUR_FID_01 , "Software fault                 " },
	{ 0x0002, D_DU_SVCTL_BIT_OCCUR_FID_02 , "Initialization failure         " },
	{ 0x0003, D_DU_SVCTL_BIT_OCCUR_FID_03 , "Fronthaul Synchronization Error" },
	{ 0x0004, D_DU_SVCTL_BIT_OCCUR_FID_04 , "Module Clock faulty            " },
	{ 0x0005, D_DU_SVCTL_BIT_OCCUR_FID_05 , "Fronthaul faulty               " },
	{ 0x0006, D_DU_SVCTL_BIT_OCCUR_FID_06 , "PS ALM                         " },
	{ 0x0007, D_DU_SVCTL_BIT_OCCUR_FID_07 , "Unit out of order              " },
	{ 0x0008, D_DU_SVCTL_BIT_OCCUR_FID_08 , "RF module fault                " },
	{ 0x0010, D_DU_SVCTL_BIT_OCCUR_FID_10 , "TX out of order                " },
	{ 0x0011, D_DU_SVCTL_BIT_OCCUR_FID_11 , "RX out of order                " },
	{ 0x0012, D_DU_SVCTL_BIT_OCCUR_FID_12 , "ANT port abnormal              " },
	{ 0x0030, D_DU_SVCTL_BIT_OCCUR_FID_30 , "Temperature too low            " },
	{ 0x0031, D_DU_SVCTL_BIT_OCCUR_FID_31 , "Unit dangerously overheating   " },
	{ 0x0040, D_DU_SVCTL_BIT_OCCUR_FID_40 , "No FAN detected                " },
	{ 0x0041, D_DU_SVCTL_BIT_OCCUR_FID_41 , "FAN ps alarm                   " },
	{ 0x0042, D_DU_SVCTL_BIT_OCCUR_FID_42 , "Cooling FAN broken FAN1        " },
	{ 0x0043, D_DU_SVCTL_BIT_OCCUR_FID_43 , "Cooling FAN broken FAN2        " },
	{ 0x0044, D_DU_SVCTL_BIT_OCCUR_FID_44 , "Cooling FAN broken FAN3        " },
	{ 0x0050, D_DU_SVCTL_BIT_OCCUR_FID_50 , "SFP Error                      " },
	{ 0x0051, D_DU_SVCTL_BIT_OCCUR_FID_51 , "SFP Fail Safe                  " },
	{ 0x00F0, D_DU_SVCTL_BIT_OCCUR_FID_F0 , "Tx Sleep                       " },
	{ 0x00F1, D_DU_SVCTL_BIT_OCCUR_FID_F1 , "Tx OFF                         " },
	{ 0x00F2, D_DU_SVCTL_BIT_OCCUR_FID_F2 , "TEST Mode                      " },
	{ 0x00FF, D_DU_SVCTL_BIT_OCCUR_FID_FF , "Forced alarm                   " }
};




static UINT f_mti_blk_state;
static unsigned long int f_mti_blk_threadId = 0;

typedef struct {
	UINT				slotno;
	UCHAR				filename[64];
	UCHAR				sdataBuf[16*1024];
}T_MTI_SWM_INFO;


static unsigned long int f_mti_swm_threadId = 0;
static T_MTI_SWM_INFO f_mti_swm_sdata;

static unsigned long int f_mti_beam_threadId = 0;
static T_MSG_BODY_BEAMID_SET_REQ	f_mti_beam_data;

static UINT f_mti_last_msgId = 0;

/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
VOID f_mti_event_header( UCHAR* rdataBuf , UINT msgId ,UCHAR* sdataBuf , ssize_t slen)
{
	T_MSGHEAD*		req;
	T_MSGHEAD*		rsp;

	req = (T_MSGHEAD*)rdataBuf;
	rsp = (T_MSGHEAD*)sdataBuf;

	rsp->msgid     = msgId;
	rsp->srcFbNo   = req->dstFbNo;
	rsp->dstFbNo   = req->srcFbNo;
	rsp->replyId   = req->replyId;
	rsp->allMsgLen = slen ;
	rsp->headSeqNo = req->headSeqNo;
	rsp->next      = 0;
	rsp->msgLen    = 0;
	rsp->nowSeqNo  = req->nowSeqNo;

	return;
}

/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
UINT f_mti_msg_nwbyteorder(UINT mode, ssize_t len, UCHAR* msg )
{
	
	T_MSGHEAD*		msghead;
	UINT			msgId = 0;
	UINT			count;
	UINT			count2;
	UINT			count3;
	UCHAR*			tmp_msg;
	UINT			num;
	UINT			size;

	msghead = (T_MSGHEAD*)msg;

	if(mode == D_SYS_ON){
		msghead->msgid     = ntohl(msghead->msgid);
		msghead->srcFbNo   = ntohl(msghead->srcFbNo);
		msghead->dstFbNo   = ntohl(msghead->dstFbNo);
		msghead->allMsgLen = ntohl(msghead->allMsgLen);
		msghead->replyId   = htonl(msghead->replyId);
		msghead->headSeqNo = ntohl(msghead->headSeqNo);
		msghead->next      = ntohl(msghead->next);
		msghead->msgLen    = ntohl(msghead->msgLen);
		msghead->nowSeqNo  = ntohl(msghead->nowSeqNo);
	}

	msgId = msghead->msgid;
	tmp_msg = msg + sizeof(T_MSGHEAD);
	for( count = 0 ; ; count++ ){
		if( f_mtir_msg_endian_tbl[count].msgId == 0){
			break;
		}
		if( f_mtir_msg_endian_tbl[count].msgId != msgId){
			continue;
		}
		for(count2 = 0; count2 < f_mtir_msg_endian_tbl[count].paraNum ; count2++){
			num = f_mtir_msg_endian_tbl[count].msgInfo[count2].num;
			size = f_mtir_msg_endian_tbl[count].msgInfo[count2].size;
			for(count3 = 0 ; count3 < num ; count3++){
				if(mode == D_SYS_ON){
					switch(size){
						case 4:
							*(UINT*)tmp_msg = ntohl(*(UINT*)tmp_msg);
							break;
						case 2:
							*(USHORT*)tmp_msg = ntohs(*(USHORT*)tmp_msg);
							break;
						default:
							break;
					}
				}else{
					switch(size){
						case 4:
							*(UINT*)tmp_msg = htonl(*(UINT*)tmp_msg);
							break;
						case 2:
							*(USHORT*)tmp_msg = htons(*(USHORT*)tmp_msg);
							break;
						default:
							break;
					}
				}
				tmp_msg += size;
			}
		}
		break;
	}
	
	if(mode == D_SYS_OFF){
		msghead->msgid     = htonl(msghead->msgid);
		msghead->srcFbNo   = htonl(msghead->srcFbNo);
		msghead->dstFbNo   = htonl(msghead->dstFbNo);
		msghead->allMsgLen = htonl(msghead->allMsgLen);
		msghead->replyId   = htonl(msghead->replyId);
		msghead->headSeqNo = htonl(msghead->headSeqNo);
		msghead->next      = htons(msghead->next);
		msghead->msgLen    = htons(msghead->msgLen);
		msghead->nowSeqNo  = htonl(msghead->nowSeqNo);
	}
	return msgId;
}



/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
INT f_mti_event_OPT_CONNECT_STT( UCHAR* rdataBuf , UCHAR* sdataBuf , ssize_t* slen)
{
	f_mti_mtLog("[R] OPT_CONNECT_STT_REQ");

	T_MSG_BODY_OPT_CONNECT_STT_RSP*	rsp;
	
	rsp = (T_MSG_BODY_OPT_CONNECT_STT_RSP*)sdataBuf;

	memset(rsp,0,sizeof(*rsp));

	rsp->data.uiOK_NG   = D_SYS_OK;
	rsp->data.uiNG_Code = E_MTI_OK;

	*slen = sizeof(*rsp);

	f_mti_event_header( rdataBuf, D_MSG_BODY_OPT_CONNECT_STT_RSP , sdataBuf ,sizeof(rsp->data) );

	f_mtiw_InfoTbl->comInfo.mt_con_state = D_SYS_ON;

	f_mti_mtLog("[S] OPT_CONNECT_STT_RSP(%ld)",*slen);

	return D_SYS_OK;
}


/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
INT f_mti_event_OPT_CONNECT_STP( UCHAR* rdataBuf , UCHAR* sdataBuf , ssize_t* slen)
{
	f_mti_mtLog("[R] OPT_CONNECT_STP_REQ");

	T_MSG_BODY_OPT_CONNECT_STP_RSP*	rsp;
	
	rsp = (T_MSG_BODY_OPT_CONNECT_STP_RSP*)sdataBuf;

	memset(rsp,0,sizeof(*rsp));

	rsp->data.uiOK_NG   = D_SYS_OK;
	rsp->data.uiNG_Code = E_MTI_OK;

	*slen = sizeof(*rsp);

	f_mti_event_header( rdataBuf, D_MSG_BODY_OPT_CONNECT_STP_RSP , sdataBuf ,sizeof(rsp->data) );

	f_mti_mtLog("[S] OPT_CONNECT_STP_RSP(%ld)",*slen);

	return D_SYS_NG;
}

/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
INT f_mti_event_EEPROM_REG_READ( UCHAR* rdataBuf , UCHAR* sdataBuf , ssize_t* slen)
{
	T_MSG_BODY_EEPROM_REG_READ_REQ*	req;
	T_MSG_BODY_EEPROM_REG_READ_RSP*	rsp;
	UINT	count;

	req = (T_MSG_BODY_EEPROM_REG_READ_REQ*)rdataBuf;
	rsp = (T_MSG_BODY_EEPROM_REG_READ_RSP*)sdataBuf;

	memset(rsp,0,sizeof(*rsp));

	f_mti_mtLog("[R] EEPROM_REG_READ_REQ F:0x%x S:0x%x",req->data.uiFuncID,req->data.uiSeqNo);
	
	rsp->data.uiFuncID   = req->data.uiFuncID;
	rsp->data.uiSeqNo    = req->data.uiSeqNo;
	rsp->data.uiOK_NG    = D_SYS_OK;
	rsp->data.uiOption   = f_mtiw_optdef_val;

	rsp->data.uiProductType = f_mtiw_productType;
	memcpy(rsp->data.ucProductName,f_mtiw_productName,sizeof(rsp->data.ucProductName));
	memcpy(rsp->data.ucSwVersion  ,f_mtiw_softVer,sizeof(rsp->data.ucSwVersion));
	memcpy(rsp->data.ucHwVersion  ,f_mtiw_hardVer,sizeof(rsp->data.ucHwVersion));

	rsp->data.uiBeamFormEnable = D_SYS_ON;	/* TODO */
	rsp->data.usFaultTotalNum = E_DU_SVCTL_OCCUR_FIDNO_MAX;

	for(count=0; count < rsp->data.usFaultTotalNum ;count++){
		rsp->data.ucFaultIndexNum[count] = f_mtir_msg_fault_tbl[count].faultId;
		strncpy((char*)&(rsp->data.ucFaultName[count][0]),(const char *)(f_mtir_msg_fault_tbl[count].ucFaultName),sizeof(rsp->data.ucFaultName[count]));
	}

	*slen = sizeof(*rsp);
	f_mti_event_header( rdataBuf, D_MSG_BODY_EEPROM_REG_READ_RSP , sdataBuf ,sizeof(rsp->data) );

	f_mti_mtLog("[S] EEPROM_REG_READ_RSP(%ld) T:%d N:%s SW:0x%02X%02X HW:0x%02X%02X",
				*slen,rsp->data.uiProductType,rsp->data.ucProductName,rsp->data.ucSwVersion[0],rsp->data.ucSwVersion[1],rsp->data.ucHwVersion[0],rsp->data.ucHwVersion[1]);

	return D_SYS_OK;
}

/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
INT	f_mti_event_DUSTATUS_GET( UCHAR* rdataBuf , UCHAR* sdataBuf , ssize_t* slen)
{
	T_MSG_BODY_DUSTATUS_GET_REQ*	req;
	T_MSG_BODY_DUSTATUS_GET_RSP*	rsp;

	req = (T_MSG_BODY_DUSTATUS_GET_REQ*)rdataBuf;
	rsp = (T_MSG_BODY_DUSTATUS_GET_RSP*)sdataBuf;

	memset(rsp,0,sizeof(*rsp));

	UINT	led;
	UINT	state[D_SYS_NUM16];
	UINT	portIn;
	INT		errcd;
	INT		rtn_bpf;

	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SYSLED, &led);
	BPF_HM_DEVC_REG_READ_BUFFER(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV1INF,sizeof(state),state);
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_OTH_GPIO_DATA_2_RO, &portIn);


	if(f_mti_last_msgId != D_MSG_BODY_DUSTATUS_GET_REQ){
		f_mti_mtLog("[R] DUSTATUS_GET_REQ F:0x%x S:0x%x",req->data.uiFuncID,req->data.uiSeqNo);
	}

	rsp->data.uiFuncID   = req->data.uiFuncID;
	rsp->data.uiSeqNo    = req->data.uiSeqNo;
	rsp->data.uiOK_NG    = D_SYS_OK;
	rsp->data.uiNG_Code  = E_MTI_OK;

	/* LED1 */
	/* レジスタ				*/
	/*	000:消灯			*/
	/*	001:赤点灯			*/
	/*	010:緑点灯			*/
	/*	011:赤点灯&緑点灯(設定禁止)	*/
	/*	100:消灯			*/
	/*	101:赤点滅			*/
	/*	110:緑点滅			*/
	/*	111:赤/緑交互点灯	*/
	
	/* MT					*/
	/*	0:消灯				*/
	/*	1:赤点灯			*/
	/*	2:緑点灯			*/
	/*	3:赤点滅			*/
	/*	4:緑点滅			*/
	/*	5:赤/緑交互点灯		*/

	switch(led & 0x7){
		case 0:
			rsp->data.ucLeD1  = (UCHAR)E_MTI_LED1_OFF;	
			break;
		case 1:
			rsp->data.ucLeD1  = (UCHAR)E_MTI_LED1_RED_ON;
			break;
		case 2:
			rsp->data.ucLeD1  = (UCHAR)E_MTI_LED1_GREEN_ON;
			break;
		case 5:
			rsp->data.ucLeD1  = (UCHAR)E_MTI_LED1_RED_BLINK;
			break;
		case 6:
			rsp->data.ucLeD1  = (UCHAR)E_MTI_LED1_GREEN_BLINK;
			break;
		case 7:
			rsp->data.ucLeD1  = (UCHAR)E_MTI_LED1_RED_GREEN_BLINK;
			break;
		default:
			rsp->data.ucLeD1  = (UCHAR)E_MTI_LED1_RED_ON;
			break;
	}

	/*	000:消灯					*/
	/*	001:橙点灯					*/
	/*	010:緑点灯					*/
	/*	011:橙点灯&緑点灯(設定禁止)	*/
	/*	100:消灯					*/
	/*	101:橙点滅					*/
	/*	110:緑点滅					*/
	/*	111:橙/緑交互点灯			*/


	/*	0:消灯						*/
	/*	1:橙点灯					*/
	/*	2:緑点灯					*/
	/*	3:橙点滅					*/
	/*	4:緑点滅					*/
	/*	5:橙/緑交互点灯				*/

	led = (led >> 4) & 0x7;
	switch(led){
		case 0:
			rsp->data.ucLeD2  = (UCHAR)E_MTI_LED2_OFF;
			break;
		case 1:
			rsp->data.ucLeD2  = (UCHAR)E_MTI_LED2_ORANGE_ON;
			break;
		case 2:
			rsp->data.ucLeD2  = (UCHAR)E_MTI_LED2_GREEN_ON;
			break;
		case 5:
			rsp->data.ucLeD2  = (UCHAR)E_MTI_LED2_ORANGE_BLINK;
			break;
		case 6:
			rsp->data.ucLeD2  = (UCHAR)E_MTI_LED2_GREEN_BLINK;
			break;
		case 7:
			rsp->data.ucLeD2  = (UCHAR)E_MTI_LED2_ORANGE_GREEN_BLINK;
			break;
		default:
			rsp->data.ucLeD2  = (UCHAR)E_MTI_LED2_OFF;
			break;
	}

	UINT	wk_state;
	UINT	wk_state2;

	/* opr-state */
	wk_state = (UINT)((state[0] & 0x300) >> 8);
	if((wk_state & 2) != 0){
		rsp->data.ucOperState  = (UCHAR)E_MTI_OPE_DIS;
	}else{
		rsp->data.ucOperState  = (UCHAR)E_MTI_OPE_ENB;
	}

	/* ab-state */
	wk_state = (UINT)((state[0] & 0x7000) >> 12);
	if((wk_state & 1) != 0){
		rsp->data.ucAblblState = (UCHAR)E_MTI_ABL_CALM;
	}else if((wk_state & 2) != 0){
		rsp->data.ucAblblState = (UCHAR)E_MTI_ABL_MALM;
	}else{
		rsp->data.ucAblblState = (UCHAR)E_MTI_ABL_NML;
	}

	/* admin-state */
	wk_state = (UINT)(state[0] & 0x7);
	wk_state2 = (UINT)((state[0] & 0x7000000) >> 24);
	if((wk_state & 1) != 0){
			rsp->data.ucAdminState = (UCHAR)E_MTI_ADM_LOCK;
	}else{
		if(( wk_state2 & 1 ) != 0){
			rsp->data.ucAdminState = (UCHAR)E_MTI_ADM_LOCK;
		}else{
			rsp->data.ucAdminState = (UCHAR)E_MTI_ADM_UNLOCK;
		}
	}

	/* usage-state */
	wk_state = (UINT)((state[0] & 0x70000) >> 16);
	if((wk_state & 1) != 0){
		rsp->data.ucUseState = (UCHAR)E_MTI_USG_BSY;
	}else if((wk_state & 2) != 0){
		rsp->data.ucUseState = (UCHAR)E_MTI_USG_ACT;
	}else{
		rsp->data.ucUseState = (UCHAR)E_MTI_USG_IDLE;
	}

	/* en-state */
	wk_state = (UINT)((state[0] & 0x30) >> 4);
	if((wk_state & 1) != 0){
		rsp->data.ucPowerState = E_MTI_PWR_SLEEP;
	}else{
		rsp->data.ucPowerState = E_MTI_PWR_AWAKE;
	}

	if((state[12] & 0x05510000) != 0){
		if((state[12] & 0x00110000) != 0){
			/* FAN未実装,FAN電源異常 */
			rsp->data.ucFanState = E_MTI_FAN_ALM_H;
		}else if((state[12] & 0x05400000) == 0x05400000){
			/* 全FAN回転低下 */
			rsp->data.ucFanState = E_MTI_FAN_ALM_H;
		}else{
			/* 一部FAN回転低下 */
			rsp->data.ucFanState = E_MTI_FAN_ALM_L;
		}
	}else{
		rsp->data.ucFanState = E_MTI_FAN_NML;
	}

	/* PORT入力 "0":入力ON(ショート)，"1":入力OFF(オープン) */
	if((portIn & 0x400) != 0){
		rsp->data.ucPortState = E_MTI_PTIN_OFF;	/* 入力off */
	}else{
		rsp->data.ucPortState = E_MTI_PTIN_ON;	/* 入力on */
	}

	if(state[3] != 0){
		UINT	count;
		for(count=0; count < E_DU_SVCTL_OCCUR_FIDNO_MAX ;count++){
			if(( state[3] & f_mtir_msg_fault_tbl[count].faultBit) != 0){
				rsp->data.ucFaultFactor[rsp->data.ucFaultFactorNum] = f_mtir_msg_fault_tbl[count].faultId;
				rsp->data.ucFaultFactorNum++;
			}
		}
	}

	UINT	ptpState;
	ptpState = (UINT)((state[15] & 0x70000000) >> 28);
	if(ptpState == 1){
		rsp->data.ucPtpState = E_MTI_PTP_FREE;
	}else if(ptpState == 2){
		rsp->data.ucPtpState = E_MTI_PTP_LOCK;
	}else{
		rsp->data.ucPtpState = E_MTI_PTP_HOLD;
	}

	if((state[5] & 0x100) != 0){
		rsp->data.ucLinkState = E_MTI_LINK_DOWN;
	}else{
		rsp->data.ucLinkState = E_MTI_LINK_UP;
	}

	if((state[5] & 0x400) != 0){
		rsp->data.ucSfpState = E_MTI_SFP_ALM;
	}else{
		rsp->data.ucSfpState = E_MTI_SFP_NML;
	}

	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_MT_INFO, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, NULL, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MT CTL sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
		/* NGでも無視 */
	}

	strncpy((char*)(rsp->data.ucEF_type)  ,(const char *)(f_mtiw_InfoTbl->sfpInfo.sfpType)   ,sizeof(rsp->data.ucEF_type));
	strncpy((char*)(rsp->data.ucVendor)   ,(const char *)(f_mtiw_InfoTbl->sfpInfo.vendorCode),sizeof(rsp->data.ucVendor));
	strncpy((char*)(rsp->data.ucSerialNo) ,(const char *)(f_mtiw_InfoTbl->sfpInfo.serialNo)  ,sizeof(rsp->data.ucSerialNo));

	rsp->data.ucOption = (UCHAR)(f_mtiw_opt_val & 0x000000FF);	/* Option	*/

	UINT	antno;
	UINT	carno;
	UCHAR	carflg[4];

	memset(carflg,0,sizeof(carflg));
	for(antno =0 ; antno < 4; antno++){
		for(carno =0 ; carno < 4; carno++){
			rsp->data.ucCarrierState[antno][carno] = f_mtiw_InfoTbl->carInfo_tx.carrierState[antno][carno] | f_mtiw_InfoTbl->carInfo_rx.carrierState[antno][carno];
			carflg[carno] |= rsp->data.ucCarrierState[antno][carno];
		}
	}

	for(carno =0 ; carno < 4; carno++){
		if(carflg[carno] == D_RRH_ON){
			rsp->data.uiFreqSnd[carno]    = f_mtiw_InfoTbl->carInfo_tx.FreqNo[carno];
			rsp->data.uiFreqRcv[carno]    = f_mtiw_InfoTbl->carInfo_rx.FreqNo[carno];
			rsp->data.uiBandwidth[carno]  = 100000;
		}
	}

	rsp->data.ucBeamId = f_mtiw_InfoTbl->comInfo.beamId;

	if(f_cmw_tra_inv_tbl->cmx_txgain > 10){
		rsp->data.ucSendOutVal = 0;
	}else{
		rsp->data.ucSendOutVal = f_cmw_tra_inv_tbl->cmx_txgain;
	}

	rsp->data.uiConnectionState = f_mtiw_InfoTbl->comInfo.cu_con_state;

	if(rtn_bpf == BPF_RU_IPCM_OK){
		rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_MT_INFO, BPF_RU_IPCM_LOCK_RW, &errcd);
		if(BPF_RU_IPCM_OK != rtn_bpf){
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MT CTL sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
		}
	}

	*slen = sizeof(*rsp);
	f_mti_event_header( rdataBuf, D_MSG_BODY_DUSTATUS_GET_RSP , sdataBuf ,sizeof(rsp->data) );

	if(f_mti_last_msgId != D_MSG_BODY_DUSTATUS_GET_REQ){
		f_mti_mtLog("[S] DUSTATUS_GET_RSP(%ld) L1:%d L2:%d O:%d Ab:%d Ad:%d U:%d P:%d F:%d PT:%d FL:%d PP:%d LS:%d SF:%d BF:%d G:%d C:%d",
		*slen,
		rsp->data.ucLeD1,
		rsp->data.ucLeD2,
		rsp->data.ucOperState,
		rsp->data.ucAblblState,
		rsp->data.ucAdminState,
		rsp->data.ucUseState,
		rsp->data.ucPowerState,
		rsp->data.ucFanState,
		rsp->data.ucPortState,
		rsp->data.ucFaultFactorNum,
		rsp->data.ucPtpState,
		rsp->data.ucLinkState,
		rsp->data.ucSfpState,
		rsp->data.ucBeamId,
		rsp->data.ucSendOutVal,
		rsp->data.uiConnectionState);
	}
	return D_SYS_OK;
}

/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
INT	f_mti_event_FORCEALM( UCHAR* rdataBuf , UCHAR* sdataBuf , ssize_t* slen)
{

	T_MSG_BODY_FORCEALM_REQ*	req;
	T_MSG_BODY_FORCEALM_RSP*	rsp;

	req = (T_MSG_BODY_FORCEALM_REQ*)rdataBuf;
	rsp = (T_MSG_BODY_FORCEALM_RSP*)sdataBuf;

	memset(rsp,0,sizeof(*rsp));

	f_mti_mtLog("[R] FORCEALM_REQ F:0x%x S:0x%x",req->data.uiFuncID,req->data.uiSeqNo);

	rsp->data.uiFuncID   = req->data.uiFuncID;
	rsp->data.uiSeqNo    = req->data.uiSeqNo;
	rsp->data.uiOK_NG    = D_SYS_OK;
	rsp->data.uiNG_Code  = E_MTI_OK;

	f_com_SVCtl(E_DU_SVCTL_OPE_FIDFF_ALM, D_DU_SVCTL_ON, D_DU_FLTID_FORCE_ALM);

	*slen = sizeof(*rsp);
	f_mti_event_header( rdataBuf, D_MSG_BODY_FORCEALM_RSP , sdataBuf ,sizeof(rsp->data) );

	f_mti_mtLog("[S] FORCEALM_RSP(%ld)",*slen);

	return D_SYS_OK;
}

/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
static VOID f_mti_blk_start(void* state)
{
	INT		errcd;
	INT		rtn_bpf;
	UINT	db_need;
	CHAR	value_buff[1024];
	CHAR	admin_state_name[32];
	INT		rtn;
	UINT	mt_blk_ctl = *(UINT*)state;

	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_MT_INFO, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, NULL, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MT CTL sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
		f_mti_blk_threadId = 0;
		return;
	}

	db_need = f_mtiw_InfoTbl->comInfo.cu_con_state;

	rtn_bpf =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_MT_INFO, BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"MT CTL sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Local BLK by MT (%d->%d) db:0x%x",f_mtiw_InfoTbl->comInfo.blk_ctl,mt_blk_ctl,db_need);
	f_mti_mtLog("[E] Local BLK by MT (%d->%d) db:0x%x tid:%ld",f_mtiw_InfoTbl->comInfo.blk_ctl,mt_blk_ctl,db_need,f_mti_blk_threadId);

	f_mtiw_InfoTbl->comInfo.blk_ctl = mt_blk_ctl;

	if(db_need == D_RRH_ON){
		UINT	reg_admin_state;

		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV1INF, &reg_admin_state);

		UINT wk_state = (UINT)(reg_admin_state & 0x7);
		UINT wk_state2 = (UINT)((reg_admin_state & 0x7000000) >> 24);

		if(
			(( (wk_state & 0x1 ) != 0 ) || ((wk_state2 & 0x1 ) != 0 )) &&
			( mt_blk_ctl == E_MTI_ADM_LOCK )
		){
			/* LOCK => LOCK */
			f_mti_mtLog("[E] Skip DB update(LOCK state cur:0x%08x ctl:0x%08x)",reg_admin_state,mt_blk_ctl);
		}
		else if(
			(( (wk_state & 0x1 ) == 0 ) && ((wk_state2 & 0x1 ) == 0 )) &&
			( mt_blk_ctl == E_MTI_ADM_UNLOCK )
		){
			/* UNLOCK => UNLOCK */
			f_mti_mtLog("[E] Skip DB update(UNLOCK state cur:0x%08x ctl:0x%08x)",reg_admin_state,mt_blk_ctl);
		}
		else{
			UINT	dump_bit;

			BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SWWORK_DPMODE, 0x00000001, &dump_bit);
			if(dump_bit != 0){
				BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_SWWORK_DPMODE, dump_bit );
			}
			
			/* DBAPI */
			memset(value_buff,0,sizeof(value_buff));
			memset(admin_state_name,0,sizeof(admin_state_name));
	
			if(mt_blk_ctl == E_MTI_ADM_LOCK){
				strncpy(admin_state_name,"locked",strlen("locked"));
			} else{
				strncpy(admin_state_name,"unlocked",strlen("unlocked"));
			}
	
			/* send-data作成 */
			snprintf(value_buff, sizeof(value_buff),
				"/opt/agent/bin/DU_dbapi "
				"subsys_local_blk "
				"'<config>"
					"<hardware xmlns=\"urn:ietf:params:xml:ns:yang:ietf-hardware\">"
						"<component>"
							"<name>%s</name>"
								"<state>"
									"<admin-state>%s</admin-state>"
								"</state>"
						"</component>"
					"</hardware>"
				"</config>'"
				" --log-level=off 1>/dev/null 2>/dev/null",
				f_mtiw_productName,
				admin_state_name);

			rtn = system(value_buff);
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "DB update %d(Admin state %s(0x%08x) reg:0x%08x",rtn,admin_state_name,mt_blk_ctl,reg_admin_state);		/* pgr0539 */
			f_mti_mtLog("[E] DB update %d(Admin state %s(0x%08x) reg:0x%08x",rtn,admin_state_name,mt_blk_ctl,reg_admin_state);

			if(dump_bit != 0){
				BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_SWWORK_DPMODE, dump_bit );
			}

		}
	}

	if(mt_blk_ctl == E_MTI_ADM_LOCK){
		f_com_SVCtl( E_DU_SVCTL_OPE_ADM , D_DU_SVCTL_CHG , D_DU_SVCTL_ON_ADM_LOCKED_LO );
	}else{
		f_com_SVCtl( E_DU_SVCTL_OPE_ADM , D_DU_SVCTL_CHG , D_DU_SVCTL_ON_ADM_UNLOCKED_LO );
	}

	f_mti_blk_threadId = 0;

	return;
}


/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
INT	f_mti_event_CARDBLK( UCHAR* rdataBuf , UCHAR* sdataBuf , ssize_t* slen)
{

	T_MSG_BODY_CARDBLK_REQ*	req;
	T_MSG_BODY_CARDBLK_RSP*	rsp;
	INT		errcd;
	INT		rtn_bpf;

	req = (T_MSG_BODY_CARDBLK_REQ*)rdataBuf;
	rsp = (T_MSG_BODY_CARDBLK_RSP*)sdataBuf;

	memset(rsp,0,sizeof(*rsp));

	f_mti_mtLog("[R] CARDBLK_REQ F:0x%x S:0x%x",req->data.uiFuncID,req->data.uiSeqNo);

	rsp->data.uiFuncID   = req->data.uiFuncID;
	rsp->data.uiSeqNo    = req->data.uiSeqNo;
	*slen = sizeof(*rsp);

	if(f_mti_blk_threadId == 0){
		f_mti_blk_state = req->data.uiState;
		rtn_bpf = BPF_RU_ITCM_THREAD_CREATE(
						&f_mti_blk_threadId,
						BPF_RU_ITCM_SCHED_OTHER,
						0,
						D_RRH_STACKSIZE_DEFAULT,
						f_mti_blk_start,
						&f_mti_blk_state,
						&errcd);

		if(rtn_bpf == BPF_RU_ITCM_OK){
			rsp->data.uiOK_NG    = D_SYS_OK;
			rsp->data.uiNG_Code  = E_MTI_OK;
			f_mti_mtLog("[S] CARDBLK_RSP(%ld) OK thid:%ld",*slen,f_mti_blk_threadId);

		}else{
			rsp->data.uiOK_NG    = D_SYS_NG;
			rsp->data.uiNG_Code  = E_MTI_NG_FUNC;
			f_mti_blk_threadId = 0;
			f_mti_mtLog("[S] CARDBLK_RSP(%ld) NG1");
		}
	}else{
		rsp->data.uiOK_NG    = D_SYS_NG;
		rsp->data.uiNG_Code  = E_MTI_NG_STATE;
		f_mti_mtLog("[S] CARDBLK_RSP NG2(%ld) thid:%ld",*slen,f_mti_blk_threadId);
	}

	f_mti_event_header( rdataBuf, D_MSG_BODY_CARDBLK_RSP , sdataBuf ,sizeof(rsp->data) );
	return D_SYS_OK;
}

/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
INT	f_mti_event_CARDRST( UCHAR* rdataBuf , UCHAR* sdataBuf , ssize_t* slen)
{

	T_MSG_BODY_CARDRST_REQ*	req;
	T_MSG_BODY_CARDRST_RSP*	rsp;
	UCHAR					eep_data = 0;
	UCHAR					sts_data = 0;
	INT						ret = -2;

	req = (T_MSG_BODY_CARDRST_REQ*)rdataBuf;
	rsp = (T_MSG_BODY_CARDRST_RSP*)sdataBuf;

	memset(rsp,0,sizeof(*rsp));

	f_mti_mtLog("[R] CARDRST_REQ F:0x%x S:0x%x",req->data.uiFuncID,req->data.uiSeqNo);

	rsp->data.uiFuncID   = req->data.uiFuncID;
	rsp->data.uiSeqNo    = req->data.uiSeqNo;
	rsp->data.uiOK_NG    = D_SYS_OK;
	rsp->data.uiNG_Code  = E_MTI_OK;
	*slen = sizeof(*rsp);

	if(req->data.uiExchange == E_MTI_RESET_CHG){
		if ( f_cmw_tra_inv_tbl->cmx_bootup_face == D_RRH_BOOT_ACT0 ){
			eep_data = D_RRH_BOOT_ACT1;
		}else{
			eep_data = D_RRH_BOOT_ACT0;
		}

		BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_SWSLOT_STATUS + eep_data, &sts_data );
		if( sts_data == D_DU_EEP_STATUS_VALID ){
			BPF_HM_DEVC_EEPROM_WRITE( D_DU_EEP_START_FLG, &eep_data );
		}else{
			rsp->data.uiOK_NG = D_SYS_NG;
			rsp->data.uiNG_Code = E_MTI_NG_STATE;

			ret = D_SYS_OK;
		}

	}else{
		eep_data = f_cmw_tra_inv_tbl->cmx_bootup_face;
	}

	if(rsp->data.uiOK_NG == D_SYS_OK){
		f_mti_mtLog("[S] CARDRST_RSP OK(%ld) exChg:%d lastFace:%d SltSts:%d",*slen , req->data.uiExchange , eep_data ,sts_data);
	}else{
		f_mti_mtLog("[S] CARDRST_RSP NG(%ld) State INVALID exChg:%d lastFace:%d SltSts:%d",*slen , req->data.uiExchange , eep_data,sts_data);
	}

	f_mti_event_header( rdataBuf, D_MSG_BODY_CARDRST_RSP , sdataBuf ,sizeof(rsp->data) );

	return ret;
}

/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
INT	f_mti_event_PAONOFF( UCHAR* rdataBuf , UCHAR* sdataBuf , ssize_t* slen)
{

	T_MSG_BODY_PAONOFF_REQ*	req;
	T_MSG_BODY_PAONOFF_RSP*	rsp;
	
	req = (T_MSG_BODY_PAONOFF_REQ*)rdataBuf;
	rsp = (T_MSG_BODY_PAONOFF_RSP*)sdataBuf;

	memset(rsp,0,sizeof(*rsp));

	f_mti_mtLog("[R] PAONOFF_REQ F:0x%x S:0x%x",req->data.uiFuncID,req->data.uiSeqNo);

	rsp->data.uiFuncID   = req->data.uiFuncID;
	rsp->data.uiSeqNo    = req->data.uiSeqNo;
	rsp->data.uiOK_NG    = D_SYS_OK;
	rsp->data.uiNG_Code  = E_MTI_OK;
	
	if(req->data.uiOnOff == E_MTI_PWR_SLEEP){
		f_com_SVCtl( E_DU_SVCTL_OPE_FIDF1_ALM , D_DU_SVCTL_ON , D_DU_FLTID_TX_OFF );
	}else{
		f_com_SVCtl( E_DU_SVCTL_OPE_FIDF1_ALM , D_DU_SVCTL_OFF , D_DU_FLTID_TX_OFF );
	}

	*slen = sizeof(*rsp);
	f_mti_event_header( rdataBuf, D_MSG_BODY_PAONOFF_RSP , sdataBuf ,sizeof(rsp->data) );

	f_mti_mtLog("[S] PAONOFF_RSP(%ld) paOnOff:%d",*slen,req->data.uiOnOff);

	return D_SYS_OK;
}

/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
INT	f_mti_event_NETCONF_AUTH_RESET( UCHAR* rdataBuf , UCHAR* sdataBuf , ssize_t* slen)
{
	T_MSG_BODY_NETCONF_AUTH_RESET_REQ*	req;
	T_MSG_BODY_NETCONF_AUTH_RESET_RSP*	rsp;
	UCHAR					eep_data;
	
	req = (T_MSG_BODY_NETCONF_AUTH_RESET_REQ*)rdataBuf;
	rsp = (T_MSG_BODY_NETCONF_AUTH_RESET_RSP*)sdataBuf;

	memset(rsp,0,sizeof(*rsp));

	f_mti_mtLog("[R] NETCONF_AUTH_RESET_REQ F:0x%x S:0x%x",req->data.uiFuncID,req->data.uiSeqNo);

	rsp->data.uiFuncID   = req->data.uiFuncID;
	rsp->data.uiSeqNo    = req->data.uiSeqNo;
	rsp->data.uiOK_NG    = D_SYS_OK;
	rsp->data.uiNG_Code  = E_MTI_OK;

	eep_data = 0xFF;
	BPF_HM_DEVC_EEPROM_WRITE( D_DU_EEP_FLASH_WRITE_FLAG_SSH, &eep_data );

	*slen = sizeof(*rsp);
	f_mti_event_header( rdataBuf, D_MSG_BODY_NETCONF_AUTH_RESET_RSP , sdataBuf ,sizeof(rsp->data) );

	f_mti_mtLog("[S] NETCONF_AUTH_RESET_RSP(%ld) 0x%x",*slen,eep_data);

	return D_SYS_OK;
}

/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
INT	f_mti_event_POWADJ_WRITE( UCHAR* rdataBuf , UCHAR* sdataBuf , ssize_t* slen)
{
	T_MSG_BODY_POWADJ_WRITE_REQ*	req;
	T_MSG_BODY_POWADJ_WRITE_RSP*	rsp;
	T_API_RFP_POW_ADJ_REQ*			sndMsg;
	INT								endcd= 0;
	INT								errcd= 0;

	req = (T_MSG_BODY_POWADJ_WRITE_REQ*)rdataBuf;
	rsp = (T_MSG_BODY_POWADJ_WRITE_RSP*)sdataBuf;

	memset(rsp,0,sizeof(*rsp));

	f_mti_mtLog("[R] POWADJ_WRITE_REQ F:0x%x S:0x%x",req->data.uiFuncID,req->data.uiSeqNo);

	rsp->data.uiFuncID   = req->data.uiFuncID;
	rsp->data.uiSeqNo    = req->data.uiSeqNo;
	rsp->data.uiOK_NG    = D_SYS_NG;
	rsp->data.uiNG_Code  = E_MTI_OK;
	*slen = sizeof(*rsp);

	if(req->data.uiSetVal <= 10){
		endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK,sizeof(T_API_RFP_POW_ADJ_REQ),(VOID **)&sndMsg,&errcd );
		if( endcd == BPF_RU_IPCM_OK ){
			sndMsg->head.uiEventNo		= D_API_RFP_MNT_POWER_ADJUST_REQ;				/* メッセージID						*/
			sndMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;								/* 返信先PQID						*/
			sndMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_CCA;							/* 送信先TQID						*/
			sndMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;								/* 送信元PQID						*/
			sndMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_MTI;							/* 送信元TQID						*/
			sndMsg->powAdjInfo.branch  = 0xFF;
			sndMsg->powAdjInfo.command = (USHORT)req->data.uiSetVal;
			endcd = BPF_RU_IPCM_MSGQ_SEND( D_SYS_THDQID_PF_CCA,(VOID *)sndMsg );
			if( endcd != BPF_RU_IPCM_OK ){
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " MSGQ_SEND NG %d ", endcd );
				BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)sndMsg);
				f_mti_mtLog("[S] POWADJ_WRITE_RSP(%ld) NG1 val:%d",*slen,req->data.uiSetVal);
				rsp->data.uiNG_Code  = E_MTI_NG_FUNC;
			}else{
				rsp->data.uiOK_NG    = D_SYS_OK;
				f_mti_mtLog("[S] POWADJ_WRITE_RSP(%ld) OK val:%d",*slen,req->data.uiSetVal);
			}
		} else{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " ADDRGET NG %d %d ", endcd ,errcd);
			f_mti_mtLog("[S] POWADJ_WRITE_RSP(%ld) NG2 val:%d",*slen,req->data.uiSetVal);
				rsp->data.uiNG_Code  = E_MTI_NG_FUNC;
		}
	}else{
		f_mti_mtLog("[S] POWADJ_WRITE_RSP(%ld) NG3 val:%d",*slen,req->data.uiSetVal);
		rsp->data.uiNG_Code  = E_MTI_NG_PARAM;
	}

	f_mti_event_header( rdataBuf, D_MSG_BODY_POWADJ_WRITE_RSP , sdataBuf ,sizeof(rsp->data) );


	return D_SYS_OK;
}


/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
static VOID f_mti_beam_start(void* buff)
{
	T_MSG_BODY_BEAMID_SET_REQ*	req;
	T_MSG_BODY_BEAMID_SET_RSP	rsp;
	T_SYS_MT_BEAMID_SET_REQ*	thdif_req;
	T_SYS_MT_BEAMID_SET_RSP*	thdif_rsp;
	INT							rtn_bpf;
	INT							errcd;
	UINT						rcvmsgsz;
	INT							ret = D_SYS_OK;

	req = (T_MSG_BODY_BEAMID_SET_REQ*)buff;

	f_mti_mtLog("[E] start send T_SYS_MT_BEAMID_SET_REQ BeamId:%d thdid:%ld",req->data.uiBeamId,f_mti_beam_threadId);

	rtn_bpf = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,
						sizeof( T_SYS_MT_BEAMID_SET_REQ ),
						(VOID **)&thdif_req,
						&errcd );

	if(rtn_bpf != BPF_RU_IPCM_OK){
		f_mti_mtLog("[E] BEAMID_SET_RSP error %d %d",rtn_bpf,errcd);
		ret = D_SYS_NG;	
	}

	if(ret == D_SYS_OK){
		thdif_req->thdIf.uiEventNo		= D_SYS_MSGID_BEAMID_SET_REQ;
		thdif_req->thdIf.uiDstPQueueID	= D_RRH_PROCQUE_PF;
		thdif_req->thdIf.uiDstTQueueID	= D_SYS_THDQID_PF_CCA;
		thdif_req->thdIf.uiSrcPQueueID	= D_RRH_PROCQUE_PF;
		thdif_req->thdIf.uiSrcTQueueID	= D_SYS_THDQID_PF_MTI;
		thdif_req->thdIf.uiLength		= sizeof(T_SYS_MT_BEAMID_SET_REQ);
		thdif_req->uiBeamId             = req->data.uiBeamId;
	
		rtn_bpf = BPF_RU_IPCM_MSGQ_SEND( D_SYS_THDQID_PF_CCA, (void *)thdif_req );
		if( rtn_bpf != BPF_RU_IPCM_OK ) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed send message. %d", rtn_bpf);
			ret = D_SYS_NG;
		}
	}
	
	if(ret == D_SYS_OK){
		while(1){
			thdif_rsp = NULL;
			rtn_bpf = BPF_RU_IPCM_MSGQ_RECV( D_SYS_THDQID_PF_MTI,0,(void **)&thdif_rsp,&rcvmsgsz );
			if(rtn_bpf != BPF_RU_IPCM_OK){
				f_mti_mtLog("[E] T_SYS_MT_BEAMID_SET_RSP error %d",rtn_bpf);
			}else{
				switch(thdif_rsp->thdIf.uiEventNo){
					case D_SYS_MSGID_BEAMID_SET_RSP:
						if(thdif_rsp->uiOK_NG == D_SYS_OK){
							f_mti_mtLog("[E] recv T_SYS_MT_BEAMID_SET_RSP OK",thdif_rsp->uiBeamId,thdif_rsp->uiOK_NG,thdif_rsp->uiNG_Code);
						}else{
							f_mti_mtLog("[E] recv T_SYS_MT_BEAMID_SET_RSP NG",thdif_rsp->uiBeamId,thdif_rsp->uiOK_NG,thdif_rsp->uiNG_Code);
							ret = D_SYS_NG;
						}
						break;
					default:
						f_mti_mtLog("[E] recv Error MSG recv id:0x%08x que:0x%08x",thdif_rsp->thdIf.uiEventNo,thdif_rsp->thdIf.uiSrcTQueueID);
						ret = D_SYS_NG;
						break;
				}
				(void)BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)thdif_rsp );
				break;
			}
		}
	}

	memset(&rsp,0,sizeof(rsp));
	f_mti_event_header( (UCHAR*)req, D_MSG_BODY_BEAMID_SET_RSP , (UCHAR*)&rsp ,sizeof(rsp.data) );
	rsp.data.uiFuncID  = req->data.uiFuncID;
	rsp.data.uiSeqNo   = req->data.uiSeqNo;

	if(ret == D_SYS_OK){
		rsp.data.uiOK_NG   = D_SYS_OK;
		rsp.data.uiNG_Code = E_MTI_OK;
		f_mti_mtLog("[E] BEAMID_SET_RSP(0x%x) OK func:%d seq:%d result:%ld beamid:%d cur:%d",rsp.msgHead.msgid,rsp.data.uiFuncID,rsp.data.uiSeqNo,rsp.data.uiOK_NG,req->data.uiBeamId,f_mtiw_InfoTbl->comInfo.beamId);
		f_mtiw_InfoTbl->comInfo.beamId = req->data.uiBeamId;
	}else{
		rsp.data.uiOK_NG   = D_SYS_NG;
		rsp.data.uiNG_Code = E_MTI_NG_FUNC;		
		f_mti_mtLog("[E] BEAMID_SET_RSP(0x%x) NG func:%d seq:%d result:%ld beamid:%d cur:%d",rsp.msgHead.msgid,rsp.data.uiFuncID,rsp.data.uiSeqNo,rsp.data.uiOK_NG,req->data.uiBeamId,f_mtiw_InfoTbl->comInfo.beamId);
	}
	f_mti_beam_threadId = 0;
	f_mti_msg_nwbyteorder(D_SYS_OFF,sizeof(T_MSG_BODY_BEAMID_SET_RSP),(UCHAR*)&rsp);
	f_mti_send((UCHAR*)&rsp,sizeof(T_MSG_BODY_BEAMID_SET_RSP));
	return;
}


/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
INT	f_mti_event_BEAMID_SET( UCHAR* rdataBuf , UCHAR* sdataBuf , ssize_t* slen)
{
	T_MSG_BODY_BEAMID_SET_REQ*	req;
	T_MSG_BODY_BEAMID_SET_RSP*	rsp;
	INT							rtn_bpf;
	INT							errcd;
	
	
	req = (T_MSG_BODY_BEAMID_SET_REQ*)rdataBuf;
	rsp = (T_MSG_BODY_BEAMID_SET_RSP*)sdataBuf;

	memset(rsp,0,sizeof(*rsp));

	f_mti_mtLog("[R] BEAMID_SET_REQ F:0x%x S:0x%x",req->data.uiFuncID,req->data.uiSeqNo);

	rsp->data.uiFuncID   = req->data.uiFuncID;
	rsp->data.uiSeqNo    = req->data.uiSeqNo;
	rsp->data.uiOK_NG    = D_SYS_OK;
	rsp->data.uiNG_Code  = E_MTI_OK;
	*slen = sizeof(*rsp);

	if(req->data.uiBeamId <= f_mtiw_beamIdMax){
		if(f_mti_beam_threadId != 0){
			rsp->data.uiOK_NG    = D_SYS_NG;
			rsp->data.uiNG_Code  = E_MTI_NG_STATE;
			f_mti_mtLog("[S] BEAMID_SET_RSP(%ld) NG BeamId:%d thdid:%ld",*slen,req->data.uiBeamId,f_mti_beam_threadId);
		}else{
			memcpy((void*)&f_mti_beam_data,(void*)req,sizeof(f_mti_beam_data));
		
			rtn_bpf = BPF_RU_ITCM_THREAD_CREATE(
					&f_mti_beam_threadId,
					BPF_RU_ITCM_SCHED_OTHER,
					0,
					D_RRH_STACKSIZE_DEFAULT,
					f_mti_beam_start,
					(VOID*)&f_mti_beam_data,
					&errcd);
			if(rtn_bpf == BPF_RU_ITCM_OK){
				/* OKルート、応答は起動スレッドにお任せ */
				f_mti_mtLog("[S] BEAMID_SET_RSP(%ld) start BeamId:%d",*slen,req->data.uiBeamId);
				return D_SYS_MNG;
			}else{
				rsp->data.uiOK_NG    = D_SYS_NG;
				rsp->data.uiNG_Code  = E_MTI_NG_FUNC;
				f_mti_mtLog("[S] BEAMID_SET_RSP(%ld) NG BeamId:%d %d thdid:%ld",*slen,req->data.uiBeamId,rtn_bpf,f_mti_beam_threadId);
				f_mti_beam_threadId = 0;
			}
		}
		
	}else{
		rsp->data.uiOK_NG    = D_SYS_NG;
		rsp->data.uiNG_Code    = E_MTI_NG_PARAM;
		f_mti_mtLog("[S] BEAMID_SET_RSP(%ld) NG BeamId:%d",*slen,req->data.uiBeamId);
	}

	f_mti_event_header( rdataBuf, D_MSG_BODY_BEAMID_SET_RSP , sdataBuf ,sizeof(rsp->data) );
	return D_SYS_OK;
}

/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
static VOID f_mti_swm_start(void* buff)
{
	UINT					rcvSize;
	VOID*					sndMsg_p;
	VOID*					rcvMsg_p;
	INT						endcd;
	INT						errcd;
	T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ*			msi_req;
	CHAR					cmd[512];
	INT						cmpflg = 0;
	T_MSG_BODY_SW_INSTALL_RSP*	rsp;
	UINT					messageId;
	UINT					requestId;
	INT						payloadSizePtr;
	VOID *					payloadPtr;
	INT						ret;


	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"unzip -o /tmp/%s -d /tmp >/dev/null 2>/dev/null", ((T_MTI_SWM_INFO*)buff)->filename ) ;
	ret = system(cmd);
	f_mti_mtLog("[E] unzip %s %d",((T_MTI_SWM_INFO*)buff)->filename,ret);	/* pgr0539 */

	endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,
						sizeof( T_RRH_HEAD ) + sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ),
						(VOID **)&sndMsg_p,
						&errcd );

	if(endcd != BPF_RU_IPCM_OK){
		f_mti_mtLog("[E] SW_INSTALL Buffer error %d",errcd);
		f_mti_swm_threadId = 0;
		return;
	}

	((T_RRH_HEAD*)sndMsg_p)->uiEventNo		= D_SYS_MSGID_MSI_RCV_IND;
	((T_RRH_HEAD*)sndMsg_p)->uiDstPQueueID	= D_RRH_PROCQUE_PF;
	((T_RRH_HEAD*)sndMsg_p)->uiDstTQueueID	= D_SYS_THDQID_PF_FDL;
	((T_RRH_HEAD*)sndMsg_p)->uiSrcPQueueID	= D_RRH_PROCQUE_PF;
	((T_RRH_HEAD*)sndMsg_p)->uiSrcTQueueID	= D_SYS_THDQID_PF_MTI;

	msi_req = (T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ*)(sndMsg_p + sizeof(T_RRH_HEAD));
	msi_req->msgHead.msgId = D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ;
	msi_req->msgHead.requestId = 1;
	msi_req->msgHead.msgLength = sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ);
	snprintf(msi_req->data.slot_name,sizeof(msi_req->data.slot_name),"slot%d",((T_MTI_SWM_INFO*)buff)->slotno);
	msi_req->data.file_num = 1;
	strncpy(&(msi_req->data.file_name[0][0]),(const char*)(((T_MTI_SWM_INFO*)buff)->filename),strlen((const char*)(((T_MTI_SWM_INFO*)buff)->filename)));

	/* 念のためパケット受信をOFF */
	BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_ETH_RXCTL, D_RRH_ON);

	/* DWL load start */
	f_com_SVCtl( E_DU_SVCTL_OPE_FLDL , D_DU_SVCTL_ON , D_DU_SVCTL_ON_FL_DWNLD ) ;

	rsp = (T_MSG_BODY_SW_INSTALL_RSP*)(((T_MTI_SWM_INFO*)buff)->sdataBuf);

	f_mti_mtLog("[E] SOFTWARE_MANAGEMENT_INSTALL_REQ send slot%d filename:%s",((T_MTI_SWM_INFO*)buff)->slotno,((T_MTI_SWM_INFO*)buff)->filename );
	endcd = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_FDL , (VOID *)sndMsg_p );
	if( endcd != D_RRH_OK ){
		BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)sndMsg_p);
		f_mti_mtLog("[E] SW_INSTALL send error %d",errcd);
		rsp->data.uiOK_NG = D_SYS_NG;
	}else{
		while(1){
			ret = BPF_RU_IPCM_MSGQ_RECV(D_SYS_THDQID_PF_MPSEND,0,(void**)&rcvMsg_p,&rcvSize);
			if(BPF_RU_IPCM_OK != ret){
				continue;
			}
			switch(((T_RRH_HEAD*)rcvMsg_p)->uiEventNo){
				case D_SYS_MSGID_MSI_SND_IND:
					f_com_MSIgetPayload(rcvMsg_p,&messageId,&requestId,&payloadSizePtr,&payloadPtr );
					switch(messageId){
						case D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_CFM:
							cmpflg++;
							f_mti_mtLog("[E] SOFTWARE_MANAGEMENT_INSTALL_CFM recv");
							break;
						case D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ:
							cmpflg = -1;
							f_mti_mtLog("[E] SOFTWARE_MANAGEMENT_INSTALL_REJ recv");
							break;
						case D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND:
							cmpflg++;
							f_mti_mtLog("[E] SOFTWARE_MANAGEMENT_INSTALL_NTF_IND recv");
							break;
						default:
							cmpflg = -1;
							f_mti_mtLog("[E] unknown 0x%08x recv",messageId);
							break;
					}
					break;
				default:
					f_mti_mtLog("[E] unknown evnt:0x%08x recv",((T_RRH_HEAD*)rcvMsg_p)->uiEventNo);
					cmpflg = -1;
					break;
			}
			BPF_RU_IPCM_PROCMSG_ADDRFREE((VOID*)rcvMsg_p);
			if(cmpflg < 0){
				rsp->data.uiOK_NG = D_SYS_NG;
				break;
			}else if(cmpflg >= 2){
				break;
			}else{
				;
			}
		}
	}

	/* ファイル削除 */
	 ret = system("rm -rf /tmp/*.zip 1>/null 2>/null");
	ret |= system("rm -rf /tmp/*.FGF 1>/null 2>/null");		/* pgr0539 */
	ret |= system("rm -rf /tmp/Fj5gDuCm* 1>/null 2>/null");
	ret |= system("rm -rf /tmp/*.bin 1>/null 2>/null");
	ret |= system("rm -rf /tmp/*.bit 1>/null 2>/null");
	ret |= system("rm -rf /tmp/installpkg 1>/null 2>/null");
	ret |= system("rm -rf /tmp/manifest.xml 1>/null 2>/null");
	ret |= system("rm -rf /tmp/sum_result 1>/null 2>/null");

	f_mti_mtLog("[E] SW_INSTALL_RSP(0x%x) func:%d seq:%d result:%ld ret:%d",rsp->msgHead.msgid,rsp->data.uiFuncID,rsp->data.uiSeqNo,rsp->data.uiOK_NG,ret);

	/* パケット受信をON */
	BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_ETH_RXCTL, D_RRH_ON);

	f_com_SVCtl( E_DU_SVCTL_OPE_FLDL , D_DU_SVCTL_OFF , D_DU_SVCTL_ON_FL_DWNLD ) ;

	f_mti_msg_nwbyteorder(D_SYS_OFF,sizeof(T_MSG_BODY_SW_INSTALL_RSP),((T_MTI_SWM_INFO*)buff)->sdataBuf);
	f_mti_send(((T_MTI_SWM_INFO*)buff)->sdataBuf,sizeof(T_MSG_BODY_SW_INSTALL_RSP));
	f_mti_swm_threadId = 0;

	return;
}

/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
INT	f_mti_event_SW_INSTALL( UCHAR* rdataBuf , UCHAR* sdataBuf , ssize_t* slen)
{
	T_MSG_BODY_SW_INSTALL_REQ*	req;
	T_MSG_BODY_SW_INSTALL_RSP*	rsp;
	T_MSG_BODY_SW_INSTALL_RSP*	rsp2;
	INT						ret;
	struct stat				stat_info ;
	CHAR					filepath[256];
	CHAR					cmd[512];
	INT						rtn_bpf;
	INT						errcd;
	

	req = (T_MSG_BODY_SW_INSTALL_REQ*)rdataBuf;
	rsp = (T_MSG_BODY_SW_INSTALL_RSP*)sdataBuf;

	memset(rsp,0,sizeof(*rsp));
	memset(&stat_info , 0 , sizeof(stat_info)) ;
	memset(filepath , 0 , sizeof(filepath)) ;
	memset(cmd , 0 , sizeof(cmd)) ;

	f_mti_mtLog("[R] SW_INSTALL_REQ F:0x%x S:0x%x",req->data.uiFuncID,req->data.uiSeqNo);

	rsp->data.uiFuncID   = req->data.uiFuncID;
	rsp->data.uiSeqNo    = req->data.uiSeqNo;
	rsp->data.uiOK_NG    = D_SYS_OK;
	rsp->data.uiNG_Code  = E_MTI_OK;
	*slen = sizeof(*rsp);

	snprintf(filepath,sizeof(filepath),"/tmp/%s",req->data.ucFileName);
	if( (ret = stat((const char *)filepath, &stat_info)) != 0 ){
		rsp->data.uiOK_NG    = D_SYS_NG;
		rsp->data.uiNG_Code  = E_MTI_NG_FUNC;
		f_mti_mtLog("[S] SW_INSTALL_RSP(%ld %d) NG name:%31s size:%d",*slen,errno,req->data.ucFileName,req->data.uiFileSize);
		
	}else{
		if( (UINT)(stat_info.st_size) != req->data.uiFileSize ){
			rsp->data.uiOK_NG    = D_SYS_NG;
			rsp->data.uiNG_Code  = E_MTI_NG_PARAM;
			f_mti_mtLog("[S] SW_INSTALL_RSP(%ld) NG name:%31s expected size:%d size:%d %d",*slen,req->data.ucFileName,stat_info.st_size,req->data.uiFileSize);
		}else{
			if(f_mti_swm_threadId != 0){
				rsp->data.uiOK_NG    = D_SYS_NG;
				rsp->data.uiNG_Code  = E_MTI_NG_STATE;
				f_mti_mtLog("[S] SW_INSTALL_RSP(%ld) thread sts NG name:%31s expected size:%d size:%d thid:%ld",*slen,req->data.ucFileName,stat_info.st_size,req->data.uiFileSize,f_mti_swm_threadId);

			}else{
				rsp2 = (T_MSG_BODY_SW_INSTALL_RSP*)(f_mti_swm_sdata.sdataBuf);
				rsp2->data.uiFuncID   = req->data.uiFuncID;
				rsp2->data.uiSeqNo    = req->data.uiSeqNo;
				rsp2->data.uiOK_NG    = D_SYS_OK;
				rsp2->data.uiNG_Code  = E_MTI_OK;
				f_mti_event_header( rdataBuf, D_MSG_BODY_SW_INSTALL_RSP , f_mti_swm_sdata.sdataBuf ,sizeof(rsp2->data) );

				if( f_cmw_tra_inv_tbl->cmx_bootup_face == D_RRH_BOOT_ACT0 ){
					f_mti_swm_sdata.slotno = D_RRH_BOOT_ACT1;
				}else{
					f_mti_swm_sdata.slotno = D_RRH_BOOT_ACT0;
				}
				memset((char*)(f_mti_swm_sdata.filename),0,sizeof(f_mti_swm_sdata.filename));
				strncpy((char*)(f_mti_swm_sdata.filename),(const char*)(req->data.ucFileName),strlen((const char*)(req->data.ucFileName)));

				rtn_bpf = BPF_RU_ITCM_THREAD_CREATE(
						&f_mti_swm_threadId,
						BPF_RU_ITCM_SCHED_OTHER,
						0,
						D_RRH_STACKSIZE_DEFAULT,
						f_mti_swm_start,
						(VOID*)&f_mti_swm_sdata,
						&errcd);

				if(rtn_bpf == BPF_RU_ITCM_OK){
					/* OKルート、応答は起動スレッドにお任せ */
					f_mti_mtLog("[S] SW_INSTALL(%ld) start name:%31s expected size:%d size:%d thid:%ld",*slen,req->data.ucFileName,stat_info.st_size,req->data.uiFileSize,f_mti_swm_threadId);
					return D_SYS_MNG;
				}else{
					rsp->data.uiOK_NG    = D_SYS_NG;
					rsp->data.uiNG_Code  = E_MTI_NG_FUNC;
					f_mti_mtLog("[S] SW_INSTALL_RSP(%ld %d) NG name:%31s size:%d e:%d %d thid:%ld",*slen,errno,req->data.ucFileName,req->data.uiFileSize,errcd,f_mti_swm_threadId);
					f_mti_swm_threadId = 0;
				}
			}
		}
	}

	/* NG時はファイル削除 */
	snprintf(cmd,sizeof(cmd),"rm -rf %s 1>/null 2>/null",filepath);
	ret = system(cmd);	/* pgr0539	*/
	f_mti_event_header( rdataBuf, D_MSG_BODY_SW_INSTALL_RSP , sdataBuf ,sizeof(rsp->data) );
	return D_SYS_OK;
}

/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/12/25 Fujitsu)Yoshida Create
 */
INT	f_mti_event_EEPWRITE( UCHAR* rdataBuf , UCHAR* sdataBuf , ssize_t* slen)
{
	T_MSG_BODY_EEPWRITE_REQ	*req;
	T_MSG_BODY_EEPWRITE_RSP	*rsp;

	INT						i;

	INT						eepAdr = 0;
	UCHAR					*eepVal;
	INT						maxSize;
	UINT					opt_val = 0xFFFFFFFF;
	INT						skip_flag = D_RRH_OFF;
	INT						rtn = E_MTI_OK;

	req = (T_MSG_BODY_EEPWRITE_REQ*)rdataBuf;
	rsp = (T_MSG_BODY_EEPWRITE_RSP*)sdataBuf;

	f_mti_mtLog("[R] EEPWRITE_REQ F:0x%x S:0x%x",req->data.uiFuncID,req->data.uiSeqNo);

	rsp->data.uiFuncID    = req->data.uiFuncID;
	rsp->data.uiSeqNo     = req->data.uiSeqNo;
	rsp->data.usWrtOffset = (USHORT)req->data.ucWrtOffset;
	*slen = sizeof(*rsp);

	/* 変数初期化	*/
	eepVal  = &req->data.ucWrtVal[0];
	maxSize = req->data.ucWrtSize;

	switch(req->data.ucWrtOffset) {
		case D_MTI_WOFFSET_BF_ENVELOPE_MODE:
			/* 変数初期化	*/
			eepAdr  = D_DU_EEP_BF_ENVELOPE_MODE;

			/* optionサポートチェック	*/
			if( (f_mtiw_optdef_val & D_MTI_OPTION_BF_ENVELOPE_MODE_ON) != D_MTI_OPTION_BF_ENVELOPE_MODE_ON ) {
				f_mti_mtLog("[R] EEPWRITE_REQ option not support:0x%08x / req:0x%08x", f_mtiw_optdef_val, D_MTI_OPTION_BF_ENVELOPE_MODE_ON);
				rtn = E_MTI_NG_PARAM;
				break;
			}
			/* パラメータチェック	*/
			if( req->data.ucWrtSize != 1 ) {
				f_mti_mtLog("[R] EEPWRITE_REQ para error offset:%d / size:%d", req->data.ucWrtOffset, req->data.ucWrtSize);
				rtn = E_MTI_NG_PARAM;
				break;
			}
			/* EEPROM書換有無チェック	*/
			if( ((f_mtiw_opt_val & D_MTI_OPTION_BF_ENVELOPE_MODE_ON) == D_MTI_OPTION_BF_ENVELOPE_MODE_ON) && (*eepVal == 0) ) {
				skip_flag = D_RRH_OFF;
				opt_val = f_mtiw_opt_val & ~D_MTI_OPTION_BF_ENVELOPE_MODE_ON;
			} else if( ((f_mtiw_opt_val & D_MTI_OPTION_BF_ENVELOPE_MODE_ON) != D_MTI_OPTION_BF_ENVELOPE_MODE_ON) && (*eepVal == 1) ) {
				skip_flag = D_RRH_OFF;
				opt_val = f_mtiw_opt_val | D_MTI_OPTION_BF_ENVELOPE_MODE_ON;
			} else {
				skip_flag = D_RRH_ON;
			}
			rtn = E_MTI_OK;
			break;
		case D_MTI_WOFFSET_FAN_SPIN_MODE:
			/* 変数初期化	*/
			eepAdr = D_DU_EEP_FAN_SPIN_MODE;

			/* パラメータチェック	*/
			if( req->data.ucWrtSize != 1 ) {
				f_mti_mtLog("[R] EEPWRITE_REQ para error offset:%d / size:%d", req->data.ucWrtOffset, req->data.ucWrtSize);
				rtn = E_MTI_NG_PARAM;
				break;
			}
			/* EEPROM書換有無チェック	*/
			if( ((f_mtiw_opt_val & D_MTI_OPTION_FAN_SPIN_MODE_ON) == D_MTI_OPTION_FAN_SPIN_MODE_ON) && (*eepVal == 0) ) {
				skip_flag = D_RRH_OFF;
				opt_val = f_mtiw_opt_val & ~D_MTI_OPTION_FAN_SPIN_MODE_ON;
			} else if( ((f_mtiw_opt_val & D_MTI_OPTION_FAN_SPIN_MODE_ON) != D_MTI_OPTION_FAN_SPIN_MODE_ON) && (*eepVal == 1) ) {
				skip_flag = D_RRH_OFF;
				opt_val = f_mtiw_opt_val | D_MTI_OPTION_FAN_SPIN_MODE_ON;
			} else {
				skip_flag = D_RRH_ON;
			}
			rtn = E_MTI_OK;
			break;
		default:
			rtn = E_MTI_NG_PARAM;
			break;
	}

	if( rtn == E_MTI_OK ) {

		if( skip_flag != D_RRH_ON ) {
			for( i = 0; i < maxSize; i++ ) {
				BPF_HM_DEVC_EEPROM_WRITE( eepAdr, (eepVal + i) );
			}
			if( opt_val != 0xFFFFFFFF ) {
				f_mtiw_opt_val = opt_val;
			}
		}
		f_mti_mtLog("[S] EEPWRITE_REQ set option val:%08X", f_mtiw_opt_val);
		rsp->data.uiOK_NG   = D_SYS_OK;
		rsp->data.uiNG_Code = E_MTI_OK;
	} else {
		rsp->data.uiOK_NG   = D_SYS_NG;
		rsp->data.uiNG_Code = rtn;
	}
	f_mti_event_header( rdataBuf, D_MSG_BODY_EEPWRITE_RSP , sdataBuf ,sizeof(rsp->data) );
	return D_SYS_OK;
}

/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
INT f_mti_event( ssize_t rlen, UCHAR* rdataBuf , UCHAR* sdataBuf , ssize_t* slen)
{
	UINT	msgId;
	INT		ret = D_SYS_NG;

	*slen = 0;
	msgId = f_mti_msg_nwbyteorder(D_SYS_ON,rlen,rdataBuf);
	switch(msgId){
		case D_MSG_BODY_OPT_CONNECT_STT_REQ:
			ret = f_mti_event_OPT_CONNECT_STT(rdataBuf,sdataBuf,slen);
			break;
		case D_MSG_BODY_OPT_CONNECT_STP_REQ:
			ret = f_mti_event_OPT_CONNECT_STP(rdataBuf,sdataBuf,slen);
			break;
		case D_MSG_BODY_EEPROM_REG_READ_REQ:
			ret = f_mti_event_EEPROM_REG_READ(rdataBuf,sdataBuf,slen);
			break;
		case D_MSG_BODY_DUSTATUS_GET_REQ:
			ret = f_mti_event_DUSTATUS_GET(rdataBuf,sdataBuf,slen);
			break;
		case D_MSG_BODY_FORCEALM_REQ:
			ret = f_mti_event_FORCEALM(rdataBuf,sdataBuf,slen);
			break;
		case D_MSG_BODY_CARDBLK_REQ:
			ret = f_mti_event_CARDBLK(rdataBuf,sdataBuf,slen);
			break;
		case D_MSG_BODY_CARDRST_REQ:
			ret = f_mti_event_CARDRST(rdataBuf,sdataBuf,slen);
			break;
		case D_MSG_BODY_PAONOFF_REQ:
			ret = f_mti_event_PAONOFF(rdataBuf,sdataBuf,slen);
			break;
		case D_MSG_BODY_NETCONF_AUTH_RESET_REQ:
			ret = f_mti_event_NETCONF_AUTH_RESET(rdataBuf,sdataBuf,slen);
			break;
		case D_MSG_BODY_POWADJ_WRITE_REQ:
			ret = f_mti_event_POWADJ_WRITE(rdataBuf,sdataBuf,slen);
			break;
		case D_MSG_BODY_BEAMID_SET_REQ:
			ret = f_mti_event_BEAMID_SET(rdataBuf,sdataBuf,slen);
			break;
		case D_MSG_BODY_SW_INSTALL_REQ:
			ret = f_mti_event_SW_INSTALL(rdataBuf,sdataBuf,slen);
			break;
		case D_MSG_BODY_EEPWRITE_REQ:
			ret = f_mti_event_EEPWRITE(rdataBuf,sdataBuf,slen);
			break;
		default:
			f_mti_mtLog("Unknown message 0x%08x", msgId);
			ret = D_SYS_MNG;
			break;
	}

	f_mti_last_msgId = msgId;
	if(ret != D_SYS_MNG){
		f_mti_msg_nwbyteorder(D_SYS_OFF,*slen,sdataBuf);
	}
	return ret;
}

/* @} */
