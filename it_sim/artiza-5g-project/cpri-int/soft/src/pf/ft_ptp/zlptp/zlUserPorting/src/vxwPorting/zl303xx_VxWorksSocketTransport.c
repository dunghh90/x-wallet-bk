

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This provides an implementation of the functions in zl303xx_Transport.h
*     using the VxWorks socket library
*
******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_ErrTrace.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Os.h"
#include "zl303xx_VxWorksSocketTransport.h"
#include "zl303xx_Pkt.h"
#include "zl303xx_SocketSelect.h"
#ifdef __VXWORKS_65
#include <net/if_arp.h>
#include <net/utils/ifconfig.h>
#include <net/utils/routeCmd.h>
#include <netinet/in_var.h>
#endif
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
_ZL303XX_LOCAL Sint32T zl303xx_VxSTInit(zl303xx_SocketDataS *pSktConnectData);
_ZL303XX_LOCAL Sint32T zl303xx_VxSTClose(zl303xx_SocketDataS *pSktConnectData);

_ZL303XX_LOCAL Sint32T zl303xx_VxGetIface(zl303xx_SocketDataS *pSktConnectData);

/********************    FUNCTION DECLARATIONS   ******************************/

Sint32T osSetIfPromiscuous(const char *, zl303xx_BooleanE);
Sint32T osGetMacAddr(char *, Uint8T *);
void osSetIpAddr(const char *, const char *, const char *);
void osAddIpAddr(const char *, const char *, const char *);
void osAddGateway(char *, char *, Uint32T);

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
   VxWorks socket transport layer

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
            ZL303XX_ERROR_NOTIFY("Non-multicast address supplied");
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
      status = zl303xx_VxSTInit(pSktData);
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


   if (status == 0)
   {  /* Call the close function */
      status = zl303xx_VxSTClose(pSktData);
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
   static zl303xx_BooleanE socketErrorOccured = ZL303XX_FALSE;

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

      ZL303XX_TRACE(ZL303XX_MOD_ID_SOCKET_LAYER, 1,"Send on Socket: %d on Port: %u of Type: %u", pSktData->socketFd, pSktData->port, pSktData->type, 0, 0, 0);
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
             if (!socketErrorOccured)
             {
                 ZL303XX_TRACE_ALWAYS("Unable to send to socket : %ld", errno, 0, 0, 0, 0, 0);
                 socketErrorOccured = ZL303XX_TRUE;
             }
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
   SINT_T fromLen = sizeof(fromAddr);

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
      /* This implementation relies on the previous configuration of the socket to
         make it non-blocking */
      nBytes = recvfrom(pSktData->socketFd, (char *)buf, buflen, 0, (struct sockaddr *)(void *)&fromAddr, &fromLen);

      if (nBytes == OS_ERROR)
      {
          /* Record the last error in case we want to inspect it later */
          Zl303xx_LastSocketErr = errno;

          ZL303XX_TRACE_ALWAYS("Unable to receive from socket : %ld", errno, 0, 0, 0, 0, 0);
          retVal = -1 ;
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
         strncpy(ifaceName, pSktData->ifaceName, sizeof(ifaceName));
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
               (zl303xx_RemoteAddrT *)(void *)pSktData->localIpAddr, addr, size);

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

   /* Convert into a local buffer which we know is big enough */
   inet_ntoa_b(*linkId, tempBuffer);

   /* Check the length of the return buffer */
   nBytes = strlen(tempBuffer);

   if (nBytes > *size)
   {  /* Buffer is not big enough */
      status = -1;
   }
   else
   {
      /* Copy the address to the return buffer */
      memcpy(addrStr, tempBuffer, nBytes);

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
   char local_addrStr[ZL303XX_INET_ADDR_MAX_LEN];

   struct in_addr *sockAddr = (struct in_addr *)linkId;

   memset(local_addrStr, 0, sizeof(local_addrStr));
   strncpy(local_addrStr, addrStr, sizeof(local_addrStr) - 1);

   if (inet_aton(local_addrStr, sockAddr) == OS_ERROR)
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
      FD_SET((UINT)pSktData->socketFd, fdSet);
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
      FD_CLR((UINT)pSktData->socketFd, fdSet);
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
   return FD_ISSET((UINT)pSktData->socketFd, fdSet);
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
            ZL303XX_TRACE_ALWAYS("Socket select interrupted",0,0,0,0,0,0);
         }
         else
         {
            /* Record the last error in case we want to inspect it later */
            ZL303XX_ERROR_TRAP("Socket select error");
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
    zl303xx_VxSTInit

  Details:
   Initialises communication paths to the ZL303XX_ device through the
   VxWorks socket transport layer

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
Sint32T zl303xx_VxSTInit(zl303xx_SocketDataS *pSktConnectData)
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
      if (pSktConnectData->type != ZL303XX_TRANSPORT_BROADCAST)
      {
         /* Create a UDP socket to be used for bidirectional communication through the ZL303XX_ device */
         pSktConnectData->socketFd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
      }
      else
      {
         /* Create a nonUDP socket to be used for bidirectional communication through the ZL303XX_ device */
         pSktConnectData->socketFd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
      }
      if (pSktConnectData->socketFd == OS_ERROR)
      {
         pSktConnectData->socketFd = INVALID_SOCKET;
         ZL303XX_TRACE_ALWAYS("Unable to open socket : %ld", errno, 0, 0, 0, 0, 0);
         Zl303xx_LastSocketErr = errno;
         status = -1;
      }
      else
      {
         ZL303XX_TRACE (ZL303XX_MOD_ID_TRNSPRT_LYR, 1, "Socket created: %d on Port: %u of Type: %u", pSktConnectData->socketFd, pSktConnectData->port, pSktConnectData->type, 0, 0, 0);
         pSktConnectData->addrFamily = AF_INET;
      }
   }

   if (status == 0)
   {
      /* Get the interface parameters.  */
      if (zl303xx_VxGetIface(pSktConnectData) < 0)
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
      Sint32T onOff = 1;

      if (ioctl(pSktConnectData->socketFd, FIONBIO, (SINT_T)&onOff) == OS_ERROR)
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

         memset(&socketAddr , 0, sizeof(struct sockaddr_in));

         socketAddr.sin_family = pSktConnectData->addrFamily;
         if (pSktConnectData->type == ZL303XX_TRANSPORT_MULTICAST)
         {
            /* Bind to the multicast address */
            memcpy(&socketAddr.sin_addr.s_addr, pSktConnectData->mcastAddr,
                   sizeof(socketAddr.sin_addr.s_addr));
         }
         else
         {  /* For unicast and broadcast messages, bind to the actual address */
            memcpy(&socketAddr.sin_addr, pSktConnectData->localIpAddr,
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

   if (pSktConnectData->type == ZL303XX_TRANSPORT_MULTICAST)
   {
      if (status == 0)
      {
         /* Note that VxWorks reference documentation defines the parameter type for this option as 'int'
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
         memcpy(&mreq.imr_interface, pSktConnectData->localIpAddr,
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
      }

      /* Set the multicast Time-to-Live value. */
      if (status == 0)
      {
         /* Note that VxWorks reference documentation defines the parameter type for this option as 'int'
            but this does not work and is contradicted by some web documents, e.g.
            http://www-kryo.desy.de/documents/vxWorks/V5.4/vxworks/netguide/c-netapi2.html */
         char ttl = (char)ZL303XX_SOCKET_MULTICAST_TTL;
         if ( (setsockopt (pSktConnectData->socketFd, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl,
                     sizeof(ttl))) < 0)
         {
             ZL303XX_TRACE_ALWAYS("Unable to set multicast time to live: %ld", errno, 0, 0, 0, 0, 0);
             Zl303xx_LastSocketErr = errno;
             status = -1;
         }
      }

      /* Set the unicast Time-to-Live value. */
      if (status == 0)
      {
         Sint32T ttl = (Sint32T)ZL303XX_SOCKET_UNICAST_TTL;
         if (setsockopt(pSktConnectData->socketFd,
                        IPPROTO_IP,
                        IP_TTL,
                        (char *)&ttl,
                        sizeof(SINT_T)) < 0)
         {
            ZL303XX_TRACE_ALWAYS("Unable to set unicast time-to-live: %ld", errno, 0,0,0,0,0);
            Zl303xx_LastSocketErr = errno;
            status = -1;
         }
      }
   }

   return status;
}

/**

  Function Name:
   zl303xx_VxSTClose

  Details:
   Closes communication path to the socket for VxWorks Socket Transport layer

  Parameters:
       pSktConnectData   Pointer to structure containing information
                           about the connection
                           See zl303xx_VxSTInit() for further details

  Return Value:
   0 (OK) or -1 if an error occurs

*******************************************************************************/
Sint32T zl303xx_VxSTClose(zl303xx_SocketDataS *pSktConnectData)
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

            (void)setsockopt(pSktConnectData->socketFd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
         }

         /*****************  Transmit connection closedown **********************/
         /* Nothing to do for transmit */

         /* Finally close the socket connection */
         (void)close(pSktConnectData->socketFd);
         OS_TASK_DELAY(1000);
      }
   }

   return status;
}

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
   Sint32T socketFd = socket(AF_INET, SOCK_RAW, 0);

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

      status = (Sint32T)ioctl(socketFd, SIOCGIFFLAGS, (SINT_T)&ifr);
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

      status = (Sint32T)ioctl(socketFd, SIOCSIFFLAGS, (SINT_T)&ifr);
   }

   if (socketFd >= 0)
      close(socketFd);

   return status;
}

/* osGetMacAddr */
/**
   Returns the MAC address of a network interface.

  Parameters:
   [in]   ifName    The name of the network interface.
   [out]  macAddr   A 6-byte buffer containing the MAC address. The most
                         significant byte is at index 0.

  Return Value:
      0   Success.
     -1   An error occurred.

*******************************************************************************/
Sint32T osGetMacAddr(char *ifName, Uint8T *macAddr)
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

#ifdef __VXWORKS_65
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
         socketFd = socket(AF_INET, SOCK_RAW, 0);
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
            if (ioctl(socketFd, SIOCGIFLLADDR, (SINT_T)&ifReq) < 0)
            {
               ZL303XX_TRACE_ERROR("Unable to get MAC Address!", 0,0,0,0,0,0);
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
#else
   {
      struct arpcom *pArpCom = NULL;

      /* Get a pointer to the ifnet structure that corresponds to ifName. */
      if (status == 0)
      {
         /* Cast the pointer returned by ifunit() as a (struct arpcom *) since
          * the first member in the arpcom structure is an ifnet structure. */
         if ((pArpCom = (struct arpcom *)ifunit(ifName)) == NULL)
         {
            ZL303XX_TRACE_ERROR("   (ERROR) Call to ifunit() failed.", 0,0,0,0,0,0);
            status = -1;
         }
      }

      /* Copy the MAC address into the provided buffer. */
      if (status ==  0)
      {
          memcpy(macAddr, pArpCom->ac_enaddr, ZL303XX_MAC_ADDRESS_BYTES);
      }
   }
#endif

   return status;
}


/**

  Function Name:
   zl303xx_VxGetIface

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
Sint32T zl303xx_VxGetIface(zl303xx_SocketDataS *pSktConnectData)
{
  struct ifconf data;
  struct ifreq device[IFCONF_LENGTH];
  Uint8T count = 0, addrTest = 0;
  Sint32T flags;

  data.ifc_len = sizeof(device);
  data.ifc_req = device;

  memset(data.ifc_buf,0,data.ifc_len);

  flags = IFF_UP|IFF_RUNNING;

  if (pSktConnectData->type == ZL303XX_TRANSPORT_MULTICAST)
  {
     flags |= IFF_MULTICAST;
  }

  /* Test if localIpAddr has been explicitly set. */
  for (count = 0; count < ZL303XX_SOCKET_ADDR_IPV4_LEN; count++)
  {
     addrTest |= pSktConnectData->localIpAddr[count];
  }

  /* look for an interface if none specified */
  if(pSktConnectData->ifaceName[0] == '\0')
  {
    /* no iface specified */
    /* get list of network interfaces*/
    if(ioctl(pSktConnectData->socketFd, (SINT_T)SIOCGIFCONF, (SINT_T)&data) < 0)
    {
      ZL303XX_ERROR_TRAP("Failed to get network interface list");
      return -1;
    }

    if(data.ifc_len == sizeof(device))
    {
      ZL303XX_TRACE_ALWAYS("Device list may exceed allocated space",0,0,0,0,0,0);
    }

    /* search through interfaces */
    for (count = 0; count < data.ifc_len/(Sint32T)sizeof(device[0]); ++count)
    {
       /* Debugging output. Display any interface device we find  */
       ZL303XX_TRACE(ZL303XX_MOD_ID_TRNSPRT_LYR, 2, "%d %s",count,(Sint32T)device[count].ifr_name,0 ,0,0,0);

      if (ioctl(pSktConnectData->socketFd, (SINT_T)SIOCGIFFLAGS, (SINT_T)&device[count]) < 0)
      {
        ZL303XX_ERROR_TRAP("Failed to get device flags");
        return -1;
      }
      else if (memcmp(INTERFACE_PREFIX, device[count].ifr_name, strlen(INTERFACE_PREFIX)))
      {
         /* This is not the one we are looking for */
         continue;
      }
      else if (((device[count].ifr_flags)&flags) == flags )
      {
        /* found a suitable interface */
        strncpy(pSktConnectData->ifaceName, device[count].ifr_name, ZL303XX_IFACE_NAME_MAX_LEN);
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
     strncpy(device[count].ifr_name, pSktConnectData->ifaceName, IFNAMSIZ-1);
     device[count].ifr_name[IFNAMSIZ-1] = '\0';
  }

  /* get ip address if it has not been explicitly specified */
  if (addrTest == 0)
  {
     if(ioctl(pSktConnectData->socketFd, (SINT_T)SIOCGIFADDR, (SINT_T)&device[count]) < 0)
     {
       ZL303XX_ERROR_TRAP("Failed to get ip address");
       return -1;
     }
     else
     {
       memcpy(pSktConnectData->localIpAddr,
              &((struct sockaddr_in *)(void *)&device[count].ifr_addr)->sin_addr,
              sizeof(struct in_addr));
     }
  }

    {
#ifdef __VXWORKS_65
      /* Get the MAC address of the ethernet port */
      if (ioctl (pSktConnectData->socketFd, SIOCGIFLLADDR, (SINT_T)&device[count]) < 0)
      {
          ZL303XX_ERROR_TRAP("Unable to get MAC Address!");
      }
      memcpy(pSktConnectData->macAddr, device[count].ifr_addr.sa_data, ZL303XX_MAC_ADDRESS_BYTES);

      /* Get the netmask */
      if (ioctl (pSktConnectData->socketFd, SIOCGIFNETMASK, (SINT_T)&device[count]) < 0)
      {
          ZL303XX_ERROR_TRAP("Unable to get NetMask!");
      }
      pSktConnectData->netMask = ((struct sockaddr_in *)(void*)&device[count].ifr_addr)->sin_addr.s_addr;
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
      if (ifMaskGet(pSktConnectData->ifaceName, (int *)&pSktConnectData->netMask) == ERROR)
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
   the VxWorks socket transport layer.

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
   /* Warning removal */
   (void)pSktData, (void)ifName, (void)initData, (void)port;

   ZL303XX_TRACE_ERROR("sockCreateNonBlock6: IPv6 sockets not supported for VxWorks.",
                       0,0,0,0,0,0);
   return -1;
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
   /* Warning removal */
   (void)pSktData, (void)destAddr, (void)buf, (void)nBytes;

   ZL303XX_TRACE_ERROR("sockSend6: IPv6 sockets not supported for VxWorks.",
                       0,0,0,0,0,0);
   return -1;
}

/* sockRecv6 */
/**
   Performs a read on the socket connection.

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
   /* Warning removal */
   (void)pSktData, (void)buf, (void)buflen, (void)remoteAddr, (void)remoteAddrLen;

   ZL303XX_TRACE_ERROR("sockRecv6: IPv6 sockets not supported for VxWorks.",
                       0,0,0,0,0,0);
   return -1;
}

/* osSetIpAddr6 */
/**
   Sets the IPv6 address for a board interface that is driving the zl303xx
   hardware.

  Parameters:
   [in]   ifName  Name of the network interface.
   [in]   addr    The source IPV6 address to use as a string.

  Return Value:
   Nothing

*******************************************************************************/
void osSetIpv6Addr(const char *ifName, const char *addr)
{
   (void)ifName, (void)addr;  /* Warning removal */

   ZL303XX_TRACE_ERROR("osSetIpv6Addr: IPv6 sockets not supported for VxWorks.",
                       0,0,0,0,0,0);
   return -1;
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

*******************************************************************************/
void osAddIpv6Addr(const char *ifName, const char *addr)
{
   (void)ifName, (void)addr;  /* Warning removal */

   ZL303XX_TRACE_ERROR("osAddIpv6Addr: IPv6 sockets not supported for VxWorks.",
                       0,0,0,0,0,0);
   return -1;
}
#endif

/**

  Function Name:
    osSetIpAddr

  Details:
  Sets the IP address for the CPU for driving the zl303xx hardware

  Parameters:
   [in]  ifName          Name of the network interface.
   [in]  srcIpAddress    The source IP address to use in IPv4 dot format
                            (e.g., "10.0.0.5").
   [in]  subnetMask      The subnet mask to use in IPv4 dot format
                            (e.g., "255.255.255.0").

  Return Value:
   Nothing

  Notes:
   None

*******************************************************************************/
void osSetIpAddr(const char *ifName, const char *srcIpAddress, const char *subnetMask)
{
#ifdef __VXWORKS_65
    char cmdString[64];
    Sint32T retVal;

    /* EvalBrd: motfcc1 and IPV4 */
    snprintf(&cmdString[0], sizeof(cmdString), "%s %s netmask %s up", ifName, srcIpAddress, subnetMask);
    if ((retVal = ifconfig(&cmdString[0])) != OK)
        printf("ifconfig failed!: %s\n", cmdString);
#else
   char name[ZL303XX_IFACE_NAME_MAX_LEN];
   char src[ZL303XX_INET_ADDR_MAX_LEN];

   (void)subnetMask;   /* Warning removal */

   memset(name, 0, sizeof(name));
   memset(src, 0, sizeof(src));

   /* Copy interface name and source IP to local buffers because ifAddrSet()
    * takes (char *) parameters rather than (const char *). */
   strncpy(name, ifName, sizeof(name) - 1);
   strncpy(src, srcIpAddress, sizeof(src) - 1);

   (void)ifAddrSet(name, src);
#endif
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
   Sint32T status = 0;
   const char *fnName = "osAddIpAddr";
   struct ifaliasreq ifAliasReq;
   struct sockaddr_in *ipAddr = (struct sockaddr_in *)(void *)&ifAliasReq.ifra_addr;
   struct sockaddr_in *mask = (struct sockaddr_in *)(void *)&ifAliasReq.ifra_mask;
   Sint32T socketFd = 0;

   /* Create a local copy of IP addresses to eliminate "discards 'const'" warnings. */
   char tempIp[ZL303XX_INET_ADDR_MAX_LEN];
   memset(tempIp, 0, sizeof(tempIp));

   /* Clear the ifaliasreq structure. */
   memset(&ifAliasReq, 0, sizeof(ifAliasReq));

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
      /* Check IP address pointer. */
      else if (srcIp == NULL)
      {
         ZL303XX_TRACE_ALWAYS("%s: (ERROR) srcIp is NULL.",
                              fnName, 0,0,0,0,0);
         status = -1;
      }
      /* Check subnet mask pointer. */
      else if (subnetMask == NULL)
      {
         ZL303XX_TRACE_ALWAYS("%s: (ERROR) subnetMask is NULL.",
                              fnName, 0,0,0,0,0);
         status = -1;
      }
   }
   if (status == 0)
   {
      strncpy(tempIp, srcIp, sizeof(tempIp) - 1);

      /* Check that the source IP is properly formatted. */
      if (inet_aton(tempIp, &ipAddr->sin_addr) == OS_ERROR)
      {
         ZL303XX_TRACE_ALWAYS("%s: (ERROR) Invalid format for srcIp=%s.",
                              fnName, srcIp, 0,0,0,0);
         status = -1;
      }
   }
   if (status == 0)
   {
      strncpy(tempIp, subnetMask, sizeof(tempIp) - 1);

      /* Check that the subnet mask is properly formatted. */
      if (inet_aton(tempIp, &mask->sin_addr) == OS_ERROR)
      {
         ZL303XX_TRACE_ALWAYS("%s: (ERROR) Invalid format for subnetMask=%s.",
                              fnName, subnetMask, 0,0,0,0);
         status = -1;
      }
   }

   /* Set the rest of the alias parameters. */
   if (status == 0)
   {
      strncpy(ifAliasReq.ifra_name, ifName, sizeof(ifAliasReq.ifra_name));
      ifAliasReq.ifra_name[sizeof(ifAliasReq.ifra_name) - 1] = '\0';

      ipAddr->sin_family = AF_INET;
      ipAddr->sin_len = sizeof(ifAliasReq.ifra_addr);

      mask->sin_family = AF_INET;
      mask->sin_len = sizeof(ifAliasReq.ifra_mask);

      /* Open a socket. */
      socketFd = socket(AF_INET, SOCK_RAW, 0);

      /* Add the new IP address. */
      status = ioctl(socketFd, (SINT_T)SIOCAIFADDR, (SINT_T)&ifAliasReq);

      /* Close the socket. */
      close(socketFd);
   }

   if (status != 0)
   {
      ZL303XX_TRACE_ALWAYS("%s failed with status=%d.",
                           fnName, status, 0,0,0,0);
   }
}

/**

  Function Name:
    osSetMtuSize

  Details:
  Sets the MTU size for a given interface.

  Parameters:
   [in]  mtuSize     New MTU size.
   [in]  ifName      Name of the network interface.

  Return Value:
   Nothing

  Notes:
   None

*******************************************************************************/
Sint16T osSetMtuSize(Uint16T mtuSize, char *ifName)
{
#ifdef __VXWORKS_65
    char cmdString[64];
    Sint32T retVal;

    /* Build ifconfig command string */
    snprintf(&cmdString[0], sizeof(cmdString), "%s mtu %u", ifName, mtuSize);
    if ((retVal = ifconfig(&cmdString[0])) != OK)
    {
        ZL303XX_TRACE_ALWAYS("osSetMtuSize: ifconfig command [%s] failed",
                cmdString,0,0,0,0,0);
        return -1;
    }
    ZL303XX_TRACE_ALWAYS("MTU for interface %s is now set to %u",
            ifName, mtuSize,0,0,0,0);
#else
    struct ifnet *ifp;
    Sint32T oldMtu;

    ifp = ifunit(ifName);

    if (ifp == NULL)
    {
        ZL303XX_TRACE_ALWAYS("osSetMtuSize: Could not find %s interface",
                ifName,0,0,0,0,0);
        return -1;
    }

    oldMtu = ifp->if_mtu;
    ifp->if_mtu = mtuSize;
    ZL303XX_TRACE_ALWAYS("Previous MTU for %s was %u - MTU is now set to %u",
            ifName, oldMtu, ifp->if_mtu,0,0,0);
#endif
    return 0;
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
#ifdef __VXWORKS_65

   /* Use command line function to set gateways for now */
   char cmdString[64];
   Uint8T octet1, octet2, octet3, octet4;

   octet1 = ((netMask & 0xff000000) >> 24);    /* Make netmask acceptable for routec */
   octet2 = ((netMask & 0x00ff0000) >> 16);
   octet3 = ((netMask & 0x0000ff00) >> 8);
   octet4 = (netMask & 0x000000ff);

   snprintf(&cmdString[0], sizeof(cmdString), "add -net -netmask %d.%d.%d.%d %s %s",
                          octet1, octet2, octet3, octet4, srcIpAddr, gwIpAddr);
   if (routec(&cmdString[0]) < 0)
   {
       ZL303XX_ERROR_TRAP("Unable to use routec to add gateway!");
   }
                            ;
#else
   (void)mRouteAdd(srcIpAddr, gwIpAddr, netMask, 0, 0);
#endif
}
