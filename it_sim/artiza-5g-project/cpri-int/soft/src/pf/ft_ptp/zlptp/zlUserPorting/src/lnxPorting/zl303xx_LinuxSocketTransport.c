

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This provides an implementation of the functions in zl303xx_Transport.h
*     using the Linux socket library
*
******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#ifdef OS_LINUX
    #define _LINUX_TYPES_H   /* ZL - Stop multiple type includes */
    #define _LINUX_TIME_H

    #include <sys/types.h>

    #if defined ZL_LNX_DENX || defined ZL_LNX_CODESOURCERY || defined ZL_LNX_INTEL
        #define __USE_MISC      /* Socket stuff */
        #define _ZL303XX_SKIP_LINUX_SOCKET_MOD
    #endif

    #include <arpa/inet.h>
    #include <net/if.h>

   #ifndef ZL_LNX_CODESOURCERY
        #include <linux/sockios.h>
        #include <linux/ioctl.h>
    #else
      #ifndef ntohl
        #include <netinet/in.h>
      #endif

    /* #warning MSCC: CODESOURCERY include issue! */
        typedef unsigned short __be16;  /* Only this is undefined */
    #endif

    #include <asm/ioctls.h>
    #include <sys/select.h>
    #include <sys/../string.h>
    #include <sys/../fcntl.h>
    #include <sys/ioctl.h>

    #define INET_ADDR_LEN    18

    #include "zl303xx_LinuxSocketTransport.h"

#ifdef ZL_INCLUDE_IPV6_SOCKET
#include <ifaddrs.h>
#endif
#endif

#include "zl303xx_Global.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Os.h"
#include "zl303xx_Pkt.h"

#ifdef __VXWORKS_65
#include <net/if_arp.h>
#include <net/utils/ifconfig.h>
#include <net/utils/routeCmd.h>
#endif

#include "zl303xx_SocketSelect.h"
#include "zl303xx_PortingFunctions.h"

/*****************   DEFINES     **********************************************/
/* The return from the socket call when it can't create a socket */
#define INVALID_SOCKET OS_ERROR

/* The return from socket calls when an error occurs */
#define SOCKET_ERROR OS_ERROR

#define IFCONF_LENGTH 10 /* Set an artificial limit of 10 possible attached interfaces */

/* Get the broadcast address associated with a specific IP address and net mask */
#define BCAST_ADDR(ipAddress, netMask) (~(netMask) | (ipAddress))

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/
_ZL303XX_LOCAL Sint32T zl303xx_LxSTInit(zl303xx_SocketDataS *pSktConnectData);
_ZL303XX_LOCAL Sint32T zl303xx_LxSTClose(zl303xx_SocketDataS *pSktConnectData);
_ZL303XX_LOCAL Sint32T zl303xx_LxGetIface(zl303xx_SocketDataS *pSktConnectData);

#ifdef ZL_INCLUDE_IPV6_SOCKET
static Sint32T zl303xx_LxSTInit6(zl303xx_SocketDataS *pSkt);
static Sint32T zl303xx_LxSTClose6(zl303xx_SocketDataS *pSkt);
static Sint32T zl303xx_LxGetIface6(zl303xx_SocketDataS *pSkt);
static Sint32T sockGetIfIndex(const char *ifName, Sint32T sockFd);
#endif

/*****************   STATIC GLOBAL VARIABLES   ********************************/
static const char zl303xx_PhysicalInterfaceString[] = "IEEE 802.3";

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/
/* Global variable for storing the last socket error encountered */
Sint32T Zl303xx_LastSocketErr = 0;

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/**

  Function Name:
    sockCreateNonBlock

  Details:
   Creates a bidirectional communication path to the ZL303XX_ device through the
   Linux socket transport layer

  Parameters:
       pSktData       Pointer to an uninitialised structure for the socket
        ifName         The name of the local interface to use
                  or NULL to cause a default ethernet port to be used.
        initData       structure of type sockAddrDataS containing application supplied
                  initialisation data (see below)
        port           Port number to create

Structure inputs:

        destIpAddr     The destination IP address as a string (format "aaa.bbb.ccc.ddd").

       pSktData      Structure members will be fully populated

  Return Value:
   0 (OK) or -1 if an error occurs

*******************************************************************************/

Sint32T sockCreateNonBlock(zl303xx_SocketDataS *pSktData, const char *ifName,
                           sockAddrDataS *initData, Uint16T port)
{
   Sint32T status = 0;

   if (pSktData == NULL)
   {
      ZL303XX_ERROR_TRAP("Invalid socket data structure pointer");
      status = -1;
   }

   if (status == 0)
   {
      if (initData == NULL)
      {
         ZL303XX_ERROR_TRAP("Invalid initialisation data structure pointer");
         status = -1;
      }
   }

   /* For multicast or broadcast convert the destination address to network format */
   if (status == 0)
   {
      if (initData->ifType == ZL303XX_TRANSPORT_MULTICAST)
      {
         /* Check that the supplied address is a valid multicast */
         if (IN_MULTICAST(ntohl(initData->destAddr.s_addr)))
         {
            /* Address is a class D, multicast address, i.e. top byte is in
             * range 224-239 inclusive */
            memcpy(pSktData->mcastAddr, &initData->destAddr,
                   sizeof(initData->destAddr.s_addr));
         }
         else
         {
            ZL303XX_ERROR_NOTIFY("Non-multicast address supplied");
            status = -1;
         }
      }
      else if (initData->ifType == ZL303XX_TRANSPORT_BROADCAST)
      {
        /* This ifType is being used to identify a raw interface used to send non-IP/UDP packets */
        /* The interface is required for MPLS based packets */

         /* Check that the supplied address is a valid broadcast address*/
         if ((initData->destAddr.s_addr == BCAST_ADDR(initData->destAddr.s_addr, pSktData->netMask)) ||
             (initData->destAddr.s_addr == 0xFFFFFFFF))
         {
            memcpy(pSktData->mcastAddr, &initData->destAddr.s_addr,
                   sizeof(initData->destAddr.s_addr));
         }
         else
         {
            ZL303XX_ERROR_NOTIFY("Non-broadcast address supplied");
            status = -1;
         }
      }
      else if (initData->ifType == ZL303XX_TRANSPORT_UNICAST)
      {
         /* Nothing to do. The mcastAddr data member will be unused */
      }
   }

   if (status == 0)
   {
      /* Set the data members in the socket data structure */
      /* Store the port and the interface type */
      pSktData->port = port;
      pSktData->type = initData->ifType;
      memcpy(pSktData->localIpAddr, &initData->srcAddr.s_addr,
             sizeof(initData->srcAddr.s_addr));

      if (ifName != NULL)
      {  /* Copy the supplied name into the structure */
         strncpy(pSktData->ifaceName, ifName, ZL303XX_IFACE_NAME_MAX_LEN);
         pSktData->ifaceName[ZL303XX_IFACE_NAME_MAX_LEN] = '\0';   /* Ensure it is terminated */
      }

      pSktData->socketFd = INVALID_SOCKET;
   }

   if (status == 0)
   {  /* Call the initialisation function */
      status = zl303xx_LxSTInit(pSktData);
   }

   return status;
}

/**

  Function Name:
    sockDestroy

  Details:
   Destroys a communication path information structure corresponding to a
   previously established connection

  Parameters:
       pTrnsInfo     Information about the connection.

  Return Value:
   0 (OK) or -1 if an error occurs

 *****************************************************************************/

Sint32T sockDestroy(zl303xx_SocketDataS *pSktData)
{
   Sint32T status = 0;

   if (pSktData == NULL)
   {
      ZL303XX_ERROR_TRAP("Invalid socket data structure pointer");
      status = -1;
   }

   /* Call the close function */
   if (status == 0)
   {
   #ifdef ZL_INCLUDE_IPV6_SOCKET
      if (pSktData->addrFamily == AF_INET6)
      {
         status = zl303xx_LxSTClose6(pSktData);
      }
      else
   #endif
      {
         status = zl303xx_LxSTClose(pSktData);
      }
   }

   return status;
}

/**
  Function Name:
   sockSend

  Details:
   Sends a buffer to the previously configured socket connection

  Parameters:
       pSktData    An identifier for a previously established and configured connection.
        destAddr    The destination address to send the packets
        buf         A pointer to the buffer to send
        nBytes      The number of octets to send

       nBytes      The number of bytes actually sent

  Return Value:
   The number of bytes written or -1 if an error occurs

*******************************************************************************/
Sint32T sockSend(zl303xx_SocketDataS *pSktData, zl303xx_RemoteAddrT *destAddr, Uint8T *buf, Uint32T *nBytes)
{
   Sint32T retVal = 0;
   Sint32T numBytes = 0;

   if (pSktData == NULL)
   {
      ZL303XX_ERROR_TRAP("Invalid socket info structure pointer");
      retVal = -1;
   }

   if (retVal == 0)
   {
      struct sockaddr_in addr;

      addr.sin_family = pSktData->addrFamily;
      addr.sin_port = htons(pSktData->port);
      addr.sin_addr = *destAddr;

      numBytes = sendto(pSktData->socketFd, (char *)buf, *nBytes, 0,
                     (struct sockaddr *)(void *)&addr, sizeof(addr));

      if (numBytes == OS_ERROR)
      {
         /* Record the last error in case we want to inspect it later */
         Zl303xx_LastSocketErr = errno;

         *nBytes = 0;   /* No bytes were sent (presumably) */

         if ((errno == EHOSTDOWN) ||
             (errno == EHOSTUNREACH) ||
             (errno == ENETUNREACH))
         {  /* Host is down or unreachable. Don't treat this as a fatal error. It simply means we can't see
               the remote end at this moment. Allow the system to keep trying to connect. */
            ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 1, "Unable to send to socket: remote host is unreachable, error %ld", errno, 0, 0, 0, 0, 0);
         }
         else
         {  /* Treat other errors as fatal */
            ZL303XX_TRACE_ALWAYS("Unable to send to socket : %ld", errno, 0, 0, 0, 0, 0);
            retVal = -1;
         }
      }
   }

   if (retVal != 0)
   {
      return retVal;
   }
   else
   {
      return *nBytes;
   }
}

/**
  Function Name:
   sockRecv

  Details:
   Performs a read on the socket connection

  Parameters:
       pSktData    A data structure for a previously established and configured socket.
        buf         A pointer to the buffer to write data into
        buflen      The length of the buffer

        buf         Filled with the received data
        buflen      Set to the length of the received data
        remoteAddr  The remote address from which the packet was received
        remoteAddrLen  The number of bytes in the remote address

  Return Value:
   The number of bytes read or -1 if an error occurs

  Notes:
   It is assumed in this application that the socket is configured as non-blocking

*******************************************************************************/
Sint32T sockRecv(zl303xx_SocketDataS *pSktData, Uint8T *buf, Uint32T buflen,
                  zl303xx_RemoteAddrT *remoteAddr, Uint16T *remoteAddrLen)
{
   Sint32T retVal = 0;
   Sint32T nBytes = 0;
   struct sockaddr_in fromAddr;
   Sint32T fromLen = sizeof(fromAddr);

   if (pSktData == NULL)
   {
      ZL303XX_ERROR_TRAP("Invalid socket info structure pointer");
      retVal = -1;
   }

   if (retVal == 0)
   {
      if (remoteAddr == NULL)
      {
         ZL303XX_ERROR_TRAP("Invalid address pointer");
         retVal = -1;
      }
   }
   if (retVal == 0)
   {
      Uint8T retries = 0;

eAgain:
      /* This implementation relies on the previous configuration of the socket to
         make it non-blocking */
      nBytes = recvfrom(pSktData->socketFd, (char *)buf, buflen, 0, (struct sockaddr *)(void *)&fromAddr, (UINT_T*)&fromLen);

      if (nBytes == OS_ERROR)
      {
         /* Record the last error in case we want to inspect it later */
         Zl303xx_LastSocketErr = errno;
#ifdef OS_LINUX
         if (errno != EAGAIN)
         {
            ZL303XX_TRACE_ALWAYS("Unable to receive from socket : %ld", errno, 0,0,0,0,0);
            retVal = -1 ;
         }
         else
             if (++retries < 4)
                 goto eAgain;
             else
                 ZL303XX_TRACE_ALWAYS("sockRecv: recvfrom() failed %ld times with errno=%d (EAGAIN)", retries-1, errno, 0,0,0,0);
#endif
      }
      else
      {
         /* Return the receive-from address. */
         *remoteAddr = fromAddr.sin_addr;
         *remoteAddrLen = sizeof(fromAddr.sin_addr);
      }
   }

   if (retVal != 0)
   {
      return retVal;
   }
   else
   {
      return nBytes;
   }
}

/**
  Function Name:
   sockSetIpTosValue

  Details:
   Sets the Type of Service (TOS) field in the IP4 header

  Parameters:
       pSktData    A data structure for a previously established and configured socket.
        ipTos       The Tos byte to use.
               This byte is interpreted differently in old and new systems:
               In older systems it consists of a precedence value (bits 7-5) and
               4 type-of-service bits (bits 4-1) of which only one can be set.
               In newer systems it consists of a Differentiated Services Code Point (DSCP)
               (bits 7-2) and two Explicit Congestion Notification bits (bits 1-0)

  Return Value:
   0 (OK) or -1 if an error occurs

  Notes:
   This function is very specific to IPv4.

*******************************************************************************/
Sint32T sockSetIpTosValue(zl303xx_SocketDataS *pSktData, Uint8T ipTos)
{
   Sint32T status = 0;
    /* The IP_TOS option for setsockopt requires an int parameter */
   SINT_T ip_tos = (SINT_T)ipTos;

   if (setsockopt(pSktData->socketFd, IPPROTO_IP, IP_TOS, (char *)&ip_tos, sizeof(SINT_T)) == OS_ERROR)
   {
      /* Record the last error in case we want to inspect it later */
      Zl303xx_LastSocketErr = errno;

      ZL303XX_TRACE_ALWAYS("Unable to configure socket TOS field. Error value: %ld", errno, 0, 0, 0, 0, 0);
      status = -1;
   }
   return status;
}

/**
  Function Name:
   sockGetHeaderLen

  Details:
   Returns the length of the header that will be prepended to the packet on the
   specified connection

  Parameters:
       pSktData    A data structure for a previously established and configured socket.

       headLen     The length of the added header

  Return Value:
   0 (OK) or -1 if an error occurs

*******************************************************************************/
Sint32T sockGetHeaderLen(zl303xx_SocketDataS *pSktData, Uint32T *headLen)
{
   if (pSktData->type == ZL303XX_TRANSPORT_BROADCAST)
   {
      /* MPLS family */
      *headLen = ZL303XX_PKT_ETHERNET_HDR_LEN + ZL303XX_PKT_MPLS_HDR_LEN;
   }
   else
   {
      /* We know the protocol as we configured the socket for UDP datagrams.
            Therefore the added protocols are Ethernet+IPv4+UDP */
      *headLen = ZL303XX_PKT_ETHERNET_HDR_LEN + ZL303XX_PKT_IPV4_HDR_LEN + ZL303XX_PKT_UDP_HDR_LEN;
   }

   return 0;
}


/**
  Function Name:
   sockGetPhysicalLayerProtocol

  Details:
   Obtains a description string for the physical layer in use

  Parameters:
       pSktData    A data structure for a previously established and configured socket.

       physicalLayerString  A pointer to a static, constant string describing the
            physical layer protocol.

  Return Value:
   0 (OK) or -1 if an error occurs

*******************************************************************************/
Sint32T sockGetPhysicalLayerProtocol(zl303xx_SocketDataS *pSktData, const char **physicalLayerString)
{
   if (pSktData == NULL)
   {
      ZL303XX_ERROR_TRAP("Invalid socket data structure pointer");
      return -1;
   }

   /* Return a constant string describing the protocol */
   *physicalLayerString = zl303xx_PhysicalInterfaceString;

   return 0;
}

/**
  Function Name:
   sockGetUuid

  Details:
   Obtains a unique identifier for the specified transport link

  Parameters:
       pSktData    A data structure for a previously established and configured socket.

       uuid     A unique identifier for this stream interface. For PTP applications
            this should be the MAC address of the interface

  Return Value:
   0 (OK) or -1 if an error occurs

*******************************************************************************/
Sint32T sockGetUuid(zl303xx_SocketDataS *pSktData, Uint8T (*uuid)[ZL303XX_UUID_NUM_BYTES])
{
   if (pSktData == NULL)
   {
      ZL303XX_ERROR_TRAP("Invalid socket data structure pointer");
      return -1;
   }

   /* Clear the output buffer */
   memset(uuid, 0, ZL303XX_UUID_NUM_BYTES);

   /* Copy the MAC address to the uuid field */
   memcpy(uuid, pSktData->macAddr, ZL303XX_UUID_NUM_BYTES <= ZL303XX_MAC_ADDRESS_BYTES ? ZL303XX_UUID_NUM_BYTES : ZL303XX_MAC_ADDRESS_BYTES);

   return 0;
}

/**
  Function Name:
   sockGetIfaceName

  Details:
   Gets the name of the interface to be used by this socket

  Parameters:
       pSktData    A data structure for a previously established and configured socket.

       ifaceName   Pointer to a buffer to receive the interface name string

  Return Value:
   0 (OK) or -1 if an error occurs

 *****************************************************************************/
Sint32T sockGetIfaceName(zl303xx_SocketDataS *pSktData, char *ifaceName)
{
   Sint32T status = 0;

   /* Check parameters */
   if (pSktData == NULL)
   {
      ZL303XX_ERROR_TRAP("Invalid socket info structure pointer");
      status = -1;
   }
   if (ifaceName == NULL)
   {
      ZL303XX_ERROR_TRAP("Invalid ifaceName pointer");
      status = -1;
   }
   else
   {
      if (status == 0)
      {
         strncpy(ifaceName, pSktData->ifaceName, sizeof(pSktData->ifaceName));
      }
   }

   return status;
}

/**
  Function Name:
   sockGetAddr

  Details:
   Returns the protocol address information for the specified socket

  Parameters:
       pSktData    An identifier for a previously established and configured connection.
        addr        pointer to an octet array to receive the address information
        size        Size of the addr buffer, i.e. maximum size

       addrFamily  Address family of the address.
               For this socket implementation it is always AF_INET
        addr        array will be filled with the address information
               as an ASCII string.
               The address information is not null terminated.
        size        Number of bytes in the address.

  Return Value:
   0 (OK) or -1 if an error occurs

  Notes:
   Does not perform validity checks on its parameters

*******************************************************************************/
Sint32T sockGetAddr(zl303xx_SocketDataS *pSktData, Uint8T *addrFamily, Uint8T *addr, Uint16T *size)
{
   Sint32T status = 0;

   *addrFamily = pSktData->addrFamily;

   status = sockMakeAddrFromLinkId(
               (zl303xx_RemoteAddrT *)(void *)&pSktData->localIpAddr, addr, size);

   return status;
}

/**
  Function Name:
   sockMakeAddrFromLinkId

  Details:
   Converts an address identifier in socket specific format into an ASCII string

  Parameters:
       linkId      Pointer to a structure to hold the link identifier.
               In this implementation it is of type "struct in_addr"
        addrStr     Pointer to a buffer to hold the address string
        addrSize    Size of the input buffer

        addrStr     Will be filled with the address in ASCII format. Not NULL terminated
        addrSize    Set to the number of ASCII characters in the address

  Return Value:
   0 (OK) or -1 if an error occurs

*******************************************************************************/

Sint32T sockMakeAddrFromLinkId(zl303xx_RemoteAddrT *linkId, Uint8T *addrStr, Uint16T *size)
{
   char tempBuffer[INET_ADDR_LEN];
   Sint32T status = 0;
   Sint32T nBytes;

   if ((addrStr == NULL) || (size == NULL) || (linkId == NULL))
   {  /* Invalid pointer */
      return -1;
   }
#if defined (OS_VXWORKS) 
   /* Convert into a local buffer which we know is big enough */
   inet_ntoa_b(*linkId, tempBuffer);
#endif
#ifdef OS_LINUX
   strncpy(&tempBuffer[0], inet_ntoa(*linkId), sizeof(tempBuffer));
#endif


   /* Check the length of the return buffer */
   nBytes = strlen(tempBuffer);

   if (nBytes > *size)
   {  /* Buffer is not big enough */
      status = -1;
   }
   else
   {
      /* Copy the address to the return buffer */
      memcpy(addrStr, tempBuffer, (Uint32T)nBytes);

      *size = nBytes;
   }

   return status;
}

/******   Functions for manipulating socket specific IDs   ****************/

/**
  Function Name:
   sockAllocLinkId

  Details:
   Allocates space for a device specific address structure

  Parameters:
       Pointer to a dynamically allocated device address structure

  Return Value:
   0 (OK) or -1 if an error occurs

  (Deprecated)
   This function is no longer used by the API.

*******************************************************************************/

Sint32T sockAllocLinkId(zl303xx_RemoteAddrT **linkId)
{
   Sint32T status = 0;

   /* We can make a unique identifier for a link by using the IP address converted to a struct in_addr */
   struct in_addr *sockAddr = (struct in_addr *)OS_CALLOC(1, sizeof(struct in_addr));
   if (sockAddr == NULL)
   {
      ZL303XX_ERROR_TRAP("Unable to allocate memory for link ID");
      *linkId = NULL;
      status = -1;
   }
   else
   {
      *linkId = sockAddr;
   }

   return status;
}

/**
  Function Name:
   sockFreeLinkId

  Details:
   Deallocates space for a device specific address structure

  Parameters:
       linkId   Pointer to a previously allocated device address structure

       linkId   Set to NULL on completion

  Return Value:
   0 (OK) or -1 if an error occurs

  (Deprecated)
   This function is no longer used by the API.

*******************************************************************************/

Sint32T sockFreeLinkId(zl303xx_RemoteAddrT **linkId)
{
   OS_FREE(*linkId);

   *linkId = NULL;

   return 0;
}

/**
  Function Name:
   sockMakeIdFromAddr

  Details:
   Converts an IP address string into an address identifier in socket specific format

  Parameters:
       addrStr     Address as a string
        linkId      Pointer to a structure to hold the link identifier.
                     In this implementation it is of type "struct in_addr"

       linkId      Populated with the unique identifier

  Return Value:
   0 (OK) or -1 if an error occurs

*******************************************************************************/

Sint32T sockMakeIdFromAddr(const char *addrStr, zl303xx_RemoteAddrT *linkId)
{
   Sint32T status = 0;

   struct in_addr *sockAddr = (struct in_addr *)linkId;

   if (inet_aton(addrStr, sockAddr) == OS_ERROR)
   {
      ZL303XX_ERROR_TRAP("Error converting ip address");
      status = -1;
   }

   return status;
}

/**
  Function Name:
   sockCmpLinkId

  Details:
   Compares two LinkId values

  Parameters:
       Id1 & Id2   LinkId values to compare

  Return Value:
   0 if addresses are equal, 1 if not, -1 if an error occurs

  (Deprecated)
   This function is no longer used by the API.

*******************************************************************************/

Sint32T sockCmpLinkId(zl303xx_RemoteAddrT *Id1, zl303xx_RemoteAddrT *Id2)
{
   if ((Id1 == NULL) || (Id2 == NULL))
   {
      ZL303XX_ERROR_TRAP("invalid pointers");
      return -1;
   }
   if (memcmp(Id1, Id2, sizeof(zl303xx_RemoteAddrT)) == 0)
   {
      return 0;
   }
   else
   {
      return 1;
   }
}

/*************************  SELECT() FUNCTIONALITY SUPPORT  *********************/

/**
  Function Name:
   sockFdSetAdd

  Details:
   Adds a socket file descriptor into an FD set

  Parameters:
       pSktData    An identifier for a previously established and configured connection.
        fdSet       Pointer to a FD_SELECT_SET

  Return Value:
   0 (OK) or -1 if an error occurs

*******************************************************************************/
Sint32T sockFdSetAdd(zl303xx_SocketDataS *pSktData, FD_SELECT_SET *fdSet)
{
   Sint32T status = 0;

   if (pSktData == NULL)
   {
      ZL303XX_ERROR_TRAP("Invalid socket info structure pointer");
      status = -1;
   }

   if (status == 0)
   {
      if (fdSet == NULL)
      {
         ZL303XX_ERROR_TRAP("Invalid FD_SELECT_SET pointer");
         status = -1;
      }
   }

   if (status == 0)
   {
      FD_SET((UINT_T)pSktData->socketFd, fdSet);
   }

   return status;
}

/**
  Function Name:
   sockFdSetRemove

  Details:
   Removes a socket file descriptor from an FD set

  Parameters:
       pSktData    An identifier for a previously established and configured connection.
        fdSet       Pointer to a FD_SELECT_SET from which to remove the connection

  Return Value:
   0 (OK) or -1 if an error occurs

*******************************************************************************/
Sint32T sockFdSetRemove(zl303xx_SocketDataS *pSktData, FD_SELECT_SET *fdSet)
{
   Sint32T status = 0;

   if (pSktData == NULL)
   {
      ZL303XX_ERROR_TRAP("Invalid socket info structure pointer");
      status = -1;
   }

   if (status == 0)
   {
      if (fdSet == NULL)
      {
         ZL303XX_ERROR_TRAP("Invalid FD_SELECT_SET pointer");
         status = -1;
      }
   }

   if (status == 0)
   {
      FD_CLR((UINT_T)pSktData->socketFd, fdSet);
   }

   return status;
}

/**
  Function Name:
   sockFdIsInSet

  Details:
   Determines whether a particular socket file descriptor is in an FD set

  Parameters:
       pSktData    An identifier for a previously established and configured socket connection.
        fdSet       Pointer to a FD_SELECT_SET

  Return Value:
   Zero (FALSE) if the file descriptor is not in the FD set, non-zero (TRUE) if it
   is

  Notes:
   Does not perform validity checks on its parameters

*******************************************************************************/
Sint32T sockFdIsInSet(zl303xx_SocketDataS *pSktData, FD_SELECT_SET *fdSet)
{
   return FD_ISSET((UINT_T)pSktData->socketFd, fdSet);
}

/**

  Function Name:
   sockSelectRd

  Details:
   Blocks indefinitely until one of the sockets in fdSet becomes ready due to
   data becoming available.

  Parameters:
       fdSet       Pointer to a FD_SELECT_SET of socket descriptors of interest

       fdSet       Pointer to a FD_SELECT_SET that indicates sockets that are ready
               to be read

  Return Value:
   >0 indicates the number of sockets ready to be read
   or -1 indicates an error occurred

  Notes:
   Does not perform validity checks on its parameters

*******************************************************************************/
Sint32T sockSelectRd(FD_SELECT_SET *fdSet)
{
   Sint32T status = 0;
   Sint32T readsocks;
   timeOutS timeOut = {ZL303XX_SOCKET_SELECT_TIMEOUT_SEC,
                       ZL303XX_SOCKET_SELECT_TIMEOUT_USEC};

   while (status == 0)
   {
      /* Only interested in sockets which are ready to read. */
      readsocks = select(FD_SETSIZE,
                         fdSet,              /* rdsocks */
                         NULL,               /* wrsocks */
                         NULL,               /* exceptionsocks */
                         (void *)&timeOut);  /* timeout */

      if (readsocks == SOCKET_ERROR)
      {
         if (errno == EINTR)
         {
         #ifndef OS_LINUX
             ZL303XX_TRACE_ALWAYS("Socket select interrupted",0,0,0,0,0,0);
         #endif
         #ifdef OS_LINUX
         ZL303XX_TRACE(ZL303XX_MOD_ID_TRNSPRT_LYR, 1, "Socket select interrupted",0,0,0,0,0,0);
             return 0;
         #endif
         }
         else
         {
         #ifdef OS_LINUX
             if (errno == 0)
             {
                 ZL303XX_TRACE_ALWAYS("Socket select interrupted errno=%d", errno,0,0,0,0,0);
                 return 0;
             }
         #endif
            /* Record the last error in case we want to inspect it later */
/*            ZL303XX_ERROR_TRAP("Socket select error"); */
         }
         Zl303xx_LastSocketErr = errno;
         status = -1;
      }
      else if (readsocks > 0)
      {
         status = readsocks;
      }
      else
      {
         /* We have been timed out or interrputed, so return to the calling
            function in case something has changed. (ie. new sockets added) */
         break;
      }
   }

   return status;
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/**

  Function Name:
    zl303xx_LxSTInit

  Details:
   Initialises communication paths to the ZL303XX_ device through the
   Linux socket transport layer

  Parameters:
       pSktConnectData   Pointer to zl303xx_SocketDataS containing information
                     about the connection

Structure inputs:

 The following information must be provided in the connection data structure
 prior to this function call.
      mcastAddr   The address to use for sending packets
      type        The connection mode for the socket (unicast, multicast, broadcast)

        socketFd       A socket descriptor;
        ifaceName      The name of the interface to use
        addrFamily     The address family to use to interpret addresses
        localIpAddr    The actual IP address of the interface used
        macAddress     The hardware address of the interface
        netMask        The subnet mask for the interface

  Return Value:
   0 (OK) or -1 if an error occurs

*******************************************************************************/
Sint32T zl303xx_LxSTInit(zl303xx_SocketDataS *pSktConnectData)
{
   Sint32T status = 0;
   struct ip_mreq mreq;

   if (pSktConnectData == NULL)
   {
      ZL303XX_ERROR_TRAP("Invalid connection info structure pointer");
      status = -1;
   }

   if (status == 0)
   {
      /* Create a nonUDP socket to be used for bidirectional communication through the ZL303XX_ device */
      pSktConnectData->socketFd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

      if (pSktConnectData->socketFd == OS_ERROR)
      {
         pSktConnectData->socketFd = INVALID_SOCKET;
         ZL303XX_TRACE_ALWAYS("Unable to open socket : %ld", errno, 0, 0, 0, 0, 0);
         Zl303xx_LastSocketErr = errno;
         status = -1;
      }
      else
      {
         pSktConnectData->addrFamily = AF_INET;
      }
   }

   if (status == 0)
   {
      /* Get the interface parameters.  */
      if (zl303xx_LxGetIface(pSktConnectData) < 0)
      {
         ZL303XX_ERROR_NOTIFY("Unable to get interface parameters");
         status = -1;
      }
   }

   if (status == 0)
   {  /* Configure this socket for address reuse . This is required because we
         connect several sockets to the same address */
      Sint32T reuseAddr = 1;
      if (setsockopt(pSktConnectData->socketFd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuseAddr, sizeof(SINT_T)) == OS_ERROR)
      {
         ZL303XX_TRACE_ALWAYS("Unable to configure socket for address reuse: %ld", errno, 0, 0, 0, 0, 0);
         Zl303xx_LastSocketErr = errno;
         status = -1;
      }
   }

   if (status == 0)
   {  /* Configure this socket as non-blocking */
      Uint32T onOff = 1;

      if (ioctl(pSktConnectData->socketFd, FIONBIO, (void *)&onOff) == OS_ERROR)
      {
         ZL303XX_TRACE_ALWAYS("Unable to configure socket as non-blocking: %d", errno, 0, 0, 0, 0, 0);
         Zl303xx_LastSocketErr = errno;
         status = -1;
      }
   }

   if (status == 0)
   {
      if (pSktConnectData->type != ZL303XX_TRANSPORT_RAWCAST)
      {
         /* Bind this socket to the required host Ethernet interface on the specified port.
            It is assumed that if multicast is required that the requested interface is multicast
            capable. This is not checked */
         struct sockaddr_in socketAddr;

         memset(&socketAddr, 0, sizeof(struct sockaddr_in));

         socketAddr.sin_family = pSktConnectData->addrFamily;
         if (pSktConnectData->type == ZL303XX_TRANSPORT_MULTICAST)
         {
            /* Bind to the multicast address */
            memcpy(&socketAddr.sin_addr.s_addr, pSktConnectData->mcastAddr,
                   sizeof(socketAddr.sin_addr.s_addr));
         }
         else
         {  /* For unicast and broadcast messages, bind to the actual address */
            memcpy(&socketAddr.sin_addr.s_addr, pSktConnectData->localIpAddr,
                   sizeof(socketAddr.sin_addr.s_addr));
         }
         socketAddr.sin_port = htons(pSktConnectData->port);

         if (bind(pSktConnectData->socketFd, (struct sockaddr *)(void *)&socketAddr, sizeof(struct sockaddr_in)) == OS_ERROR)
         {
            ZL303XX_TRACE_ALWAYS("Unable to bind socket : %ld", errno, 0, 0, 0, 0, 0);
            Zl303xx_LastSocketErr = errno;
            status = -1;
         }
      }
   }

   /***************** Configure the mode specific behaviour for the socket *************/

   if (status == 0)
   {
      if (pSktConnectData->type == ZL303XX_TRANSPORT_MULTICAST)
      {
         if (status == 0)
         {
            /* Note that Linux reference documentation defines the parameter type for this option as 'int'
               but this does not work and is contradicted by some web documents, e.g.
               http://www-kryo.desy.de/documents/vxWorks/V5.4/vxworks/netguide/c-netapi2.html */
            char ttl = (char)ZL303XX_SOCKET_MULTICAST_TTL;
            if (setsockopt(pSktConnectData->socketFd, IPPROTO_IP, IP_MULTICAST_TTL,
                     &ttl, sizeof(ttl)) == OS_ERROR)
            {
               ZL303XX_TRACE_ALWAYS("Unable to set time to live: %ld", errno, 0, 0, 0, 0, 0);
               Zl303xx_LastSocketErr = errno;
               status = -1;
            }
         }

         /***************** Configure the multicast receive mode ******************************/
         if (status == 0)
         {  /* Add socket as a multicast receiver using the specified interface.
               The multicast address will be the same as the transmit address */
            memset(&mreq, 0, sizeof(mreq));
            memcpy(&mreq.imr_multiaddr.s_addr, pSktConnectData->mcastAddr,
                   sizeof(mreq.imr_multiaddr.s_addr));
            memcpy(&mreq.imr_interface.s_addr, pSktConnectData->localIpAddr,
                   sizeof(mreq.imr_interface.s_addr));

            if (setsockopt(pSktConnectData->socketFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) == OS_ERROR)
            {
               ZL303XX_TRACE_ALWAYS("Unable to add to multicast receive group: %ld", errno, 0, 0, 0, 0, 0);
               Zl303xx_LastSocketErr = errno;
               status = -1;
            }
         }
      }
      else if (pSktConnectData->type == ZL303XX_TRANSPORT_BROADCAST)
      {
         if (status == 0)
         {   /* Enable this socket for broadcast transmits */
            Sint32T bcastTx = 1;
            if (setsockopt(pSktConnectData->socketFd, SOL_SOCKET, SO_BROADCAST, (char *)&bcastTx, sizeof(SINT_T)) == OS_ERROR)
            {
               ZL303XX_TRACE_ALWAYS("Unable to configure socket for broadcast address: %ld", errno, 0, 0, 0, 0, 0);
               Zl303xx_LastSocketErr = errno;
               status = -1;
            }
         }
      }
      else if (pSktConnectData->type == ZL303XX_TRANSPORT_UNICAST)
      {
         if (status == 0)
         {  /* Enable this socket for multicast transmits. We use the unicast socket for all transmits,
               including multicasts, because we want to use the correct local source IP address */
            if (setsockopt(pSktConnectData->socketFd, IPPROTO_IP, IP_MULTICAST_IF,
                  (char *)pSktConnectData->localIpAddr, sizeof(struct in_addr)) == OS_ERROR)
            {
               ZL303XX_TRACE_ALWAYS("Unable to configure multicast socket : %ld", errno, 0, 0, 0, 0, 0);
               Zl303xx_LastSocketErr = errno;
               status = -1;
            }

            if (status == 0)
            {
                /* Note that Linux reference documentation defines the parameter type for this option as 'int'
                   but this does not work and is contradicted by some web documents, e.g.
                   http://www-kryo.desy.de/documents/vxWorks/V5.4/vxworks/netguide/c-netapi2.html */
                char ttl = (char)ZL303XX_SOCKET_UNICAST_TTL;
                if ( (setsockopt (pSktConnectData->socketFd, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl,
                            sizeof(ttl))) < 0)
                {
                    ZL303XX_TRACE_ALWAYS("Unable to set time to live: %ld", errno, 0, 0, 0, 0, 0);
                    Zl303xx_LastSocketErr = errno;
                    status = -1;
                }
            }
         }
      }
   }

   return status;
}

/**

  Function Name:
   zl303xx_LxSTClose

  Details:
   Closes communication path to the socket for Linux Socket Transport layer

  Parameters:
       pSktConnectData   Pointer to structure containing information
                           about the connection
                           See zl303xx_LxSTInit() for further details

  Return Value:
   0 (OK) or -1 if an error occurs

*******************************************************************************/
Sint32T zl303xx_LxSTClose(zl303xx_SocketDataS *pSktConnectData)
{
   Sint32T status = 0;

   if (pSktConnectData == NULL)
   {
      ZL303XX_ERROR_TRAP("Invalid connection info structure pointer");
      status = -1;
   }

   if (status == 0)
   {
      if (pSktConnectData->socketFd != INVALID_SOCKET)
      {
          ZL303XX_TRACE(ZL303XX_MOD_ID_TRNSPRT_LYR, 1, "Closing socket", 0, 0, 0, 0, 0, 0);


         /*****************  Receive connection closedown **********************/
         if (pSktConnectData->type == ZL303XX_TRANSPORT_MULTICAST)
         {  /* Remove the socket from the multicast group. Ignore any errors */
            struct ip_mreq mreq;
            memset(&mreq, 0, sizeof(mreq));
            memcpy(&mreq.imr_multiaddr.s_addr, pSktConnectData->mcastAddr,
                   sizeof(mreq.imr_multiaddr.s_addr));
            memcpy(&mreq.imr_interface.s_addr, pSktConnectData->localIpAddr,
                   sizeof(mreq.imr_interface.s_addr));

            (void)setsockopt(pSktConnectData->socketFd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&mreq, sizeof(struct ip_mreq));
         }

         /*****************  Transmit connection closedown **********************/
         /* Nothing to do for transmit */

         /* Finally close the socket connection */
         (void)close(pSktConnectData->socketFd);
      }
   }

   return status;
}

/* osGetMacAddr */
/**
   Returns the MAC address of the specified network interface.

  Parameters:
   [in]   ifName   Name of the network interface.
   [out]  macAddr  MAC address of the network interface.

  Return Value:
     0   Success
     -1  Error occurred.

*******************************************************************************/
Sint32T osGetMacAddr(const char *ifName, Uint8T *macAddr)
{
   Sint32T status = 0;

   ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 1, "osGetMacAddr:", 0,0,0,0,0,0);

   /* Check input parameters. */
   if (status == 0)
   {
      if (ifName == NULL)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) ifName is NULL.", 0,0,0,0,0,0);
         status = -1;
      }
      else if (strlen(ifName) >= IFNAMSIZ)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) ifName length > %u", IFNAMSIZ - 1, 0,0,0,0,0);
      }
      if (macAddr == NULL)
      {
         ZL303XX_TRACE_ERROR("   (ERROR) macAddr is NULL.", 0,0,0,0,0,0);
         status = -1;
      }
   }

   {
      struct ifreq ifReq;
      Sint32T socketFd = 0;

      if (status == 0)
      {
         /* Clear the ifreq structure. */
         memset(&ifReq, 0, sizeof(ifReq));

         /* Copy name into the ifreq struct as it is required for the
            following ioctl calls */
         strncpy(ifReq.ifr_name, ifName, IFNAMSIZ-1);
         ifReq.ifr_name[IFNAMSIZ-1] = '\0';

         /* Open a socket. */
         socketFd = socket(AF_INET, SOCK_DGRAM, 0);
         if (socketFd < 0)
         {
            ZL303XX_TRACE_ERROR(
                  "   (ERROR) Socket Create Failed (errno=%d).",
                  OS_ERRNO, 0,0,0,0,0);
            status = -1;
         }

         /* Get the MAC address. */
         else
         {
            if (ioctl(socketFd, SIOCGIFHWADDR, (void *)&ifReq) < 0)
            {
               ZL303XX_TRACE_ERROR("Unable to get MAC Address for %s!",
                                   ifName, 0,0,0,0,0);
               status = -1;
            }

            /* Close the socket. */
            close(socketFd);
         }
      }

      /* Copy the MAC address into the provided buffer. */
      if (status == 0)
      {
         memcpy(macAddr, ifReq.ifr_addr.sa_data, ZL303XX_MAC_ADDRESS_BYTES);
      }
   }

   return status;
}

/**

  Function Name:
   zl303xx_LxGetIface

  Details:
   Get the name and communication parameters of a suitable ethernet interface to
   use. This is called if no particular interface is specified by the application

  Parameters:
       pSktConnectData   Pointer to a socket connection data structure

Structure inputs:

        pSktConnectData->socketFd  must be set to a previously created socket identifier
        ifaceName         Optionally set to the name of the desired interface to use

        ifaceName         Set to the name of a suitable interface if
                           one could be found
        localIpAddr       Set to the IP address of the interface.
        macAddr           The hardware MAC address.
        netMask           The subnet mask for the interface.

  Return Value:
   0 (OK) or -1 if no suitable interface can be found

*******************************************************************************/
Sint32T zl303xx_LxGetIface(zl303xx_SocketDataS *pSktConnectData)
{
  struct ifconf data;
  struct ifreq device[IFCONF_LENGTH];
  Uint8T i = 0, addrTest = 0;
  Sint32T flags;

  data.ifc_len = sizeof(device);
  data.ifc_req = device;

  memset(data.ifc_buf, 0, (Uint32T)data.ifc_len);

  flags = IFF_UP|IFF_RUNNING;

  if (pSktConnectData->type == ZL303XX_TRANSPORT_MULTICAST)
  {
     flags |= IFF_MULTICAST;
  }

  /* Test if localIpAddr has been explicitly set. */
  for (i = 0; i < ZL303XX_SOCKET_ADDR_IPV4_LEN; i++)
  {
     addrTest |= pSktConnectData->localIpAddr[i];
  }

  /* look for an interface if none specified */
  if(pSktConnectData->ifaceName[0] == '\0')
  {
    /* no iface specified */
    /* get list of network interfaces*/
    if(ioctl(pSktConnectData->socketFd, (SINT_T)SIOCGIFCONF, (void *)&data) < 0)
    {
      ZL303XX_ERROR_TRAP("Failed to get network interface list");
      return -1;
    }

    if(data.ifc_len == sizeof(device))
    {
      ZL303XX_TRACE_ALWAYS("Device list may exceed allocated space",0,0,0,0,0,0);
    }

    /* search through interfaces */
    for (i = 0; i < data.ifc_len/(SINT_T)sizeof(device[0]); ++i)
    {
       /* Debugging output. Display any interface device we find  */
       ZL303XX_TRACE(ZL303XX_MOD_ID_TRNSPRT_LYR, 2, "%d %s",i,device[i].ifr_name, 0,0,0,0);

      if (ioctl(pSktConnectData->socketFd, (SINT_T)SIOCGIFFLAGS, (void *)&device[i]) < 0)
      {
        ZL303XX_ERROR_TRAP("Failed to get device flags");
        return -1;
      }
      else if (memcmp(INTERFACE_PREFIX, device[i].ifr_name, strlen(INTERFACE_PREFIX)))
      {
         /* This is not the one we are looking for */
         continue;
      }
      else if (((device[i].ifr_flags)&flags) == flags )
      {
        /* found a suitable interface */
        strncpy(pSktConnectData->ifaceName, device[i].ifr_name, ZL303XX_IFACE_NAME_MAX_LEN);
        pSktConnectData->ifaceName[ZL303XX_IFACE_NAME_MAX_LEN] = '\0';   /* Ensure it is terminated */
        break;
      }
    }

    if(pSktConnectData->ifaceName[0] == '\0')
    {
      ZL303XX_ERROR_NOTIFY("Failed to find an ethernet interface");
      return -1;
    }
  }
  else
  {
     /* iface specified. Copy name into the ifreq struct as it is required for the
       following ioctl calls */
     strncpy(device[i].ifr_name, pSktConnectData->ifaceName, IFNAMSIZ-1);
     device[i].ifr_name[IFNAMSIZ-1] = '\0';
  }

  /* get ip address if it has not been explicitly specified */
  if (addrTest == 0)
  {
     if(ioctl(pSktConnectData->socketFd, (SINT_T)SIOCGIFADDR, (void *)&device[i]) < 0)
     {
       ZL303XX_ERROR_TRAP("Failed to get ip address");
       return -1;
     }
     else
     {
       memcpy(pSktConnectData->localIpAddr,
              &((struct sockaddr_in *)(void *)&device[i].ifr_addr)->sin_addr,
              sizeof(struct in_addr));
     }
  }

    {
#if defined (__VXWORKS_65) || defined (OS_LINUX)
      /* Get the MAC address of the ethernet port */
    #if defined (__VXWORKS_65)
      if (ioctl (pSktConnectData->socketFd, SIOCGIFLLADDR, (void *)&device[i]) < 0)
    #else
      if (ioctl (pSktConnectData->socketFd, SIOCGIFHWADDR, (void *)&device[i]) < 0)
    #endif
      {
          ZL303XX_ERROR_TRAP("Unable to get MAC Address!");
      }
      memcpy(pSktConnectData->macAddr, device[i].ifr_addr.sa_data, ZL303XX_MAC_ADDRESS_BYTES);

      /* Get the netmask */
      if (ioctl (pSktConnectData->socketFd, SIOCGIFNETMASK, (void *)&device[i]) < 0)
      {
          ZL303XX_ERROR_TRAP("Unable to get NetMask!");
      }
      pSktConnectData->netMask = ((struct sockaddr_in *)(void*)&device[i].ifr_addr)->sin_addr.s_addr;
#else
      /* Get the MAC address of the ethernet port. */
      {

         if ((pSktConnectData->pIf = ifunit(pSktConnectData->ifaceName)) == NULL)
         {
           ZL303XX_ERROR_TRAP("Failed to get ifnet info");
           return -1;
         }
         else
         {
           if (!(pSktConnectData->pIf->if_flags & IFF_POINTOPOINT) &&
               !(pSktConnectData->pIf->if_flags & IFF_LOOPBACK))
           {
             struct arpcom * pEtherIf = (struct arpcom *)pSktConnectData->pIf;
             memcpy(pSktConnectData->macAddr, pEtherIf->ac_enaddr, ZL303XX_MAC_ADDRESS_BYTES);
           }
           else
           {
             ZL303XX_ERROR_TRAP("Could not determine HW address");
             return -1;
           }
         }
      }
      /* Get the netmask */
      if (ifMaskGet(pSktConnectData->ifaceName, (SINT_T *)&pSktConnectData->netMask) == ERROR)
      {
         ZL303XX_ERROR_TRAP("Unable to get net mask");
         return -1;
      }
#endif
    }

  ZL303XX_TRACE(ZL303XX_MOD_ID_TRNSPRT_LYR, 1, "Device \"%s\" IP= %08X NetMask= %08X", pSktConnectData->ifaceName,
                *(Uint32T *)(void *)pSktConnectData->localIpAddr, pSktConnectData->netMask, 0,0,0);
  ZL303XX_TRACE(ZL303XX_MOD_ID_TRNSPRT_LYR, 1, "Device address= %02x:%02x:%02x:%02x:%02x:%02x",
              pSktConnectData->macAddr[0], pSktConnectData->macAddr[1], pSktConnectData->macAddr[2],
              pSktConnectData->macAddr[3], pSktConnectData->macAddr[4], pSktConnectData->macAddr[5]);
  return 0;
}

#ifdef ZL_INCLUDE_IPV6_SOCKET
/* sockCreateNonBlock6 */
/**
   Creates a bidirectional IPv6 communication path to the ZL303XX_ device through
   the Linux socket transport layer.

  Parameters:
   [out]  pSktData  Pointer to an uninitialised structure for the socket.
   [in]   ifName    The name of the local interface to use.
   [in]   initData  Structure of type sockAddrData6S containing application
                         supplied initialisation data.
   [in]   port      Port number to create.

  Return Value:
      0  Success.
     -1  An error occurred.

*******************************************************************************/
Sint32T sockCreateNonBlock6(zl303xx_SocketDataS *pSktData, const char *ifName,
                            sockAddrData6S *initData, Uint16T port)
{
   Sint32T status = 0;
   const char *fnName = "sockCreateNonBlock6";

   /* Check input parameters. */
   if (status == 0)
   {
      if (pSktData == NULL)
      {
         ZL303XX_TRACE_ERROR("%s: Invalid socket data structure pointer.", fnName, 0,0,0,0,0);
         status = -1;
      }
      else if (ifName == NULL)
      {
         ZL303XX_TRACE_ERROR("%s: Invalid interface name.", fnName, 0,0,0,0,0);
         status = -1;
      }
      else if (initData == NULL)
      {
         ZL303XX_TRACE_ERROR("%s: Invalid initialisation data structure pointer.", fnName, 0,0,0,0,0);
         status = -1;
      }
   }

   /* Check if the destination address is valid or necessary. */
   if (status == 0)
   {
      if (initData->ifType == ZL303XX_TRANSPORT_MULTICAST)
      {
         /* Check that the supplied address is a valid multicast */
         if (IN6_IS_ADDR_MULTICAST(initData->destAddr))
         {
            /* Address is a class D, multicast address, i.e. top byte is in
             * range 224-239 inclusive */
            memcpy(pSktData->mcastAddr, initData->destAddr, sizeof(initData->destAddr));
         }
         else
         {
            ZL303XX_TRACE_ERROR("%s: Non-multicast address supplied.", fnName, 0,0,0,0,0);
            status = -1;
         }
      }
      else if (initData->ifType == ZL303XX_TRANSPORT_BROADCAST)
      {
         ZL303XX_TRACE_ERROR("%s: IPv6 does not implement broadcasting.", fnName, 0,0,0,0,0);
         status = -1;
      }
      else if (initData->ifType == ZL303XX_TRANSPORT_UNICAST)
      {
         /* Nothing to do. The mcastAddr data member will be unused */
      }
   }

   if (status == 0)
   {
      /* Set the data members in the socket data structure */
      /* Store the port and the interface type */
      pSktData->port = port;
      pSktData->type = initData->ifType;
      memcpy(&pSktData->localIpAddr, &initData->srcAddr, sizeof(initData->srcAddr));

      memset(pSktData->ifaceName, 0, sizeof(pSktData->ifaceName));
      strncpy(pSktData->ifaceName, ifName, sizeof(pSktData->ifaceName) - 1);

      pSktData->socketFd = INVALID_SOCKET;
   }

   /* Call the initialisation function */
   if (status == 0)
   {
      status = zl303xx_LxSTInit6(pSktData);
   }

   return status;
}

/* zl303xx_LxSTInit6 */
/**
   Creates and configures an IPv6 socket.

  Parameters:
   [in,out]  pSkt  Pointer to zl303xx_SocketDataS containing information about
                        the connection.

  Return Value:
      0  Success.
     -1  An error occurred.

*******************************************************************************/
static Sint32T zl303xx_LxSTInit6(zl303xx_SocketDataS *pSkt)
{
   Sint32T status = 0, ifIndex;
   Uint32T bindRetry;

   /* Arbitrary values used to give the Duplicate Address Detection (DAD) for
    * IPv6 time to complete while attempting to call bind(). */
   #define IPV6_BIND_RETRY_MAX       10
   #define IPV6_BIND_RETRY_DELAY_MS  250

   if (status == 0)
   {
      /* Create a UDP socket to be used for bidirectional communication through
       * the ZL303XX_ device */
      pSkt->socketFd = socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP);

      if (pSkt->socketFd == OS_ERROR)
      {
         pSkt->socketFd = INVALID_SOCKET;
         ZL303XX_TRACE_ALWAYS("Unable to open socket : %ld", errno, 0, 0, 0, 0, 0);
         Zl303xx_LastSocketErr = errno;
         status = -1;
      }
      else
      {
         pSkt->addrFamily = AF_INET6;
      }
   }

   /* Get the interface parameters.  */
   if (status == 0)
   {
      status = zl303xx_LxGetIface6(pSkt);
   }

   if (status == 0)
   {  /* Configure this socket for address reuse . This is required because we
         connect several sockets to the same address */
      Sint32T reuseAddr = 1;
      if (setsockopt(pSkt->socketFd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuseAddr, sizeof(SINT_T)) == OS_ERROR)
      {
         ZL303XX_TRACE_ALWAYS("Unable to configure socket for address reuse: %ld", errno, 0, 0, 0, 0, 0);
         Zl303xx_LastSocketErr = errno;
         status = -1;
      }
   }

   if (status == 0)
   {  /* Configure this socket as non-blocking */
      Uint32T onOff = 1;

      if (ioctl(pSkt->socketFd, FIONBIO, (void *)&onOff) == OS_ERROR)
      {
         ZL303XX_TRACE_ALWAYS("Unable to configure socket as non-blocking: %d", errno, 0, 0, 0, 0, 0);
         Zl303xx_LastSocketErr = errno;
         status = -1;
      }
   }

   if (status == 0)
   {
      ifIndex = sockGetIfIndex(pSkt->ifaceName, pSkt->socketFd);
   }

   if (status == 0)
   {
      /* Bind this socket to the required host Ethernet interface on the
       * specified port. It is assumed that if multicast is required that the
       * requested interface is multicast capable. This is not checked */
      struct sockaddr_in6 socketAddr;

      memset(&socketAddr, 0, sizeof(socketAddr));

      socketAddr.sin6_family = pSkt->addrFamily;
      if (pSkt->type == ZL303XX_TRANSPORT_MULTICAST)
      {
         /* Bind to the multicast address */
         memcpy(socketAddr.sin6_addr.s6_addr, pSkt->mcastAddr,
                sizeof(socketAddr.sin6_addr.s6_addr));
      }
      else
      {  /* For unicast and broadcast messages, bind to the actual address */
         memcpy(socketAddr.sin6_addr.s6_addr, pSkt->localIpAddr,
                sizeof(socketAddr.sin6_addr.s6_addr));
      }
      socketAddr.sin6_port = htons(pSkt->port);

      /* It is possible for the call to bind() to fail if it is executed too
       * soon after adding an IPv6 address to an interface. This is because the
       * default behaviour in Linux is to bring the address up in a "tentative"
       * state while waiting for the Duplicate Address Detection (DAD) mechanism
       * to complete. Afterward, the address will be in the "permanent" state.
       *
       * More information can be found in:
       * IETF RFC 4429: Optimistic Duplicate Address Detection (DAD) for IPv6
       * Linux source /net/ipv6/addrconf.c, function ipv6_addr_add()
       *
       * To get around this issue, we keep calling bind() until it succeeds,
       * a failure other than EADDRNOTAVAIL occurs, or the retry count is
       * exceeded. */
      for (bindRetry = IPV6_BIND_RETRY_MAX; bindRetry > 0; bindRetry--)
      {
         status = bind(pSkt->socketFd, (struct sockaddr *)(void *)&socketAddr,
                       sizeof(socketAddr));

         if ((status == 0) || (errno != EADDRNOTAVAIL))
         {
            break;
         }
         else
         {
            OS_TASK_DELAY(IPV6_BIND_RETRY_DELAY_MS);
         }
      }

      if (status != 0)
      {
         ZL303XX_TRACE_ALWAYS("Unable to bind socket : %ld", errno, 0, 0, 0, 0, 0);
         Zl303xx_LastSocketErr = errno;
      }
   }

   /***************** Configure the mode specific behaviour for the socket *************/
   if (pSkt->type == ZL303XX_TRANSPORT_MULTICAST)
   {
      if (status == 0)
      {
         /* Note that Linux reference documentation defines the parameter type for this option as 'int'
            but this does not work and is contradicted by some web documents, e.g.
            http://www-kryo.desy.de/documents/vxWorks/V5.4/vxworks/netguide/c-netapi2.html */
         int hops = ZL303XX_SOCKET_MULTICAST_TTL;
         if (setsockopt(pSkt->socketFd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS,
                        &hops, sizeof(hops)) == OS_ERROR)
         {
            ZL303XX_TRACE_ALWAYS("Unable to set multicast hop limit: %ld", errno, 0,0,0,0,0);
            Zl303xx_LastSocketErr = errno;
            status = -1;
         }
      }

      if (status == 0)
      {
         struct ipv6_mreq mreq;

         /* Add socket as a multicast receiver using the specified interface.
            The multicast address will be the same as the transmit address */
         memcpy(mreq.ipv6mr_multiaddr.s6_addr, pSkt->mcastAddr,
                sizeof(mreq.ipv6mr_multiaddr.s6_addr));
         mreq.ipv6mr_interface = ifIndex;
         if (setsockopt(pSkt->socketFd, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP,
                        (char *)&mreq, sizeof(mreq)) == OS_ERROR)
         {
            ZL303XX_TRACE_ALWAYS("Unable to add to multicast receive group: %ld", errno, 0,0,0,0,0);
            Zl303xx_LastSocketErr = errno;
            status = -1;
         }
      }
   }
   else if (pSkt->type == ZL303XX_TRANSPORT_UNICAST)
   {
      if (status == 0)
      {  /* Enable this socket for multicast transmits. We use the unicast socket for all transmits,
            including multicasts, because we want to use the correct local source IP address */
         if (setsockopt(pSkt->socketFd, IPPROTO_IPV6, IPV6_MULTICAST_IF,
                        &ifIndex, sizeof(ifIndex)) == OS_ERROR)
         {
            ZL303XX_TRACE_ALWAYS("Unable to configure multicast socket : %ld", errno, 0,0,0,0,0);
            Zl303xx_LastSocketErr = errno;
            status = -1;
         }
      }

      if (status == 0)
      {
          /* Note that Linux reference documentation defines the parameter type for this option as 'int'
             but this does not work and is contradicted by some web documents, e.g.
             http://www-kryo.desy.de/documents/vxWorks/V5.4/vxworks/netguide/c-netapi2.html */
          int hops = ZL303XX_SOCKET_UNICAST_TTL;
          if (setsockopt(pSkt->socketFd, IPPROTO_IPV6, IPV6_UNICAST_HOPS,
                         &hops, sizeof(hops)) < 0)
          {
              ZL303XX_TRACE_ALWAYS("Unable to set unicast hop limit: %ld", errno, 0,0,0,0,0);
              Zl303xx_LastSocketErr = errno;
              status = -1;
          }
      }
   }

   return status;
}

/* zl303xx_LxSTClose6 */
/**
   Closes an IPv6 socket.

  Parameters:
   [in]   pSktConnectData  Pointer to structure containing information about
                                the connection.

  Return Value:
      0  Success.
     -1  An error occurred.

*******************************************************************************/
static Sint32T zl303xx_LxSTClose6(zl303xx_SocketDataS *pSkt)
{
   Sint32T status = 0;

   if (status == 0)
   {
      if (pSkt->socketFd != INVALID_SOCKET)
      {
          ZL303XX_TRACE(ZL303XX_MOD_ID_TRNSPRT_LYR, 1, "Closing socket", 0, 0, 0, 0, 0, 0);

         /*****************  Receive connection closedown **********************/
         if (pSkt->type == ZL303XX_TRANSPORT_MULTICAST)
         {  /* Remove the socket from the multicast group. Ignore any errors */
            struct ipv6_mreq mreq;

            memcpy(mreq.ipv6mr_multiaddr.s6_addr, pSkt->mcastAddr,
                   sizeof(mreq.ipv6mr_multiaddr.s6_addr));
            mreq.ipv6mr_interface = 0;

            (void)setsockopt(pSkt->socketFd, IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP,
                             (char *)&mreq, sizeof(mreq));
         }

         /* Finally close the socket connection */
         (void)close(pSkt->socketFd);
      }
   }

   return status;
}

/**

  Function Name:
   zl303xx_LxGetIface6

  Details:
   Get the name and communication parameters of a suitable ethernet interface to
   use. This is called if no particular interface is specified by the application

  Parameters:
       pSktConnectData   Pointer to a socket connection data structure

  Return Value:
   0 (OK) or -1 if no suitable interface can be found

*******************************************************************************/
static Sint32T zl303xx_LxGetIface6(zl303xx_SocketDataS *pSkt)
{
   Sint32T status = 0;

   if (status == 0)
   {
      pSkt->netMask = 0;
      status = osGetMacAddr(pSkt->ifaceName, pSkt->macAddr);
   }

   if (status == 0)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRNSPRT_LYR, 1, "Device \"%s\"",
                    pSkt->ifaceName, 0,0,0,0,0);
      ZL303XX_TRACE(ZL303XX_MOD_ID_TRNSPRT_LYR, 1, "Device address= %02x:%02x:%02x:%02x:%02x:%02x",
                    pSkt->macAddr[0], pSkt->macAddr[1], pSkt->macAddr[2],
                    pSkt->macAddr[3], pSkt->macAddr[4], pSkt->macAddr[5]);
   }

   return status;
}

/* sockSend6 */
/**
   Sends a buffer to the previously configured socket connection. Can be used to
   transmit over IPv4 and IPv6 sockets.

  Parameters:
   [in]   pSktData  An identifier for a previously established and configured
                         connection.
   [in]   destAddr  The destination address to send the packets.
   [in]   buf       A pointer to the buffer to send.
   [in]   nBytes    The number of bytes to send.

   [out]  nBytes    The number of bytes actually sent.

  Return Value:
     Sint32T  The number of bytes sent.
     -1       An error occurred.

*******************************************************************************/
Sint32T sockSend6(zl303xx_SocketDataS *pSktData, Uint8T *destAddr, Uint8T *buf,
                  Uint32T *nBytes)
{
   Sint32T ret = 0;
   const char *fnName = "sockSend6";
   Sint32T numBytes = 0;

   /* Check input parameters. */
   if (ret == 0)
   {
      if (pSktData == NULL)
      {
         ZL303XX_TRACE_ERROR("%s: Invalid socket info structure pointer.",
                             fnName, 0,0,0,0,0);
         ret = -1;
      }
   }

   if (ret == 0)
   {
      if (pSktData->addrFamily == AF_INET6)
      {
         struct sockaddr_in6 addr6;

         addr6.sin6_family = pSktData->addrFamily;
         addr6.sin6_port   = htons(pSktData->port);
         memcpy(addr6.sin6_addr.s6_addr, destAddr, sizeof(addr6.sin6_addr.s6_addr));

         numBytes = sendto(pSktData->socketFd, (char *)buf, *nBytes, 0,
                           (struct sockaddr *)(void *)&addr6, sizeof(addr6));
      }
      else
      {
         struct sockaddr_in addr;

         addr.sin_family = pSktData->addrFamily;
         addr.sin_port = htons(pSktData->port);
         memcpy(&addr.sin_addr.s_addr, destAddr, sizeof(addr.sin_addr.s_addr));

         numBytes = sendto(pSktData->socketFd, (char *)buf, *nBytes, 0,
                           (struct sockaddr *)(void *)&addr, sizeof(addr));
      }

      if (numBytes == OS_ERROR)
      {
         /* Record the last error in case we want to inspect it later */
         Zl303xx_LastSocketErr = errno;

         *nBytes = 0;   /* No bytes were sent (presumably) */

         if ((errno == EHOSTDOWN) ||
             (errno == EHOSTUNREACH) ||
             (errno == ENETUNREACH))
         {  /* Host is down or unreachable. Don't treat this as a fatal error. It simply means we can't see
               the remote end at this moment. Allow the system to keep trying to connect. */
            ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 1,
                          "Unable to send to socket: remote host is unreachable, error %ld",
                          errno, 0,0,0,0,0);
         }
         else
         {  /* Treat other errors as fatal */
            ZL303XX_TRACE_ALWAYS("Unable to send to socket : %ld", errno, 0, 0, 0, 0, 0);
            ret = -1;
         }
      }
   }

   if (ret != 0)
   {
      return ret;
   }

   return numBytes;
}

/* sockRecv6 */
/**
   Performs a read on the socket connection. Can be used to read IPv4 and IPv6
   sockets.

  Parameters:
   [in]   pSktData       A data structure for a previously established and
                              configured socket.
   [out]  buf            Filled with the received data
   [in]   buflen         Maximum size of buf.
   [out]  remoteAddr     The remote address from which the packet was received.
   [out]  remoteAddrLen  The number of bytes in the remote address.

  Return Value:
     Sint32T  The number of bytes received.
     -1       An error occurred.

  Notes:
   It is assumed in this application that the socket is configured as non-blocking

*******************************************************************************/
Sint32T sockRecv6(zl303xx_SocketDataS *pSktData, Uint8T *buf, Uint32T buflen,
                  Uint8T *remoteAddr, Uint16T *remoteAddrLen)
{
   Sint32T ret = 0;
   const char *fnName = "sockRecv6";
   Sint32T nBytes = 0;
   UINT_T fromLen;

   /* Check input parameters. */
   if (ret == 0)
   {
      if (pSktData == NULL)
      {
         ZL303XX_TRACE_ERROR("%s: Invalid socket info structure pointer.",
                             fnName, 0,0,0,0,0);
         ret = -1;
      }
      else if (remoteAddr == NULL)
      {
         ZL303XX_TRACE_ERROR("%s: Invalid address pointer.", fnName, 0,0,0,0,0);
         ret = -1;
      }
   }

   /* This implementation relies on the previous configuration of the socket to
      make it non-blocking */
   if (ret == 0)
   {
      if (pSktData->addrFamily == AF_INET6)
      {
         struct sockaddr_in6 fromAddr6;

         fromLen = sizeof(fromAddr6);
         nBytes = recvfrom(pSktData->socketFd, (char *)buf, buflen, 0,
                           (struct sockaddr *)(void *)&fromAddr6, &fromLen);

         memcpy(remoteAddr, &fromAddr6.sin6_addr.s6_addr, sizeof(fromAddr6.sin6_addr.s6_addr));
         *remoteAddrLen = sizeof(fromAddr6.sin6_addr.s6_addr);
      }
      else
      {
         struct sockaddr_in fromAddr;

         fromLen = sizeof(fromAddr);
         nBytes = recvfrom(pSktData->socketFd, (char *)buf, buflen, 0,
                           (struct sockaddr *)(void *)&fromAddr, &fromLen);

         memcpy(remoteAddr, &fromAddr.sin_addr.s_addr, sizeof(fromAddr.sin_addr.s_addr));
         *remoteAddrLen = sizeof(fromAddr.sin_addr.s_addr);
      }

      if (nBytes == OS_ERROR)
      {
         /* Record the last error in case we want to inspect it later */
         Zl303xx_LastSocketErr = errno;
      }
   }

   if (ret != 0)
   {
      return ret;
   }

   return nBytes;
}

/* sockMakeIdFromAddr6 */
/**
   Converts an IPv6 address string into an byte array in network order and
   extracts the prefix length.

  Parameters:
   [in]   addr    IPv6 address as a string.
   [out]  id      IPv6 address as a byte array.
   [out]  prefix  (Optional) Prefix length. If no prefix length is appended
                       to the address, 0 will be returned.

  Return Value:
      0  Success
     -1  An error occurred

*******************************************************************************/
Sint32T sockMakeIdFromAddr6(const char *addr, Uint8T *id, Uint32T *prefix)
{
   struct in6_addr in6;
   char temp[ZL303XX_INET_ADDR_MAX_LEN], *prefixStr;
   Uint32T prefixVal = 0;

   memset(temp, 0, sizeof(temp));
   strncpy(temp, addr, sizeof(temp) - 1);

   /* Get rid of the prefix, so inet_pton() doesn't fail. */
   prefixStr = strchr(temp, '/');
   if (prefixStr != NULL)
   {
      *prefixStr = '\0';
      prefixStr++;
      prefixVal = (Uint32T)atoi(prefixStr);
   }

   if (inet_pton(AF_INET6, temp, &in6) != 1)
   {
      ZL303XX_TRACE_ERROR("sockMakeIdFromAddr6: Unable to convert IPv6 address %s (%s).",
                          addr, temp, 0,0,0,0);
      return -1;
   }
   memcpy(id, in6.s6_addr, sizeof(in6.s6_addr));

   if (prefix != NULL)
      *prefix = prefixVal;

   return 0;
}

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
/* osSetIpv6Addr */
/**
   Sets the IPv6 address for a board interface that is driving the zl303xx
   hardware. Calling this function removes all non-link-local IPv6 addresses
   from the interface.

  Parameters:
   [in]   ifName     Name of the network interface.
   [in]   addr       The source IPV6 address to use as a string.

  Return Value:
   Nothing

  Notes:
   This function will only set the interface when it is run with root privileges.
   Also the function relies on the driver supporting the functionality.
   If the interface has already been configured, this function may fail.
   This function has a high degree of OS and hardware dependence.

*******************************************************************************/

/* Linux requires a (struct in6_ifreq) when calling the SIOCSIFADDR and
 * SIOCDIFADDR ioctl()s, instead of the normal (struct ifreq) so the prefix
 * length of the address can be used. However, this struct is defined in
 * linux/ipv6.h, which should not be included directly because:
 *    - This is a kernel header, so it's not meant for userspace applications
 *    - Including it causes conflicts with netinet/ip6.h
 *
 * So, copy the structure definition here for now. */
struct in6_ifreq
{
   struct in6_addr ifr6_addr;
   Uint32T ifr6_prefixlen;
   int ifr6_ifindex;
};

void osSetIpv6Addr(const char *ifName, const char *addr)
{
   const char *fnName = "osSetIpv6Addr";
   struct ifaddrs *ifaddr;
   Sint32T sock, ifIndex;
   struct in6_ifreq ifr6;
   struct sockaddr_in6 *sa6;
   Uint32T addrRemoved, plen, count;
   struct ifreq ifr;
   const Uint8T linkLocal[2] = {0xFE, 0x80};

   memset(&ifr6, 0, sizeof(ifr6));

   sock = socket(AF_INET6, SOCK_DGRAM, 0);

   /* Get the interface index. */
   ifIndex = sockGetIfIndex(ifName, sock);
   if (ifIndex < 0)
   {
      goto cleanup_socket;
   }
   ifr6.ifr6_ifindex = ifIndex;

   do
   {
      addrRemoved = 0;

      /* Get a list of addresses on all interfaces. This function will only
       * return one address of each type for each interface (e.g., there will
       * be one IPv4 address and one IPv6 address for each interface). This
       * function needs to be called multiple times, until all IPv6 addresses
       * have been deleted. */
      if (getifaddrs(&ifaddr) != 0)
      {
         ZL303XX_TRACE_ERROR("%s: Failed to get interface addresses (errno=%d).",
                             fnName, errno, 0,0,0,0);
         goto cleanup_socket;
      }

      /* Delete all IPv6 addresses configured for this interface. */
      for (; ifaddr != NULL; ifaddr = ifaddr->ifa_next)
      {
         sa6 = (struct sockaddr_in6 *)(void *)ifaddr->ifa_addr;

         /* Skip any address that is not IPv6 or... */
         if ((ifaddr->ifa_addr->sa_family != AF_INET6) ||
             /* any address that is not on this interface or... */
             (strcmp(ifName, ifaddr->ifa_name) != 0) ||
             /* a link local address. */
             (memcmp(sa6->sin6_addr.s6_addr, linkLocal,
                     sizeof(linkLocal)) == 0))
         {
            continue;
         }

         sa6 = (struct sockaddr_in6 *)(void *)ifaddr->ifa_netmask;

         /* Convert the netmask into a prefix length. */
         /* Count number of bytes == 0xFF. */
         for (count = 0;
              count < sizeof(sa6->sin6_addr) && sa6->sin6_addr.s6_addr[count] == 0xFF;
              count++);
         plen = count * 8;

         /* Count number of bits set. */
         if (plen < 128)
         {
            for (count = 0;
                 sa6->sin6_addr.s6_addr[plen / 8] & (1 << (7 - count));
                 count++);
            plen += count;
         }

         ifr6.ifr6_prefixlen = plen;
         sa6 = (struct sockaddr_in6 *)(void *)ifaddr->ifa_addr;
         memcpy(&ifr6.ifr6_addr, &sa6->sin6_addr, sizeof(ifr6.ifr6_addr));
         if (ioctl(sock, SIOCDIFADDR, &ifr6) != 0)
         {
            ZL303XX_TRACE_ERROR("%s: Failed to delete IPv6 address on %s (errno=%d).",
                                fnName, ifName, errno, 0,0,0);
            goto cleanup_ifaddrs;
         }

         addrRemoved++;
      }

      freeifaddrs(ifaddr);
   } while (addrRemoved);

   /* Now, add the requested IPv6 address to this interface. */
   osAddIpv6Addr(ifName, addr);

   /* Set the interface flags to broadcast, multicast, running and up */
   memset(&ifr, 0, sizeof(ifr));
   ifr.ifr_flags = IFF_UP | IFF_BROADCAST | IFF_MULTICAST | IFF_RUNNING;
   if (ioctl(sock, SIOCSIFFLAGS, &ifr))
   {
      ZL303XX_TRACE_ERROR("%s: Unable to set the Ethernet interface flags!", fnName, 0,0,0,0,0);
      goto cleanup_socket;
   }

   if( sock >= 0 )
   {
      close(sock);
      sock = -1;
   }

cleanup_ifaddrs :
   freeifaddrs(ifaddr);
cleanup_socket :
   if( sock >= 0 )
   {
      close(sock);
      sock = -1;
   }

}

/* osAddIpv6Addr */
/**
   Adds the IPv6 address for a board interface that is driving the zl303xx
   hardware.

  Parameters:
   [in]   ifName     Name of the network interface.
   [in]   addr       The source IPV6 address to use as a string.

  Return Value:
   Nothing

  Notes:
   This function will only set the interface when it is run with root privileges.
   Also the function relies on the driver supporting the functionality.
   If the interface has already been configured, this function may fail.
   This function has a high degree of OS and hardware dependence.

*******************************************************************************/
void osAddIpv6Addr(const char *ifName, const char *addr)
{
   const char *fnName = "osAddIpv6Addr";
   Sint32T sock, ifIndex;
   struct ifreq ifr;
   struct in6_ifreq ifr6;
   Uint32T prefixLen = 0;

   memset(&ifr, 0, sizeof(ifr));
   memset(&ifr6, 0, sizeof(ifr6));

   /* Create and open a socket */
   sock = socket(AF_INET6, SOCK_DGRAM, 0);

   ifIndex = sockGetIfIndex(ifName, sock);
   if (ifIndex < 0)
   {
      goto cleanup;
   }

   if (sockMakeIdFromAddr6(addr, ifr6.ifr6_addr.s6_addr, &prefixLen) != 0)
   {
      goto cleanup;
   }

   if (prefixLen == 0)
   {
      prefixLen = 64;
   }

   ifr6.ifr6_prefixlen = prefixLen;
   ifr6.ifr6_ifindex = ifIndex;

   /* Set the IP address of the interface */
   if (ioctl(sock, SIOCSIFADDR, &ifr6) != 0)
   {
      ZL303XX_TRACE_ERROR("%s: Unable to add source IPv6 address to %s! (errno=%d)\n",
                          fnName, ifName, errno, 0,0,0);
      goto cleanup;
   }

cleanup :
   if( sock >= 0)
   {
      /* Close the socket */
      close(sock);
      sock = -1;
   }
}
#else
void osSetIpv6Addr(const char *ifName, const char *addr)
{
   (void)ifName, (void)addr;  /* Warning removal */
   /* #warning MSCC: No set IP address code for an unknown board exists yet - use a command line or write one here. */
}

void osAddIpv6Addr(const char *ifName, const char *addr)
{
   (void)ifName, (void)addr;  /* Warning removal */
   /* #warning MSCC: No add IP address code for an unknown board exists yet - use a command line or write one here. */
}
#endif   /*  _ZL303XX_ZLE30360_BOARD ||  _ZL303XX_ZLE1588_BOARD */

/* sockGetIfIndex */
/** 

   Returns the index for a given network interface name.

  Parameters:
   [in]   ifName  Interface name
   [in]   sockFd  An open socket descriptor.

  Return Value:
     Sint32T  The interface index.
     -1       An error occurred.

*******************************************************************************/
static Sint32T sockGetIfIndex(const char *ifName, Sint32T sockFd)
{
   struct ifreq ifr;
   memset(&ifr, 0, sizeof(ifr));
   strncpy(ifr.ifr_name, ifName, sizeof(ifr.ifr_name) - 1);

   if (ioctl(sockFd, SIOCGIFINDEX, &ifr) != 0)
   {
      ZL303XX_TRACE_ERROR("Unable to get interface index for %s.", ifName, 0,0,0,0,0);
      return -1;
   }

   return (Sint32T)ifr.ifr_ifindex;
}
#endif   /* ZL_INCLUDE_IPV6_SOCKET */

#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
/**

  Function Name:
    osSetIpAddr

  Details:
  Sets the IP address for the Embedded Planet EP8260 board interface that is used
  on the development board for driving the zl303xx hardware

  Parameters:
   [in]  ifName          Name of the network interface.
   [in]  srcIpAddress    The source IP address to use in IPv4 dot format,
                                e.g. "10.0.0.5".
   [in]  subnetMask      The subnet mask to use in IPv4 dot format,
                                e.g., "255.255.255.0".

  Return Value:
   Nothing

  Notes:
   This function will only set the interface when it is run with root priveledges.
   Also the function relies on the driver supporting the functionality.
   If the interface has already been configured, this function may fail.
   This function has a high degree of OS and hardware dependence.

*******************************************************************************/
void osSetIpAddr(const char *ifName, const char *srcIpAddress, const char *subnetMask)
{
    struct ifreq ifr;
    /* Create and open the socket */
    Sint32T sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in *ipaddr, *netmask;

    /* Check that the socket was ipened */
    if (sock < 0)
    {
       ZL303XX_TRACE_ERROR(
             "   (ERROR) Socket Create Failed (errno=%d).",
             OS_ERRNO, 0,0,0,0,0);
       exit(-5);
    }

    /* Set the pointer to the appropriate fields in the ifreq structure */
    /* Casting the pointers through a void pointer to avoid cast-align warnings */
    ipaddr = (struct sockaddr_in *)(void *)&(ifr.ifr_addr);
    netmask = (struct sockaddr_in *)(void *)&(ifr.ifr_addr);

    /* Set the interface name */
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, ifName, sizeof(ifr.ifr_name) - 1);
    ipaddr->sin_family=AF_INET;
    /* set the IP address */
    ipaddr->sin_addr.s_addr=inet_addr(srcIpAddress);
    /* Set the IP address of the interface */
    if(ioctl(sock,  SIOCSIFADDR, &ifr) != 0) {
        ZL303XX_ERROR_TRAP("Unable to set the source IP address!");
        close(sock);
        exit(-2);
    }

    /* Set the network mask */
    netmask->sin_family=AF_INET;
    netmask->sin_addr.s_addr=inet_addr(subnetMask);
    if(ioctl(sock, SIOCSIFNETMASK, &ifr) != 0) {
        ZL303XX_ERROR_TRAP("Unable to set the source IP mask!");
        close(sock);
        exit(-3);
    }

    /* Set the interface flags to broadcast, multicast, running and up */
    ifr.ifr_flags = IFF_UP | IFF_BROADCAST | IFF_MULTICAST | IFF_RUNNING;
    if (ioctl(sock, SIOCSIFFLAGS, &ifr)) {
        ZL303XX_ERROR_TRAP("Unable to set the ethernet interface flags!");
        close(sock);
        exit(-4);
    }

    /* Close the socket */
    close(sock);
}

/**

  Function Name:
   osAddGateway

  Details:
   Adds a gateway address for the CPU for driving the zl303xx hardware

  Parameters:
       srcIPAddr    The source/sub-net IP address to route: in IPv4 format ("10.9.8.5")
        gwIPAddr     The gateway IP address to route to: in IPv4 format ("10.9.5.5")
        netMask  The network mask to apply to the route: in net format (0xffff0000)

  Return Value:
   Nothing

  Notes:
   None

*******************************************************************************/
void osAddGateway(char *srcIpAddr, char *gwIpAddr, Uint32T netMask)
{
    /* #warning MSCC: No Gateway code for Non-VxWorks exists yet - use a command line or write one here. */
    if (srcIpAddr && gwIpAddr && netMask) {;}   /* Eliminates warnings. */
}

/**

  Function Name:
    osAddIpAddr

  Details:
  Adds an IP address to a specified network interface.

  Parameters:
   [in]  ifName      Name of the network interface.
   [in]  srcIp       The source IP address to use in IPv4 dot format
                          (e.g., "10.0.0.5").
   [in]  subnetMask  The subnet mask to use in IPv4 dot format
                          (e.g., "255.255.255.0").

  Return Value:   N/A

*******************************************************************************/
void osAddIpAddr(const char *ifName, const char *srcIp, const char *subnetMask)
{
   SINT_T status = 0;
   const char *fnName = "osAddIpAddr";

   static Sint32T numAliases = 0;

   struct ifreq ifReq;
   struct sockaddr_in *ipAddr = (struct sockaddr_in *)(void*)&ifReq.ifr_addr;
   struct sockaddr_in *mask = (struct sockaddr_in *)(void*)&ifReq.ifr_netmask;
   SINT_T socketFd = 0, socketOpen = 0;

   /* Clear the ifreq structure. */
   memset(&ifReq, 0, sizeof(ifReq));

   /* Open a socket. */
   if (status == 0)
   {
      socketFd = socket(AF_INET, SOCK_DGRAM, 0);

      if (socketFd < 0)
      {
         ZL303XX_TRACE_ALWAYS("%s: (ERROR) Could not open socket.",
                              fnName, 0,0,0,0,0);
         status = -1;
      }
      else
      {
         socketOpen = 1;
      }
   }

   /* Check input parameters. */
   if (status == 0)
   {
      /* Check interface name pointer. */
      if (ifName == NULL)
      {
         ZL303XX_TRACE_ALWAYS("%s: (ERROR) ifName is NULL.",
                              fnName, 0,0,0,0,0);
         status = -1;
      }
      /* Check interface name format. */
      else if (strlen(ifName) == 0)
      {
         ZL303XX_TRACE_ALWAYS("%s: (ERROR) ifName is zero-length.",
                              fnName, 0,0,0,0,0);
         status = -1;
      }
   }
   if (status == 0)
   {
      /* Check that the source IP is properly formatted. */
      if ((srcIp == NULL) ||
          (inet_aton(srcIp, &ipAddr->sin_addr) == OS_ERROR))
      {
         ZL303XX_TRACE_ALWAYS("%s: (ERROR) Invalid format for srcIp=%s.",
                              fnName,
                              ((srcIp == NULL) ? "NULL" : srcIp),
                              0,0,0,0);
         status = -1;
      }
   }

   /* Set the rest of the alias parameters. */
   if (status == 0)
   {
      SINT_T ret = 0;

      /* IP address & netmask. */
      ipAddr->sin_family = AF_INET;

      /* Interface name.
       * e.g., each new alias for interface eth1 is bound to an interface
       * named eth1:1, eth1:2, etc. */
      numAliases++;
      ret = snprintf(ifReq.ifr_name, sizeof(ifReq.ifr_name), "%s:%d", ifName, (SINT_T)numAliases);

      if ((ret == -1) || (ret >= (SINT_T)sizeof(ifReq.ifr_name)))
      {
         ZL303XX_TRACE_ALWAYS("%s: (ERROR) Error formatting interface name.",
                              ifName, 0,0,0,0,0);
         status = -1;
      }
   }

   /* Add the new IP address. */
   if (status == 0)
   {
      if ((status = ioctl(socketFd, SIOCSIFADDR, &ifReq)) != 0)
      {
         ZL303XX_TRACE_ALWAYS("%s: (ERROR) Adding IP address failed (SIOCSIFADDR).",
                              fnName,0,0,0,0,0);
      }
   }

   /* Check that the subnet mask is properly formatted. */
   if (status == 0)
   {
      /* We can't do this conversion earlier because it will overwrite
         ifReq.ifr_addr. Both of these addresses are in the same union. */
      if ((subnetMask == NULL) ||
          (inet_aton(subnetMask, &mask->sin_addr) == OS_ERROR))
      {
         ZL303XX_TRACE_ALWAYS("%s: (ERROR) Invalid format for subnetMask=%s.",
                              fnName,
                              ((subnetMask == NULL) ? "NULL" : subnetMask),
                              0,0,0,0);
         status = -1;
      }
   }

   /* Set the subnet mask. */
   if (status == 0)
   {
      if (subnetMask != NULL)
      {
         if ((status = ioctl(socketFd, (SINT_T)SIOCSIFNETMASK, &ifReq)) != 0)
         {
            ZL303XX_TRACE_ALWAYS("%s: Setting subnet mask failed (SIOCSIFNETMASK).",
                                 fnName, 0,0,0,0,0);
         }
      }
   }

   /* Bring the "interface" up. */
   if (status == 0)
   {
      ifReq.ifr_flags |= IFF_UP;

      if ((status = ioctl(socketFd, (SINT_T)SIOCSIFFLAGS, &ifReq)) != 0)
      {
         ZL303XX_TRACE_ALWAYS("%s: Setting interface flags failed (SIOCSIFFLAGS).",
                              fnName, 0,0,0,0,0);
      }
   }

   /* Close the socket. */
   if (socketOpen == 1)
   {
      close(socketFd);
   }

   if (status != 0)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 1,
                    "%s: Function failed with status=%d.",
                    fnName, (Uint32T)status, 0,0,0,0);
   }
}

#else

void osSetIpAddr(const char *ifName, const char *srcIpAddress, const char *subnetMask)
{
    /* Compiler warning elimination. */
    if ((ifName) || (srcIpAddress) || (subnetMask)) {;}

    /* #warning MSCC: No set IP address code for an unknown board exists yet - use a command line or write one here. */
}

void osAddGateway(char *srcIpAddr, char *gwIpAddr, Uint32T netMask)
{
    /* Compiler warning elimination. */
    if ((srcIpAddr) || (gwIpAddr) || (netMask)) {;}

    /* #warning MSCC: No Gateway code for an unknown board exists yet - use a command line or write one here. */
}

void osAddIpAddr(const char *ifName, const char *srcIp, const char *subnetMask)
{
   /* Compiler warning elimination. */
   if ((ifName) || (srcIp) || (subnetMask)) {;}

   /* #warning MSCC: osAddIpAddr() does not exist for an unknown board yet - use a command line or write one here. */
}

#endif /* _ZL303XX_ZLE30310/20/60/1588_BOARD */

/**

  Function Name:
    osSetIfPromiscuous

  Details:
   Sets the promiscuous mode bit of a network interface. Promiscuous mode is
   enabled for RAW socket protocols.

  Parameters:
   [in]   ifName   Name of the network interface.
   [in]   isPromiscuous   Enables/disables promiscuous mode.

  Return Value:   0   Success. Otherwise, an error code is returned.

*******************************************************************************/
Sint32T osSetIfPromiscuous(const char *ifName, zl303xx_BooleanE isPromiscuous)
{
   Sint32T status = 0;
   struct ifreq ifr;

   /* Open a socket. */
   Sint32T socketFd = socket(AF_INET, SOCK_DGRAM, 0);

   ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 1, "osSetIfPromiscuous:", 0,0,0,0,0,0);

   if (socketFd < 0)
   {
      ZL303XX_TRACE_ALWAYS(
            "   (ERROR) Socket Create Failed (errno=%d).",
            OS_ERRNO, 0,0,0,0,0);
      status = -1;
   }

   /* Check input parameters. */
   if (status == 0)
   {
      /* Check interface name pointer. */
      if (ifName == NULL)
      {
         ZL303XX_TRACE_ALWAYS("   (ERROR) ifName is NULL.", 0,0,0,0,0,0);
         status = -1;
      }
      /* Check interface name format. */
      else if (strlen(ifName) == 0)
      {
         ZL303XX_TRACE_ALWAYS("   (ERROR) ifName is zero-length.", 0,0,0,0,0,0);
         status = -1;
      }
   }

   /* Set the promiscuous mode for this interface. */
   if (status == 0)
   {
      /* Get the flags for this interface. */
      strncpy(ifr.ifr_name, ifName, sizeof(ifr.ifr_name) - 1);
      ifr.ifr_name[sizeof(ifr.ifr_name) - 1] = '\0';

      status = (Sint32T)ioctl(socketFd, SIOCGIFFLAGS, (void *)&ifr);
   }

   /* Set the interface promiscuous mode bit. */
   if (status == 0)
   {
      if (isPromiscuous == ZL303XX_TRUE)
      {
         ifr.ifr_flags |= IFF_PROMISC;
      }
      else
      {
         ifr.ifr_flags &= ~IFF_PROMISC;
      }

      status = (Sint32T)ioctl(socketFd, SIOCSIFFLAGS, (void *)&ifr);
   }

   /* Close the socket. */
   if (socketFd >= 0)
      close(socketFd);

   return status;
}
