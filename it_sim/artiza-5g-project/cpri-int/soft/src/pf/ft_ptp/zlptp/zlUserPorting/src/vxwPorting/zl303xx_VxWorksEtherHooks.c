

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Functions for attaching hook functions into the raw Ethernet packet.
*     This functionality is specific to the VxWorks network stack. For other OS/stacks a different approach
*     For other OS/stacks a different approach is likely to be required.
*
******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Global.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_Os.h"
#include "zl303xx_PtpTsPktStream.h"
#include "zl303xx_SocketManager.h"
#include "zl303xx_FpeApi.h"

#if defined OS_VXWORKS && (defined (ZL_USE_ETHERHOOK) || defined (ZL_USE_SNARF))

#include "zl303xx_VxWorksEtherHooks.h"

#ifdef ZL_USE_ETHERHOOK
  #include <etherLib.h>
#endif /* ZL_USE_ETHERHOOK */

#ifdef ZL_USE_SNARF
#include "zl303xx_Pkt.h"
#include "vxWorks.h"
#include "stdio.h"
#include "muxLib.h"
#include "private/muxLibP.h"
#include "net/mbuf.h"
#include "netBufLib.h"
#include "netinet/ip.h"

/*****************   STATIC GLOBAL VARIABLES   ********************************/
LOCAL void * cookieIf;
#endif  /* ZL_USE_SNARF */

/*****************   DEFINES     **********************************************/

/*****************   DATA TYPES & STRUCTURES  *********************************/

/*****************   IMPORTED GLOBAL VARIABLES    ******************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/
#ifdef ZL_USE_ETHERHOOK
_ZL303XX_LOCAL SINT_T zl303xx_EtherRxFunc( struct ifnet *pIf, char *buffer, SINT_T length);
#endif /* ZL_USE_ETHERHOOK */

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/


/*******************************************************************************
 Ethernet hook functions

 The receive timestamp is appended to the the received packet and needs to be
 extracted from the packet before entry to the protocol stack. This requires
 the raw packet to be intercepted.

 The following functions only apply to VxWorks OS and are non expected to be
 portable. Hence they are not included in the normal OS abstraction layer

 *******************************************************************************/

#ifdef ZL_USE_SNARF
/**

  Function Name:
    snarfRtn

  Details:
   Attaches a low-level hook function to be called every time an ethernet packet
   is received

  Parameters:
        (all params are predefined by the OS headers)
       pCookie      Pointer to an interface cookie for the SNARF protocol
       type         ethertype of pkt
       mbuffer      Pointer to the current (mbuf) struct in the END RBD
       pLinkHdrInfo Link layer header info
       pMode        Pointer to proto control struct - unused here

  Return Value:
   BOOLEAN - TRUE if the function handled the packet, FALSE to pass it up the stack

  Notes:

*******************************************************************************/
#ifdef __VXWORKS_65

BOOL snarfRtn (void *, long, register struct mbuf *, LL_HDR_INFO *, void *);
BOOL snarfRtn (
              void *          pCookie,
              long            type,
/*              M_BLK_ID        pMblk, */  /* Changed to *mbuf between 5.5 and 6.5 */
              register struct mbuf *mbuffer,
              LL_HDR_INFO *   pLinkHdrInfo,
              void *          pMode
          )
{
    Uint16T mLen = (Uint16T) mbuffer->mBlkHdr.mLen;
    (void) pCookie;  /* To avoid a spurious warnings */
    (void) type;
    (void) pLinkHdrInfo;
    (void) pMode;

    /* Call to save in-bound H/W timestamp info if it matches */
    if (zl303xx_FpeProcessRxPacket((Uint8T *)mbuffer->mBlkHdr.mData, mLen) == ZL303XX_OK)
    {
#ifdef ZL_RX_TS_PKTS_BYPASS_STACK
        netMblkClChainFree(mbuffer);  /* Set cluster free */
        return(TRUE);           /* We processed it */
#endif
    }

    return (FALSE);             /* packets marked FALSE will be processed by upper layers */
}

#else
    BOOL snarfRtn (
              void *          pCookie,
              long            type,
              M_BLK_ID        pMblk,   /* Changed to *mbuf between VxWorks 5.5 and 6.5 */
/*              register struct mbuf *mbuffer, */
              LL_HDR_INFO *   pLinkHdrInfo,
              void *          pMode
          )
    {
        /* #warning - SNARF for other releases not coded or tested */

        return(FALSE);
    }
#endif /* !__VXWORKS_65 */

/*

 Functions:
    snarfShutdownRtn(), stopSnarf(), snarfErrorRtn(), snarfRestartRtn(), snarfBind()

  Details:
   SNARF support functions

  Parameters:
        (all params are predefined by the OS headers)

  Return Value:
   STATUS - for restart func. All others are void

 *****************************************************************************/

STATUS snarfShutdownRtn(void* cookie, void* spare);
STATUS snarfShutdownRtn(void* cookie, void* spare)
{
    (void) cookie;  /* To avoid a spurious warnings */
    (void) spare;
    ZL303XX_TRACE_ALWAYS("SNARF detached", 0,0,0,0,0,0);
    return muxUnbind(cookie, MUX_PROTO_SNARF, snarfRtn);
}

void stopSnarf(void);
void stopSnarf(void)
{
   snarfShutdownRtn(cookieIf, NULL);
   return;
}

void snarfErrorRtn(END_OBJ* end, END_ERR* error, void* spare);
void snarfErrorRtn(END_OBJ* end, END_ERR* error, void* spare)
{
    (void) end;  /* To avoid a spurious warnings */
    (void) error;
    (void) spare;
    printf("Error returned from snarf routine\n");
    return;
}

STATUS snarfRestartRtn(void* cookie, void* spare);
STATUS snarfRestartRtn(void* cookie, void* spare)
{
    (void) cookie;  /* To avoid a spurious warnings */
    (void) spare;
    printf("Restarting snarf routine doesn't exist\n");
    return OK;
}


void snarfBind(char *ifname, SINT_T unit);
void snarfBind(char *ifname, SINT_T unit)
{
    char protocolDescr[64];

    if (muxDevExists(ifname,unit) == FALSE)
    {
        printf("snarfBind(): %s%d does not exist - cannot bind.\n", ifname, unit);
        return;
    }

    snprintf (protocolDescr, sizeof(protocolDescr), "SNARF protocol attached to %s%d", ifname, unit);

    if ((cookieIf = (END_OBJ*) muxBind (ifname,unit,
                                        snarfRtn,
                                        snarfShutdownRtn,
                                        snarfRestartRtn,
                                        snarfErrorRtn,
                                        MUX_PROTO_SNARF,
                                        protocolDescr,
                                        NULL)) == FALSE)
    {
        printf("muxBind() of snarf protocol to %s%d failed\n", ifname, unit);
        return;
    }

    ZL303XX_TRACE_ALWAYS("SNARF attached", 0,0,0,0,0,0);

    return;
 }
#endif /* ZL_USE_SNARF */


/**

  Function Name:
    zl303xx_VxAttachEtherHook

  Details:
   Attaches a low-level hook function to be called every time an ethernet packet
   is received

  Parameters:
       ifaceName   Pointer to an interface type name string, e.g. "motfcc", "eth", etc.
       ifaceNumber The instance number of the interface to which to attach

  Return Value:
   zlStatusE

  Notes:

*******************************************************************************/
zlStatusE zl303xx_VxAttachEtherHook(char * ifaceName, Uint16T ifaceNumber)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
#ifdef ZL_USE_ETHERHOOK
      /* The VxWorks 5.4 header files may cause a warning when compiling the following function call.
         This is because in that version the function is declared with an incomplete ANSI prototype.
         The compiler warning, if any, may be ignored */
      OS_STATUS osStatus = etherInputHookAdd((OS_FUNC_PTR)zl303xx_EtherRxFunc, ifaceName, ifaceNumber);

      if (osStatus != OS_OK)
      {
         ZL303XX_ERROR_NOTIFY("zl303xx_VxAttachEtherHook: failed to add EtherHook!");
         status = ZL303XX_TRANSPORT_LAYER_ERROR;
      }
      else
          ZL303XX_TRACE_ALWAYS("EtherHook attached", 0,0,0,0,0,0);
#endif /* ZL_USE_ETHERHOOK */

#ifdef ZL_USE_SNARF
      snarfBind(ifaceName, (SINT_T)ifaceNumber);
#endif /* ZL_USE_SNARF */
   }

   return status;
}

/**

  Function Name:
    zl303xx_VxDetachEtherHook

  Details:
   Detaches a previously attached low level Ethernet hook function

  Parameters:
       ifaceName   Pointer to an interface type name string, e.g. "motfcc", "eth", etc.
       ifaceNumber The instance number of the interface to which to attach

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE zl303xx_VxDetachEtherHook(char * ifaceName, Uint16T ifaceNumber)
{
   zlStatusE status = ZL303XX_OK;

   (void) ifaceName;
   (void) ifaceNumber;

   if (status == ZL303XX_OK)
   {
#ifdef ZL_USE_ETHERHOOK
      /* Delete the ethernet hook */
      /* The VxWorks 5.4 header files may cause a warning when compiling the following function call.
         This is because in that version the function is declared with an incomplete ANSI prototype.
         The compiler warning, if any, may be ignored */
      etherInputHookDelete((OS_FUNC_PTR)zl303xx_EtherRxFunc, ifaceName, ifaceNumber);
#endif /* ZL_USE_ETHERHOOK */

#ifdef ZL_USE_SNARF
      stopSnarf();
#endif /* ZL_USE_SNARF */

      ZL303XX_TRACE_ALWAYS("EtherHook detached", 0,0,0,0,0,0);
   }

   return status;
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

#ifdef ZL_USE_ETHERHOOK
/**

  Function Name:
    zl303xx_EtherRxFunc

  Details:
   A low level hook function to extract timestamps from received PTP packets

  Parameters:
       pIf         interface packet was received on
       buffer      received packet
       length      length of received packet

  Return Value:
   int

*******************************************************************************/

SINT_T zl303xx_EtherRxFunc(struct ifnet *pIf, char *buffer, SINT_T length)
{
   SINT_T rxPktHandled = (zl303xx_BooleanE)zl303xx_FpeProcessRxPacket((Uint8T *)buffer, (Uint16T)length);

   (void) pIf;
   return rxPktHandled;
}
#endif  /* ZL_USE_ETHERHOOK */

#endif /* defined OS_VXWORKS && (defined ZL_USE_ETHERHOOK || defined ZL_USE_SNARF) */
