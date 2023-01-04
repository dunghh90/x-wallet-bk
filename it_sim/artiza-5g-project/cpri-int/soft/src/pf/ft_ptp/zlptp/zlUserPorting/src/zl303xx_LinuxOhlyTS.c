

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Ohly-style timestamp socket setup and read utilities.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Macros.h"
#include "zl303xx_Int64.h"
#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpStdTypes.h"
#include "zl303xx_PtpApiTypes.h"

#include <errno.h>
#include <string.h>

#if defined LINUX_3
//#include <linux/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <time.h>
#include <linux/net_tstamp.h>
#include <linux/if.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include <linux/if_packet.h>
#include <sys/socket.h>
#include <net/ethernet.h>

#define LINUX_OHLY_SUPPORT_VLAN
#if defined LINUX_OHLY_SUPPORT_VLAN
/* MSCC: Assumes a single VLAN tag with this flag set */
#include <linux/if_vlan.h>
#endif

#include "zl303xx_LinuxOhlyTS.h"

//typedef void* caddr_t;  /* TODO: Why missing? */

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_ConfigIPV4Address */
/**
   A function to run ifconfig on a Linux platform to set IPV4 on an interface

*******************************************************************************/
zlStatusE zl303xx_ConfigIPV4Address(const char* interface, const char* srcIpAddressP)
{
    zlStatusE status = ZL303XX_OK;
    char cmdString[64];
    Sint32T retVal;

    /* FSM99XX EvalBrd: eth1 and IPV4 */
    snprintf(cmdString, sizeof(cmdString), "ifconfig %s %s up", interface, srcIpAddressP);
    if ((retVal = system(cmdString)) != OS_OK)
    {
        printf("zl303xx_ConfigIPV4Address: ifconfig failed running cmd: %s ; Returned errno=%d\n", cmdString, errno);
        status = ZL303XX_TRANSPORT_LAYER_ERROR;
    }

    return status;
}


/* zl303xx_ConfigOhlyHWTimestamping */
/**
   A function to run on a Linux platform to set Ohly-syle hardware time-stamping
   on a socket

*******************************************************************************/
zlStatusE zl303xx_ConfigOhlyHWTimestamping(char const * ifName, Sint32T fd)
{
    zlStatusE status = ZL303XX_OK;
    struct ifreq ifReq;
    struct hwtstamp_config hwtsCfg;
    Uint32T                flags, val, index;
    socklen_t              valLen = sizeof(val);

    ZL303XX_TRACE_ALWAYS("zl303xx_ConfigOhlyHWTimestamping on %s fd %d", ifName, fd,0,0,0,0);

    memset(&ifReq, 0, sizeof(ifReq));
    snprintf(ifReq.ifr_name,
             sizeof(ifReq.ifr_name),
             ifName);
    if((ioctl(fd, SIOCGIFINDEX, &ifReq)) == -1)
    {
        ZL303XX_TRACE_ALWAYS("zl303xx_ConfigOhlyHWTimestamping: Error getting index!\n", 0,0,0,0,0,0);
        ZL303XX_ERROR_TRAP("Failed to get index from i/f name");
    }
    index = ifReq.ifr_ifindex;

    if (setsockopt(fd,
                  SOL_SOCKET, SO_REUSEADDR,
                  (void*) ifReq.ifr_name, IFNAMSIZ) != OS_OK)
    {
        status = ZL303XX_ERROR;
        ZL303XX_TRACE_ALWAYS("zl303xx_ConfigOhlyHWTimestamping: Error reuseaddr !\n", 0,0,0,0,0,0);
        ZL303XX_ERROR_TRAP("Failed to reuseaddr");
    }

    if (setsockopt(fd,
                  SOL_SOCKET, SO_BINDTODEVICE,
                  (void*) ifReq.ifr_name, IFNAMSIZ) != OS_OK)
    {
        status = ZL303XX_ERROR;
        ZL303XX_TRACE_ALWAYS("zl303xx_ConfigOhlyHWTimestamping: Error binding !\n", 0,0,0,0,0,0);
        ZL303XX_ERROR_TRAP("Failed to bind");
    }

/*
* Configure Patrick Ohly timestamping...
*/
   memset(&hwtsCfg, 0, sizeof(hwtsCfg));

   hwtsCfg.tx_type = HWTSTAMP_TX_ON;
   hwtsCfg.rx_filter = HWTSTAMP_FILTER_ALL;

   memset(&ifReq, 0, sizeof(ifReq));
   ifReq.ifr_ifindex = index;

   snprintf(ifReq.ifr_name,
            sizeof(ifReq.ifr_name),
            ifName);
   ifReq.ifr_data = (char*) &hwtsCfg;

#if defined LINUX_OHLY_SUPPORT_VLAN
   /* Enable hardware timestamping on real VLAN device */
   {
        struct vlan_ioctl_args args;

        memset(&args, 0, sizeof(args));

        snprintf(args.device1,
            sizeof(args.device1),
            ifName);
        args.cmd = GET_VLAN_REALDEV_NAME_CMD;

        /* Get real device name, fails with EINVAL if not vlan device */
        if (ioctl(fd, SIOCGIFVLAN, (void*)&args) == OS_OK)
        {
            /* VLAN real device found,  */
            ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 1,"zl303xx_ConfigOhlyHWTimestamping: Using VLAN %s parent %s",
                ifName, args.u.device2,0,0,0,0);

            /* Update ifr_name to enable hw timestamp on the real device */
            snprintf(ifReq.ifr_name,
                    sizeof(ifReq.ifr_name),
                    args.u.device2);
        }
        else if (errno == EINVAL)
        {
            /* Not vlan device, fallthrough below to try setting HW timestamping anyway (might fail)*/
            ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2,"zl303xx_ConfigOhlyHWTimestamping: Interface %s may not be VLAN (or argument error), OK",
                ifName, 0,0,0,0,0);
        }
        else
        {
            /* Other error with ioctl */
            ZL303XX_TRACE_ALWAYS("zl303xx_ConfigOhlyHWTimestamping: Error getting VLAN parent %s, %d\n", ifName, errno, 0,0,0,0);
            ZL303XX_ERROR_TRAP("Failed to set HW timestamping on VLAN device");
        }
   }
#endif

   if ((ioctl(fd, SIOCSHWTSTAMP, &ifReq)) != OS_OK)
   {
       ZL303XX_TRACE_ALWAYS("zl303xx_ConfigOhlyHWTimestamping: Error setting HW timestamping on %s\n", ifReq.ifr_name,0,0,0,0,0);
       ZL303XX_ERROR_TRAP("Failed to set HW timestamping");
   }

   flags =
       SOF_TIMESTAMPING_RX_HARDWARE  |
       SOF_TIMESTAMPING_TX_HARDWARE  |
       SOF_TIMESTAMPING_RAW_HARDWARE |
       SOF_TIMESTAMPING_SYS_HARDWARE;

   if ((setsockopt(fd, SOL_SOCKET, SO_TIMESTAMPING, (void*) &flags, sizeof(flags))) != OS_OK)
   {
       ZL303XX_TRACE_ALWAYS("zl303xx_ConfigOhlyHWTimestamping: Error setting HW flags!\n", 0,0,0,0,0,0);
       ZL303XX_ERROR_TRAP("Failed to set HW flags");
   }

   /*
    * Make sure the flags above took...
    */
   val = 0;
   if ((getsockopt(fd, SOL_SOCKET, SO_TIMESTAMPING, &val, &valLen)) != OS_OK)
   {
       ZL303XX_TRACE_ALWAYS("zl303xx_ConfigOhlyHWTimestamping: Error getting HW flags !\n", 0,0,0,0,0,0);
       ZL303XX_ERROR_TRAP("Failed to get HW flags");
   }
   else if (val != flags)
   {
       ZL303XX_TRACE_ALWAYS("zl303xx_ConfigOhlyHWTimestamping: Error setting HW flags %d != %d!\n", val, flags,0,0,0,0);
       ZL303XX_ERROR_TRAP("Failed to get HW flags");
   }
   else
   {
      ZL303XX_TRACE_ALWAYS("zl303xx_ConfigOhlyHWTimestamping: HW timestamping enabled for fd=%d (%s)\n", fd, ifName,0,0,0,0);
   }

   return status;
}

/* zl303xx_RetrieveOhlyHWTimestamp */
/**
   A function to run on a Linux platform to retreive Ohly-syle hardware time-stamping
   on a socket

   Returns ZL303XX_OK for success,
           ZL303XX_DATA_CORRUPTION for timestamp issue,
           ZL303XX_ERROR for other issue

*******************************************************************************/
zlStatusE zl303xx_RetrieveOhlyHWTimestamp(examplePtpSocketTblS *pTblEntry, Uint32T flags)
{
    zlStatusE status = ZL303XX_OK;
    Uint8T *rawFrameP;
    zl303xx_TimeStamp fifoTS = {{0,0}, 0};
    PtpEthFrame_t rawFrame;

    struct msghdr msgHdr;
    char          control[512];
    struct iovec  iov = { &rawFrame, sizeof(rawFrame) };

    struct sockaddr_storage from_addr;


    int retVal;

    errno = 0;

    /*
     * Prep to use recvmsg...
     */
    memset(&rawFrame, 0, sizeof(rawFrame));
    memset(&from_addr, 0, sizeof(from_addr));

    memset(&msgHdr, 0, sizeof(msgHdr));

    memset(&control, 0, sizeof(control));

    msgHdr.msg_iov        = &iov;
    msgHdr.msg_iovlen     = 1;
    msgHdr.msg_control    = control;
    msgHdr.msg_controllen = sizeof(control);
    msgHdr.msg_name       = &from_addr;
    msgHdr.msg_namelen    = sizeof(from_addr);

    /*
     * Read data out of the socket...
     */
again:

    errno = 0;
    if ((retVal = recvmsg(pTblEntry->fd, &msgHdr, flags)) < 0)
    {
        if ( errno == EINTR )
        {
            goto again;
        }
        else if ((flags & MSG_ERRQUEUE) && errno == EAGAIN)
        {
            /* Boomerang channel can be empty so absorb the error and return OK */
            return OS_OK;
        }
        else
        {
            ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 1,
                        "zl303xx_RetrieveOhlyHWTimestamp: recvmsg FAIL =%d, errno=%d", retVal, errno, 0,0,0,0);
            return -1;
        }
    }

    ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2,
                "zl303xx_RetrieveOhlyHWTimestamp: recvmsg SUCCESS =%d on fd=%d, stream=%d, flags=0x%x", retVal, pTblEntry->fd, pTblEntry->streamHandle, flags,0,0);

#undef DUMP_PKT
#if defined DUMP_PKT
    {   /* rawFrame */
        int i;
        char *p = (char*)&rawFrame;

        printf("zl303xx_RetrieveOhlyHWTimestamp: 0x");
        for (i=0; i<retVal; i++,p++)
            printf(" %x", *p);
        printf("\n");
    }
#endif

    if (1)
    {
        struct cmsghdr*  cmsgPtr = 0;
        struct msghdr*   msgPtr = &msgHdr;

        for ( cmsgPtr = CMSG_FIRSTHDR(msgPtr);
              cmsgPtr;
              cmsgPtr = CMSG_NXTHDR(msgPtr, cmsgPtr) )
        {
            if( cmsgPtr->cmsg_level == SOL_SOCKET
                &&
                cmsgPtr->cmsg_type == SO_TIMESTAMPING )
            {
                struct timespec* pktTime = (struct timespec*) (void*) CMSG_DATA(cmsgPtr);
                /*
                 * Use sys HW timestamps...
                 */
                fifoTS.secondsField.lo = pktTime[2].tv_sec;
                fifoTS.nanosecondsField = pktTime[2].tv_nsec;
                ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2,
                            " zl303xx_RetrieveOhlyHWTimestamp: fifoTS=%u %u.%09u", fifoTS.secondsField.hi, fifoTS.secondsField.lo, fifoTS.nanosecondsField, 0,0,0);
                break;
            }

        }   /* for */
    }


    /* Check for missing timestamp (configuration issue) */
    if (fifoTS.secondsField.lo == 0 && fifoTS.nanosecondsField == 0)
    {
        /* Timestamp not obtained from socket (no SOL_SOCKET). We expected TS on all packets as configured */
        /* Drop the packet and return error */

        /* Possible cause: Timestamping may have been disabled in the driver (by other app).
                           It is caller's responsibility try to recover.
        */
        ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 1,
                            " zl303xx_RetrieveOhlyHWTimestamp: ERROR no timestamp (fifoTS=%u %u.%09u)",
                             fifoTS.secondsField.hi, fifoTS.secondsField.lo, fifoTS.nanosecondsField, 0,0,0);

        return ZL303XX_DATA_CORRUPTION;
    }

    if (1)
    {
        zl303xx_PtpTsRecordS tsRecord;

        rawFrameP = (Uint8T*)&rawFrame;  /* To be used to get PTP payload */

         if ( flags & MSG_ERRQUEUE ) /* This is on the boomerang channel (raw including eth+ip headers) */
         {
         #if defined LINUX_OHLY_SUPPORT_VLAN
            Uint16T etherType = (rawFrameP[12] << 8) | rawFrameP[13];
            if (etherType == 0x8100) /* 802.1Q VLAN */
            {
                /* Offset the 802.1Q VLAN header. The Eth2 header will be offset in protocols below. */
                /* Note Q-in-Q or stacked VLANs is not supported */
                rawFrameP += 4;
                retVal -= 4;
            }
            /*printf("zl303xx_RetrieveOhlyHWTimestamp received MSG_ERRQUEUE etherType %x\n", etherType);*/
         #endif

            /* Assume the offset from the socket's configured local address type */
            switch (pTblEntry->srcAddr.networkProtocol)
            {
                case ZL303XX_NETWORK_PROTOCOL_UDP_IPv4:
                /* Offset 42 assumes Eth2/IPv4/UDP encapsulation */
                     rawFrameP += 42;   /* Offset bytes to PTP payload */
                     retVal -= 42;

                     break;
                case ZL303XX_NETWORK_PROTOCOL_UDP_IPv6:
            #if defined ZL_INCLUDE_IPV6_SOCKET

                    /* Offset 62 assumes Eth2/IPv6/UDP encapsulation */
                    rawFrameP += 62;   /* Offset bytes to PTP payload */
                    retVal -= 62;

                    break;
            #endif
                case ZL303XX_NETWORK_PROTOCOL_802_3:
            #if defined ZL_INCLUDE_ETH2_SOCKET

                    /* Offset 14 assumes Eth2 encapsulation */
                    rawFrameP += 14;   /* Offset bytes to PTP payload */
                    retVal -= 14;

                    break;
            #endif        
                case ZL303XX_NETWORK_PROTOCOL_DEVICENET:
                case ZL303XX_NETWORK_PROTOCOL_CONTROLNET:
                case ZL303XX_NETWORK_PROTOCOL_PROFINET:
                case ZL303XX_NETWORK_PROTOCOL_UNKNOWN:
                default:
                    ZL303XX_TRACE_ALWAYS(
                        "zl303xx_RetrieveOhlyHWTimestamp: packet from unsupported networkProtocol %d received on boomerang, dropped",
                        pTblEntry->srcAddr.networkProtocol, 0,0,0,0,0);
                    return ZL303XX_ERROR;
            }
         }

#if defined DUMP_PKT
        {   /* rawFramP after offset applied */
            int i;
            char *p = (char*)rawFrameP;

            printf(" rawFrameP: 0x");
            for (i=0; i<retVal; i++,p++)
                printf(" %x", *p);
            printf("\n");
        }
#endif

        /* Identify event type egress packets on boomerang channel */
        if ((flags & MSG_ERRQUEUE) && zl303xx_PtpRxMsgIsEvent(rawFrameP, retVal))
        {
            /* Event Rx boomerang packet, deliver egress timestamp to PTP */
            memset(&tsRecord, 0, sizeof(zl303xx_PtpTsRecordS));
            tsRecord.messageType = (*rawFrameP & 0xF);
            tsRecord.streamHandle = pTblEntry->streamHandle;
            tsRecord.timeStamp = fifoTS;
            tsRecord.sequenceId = (*(rawFrameP +ZL303XX_PTP_V2_HDR_SEQ_ID_OFST) << 8 | *(rawFrameP +ZL303XX_PTP_V2_HDR_SEQ_ID_OFST +1));   /* Ptp payload seqId offset */

            ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2,
                        "zl303xx_RetrieveOhlyHWTimestamp: msgType=%d streamHandle=%d tsRecord.timeStamp=%u %u.%09u sequenceId=%d",
                        tsRecord.messageType, tsRecord.streamHandle,
                        tsRecord.timeStamp.secondsField.hi, tsRecord.timeStamp.secondsField.lo,
                        tsRecord.timeStamp.nanosecondsField, tsRecord.sequenceId);

            if ((status = zl303xx_PtpTsRecordProcess(&tsRecord)) != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("zl303xx_RetrieveOhlyHWTimestamp: event call to zl303xx_PtpTsRecordProcess() failed=%d", status, 0,0,0,0,0);
            }
            else
            {
                ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 2,
                            "zl303xx_RetrieveOhlyHWTimestamp: event streamHandle=%d fifoTS=%u %u.%09u sequenceId=%d", tsRecord.streamHandle, fifoTS.secondsField.hi, fifoTS.secondsField.lo, fifoTS.nanosecondsField, tsRecord.sequenceId, 0);
            }
        }
        else if (flags & MSG_ERRQUEUE)
        {
            /* Non-event Rx boomerang packet, drop */
            /* No logging for efficiency */
        }
        else
        {
            /* Regular Rx packet, timestamps only necessary for event type but OK for all */
            zl303xx_PtpRxMsgDataS pMsgData;

            pMsgData.rxTs = fifoTS;

            /* Convert the socket source address structure to zl303xx_PortAddress. */
            switch (from_addr.ss_family)
            {
                case AF_INET:
                {
                     struct sockaddr_in *pSock = (struct sockaddr_in*) &from_addr;
                     pMsgData.srcAddr.networkProtocol = ZL303XX_NETWORK_PROTOCOL_UDP_IPv4;
                     pMsgData.srcAddr.addressLength = 4;
                     memcpy(pMsgData.srcAddr.addressField, &(pSock->sin_addr.s_addr), pMsgData.srcAddr.addressLength);
                     break;
                }
#if defined ZL_INCLUDE_IPV6_SOCKET
                case AF_INET6:
                {
                     struct sockaddr_in6 *pSock = (struct sockaddr_in6*) &from_addr;
                     pMsgData.srcAddr.networkProtocol = ZL303XX_NETWORK_PROTOCOL_UDP_IPv6;
                     pMsgData.srcAddr.addressLength = 16;
                     memcpy(pMsgData.srcAddr.addressField, &(pSock->sin6_addr), pMsgData.srcAddr.addressLength);
                     break;
                }
#endif
#if defined ZL_INCLUDE_ETH2_SOCKET
                case AF_PACKET:
                {
                     struct sockaddr_ll *pSock = (struct sockaddr_ll *)&from_addr;
                     pMsgData.srcAddr.networkProtocol = ZL303XX_NETWORK_PROTOCOL_802_3;
                     pMsgData.srcAddr.addressLength = 6;
                     memcpy(pMsgData.srcAddr.addressField, &(pSock->sll_addr), pMsgData.srcAddr.addressLength);
                     break;
                }
#endif
                default:
                    ZL303XX_TRACE_ALWAYS(
                        "zl303xx_RetrieveOhlyHWTimestamp: packet from unknown socket family %d received, dropped",
                        from_addr.ss_family, 0,0,0,0,0);
                    return ZL303XX_ERROR;
            }
             
            /* Convert the in_addr source address of the socket table entry to a zl303xx_PortAddress,
             * to use as the destination address. Or if multicast then dest is multicast group */
            memcpy(&pMsgData.destAddr, 
                (pTblEntry->mcastAddr.addressLength == 0) ? 
                    &pTblEntry->srcAddr : &pTblEntry->mcastAddr,
                    sizeof(pMsgData.destAddr));

            if ((status = zl303xx_PtpRxMsgProcess(rawFrameP, (Uint16T) retVal, &pMsgData)) != ZL303XX_OK)
            {
                ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 1,
                    "zl303xx_RetrieveOhlyHWTimestamp: Call to zl303xx_PtpRxMsgProcess() failed=%d, streamHandle=%d fifoTS=%u %u.%09u",
                    status, pTblEntry->streamHandle,
                    fifoTS.secondsField.hi, fifoTS.secondsField.lo, fifoTS.nanosecondsField, 0);
            }
            else
            {
                ZL303XX_TRACE(ZL303XX_MOD_ID_TS_RECORD_MGR, 4,
                    " zl303xx_RetrieveOhlyHWTimestamp: OTHER streamHandle=%d msgtype=%d fifoTS=%u %u.%09u",
                    pTblEntry->streamHandle, rawFrameP[0],
                    fifoTS.secondsField.hi, fifoTS.secondsField.lo, fifoTS.nanosecondsField, 0);
            }
        }
    }

    return status;
}


#else
/* #warning MSCC: Unable to compile with this version of header files */
#endif /* LINUX_3 */
