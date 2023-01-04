/*!
 * @skip  $ld:$
 * @file  l2_com_def.h
 * @brief common definition
 * @date  2013/11/18 FFCS)hongj create for zynq
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_L2
 */
#ifndef L2_COM_DEF_H
#define L2_COM_DEF_H

 /*!
 * @note note Thread name
 */
#define D_L2_THRDNAME_MAIN		"L2_main"		/* L2_main name		*/
#define D_L2_THRDNAME_LPB		"L2_lpb" 		/* L2_lpb  name		*/
#define D_L2_THRDNAME_POL		"L2_pol "		/* L2_pol  name		*/
#define D_L2_THRDNAME_TXS		"L2_txs "		/* L2_txs  name		*/
#define D_L2_THRDNAME_RXS		"L2_rxs "		/* L2_rxs  name		*/
#define D_L2_THRDNAME_SEND		"L2_send"		/* L2_send name		*/
#define D_L2_THRDNAME_RECV		"L2_recv"		/* L2_recv name		*/
#define D_L2_THRDNAME_DBG		"L2_dbg"		/* L2_dbg  name		*/

/* @} */

 /*!
 * @note Thread priority
 */
#define D_L2_PRIORITY_MAIN			96/* L2_main priority		*/
#define D_L2_PRIORITY_LPB			77/* L2_lpb  priority		*/
#define D_L2_PRIORITY_POL			78/* L2_pol  priority		*/
#define D_L2_PRIORITY_TXS			78/* L2_txs  priority		*/
#define D_L2_PRIORITY_RXS			78/* L2_rxs  priority		*/
#define D_L2_PRIORITY_SEND			79/* L2_send priority		*/
#define D_L2_PRIORITY_RECV			79/* L2_recv priority		*/
#define D_L2_PRIORITY_DBG			60/* L2_dbg  priority		*/

/* @} */  

 /*!
 * @note Thread stack
 */
 
#define D_L2_STACKSIZE_MAIN	 0		/* L2_main stack		*/
#define D_L2_STACKSIZE_LPB	 0		/* L2_lpb  stack		*/
#define D_L2_STACKSIZE_POL	 0		/* L2_pol  stack		*/
#define D_L2_STACKSIZE_TXS	 0		/* L2_txs  stack		*/
#define D_L2_STACKSIZE_RXS	 0		/* L2_rxs  stack		*/
#define D_L2_STACKSIZE_SEND  0		/* L2_send stack		*/
#define D_L2_STACKSIZE_RECV	 0		/* L2_recv stack		*/
#define D_L2_STACKSIZE_DBG	 0		/* L2_dbg  stack		*/

/* @} */

 /*!
 * @note Thread message number
 */
#define D_L2_MSGQ_MAIN		 1		/* L2_main message number*/
#define D_L2_MSGQ_LPB		 2		/* L2_lpb  message number*/
#define D_L2_MSGQ_POL		 3		/* L2_pol  message number*/
#define D_L2_MSGQ_TXS		 4		/* L2_txs  message number*/
#define D_L2_MSGQ_RXS		 5		/* L2_rxs  message number*/
#define D_L2_MSGQ_SEND		 6		/* L2_send message number*/
#define D_L2_MSGQ_RECV		 7		/* L2_recv message number*/
#define D_L2_MSGQ_DBG		 8		/* L2_dbg message number*/

/* @} */


 /*!
 * @note Thread NO 
 */
#define D_L2_THRDNO_MAIN	   0	/* L2_main thread NO*/
#define D_L2_THRDNO_LPB	 	   1	/* L2_lpb  thread NO*/
#define D_L2_THRDNO_POL	 	   2	/* L2_pol  thread NO*/
#define D_L2_THRDNO_TXS	 	   3	/* L2_txs  thread NO*/
#define D_L2_THRDNO_RXS	 	   4	/* L2_rxs  thread NO*/
#define D_L2_THRDNO_SEND 	   5	/* L2_send thread NO*/
#define D_L2_THRDNO_RECV 	   6	/* L2_recv thread NO*/
#define D_L2_THRDNO_DBG 	   7	/* L2_dbg  thread NO*/
#define D_L2_THRDNO_MAX        8	/* L2 max  thread NO*/
/* @} */

 /*!
 * @note Event NO
 */

/* main event */
#define D_L2_EVTNO_PROCESS_CREATE          0x00
#define D_L2_EVTNO_TASK_INIT_REP           0x01
#define D_L2_EVTNO_SYSTEM_INIT_NOTI        0x02
/* Lpb event */
#define D_L2_EVTNO_L3_TEST_START_NOTI      0x100
#define D_L2_EVTNO_L3_TEST_STOP_NOTI       0x101
/*
#define D_L2_EVTNO_LINK_SETUP_REQ          0x01
#define D_L2_EVTNO_LINK_REL_REQ            0x02
#define D_L2_EVTNO_LINK_DEACT_IND          0x03
*/

/* @} */

 /*!
 * @note Process number
 */
#define D_L2_PROC_NUM 7
/* @} */

 /*!
 * @note Wrong Process ID
 */
#define D_L2_PROCID_GETNG 0xFFFFFFFF
/* @} */

 /*!
 * @note Wrong Thread ID
 */
#define D_L2_THRDID_GETNG 0xFFFFFFFF
/* @} */


 /*!
 * @note Link state
 */
#define D_L2_LINKSTATE_ON		1
#define D_L2_LINKSTATE_OFF		2
#define D_L2_LINKSTATE_ABNORMAL	3
/* @} */


/*!
 * @note maximum of occur counter
 */
#define D_L2_STATI_OCCOUR_MAX		0xFFFFFFFF
/* @} */

/*!
 * @note send and receive flag
 */
#define D_L2_COM_BEFORE    -1
#define D_L2_COM_EQUAL     0
#define D_L2_COM_AFTER     1
/* @} */
 
 
/*!
 * @note send and receive flag
 */
#define D_L2_COM_DIRE_SEND		0
#define D_L2_COM_DIRE_RECV		1
#define D_L2_COM_DIRE_EVNT		2
/* @} */

/*!
 * @note singal index for l3 statistics
 */
#define	 D_L2_STATI_HCREQ				0   /**< ヘルスチェック要�?			*/
#define	 D_L2_STATI_HCRES				1   /**< ヘルスチェック応�?			*/
#define	 D_L2_STATI_RERSTREQ			2   /**< REリセット要求					*/
#define	 D_L2_STATI_RETIMSETREQ    		3   /**< RE時刻設定要求					*/
#define	 D_L2_STATI_LINKSTSREQ			4   /**< RE共通部リンク確立要�?		*/
#define	 D_L2_STATI_LINKSTSRES			5   /**< RE共通部リンク確立応�?		*/
#define	 D_L2_STATI_FIRMFILEREQ			6   /**< ファームウェアファイル報告要�?*/
#define	 D_L2_STATI_FIRMFILERES			7   /**< ファームウェアファイル報告応�?*/
#define	 D_L2_STATI_FILEINFREQ			8   /**< ファイル情報報告通知				*/
#define	 D_L2_STATI_FILEINFRES			9   /**< ファイル情報報告応答				*/
#define	 D_L2_STATI_FILEDATSND			10  /**< ファイルデータ送信				*/
#define	 D_L2_STATI_FILSNDFINNTC		11  /**< ファイルデータ送信完了報告通知	*/
#define	 D_L2_STATI_FILSNDFINRES		12  /**< ファイルデータ送信完了報告応答	*/
#define	 D_L2_STATI_UFIRMFILEREQ		13  /**< 運用中ファームウェアファイル報告要求	*/
#define	 D_L2_STATI_UFIRMFILERES		14  /**< 運用中ファームウェアファイル報告応答	*/
#define	 D_L2_STATI_UFILEINFREQ			15  /**< 運用中ファイル情報報告通知				*/
#define	 D_L2_STATI_UFILEINFRES			16  /**< 運用中ファイル情報報告応�?			*/
#define	 D_L2_STATI_UFILEDATSND			17  /**< 運用中ファイルデータ送信				*/
#define	 D_L2_STATI_UFILSNDFINNTC		18  /**< 運用中ファイルデータ送信完了報告通知	*/
#define	 D_L2_STATI_UFILSNDFINRES		19  /**< 運用中ファイルデータ送信完了報告応答	*/
#define	 D_L2_STATI_TRXSETREQ			20  /**< TRX設定要求						*/
#define	 D_L2_STATI_TRXSETRES			21  /**< TRX設定応答						*/
#define	 D_L2_STATI_TRXRELREQ			22  /**< TRX解放要求						*/
#define	 D_L2_STATI_TRXRELRES			23  /**< TRX解放応答						*/
#define	 D_L2_STATI_TOFFSETINFREQ		24  /**< Toffset値報告要�?			*/
#define	 D_L2_STATI_TOFFSETINFRES		25  /**< Toffset値報告応�?			*/
#define	 D_L2_STATI_CARONOFFREQ			26  /**< 無変調キャリアON/OFF指定要求		*/
#define	 D_L2_STATI_CARONOFFRES			27  /**< 無変調キャリアON/OFF指定応答		*/
#define	 D_L2_STATI_EQPSTRREQ			28  /**< RE装置構成情報報告要求			*/
#define	 D_L2_STATI_EQPSTRRES		    29  /**< RE装置構成情報報告応答			*/
#define	 D_L2_STATI_SLOTINFREQ			30  /**< REスロット情報報告要求			*/
#define	 D_L2_STATI_SLOTINFRES			31  /**< REスロット情報報告応答			*/
#define	 D_L2_STATI_RESTSREQ			32  /**< RE状態報告要求					*/
#define	 D_L2_STATI_RESTSRES			33  /**< RE状態報告応答					*/
#define	 D_L2_STATI_CARDINFREQ			34  /**< REカード情報報告要�?			*/
#define	 D_L2_STATI_CARDINFRES			35  /**< REカード情報報告応�?			*/
#define	 D_L2_STATI_REVERREQ			36  /**< REバージョン報告要�?			*/
#define	 D_L2_STATI_REVERRES			37  /**< REバージョン報告応�?			*/
#define	 D_L2_STATI_RECARDCNTREQ		38  /**< REカード制御要�?				*/
#define	 D_L2_STATI_RECARDCNTRES		39  /**< REカード制御応�?				*/
#define	 D_L2_STATI_CARDSTSREQ			40  /**< REカード状態報告要�?			*/
#define	 D_L2_STATI_CARDSTSRES			41  /**< REカード状態報告応�?			*/
#define	 D_L2_STATI_CARLSTSREQ			42  /**< REキャリア状態報告要求			*/
#define	 D_L2_STATI_CARLSTSRES			43  /**< REキャリア状態報告応答			*/
#define	 D_L2_STATI_REPORTCNTREQ		44  /**< RE PORT制御要求					*/
#define	 D_L2_STATI_REPORTCNTRES		45  /**< RE PORT制御応答					*/
#define	 D_L2_STATI_REPORTSTSREQ		46  /**< RE PORT状態報告要求				*/
#define	 D_L2_STATI_REPORTSTSRES		47  /**< RE PORT状態報告応答				*/
#define	 D_L2_STATI_RECRDDIAREQ 		48  /**< REカード診断要�?				*/
#define	 D_L2_STATI_RECRDDIARES			49  /**< REカード診断報�?				*/
#define	 D_L2_STATI_REOBSLOGREQ			50  /**< RE障害ログ取得要求				*/
#define	 D_L2_STATI_REOBSLOGRES			51  /**< RE障害ログ取得応答				*/
#define	 D_L2_STATI_REOBLOGSTPREQ		52  /**< RE障害ログ取得中止要求			*/
#define	 D_L2_STATI_REOBLOGSTPRES		53  /**< RE障害ログ取得中止応答			*/
#define	 D_L2_STATI_EXTDATSNDREQ		54  /**< 外部装置データ送信要求			*/
#define	 D_L2_STATI_EXTDATSNDRES	    55  /**< 外部装置データ送信応答			*/
#define	 D_L2_STATI_EXTDATRCVREQ		56  /**< 外部装置データ受信要�?		*/
#define	 D_L2_STATI_EXTDATRCVRES		57  /**< 外部装置データ受信応�?		*/
#define	 D_L2_STATI_MTADDSETREQ			58  /**< MT address setting request for 3G	*/
#define	 D_L2_STATI_MTADDSETRES			59  /**< MTアドレス設定応答				*/
#define	 D_L2_STATI_FLDMTCNCREQ			60  /**< FLD-MT接続要求					*/
#define	 D_L2_STATI_FLDMTCNCRES			61  /**< FLD-MT接続応答					*/
#define	 D_L2_STATI_FLDCOMSNDREQ		62  /**< FLD-MTデータコマンド転送送信要求	*/
#define	 D_L2_STATI_FLDCOMSNDRES		63  /**< FLD-MTデータコマンド転送送信応答	*/
#define	 D_L2_STATI_FLDCOMRCVREQ		64  /**< FLD-MTデータコマンド転送受信要�?*/
#define	 D_L2_STATI_FLDCOMRCVRES		65  /**< FLD-MTデータコマンド転送受信応�?*/
#define	 D_L2_STATI_FLDFILSNDREQ		66  /**< FLD-MTデータファイル転送送信要求	*/
#define	 D_L2_STATI_FLDFILSNDRES		67  /**< FLD-MTデータファイル転送送信応答	*/
#define	 D_L2_STATI_FLDFILRCVREQ		68  /**< FLD-MTデータファイル転送受信要�?*/
#define	 D_L2_STATI_FLDFILRCVRES		69  /**< FLD-MTデータファイル転送受信応�?*/
#define	 D_L2_STATI_FLMTCNCRELREQ		70  /**< FLD-MT接続解放要求				*/
#define	 D_L2_STATI_FLMTCNCRELRES		71  /**< FLD-MT接続解放応答				*/
#define	 D_L2_STATI_FLMTCMPRELREQ		72  /**< FLD-MT接続強制解放要求			*/
#define	 D_L2_STATI_FLMTCMPRELRES		73  /**< FLD-MT接続強制解放応答			*/
#define	 D_L2_STATI_SVMTCNTREQ			74  /**< SV-MT接続要求					*/
#define	 D_L2_STATI_SVMTCNTRES			75  /**< SV-MT接続応答					*/
#define	 D_L2_STATI_SVMTDATSNDREQ		76  /**< SV-MTデータ送信要求				*/
#define	 D_L2_STATI_SVMTDATSNDRES		77  /**< SV-MTデータ送信応答				*/
#define	 D_L2_STATI_SVMTDATRCVREQ		78  /**< SV-MTデータ受信要�?			*/
#define	 D_L2_STATI_SVMTDATRCVRES		79  /**< SV-MTデータ受信応�?			*/
#define	 D_L2_STATI_SVMTCNTRELREQ		80  /**< SV-MT接続解放要求				*/
#define	 D_L2_STATI_SVMTCNTRELRES		81  /**< SV-MT接続解放応答				*/
#define	 D_L2_STATI_SVMTFORRELREQ		82  /**< SV-MT接続強制解放要求			*/
#define	 D_L2_STATI_SVMTFORRELRES		83  /**< SV-MT接続強制解放応答			*/
#define	 D_L2_STATI_MKMTDATSND			84  /**< メーカMTデータ送信				*/
#define	 D_L2_STATI_MKMTCNTREQ          85  /**< メーカMT接続要求					*/
#define	 D_L2_STATI_MKMTCNTRES          86  /**< メーカMT接続応答					*/
#define	 D_L2_STATI_MKMTCNTRELREQ       87  /**< メーカMT接続解放要求				*/
#define	 D_L2_STATI_MKMTCNTRELRES       88  /**< メーカMT接続解放応答				*/
#define	 D_L2_STATI_MKMTFORRELREQ       89  /**< メーカMT接続強制解放要求			*/
#define	 D_L2_STATI_MKMTFORRELRES       90  /**< メーカMT接続強制解放応答			*/
#define	 D_L2_STATI_FTPPORTSETREQ       91  /**< FTP データ送信用PORT 番号設定要求	*/
#define	 D_L2_STATI_FTPPORTSETRES       92  /**< FTP データ送信用PORT 番号設定応答	*/
#define  D_L2_STATI_FIRMFILERES2        93  /**< ファームウェアファイル報告応答2	*/
#define	 D_L2_STATI_UFIRMFILERES2		94  /**< 運用中ファームウェアファイル報告応答	*/
/* @} */


/*!
 * @note L2 L3 Send and Receive message Log information number
 */
#define D_L2_TXRV_LOG_INF_NONCYC	512
#define D_L2_TXRV_LOG_INF_NONCYC2	1024
/* @} */

/* USHORT swap */
#define BigLittleSwap16(V)  ((((USHORT)(V) & 0xff00) >> 8) | \
                            (((USHORT)(V) & 0x00ff) << 8))

/* UINT swap */
#define BigLittleSwap32(V)  ((((UINT)(V) & 0xff000000) >> 24) | \
                            (((UINT)(V) & 0x00ff0000) >> 8) | \
                            (((UINT)(V) & 0x0000ff00) << 8) | \
                            (((UINT)(V) & 0x000000ff) << 24))

/*!
 * @note Re-define thread ID
 */

#define D_RRH_THDID_L2_MAIN			0x00000040	/**< L2 Main Thread		*/
#define D_RRH_THDID_L2_LAPB_MNG		0x00000041	/**< LAPB Message Sending (TXIOCS) Thread	*/
#define D_RRH_THDID_L2_POLL			0x00000042	/**< Polling Thread		*/	
#define D_RRH_THDID_L2_LAPB_SEND	0x00000043	/**< LAPB Message Sending (TXIOCS) Thread	*/	
#define D_RRH_THDID_L2_LAPB_RECV	0x00000044	/**< LAPB Message Sending (TXIOCS) Thread	*/	
#define D_RRH_THDID_L2_SEND			0x00000045	/**< L2 CMU send Thread						*/	
#define D_RRH_THDID_L2_RECV			0x00000046	/**< L2 CMU recv  Thread					*/	
#define D_RRH_THDID_L2_DBG			0x00000047	/**< L2 debug  Thread					*/	
#define D_RRH_THDID_L2_MAX			8

#define CMD_TSKID_INI				1        
#define CMD_TSKID_LPB				2			/**< LAPB Management Thread					*/
#define CMD_TSKID_POL				3			/**< Polling Thread							*/
#define CMD_TSKID_TXS				4   		/**< LAPB Message Sending (TXIOCS) Thread	*/
#define CMD_TSKID_RXS				5			/**< LAPB Message Receiving (RXIOCS) Thread	*/
#define CMD_TSKID_SND				6			/**< L2 Main Thread							*/
#define CMD_TSKID_RCV				7			/**< L2 Main Thread							*/
#define CMD_TSKID_DBG				8			/**< L2 Main Thread							*/
#define CMD_TSKID_CPR				D_RRH_PROCID_Q_L3			/**< L2 Main Thread									*/

#define CMD_TSKID_LGMK_L2			21			/**< Log make Thread						*/

/* @} */

											/*********************************/
											/*          L3 message number    */
											/*********************************/
#define	D_L2_L3MSGNUM	256                       /*!< L3 message number       */

#define D_SYS_MSGID_ALLTSKININTC			0x10010007			/**< All thread initialize Notification		*/

#define	 D_L2_ICTL     			0x2                                        /*!< I  Frame        			*/
#define	 D_L2_RRCTL   		    0x3                                        /*!< RR  Frame        		*/
#define	 D_L2_RNRCTL 		    0x4                                        /*!< RNR Frame        		*/
#define	 D_L2_REJCTL		    0x5                                        /*!< REJ Frame        		*/
#define	 D_L2_SBMCTL		    0x6                                        /*!< SABM Frame        		*/
#define	 D_L2_DISCCTL		    0x7                                        /*!< DISC Frame        		*/
#define	 D_L2_DMCTL  		    0x8                                        /*!< DM Frame        			*/
#define	 D_L2_UACTL  		    0x9                                        /*!< UA Frame        			*/
#define	 D_L2_FRMRCTL 		    0xA                                        /*!< FRMR Frame        		*/

#endif
/* @} */
