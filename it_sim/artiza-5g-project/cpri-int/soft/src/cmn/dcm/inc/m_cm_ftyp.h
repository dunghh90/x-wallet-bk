/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cm_ftyp.h
 *  @brief	TRA process common function prototype definition
 *  @date   2008/07/18 FFCS)Tangj create
 *  @date   2009/03/05  FFCS)Wuh modify for M-S3G-eNBPF-01471
 *					    stop writing FLASH after receiving CPRI message
 *  @date   2009/10/08 QNET)Kabasima MOD CR-00071-001(TRA-HS-V2R11 RE AUTO RESET and 3GRF-INF)
 *  @date   2009/12/27 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
 *  @date   2010/01/04 QNET)Kabasima ADD M-S3G-eNBPF-02869:[品質改善]REデバッグコマンド機能拡充
 *  @date   2010/06/01 FJT)Taniguchi CR-00091-001:[DCM向け]REハード・ソフトインタフェース仕様書v2.18版リリース
 *  @date   2011/04/06 FJT)Koshida modify for M-S3G-eNBPF-04023対応
 *  @date   2013/09/30 FFCS)Xut Modify for 13B-03-005-04
 *  @date   2013/10/10 FJT)Matsumoto 13B-03-005 13B問題対応(TRA)
 *  @date   2015/04/27 FFCS)zhangxq TDD SRE
 *  @date   2015/09/24 ALPHA)ueda modify for TDD-RRE(Zynq)
 *  @date   2015/11/05 ALPHA)ueda modify for TDD-RRE(Zynq)
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/******************************************************************************************************************************/


/* For multiple include */

#ifndef    M_CM_FTYP
#define    M_CM_FTYP

/** @addtogroup RRH_DCM
* @{ */

#define S_FILE __FILE__

/* ======================================================================== */
/*	共通関数プロトタイプ／マクロ定義								*/
/* ======================================================================== */
/*	Message送信処理	*/
#define	 cm_Enter( tskNo, tskSts, evtNo, dat_p ) BPF_RU_IPCM_MSGQ_SEND( tskNo, dat_p )

/*	Message受信処理	*/
#define	cm_Recieve( q_num, wait_time, msgp, msgsize ) BPF_RU_IPCM_MSGQ_RECV( q_num, wait_time, msgp, msgsize )

/*	タスク停止処理	*/
extern	VOID cmR_MStop(VOID*, UINT);
#define	 cm_MStop() cmR_MStop( S_FILE, __LINE__ )

/*	タスク一時停止処理	*/
extern	VOID cmR_MWait(UINT, const VOID*, UINT);
#define	 cm_MWait( waitTim ) cmR_MWait( waitTim, S_FILE, __LINE__ )

/*	バッファ取得処理	*/
extern	UINT cmR_BReq(UINT, UINT, VOID**, VOID*, UINT);
#define  cm_BReq( buf_Category, bufSize, bufAdr_p ) cmR_BReq( buf_Category, bufSize, bufAdr_p, S_FILE, __LINE__ )

extern	UINT cmR_BReq2(UINT, UINT, VOID**, VOID*, UINT);
#define  cm_BReq2( buf_Category, bufSize, bufAdr_p ) cmR_BReq( buf_Category, bufSize, bufAdr_p, S_FILE, __LINE__ )
/* ZTX_ZTX   */

/*	バッファ解放処理	*/
#define  cm_BFree( bufAdr_p ) BPF_RU_IPCM_PROCMSG_ADDRFREE( bufAdr_p )

#define  cm_BFree2( bufAdr_p ) BPF_RU_IPCM_PROCMSG_ADDRFREE( bufAdr_p )
/* ZTX_ZTX   */

/*	セマフォ取得処理	*/
#define  cm_Tssm( smNo, thr_sem_flag, timeout ) BPF_RU_ITCM_THREAD_SEM_TAKE( smNo, thr_sem_flag, timeout)

/*	セマフォ解放処理	*/
#define	 cm_Rssm( smNo ) BPF_RU_ITCM_THREAD_SEM_GIVE( smNo )

/*	タイマ起動処理	*/
#define  cm_TStat(timerid, time,  notify_type, eventno, queueid, errcd ) \
				BPF_RU_HRTM_REGISTER( notify_type, timerid, time, eventno, queueid, errcd )

#define  cm_TStat_id(timerid, time,  notify_type, eventno, queueid, errcd , id_p , osid_p,usekey ) \
				BPF_RU_HRTM_REGISTER_ID( notify_type, timerid, time, eventno, queueid, errcd , id_p , osid_p,usekey)

/*	タイマ停止処理	*/
#define  cm_TStop( timNo, errcd ) BPF_RU_HRTM_CANCEL( timNo, errcd )

/*	アボート処理	*/
extern	VOID cmR_MAbort(UINT, const VOID*, const VOID*, UINT, UINT, UINT, const VOID*, UINT);
#define  cm_MAbort( almCode, FuncName_p, Msg_p, uCode1, uCode2, uCode3 ) \
		cmR_MAbort(almCode, FuncName_p, Msg_p, uCode1, uCode2, uCode3, S_FILE, __LINE__);

/*	出力要求処理 */
#define  cm_Print( printString_p,  printdata ) printf( printString_p,  printdata )

/*	出力要求処理(デバイス指定) */
#define  cm_PrintDev(UINT, format_p, ... ) printf( format_p, ##__VA_ARGS__ )

#define  cm_PrintDevNet(CMT_DBGPPC_DBGINFO , format_p, ... ) printf( format_p, ##__VA_ARGS__ )

/*	実行中自タスク番号取得処理 */
#define	 cm_GetPid() getpid( )

/*	メモリコピー処理	*/
#define	 cm_MemCpy( dstAdr_p, srcAdr_p, cpyLen ) memcpy( dstAdr_p, srcAdr_p, cpyLen )

/*	メモリクリア処理	*/
#define	 cm_MemClr(setAdr_p, setLen) memset(setAdr_p, 0, setLen )

/*	チェックサム計算処理	*/
extern	VOID cmR_CheckSumCalc(const VOID*, UINT, USHORT*, VOID*, UINT);
#define	 cm_CheckSumCalc( chkSumAdr_p, datLen, chkSum_p ) cmR_CheckSumCalc( chkSumAdr_p, datLen, chkSum_p, S_FILE, __LINE__ )

/*	チェックサム計算処理(CPRI信号用)	*/
extern	VOID cmR_CheckSumCalcCpri(const VOID*, UINT, USHORT*, VOID*, UINT);
#define	 cm_CheckSumCalcCpri( chkSumAdr_p, datLen, chkSum_p ) cmR_CheckSumCalcCpri( chkSumAdr_p, datLen, chkSum_p, S_FILE, __LINE__ )

/*	チェックサム計算処理 File	*/
extern	VOID cmR_CheckSum4ByteCalc(const VOID*, UINT, UINT*, VOID*, UINT);
#define	 cm_CheckSum4ByteCalc( chkSumAdr_p, datLen, chkSum_p ) cmR_CheckSum4ByteCalc( chkSumAdr_p, datLen, chkSum_p, S_FILE, __LINE__ )


/*	Flash削除処理２(FPGA／障害ログ)	*/
extern	UINT cmR_ReadEeprom(UINT, UCHAR*,VOID*, UINT);
#define	 cm_ReadEeprom( eepDevAdr, readDat_p  ) cmR_ReadEeprom(  eepDevAdr, readDat_p, S_FILE, __LINE__ )

extern	UINT cmR_WriteEeprom(UINT,UCHAR, VOID*, UINT);
#define	 cm_WriteEeprom( eepDevAdr, writeDat ) cmR_WriteEeprom( eepDevAdr, writeDat, S_FILE, __LINE__ )


/*	アサート出力処理	*/
extern	VOID cmR_Assert(UINT, UINT, const VOID*, const VOID*, UINT);
// 擬似環境
#define	 cm_Assert( Level, UserCode, Message_p )					\
do {																\
	cmR_Assert( Level, UserCode, Message_p, S_FILE, __LINE__ );		\
	BPF_COM_PLOG_TRACE_PROC(Level,"0x%08x,%s",UserCode,Message_p);	\
}while(0)
// 擬似環境

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/*	RTC設定処理		*/
#define	 cm_RtcSet( datetime ) BPF_HM_DEVC_SETTIME_BCD( datetime ) 
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/*	RTC読出処理		*/
#define	 cm_RtcGet( datetime ) BPF_HM_DEVC_GETTIME_BCD( datetime ) 

/*	OS時刻読み出し処理		*/
#define	 cm_OsTimeGet( datetime ) BPF_HM_DEVC_GETTIME_OS( datetime ) 

/*	データパディング処理C	*/
extern	VOID cmR_MemPadc(UCHAR*, UCHAR, UINT, const VOID*, UINT);
#define	 cm_MemPadc( Address, Pattern, charSize ) cmR_MemPadc( Address, Pattern, charSize, S_FILE, __LINE__ )

/*	データパディング処理S	*/
extern	VOID cmR_MemPads(USHORT*, USHORT, UINT, const VOID*, UINT);
#define	 cm_MemPads( Address, Pattern, shortSize ) cmR_MemPads( Address, Pattern, shortSize, S_FILE, __LINE__ )

/*	データパディング処理I	*/
extern	VOID cmR_MemPadi(UINT*, UINT, UINT, const VOID*, UINT);
#define	 cm_MemPadi( Address, Pattern, intSize ) cmR_MemPadi( Address, Pattern, intSize, S_FILE, __LINE__ )

/*	メモリ比較処理	*/
#define	 cm_MemCmp( Address_A, Address_B, cmpSize ) memcmp( Address_A, Address_B, cmpSize )

/*	ＯＳバッファ獲得処理	*/
#define	 cm_Bfget( buffer_type, buffer_remain, alloc_buffer_type, buffer_address ) BPF_RU_MBLM_BUFFERALLOC( buffer_type, buffer_remain, alloc_buffer_type, buffer_address )

/*	ＯＳバッファ解放処理	*/
#define	 cm_Bfrel( buffer_address ) BPF_RU_MBLM_BUFFERFREE( buffer_address )

/*	L2バッファ獲得処理	*/
#define	 cm_L2BfGet( buf_Category, bufsize, link_no, bufAdr_p ) cmR_BReq( buf_Category, bufsize, bufAdr_p, S_FILE, __LINE__ )

/*	文字列長取得	*/
#define	cm_StrLen(s) strlen(s)

/*	文字列比較	*/
#define	cm_StrCmp(s1, s2) strcmp(s1, s2)

/*	文字列比較(文字数指定)	*/
#define	cm_StrNCmp(s1, s2, n) strncmp(s1, s2, n)

/*	文字列比較(大文字小文字無視)	*/
#define	cm_StrCaseCmp(s1, s2) strcasecmp(s1, s2)

/*	文字列コピー	*/
#define	cm_StrCpy(dest, src) strcpy(dest, src)

/*	文字列コピー(文字数指定)	*/
#define	cm_StrNCpy(dest, src, n) strncpy(dest, src, n)

/*	文字列連結	*/
#define cm_StrCat(dest, src) strcat(dest, src)

/*	文字列連結(文字数指定)	*/
#define cm_StrNCat(dest, src, n) strncat(dest, src, n)

/*	文字列検索	*/
#define cm_StrStr(haystack, needle) strstr(haystack, needle)

/*	整数文字列変換処理	*/
extern	UINT cmR_Itoa(UINT, UCHAR*, UINT, UINT, UINT, const VOID*, UINT);
#define	cm_Itoa(val, buf_p, size, radix, chCase) cmR_Itoa(val, buf_p, size, radix, chCase, S_FILE, __LINE__)

/* Convert String to Hex */
extern UINT cm_Str2Hex( CHAR *, UINT *);

/* Convert Hex to String */
extern UINT cm_Hex2Str( CHAR *, CHAR *,INT );		

/*	ISSPACE	*/
extern	INT cmR_IsSpace(INT, const VOID*, UINT);
#define	cm_IsSpace(c) cmR_IsSpace(c, S_FILE, __LINE__)

/*	ISDIGIT	*/
#define	cm_IsDigit(c) isdigit(c)


/* Active状態チェック処理 */
extern	UINT	m_cmR_ActiveChk( USHORT,VOID*, UINT );
#define	m_cm_ActiveChk(  sysType) m_cmR_ActiveChk( sysType,S_FILE, __LINE__ )

/*  カード状態チェック処理 */
extern	USHORT	m_cmR_ChkCardSta( UINT, UINT, USHORT ,VOID*, UINT );
#define	m_cm_ChkCardSta(  targetkind, chktarget, chkcardsta ) m_cmR_ChkCardSta( targetkind, chktarget, chkcardsta, S_FILE, __LINE__ )

/*  カード状態取得処理 */
extern	VOID	m_cmR_GetCardSta( USHORT*,VOID*, UINT );
#define	m_cm_GetCardSta(  usCardSta) m_cmR_GetCardSta( usCardSta, S_FILE, __LINE__ )


/*  カード状態取得処理 2 */
extern	VOID	m_cmR_GetCardSta2( USHORT*, USHORT*,VOID*, UINT );
#define	m_cm_GetCardSta2(  usCardSta,usSingleAlm ) m_cmR_GetCardSta2( usCardSta, usSingleAlm,S_FILE, __LINE__ )

/*TDD SRE  TRX setting S. */
/*  Brx状態チェック処理 */
extern	USHORT	m_cmR_ChkBrxSta(  USHORT, USHORT , USHORT, VOID*, UINT );
#define	m_cm_ChkBrxSta(  chktarget, chkcardsta, carr_no ) m_cmR_ChkBrxSta( chktarget, chkcardsta, carr_no, S_FILE, __LINE__ )

/*  Brx状態取得処理 */
extern	VOID	m_cmR_GetBrxSta( USHORT, USHORT, USHORT*, VOID*, UINT );
#define	m_cm_GetBrxSta(chktarget, carr_no, usCardSta ) m_cmR_GetBrxSta( chktarget, carr_no, usCardSta, S_FILE, __LINE__ )

/*  Brx状態チェック処理 */
extern	USHORT m_cm_ChkBrSta_l3(USHORT br_no,USHORT br_stat,USHORT carr_no);
/*TDD SRE  TRX setting E.*/

/*  カード検索処理 */
extern	UINT	m_cmR_CardSarch( UINT, VOID*, UINT );
#define	m_cm_CardSarch(  slot_no ) m_cmR_CardSarch( slot_no, S_FILE, __LINE__ )

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/*  L3信号中継送信 */
extern	UINT	m_cmR_L3MsgSend( USHORT linkno, VOID*, VOID*, UINT );
#define	m_cm_L3MsgSend(cprimsgbuff_p) m_cmR_L3MsgSend(D_RRH_CPRINO_REC, cprimsgbuff_p,  S_FILE, __LINE__ )
#define	m_cm_L3MsgSend_REC(cprimsgbuff_p) m_cmR_L3MsgSend(D_RRH_CPRINO_REC, cprimsgbuff_p,  S_FILE, __LINE__ )
#define	m_cm_L3MsgSend_RE(cprino, cprimsgbuff_p) m_cmR_L3MsgSend(cprino, cprimsgbuff_p,  S_FILE, __LINE__ )
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/*  L3信号中継送信(for slave RE */
extern	UINT	m_cmR_L3MsgSend_cscd( VOID*, VOID*, UINT );
#define	m_cm_L3MsgSend_cscd(cprimsgbuff_p) m_cmR_L3MsgSend_cscd(cprimsgbuff_p,  S_FILE, __LINE__ )

/* write LED register */
extern  VOID m_cmR_WriteLEDReg( VOID* , UINT );
#define m_cm_WriteLEDReg( ) m_cmR_WriteLEDReg(  S_FILE, __LINE__)

extern VOID  m_cmR_Delay(UINT, const VOID* , UINT );
#define m_cm_Delay(mDelay ) m_cmR_Delay(mDelay, S_FILE, __LINE__)

extern UINT m_cm_chksig(USHORT,USHORT *);
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
extern VOID	m_cmR_ErrCntRegSetRE(USHORT, USHORT, UINT, UINT, USHORT, const VOID* , UINT);
extern VOID m_cmR_AxC(VOID);
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define m_cm_ErrCntRegSetRE(linkno, action_flg, writeAdr, writeDat, chkalm_flg) \
		m_cmR_ErrCntRegSetRE(linkno, action_flg, writeAdr, writeDat, chkalm_flg, S_FILE, __LINE__)
#define m_cm_ErrCntRegSet(action_flg, writeAdr, writeDat, chkalm_flg) \
		m_cmR_ErrCntRegSetRE(D_RRH_CPRINO_REC, action_flg, writeAdr, writeDat, chkalm_flg, S_FILE, __LINE__)
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

extern VOID m_cm_SysParaDSet( UINT );

/*when send_system_number is nt2r and br2 br3 product alm mask alm bit*/
extern VOID m_cm_CardSta_MaskAlm(USHORT *card_svitem);

#endif	/*M_CM_FTYP*/
