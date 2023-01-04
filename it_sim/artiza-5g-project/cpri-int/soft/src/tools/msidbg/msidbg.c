/**************************************************************************************************/
/*!
 * @skip  $ld:$
 * @file  msidbg.c
 * @brief 
 * @date  2018/11/29 FJT)Taniguchi Create\n
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2018
 */
/**************************************************************************************************/
 
/*!
 * @addtogroup RRH_CMN
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "f_mpsw_msi.h"
#include "f_mpsw_msg_MSI.h"
#include "f_mpsw_msg_enum.h"
#include "f_mpsw_msg_subscribe_def.h"
#include "f_mpsw_msg_o-ran_sync_def.h"
#include "f_mpsw_msg_o-ran_transceiver_def.h"
#include "f_mpsw_msg_o-ran_usermgmt_typ.h"
#include "f_mpsw_msg_ietf_interfaces_typ.h"
#include "f_mpsw_msg_o-ran_operations_def.h"
#include "f_mpsw_msg_o-ran_performance_management_typ.h"
#include "f_mpsw_msg_o-ran_interface_typ.h"
#include "f_mpsw_msg_o-ran_mplane_int_typ.h"
#include "f_mpsw_msg_ready_def.h"
#include "f_mpsw_msg_ready_typ.h"
#include "f_mpsw_msg_o-ran_troubleshooting_def.h"
#include "f_mpsw_msg_o-ran_software_management_def.h"
#include "f_mpsw_msg_ietf_interfaces_def.h"
#include "f_mpsw_msg_ietf_ip_def.h"
#include "f_mpsw_msg_o-ran_dhcp_def.h"
#include "f_mpsw_msg_o-ran_interface_def.h"
#include "f_mpsw_msg_o-ran_mplane_int_def.h"
#include "f_mpsw_msg_o-ran_usermgmt_def.h"
#include "f_mpsw_msg_o-ran_lbm_def.h"
#include "f_mpsw_msg_o-ran_sync_typ.h"
#include "f_mpsw_msg_o-ran_processing_element_def.h"
#include "f_mpsw_msg_o-ran_processing_element_typ.h"
#include "f_mpsw_msg_o-ran_uplane_conf_def.h"
#include "f_mpsw_msg_o-ran_dhcp_typ.h"
#include "f_mpsw_msg_o-ran_transceiver_typ.h"
#include "f_mpsw_msg_o-ran_performance_management_def.h"
#include "f_mpsw_msg_ietf_hardware_def.h"
#include "f_mpsw_msg_o-ran_troubleshooting_typ.h"
#include "f_mpsw_msg_o-ran_software_management_typ.h"
#include "f_mpsw_msg_o-ran_hardware_def.h"
#include "f_mpsw_msg_ietf_hardware_typ.h"
#include "f_mpsw_msg_o-ran_hardware_typ.h"
#include "f_mpsw_msg_ietf_ip_typ.h"
#include "f_mpsw_msg_o-ran_fm_def.h"
#include "f_mpsw_msg_o-ran_fm_typ.h"
#include "f_mpsw_msg_o-ran_lbm_typ.h"
#include "f_mpsw_msg_subscribe_typ.h"
#include "f_mpsw_msg_o-ran_operations_typ.h"
#include "f_mpsw_msg_o-ran_uplane_conf_typ.h"
#include "f_mpsw_msg_o-ran_file_management_def.h"
#include "f_mpsw_msg_o-ran_file_management_typ.h"
#include "f_mpsw_msg_o-ran_externalio_def.h"
#include "f_mpsw_msg_o-ran_externalio_typ.h"
#include "f_mpsw_msg_o-ran_fan_def.h"
#include "f_mpsw_msg_o-ran_fan_typ.h"
#include "f_mpsw_msg_debug_def.h"
#include "f_mpsw_msg_debug_typ.h"


typedef struct {
	char*	name;
	UINT	msgid;
	INT		datasize;
	INT		flag;
	INT		portno;
}T_TEST_MSG;


static int vmode_flg = 0;
static T_MSI_SUBSCRIBE_MOD_INFO	msidbg_l_modInfo[0x100];


static T_TEST_MSG msidbg_test_data [] = 
{
	{	"MSI_O_RAN_SYNC_GET_REQ"								,D_MSG_MSI_O_RAN_SYNC_GET_REQ							,sizeof(T_MSG_MSI_O_RAN_SYNC_GET_REQ)							, 0, 65101	},
	{	"MSI_O_RAN_SYNC_GET_CFM"								,D_MSG_MSI_O_RAN_SYNC_GET_CFM							,sizeof(T_MSG_MSI_O_RAN_SYNC_GET_CFM)							, 0, 65101	},
	{	"MSI_O_RAN_SYNC_EDIT_REQ"							,D_MSG_MSI_O_RAN_SYNC_EDIT_REQ							,sizeof(T_MSG_MSI_O_RAN_SYNC_EDIT_REQ)							, 1, 65101	},
	{	"MSI_O_RAN_SYNC_EDIT_CFM"							,D_MSG_MSI_O_RAN_SYNC_EDIT_CFM							,sizeof(T_MSG_MSI_O_RAN_SYNC_EDIT_CFM)							, 0, 65101	},
	{	"MSI_O_RAN_SYNC_EDIT_REJ"							,D_MSG_MSI_O_RAN_SYNC_EDIT_REJ							,sizeof(T_MSG_MSI_O_RAN_SYNC_EDIT_REJ)							, 0, 65101	},
	{	"MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND"			,D_MSG_MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND			,sizeof(T_MSG_MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND)			, 0, 65101	},
	{	"MSI_O_RAN_SYNC_PTPSTATE_CHANGE_NTF_IND"				,D_MSG_MSI_O_RAN_SYNC_PTPSTATE_CHANGE_NTF_IND			,sizeof(T_MSG_MSI_O_RAN_SYNC_PTPSTATE_CHANGE_NTF_IND)			, 0, 65101	},
	{	"MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND"			,D_MSG_MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND			,sizeof(T_MSG_MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND)			, 0, 65101	},
	{	"MSI_O_RAN_TRANSCEIVER_GET_REQ"						,D_MSG_MSI_O_RAN_TRANSCEIVER_GET_REQ						,sizeof(T_MSG_MSI_O_RAN_TRANSCEIVER_GET_REQ)						, 0, 65100	},
	{	"MSI_O_RAN_TRANSCEIVER_GET_CFM"						,D_MSG_MSI_O_RAN_TRANSCEIVER_GET_CFM						,sizeof(T_MSG_MSI_O_RAN_TRANSCEIVER_GET_CFM)						, 0, 65100	},
	{	"MSI_O_RAN_TRANSCEIVER_GET_REJ"						,D_MSG_MSI_O_RAN_TRANSCEIVER_GET_REJ						,sizeof(T_MSG_MSI_O_RAN_TRANSCEIVER_GET_REJ)						, 0, 65100	},
	{	"MSI_O_RAN_OPERATIONS_GET_REQ"						,D_MSG_MSI_O_RAN_OPERATIONS_GET_REQ						,sizeof(T_MSG_MSI_O_RAN_OPERATIONS_GET_REQ)						, 0, 65100	},
	{	"MSI_O_RAN_OPERATIONS_GET_CFM"						,D_MSG_MSI_O_RAN_OPERATIONS_GET_CFM						,sizeof(T_MSG_MSI_O_RAN_OPERATIONS_GET_CFM)						, 0, 65100	},
	{	"MSI_O_RAN_OPERATIONS_GET_REJ"						,D_MSG_MSI_O_RAN_OPERATIONS_GET_REJ						,sizeof(T_MSG_MSI_O_RAN_OPERATIONS_GET_REJ)						, 0, 65100	},
	{	"MSI_O_RAN_OPERATIONS_RESET_IND"						,D_MSG_MSI_O_RAN_OPERATIONS_RESET_IND						,sizeof(T_MSG_MSI_O_RAN_OPERATIONS_RESET_IND)						, 0, 65100	},
	{	"MSI_READY_IND"										,D_MSG_MSI_READY_IND									,sizeof(T_MSG_MSI_READY_IND)									, 0, 65100	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REQ"	,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REQ	,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REQ)	, 0, 65100	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM"	,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM	,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM)	, 0, 65100	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ"	,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ	,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ)	, 0, 65100	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ		,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ)		, 1, 65100	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM		,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM)		, 0, 65100	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ		,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ)		, 0, 65100	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ			,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ)			, 1, 65100	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_CFM"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_CFM			,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_CFM)			, 0, 65100	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ			,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ)			, 0, 65100	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ		,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ)		, 1, 65100	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM		,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM)		, 0, 65100	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ		,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ)		, 0, 65100	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND"		,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND	,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND)	, 0, 65100	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND"		,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND		,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND)		, 0, 65100	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND"		,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND	,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND)	, 0, 65100	},
	{	"MSI_IETF_INTERFACES_GET_REQ"						,D_MSG_MSI_IETF_INTERFACES_GET_REQ						,sizeof(T_MSG_MSI_IETF_INTERFACES_GET_REQ)						, 0, 65100	},
	{	"MSI_IETF_INTERFACES_GET_CFM"						,D_MSG_MSI_IETF_INTERFACES_GET_CFM						,sizeof(T_MSG_MSI_IETF_INTERFACES_GET_CFM)						, 0, 65100	},
	{	"MSI_IETF_INTERFACES_GET_REJ"						,D_MSG_MSI_IETF_INTERFACES_GET_REJ						,sizeof(T_MSG_MSI_IETF_INTERFACES_GET_REJ)						, 0, 65100	},
	{	"MSI_IETF_IP_EDIT_REQ"								,D_MSG_MSI_IETF_IP_EDIT_REQ								,sizeof(T_MSG_MSI_IETF_IP_EDIT_REQ)								, 1, 65100	},
	{	"MSI_IETF_IP_EDIT_CFM"								,D_MSG_MSI_IETF_IP_EDIT_CFM								,sizeof(T_MSG_MSI_IETF_IP_EDIT_CFM)								, 0, 65100	},
	{	"MSI_IETF_IP_EDIT_REJ"								,D_MSG_MSI_IETF_IP_EDIT_REJ								,sizeof(T_MSG_MSI_IETF_IP_EDIT_REJ)								, 0, 65100	},
	{	"MSI_O_RAN_DHCP_GET_REQ"								,D_MSG_MSI_O_RAN_DHCP_GET_REQ							,sizeof(T_MSG_MSI_O_RAN_DHCP_GET_REQ)							, 0, 65100	},
	{	"MSI_O_RAN_DHCP_GET_CFM"								,D_MSG_MSI_O_RAN_DHCP_GET_CFM							,sizeof(T_MSG_MSI_O_RAN_DHCP_GET_CFM)							, 0, 65100	},
	{	"MSI_O_RAN_DHCP_GET_REJ"								,D_MSG_MSI_O_RAN_DHCP_GET_REJ							,sizeof(T_MSG_MSI_O_RAN_DHCP_GET_REJ)							, 0, 65100	},
	{	"MSI_O_RAN_INTERFACE_EDIT_REQ"						,D_MSG_MSI_O_RAN_INTERFACE_EDIT_REQ						,sizeof(T_MSG_MSI_O_RAN_INTERFACE_EDIT_REQ)						, 1, 65100	},
	{	"MSI_O_RAN_INTERFACE_EDIT_CFM"						,D_MSG_MSI_O_RAN_INTERFACE_EDIT_CFM						,sizeof(T_MSG_MSI_O_RAN_INTERFACE_EDIT_CFM)						, 0, 65100	},
	{	"MSI_O_RAN_INTERFACE_EDIT_REJ"						,D_MSG_MSI_O_RAN_INTERFACE_EDIT_REJ						,sizeof(T_MSG_MSI_O_RAN_INTERFACE_EDIT_REJ)						, 0, 65100	},
	{	"MSI_O_RAN_MPLANE_INT_EDIT_REQ"						,D_MSG_MSI_O_RAN_MPLANE_INT_EDIT_REQ						,sizeof(T_MSG_MSI_O_RAN_MPLANE_INT_EDIT_REQ)						, 1, 65100	},
	{	"MSI_O_RAN_MPLANE_INT_EDIT_CFM"						,D_MSG_MSI_O_RAN_MPLANE_INT_EDIT_CFM						,sizeof(T_MSG_MSI_O_RAN_MPLANE_INT_EDIT_CFM)						, 0, 65100	},
	{	"MSI_O_RAN_MPLANE_INT_EDIT_REJ"						,D_MSG_MSI_O_RAN_MPLANE_INT_EDIT_REJ						,sizeof(T_MSG_MSI_O_RAN_MPLANE_INT_EDIT_REJ)						, 0, 65100	},
	{	"MSI_O_RAN_USERMGMT_EDIT_REQ"						,D_MSG_MSI_O_RAN_USERMGMT_EDIT_REQ						,sizeof(T_MSG_MSI_O_RAN_USERMGMT_EDIT_REQ)						, 1, 65100	},
	{	"MSI_O_RAN_USERMGMT_EDIT_CFM"						,D_MSG_MSI_O_RAN_USERMGMT_EDIT_CFM						,sizeof(T_MSG_MSI_O_RAN_USERMGMT_EDIT_CFM)						, 0, 65100	},
	{	"MSI_O_RAN_USERMGMT_EDIT_REJ"						,D_MSG_MSI_O_RAN_USERMGMT_EDIT_REJ						,sizeof(T_MSG_MSI_O_RAN_USERMGMT_EDIT_REJ)						, 0, 65100	},
	{	"MSI_O_RAN_LBM_EDIT_REQ"								,D_MSG_MSI_O_RAN_LBM_EDIT_REQ							,sizeof(T_MSG_MSI_O_RAN_LBM_EDIT_REQ)							, 1, 65100	},
	{	"MSI_O_RAN_LBM_EDIT_CFM"								,D_MSG_MSI_O_RAN_LBM_EDIT_CFM							,sizeof(T_MSG_MSI_O_RAN_LBM_EDIT_CFM)							, 0, 65100	},
	{	"MSI_O_RAN_LBM_EDIT_REJ"								,D_MSG_MSI_O_RAN_LBM_EDIT_REJ							,sizeof(T_MSG_MSI_O_RAN_LBM_EDIT_REJ)							, 0, 65100	},
	{	"MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ"				,D_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ				,sizeof(T_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ)				, 1, 65100	},
	{	"MSI_O_RAN_PROCESSING_ELEMENT_EDIT_CFM"				,D_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_CFM				,sizeof(T_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_CFM)				, 0, 65100	},
	{	"MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REJ"				,D_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REJ				,sizeof(T_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REJ)				, 0, 65100	},
	{	"MSI_O_RAN_UPLANE_CONF_GET_REQ"						,D_MSG_MSI_O_RAN_UPLANE_CONF_GET_REQ						,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_GET_REQ)						, 0, 65100	},
	{	"MSI_O_RAN_UPLANE_CONF_GET_CFM"						,D_MSG_MSI_O_RAN_UPLANE_CONF_GET_CFM						,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_GET_CFM)						, 0, 65100	},
	{	"MSI_O_RAN_UPLANE_CONF_GET_REJ"						,D_MSG_MSI_O_RAN_UPLANE_CONF_GET_REJ						,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_GET_REJ)						, 0, 65100	},
	{	"MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ"					,D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ					,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ)					, 0, 65100	},
	{	"MSI_O_RAN_UPLANE_CONF_EDIT_TX_CFM"					,D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_CFM					,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_CFM)					, 0, 65100	},
	{	"MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ"					,D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ					,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ)					, 0, 65100	},
	{	"MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ"					,D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ					,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ)					, 0, 65100	},
	{	"MSI_O_RAN_UPLANE_CONF_EDIT_RX_CFM"					,D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_CFM					,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_CFM)					, 0, 65100	},
	{	"MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ"					,D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ					,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ)					, 0, 65100	},
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ"							,D_MSG_MSI_O_RAN_PFM_MNG_EDIT_REQ						,sizeof(T_MSG_MSI_O_RAN_PFM_MNG_EDIT_REQ)						, 1, 65100	},
	{	"MSI_O_RAN_PFM_MNG_EDIT_CFM"							,D_MSG_MSI_O_RAN_PFM_MNG_EDIT_CFM						,sizeof(T_MSG_MSI_O_RAN_PFM_MNG_EDIT_CFM)						, 0, 65100	},
	{	"MSI_O_RAN_PFM_MNG_EDIT_REJ"							,D_MSG_MSI_O_RAN_PFM_MNG_EDIT_REJ						,sizeof(T_MSG_MSI_O_RAN_PFM_MNG_EDIT_REJ)						, 0, 65100	},
	{	"MSI_O_RAN_PFM_MNG_GET_REQ"							,D_MSG_MSI_O_RAN_PFM_MNG_GET_REQ							,sizeof(T_MSG_MSI_O_RAN_PFM_MNG_GET_REQ)							, 0, 65100	},
	{	"MSI_O_RAN_PFM_MNG_GET_CFM"							,D_MSG_MSI_O_RAN_PFM_MNG_GET_CFM							,sizeof(T_MSG_MSI_O_RAN_PFM_MNG_GET_CFM)							, 0, 65100	},
	{	"MSI_O_RAN_PFM_MNG_GET_REJ"							,D_MSG_MSI_O_RAN_PFM_MNG_GET_REJ							,sizeof(T_MSG_MSI_O_RAN_PFM_MNG_GET_REJ)							, 0, 65100	},
	{	"MSI_O_RAN_PFM_MNG_NTF_IND"							,D_MSG_MSI_O_RAN_PFM_MNG_NTF_IND							,sizeof(T_MSG_MSI_O_RAN_PFM_MNG_NTF_IND)							, 0, 65100	},
	{	"MSI_IETF_HARDWARE_GET_REQ"							,D_MSG_MSI_IETF_HARDWARE_GET_REQ						,sizeof(T_MSG_MSI_IETF_HARDWARE_GET_REQ)						, 0, 65100	},
	{	"MSI_IETF_HARDWARE_GET_CFM"							,D_MSG_MSI_IETF_HARDWARE_GET_CFM						,sizeof(T_MSG_MSI_IETF_HARDWARE_GET_CFM)						, 0, 65100	},
	{	"MSI_IETF_HARDWARE_GET_REJ"							,D_MSG_MSI_IETF_HARDWARE_GET_REJ						,sizeof(T_MSG_MSI_IETF_HARDWARE_GET_REJ)						, 0, 65100	},
	{	"MSI_IETF_HARDWARE_STATE_GET_REQ"					,D_MSG_MSI_IETF_HARDWARE_STATE_GET_REQ					,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_GET_REQ)					, 0, 65100	},
	{	"MSI_IETF_HARDWARE_STATE_GET_CFM"					,D_MSG_MSI_IETF_HARDWARE_STATE_GET_CFM					,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_GET_CFM)					, 0, 65100	},
	{	"MSI_IETF_HARDWARE_STATE_GET_REJ"					,D_MSG_MSI_IETF_HARDWARE_STATE_GET_REJ					,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_GET_REJ)					, 0, 65100	},
	{	"MSI_IETF_HARDWARE_STATE_EDIT_REQ"					,D_MSG_MSI_IETF_HARDWARE_STATE_EDIT_REQ					,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_EDIT_REQ)					, 1, 65100	},
	{	"MSI_IETF_HARDWARE_STATE_EDIT_CFM"					,D_MSG_MSI_IETF_HARDWARE_STATE_EDIT_CFM					,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_EDIT_CFM)					, 0, 65100	},
	{	"MSI_IETF_HARDWARE_STATE_EDIT_REJ"					,D_MSG_MSI_IETF_HARDWARE_STATE_EDIT_REJ					,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_EDIT_REJ)					, 0, 65100	},
	{	"MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND"			,D_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND			,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND)			, 0, 65100	},
	{	"MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND"		,D_MSG_MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND		,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND)		, 0, 65100	},
	{	"MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND"		,D_MSG_MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND	,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND)	, 0, 65100	},
	{	"MSI_IETF_HARDWARE_STATE_CHANGE_START_IND"			,D_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_START_IND			,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_START_IND)			, 0, 65100	},
	{	"MSI_O_RAN_HARDWARE_GET_REQ"							,D_MSG_MSI_O_RAN_HARDWARE_GET_REQ						,sizeof(T_MSG_MSI_O_RAN_HARDWARE_GET_REQ)						, 0, 65100	},
	{	"MSI_O_RAN_HARDWARE_GET_CFM"							,D_MSG_MSI_O_RAN_HARDWARE_GET_CFM						,sizeof(T_MSG_MSI_O_RAN_HARDWARE_GET_CFM)						, 0, 65100	},
	{	"MSI_O_RAN_HARDWARE_GET_REJ"							,D_MSG_MSI_O_RAN_HARDWARE_GET_REJ						,sizeof(T_MSG_MSI_O_RAN_HARDWARE_GET_REJ)						, 0, 65100	},
	{	"MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REQ"			,D_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REQ			,sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REQ)			, 0, 65100	},
	{	"MSI_O_RAN_TROUBLESHOOTING_START_LOGS_CFM"			,D_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_CFM			,sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_CFM)			, 0, 65100	},
	{	"MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REJ"			,D_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REJ			,sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REJ)			, 0, 65100	},
	{	"MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REQ"			,D_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REQ			,sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REQ)			, 0, 65100	},
	{	"MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_CFM"			,D_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_CFM			,sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_CFM)			, 0, 65100	},
	{	"MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REJ"			,D_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REJ			,sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REJ)			, 0, 65100	},
	{	"MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND"	,D_MSG_MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND	,sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND)	, 0, 65100	},
	{	"MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND"					,D_MSG_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND					,sizeof(T_MSG_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND)					, 0, 65100	},
	{	"MSI_O_RAN_FM_ALARM_START_IND"						,D_MSG_MSI_O_RAN_FM_ALARM_START_IND						,sizeof(T_MSG_MSI_O_RAN_FM_ALARM_START_IND)						, 0, 65100	},
	{	"MSI_UPLOAD_REQ"									,D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ				,sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ)				, 1, 65100	},
	{	"MSI_UPLOAD_CFM"									,D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_CFM				,sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_CFM)				, 0, 65100	},
	{	"MSI_UPLOAD_REJ"									,D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REJ				,sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REJ)				, 0, 65100	},
	{	"MSI_UPLOAD_NTF_IND"								,D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND			,sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND)			, 0, 65100	},
	{	"MSI_RETRIEVE_REQ"									,D_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ			,sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ)			, 1, 65100	},
	{	"MSI_RETRIEVE_CFM"									,D_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM			,sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM)			, 0, 65100	},
	{	"MSI_RETRIEVE_REJ"									,D_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ			,sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ)			, 0, 65100	},
	{	"MSI_O_RAN_EXTERNALIO_GET_REQ"						,D_MSG_MSI_O_RAN_EXTERNALIO_GET_REQ						,sizeof(T_MSG_MSI_O_RAN_EXTERNALIO_GET_REQ)						, 0, 65100	},
	{	"MSI_O_RAN_EXTERNALIO_GET_CFM"						,D_MSG_MSI_O_RAN_EXTERNALIO_GET_CFM						,sizeof(T_MSG_MSI_O_RAN_EXTERNALIO_GET_CFM)						, 0, 65100	},
	{	"MSI_O_RAN_EXTERNALIO_EDIT_REQ"						,D_MSG_MSI_O_RAN_EXTERNALIO_EDIT_REQ						,sizeof(T_MSG_MSI_O_RAN_EXTERNALIO_EDIT_REQ)						, 0, 65100	},
	{	"MSI_O_RAN_EXTERNALIO_EDIT_CFM"						,D_MSG_MSI_O_RAN_EXTERNALIO_EDIT_CFM						,sizeof(T_MSG_MSI_O_RAN_EXTERNALIO_EDIT_CFM)						, 0, 65100	},
	{	"MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND"			,D_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND			,sizeof(T_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND)			, 0, 65100	},
	{	"MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_START_IND"		,D_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_START_IND		,sizeof(T_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_START_IND)		, 0, 65100	},
	{	"MSI_O_RAN_FAN_GET_REQ"								,D_MSG_MSI_O_RAN_FAN_GET_REQ								,sizeof(T_MSG_MSI_O_RAN_FAN_GET_REQ)								, 0, 65100	},
	{	"MSI_O_RAN_FAN_GET_CFM"								,D_MSG_MSI_O_RAN_FAN_GET_CFM								,sizeof(T_MSG_MSI_O_RAN_FAN_GET_CFM)								, 0, 65100	},

	{	"MSI_SUBSCRIBE_REQ"									,D_MSG_MSI_SUBSCRIBE_REQ								,sizeof(T_MSG_MSI_SUBSCRIBE_REQ)								, 0, 65100	},
	{	"MSI_SUBSCRIBE_CFM"									,D_MSG_MSI_SUBSCRIBE_CFM								,sizeof(T_MSG_MSI_SUBSCRIBE_CFM)								, 0, 65100	},
	{	"MSI_SUBSCRIBE_REJ"									,D_MSG_MSI_SUBSCRIBE_REJ								,sizeof(T_MSG_MSI_SUBSCRIBE_REJ)								, 0, 65100	},
	{	"MSI_DESUBSCRIBE_IND"								,D_MSG_MSI_DESUBSCRIBE_IND								,sizeof(T_MSG_MSI_DESUBSCRIBE_IND)								, 0, 65100	},

	{	"D_MSG_MSI_DEBUG_IND"								,D_MSG_MSI_DEBUG_IND									,sizeof(T_MSG_MSI_DEBUG_IND)									, 0, 65100	},

	{	NULL												,0														,0																, 0, 0		}
};

void func_vmsg(const char* format, ...){
	if(vmode_flg == 0){
		return;
	}
	printf("pid:%d ",getpid());
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
	return;
}

void func_msi_list( void )
{
	int count;
	
	printf("MSI LIST \n");
	printf("Msg name MsgId MsgSize \n");
	count = 0;
	while(1){
		if(msidbg_test_data[count].name == NULL){
			break;
		}
		printf("No.%d %-48s 0x%08X 0x%X(%d)bytes\n",count+1,msidbg_test_data[count].name,msidbg_test_data[count].msgid,msidbg_test_data[count].datasize,msidbg_test_data[count].datasize);
		count++;
	}
	printf("\n");
	return;
}


void func_usage( char* argv[] )
{
	printf("%s Usage:\n",argv[0]);
	printf("\n");
	printf(" %s [options] ... \n",argv[0]);
	printf("\n");
	printf("  [options]                      : description \n");
	printf("   -N <init option>              : NSA init \n");
	printf("   -S <init option>              : SAA init \n");
	printf("   -c <con option>               : MSI subscribe , desubscribe \n");
	printf("   -d <errorCode> <errorText>    : MSI debug message \n");
	printf("   -t                            : MSI test mode \n");
	printf("   -p                            : MSI send port \n");
	printf("   -m                            : MSI send msg \n");
	printf("   -l                            : MSI list\n");
	printf("   -h                            : Usage\n");
	printf("   -v                            : Verbose mode\n");
	printf("\n");
	return;
}

typedef union {
	T_MPSW_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ			pe_edit_req;
	T_MPSW_MSI_IETF_IP_EDIT_REQ							ip_edit_req;
	T_MPSW_MSI_O_RAN_INTERFACE_EDIT_REQ					if_edit_req;
	T_MPSW_MSI_O_RAN_MPLANE_INT_EDIT_REQ					mi_edit_req;
	T_MPSW_MSI_IETF_HARDWARE_STATE_EDIT_REQ				hw_edit_req;
	T_MPSW_MSI_O_RAN_LBM_EDIT_REQ						lb_edit_req;
	T_MPSW_MSI_O_RAN_SYNC_EDIT_REQ						sy_edit_req;
	T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ					um_edit_req;
	T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REQ					pm_edit_req;
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ		fdl_install_req;
	T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ		ful_retrieve_req;
	T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ			ful_uplpad_reg;
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ	fdl_act_req;
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ	fdl_dwnld_req;
	T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ			ful_upld_req;
} T_MPSW_MSI_PAYLOAD;

typedef struct {
	char*	i_name;
	char*	p_name;
	INT		size;
	UCHAR	*data;
	INT		loop_flag;
}T_TEST_PRAM_MSG;

T_MPSW_MSI_PAYLOAD	payload;

static T_TEST_PRAM_MSG msidbg_test_param[] = 
{
	{	"MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ",		"port",									4,		(UCHAR *)&payload.pe_edit_req.port,										0 },
	{	"MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ",		"ru_mac_address",						6,		(UCHAR *)&payload.pe_edit_req.ru_mac_address[0],						0 },
	{	"MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ",		"vlan_id",								2,		(UCHAR *)&payload.pe_edit_req.vlan_id,									0 },
	{	"MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ",		"o_du_mac_address",					6,		(UCHAR *)&payload.pe_edit_req.o_du_mac_address[0],						0 },
	{	"MSI_IETF_IP_EDIT_REQ",						"ip",									4,		(UCHAR *)&payload.ip_edit_req.ip,										0 },
	{	"MSI_IETF_IP_EDIT_REQ",						"prefix_length",						4,		(UCHAR *)&payload.ip_edit_req.prefix_length,							0 },
	{	"MSI_O_RAN_INTERFACE_EDIT_REQ",				"plane",								4,		(UCHAR *)&payload.if_edit_req.plane,									0 },
	{	"MSI_O_RAN_INTERFACE_EDIT_REQ",				"port",									4,		(UCHAR *)&payload.if_edit_req.port,										0 },
	{	"MSI_O_RAN_INTERFACE_EDIT_REQ",				"marking0",								2,		(UCHAR *)&payload.if_edit_req.marking[0],								0 },
	{	"MSI_O_RAN_INTERFACE_EDIT_REQ",				"marking1",								2,		(UCHAR *)&payload.if_edit_req.marking[1],								0 },
	{	"MSI_O_RAN_INTERFACE_EDIT_REQ",				"vlan_id",								2,		(UCHAR *)&payload.if_edit_req.vlan_id,									0 },
	{	"MSI_O_RAN_MPLANE_INT_EDIT_REQ",				"mplane_ssh_port.call_home_ssh_port",	4,		(UCHAR *)&payload.mi_edit_req.mplane_ssh_port.call_home_ssh_port,		0 },
	{	"MSI_O_RAN_MPLANE_INT_EDIT_REQ",				"mplane_ssh_port.server_ssh_port",		4,		(UCHAR *)&payload.mi_edit_req.mplane_ssh_port.server_ssh_port,			0 },
	{	"MSI_O_RAN_MPLANE_INT_EDIT_REQ",				"access_vlan_info.lowest_vlan_id",		2,		(UCHAR *)&payload.mi_edit_req.access_vlan_info.lowest_vlan_id,			0 },
	{	"MSI_O_RAN_MPLANE_INT_EDIT_REQ",				"access_vlan_info.highest_vlan_id",		2,		(UCHAR *)&payload.mi_edit_req.access_vlan_info.highest_vlan_id,			0 },
	{	"MSI_O_RAN_MPLANE_INT_EDIT_REQ",				"access_vlan_info.num_of_vlans",		2,		(UCHAR *)&payload.mi_edit_req.access_vlan_info.num_of_vlans,			0 },
	{	"MSI_O_RAN_MPLANE_INT_EDIT_REQ",				"access_vlan_info.searchable_vlan_id",	2,		(UCHAR *)&payload.mi_edit_req.access_vlan_info.searchable_vlan_id[0],	1 },
	{	"MSI_IETF_HARDWARE_STATE_EDIT_REQ",			"admin_state",							4,		(UCHAR *)&payload.hw_edit_req.admin_state,								0 },
	{	"MSI_O_RAN_LBM_EDIT_REQ",					"id",									64,		(UCHAR *)&payload.lb_edit_req.id,										0 },
	{	"MSI_O_RAN_LBM_EDIT_REQ",					"name",									64,		(UCHAR *)&payload.lb_edit_req.name,										0 },
	{	"MSI_O_RAN_LBM_EDIT_REQ",					"md_level",								2,		(UCHAR *)&payload.lb_edit_req.md_level,									0 },
	{	"MSI_O_RAN_SYNC_EDIT_REQ",					"ptp_config_t.domain_number",			1,		(UCHAR *)&payload.sy_edit_req.ptp_config_t.domain_number,				0 },
	{	"MSI_O_RAN_SYNC_EDIT_REQ",					"ptp_config_t.multicast_mac_address",	1,		(UCHAR *)&payload.sy_edit_req.ptp_config_t.multicast_mac_address,		0 },
	{	"MSI_O_RAN_SYNC_EDIT_REQ",					"ptp_config_t.clock_classes",			1,		(UCHAR *)&payload.sy_edit_req.ptp_config_t.clock_classes,				0 },
	{	"MSI_O_RAN_SYNC_EDIT_REQ",					"ptp_config_t.delay_asymmetry",			2,		(UCHAR *)&payload.sy_edit_req.ptp_config_t.delay_asymmetry,				0 },
	{	"MSI_O_RAN_SYNC_EDIT_REQ",					"ptp_config_t.ptp_reporting_period",	1,		(UCHAR *)&payload.sy_edit_req.ptp_config_t.ptp_reporting_period,		0 },
	{	"MSI_O_RAN_SYNC_EDIT_REQ",					"ptp_config_t.syncE_reporting_period",	1,		(UCHAR *)&payload.sy_edit_req.ptp_config_t.syncE_reporting_period,		0 },
	{	"MSI_O_RAN_USERMGMT_EDIT_REQ",				"user",									64,		(UCHAR *)&payload.um_edit_req.user,										0 },
	{	"MSI_O_RAN_USERMGMT_EDIT_REQ",				"password",								256,	(UCHAR *)&payload.um_edit_req.password,									0 },
	{	"MSI_O_RAN_USERMGMT_EDIT_REQ",				"enabled",								4,		(UCHAR *)&payload.um_edit_req.enabled,									0 },
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ",				"mea_interval",							2,		(UCHAR *)&payload.pm_edit_req.mea_interval,								0 },
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ",				"ntf_interval",							2,		(UCHAR *)&payload.pm_edit_req.ntf_interval,								0 },
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ",				"measure_t_num",						2,		(UCHAR *)&payload.pm_edit_req.measure_t_num,							0 },
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ",				"rx_window_measure_t0.measure_obj",		4,		(UCHAR *)&payload.pm_edit_req.rx_window_measure_t[0].measure_obj,		0 },
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ",				"rx_window_measure_t0.act_flag",		2,		(UCHAR *)&payload.pm_edit_req.rx_window_measure_t[0].act_flag,			0 },
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ",				"rx_window_measure_t1.measure_obj",		4,		(UCHAR *)&payload.pm_edit_req.rx_window_measure_t[1].measure_obj,		0 },
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ",				"rx_window_measure_t1.act_flag",		2,		(UCHAR *)&payload.pm_edit_req.rx_window_measure_t[1].act_flag,			0 },
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ",				"rx_window_measure_t2.measure_obj",		4,		(UCHAR *)&payload.pm_edit_req.rx_window_measure_t[2].measure_obj,		0 },
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ",				"rx_window_measure_t2.act_flag",		2,		(UCHAR *)&payload.pm_edit_req.rx_window_measure_t[2].act_flag,			0 },
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ",				"rx_window_measure_t3.measure_obj",		4,		(UCHAR *)&payload.pm_edit_req.rx_window_measure_t[3].measure_obj,		0 },
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ",				"rx_window_measure_t3.act_flag",		2,		(UCHAR *)&payload.pm_edit_req.rx_window_measure_t[3].act_flag,			0 },
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ",				"rx_window_measure_t4.measure_obj",		4,		(UCHAR *)&payload.pm_edit_req.rx_window_measure_t[4].measure_obj,		0 },
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ",				"rx_window_measure_t4.act_flag",		2,		(UCHAR *)&payload.pm_edit_req.rx_window_measure_t[4].act_flag,			0 },
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ",				"rx_window_measure_t5.measure_obj",		4,		(UCHAR *)&payload.pm_edit_req.rx_window_measure_t[5].measure_obj,		0 },
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ",				"rx_window_measure_t5.act_flag",		2,		(UCHAR *)&payload.pm_edit_req.rx_window_measure_t[5].act_flag,			0 },
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ",	"slot_name",							16,		(UCHAR *)&payload.fdl_install_req.slot_name,							0 },
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ",	"file_num",								1,		(UCHAR *)&payload.fdl_install_req.file_num,								0 },
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ",	"file_name",							256,	(UCHAR *)&payload.fdl_install_req.file_name[0],							0 },
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ","slot_name",							16,		(UCHAR *)&payload.fdl_act_req.slot_name,								0 },
	{	"MSI_RETRIEVE_REQ",							"logical_path",							256,	(UCHAR *)&payload.ful_retrieve_req.logical_path,						0 },
	{	"MSI_RETRIEVE_REQ",							"file_name_filter",						256,	(UCHAR *)&payload.ful_retrieve_req.file_name_filter,					0 },
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ","uri",									256,	(UCHAR *)&payload.fdl_dwnld_req.uri,									0 },
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ","password",								256,	(UCHAR *)&payload.fdl_dwnld_req.password,								0 },
	{	"MSI_UPLOAD_REQ",							"local_logical_file_path",				256,	(UCHAR *)&payload.ful_upld_req.local_logical_file_path,					0 },
	{	"MSI_UPLOAD_REQ",							"remote_file_path",						256,	(UCHAR *)&payload.ful_upld_req.remote_file_path,						0 },
	{	"MSI_UPLOAD_REQ",							"password",								64,		(UCHAR *)&payload.ful_upld_req.password,								0 },
	{	NULL,										NULL,									0,		NULL,																	0 }
};

#define D_MSIDBG_LOGDISP	1

int func_msi_param( char	*msg_str, CHAR	*msiMsg, INT	data_size ) {

	FILE		*fp;
	int			payload_size = 0;

	INT			i = 0;
	char		msi_name[64];
	char		prm_name[64];
	char		line[256];
	INT			data[6] = { 0, 0, 0, 0, 0, 0 };
	INT			index = 0;

	INT			*int_data;
	SHORT		*sht_data;
	CHAR		*chr_data;


	memset( &payload, 0, sizeof(payload) );

	if( (fp = fopen("/tmp/msidbg.conf", "r")) != NULL ) { 

		while(1) {
			memset( line, 0, sizeof(line) );
			i = 0;

			if( fgets(line, 255, fp) == NULL) {
				break;
			}
			while( msidbg_test_param[i].size != 0 ) {

				if( (strstr(line, msg_str) != NULL)
				 && (strstr(line, msidbg_test_param[i].i_name) != NULL)
				 && (strstr(line, msidbg_test_param[i].p_name) != NULL) ) {

#if defined(D_MSIDBG_LOGDISP)	/* TODO：試験完了時に消す！	*/
/**/				printf( "[debug] msidbg.conf line  : %s", line );
/**/				printf( "        Table value index : %02d, interface : %s, parameter : %s, size : %02d, data : 0x%p \n",
/**/						i, msidbg_test_param[i].i_name, msidbg_test_param[i].p_name, msidbg_test_param[i].size, msidbg_test_param[i].data );
#endif
					if( msidbg_test_param[i].loop_flag == 0 ) {
						index = 0;
					}

					if( msidbg_test_param[i].size == 1 ) {

						sscanf( line, "%s %s %d", msi_name, prm_name, &data[0] );
						chr_data = (CHAR *)msidbg_test_param[i].data;
						*(chr_data + index) = (CHAR)data[0];

#if defined(D_MSIDBG_LOGDISP)	/* TODO：試験完了時に消す！	*/
/**/					printf( "        <<i_name : %s>>, <<p_name : %s>> / data[0] : %08x \n", msidbg_test_param[i].i_name, msidbg_test_param[i].p_name, data[0] );
/**/					printf( "        msidbg_test_param[i].data(%p) = %02x, data[0] = %02x \n", (chr_data + index), *(chr_data + index), (UCHAR)data[0] );
#endif

					} else if( msidbg_test_param[i].size == 2 ) {

						sscanf( line, "%s %s %d", msi_name, prm_name, &data[0] );

						sht_data = (SHORT *)msidbg_test_param[i].data;
						*(sht_data + index) = (SHORT)data[0];
#if defined(D_MSIDBG_LOGDISP)	/* TODO：試験完了時に消す！	*/
/**/					printf( "        <<i_name : %s>>, <<p_name : %s>> / data[0] : %08x \n", msidbg_test_param[i].i_name, msidbg_test_param[i].p_name, data[0] );
/**/					printf( "        msidbg_test_param[i].data(%p) = %04x, data[0] = %04x \n", (sht_data + index), *(sht_data + index), (USHORT)data[0] );
#endif

					} else if( msidbg_test_param[i].size == 4 ) {

						sscanf( line, "%s %s %d", msi_name, prm_name, &data[0] );
						int_data = (INT *)msidbg_test_param[i].data;
						*(int_data + index) = (INT)data[0];
#if defined(D_MSIDBG_LOGDISP)	/* TODO：試験完了時に消す！	*/
/**/					printf( "        <<i_name : %s>>, <<p_name : %s>> / data[0] : %08x \n", msidbg_test_param[i].i_name, msidbg_test_param[i].p_name, data[0] );
/**/					printf( "        msidbg_test_param[i].data(%p) = %08x, data[0] = %08x \n", (int_data + index), *(int_data + index), data[0] );
#endif

					} else if( msidbg_test_param[i].size == 6 ) {

						sscanf( line, "%s %s %d:%d:%d:%d:%d:%d", msi_name, prm_name, &data[0], &data[1], &data[2], &data[3], &data[4], &data[5] );
						chr_data        = (CHAR *)msidbg_test_param[i].data;
						*(chr_data)     = (CHAR)data[0];
						*(chr_data + 1) = (CHAR)data[1];
						*(chr_data + 2) = (CHAR)data[2];
						*(chr_data + 3) = (CHAR)data[3];
						*(chr_data + 4) = (CHAR)data[4];
						*(chr_data + 5) = (CHAR)data[5];
#if defined(D_MSIDBG_LOGDISP)	/* TODO：試験完了時に消す！	*/
/**/					printf( "        <<i_name : %s>>, <<p_name : %s>> / data : %08x, %08x, %08x, %08x, %08x, %08x \n", 
/**/							msidbg_test_param[i].i_name, msidbg_test_param[i].p_name, data[0], data[1], data[2], data[3], data[4], data[5] );
/**/					printf( "        msidbg_test_param[i].data(%p) = %02x, data[0] = %08x \n", (chr_data),     *(chr_data),     (UCHAR)data[0] );
/**/					printf( "        msidbg_test_param[i].data(%p) = %02x, data[0] = %08x \n", (chr_data + 1), *(chr_data + 1), (UCHAR)data[1] );
/**/					printf( "        msidbg_test_param[i].data(%p) = %02x, data[0] = %08x \n", (chr_data + 2), *(chr_data + 2), (UCHAR)data[2] );
/**/					printf( "        msidbg_test_param[i].data(%p) = %02x, data[0] = %08x \n", (chr_data + 3), *(chr_data + 3), (UCHAR)data[3] );
/**/					printf( "        msidbg_test_param[i].data(%p) = %02x, data[0] = %08x \n", (chr_data + 4), *(chr_data + 4), (UCHAR)data[4] );
/**/					printf( "        msidbg_test_param[i].data(%p) = %02x, data[0] = %08x \n", (chr_data + 5), *(chr_data + 5), (UCHAR)data[5] );
#endif

					} else {
						sscanf( line, "%s %s %s", msi_name, prm_name, msidbg_test_param[i].data );
#if defined(D_MSIDBG_LOGDISP)	/* TODO：試験完了時に消す！	*/
/**/					printf( "        <<i_name : %s>>, <<p_name : %s>> / data : %s \n", 
/**/							msidbg_test_param[i].i_name, msidbg_test_param[i].p_name, msidbg_test_param[i].data );
#endif
					}
					payload_size += msidbg_test_param[i].size;
					if( msidbg_test_param[i].loop_flag != 0 ) {
						index++;
					}
#if defined(D_MSIDBG_LOGDISP)	/* TODO：試験完了時に消す！	*/
/**/				printf( "        <<payload_size = %d (+%d)>> \n", payload_size, msidbg_test_param[i].size );
#endif
					break;
				}
				i++;
			}
		}
		fclose(fp);
	}
	if( payload_size != 0 ) {
		memcpy( msiMsg, &payload, data_size );
	}
	return	payload_size;
}

/**************************************************************************************************/
/*!
 *  @brief  main
 *  @note   msi debug表示
 *  @return None
 *  @date   
***************************************************************************************************/
int main(int argc , char** argv)
{
	int					rtn;
	int					result;

	char				text_str[4096];
	int					text_str_len;
	char*				text_str_ptr;
	UINT				index;
	T_MSI_APP_LIST		Nsa_pList;
	T_MSI_APP_LIST		Saa_pList;
	T_MSI_APP_LIST*		Nsa_pList_p = NULL;
	T_MSI_APP_LIST*		Saa_pList_p = NULL;
	CHAR				Nsa_pram[32][128];
	CHAR				Saa_pram[32][128];
	CHAR*				Nsa_prams = NULL;
	CHAR*				Saa_prams = NULL;
	pid_t				Nsa_Pid;
	pid_t				Saa_Pid;
	T_MSI_HANDLE		msi_handle;
	int					subscribeflg = 0;
	int					testmodeflg = 0;
	CHAR				ipAddr[32];
	char				msg_str[256];
	INT					msiLen;
	CHAR				msiMsg[0x10000];
	UINT				count = 0;
	UINT				msgid = 0;
	INT					portno = 65100;
	int					sd;
	struct sockaddr_in	ssaddr;
	INT					hlen;
	char* 				endptr = "\0";
	int					errorCode;
	UINT				optLen;

	if(argc < 2){
		func_usage(argv);
		return 0;
	}

	while((rtn = getopt(argc,argv,"vhlN:S:c:t:m:p:d:")) != -1){
		switch(rtn){
			case 'N':
				memset(text_str,0,sizeof(text_str));
				if((text_str_len = snprintf(text_str,sizeof(text_str),"%s",optarg)) <= 0){
					printf("%c option input error \n",rtn);
					func_usage(argv);
					return 1;
				}
				memset(&Nsa_pList,0,sizeof(Nsa_pList));
				memset(Nsa_pram,0,sizeof(Nsa_pram));
				strncpy(Nsa_pram[0],"netconfd-pro",strlen("netconfd-pro"));
				index = 1;
				text_str_ptr = text_str;
				while(sscanf(text_str_ptr,"%s",Nsa_pram[index]) != EOF){
					func_vmsg("%s\n",Nsa_pram[index]);
					text_str_ptr += strlen(Nsa_pram[index]);
					text_str_ptr++; /* spase */
					if(text_str_ptr >= (text_str+text_str_len)){
						break;
					}
					index++;
					if(index >= (sizeof(Nsa_pram)/sizeof(Nsa_pram[0]))){
						break;
					}
				}
				Nsa_pList.startPrg = (const char*)Nsa_pram[0];
				Nsa_prams = Nsa_pram[0];
				Nsa_pList.startOpt = &Nsa_prams;
				Nsa_pList_p = &Nsa_pList;
				break;
			case 'S':
				memset(text_str,0,sizeof(text_str));
				if((text_str_len = snprintf(text_str,sizeof(text_str),"%s",optarg)) <= 0){
					printf("%c option input error \n",rtn);
					func_usage(argv);
					return 1;
				}
				memset(&Saa_pList,0,sizeof(Saa_pList));
				memset(Saa_pram,0,sizeof(Saa_pram));
				strncpy(Saa_pram[0],"DU_agent",strlen("DU_agent"));
				index = 1;
				text_str_ptr = text_str;
				while(sscanf(text_str_ptr,"%s",Saa_pram[index]) != EOF){
					func_vmsg("%s\n",Saa_pram[index]);
					text_str_ptr += strlen(Saa_pram[index]);
					text_str_ptr++; /* spase */
					if(text_str_ptr >= (text_str+text_str_len)){
						break;
					}
					index++;
					if(index >= (sizeof(Saa_pram)/sizeof(Saa_pram[0]))){
						break;
					}
				}
				Saa_pList.startPrg = (const char*)Saa_pram[0];
				Saa_prams = Saa_pram[0];
				Saa_pList.startOpt = &Saa_prams;
				Saa_pList_p = &Saa_pList;
				break;
			case 'v':
				vmode_flg = 1;
				break;
			case 'c':
				memset(ipAddr,0,sizeof(ipAddr));
				if((text_str_len = snprintf(ipAddr,sizeof(ipAddr),"%s",optarg)) <= 0){
					strncpy(ipAddr,"127.0.0.1",strlen("127.0.0.1"));
				}
				subscribeflg = 1;
				break;
			case 't':
				if(subscribeflg == 0){
					memset(ipAddr,0,sizeof(ipAddr));
					strncpy(ipAddr,"127.0.0.1",strlen("127.0.0.1"));
				}
				subscribeflg = 1;
				memset(msg_str,0,sizeof(msg_str));
				if((text_str_len = snprintf(msg_str,sizeof(msg_str),"%s",optarg)) <= 0){
					printf("%c option input error \n",rtn);
					func_usage(argv);
					return 1;
				}
				msgid = 0xFFFFFFFF;
				for(count = 0; ; count++){
					if(msidbg_test_data[count].name == NULL){
						break;
					}
					if(!strncmp(msg_str,msidbg_test_data[count].name,text_str_len)){
						msgid = msidbg_test_data[count].msgid;
						break;
					}
				}
				if(msgid == 0xFFFFFFFF){
					printf("Not entry msg %s\n",msg_str);
					func_usage(argv);
					return 1;
				}
				testmodeflg = 1;	
				break;
			case 'p':
				if((portno = strtol(optarg, NULL, 10)) == 0){
					func_usage(argv);
					return 1;
				}
				break;
			case 'm':
				memset(msg_str,0,sizeof(msg_str));
				if((text_str_len = snprintf(msg_str,sizeof(msg_str),"%s",optarg)) <= 0){
					printf("%c option input error \n",rtn);
					func_usage(argv);
					return 1;
				}
				for(count = 0; ; count++){
					if(msidbg_test_data[count].name == NULL){
						break;
					}
					if(!strncmp(msg_str,msidbg_test_data[count].name,text_str_len)){
						memset(msiMsg,0,sizeof(msiMsg));
						/* ↓ここから */
						if( msidbg_test_data[count].flag == 1 ) {
							func_msi_param( msidbg_test_data[count].name, &msiMsg[32], msidbg_test_data[count].datasize );
						} 
						/* ↑ここまで */
						if((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
							printf("socket error \n");
							break;
						}
						hlen= sizeof(ssaddr);
						ssaddr.sin_family      = AF_INET;
						ssaddr.sin_port        = htons(msidbg_test_data[count].portno);
						ssaddr.sin_addr.s_addr = htonl(0x7F000001);

						((T_MPSW_SRV_MSGHEAD*)msiMsg)->msgId = msidbg_test_data[count].msgid;
						((T_MPSW_SRV_MSGHEAD*)msiMsg)->requestId = rand();
						((T_MPSW_SRV_MSGHEAD*)msiMsg)->msgLength = msidbg_test_data[count].datasize - sizeof(T_MPSW_SRV_MSGHEAD);

						(void)sendto(sd,msiMsg, msidbg_test_data[count].datasize , 0, (const struct sockaddr *)&ssaddr,hlen);
						printf("MSI sent(%s) port:%d \n",strerror(errno),msidbg_test_data[count].portno);
						close(sd);
						break;
					}
				}
				return 0;
				break;
			case 'd':
				memset(msiMsg,0,sizeof(msiMsg));

				errorCode = strtol(optarg, &endptr, 16);
				if(errno == ERANGE){
					func_usage(argv);
					return 1;
				}
				if( strlen(endptr)!= 0 ){
					func_usage(argv);
					return 1;
				}
				memset(text_str,0,sizeof(text_str));
				if(errorCode == 0){
					optLen = 0;
				} else {
					optLen = sizeof(T_MPSW_SRV_MSGERROR);
					if(optind < argc){
						if((text_str_len = snprintf(text_str,sizeof(text_str),"%s",argv[optind])) <= 0){
							func_usage(argv);
							return 1;
						}
						if(text_str_len >= sizeof(((T_MSG_MSI_DEBUG_IND*)msiMsg)->data.msgErr.errorDescription)){
							func_usage(argv);
							return 1;
						}
					}else{
						snprintf(text_str,sizeof(text_str),"MSI Reject Mode");
					}
				}

				if((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
					printf("socket error \n");
					return 1;

				}

				hlen= sizeof(ssaddr);
				ssaddr.sin_family      = AF_INET;
				ssaddr.sin_port        = htons(portno);
				ssaddr.sin_addr.s_addr = htonl(0x7F000001);

				((T_MPSW_SRV_MSGHEAD*)msiMsg)->msgId = D_MSG_MSI_DEBUG_IND;
				((T_MPSW_SRV_MSGHEAD*)msiMsg)->msgLength = sizeof(T_MSG_MSI_DEBUG_IND) - sizeof(T_MPSW_SRV_MSGHEAD);

				((T_MSG_MSI_DEBUG_IND*)msiMsg)->data.optLen = optLen;
				if(optLen > 0){
					((T_MSG_MSI_DEBUG_IND*)msiMsg)->data.opeCode = 1;
					((T_MSG_MSI_DEBUG_IND*)msiMsg)->data.msgErr.errorCode = errorCode;
					strncpy(((T_MSG_MSI_DEBUG_IND*)msiMsg)->data.msgErr.errorDescription,text_str,sizeof(((T_MSG_MSI_DEBUG_IND*)msiMsg)->data.msgErr.errorDescription));
				}else{
					((T_MSG_MSI_DEBUG_IND*)msiMsg)->data.opeCode = 0;
				}
				(void)sendto(sd,msiMsg, sizeof(T_MSG_MSI_DEBUG_IND) , 0, (const struct sockaddr *)&ssaddr,hlen);
				close(sd);
				printf("MSI Debug send(0x%x 0x%x %s)\n",errorCode,optLen,text_str);
				break;
			case 'l':
				func_msi_list();
				break;
			case 'h':
				func_usage(argv);
				return 0;
			default:
				break;
		}
	}

	if((Nsa_pList_p != NULL) || (Saa_pList_p != NULL)){
		if(( result = msi_app_initialize(Nsa_pList_p,Saa_pList_p,10000,NULL,&Nsa_Pid,&Saa_Pid))!= MSI_FUNCIF_OK ){
			func_vmsg("msi_app_initialize Error\n");
			return 1;
		}
	}

	if(subscribeflg != 0){
		T_MSI_SUBSCRIBE_MOD_INFO* modInfo_p;
		UINT					modInfoNum = sizeof(msidbg_l_modInfo)/sizeof(msidbg_l_modInfo[0]) ;
		UINT					modIndex;
		
		memset(msidbg_l_modInfo,0,sizeof(msidbg_l_modInfo));
		for(modIndex=0 ; modIndex < modInfoNum ; modIndex++){
			msidbg_l_modInfo[modIndex].modId = modIndex;
			msidbg_l_modInfo[modIndex].modOptBit = E_MSI_MSGOPE_ALL;
		}
		modInfo_p = msidbg_l_modInfo;
		func_vmsg("msi_subscribe start %s %d\n",ipAddr,modInfoNum);
		if(( result = msi_subscribe(0,ipAddr,modInfoNum,&modInfo_p,NULL,NULL,NULL,&msi_handle)) != MSI_FUNCIF_OK ){
			func_vmsg("msi_subscribe Error\n");
			return 1;
		}

	}
	
	if(testmodeflg != 0 ){
		func_vmsg("msi_receive_from_agent start...\n");
		while(1){
			if(( result = msi_receive_from_agent(&msi_handle,sizeof(msiMsg),&msiLen,msiMsg)) == MSI_FUNCIF_OK ){
				func_vmsg("msgid received 0x%08x \n",((T_MPSW_SRV_MSGHEAD*)msiMsg)->msgId);
				if(((T_MPSW_SRV_MSGHEAD*)msiMsg)->msgId == msgid){
					((T_MPSW_SRV_MSGHEAD*)msiMsg)->msgId = msgid + 1;
					for(count = 0; ; count++){
						if( msidbg_test_data[count].msgid == ((T_MPSW_SRV_MSGHEAD*)msiMsg)->msgId ){
							func_vmsg("msi_send_to_agent 0x%08x\n",((T_MPSW_SRV_MSGHEAD*)msiMsg)->msgId);
							msi_send_to_agent(&msi_handle,msidbg_test_data[count].datasize,msiMsg);
							break;
						}
					}
					break;
				}
			}else{
				func_vmsg("msi_receive_from_agent error\n");
				break;
			}
		}
	}
	
	if(subscribeflg != 0){
		if(( result = msi_desubscribe(&msi_handle)) !=0 ){
			func_vmsg("msi_desubscribe Error\n");
			return 1;
		}
	}

	return(0);
}

/* @} */
