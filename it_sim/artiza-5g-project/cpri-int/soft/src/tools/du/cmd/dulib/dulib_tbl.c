/*!
 *  @skip	$Id:$
 *  @file	dulib_tbl.c
 *  @brief	du command library routine
 *  @author	FJT)Taniguchi
 *  @date	2018/08/29 FJT)Taniguchi create
 *  @date	2018/12/12 KCN)Wang modify
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

#include <signal.h>
#include "dulib_local.h"

T_RRH_DU_CMD_INFO* dulib_cmdshm_addr = NULL;


static du_t_members dulib_member_fh_port[2][6] = {
	{{ "link_onoff",sizeof(UINT), NULL,NULL,0},{ "remote_mac_addr_h",sizeof(UINT), NULL,NULL,0},{ "remote_mac_addr_l",sizeof(UINT),NULL,NULL,0},{ "vid_dl",sizeof(UINT),NULL,NULL,0},{ "vid_ul",sizeof(UINT),NULL,NULL,0},{ "pcp_ul",sizeof(UINT),NULL,NULL,0}},
	{{ "link_onoff",sizeof(UINT), NULL,NULL,0},{ "remote_mac_addr_h",sizeof(UINT), NULL,NULL,0},{ "remote_mac_addr_l",sizeof(UINT),NULL,NULL,0},{ "vid_dl",sizeof(UINT),NULL,NULL,0},{ "vid_ul",sizeof(UINT),NULL,NULL,0},{ "pcp_ul",sizeof(UINT),NULL,NULL,0}}
};

static du_t_members dulib_member_txendp_cc0[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};

static du_t_members dulib_member_txendp_cc1[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};

static du_t_members dulib_member_txendp_cc2[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};

static du_t_members dulib_member_txendp_cc3[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};


static du_t_members dulib_member_txendp_cc4[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};

static du_t_members dulib_member_txendp_cc5[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};

static du_t_members dulib_member_txendp_cc6[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};

static du_t_members dulib_member_txendp_cc7[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};


static du_t_members dulib_member_txarraycar_cc0[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};

static du_t_members dulib_member_txarraycar_cc1[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};


static du_t_members dulib_member_txarraycar_cc2[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};

static du_t_members dulib_member_txarraycar_cc3[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};

static du_t_members dulib_member_txarraycar_cc4[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};

static du_t_members dulib_member_txarraycar_cc5[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};

static du_t_members dulib_member_txarraycar_cc6[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};

static du_t_members dulib_member_txarraycar_cc7[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};



static du_t_members dulib_member_rxendp_cc0[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};

static du_t_members dulib_member_rxendp_cc1[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};

static du_t_members dulib_member_rxendp_cc2[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};

static du_t_members dulib_member_rxendp_cc3[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};

static du_t_members dulib_member_rxendp_cc4[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};

static du_t_members dulib_member_rxendp_cc5[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};

static du_t_members dulib_member_rxendp_cc6[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};

static du_t_members dulib_member_rxendp_cc7[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}},
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "Acell_id",sizeof(UINT),NULL,NULL,0},{ "CC_id",sizeof(UINT),NULL,NULL,0},{ "Port_id",sizeof(UINT),NULL,NULL,0},{ "Acell_id_ex",sizeof(UINT),NULL,NULL,0},{ "CC_id_ex",sizeof(UINT),NULL,NULL,0},{ "Port_id_ex",sizeof(UINT),NULL,NULL,0}}
};


static du_t_members dulib_member_rxarraycar_cc0[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};

static du_t_members dulib_member_rxarraycar_cc1[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};


static du_t_members dulib_member_rxarraycar_cc2[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};

static du_t_members dulib_member_rxarraycar_cc3[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};                                                                                                                                                                 

static du_t_members dulib_member_rxarraycar_cc4[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};

static du_t_members dulib_member_rxarraycar_cc5[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};

static du_t_members dulib_member_rxarraycar_cc6[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};

static du_t_members dulib_member_rxarraycar_cc7[8][7] = {
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} },
	{{ "onoff",sizeof(UINT),NULL,NULL,0},{ "test_onoff",sizeof(UINT),NULL, NULL,0},{ "nr_freq",sizeof(UINT),NULL,NULL,0},{ "nr_bandwidth",sizeof(UINT),NULL,NULL,0},{ "ul_fft_smpl_ofs",sizeof(UINT),NULL,NULL,0},{ "ta_offset",sizeof(UINT),NULL,NULL,0},{ "nr_scs",sizeof(UINT),NULL,NULL,0} }
};


static du_t_members dulib_member_txcc[8][16] = {
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc0[0]     , sizeof(dulib_member_txendp_cc0[0])/sizeof(dulib_member_txendp_cc0[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc0[1]     , sizeof(dulib_member_txendp_cc0[0])/sizeof(dulib_member_txendp_cc0[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc0[2]     , sizeof(dulib_member_txendp_cc0[0])/sizeof(dulib_member_txendp_cc0[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc0[3]     , sizeof(dulib_member_txendp_cc0[0])/sizeof(dulib_member_txendp_cc0[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc0[4]     , sizeof(dulib_member_txendp_cc0[0])/sizeof(dulib_member_txendp_cc0[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc0[5]     , sizeof(dulib_member_txendp_cc0[0])/sizeof(dulib_member_txendp_cc0[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc0[6]     , sizeof(dulib_member_txendp_cc0[0])/sizeof(dulib_member_txendp_cc0[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc0[7]     , sizeof(dulib_member_txendp_cc0[0])/sizeof(dulib_member_txendp_cc0[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc0[0] , sizeof(dulib_member_txarraycar_cc0[0])/sizeof(dulib_member_txarraycar_cc0[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc0[1] , sizeof(dulib_member_txarraycar_cc0[1])/sizeof(dulib_member_txarraycar_cc0[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc0[2] , sizeof(dulib_member_txarraycar_cc0[2])/sizeof(dulib_member_txarraycar_cc0[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc0[3] , sizeof(dulib_member_txarraycar_cc0[3])/sizeof(dulib_member_txarraycar_cc0[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc0[4] , sizeof(dulib_member_txarraycar_cc0[4])/sizeof(dulib_member_txarraycar_cc0[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc0[5] , sizeof(dulib_member_txarraycar_cc0[5])/sizeof(dulib_member_txarraycar_cc0[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc0[6] , sizeof(dulib_member_txarraycar_cc0[6])/sizeof(dulib_member_txarraycar_cc0[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc0[7] , sizeof(dulib_member_txarraycar_cc0[7])/sizeof(dulib_member_txarraycar_cc0[7][0])  },
	},
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc1[0]     , sizeof(dulib_member_txendp_cc1[0])/sizeof(dulib_member_txendp_cc1[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc1[1]     , sizeof(dulib_member_txendp_cc1[0])/sizeof(dulib_member_txendp_cc1[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc1[2]     , sizeof(dulib_member_txendp_cc1[0])/sizeof(dulib_member_txendp_cc1[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc1[3]     , sizeof(dulib_member_txendp_cc1[0])/sizeof(dulib_member_txendp_cc1[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc1[4]     , sizeof(dulib_member_txendp_cc1[0])/sizeof(dulib_member_txendp_cc1[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc1[5]     , sizeof(dulib_member_txendp_cc1[0])/sizeof(dulib_member_txendp_cc1[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc1[6]     , sizeof(dulib_member_txendp_cc1[0])/sizeof(dulib_member_txendp_cc1[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc1[7]     , sizeof(dulib_member_txendp_cc1[0])/sizeof(dulib_member_txendp_cc1[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc1[0] , sizeof(dulib_member_txarraycar_cc1[0])/sizeof(dulib_member_txarraycar_cc1[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc1[1] , sizeof(dulib_member_txarraycar_cc1[1])/sizeof(dulib_member_txarraycar_cc1[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc1[2] , sizeof(dulib_member_txarraycar_cc1[2])/sizeof(dulib_member_txarraycar_cc1[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc1[3] , sizeof(dulib_member_txarraycar_cc1[3])/sizeof(dulib_member_txarraycar_cc1[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc1[4] , sizeof(dulib_member_txarraycar_cc1[4])/sizeof(dulib_member_txarraycar_cc1[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc1[5] , sizeof(dulib_member_txarraycar_cc1[5])/sizeof(dulib_member_txarraycar_cc1[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc1[6] , sizeof(dulib_member_txarraycar_cc1[6])/sizeof(dulib_member_txarraycar_cc1[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc1[7] , sizeof(dulib_member_txarraycar_cc1[7])/sizeof(dulib_member_txarraycar_cc1[7][0])  },
	},
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc2[0]     , sizeof(dulib_member_txendp_cc2[0])/sizeof(dulib_member_txendp_cc2[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc2[1]     , sizeof(dulib_member_txendp_cc2[0])/sizeof(dulib_member_txendp_cc2[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc2[2]     , sizeof(dulib_member_txendp_cc2[0])/sizeof(dulib_member_txendp_cc2[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc2[3]     , sizeof(dulib_member_txendp_cc2[0])/sizeof(dulib_member_txendp_cc2[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc2[4]     , sizeof(dulib_member_txendp_cc2[0])/sizeof(dulib_member_txendp_cc2[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc2[5]     , sizeof(dulib_member_txendp_cc2[0])/sizeof(dulib_member_txendp_cc2[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc2[6]     , sizeof(dulib_member_txendp_cc2[0])/sizeof(dulib_member_txendp_cc2[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc2[7]     , sizeof(dulib_member_txendp_cc2[0])/sizeof(dulib_member_txendp_cc2[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc2[0] , sizeof(dulib_member_txarraycar_cc2[0])/sizeof(dulib_member_txarraycar_cc2[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc2[1] , sizeof(dulib_member_txarraycar_cc2[1])/sizeof(dulib_member_txarraycar_cc2[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc2[2] , sizeof(dulib_member_txarraycar_cc2[2])/sizeof(dulib_member_txarraycar_cc2[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc2[3] , sizeof(dulib_member_txarraycar_cc2[3])/sizeof(dulib_member_txarraycar_cc2[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc2[4] , sizeof(dulib_member_txarraycar_cc2[4])/sizeof(dulib_member_txarraycar_cc2[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc2[5] , sizeof(dulib_member_txarraycar_cc2[5])/sizeof(dulib_member_txarraycar_cc2[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc2[6] , sizeof(dulib_member_txarraycar_cc2[6])/sizeof(dulib_member_txarraycar_cc2[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc2[7] , sizeof(dulib_member_txarraycar_cc2[7])/sizeof(dulib_member_txarraycar_cc2[7][0])  },
	},
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc3[0]     , sizeof(dulib_member_txendp_cc3[0])/sizeof(dulib_member_txendp_cc3[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc3[1]     , sizeof(dulib_member_txendp_cc3[0])/sizeof(dulib_member_txendp_cc3[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc3[2]     , sizeof(dulib_member_txendp_cc3[0])/sizeof(dulib_member_txendp_cc3[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc3[3]     , sizeof(dulib_member_txendp_cc3[0])/sizeof(dulib_member_txendp_cc3[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc3[4]     , sizeof(dulib_member_txendp_cc3[0])/sizeof(dulib_member_txendp_cc3[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc3[5]     , sizeof(dulib_member_txendp_cc3[0])/sizeof(dulib_member_txendp_cc3[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc3[6]     , sizeof(dulib_member_txendp_cc3[0])/sizeof(dulib_member_txendp_cc3[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc3[7]     , sizeof(dulib_member_txendp_cc3[0])/sizeof(dulib_member_txendp_cc3[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc3[0] , sizeof(dulib_member_txarraycar_cc3[0])/sizeof(dulib_member_txarraycar_cc3[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc3[1] , sizeof(dulib_member_txarraycar_cc3[1])/sizeof(dulib_member_txarraycar_cc3[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc3[2] , sizeof(dulib_member_txarraycar_cc3[2])/sizeof(dulib_member_txarraycar_cc3[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc3[3] , sizeof(dulib_member_txarraycar_cc3[3])/sizeof(dulib_member_txarraycar_cc3[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc3[4] , sizeof(dulib_member_txarraycar_cc3[4])/sizeof(dulib_member_txarraycar_cc3[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc3[5] , sizeof(dulib_member_txarraycar_cc3[5])/sizeof(dulib_member_txarraycar_cc3[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc3[6] , sizeof(dulib_member_txarraycar_cc3[6])/sizeof(dulib_member_txarraycar_cc3[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc3[7] , sizeof(dulib_member_txarraycar_cc3[7])/sizeof(dulib_member_txarraycar_cc3[7][0])  },
	},
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc4[0]     , sizeof(dulib_member_txendp_cc4[0])/sizeof(dulib_member_txendp_cc4[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc4[1]     , sizeof(dulib_member_txendp_cc4[0])/sizeof(dulib_member_txendp_cc4[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc4[2]     , sizeof(dulib_member_txendp_cc4[0])/sizeof(dulib_member_txendp_cc4[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc4[3]     , sizeof(dulib_member_txendp_cc4[0])/sizeof(dulib_member_txendp_cc4[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc4[4]     , sizeof(dulib_member_txendp_cc4[0])/sizeof(dulib_member_txendp_cc4[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc4[5]     , sizeof(dulib_member_txendp_cc4[0])/sizeof(dulib_member_txendp_cc4[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc4[6]     , sizeof(dulib_member_txendp_cc4[0])/sizeof(dulib_member_txendp_cc4[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc4[7]     , sizeof(dulib_member_txendp_cc4[0])/sizeof(dulib_member_txendp_cc4[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc4[0] , sizeof(dulib_member_txarraycar_cc4[0])/sizeof(dulib_member_txarraycar_cc4[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc4[1] , sizeof(dulib_member_txarraycar_cc4[1])/sizeof(dulib_member_txarraycar_cc4[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc4[2] , sizeof(dulib_member_txarraycar_cc4[2])/sizeof(dulib_member_txarraycar_cc4[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc4[3] , sizeof(dulib_member_txarraycar_cc4[3])/sizeof(dulib_member_txarraycar_cc4[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc4[4] , sizeof(dulib_member_txarraycar_cc4[4])/sizeof(dulib_member_txarraycar_cc4[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc4[5] , sizeof(dulib_member_txarraycar_cc4[5])/sizeof(dulib_member_txarraycar_cc4[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc4[6] , sizeof(dulib_member_txarraycar_cc4[6])/sizeof(dulib_member_txarraycar_cc4[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc4[7] , sizeof(dulib_member_txarraycar_cc4[7])/sizeof(dulib_member_txarraycar_cc4[7][0])  },
	},
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc5[0]     , sizeof(dulib_member_txendp_cc5[0])/sizeof(dulib_member_txendp_cc5[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc5[1]     , sizeof(dulib_member_txendp_cc5[0])/sizeof(dulib_member_txendp_cc5[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc5[2]     , sizeof(dulib_member_txendp_cc5[0])/sizeof(dulib_member_txendp_cc5[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc5[3]     , sizeof(dulib_member_txendp_cc5[0])/sizeof(dulib_member_txendp_cc5[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc5[4]     , sizeof(dulib_member_txendp_cc5[0])/sizeof(dulib_member_txendp_cc5[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc5[5]     , sizeof(dulib_member_txendp_cc5[0])/sizeof(dulib_member_txendp_cc5[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc5[6]     , sizeof(dulib_member_txendp_cc5[0])/sizeof(dulib_member_txendp_cc5[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc5[7]     , sizeof(dulib_member_txendp_cc5[0])/sizeof(dulib_member_txendp_cc5[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc5[0] , sizeof(dulib_member_txarraycar_cc5[0])/sizeof(dulib_member_txarraycar_cc5[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc5[1] , sizeof(dulib_member_txarraycar_cc5[1])/sizeof(dulib_member_txarraycar_cc5[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc5[2] , sizeof(dulib_member_txarraycar_cc5[2])/sizeof(dulib_member_txarraycar_cc5[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc5[3] , sizeof(dulib_member_txarraycar_cc5[3])/sizeof(dulib_member_txarraycar_cc5[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc5[4] , sizeof(dulib_member_txarraycar_cc5[4])/sizeof(dulib_member_txarraycar_cc5[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc5[5] , sizeof(dulib_member_txarraycar_cc5[5])/sizeof(dulib_member_txarraycar_cc5[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc5[6] , sizeof(dulib_member_txarraycar_cc5[6])/sizeof(dulib_member_txarraycar_cc5[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc5[7] , sizeof(dulib_member_txarraycar_cc5[7])/sizeof(dulib_member_txarraycar_cc5[7][0])  },
	},
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc6[0]     , sizeof(dulib_member_txendp_cc6[0])/sizeof(dulib_member_txendp_cc6[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc6[1]     , sizeof(dulib_member_txendp_cc6[0])/sizeof(dulib_member_txendp_cc6[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc6[2]     , sizeof(dulib_member_txendp_cc6[0])/sizeof(dulib_member_txendp_cc6[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc6[3]     , sizeof(dulib_member_txendp_cc6[0])/sizeof(dulib_member_txendp_cc6[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc6[4]     , sizeof(dulib_member_txendp_cc6[0])/sizeof(dulib_member_txendp_cc6[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc6[5]     , sizeof(dulib_member_txendp_cc6[0])/sizeof(dulib_member_txendp_cc6[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc6[6]     , sizeof(dulib_member_txendp_cc6[0])/sizeof(dulib_member_txendp_cc6[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc6[7]     , sizeof(dulib_member_txendp_cc6[0])/sizeof(dulib_member_txendp_cc6[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc6[0] , sizeof(dulib_member_txarraycar_cc6[0])/sizeof(dulib_member_txarraycar_cc6[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc6[1] , sizeof(dulib_member_txarraycar_cc6[1])/sizeof(dulib_member_txarraycar_cc6[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc6[2] , sizeof(dulib_member_txarraycar_cc6[2])/sizeof(dulib_member_txarraycar_cc6[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc6[3] , sizeof(dulib_member_txarraycar_cc6[3])/sizeof(dulib_member_txarraycar_cc6[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc6[4] , sizeof(dulib_member_txarraycar_cc6[4])/sizeof(dulib_member_txarraycar_cc6[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc6[5] , sizeof(dulib_member_txarraycar_cc6[5])/sizeof(dulib_member_txarraycar_cc6[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc6[6] , sizeof(dulib_member_txarraycar_cc6[6])/sizeof(dulib_member_txarraycar_cc6[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc6[7] , sizeof(dulib_member_txarraycar_cc6[7])/sizeof(dulib_member_txarraycar_cc6[7][0])  },
	},
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc7[0]     , sizeof(dulib_member_txendp_cc7[0])/sizeof(dulib_member_txendp_cc7[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc7[1]     , sizeof(dulib_member_txendp_cc7[0])/sizeof(dulib_member_txendp_cc7[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc7[2]     , sizeof(dulib_member_txendp_cc7[0])/sizeof(dulib_member_txendp_cc7[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc7[3]     , sizeof(dulib_member_txendp_cc7[0])/sizeof(dulib_member_txendp_cc7[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc7[4]     , sizeof(dulib_member_txendp_cc7[0])/sizeof(dulib_member_txendp_cc7[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc7[5]     , sizeof(dulib_member_txendp_cc7[0])/sizeof(dulib_member_txendp_cc7[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc7[6]     , sizeof(dulib_member_txendp_cc7[0])/sizeof(dulib_member_txendp_cc7[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_txendp_cc7[7]     , sizeof(dulib_member_txendp_cc7[0])/sizeof(dulib_member_txendp_cc7[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc7[0] , sizeof(dulib_member_txarraycar_cc7[0])/sizeof(dulib_member_txarraycar_cc7[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc7[1] , sizeof(dulib_member_txarraycar_cc7[1])/sizeof(dulib_member_txarraycar_cc7[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc7[2] , sizeof(dulib_member_txarraycar_cc7[2])/sizeof(dulib_member_txarraycar_cc7[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc7[3] , sizeof(dulib_member_txarraycar_cc7[3])/sizeof(dulib_member_txarraycar_cc7[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc7[4] , sizeof(dulib_member_txarraycar_cc7[4])/sizeof(dulib_member_txarraycar_cc7[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc7[5] , sizeof(dulib_member_txarraycar_cc7[5])/sizeof(dulib_member_txarraycar_cc7[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc7[6] , sizeof(dulib_member_txarraycar_cc7[6])/sizeof(dulib_member_txarraycar_cc7[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_txarraycar_cc7[7] , sizeof(dulib_member_txarraycar_cc7[7])/sizeof(dulib_member_txarraycar_cc7[7][0])  },
	}
};

static du_t_members dulib_member_rxcc[8][16] = {
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc0[0]     , sizeof(dulib_member_rxendp_cc0[0])/sizeof(dulib_member_rxendp_cc0[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc0[1]     , sizeof(dulib_member_rxendp_cc0[0])/sizeof(dulib_member_rxendp_cc0[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc0[2]     , sizeof(dulib_member_rxendp_cc0[0])/sizeof(dulib_member_rxendp_cc0[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc0[3]     , sizeof(dulib_member_rxendp_cc0[0])/sizeof(dulib_member_rxendp_cc0[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc0[4]     , sizeof(dulib_member_rxendp_cc0[0])/sizeof(dulib_member_rxendp_cc0[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc0[5]     , sizeof(dulib_member_rxendp_cc0[0])/sizeof(dulib_member_rxendp_cc0[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc0[6]     , sizeof(dulib_member_rxendp_cc0[0])/sizeof(dulib_member_rxendp_cc0[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc0[7]     , sizeof(dulib_member_rxendp_cc0[0])/sizeof(dulib_member_rxendp_cc0[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc0[0] , sizeof(dulib_member_rxarraycar_cc0[0])/sizeof(dulib_member_rxarraycar_cc0[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc0[1] , sizeof(dulib_member_rxarraycar_cc0[1])/sizeof(dulib_member_rxarraycar_cc0[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc0[2] , sizeof(dulib_member_rxarraycar_cc0[2])/sizeof(dulib_member_rxarraycar_cc0[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc0[3] , sizeof(dulib_member_rxarraycar_cc0[3])/sizeof(dulib_member_rxarraycar_cc0[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc0[4] , sizeof(dulib_member_rxarraycar_cc0[4])/sizeof(dulib_member_rxarraycar_cc0[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc0[5] , sizeof(dulib_member_rxarraycar_cc0[5])/sizeof(dulib_member_rxarraycar_cc0[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc0[6] , sizeof(dulib_member_rxarraycar_cc0[6])/sizeof(dulib_member_rxarraycar_cc0[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc0[7] , sizeof(dulib_member_rxarraycar_cc0[7])/sizeof(dulib_member_rxarraycar_cc0[7][0])  },
	},
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc1[0]     , sizeof(dulib_member_rxendp_cc1[0])/sizeof(dulib_member_rxendp_cc1[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc1[1]     , sizeof(dulib_member_rxendp_cc1[0])/sizeof(dulib_member_rxendp_cc1[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc1[2]     , sizeof(dulib_member_rxendp_cc1[0])/sizeof(dulib_member_rxendp_cc1[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc1[3]     , sizeof(dulib_member_rxendp_cc1[0])/sizeof(dulib_member_rxendp_cc1[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc1[4]     , sizeof(dulib_member_rxendp_cc1[0])/sizeof(dulib_member_rxendp_cc1[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc1[5]     , sizeof(dulib_member_rxendp_cc1[0])/sizeof(dulib_member_rxendp_cc1[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc1[6]     , sizeof(dulib_member_rxendp_cc1[0])/sizeof(dulib_member_rxendp_cc1[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc1[7]     , sizeof(dulib_member_rxendp_cc1[0])/sizeof(dulib_member_rxendp_cc1[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc1[0] , sizeof(dulib_member_rxarraycar_cc1[0])/sizeof(dulib_member_rxarraycar_cc1[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc1[1] , sizeof(dulib_member_rxarraycar_cc1[1])/sizeof(dulib_member_rxarraycar_cc1[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc1[2] , sizeof(dulib_member_rxarraycar_cc1[2])/sizeof(dulib_member_rxarraycar_cc1[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc1[3] , sizeof(dulib_member_rxarraycar_cc1[3])/sizeof(dulib_member_rxarraycar_cc1[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc1[4] , sizeof(dulib_member_rxarraycar_cc1[4])/sizeof(dulib_member_rxarraycar_cc1[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc1[5] , sizeof(dulib_member_rxarraycar_cc1[5])/sizeof(dulib_member_rxarraycar_cc1[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc1[6] , sizeof(dulib_member_rxarraycar_cc1[6])/sizeof(dulib_member_rxarraycar_cc1[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc1[7] , sizeof(dulib_member_rxarraycar_cc1[7])/sizeof(dulib_member_rxarraycar_cc1[7][0])  },
	},
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc2[0]     , sizeof(dulib_member_rxendp_cc2[0])/sizeof(dulib_member_rxendp_cc2[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc2[1]     , sizeof(dulib_member_rxendp_cc2[0])/sizeof(dulib_member_rxendp_cc2[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc2[2]     , sizeof(dulib_member_rxendp_cc2[0])/sizeof(dulib_member_rxendp_cc2[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc2[3]     , sizeof(dulib_member_rxendp_cc2[0])/sizeof(dulib_member_rxendp_cc2[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc2[4]     , sizeof(dulib_member_rxendp_cc2[0])/sizeof(dulib_member_rxendp_cc2[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc2[5]     , sizeof(dulib_member_rxendp_cc2[0])/sizeof(dulib_member_rxendp_cc2[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc2[6]     , sizeof(dulib_member_rxendp_cc2[0])/sizeof(dulib_member_rxendp_cc2[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc2[7]     , sizeof(dulib_member_rxendp_cc2[0])/sizeof(dulib_member_rxendp_cc2[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc2[0] , sizeof(dulib_member_rxarraycar_cc2[0])/sizeof(dulib_member_rxarraycar_cc2[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc2[1] , sizeof(dulib_member_rxarraycar_cc2[1])/sizeof(dulib_member_rxarraycar_cc2[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc2[2] , sizeof(dulib_member_rxarraycar_cc2[2])/sizeof(dulib_member_rxarraycar_cc2[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc2[3] , sizeof(dulib_member_rxarraycar_cc2[3])/sizeof(dulib_member_rxarraycar_cc2[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc2[4] , sizeof(dulib_member_rxarraycar_cc2[4])/sizeof(dulib_member_rxarraycar_cc2[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc2[5] , sizeof(dulib_member_rxarraycar_cc2[5])/sizeof(dulib_member_rxarraycar_cc2[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc2[6] , sizeof(dulib_member_rxarraycar_cc2[6])/sizeof(dulib_member_rxarraycar_cc2[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc2[7] , sizeof(dulib_member_rxarraycar_cc2[7])/sizeof(dulib_member_rxarraycar_cc2[7][0])  },
	},
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc3[0]     , sizeof(dulib_member_rxendp_cc3[0])/sizeof(dulib_member_rxendp_cc3[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc3[1]     , sizeof(dulib_member_rxendp_cc3[0])/sizeof(dulib_member_rxendp_cc3[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc3[2]     , sizeof(dulib_member_rxendp_cc3[0])/sizeof(dulib_member_rxendp_cc3[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc3[3]     , sizeof(dulib_member_rxendp_cc3[0])/sizeof(dulib_member_rxendp_cc3[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc3[4]     , sizeof(dulib_member_rxendp_cc3[0])/sizeof(dulib_member_rxendp_cc3[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc3[5]     , sizeof(dulib_member_rxendp_cc3[0])/sizeof(dulib_member_rxendp_cc3[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc3[6]     , sizeof(dulib_member_rxendp_cc3[0])/sizeof(dulib_member_rxendp_cc3[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc3[7]     , sizeof(dulib_member_rxendp_cc3[0])/sizeof(dulib_member_rxendp_cc3[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc3[0] , sizeof(dulib_member_rxarraycar_cc3[0])/sizeof(dulib_member_rxarraycar_cc3[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc3[1] , sizeof(dulib_member_rxarraycar_cc3[1])/sizeof(dulib_member_rxarraycar_cc3[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc3[2] , sizeof(dulib_member_rxarraycar_cc3[2])/sizeof(dulib_member_rxarraycar_cc3[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc3[3] , sizeof(dulib_member_rxarraycar_cc3[3])/sizeof(dulib_member_rxarraycar_cc3[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc3[4] , sizeof(dulib_member_rxarraycar_cc3[4])/sizeof(dulib_member_rxarraycar_cc3[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc3[5] , sizeof(dulib_member_rxarraycar_cc3[5])/sizeof(dulib_member_rxarraycar_cc3[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc3[6] , sizeof(dulib_member_rxarraycar_cc3[6])/sizeof(dulib_member_rxarraycar_cc3[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc3[7] , sizeof(dulib_member_rxarraycar_cc3[7])/sizeof(dulib_member_rxarraycar_cc3[7][0])  },
	},
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc4[0]     , sizeof(dulib_member_rxendp_cc4[0])/sizeof(dulib_member_rxendp_cc4[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc4[1]     , sizeof(dulib_member_rxendp_cc4[0])/sizeof(dulib_member_rxendp_cc4[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc4[2]     , sizeof(dulib_member_rxendp_cc4[0])/sizeof(dulib_member_rxendp_cc4[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc4[3]     , sizeof(dulib_member_rxendp_cc4[0])/sizeof(dulib_member_rxendp_cc4[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc4[4]     , sizeof(dulib_member_rxendp_cc4[0])/sizeof(dulib_member_rxendp_cc4[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc4[5]     , sizeof(dulib_member_rxendp_cc4[0])/sizeof(dulib_member_rxendp_cc4[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc4[6]     , sizeof(dulib_member_rxendp_cc4[0])/sizeof(dulib_member_rxendp_cc4[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc4[7]     , sizeof(dulib_member_rxendp_cc4[0])/sizeof(dulib_member_rxendp_cc4[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc4[0] , sizeof(dulib_member_rxarraycar_cc4[0])/sizeof(dulib_member_rxarraycar_cc4[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc4[1] , sizeof(dulib_member_rxarraycar_cc4[1])/sizeof(dulib_member_rxarraycar_cc4[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc4[2] , sizeof(dulib_member_rxarraycar_cc4[2])/sizeof(dulib_member_rxarraycar_cc4[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc4[3] , sizeof(dulib_member_rxarraycar_cc4[3])/sizeof(dulib_member_rxarraycar_cc4[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc4[4] , sizeof(dulib_member_rxarraycar_cc4[4])/sizeof(dulib_member_rxarraycar_cc4[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc4[5] , sizeof(dulib_member_rxarraycar_cc4[5])/sizeof(dulib_member_rxarraycar_cc4[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc4[6] , sizeof(dulib_member_rxarraycar_cc4[6])/sizeof(dulib_member_rxarraycar_cc4[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc4[7] , sizeof(dulib_member_rxarraycar_cc4[7])/sizeof(dulib_member_rxarraycar_cc4[7][0])  },
	},
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc5[0]     , sizeof(dulib_member_rxendp_cc5[0])/sizeof(dulib_member_rxendp_cc5[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc5[1]     , sizeof(dulib_member_rxendp_cc5[0])/sizeof(dulib_member_rxendp_cc5[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc5[2]     , sizeof(dulib_member_rxendp_cc5[0])/sizeof(dulib_member_rxendp_cc5[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc5[3]     , sizeof(dulib_member_rxendp_cc5[0])/sizeof(dulib_member_rxendp_cc5[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc5[4]     , sizeof(dulib_member_rxendp_cc5[0])/sizeof(dulib_member_rxendp_cc5[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc5[5]     , sizeof(dulib_member_rxendp_cc5[0])/sizeof(dulib_member_rxendp_cc5[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc5[6]     , sizeof(dulib_member_rxendp_cc5[0])/sizeof(dulib_member_rxendp_cc5[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc5[7]     , sizeof(dulib_member_rxendp_cc5[0])/sizeof(dulib_member_rxendp_cc5[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc5[0] , sizeof(dulib_member_rxarraycar_cc5[0])/sizeof(dulib_member_rxarraycar_cc5[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc5[1] , sizeof(dulib_member_rxarraycar_cc5[1])/sizeof(dulib_member_rxarraycar_cc5[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc5[2] , sizeof(dulib_member_rxarraycar_cc5[2])/sizeof(dulib_member_rxarraycar_cc5[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc5[3] , sizeof(dulib_member_rxarraycar_cc5[3])/sizeof(dulib_member_rxarraycar_cc5[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc5[4] , sizeof(dulib_member_rxarraycar_cc5[4])/sizeof(dulib_member_rxarraycar_cc5[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc5[5] , sizeof(dulib_member_rxarraycar_cc5[5])/sizeof(dulib_member_rxarraycar_cc5[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc5[6] , sizeof(dulib_member_rxarraycar_cc5[6])/sizeof(dulib_member_rxarraycar_cc5[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc5[7] , sizeof(dulib_member_rxarraycar_cc5[7])/sizeof(dulib_member_rxarraycar_cc5[7][0])  },
	},
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc6[0]     , sizeof(dulib_member_rxendp_cc6[0])/sizeof(dulib_member_rxendp_cc6[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc6[1]     , sizeof(dulib_member_rxendp_cc6[0])/sizeof(dulib_member_rxendp_cc6[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc6[2]     , sizeof(dulib_member_rxendp_cc6[0])/sizeof(dulib_member_rxendp_cc6[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc6[3]     , sizeof(dulib_member_rxendp_cc6[0])/sizeof(dulib_member_rxendp_cc6[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc6[4]     , sizeof(dulib_member_rxendp_cc6[0])/sizeof(dulib_member_rxendp_cc6[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc6[5]     , sizeof(dulib_member_rxendp_cc6[0])/sizeof(dulib_member_rxendp_cc6[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc6[6]     , sizeof(dulib_member_rxendp_cc6[0])/sizeof(dulib_member_rxendp_cc6[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc6[7]     , sizeof(dulib_member_rxendp_cc6[0])/sizeof(dulib_member_rxendp_cc6[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc6[0] , sizeof(dulib_member_rxarraycar_cc6[0])/sizeof(dulib_member_rxarraycar_cc6[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc6[1] , sizeof(dulib_member_rxarraycar_cc6[1])/sizeof(dulib_member_rxarraycar_cc6[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc6[2] , sizeof(dulib_member_rxarraycar_cc6[2])/sizeof(dulib_member_rxarraycar_cc6[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc6[3] , sizeof(dulib_member_rxarraycar_cc6[3])/sizeof(dulib_member_rxarraycar_cc6[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc6[4] , sizeof(dulib_member_rxarraycar_cc6[4])/sizeof(dulib_member_rxarraycar_cc6[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc6[5] , sizeof(dulib_member_rxarraycar_cc6[5])/sizeof(dulib_member_rxarraycar_cc6[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc6[6] , sizeof(dulib_member_rxarraycar_cc6[6])/sizeof(dulib_member_rxarraycar_cc6[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc6[7] , sizeof(dulib_member_rxarraycar_cc6[7])/sizeof(dulib_member_rxarraycar_cc6[7][0])  },
	},
	{
		{ "a_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc7[0]     , sizeof(dulib_member_rxendp_cc7[0])/sizeof(dulib_member_rxendp_cc7[0][0])  },
		{ "b_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc7[1]     , sizeof(dulib_member_rxendp_cc7[0])/sizeof(dulib_member_rxendp_cc7[1][0])  },
		{ "c_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc7[2]     , sizeof(dulib_member_rxendp_cc7[0])/sizeof(dulib_member_rxendp_cc7[2][0])  },
		{ "d_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc7[3]     , sizeof(dulib_member_rxendp_cc7[0])/sizeof(dulib_member_rxendp_cc7[3][0])  },
		{ "e_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc7[4]     , sizeof(dulib_member_rxendp_cc7[0])/sizeof(dulib_member_rxendp_cc7[4][0])  },
		{ "f_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc7[5]     , sizeof(dulib_member_rxendp_cc7[0])/sizeof(dulib_member_rxendp_cc7[5][0])  },
		{ "g_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc7[6]     , sizeof(dulib_member_rxendp_cc7[0])/sizeof(dulib_member_rxendp_cc7[6][0])  },
		{ "h_endp"	,	sizeof(T_RRH_DU_CMD_INFO_ENDP)     ,NULL ,dulib_member_rxendp_cc7[7]     , sizeof(dulib_member_rxendp_cc7[0])/sizeof(dulib_member_rxendp_cc7[7][0])  },
		{ "a_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc7[0] , sizeof(dulib_member_rxarraycar_cc7[0])/sizeof(dulib_member_rxarraycar_cc7[0][0])  },
		{ "b_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc7[1] , sizeof(dulib_member_rxarraycar_cc7[1])/sizeof(dulib_member_rxarraycar_cc7[1][0])  },
		{ "c_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc7[2] , sizeof(dulib_member_rxarraycar_cc7[2])/sizeof(dulib_member_rxarraycar_cc7[2][0])  },
		{ "d_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc7[3] , sizeof(dulib_member_rxarraycar_cc7[3])/sizeof(dulib_member_rxarraycar_cc7[3][0])  },
		{ "e_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc7[4] , sizeof(dulib_member_rxarraycar_cc7[4])/sizeof(dulib_member_rxarraycar_cc7[4][0])  },
		{ "f_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc7[5] , sizeof(dulib_member_rxarraycar_cc7[5])/sizeof(dulib_member_rxarraycar_cc7[5][0])  },
		{ "g_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc7[6] , sizeof(dulib_member_rxarraycar_cc7[6])/sizeof(dulib_member_rxarraycar_cc7[6][0])  },
		{ "h_array"	,	sizeof(T_RRH_DU_CMD_INFO_ARRAYCAR) ,NULL ,dulib_member_rxarraycar_cc7[7] , sizeof(dulib_member_rxarraycar_cc7[7])/sizeof(dulib_member_rxarraycar_cc7[7][0])  },
	}
};



static du_t_members dulib_member_com_info[21] = {
	{ "pid"				,sizeof(INT)   ,NULL ,NULL ,0 },{ "device_type"	,sizeof(UINT)  ,NULL ,NULL ,0 },{ "support_band"	,sizeof(UINT)  ,NULL ,NULL ,0 },
	{ "mmw_power_mode"	,sizeof(UINT)  ,NULL ,NULL ,0 },{ "tx_mode"		,sizeof(UINT)  ,NULL ,NULL ,0 },{ "radio_frame_ofs"	,sizeof(UINT)  ,NULL ,NULL ,0 },
	{ "sfn_ofs"			,sizeof(INT)   ,NULL ,NULL ,0 },{ "ptp_valid"	,sizeof(UINT)  ,NULL ,NULL ,0 },{ "fh_valid"		,sizeof(UINT)  ,NULL ,NULL ,0 },
	{ "reset_reason"	,sizeof(UINT)  ,NULL ,NULL ,0 },{ "force_uplane",sizeof(UINT)  ,NULL ,NULL ,0 },

	{ "mplane_con"		,sizeof(UINT)  ,NULL ,NULL ,0 },{ "low_vlan",sizeof(UINT)  ,NULL ,NULL ,0 },
	{ "high_vlan"		,sizeof(UINT)  ,NULL ,NULL ,0 },{ "last_vlan",sizeof(UINT)  ,NULL ,NULL ,0 },
	{ "pcp"				,sizeof(UINT)  ,NULL ,NULL ,0 },{ "ssh_port",sizeof(UINT)  ,NULL ,NULL ,0 },
	{ "ssh_port_callhome",sizeof(UINT)  ,NULL ,NULL ,0 }, { "startup_face",sizeof(UINT)  ,NULL ,NULL ,0 },
	{ "bfmode"			,sizeof(UINT)  ,NULL ,NULL ,0 }, { "txgain"		,sizeof(UINT)  ,NULL ,NULL ,0 }
};

static du_t_members dulib_member_ptp_info[8] = {
	{ "port_number"		,sizeof(UINT)  ,NULL,NULL ,0 },{ "domain_number"	,sizeof(UINT)  ,NULL,NULL ,0 },
	{ "multicast_mac_addr",sizeof(UINT),NULL,NULL ,0 },{ "accepted_clock_classes",sizeof(UINT),NULL,NULL ,0 },
	{ "sync_status"		,sizeof(UINT)  ,NULL,NULL ,0 },{ "syncE_status"		 ,sizeof(UINT),NULL,NULL ,0 },
	{ "ptp_status"		,sizeof(UINT)  ,NULL,NULL ,0 },{ "ptp_clock_class"	,sizeof(UINT)  ,NULL,NULL ,0 }
};

static du_t_members dulib_member_fh_info[2] = {
	{ "0_port"					,	sizeof(T_RRH_DU_CMD_INFO_FH)     ,NULL,dulib_member_fh_port[0]  , sizeof(dulib_member_fh_port[0]) / sizeof(dulib_member_fh_port[0][0]) },
	{ "1_port"					,	sizeof(T_RRH_DU_CMD_INFO_FH)     ,NULL,dulib_member_fh_port[1]  , sizeof(dulib_member_fh_port[1]) / sizeof(dulib_member_fh_port[1][0]) }
};

static du_t_members dulib_member_txcc_info[8] = {
	{ "0_cc"					,	sizeof(T_RRH_DU_CMD_INFO_TXCC)   ,NULL,dulib_member_txcc[0]     , sizeof(dulib_member_txcc[0]) / sizeof(dulib_member_txcc[0][0]) },
	{ "1_cc"					,	sizeof(T_RRH_DU_CMD_INFO_TXCC)   ,NULL,dulib_member_txcc[1]     , sizeof(dulib_member_txcc[1]) / sizeof(dulib_member_txcc[1][0]) },
	{ "2_cc"					,	sizeof(T_RRH_DU_CMD_INFO_TXCC)   ,NULL,dulib_member_txcc[2]     , sizeof(dulib_member_txcc[2]) / sizeof(dulib_member_txcc[2][0]) },
	{ "3_cc"					,	sizeof(T_RRH_DU_CMD_INFO_TXCC)   ,NULL,dulib_member_txcc[3]     , sizeof(dulib_member_txcc[3]) / sizeof(dulib_member_txcc[3][0]) },
	{ "4_cc"					,	sizeof(T_RRH_DU_CMD_INFO_TXCC)   ,NULL,dulib_member_txcc[4]     , sizeof(dulib_member_txcc[4]) / sizeof(dulib_member_txcc[4][0]) },
	{ "5_cc"					,	sizeof(T_RRH_DU_CMD_INFO_TXCC)   ,NULL,dulib_member_txcc[5]     , sizeof(dulib_member_txcc[5]) / sizeof(dulib_member_txcc[5][0]) },
	{ "6_cc"					,	sizeof(T_RRH_DU_CMD_INFO_TXCC)   ,NULL,dulib_member_txcc[6]     , sizeof(dulib_member_txcc[6]) / sizeof(dulib_member_txcc[6][0]) },
	{ "7_cc"					,	sizeof(T_RRH_DU_CMD_INFO_TXCC)   ,NULL,dulib_member_txcc[7]     , sizeof(dulib_member_txcc[7]) / sizeof(dulib_member_txcc[7][0]) }
};

static du_t_members dulib_member_rxcc_info[8] = {
	{ "0_cc"					,	sizeof(T_RRH_DU_CMD_INFO_RXCC)   ,NULL,dulib_member_rxcc[0]     , sizeof(dulib_member_rxcc[0]) / sizeof(dulib_member_rxcc[0][0]) },
	{ "1_cc"					,	sizeof(T_RRH_DU_CMD_INFO_RXCC)   ,NULL,dulib_member_rxcc[1]     , sizeof(dulib_member_rxcc[1]) / sizeof(dulib_member_rxcc[1][0]) },
	{ "2_cc"					,	sizeof(T_RRH_DU_CMD_INFO_RXCC)   ,NULL,dulib_member_rxcc[2]     , sizeof(dulib_member_rxcc[2]) / sizeof(dulib_member_rxcc[2][0]) },
	{ "3_cc"					,	sizeof(T_RRH_DU_CMD_INFO_RXCC)   ,NULL,dulib_member_rxcc[3]     , sizeof(dulib_member_rxcc[3]) / sizeof(dulib_member_rxcc[3][0]) },
	{ "4_cc"					,	sizeof(T_RRH_DU_CMD_INFO_RXCC)   ,NULL,dulib_member_rxcc[4]     , sizeof(dulib_member_rxcc[4]) / sizeof(dulib_member_rxcc[4][0]) },
	{ "5_cc"					,	sizeof(T_RRH_DU_CMD_INFO_RXCC)   ,NULL,dulib_member_rxcc[5]     , sizeof(dulib_member_rxcc[5]) / sizeof(dulib_member_rxcc[5][0]) },
	{ "6_cc"					,	sizeof(T_RRH_DU_CMD_INFO_RXCC)   ,NULL,dulib_member_rxcc[6]     , sizeof(dulib_member_rxcc[6]) / sizeof(dulib_member_rxcc[6][0]) },
	{ "7_cc"					,	sizeof(T_RRH_DU_CMD_INFO_RXCC)   ,NULL,dulib_member_rxcc[7]     , sizeof(dulib_member_rxcc[7]) / sizeof(dulib_member_rxcc[7][0]) }
};

static du_t_members dulib_members_info_cmd[5] = {
	{ "com_info"				,	sizeof(T_RRH_DU_CMD_INFO_COMMON) ,NULL, dulib_member_com_info   , sizeof(dulib_member_com_info)/sizeof(dulib_member_com_info[0])   },
	{ "ptp_info"				,	sizeof(T_RRH_DU_CMD_INFO_PTP)    ,NULL, dulib_member_ptp_info   , sizeof(dulib_member_ptp_info)/sizeof(dulib_member_ptp_info[0])   },
	{ "fh_info"					,	sizeof(T_RRH_DU_CMD_INFO_FH)*2   ,NULL, dulib_member_fh_info    , sizeof(dulib_member_fh_info)/sizeof(dulib_member_fh_info[0])     },
	{ "txcc_info"				,	sizeof(T_RRH_DU_CMD_INFO_TXCC)*8 ,NULL, dulib_member_txcc_info  , sizeof(dulib_member_txcc_info)/sizeof(dulib_member_txcc_info[0]) },
	{ "rxcc_info"				,	sizeof(T_RRH_DU_CMD_INFO_RXCC)*8 ,NULL, dulib_member_rxcc_info  , sizeof(dulib_member_rxcc_info)/sizeof(dulib_member_rxcc_info[0]) }
};

/*!
 * @brief		コマンド情報初期化処理
 * @note
	<pre>
	duコマンドライブラリの初期化を行う.
	</pre>
 * @param		mrb		[in]	mrb status
 * @return		N/A
 * @date		2013/11/18 FJT)Taniguchi Create
 * @date		2018/12/12 KCN)Wang modify
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
unsigned int rrhlib_init_cmdshm(mrb_state* mrb){
	UINT	count1,count2;
	UCHAR		eepdata;
	du_t_members*	info_members;
	du_t_members*	member;
	du_t_members*	member2;
	du_t_members*	member3;
	int	endcd;
	int	errcd;
	struct bpf_timespec		dtime;	/* sem waittime */	
	UINT			radio_frame_ofs = 0;
	INT				sfn_ofs = 0;

	dtime.tv_sec = 3;
	dtime.tv_nsec = 0;
	
	if( (endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_DU_CMD_INFO,(VOID **)&dulib_cmdshm_addr,&errcd)) != BPF_RU_IPCM_OK ){
		rrhlib_dmsg("Memory Get key NG = %d ,errcd = %d \n",endcd,errcd);
		return D_RRH_NG;
	}

	endcd = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_DU_CMD_INFO,BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, &dtime, &errcd);
	if(BPF_RU_IPCM_OK != endcd){
		rrhlib_dmsg("sem take NG = %d ,errcd = %d \n",endcd,errcd);
		return D_RRH_NG;
	}

	if (dulib_cmdshm_addr->com_info.device_type == 0) {
		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_DEVICE_KIND0,&eepdata);
		dulib_cmdshm_addr->com_info.device_type = (UINT)eepdata;

		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_DEVICE_KIND1,&eepdata);
		dulib_cmdshm_addr->com_info.support_band = (UINT)eepdata;

		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_PTP_STOP_FLAG,&eepdata);
		dulib_cmdshm_addr->com_info.tx_mode = (UINT)eepdata;

		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_OFFSET_A_H,&eepdata);
		radio_frame_ofs = ((UINT)eepdata) << 16;
		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_OFFSET_A_M,&eepdata);
		radio_frame_ofs |= ((UINT)eepdata) << 8;
		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_OFFSET_A_L,&eepdata);
		radio_frame_ofs |= ((UINT)eepdata);
		dulib_cmdshm_addr->com_info.radio_frame_ofs = radio_frame_ofs;
		
		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_OFFSET_B_H,&eepdata);
		sfn_ofs = ((INT)eepdata) << 8;
		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_OFFSET_B_L,&eepdata);
		sfn_ofs |= ((INT)eepdata);
		if(sfn_ofs > 0x7FFF){
			dulib_cmdshm_addr->com_info.sfn_ofs = (sfn_ofs | 0xFFFF0000);
		}else{
			dulib_cmdshm_addr->com_info.sfn_ofs = sfn_ofs;
		}

		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_MP_CONNECT_FLAG,&eepdata);
		if(eepdata == D_RRH_ON){
			dulib_cmdshm_addr->com_info.mplane_con = D_RRH_ON;
		}else{
			dulib_cmdshm_addr->com_info.mplane_con = D_RRH_OFF;
		}
		
		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_SSH_USEABLE_FLAG,&eepdata);
		if(eepdata == D_RRH_ON){

			BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_SERVER_SSH_PORT_H,&eepdata);
			dulib_cmdshm_addr->com_info.ssh_port = (UINT)((eepdata << 8)&0xFF00);
			BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_SERVER_SSH_PORT_L,&eepdata);
			dulib_cmdshm_addr->com_info.ssh_port |= (UINT)(eepdata & 0xFF);

			BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CALLHOME_SSH_PORT_H,&eepdata);
			dulib_cmdshm_addr->com_info.ssh_port_callhome = (UINT)((eepdata << 8)&0xFF00);
			BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CALLHOME_SSH_PORT_L,&eepdata);
			dulib_cmdshm_addr->com_info.ssh_port_callhome |= (UINT)(eepdata & 0xFF);

		}else{
			dulib_cmdshm_addr->com_info.ssh_port = D_DU_DEFAULT_SSHPORTNO;
			dulib_cmdshm_addr->com_info.ssh_port_callhome = D_DU_DEFAULT_SSHPORTNO_CALLHOME;
		}
		
		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_VID_LOW_PORT1_H,&eepdata);
		dulib_cmdshm_addr->com_info.low_vlan = (UINT)((eepdata << 8)&0xFF00);
		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_VID_LOW_PORT1_L,&eepdata);
		dulib_cmdshm_addr->com_info.low_vlan |= (UINT)(eepdata & 0xFF);
		if(dulib_cmdshm_addr->com_info.low_vlan > D_RRH_VID_MAX){
			dulib_cmdshm_addr->com_info.low_vlan = 1;
		}

		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_VID_HIGH_PORT1_H,&eepdata);
		dulib_cmdshm_addr->com_info.high_vlan = (UINT)((eepdata << 8)&0xFF00);
		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_VID_HIGH_PORT1_L,&eepdata);
		dulib_cmdshm_addr->com_info.high_vlan |= (UINT)(eepdata & 0xFF);
		if(dulib_cmdshm_addr->com_info.high_vlan > D_RRH_VID_MAX){
			dulib_cmdshm_addr->com_info.high_vlan = 1;
		}

		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_VID_LAST_PORT1_H,&eepdata);
		dulib_cmdshm_addr->com_info.last_vlan = (UINT)((eepdata << 8)&0xFF00);
		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_VID_LAST_PORT1_L,&eepdata);
		dulib_cmdshm_addr->com_info.last_vlan |= (UINT)(eepdata & 0xFF);
		if(dulib_cmdshm_addr->com_info.last_vlan > D_RRH_VID_MAX){
			dulib_cmdshm_addr->com_info.last_vlan = 1;
		}

		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_VID_MPLANE_PCP,&eepdata);
		dulib_cmdshm_addr->com_info.pcp = (UINT)(eepdata);
		if(dulib_cmdshm_addr->com_info.pcp > D_RRH_PCP_MAX){
			dulib_cmdshm_addr->com_info.pcp = D_RRH_PCP_MPLANE;
		}

		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_TXGAIN,&eepdata);
		if(eepdata > 10){
			dulib_cmdshm_addr->com_info.txgain = 0;
		}else{
			dulib_cmdshm_addr->com_info.txgain = eepdata;
		}

	}

	if(dulib_cmdshm_addr->com_info.ptp_valid == D_RRH_OFF){
		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_VID_DOMAIN_NO,&eepdata);
		if((eepdata < 0x18) || (eepdata > 0x2B)){
			eepdata = 0x18;
		}
		dulib_cmdshm_addr->ptp_info.domain_number = (UINT)eepdata;

		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_VID_MULTI_MACADDR,&eepdata);
		dulib_cmdshm_addr->ptp_info.multicast_mac_addr = (UINT)eepdata;

		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_VID_ACPT_CLK_CLASS,&eepdata);
		dulib_cmdshm_addr->ptp_info.accepted_clock_classes = (UINT)eepdata;
		dulib_cmdshm_addr->com_info.ptp_valid = D_RRH_ON;
	}

	info_members                      = &dulib_members_info_cmd[0];
	info_members->shm_addr            = &(dulib_cmdshm_addr->com_info);
	member = info_members->member;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.pid);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.device_type);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.support_band);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.mmw_power_mode);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.tx_mode);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.radio_frame_ofs);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.sfn_ofs);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.ptp_valid);	
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.fh_valid);	
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.reset_reason);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.force_uplane);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.mplane_con);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.low_vlan);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.high_vlan);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.last_vlan);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.pcp);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.ssh_port);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.ssh_port_callhome);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.startup_face);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.bfmode);
	member++;
	member->shm_addr = (void*)&(dulib_cmdshm_addr->com_info.txgain);

	info_members                      = &dulib_members_info_cmd[1];
	info_members->shm_addr            = &(dulib_cmdshm_addr->ptp_info);
	member = info_members->member;
	member->shm_addr = &(dulib_cmdshm_addr->ptp_info.port_number);
	member++;
	member->shm_addr = &(dulib_cmdshm_addr->ptp_info.domain_number);
	member++;
	member->shm_addr = &(dulib_cmdshm_addr->ptp_info.multicast_mac_addr);
	member++;
	member->shm_addr = &(dulib_cmdshm_addr->ptp_info.accepted_clock_classes);
	member++;
	member->shm_addr = &(dulib_cmdshm_addr->ptp_info.sync_status);
	member++;
	member->shm_addr = &(dulib_cmdshm_addr->ptp_info.syncE_status);
	member++;
	member->shm_addr = &(dulib_cmdshm_addr->ptp_info.ptp_status);
	member++;
	member->shm_addr = &(dulib_cmdshm_addr->ptp_info.ptp_clock_class);

	info_members                      = &dulib_members_info_cmd[2];
	info_members->shm_addr            = &(dulib_cmdshm_addr->fh_info);
	member = info_members->member;
	for( count1=0 ; count1 < 2; count1++,member++){
		member->shm_addr = &(dulib_cmdshm_addr->fh_info[count1]);
		member2 = ((du_t_members*)member)->member;
		member2->shm_addr = &(dulib_cmdshm_addr->fh_info[count1].link_onoff);
		member2++;
		member2->shm_addr = &(dulib_cmdshm_addr->fh_info[count1].remote_mac_addr_h);
		member2++;
		member2->shm_addr = &(dulib_cmdshm_addr->fh_info[count1].remote_mac_addr_l);
		member2++;
		member2->shm_addr = &(dulib_cmdshm_addr->fh_info[count1].vid_dl);
		member2++;
		member2->shm_addr = &(dulib_cmdshm_addr->fh_info[count1].vid_ul);
		member2++;
		member2->shm_addr = &(dulib_cmdshm_addr->fh_info[count1].pcp_ul);
	}

	info_members                      = &dulib_members_info_cmd[3];
	info_members->shm_addr            = &(dulib_cmdshm_addr->txcc_info);
	member = info_members->member;

	for( count1=0 ; count1 < 8 ; count1++,member++){
		member->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1]);
		member2 = (du_t_members*)(member->member);

		for(count2=0;count2 < 8;count2++,member2++){
			member2->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].endpoint[count2]);

			member3 = (du_t_members*)(member2->member);
			member3->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].endpoint[count2].onoff);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].endpoint[count2].Acell_id);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].endpoint[count2].CC_id);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].endpoint[count2].Port_id);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].endpoint[count2].Acell_id_ex);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].endpoint[count2].CC_id_ex);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].endpoint[count2].Port_id_ex);
		}

		for(count2=0;count2 < 8;count2++,member2++){
			member2->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].array[count2]);

			member3 = (du_t_members*)(member2->member);
			member3->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].array[count2].onoff);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].array[count2].test_onoff);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].array[count2].nr_freq);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].array[count2].nr_bandwidth);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].array[count2].ul_fft_smpl_ofs);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].array[count2].ta_offset);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->txcc_info[count1].array[count2].nr_scs);
		}
	}

	info_members                      = &dulib_members_info_cmd[4];
	info_members->shm_addr            = &(dulib_cmdshm_addr->rxcc_info);
	member = info_members->member;

	for( count1=0 ; count1 < 8; count1++,member++){
		member->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1]);
		member2 = (du_t_members*)(member->member);
		for(count2=0;count2 < 8;count2++,member2++){
			member2->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].endpoint[count2]);

			member3 = (du_t_members*)(member2->member);
			member3->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].endpoint[count2].onoff);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].endpoint[count2].Acell_id);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].endpoint[count2].CC_id);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].endpoint[count2].Port_id);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].endpoint[count2].Acell_id_ex);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].endpoint[count2].CC_id_ex);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].endpoint[count2].Port_id_ex);
		}

		for(count2=0;count2 < 8;count2++,member2++){
			member2->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].array[count2]);

			member3 = (du_t_members*)(member2->member);
			member3->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].array[count2].onoff);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].array[count2].test_onoff);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].array[count2].nr_freq);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].array[count2].nr_bandwidth);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].array[count2].ul_fft_smpl_ofs);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].array[count2].ta_offset);
			member3++;
			member3->shm_addr = &(dulib_cmdshm_addr->rxcc_info[count1].array[count2].nr_scs);
		}

	}
	endcd =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_DU_CMD_INFO,BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != endcd){
		rrhlib_dmsg("sem give NG = %d ,errcd = %d \n",endcd,errcd);
	}
	return D_RRH_OK;
}

/*!
 * @brief        共有メモリ検索処理
 * @note
<pre>
 	共有メモリを検索する処理である
</pre>
 * @tparam      書き込みアドレス   [in]    Integer
 * @tparam      書き込みデータ     [in]    Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/16
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
static du_t_members* rrhlib_cmdshm_key_search( void* high_str , void* low_str )
{
	unsigned int	count;
	du_t_members*	info_members = NULL;
	du_t_members*	member = NULL;
	
	for(count=0 ; count < (sizeof(dulib_members_info_cmd)/sizeof(dulib_members_info_cmd[0])) ; count++ ){
		info_members = &dulib_members_info_cmd[count];
		if (!strncmp(high_str,info_members->name,strlen(high_str))){
			break;
		}
		info_members = NULL;
	}
	if(info_members == NULL){
		return info_members;
	}
	member = (du_t_members*)(info_members->member);
	for(count=0 ; count < info_members->member_size ; count++,member++ ){
		if(!strncmp(low_str,member->name,strlen(low_str))){
			break;	
		}
	}
	if(count >= info_members->member_size){
		member = NULL;
	}
	return member;
}

static du_t_members* rrhlib_cmdshm_key_search2( void* high_str , void* mid_str, void* low_str )
{
	unsigned int	count;
	du_t_members*	info_members = NULL;
	du_t_members*	member = NULL;

	info_members = rrhlib_cmdshm_key_search(high_str,mid_str);
	if(info_members == NULL){
		return info_members;
	}
	member = (du_t_members*)(info_members->member);
	for(count=0 ; count < info_members->member_size ; count++ ,member++){
		if (!strncmp(low_str,member->name,strlen(low_str))){
			break;	
		}
	}
	if(count >= info_members->member_size){
		member = NULL;
	}
	return member;
}

static du_t_members* rrhlib_cmdshm_key_search3( void* top_str ,void* high_str , void* mid_str,void* low_str )
{
	unsigned int	count;
	du_t_members*	info_members = NULL;
	du_t_members*	member = NULL;

	info_members = rrhlib_cmdshm_key_search2(top_str,high_str,mid_str);
	if(info_members == NULL){
		return info_members;
	}
	member = (du_t_members*)(info_members->member);
	for(count=0 ; count < info_members->member_size ; count++ ,member++){
		if (!strncmp(low_str,member->name,strlen(low_str))){
			break;	
		}
	}
	if(count >= info_members->member_size){
		member = NULL;
	}
	return member;
}

void rrhlib_cmdshm_update(void* dst_p ,void* src_p , int size )
{
	int	endcd;
	int	errcd;
	struct bpf_timespec		dtime;	/* sem waittime */	
	dtime.tv_sec = 3;
	dtime.tv_nsec = 0;

	endcd = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_DU_CMD_INFO,BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, &dtime, &errcd);
	if(BPF_RU_IPCM_OK != endcd){
		rrhlib_dmsg("sem take NG = %d ,errcd = %d \n",endcd,errcd);
		return;
	}
	
	memcpy(dst_p,src_p,size);

	endcd =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_DU_CMD_INFO,BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != endcd){
		rrhlib_dmsg("sem give NG = %d ,errcd = %d \n",endcd,errcd);
	}
	return;
}

/*!
 * @brief        共有メモリ読み取り処理
 * @note
<pre>
 	共有メモリからデータを読み取る処理である.
</pre>
 * @tparam      読み取り種別     [in]    String
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/13
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.shmread
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_cmdshm_read(mrb_state* mrb, mrb_value self,void* key)
{
	mrb_value		highstr;
	unsigned int	data;
	char			rtn_str[64];
	int				size;
	du_t_members*	member = NULL;

	mrb_get_args(mrb, "S", &highstr);

	member = rrhlib_cmdshm_key_search(key,RSTRING_PTR(highstr));
	if(member == NULL){
		rrhlib_dmsg("Not entry %s.%s \n", key,RSTRING_PTR(highstr));
		return mrb_false_value();
	}

	rrhlib_cmdshm_update(&data,member->shm_addr,member->size);
	
	size = snprintf(rtn_str, sizeof(rtn_str), "%08x", data);
	if(size < 0){
		return mrb_false_value();
	}
	rrhlib_dmsg("GET %s.%s -> 0x%x \n",  key,RSTRING_PTR(highstr),data);
	return mrb_str_new(mrb, rtn_str, size);
}

/* com */
mrb_value rrhlib_cmdshm_read_cm(mrb_state* mrb, mrb_value self)
{
	return rrhlib_cmdshm_read(mrb,self,"com_info");
}

/* ptp */
mrb_value rrhlib_cmdshm_read_ptp(mrb_state* mrb, mrb_value self)
{
	return rrhlib_cmdshm_read(mrb,self,"ptp_info");
}



/*!
 * @brief        共有メモリ読み取り処理
 * @note
<pre>
 	共有メモリからデータを読み取る処理である.
</pre>
 * @tparam      読み取り種別     [in]    String
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/13
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.shmread
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_cmdshm_read2(mrb_state* mrb, mrb_value self, void*key)
{
	mrb_value		highstr;
	mrb_value		lowstr;
	unsigned int	data;
	char			rtn_str[64];
	int				size;
	du_t_members*	member = NULL;

	mrb_get_args(mrb, "SS",  &highstr, &lowstr);

	member = rrhlib_cmdshm_key_search2(key,RSTRING_PTR(highstr),RSTRING_PTR(lowstr));
	if(member == NULL){
		rrhlib_dmsg("Not entry %s.%s.%s \n", key,RSTRING_PTR(highstr),RSTRING_PTR(lowstr));
		return mrb_false_value();
	}

	rrhlib_cmdshm_update(&data,member->shm_addr,member->size);

	size = snprintf(rtn_str, sizeof(rtn_str), "%08x", data);
	if(size < 0){
		return mrb_false_value();
	}
	rrhlib_dmsg("GET2 %s.%s.%s -> 0x%x \n",key,RSTRING_PTR(highstr),RSTRING_PTR(lowstr),data);
	return mrb_str_new(mrb, rtn_str, size);
}

/* fh */
mrb_value rrhlib_cmdshm_read2_fh(mrb_state* mrb, mrb_value self)
{
	return rrhlib_cmdshm_read2(mrb,self,"fh_info");
}



/*!
 * @brief        共有メモリ読み取り処理
 * @note
<pre>
 	共有メモリからデータを読み取る処理である.
</pre>
 * @tparam      読み取り種別     [in]    String
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/13
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.shmread
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_cmdshm_read3(mrb_state* mrb, mrb_value self , void* key)
{
	mrb_value		highstr;
	mrb_value		midstr;
	mrb_value		lowstr;
	unsigned int	data;
	char			rtn_str[64];
	int				size;
	du_t_members*	member = NULL;

	mrb_get_args(mrb, "SSS", &highstr, &midstr, &lowstr);

	member = rrhlib_cmdshm_key_search3(key,RSTRING_PTR(highstr),RSTRING_PTR(midstr),RSTRING_PTR(lowstr));
	if(member == NULL){
		rrhlib_dmsg("Not entry %s.%s.%s.%s \n", key,RSTRING_PTR(highstr),RSTRING_PTR(midstr),RSTRING_PTR(lowstr));
		return mrb_false_value();
	}

	rrhlib_cmdshm_update(&data,member->shm_addr,member->size);

	size = snprintf(rtn_str, sizeof(rtn_str), "%08x", data);
	if(size < 0){
		return mrb_false_value();
	}
	rrhlib_dmsg("GET3 %s.%s.%s.%s -> 0x%x \n",  key,RSTRING_PTR(highstr),RSTRING_PTR(midstr),RSTRING_PTR(lowstr),data);
	return mrb_str_new(mrb, rtn_str, size);
}

/* tx */
mrb_value rrhlib_cmdshm_read3_tx(mrb_state* mrb, mrb_value self)
{
	return rrhlib_cmdshm_read3(mrb,self,"txcc_info");
}

/* rx */
mrb_value rrhlib_cmdshm_read3_rx(mrb_state* mrb, mrb_value self)
{
	return rrhlib_cmdshm_read3(mrb,self,"rxcc_info");
}


/*!
 * @brief        共有メモリ書き込み処理
 * @note
<pre>
 	共有メモリにトレーニングデータを書き込む処理である.
</pre>
 * @tparam      書き込みアドレス   [in]    Integer
 * @tparam      書き込みデータ     [in]    Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/16
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.shmwrite
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_cmdshm_write(mrb_state* mrb, mrb_value self,void*key)
{
	mrb_value		highstr;
	mrb_int			data;
	du_t_members*	member = NULL;

	mrb_get_args(mrb, "Si", &highstr ,&data );
	member = rrhlib_cmdshm_key_search(key,RSTRING_PTR(highstr));
	if(member == NULL){
		rrhlib_dmsg("Not entry %s.%s \n", key,RSTRING_PTR(highstr));
		return mrb_false_value();
	}

	rrhlib_cmdshm_update(member->shm_addr,&data,member->size);

	rrhlib_dmsg("SET:%s.%s <- 0x%x \n",  key,RSTRING_PTR(highstr),data);
	return mrb_true_value();
}

mrb_value rrhlib_cmdshm_write_cm(mrb_state* mrb, mrb_value self)
{
	return rrhlib_cmdshm_write(mrb,self,"com_info");
}

mrb_value rrhlib_cmdshm_write_ptp(mrb_state* mrb, mrb_value self)
{
	return rrhlib_cmdshm_write(mrb,self,"ptp_info");
}

/*!
 * @brief        共有メモリ書き込み処理
 * @note
<pre>
 	共有メモリにトレーニングデータを書き込む処理である.
</pre>
 * @tparam      書き込みアドレス   [in]    Integer
 * @tparam      書き込みデータ     [in]    Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/16
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.shmwrite
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_cmdshm_write2(mrb_state* mrb, mrb_value self,void* key)
{
	mrb_value		highstr;
	mrb_value		lowstr;
	mrb_int			data;
	du_t_members*	member = NULL;

	mrb_get_args(mrb, "SSi",  &highstr, &lowstr, &data );

	member = rrhlib_cmdshm_key_search2(key,RSTRING_PTR(highstr),RSTRING_PTR(lowstr));
	if(member == NULL){
		rrhlib_dmsg("Not entry %s.%s.%s \n", key,RSTRING_PTR(highstr),RSTRING_PTR(lowstr));
		return mrb_false_value();
	}

	rrhlib_cmdshm_update(member->shm_addr,&data,member->size);

	rrhlib_dmsg("SET2:%s.%s.%s <- 0x%x \n",  key,RSTRING_PTR(highstr),RSTRING_PTR(lowstr),data);
	return mrb_true_value();
}

mrb_value rrhlib_cmdshm_write2_fh(mrb_state* mrb, mrb_value self)
{
	return rrhlib_cmdshm_write2(mrb,self,"fh_info");
}

/*!
 * @brief        共有メモリ書き込み処理
 * @note
<pre>
 	共有メモリにトレーニングデータを書き込む処理である.
</pre>
 * @tparam      書き込みアドレス   [in]    Integer
 * @tparam      書き込みデータ     [in]    Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/16
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.shmwrite
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_cmdshm_write3(mrb_state* mrb, mrb_value self,void* key)
{
	mrb_value		highstr;
	mrb_value		midstr;
	mrb_value		lowstr;
	mrb_int			data;
	du_t_members*	member = NULL;
 
	mrb_get_args(mrb, "SSSi",&highstr,&midstr, &lowstr, &data );
	member = rrhlib_cmdshm_key_search3(key,RSTRING_PTR(highstr),RSTRING_PTR(midstr),RSTRING_PTR(lowstr));
	if(member == NULL){
		rrhlib_dmsg("Not entry %s.%s.%s.%s \n", key,RSTRING_PTR(highstr),RSTRING_PTR(midstr),RSTRING_PTR(lowstr));
		return mrb_false_value();
	}

	rrhlib_cmdshm_update(member->shm_addr,&data,member->size);

	rrhlib_dmsg("SET3:%s.%s.%s.%s <- 0x%x \n",key,RSTRING_PTR(highstr),RSTRING_PTR(midstr),RSTRING_PTR(lowstr),data);
	return mrb_true_value();
}

mrb_value rrhlib_cmdshm_write3_tx(mrb_state* mrb, mrb_value self)
{
	return rrhlib_cmdshm_write3(mrb,self,"txcc_info");
}

mrb_value rrhlib_cmdshm_write3_rx(mrb_state* mrb, mrb_value self)
{
	return rrhlib_cmdshm_write3(mrb,self,"rxcc_info");
}

/*!
 * @brief        DUコマンド用テーブルアクセス系モジュール登録処理
 * @note
<pre>
	スクリプト上で使用するクラス、モジュール、メソッドを登録する関数である.
</pre>
 * @param		mrb	[in]	mrb status
 * @return		N/A
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
unsigned int rrhlib_tbl_access_install(mrb_state* mrb ,struct RClass * module_class)
{
	unsigned int result;

	if(( result = rrhlib_init_cmdshm(mrb)) == D_RRH_OK){
		mrb_define_module_function(mrb, module_class, "get_shmcm"  ,  rrhlib_cmdshm_read_cm         , MRB_ARGS_REQ(1));
		mrb_define_module_function(mrb, module_class, "get_shmptp" ,  rrhlib_cmdshm_read_ptp        , MRB_ARGS_REQ(1));
		mrb_define_module_function(mrb, module_class, "get_shmfh"  ,  rrhlib_cmdshm_read2_fh        , MRB_ARGS_REQ(2));
		mrb_define_module_function(mrb, module_class, "get_shmtx"  ,  rrhlib_cmdshm_read3_tx        , MRB_ARGS_REQ(3));
		mrb_define_module_function(mrb, module_class, "get_shmrx"  ,  rrhlib_cmdshm_read3_rx        , MRB_ARGS_REQ(3));
		mrb_define_module_function(mrb, module_class, "set_shmcm"  ,  rrhlib_cmdshm_write_cm        , MRB_ARGS_REQ(2));
		mrb_define_module_function(mrb, module_class, "set_shmptp" ,  rrhlib_cmdshm_write_ptp       , MRB_ARGS_REQ(2));
		mrb_define_module_function(mrb, module_class, "set_shmfh"  ,  rrhlib_cmdshm_write2_fh       , MRB_ARGS_REQ(3));
		mrb_define_module_function(mrb, module_class, "set_shmtx"  ,  rrhlib_cmdshm_write3_tx       , MRB_ARGS_REQ(4));
		mrb_define_module_function(mrb, module_class, "set_shmrx"  ,  rrhlib_cmdshm_write3_rx       , MRB_ARGS_REQ(4));
	}
	return result;
}

/* @} */
