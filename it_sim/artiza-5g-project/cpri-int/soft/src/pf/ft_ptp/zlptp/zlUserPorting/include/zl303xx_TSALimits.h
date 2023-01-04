

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Application configuration values for a running system
*
******************************************************************************/

#ifndef _ZL303XX_TSA_LIMITS_H_
#define _ZL303XX_TSA_LIMITS_H_

#ifdef __cplusplus
extern "C" {
#endif


/*****************   INCLUDES   ***********************************************/
#if defined APR_INCLUDED || defined MUX_PTP_STREAMS_TO_APR_SLOTS
#include "zl303xx_Apr.h"
#endif

/*****************   DEFINES   ************************************************/
#if defined _ZL303XX_FMC_BOARD || defined _ZL303XX_NTM_BOARD
#define NTM_MAX_PTP_CLOCKS   (1)
#define NTM_MAX_PHY_PORTS    (1)

#define NTM_MAX_PTP_PORTS    (128)

#define NTM_MAX_PTP_STREAMS  (1024)

#define NTM_MAX_APR_STREAMS  (ZL303XX_APR_MAX_NUM_MASTERS)   /* Pkt + Elec. - usually 8 */

/* Override the MAXIMUMS for config structs in startup code in the SINGLE LOCATION */
#undef ZL303XX_MAX_PHY_PORTS
#define ZL303XX_MAX_PHY_PORTS             (NTM_MAX_PHY_PORTS)

#undef ZL303XX_MAX_PTP_STREAMS_PER_PORT
#define ZL303XX_MAX_PTP_STREAMS_PER_PORT  (NTM_MAX_PTP_STREAMS)

#undef ZL303XX_MAX_PTP_PORTS_PER_CLOCK
#define ZL303XX_MAX_PTP_PORTS_PER_CLOCK   (NTM_MAX_PTP_PORTS)

#undef ZL303XX_MAX_PTP_CLOCKS
#define ZL303XX_MAX_PTP_CLOCKS            (NTM_MAX_PTP_CLOCKS)

#undef ZL303XX_MAX_EXAMPLE_PTP_CLKS
#define ZL303XX_MAX_EXAMPLE_PTP_CLKS      (NTM_MAX_PTP_CLOCKS)
#define ZL303XX_PTP_NUM_CLOCKS_MAX        (NTM_MAX_PTP_CLOCKS)

#undef ZL303XX_MAX_EXAMPLE_PTP_PORTS
#define ZL303XX_MAX_EXAMPLE_PTP_PORTS     (NTM_MAX_PTP_PORTS)
#define ZL303XX_PTP_NUM_PORTS_MAX         (NTM_MAX_PTP_PORTS)

#undef ZL303XX_PTP_NUM_STREAMS_MAX
#define ZL303XX_PTP_NUM_STREAMS_MAX       (NTM_MAX_PTP_STREAMS)

#else /* !(defined _ZL303XX_FMC_BOARD || defined _ZL303XX_NTM_BOARD) */

#if !defined ZL303XX_MAX_EXAMPLE_PTP_CLKS
#define ZL303XX_MAX_EXAMPLE_PTP_CLKS      (1)
#endif
#if !defined ZL303XX_PTP_NUM_STREAMS_MAX
#define ZL303XX_PTP_NUM_STREAMS_MAX       (128)
#endif

#define ZL303XX_MAX_PHY_PORTS             (1)
#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD 
#undef ZL303XX_MAX_PHY_PORTS
#define ZL303XX_MAX_PHY_PORTS             (4)
#endif


#define ZL303XX_MAX_PTP_STREAMS_PER_PORT  (ZL303XX_PTP_NUM_STREAMS_MAX)

#define ZL303XX_MAX_PTP_PORTS_PER_CLOCK   (ZL303XX_MAX_EXAMPLE_PTP_PORTS)

#define ZL303XX_MAX_PTP_CLOCKS            (ZL303XX_MAX_EXAMPLE_PTP_CLKS)
#define ZL303XX_PTP_NUM_CLOCKS_MAX        (ZL303XX_MAX_EXAMPLE_PTP_CLKS)

#define ZL303XX_MAX_EXAMPLE_PTP_PORTS     (ZL303XX_MAX_PHY_PORTS)

#endif



#if defined APR_INCLUDED && defined MUX_PTP_STREAMS_TO_APR_SLOTS


  #if !defined NUM_APR_SLOTS_MUX
  #define NUM_APR_SLOTS_MUX ZL303XX_APR_MAX_NUM_MASTERS /* Must be ZL303XX_APR_MAX_NUM_MASTERS or less */
  #endif

#endif  /* APR_INCLUDED && MUX_PTP_STREAMS_TO_APR_SLOTS*/


#ifdef __cplusplus
}
#endif

#endif
