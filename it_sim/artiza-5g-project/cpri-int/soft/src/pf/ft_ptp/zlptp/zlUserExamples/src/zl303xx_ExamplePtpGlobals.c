

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Global variables for use in examples - PTP
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"  /* This should always be the first include file */
#include "zl303xx_Trace.h"
#include "zl303xx_ExamplePtpGlobals.h"
#include "zl303xx_Ptsf.h"


#if defined ZLS30390_INCLUDED 
#include "zl303xx_PtpApiTypes.h"
#include "zl303xx_PtpSetup.h"
#endif

#include <errno.h>
#include "f_rrh_def.h"
#include "f_rrh_type.h"
#include "f_sys_def.h"
#include "f_rrh_inc.h"
#include "BPF_RU_IPCM.h"
#include "BPF_HM_DEVC.h"

#include "f_du_reg_eep.h"


/*****************   DATATYPES   **********************************************/

/*****************   DEFINES   ************************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/
extern void f_cmn_com_abort(unsigned int processID, unsigned int almCode, const void * fileName, unsigned int lineNo );

#define  f_com_abort( processID, almCode ) f_cmn_com_abort( processID, almCode, __FILE__, __LINE__)

/*****************   STATIC GLOBAL VARIABLES   ********************************/

static Uint8T zl303xx_ExampleInterfaceCustomStr[ZL303XX_MAX_EXAMPLE_INTERFACE_LENGTH] = "uninitialized"; /* For use with portNo -1 */

#if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD)
static Uint8T zl303xx_ExampleInterfaceStr[ZL303XX_MAX_EXAMPLE_INTERFACE_PORTS][ZL303XX_MAX_EXAMPLE_INTERFACE_LENGTH] = \
            { EXAMPLE_ETHERNET_INTERFACE_0,
              EXAMPLE_ETHERNET_INTERFACE_1,
              EXAMPLE_ETHERNET_INTERFACE_2,
              EXAMPLE_ETHERNET_INTERFACE_3 };
#else
/* #warning MSCC: zl303xx_ExampleInterfaceStr DEFAULTS USED */
#define ZL303XX_MAX_EXAMPLE_INTERFACE_PORTS 2
static Uint8T zl303xx_ExampleInterfaceStr[ZL303XX_MAX_EXAMPLE_INTERFACE_PORTS][ZL303XX_MAX_EXAMPLE_INTERFACE_LENGTH] = { "fhethS0","fhethS1" };
#endif



/*****************   EXPORTED GLOBAL VARIABLES   ******************************/
#if defined ZLS30390_INCLUDED
zl303xx_PtpProfileE zl303xx_PtpProfile = ZL303XX_PTP_PROFILE_DEFAULT;
#endif

#if defined ZLS30383_INCLUDED || defined ZLS30393_INCLUDED
/* Use eth1 by default on ZLS30393 */
#define EXAMPLE_DEFAULT_INTERFACE_NUM 1
#endif

#if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD )
/* User-set transport mode (unicast or multicast) */
zl303xx_BooleanE settingIsUnicast = ZL303XX_TRUE;

Sint32T exampleUseInterfaceNo = EXAMPLE_DEFAULT_INTERFACE_NUM;



#else
/* PTP default */
Sint32T exampleUseInterfaceNo = -1;
#endif

/* The address to use when creating the clock stream and the handle returned from the create stream function */
examplePtpStreamS exampleMultiPtpStream = {(zl303xx_PtpStreamHandleT)ZL303XX_INVALID, DEFAULT_PTP_DOMAIN_ADDRESS_IPV4, "224.0.0.1"};
examplePtpStreamS exampleUniPtpStream = {(zl303xx_PtpStreamHandleT)ZL303XX_INVALID, "10.0.0.234", "10.0.0.1"};
examplePtpStreamS exampleMultiPtpStream2 = {(zl303xx_PtpStreamHandleT)ZL303XX_INVALID,  {0}, {0}};
examplePtpStreamS examplePdelayPtpStream = {(zl303xx_PtpStreamHandleT)ZL303XX_INVALID, DEFAULT_MULTICAST_IP_PDELAY, "224.0.0.127"};
examplePtpStreamS exampleUniPtpStream2 = {(zl303xx_PtpStreamHandleT)ZL303XX_INVALID, {0}, {0}};

examplePtpStreamS exampleIPv6PtpStream = {(zl303xx_PtpStreamHandleT)ZL303XX_INVALID, {0}, {0}};
examplePtpStreamS exampleIPv6UniPtpStream   = {(zl303xx_PtpStreamHandleT)ZL303XX_INVALID, {0}, {0}};


#if defined ZLS30390_INCLUDED 
/* Configurable PTP clock parameters. */
static zl303xx_BooleanE examplePtpTwoStepFlag = ZL303XX_PTP_DEFAULT_CLOCK_TWO_STEP;   /* FALSE */
static zl303xx_PtpClockEgressQlUpdateE exampleEgressClassUpdateTrigger = ZL303XX_EGRESS_QL_UPDATE_ON_QUALIFY;

/* exampleGetTwoStepFlag */
/**
   Allows the customer to get the PTP twoStepFlag global variable used in this
   example code.

*******************************************************************************/
zl303xx_BooleanE exampleGetTwoStepFlag(void)
{
    return examplePtpTwoStepFlag;
}

/* exampleSetTwoStepFlag */
/**
   Allows the customer to set the PTP twoStepFlag global variable used in this
   example code.

*******************************************************************************/
zlStatusE exampleSetTwoStepFlag(zl303xx_BooleanE twoStep)
{
    examplePtpTwoStepFlag = twoStep;
    return ZL303XX_OK;
}

/* exampleGetEgressClassUpdateTrigger */
/**
   Allows the customer to get the PTP egressClassUpdateTrigger global variable used in this
   example code.

*******************************************************************************/
zl303xx_PtpClockEgressQlUpdateE exampleGetEgressClassUpdateTrigger(void)
{
    return exampleEgressClassUpdateTrigger;
}

/* exampleSetEgressClassUpdateTrigger */
/**
   Allows the customer to set the PTP egressClassUpdateTrigger global variable used in this
   example code.

*******************************************************************************/
zlStatusE exampleSetEgressClassUpdateTrigger(zl303xx_PtpClockEgressQlUpdateE updateTrigger)
{
    exampleEgressClassUpdateTrigger = updateTrigger;
    return ZL303XX_OK;
}
#endif



/* RAW SOCKET protocol addresses. */
/* MAC addresses are needed for all raw socket examples. */
Uint8T exampleMaster1MacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN]   = {0x00, 0xA0, 0x87, 0x00, 0x12, 0x05};
Uint8T exampleMaster2MacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN]   = {0x00, 0xA0, 0x87, 0x80, 0xE0, 0xA4};
Uint8T exampleSlave1MacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN]    = {0x00, 0xA0, 0x87, 0x00, 0x11, 0x06};
Uint8T exampleSlave2MacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN]    = {0x00, 0xA0, 0x87, 0x80, 0xE0, 0xA4};
/* PTP PeerDelay Annex F MAC */
Uint8T examplePeerDelayMultiMacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN]            = {0x01, 0x1B, 0x19, 0x00, 0x00, 0x00};
Uint8T examplePeerDelayMultiMacAddrNonRoutable[ZL303XX_PKT_HDR_ETH2_ADDR_LEN] = {0x01, 0x80, 0xC2, 0x00, 0x00, 0x0E};

/* Examples use 2 PTP ports sharing the same physical interface. */
Uint8T exampleBoundary1MacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN] = {0x00, 0xA0, 0x87, 0x80, 0xDC, 0x0F};
Uint8T exampleBoundary2MacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN] = {0x00, 0xA0, 0x87, 0x80, 0xDC, 0x0F};

Uint8T exampleBlockMacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN]    = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
Uint8T exampleZeroMacAddr[ZL303XX_PKT_HDR_ETH2_ADDR_LEN]     = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* IPv6 addresses are needed for raw socket examples with an IPv6 header. */
Uint8T exampleMaster1IPv6Addr[ZL303XX_PKT_HDR_IPV6_ADDR_LEN] =
                      {0xFD, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
Uint8T exampleMaster2IPv6Addr[ZL303XX_PKT_HDR_IPV6_ADDR_LEN] =
                      {0xFD, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02};

Uint8T exampleSlave1IPv6Addr[ZL303XX_PKT_HDR_IPV6_ADDR_LEN] =
                      {0xFD, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01};
Uint8T exampleSlave2IPv6Addr[ZL303XX_PKT_HDR_IPV6_ADDR_LEN] =
                      {0xFD, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02};

Uint8T exampleBoundary1IPv6Addr[ZL303XX_PKT_HDR_IPV6_ADDR_LEN] =
                      {0xFD, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01};
Uint8T exampleBoundary2IPv6Addr[ZL303XX_PKT_HDR_IPV6_ADDR_LEN] =
                      {0xFD, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02};

Uint8T const *exampleDestMacAddr = NULL;

Uint8T exampleCookieMagic[ZL303XX_PKT_HDR_COOKIE_MAGIC_LEN] = {
          0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF};


#if defined ZLS30390_INCLUDED
zl303xx_BooleanE zl303xx_UseDelayResponse = ZL303XX_TRUE;
zl303xx_BooleanE zl303xx_UsePeerDelayResponse = ZL303XX_FALSE;
zl303xx_BooleanE exampleUseAnnexF = ZL303XX_FALSE;
zl303xx_BooleanE exampleUsePeerToPeer = ZL303XX_FALSE;
Uint16T Zl303xx_ClockType = ZL303XX_PTP_DEFAULT_CLOCK_TYPE; /* From zl303xx_PtpSetup.h */

/* zl303xx_SetPtpProfile */
/**
   Sets the PTP Profile that will be used for the next created PTP clock.

  Parameters:
   [in]   profile   PTP profile to use.

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetPtpProfile(zl303xx_PtpProfileE profile)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      if (profile >= ZL303XX_PTP_PROFILE_NUM_TYPES)
      {
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
      zl303xx_PtpProfile = profile;
   }

   return status;
}

/* zl303xx_SetPtpProfile */
/**
   Returns the current PTP Profile.

  Parameters:
   [in]  None

  Return Value:   zl303xx_PtpProfileE

*******************************************************************************/
zl303xx_PtpProfileE zl303xx_GetPtpProfile(void)
{
   return zl303xx_PtpProfile;
}

/* zl303xx_SetUseDelayResponseFlag */
/**
   Set the Use Delay Response flag

  Parameters:
   [in]   useDelayResponse

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetUseDelayResponseFlag(zl303xx_BooleanE useDelayResponse)
{
   zl303xx_UseDelayResponse = useDelayResponse;

   /* Both Delay-Response and Peer-Delay cannot be on at the same time.
    * However, turning off E2E does not automatically turn on P2P.   */
   if (zl303xx_UseDelayResponse == ZL303XX_TRUE)
   {
      zl303xx_UsePeerDelayResponse = ZL303XX_FALSE;
   }

   return ZL303XX_OK;
}

/* zl303xx_GetUseDelayResponseFlag */
/**
   Get the Use Delay Response flag

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetUseDelayResponseFlag(void)
{
   return zl303xx_UseDelayResponse;
}

/* zl303xx_SetUsePeerDelayResponseFlag */
/**
   Set the Use Peer-Delay Response flag

  Parameters:
   [in]   usePeerDelayResponse

  Return Value:  zlStatusE

*******************************************************************************/
zlStatusE zl303xx_SetUsePeerDelayResponseFlag(zl303xx_BooleanE usePeerDelayResponse)
{
   zl303xx_UsePeerDelayResponse = usePeerDelayResponse;

   /* Both Delay-Response and Peer-Delay cannot be on at the same time.
    * However, turning off E2E does not automatically turn on P2P.   */
   /* Also see exampleSetUsePeerToPeer() */
   if (zl303xx_UsePeerDelayResponse == ZL303XX_TRUE)
   {
      zl303xx_UseDelayResponse = ZL303XX_FALSE;
   }

   return ZL303XX_OK;
}

/* zl303xx_GetUsePeerDelayResponseFlag */
/**
   Get the Use Peer-Delay Response flag

  Parameters:
      None

  Return Value:  zl303xx_BooleanE

*******************************************************************************/
zl303xx_BooleanE zl303xx_GetUsePeerDelayResponseFlag(void)
{
   return zl303xx_UsePeerDelayResponse;
}

/** exampleSetUseAnnexF

*   The function sets the global for using ANNEX_F Eth2 Mac
*   address for PeerToPeer (PeerDelay)

  Return Value: zlStatusE

****************************************************************************/
zlStatusE exampleSetUseAnnexF(zl303xx_BooleanE enabled)
{
    exampleUseAnnexF = enabled;

    return ZL303XX_OK;
}

/** exampleGetUseAnnexF

*   The function gets the global for using ANNEX_F Eth2 Mac
*   address for PeerToPeer (PeerDelay)

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE exampleGetUseAnnexF(void)
{
    return exampleUseAnnexF;
}

/** exampleSetUsePeerToPeer

*   The function sets the global for using Peer to Peer
*   timestamp format for PeerDelay

  Return Value: zlStatusE

****************************************************************************/
zlStatusE exampleSetUsePeerToPeer(zl303xx_BooleanE enabled)
{
   zlStatusE status;
   /* Set the PTP Profile to Peer-Delay. */
    if (enabled)
    {
        /* Set the Peer-Delay mechanism*/
        status = zl303xx_SetUsePeerDelayResponseFlag(ZL303XX_TRUE);
    }
    else
    {
        /* Set the Peer-Delay mechanism */
        status = zl303xx_SetUsePeerDelayResponseFlag(ZL303XX_FALSE);
    }
    return status;
}

/** exampleGetUsePeerToPeer

*   The function gets the global for using Peer to Peer
*   timestamp format for PeerDelay

  Return Value: zl303xx_BooleanE

****************************************************************************/
zl303xx_BooleanE exampleGetUsePeerToPeer(void)
{
   return zl303xx_GetUsePeerDelayResponseFlag();
}

/** zl303xx_GetClockType

*   Function to get the clockType parameter used when initializing
*   and creating a PTP Clock.

  Return Value: Uint16T

****************************************************************************/
Uint16T zl303xx_GetClockType(void)
{
   return Zl303xx_ClockType;
}

/** zl303xx_SetClockType

*   Function to set the clockType parameter used when initializing
*   and creating a PTP Clock.

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_SetClockType(Uint16T clockType)
{
   Zl303xx_ClockType = clockType;
   return ZL303XX_OK;
}

#endif

/* zl303xx_SetExampleInterface

   The function sets the ethernet interface to use for the given PTP port number
   portNo -1 is the custom interface (see zl303xx_SetExampleInterfaceCustomStr)

   Ex:
        zl303xx_SetExampleInterface(0);    // Use eth1 for PTP port 0

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_SetExampleInterface(Uint32T portNo)
{
    if (portNo != (Uint32T)-1 && portNo >= ZL303XX_MAX_EXAMPLE_INTERFACE_PORTS)
        return ZL303XX_PARAMETER_INVALID;

    exampleUseInterfaceNo = portNo;

    return ZL303XX_OK;
}

/* zl303xx_SetExampleInterfaceCustomStr

   The function sets the custom ethernet interface to use for custom interfaceNo (-1)

   Ex: Use br0 interface
        zl303xx_SetExampleInterfaceCustomStr("br0");
        zl303xx_SetExampleInterface(-1);

  Return Value: zlStatusE

****************************************************************************/
zlStatusE zl303xx_SetExampleInterfaceCustomStr(const char *interfaceStr)
{
    strncpy((char*)zl303xx_ExampleInterfaceCustomStr, interfaceStr, sizeof(zl303xx_ExampleInterfaceCustomStr));
    zl303xx_ExampleInterfaceCustomStr[sizeof(zl303xx_ExampleInterfaceCustomStr)-1] = '\0';
    return ZL303XX_OK;
}

/* zl303xx_GetExampleInterface

   The function gets the ethernet interface in use

  Return Value: Uint32T exampleUseInterfaceNo

****************************************************************************/
Uint32T zl303xx_GetExampleInterface(void)
{
	/* 0固定を返していた箇所を共有メモリから取得するように変更 */
	/* 共有メモリは0:No use 1:Port0 2:Port1を示す */
	T_RRH_PTPCONF_MNG		*f_ptp_tbl_adr_getexinf;	/*  PTP状態テーブル	*/
	Sint32T					endcd, errcd;				/*  返り値 */


	if(exampleUseInterfaceNo != -1) {

		/* ActPort確定済み	*/
		return exampleUseInterfaceNo;
	}

	/* ActPort未確定	*/
	endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_PTP_CONF,			/* 共有メモリ番号				*/
		(VOID **)&f_ptp_tbl_adr_getexinf,	/* 共有メモリアドレス			*/
		&errcd								/* 詳細NGコード					*/
	);
	/* 共有メモリの取得失敗時 */
	if( endcd != BPF_RU_IPCM_OK )
	{
		ZL303XX_TRACE_ALWAYS( "zl303xx_GetExampleInterface() [Error] shared mem get NG.NG:%x ERRCD:%x", 
							  endcd, errcd, 0, 0, 0, 0 );
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_FNC); 
		return 0;
	}

	if( f_ptp_tbl_adr_getexinf->actport == 0 ) {

		/* Act Port未確定	*/
		ZL303XX_TRACE_ALWAYS( "zl303xx_GetExampleInterface() [Error] Unsettled ActPort : %d", 
							  f_ptp_tbl_adr_getexinf->actport, 0, 0, 0, 0, 0 );
		return 0;

	} else if( f_ptp_tbl_adr_getexinf->actport > 2 ) {

		/* Act Port設定値異常	*/
		ZL303XX_TRACE_ALWAYS( "zl303xx_GetExampleInterface() [Error] Invalid value - ActPort No : %d", 
							  f_ptp_tbl_adr_getexinf->actport, 0, 0, 0, 0, 0 );
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_DNG);
		return 0;

	} else {

		exampleUseInterfaceNo = f_ptp_tbl_adr_getexinf->actport - 1;
	}
	return exampleUseInterfaceNo;
}

/** zl303xx_GetExampleInterfaceStr

   The function gets the ethernet interface to use for the given PTP port number

  Return Value: char *

****************************************************************************/
char * zl303xx_GetExampleInterfaceStr(Uint32T portNo)
{
    if ((Sint32T)portNo == -1)
    {
        /* Port -1 is custom string, see zl303xx_SetExampleInterfaceCustomStr */
        return (char*)zl303xx_ExampleInterfaceCustomStr;
    }
    else if (portNo > ZL303XX_MAX_EXAMPLE_INTERFACE_PORTS)
    {
        ZL303XX_ERROR_NOTIFY("zl303xx_GetExampleInterfaceStr() - returning NULL string!");
        return NULL;
    }

    return (char*)zl303xx_ExampleInterfaceStr[portNo];
}

/* examplePtsfDefaultCallback */
/** 

   Example PTSF Callback: currently just logs the change.

  Parameters:
   [in]   pEvent  PTSF change event data.

  Return Value:  N/A

*******************************************************************************/
void examplePtsfDefaultCallback(zl303xx_PtsfEventS *pEvent);
void examplePtsfDefaultCallback(zl303xx_PtsfEventS *pEvent)
{
   const char *ptsfStr = zl303xx_PtsfFlagToStr(pEvent->flag);

   ZL303XX_TRACE(ZL303XX_MOD_ID_PTSF, 2,
         "Stream %lu: Flag[%d] (%s) changed to %d.",
         pEvent->streamHandle,
         pEvent->flag, ptsfStr,
         pEvent->value, 0,0);
}


/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/*****************   END   ****************************************************/
