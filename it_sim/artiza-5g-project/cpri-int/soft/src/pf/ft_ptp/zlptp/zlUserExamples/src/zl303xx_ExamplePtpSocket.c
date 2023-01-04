

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Example socket abstraction. Create/manage sockets and simple socket
*     receive task
*
*******************************************************************************/


/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Error.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Os.h"
#include "zl303xx_PtpApi.h"
#include "zl303xx_ExamplePtpSocket.h"
#include "zl303xx_ExamplePtp.h"
#include "zl303xx_Macros.h"
#include "zl303xx_OsHeaders.h"
#include "zl303xx_RdWr.h"

#include "zl303xx_ExamplePtpGlobals.h"
#include "zl303xx_Dpll77x.h"

#ifdef OS_VXWORKS
#include <sockLib.h>
typedef SINT_T SOCKLEN_T;  /* The fromlen parameter of recvfrom() is not consistent
                            * across all OS's. */
#endif

#ifdef OS_LINUX
#if defined ZL_LNX_DENX || defined ZL_LNX_CODESOURCERY || defined ZL_LNX_INTEL
//#define __USE_MISC            /* TODO: Should define _BSD_SOURCE instead at higher level, see features.h */
#endif
#include <net/if.h>           /* for ifreq (may need __USE_MISC from _BSD_SOURCE) */
#ifdef ZL_LNX_CODESOURCERY
//#define __USE_MISC
#ifndef ntohl
#include <netinet/in.h>
#endif
/* This was supposed to give us this; it didn't!!!! */
/* MSCC TODO: may need to define _BSD_SOURCE */
//struct ip_mreq
//  {
    /* IP multicast address of group.  */
//    struct in_addr imr_multiaddr;

    /* Local IP address of interface.  */
//    struct in_addr imr_interface;
//  };
#endif
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/ioctl.h>
typedef socklen_t SOCKLEN_T;

#ifndef ZL_LNX_CODESOURCERY
#include <linux/sockios.h>
#endif
/* #include <netpacket/packet.h> */
#endif

#if defined OS_LINUX && defined OHLY_TIMESTAMPS && defined LINUX_3
#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_Pkt.h"
#include "zl303xx_LinuxOhlyTS.h"
#include "zl303xx_ExamplePtpGlobals.h"
#endif


#include <arpa/inet.h> /* For inet_pton and inet_ntop */
#if defined ZL_INCLUDE_ETH2_SOCKET
#include <linux/if_packet.h>
#include <net/ethernet.h>
#endif

#include <errno.h>
#include "f_rrh_def.h"
#include "f_rrh_type.h"
#include "f_sys_def.h"
#include "f_rrh_inc.h"
#include "BPF_RU_IPCM.h"
#include "BPF_HM_DEVC.h"
#include "BPF_COM_CMD.h"
#include "f_du_reg_eep.h"

/*****************   DEFINES     **********************************************/

/* RX task parameters */
#define EXAMPLE_PTP_RX_TASK_NAME    "zlPtpRxTask"
#define EXAMPLE_PTP_RX_TASK_PRI     59
#define EXAMPLE_PTP_RX_TASK_OPTS    0
#define EXAMPLE_PTP_RX_TASK_STACK   10000
#define EXAMPLE_PTP_RX_MSG_Q_SIZE   16
#define EXAMPLE_SOCKET_SELECT_TIMEOUT_MSEC   500  /* Must be < 1000 */
#define EXAMPLE_PTP_RX_TASK_TIMEOUT_MSEC     (EXAMPLE_SOCKET_SELECT_TIMEOUT_MSEC * 10)

/* Memory usage parameters */
#define EXAMPLE_PTP_RX_BUFFER_LENGTH         1600

/*****************   STATIC DATA TYPES   **************************************/

/* This completes the opaque examplePtpSocketS. */
/* .hに定義移動 start */
/*
typedef struct
{
   Sint32T fd;
   Uint16T udpPort;
   Sint32T ifindex;
   char    ifname[32];
} socketS;

typedef struct examplePtpSocketS
{
   socketS socket[ZL303XX_PTP_INTERFACE_NUM_TYPES];
} examplePtpSocketS;
*/
/* .hに定義移動 end */

/* Message types that can be sent to the RX task. */
typedef enum
{
   EXAMPLE_PTP_SOCKET_NO_MSG = 0,
   EXAMPLE_PTP_SOCKET_ADD,
   EXAMPLE_PTP_SOCKET_DELETE,
   EXAMPLE_PTP_SOCKET_TASK_STOP
} examplePtpSocketMsgE;

/* Task message structure */
typedef struct
{
   examplePtpSocketMsgE  type;
   SINT_T                fd;
} examplePtpSocketMsgS;



/*****************   STATIC FUNCTION DECLARATIONS   ***************************/
static zlStatusE examplePtpSocketAlloc(examplePtpSocketS **ppPtpSkt);
void      examplePtpSocketFree(examplePtpSocketS **ppPtpSkt);
static zlStatusE examplePtpSocketOpen(zl303xx_PortAddress  *pSrcAddr,
                                      zl303xx_PortAddress  *pMcastAddr,
                                      examplePtpSocketS **ppPtpSkt);

static zlStatusE examplePtpSocketConfig(zl303xx_PortAddress *pSrcAddr,
                                        zl303xx_PortAddress *pMcastAddr,
                                        socketS *skt);
static zlStatusE examplePtpSocketConfigIPv4(zl303xx_PortAddress *pSrcAddr,
                                            zl303xx_PortAddress *pMcastAddr,
                                            socketS *skt);
static zlStatusE examplePtpSocketConfigIPv6(zl303xx_PortAddress *pSrcAddr,
                                            zl303xx_PortAddress *pMcastAddr,
                                            socketS *skt);
static zlStatusE examplePtpSocketConfig802p3(zl303xx_PortAddress *pSrcAddr,
                                             zl303xx_PortAddress *pMcastAddr,
                                             socketS *skt);

static zlStatusE examplePtpRxMsgQSend(examplePtpSocketMsgS *pSktMsg);
static zlStatusE examplePtpRxMsgQRead(examplePtpSocketMsgS *pSktMsg);

static zlStatusE examplePtpSocketTblAdd(zl303xx_PortAddress *pSrcAddr,
                                        zl303xx_PortAddress *pMcastAddr,
                                        socketS *skt);

static void examplePtpSocketTblRem(SINT_T fd);
static void examplePtpSocketTblRemAll(void);
static void examplePtpSocketRecvFrom(examplePtpSocketTblS *pTblEntry);

static Sint32T examplePtpRxTask(void);

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/* Globals used by the RX task */
static OS_TASK_ID ExamplePtpRxTaskId = OS_TASK_INVALID;
static OS_MUTEX_ID ExamplePtpRxMutex = OS_MUTEX_INVALID;
static OS_MSG_Q_ID ExamplePtpRxMsgQ = OS_MSG_Q_INVALID;

examplePtpSocketTblS ExamplePtpSocketTbl[EXAMPLE_PTP_SOCKETS_MAX];
static Uint8T *ExampleRxBuffer = NULL;

extern zl303xx_ParamsS *zl303xx_Params0;
zl303xx_BooleanE onceflg = ZL303XX_FALSE;				/* 受信ポート決定フラグ */



/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* examplePtpSocketEnvInit */
/**
   Initializes the socket environment and the global variables used in
   this example code.

*******************************************************************************/
zlStatusE examplePtpSocketEnvInit(void)
{
   zlStatusE status = ZL303XX_OK;

   /* Check if the task or mutex has already been created. */
   if (status == ZL303XX_OK)
   {
      if (ExamplePtpRxTaskId != OS_TASK_INVALID)
      {
         ZL303XX_TRACE_ALWAYS("Example PTP RX task already created.", 0,0,0,0,0,0);
         status = ZL303XX_INIT_ERROR;
      }
      if (ExamplePtpRxMutex != OS_MUTEX_INVALID)
      {
         ZL303XX_TRACE_ALWAYS("Example PTP RX mutex already created.", 0,0,0,0,0,0);
         status = ZL303XX_INIT_ERROR;
      }
      if (ExamplePtpRxMsgQ != OS_MSG_Q_INVALID)
      {
         ZL303XX_TRACE_ALWAYS("Example PTP RX message queue already created.", 0,0,0,0,0,0);
         status = ZL303XX_INIT_ERROR;
      }
   }

   /* Initialize the socket table and the RX buffer. */
   if (status == ZL303XX_OK)
   {
      memset(ExamplePtpSocketTbl, 0, sizeof(ExamplePtpSocketTbl));

      ExampleRxBuffer = OS_CALLOC(1, EXAMPLE_PTP_RX_BUFFER_LENGTH);

      if (ExampleRxBuffer == NULL)
      {
         ZL303XX_ERROR_TRAP("Error allocating RX buffer memory.");
         status = ZL303XX_RTOS_MEMORY_FAIL;
      }
   }

   /* Create the RX mutex. */
   if (status == ZL303XX_OK)
   {
      ExamplePtpRxMutex = OS_MUTEX_CREATE();

      if (ExamplePtpRxMutex == OS_MUTEX_INVALID)
      {
         ZL303XX_TRACE_ALWAYS("Error creating example PTP RX mutex.", 0,0,0,0,0,0);
         status = ZL303XX_RTOS_MUTEX_CREATE_FAIL;
      }
   }

   /* Create the RX task message queue. */
   if (status == ZL303XX_OK)
   {
      ExamplePtpRxMsgQ = OS_MSG_Q_CREATE(EXAMPLE_PTP_RX_MSG_Q_SIZE, sizeof(examplePtpSocketMsgS));

      if (ExamplePtpRxMsgQ == OS_MSG_Q_INVALID)
      {
         ZL303XX_TRACE_ALWAYS("Error creating example PTP RX message queue.", 0,0,0,0,0,0);
         status = ZL303XX_RTOS_MSGQ_CREATE_FAIL;
      }
   }

   /* Create the RX task. */
   if (status == ZL303XX_OK)
   {
      ExamplePtpRxTaskId = OS_TASK_SPAWN(EXAMPLE_PTP_RX_TASK_NAME,
                                         EXAMPLE_PTP_RX_TASK_PRI,
                                         EXAMPLE_PTP_RX_TASK_OPTS,
                                         EXAMPLE_PTP_RX_TASK_STACK,
                                         (OS_TASK_FUNC_PTR)examplePtpRxTask,
                                         0);

      if (ExamplePtpRxTaskId == OS_TASK_INVALID)
      {
         ZL303XX_TRACE_ALWAYS("Error starting %s.",
                              EXAMPLE_PTP_RX_TASK_NAME, 0,0,0,0,0);
         status = ZL303XX_RTOS_TASK_CREATE_FAIL;
      }
   }

   return ZL303XX_OK;
}

/* examplePtpSocketEnvClose */
/**
   Closes the PTP environment and resets global variables used in this
   example code, as necessary.

*******************************************************************************/
zlStatusE examplePtpSocketEnvClose(void)
{
   zlStatusE status = ZL303XX_OK, mStatus = ZL303XX_RTOS_MUTEX_TAKE_FAIL;
   examplePtpSocketMsgS sktMsg;

   /* Send the task a message to stop. This will break the task loop so the task
    * doesn't get deleted while in the middle of a socket select(). */
   if (status == ZL303XX_OK)
   {
      sktMsg.type = EXAMPLE_PTP_SOCKET_TASK_STOP;

      status = examplePtpRxMsgQSend(&sktMsg);
      OS_TASK_DELAY(500);
   }

   /* Take the RX mutex. */
   if (status == ZL303XX_OK && ExamplePtpRxMutex != OS_MUTEX_INVALID)
   {
      if (OS_MUTEX_TAKE(ExamplePtpRxMutex) == OS_OK)
      {
         mStatus = ZL303XX_OK;
      }
      else
      {
         ZL303XX_TRACE_ALWAYS("examplePtpSocketEnvClose: Error taking example PTP RX mutex.", 0,0,0,0,0,0);
      }

      status = mStatus;
   }

   /* Delete the RX task. */
   if (status == ZL303XX_OK && ExamplePtpRxTaskId != OS_TASK_INVALID)
   {
      if (OS_TASK_DELETE(ExamplePtpRxTaskId) == OS_OK)
      {
         ExamplePtpRxTaskId = OS_TASK_INVALID;
      }
      else
      {
         ZL303XX_TRACE_ALWAYS("examplePtpSocketEnvClose: Error deleting example PTP RX task.", 0,0,0,0,0,0);
         status = ZL303XX_RTOS_TASK_DELETE_FAIL;
      }
   }

   /* Delete the RX task message queue. */
   if (status == ZL303XX_OK && ExamplePtpRxMsgQ != OS_MUTEX_INVALID)
   {
      if (OS_MSG_Q_DELETE(ExamplePtpRxMsgQ) == OS_OK)
      {
         ExamplePtpRxMsgQ = OS_MSG_Q_INVALID;
      }
      else
      {
         ZL303XX_TRACE_ALWAYS("examplePtpSocketEnvClose: Error deleting example PTP RX message queue.", 0,0,0,0,0,0);
         status = ZL303XX_RTOS_MSGQ_DELETE_FAIL;
      }
   }

   /* If we currently have the RX mutex... */
   if (mStatus == ZL303XX_OK)
   {
      /* Free the RX buffer. */
      OS_FREE(ExampleRxBuffer);
      ExampleRxBuffer = NULL;

      /* Delete the RX mutex. */
      if (status == ZL303XX_OK)
      {
         if (OS_MUTEX_DELETE(ExamplePtpRxMutex) == OS_OK)
         {
            ExamplePtpRxMutex = OS_MUTEX_INVALID;
         }
         else
         {
            ZL303XX_TRACE_ALWAYS("examplePtpSocketEnvClose: Error deleting example PTP RX mutex.", 0,0,0,0,0,0);
            status = ZL303XX_RTOS_MUTEX_DELETE_FAIL;
         }
      }
   }

   return status;
}


/* examplePtpSocketPortHandleSet */
/**
   Assigns portHandle to socket (for receive backreference and egress timestamp).

   Returns ZL303XX_OK on success.

*******************************************************************************/
zlStatusE examplePtpSocketPortHandleSet(examplePtpSocketT pPtpSktT, Sint32T portHandle)
{
   zlStatusE status = ZL303XX_TABLE_EMPTY;
   Uint32T count = 0;

   for (count = 0; count < EXAMPLE_PTP_SOCKETS_MAX; count++)
   {
      if (ExamplePtpSocketTbl[count].fd == pPtpSktT->socket[ZL303XX_PTP_INTERFACE_EVENT].fd ||
          ExamplePtpSocketTbl[count].fd == pPtpSktT->socket[ZL303XX_PTP_INTERFACE_GENERAL].fd)
      {
          ZL303XX_TRACE_ALWAYS("examplePtpSocketPortHandleSet: Assigned portHandle %d to socket %d (old handle %d)",
                        portHandle, ExamplePtpSocketTbl[count].fd, ExamplePtpSocketTbl[count].portHandle,0,0,0);

          ExamplePtpSocketTbl[count].portHandle = portHandle;

          status = ZL303XX_OK;
          /* No break */
      }
   }

   return status;
}

/* examplePtpSocketStreamHandleSet */
/**
   Assigns streamHandle to socket (for receive backreference and egress timestamp).

   Returns ZL303XX_OK on success.

*******************************************************************************/
zlStatusE examplePtpSocketStreamHandleSet(examplePtpSocketT pPtpSktT, Sint32T streamHandle)
{
   zlStatusE status = ZL303XX_TABLE_EMPTY;
   Uint32T count = 0;

   for (count = 0; count < EXAMPLE_PTP_SOCKETS_MAX; count++)
   {
      if (ExamplePtpSocketTbl[count].fd == pPtpSktT->socket[ZL303XX_PTP_INTERFACE_EVENT].fd ||
          ExamplePtpSocketTbl[count].fd == pPtpSktT->socket[ZL303XX_PTP_INTERFACE_GENERAL].fd)
      {
          ZL303XX_TRACE_ALWAYS("examplePtpSocketStreamHandleSet: Assigned streamHandle %d to socket %d (old handle %d)",
                        streamHandle, ExamplePtpSocketTbl[count].fd, ExamplePtpSocketTbl[count].streamHandle,0,0,0);

          ExamplePtpSocketTbl[count].streamHandle = streamHandle;

          status = ZL303XX_OK;
          /* No break */
      }
   }

   return status;
}

/* examplePtpSocketUcastOpen */
/**
   Opens two unicast sockets and binds them to UDP ports 319 and 320.

*******************************************************************************/
zlStatusE examplePtpSocketUcastOpen(zl303xx_PortAddress *pLocalAddr,
                                    examplePtpSocketT *pPtpSktT)
{
   return examplePtpSocketOpen(pLocalAddr, NULL, pPtpSktT);
}

/* examplePtpSocketMcastOpen */
/**
   Opens two multicast sockets and binds them to UDP ports 319 and 320.

*******************************************************************************/
zlStatusE examplePtpSocketMcastOpen(zl303xx_PortAddress *pLocalAddr,
                                    zl303xx_PortAddress *pMcastAddr,
                                    examplePtpSocketT *pPtpSktT)
{
   return examplePtpSocketOpen(pLocalAddr, pMcastAddr, pPtpSktT);
}

/* examplePtpSocketClose */
/**
   Closes the previously opened sockets and frees memory used by the structure.

*******************************************************************************/
zlStatusE examplePtpSocketClose(examplePtpSocketT *pPtpSktT)
{
   zlStatusE status = ZL303XX_OK;
   examplePtpSocketS **ppPtpSkt = pPtpSktT;
   zl303xx_PtpInterfaceE interface;
   examplePtpSocketMsgS sktMsg;

   if (status == ZL303XX_OK)
   {
      /* If the socket does not exist, return early (not an error). */
      if (*ppPtpSkt == NULL)
      {
         return status;
      }

      /* Send the task a message to remove this socket from the table. */
      for (interface = ZL303XX_PTP_INTERFACE_EVENT;
           (status == ZL303XX_OK) && (interface < ZL303XX_PTP_INTERFACE_NUM_TYPES);
           interface++)
      {
         sktMsg.type = EXAMPLE_PTP_SOCKET_DELETE;
         sktMsg.fd = (*ppPtpSkt)->socket[interface].fd;

         status = examplePtpRxMsgQSend(&sktMsg);
      }
   }

   /* Free the memory used by this structure. */
   if (status == ZL303XX_OK)
   {
      examplePtpSocketFree(ppPtpSkt);
   }

   return status;
}

/* examplePtpSocketTx */
/**
   Sends a PTP message to a previously created socket.

*******************************************************************************/
zlStatusE examplePtpSocketTx(examplePtpSocketS *pPtpSkt,
                             Uint8T *buffer, Uint16T bufferLen,
                             zl303xx_PortAddress *pDestAddr, zl303xx_PtpInterfaceE interface)
{
   zlStatusE status = ZL303XX_OK;
   Sint32T bytes = -1;

   ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 5, "examplePtpSocketTx: fd %d bufLen %u if %u buf[0] %x",
        pPtpSkt->socket[interface].fd,
        bufferLen,
        interface, buffer[0],0,0);

   if (status == ZL303XX_OK)
   {
      /* Convert the PTP PortAddress and interface to a sockaddr_in structure. */
      /* Send the message to the appropriate socket. */
      switch (pDestAddr->networkProtocol)
      {
        case ZL303XX_NETWORK_PROTOCOL_UDP_IPv4:
        {
              struct sockaddr_in destAddr;
              memset(&destAddr, 0, sizeof(destAddr));
              destAddr.sin_family = AF_INET;
              destAddr.sin_port = htons(pPtpSkt->socket[interface].udpPort);
              memcpy(&destAddr.sin_addr.s_addr, pDestAddr->addressField, sizeof(destAddr.sin_addr.s_addr));

              bytes = sendto(pPtpSkt->socket[interface].fd, (char *)buffer, (SINT_T)bufferLen, 0,
                     (struct sockaddr *)(void *)&destAddr, sizeof(destAddr));
              break;
        }

        case ZL303XX_NETWORK_PROTOCOL_UDP_IPv6:
#if defined ZL_INCLUDE_IPV6_SOCKET
        {
              struct sockaddr_in6 destAddr;
              memset(&destAddr, 0, sizeof(destAddr));
              destAddr.sin6_family = AF_INET6;
              destAddr.sin6_port = htons(pPtpSkt->socket[interface].udpPort);
              memcpy(&destAddr.sin6_addr, pDestAddr->addressField, pDestAddr->addressLength);

              bytes = sendto(pPtpSkt->socket[interface].fd, (char *)buffer, (SINT_T)bufferLen, 0,
                     (struct sockaddr *)(void *)&destAddr, sizeof(destAddr));
              break;
        }
#endif
        case ZL303XX_NETWORK_PROTOCOL_802_3:
#if defined ZL_INCLUDE_ETH2_SOCKET
        {
            struct sockaddr_ll destAddr;
            memset(&destAddr, 0, sizeof(destAddr));
            memcpy(destAddr.sll_addr, pDestAddr->addressField, ETHER_ADDR_LEN);
            destAddr.sll_halen    = ETHER_ADDR_LEN;
            destAddr.sll_family   = AF_PACKET;
            destAddr.sll_ifindex  = pPtpSkt->socket[interface].ifindex;
            destAddr.sll_protocol = htons(0x88F7);

            bytes = sendto(pPtpSkt->socket[interface].fd, (char *)buffer, (SINT_T)bufferLen, 0, (struct sockaddr*)&destAddr, sizeof(destAddr));
            break;
        }
#endif
        case ZL303XX_NETWORK_PROTOCOL_DEVICENET:
        case ZL303XX_NETWORK_PROTOCOL_CONTROLNET:
        case ZL303XX_NETWORK_PROTOCOL_PROFINET:
        case ZL303XX_NETWORK_PROTOCOL_UNKNOWN:
        default:
            ZL303XX_TRACE_ALWAYS("examplePtpSocketTx: Unsupported network protocol %d", pDestAddr->networkProtocol, 0,0,0,0,0);
            status = ZL303XX_UNSUPPORTED_OPERATION;
      }

      if (status == ZL303XX_OK && bytes == -1)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 1,
                       "socket write failed. (errno=%d)",
                       errno, 0,0,0,0,0);

         ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1,
                       "socket write failed. (errno=%d)",
                       errno, 0,0,0,0,0);

         status = ZL303XX_ERROR;
      }
   }

   return status;
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/* examplePtpSocketAlloc */
/**
   Allocates and initializes memory for an examplePtpSocketS structure and
   returns a pointer to it.

*******************************************************************************/
static zlStatusE examplePtpSocketAlloc(examplePtpSocketS **ppPtpSkt)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpInterfaceE interface;

   /* Allocate memory for the structure. */
   if (status == ZL303XX_OK)
   {
      *ppPtpSkt = OS_CALLOC(1, sizeof(**ppPtpSkt));

      if (*ppPtpSkt == NULL)
      {
         ZL303XX_TRACE_ALWAYS("Memory allocate for port socket structure failed.", 0,0,0,0,0,0);
         status = ZL303XX_RTOS_MEMORY_FAIL;
      }
      else
          ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 2, "examplePtpSocketAlloc: @ %p", *ppPtpSkt, 0,0,0,0,0);
   }


   /* Initialize the structure. */
   if (status == ZL303XX_OK)
   {
      for (interface = ZL303XX_PTP_INTERFACE_EVENT;
           interface < ZL303XX_PTP_INTERFACE_NUM_TYPES;
           interface++)
      {
         (*ppPtpSkt)->socket[interface].fd = -1;
         (*ppPtpSkt)->socket[interface].udpPort = (interface == ZL303XX_PTP_INTERFACE_EVENT)
                                                     ? DEFAULT_PTP_UDP_EVENT_PORT
                                                     : DEFAULT_PTP_UDP_GENERAL_PORT;
      }
   }

   return status;
}

/* examplePtpSocketFree */
/**
   Frees memory for an examplePtpSocketS structure and sets the pointer to NULL.

*******************************************************************************/
void examplePtpSocketFree(examplePtpSocketS **ppPtpSkt)
{
    if (ppPtpSkt && *ppPtpSkt)
    {
        ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 2, "examplePtpSocketFree: *ppPtpSkt=%p", *ppPtpSkt, 0,0,0,0,0);

        OS_FREE(*ppPtpSkt);
        *ppPtpSkt = NULL;
    }
}

/* examplePtpSocketOpen */
/**
   Opens two sockets and binds them to UDP ports 319 and 320.

*******************************************************************************/
static zlStatusE examplePtpSocketOpen(zl303xx_PortAddress  *pSrcAddr,
                                      zl303xx_PortAddress  *pMcastAddr,
                                      examplePtpSocketS **ppPtpSkt)
{
   zlStatusE status = ZL303XX_OK;
   zl303xx_PtpInterfaceE interface;
   struct in_addr in_addr_mcast = {0};

   /* Check input parameters. */
   if (status == ZL303XX_OK)
   {
      if (pSrcAddr->networkProtocol != ZL303XX_NETWORK_PROTOCOL_UDP_IPv4 &&
#if defined ZL_INCLUDE_IPV6_SOCKET
          pSrcAddr->networkProtocol != ZL303XX_NETWORK_PROTOCOL_UDP_IPv6 &&
#endif
#if defined ZL_INCLUDE_ETH2_SOCKET
          pSrcAddr->networkProtocol != ZL303XX_NETWORK_PROTOCOL_802_3 &&
#endif
          1) /* To simplify ifdefs above */
      {
         ZL303XX_TRACE_ALWAYS("examplePtpSocketOpen: Unsupported PTP network protocol=%lu",
                              pSrcAddr->networkProtocol, 0,0,0,0,0);
         status = ZL303XX_PARAMETER_INVALID;
      }
   }

   if (status == ZL303XX_OK)
   {
/* #warning MSCC: TODO Check valid multicast address for each supported protocol type */
      /* Convert the input zl303xx_PortAddress structures to in_addr structures. */
      if (pMcastAddr && pMcastAddr->networkProtocol == ZL303XX_NETWORK_PROTOCOL_UDP_IPv4)
      {
         memcpy(&in_addr_mcast.s_addr, pMcastAddr->addressField, sizeof(in_addr_mcast.s_addr));

         if (__BYTE_ORDER == __LITTLE_ENDIAN)
         {
             Uint32T tmpAddr = 0;

             tmpAddr =  pMcastAddr->addressField[0] <<24;
             tmpAddr |= pMcastAddr->addressField[1] <<16;
             tmpAddr |= pMcastAddr->addressField[2] <<8;
             tmpAddr |= pMcastAddr->addressField[3];

             /* Check if a valid multicast address was supplied. */
             if (!IN_MULTICAST(tmpAddr))
             {
                ZL303XX_TRACE_ALWAYS("examplePtpSocketOpen: Multicast address=0x%08lX is not valid.",
                                     tmpAddr, 0,0,0,0,0);
                status = ZL303XX_PARAMETER_INVALID;
             }
         }
         else
         {
             /* Check if a valid multicast address was supplied. */
             if (!IN_MULTICAST(in_addr_mcast.s_addr))
             {
                ZL303XX_TRACE_ALWAYS("examplePtpSocketOpen: Multicast address=0x%08lX is not valid.",
                                     in_addr_mcast.s_addr, 0,0,0,0,0);
                status = ZL303XX_PARAMETER_INVALID;
             }
         }

      }
   }

   /* Allocate memory for the socket structure. */
   if (status == ZL303XX_OK)
   {
      if ((status = examplePtpSocketAlloc(ppPtpSkt)) != ZL303XX_OK)
      {
          ZL303XX_TRACE_ALWAYS("examplePtpSocketOpen: examplePtpSocketAlloc failed=%d.",
                               status, 0,0,0,0,0);
      }
   }

   if (status == ZL303XX_OK)
   {
       /* Create a new socket for this port. */
       for (interface = ZL303XX_PTP_INTERFACE_EVENT;
            (status == ZL303XX_OK) && (interface < ZL303XX_PTP_INTERFACE_NUM_TYPES);
            interface++)
       {
           Sint32T tblIndex = 0;

          /* Add this socket to the internal table. The socket will be opened inside
           * this function. */
           socketS *pSkt = &((*ppPtpSkt)->socket[interface]);
#if defined ZL_INCLUDE_ETH2_SOCKET
           if (pSrcAddr->networkProtocol == ZL303XX_NETWORK_PROTOCOL_802_3 && interface != ZL303XX_PTP_INTERFACE_EVENT)
           {
               /* Re-use ZL303XX_PTP_INTERFACE_EVENT socket for all other interfaces since 802.3 does not have unique interfaces/ports */
               memcpy(pSkt, &((*ppPtpSkt)->socket[ZL303XX_PTP_INTERFACE_EVENT]), sizeof(socketS));
           }
           else
#endif
           {
                status = examplePtpSocketTblAdd(pSrcAddr,
                                          pMcastAddr,
                                          pSkt);
           }
         (void) tblIndex; /* Unused. Warning removal */
        }    /* for */
   }

   return status;
}

/* examplePtpSocketTblAdd */
/**
   Adds an entries to the socket table used by the RX task. Fills the
   examplePtpSocketS structure if the entry already exists.

*******************************************************************************/
static zlStatusE examplePtpSocketTblAdd(zl303xx_PortAddress *pSrcAddr,
                                        zl303xx_PortAddress *pMcastAddr,
                                        socketS *skt)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T count, empty;
   examplePtpSocketTblS *pTblEntry;

   if (status == ZL303XX_OK)
   {
      /* Search for a matching entry in the table. */
      for (count = 0, empty = EXAMPLE_PTP_SOCKETS_MAX;
           count < EXAMPLE_PTP_SOCKETS_MAX;
           count++)
      {
         pTblEntry = &ExamplePtpSocketTbl[count];

         /* Grab the first empty entry. */
         if ((empty > count) && (pTblEntry->active == 0))
         {
            empty = count;
         }
         /* Check for a match. */
#if !defined OHLY_TIMESTAMPS    /* Ohly timestamping can have duplicate sockets for egress timestamp association */
         /* TODO: Note more than one LL sockets (ETH2) with same srcAddr may cause duplicate packets to be received on some systems */
         else if ((pTblEntry->active > 0) &&
                  (memcmp(pSrcAddr, &pTblEntry->srcAddr, sizeof(*pSrcAddr)) == 0) &&
                  (pSrcAddr->networkProtocol == ZL303XX_NETWORK_PROTOCOL_802_3 || skt->udpPort == pTblEntry->udpPort) && /* 802.3 has no udpPorts so allow duplicates */
                  (  (pMcastAddr == NULL) ||
                     (memcmp(pMcastAddr, &pTblEntry->mcastAddr, sizeof(*pMcastAddr)) == 0)
                 ))
         {
            ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 1, "examplePtpSocketTblAdd: Match index %d fd %d active %d",
                count, pTblEntry->fd, pTblEntry->active,0,0,0);
            break;
         }
#endif
      } /* for */

      /* No matching entry... */
      if (count >= EXAMPLE_PTP_SOCKETS_MAX)
      {
         /* ...but an empty entry is available. Add to table. */
         if (empty < EXAMPLE_PTP_SOCKETS_MAX)
         {
            pTblEntry = &ExamplePtpSocketTbl[empty];
            memset(pTblEntry, 0, sizeof(*pTblEntry));

            pTblEntry->udpPort = skt->udpPort;

            pTblEntry->portHandle = -1;
            pTblEntry->streamHandle = -1;


            memcpy(&pTblEntry->srcAddr, pSrcAddr, sizeof(pTblEntry->srcAddr));
            if (pMcastAddr != NULL)
            {
               memcpy(&pTblEntry->mcastAddr, pMcastAddr, sizeof(pTblEntry->mcastAddr));
            }
            else
            {
                /* Flag that multicast is not used */
                memset(&pTblEntry->mcastAddr, 0, sizeof(pTblEntry->mcastAddr));
            }

/*            printf("examplePtpSocketTblAdd: emptySlot=%d Port=%d  pTblEntry->srcAddr= 0x%x\n", empty, udpPort, (Uint32T)((Uint32T*)&pTblEntry->srcAddr));
            printf("examplePtpSocketTblAdd: pTblEntry->mcastAddr= 0x%x\n", pTblEntry->mcastAddr); */

            /* Open a socket and return the file descriptor. */
            switch (pSrcAddr->networkProtocol)
            {
                case ZL303XX_NETWORK_PROTOCOL_UDP_IPv4:
                    pTblEntry->fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
                    break;

                case ZL303XX_NETWORK_PROTOCOL_UDP_IPv6:
#if defined ZL_INCLUDE_IPV6_SOCKET
                    pTblEntry->fd = socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP);
                    break;
#endif
                case ZL303XX_NETWORK_PROTOCOL_802_3:
#if defined ZL_INCLUDE_ETH2_SOCKET
                    pTblEntry->fd = socket(PF_PACKET, SOCK_DGRAM, htons(0x88F7));
                    break;
#endif
                case ZL303XX_NETWORK_PROTOCOL_DEVICENET:
                case ZL303XX_NETWORK_PROTOCOL_CONTROLNET:
                case ZL303XX_NETWORK_PROTOCOL_PROFINET:
                case ZL303XX_NETWORK_PROTOCOL_UNKNOWN:
                default:
                    ZL303XX_TRACE_ALWAYS("examplePtpSocketTblAdd: Unsupported network protocol %d",
                        pSrcAddr->networkProtocol,0,0,0,0,0);
                    pTblEntry->fd = -1;
            }

            if (pTblEntry->fd != -1)
            {
               skt->fd = pTblEntry->fd;

               /* Configure the socket. */
               status = examplePtpSocketConfig(pSrcAddr, pMcastAddr, skt);

               /* Save the socket interface if there is one (802.3) */
               pTblEntry->ifindex = skt->ifindex;
            }
            else
            {
               ZL303XX_TRACE_ALWAYS("Error opening socket. (errno=%d)", errno, 0,0,0,0,0);
               status = ZL303XX_INIT_ERROR;
            }

            /* printf("UNI: TblAdd pTblEntry->fd=0x%x = %d\n", &pTblEntry->fd, pTblEntry->fd); */
         count = empty; /* current index */
         } /* empty */
         /* ...and no empty entry. Return table full error. */
         else
         {
            ZL303XX_TRACE_ALWAYS("Maximum number of sockets reached.", 0,0,0,0,0,0);
            status = ZL303XX_TABLE_FULL;
         }
     }    /* count */
      else
      {
          /* Matching entry found, copy the information to skt */
          pTblEntry = &ExamplePtpSocketTbl[count];

          skt->fd = pTblEntry->fd;
          skt->ifindex = pTblEntry->ifindex;
      }

      if (status == ZL303XX_OK)
      {
         /* If status is OK, there is a matching entry or a newly created entry.
          * Increment the active counter and return the file descriptor for this socket. */
          pTblEntry->active++;
      }
   }

   return status;
}

/* examplePtpSocketTblRem */
/**
   Removes an entry in the socket table used by the RX task.

*******************************************************************************/
static void examplePtpSocketTblRem(SINT_T fd)
{
   Uint32T count;

   /* Search the table for a matching entry. */
   for (count = 0; count < EXAMPLE_PTP_SOCKETS_MAX; count++)
   {
      if ((ExamplePtpSocketTbl[count].active > 0) &&
          (ExamplePtpSocketTbl[count].fd == fd))
      {
         /* Decrement the active counter, and close the socket if it is no
          * longer in use. */
         if ((--ExamplePtpSocketTbl[count].active) == 0)
         {
            close(ExamplePtpSocketTbl[count].fd);
         }
      }
   }
}

/* examplePtpSocketTblRemAll */
/**
   Removes all entries in the socket table used by the RX task.

*******************************************************************************/
static void examplePtpSocketTblRemAll(void)
{
   Uint32T count;

   for (count = 0; count < EXAMPLE_PTP_SOCKETS_MAX; count++)
   {
      if (ExamplePtpSocketTbl[count].active > 0)
      {
         ExamplePtpSocketTbl[count].active = 0;
         close(ExamplePtpSocketTbl[count].fd);
      }
   }
}

/* examplePtpSocketTblPrint */
/**
   Prints the socket table information

*******************************************************************************/
void examplePtpSocketTblPrint(void)
{
   Uint32T count;

   ZL303XX_TRACE_ALWAYS("examplePtpSocketTblPrint:", 0,0,0,0,0,0);

   /* Print the table */
   for (count = 0; count < EXAMPLE_PTP_SOCKETS_MAX; count++)
   {
      examplePtpSocketTblS *entry = &ExamplePtpSocketTbl[count];

      if (entry->active)
      {
          ZL303XX_TRACE_ALWAYS("%02u. fd %d active %d udp %d port %d stream %d",
                count, entry->fd, entry->active, entry->udpPort, entry->portHandle, entry->streamHandle);

          {
              char srcAddrStr[64] = "<empty>";
              char mcastAddrStr[64] = "<empty>";
              examplePtpSocketPortAddressToStr(&entry->srcAddr, srcAddrStr, sizeof(srcAddrStr));
              if (entry->mcastAddr.addressLength != 0)
              {
                examplePtpSocketPortAddressToStr(&entry->mcastAddr, mcastAddrStr, sizeof(mcastAddrStr));
              }
              ZL303XX_TRACE_ALWAYS("      srcAddr %s mcastAddr %s", srcAddrStr, mcastAddrStr, 0,0,0,0);
          }
      }
   }
}

/* examplePtpSocketConfig */
/**
   Sets up all of the socket options then binds it to an address/port.

*******************************************************************************/
static zlStatusE examplePtpSocketConfig(zl303xx_PortAddress *pSrcAddr,
                                        zl303xx_PortAddress *pMcastAddr,
                                        socketS *skt)
{
    zlStatusE status = ZL303XX_OK;
    char *ifName;

    /* 昔は0固定だったがマルチポート対応により変更 */
    /* char *ifName = zl303xx_GetExampleInterfaceStr(zl303xx_GetExampleInterface()); */
    
    /* EEPROM */
    int i;
    unsigned short	addr0 = D_DU_EEP_MAC_ADDR_PORT1_H1;			/* Port0 0x0220 */
    unsigned short	addr1 = D_DU_EEP_MAC_ADDR_PORT2_H1;			/* Port1 0x0260 */
    unsigned char	data = 0;
    unsigned int	cfg_err = 0 ;
    
    zl303xx_PortAddress tmpAddr0;
    zl303xx_PortAddress tmpAddr1;
    
    for( i = 0; i < 6; i++, addr0++ , addr1++ ) {

     	BPF_HM_DEVC_EEPROM_READ( addr0, &data );
     	tmpAddr0.addressField[i] = data;
     	BPF_HM_DEVC_EEPROM_READ( addr1, &data );
     	tmpAddr1.addressField[i] = data;
    }

	ZL303XX_TRACE_ALWAYS("examplePtpSocketConfig() srcAddr: %02x:%02x:%02x:%02x:%02x:%02x",
							pSrcAddr->addressField[0],pSrcAddr->addressField[1],
							pSrcAddr->addressField[2],pSrcAddr->addressField[3],
							pSrcAddr->addressField[4],pSrcAddr->addressField[5]);
	ZL303XX_TRACE_ALWAYS("examplePtpSocketConfig() tmpAddr0: %02x:%02x:%02x:%02x:%02x:%02x",
							tmpAddr0.addressField[0],tmpAddr0.addressField[1],
							tmpAddr0.addressField[2],tmpAddr0.addressField[3],
							tmpAddr0.addressField[4],tmpAddr0.addressField[5]);
	ZL303XX_TRACE_ALWAYS("examplePtpSocketConfig() tmpAddr1: %02x:%02x:%02x:%02x:%02x:%02x",
							tmpAddr1.addressField[0],tmpAddr1.addressField[1],
							tmpAddr1.addressField[2],tmpAddr1.addressField[3],
							tmpAddr1.addressField[4],tmpAddr1.addressField[5]);
	cfg_err = 0 ;
	for( i = 0 ; i < 6 ;i++)
	{
		/* Port0ルートの判定 */
		if (pSrcAddr->addressField[i] != tmpAddr0.addressField[i])
		{
			cfg_err = 1 ;
			break;
		}
	}
	if( cfg_err == 0 )
	{
		/* Port0ルート */
		ifName = zl303xx_GetExampleInterfaceStr(0);
	}
	else
	{
		cfg_err = 0 ;
		for( i = 0 ; i < 6 ; i++)
		{
			/* Port1ルートの判定 */
			if(pSrcAddr->addressField[i] != tmpAddr1.addressField[i])
			{
				/* 異常処理 */
				cfg_err = 2 ;
				break ;
			}
		}
		if ( cfg_err == 0 )
		{
			/* Port1のルート */
			ifName = zl303xx_GetExampleInterfaceStr(1);
		}
		else
		{
			/* 異常ルートだが暫定でPort0に */
			ZL303XX_TRACE_ALWAYS("examplePtpSocketConfig() EEPROM Read value NG",0, 0, 0,0,0,0);
			ifName = zl303xx_GetExampleInterfaceStr(0);
		}
	}
    /* Obtain ifindex of desired network interface (note: uses globals) */
    if (status == ZL303XX_OK)
    {
        struct ifreq ifr;

        memset(&ifr, 0, sizeof(struct ifreq));
        strncpy(ifr.ifr_name, ifName, IFNAMSIZ-1);

        if(skt->fd == -1 || (ioctl(skt->fd, SIOCGIFINDEX, &ifr)) == -1)
        {
            ZL303XX_TRACE_ALWAYS("Unable to obtain ifindex (errno=%ld) ifName %s fd %d",
                                     errno, ifName, skt->fd,0,0,0);
            status = ZL303XX_INIT_ERROR;
        }
        else
        {
            skt->ifindex = ifr.ifr_ifindex;
            strncpy(skt->ifname, ifName, sizeof(skt->ifname));
            skt->ifname[sizeof(skt->ifname)-1] = '\0';
        }
    }

    if (status == ZL303XX_OK)
    {
        char srcAddrStr[64];
        char mcastAddrStr[64];

        (void) examplePtpSocketPortAddressToStr(pSrcAddr, srcAddrStr, sizeof(srcAddrStr));
        (void) examplePtpSocketPortAddressToStr(pMcastAddr, mcastAddrStr, sizeof(mcastAddrStr));

        ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 1,
            "examplePtpSocketConfig: ifname %s ifindex %d udpPort %d srcAddr %s mcastAddr %s",
            skt->ifname, skt->ifindex, skt->udpPort, srcAddrStr, mcastAddrStr, 0);
    }

    if (status == ZL303XX_OK)
    {
        switch (pSrcAddr->networkProtocol)
        {
            case ZL303XX_NETWORK_PROTOCOL_UDP_IPv4:
                status = examplePtpSocketConfigIPv4(pSrcAddr, pMcastAddr, skt);
                break;
            case ZL303XX_NETWORK_PROTOCOL_UDP_IPv6:
                status = examplePtpSocketConfigIPv6(pSrcAddr, pMcastAddr, skt);
                break;
            case ZL303XX_NETWORK_PROTOCOL_802_3:
                status = examplePtpSocketConfig802p3(pSrcAddr, pMcastAddr, skt);
                break;

            case ZL303XX_NETWORK_PROTOCOL_DEVICENET:
            case ZL303XX_NETWORK_PROTOCOL_CONTROLNET:
            case ZL303XX_NETWORK_PROTOCOL_PROFINET:
            case ZL303XX_NETWORK_PROTOCOL_UNKNOWN:
            default:
                status = ZL303XX_UNSUPPORTED_OPERATION;
        }

        if (status == ZL303XX_UNSUPPORTED_OPERATION)
        {
            ZL303XX_TRACE_ALWAYS("examplePtpSocketConfig: Unsupported protocol %d", pSrcAddr->networkProtocol,0,0,0,0,0);
        }
    }

    return status;
 }

/* examplePtpSocketConfigIpv4 */
/**
   Sets up all of the socket options then binds it to an IPv4 address/UDP port.

*******************************************************************************/
static zlStatusE examplePtpSocketConfigIPv4(zl303xx_PortAddress *pSrcAddr,
                                        zl303xx_PortAddress *pMcastAddr,
                                        socketS *skt)
{
    zlStatusE status = ZL303XX_OK;
    examplePtpSocketMsgS sktMsg;
    Uint16T udpPort = skt->udpPort;
    Sint32T fd = skt->fd;
    SINT_T ifindex = skt->ifindex;
    char *ifname = skt->ifname;

    struct sockaddr_in srcAddr;
    struct sockaddr_in mcastAddr;

    memset(&srcAddr, 0, sizeof(srcAddr));
    if (pSrcAddr)
    {
        srcAddr.sin_family = AF_INET;
        srcAddr.sin_port = htons(udpPort);
        memcpy(&srcAddr.sin_addr, pSrcAddr->addressField, pSrcAddr->addressLength);
    }

    memset(&mcastAddr, 0, sizeof(mcastAddr));
    if (pMcastAddr)
    {
        mcastAddr.sin_family = AF_INET;
        mcastAddr.sin_port = htons(udpPort);
        memcpy(&mcastAddr.sin_addr, pMcastAddr->addressField, pMcastAddr->addressLength);
    }

    /* printf("examplePtpSocketConfig: pSrcAddr= 0x%x\n", pSrcAddr); */

   /* Allow this address to be reused. */
   if (status == ZL303XX_OK)
   {
      SINT_T reuseAddr = 1;

      if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
                     (char *)&reuseAddr, sizeof(reuseAddr)) == -1)
      {
         ZL303XX_TRACE_ALWAYS("Unable to configure socket for address reuse. (errno=%d); udpPort=%d",
                              errno, udpPort ,0,0,0,0);
         status = ZL303XX_INIT_ERROR;
      }
   }

   /* Force no UDP checksum. */
   if (status == ZL303XX_OK)
   {
      SINT_T noCS = 1;

      if (setsockopt(fd, SOL_SOCKET, SO_NO_CHECK,
                     (char *)&noCS, sizeof(noCS)) == -1)
      {
         ZL303XX_TRACE_ALWAYS("Unable to configure socket for no checksum. (errno=%d); udpPort=%d",
                              errno, udpPort ,0,0,0,0);
         status = ZL303XX_INIT_ERROR;
      }
   }


   /* Configure socket as non-blocking. */
   if (status == ZL303XX_OK)
   {
      SINT_T onOff = 1;

      if (ioctl(fd, FIONBIO, (SnativeT)&onOff) == -1)
      {
         ZL303XX_TRACE_ALWAYS("Unable to configure socket as non-blocking. (errno=%d); udpPort=%d",
                              errno, udpPort, 0,0,0,0);
         status = ZL303XX_INIT_ERROR;
      }
   }

   /* Enable this socket for multicast transmits. We use the unicast socket for all transmits,
    * including multicasts, because we want to use the correct local source IP address. */
   if (status == ZL303XX_OK)
   {
      if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF,
                     (char *)&srcAddr.sin_addr, sizeof(srcAddr.sin_addr)) == -1)
      {
         ZL303XX_TRACE_ALWAYS("Unable to configure socket for multicast TX. (errno=%ld); udpPort=%d pSrcAddr=0x%x",
                              errno, udpPort, srcAddr.sin_addr.s_addr ,0,0,0);
         status = ZL303XX_INIT_ERROR;
      }
   }

   /* If this is an event socket (port 319), set the DSCP field in the IPv4
    * header to Expedited Forwarding (0x2E). See IEEE 1588-2008 Annex D.5 for
    * more details. */
   if (status == ZL303XX_OK)
   {
      if (udpPort == DEFAULT_PTP_UDP_EVENT_PORT)
      {
         /* DSCP is stored in bits 7:2 of byte 1 in the IPv4 header. */
         SINT_T ip_tos = (0x2E << 2) & 0xFC;

         if (setsockopt(fd, IPPROTO_IP, IP_TOS, (char *)&ip_tos, sizeof(ip_tos)) == -1)
         {
            ZL303XX_TRACE_ALWAYS("Unable to configure socket DSCP. (errno=%d)",
                                 errno, 0,0,0,0,0);
            status = ZL303XX_INIT_ERROR;
         }
      }
   }

   if (status == ZL303XX_OK)
   {
      if (pMcastAddr != NULL)
      {
          /* printf("examplePtpSocketConfig: pMcastAddr= 0x%x\n", pMcastAddr); */
         /* Add socket as a multicast receiver using the specified interface.
          * The multicast address will be the same as the transmit address. */
         struct ip_mreq mreq;
         memset(&mreq, 0, sizeof(mreq));

         memcpy(&mreq.imr_multiaddr, &mcastAddr.sin_addr, sizeof(mreq.imr_multiaddr));
         memcpy(&mreq.imr_interface, &srcAddr.sin_addr,   sizeof(mreq.imr_interface));
         if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                        (char *)&mreq, sizeof(mreq)) == -1)
         {
            ZL303XX_TRACE_ALWAYS("Unable to add to multicast receive group. (errno=%ld)",
                                 errno, 0,0,0,0,0);
            status = ZL303XX_INIT_ERROR;
         }
      }
   }

    /*
     * We don't want multicasted data on the loopback...
     */
     {
         Uint8T enabled = 0;
        if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP,
                       &enabled, sizeof(enabled)) == -1)
        {
           ZL303XX_TRACE_ALWAYS("Unable to configure socket for multicast Loopback. (errno=%ld); udpPort=%d",
                                errno, udpPort, 0,0,0,0);
           status = ZL303XX_INIT_ERROR;
        }
     }

#if defined OS_LINUX && defined OHLY_TIMESTAMPS && defined LINUX_3
    if ((status = zl303xx_ConfigOhlyHWTimestamping(zl303xx_GetExampleInterfaceStr(zl303xx_GetExampleInterface()), fd)) != ZL303XX_OK)
    {
        ZL303XX_TRACE_ALWAYS("Unable to set Ohly HW timestamping. (status %d, errno=%d)", status, errno,0,0,0,0);
    }
#endif

   /* Bind the socket. */
   if (status == ZL303XX_OK)
   {
      struct sockaddr_in socketAddr;
      memset(&socketAddr, 0, sizeof(socketAddr));

      socketAddr.sin_family = AF_INET;
      socketAddr.sin_port = htons(udpPort);

      if (pMcastAddr != NULL)
      {
         memcpy(&socketAddr.sin_addr, &mcastAddr.sin_addr, sizeof(socketAddr.sin_addr));
      }
      else
      {
         memcpy(&socketAddr.sin_addr, &srcAddr.sin_addr, sizeof(socketAddr.sin_addr));
      }

      if (bind(fd, (struct sockaddr *)(void *)&socketAddr, sizeof(socketAddr)) == -1)
      {
         char addrStr[64];
         examplePtpSocketPortAddressToStr(pMcastAddr ? pMcastAddr : pSrcAddr, addrStr, sizeof(addrStr));

         ZL303XX_TRACE_ALWAYS("examplePtpSocketConfigIPv4: Unable to bind socket. (errno=%d %s) address %s ifindex %d (%s)",
            errno, (void*)strerror(errno), addrStr, ifindex, ifname,0);
         status = ZL303XX_INIT_ERROR;
      }
      else
      {
          ZL303XX_TRACE_ALWAYS("SUCCESS: bind socket to udpPort=%d", udpPort, 0,0,0,0,0);
      }

   }

   /* Send a message to the RX task to update the fd_set. */
   if (status == ZL303XX_OK)
   {
      sktMsg.type = EXAMPLE_PTP_SOCKET_ADD;
      sktMsg.fd = fd;

      status = examplePtpRxMsgQSend(&sktMsg);
      if (status != ZL303XX_OK)
      {
          ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "examplePtpSocketConfig: examplePtpRxMsgQSend() failed=%lu", status, 0,0,0,0,0);
          ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1, "examplePtpSocketConfig: examplePtpRxMsgQSend() failed=%lu", status, 0,0,0,0,0);
      }
   }

   return status;
}

/* examplePtpSocketConfigIPv6 */
/**
   Sets up all of the socket options then binds it to an IPv6 address/UDP port.

*******************************************************************************/
static zlStatusE examplePtpSocketConfigIPv6(zl303xx_PortAddress *pSrcAddr,
                                            zl303xx_PortAddress *pMcastAddr,
                                            socketS *skt)
{
#if !defined ZL_INCLUDE_IPV6_SOCKET
    if (pSrcAddr || pMcastAddr || skt){;} /* added to avoid 'unused parameter' warnings*/
    return ZL303XX_UNSUPPORTED_OPERATION;
#else
    zlStatusE status = ZL303XX_OK;
    examplePtpSocketMsgS sktMsg;
    Uint16T udpPort = skt->udpPort;
    Sint32T fd = skt->fd;
    SINT_T ifindex = skt->ifindex;
    char *ifname = skt->ifname;

    struct sockaddr_in6 srcAddr;
    struct sockaddr_in6 mcastAddr;

    memset(&srcAddr, 0, sizeof(srcAddr));
    if (pSrcAddr)
    {
        srcAddr.sin6_family = AF_INET6;
        srcAddr.sin6_port = htons(udpPort);
        memcpy(&srcAddr.sin6_addr, pSrcAddr->addressField, pSrcAddr->addressLength);

        if (pSrcAddr->addressField[0] == 0xFE && pSrcAddr->addressField[1] == 0x80)
        {
            /* Note without this bind returns invalid argument error */
            ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 2,
                "examplePtpSocketConfigIPv6: Link local address setting sin6_scope_id to %d (%s)", ifindex, ifname, 0,0,0,0);
            srcAddr.sin6_scope_id = ifindex;
        }
    }

    memset(&mcastAddr, 0, sizeof(mcastAddr));
    if (pMcastAddr)
    {
        mcastAddr.sin6_family = AF_INET6;
        mcastAddr.sin6_port = htons(udpPort);
        memcpy(&mcastAddr.sin6_addr, pMcastAddr->addressField, pMcastAddr->addressLength);

        if (pMcastAddr->addressField[0] == 0xFF && pMcastAddr->addressField[1] == 0x02)
        {
            /* Note without this bind returns invalid argument error */
            ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 2,
                "examplePtpSocketConfigIPv6: Link local address setting sin6_scope_id to %d (%s)", ifindex, ifname, 0,0,0,0);
            mcastAddr.sin6_scope_id = ifindex;
        }
    }

   /* Allow this address to be reused. */
   if (status == ZL303XX_OK)
   {
      SINT_T reuseAddr = 1;

      if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
                     (char *)&reuseAddr, sizeof(reuseAddr)) == -1)
      {
         ZL303XX_TRACE_ALWAYS("examplePtpSocketConfigIPv6: Unable to configure socket for address reuse. (errno=%d); udpPort=%d",
                              errno, udpPort ,0,0,0,0);
         status = ZL303XX_INIT_ERROR;
      }
   }

   /* Configure socket as non-blocking. */
   if (status == ZL303XX_OK)
   {
      SINT_T onOff = 1;

      if (ioctl(fd, FIONBIO, (SnativeT)&onOff) == -1)
      {
         ZL303XX_TRACE_ALWAYS("examplePtpSocketConfigIPv6: Unable to configure socket as non-blocking. (errno=%d); udpPort=%d",
                              errno, udpPort, 0,0,0,0);
         status = ZL303XX_INIT_ERROR;
      }
   }

   /* Enable this socket for multicast transmits. We use the unicast socket for all transmits,
    * including multicasts, because we want to use the correct local source IP address. */
   if (status == ZL303XX_OK)
   {
      if (setsockopt(fd, IPPROTO_IPV6, IPV6_MULTICAST_IF,
                     &ifindex, sizeof(ifindex)) == -1)
      {
         ZL303XX_TRACE_ALWAYS("examplePtpSocketConfigIPv6: Unable to configure socket for multicast TX. (errno=%ld); udpPort=%d",
                              errno, udpPort, 0,0,0,0);
         status = ZL303XX_INIT_ERROR;
      }
   }



   if (status == ZL303XX_OK)
   {
        /* Prevent multicast data from coming back on the loop */
        Uint32T loop = 0;
        if (setsockopt(fd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
                       &loop, sizeof(loop)) == -1)
        {
           ZL303XX_TRACE_ALWAYS("examplePtpSocketConfigIPv6: Unable to configure socket for multicast ;oopback. (errno=%ld); udpPort=%d",
                                errno, udpPort, 0,0,0,0);
           status = ZL303XX_INIT_ERROR;
        }
   }

   /* #warning MSCC TODO: IEEE 1588-2008 Annex E.5 highest traffic class selector available */

   if (status == ZL303XX_OK)
   {
      if (pMcastAddr != NULL)
      {
          /* printf("examplePtpSocketConfig: pMcastInAddr= 0x%x\n", pMcastInAddr); */
         /* Add socket as a multicast receiver using the specified interface.
          * The multicast address will be the same as the transmit address. */
         struct ipv6_mreq mreq;
         memset(&mreq, 0, sizeof(mreq));

         memcpy(&mreq.ipv6mr_multiaddr, &mcastAddr.sin6_addr, sizeof(mreq.ipv6mr_multiaddr));
         mreq.ipv6mr_interface = ifindex;

         if (setsockopt(fd, IPPROTO_IPV6, IPV6_JOIN_GROUP,
                        (char *)&mreq, sizeof(mreq)) == -1)
         {
            ZL303XX_TRACE_ALWAYS("examplePtpSocketConfigIPv6: Unable to add to IPv6 multicast receive group. (errno=%ld)",
                                 errno, 0,0,0,0,0);
            status = ZL303XX_INIT_ERROR;
         }
      }
   }

#if defined OS_LINUX && defined OHLY_TIMESTAMPS && defined LINUX_3
    if ((status = zl303xx_ConfigOhlyHWTimestamping(zl303xx_GetExampleInterfaceStr(zl303xx_GetExampleInterface()), fd)) != ZL303XX_OK)
    {
        ZL303XX_TRACE_ALWAYS("examplePtpSocketConfigIPv6: Unable to set Ohly HW timestamping. (status %d, errno=%d)", status, errno,0,0,0,0);
    }
#endif

   /* Bind the socket. */
   if (status == ZL303XX_OK)
   {
       Sint32T ret = 0;

      if (pMcastAddr != NULL)
      {
         ret = bind(fd, (struct sockaddr *)(void *)&mcastAddr, sizeof(mcastAddr));
      }
      else
      {
          ret = bind(fd, (struct sockaddr *)(void *)&srcAddr, sizeof(srcAddr));
      }

      if (ret == -1)
      {
         char addrStr[64];
         examplePtpSocketPortAddressToStr(pMcastAddr ? pMcastAddr : pSrcAddr, addrStr, sizeof(addrStr));

         ZL303XX_TRACE_ALWAYS("examplePtpSocketConfigIPv6: Unable to bind IPv6 socket. (errno=%d %s) address %s ifindex %d (%s)",
            errno, (void*)strerror(errno), addrStr, ifindex, ifname,0);
         status = ZL303XX_INIT_ERROR;
      }
      else
          ZL303XX_TRACE_ALWAYS("examplePtpSocketConfigIPv6: SUCCESS: bind IPv6 socket to udpPort=%d", udpPort, 0,0,0,0,0);
   }

   /* Send a message to the RX task to update the fd_set. */
   if (status == ZL303XX_OK)
   {
      sktMsg.type = EXAMPLE_PTP_SOCKET_ADD;
      sktMsg.fd = fd;

      status = examplePtpRxMsgQSend(&sktMsg);
      if (status != ZL303XX_OK)
      {
          ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "examplePtpSocketConfigIPv6: examplePtpRxMsgQSend() failed=%lu", status, 0,0,0,0,0);
          ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1, "examplePtpSocketConfigIPv6: examplePtpRxMsgQSend() failed=%lu", status, 0,0,0,0,0);
       }
    }

   return status;
#endif
}

/* examplePtpSocketConfig802p3 */
/**
   Sets up all of the socket options then binds it to an Eth address

*******************************************************************************/
static zlStatusE examplePtpSocketConfig802p3(zl303xx_PortAddress *pSrcAddr,
                                            zl303xx_PortAddress *pMcastAddr,
                                            socketS *skt)
{
#if !defined ZL_INCLUDE_ETH2_SOCKET
    if (pSrcAddr || pMcastAddr || skt){;} /* added to avoid 'unused parameter' warnings*/
    return ZL303XX_UNSUPPORTED_OPERATION;
#else
    zlStatusE status = ZL303XX_OK;
    struct sockaddr_ll sll;
    Sint32T fd = skt->fd;
    Uint32T ifindex = skt->ifindex;
    char *ifname = skt->ifname;

    (void) pSrcAddr; /* Warning removal */

    memset(&sll, 0, sizeof(struct sockaddr_ll));

    if (status == ZL303XX_OK)
    {
        /* Bind our raw socket to this interface */
        sll.sll_family = AF_PACKET;
        sll.sll_ifindex = ifindex;
        sll.sll_protocol = htons(0x88F7);
        sll.sll_halen = ETHER_ADDR_LEN;
    }

    if (status == ZL303XX_OK)
    {
        if (pMcastAddr != NULL)
        {
            /* Join 802.3 multicast group */
            struct packet_mreq mreq;

            memset(&mreq, 0, sizeof(mreq));
            mreq.mr_ifindex = ifindex;
            mreq.mr_type = PACKET_MR_MULTICAST;
            mreq.mr_alen = ETHER_ADDR_LEN;
            memcpy(&mreq.mr_address, pMcastAddr->addressField, ETHER_ADDR_LEN);

            if (setsockopt(fd, SOL_PACKET, PACKET_ADD_MEMBERSHIP,
                    (char *)&mreq, sizeof(mreq)) == -1)
            {
                ZL303XX_TRACE_ALWAYS("examplePtpSocketConfig802p3: Unable to add to 802.3 multicast receive group. (errno=%ld)",
                                     errno, 0,0,0,0,0);
                status = ZL303XX_INIT_ERROR;
            }
        }
    }

#if defined OS_LINUX && defined OHLY_TIMESTAMPS && defined LINUX_3
    if ((status = zl303xx_ConfigOhlyHWTimestamping(zl303xx_GetExampleInterfaceStr(zl303xx_GetExampleInterface()), fd)) != ZL303XX_OK)
    {
        ZL303XX_TRACE_ALWAYS("examplePtpSocketConfig802p3: Unable to set HW timestamping on FSM99XX. (errno=%d)", errno, 0,0,0,0,0);
    }
#endif

    if(status == ZL303XX_OK)
    {
       if ((bind(fd, (struct sockaddr *)&sll, sizeof(sll))) == -1)
       {
            char addrStr[64];
            examplePtpSocketPortAddressToStr(pMcastAddr ? pMcastAddr : pSrcAddr, addrStr, sizeof(addrStr));

            ZL303XX_TRACE_ALWAYS("examplePtpSocketConfig802p3: Unable to bind 802.3 socket. (errno=%d %s) address %s ifindex %d (%s)",
                errno, (void*)strerror(errno), addrStr, ifindex, ifname,0);
            status = ZL303XX_INIT_ERROR;
       }
    }

    /* Send a message to the RX task to update the fd_set. */
    if (status == ZL303XX_OK)
    {
        examplePtpSocketMsgS sktMsg;

        memset(&sktMsg, 0, sizeof(sktMsg));
        sktMsg.type = EXAMPLE_PTP_SOCKET_ADD;
        sktMsg.fd = fd;

        status = examplePtpRxMsgQSend(&sktMsg);
        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1, "examplePtpSocketConfig802p3: examplePtpRxMsgQSend() failed=%lu", status, 0,0,0,0,0);
            ZL303XX_TRACE(ZL303XX_MOD_ID_TRACK_PKT_PROCESS, 1, "examplePtpSocketConfig802p3: examplePtpRxMsgQSend() failed=%lu", status, 0,0,0,0,0);
        }
    }

   return status;
#endif
}

/* examplePtpSocketRecvFrom */
/**
   Receives a packet from a socket and sends it to PTP.

*******************************************************************************/
static void examplePtpSocketRecvFrom(examplePtpSocketTblS *pTblEntry)
{
    zlStatusE status = ZL303XX_OK;
    SINT_T nBytes = 0;
    struct sockaddr_storage sockaddr_in_src;
    SOCKLEN_T srcLen = (SOCKLEN_T)sizeof(sockaddr_in_src);
    zl303xx_PortAddress originAddr, destAddr;

    ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 4, "examplePtpSocketRecvFrom: fd %d udpPort %d port %d stream %d",
        pTblEntry->fd,
        pTblEntry->udpPort,
        pTblEntry->portHandle,
        pTblEntry->streamHandle,
        0,0);

#if defined OS_LINUX && defined OHLY_TIMESTAMPS && defined LINUX_3

     /* First, look for boomeranged data...
     * Then, look for regular input...
     */
     {
         zl303xx_BooleanE timestampProblem = ZL303XX_FALSE;
         static Uint32T timestampProblemCnt = 0;

         if ((status = zl303xx_RetrieveOhlyHWTimestamp(pTblEntry, MSG_DONTWAIT | MSG_ERRQUEUE)) != ZL303XX_OK)
         {
             if (errno != EAGAIN && errno != ENOMSG)
                 ZL303XX_TRACE_ALWAYS("Unable to receive from echo socket=%d. (errno=%d status=%d)", pTblEntry->fd, errno, status,0,0,0);

             if (status == ZL303XX_DATA_CORRUPTION)
             {
                timestampProblem = ZL303XX_TRUE;
             }
         }

         if ((status = zl303xx_RetrieveOhlyHWTimestamp(pTblEntry, MSG_DONTWAIT)) != ZL303XX_OK)
         {
             if (errno != EAGAIN && errno != ENOMSG)
                 ZL303XX_TRACE_ALWAYS("Unable to receive from socket=%d. (errno=%d status=%d)", pTblEntry->fd, errno, status,0,0,0);

             if (status == ZL303XX_DATA_CORRUPTION)
             {
                timestampProblem = ZL303XX_TRUE;
             }
         }

         if (timestampProblem == ZL303XX_TRUE)
         {
             timestampProblemCnt++;

             /* Failed to obtain timestamps, interface may not be properly configured or was reset */
            ZL303XX_TRACE_ALWAYS("examplePtpSocketRecvFrom: ERROR zero TS, timestamping not properly configured? fd=%d, status %d, count %d", pTblEntry->fd, status, timestampProblemCnt,0,0,0);

            /* Ignore transient problems (only a few packets in a row are bad) */
            if (timestampProblemCnt > 100)
            {
#if defined ABORT_ON_TIMSTAMP_PROBLEM
                /* Abort the entire program if lots of errors happen (assumes system will restart application) */
                ZL303XX_ERROR_TRAP("examplePtpSocketRecvFrom: ERROR Timestamping not properly configured, ABORTING");
#else
                /* Try to reconfigure interface */
                ZL303XX_TRACE_ALWAYS("examplePtpSocketRecvFrom: ERROR needs reconfig, attempting to reconfigure timestamper", 0,0,0,0,0,0);

                if ((status = zl303xx_ConfigOhlyHWTimestamping(zl303xx_GetExampleInterfaceStr(zl303xx_GetExampleInterface()), pTblEntry->fd)) != ZL303XX_OK)
                {
                    ZL303XX_TRACE_ALWAYS("examplePtpSocketRecvFrom: Unable to reconfigure HW timestamping on FSM99XX. (errno=%d status=%d)", errno, status,0,0,0,0);
                }
                else
                {
                    /* Need to reset ToD manager to redo FTM to MAC sync */
                    zl303xx_QcomTodMachineStateE todState = zl303xx_QcomTodMachineGetState();

                    ZL303XX_TRACE_ALWAYS("examplePtpSocketRecvFrom: ERROR needs reconfig, attempting to reconfigure ToD (state %d)", todState,0,0,0,0,0);

                    zl303xx_QcomTodMachineSetState(ZL303XX_QCOM_TOD_MACHINE_INIT);
                }
#endif
            }
         }
         else
         {
             /* Reset the problem counter if a good timestamp arrives */
             timestampProblemCnt = 0;
         }

         return;    /* Done */
     }
#endif

   /* This implementation relies on the previous configuration of the socket to
      make it non-blocking */
     if (nBytes == 0)
     {
         nBytes = recvfrom(pTblEntry->fd, (char *)ExampleRxBuffer, EXAMPLE_PTP_RX_BUFFER_LENGTH,
                           0, (struct sockaddr *)(void *)&sockaddr_in_src, &srcLen);

         if (nBytes == -1)
         {
             ZL303XX_TRACE_ALWAYS("Unable to receive from socket=%d. (errno=%d)", pTblEntry->fd, errno, 0,0,0,0);
             return;
         }

         /* Convert the socket source address structure to zl303xx_PortAddress. */
         switch (sockaddr_in_src.ss_family)
         {
            case AF_INET:
            {
                 struct sockaddr_in *pSock = (struct sockaddr_in*) &sockaddr_in_src;
                 originAddr.networkProtocol = ZL303XX_NETWORK_PROTOCOL_UDP_IPv4;
                 originAddr.addressLength = 4;
                 memcpy(originAddr.addressField, &(pSock->sin_addr.s_addr), originAddr.addressLength);
                 break;
            }
#if defined ZL_INCLUDE_IPV6_SOCKET
            case AF_INET6:
            {
                 struct sockaddr_in6 *pSock = (struct sockaddr_in6*) &sockaddr_in_src;
                 originAddr.networkProtocol = ZL303XX_NETWORK_PROTOCOL_UDP_IPv6;
                 originAddr.addressLength = 16;
                 memcpy(originAddr.addressField, &(pSock->sin6_addr), originAddr.addressLength);
                 break;
            }
#endif
#if defined ZL_INCLUDE_ETH2_SOCKET
            case AF_PACKET:
            {
                 struct sockaddr_ll *pSock = (struct sockaddr_ll *)&sockaddr_in_src;
                 originAddr.networkProtocol = ZL303XX_NETWORK_PROTOCOL_802_3;
                 originAddr.addressLength = 6;
                 memcpy(originAddr.addressField, &(pSock->sll_addr), originAddr.addressLength);
                 break;
            }
#endif
            default:
                ZL303XX_TRACE_ALWAYS("Unable to receive from unsupported family %d on socket=%d",
                                sockaddr_in_src.ss_family, pTblEntry->fd, 0,0,0,0);
                return;
         }
     }

    if (status == ZL303XX_OK)
    {
        /* Send the packet to PTP if it did not originate from us (multicast packets). */
        if (memcmp(originAddr.addressField, pTblEntry->srcAddr.addressField, originAddr.addressLength) != 0)
        {
            /* Use socket table configured local address or the multicast group as the destination address. */
            destAddr = (pTblEntry->mcastAddr.addressLength == 0) ? pTblEntry->srcAddr
                                                                 : pTblEntry->mcastAddr;

            ZL303XX_DEBUG_FILTER(ZL303XX_MOD_ID_SOCKET_LAYER, 4)
            {
                char srcStr[64];
                char destStr[64];

                if (examplePtpSocketPortAddressToStr(&originAddr, srcStr, sizeof(srcStr)) != ZL303XX_OK)
               {
                   strncpy(srcStr, "ERROR", sizeof(srcStr));
               }
                if (examplePtpSocketPortAddressToStr(&destAddr, destStr, sizeof(destStr)) != ZL303XX_OK)
               {
                   strncpy(destStr, "ERROR", sizeof(destStr));
               }

               /* Filtered above */
                ZL303XX_TRACE_ALWAYS("examplePtpSocketRecvFrom: fd %d port %d origin %s dest %s buf0 %02x len %d",
                    pTblEntry->fd,
                    pTblEntry->portHandle,
                    srcStr, destStr, ExampleRxBuffer[0], nBytes);
            }
            /* Timestamp対応 */
            /* (void)examplePtpPortRxMsgProcess(pTblEntry->portHandle, ExampleRxBuffer, nBytes, &originAddr, &destAddr); */
            (void)examplePtpPortRxMsgProcess(pTblEntry->fd, pTblEntry->portHandle, ExampleRxBuffer, nBytes, &originAddr, &destAddr);
        }
    }
}

/* examplePtpRxMsgQSend */
/**
   Sends a message to the RX task's queue.

*******************************************************************************/
static zlStatusE examplePtpRxMsgQSend(examplePtpSocketMsgS *pTaskMsg)
{
   zlStatusE status = ZL303XX_OK;

   /* Check that the msgQ is valid. */
   if (status == ZL303XX_OK)
   {
      if (ExamplePtpRxMsgQ == OS_MSG_Q_INVALID)
      {
         ZL303XX_TRACE_ALWAYS("PTP RX message queue does not exist.", 0,0,0,0,0,0);
         status = ZL303XX_RTOS_MSGQ_INVALID;
      }
   }

   /* Send the message into the queue. */
   if (status == ZL303XX_OK)
   {
       Sint32T waitType = OS_WAIT_FOREVER;

       if (pTaskMsg->type == EXAMPLE_PTP_SOCKET_NO_MSG)
           waitType = OS_NO_WAIT;
      /* Queue the message. If there is no space then discard the message.
         This saves it from becoming overloaded */

      if (OS_OK != OS_MSG_Q_SEND(ExamplePtpRxMsgQ,
                                 (Sint8T *)pTaskMsg,
                                 sizeof(*pTaskMsg),
                                 waitType))
      {
         ZL303XX_TRACE_ALWAYS("Message queue send failed. errno= %d",
            errno, 0,0,0,0,0);
         status = ZL303XX_RTOS_MSGQ_SEND_FAIL;
      }
   }

   return status;
}

/* examplePtpRxMsgQRead */
/**
   Reads the first message from the RX task's queue, if one is available.

*******************************************************************************/
static zlStatusE examplePtpRxMsgQRead(examplePtpSocketMsgS *pTaskMsg)
{
   zlStatusE status = ZL303XX_OK;

   if (ExamplePtpRxMsgQ == OS_MSG_Q_INVALID)
   {
      ZL303XX_TRACE_ALWAYS("PTP RX message queue does not exist.", 0,0,0,0,0,0);
      status = ZL303XX_RTOS_MSGQ_INVALID;
   }

   /* Receive the message from the queue. */
   else if (OS_MSG_Q_RECEIVE(ExamplePtpRxMsgQ, (Sint8T *)pTaskMsg,
                             sizeof(*pTaskMsg), OS_NO_WAIT) == OS_ERROR)
   {
      /* No message to process just now */
      pTaskMsg->type = EXAMPLE_PTP_SOCKET_NO_MSG;
   }

   return status;
}

/* examplePtpRxTask */
/**
   An example task to monitor a list of sockets.

*******************************************************************************/
static Sint32T examplePtpRxTask(void)
{
   SINT_T numFd;
   examplePtpSocketMsgS taskMsg;
   Uint32T count;
   fd_set taskFdSet, workingFdSet;
   struct timeval taskSelectTimeout, workingSelectTimeout;
   zl303xx_BooleanE runTask;
   zlStatusE status = ZL303XX_OK;


   /* Initialize task structures. */
   FD_ZERO(&taskFdSet);
   taskSelectTimeout.tv_sec = 0;
   taskSelectTimeout.tv_usec = EXAMPLE_SOCKET_SELECT_TIMEOUT_MSEC * 1000;

   /* Take the RX mutex. It doesn't get released until a TASK_STOP message is
    * received. */
   runTask = (OS_MUTEX_TAKE(ExamplePtpRxMutex) == OS_OK)
                ? ZL303XX_TRUE
                : ZL303XX_FALSE;

   OS_TASKMON_FUNC_START();

   while (runTask)
   {

      taskMsg.type = EXAMPLE_PTP_SOCKET_NO_MSG;

      /* Process any socket add/deletes. */
      while (runTask && (examplePtpRxMsgQRead(&taskMsg) == ZL303XX_OK) &&
             (taskMsg.type != EXAMPLE_PTP_SOCKET_NO_MSG))
      {
         switch (taskMsg.type)
         {
            case EXAMPLE_PTP_SOCKET_ADD :
               /* Add the PTP socket to the file descriptor set. */
               FD_SET(taskMsg.fd, &taskFdSet);
               break;

            case EXAMPLE_PTP_SOCKET_DELETE :
               /* Remove the PTP socket from the file descriptor set and the
                * socket table. */
               if (taskMsg.fd > 0)
               {
                   FD_CLR(taskMsg.fd, &taskFdSet);
                   examplePtpSocketTblRem(taskMsg.fd);
               }
               break;

            case EXAMPLE_PTP_SOCKET_TASK_STOP :
               runTask = ZL303XX_FALSE;
               break;

            case EXAMPLE_PTP_SOCKET_NO_MSG : /* Eliminates warnings */
            default :
               break;
         }
         taskMsg.type = EXAMPLE_PTP_SOCKET_NO_MSG;
      }

      if (runTask)
      {
          /* Monitor the list of sockets for a received packet. */
          memcpy(&workingFdSet, &taskFdSet, sizeof(workingFdSet));
          memcpy(&workingSelectTimeout, &taskSelectTimeout, sizeof(workingSelectTimeout));

          OS_TASKMON_FUNC_END(); /* End here to not count time waiting in select */

          numFd = select(FD_SETSIZE, &workingFdSet, NULL, NULL, &workingSelectTimeout);

          OS_TASKMON_FUNC_START();

          if (numFd > 0)
          {
              ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 5,
                          "examplePtpRxTask: numFd=%d", numFd, 0,0,0,0,0);
             /* Find which sockets in workingFdSet have packets available. */
             for (count = 0; count < EXAMPLE_PTP_SOCKETS_MAX; count++)
             {
                if ((ExamplePtpSocketTbl[count].active) &&
                    (FD_ISSET(ExamplePtpSocketTbl[count].fd, &workingFdSet)))
                {
					/* PTP受信ポート決定処理 */
					if ( onceflg == ZL303XX_FALSE)
					{
	                	ExamplePtpSocketTbl[count].master = ZL303XX_TRUE;

						ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 4, "examplePtpRxTask: fd %d udpPort %d port %d stream %d",
	        			ExamplePtpSocketTbl[count].fd,
	        			ExamplePtpSocketTbl[count].udpPort,
	        			ExamplePtpSocketTbl[count].portHandle,
	        			ExamplePtpSocketTbl[count].streamHandle,0,0);
					
						T_RRH_PTPCONF_MNG		*f_ptp_tbl_adr_sktrf;		/*  PTP状態テーブル	*/
						int						endcd, errcd;				/*  返り値 */
			
						/* 共有メモリにポート情報を書き込み */
						endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
	        				E_RRH_SHMID_APL_PTP_CONF,   		/* 共有メモリ番号               */
	        				(VOID **)&f_ptp_tbl_adr_sktrf,      /* 共有メモリアドレス           */
	        				&errcd                              /* 詳細NGコード                 */
						);
						/* 共有メモリの取得失敗時 */
						if( endcd != BPF_RU_IPCM_OK )
						{
							ZL303XX_TRACE_ALWAYS("examplePtpRxTask() shared mem get NG.NG:%x ERRCD:%x", endcd,errcd,0,0,0,0);
							return OS_ERROR;
						}
					
						if (ExamplePtpSocketTbl[count].portHandle == 1){
						
							f_ptp_tbl_adr_sktrf->actport = 2;		/* 2:Port1 */
							
							/* SyncE回線クロック選択 FH Port2 */
							/* dpll_mode_refsel[7:4]=0010とするため2を指定 */
							status = zl303xx_Dpll77xCurrRefSet(zl303xx_Params0,ZL303XX_REF_ID_2);
	     				}
	     				else{
							f_ptp_tbl_adr_sktrf->actport = 1;		/* 1:Port0 他の場合はこないはずだが念のためPort0として動作させる */
							
							/* Port0の場合はzl303xx_Dpll77xCurrRefSetでの設定が不要 */;
	     				}
	
						BPF_COM_CMD_SET_CUR_PORTNO((UINT)(f_ptp_tbl_adr_sktrf->actport));
	     			
	     				ZL303XX_TRACE_ALWAYS("examplePtpRxTask: shared mem write =%d.", f_ptp_tbl_adr_sktrf->actport,0,0,0,0,0);
	     			
						if( status != ZL303XX_OK )
						{
							ZL303XX_TRACE_ALWAYS("examplePtpRxTask() zl303xx_Write NG.NG:%x port:%x", status,count,0,0,0,0);
							return OS_ERROR;
						}
						for (count = 0; count < EXAMPLE_PTP_SOCKETS_MAX; count++)
						{
							if ((ExamplePtpSocketTbl[count].active) &&
								((ExamplePtpSocketTbl[count].master == ZL303XX_FALSE)))
							{
								/* 選択したstream以外は削除する */
								examplePtpStreamDelete(ExamplePtpSocketTbl[count].streamHandle);
							}
						}  /* for */
	     				onceflg = ZL303XX_TRUE;		/* 実行済フラグ立てる */
	     			}
					
          	        examplePtpSocketRecvFrom(&ExamplePtpSocketTbl[count]);
                	/* 1ソケット分受信したらループを抜ける */
                    break;
                }
             }  /* for */
          }
          else if (numFd == -1)
          {
             if (errno != EINTR)
             {
                ZL303XX_TRACE_ALWAYS("examplePtpRxTask: Socket select() failed. (errno=%d)", errno, 0,0,0,0,0);
             }
          }
      } /* if runTask */
   }    /* while */

   /* Clean up any open sockets. */
   examplePtpSocketTblRemAll();

   OS_TASKMON_FUNC_END();

   /* Release the mutex and then wait to be deleted. */
   if (ExamplePtpRxMutex && ExamplePtpRxMutex != OS_MUTEX_INVALID)
       OS_MUTEX_GIVE(ExamplePtpRxMutex);
   OS_TASK_DELAY(EXAMPLE_SOCKET_SELECT_TIMEOUT_MSEC  *4);

   /* Task was not deleted in a reasonable amount of time, so return an error. */
   return OS_TIMEOUT;
}

/** examplePtpSocketStrToPortAddress

   Converts an address string to PTP zl303xx_PortAddress type

  Parameters:
   [in]   strAddr        The string address to convert
    [out]  portAddr       Pointer to zl303xx_PortAddress to store result

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE examplePtpSocketStrToPortAddress(const char *strAddr, zl303xx_PortAddress *portAddr)
{
    zlStatusE status = ZL303XX_OK;
    zl303xx_BooleanE matched = ZL303XX_FALSE;

    /* Try IPv4 */
    if (status == ZL303XX_OK && matched == ZL303XX_FALSE)
    {
        unsigned char buf[sizeof(struct in_addr)];
        if (inet_pton(AF_INET, strAddr, buf) > 0)
        {
            matched = ZL303XX_TRUE;
            portAddr->networkProtocol = ZL303XX_NETWORK_PROTOCOL_UDP_IPv4;
            portAddr->addressLength = 4;
            memcpy(&portAddr->addressField, buf, portAddr->addressLength);

            ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 5,
                "examplePtpSocketStrToPortAddress: converted IPv4 string %s", strAddr, 0,0,0,0,0);
        }
    }

#if defined ZL_INCLUDE_IPV6_SOCKET
    /* Try IPv6 */
    if (status == ZL303XX_OK && matched == ZL303XX_FALSE)
    {
        unsigned char buf[sizeof(struct in6_addr)];
        if (inet_pton(AF_INET6, strAddr, buf) > 0)
        {
            matched = ZL303XX_TRUE;
            portAddr->networkProtocol = ZL303XX_NETWORK_PROTOCOL_UDP_IPv6;
            portAddr->addressLength = 16;
            memcpy(&portAddr->addressField, buf, portAddr->addressLength);

            ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 5,
                "examplePtpSocketStrToPortAddress: converted IPv6 string %s", strAddr, 0,0,0,0,0);
        }
    }
#endif

#if defined ZL_INCLUDE_ETH2_SOCKET
    /* Try 802.3 */
    if (status == ZL303XX_OK && matched == ZL303XX_FALSE)
    {
        Uint32T buf[6]; /* buf[0] is MSB of address, int for sscanf */

        /* Hex address sperated by colons, eg. 2F:1C:03:00:01:32 */
        if (sscanf(strAddr, "%x:%x:%x:%x:%x:%x", &buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5]) == 6)
        {
            matched = ZL303XX_TRUE;

            portAddr->networkProtocol = ZL303XX_NETWORK_PROTOCOL_802_3;
            portAddr->addressLength = 6;
            portAddr->addressField[0] = (Uint8T)buf[0];
            portAddr->addressField[1] = (Uint8T)buf[1];
            portAddr->addressField[2] = (Uint8T)buf[2];
            portAddr->addressField[3] = (Uint8T)buf[3];
            portAddr->addressField[4] = (Uint8T)buf[4];
            portAddr->addressField[5] = (Uint8T)buf[5];

            ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 5,
                "examplePtpSocketStrToPortAddress: converted 802.3 string %s", strAddr, 0,0,0,0,0);
        }
    }
#endif

    if (status == ZL303XX_OK && matched == ZL303XX_FALSE)
    {
        ZL303XX_TRACE_ALWAYS("examplePtpSocketStrToPortAddress: ERROR cannot convert string %s to zl303xx_PortAddress (unsupported protocol?)", strAddr,0,0,0,0,0);
        status = ZL303XX_UNSUPPORTED_OPERATION;
    }

    return status;
}


/** examplePtpSocketPortAddressToStr

   Converts PTP zl303xx_PortAddress to string address type

  Parameters:
   [in]   portAddr       Pointer to zl303xx_PortAddress to convert
    [out]  strAddr        The string to store address result
    [in]   strSize        Maximum string size of strAddr buffer

  Return Value:
   zlStatusE

*******************************************************************************/
zlStatusE examplePtpSocketPortAddressToStr(zl303xx_PortAddress *portAddr, char *strAddr, Uint32T strSize)
{
    zlStatusE status = ZL303XX_OK;

    if (portAddr == NULL)
    {
        if (strAddr)
            strAddr[0] = '\0';
        return ZL303XX_ERROR;
    }

    switch (portAddr->networkProtocol)
    {
        case ZL303XX_NETWORK_PROTOCOL_UDP_IPv4:
            if (!strAddr || strSize < INET_ADDRSTRLEN || inet_ntop(AF_INET, &portAddr->addressField, strAddr, strSize) == NULL)
            {
                status = ZL303XX_ERROR;
            }
            break;
        case ZL303XX_NETWORK_PROTOCOL_UNKNOWN:
            snprintf(strAddr, strSize, "UNKNOWN protocol");
            break;

        case ZL303XX_NETWORK_PROTOCOL_UDP_IPv6:
#if defined ZL_INCLUDE_IPV6_SOCKET
            if (!strAddr || strSize < INET6_ADDRSTRLEN || inet_ntop(AF_INET6, &portAddr->addressField, strAddr, strSize) == NULL)
            {
                status = ZL303XX_ERROR;
            }
            break;
#endif
        case ZL303XX_NETWORK_PROTOCOL_802_3:
#if defined ZL_INCLUDE_ETH2_SOCKET
            /* Hex address sperated by colons, eg. 2F:1C:03:00:01:32 */
            if (!strAddr || (Uint32T)snprintf(strAddr, strSize, "%02X:%02X:%02X:%02X:%02X:%02X",
                        portAddr->addressField[0],
                        portAddr->addressField[1],
                        portAddr->addressField[2],
                        portAddr->addressField[3],
                        portAddr->addressField[4],
                        portAddr->addressField[5]) > strSize)
            {
               status = ZL303XX_ERROR;
            }

            if (strAddr)
                strAddr[strSize-1] = '\0';
            break;
#endif
        /* Above should fallthrough if support not defined */
        case ZL303XX_NETWORK_PROTOCOL_DEVICENET:
        case ZL303XX_NETWORK_PROTOCOL_CONTROLNET:
        case ZL303XX_NETWORK_PROTOCOL_PROFINET:
        default:
            ZL303XX_TRACE_ALWAYS("examplePtpSocketPortAddressToStr: ERROR unsupported protocol %d", portAddr->networkProtocol,0,0,0,0,0);
            status = ZL303XX_UNSUPPORTED_OPERATION;
    }

    return status;
}

/*****************   END   ****************************************************/

