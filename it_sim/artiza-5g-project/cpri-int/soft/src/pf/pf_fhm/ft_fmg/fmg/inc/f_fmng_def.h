/*!
* @skip $Id:$
* @file f_fmng_def.h
* @brief fmg task Define Header
* @date  2015/08/11 TDIPS)sasaki update
* @date  2015/08/12 TDIPS)sasaki update API kind にREADを追加
* @date  2015/08/17 TDIPS)sasaki update メーカ識別に'X'(FHM用)を追加
* @date  2015/08/19 TDIPS)sasaki update 応答結果に50:ハードウェア異常(D_FMNG_HW_ERR)を追加(MKレビュー指摘事項No.62対応)
* @date  2015/08/19 TDIPS)sasaki update RE-FLASH R/W タイマ値定義(D_FMNG_TIMEVAL_RE_FLSRW)を追加
* @date  2015/08/20 TDIPS)sasaki update イベントID(D_FMNG_EVTIDX_RE_FILE_RW_TO)、状態ID(D_FMNG_THRDSTA_FWINFO_SAVE)を追加
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
*/
/** @addtogroup RRH_PF_FMNG
 *  @{
 */
#ifndef F_FMNG_DEF_H
#define F_FMNG_DEF_H

/*!
 * @name Data length 
 * @note Data length definition
 * @{
 */
#define D_FMNG_PRODUCTDATE_LEN			10
#define D_FMNG_PRODUCTNUM_LEN			26
#define D_FMNG_PRODUCTNAME_LEN			14
#define D_FMNG_PRODUCTREV_LEN			8
#define D_FMNG_SERIALNUM_LEN			14
#define D_FMNG_CRC1SIZE					56
#define D_FMNG_CHKSUM_OFFSET			0x44
#define D_FMNG_FPGA_MASK			    0x00008000
/* @} */


/*!
 * @name Event index
 * @note Event index definition used in f_fmngr_matrix_tbl
 * @{
 */
#define D_FMNG_EVTIDX_FIRMERASE_REQ			0
#define D_FMNG_EVTIDX_FLSHERASE_RSP			1
#define D_FMNG_EVTIDX_FIRMUPD_REQ			2
#define D_FMNG_EVTIDX_FLSHWRT_RSP			3
#define D_FMNG_EVTIDX_FIRMVER_REQ			4
#define D_FMNG_EVTIDX_FLSHACCESS_TOUT		5
#define D_FMNG_EVTIDX_FIRMCANCEL_IND		6
#define D_FMNG_EVTIDX_TARPROCESS_RSP		7
#define D_FMNG_EVTIDX_FWINFO_UPD_START		8
#define D_FMNG_EVTIDX_FWINFO_DEL_START		9
#define D_FMNG_EVTIDX_FWINFO_SAVE_RSP		10
#define D_FMNG_EVTIDX_RE_FILE_RW_TO			11
/* @} */

/*!
 * @name Event index
 * @note Event index definition used in f_fmngr_re_matrix_tbl
 * @{
 */
#define D_FMNG_EVTIDX_RE_FIRM_READ_REQ		0
#define D_FMNG_EVTIDX_RE_FIRM_SAVE_REQ		1
#define D_FMNG_EVTIDX_RE_FIRM_CANCEL_IND	2	
#define D_FMNG_EVTIDX_RE_FIRM_READ_RSP		3
#define D_FMNG_EVTIDX_RE_FIRM_WRITE_RSP		4
#define D_FMNG_EVTIDX_RE_FIRM_ERASE_RSP		5
#define D_FMNG_EVTIDX_RE_FWINFO_SAVE_RSP	6
#define D_FMNG_EVTIDX_RE_FLASH_ACCESS_TO	7
/* @} */

/************************************************************************************************************/
/* MAXIMUM constant definition                                                                              */
/************************************************************************************************************/
/*!
 * @name MAXIMUM constant
 * @note MAXIMUM constant definition
 * @{
 */
#define D_FMNG_STATMAX                  7       /*!< Maximum task state number                              */
#define D_FMNG_EVTMAX                   12      /*!< Maximum event number                                   */
#define D_FMNG_RE_STATMAX               5       /*!< Maximum task state number for RE-FLASH                 */
#define D_FMNG_RE_EVTMAX                8       /*!< Maximum event number for RE-FLASH                      */
#define D_FMNG_MSGLENMAX                512     /*!< Maximum length of msg received by FMNG task            */
/* @} */

/*!
 * @name EEPROM index 
 * @note EEPROM index  definition
 * @{
 */
#define D_FMNG_THRDSTA_INIT             0       /*!< Initial                                                */
#define D_FMNG_THRDSTA_RUN              1       /*!< Runing                                                 */
#define D_FMNG_THRDSTA_FLSHRW           2       /*!< Flash Reading or Writing                               */
#define D_FMNG_THRDSTA_FLSHRWWAIT       3       /*!< Flash Reading or Writing Waiting                       */
#define D_FMNG_THRDSTA_FLSHRWCANCEL     4       /*!< Flash Reading or Writing Cancel                        */
#define D_FMNG_THRDSTA_TARPROCESS       5       /*!< Decompressing                                          */
#define D_FMNG_THRDSTA_FWINFO_SAVE      6       /*!< FW Info Saving                                         */
/* @} */

/*!
 * @name State index for RE-FLASH
 * @note State index for RE-FLASH definition
 * @{
 */
#define D_FMNG_THRDSTA_RE_IDLE          0       /*!< Idle                                                   */
#define D_FMNG_THRDSTA_RE_FLSHRW        1       /*!< RE-FLASH READ/WRITE Processing                         */
#define D_FMNG_THRDSTA_RE_FLSHRWWAIT    2       /*!< RE-FLASH WRITE Waiting                                 */
#define D_FMNG_THRDSTA_RE_FLSHRWCANCEL  3       /*!< RE-FLASH READ/WRITE Cancel                             */
#define D_FMNG_THRDSTA_RE_FWINFO_SAVE   4       /*!< FW Info Saving                                         */
/* @} */


/*!
 * @name Timer value
 * @note Timer value definition (msec)
 * @{
 */
#define D_FMNG_TIMEVAL_FLSERASE     10 * 600 * 100 * 2          /*!< Flash Erase timer (T.B.D)              */
#define D_FMNG_TIMEVAL_FLSWRT       10 * 600 * 100 * 2          /*!< Flash write timer (T.B.D)              */
#define D_FMNG_TIMEVAL_RE_FLSRW     (600 * 1000)                /*!< RE-FLASH R/W timer(600sec)             */
/* @} */

/*!	
 * @name Timer value
 * @note Timer value definition (msec)
 * @{
 */
#define D_FMNG_ERASE_DELETE 1
#define D_FMNG_ERASE_UPDATE 2
/* @} */

/************************************************************************************************************/
/*  recieve and send flag                                                                                   */
/************************************************************************************************************/
/*!
 * @name recieve and send flag
 * @note recieve and send flag definition
 * @{
 */
#define D_FMNG_RECIEVEFLG               0                   /*!< recieve Flag                               */
#define D_FMNG_SENDFLG                  1                   /*!< send flag                                  */
/* @} */

/*!
 * @name データ格納テーブル定数
 * @note データ格納テーブル定数 definition
 * @{
 */
#define D_FMNG_RCV_MPU_NUM              3145728             /*!< the number of the MPU size(3M)             */
#define D_FMNG_RCVDAT_MKRSV             0x1AE               /*!< 430byte                                    */
/*! @} */

/*!
 * @name ファイル情報報告通知受信処理用
 * @note ファイル情報報告通知受信処理用 definition
 * @{
 */
#define D_FMNG_HEAD_SIZE                0x400               /*!< 受信データヘッダサイズ	                    */
#define D_FMNG_HDL_NG                   51                  /*!< 圧縮データサイズ                           */
/* @} */

/*!  
 * @name maker name (3001 type)
 * @{  
 */
#define D_FMNG_MAKER1                   0x0046              /*!< F                                          */
#define D_FMNG_MAKER2                   0x0052              /*!< R                                          */
#define D_FMNG_MAKER3                   0x004B              /*!< K                                          */
#define D_FMNG_MAKER4                   0x0056              /*!< V                                          */
#define D_FMNG_MAKER5                   0x0058              /*!< X                                          */

#define D_FMNG_MJ1                      0                   /*!< maker judge 1                              */
#define D_FMNG_MJ2                      1                   /*!< maker judge 2                              */
#define D_FMNG_MJ3                      2                   /*!< maker judge 3                              */
#define D_FMNG_MJ4                      3                   /*!< maker judge 4                              */
#define D_FMNG_MJ5                      4                   /*!< maker judge 5                              */

/*!  
 * @name received API kind
 * @{  
 */
#define D_FMNG_API_NONE	0
#define D_FMNG_API_ERA	1
#define D_FMNG_API_WRT	2
#define D_FMNG_API_RD	3
/*! @} */

/*!  
 * @name Send Response kind
 * @{  
 */
#define D_FMNG_RSP_NONE		0
#define D_FMNG_RSP_SAVE		1
#define D_FMNG_RSP_CANCEL	2
/*! @} */

/*!  
 * @name FW Info Save kind
 * @{  
 */
#define D_FMNG_FWINFO_UPD	1
#define D_FMNG_FWINFO_DEL	2
/*! @} */

/*!  
 * @name RE kind for checking dl info
 * @{  
 */
#define D_FMNG_RRH_INVALID      0x00    /*!< 0000 0000                                                      */
#define D_FMNG_RRH_SRE_15G      0x01    /*!< 0000 0001                                                      */
#define D_FMNG_RRH_RRE_15G      0x02    /*!< 0000 0010                                                      */
#define D_FMNG_RRH_RRE_17G      0x04    /*!< 0000 0100                                                      */
#define D_FMNG_RRH_RRE_700M     0x08    /*!< 0000 1000                                                      */
#define D_FMNG_RRH_SRE_15G_DPD  0x10    /*!< 0001 0000                                                      */
#define D_FMNG_RRH_SLC_2G       0x20    /*!< 0010 0000                                                      */
#define D_FMNG_RRH_SLC_800M     0x40    /*!< 0100 0000                                                      */
#define D_FMNG_RRH_1CHP_15G     0x80    /*!< 1000 0000                                                      */
#define D_FMNG_RRH_1CHP_17G     0x100   /*!< 0001 0000 0000                                                 */
#define D_FMNG_RRH_SRE_35G      0x01    /*!< 0000 0001                                                      */
#define D_FMNG_RRH_SRE_35G_R2   0x02    /*!< 0000 0010                                                      */
#define D_FMNG_RRH_SRE_35G_ZYNQ 0x10    /*!< 0001 0000                                                      */
/* FLASH EOL対応 add start */
#define D_FMNG_RRH_FLASH_EOL_FLAG  0x200	/*!< 0010 0000 0000 */  /* ダウングレード抑止フラグ           */
																  /* Macronixの時はこのフラグを設定して */
																  /* ダウングレードを抑止する           */
/* FLASH EOL対応 add end */
/*! @} */

 /*!  
  * @name download type
  * @{	
  */
#define D_FMNG_DL_STARTUP   0                           /*!< the download type is download in startup state */
#define D_FMNG_DL_USE       1                           /*!< the download type is download in use state     */
/*! @} */

/*!
 * @name return value
 * @{
 */
#define D_FMNG_HW_ERR               50                  /*!< ハードウェア異常                               */
#define D_FMNG_CHKSUM_NG_S3G        60                  /*!< チェックサム異常                               */
#define D_FMNG_REFW_NG              221                 /*!< ダウンロードファイル異常                       */
/*! @} */

#define swap_32(V) \
				((((UINT)(V) & 0xff000000) >> 24) | \
				(((UINT)(V) & 0x00ff0000) >> 8) | \
				(((UINT)(V) & 0x0000ff00) << 8) | \
				(((UINT)(V) & 0x000000ff) << 24))
				
#define swap_16(V) \
				((((USHORT)(V) & 0xff00) >> 8) | \
				(((USHORT)(V) & 0x00ff) << 8))

/** @name running history log		*/
/* @{ */
#define	D_FMNG_RUNHIS_BODYSIZE_MAX	48		/**< ボディサイズMAX */
#define	D_FMNG_RUNHIS_FACENUM_MAX	1024	/**< 面数MAX */
#define	D_FMNG_RUNHIS_FLG_RCV		0x00	/**<  送受信フラグ(受信)   */
#define	D_FMNG_RUNHIS_FLG_SND		0x01	/**<  送受信フラグ(送信)   */
#define	D_FMNG_RUNHIS_FLG_SND_NG	0xff	/**<  送受信フラグ(送信NG) */
/* @} */

/* ================================================================================ */
/* FLDC4ヘッダの文字列																*/
/* ================================================================================ */
#define D_FMNG_FLDC4_HEADER_STRING "FLDC4 USE"	/**<  FLDC4ヘッダ(new)の文字列 */
/* the size of CPRIFPGA file is 12M */
#define D_FMNG_DEVDAT_CPRIFPGA		0xC00000			/* MDIF-FPGA/OFMSDIF-FPGAサイズ				*/

#endif
/** @} */
