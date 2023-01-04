/*!
 * @skip    $Id$
 * @file    tm_lb_def.h
 * @brief   LAPB define定義
 * @date    2008.07.22 FFCS)Shihzh create.
 * @date    2013.11.18 FFCS)hongj modify for zynq.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/


/********************************************************************************************************/

#ifndef		TM_LB_DEF
#define		TM_LB_DEF



/****************************************************************************/
/*	state flag definition													*/
/****************************************************************************/
#define	LBD_STT_INIT	0                                                  /*!< initialize state				*/
#define	LBD_STT_NORMAL	1                                                  /*!< normal state					*/
#define	LBD_STT_L2SETUP	2                                                  /*!< L2 Setup						*/

#define			LBD_SIGNAL_MX_STATE		9                                  /*!< single	maximum state number	*/
#define			LBD_SIGNAL_MX_EVENT		27                                 /*!< single link  maximum event number	*/

											/*********************************/
											/*         LAPBタスク状諷?       */
											/*********************************/

#define			LBD_L2_L1ACTSTS 	        0x0                            /*!< L1 ACT･L2 DEACT状態    */
#define			LBD_L2_L2ACTSTS		  		0x1                            /*!< L1 ACT･L2 ACT状態     */
#define			LBD_L2_ESTSTS 			 	0x2                            /*!< Link Set UP 状態      */
#define			LBD_L2_RELSTS		 	  	0x3                            /*!< Link Disconnect 状態   */
#define			LBD_L2_INFSTS 		 	  	0x4                            /*!< Information Transfer状態  */
#define			LBD_L2_ACKSTS		  	  	0x5                            /*!< Waiting Acknowledgement状態*/
#define			LBD_L2_NE_INFSTS 		  	0x6                            /*!< Information Transfer状態(N/E)  */
#define			LBD_L2_NE_ACKSTS  		  	0x7                            /*!< Waiting Acknowledgement状態(N/E) */
#define			LBD_L2_TESTSTS  		  	0x8                            /*!< TEST 状態        */




/****************************************************************************/
/*	LAPB protocol L2 parameter of  DoCoMo specification						*/
/****************************************************************************/
#define			LBD_L2PRMTR_N1		0x280E                                 /*!< N1:10254*/
#define			LBD_L2PRMTR_N2		0x5                                    /*!< N2 */
#define			LBD_L2PRMTR_K		0x7                                    /*!< K  */
/********* 2006/12/06 Modified start by FFCS)Xia for 07A-01-01-011 ************/
#define			LBD_L2PRMTR_T1		150			                          /*!< T1:15*10ms unit	*/
#define			LBD_L2PRMTR_T2		50		                              /*!< T2: 5*10ms unit	*/
/********* 2006/12/06 Modified End ********************************************/
#define			LBD_L2PRMTR_T3		1000		                          /*!< T3:100*10ms unit  */
#define			LBD_L2PRMTR_T4		10000		                          /*!< T4:1000*10ms unit */

											/***************************************/
											/* Layer 2 busy cancel detect timer蛟? */
											/***************************************/
#define			LBD_L2BSYTMRVAL     1000	                              /*!< Layer 2 busy cancel detect timer蛟?100*10ms unit	*/

/* ################# */
/* ### IT3用追蜉?### */
/* ################# */


#define			LBD_L1BSYTMRVAL      	300                                /*!< L1 busy detect timer蛟?30*10ms unit	*/

											/****************************************/
											/*single link internal event number		*/
											/****************************************/

#define	 LBD_L1DATCNF			0x0                                        /*!< L1-DATA-CNF       		*/
#define	 LBD_CPRISNDNTC			0x1                                        /*!< CPRI Sending notification	*/
#define	 LBD_ICTL     			D_L2_ICTL                                  /*!< I  Frame        			*/
#define	 LBD_RRCTL   		    D_L2_RRCTL                                 /*!< RR  Frame        		*/
#define	 LBD_RNRCTL 		    D_L2_RNRCTL                                /*!< RNR Frame        		*/
#define	 LBD_REJCTL		        D_L2_REJCTL                                /*!< REJ Frame        		*/
#define	 LBD_SBMCTL		        D_L2_SBMCTL                                /*!< SABM Frame        		*/
#define	 LBD_DISCCTL		    D_L2_DISCCTL                               /*!< DISC Frame        		*/
#define	 LBD_DMCTL  		    D_L2_DMCTL                                 /*!< DM Frame        			*/
#define	 LBD_UACTL  		    D_L2_UACTL                                 /*!< UA Frame        			*/
#define	 LBD_FRMRCTL 		    D_L2_FRMRCTL                               /*!< FRMR Frame        		*/
#define	 LBD_ERROR  			0xB                                        /*!< ERROR Frame   			*/
#define	 LBD_T1TIMEOUT		 	0xC                                        /*!< T1 time out       		*/
#define	 LBD_T2TIMEOUT		 	0xD                                        /*!< T2 time out       		*/
#define	 LBD_T3TIMEOUT		 	0xE                                        /*!< T3 time out       		*/
#define	 LBD_T4TIMEOUT		 	0xF                                        /*!< T4 time out       		*/
#define	LBD_L2BSYDETTIMEOUT		0x10                                       /*!< L2 busy detect timer time out	*/
#define	 LBD_L2DEACTREQ			0x11                                       /*!< L2 deact request       	*/
#define	 LBD_ESTREQ				0x12                                       /*!< L2 establishment request	*/
#define	 LBD_RELREQ				0x13                                       /*!< L2 release request      	*/
#define	 LBD_L2BSY				0x14                                       /*!< L2 busy happen       	*/
#define	 LBD_L2BSYCLR			0x15                                       /*!< L2 busy cancel       	*/
#define	 LBD_L2STPREQ		 	0x16                                       /*!< L2 stop request       	*/
#define	 LBD_L3TESTSTR			0x17                                       /*!< L3 test start   			*/
#define	 LBD_L3TESTSTP	 		0x18                                       /*!< L3 test stop    			*/
#define	 LBD_L1BSYDETTIMEOUT	0x19                                       /*!< L3 test stop    			*/
#define	 LBD_OTHERS   		    0x1A                                       /*!< OTHERS          			*/

/*four types of error*/
#define	 LBD_ERRORW		        0x100                                      /*!< ERROR Frame (W)    */
#define	 LBD_ERRORX		        0x101                                      /*!< ERROR Frame (X)    */
#define	 LBD_ERRORY  	        0x102                                      /*!< ERROR Frame (Y)    */
#define	 LBD_ERRORZ  			0x103                                      /*!< ERROR Frame (Z)    */


#define	LBD_L2CTL_OFFSET		0x30                                       /*!< offset from TASK_HEAD to disp    	*/

#define	LBD_L2CTL_TXOFFSET		0x32                                       /*!< offset from TASK_HEAD to addres		*/

#define	LBD_FRMRSIZE			0x20                                       /*!< size of  FRMR frame    				*/

											/*********************************/
											/*          フレーム種別         */
											/*********************************/

												/*****************************/
												/*    Control Field 検索逕?  */
												/*****************************/

#define			LBD_L2_TXI       0x1                                           /*!< 送信 I FRAME       */
#define			LBD_L2_TXS       0x2                                           /*!< 送信 S FRAME       */
#define			LBD_L2_TXU       0x3                                           /*!< 送信 U FRAME       */
#define			LBD_L2_TXT       0x4                                           /*!< 送信 Test  FRAME   */
#define			LBD_L2_RXI       0x5                                           /*!< receiving  I FRAME */
#define			LBD_L2_RXS       0x6                                           /*!< receiving S FRAME  */
#define			LBD_L2_RXU       0x7                                           /*!< receiving U FRAME  */



											/*********************************/
											/*         DISP 蛟?              */
											/*********************************/

#define			LBD_L2_DISP      0xxxxx                                        /*!< S/U Frame逕?DISP       */


											/*********************************/
											/*          L3 message number    */
											/*********************************/
											/*********************************/
											/*     統計データカウンタ種蛻?   */
											/*********************************/

//#define			LBD_L2_DOV       0                                             /*!< 受信データ長オーバー発生回数 	*/
#define			LBD_L2_STO       1                                             /*!< 送信完了待ちT.O 発生回数 		*/
//#define			LBD_L2_DRV       2                                             /*!< データ正常受信回謨?    		*/
//#define			LBD_L2_DSD       3                                             /*!< データ正常送信回数     		*/
#define			LBD_L2_INS       4                                             /*!< 無効 Ns 受信回数     		*/
//#define			LBD_L2_ILR       5                                             /*!< イリーガル受信回謨?     	*/
#define			LBD_L2_ITO       6                                             /*!< 情報転送中T.O 発生回数  		*/
//#define			LBD_L2_LTO       7                                             /*!< リンク確遶?切断 T.O 発生回数 	*/
#define			LBD_L2_IRO       8                                             /*!< 情報転送中R.O 発生回数  		*/
//#define			LBD_L2_LRO       9                                             /*!< リンク確遶?切断 R.O 発生回数 	*/
//#define			LBD_L2_RST      10                                             /*!< リンク切譁?リセット 発生回数	*/
#define			LBD_L2_TO3      11                                             /*!< 情報転送中TimerT3 T.O発生回数	*/

//#define			LBD_L2_RXF      12                                             /*!< 受信フレーム総数        	*/
//#define			LBD_L2_TXF      13                                             /*!< 送信フレーム総数        	*/
//#define			LBD_L2_RXO      14                                             /*!< 受信オクテッ繝?総数       	*/
//#define			LBD_L2_TXO      15                                             /*!< 送信オクテッ繝?総数       	*/
//#define			LBD_L2_LRST     16                                             /*!< リン繧?リセット発生回数謨?    */
#define			LBD_L2_RBSY     17                                             /*!< 受信バッファ ビジー発生回謨? 	*/
//#define			LBD_L2_NTXF     18                                             /*!< 再送フレー繝?総数 オクテット総数 */
//#define			LBD_L2_NTXO     19                                             /*!< 再送オクテッ繝?総数      	*/
//#define			LBD_L2_RDTO     20                                             /*!< 受信データオクテット 総数    */
//#define			LBD_L2_SDTO     21                                             /*!< 送信データオクテット 総数    */
//#define			LBD_L2_RI       22                                             /*!< 受信 Iフレーム 総数      	*/
//#define			LBD_L2_SI       23                                             /*!< 送信 Iフレーム 総数      	*/
//#define			LBD_L2_NI       24                                             /*!< 再送 Iフレーム 総数      	*/
//#define			LBD_L2_RS       25                                             /*!< 受信 Sフレーム 総数      	*/
//#define			LBD_L2_SS       26                                             /*!< 送信 Sフレーム 総数      	*/
//#define			LBD_L2_NS       27                                             /*!< 再送 Sフレーム 総数      	*/
//#define			LBD_L2_RU       28                                             /*!< 受信 Uフレーム 総数      	*/
//#define			LBD_L2_SU       29                                             /*!< 送信 Uフレーム 総数      	*/
//#define			LBD_L2_NU       30                                             /*!< 再送 Uフレーム 総数      	*/
#define			LBD_L2_RRNR     31                                             /*!< 受信 RNRフレーム 総数     	*/
#define			LBD_L2_SRNR     32                                             /*!< 送信 RNRフレーム 総数     	*/
#define			LBD_L2_RREJ     33                                             /*!< 受信 REJフレーム 総数     	*/
#define			LBD_L2_SREJ     34                                             /*!< 送信 REJフレーム 総数     	*/
#define			LBD_L2_RDM      35                                             /*!< 受信 DM フレーム 総数     	*/
#define			LBD_L2_SDM      36                                             /*!< 送信 DM フレーム 総数     	*/
#define			LBD_L2_RDSC     37                                             /*!< 受信 DISCフレーム 総数    	*/
#define			LBD_L2_SDSC     38                                             /*!< 送信 DISCフレーム 総数    	*/
#define			LBD_L2_RUA      39                                             /*!< 受信 UA フレーム 総数     	*/
#define			LBD_L2_SUA      40                                             /*!< 送信 UA フレーム 総数     	*/
//#define			LBD_L2_RFMR     41                                             /*!< 受信 FRMRフレーム 総数    	*/
//#define			LBD_L2_SFMR     42                                             /*!< 送信 FRMRフレーム 総数1   	*/
//#define			LBD_L2_SFMR2    43                                             /*!< 送信 FRMRフレーム 総数2   	*/
#define			LBD_L2_RSBM     44                                             /*!< 受信 SABMフレーム 総数    	*/
#define			LBD_L2_SSBM     45                                             /*!< 送信 SABMフレーム 総数    	*/
//#define			LBD_L2_RC       46                                             /*!< 受信 Cフレーム 総数      	*/
//#define			LBD_L2_SC       47                                             /*!< 送信 Cフレーム 総数      	*/
#define			LBD_L2_DSC_I    48                                             /*!< 廃棄フレーム 総数       	*/

#define			LBD_L2_ESTIND_N       49                                       /*!<	データリンク設定表示(データ送受信状態以外)           */
#define			LBD_L2_ESTIND_U       50                                       /*!<	データリンク設定表示(データ送受信状態)       */
#define			LBD_L2_ESTCNF         51                                       /*!<	データリンク設定確認       */
#define			LBD_L2_RELIND_N       52                                       /*!< データリンク解放表示(データ送受信状態以外)        */
#define			LBD_L2_RELIND_U       53                                       /*!< データリンク解放表示(データ送受信状態)   */
#define			LBD_L2_RELCNF         54                                       /*!< データリンク解放確認   */
#define			LBD_L2_INGIND_N       55                                       /*!< データリンク設定中表示(データ送受信状態以外)   */
#define			LBD_L2_INGIND_U       56                                       /*!< データリンク設定中表示(データ送受信状態   */
#define			LBD_L2_DSR2T1O        57                                       /*!N2回のT1タイマリトライアウト(データ送受信状態)   */
#define			LBD_L2_WAI2T1O        58                                       /*!N2回のT1タイマリトライアウト(待ち状態)   */
#define			LBD_L2_T4O            59                                       /*!< T4タイムアウト   */
#define			LBD_L2_RDE            60                                       /*!< L2 DEACT受信   */
#define			LBD_L2_RSTO           61                                       /*!< L2停止要求受信   */
#define			LBD_L2_DSRT1O         62                                       /*!< T1タイムアウト(データ送受信状態)   */
//#define			LBD_L2_WAITIO         63                                       /*!< T1タイムアウト(待ち状態)       */
#define			LBD_L2_IDSC           64                                       /*!< I qeueu破棄フレーム数          */
#define			LBD_L2_BIDSC          65                                       /*!< 自局ビジー中のI fram破棄フレーム数  */
#define			LBD_L2_RSEND          66                                       /*!< REJによる再送フレーム数     */
#define			LBD_L2_RFRMW          67                                       /*!< FRMR受信:ERROR(W)           */
#define			LBD_L2_RFRMX          68                                       /*!< FRMR受信:ERROR(X)           */
#define			LBD_L2_RFRMY          69                                       /*!< FRMR受信:ERROR(Y)           */
#define			LBD_L2_RFRMZ          70                                       /*!< FRMR受信:ERROR(Z)           */
#define			LBD_L2_SFRMW          71                                       /*!< FRMR送信:ERROR(W)           */
#define			LBD_L2_SFRMX          72                                       /*!< FRMR送信:ERROR(X)           */
#define			LBD_L2_SFRMY          73                                       /*!< FRMR送信:ERROR(Y)           */
#define			LBD_L2_SFRMZ          74                                       /*!< FRMR送信:ERROR(Z)           */
//#define			LBD_L2_SRRP1          75                                       /*!< RR(P=1)送信       */
//#define			LBD_L2_SRRF1          76                                       /*!< RR(F=1)送信       */
//#define			LBD_L2_SRRF0          77                                       /*!< RR(F=0)送信       */
#define			LBD_L2_QUENUM         78                                       /*!< モジュロ8(V(S)=V(A)+k)によるQueuing回数       */
#define			LBD_L2_BSRNR          79                                       /*!< 自局ビジー検出によるRNR送信      */
#define			LBD_L2_BCSRR          80                                       /*!< 自局ビジー解除によるRR送信       */
#define			LBD_L2_OBRRNR         81                                       /*!< 他局ビジー検出(RNR受信)          */
#define			LBD_L2_OBCRRR         82                                       /*!< 他局ビジー解除(RR/REJ受信)       */
#define			LBD_L2_NS2END         83                                       /*!< 2回目NS抜けは、V(R)<N(S)<=V(M) [V(M)は初回NS抜け検出時のNS値]       */

#define			LBD_L2_STSNUM         84                                       /*!< */
											/******************************************/
											/*   リンク異常統計データ カウンタ 種別   */
											/******************************************/

#define			LBD_L2_ABRDM       0                                           /*!< SABM 送信蠕?DM受信回数  		*/
#define			LBD_L2_ABSDM       1                                           /*!< SABM 受信蠕?DM送信回数  		*/
#define			LBD_L2_ABIFM       2                                           /*!< 未定義フレーム受菫?     		*/
#define			LBD_L2_ABSUF       3                                           /*!< 異常S/Uフレーム長受信回数  		*/
#define			LBD_L2_ABNRF       4                                           /*!< N(R)誤りフレーム 受信回数   	*/
#define			LBD_L2_ABNOF       5                                           /*!< N1 オーバー フレーム 受信回数  	*/
#define			LBD_L2_ABUNF       6                                           /*!< 非期待フレー繝?受信回数    		*/
#define			LBD_L2_ABRSF       7                                           /*!< FRMR 受信回数       			*/
#define			LBD_L2_ABUFU       8                                           /*!< 無効フレーム 受信回数(未定義長)  	*/
#define			LBD_L2_ABUFA       9                                           /*!< 無効フレーム 受信回数(アドレス異常) */

#define			LBD_L2_ABNUM	   10                                          /*!< 無効受信回数 */

#define			LBD_L2_FMRLOGNUM   100                                         /*!< FRMR log number*/


/****************************************************************************/
/* 走行履歴情報テーブル逕?												*/
/****************************************************************************/
#define			LBD_RUNHIS_FACENUM_MAX	1024                               /*!< 面数MAX */
#define			LBD_KEYRUNHIS_FACENUM_MAX	LBD_RUNHIS_FACENUM_MAX/CMD_NUM6/*!< 面数MAX */
#define			LBD_RUNHIS_FLG_RCV		0x00                               /*!< 送受信フラグ(受信)   */
#define			LBD_RUNHIS_FLG_SND		0x01                               /*!< 送受信フラグ(送信)   */
#define			LBD_RUNHIS_IFRAME_MAX	16                                 /*!< MAX I frame number in I queue */

#endif/*LB_DEF*/

