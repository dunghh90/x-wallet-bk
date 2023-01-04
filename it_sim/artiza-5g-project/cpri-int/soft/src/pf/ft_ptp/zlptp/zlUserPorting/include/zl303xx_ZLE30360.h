

/******************************************************************************
*
*  $Id: zl303xx_ZLE30360.h 15782 2017-09-13 19:42:21Z DP $
*
*  Copyright 2006-2017 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Functions used by the ZLE30360 evaluation board
*
******************************************************************************/

#ifndef _ZL303XX_ZLE30360_H_
#define _ZL303XX_ZLE30360_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#if defined ZLS30390_INCLUDED
#include "zl303xx_PtpApi.h"
#endif
/*****************   DEFINES     **********************************************/

/*****************   DATA TYPES   *********************************************/

typedef enum {
    ZLE30360_PHY_PORT_A = 0,
    ZLE30360_PHY_PORT_B = 1,
    ZLE30360_PHY_PORT_C = 2,
    ZLE30360_PHY_PORT_D = 3
} ZLE30360_PhyPortE;

/*****************   DATA STRUCTURES   ****************************************/

typedef struct {
    zl303xx_BooleanE      initialized;

    Uint32T             clockFreq;                  /* See vtss_phy_ts_clockfreq_t */
    zl303xx_BooleanE      warmStart;
    zl303xx_BooleanE      twoStep;
    Uint32T             delayMechanism;             /* See vtss_phy_ts_ptp_delaym_type_t */
    zl303xx_BooleanE      vlanEnabled;
    Uint16T             tcMode;                     /* 0: off (BC mode), 1: TC-modeA, 2: TC-modeB */
    zl303xx_BooleanE      ipv6;                       /* Enable PTP over IPv6 timestamping */
    zl303xx_BooleanE      eth2;                       /* Enable PTP over ETH2 timestamping */
    zl303xx_BooleanE      useTsFifo;                  /* Use egress TS fifo */
    zl303xx_BooleanE      recoverClk1Enabled;
    zl303xx_BooleanE      recoverClk2Enabled;
    zl303xx_BooleanE      one_step_txfifo;
    ZLE30360_PhyPortE   recoverClk1Port;
    ZLE30360_PhyPortE   recoverClk2Port;

    Uint32T             traceLevelCfg;
    Uint32T             traceLevelRun;

    struct {
        zl303xx_BooleanE  enableTS;
        zl303xx_BooleanE  phy1gTimingAnegMaster;
        zl303xx_BooleanE  phy1gTimingAnegForced;
    } port[4];

} zl303xx_VTSS_config_t;

/*****************   EXPORTED GLOBAL DECLARATIONS   *************************/
#if defined ZLE30360_FIFOLESS_INCLUDED
extern zl303xx_BooleanE bUseFifolessMode;
#endif

extern zl303xx_VTSS_config_t zl303xx_VTSS_config;

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/

zlStatusE zl303xx_VTSS_config_struct_init(zl303xx_VTSS_config_t *config);
zlStatusE zl303xx_VTSS_configure(zl303xx_VTSS_config_t *config);
zlStatusE zl303xx_VTSS_set_clock_freq(zl303xx_VTSS_config_t *config);
zlStatusE zl303xx_VTSS_shutdown(void);
zlStatusE zl303xx_VTSS_get_time(Uint64S *sec, Uint32T *nsec);
zlStatusE zl303xx_VTSS_set_time(Uint64S seconds, Uint32T nanoseconds);
zlStatusE zl303xx_VTSS_step_time(Sint32T nanosec);
zlStatusE zl303xx_VTSS_wait_for_1pps_window(Uint32T min_window_ms, Uint32T timeout_ms);
zlStatusE zl303xx_VTSS_sync_to_ext_pulse(Uint32T timeout_ms);
zlStatusE zl303xx_VTSS_print_debug(Sint32T port_no);
zlStatusE zl303xx_VTSS_reg_read(Uint32T port_no, Uint32T page, Uint16T addr, Uint16T *value);
zlStatusE zl303xx_VTSS_reg_write(Uint32T port_no, Uint32T page, Uint16T addr, Uint16T value);
zlStatusE zl303xx_VTSS_reg_read1588(Uint32T port_no, Uint32T blk_id, Uint16T addr, Uint32T *value);
zlStatusE zl303xx_VTSS_reg_write1588(Uint32T port_no, Uint32T blk_id, Uint16T addr, Uint32T value);
zlStatusE zl303xx_VTSS_reg_dump(void);
zlStatusE zl303xx_VTSS_get_time_with_step(Uint64S *sec, Uint32T *nsec, Uint64S *baseSec, Uint32T *baseNsec, Sint32T step_ns);

Uint32T zl303xx_VTSS_miim_lock(const Uint8T * function);
Uint32T zl303xx_VTSS_miim_unlock(const Uint8T * function);

#if defined ZLS30390_INCLUDED
zlStatusE ZLE30360_PtpRxMsgProcess(Uint8T *buffer, Uint16T bufferLen,
                                   zl303xx_PtpRxMsgDataS *pRxMsgData);
zlStatusE ZLE30360_PtpPortRxMsgProcess(Uint8T *buffer, Uint16T bufferLen,
                                   zl303xx_PtpPortRxMsgDataS *pRxMsgData);
zlStatusE ZLE30360_PtpTxMsgProcess(Uint8T *buffer, Uint16T bufferLen,
                                   zl303xx_PtpTxMsgDataS *pTxMsgData);
#endif

#ifdef __cplusplus
}
#endif

#endif

