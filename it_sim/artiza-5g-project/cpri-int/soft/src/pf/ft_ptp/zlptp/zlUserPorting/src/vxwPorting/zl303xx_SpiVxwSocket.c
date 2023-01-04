

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     SPI socket task on the CPU.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_Porting.h"
#include "zl303xx_SpiSocket.h"
#include "zl303xx_SpiVxwSocket.h"

/*****************   DEFINES     **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/
void zl303xx_SpiServerTask(void);

/*****************   STATIC GLOBAL VARIABLES   ********************************/
Uint16T localPortNum = 5000;
Sint32T spiSocketFd = INVALID_SOCKET;

struct sockaddr_in spiSocketAddr;
OS_TASK_ID spiServerTaskId = (OS_TASK_ID)NULL;

/* Sempahore & Mutex Ids */
OS_MUTEX_ID spiMutexId  = OS_SEM_INVALID;
OS_SEM_ID   spiIsrSemId = OS_SEM_INVALID;

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/**

  Function Name:
   spiLock

  Details:
   Provides OS exclusion to the SPI interface

  Parameters:
        None

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE spiLock(void)
{
   zlStatusE status = ZL303XX_OK;
   OS_STATUS osStatus = OS_OK;

   osStatus = OS_MUTEX_TAKE(spiMutexId);

   if (osStatus != OS_OK)
   {
      status = ZL303XX_RTOS_SEM_TAKE_FAIL;
      ZL303XX_ERROR_TRAP("SPI mutex take failure");
   }

   return status;
}  /* END spiLock */

/**

  Function Name:
   spiUnlock

  Details:
   Provides OS exclusion to the SPI interface

  Parameters:
        None

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE spiUnlock(void)
{
   zlStatusE status = ZL303XX_OK;
   OS_STATUS osStatus = OS_OK;

   /* Give up semaphore */
   osStatus = OS_MUTEX_GIVE(spiMutexId);

   if (osStatus != OS_OK)
   {
      status = ZL303XX_RTOS_SEM_GIVE_FAIL;
      ZL303XX_ERROR_TRAP("SPI mutex give failure");
   }

   return status;
}  /* END spiUnlock */

/**

  Function Name:
    zl303xx_SpiSocketCreate

  Details:
   Initialises communication path for the SPI controller

  Parameters:
        None

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_SpiSocketCreate(void)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      (void)OS_LOG_MSG("Opening socket: %d", localPortNum, 0, 0, 0, 0, 0);

      spiSocketFd = socket(AF_INET, REMOTE_SOCK_TYPE, 0);

      if (spiSocketFd == INVALID_SOCKET)
      {
         (void)OS_LOG_MSG("Unable to open socket : %d", localPortNum, 0, 0, 0, 0, 0);
         status = ZL303XX_ERROR;
      }
   }

   if (status == ZL303XX_OK)
   {
      memset((char *)&spiSocketAddr, 0, sizeof(struct sockaddr_in));

      spiSocketAddr.sin_family = AF_INET;
      spiSocketAddr.sin_port = htons(localPortNum);
      spiSocketAddr.sin_addr.s_addr = INADDR_ANY;
   }

   if (status == ZL303XX_OK)
   {
      if (bind(spiSocketFd, (struct sockaddr *)&spiSocketAddr, sizeof(struct sockaddr_in)) < 0) 
      {
         (void)OS_LOG_MSG("Unable to bind socket : %d", localPortNum, 0, 0, 0, 0, 0);
         status = ZL303XX_ERROR;
      }
   }

   if (status == ZL303XX_OK)
   {
      #if (REMOTE_SOCK_TYPE == SOCK_STREAM)
      {
         if (listen(spiSocketFd, 1) == INVALID_SOCKET)
         {
            (void)OS_LOG_MSG("Unable to listen on socket : %d", localPortNum, 0, 0, 0, 0, 0);
            status = ZL303XX_ERROR;
         }
      }
      #else
      {
         Sint32T onOff = 1;

         if (ioctl(spiSocketFd, FIONBIO, (SINT_T) &onOff) != 0)
         {
            (void)OS_LOG_MSG("Unable to change socket to non-blocking : %d", localPortNum, 0, 0, 0, 0, 0);
            status = ZL303XX_ERROR;
         }
      }
      #endif
   }

   if (status == ZL303XX_OK)
   {
      if ((spiServerTaskId = OS_TASK_SPAWN("zlSpiServerTaskzl303xx",
                                       100,
                                       0,
                                       20000,
                                       (OS_TASK_FUNC_PTR)zl303xx_SpiServerTask,
                                       0)) == (OS_TASK_ID)OS_ERROR)
      {
         (void)OS_LOG_MSG("Failed task spawn", 0, 0, 0, 0, 0, 0);
         status = ZL303XX_ERROR;
      }
   }

   return status;
}

/**

  Function Name:
   zl303xx_SpiSocketClose

  Details:
   Closes communication path for the SPI controller

  Parameters:
        None

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_SpiSocketClose(void)
{
   zlStatusE status = ZL303XX_OK;

   if (spiServerTaskId != (OS_TASK_ID)NULL)
   {
      if (OS_TASK_DELETE(spiServerTaskId) == OS_OK)
      {
         spiServerTaskId = (OS_TASK_ID)NULL;
      }
      else
      {
         (void)OS_LOG_MSG("Task delete failed", 0, 0, 0, 0, 0, 0);
      }
   }

   if (spiSocketFd != INVALID_SOCKET)
   {
      (void)OS_LOG_MSG("Closing socket", 0, 0, 0, 0, 0, 0);
      close(spiSocketFd);
   }

   return status;
}

/**

  Function Name:
    zl303xx_SpiServerTask

  Details:
   Task on the remote CPU that processes SPI Rd/Wr requests.

  Parameters:
   
.

  Return Value:
   None

*******************************************************************************/

void zl303xx_SpiServerTask(void)
{
   Sint32T pktLen;
   Uint32T fromLen;
   spiSocketMsgS msg;
   spiSocketMsgS msgHost;
   /*Uint32T cnt;*/
   char strBuf[80];
   struct sockaddr fromAddr;
   Sint32T spiAcceptSocketFd = -1;

   /*cnt = 0;*/
   while (1)
   {
      #if (REMOTE_SOCK_TYPE == SOCK_STREAM)
      {
         fromLen = sizeof(struct sockaddr);

         while (1)
         {
            spiAcceptSocketFd = accept(spiSocketFd, (struct sockaddr *)&fromAddr, (SINT_T *)&fromLen);

            if (spiAcceptSocketFd != SOCKET_ERROR)
            {
               strncpy(strBuf, inet_ntoa(((struct sockaddr_in *)(void *)&fromAddr)->sin_addr), sizeof(strBuf));

               (void)OS_LOG_MSG("Accepted %s:%hu",
                     (Uint32T)strBuf,
                     ntohs(((struct sockaddr_in *)(void *)&fromAddr)->sin_port), 0, 0, 0, 0);

               break;
            }

            (void)OS_TASK_DELAY(100);
         }
      }
      #else
      {
         spiAcceptSocketFd = spiSocketFd;
      }
      #endif

      while (1)
      {
         fromLen = sizeof(struct sockaddr);
         pktLen = recvfrom(spiAcceptSocketFd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&fromAddr, (SINT_T *)&fromLen);

         #if (REMOTE_SOCK_TYPE == SOCK_STREAM)
         if ((pktLen == SOCKET_ERROR) || (pktLen == 0))
         {
            break;
         }
         #endif

         if (pktLen != SOCKET_ERROR)
         {
            strncpy(strBuf, inet_ntoa(((struct sockaddr_in *)(void *)&fromAddr)->sin_addr), sizeof(strBuf));

            if (pktLen > 0)
            {
           /*    REMOTE_DEBUG("From IP %s:%hu, len %lu",
                     (Uint32T)strBuf,
                     ntohs(((struct sockaddr_in *)&fromAddr)->sin_port), pktLen, 0, 0, 0);
*/
               if (pktLen == sizeof(msg))
               {
                  /*cnt++;*/

                  msgHost.ver = ntohs(msg.ver);
                  msgHost.seq = ntohs(msg.seq);
                  msgHost.action = ntohl(msg.action);
                  msgHost.addr = ntohl(msg.addr);
                  msgHost.data = ntohl(msg.data);

            /*      REMOTE_DEBUG("   msg %lu: ver %hu, seq %hu, type %lu, addr %08lX, data %08lX",
                        cnt, msgHost.ver, msgHost.seq, msgHost.action, msgHost.addr, msgHost.data);
*/
                  switch (ntohl(msg.action))
                  {
                     case REMOTE_SPI_READ:
                     {
                        if (cpuSpiRead(NULL, msgHost.addr, (Uint8T*)&(msgHost.data), 4) != 0)
                        {
                           msg.action = htonl((Uint32T)REMOTE_SPI_ERROR);
                        }
                        else
                        {
                           msg.action = htonl((Uint32T)REMOTE_SPI_READ_ACK);
                        }

                        msg.data = htonl(msgHost.data);
                        break;
                     }

                     case REMOTE_SPI_WRITE:
                     {
                        if (cpuSpiWrite(NULL, msgHost.addr, (Uint8T*)&(msgHost.data), 4) != 0)
                        {
                           msg.action = htonl((Uint32T)REMOTE_SPI_ERROR);
                        }
                        else
                        {
                           msg.action = htonl((Uint32T)REMOTE_SPI_WRITE_ACK);
                        }
                        break;
                     }

                     case REMOTE_SPI_READ_ACK:
                     case REMOTE_SPI_WRITE_ACK:
                     case REMOTE_SPI_ERROR:
                     default:
                        msg.action = htonl((Uint32T)REMOTE_SPI_ERROR);
                        break;
                  }

                  pktLen = sendto(spiAcceptSocketFd, (char *)&msg, sizeof(msg), 0,
                        (struct sockaddr *)&fromAddr, sizeof(struct sockaddr));
               }
            }
         }
         else
         {
            (void)OS_TASK_DELAY(100);
         }
      }

      #if (REMOTE_SOCK_TYPE == SOCK_STREAM)
      {
         (void)OS_LOG_MSG("Dropped connection", 0, 0, 0, 0, 0, 0);

         close(spiAcceptSocketFd);
      }
      #endif
   }
}
/**

  Function Name:
    zl303xx_SpiInitInterrupts

  Details:
   Creates the SPI Mutex, the ISR Tx/Rx Completed Semaphore and connects the SPI
   Interrupt to the SPI Interrupt Service Routine.

  Parameters:
        None

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_SpiInitInterrupts(Uint32T spi_irq)
{
   cpuStatusE cpuStatus = CPU_OK;
   OS_STATUS osStatus;

   if (cpuStatus == CPU_OK)
   {
      if (spiMutexId == OS_SEM_INVALID)
      {
         spiMutexId = OS_MUTEX_CREATE();

         if (spiMutexId == OS_SEM_INVALID)
         {
             cpuStatus = CPU_SPI_SEM_CREATE_FAIL;
             ZL303XX_ERROR_NOTIFY("Failed to create SPI mutex");
         }
      }
   }

   /* Create the 'spiIsrSemId' Semaphore if it does not already exist */
   if (cpuStatus == CPU_OK)
   {
      if (spiIsrSemId == OS_SEM_INVALID)
      {
         spiIsrSemId = OS_SEMA4_CREATE_BINARY(SEM_EMPTY);

         if (spiIsrSemId == OS_SEM_INVALID)
         {
             cpuStatus = CPU_SPI_SEM_CREATE_FAIL;
             ZL303XX_ERROR_NOTIFY("Failed to create SPI semaphore");
         }
      }
   }

   /* Initialize the Interrupt and ISR. */
   if (cpuStatus == CPU_OK)
   {
       /* Attach interrupt handlers for hardware */
       osStatus = OS_INTERRUPT_CONNECT((OS_VOID_FUNC_PTR *)spi_irq,
               (OS_VOID_FUNC_PTR)cpuSpiIsrHandler, 0);

       if (osStatus != OS_OK)
       {
           cpuStatus = CPU_SPI_ISR_CONNECT_FAIL;
           ZL303XX_ERROR_NOTIFY("Interrupt connect failed");
       }
   }

   if (cpuStatus == CPU_OK)
   {
       /* Enable the interrupt */
       osStatus = OS_INTERRUPT_ENABLE(spi_irq);

       if (osStatus != OS_OK)
       {
           cpuStatus = CPU_SPI_ISR_ENABLE_FAIL;
           ZL303XX_ERROR_NOTIFY("Interrupt enable failed");
       }
   }

   return cpuStatus;
}  /* END zl303xx_SpiInitInterrupts */

/**

  Function Name:
    zl303xx_SpiDisableInterrupts

  Details:
   Deletes the SPI Mutex, the ISR Tx/Rx Completed Semaphore and disconnects the SPI
   Interrupt from the SPI Interrupt Service Routine.

  Parameters:
        None

  Return Value:
   zlStatusE

*******************************************************************************/

zlStatusE zl303xx_SpiDisableInterrupts(Uint32T spi_irq)
{
   cpuStatusE cpuStatus = CPU_OK;
   OS_STATUS osStatus;

   if (cpuStatus == CPU_OK)
   {
       /* Disable the interrupt */
       osStatus = OS_INTERRUPT_DISABLE(spi_irq);

       if (osStatus != OS_OK)
       {
           cpuStatus = CPU_SPI_ISR_DISABLE_FAIL;
           ZL303XX_ERROR_NOTIFY("Interrupt disable failed");
       }
   }

   /* Delete the 'spiMutexId' Mutex if it exist */
   if (cpuStatus == CPU_OK)
   {
      if (spiMutexId != OS_MUTEX_INVALID)
      {
         osStatus = OS_MUTEX_DELETE(spiMutexId);

         if(osStatus != OS_OK)
         {
             cpuStatus = CPU_SPI_SEM_DELETE_FAIL;
             ZL303XX_ERROR_NOTIFY("Failed to delete SPI mutex");
         }
         else
         {
            spiMutexId = OS_MUTEX_INVALID;
         }
      }
   }

   /* Delete the 'spiIsrSemId' Semaphore if it exist */
   if (cpuStatus == CPU_OK)
   {
      if (spiIsrSemId != OS_SEM_INVALID)
      {
         osStatus = OS_SEMA4_DELETE(spiIsrSemId);

         if (osStatus == OS_ERROR)
         {
            cpuStatus = CPU_SPI_SEM_DELETE_FAIL;
            ZL303XX_ERROR_TRAP("Interrupt semaphore delete failed");
         }
         else
         {
            spiIsrSemId = OS_SEM_INVALID;
         }
      }
   }

   return cpuStatus;
}  /* END zl303xx_SpiDisableInterrupts */


/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/*****************   END   ****************************************************/

