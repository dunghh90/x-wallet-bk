/*!
 * @skip 		$ld:$
 * @file 		f_eom_main.c
 * @brief		pf_eomスレッド メイン処理
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_EOM
 * @{
 */
#include <sys/ioctl.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/msg.h>
#include <net/if.h> 
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> 

#include "f_eom_inc.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_eom_main
 * @note		pf_eomスレッドメイン。スレッド起動からMSG受信待ちのLoop処理を実施
 *				-# BPFへスレッド情報登録する
 *				-# EOMスレッド状態を"初期状態"に設定する
 *				-# 初期化関数をCallする(f_eom_init())
 *				-# MSG待ち無限Loop処理へ移行する
 *					-# スレッド間通信キュー受信関数をCallする(BPF_RU_IPCM_MSGQ_RECV())
 *					-# 受信メッセージ解析関数をCallする(f_eom_anlz())
 *					-# 通信用共有メモリの解放(BPF_RU_IPCM_PROCMSG_ADDRFREE())
 * @param		N/A
 * @retval		N/A
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
VOID f_eom_main_fheth( INT	*fhport )
{
	INT								i;

	INT								ret;
	INT								sock;
	struct ifreq					ifr;
	struct sockaddr_ll				sll;

	CHAR							cmd_str[D_SYS_NUM128];				/* コマンド文字列		*/
	CHAR							if_name[D_SYS_NUM16];

	T_EOM_LBX_FRAME					msgframe;
	INT								msgsize;

	UCHAR							ma_level;

	UCHAR							*lbr_payload;
	SHORT							len;
	SHORT							ptn_len;
	USHORT							ptn_err_flag;
	USHORT							end_chk_flag;


	/************************************************************************************************************************/
	/* スレッド情報登録																										*/
	/************************************************************************************************************************/
	if( *fhport == D_RRH_FHPORT0 ) {

		/* スレッド情報登録	*/
		prctl(PR_SET_NAME, "pf_eom1", 0, 0, 0);
		BPF_RM_SVRM_THREAD_INFO_SET( D_RRH_PROCID_PF, D_SYS_THDID_PF_EOM_0, D_SYS_THDQID_PF_EOM_0, (unsigned long int)pthread_self() );

	} else {

		/* スレッド情報登録	*/
		prctl(PR_SET_NAME, "pf_eom2", 0, 0, 0);
		BPF_RM_SVRM_THREAD_INFO_SET( D_RRH_PROCID_PF, D_SYS_THDID_PF_EOM_1, D_SYS_THDQID_PF_EOM_1, (unsigned long int)pthread_self());
	}

	/************************************************************************************************************************/
	/* U-Plane VLAN設定待ち																									*/
	/************************************************************************************************************************/
	while(1) {
		if( f_comw_du_connection_tbl->ucp[*fhport].umac.vid != 0xFFFF ) {
			break;
		}
		usleep(1000000);	/* wait 10sec */
	}
	if( *fhport == D_RRH_FHPORT0 ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "pf_eom1 LBM/LBR Process Start" );
	} else {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "pf_eom2 LBM/LBR Process Start" );
	}
	f_comw_lbm_mngment_tbl->port_info[*fhport].valid_flag = D_RRH_ON;

	/************************************************************************************************************************/
	/* U-Plane VLANあり interface作成 (802.1Q)																				*/
	/************************************************************************************************************************/
	if( *fhport == D_RRH_FHPORT0 ) {

		memset ( if_name, 0, sizeof(if_name) );
		sprintf( if_name, "%s", D_RRH_IF_NAME_0 );

	} else {

		memset ( if_name, 0, sizeof(if_name) );
		sprintf( if_name, "%s", D_RRH_IF_NAME_1 );
	}

	/* Interfaceにvlan-idを設定	*/
	memset ( cmd_str, 0, sizeof(cmd_str) );
	sprintf( cmd_str, D_EOM_CMD_VLAN_ADD, if_name, f_comw_du_connection_tbl->ucp[*fhport].umac.vid );
	ret = system ( cmd_str );
	if( ret != 0 ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "pf_eom%d command error, [%s]", (*fhport + 1), cmd_str );
	}
	/* pcpを設定	*/
	for(i = 0; i < 8; i++) {
		memset ( cmd_str, 0, sizeof(cmd_str) );
		sprintf( cmd_str, D_EOM_CMD_VLAN_EGRESS, if_name, f_comw_du_connection_tbl->ucp[*fhport].umac.vid,
												 i, f_comw_du_connection_tbl->ucp[*fhport].umac.pcp );
		ret = system ( cmd_str );
		if( ret != 0 ) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "pf_eom%d command error, [%s]", (*fhport + 1), cmd_str );
		}
	}
	/* Interfaceを起動	*/
	memset ( cmd_str, 0, sizeof(cmd_str) );
	sprintf( cmd_str, D_EOM_CMD_IFCONFIG_UP, if_name, f_comw_du_connection_tbl->ucp[*fhport].umac.vid );
	ret = system ( cmd_str );
	if( ret != 0 ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "pf_eom%d command error, [%s]", (*fhport + 1), cmd_str );
	}

	/************************************************************************************************************************/
	/* Socket設定																											*/
	/************************************************************************************************************************/
	/* Socket Open	*/
	if( (sock = socket(PF_PACKET, SOCK_RAW, htons(0x8902))) < 0 ) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "pf_eom%d socket open error, errno = %08x", (*fhport + 1), errno );
		return;
	}
	f_comw_lbm_mngment_tbl->port_info[*fhport].sock = sock;

	/* Interface number Get	*/
	memset( &ifr, 0, sizeof(struct ifreq) );
	snprintf( ifr.ifr_name, sizeof(ifr.ifr_name), "fheth%d.%d", *fhport, f_comw_du_connection_tbl->ucp[*fhport].umac.vid);
	if( ioctl(sock, SIOCGIFINDEX, &ifr) < 0 ) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "pf_eom%d ioctl : SIOCGIFINDEX error, errno = %08x", (*fhport + 1), errno );
		close( sock );
		return;
	}

#if 0
	/* vlan device Set	*/
	ifr.ifr_flags = ifr.ifr_flags | IFF_802_1Q_VLAN;
	if( ioctl(sock, SIOCSIFPFLAGS, &ifr) < 0 ) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "pf_eom%d ioctl : SIOCSIFPFLAGS error, errno = %08x", (*fhport + 1), errno );
		close( sock );
		return;
	}
#endif
	/* ソケット情報設定 */
	memset( &sll, 0, sizeof(struct sockaddr_ll) );
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(0x8902);
	sll.sll_halen = ETHER_ADDR_LEN;

	/* bind	*/
	if( bind(sock, (struct sockaddr *)&sll, sizeof(sll)) < 0 ) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "pf_eom%d bind error, errno = %08x", (*fhport + 1), errno );
		close( sock );
		return;
	}
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "pf_eom%d socket open! (if:%s, sock:%08x) ", (*fhport + 1), ifr.ifr_name, f_comw_lbm_mngment_tbl->port_info[*fhport].sock );

	/************************************************************************************************************************/
	/* メッセージ受信待ち																									*/
	/************************************************************************************************************************/
	while(1) {

		/* Frame格納領域初期化	*/
		memset( (UCHAR *)&msgframe, 0, sizeof(T_EOM_LBX_FRAME) );

		/********************************************************************************************************************/
		/* LBM受信																											*/
		/********************************************************************************************************************/
		/* メッセージ受信	*/
		if( ( msgsize = recv(sock, (VOID *)&msgframe, sizeof(T_EOM_LBX_FRAME), 0) ) < 0 ) {

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "pf_eom%d recv error, errno = %08x", (*fhport + 1), errno );
			close( sock );
			break;
		}

		/********************************************************************************************************************/
		/* LBMパラメータチェック																							*/
		/********************************************************************************************************************/
		/* OPE-CODEチェック	*/
		if( msgframe.body.OpCode != D_EOM_LBX_OPCODE_LBM ) {

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "pf_eom%d LBM OpCode error, OpCode = %02x", (*fhport + 1), msgframe.body.OpCode );
			continue;
		}
		/* MA LEVEL有効チェック	*/
		ma_level = (msgframe.body.version & D_EOM_LBX_MA_LEVEL_MASK) >> D_EOM_LBX_MA_LEVEL_SHFT;
		if( f_comw_lbm_mngment_tbl->info[ma_level].valid_flag != D_RRH_ON ) {

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "pf_eom%d LBM ma-level invalid, ma-level = %02x", (*fhport + 1), ma_level );
			continue;
		}
		/* Payloadチェック	*/
		lbr_payload  = (UCHAR *)&msgframe.body.PatternData[0];
		ptn_len      = msgsize - (sizeof(T_EOM_ETH_HEADER) + 8);
		ptn_err_flag = D_RRH_ON;
		end_chk_flag = D_RRH_OFF;

		while( ptn_len > 0 ) {

			if( *lbr_payload == 3 ) {
				len = *(lbr_payload + 1) << 8 | *(lbr_payload + 2);
				if( len < 0 ) {
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "pf_eom%d LBM pattern length error, length = %d", (*fhport + 1), (ptn_len - len) );
					ptn_err_flag = D_RRH_ON;
					end_chk_flag = D_RRH_ON;
					break;
				} if( (ptn_len - len) < 0 ) {
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "pf_eom%d LBM pattern length error, length = %d", (*fhport + 1), (ptn_len - len) );
					ptn_err_flag = D_RRH_ON;
					end_chk_flag = D_RRH_ON;
					break;
				} else {
					lbr_payload  = (lbr_payload + len + 1 + 2);	/* Type : 1byte + Length : 2byte	*/
					ptn_len      = ptn_len - (len + 1 + 2);		/* Type : 1byte + Length : 2byte	*/
				}
			} else if( *lbr_payload == 0 ) {
				ptn_err_flag = D_RRH_OFF;
				break;
			} else {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "pf_eom%d LBM pattern type error, type = %x", (*fhport + 1), *lbr_payload );
				ptn_err_flag = D_RRH_ON;
				end_chk_flag = D_RRH_ON;
				break;
			}
		}
		if( ptn_err_flag == D_RRH_ON ) {

			if( end_chk_flag == D_RRH_OFF ) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "pf_eom%d LBM pattern no end type", (*fhport + 1) );
			}
			continue;
		}

		/********************************************************************************************************************/
		/* LBR送信																											*/
		/********************************************************************************************************************/
		/* LBM編集	*/
		memcpy( msgframe.head.dst_mac_addr, msgframe.head.src_mac_addr, ETHER_ADDR_LEN );
		memcpy( msgframe.head.src_mac_addr, &f_comw_du_netinfo_eep_tbl->localMacAddress[*fhport][0], ETHER_ADDR_LEN );
		msgframe.body.OpCode = D_EOM_LBX_OPCODE_LBR;

		/* メッセージ送信	*/
		if( send(sock, (VOID *)&msgframe, msgsize, 0) < 0) {

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "pf_eom%d send error, errno = %08x", (*fhport + 1), errno );
			close( sock );
			break;
		}
	}
	return;
}
/* @} */
