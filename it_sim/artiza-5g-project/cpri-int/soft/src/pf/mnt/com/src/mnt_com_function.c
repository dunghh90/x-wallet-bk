/*!
 * @skip  $ld:$
 * @file  mnt_com_function.c
 * @brief MNT 共通
 * @date   2013/11/20 FFCS)zhaodx create for zynq
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/** @addtogroup RRH_MNT
 *  @{
 */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "mnt_com_inc.h"

T_CMD_TCPCONN_NUM mnt_com_get_tcpconn_simulator(USHORT link_num) {
    T_CMD_TCPCONN_NUM tcpconn;

    if (D_RRH_CPRINO_REC == link_num) {
        tcpconn = CMD_TCPCONN_NUM_SIMNO_REC;
    } else {
        tcpconn = CMD_TCPCONN_NUM_SIMNO_RE01_06 + ((link_num - 1) / 6);
    }

    return tcpconn;
}

T_CMD_TCPCONN_NUM mnt_com_get_tcpconn_monitor(USHORT link_num) {
    T_CMD_TCPCONN_NUM tcpconn;

    if (D_RRH_CPRINO_REC == link_num) {
        tcpconn = CMD_TCPCONN_NUM_MONNO_REC;
    } else{
        tcpconn = CMD_TCPCONN_NUM_MONNO_RE01_06 + ((link_num - 1) / 6);
    }

    return tcpconn;
}

USHORT mnt_com_conv_linkno_internal_to_simu(USHORT link_num) {
    USHORT after_num;

    switch (link_num) {
    case 0:
        after_num = 1;
        break;
    case 1: case 2: case 3: case 4: case 5: case 6:
        after_num = link_num;
        break;
    case 7: case 8: case 9: case 10: case 11: case 12:
        after_num = link_num - 6;
        break;
    case 13: case 14: case 15: case 16:
        after_num = link_num - 12;
        break;
    default:
        after_num = 1;
        break;
    }

    return after_num;
}

USHORT mnt_com_conv_linkno_simu_to_internal(USHORT link_num, INT linkhandle) {
    USHORT after_num;

    if (
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_REC].tcplinkhdl) ||  /* pgr0012 pgr0860 */
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_REC].tcplinkhdl) ){  /* pgr0012 pgr0860 */
        after_num = D_RRH_CPRINO_REC;
    } else if (
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE01_06].tcplinkhdl) || /* pgr0012 pgr0860 */
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE01_06].tcplinkhdl) ){ /* pgr0012 pgr0860 */
        after_num = link_num;
    } else if (
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE07_12].tcplinkhdl) || /* pgr0012 pgr0860 */
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE07_12].tcplinkhdl) ){ /* pgr0012 pgr0860 */
        after_num = D_RRH_CPRINO_RE6 + link_num;
    } else if (
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE13_16].tcplinkhdl) || /* pgr0012 pgr0860 */
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE13_16].tcplinkhdl) ){ /* pgr0012 pgr0860 */
        after_num = D_RRH_CPRINO_RE12 + link_num;
    } else {
        after_num = D_RRH_CPRINO_REC;
    }

    return after_num;
}

VOID mnt_com_conv_linkhandle_to_internalrange(INT linkhandle, UINT *link_min, UINT *link_max) {
    if (
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_REC].tcplinkhdl) || /* pgr0012 pgr0860 */
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_REC].tcplinkhdl) ){ /* pgr0012 pgr0860 */
        *link_min = D_RRH_CPRINO_REC;
        *link_max = D_RRH_CPRINO_REC;
    } else if (
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE01_06].tcplinkhdl) || /* pgr0012 pgr0860 */
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE01_06].tcplinkhdl) ){ /* pgr0012 pgr0860 */
        *link_min = D_RRH_CPRINO_RE1;
        *link_max = D_RRH_CPRINO_RE6;
    } else if (
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE07_12].tcplinkhdl) || /* pgr0012 pgr0860 */
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE07_12].tcplinkhdl) ){ /* pgr0012 pgr0860 */
        *link_min = D_RRH_CPRINO_RE7;
        *link_max = D_RRH_CPRINO_RE12;
    } else if (
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE13_16].tcplinkhdl) || /* pgr0012 pgr0860 */
    (linkhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE13_16].tcplinkhdl) ){ /* pgr0012 pgr0860 */
        *link_min = D_RRH_CPRINO_RE13;
        *link_max = D_RRH_CPRINO_RE16;
    } else {
        *link_min = D_RRH_CPRINO_REC;
        *link_max = D_RRH_CPRINO_REC;
    }
}

/** @} */

