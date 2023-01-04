/*!
 * @skip		$ld:$
 * @file		f_main_iniTbl_flash.c
 * @brief		pf_main FLASHから初期値の取得
 * @author		FJT) Yoshida
 * @date		2019/02/12 FJT) Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */
#include "f_main_inc.h"

/*!
 * @brief		f_main_iniTbl_flash
 * @note		FlashROMから初期設定値を取得し、共有メモリに展開する。
 *					-# seachable-access-vlansをFLASHから読出し、共有メモリに展開する。
 *					-# NETCONFユーザー情報をFLASHから読出し、共有メモリに展開する。
 * @param		-
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	5GDU-002-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2019/02/12 FJT) Yoshida Create
 */
INT f_main_iniTbl_flash( VOID )
{
	T_RRH_NETCONF_INFO_FLASH_TBL	*netconf_info_flash_tbl;

	INT								i;
	UCHAR							reg_read_data;

	INT								rtn_bpf = 0;

	INT								cmd_rtn;
	char							cmd_str[64];
	char							user_name[64];

// 擬似環境 20210316
//#ifdef SIM_DEBUG
    printf("\n return func %s",__func__);
	return D_SYS_OK;
//#endif
// 擬似環境 20210316
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_iniTbl_flash] ENTER" );

	/* ==================================================================== */
	/* NETCONF user information												*/
	/* ==================================================================== */
	/* NETCONFユーザー情報テーブル 初期化	*/
	f_comw_netconf_tbl->face_cnt = D_DU_NETCONF_NUM_OF_USER;
	f_comw_netconf_tbl->num_of_use = 0;
	for( i = 0; i < f_comw_netconf_tbl->face_cnt; i++ ) {
		memset( (VOID *)&f_comw_netconf_tbl->info[i], 0, sizeof(T_RRH_NETCONF_USER_INF) );
	}

	/* FLASH読出しデータ初期化	*/
	memset( f_mainw_iniData, 0, sizeof(f_mainw_iniData) );

	/* SSHユーザーアカウント情報FLASH書込みフラグ 読出し	*/
	rtn_bpf = BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_FLASH_WRITE_FLAG_SSH, &reg_read_data );
	if(BPF_HM_DEVC_COMPLETE != rtn_bpf) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "EEPROM Read NG. rtn_bpf=%08x", rtn_bpf );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_iniTbl_flash] RETURN" );
		return D_SYS_NG;
	}

	/* FLASH書込み有無チェック	*/
	if(reg_read_data == D_RRH_ON) {

		/* FLASH読出し	*/
		rtn_bpf = BPF_HM_DEVC_QSPI_FLASH_READ( D_RRH_QSPI_FLASH_FACE1,						/* Flash面					*/
											   D_RRH_QSPI_FLASH_OFFSET_NETCONF_USER_INFO,	/* Flash読み込みオフセット	*/
											   sizeof(T_RRH_NETCONF_INFO_FLASH_TBL),		/* 読み込みサイズ			*/
											   (UCHAR *)f_mainw_iniData );					/* 出力先アドレス			*/
		if(BPF_HM_DEVC_COMPLETE != rtn_bpf) {

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Flash Initial Data load NG. rtn_bpf=%08x", rtn_bpf );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_iniTbl_flash] RETURN" );
			return D_SYS_NG;
		}
		netconf_info_flash_tbl = (T_RRH_NETCONF_INFO_FLASH_TBL *)f_mainw_iniData;

		/* FLASH格納データチェック	*/
		if( netconf_info_flash_tbl->num_of_use == 0 ) {	/* ユーザー登録なし	*/

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "FLASH : No user, number of NETCONF user = %08x.", netconf_info_flash_tbl->num_of_use );

		} else if( netconf_info_flash_tbl->num_of_use > D_DU_NETCONF_NUM_OF_USER ) {	/* ユーザー登録数超過	*/

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "FLASH : Over user, numbeer of NETCONF user = %08x.", netconf_info_flash_tbl->num_of_use );

		} else {	/* 正常	*/

			/* パラメータ異常 => テーブルコピーしない	*/
			for( i = 0; i < netconf_info_flash_tbl->num_of_use; i++ ) {
				if( strlen(netconf_info_flash_tbl->info[i].user) == 0 ) {
					break;
				}
				if( strlen(netconf_info_flash_tbl->info[i].password) == 0 ) {
					break;
				}
				if( (netconf_info_flash_tbl->info[i].enabled != D_RRH_OFF)
				 && (netconf_info_flash_tbl->info[i].enabled != D_RRH_ON ) ) {
					break;
				}
			}
			if( i != netconf_info_flash_tbl->num_of_use ) {

				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "FLASH : NETCONF user info Table error user = %08x, err_user = %08x.",
									netconf_info_flash_tbl->num_of_use, i );
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => user     : %s (len : %04x).", 
									netconf_info_flash_tbl->info[i].user, strlen(netconf_info_flash_tbl->info[i].user) );
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => password : %s (len : %04x).", 
									netconf_info_flash_tbl->info[i].password, strlen(netconf_info_flash_tbl->info[i].password) );
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => enabled  : %08x.", netconf_info_flash_tbl->info[i].enabled );

			} else {

				/* enable userなし => テーブルコピーしない	*/
				for( i = 0; i < netconf_info_flash_tbl->num_of_use; i++ ) {
					if( netconf_info_flash_tbl->info[i].enabled == D_RRH_ON ) {
						break;
					}
				}
				/* enable userなし => テーブルコピーしない	*/
				if( i == netconf_info_flash_tbl->num_of_use ) {

					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "FLASH : NETCONF user all disabled. (user = %08x)", netconf_info_flash_tbl->num_of_use );
				} else {

					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "FLASH : NETCONF user = %08x.", netconf_info_flash_tbl->num_of_use );
					for( i = 0; i < netconf_info_flash_tbl->num_of_use; i++ ) {

						/* FLASH情報を共有メモリに展開	*/
						memcpy( f_comw_netconf_tbl->info[i].user,     netconf_info_flash_tbl->info[i].user,     D_DU_NETCONF_CHAR_OF_USER );
						memcpy( f_comw_netconf_tbl->info[i].password, netconf_info_flash_tbl->info[i].password, D_DU_NETCONF_CHAR_OF_PASS );

						f_comw_netconf_tbl->info[i].enabled    = netconf_info_flash_tbl->info[i].enabled;
						f_comw_netconf_tbl->num_of_use++;

						if( f_comw_netconf_tbl->info[i].enabled == D_RRH_ON ) {

							BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => user     : %s", f_comw_netconf_tbl->info[i].user );
#if 0
							BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => password : %s", f_comw_netconf_tbl->info[i].password );
#endif
							if( f_comw_default_user_disabled_flag == D_RRH_OFF ) {

								/* LINUXコマンド発行 (アカウント属性変更:disable)	*/
								memset ( user_name, '\0', 64 );
								sprintf( user_name, "ysjgmr" );

								memset ( cmd_str, '\0', 64 );
								sprintf( cmd_str, "usermod -s /bin/false %s", user_name );
								cmd_rtn = system ( cmd_str );

								if( cmd_rtn == D_RRH_MNG ) {
									BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "ERROR,default user disabled!(cur usercnt:%d)",netconf_info_flash_tbl->num_of_use);
								} else {
									BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "OK,disabled default user(cur usercnt:%d)",netconf_info_flash_tbl->num_of_use);
									f_comw_default_user_disabled_flag = D_RRH_ON;
								}
							}
						}
					}
				}
			}
		}
	}

	/* ==================================================================== */
	/* searchable-access-vlans information									*/
	/* ==================================================================== */
	/* FLASH読出しデータ初期化	*/
	memset( f_mainw_iniData, 0xFF, sizeof(f_mainw_iniData) );

	/* VLAN情報FLASH書込みフラグ 読出し	*/
	rtn_bpf = BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_FLASH_WRITE_FLAG_VLAN, &reg_read_data );
	if(BPF_HM_DEVC_COMPLETE != rtn_bpf) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "EEPROM Read NG. rtn_bpf=%08x", rtn_bpf );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_iniTbl_flash] RETURN" );
		return D_SYS_NG;
	}

	/* FLASH書込み有無チェック	*/
	if(reg_read_data == D_RRH_ON) {

		/* FLASH読出し	*/
		rtn_bpf = BPF_HM_DEVC_QSPI_FLASH_READ( D_RRH_QSPI_FLASH_FACE0,							/* Flash面					*/
											   D_RRH_QSPI_FLASH_OFFSET_SEARCHABLE_VLAN_INFO,	/* Flash読み込みオフセット	*/
											   D_DU_SEARCHABLE_VLAN_MAX,						/* 読み込みサイズ			*/
											   (UCHAR *)f_mainw_iniData );						/* 出力先アドレス			*/
		if(BPF_HM_DEVC_COMPLETE != rtn_bpf) {

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Flash Initial Data load NG. rtn_bpf=%08x", rtn_bpf );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_iniTbl_flash] RETURN" );
			return D_SYS_NG;
		}
	} else {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "FLASH : No set searchable-access-vlans." );
	}
	memcpy( f_comw_searchable_vlan_info->seachable_vlan_id, f_mainw_iniData, D_DU_SEARCHABLE_VLAN_MAX );	/* pgr0832	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_iniTbl_flash] RETURN" );
	return D_SYS_OK;
}
/* @} */
