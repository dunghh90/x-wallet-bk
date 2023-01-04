#include "rrupfdump.h"
#include "m_cm_def.h"
#include "f_sys_inc.h"
#include "rrhApi_Com.h"
#include "rrhApi_Cpri.h"
#include "rrhApi_Mcpri.h"
#include "rrhApi_Nego.h"
#include "rrhApi_Dbg.h"
#include "rrhApi_Dia.h"
#include "rrhApi_Ext.h"
#include "rrhApi_File.h"
#include "rrhApi_Inv.h"
#include "rrhApi_L2.h"
#include "rrhApi_Log.h"
#include "rrhApi_Rfp.h"
#include "rrhApi_Svp.h"
#include "rrhApi_Tilt.h"
#include "duApi_Mpn.h"
#include "duApi_Ptp.h"

pcap_thrd_name rrupf_thd_name_init [] = 
{
	{"main"},		/* 0x10 */
	{"alm "},		/* 0x11 */
	{"hlt "},		/* 0x12 */
	{"run "},		/* 0x13 */
	{"con "},		/* 0x14 */
	{"who?"}
};

pcap_thrd_name rrupf_thd_name_pf [] = 
{
	{"main "},		/* 0x20 1 */
	{"recv "},   	/* 0x21 2 */
	{"send "},   	/* 0x22 3 */
	{"debg "},   	/* 0x23 4 */
	{"hdl  "},   	/* 0x24 5 */
	{"eqs  "},   	/* 0x25 6 */
	{"wdt  "},   	/* 0x26 7 */
	{"nma  "},   	/* 0x27 8 */
	{"nam_s"},   	/* 0x28 9 */
	{"lgc  "},    	/* 0x29 10 */
	{"cca  "},   	/* 0x2A 11 */
	{"rfa  "},    	/* 0x2B 12 */
	{"dhcp "},   	/* 0x2C 13 */
	{"sfp  "},   	/* 0x2D 14 */
	{"who? "},   	/* 0x2E 15 */
	{"fhs  "},   	/* 0x2F 16 */
	{"mprxr"},    	/* 0x30 17 */
	{"mprxs"},   	/* 0x31 18 */
	{"eom  "},   	/* 0x32 19 */
	{"eom1 "},    	/* 0x33 20 */
	{"eom2 "},    	/* 0x34 21 */
	{"fdl  "},  	/* 0x35 22 */
	{"ful  "},  	/* 0x36 23 */
	{"pfm  "},  	/* 0x37 24 */
	{"eio  "},  	/* 0x38 25 */
	{"mti  "},  	/* 0x39 26 */
	{"who? "},  	/* 0x3A 27 */
	{"who? "},  	/* 0x3B 28 */
	{"who? "},  	/* 0x3C 29 */
	{"who? "},  	/* 0x3D 30 */
	{"who? "},  	/* 0x3E 31 */
	{"who? "},  	/* 0x3F 32 */
	{"hdwdt"},  	/* 0x40 33 */
	{"hd1s "},  	/* 0x41 34 */
	{"hd1ps"},  	/* 0x42 35 */
	{"hdsv "},  	/* 0x43 36 */
	{"hdms1"},  	/* 0x44 37 */
	{"hdms2"},  	/* 0x45 38 */
	{"who? "}, 		/* 0x46 39 */
	{"who? "},  	/* 0x47 */
	{"who? "}, 		/* 0x48 */
	{"who? "},  	/* 0x49 */
	{"who? "},  	/* 0x4A */
	{"who? "}, 	 	/* 0x4B */
	{"who? "},		/* 0x4C */
	{"who? "},		/* 0x4D */
	{"who? "},		/* 0x4E */
	{"who? "}
};

pcap_thrd_name rrupf_thd_name_ptp [] = 
{
	{"prnt "},		/* 0x20 */
	{"recv "},   	/* 0x21 */
	{"send "},   	/* 0x22 */
	{"debg "},   	/* 0x23 */
	{"who? "},   	/* 0x24 */
	{"who? "},   	/* 0x25 */
	{"who? "},   	/* 0x26 */
	{"who? "},   	/* 0x27 */
	{"who? "},   	/* 0x28 */
	{"who? "},    	/* 0x29 */
	{"who? "},   	/* 0x2A */
	{"who? "},    	/* 0x2B */
	{"who? "},   	/* 0x2C */
	{"who? "},   	/* 0x2D */
	{"ptp  "},   	/* 0x2E */
	{"who? "},   	/* 0x2F */
	{"mprxr"},    	/* 0x30 */
	{"mprxs"},   	/* 0x31 */
	{"who? "},   	/* 0x32 */
	{"who? "},    	/* 0x33 */
	{"who? "},    	/* 0x34 */
	{"who? "},  	/* 0x35 */
	{"who? "},  	/* 0x36 */
	{"who? "},  	/* 0x37 */
	{"who? "},  	/* 0x38 */
	{"who? "},  	/* 0x39 */
	{"who? "},  	/* 0x3A */
	{"who? "},  	/* 0x3B */
	{"who? "},  	/* 0x3C */
	{"who? "},  	/* 0x3D */
	{"who? "},  	/* 0x3E */
	{"who? "},  	/* 0x3F */
	{"who? "},  	/* 0x40 */
	{"who? "},  	/* 0x41 */
	{"who? "},  	/* 0x42 */
	{"who? "},  	/* 0x43 */
	{"who? "},  	/* 0x44 */
	{"who? "},  	/* 0x45 */
	{"who? "}, 		/* 0x46 */
	{"who? "},  	/* 0x47 */
	{"who? "}, 		/* 0x48 */
	{"who? "},  	/* 0x49 */
	{"who? "},  	/* 0x4A */
	{"who? "}, 	 	/* 0x4B */
	{"who? "},		/* 0x4C */
	{"who? "},		/* 0x4D */
	{"who? "},		/* 0x4E */
	{"who? "}
};

pcap_thrd_name rrupf_thd_name_l2 [] = 
{
	{"main"},		/* 0x40 */
	{"lpb "},   	/* 0x41 */
	{"pol "},   	/* 0x42 */
	{"txs "},   	/* 0x43 */
	{"rxs "},   	/* 0x44 */
	{"cmus"},   	/* 0x45 */
	{"cmur"},   	/* 0x46 */
	{"dbg "},   	/* 0x47 */
	{"who?"}
};

pcap_thrd_name rrupf_thd_name_l3 [] = 
{
	{"main"},		/* 0x50 */
	{"cpri"},   	/* 0x51 */
	{"rct "},   	/* 0x52 */
	{"dlm "},   	/* 0x53 */
	{"ext "},   	/* 0x54 */
	{"log "},   	/* 0x55 */
	{"diag"},   	/* 0x56 */
	{"cmus"},   	/* 0x57 */
	{"cmur"},   	/* 0x58 */
	{"dbg "},    	/* 0x59 */
	{"who?"}
};

pcap_thrd_name rrupf_thd_name_mnt [] = 
{
	{"main"},		/* 0x60 */
	{"mtm "},   	/* 0x61 */
	{"mkm "},   	/* 0x62 */
	{"mlr "},   	/* 0x63 */
	{"mls "},   	/* 0x64 */
	{"lnr "},   	/* 0x65 */
	{"lns "},   	/* 0x66 */
	{"cmur"},   	/* 0x67 */
	{"cmus"},   	/* 0x68 */
	{"dbg "},    	/* 0x69 */
	{"who?"}
};

pcap_thrd_name rrupf_thd_name_pl [] = 
{
	{"main"},		/* 0x80 */
	{"lpb "},   	/* 0x81 */
	{"pol "},   	/* 0x82 */
	{"txs "},   	/* 0x83 */
	{"rxs "},   	/* 0x84 */
	{"cmus"},   	/* 0x85 */
	{"cmur"},   	/* 0x86 */
	{"dbg "},   	/* 0x87 */
	{"who?"}
};


pcap_thrd_name rrupf_thd_name_oth_if [] = 
{
	{"who?"}
};

int func_ptif_dummy_if(FILE* fp , char* data_p ,int datalen , unsigned int kind)
{
	return 0;
}


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
#include "f_mpsw_msg_o-ran_supervison_def.h"
#include "f_mpsw_msg_o-ran_supervison_typ.h"

typedef struct {
	char*	name;
	UINT	msgid;
	INT		datasize;
}T_TEST_MSG;

static T_TEST_MSG msidbg_test_data [] = 
{
	{	"MSI_O_RAN_SYNC_GET_REQ"								,D_MSG_MSI_O_RAN_SYNC_GET_REQ							,sizeof(T_MSG_MSI_O_RAN_SYNC_GET_REQ)							},
	{	"MSI_O_RAN_SYNC_GET_CFM"								,D_MSG_MSI_O_RAN_SYNC_GET_CFM							,sizeof(T_MSG_MSI_O_RAN_SYNC_GET_CFM)							},
	{	"MSI_O_RAN_SYNC_EDIT_REQ"							,D_MSG_MSI_O_RAN_SYNC_EDIT_REQ							,sizeof(T_MSG_MSI_O_RAN_SYNC_EDIT_REQ)							},
	{	"MSI_O_RAN_SYNC_EDIT_CFM"							,D_MSG_MSI_O_RAN_SYNC_EDIT_CFM							,sizeof(T_MSG_MSI_O_RAN_SYNC_EDIT_CFM)							},
	{	"MSI_O_RAN_SYNC_EDIT_REJ"							,D_MSG_MSI_O_RAN_SYNC_EDIT_REJ							,sizeof(T_MSG_MSI_O_RAN_SYNC_EDIT_REJ)							},
	{	"MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND"			,D_MSG_MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND			,sizeof(T_MSG_MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND)			},
	{	"MSI_O_RAN_SYNC_PTPSTATE_CHANGE_NTF_IND"				,D_MSG_MSI_O_RAN_SYNC_PTPSTATE_CHANGE_NTF_IND			,sizeof(T_MSG_MSI_O_RAN_SYNC_PTPSTATE_CHANGE_NTF_IND)			},
	{	"MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND"			,D_MSG_MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND			,sizeof(T_MSG_MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND)			},
	{	"MSI_O_RAN_TRANSCEIVER_GET_REQ"						,D_MSG_MSI_O_RAN_TRANSCEIVER_GET_REQ						,sizeof(T_MSG_MSI_O_RAN_TRANSCEIVER_GET_REQ)						},
	{	"MSI_O_RAN_TRANSCEIVER_GET_CFM"						,D_MSG_MSI_O_RAN_TRANSCEIVER_GET_CFM						,sizeof(T_MSG_MSI_O_RAN_TRANSCEIVER_GET_CFM)						},
	{	"MSI_O_RAN_TRANSCEIVER_GET_REJ"						,D_MSG_MSI_O_RAN_TRANSCEIVER_GET_REJ						,sizeof(T_MSG_MSI_O_RAN_TRANSCEIVER_GET_REJ)						},
	{	"MSI_O_RAN_OPERATIONS_GET_REQ"						,D_MSG_MSI_O_RAN_OPERATIONS_GET_REQ						,sizeof(T_MSG_MSI_O_RAN_OPERATIONS_GET_REQ)						},
	{	"MSI_O_RAN_OPERATIONS_GET_CFM"						,D_MSG_MSI_O_RAN_OPERATIONS_GET_CFM						,sizeof(T_MSG_MSI_O_RAN_OPERATIONS_GET_CFM)						},
	{	"MSI_O_RAN_OPERATIONS_GET_REJ"						,D_MSG_MSI_O_RAN_OPERATIONS_GET_REJ						,sizeof(T_MSG_MSI_O_RAN_OPERATIONS_GET_REJ)						},
	{	"MSI_O_RAN_OPERATIONS_RESET_IND"						,D_MSG_MSI_O_RAN_OPERATIONS_RESET_IND						,sizeof(T_MSG_MSI_O_RAN_OPERATIONS_RESET_IND)						},
	{	"MSI_READY_IND"										,D_MSG_MSI_READY_IND									,sizeof(T_MSG_MSI_READY_IND)									},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REQ"	,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REQ	,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REQ)	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM"	,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM	,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM)	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ"	,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ	,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ)	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ		,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ)		},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM		,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM)		},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ		,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ)		},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ			,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ)			},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_CFM"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_CFM			,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_CFM)			},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ			,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ)			},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ		,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ)		},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM		,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM)		},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ"			,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ		,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ)		},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND"		,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND	,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND)	},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND"		,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND		,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND)		},
	{	"MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND"		,D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND	,sizeof(T_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND)	},
	{	"MSI_IETF_INTERFACES_GET_REQ"						,D_MSG_MSI_IETF_INTERFACES_GET_REQ						,sizeof(T_MSG_MSI_IETF_INTERFACES_GET_REQ)						},
	{	"MSI_IETF_INTERFACES_GET_CFM"						,D_MSG_MSI_IETF_INTERFACES_GET_CFM						,sizeof(T_MSG_MSI_IETF_INTERFACES_GET_CFM)						},
	{	"MSI_IETF_INTERFACES_GET_REJ"						,D_MSG_MSI_IETF_INTERFACES_GET_REJ						,sizeof(T_MSG_MSI_IETF_INTERFACES_GET_REJ)						},
	{	"MSI_IETF_IP_EDIT_REQ"								,D_MSG_MSI_IETF_IP_EDIT_REQ								,sizeof(T_MSG_MSI_IETF_IP_EDIT_REQ)								},
	{	"MSI_IETF_IP_EDIT_CFM"								,D_MSG_MSI_IETF_IP_EDIT_CFM								,sizeof(T_MSG_MSI_IETF_IP_EDIT_CFM)								},
	{	"MSI_IETF_IP_EDIT_REJ"								,D_MSG_MSI_IETF_IP_EDIT_REJ								,sizeof(T_MSG_MSI_IETF_IP_EDIT_REJ)								},
	{	"MSI_O_RAN_DHCP_GET_REQ"								,D_MSG_MSI_O_RAN_DHCP_GET_REQ							,sizeof(T_MSG_MSI_O_RAN_DHCP_GET_REQ)							},
	{	"MSI_O_RAN_DHCP_GET_CFM"								,D_MSG_MSI_O_RAN_DHCP_GET_CFM							,sizeof(T_MSG_MSI_O_RAN_DHCP_GET_CFM)							},
	{	"MSI_O_RAN_DHCP_GET_REJ"								,D_MSG_MSI_O_RAN_DHCP_GET_REJ							,sizeof(T_MSG_MSI_O_RAN_DHCP_GET_REJ)							},
	{	"MSI_O_RAN_INTERFACE_EDIT_REQ"						,D_MSG_MSI_O_RAN_INTERFACE_EDIT_REQ						,sizeof(T_MSG_MSI_O_RAN_INTERFACE_EDIT_REQ)						},
	{	"MSI_O_RAN_INTERFACE_EDIT_CFM"						,D_MSG_MSI_O_RAN_INTERFACE_EDIT_CFM						,sizeof(T_MSG_MSI_O_RAN_INTERFACE_EDIT_CFM)						},
	{	"MSI_O_RAN_INTERFACE_EDIT_REJ"						,D_MSG_MSI_O_RAN_INTERFACE_EDIT_REJ						,sizeof(T_MSG_MSI_O_RAN_INTERFACE_EDIT_REJ)						},
	{	"MSI_O_RAN_MPLANE_INT_EDIT_REQ"						,D_MSG_MSI_O_RAN_MPLANE_INT_EDIT_REQ						,sizeof(T_MSG_MSI_O_RAN_MPLANE_INT_EDIT_REQ)						},
	{	"MSI_O_RAN_MPLANE_INT_EDIT_CFM"						,D_MSG_MSI_O_RAN_MPLANE_INT_EDIT_CFM						,sizeof(T_MSG_MSI_O_RAN_MPLANE_INT_EDIT_CFM)						},
	{	"MSI_O_RAN_MPLANE_INT_EDIT_REJ"						,D_MSG_MSI_O_RAN_MPLANE_INT_EDIT_REJ						,sizeof(T_MSG_MSI_O_RAN_MPLANE_INT_EDIT_REJ)						},
	{	"MSI_O_RAN_USERMGMT_EDIT_REQ"						,D_MSG_MSI_O_RAN_USERMGMT_EDIT_REQ						,sizeof(T_MSG_MSI_O_RAN_USERMGMT_EDIT_REQ)						},
	{	"MSI_O_RAN_USERMGMT_EDIT_CFM"						,D_MSG_MSI_O_RAN_USERMGMT_EDIT_CFM						,sizeof(T_MSG_MSI_O_RAN_USERMGMT_EDIT_CFM)						},
	{	"MSI_O_RAN_USERMGMT_EDIT_REJ"						,D_MSG_MSI_O_RAN_USERMGMT_EDIT_REJ						,sizeof(T_MSG_MSI_O_RAN_USERMGMT_EDIT_REJ)						},
	{	"MSI_O_RAN_LBM_EDIT_REQ"								,D_MSG_MSI_O_RAN_LBM_EDIT_REQ							,sizeof(T_MSG_MSI_O_RAN_LBM_EDIT_REQ)							},
	{	"MSI_O_RAN_LBM_EDIT_CFM"								,D_MSG_MSI_O_RAN_LBM_EDIT_CFM							,sizeof(T_MSG_MSI_O_RAN_LBM_EDIT_CFM)							},
	{	"MSI_O_RAN_LBM_EDIT_REJ"								,D_MSG_MSI_O_RAN_LBM_EDIT_REJ							,sizeof(T_MSG_MSI_O_RAN_LBM_EDIT_REJ)							},
	{	"MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ"				,D_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ				,sizeof(T_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ)				},
	{	"MSI_O_RAN_PROCESSING_ELEMENT_EDIT_CFM"				,D_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_CFM				,sizeof(T_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_CFM)				},
	{	"MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REJ"				,D_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REJ				,sizeof(T_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REJ)				},
	{	"MSI_O_RAN_UPLANE_CONF_GET_REQ"						,D_MSG_MSI_O_RAN_UPLANE_CONF_GET_REQ						,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_GET_REQ)						},
	{	"MSI_O_RAN_UPLANE_CONF_GET_CFM"						,D_MSG_MSI_O_RAN_UPLANE_CONF_GET_CFM						,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_GET_CFM)						},
	{	"MSI_O_RAN_UPLANE_CONF_GET_REJ"						,D_MSG_MSI_O_RAN_UPLANE_CONF_GET_REJ						,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_GET_REJ)						},
	{	"MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ"					,D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ					,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ)					},
	{	"MSI_O_RAN_UPLANE_CONF_EDIT_TX_CFM"					,D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_CFM					,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_CFM)					},
	{	"MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ"					,D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ					,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REJ)					},
	{	"MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ"					,D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ					,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ)					},
	{	"MSI_O_RAN_UPLANE_CONF_EDIT_RX_CFM"					,D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_CFM					,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_CFM)					},
	{	"MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ"					,D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ					,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ)					},
	{	"MSI_O_RAN_UPLANE_CONF_TX_STATE_CHANGE_NTF_IND"		,D_MSG_MSI_O_RAN_UPLANE_CONF_TX_STATE_CHANGE_NTF_IND		,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_TX_STATE_CHANGE_NTF_IND)		},
	{	"MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND"		,D_MSG_MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND		,sizeof(T_MSG_MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND)		},
	{	"MSI_O_RAN_PFM_MNG_EDIT_REQ"							,D_MSG_MSI_O_RAN_PFM_MNG_EDIT_REQ						,sizeof(T_MSG_MSI_O_RAN_PFM_MNG_EDIT_REQ)						},
	{	"MSI_O_RAN_PFM_MNG_EDIT_CFM"							,D_MSG_MSI_O_RAN_PFM_MNG_EDIT_CFM						,sizeof(T_MSG_MSI_O_RAN_PFM_MNG_EDIT_CFM)						},
	{	"MSI_O_RAN_PFM_MNG_EDIT_REJ"							,D_MSG_MSI_O_RAN_PFM_MNG_EDIT_REJ						,sizeof(T_MSG_MSI_O_RAN_PFM_MNG_EDIT_REJ)						},
	{	"MSI_O_RAN_PFM_MNG_GET_REQ"							,D_MSG_MSI_O_RAN_PFM_MNG_GET_REQ							,sizeof(T_MSG_MSI_O_RAN_PFM_MNG_GET_REQ)							},
	{	"MSI_O_RAN_PFM_MNG_GET_CFM"							,D_MSG_MSI_O_RAN_PFM_MNG_GET_CFM							,sizeof(T_MSG_MSI_O_RAN_PFM_MNG_GET_CFM)							},
	{	"MSI_O_RAN_PFM_MNG_GET_REJ"							,D_MSG_MSI_O_RAN_PFM_MNG_GET_REJ							,sizeof(T_MSG_MSI_O_RAN_PFM_MNG_GET_REJ)							},
	{	"MSI_O_RAN_PFM_MNG_NTF_IND"							,D_MSG_MSI_O_RAN_PFM_MNG_NTF_IND							,sizeof(T_MSG_MSI_O_RAN_PFM_MNG_NTF_IND)							},
	{	"MSI_IETF_HARDWARE_GET_REQ"							,D_MSG_MSI_IETF_HARDWARE_GET_REQ						,sizeof(T_MSG_MSI_IETF_HARDWARE_GET_REQ)						},
	{	"MSI_IETF_HARDWARE_GET_CFM"							,D_MSG_MSI_IETF_HARDWARE_GET_CFM						,sizeof(T_MSG_MSI_IETF_HARDWARE_GET_CFM)						},
	{	"MSI_IETF_HARDWARE_GET_REJ"							,D_MSG_MSI_IETF_HARDWARE_GET_REJ						,sizeof(T_MSG_MSI_IETF_HARDWARE_GET_REJ)						},
	{	"MSI_IETF_HARDWARE_STATE_GET_REQ"					,D_MSG_MSI_IETF_HARDWARE_STATE_GET_REQ					,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_GET_REQ)					},
	{	"MSI_IETF_HARDWARE_STATE_GET_CFM"					,D_MSG_MSI_IETF_HARDWARE_STATE_GET_CFM					,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_GET_CFM)					},
	{	"MSI_IETF_HARDWARE_STATE_GET_REJ"					,D_MSG_MSI_IETF_HARDWARE_STATE_GET_REJ					,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_GET_REJ)					},
	{	"MSI_IETF_HARDWARE_STATE_EDIT_REQ"					,D_MSG_MSI_IETF_HARDWARE_STATE_EDIT_REQ					,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_EDIT_REQ)					},
	{	"MSI_IETF_HARDWARE_STATE_EDIT_CFM"					,D_MSG_MSI_IETF_HARDWARE_STATE_EDIT_CFM					,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_EDIT_CFM)					},
	{	"MSI_IETF_HARDWARE_STATE_EDIT_REJ"					,D_MSG_MSI_IETF_HARDWARE_STATE_EDIT_REJ					,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_EDIT_REJ)					},
	{	"MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND"			,D_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND			,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_NTF_IND)			},
	{	"MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND"		,D_MSG_MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND		,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_OPER_ENABLED_NTF_IND)		},
	{	"MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND"		,D_MSG_MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND	,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_OPER_DISABLED_NTF_IND)	},
	{	"MSI_IETF_HARDWARE_STATE_CHANGE_START_IND"			,D_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_START_IND			,sizeof(T_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_START_IND)			},
	{	"MSI_O_RAN_HARDWARE_GET_REQ"							,D_MSG_MSI_O_RAN_HARDWARE_GET_REQ						,sizeof(T_MSG_MSI_O_RAN_HARDWARE_GET_REQ)						},
	{	"MSI_O_RAN_HARDWARE_GET_CFM"							,D_MSG_MSI_O_RAN_HARDWARE_GET_CFM						,sizeof(T_MSG_MSI_O_RAN_HARDWARE_GET_CFM)						},
	{	"MSI_O_RAN_HARDWARE_GET_REJ"							,D_MSG_MSI_O_RAN_HARDWARE_GET_REJ						,sizeof(T_MSG_MSI_O_RAN_HARDWARE_GET_REJ)						},

	{	"MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REQ"			,D_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REQ			,sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REQ		)		},
	{	"MSI_O_RAN_TROUBLESHOOTING_START_LOGS_CFM"			,D_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_CFM			,sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_CFM		)		},
	{	"MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REJ"			,D_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REJ			,sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REJ		)		},
	{	"MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REQ"			,D_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REQ			,sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REQ		)		},
	{	"MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_CFM"			,D_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_CFM			,sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_CFM		)		},
	{	"MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REJ"			,D_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REJ			,sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REJ		)		},
	{	"MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND"	,D_MSG_MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND	,sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND)		},

	{	"MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND"					,D_MSG_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND					,sizeof(T_MSG_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND)					},
	{	"MSI_O_RAN_FM_ALARM_START_IND"						,D_MSG_MSI_O_RAN_FM_ALARM_START_IND						,sizeof(T_MSG_MSI_O_RAN_FM_ALARM_START_IND)						},

	{	"MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ"				,D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ				,sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ)				},
	{	"MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_CFM"				,D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_CFM				,sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_CFM)				},
	{	"MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REJ"				,D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REJ				,sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REJ)				},
	{	"MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND"			,D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND			,sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND)			},
	{	"MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ"				,D_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ			,sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ)			},
	{	"MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM"				,D_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM			,sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM)			},
	{	"MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ"				,D_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ			,sizeof(T_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ)			},
	{	"MSI_O_RAN_EXTERNALIO_GET_REQ"						,D_MSG_MSI_O_RAN_EXTERNALIO_GET_REQ						,sizeof(T_MSG_MSI_O_RAN_EXTERNALIO_GET_REQ)						},
	{	"MSI_O_RAN_EXTERNALIO_GET_CFM"						,D_MSG_MSI_O_RAN_EXTERNALIO_GET_CFM						,sizeof(T_MSG_MSI_O_RAN_EXTERNALIO_GET_CFM)						},
	{	"MSI_O_RAN_EXTERNALIO_EDIT_REQ"						,D_MSG_MSI_O_RAN_EXTERNALIO_EDIT_REQ						,sizeof(T_MSG_MSI_O_RAN_EXTERNALIO_EDIT_REQ)						},
	{	"MSI_O_RAN_EXTERNALIO_EDIT_CFM"						,D_MSG_MSI_O_RAN_EXTERNALIO_EDIT_CFM						,sizeof(T_MSG_MSI_O_RAN_EXTERNALIO_EDIT_CFM)						},
	{	"MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND"			,D_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND			,sizeof(T_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND)			},
	{	"MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_START_IND"		,D_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_START_IND		,sizeof(T_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_START_IND)		},
	{	"MSI_O_RAN_FAN_GET_REQ"								,D_MSG_MSI_O_RAN_FAN_GET_REQ								,sizeof(T_MSG_MSI_O_RAN_FAN_GET_REQ)								},
	{	"MSI_O_RAN_FAN_GET_CFM"								,D_MSG_MSI_O_RAN_FAN_GET_CFM								,sizeof(T_MSG_MSI_O_RAN_FAN_GET_CFM)								},
	{	"MSI_O_RAN_SUPERVISION_NTF_START_IND"				,D_MSG_MSI_O_RAN_SUPERVISION_NTF_START_IND					,sizeof(T_MSG_MSI_O_RAN_SUPERVISION_NTF_START_IND)					},

	{	"MSI_SUBSCRIBE_REQ"									,D_MSG_MSI_SUBSCRIBE_REQ								,sizeof(T_MSG_MSI_SUBSCRIBE_REQ)								},
	{	"MSI_SUBSCRIBE_CFM"									,D_MSG_MSI_SUBSCRIBE_CFM								,sizeof(T_MSG_MSI_SUBSCRIBE_CFM)								},
	{	"MSI_SUBSCRIBE_REJ"									,D_MSG_MSI_SUBSCRIBE_REJ								,sizeof(T_MSG_MSI_SUBSCRIBE_REJ)								},
	{	"MSI_DESUBSCRIBE_IND"								,D_MSG_MSI_DESUBSCRIBE_IND								,sizeof(T_MSG_MSI_DESUBSCRIBE_IND)								},
	{	NULL												,0			                              			    ,0                                                              }
};

static int msi_head_size = sizeof(T_MPSW_SRV_MSGHEAD);

int func_ptif_msi_if(FILE* fp , char* data_p ,int datalen , unsigned int kind)
{
	int msidata_size = (((pcap_rrupf_userheader_if* )data_p)->len) - sizeof(pcap_rrupf_userheader_if);
	if((msidata_size < msi_head_size)){
		return 0;
	}

	T_MPSW_SRV_MSGHEAD* head_p = (T_MPSW_SRV_MSGHEAD*)(data_p + sizeof(pcap_rrupf_userheader_if));
	UINT	msgid 	  = head_p->msgId;
	UINT	requestId = head_p->requestId;
	CHAR*	msgname   = "Unknown MSI";
	UINT	count;
	for(count=0;;count++){
		if(msidbg_test_data[count].name == NULL){
			break;
		}
		if(msidbg_test_data[count].msgid == msgid){
			msgname = msidbg_test_data[count].name;
			break;
		}
	}
	fprintf(fp,"\n%s(0x%08X) ReqId:%d",msgname,msgid,requestId);
	return 0;
}


pcap_ptif_name rrupf_ptif_name_init [] = 
{
	{ 0x12345678							,0,"BPF_RM_SVRM_INITEND_IND"				,func_ptif_dummy_if},
	{ CMD_TSKIF_ALLTSKININTC				,0,"CMD_TSKIF_ALLTSKININTC"					,func_ptif_dummy_if},
	{ D_SYS_MSGID_ALLTSKININTC 				,0,"D_SYS_MSGID_ALLTSKININTC"				,func_ptif_dummy_if},
	{0,0,"Unknown",NULL}					/* END */
};

pcap_ptif_name rrupf_ptif_name_pf [] = 
{
	{ D_SYS_MSGID_TIMOUTNTC					,0,"D_SYS_MSGID_TIMOUTNTC"					,func_ptif_dummy_if  },
	{ D_SYS_MSGID_TRA_IRQNTC				,0,"D_SYS_MSGID_TRA_IRQNTC"					,func_ptif_dummy_if  },
	{ D_SYS_MSGID_MSI_RCV_IND				,0,"D_SYS_MSGID_MSI_RCV_IND"				,func_ptif_msi_if    },
	{ D_SYS_MSGID_MSI_SND_IND				,0,"D_SYS_MSGID_MSI_SND_IND"				,func_ptif_msi_if    },
	{ D_SYS_MSGID_MPLANE_SW_INIT_COMP_NTC	,0,"D_SYS_MSGID_MPLANE_SW_INIT_COMP_NTC"	,func_ptif_dummy_if  },
	{ D_SYS_MSGID_ALLTSKININTC 				,0,"D_SYS_MSGID_ALLTSKININTC"				,func_ptif_dummy_if  },
	{ D_SYS_MSGID_INIT_COMP_NTC				,0,"D_SYS_MSGID_INIT_COMP_NTC"				,func_ptif_dummy_if  },
	{ D_SYS_MSGID_NMA_NETCONF_WRITE_REQ		,0,"D_SYS_MSGID_NMA_NETCONF_WRITE_REQ"		,func_ptif_dummy_if  },
	{ D_SYS_MSGID_NMA_NETCONF_WRITE_RSP		,0,"D_SYS_MSGID_NMA_NETCONF_WRITE_RSP"		,func_ptif_dummy_if  },
	{ D_SYS_MSGID_NMA_VLAN_WRITE_REQ		,0,"D_SYS_MSGID_NMA_VLAN_WRITE_REQ"			,func_ptif_dummy_if  },
	{ D_SYS_MSGID_NMA_VLAN_WRITE_RSP		,0,"D_SYS_MSGID_NMA_VLAN_WRITE_RSP"			,func_ptif_dummy_if  },
	{ D_SYS_MSGID_PTPSTSTO_NTC				,0,"D_SYS_MSGID_PTPSTSTO_NTC"				,func_ptif_dummy_if  },
	{ D_SYS_MSGID_SYNCESTSTO_NTC			,0,"D_SYS_MSGID_SYNCESTSTO_NTC"				,func_ptif_dummy_if  },
	{ D_SYS_MSGID_SYNCSTSTO_NTC				,0,"D_SYS_MSGID_SYNCSTSTO_NTC"				,func_ptif_dummy_if  },
	{ D_SYS_MSGID_EIO_EXTOUTSV_TO			,0,"D_SYS_MSGID_EIO_EXTOUTSV_TO"			,func_ptif_dummy_if  },
	{ D_SYS_MSGID_EIO_FANSVCTRL_TO			,0,"D_SYS_MSGID_EIO_FANSVCTRL_TO"			,func_ptif_dummy_if  },
	{ D_SYS_MSGID_TDDEN_CHG_NTC				,0,"D_SYS_MSGID_TDDEN_CHG_NTC"				,func_ptif_dummy_if  },
	{ D_SYS_MSGID_CARRIER_ON_SETTING_REQ	,0,"D_SYS_MSGID_CARRIER_ON_SETTING_REQ"		,func_ptif_dummy_if  },
	{ D_SYS_MSGID_CARRIER_OFF_SETTING_REQ	,0,"D_SYS_MSGID_CARRIER_OFF_SETTING_REQ"	,func_ptif_dummy_if  },
	{ D_SYS_MSGID_TXON_REQ					,0,"D_SYS_MSGID_TXON_REQ"					,func_ptif_dummy_if  },
	{ D_SYS_MSGID_TXON_CFM					,0,"D_SYS_MSGID_TXON_CFM"					,func_ptif_dummy_if  },
	{ D_SYS_MSGID_RXON_REQ					,0,"D_SYS_MSGID_RXON_REQ"					,func_ptif_dummy_if  },
	{ D_SYS_MSGID_RXON_CFM					,0,"D_SYS_MSGID_RXON_CFM"					,func_ptif_dummy_if  },
	{ D_SYS_MSGID_TXOFF_REQ					,0,"D_SYS_MSGID_TXOFF_REQ"					,func_ptif_dummy_if  },
	{ D_SYS_MSGID_TXOFF_CFM					,0,"D_SYS_MSGID_TXOFF_CFM"					,func_ptif_dummy_if  },
	{ D_SYS_MSGID_RXOFF_REQ					,0,"D_SYS_MSGID_RXOFF_REQ"					,func_ptif_dummy_if  },
	{ D_SYS_MSGID_RXOFF_CFM					,0,"D_SYS_MSGID_RXOFF_CFM"					,func_ptif_dummy_if  },
	{ D_SYS_MSGID_TXSLEEP_REQ				,0,"D_SYS_MSGID_TXSLEEP_REQ"				,func_ptif_dummy_if  },
	{ D_SYS_MSGID_RXSLEEP_REQ				,0,"D_SYS_MSGID_RXSLEEP_REQ"				,func_ptif_dummy_if  },
	{ D_SYS_MSGID_SYNCESSMTO_NTC			,9,"D_SYS_MSGID_SYNCESSMTO_NTC"				,func_ptif_dummy_if  },
	{ D_SYS_MSGID_BEAMID_SET_REQ			,0,"D_SYS_MSGID_BEAMID_SET_REQ"				,func_ptif_dummy_if  },
	{ 0x12345678							,0,"BPF_RM_SVRM_INITEND_IND"				,func_ptif_dummy_if  },
	{0,0,"Unknown",NULL}					/* END */
};

pcap_ptif_name rrupf_ptif_name_l2 [] = 
{
	{CMD_TSKIF_L1DATRCVNTC		,0,			"CMD_TSKIF_L1DATRCVNTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_L1DATTXREQ		,0,			"CMD_TSKIF_L1DATTXREQ"		,func_ptif_dummy_if},
	{CMD_TSKIF_L1DATCNF			,0,			"CMD_TSKIF_L1DATCNF"		,func_ptif_dummy_if},
	{CMD_TSKIF_L1MONDATSNDNTC	,0,			"CMD_TSKIF_L1MONDATSNDNTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_L1MONDATRCVNTC	,0,			"CMD_TSKIF_L1MONDATRCVNTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_CPRIRCVNTC		,0xffffffff,"CMD_TSKIF_CPRIRCVNTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_CPRISNDNTC		,0xffffffff,"CMD_TSKIF_CPRISNDNTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_TIMOUTNTC		,0,			"CMD_TSKIF_TIMOUTNTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_L2BSY			,0,			"CMD_TSKIF_L2BSY"			,func_ptif_dummy_if},
	{CMD_TSKIF_L2BSYCLR			,0,			"CMD_TSKIF_L2BSYCLR"		,func_ptif_dummy_if},
	{CMD_TSKIF_POLINTRNTC		,0,			"CMD_TSKIF_POLINTRNTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_CPRISTACHGNTC	,0,			"CMD_TSKIF_CPRISTACHGNTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_ESTCNF			,0,			"CMD_TSKIF_ESTCNF"			,func_ptif_dummy_if},
	{CMD_TSKIF_ESTIND			,0,			"CMD_TSKIF_ESTIND"			,func_ptif_dummy_if},
	{CMD_TSKIF_ESTINGIND		,0,			"CMD_TSKIF_ESTINGIND"		,func_ptif_dummy_if},
	{CMD_TSKIF_RELIND			,0,			"CMD_TSKIF_RELIND"			,func_ptif_dummy_if},
	{CMD_TSKIF_RELREQ			,0,			"CMD_TSKIF_RELREQ"			,func_ptif_dummy_if},
	{CMD_TSKIF_RELCNF			,0,			"CMD_TSKIF_RELCNF"			,func_ptif_dummy_if},
	{CMD_TSKIF_L2STANTC			,0,			"CMD_TSKIF_L2STANTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_L2DEACTREQ		,0,			"CMD_TSKIF_L2DEACTREQ"		,func_ptif_dummy_if},
	{CMD_TSKIF_ESTREQ			,0,			"CMD_TSKIF_ESTREQ"			,func_ptif_dummy_if},
	{CMD_TSKIF_L2STPREQ			,0,			"CMD_TSKIF_L2STPREQ"		,func_ptif_dummy_if},
	{CMD_TSKIF_L3MODESTRNTC		,0,			"CMD_TSKIF_L3MODESTRNTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_L3MODESTPNTC		,0,			"CMD_TSKIF_L3MODESTPNTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_L3MODESTPNTC		,0,			"CMD_TSKIF_L3MODESTPNTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_ALLTSKININTC		,0,			"CMD_TSKIF_ALLTSKININTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_INIREQ			,0,			"CMD_TSKIF_INIREQ"			,func_ptif_dummy_if},
	{CMD_TSKIF_INIRSP			,0,			"CMD_TSKIF_INIRSP"			,func_ptif_dummy_if},
	{ 0x12345678				,0,			"BPF_RM_SVRM_INITEND_IND"	,func_ptif_dummy_if},
	{ CMD_TSKIF_TXABNMLNTC		,0,			"CMD_TSKIF_TXABNMLNTC"		,func_ptif_dummy_if},
	{ D_SYS_MSGID_ALLTSKININTC 	,0,			"D_SYS_MSGID_ALLTSKININTC"	,func_ptif_dummy_if},
	{0							,0,			"Unknown"					,NULL			   }		/* END */
};

pcap_ptif_name rrupf_ptif_name_l3 [] = 
{
	{CMD_TSKIF_CPRIRCVNTC		,0xffffffff,"CMD_TSKIF_CPRIRCVNTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_CPRISNDNTC		,0xffffffff,"CMD_TSKIF_CPRISNDNTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_CPRIDSCNTC  		,0,			"CMD_TSKIF_CPRIDSCNTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_CPRISTANTC		,0,			"CMD_TSKIF_CPRISTANTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_CPRIACTNEGNTC	,0,			"CMD_TSKIF_CPRIACTNEGNTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_L3STANTC			,0,			"CMD_TSKIF_L3STANTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_L2STANTC			,0,			"CMD_TSKIF_L2STANTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_TRXRLS_NTC		,0,			"CMD_TSKIF_TRXRLS_NTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_ALLTSKININTC		,0,			"CMD_TSKIF_ALLTSKININTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_INIREQ			,0,			"CMD_TSKIF_INIREQ"			,func_ptif_dummy_if},
	{CMD_TSKIF_INIRSP			,0,			"CMD_TSKIF_INIRSP"			,func_ptif_dummy_if},
	{CMD_TSKIF_CPR_STA_NTC		,0,			"CMD_TSKIF_CPR_STA_NTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_CAS_RETIMESETREQ ,0,			"CMD_TSKIF_CAS_RETIMESETREQ",func_ptif_dummy_if},
	{CMD_TSKIF_RE_SET_REQ		,0,			"CMD_TSKIF_RE_SET_REQ"		,func_ptif_dummy_if},
	{CMD_TSKIF_RE_SET_RSP		,0,			"CMD_TSKIF_RE_SET_RSP"		,func_ptif_dummy_if},
	{ 0x12345678				,0,			"BPF_RM_SVRM_INITEND_IND"	,func_ptif_dummy_if},
	{ D_SYS_MSGID_ALLTSKININTC 	,0,			"D_SYS_MSGID_ALLTSKININTC"	,func_ptif_dummy_if},
	{0							,0,			"Unknown"					,NULL			   }		/* END */
};

pcap_ptif_name rrupf_ptif_name_mnt [] = 
{
	{CMD_TSKIF_CPRIRCVNTC		,0xffffffff,"CMD_TSKIF_CPRIRCVNTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_CPRISNDNTC		,0xffffffff,"CMD_TSKIF_CPRISNDNTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_TCPSRVINITREQ	,0		   ,"CMD_TSKIF_TCPSRVINITREQ"	,func_ptif_dummy_if},
	{CMD_TSKIF_TCPSRVINITRSP	,0		   ,"CMD_TSKIF_TCPSRVINITRSP"	,func_ptif_dummy_if},
	{CMD_TSKIF_CPR_STA_NTC		,0		   ,"CMD_TSKIF_CPR_STA_NTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_TRXRLS_NTC		,0		   ,"CMD_TSKIF_TRXRLS_NTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_CARDSTANTC		,0		   ,"CMD_TSKIF_CARDSTANTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_TCPSRVSTRREQ		,0		   ,"CMD_TSKIF_TCPSRVSTRREQ"	,func_ptif_dummy_if},
	{CMD_TSKIF_TCPSRVSTRRSP		,0		   ,"CMD_TSKIF_TCPSRVSTRRSP"	,func_ptif_dummy_if},
	{CMD_TSKIF_TCPSRVRSTIND		,0		   ,"CMD_TSKIF_TCPSRVRSTIND"	,func_ptif_dummy_if},
	{CMD_TSKIF_TCPSRVSTPNTC		,0		   ,"CMD_TSKIF_TCPSRVSTPNTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_ALLTSKININTC		,0		   ,"CMD_TSKIF_ALLTSKININTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_INIREQ			,0		   ,"CMD_TSKIF_INIREQ"			,func_ptif_dummy_if},
	{CMD_TSKIF_INIRSP			,0		   ,"CMD_TSKIF_INIRSP"			,func_ptif_dummy_if},
	{ 0x12345678				,0		   ,"BPF_RM_SVRM_INITEND_IND"	,func_ptif_dummy_if},
	{ D_SYS_MSGID_ALLTSKININTC 	,0		   ,"D_SYS_MSGID_ALLTSKININTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_L3STANTC			,0		   ,"CMD_TSKIF_L3STANTC"		,func_ptif_dummy_if},
	{0							,0		   ,"Unknown"					,NULL			   }	/* END */
};


pcap_ptif_name rrupf_ptif_name_pl [] = 
{
	{CMD_TSKIF_L1DATRCVNTC		,0,			"CMD_TSKIF_L1DATRCVNTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_L1DATTXREQ		,0,			"CMD_TSKIF_L1DATTXREQ"		,func_ptif_dummy_if},
	{CMD_TSKIF_L1DATCNF			,0,			"CMD_TSKIF_L1DATCNF"		,func_ptif_dummy_if},
	{CMD_TSKIF_L1MONDATSNDNTC	,0,			"CMD_TSKIF_L1MONDATSNDNTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_L1MONDATRCVNTC	,0,			"CMD_TSKIF_L1MONDATRCVNTC"	,func_ptif_dummy_if},	
	{CMD_TSKIF_CPRIRCVNTC		,0xffffffff,"CMD_TSKIF_CPRIRCVNTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_CPRISNDNTC		,0xffffffff,"CMD_TSKIF_CPRISNDNTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_TIMOUTNTC		,0,			"CMD_TSKIF_TIMOUTNTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_L2BSY			,0,			"CMD_TSKIF_L2BSY"			,func_ptif_dummy_if},
	{CMD_TSKIF_L2BSYCLR			,0,			"CMD_TSKIF_L2BSYCLR"		,func_ptif_dummy_if},
	{CMD_TSKIF_POLINTRNTC		,0,			"CMD_TSKIF_POLINTRNTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_CPRISTACHGNTC	,0,			"CMD_TSKIF_CPRISTACHGNTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_ESTCNF			,0,			"CMD_TSKIF_ESTCNF"			,func_ptif_dummy_if},
	{CMD_TSKIF_ESTIND			,0,			"CMD_TSKIF_ESTIND"			,func_ptif_dummy_if},
	{CMD_TSKIF_ESTINGIND		,0,			"CMD_TSKIF_ESTINGIND"		,func_ptif_dummy_if},
	{CMD_TSKIF_RELIND			,0,			"CMD_TSKIF_RELIND"			,func_ptif_dummy_if},
	{CMD_TSKIF_RELREQ			,0,			"CMD_TSKIF_RELREQ"			,func_ptif_dummy_if},
	{CMD_TSKIF_RELCNF			,0,			"CMD_TSKIF_RELCNF"			,func_ptif_dummy_if},
	{CMD_TSKIF_L2STANTC			,0,			"CMD_TSKIF_L2STANTC"		,func_ptif_dummy_if},
	{CMD_TSKIF_L2DEACTREQ		,0,			"CMD_TSKIF_L2DEACTREQ"		,func_ptif_dummy_if},
	{CMD_TSKIF_ESTREQ			,0,			"CMD_TSKIF_ESTREQ"			,func_ptif_dummy_if},
	{CMD_TSKIF_L2STPREQ			,0,			"CMD_TSKIF_L2STPREQ"		,func_ptif_dummy_if},
	{CMD_TSKIF_L3MODESTRNTC		,0,			"CMD_TSKIF_L3MODESTRNTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_L3MODESTPNTC		,0,			"CMD_TSKIF_L3MODESTPNTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_L3MODESTPNTC		,0,			"CMD_TSKIF_L3MODESTPNTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_ALLTSKININTC		,0,			"CMD_TSKIF_ALLTSKININTC"	,func_ptif_dummy_if},
	{CMD_TSKIF_INIREQ			,0,			"CMD_TSKIF_INIREQ"			,func_ptif_dummy_if},
	{CMD_TSKIF_INIRSP			,0,			"CMD_TSKIF_INIRSP"			,func_ptif_dummy_if},
	{ 0x12345678				,0,			"BPF_RM_SVRM_INITEND_IND"	,func_ptif_dummy_if},
	{ CMD_TSKIF_TXABNMLNTC		,0,			"CMD_TSKIF_TXABNMLNTC"		,func_ptif_dummy_if},
	{ D_SYS_MSGID_ALLTSKININTC 	,0,			"D_SYS_MSGID_ALLTSKININTC"	,func_ptif_dummy_if},
	{0							,0,			"Unknown"					,NULL				}	/* END */
};

pcap_ptif_name rrupf_ptif_name_oth_if [] = 
{
	{ D_SYS_MSGID_TIMOUTNTC		,0,			"D_SYS_MSGID_TIMOUTNTC"		,func_ptif_dummy_if  },
	{0							,0,			"Unknown"					,NULL				 }	/* END */
};

pcap_ptif_name rrupf_ptif_name_api [] = 
{
	{ D_API_MSGID_MPN_MPLANE_RECV_NTF		,0,"D_API_MSGID_MPN_MPLANE_RECV_NTF"	,func_ptif_dummy_if  },
	{ D_API_MSGID_MPN_MPLANE_SEND_REQ		,0,"D_API_MSGID_MPN_MPLANE_SEND_REQ"	,func_ptif_dummy_if  },
	{ D_API_MSGID_MPN_MPLANE_READY_REQ		,0,"D_API_MSGID_MPN_MPLANE_READY_REQ"	,func_ptif_dummy_if  },
	{ D_API_MSGID_MPN_MPLANE_READY_RSP		,0,"D_API_MSGID_MPN_MPLANE_READY_RSP"	,func_ptif_dummy_if  },
	{ D_API_MSGID_MPN_MPLANE_START_REQ		,0,"D_API_MSGID_MPN_MPLANE_START_REQ"	,func_ptif_dummy_if  },
	{ D_API_MSGID_MPN_MPLANE_START_RSP		,0,"D_API_MSGID_MPN_MPLANE_START_RSP"	,func_ptif_dummy_if  },
	{ D_API_MSGID_MPN_MPLANE_RECV_REQ		,0,"D_API_MSGID_MPN_MPLANE_RECV_REQ"	,func_ptif_dummy_if  },
	{ D_API_MSGID_PTP_START_REQ				,0,"D_API_MSGID_PTP_START_REQ"			,func_ptif_dummy_if  },
	{ D_API_MSGID_PTP_START_RSP				,0,"D_API_MSGID_PTP_START_RSP"			,func_ptif_dummy_if  },
	{ D_API_MSGID_PTP_SHUTDOWN_REQ			,0,"D_API_MSGID_PTP_SHUTDOWN_REQ"		,func_ptif_dummy_if  },
	{ D_API_MSGID_PTP_SHUTDOWN_RSP			,0,"D_API_MSGID_PTP_SHUTDOWN_RSP"		,func_ptif_dummy_if  },
	{ D_API_MSGID_PTP_TLOG_SET_REQ			,0,"D_API_MSGID_PTP_TLOG_SET_REQ"		,func_ptif_dummy_if  },
	{ D_API_MSGID_PTP_APRLOG_SET_REQ		,0,"D_API_MSGID_PTP_APRLOG_SET_REQ"		,func_ptif_dummy_if  },
	{ D_API_MSGID_PTP_TRAFFIC_GET_REQ		,0,"D_API_MSGID_PTP_TRAFFIC_GET_REQ"	,func_ptif_dummy_if  },
	{ D_API_MSGID_PTP_DEV_STATE_GET_REQ		,0,"D_API_MSGID_PTP_DEV_STATE_GET_REQ"	,func_ptif_dummy_if  },
	{ D_API_MSGID_PTP_CLOCK_GET_REQ			,0,"D_API_MSGID_PTP_CLOCK_GET_REQ"		,func_ptif_dummy_if  },
	{ D_API_MSGID_PTP_SYNC_INFO_REQ			,0,"D_API_MSGID_PTP_SYNC_INFO_REQ"		,func_ptif_dummy_if  },
	{ D_API_MSGID_PTP_SYNC_INFO_NTF			,0,"D_API_MSGID_PTP_SYNC_INFO_NTF"		,func_ptif_dummy_if  },
	{ D_API_RFP_MNT_POWER_ADJUST_REQ		,0,"D_API_RFP_MNT_POWER_ADJUST_REQ"		,func_ptif_dummy_if  },
	{0										,0,"Unknown"							,NULL			     }	/* END */
};

pcap_proc_name proc_name_if [] = 
{
	{"in" ,rrupf_thd_name_init   , rrupf_ptif_name_init    },	/* 0 */
	{"pf" ,rrupf_thd_name_pf     , rrupf_ptif_name_pf      },	/* 1 */
	{"l2" ,rrupf_thd_name_l2     , rrupf_ptif_name_l2      },	/* 2 */
	{"pt" ,rrupf_thd_name_ptp    , rrupf_ptif_name_pf      },	/* 3 */
	{"mt" ,rrupf_thd_name_mnt    , rrupf_ptif_name_mnt     },	/* 4 */
	{"??" ,rrupf_thd_name_oth_if , rrupf_ptif_name_oth_if  },	/* 5 */
	{"pl" ,rrupf_thd_name_pl     , rrupf_ptif_name_pl      },	/* 6 */
	{"??" ,rrupf_thd_name_oth_if , rrupf_ptif_name_oth_if  }
};

void func_ptif_header_if(FILE* fp , char* data_p ,int datalen , char* name)
{
	fprintf(fp,"qid_s(%X_%X)->d(%X_%X);evt-%X;sig-%X;key-%X;%s;%ubytes",
			((pcap_rrupf_userheader_if* )data_p)->sPrcQid,
			((pcap_rrupf_userheader_if* )data_p)->sThrdQid,
			((pcap_rrupf_userheader_if* )data_p)->dPrcQid,
			((pcap_rrupf_userheader_if* )data_p)->dThrdQid,
			((pcap_rrupf_userheader_if* )data_p)->eventNo,
			((pcap_rrupf_userheader_if* )data_p)->sigKind,
			((pcap_rrupf_userheader_if* )data_p)->shmKeyid,
			name,
			((pcap_rrupf_userheader_if* )data_p)->len
	);
	return;
}

/* process name get */
char* func_com_prc_name_if(unsigned int prcindex)
{
	if(prcindex >= (sizeof(proc_name_if)/sizeof(proc_name_if[0])))
	{
		prcindex = (sizeof(proc_name_if)/sizeof(proc_name_if[0])) - 1;
	}
	return(proc_name_if[prcindex].name);
}



/* thread name get */
char* func_com_thrd_name_if(unsigned int prcindex ,unsigned int thrdindex )
{
	if(prcindex >= (sizeof(proc_name_if)/sizeof(proc_name_if[0])))
	{
		prcindex = (sizeof(proc_name_if)/sizeof(proc_name_if[0])) - 1;
	}
	switch ( prcindex )
	{
		case 0:
			thrdindex -= 0x10;
			if(thrdindex >= (sizeof(rrupf_thd_name_init)/sizeof(rrupf_thd_name_init[0])))
			{
				thrdindex = (sizeof(rrupf_thd_name_init)/sizeof(rrupf_thd_name_init[0])) - 1;
			}
			break;
		case 1:
			thrdindex -= 0x20;
			if(thrdindex >= (sizeof(rrupf_thd_name_pf)/sizeof(rrupf_thd_name_pf[0])))
			{
				thrdindex = (sizeof(rrupf_thd_name_pf)/sizeof(rrupf_thd_name_pf[0])) - 1;
			}
			break;
		case 3:
			thrdindex -= 0x20;
			if(thrdindex >= (sizeof(rrupf_thd_name_ptp)/sizeof(rrupf_thd_name_ptp[0])))
			{
				thrdindex = (sizeof(rrupf_thd_name_ptp)/sizeof(rrupf_thd_name_ptp[0])) - 1;
			}
			break;
		case 2:
			thrdindex -= 0x40;
			if(thrdindex >= (sizeof(rrupf_thd_name_l2)/sizeof(rrupf_thd_name_l2[0])))
			{
				thrdindex = (sizeof(rrupf_thd_name_l2)/sizeof(rrupf_thd_name_l2[0])) - 1;
			}
			break;
		case 4:
			thrdindex -= 0x60;
			if(thrdindex >= (sizeof(rrupf_thd_name_mnt)/sizeof(rrupf_thd_name_mnt[0])))
			{
				thrdindex = (sizeof(rrupf_thd_name_mnt)/sizeof(rrupf_thd_name_mnt[0])) - 1;
			}
			break;
		case 6:
			thrdindex -= 0x80;
			if(thrdindex >= (sizeof(rrupf_thd_name_pl)/sizeof(rrupf_thd_name_pl[0])))
			{
				thrdindex = (sizeof(rrupf_thd_name_pl)/sizeof(rrupf_thd_name_pl[0])) - 1;
			}
			break;
		default:
			thrdindex = 0;
			break;
	}
	return(proc_name_if[prcindex].thrd[thrdindex].name);
}

/* thread name get */
unsigned int func_com_thrd_index_if(unsigned int prcindex ,unsigned int thrdno )
{
	unsigned int thrdindex = thrdno;
	if(prcindex >= (sizeof(proc_name_if)/sizeof(proc_name_if[0])))
	{
		prcindex = (sizeof(proc_name_if)/sizeof(proc_name_if[0])) - 1;
	}
	switch ( prcindex )
	{
		case 0:
			thrdindex -= 0x10;
			if(thrdindex >= (sizeof(rrupf_thd_name_init)/sizeof(rrupf_thd_name_init[0])))
			{
				thrdindex = (sizeof(rrupf_thd_name_init)/sizeof(rrupf_thd_name_init[0])) - 1;
			}
			break;
		case 1:
			thrdindex -= 0x20;
			if(thrdindex >= (sizeof(rrupf_thd_name_pf)/sizeof(rrupf_thd_name_pf[0])))
			{
				thrdindex = (sizeof(rrupf_thd_name_pf)/sizeof(rrupf_thd_name_pf[0])) - 1;
			}
			break;
		case 3:
			thrdindex -= 0x20;
			if(thrdindex >= (sizeof(rrupf_thd_name_ptp)/sizeof(rrupf_thd_name_ptp[0])))
			{
				thrdindex = (sizeof(rrupf_thd_name_ptp)/sizeof(rrupf_thd_name_ptp[0])) - 1;
			}
			break;
		case 2:
			thrdindex -= 0x40;
			if(thrdindex >= (sizeof(rrupf_thd_name_l2)/sizeof(rrupf_thd_name_l2[0])))
			{
				thrdindex = (sizeof(rrupf_thd_name_l2)/sizeof(rrupf_thd_name_l2[0])) - 1;
			}
			break;
		case 4:
			thrdindex -= 0x60;
			if(thrdindex >= (sizeof(rrupf_thd_name_mnt)/sizeof(rrupf_thd_name_mnt[0])))
			{
				thrdindex = (sizeof(rrupf_thd_name_mnt)/sizeof(rrupf_thd_name_mnt[0])) - 1;
			}
			break;
		case 6:
			thrdindex -= 0x80;
			if(thrdindex >= (sizeof(rrupf_thd_name_pl)/sizeof(rrupf_thd_name_pl[0])))
			{
				thrdindex = (sizeof(rrupf_thd_name_pl)/sizeof(rrupf_thd_name_pl[0])) - 1;
			}
			break;
		default:
			thrdindex = 0;
			break;
	}
	return(thrdindex);
}

/* ユーザデータ表示関数 */
int func_userdata_dsp_if(
	FILE* fp,
	pcap_rrupf_userheader_if* user_p,
	int len,
	unsigned int procindex)
{
	unsigned int count;
	unsigned int count2;
	int rtn = D_RRUPKTDUMP_TYPE_UNKNOWN;
	
	if(len < sizeof(*user_p))
	{
		return (rtn);
	}

	func_gmsg("sQ %08X,%08X => dQ %08X,%08X Keys:%08X;",
		user_p->sPrcQid,
		user_p->sThrdQid,
		user_p->dPrcQid,
		user_p->dThrdQid,
		user_p->shmKeyid
	);

	for(count=0 ; ; count++)
	{
		/* API */
		if( (user_p->eventNo >= D_API_MSGID_CPRI_BASE) && (user_p->eventNo <= 0xAFFFFFFF) )
		{
			for(count2 = 0 ; count2 < sizeof(rrupf_ptif_name_api)/sizeof(rrupf_ptif_name_api[0]) ; count2++)
			{
				if(rrupf_ptif_name_api[count2].eventNo == user_p->eventNo)
				{
					func_ptif_header_if(fp,(char*)user_p,len,rrupf_ptif_name_api[count2].signame);
					rrupf_ptif_name_api[count2].func(fp,(char*)user_p,len,0);
					rtn = D_RRUPKTDUMP_TYPE_INTERNL;
					break;
				}
			}
			if( count2 >= sizeof(rrupf_ptif_name_api)/sizeof(rrupf_ptif_name_api[0]) )
			{
				func_ptif_header_if(fp,(char*)user_p,len,rrupf_ptif_name_api[ (sizeof(rrupf_ptif_name_api)/sizeof(rrupf_ptif_name_api[0])) - 1].signame);
			}
			break;
		}
		if(proc_name_if[procindex].ptif[count].func == NULL)
		{
			func_ptif_header_if(fp,(char*)user_p,len,proc_name_if[procindex].ptif[count].signame);
			break;
		}
		if( proc_name_if[procindex].ptif[count].eventNo == user_p->eventNo )
		{
			if (proc_name_if[procindex].ptif[count].sigKind != user_p->sigKind)
			{
				if(proc_name_if[procindex].ptif[count].sigKind != 0xFFFFFFFF )
				{
					continue;
				}
			}
			func_ptif_header_if(fp,(char*)user_p,len,proc_name_if[procindex].ptif[count].signame);
			(void)proc_name_if[procindex].ptif[count].func(fp,(char*)user_p,len,0);
			rtn = D_RRUPKTDUMP_TYPE_INTERNL;
			break;
		}
	}
	return(rtn);
}

/* IPヘッダ表示関数 */
unsigned int func_iphead_dsp_if(
	FILE* fp ,
	pcap_data_timeoffs_t* time_p,
	pcap_rrupf_ipheader* data
)
{
	unsigned int s_prcindex;
	unsigned int s_thrdindex;
	unsigned int d_prcindex;
	unsigned int d_thrdindex;
	unsigned int srcip;
	unsigned int dstip;
	struct tm lctimep;
	time_t local_sec;

	srcip = ntohl(data->h_ip.ip_src.s_addr);
	dstip = ntohl(data->h_ip.ip_dst.s_addr);

	s_prcindex = (unsigned int)((srcip & 0x0000FF00) >> 8);
	s_thrdindex = (unsigned int)(srcip & 0x000000FF);
	d_prcindex = (unsigned int)((dstip & 0x0000FF00) >> 8);
	d_thrdindex = (unsigned int)(dstip & 0x000000FF);
	
	if(time_p != NULL)
	{
		memset(&lctimep,0,sizeof(lctimep));
		local_sec = (time_t)time_p->sec;
		localtime_r((const time_t *)&local_sec,&lctimep);
		fprintf(fp,"%04d/%02d/%02d %02d:%02d:%02d.%06ld;",
			lctimep.tm_year + 1900,
			lctimep.tm_mon+1,
			lctimep.tm_mday,
			lctimep.tm_hour,
			lctimep.tm_min,
			lctimep.tm_sec,
			time_p->usec
		);
	}
	fprintf(fp,"%s_%s->%s_%s;",
		func_com_prc_name_if(s_prcindex),
		func_com_thrd_name_if(s_prcindex ,s_thrdindex),
		func_com_prc_name_if(d_prcindex),
		func_com_thrd_name_if(d_prcindex ,d_thrdindex)
	);
	
	func_gmsg("(%08x => %08x %ubytes);",srcip,dstip,ntohs(data->h_ip.ip_len));
	if(d_prcindex >= (sizeof(proc_name_if)/sizeof(proc_name_if[0])))
	{
		d_prcindex = (sizeof(proc_name_if)/sizeof(proc_name_if[0])) - 1;
	}
	return(d_prcindex);
}

