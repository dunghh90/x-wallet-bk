/*!
 * @skip  $ld:$
 * @file  f_sfp_main.c
 * @brief pf_sfp メイン処理
 * @date  2018/08/16 Fujitsu)Taniguchi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_SFP
 * @{
 */

#include "f_sfp_inc.h"

UINT	f_sfpw_thdState;
UINT	f_sfpw_monState[D_SFP_MAXNUM];
UINT	f_sfpw_monTimingCount;
UINT	f_sfpw_logflg = D_RRH_LOG_AST_LV_ERROR;

UINT	f_sfpw_reg_sfpsta;							/* レジスタ読出し値(CTB_SFPSTA)	*/
UINT	f_sfpw_failsafe1_flag       = D_RRH_OFF;	/* RU未実装FailSafe条件発生フラグ(条件①) ON:発生/OFF:未発生	*/
UINT	f_sfpw_failsafe2_flag       = D_RRH_OFF;	/* RU未実装FailSafe条件発生フラグ(条件②) ON:発生/OFF:未発生	*/
UINT	f_sfpw_failsafe3_flag       = D_RRH_OFF;	/* RU未実装FailSafe条件発生フラグ(条件③) ON:発生/OFF:未発生	*/
UINT	f_sfpw_failsafe3_cnt        = 0;			/* リンク断発生回数(条件③)	*/
UINT	f_sfpw_failsafe3_recv_cnt   = 0xFFFFFFFF;	/* リンク確立連続回数(条件③復旧)	*/

UINT	f_sfpw_FaultNtf_stop_flag   = 0xFFFFFFFF;	/* Fault通知抑止フラグ ON:通知なし/OFF:通知あり/all-F:判定前	*/
UINT	f_sfpw_FaultNtf_stop_cnt    = 0;			/* Fault通知抑止時間カウンタ	*/

UINT	f_sfpw_FaultNtf_send_flag   = D_RRH_OFF;	/* Fault通知送信中フラグ	*/
UINT	f_sfpw_FaultNtf_recover_cnt = 0;			/* Fault通知復旧カウンタ	*/

UINT	*f_sfpw_fhfsOffFlg;							/* SFP Fail Safe ON/OFF Flag (共有メモリ)	*/


/* SFP データ取得マップ */
/***************************************************
 * SFP 取得データマッピング
 ****************************************************
1	0	3	8	0	6	6	Transceiver	0
--
2	0	12	12	1	1		BR, Nominal	6
	0	13	13	0	1	2	Rate Identifier	7
--
3	0	20	35	1	16		Vendor name	8
	0	36	36	1	1		Transceiver	24
	0	37	39	1	3		Vendor OUI	25
	0	40	55	1	16		Vendor PN	28
	0	56	59	0	4	40	Vendor rev	44
--
4	0	68	83	1	16		Vendor SN	48
	0	84	91	1	8		Date code	64
	0	92	92	1	1		Diagnostic Monitoring Type	72
	0	93	93	0	1	26	Enhanced Options	73
--
5	0	96	127	0	32	32	Vendor Specific	74
-
6	1	0	39	0	40	40	Thresholds	106
*******************************************************/
typedef	struct	{
	int		a0a2			;	/* 0:a0/1:a2 				*/
	int		st				;	/* 開始アドレス 			*/
	int		sz				;	/* 取得サイズ				*/
	int		wp				;	/* ログバッファ書き込み位置	*/
} T_HDL_SFP_DATA_MAP;
static T_HDL_SFP_DATA_MAP	sfpDataMap[]	=	{
	{	0,	3,		8,		0		},	/* Transceiver0(6)					*/
	{	0,	12,		2,		8		},	/* BR, Nominal(1), Rate Identifier(1)		*/
	{	0,	20,		40,		10		},	/* Vendor name(16), Transceiver2(1), Vendor OUI(3), Vendor PN(16), Vendor rev(4)	計40byte 	*/
	{	0,	68,		26,		50		},	/* Vendor SN(16), Date code(8), Diagnostic Monitoring Type(1), Enhanced Options(1)	計73byte	*/
	{	0,	96,		32,		76		},	/* Vendor Specific(74)																計74byte	*/
	{	1,	0,		40,		108		},	/* Thresholds																					*/	
};

#define D_SFP_FAILSAFE_AB_CNT_MAX			32					/* フェールセーフABカウンタ上限 */
#define D_SFP_FAILSAFE_ABCD_CNT_MAX			8					/* フェールセーフABCDカウンタ上限 */

/**
 * @brief	SFP Type情報
 * @note	SFP Type情報の定義
 * 
 */
typedef struct{
	CHAR	name[11];							/* SFP の 図番						*/
	UINT	trxInfType;							/* タイプ							*/
	UINT	emphasis;							/* emphasis							*/
}T_SFP_TYPE_TBL;


/* SFPタイプ情報 */
const T_SFP_TYPE_TBL f_sfpr_sfpType[ 3 ] =
{
	/* SFP図番確認出来ない異常ケース */
	{ "FCMO025210",0xE ,0x00100004},	/* E-type:10K */
	{ "FCMO025110",0xF ,0x00100004},	/* F-type:30K */
	{ ""          ,0x0 ,0x00100004}	/* unknown */
};


/*!
 * @brief 関数機能概要: pf_sfp ＳＦＰ情報読み込み。ＳＦＰから情報を読み込む
 * @note  関数処理内容:
 *        -# ＳＦＰから各情報を読み込む
 * @param    	port				0:Primary / 1:Secondary
 * @parm		data				char[146]	sfp取得データ
 * @return   	D_RRH_OK / D_RRH_NG
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2018/08/22 Taniguchi Create
 */
VOID f_sfp_read(USHORT mountstate, UINT port )	{

	int				mapindex	=	0		;
	int				endcd		=	D_RRH_OK;
	T_RRH_SFPINFO	sfpInfo;
	UCHAR*			sfpdata		=	sfpInfo.sfpinf;
	
	memset(&sfpInfo,0,sizeof(sfpInfo));

	/*  SFP情報更新 */
	sfpInfo.fefver     = (USHORT)f_comw_rrhVersion;
	sfpInfo.mountstate = mountstate;
	switch(mountstate){
		case D_RRH_SFP_MOUNT:
			for( mapindex = 0 ; mapindex < 6 ; mapindex++ )	{
				if( port == 0 )	{
					endcd = BPF_HM_DEVC_SFP_READ_BUFFER(
								(sfpDataMap[mapindex].a0a2 == 0)? BPF_HM_DEVC_SFP_A0: BPF_HM_DEVC_SFP_A2,	/* A0/A2			*/
								sfpDataMap[mapindex].st,													/* 開始アドレス		*/
								sfpDataMap[mapindex].sz,													/* 読み込みサイズ	*/
								(UCHAR*)(sfpdata + sfpDataMap[mapindex].wp)									/* 書込みポジション	*/
						);
				}
				else {
					endcd = BPF_HM_DEVC_SFP_READ_BUFFER( 
								(sfpDataMap[mapindex].a0a2 == 0)? BPF_HM_DEVC_SFP_SA0: BPF_HM_DEVC_SFP_SA2,	/* SA0/SA2			*/
								sfpDataMap[mapindex].st,													/* 開始アドレス		*/
								sfpDataMap[mapindex].sz,													/* 読み込みサイズ	*/
								(UCHAR*)(sfpdata + sfpDataMap[mapindex].wp)									/* 書込みポジション	*/
							);
				}
				if( endcd != BPF_HM_DEVC_COMPLETE )	{
					BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d mapindex:%d ret:0x%08x ", port, mapindex, endcd );
					memset(sfpInfo.sfpinf,0xff,sizeof(sfpInfo.sfpinf));
					break;
				}
			}
			break;
		case D_RRH_SFP_UNMOUNT:
		default:
			break;
	}
	BPF_COM_LOG_SFP_INFO_STORE( port, &sfpInfo );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "SFP info get state:%d port:%d", mountstate,port );
	return;
}


/*!
 * @brief 関数機能概要: pf_sfp M-Plane報告用のSFP情報読み込み。
 * @note  関数処理内容:
 *        -# SFPから各情報を読み込む
 * @param		N/A
 * @return   	N/A
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2018/08/22 Taniguchi Create
 * @date  2021/03/03 M&C)DuongMX Update SFP Management
 */
VOID f_sfp_monitor( UINT sfppwfault_count ) {

	INT						i, j;
	INT						error_cnt = 0;
	INT						endcd;
	INT						errcd;

	UINT					sfp_a0_addr[D_SFP_MAXNUM] = {BPF_HM_DEVC_SFP_A0, BPF_HM_DEVC_SFP_A0, BPF_HM_DEVC_SFP_A0};
	UINT					sfp_a2_addr[D_SFP_MAXNUM] = {BPF_HM_DEVC_SFP_A2, BPF_HM_DEVC_SFP_A2, BPF_HM_DEVC_SFP_A2};

	T_COM_TRANSCEIVER_INFO	work[D_SFP_MAXNUM];
	UCHAR					sfp_char_data[4];
	SHORT					sfp_sht_data;
	USHORT					sfp_usht_data;

	struct bpf_timespec		dtime;	/* sem waittime */	


	dtime.tv_sec  = 0;
	dtime.tv_nsec = BPF_RU_IPCM_WAITTIME_LOG;

	/* work領域を初期化	*/
	memcpy( (VOID *)work, (VOID *)&f_comw_transcelver_mon_tbl.info[0], sizeof(work) );

	/* 状態変化がない場合、20秒に1回だけ収集	*/
	if(f_comw_transcelver_mon_tbl.info[0].present == f_sfpw_monState[0]) {
		f_sfpw_monTimingCount++;
	} else {
		f_sfpw_monTimingCount = D_SFP_MONITOR_TIMING;
	}
	if( f_sfpw_monTimingCount < D_SFP_MONITOR_TIMING ) {
		return;
	}
	f_sfpw_monTimingCount = 0;

	/* SFP数分繰返し	*/
	for( i = 0; i < D_SFP_MAXNUM; i++ ) {

		if( (f_sfpw_monState[i] == D_COM_SFP_MON_STATE_MOUNT)
		 || (f_sfpw_monState[i] == D_COM_SFP_MON_STATE_MOUNT_CHG) ) {

			work[i].present = D_RRH_ON;

			if( f_sfpw_monState[i] == D_COM_SFP_MON_STATE_MOUNT_CHG ) {

				/* Vendor OUI	*/
				endcd = BPF_HM_DEVC_SFP_READ_BUFFER( sfp_a0_addr[i],							/* A0/A2			*/
													 D_DU_REG_SFP_A0_VENDOR_OUI,				/* 開始アドレス		*/
													 D_DU_SIZE_SFP_A0_VENDOR_OUI,				/* 読み込みサイズ	*/
													(UCHAR *)&sfp_char_data[0] );				/* 書込みポジション	*/
				if( endcd != BPF_HM_DEVC_COMPLETE ) {
					BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d, adr:%02d/%04d ret:0x%08x",
										i, sfp_a0_addr[i], D_DU_REG_SFP_A0_VENDOR_OUI, endcd );
					error_cnt++;
				}
				/* 3byteデータを後ろ詰め	*/
				memset( &work[i].vendor_id[0], '\0', 8 );
				sprintf( (char *)&work[i].vendor_id[0], "%02x%02x%02x", sfp_char_data[0], sfp_char_data[1], sfp_char_data[2] );

				/* Vendor PN	*/
				memset( &work[i].vendor_part[0], '\0', 20 );
				endcd = BPF_HM_DEVC_SFP_READ_BUFFER( sfp_a0_addr[i],							/* A0/A2			*/
													 D_DU_REG_SFP_A0_VENDOR_PN,					/* 開始アドレス		*/
													 D_DU_SIZE_SFP_A0_VENDOR_PN,				/* 読み込みサイズ	*/
													(UCHAR *)&work[i].vendor_part[0] );			/* 書込みポジション	*/
				if( endcd != BPF_HM_DEVC_COMPLETE ) {
					BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d, adr:%02d/%04d ret:0x%08x",
										i, sfp_a0_addr[i], D_DU_REG_SFP_A0_VENDOR_PN, endcd );
					error_cnt++;
				}
				for( j = (D_DU_SIZE_SFP_A0_VENDOR_PN - 1); j >= 0; j-- ) {
					if( work[i].vendor_part[j] == 0x20 ) {
						work[i].vendor_part[j] = 0x00;
					} else {
						break;
					}
				}

				/* Vendor rev	*/
				memset( &work[i].vendor_rev[0], '\0', 8 );
				endcd = BPF_HM_DEVC_SFP_READ_BUFFER( sfp_a0_addr[i],							/* A0/A2			*/
													 D_DU_REG_SFP_A0_VENDOR_REV,				/* 開始アドレス		*/
													 D_DU_SIZE_SFP_A0_VENDOR_REV,				/* 読み込みサイズ	*/
													(UCHAR *)&work[i].vendor_rev[0] );			/* 書込みポジション	*/
				if( endcd != BPF_HM_DEVC_COMPLETE ) {
					BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d, adr:%02d/%04d ret:0x%08x",
										i, sfp_a0_addr[i], D_DU_REG_SFP_A0_VENDOR_REV, endcd );
					error_cnt++;
				}
				for( j = (D_DU_SIZE_SFP_A0_VENDOR_REV - 1); j >= 0; j-- ) {
					if( work[i].vendor_rev[j] == 0x20 ) {
						work[i].vendor_rev[j] = 0x00;
					} else {
						break;
					}
				}

				/* Vendor SN	*/
				memset( &work[i].serial_no[0], '\0', 20 );
				endcd = BPF_HM_DEVC_SFP_READ_BUFFER( sfp_a0_addr[i],							/* A0/A2			*/
													 D_DU_REG_SFP_A0_VENDOR_SN,					/* 開始アドレス		*/
													 D_DU_SIZE_SFP_A0_VENDOR_SN,				/* 読み込みサイズ	*/
													(UCHAR *)&work[i].serial_no[0] );			/* 書込みポジション	*/
				if( endcd != BPF_HM_DEVC_COMPLETE ) {
					BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d, adr:%02d/%04d ret:0x%08x",
										i, sfp_a0_addr[i], D_DU_REG_SFP_A0_VENDOR_SN, endcd );
					error_cnt++;
				}
				for( j = (D_DU_SIZE_SFP_A0_VENDOR_SN - 1); j >= 0; j-- ) {
					if( work[i].serial_no[j] == 0x20 ) {
						work[i].serial_no[j] = 0x00;
					} else {
						break;
					}
				}

				/* SFF-8472 Compliance	*/
				endcd = BPF_HM_DEVC_SFP_READ_BUFFER( sfp_a0_addr[i],							/* A0/A2			*/
													 D_DU_REG_SFP_A0_SFF8472_COMP,				/* 開始アドレス		*/
													 D_DU_SIZE_SFP_A0_SFF8472_COMP,				/* 読み込みサイズ	*/
													(UCHAR *)&work[i].SFF8472_compliance_code );	/* 書込みポジション	*/
				if( endcd != BPF_HM_DEVC_COMPLETE ) {
					BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d, adr:%02d/%04d ret:0x%08x",
										i, sfp_a0_addr[i], D_DU_REG_SFP_A0_SFF8472_COMP, endcd );
					error_cnt++;
				}

				/* Connector	*/
				endcd = BPF_HM_DEVC_SFP_READ_BUFFER( sfp_a0_addr[i],							/* A0/A2			*/
													 D_DU_REG_SFP_A0_CONNECTOR,					/* 開始アドレス		*/
													 D_DU_SIZE_SFP_A0_CONNECTOR,				/* 読み込みサイズ	*/
													(UCHAR *)&work[i].connector_type );			/* 書込みポジション	*/
				if( endcd != BPF_HM_DEVC_COMPLETE ) {
					BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d, adr:%02d/%04d ret:0x%08x",
										i, sfp_a0_addr[i], D_DU_REG_SFP_A0_CONNECTOR, endcd );
					error_cnt++;
				}

				/* BR, Nominal	*/
				endcd = BPF_HM_DEVC_SFP_READ_BUFFER( sfp_a0_addr[i],							/* A0/A2			*/
													 D_DU_REG_SFP_A0_BR_NOMINAL,				/* 開始アドレス		*/
													 D_DU_SIZE_SFP_A0_BR_NOMINAL,				/* 読み込みサイズ	*/
													(UCHAR *)&work[i].nominal_bitrate );		/* 書込みポジション	*/
				if( endcd != BPF_HM_DEVC_COMPLETE ) {
					BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d, adr:%02d/%04d ret:0x%08x",
										i, sfp_a0_addr[i], D_DU_SIZE_SFP_A0_BR_NOMINAL, endcd );
					error_cnt++;
				}

				/* BR, min	*/
				endcd = BPF_HM_DEVC_SFP_READ_BUFFER( sfp_a0_addr[i],							/* A0/A2			*/
													 D_DU_REG_SFP_A0_BR_MIN,					/* 開始アドレス		*/
													 D_DU_SIZE_SFP_A0_BR_MIN,					/* 読み込みサイズ	*/
													(UCHAR *)&work[i].low_bitrate_margin );		/* 書込みポジション	*/
				if( endcd != BPF_HM_DEVC_COMPLETE ) {
					BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d, adr:%02d/%04d ret:0x%08x",
										i, sfp_a0_addr[i], D_DU_REG_SFP_A0_BR_MIN, endcd );
					error_cnt++;
				}

				/* BR, max	*/
				endcd = BPF_HM_DEVC_SFP_READ_BUFFER( sfp_a0_addr[i],							/* A0/A2			*/
													 D_DU_REG_SFP_A0_BR_MAX,					/* 開始アドレス		*/
													 D_DU_SIZE_SFP_A0_BR_MAX,					/* 読み込みサイズ	*/
													(UCHAR *)&work[i].high_bitrate_margin );	/* 書込みポジション	*/
				if( endcd != BPF_HM_DEVC_COMPLETE ) {
					BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d, adr:%02d/%04d ret:0x%08x",
										i, sfp_a0_addr[i], D_DU_REG_SFP_A0_BR_MAX, endcd );
					error_cnt++;
				}

				/* Diagnostic Monitoring Type	*/
				endcd = BPF_HM_DEVC_SFP_READ_BUFFER( sfp_a0_addr[i],							/* A0/A2			*/
													 D_DU_REG_SFP_A0_DIAG_MON_TYPE,				/* 開始アドレス		*/
													 D_DU_SIZE_SFP_A0_DIAG_MON_TYPE,			/* 読み込みサイズ	*/
													(UCHAR *)&work[i].rx_power_type );			/* 書込みポジション	*/
				if( endcd != BPF_HM_DEVC_COMPLETE ) {
					BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d, adr:%02d/%04d ret:0x%08x",
										i, sfp_a0_addr[i], D_DU_REG_SFP_A0_DIAG_MON_TYPE, endcd );
					error_cnt++;
				}
				work[i].rx_power_type = (work[i].rx_power_type & 0x08) >> 3;
			}

			/* RX Power	*/
			endcd = BPF_HM_DEVC_SFP_READ_BUFFER( sfp_a2_addr[i],								/* A0/A2			*/
												 D_DU_REG_SFP_A2_RX_POW,						/* 開始アドレス		*/
												 D_DU_SIZE_SFP_A2_RX_POW,						/* 読み込みサイズ	*/
												(UCHAR *)&sfp_char_data[0] );					/* 書込みポジション	*/
			if( endcd != BPF_HM_DEVC_COMPLETE ) {

				BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d, adr:%02d/%04d ret:0x%08x",
									i, sfp_a2_addr[i], D_DU_REG_SFP_A2_RX_POW, endcd );
				error_cnt++;
			}
			sfp_usht_data = (USHORT)((sfp_char_data[0] << 8) | sfp_char_data[1]);
			work[i].rx_power = (DL)sfp_usht_data;
			work[i].rx_power = work[i].rx_power / 10000;

			/* TX Power	*/
			endcd = BPF_HM_DEVC_SFP_READ_BUFFER( sfp_a2_addr[i],								/* A0/A2			*/
												 D_DU_REG_SFP_A2_TX_POW,						/* 開始アドレス		*/
												 D_DU_SIZE_SFP_A2_TX_POW,						/* 読み込みサイズ	*/
												(UCHAR *)&sfp_char_data[0] );					/* 書込みポジション	*/
			if( endcd != BPF_HM_DEVC_COMPLETE ) {

				BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d, adr:%02d/%04d ret:0x%08x",
									i, sfp_a2_addr[i], D_DU_REG_SFP_A2_TX_POW, endcd );
				error_cnt++;
			}
			sfp_usht_data = (USHORT)((sfp_char_data[0] << 8) | sfp_char_data[1]);
			work[i].tx_power = (DL)sfp_usht_data;
			work[i].tx_power = work[i].tx_power / 10000;

			/* TX Bias	*/
			endcd = BPF_HM_DEVC_SFP_READ_BUFFER( sfp_a2_addr[i],								/* A0/A2			*/
												 D_DU_REG_SFP_A2_TX_BIAS,						/* 開始アドレス		*/
												 D_DU_SIZE_SFP_A2_TX_BIAS,						/* 読み込みサイズ	*/
												(UCHAR *)&sfp_char_data[0] );					/* 書込みポジション	*/
			if( endcd != BPF_HM_DEVC_COMPLETE ) {

				BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d, adr:%02d/%04d ret:0x%08x",
									i, sfp_a2_addr[i], D_DU_REG_SFP_A2_TX_BIAS, endcd );
				error_cnt++;
			}
			sfp_usht_data = (USHORT)((sfp_char_data[0] << 8) | sfp_char_data[1]);
			work[i].tx_bias_current = (DL)sfp_usht_data;
			work[i].tx_bias_current = work[i].tx_bias_current / 500;

			/* Vcc	*/
			endcd = BPF_HM_DEVC_SFP_READ_BUFFER( sfp_a2_addr[i],								/* A0/A2			*/
												 D_DU_REG_SFP_A2_VCC,							/* 開始アドレス		*/
												 D_DU_SIZE_SFP_A2_VCC,							/* 読み込みサイズ	*/
												(UCHAR *)&sfp_char_data[0] );					/* 書込みポジション	*/
			if( endcd != BPF_HM_DEVC_COMPLETE ) {

				BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d, adr:%02d/%04d ret:0x%08x",
									i, sfp_a2_addr[i], D_DU_REG_SFP_A2_VCC, endcd );
				error_cnt++;
			}
			sfp_usht_data = (USHORT)((sfp_char_data[0] << 8) | sfp_char_data[1]);
			work[i].voltage = (DL)sfp_usht_data;
			work[i].voltage = work[i].voltage / 10;

			/* Temperature	*/
			endcd = BPF_HM_DEVC_SFP_READ_BUFFER( sfp_a2_addr[i],								/* A0/A2			*/
												 D_DU_REG_SFP_A2_TEMP,							/* 開始アドレス		*/
												 D_DU_SIZE_SFP_A2_TEMP,							/* 読み込みサイズ	*/
												(UCHAR *)&sfp_char_data[0] );					/* 書込みポジション	*/
			if( endcd != BPF_HM_DEVC_COMPLETE ) {

				BPF_COM_LOG_ASSERT( f_sfpw_logflg, "error BPF_HM_DEVC_SFP_READ_BUFFER port:%d, adr:%02d/%04d ret:0x%08x",
									i, sfp_a2_addr[i], D_DU_REG_SFP_A2_TEMP, endcd );
				error_cnt++;
			}
			sfp_sht_data = (SHORT)((sfp_char_data[0] << 8) | sfp_char_data[1]);
			work[i].temperature = (DL)sfp_sht_data;
			work[i].temperature = work[i].temperature / 256;

		} else {

			work[i].present = D_RRH_OFF;
			memset( &work[i], '\0', sizeof(T_COM_TRANSCEIVER_INFO) );
		}
	}

	/* Take semphore of SFP monitor information */
	endcd = BPF_RU_IPCM_PROCSEM_TAKE( E_RRH_SEMID_APL_SFP_INFO, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, &dtime, &errcd );
	if(endcd != BPF_RU_IPCM_OK) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "SFP monitor information sem take error rtn_bpf=0x%08x errcd=0x%08x", endcd, errcd );
	}

	f_comw_transcelver_mon_tbl.error_cnt = error_cnt;
	/* SFP Read error未検出	*/
	if( error_cnt == 0 ) {
		memcpy( (VOID *)&f_comw_transcelver_mon_tbl.info[0], (VOID *)work, sizeof(T_COM_TRANSCEIVER_INFO) );
	} else {
		f_comw_transcelver_mon_tbl.info[0].present = work[0].present;
		memset( work, 0, sizeof(work) );
	}

	/* Give semphore of SFP monitor information */
	endcd =  BPF_RU_IPCM_PROCSEM_GIVE( E_RRH_SEMID_APL_SFP_INFO, BPF_RU_IPCM_LOCK_RW, &errcd );
	if( endcd != BPF_RU_IPCM_OK ) {

		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "SFP monitor information sem give error rtn_bpf=0x%08x errcd=0x%08x", endcd, errcd);
	}

	/* SFP未実装時,SFP電源断時はFault検出時に即回復する */
	/* SFP電源ON中のみ判定を行う */
	if(sfppwfault_count == 0){
		if(error_cnt == 0){
			f_com_SVCtl(E_DU_SVCTL_OPE_FID50_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_SFP_ACCESS_ERR);
			f_sfpw_logflg = D_RRH_LOG_AST_LV_ERROR;
		}else{
			f_com_SVCtl(E_DU_SVCTL_OPE_FID50_ALM, D_DU_SVCTL_ON, D_DU_FLTID_SFP_ACCESS_ERR);

			/* RU未実装FailSafe条件発生フラグ(条件②)をON	*/
			f_sfpw_failsafe2_flag = D_RRH_ON;

			if(f_sfpw_logflg == D_RRH_LOG_AST_LV_ERROR){
				BPF_COM_LOG_ASSERT( f_sfpw_logflg, "Fault Raise SFP0 ACCESS ERROR(%d)", error_cnt);
				f_sfpw_logflg = D_RRH_LOG_AST_LV_ENTER;
			}
		}

		//* SV-BusのSFP未実装の設定を行う */
		f_com_SVCtl(E_DU_SVCTL_OPE_FLDL, D_DU_SVCTL_OFF, D_DU_SVCTL_ON_SFP_NOT_IMPL);
		BPF_COM_LOG_ASSERT( f_sfpw_logflg, "Fault Raise SFP SV-Bus ACCESS ERROR(%d)", error_cnt);
	}
	return;
}


/*!
 * @brief 関数機能概要:FHSTATLOGを取得する
 * @note  関数処理内容:
 *        -# SFPから各情報を読み込む
 * @param		N/A
 * @return   	N/A
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2019/04/12 Taniguchi Create
 */
VOID f_sfp_fhstat_log(UINT regval_info , T_RRH_LOG_FHSTAT_HIS* his_p )
{

	UINT	index;
	UINT	regval_ctl = D_RRH_ON;
	UINT	regval_addr;
	UINT	count;

	if( his_p->linkinfo_pre != regval_info ){
		if(regval_info == D_RRH_OFF){
			/* LINK UPで統計1回初期化 */
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_DU_REG_ETH_STATLAT,&regval_ctl);
		}
	}

	his_p->linkinfo_pre = regval_info;
	his_p->counter++;
	if(his_p->counter < 60 ){
		return;
	}
	his_p->counter = 0;

	/* 1分に１回クリアする（duコマンドと競合するが良しとする） */
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_POL,D_DU_REG_ETH_STATLAT,&regval_ctl);

	index = his_p->uiWriteIdx;

	his_p->tRecord[index].sysTime.year = 0;	/* 無効データ */
	his_p->tRecord[index].linkinfo      = regval_info;

	regval_addr = D_DU_REG_ETH_RXUPLANE;
	for( count=0 ;count < D_DU_FHSTATLOG_MAX ;count++){
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regval_addr + (count * 4) ,&(his_p->tRecord[index].regdata[count]));
	}

	BPF_HM_DEVC_GETTIME(&(his_p->tRecord[index].sysTime));

	index++;
	index %= D_DU_FHSTATLOG_RCD_NUM;
	his_p->uiWriteIdx = index;
	return;

}

static T_RRH_LOG_FH_L1_HIS_RECORD f_sfpw_fhl1his_pre_recode;

/*!
 * @brief 関数機能概要:FHL1 HISLOGを取得する
 * @note  関数処理内容:FHL1 HISLOGを取得する
 * @param		N/A
 * @return   	N/A
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2019/07/19 Taniguchi Create
 */
VOID f_sfp_fhl1his_log(UINT regval_info , T_RRH_LOG_FH_L1_HIS* his_p )
{

	UINT	index;
	UINT	index_chg;

	index = his_p->uiWriteIdx;
	his_p->tRecord[index].sysTime.year = 0;	/* 無効データ */
	his_p->tRecord[index].link_state      = regval_info;
	his_p->tRecord[index].version         = f_comw_rrhVersion;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_ETH_HISMACST ,&(his_p->tRecord[index].regdata[0]));
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SFPSTA   ,&(his_p->tRecord[index].regdata[1]));
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_JCPLLALM ,&(his_p->tRecord[index].regdata[2]));
	BPF_HM_DEVC_GETTIME(&(his_p->tRecord[index].sysTime));

	/* Fault通知抑止中の場合、300秒間waitする	*/
	if( f_sfpw_FaultNtf_stop_flag == D_RRH_ON ) {

		/* 300秒間カウント	*/
		f_sfpw_FaultNtf_stop_cnt++;
		if( f_sfpw_FaultNtf_stop_cnt > D_SFP_FAULTNTF_STOP_MAXCNT ) {

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "RU Unimplemented Fail Safe : Fault Notification send start" );

			/* Fault抑止を解除する	*/
			f_sfpw_FaultNtf_stop_flag = D_RRH_OFF;
			f_sfpw_FaultNtf_stop_cnt  = 0;
		}
	}
	/* 条件③ ： Link状態チェック	*/
	if( (regval_info & 0x00000001) == 0x00000000 ) {	/* リンク確立	*/
		/* Link確立の場合、Link断回数を初期化	*/
		f_sfpw_failsafe3_cnt = 0;

		/* 復旧カウンタ更新中チェック	*/
		if( f_sfpw_failsafe3_recv_cnt != 0xFFFFFFFF ) {
			f_sfpw_failsafe3_recv_cnt++;	/* リンク確立の場合、カウンタ更新	*/
		}
	} else {	/* リンク断	*/
		/* Link断の場合、Link断回数をcount up	*/
		f_sfpw_failsafe3_cnt++;

		if( f_sfpw_failsafe3_cnt > D_SFP_FAILSAFE3_MAXCNT ) {
			/* RU未実装FailSafe条件発生フラグ(条件③)をON	*/
			f_sfpw_failsafe3_flag = D_RRH_ON;
			f_sfpw_failsafe3_cnt = 0;
		}

		/* 復旧カウンタ更新中チェック	*/
		if( f_sfpw_failsafe3_recv_cnt != 0xFFFFFFFF ) {
			f_sfpw_failsafe3_recv_cnt = 0;	/* リンク断の場合、カウンタクリア	*/
		}
	}

	/* LINK変化 */
	if(
		( regval_info == 0xFFFFFFFF ) || 
		( f_sfpw_fhl1his_pre_recode.link_state != regval_info) || 
		( f_sfpw_fhl1his_pre_recode.regdata[0] != his_p->tRecord[index].regdata[0] ) || 
		( f_sfpw_fhl1his_pre_recode.regdata[1] != his_p->tRecord[index].regdata[1] ) || 
		( f_sfpw_fhl1his_pre_recode.regdata[2] != his_p->tRecord[index].regdata[2] )
	){
		index_chg = his_p->uiWriteIdx_chg;
		memcpy(&(his_p->tRecord_chg[index_chg]),&(his_p->tRecord[index]),sizeof(his_p->tRecord_chg[index_chg]));
		index_chg++;
		index_chg %= D_DU_LOG_FH_L1_HIS_RCD_NUM;
		his_p->uiWriteIdx_chg = index_chg;

		/* Link断→Link確立の場合、通知抑止期間：300秒のカウントを開始する	*/
		if( ((regval_info & 0x00000001) == 0x00000000) && (f_sfpw_FaultNtf_stop_flag == 0xFFFFFFFF) ) {
			f_sfpw_FaultNtf_stop_flag = D_RRH_ON;
		}
	}
	memcpy(&f_sfpw_fhl1his_pre_recode,&(his_p->tRecord[index]),sizeof(f_sfpw_fhl1his_pre_recode));

	index++;
	index %= D_DU_LOG_FH_L1_HIS_RCD_NUM;
	his_p->uiWriteIdx = index;

	return;

}



/*!
 * @brief 関数機能概要:pf_hdlメインスレッド。起動からMSG受信待ちのLoop処理を実施
 * @note  関数処理内容.
 *		-# 初期化タスク状態を"初期化状態"に設定
 *		-# hdl無い各IRQ待ち受けスレッド生成			f_hdl_init()        関数Call
 *		-# MSG待ち無限Loop処理へ移行				f_com_msgQReceive() 関数Call
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2018/06/22 Fujitsu)吉田 Create
 * @date 2021/03/03 M&C)DuongMX Update SFP Management
 */
VOID f_sfp_main( VOID )
{
	T_SYS_INIT_COMP_NTC*	sndMsg;							/* 送信メッセージ(初期化完了通知)	*/
	INT				endcd		= 0;
	INT				errcd		= 0;
	UINT			regVal;
	UINT			mountWaitCount[D_SFP_MAXNUM];
	UINT			cur_mount_port[D_SFP_MAXNUM];
	UINT			pre_mount_port[D_SFP_MAXNUM];
	UINT			sfp_txen;
	UINT			linkCount;
	UINT			count;
	UINT			sfpumfault_count[D_SFP_MAXNUM];
	UINT			sfppwfault_count[D_SFP_MAXNUM];
	UINT			sfpumfault_cease_count[D_SFP_MAXNUM];
	UINT			sfppwfault_cease_count[D_SFP_MAXNUM];
	UCHAR			sfp_data;
	UCHAR			sfp_name[16];
	size_t			sfp_namelen;
	UCHAR			vendor_name[D_DU_SIZE_SFP_A0_VENDOR_NAME + 1];
	UCHAR			serinal_name[D_DU_SIZE_SFP_A0_VENDOR_SN + 1];

	UINT			critical_ber_count;
	UINT			critical_ber_cease_count;
	UINT			critical_ber_fault;

	UINT			uiFlagPtn;

	T_RRH_LOG_FHSTAT_HIS*			his_p = NULL;
	UINT regval_info;
	T_RRH_LOG_FH_L1_HIS*			fhl1_p = NULL;

	INT				rtn_bpf = 0;
	UCHAR			eep_data;

	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "pf_sfp", 0, 0, 0);

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET( D_RRH_PROCID_PF,					/* ProcessID		*/
								 D_SYS_THDID_PF_SFP,				/* ThreadID			*/
								 D_SYS_THDQID_PF_SFP,				/* ThreadQueueID	*/
								 (unsigned long int)pthread_self());	/* pThreadID	*/

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_sfp_main] ENTER" );
	
	f_sfpw_thdState = D_SFP_THD_STS_INIT;
	
	/****************************************************************************************************************/
	/* 初期化完了通知送信																							*/
	/****************************************************************************************************************/
	endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof(T_SYS_INIT_COMP_NTC),			/* MessageSize						*/
										(VOID **)&sndMsg,						/* MessageBuffer					*/
										&errcd );								/* ErrorCode						*/
	if( endcd != BPF_RU_IPCM_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " ADDRGET NG %d %d ", endcd ,errcd);
		return;
	}

	sndMsg->head.uiEventNo		= D_SYS_MSGID_INIT_COMP_NTC;					/* メッセージID						*/
	sndMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;								/* 返信先PQID						*/
	sndMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_MAIN;							/* 送信先TQID						*/
	sndMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;								/* 送信元PQID						*/
	sndMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_SFP;							/* 送信元TQID						*/

	endcd = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MAIN,						/* 送信MSGキューID					*/
							(VOID *)sndMsg );									/* 送信MSGポインタ					*/
	if( endcd != BPF_RU_IPCM_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " MSGQ_SEND NG %d ", endcd );
		return;
	}
	
	f_sfpw_thdState = D_SFP_THD_STS_ACT;
	memset(pre_mount_port,0,sizeof(pre_mount_port));
	memset(cur_mount_port,0,sizeof(cur_mount_port));
	memset(mountWaitCount,0,sizeof(mountWaitCount));
	memset(sfpumfault_count,0,sizeof(sfpumfault_count));
	memset(sfppwfault_count,0,sizeof(sfppwfault_count));
	memset(sfpumfault_cease_count,0,sizeof(sfpumfault_cease_count));
	memset(sfppwfault_cease_count,0,sizeof(sfppwfault_cease_count));

	memset(f_sfpw_monState,0xFF,sizeof(f_sfpw_monState));

	memset( (UCHAR *)&f_comw_transcelver_mon_tbl, 0, sizeof(f_comw_transcelver_mon_tbl) );
	f_comw_transcelver_mon_tbl.info[0].present = D_COM_SFP_MON_STATE_INIT;

	f_sfpw_monTimingCount = D_SFP_MONITOR_TIMING;

	critical_ber_count = D_RRH_OFF;
	critical_ber_cease_count = D_RRH_OFF;
	critical_ber_fault  = D_RRH_OFF;

	(void)BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_FH_STAT,(VOID**)&his_p, &errcd);
	his_p->linkinfo_pre = D_RRH_ON;	/* LINK down */

	(void)BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_FH_L1_HIS,(VOID**)&fhl1_p, &errcd);
	if(fhl1_p->uiWriteIdx >= D_DU_LOG_FH_L1_HIS_RCD_NUM){
		fhl1_p->uiWriteIdx = 0;
	}
	if(fhl1_p->uiWriteIdx_chg >= D_DU_LOG_FH_L1_HIS_RCD_NUM){
		fhl1_p->uiWriteIdx_chg = 0;
	}
	memset(&f_sfpw_fhl1his_pre_recode,0xFF,sizeof(f_sfpw_fhl1his_pre_recode));

	(void)BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_SFPFS_FLG, (VOID **)&f_sfpw_fhfsOffFlg, &errcd);
	*f_sfpw_fhfsOffFlg = D_RRH_OFF;

	/* 初期化 */
	f_sfp_fhl1his_log(0xFFFFFFFF,fhl1_p);

	/* 統計情報初期化 */
	regVal = D_RRH_ON;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_DU_REG_ETH_STATLAT,&regVal);

	/* チェック完了後通常のループ開始 */
	while(1){
		sfp_txen = 0;

		f_sfpw_reg_sfpsta = 0;
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SFPSTA, &f_sfpw_reg_sfpsta );

		for(linkCount=0 ; linkCount < D_SFP_MAXNUM ; linkCount++ ){
			pre_mount_port[linkCount] = cur_mount_port[linkCount];
			cur_mount_port[linkCount] = f_sfpw_reg_sfpsta & (1 << linkCount);
			/* unMount -> Mount */
			if((cur_mount_port[linkCount] != D_RRH_OFF) && (pre_mount_port[linkCount] == D_RRH_OFF)){
				if(mountWaitCount[linkCount] < D_SFP_COUNT_SFPMOUNT ){
					cur_mount_port[linkCount] = pre_mount_port[linkCount] = 0;
				}
				else{
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Detected SFP%d Mount", linkCount );
					BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_HDL_READ_WRITE,D_DU_REG_CTB_SFPPSSD,cur_mount_port[linkCount]);
					f_com_taskDelay(D_SFP_TIMEVAL_SFPGUARD,0,0,0);
					/* TBD SFP RATE SELL */
					sfp_txen |= cur_mount_port[linkCount];
				}
				mountWaitCount[linkCount]++;
			}
			/* Mount -> unMount */
			else if ((cur_mount_port[linkCount] == D_RRH_OFF) && (pre_mount_port[linkCount] != D_RRH_OFF)){
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Detected SFP%d UnMount", linkCount );
				BPF_HM_DEVC_REG_WRITE_BITON( D_RRH_LOG_REG_LV_HDL_READ_WRITE,D_DU_REG_CTB_SFPPSSD,pre_mount_port[linkCount]);
				BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_HDL_READ_WRITE,D_DU_REG_CTB_SFPTXEN,pre_mount_port[linkCount]);
				mountWaitCount[linkCount] = 0;
				f_sfpw_monState[linkCount] = D_COM_SFP_MON_STATE_UNMOUNT;
				f_sfp_read(D_RRH_SFP_UNMOUNT,linkCount);
				f_com_SVCtl(E_DU_SVCTL_OPE_FID50_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_SFP_TYP_ERR);
				f_com_SVCtl(E_DU_SVCTL_OPE_FID50_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_SFP_ACCESS_ERR);
				f_sfpw_logflg = D_RRH_LOG_AST_LV_ERROR;
				f_com_mtCtl_sfpInfo(0xFF,NULL,NULL);


			}else{
				/* do nothing */
			}
		}

		/* 1sec wait */
		for(count=0;count < D_SFP_POLL_COUNT; count++){
			BPF_HM_DEVC_REG_READ_BIT( D_RRH_LOG_REG_LV_HDL_READ, D_DU_REG_CTB_SFPSTA, 0xF , &f_sfpw_reg_sfpsta ) ;
			for(linkCount=0 ; linkCount < D_SFP_MAXNUM ; linkCount++ ){
				/* SFP unmount? */
				if(( f_sfpw_reg_sfpsta & (1 << linkCount)) != 0){
					if(sfpumfault_cease_count[linkCount] < 2){
						sfpumfault_cease_count[linkCount]++;
					}
					sfpumfault_count[linkCount] = 0;
				}else{
					if(sfpumfault_count[linkCount] < 2){
						sfpumfault_count[linkCount]++;
					}
					sfpumfault_cease_count[linkCount] = 0;
				}
				switch(sfpumfault_count[linkCount]){
					case 0:
						if(sfpumfault_cease_count[linkCount] == 2){
							f_com_SVCtl(E_DU_SVCTL_OPE_FID05_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_SFP_UNMOUNT_FH1);
							BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Fault Cease SFP%d UnMount(0x%08x)", linkCount ,f_sfpw_reg_sfpsta);
							sfpumfault_cease_count[linkCount] = 3;
							sfppwfault_count[linkCount] = 0;
							sfppwfault_cease_count[linkCount] = 0;
						}
						if(mountWaitCount[linkCount] > D_SFP_COUNT_SFPMOUNT){
							/* SFP Power Down? */
							/*	port#0=0xA000_0300[2],port#1=0xA000_0300[3] => port=0xA000_0300[1]に変更	*//*	ハソ変更v1.09	*/
							if(( f_sfpw_reg_sfpsta & (1 << (linkCount+1))) == 0){
								if(sfppwfault_cease_count[linkCount] < 2){
									sfppwfault_cease_count[linkCount]++;
								}
								sfppwfault_count[linkCount] = 0;
							}
							else{
								if(sfppwfault_count[linkCount] < 2){
									sfppwfault_count[linkCount]++;
								}
								sfppwfault_cease_count[linkCount]= 0;
							}
							switch(sfppwfault_count[linkCount]){
								case 0:
									if(sfppwfault_cease_count[linkCount] == 2){
										f_com_SVCtl(E_DU_SVCTL_OPE_FID06_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_SFP_PDOWN_FH1);
										sfppwfault_cease_count[linkCount] = 3;
										BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Fault Cease SFP%d PowerDOWN(0x%08x)", linkCount ,f_sfpw_reg_sfpsta);
									}
									break;
								case 2:
									f_com_SVCtl(E_DU_SVCTL_OPE_FID06_ALM, D_DU_SVCTL_ON, D_DU_FLTID_SFP_PDOWN_FH1);
									f_com_SVCtl(E_DU_SVCTL_OPE_FID50_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_SFP_ACCESS_ERR);
									f_sfpw_logflg = D_RRH_LOG_AST_LV_ERROR;
									sfppwfault_count[linkCount] = 3;
									BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Fault Raise SFP%d PowerDOWN(0x%08x)", linkCount ,f_sfpw_reg_sfpsta);
									break;
								default:
									break;
							}
						}
						break;
					case 2:
						f_com_SVCtl(E_DU_SVCTL_OPE_FID05_ALM, D_DU_SVCTL_ON, D_DU_FLTID_SFP_UNMOUNT_FH1);
						BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Fault Raise SFP%d UnMount(0x%08x)", linkCount ,f_sfpw_reg_sfpsta);
						sfpumfault_count[linkCount] = 3;
						f_com_SVCtl(E_DU_SVCTL_OPE_FID06_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_SFP_PDOWN_FH1);
						BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Fault Cease SFP%d PowerDOWN(0x%08x)", linkCount ,f_sfpw_reg_sfpsta);
						sfppwfault_count[linkCount] = 0;
						sfppwfault_cease_count[linkCount] = 0;
						break;
					default:
						break;
				}
			}
			f_com_taskDelay(D_SFP_TIMEVAL_POLL,0,0,0);
		}

		if( sfp_txen != D_RRH_OFF ){
			
			sfp_data=0;
			BPF_HM_DEVC_SFP_READ(BPF_HM_DEVC_SFP_A2,D_RRH_SFP_A0_RX_RATE_SEL_RS0,&sfp_data);
			sfp_data |= 0x08;
			BPF_HM_DEVC_SFP_WRITE(BPF_HM_DEVC_SFP_A2,D_RRH_SFP_A0_RX_RATE_SEL_RS0,&sfp_data);
			
			sfp_data=0;
			BPF_HM_DEVC_SFP_READ(BPF_HM_DEVC_SFP_A2,D_RRH_SFP_A0_RX_RATE_SEL_RS1,&sfp_data);
			sfp_data |= 0x08;
			BPF_HM_DEVC_SFP_WRITE(BPF_HM_DEVC_SFP_A2,D_RRH_SFP_A0_RX_RATE_SEL_RS1,&sfp_data);

			memset(sfp_name,0,sizeof(sfp_name));
			memset(vendor_name,0,sizeof(vendor_name));
			memset(serinal_name,0,sizeof(serinal_name));

			BPF_HM_DEVC_SFP_READ_BUFFER( BPF_HM_DEVC_SFP_A0,D_RRH_SFP_A0_VENDOR_SPEC,12,(UCHAR *)sfp_name );
			BPF_HM_DEVC_SFP_READ_BUFFER( BPF_HM_DEVC_SFP_A0,D_DU_REG_SFP_A0_VENDOR_NAME,D_DU_SIZE_SFP_A0_VENDOR_NAME,(UCHAR *)vendor_name );
			BPF_HM_DEVC_SFP_READ_BUFFER( BPF_HM_DEVC_SFP_A0,D_DU_REG_SFP_A0_VENDOR_SN,D_DU_SIZE_SFP_A0_VENDOR_SN,(UCHAR *)serinal_name );

			sfp_namelen = strlen((const char*)sfp_name);
			if(sfp_namelen >= 10){
				for(count=0;count < 2;count++){
					if(!strncmp((const char *)sfp_name,(const char *)(f_sfpr_sfpType[count].name),10)){
						BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Known SFP Detected:%s type-%X l:%d",sfp_name,f_sfpr_sfpType[count].trxInfType,sfp_namelen);
						f_com_mtCtl_sfpInfo(f_sfpr_sfpType[count].trxInfType,vendor_name,serinal_name);
						break;
					}
				}
				if(count >= 2){
					f_com_SVCtl(E_DU_SVCTL_OPE_FID50_ALM, D_DU_SVCTL_ON, D_DU_FLTID_SFP_TYP_ERR);
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "UnKnown SFP Detected:%s type-?? l:%d",sfp_name,sfp_namelen);
					f_com_mtCtl_sfpInfo(f_sfpr_sfpType[2].trxInfType,vendor_name,serinal_name);
				}
			}else{
				f_com_SVCtl(E_DU_SVCTL_OPE_FID50_ALM, D_DU_SVCTL_ON, D_DU_FLTID_SFP_TYP_ERR);
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "UnKnown SFP Detected:%s type-?? l:%d",sfp_name,sfp_namelen);
				f_com_mtCtl_sfpInfo(f_sfpr_sfpType[2].trxInfType,vendor_name,serinal_name);
			}

			BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_HDL_READ_WRITE,D_DU_REG_CTB_SFPTXEN,sfp_txen);
			for(linkCount=0 ; linkCount < D_SFP_MAXNUM ; linkCount++){
				if((sfp_txen & (1 << linkCount)) != D_RRH_OFF){
					f_sfp_read(D_RRH_SFP_MOUNT,linkCount);
					f_sfpw_monState[linkCount] = D_COM_SFP_MON_STATE_MOUNT_CHG;
				}
			}
		}

		/* SFPモニター情報読出し	*/
		for(linkCount=0 ; linkCount < D_SFP_MAXNUM ; linkCount++){
			f_sfp_monitor(sfppwfault_count[linkCount]);
			if( f_sfpw_monState[linkCount] == D_COM_SFP_MON_STATE_MOUNT_CHG ) {
				f_sfpw_monState[linkCount] = D_COM_SFP_MON_STATE_MOUNT;
			}
		}
		
		(void)BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, D_DU_REG_ETH_IRQMACSTI, D_RRH_ON , &regval_info); 
		if(regval_info == 0){
			(void)BPF_HM_DEVC_REG_READ_BIT( D_RRH_LOG_REG_LV_HDL_READ, D_DU_REG_ETH_HISRXST, 0x4 , &regVal ) ;
			if(regVal != 0){
				if(critical_ber_count == 10){
					f_com_SVCtl(E_DU_SVCTL_OPE_FID05_ALM, D_DU_SVCTL_ON, D_DU_FLTID_BER_DET_FH1);
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Fault Raise Critical BER detected(0x%08x 0x%08x 0x%x)", critical_ber_count,critical_ber_cease_count,regVal);
					critical_ber_fault = D_RRH_ON;
				}
				critical_ber_cease_count = 0;
				critical_ber_count++;
				if(critical_ber_count > 10){
					critical_ber_count = 11;
				}
			}else{
				if((critical_ber_fault == D_RRH_ON) && (critical_ber_cease_count == 5)){
					f_com_SVCtl(E_DU_SVCTL_OPE_FID05_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_BER_DET_FH1);
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Fault Cease Critical BER detected(0x%08x 0x%08x 0x%x)", critical_ber_count,critical_ber_cease_count,regVal);
					critical_ber_fault= D_RRH_OFF;
				}
				critical_ber_count = 0;
				critical_ber_cease_count++;
				if(critical_ber_cease_count > 5){
					critical_ber_cease_count = 6;
				}
			}
		}else{
			/* LOS発生中は復旧	*/
			if(critical_ber_fault == D_RRH_ON){
				f_com_SVCtl(E_DU_SVCTL_OPE_FID05_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_BER_DET_FH1);
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Fault Cease Critical BER detected(0x%08x 0x%08x 0x%x)", critical_ber_count,critical_ber_cease_count,regVal);
				critical_ber_fault= D_RRH_OFF;
			}
			critical_ber_count = 0;
			critical_ber_cease_count = 0;
		}
		
		/* FHSTAT log */
		f_sfp_fhstat_log(regval_info , his_p);
		/* FHL1 LOG */
		f_sfp_fhl1his_log(regval_info, fhl1_p);

		if( (f_sfpw_reg_sfpsta & 0x00000005) == 0x00000005 ) {
			/* RU未実装FailSafe条件発生フラグ(条件①)をON	*/
			f_sfpw_failsafe1_flag = D_RRH_ON;
		}

		/* RU未実装フェールセーフ処理 - MEMO:SFP#0のみを実装する	*/
		if( *f_sfpw_fhfsOffFlg == D_RRH_OFF ) {	/* ユーザーによる抑止指定なし	*/

			/* SFP Mountチェック	*/
			if( (f_sfpw_reg_sfpsta & 0x00000001) == 0x00000001 ) {

				if( f_sfpw_FaultNtf_send_flag == D_RRH_ON ) {

					if( (f_sfpw_failsafe1_flag == D_RRH_ON) || (f_sfpw_failsafe2_flag == D_RRH_ON) ) {
						uiFlagPtn = 1;	/* 自律リセット		*/
					} else if( f_sfpw_failsafe3_flag == D_RRH_ON ) {
						f_sfpw_failsafe3_recv_cnt = 0;
						uiFlagPtn = 2;	/* SFP電源OFF→ON	*/
					} else {
						uiFlagPtn = 3;	/* 復旧カウンタ更新	*/
					}
				} else {
					if( (f_sfpw_failsafe1_flag == D_RRH_ON) || (f_sfpw_failsafe2_flag == D_RRH_ON) ) {
						uiFlagPtn = 2;	/* SFP電源OFF→ON	*/
					} else if( f_sfpw_failsafe3_flag == D_RRH_ON ) {
						f_sfpw_failsafe3_recv_cnt = 0;
						uiFlagPtn = 2;	/* SFP電源OFF→ON	*/
					} else {
						uiFlagPtn = 0;
					}
				}

				switch( uiFlagPtn ) {
					case 1:	/* 自律リセット		*/
						/* ABカウンタが上限か */
						if(f_cmw_tra_inv_tbl->cmx_sfp_failsafe_ab_cnt > D_SFP_FAILSAFE_AB_CNT_MAX)
						{
							break;
						}
						/* ABCDカウンタが上限か */
						if(f_cmw_tra_inv_tbl->cmx_sfp_failsafe_abcd_cnt > D_SFP_FAILSAFE_ABCD_CNT_MAX)
						{
							break;
						}
						/* ABCDカウンタ更新(+1) */
						eep_data = f_cmw_tra_inv_tbl->cmx_sfp_failsafe_abcd_cnt + 1;
						rtn_bpf = BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_SFP_FAILSAFE_ABCD_CNT, &eep_data);
						if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
						{
							/* エラー */
							BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "EEP-ROM Write Error(%04x) rtn_bpf=%08x", D_DU_EEP_SFP_FAILSAFE_ABCD_CNT, rtn_bpf);
						}
						f_cmw_tra_inv_tbl->cmx_sfp_failsafe_abcd_cnt = eep_data;
						/* ABカウンタ更新(-1) */
						eep_data = f_cmw_tra_inv_tbl->cmx_sfp_failsafe_ab_cnt;
						/* 念のため0だったら減算しない */
						if(eep_data > 0)
						{
							eep_data--;
							rtn_bpf = BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_SFP_FAILSAFE_AB_CNT, &eep_data);
							if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
							{
								/* エラー */
								BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "EEP-ROM Write Error(%04x) rtn_bpf=%08x", D_DU_EEP_SFP_FAILSAFE_AB_CNT, rtn_bpf);
							}
							f_cmw_tra_inv_tbl->cmx_sfp_failsafe_ab_cnt = eep_data;
						}
						else
						{
							BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "RU Unimplemented Fail Safe : Self Reset, AB Counter Underflow");
						}

						BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "RU Unimplemented Fail Safe : Self Reset, term1=%d, term2=%d, term3=%d ab=%d abcd=%d",
											f_sfpw_failsafe1_flag, f_sfpw_failsafe2_flag, f_sfpw_failsafe3_flag,
											f_cmw_tra_inv_tbl->cmx_sfp_failsafe_ab_cnt, f_cmw_tra_inv_tbl->cmx_sfp_failsafe_abcd_cnt);
						BPF_COM_LOG_DMESG( "RU Unimplemented Fail Safe : Self Reset, term1=%d, term2=%d, term3=%d ab=%d abcd=%d",
											f_sfpw_failsafe1_flag, f_sfpw_failsafe2_flag, f_sfpw_failsafe3_flag,
											f_cmw_tra_inv_tbl->cmx_sfp_failsafe_ab_cnt, f_cmw_tra_inv_tbl->cmx_sfp_failsafe_abcd_cnt);
						BPF_RM_SVRM_SIGNAL_SEND(D_RRH_PROCID_INI, SIGUSR2, D_RRH_SIGUSR2_OPT_CPRIL1RST );
						break;

					case 2:	/* SFP電源OFF→ON	*/
						/* ABカウンタが上限か */
						if(f_cmw_tra_inv_tbl->cmx_sfp_failsafe_ab_cnt > D_SFP_FAILSAFE_AB_CNT_MAX) {
							break;
						}
						/* ABカウンタ更新(+1) */
						eep_data = f_cmw_tra_inv_tbl->cmx_sfp_failsafe_ab_cnt + 1;
						rtn_bpf = BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_SFP_FAILSAFE_AB_CNT, &eep_data);
						if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
						{
							/* エラー */
							BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "EEP-ROM Write Error(%04x) rtn_bpf=%08x", D_DU_EEP_SFP_FAILSAFE_AB_CNT, rtn_bpf);
						}
						f_cmw_tra_inv_tbl->cmx_sfp_failsafe_ab_cnt = eep_data;

						BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "RU Unimplemented Fail Safe : power supply OFF->ON, term1=%d, term2=%d, term3=%d ab=%d abcd=%d", 
											f_sfpw_failsafe1_flag, f_sfpw_failsafe2_flag, f_sfpw_failsafe3_flag,
											f_cmw_tra_inv_tbl->cmx_sfp_failsafe_ab_cnt, f_cmw_tra_inv_tbl->cmx_sfp_failsafe_abcd_cnt);

						/* SFP TXD設定	*/
						BPF_HM_DEVC_REG_WRITE_BITOFF( D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_SFPTXEN, 1 );

						/* SFP LDO(電源) OFF	*/
						BPF_HM_DEVC_REG_WRITE_BITON ( D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_SFPPSSD, 1 );
						f_com_taskDelay( (D_SFP_TIMEVAL_FAILSAFE * 2), 0, 0, 0 );	/* 2s wait	*/

						/* SFP LDO(電源) ON	*/
						BPF_HM_DEVC_REG_WRITE_BITOFF( D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_SFPPSSD, 1 );
						f_com_taskDelay( D_SFP_TIMEVAL_FAILSAFE, 0, 0, 0 );			/* 1s wait	*/

						/* SFPインベントリ情報取得	*/
						memset(sfp_name,0,sizeof(sfp_name));
						memset(vendor_name,0,sizeof(vendor_name));
						memset(serinal_name,0,sizeof(serinal_name));

						BPF_HM_DEVC_SFP_READ_BUFFER( BPF_HM_DEVC_SFP_A0,D_RRH_SFP_A0_VENDOR_SPEC,12,(UCHAR *)sfp_name );
						BPF_HM_DEVC_SFP_READ_BUFFER( BPF_HM_DEVC_SFP_A0,D_DU_REG_SFP_A0_VENDOR_NAME,D_DU_SIZE_SFP_A0_VENDOR_NAME,(UCHAR *)vendor_name );
						BPF_HM_DEVC_SFP_READ_BUFFER( BPF_HM_DEVC_SFP_A0,D_DU_REG_SFP_A0_VENDOR_SN,D_DU_SIZE_SFP_A0_VENDOR_SN,(UCHAR *)serinal_name );

						sfp_namelen = strlen((const char*)sfp_name);
						if(sfp_namelen >= 10){
							for(count=0;count < 2;count++){
								if(!strncmp((const char *)sfp_name,(const char *)(f_sfpr_sfpType[count].name),10)){
									BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Known SFP Detected:%s type-%X l:%d",sfp_name,f_sfpr_sfpType[count].trxInfType,sfp_namelen);
									f_com_mtCtl_sfpInfo(f_sfpr_sfpType[count].trxInfType,vendor_name,serinal_name);
									break;
								}
							}
							if(count >= 2){
								f_com_SVCtl(E_DU_SVCTL_OPE_FID50_ALM, D_DU_SVCTL_ON, D_DU_FLTID_SFP_TYP_ERR);
								BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "UnKnown SFP Detected:%s type-?? l:%d",sfp_name,sfp_namelen);
								f_com_mtCtl_sfpInfo(f_sfpr_sfpType[2].trxInfType,vendor_name,serinal_name);
							}
						}else{
							f_com_SVCtl(E_DU_SVCTL_OPE_FID50_ALM, D_DU_SVCTL_ON, D_DU_FLTID_SFP_TYP_ERR);
							BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "UnKnown SFP Detected:%s type-?? l:%d",sfp_name,sfp_namelen);
							f_com_mtCtl_sfpInfo(f_sfpr_sfpType[2].trxInfType,vendor_name,serinal_name);
						}

						/* SFP Soft RX rate select(RS0)	*/
						endcd    = BPF_HM_DEVC_SFP_READ ( BPF_HM_DEVC_SFP_A2, D_DU_REG_SFP_A2_STAT_CONT, (UCHAR *)&sfp_data );
						sfp_data = (sfp_data & 0xF7) | 0x08;
						endcd   += BPF_HM_DEVC_SFP_WRITE( BPF_HM_DEVC_SFP_A2, D_DU_REG_SFP_A2_STAT_CONT, (UCHAR *)&sfp_data );
						if( endcd != BPF_HM_DEVC_COMPLETE ) {
							BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "error RU Unimplemented Fail Safe : SFP Access Error, Addr=%02x, endcd=%08x", D_DU_REG_SFP_A2_STAT_CONT, endcd );
						}

						/* SFP Soft RX rate select(RS1)	*/
						endcd    = BPF_HM_DEVC_SFP_READ ( BPF_HM_DEVC_SFP_A2, D_DU_REG_SFP_A2_EXT_STAT_CONT, (UCHAR *)&sfp_data );
						sfp_data = (sfp_data & 0xF7) | 0x08;
						endcd   += BPF_HM_DEVC_SFP_WRITE( BPF_HM_DEVC_SFP_A2, D_DU_REG_SFP_A2_EXT_STAT_CONT, (UCHAR *)&sfp_data );
						if( endcd != BPF_HM_DEVC_COMPLETE ) {
							BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "error RU Unimplemented Fail Safe : SFP Access Error, Addr=%02x, endcd=%08x", D_DU_REG_SFP_A2_STAT_CONT, endcd );
						}

						/* SFP TXEN設定	*/
						BPF_HM_DEVC_REG_WRITE_BITON ( D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_SFPTXEN, 1 );

						/* フェールセーフ関連パラメータを初期化する	*/
						f_sfpw_failsafe1_flag = D_RRH_OFF;	/* RU未実装FailSafe条件発生フラグ(条件①)	*/
						f_sfpw_failsafe2_flag = D_RRH_OFF;	/* RU未実装FailSafe条件発生フラグ(条件②)	*/
						f_sfpw_failsafe3_flag = D_RRH_OFF;	/* RU未実装FailSafe条件発生フラグ(条件③)	*/

						/* Fault通知発動	*/
						if( f_sfpw_FaultNtf_send_flag == D_RRH_OFF ) {
							f_com_SVCtl(E_DU_SVCTL_OPE_FID51_NOALM, D_DU_SVCTL_ON, D_DU_FLTID_SFP_FAILSAFE);
							if( f_sfpw_FaultNtf_stop_flag == D_RRH_OFF ) {
								f_com_SVCtl(E_DU_SVCTL_OPE_FID51_ALM, D_DU_SVCTL_ON, D_DU_FLTID_SFP_FAILSAFE_NTF);
							}
						}
						f_sfpw_FaultNtf_send_flag   = D_RRH_ON;
						f_sfpw_FaultNtf_recover_cnt = 0;
						break;

					case 3:	/* 復旧カウンタ更新	*/

						if( f_sfpw_failsafe3_recv_cnt == 0xFFFFFFFF ) {

							f_sfpw_FaultNtf_recover_cnt++;
							if( f_sfpw_FaultNtf_recover_cnt <= D_SFP_FAILSAFE_RECOVER_MAXCNT ) {
								break;
							}
						} else {
							
							if( f_sfpw_failsafe3_recv_cnt <= D_SFP_FAILSAFE_RECOVER_MAXCNT ) {
								break;
							}
						}
						f_sfpw_FaultNtf_recover_cnt = 0;
						f_sfpw_failsafe3_recv_cnt = 0xFFFFFFFF;

						BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "RU Unimplemented Fail Safe : recover, term1=%d, term2=%d, term3=%d ab=%d abcd=%d",
											f_sfpw_failsafe1_flag, f_sfpw_failsafe2_flag, f_sfpw_failsafe3_flag,
											f_cmw_tra_inv_tbl->cmx_sfp_failsafe_ab_cnt, f_cmw_tra_inv_tbl->cmx_sfp_failsafe_abcd_cnt);
						f_com_SVCtl(E_DU_SVCTL_OPE_FID51_NOALM, D_DU_SVCTL_OFF, D_DU_FLTID_SFP_FAILSAFE);
						if( f_sfpw_FaultNtf_stop_flag == D_RRH_OFF ) {
							f_com_SVCtl(E_DU_SVCTL_OPE_FID51_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_SFP_FAILSAFE_NTF);
						}
						f_sfpw_FaultNtf_send_flag = D_RRH_OFF;
						break;

					default:
						break;
				}
			} else {		/* Unmount	*/

				/* SVレジスタは復旧に設定する	*/
				if( f_sfpw_FaultNtf_send_flag == D_RRH_ON ) {
					f_com_SVCtl(E_DU_SVCTL_OPE_FID51_NOALM, D_DU_SVCTL_OFF, D_DU_FLTID_SFP_FAILSAFE);
					if( f_sfpw_FaultNtf_stop_flag == D_RRH_OFF ) {
						f_com_SVCtl(E_DU_SVCTL_OPE_FID51_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_SFP_FAILSAFE_NTF);
					}
				}
				f_sfpw_FaultNtf_send_flag   = D_RRH_OFF;
				f_sfpw_FaultNtf_recover_cnt = 0;

				/* フェールセーフ関連パラメータを初期化する	*/
				f_sfpw_failsafe1_flag     = D_RRH_OFF;	/* RU未実装FailSafe条件発生フラグ(条件①)	*/
				f_sfpw_failsafe2_flag     = D_RRH_OFF;	/* RU未実装FailSafe条件発生フラグ(条件②)	*/
				f_sfpw_failsafe3_flag     = D_RRH_OFF;	/* RU未実装FailSafe条件発生フラグ(条件③)	*/
				f_sfpw_failsafe3_cnt      = 0;			/* リンク断発生回数(条件③)	*/
				f_sfpw_failsafe3_recv_cnt = 0xFFFFFFFF;	/* リンク確立連続回数(条件③復旧)	*/
			}
		} else {			/* ユーザーによる抑止指定あり	*/

			/* SVレジスタは復旧に設定する	*/
			if( f_sfpw_FaultNtf_send_flag == D_RRH_ON ) {
				f_com_SVCtl(E_DU_SVCTL_OPE_FID51_NOALM, D_DU_SVCTL_OFF, D_DU_FLTID_SFP_FAILSAFE);
				if( f_sfpw_FaultNtf_stop_flag == D_RRH_OFF ) {
					f_com_SVCtl(E_DU_SVCTL_OPE_FID51_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_SFP_FAILSAFE_NTF);
				}
			}
			f_sfpw_FaultNtf_send_flag   = D_RRH_OFF;
			f_sfpw_FaultNtf_recover_cnt = 0;

			/* フェールセーフ関連パラメータを初期化する	*/
			f_sfpw_failsafe1_flag     = D_RRH_OFF;	/* RU未実装FailSafe条件発生フラグ(条件①)	*/
			f_sfpw_failsafe2_flag     = D_RRH_OFF;	/* RU未実装FailSafe条件発生フラグ(条件②)	*/
			f_sfpw_failsafe3_flag     = D_RRH_OFF;	/* RU未実装FailSafe条件発生フラグ(条件③)	*/
			f_sfpw_failsafe3_cnt      = 0;			/* リンク断発生回数(条件③)	*/
			f_sfpw_failsafe3_recv_cnt = 0xFFFFFFFF;	/* リンク確立連続回数(条件③復旧)	*/
		}

		/* for CT */
		if(f_sfpw_thdState == D_SFP_THD_STS_DBG){
			break;
		}
	}
	return;
}

/* @} */
