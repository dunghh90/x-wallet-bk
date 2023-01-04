/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_rsv_ftyp.h
 *  @brief	RE Supervision common function prototype definition
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2010/03/02 QNET)Kabasima MOD M-S3G-eNBPF-03221:CPRI共用からLTE単独構成変更時にREのレジスタに3G-L3ERRが残る場合がある
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/******************************************************************************************************************************/

/* 二重取込み防止 */
#ifndef		F_COM_FTYP
#define		F_COM_FTYP

#include	"fldc4.h"

/** @addtogroup RRH_PF_COM
* @{ */
extern VOID f_com_almReport(UINT, UINT );
extern VOID f_com_almClear(UINT, UINT );

extern VOID f_com_SVCtl_real(UINT, UINT, UINT, UINT, const VOID*, UINT);
#define f_com_SVCtl(a, b, c) f_com_SVCtl_real(a, b, c, D_RRH_ON, __FILE__, __LINE__)
extern VOID f_com_SVCtl_abort(UINT,UINT);

extern UINT f_com_GetHardAlmFact(UINT);
extern UINT f_com_Str2Hex( CHAR *, UINT *);
extern VOID f_com_SVLogSet_real(T_RRH_SVSTATELOG *, UCHAR *, const VOID*, UINT);
#define f_com_SVLogSet(a) f_com_SVLogSet_real(a, NULL, __FILE__, __LINE__)

extern VOID f_com_SVCTL_setDefault(UINT);

extern INT f_com_msgQSend(UINT ,VOID *,UINT ,UINT ,UINT ,UINT  );

extern INT f_com_msgQReceive(UINT ,INT ,VOID*,INT*,INT*);

extern VOID f_com_sendMsgToApl(UINT ,void *,UINT ,UINT ,UINT  );

extern VOID f_com_abortLog(UINT,UINT);

extern VOID	f_com_logSaveFlash( UINT wdtLogFlg );

/* 2021/01/19 M&C) Merge 4G FHM src (start add) */
extern void f_com_SVCtl_OtherProc(UINT, UINT, UINT, UINT);
extern void f_com_SVCtl_AlmCodeSet(UINT);
extern VOID f_com_abortLogFHM(void);
extern VOID	f_com_logSaveFlashFHM(void);
extern VOID f_com_iniSysParaTblFHM(void);
/* 2021/01/19 M&C) Merge 4G FHM src (end add) */

extern int f_com_FA_encode(
 unsigned char *InBuf,		 	/* 被圧縮データ用バッファアドレス	 	*/
 unsigned char *OutBuf, 	 	/* 圧縮データ用バッファアドレス 		*/
 unsigned int	inlen,	 		/* 被圧縮データのデータバイトサイズ		*/
 unsigned int	outbsz, 		/* 圧縮データ用バッファバイトサイズ		*/
 FLDC4_ELEM    *blsadr, 		/* fldc4用圧縮辞書アドレス 				*/
 unsigned int  *outlen			/* 圧縮後のデータバイトサイズ 			*/
);

extern int f_com_FA_decode(
 unsigned char *InBuf,		 	/* 圧縮データ用バッファアドレス 		*/
 unsigned char *OutBuf, 	 	/* 復元データ用バッファアドレス 		*/
 unsigned int	inlen,	 		/* 圧縮データのデータバイトサイズ	 	*/
 unsigned int	outbsz, 		/* 復元データ用バッファバイトサイズ		*/
 unsigned char *TmpBuf,			/* 復元テンポラリバッファアドレス		*/
 FLDC4_ELEM    *blsadr,			/* fldc4用圧縮辞書アドレス 				*/
 unsigned int  *outlen	 		/* 復元後のデータバイトサイズ		 	*/
);

extern VOID f_com_alarmHitoryLogSet( UINT, UINT, UINT* );
extern VOID f_com_alarmHitoryLogSet_com( UINT, UINT, UINT* );

extern VOID f_com_iniSysParaTbl(void);

extern INT f_com_MSIsend(
	UINT		messageId,
	UINT		requestId,
	INT			payloadSize,
	VOID*		payloadPtr,
	UINT		srcPQueueID,
	UINT		srcTQueueID
);

VOID f_com_MSIgetPayload(
	VOID*		msgPtr,
	UINT*		messageId,
	UINT*		requestId,
	INT*		payloadSizePtr,
	VOID**		payloadPtr
);

extern	INT		f_com_search_vid_chk( UCHAR*, USHORT );
extern	VOID	f_com_search_vid_set( UCHAR*, USHORT, INT );
extern	INT		f_com_search_vid_cnt( UCHAR*, USHORT, USHORT );

extern	VOID f_com_mtCtl_init(VOID );
extern	VOID f_com_mtCtl_con(UCHAR onoff);
extern	VOID f_com_mtCtl_carInfo(UINT antno, UINT carno, UINT onoff, UINT freq , UINT bandwith, UINT dir);
extern	VOID f_com_mtCtl_sfpInfo(UINT sfp_type, UCHAR* name , UCHAR* serial);
extern	VOID f_com_mtCtl_aisgCtl(UINT onoff);

extern	VOID f_com_dumpMsgInit( VOID );
extern	VOID f_com_dumpMsgSend(e_bpf_com_dump_etype ether_type ,const char* dumpMessage_p , ... );
extern	VOID f_com_dumpDataSend(e_bpf_com_dump_etype ether_type ,int direct , int dataLen ,CHAR* data_p  );

#define f_com_dumpMsgAisgSend( ... )		f_com_dumpMsgSend(E_BPF_COM_DPLOG_ETHTYPE_AISG , __VA_ARGS__)
#define f_com_dumpDataAisgSend(Len,Data)	f_com_dumpDataSend(E_BPF_COM_DPLOG_ETHTYPE_AISG,BPF_COM_DPLOG_DIR_TX,Len,Data)
#define f_com_dumpDataAisgRecv(Len,Data)	f_com_dumpDataSend(E_BPF_COM_DPLOG_ETHTYPE_AISG,BPF_COM_DPLOG_DIR_RX,Len,Data)
#define f_com_dumpMsgEventSend( ... )		f_com_dumpMsgSend(E_BPF_COM_DPLOG_ETHTYPE_DBG , __VA_ARGS__)

#endif	/* M_RS_FTYP */
