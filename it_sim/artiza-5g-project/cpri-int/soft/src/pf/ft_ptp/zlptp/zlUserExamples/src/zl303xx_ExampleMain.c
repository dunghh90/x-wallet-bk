

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Main example application module.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/

#include <sys/types.h> 
#include <sys/wait.h>
#include <unistd.h>

#include "zl303xx_Global.h"
#include "zl303xx_Os.h"
#include "zl303xx_Trace.h"
#include "zl303xx_ExampleMain.h"
#include "zl303xx_Var.h"

#include "f_rrh_def.h"
#include "f_du_reg_eep.h"
/* #include "f_com_ext.h" */
/* #include "../../../cmn/com/inc/f_cmn_com_ext.h" */
/* #include "../../../cmn/com/inc/f_cmn_com_def.h" */

#include "f_rrh_inc.h"
#include "BPF_RU_IPCM.h"
#include "BPF_HM_DEVC.h"
#include "f_sys_def.h"
#include "f_du_reg_eth.h"

#define D_SYS_THDID_PTP_MAIN								0x00000050


#if defined OS_LINUX
#include "zl303xx_LinuxOs.h"
#endif

#if defined APR_INCLUDED
#include "zl303xx_Apr1Hz.h"
#include "zl303xx_ExampleAprGlobals.h"
#endif

#if defined ZLS30341_INCLUDED
#if defined ZLS30341_INCLUDED
#include "zl303xx_Example34x.h"
#endif
#include "zl303xx_Dpll34xDpllLow.h"
#endif

#include "zl303xx_DeviceSpec.h"
#include "zl303xx_Params.h"

#if defined ZLS30390_INCLUDED
#include "zl303xx_PtpApi.h"
#include "zl303xx_PtpC37p238Api.h"
#endif

#if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD) && defined OS_LINUX
#include "zl303xx_ZLE30360.h"
#endif


#if defined ZLS30361_INCLUDED
#include "zl303xx_Example36x.h"
#endif

#if defined ZLS30701_INCLUDED
#include "zl303xx_Example70x.h"
#endif

#if defined ZLS30721_INCLUDED
#include "zl303xx_Example72x.h"
#endif

#if defined ZLS30751_INCLUDED
#include "zl303xx_Example75x.h"
#endif

#if defined ZLS30771_INCLUDED
#include "zl303xx_Example77x.h"
#endif


#if defined ZLS30751_INCLUDED
#include "zl303xx_Dpll75x.h"
#endif

#if defined ZLS30721_INCLUDED
#include "zl303xx_Dpll72x.h"
#endif

#if defined ZLS30701_INCLUDED
#include "zl303xx_Dpll70x.h"
#endif

#if defined ZLS30771_INCLUDED
#include "zl303xx_Dpll77x.h"
#endif

#if defined ZLS30390_INCLUDED
#include "zl303xx_ExamplePtp.h"
#endif

#if defined ZLS30361_INCLUDED && (defined _ZL303XX_ZLE1588_BOARD || defined _ZL303XX_ZLE30360_BOARD)
#include "zl303xx_RdWr.h"
#endif


#if defined ZLS30390_INCLUDED
    #if (defined _ZL303XX_ZLE1588_BOARD || defined _ZL303XX_ZLE30360_BOARD )
        #include "zl303xx_ApiConfig.h"
    #endif
#endif

/*****************   DEFINES     **********************************************/
/* Use the same src and dest addresses in all examples */
#define DEFAULT_SLAVE_IPV4_ADDRESS  "10.0.0.235"
#define DEFAULT_MASTER_IPV4_ADDRESS "10.0.0.230"


/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/
#if defined ZLS30701_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
zl303xx_ParamsS *zl303xx_Params1;   /* The device instance structure for the 2nd device for the example code*/
#endif

zl303xx_BooleanE exampleAppRunning = ZL303XX_FALSE;
/*****************   EXPORTED GLOBAL VARIABLES   ******************************/
#if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
/* Only PLL1 has a valid NCO clock to control the TSU on the ZLE30360 and ZLE1588 boards */
#if defined ZLE30360_770_FRANKEN_BOARD
    Uint8T TARGET_DPLL = 2;
#else
    Uint8T TARGET_DPLL = 1;
#endif
#else
Uint8T TARGET_DPLL = 0;
#endif

Uint8T SYNCE_DPLL = 1;
Uint8T CHASSIS_DPLL = 2;
/* The NCO-assist DPLL for ZL30771, ZL30772, and ZL30773 is DPLL 3. */
Uint8T NCO_ASSIST_DPLL = 3;

extern char examplePtpLocalIpv4[];
extern char examplePtpDestIpv4[];

exampleAppS zlExampleApp;

zl303xx_BooleanE reset1588HostRegisterOnClockCreate = ZL303XX_TRUE;

zl303xx_ParamsS *zl303xx_Params0 = NULL;

extern void f_cmn_com_abort(unsigned int processID, unsigned int almCode, const void * fileName, unsigned int lineNo );

#define  f_com_abort( processID, almCode ) f_cmn_com_abort( processID, almCode, __FILE__, __LINE__)

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/
int BPF_HM_DEVC_EEPROM_READ(unsigned short offset, unsigned char *data_p);

/* exampleShutdown */
/**
   Stops the app and closes the environment

*******************************************************************************/
zlStatusE exampleShutdown(void)
{
   zlStatusE status = ZL303XX_OK;

   if (status == ZL303XX_OK)
   {
      status = exampleAppStop(&zlExampleApp);
   }

   if (status == ZL303XX_OK)
   {
      status = exampleEnvClose();
   }


   return status;
}



/* exampleEnvInit */
/**
   Initializes all global environment variables.

*******************************************************************************/
zlStatusE exampleEnvInit(void)
{
   zlStatusE status = ZL303XX_OK;
   FILE *fp = NULL;

   OS_SETUP(); /* O/S-specific setup functions - e.g., signal handler task for Linux */

   /* zl303xx_TraceInit(stdout); */
   zl303xx_TraceInit(fp);				/* ログファイルオープン */

   if (exampleAppIsRunning() == ZL303XX_TRUE)
   {
      ZL303XX_TRACE_ALWAYS("An app is currently running, stop it before starting another app",0,0,0,0,0,0);
      status = ZL303XX_UNSUPPORTED_OPERATION;
   }

#if defined ZLS30390_INCLUDED
   if (status == ZL303XX_OK)
   {
      status = examplePtpEnvInit();
   }
#endif

#if defined APR_INCLUDED
   if (status == ZL303XX_OK)
   {
      status = exampleAprEnvInit();
   }
#endif

#if defined ZLS30341_INCLUDED
   if (status == ZL303XX_OK)
   {
      if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE )
        )
      {
         status = example34xEnvInit();
      }
   }
#endif

#if defined ZLS30361_INCLUDED
   if (status == ZL303XX_OK)
   {
      if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
      {
         status = example36xEnvInit();
      }
   }
#endif

#if defined ZLS30721_INCLUDED
   if (status == ZL303XX_OK)
   {
      if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
      {
         status = example72xEnvInit();
      }
   }
#endif

#if defined ZLS30701_INCLUDED
   if (status == ZL303XX_OK)
   {
      if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
      {
         status = example70xEnvInit();
      }
   }
#endif
#if defined ZLS30751_INCLUDED
   if (status == ZL303XX_OK)
   {
      if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
      {
         status = example75xEnvInit();
      }
   }
#endif
#if defined ZLS30771_INCLUDED
   if (status == ZL303XX_OK)
   {
      if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
      {
         status = example77xEnvInit();
      }
   }
#endif

   /* #warning MSCC: ZL303XX_MOD_ID_NOTIFY enabled */
   /* zl303xx_TraceSetLevel(ZL303XX_MOD_ID_NOTIFY, 1); */ /* Default notifications handling is ON to aid debugging */
   
   zl303xx_AprLogTimestampInputStart(2);			/* APRログにT1,T2,T3,T4 timestamp情報も付与	*/
   zl303xx_SetAprLogLevel(3);						/* APRログレベルをLevel=3に設定	*/
   zl303xx_TraceSetLevelAll(1);						/* Level=1に設定	*/
   zl303xx_TraceSetLevel(ZL303XX_MOD_ID_PLL, 2);	/* DPLLログはLevel=2に設定	*/
   zl303xx_TraceSetLevel(ZL303XX_MOD_ID_RDWR, 0);	/* SPIアクセスをログミュート */

    /*
   zl303xx_TraceSetLevel(ZL303XX_MOD_ID_PTP_ENGINE, 3);
   zl303xx_TraceSetLevel(ZL303XX_MOD_ID_PTP_BMC, 3);
   zl303xx_TraceSetLevel(ZL303XX_MOD_ID_PTP_STATE_UPD, 3);
   */

   return status;
}

/* exampleEnvClose */
/**
   Destroys global environment variables.

*******************************************************************************/
zlStatusE exampleEnvClose(void)
{
   zlStatusE status = ZL303XX_OK;


#if defined ZLS30390_INCLUDED
   if (status == ZL303XX_OK &&
       (status = examplePtpEnvClose()) != ZL303XX_OK)
   {
       ZL303XX_TRACE_ALWAYS("exampleEnvClose: Call to examplePtpEnvClose() failure=%d", status, 0,0,0,0,0);
   }
#endif

#if defined APR_INCLUDED
   if (status == ZL303XX_OK &&
       (status = exampleAprEnvClose()) != ZL303XX_OK)
   {
       ZL303XX_TRACE_ALWAYS("exampleEnvClose: Call to exampleAprEnvClose() failure=%d", status, 0,0,0,0,0);
   }
#endif

#if defined ZLS30341_INCLUDED
   if (status == ZL303XX_OK )
   {
      if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE ))
      {
         if((status = example34xEnvClose()) != ZL303XX_OK)
         {
            ZL303XX_TRACE_ALWAYS("exampleEnvClose: Call to example34xEnvClose() failure=%d", status, 0,0,0,0,0);
         }
      }
   }
#endif

#if defined ZLS30361_INCLUDED
   if (status == ZL303XX_OK )
   {
      if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
      {
         if((status = example36xEnvClose()) != ZL303XX_OK)
         {
            ZL303XX_TRACE_ALWAYS("exampleEnvClose: Call to example36xEnvClose() failure=%d", status, 0,0,0,0,0);
         }
      }
   }
#endif

#if defined ZLS30721_INCLUDED
   if (status == ZL303XX_OK )
   {
      if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
      {
         if((status = example72xEnvClose()) != ZL303XX_OK)
         {
             ZL303XX_TRACE_ALWAYS("exampleEnvClose: Call to example72xEnvClose() failure=%d", status, 0,0,0,0,0);
         }
      }
   }
#endif

#if defined ZLS30701_INCLUDED
   if (status == ZL303XX_OK )
   {
      if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
      {
         if((status = example70xEnvClose()) != ZL303XX_OK)
         {
            ZL303XX_TRACE_ALWAYS("exampleEnvClose: Call to example70xEnvClose() failure=%d", status, 0,0,0,0,0);
         }
      }
   }
#endif

#if defined ZLS30751_INCLUDED
   if (status == ZL303XX_OK )
   {
      if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
      {
         if((status = example75xEnvClose()) != ZL303XX_OK)
         {
            ZL303XX_TRACE_ALWAYS("exampleEnvClose: Call to example75xEnvClose() failure=%d", status, 0,0,0,0,0);
         }
      }
   }
#endif

#if defined ZLS30771_INCLUDED
   if (status == ZL303XX_OK )
   {
      if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
      {
         if((status = example77xEnvClose()) != ZL303XX_OK)
         {
            ZL303XX_TRACE_ALWAYS("exampleEnvClose: Call to example77xEnvClose() failure=%d", status, 0,0,0,0,0);
         }
      }
   }
#endif



   zl303xx_TraceClose(fdOfTrace);

   OS_CLEANUP(); /* O/S-specific cleanup functions - e.g., shutdown signal handler task for Linux */

   return status;
}

/* exampleAppStructInit */
/**
   Initializes an exampleAppS structure and allocates memory for an arbitrary
   number of clocks, ports, and streams.

*******************************************************************************/
zlStatusE exampleAppStructInit(Uint32T numClocks,  Uint32T numPorts,
                               Uint32T numStreams, exampleAppS *pApp)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T count;

   ZL303XX_CHECK_POINTER(pApp);

   if (exampleAppIsRunning() == ZL303XX_TRUE)
   {
      ZL303XX_TRACE_ALWAYS("An appl is currently running, stop it before starting another appl",0,0,0,0,0,0);
      status = ZL303XX_UNSUPPORTED_OPERATION;
   }

    /* Allocate memory */
   if (status == ZL303XX_OK)  /* Clocks */
   {
      pApp->clockCount = numClocks;

      if (numClocks > 0)
      {
         pApp->clock = OS_CALLOC(numClocks, sizeof(*(pApp->clock)));    /* Allocate and think of them like an array of CLOCKs */
         if (pApp->clock == NULL)
         {
            pApp->clockCount = 0;
            status = ZL303XX_RTOS_MEMORY_FAIL;
         }
      }
   }

   if (status == ZL303XX_OK)  /* Ports */
   {
      pApp->portCount = numPorts;

      if (numPorts > 0)
      {
         pApp->port = OS_CALLOC(numPorts, sizeof(*(pApp->port)));       /* Array of PORTs */
         if (pApp->port == NULL)
         {
            pApp->portCount = 0;
            status = ZL303XX_RTOS_MEMORY_FAIL;
         }
      }
   }

   if (status == ZL303XX_OK)  /* Steams */
   {
      pApp->streamCount = numStreams;

      if (numStreams > 0)
      {
         pApp->stream = OS_CALLOC(numStreams, sizeof(*(pApp->stream))); /* Array of STREAMs */

         if (pApp->stream == NULL)
         {
            pApp->streamCount = 0;
            status = ZL303XX_RTOS_MEMORY_FAIL;
         }
      }
   }

#if defined MUX_PTP_STREAMS_TO_APR_SLOTS 
   if (status == ZL303XX_OK)  /* Steams */
   {
       zl303xx_InitPtpStreamMuxToAprSlots();
   }
#endif

   /* Initialize all of the example structures. */
   if (status == ZL303XX_OK)
   {
      for (count = 0; count < numClocks; count++)   /* CLOCKS */
      {
      #if defined APR_INCLUDED
         if (status == ZL303XX_OK)
         {
            status |= exampleAprClockCreateStructInit(&pApp->clock[count].apr);
         }
         if (status != ZL303XX_OK)
            ZL303XX_TRACE_ALWAYS("exampleAppStructInit: exampleAprClockCreateStructInit(%d) failed with status = %lu",
                                 count, status, 0,0,0,0);
      #endif

      #if defined ZLS30390_INCLUDED
         if (status == ZL303XX_OK)
         {
            status |= examplePtpClockCreateStructInit(&pApp->clock[count].ptp);
            if (status != ZL303XX_OK)
            {
               ZL303XX_TRACE_ALWAYS("exampleAppStructInit: examplePtpClockCreateStructInit(%d) failed with status = %lu",
                                    count, status, 0,0,0,0);
            }
         }
      #endif

      #if defined ZLS30341_INCLUDED
         if (status == ZL303XX_OK)
         {
            if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE )
              )
            {
               status |= example34xClockCreateStructInit(&pApp->clock[count].zl3034x);
               if (status != ZL303XX_OK)
                  ZL303XX_TRACE_ALWAYS("exampleAppStructInit: example34xClockCreateStructInit(%d) failed with status = %lu",
                                    count, status, 0,0,0,0);
            }
         }
      #endif

      #if defined ZLS30361_INCLUDED
         if (status == ZL303XX_OK)
         {
            if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
            {
               status |= example36xClockCreateStructInit(&pApp->clock[count].zl3036x);
               if (status != ZL303XX_OK)
                  ZL303XX_TRACE_ALWAYS("exampleAppStructInit: example36xClockCreateStructInit(%d) failed with status = %lu",
                                    count, status, 0,0,0,0);
               else
               {
                   pApp->clock[count].zl3036x.pllId = count;
               /* #warning MSCC: Hook an example sticky lock register handling callout function on this clock. */
                   pApp->clock[count].zl3036x.zl303xx_Params->pllParams.stickyLockCallout = (hwFuncPtrStickyLock) example36xStickyLockCallout;
               }
            }
         }
      #endif


      #if defined ZLS30721_INCLUDED
         if (status == ZL303XX_OK)
         {
            if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
            {
               status |= example72xClockCreateStructInit(&pApp->clock[count].zl3072x);
               if (status != ZL303XX_OK)
                  ZL303XX_TRACE_ALWAYS("exampleAppStructInit: example72xClockCreateStructInit(%d) failed with status = %lu",
                                       count, status, 0,0,0,0);
               else
               {
                   pApp->clock[count].zl3072x.pllId = count;

                   if ( count == 0)
                   {
                       zl303xx_Params1 =  pApp->clock[count].zl3072x.zl303xx_Params;
                   }
                   else if ( count == 1)
                   {
                       zl303xx_Params0 =  pApp->clock[count].zl3072x.zl303xx_Params;
                   }
                   else
                   {
                       status = ZL303XX_INVALID_POINTER;
                       ZL303XX_TRACE_ALWAYS("exampleAppStructInit: Incorrect zl303xx_ParamsX pointer; status = %lu",
                                         status, 0,0,0,0,0);
                   }
               }
            }
         }
      #endif

      #if defined ZLS30701_INCLUDED
         if (status == ZL303XX_OK)
         {
            if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
            {
               status |= example70xClockCreateStructInit(&pApp->clock[count].zl3070x);
               if (status != ZL303XX_OK)
                  ZL303XX_TRACE_ALWAYS("exampleAppStructInit: example70xClockCreateStructInit(%d) failed with status = %lu",
                                    count, status, 0,0,0,0);
               else
               {
                   pApp->clock[count].zl3070x.pllId = count;
               /* #warning MSCC: Hook an example sticky lock register handling callout function on this clock. */
                   pApp->clock[count].zl3070x.zl303xx_Params->pllParams.d70x.stickyLockCallout = (hwFuncPtrStickyLock) example70xStickyLockCallout;
               }
            }
         }
      #endif

      #if defined ZLS30751_INCLUDED
         if (status == ZL303XX_OK)
         {
            if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
            {
               status |= example75xClockCreateStructInit(&pApp->clock[count].zl3075x);
               if (status != ZL303XX_OK)
                  ZL303XX_TRACE_ALWAYS("exampleAppStructInit: example75xClockCreateStructInit(%d) failed with status = %lu",
                                    count, status, 0,0,0,0);
               else
               {
                   pApp->clock[count].zl3075x.pllId = count;
               /* #warning MSCC: Hook an example sticky lock register handling callout function on this clock. */
                   pApp->clock[count].zl3075x.zl303xx_Params->pllParams.d75x.stickyLockCallout = (hwFuncPtrStickyLock) example75xStickyLockCallout;
               }
            }
         }
      #endif

      #if defined ZLS30771_INCLUDED
         if (status == ZL303XX_OK)
         {
            if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
            {
               status |= example77xClockCreateStructInit(&pApp->clock[count].zl3077x);
               if (status != ZL303XX_OK)
                  ZL303XX_TRACE_ALWAYS("exampleAppStructInit: example77xClockCreateStructInit(%d) failed with status = %lu",
                                    count, status, 0,0,0,0);
               else
               {
                   pApp->clock[count].zl3077x.pllId = count;
               /* #warning MSCC: Hook an example sticky lock register handling callout function on this clock. */
                   pApp->clock[count].zl3077x.zl303xx_Params->pllParams.d77x.stickyLockCallout = (hwFuncPtrStickyLock) example77xStickyLockCallout;
               }
            }
         }
      #endif
      } /* Clocks */


   #if defined ZLS30390_INCLUDED
      if (status == ZL303XX_OK)
      {
          for (count = 0; count < numPorts; count++)    /* PORTS */
          {
             pApp->port[count].exampleInterface = ZL303XX_EXAMPLE_INTERFACE_IGNORE; /* Don't call zl303xx_SetExampleInterface automatically (legacy behaviour) */
             pApp->port[count].exampleInterfaceCustomStr[0] = '\0';
             status |= examplePtpPortCreateStructInit(&pApp->port[count].ptp, count);
              if (status != ZL303XX_OK)
                  ZL303XX_TRACE_ALWAYS("exampleAppStructInit: examplePtpPortCreateStructInit(%d) failed with status = %lu",
                                     count, status, 0,0,0,0);
          } /* Ports */
      }
   #endif

   #if defined APR_INCLUDED || defined ZLS30390_INCLUDED
      if (status == ZL303XX_OK)
      {
          for (count = 0; count < numStreams; count++)  /* STREAMS */
          {
          #if defined ZLS30390_INCLUDED
             status |= examplePtpStreamCreateStructInit(&pApp->stream[count].ptp,count);
             if (status != ZL303XX_OK)
                ZL303XX_TRACE_ALWAYS("exampleAppStructInit: examplePtpStreamCreateStructInit(%d) failed with status = %lu",
                                     count, status, 0,0,0,0);
          #endif

          #if defined APR_INCLUDED
             if (status == ZL303XX_OK)
             {
                /* #warning MSCC: Inform APR of Packet Rates */
                 /* Inform APR of current pkt rates */
                 if (status == ZL303XX_OK)
                 {
                 #if defined ZLS30390_INCLUDED && (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD ) && defined OS_LINUX
                     /* #warning MSCC: Using server 0 for starting PPS rates */
                     zl303xx_AprSetPktRate(zl303xx_GetPps(0), ZL303XX_TRUE);
                     zl303xx_AprSetPktRate(zl303xx_GetDelayReqPps(0), ZL303XX_FALSE);
                 #else
                     /* #warning MSCC: APR will use the default SYNC and DR PPS Rates (64/16) */
                 #endif
                 }
                 status |= exampleAprStreamCreateStructInit(&pApp->stream[count].apr);

                 if (status != ZL303XX_OK)
                 {
                     ZL303XX_TRACE_ALWAYS("exampleAppStructInit: exampleAprStreamCreateStructInit(%d) failed with status = %lu",
                                        count, status, 0,0,0,0);
                 }
             }
          #endif
          } /* Streams */
      }
   #endif
   }

   return status;
}   /* exampleAppStructInit() */

/* exampleAppStructFree */
/**
   Frees memory used by an exampleAppS structure and sets counters to 0.

*******************************************************************************/
zlStatusE exampleAppStructFree(exampleAppS *pApp)
{
    if(pApp->stream)
    {
         OS_FREE(pApp->stream);
         pApp->stream = NULL;
         pApp->streamCount = 0;
    }

    if (pApp->port)
    {
        OS_FREE(pApp->port);
        pApp->port = NULL;
        pApp->portCount = 0;
    }

    if (pApp->clock)
    {
        OS_FREE(pApp->clock);
        pApp->clock = NULL;
        pApp->clockCount = 0;
    }

   return ZL303XX_OK;
}

static zlStatusE exampleAppStart_FjdeviceSetup( void )
{
	int res;
	unsigned char	data;
	int wres;
	unsigned int errcode = 0;
	zlStatusE status;

	if(zl303xx_Params0 != NULL){
		status  = zl303xx_Write(zl303xx_Params0, NULL,ZL303XX_MAKE_MEM_ADDR_77X(0x237, ZL303XX_MEM_SIZE_1_BYTE),0x02);
		status |= zl303xx_Write(zl303xx_Params0, NULL,ZL303XX_MAKE_MEM_ADDR_77X(0x238, ZL303XX_MEM_SIZE_1_BYTE),0x01);
		status |= zl303xx_Write(zl303xx_Params0, NULL,ZL303XX_MAKE_MEM_ADDR_77X(0x230, ZL303XX_MEM_SIZE_1_BYTE),0x03);
		usleep(510000);	/* 500msec + 10msec wait */
		status |= zl303xx_Write(zl303xx_Params0, NULL,ZL303XX_MAKE_MEM_ADDR_77X(0x230, ZL303XX_MEM_SIZE_1_BYTE),0x00);
		if(ZL303XX_OK != status){
			ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: zl303xx_Write status error : %d",status, 0,0,0,0,0);
		}
	}else{
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: zl303xx_Write zl303xx_Params0 error ",0, 0,0,0,0,0);
	}

	/* 機種種別取得 */
	BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_DEVICE_KIND0, &data );
	if( data == D_DU_EEP_DEV_KIND_5GDU_SUB6 ) {
		system("rrh -jcpllinit_sub6");
	} else if ( data == D_DU_EEP_DEV_KIND_5GDU_MMW ) {
		system("rrh -jcpllinit_mmw");
	} else {
		errcode = D_DU_FLTID_DEVKIND_ABNORMAL;
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: devkind is not Sub6 or mmW. reg= %d",data, 0,0,0,0,0);
		/* abort処理(f_com_abort())を呼ぶ	*/
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_DEVKIND_ABNORMAL);
		return ZL303XX_ERROR;
	}
	res = system("rrh -chkllbreset");
	wres = WEXITSTATUS(res);
	
	if (wres == 3){
		errcode = D_DU_FLTID_LLB_SYS_PLL_NG;
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: LLB SYS PLL NG. chkllbreset command return= %d",wres, 0,0,0,0,0);
		/* abort処理(f_com_abort())を呼ぶ	*/
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_LLB_SYS_PLL_NG);
	}else if (wres == 4){
		errcode = D_DU_FLTID_TRX1_SYS_PLL_NG;
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: TRX#1 SYS PLL NG. chkllbreset command return= %d",wres, 0,0,0,0,0);
		/* abort処理(f_com_abort())を呼ぶ	*/
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_TRX1_SYS_PLL_NG);
	}else if (wres == 6){
		errcode = D_DU_FLTID_LLB_IC_PLL_NG;
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: LLB IC PLL NG. chkllbreset command return= %d",wres, 0,0,0,0,0);
		/* abort処理(f_com_abort())を呼ぶ	*/
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_LLB_IC_PLL_NG);
	}else if (wres == 7){
		errcode = D_DU_FLTID_TRX1_IC_PLL_NG;
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: TRX#1 IC PLL NG. chkllbreset command return= %d",wres, 0,0,0,0,0);
		/* abort処理(f_com_abort())を呼ぶ	*/
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_TRX1_IC_PLL_NG);
	}else{
		errcode = 0x000;
	}
	if (errcode != 0x000){

		return ZL303XX_ERROR;
	}
	
	res = system("rrh -icbicxinit");
	wres = WEXITSTATUS(res);
	
	if (wres == 2){
		errcode = D_DU_FLTID_IC_EST_NG01;
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: IC establish NG1. icbicxinit command return= %d",wres, 0,0,0,0,0);
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_IC_EST_NG01);
	}else if (wres == 3){
		errcode = D_DU_FLTID_IC_EST_NG02;
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: IC establish NG2. icbicxinit command return= %d",wres, 0,0,0,0,0);
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_IC_EST_NG02);
	}else if (wres == 4){
		errcode = D_DU_FLTID_IC_EST_NG03;
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: IC establish NG3. icbicxinit command return= %d",wres, 0,0,0,0,0);
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_IC_EST_NG03);
	}else if (wres == 5){
		errcode = D_DU_FLTID_IC_EST_NG04;
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: IC establish NG4. icbicxinit command return= %d",wres, 0,0,0,0,0);
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_IC_EST_NG04);
	}else if (wres == 6){
		errcode = D_DU_FLTID_IC_EST_NG05;
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: IC establish NG5. icbicxinit command return= %d",wres, 0,0,0,0,0);
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_IC_EST_NG05);
	}else if (wres == 7){
		errcode = D_DU_FLTID_IC_EST_NG06;
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: IC establish NG6. icbicxinit command return= %d",wres, 0,0,0,0,0);
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_IC_EST_NG06);
	}else if (wres == 8){
		errcode = D_DU_FLTID_IC_EST_NG07;
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: IC establish NG7. icbicxinit command return= %d",wres, 0,0,0,0,0);
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_IC_EST_NG07);
	}else if (wres == 9){
		errcode = D_DU_FLTID_IC_EST_NG08;
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: IC establish NG8. icbicxinit command return= %d",wres, 0,0,0,0,0);
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_IC_EST_NG08);
	}else if (wres == 10){
		errcode = D_DU_FLTID_IC_EST_NG09;
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: IC establish NG9. icbicxinit command return= %d",wres, 0,0,0,0,0);
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_IC_EST_NG09);
	}else if (wres == 11){
		errcode = D_DU_FLTID_IC_EST_NG10;
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: IC establish NG10. icbicxinit command return= %d",wres, 0,0,0,0,0);
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_IC_EST_NG10);
	}else{
		errcode = 0x000;
	}
	
	if (errcode != 0x000){
		return ZL303XX_ERROR;
	}
	
	/* PTP初期化処理削除によるコマンド実行 */
	res = system("rrh -etherinit");
	wres = WEXITSTATUS(res);
	if (wres == 1){
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: ETH PLL NG. etherinit command return= %d",wres, 0,0,0,0,0);
		/* abort処理(f_com_abort())を呼ぶ	*/
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_ETH_PLL_NG_INIT);
		return ZL303XX_ERROR;
	}else if (wres == 2){
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: TX DONE NG",0,0,0,0,0,0);
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_FH1_TX_NOT_DONE);
		return ZL303XX_ERROR;
	}else if(wres == 3){
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: RX DONE NG",0,0,0,0,0,0);
		f_com_abort(D_SYS_THDID_PTP_MAIN, D_DU_ALMCD_FH1_RX_NOT_DONE);
		return ZL303XX_ERROR;
	}else{
		ZL303XX_TRACE_ALWAYS("exampleAppStart_FjdeviceSetup: etherinit OK",0,0,0,0,0,0);
	}

	system("insmod /opt/lkm/fheth.ko");
	system("echo 1 > /proc/irq/38/smp_affinity");
	system("echo 4 > /proc/irq/39/smp_affinity");

	UCHAR	mac_ptn;
	UINT	reg_data1;
	UINT	reg_data2;

	BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_VID_MULTI_MACADDR, &mac_ptn );
	if( mac_ptn == 0x01 ) {

		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_DU_REG_ETH_FILTDA21,&reg_data1);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_DU_REG_ETH_FILTDA22,&reg_data2);
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_DU_REG_ETH_FILTDA21,&reg_data1);
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_DU_REG_ETH_FILTDA22,&reg_data2);

		reg_data1=0;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_DU_REG_ETH_FILTDA31,&reg_data1);
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_DU_REG_ETH_FILTDA32,&reg_data1);

	}else{
		reg_data1=0;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_DU_REG_ETH_FILTDA21,&reg_data1);
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_DU_REG_ETH_FILTDA22,&reg_data1);


		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_DU_REG_ETH_FILTDA31,&reg_data1);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_DU_REG_ETH_FILTDA32,&reg_data2);
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_DU_REG_ETH_FILTDA31,&reg_data1);
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_DU_REG_ETH_FILTDA32,&reg_data2);

	}

	system("rrh -ifup fheth0");
	system("rrh -ifup fhethS0");
	system("rrh -etherup");
	return ZL303XX_OK;
}

/* exampleAppStart */
/**
   Example code to start an "application" with the provided configuration.

*******************************************************************************/
zlStatusE exampleAppStart(exampleAppS *pApp)
{
   zlStatusE status = ZL303XX_OK;
   Uint32T count;

   ZL303XX_CHECK_POINTER(pApp);

   if (exampleAppIsRunning() == ZL303XX_TRUE)
   {
      ZL303XX_TRACE_ALWAYS("exampleAppStart: An appl is currently running, stop it before starting another appl",0,0,0,0,0,0);
      status = ZL303XX_UNSUPPORTED_OPERATION;
   }
   else
      exampleAppRunning = ZL303XX_TRUE;

   /* Create Clocks */
   if (status == ZL303XX_OK)
   {
      for (count = 0; count < pApp->clockCount; count++)    /* Create Clocks */
      {
          if ((status = exampleAppStartClock(pApp, count)) != ZL303XX_OK)
          {
              ZL303XX_TRACE_ALWAYS("exampleAppStart() exampleAppStartClock(%d) failure=%d.", count, status, 0,0,0,0);
              break;
          }
      }
#if defined ZLS30771_INCLUDED && defined APR_INCLUDED
      if (status == ZL303XX_OK)
      {
         if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
         {
            /* For the ZL30770 series, an extra DPLL is used to enhance
               ACI_BC_FULL_ON_PATH_PHASE_SYNCE. We need to maintain a mapping
               between APR's DPLL and the NCO-assist DPLL.

               TARGET_DPLL is APR's DPLL.
               The NCO-assist DPLL for ZL30771, ZL30772, and ZL30773 is DPLL 3.
            */
            zl303xx_Dpll77xSetNCOAssistParamsSAssociation(pApp->clock[TARGET_DPLL].apr.cguId,
                                           pApp->clock[NCO_ASSIST_DPLL].apr.cguId);
         }
      }
#endif
   }

	if (status == ZL303XX_OK)
	{
		status = exampleAppStart_FjdeviceSetup();
	}

    /* Create Ports */
    if (status == ZL303XX_OK)
    {
        for (count = 0; count < pApp->portCount; count++)
        {
            if ((status = exampleAppStartPort(pApp, count)) != ZL303XX_OK)
            {
                break;
            }
        }
    }

    /* Create Streams */
    if (status == ZL303XX_OK)
    {
        for (count = 0; count < pApp->streamCount; count++)
        {
            pApp->srvId = count; /* Used for streamHandle (requestedHandle) */

            if ((status = exampleAppStartStream(pApp, count)) != ZL303XX_OK)
            {
                break;
            }
        }
    }

    ZL303XX_TRACE_ALWAYS("exampleAppStart: Completed with status=%d", status, 0,0,0,0,0);

    return status;
}


zlStatusE exampleAppStartClock(exampleAppS *pApp, Uint32T indx)
{
    zlStatusE status = ZL303XX_OK;
    exampleAppClockS *pClock = &pApp->clock[indx];
    zl303xx_ParamsS *cguId = NULL;   /* Associates a hardware device with APR and/or PTP */


#if defined APR_INCLUDED
    if (status == ZL303XX_OK)
    {
        /* The clock we generated in exampleAppStructInit()*/
        cguId = pApp->clock[indx].apr.cguId;
    }
#endif

#if defined ZLS30341_INCLUDED
    if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE )
      )
    {
        pClock->zl3034x.pllId = indx;

        if (status == ZL303XX_OK)
        {
        #if defined _ZL303XX_ZLE1588_BOARD
            if (indx != TARGET_DPLL)    /* SYNCE device on EVB*/
            {
                pClock->zl3034x.deviceMode = ZL303XX_MODE_REF_EXTERNAL_FORCED;
            }
            else
        #endif
            {
                if (pClock->zl3034x.deviceMode == ZL303XX_MODE_UNKNOWN)
                {
                   pClock->zl3034x.deviceMode = pClock->pktRef ? ZL303XX_MODE_REF_TOP
                                                   : ZL303XX_MODE_REF_EXTERNAL;
                }
            }

            status = example34xClockCreate(&pClock->zl3034x);
            if (status == ZL303XX_OK)
            {
                cguId = pClock->zl3034x.zl303xx_Params;
                /* Align with your PLL mode requirements (within example34xClockCreate()) */
                /* #warning MSCC: Align with your PLL mode requirements */
                pClock->zl3034x.zl303xx_Params->pllParams.config[indx].pllPriorMode = ZL303XX_DPLL_MODE_AUTO;
                pClock->zl3034x.zl303xx_Params->pllParams.config[indx].selectedRef = ZL303XX_REF_AUTO;
            }
            else
            {
                ZL303XX_TRACE_ALWAYS("exampleAppStartClock: Call to example34xClockCreate(%d) failed=%d",
                                    indx, status, 0,0,0,0);
            }
        }
    }
#endif

#if defined ZLS30361_INCLUDED
    if (status == ZL303XX_OK)
    {
        if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
        {
            pClock->zl3036x.pllId = indx;

            if (pClock->zl3036x.deviceMode == ZL303XX_MODE_UNKNOWN)
            {
               pClock->zl3036x.deviceMode = pClock->pktRef ? ZL303XX_MODE_REF_TOP
                                           : ZL303XX_MODE_REF_EXTERNAL;
            }

            status = example36xClockCreate(&pClock->zl3036x);
            cguId = pClock->zl3036x.zl303xx_Params;

            #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
            /* #warning MSCC: For ZLE30360_BOARD, we set the DPLL mode to REF LOCK instead of AUTO! */
                pClock->zl3036x.zl303xx_Params->pllParams.pllPriorMode = ZLS3036X_DPLL_MODE_REFLOCK;
                pClock->zl3036x.zl303xx_Params->pllParams.selectedRef = 5;
            #else
               /* Align with your chosen PLL mode! */
               pClock->zl3036x.zl303xx_Params->pllParams.pllPriorMode = ZLS3036X_DPLL_MODE_AUTO;

               /* To set forced ref mode, use the following code */
               /*
               pClock->zl3036x.zl303xx_Params->pllParams.pllPriorMode = ZLS3036X_DPLL_MODE_REFLOCK;
               pClock->zl3036x.zl303xx_Params->pllParams.selectedRef = 5;
               */
            #endif

            if (status != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("exampleAppStartClock: Call to example36xClockCreate(%d) failed=%d",
                indx, status, 0,0,0,0);
            }
            else if (indx > ZLS3036X_DPLL_MAX)
            {
                ZL303XX_TRACE_ALWAYS("exampleAppStartClock: Count=%d exceeds number of DPLLs in device=0x%x",
                indx, ZL30362_DEV_TYPE, 0,0,0,0);
            }
        }
    }
#endif

#if defined ZLS30721_INCLUDED
    if (status == ZL303XX_OK)
    {
        if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
        {
            pClock->zl3072x.pllId = indx;

            if (pClock->zl3072x.deviceMode == ZL303XX_MODE_UNKNOWN)
            {
               pClock->zl3072x.deviceMode = ZL303XX_MODE_REF_EXTERNAL;
            }

            {
                status = example72xClockCreate(&pClock->zl3072x);
                if (status == ZL303XX_OK)
                {
                    cguId = pClock->zl3072x.zl303xx_Params;

                   /* Align with your chosen PLL mode! */
                    pClock->zl3072x.zl303xx_Params->pllParams.d72x.priorSubMode = ZL303XX_PSM_AUTOMATIC;

                }

                if (status != ZL303XX_OK)
                {
                    ZL303XX_TRACE_ALWAYS("exampleAppStartClock: Call to example72xClockCreate(%d) failed=%d",
                    indx, status, 0,0,0,0);
                }
                else if (indx > ZLS3072X_DPLL_MAX)
                {
                    ZL303XX_TRACE_ALWAYS("exampleAppStartClock: Count=%d exceeds number of DPLLs in device=0x%x",
                    indx, ZL30362_DEV_TYPE, 0,0,0,0);
                }
            }
        }
    }
#endif

#if defined ZLS30701_INCLUDED
    if (status == ZL303XX_OK)
    {
        if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
        {
            pClock->zl3070x.pllId = indx;

            if (pClock->zl3070x.deviceMode == ZL303XX_MODE_UNKNOWN)
            {
               pClock->zl3070x.deviceMode = pClock->pktRef ? ZL303XX_MODE_REF_TOP
                                           : ZL303XX_MODE_REF_EXTERNAL;
            }

            {
                status = example70xClockCreate(&pClock->zl3070x);
                if (status == ZL303XX_OK)
                {
                    cguId = pClock->zl3070x.zl303xx_Params;

                   /* Align with your chosen PLL mode! */
                    pClock->zl3070x.zl303xx_Params->pllParams.d70x.pllPriorMode = ZLS3070X_DPLL_MODE_AUTO_LOCK;

                    /* To set forced ref mode, use the following code. */
                    /*
                    pClock->zl3070x.zl303xx_Params->pllParams.d70x.pllPriorMode = ZLS3070X_DPLL_MODE_REFLOCK;
                    pClock->zl3070x.zl303xx_Params->pllParams.selectedRef = 5;
                    */

                #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
                /* #warning MSCC: For ZLE30360_BOARD, we set the DPLL mode to REF LOCK instead of AUTO! */
                    pClock->zl3070x.zl303xx_Params->pllParams.d70x.pllPriorMode = ZLS3070X_DPLL_MODE_REFLOCK;
                    pClock->zl3070x.zl303xx_Params->pllParams.selectedRef = 5;
                #endif
                }
            }

            if (status != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("exampleAppStartClock: Call to example70xClockCreate(%d) failed=%d",
                indx, status, 0,0,0,0);
            }
            else if (indx > ZLS3070X_DPLL_MAX)
            {
                ZL303XX_TRACE_ALWAYS("exampleAppStartClock: Count=%d exceeds number of DPLLs in device=0x%x",
                indx, ZL30362_DEV_TYPE, 0,0,0,0);
            }
        }
    }
#endif

#if defined ZLS30751_INCLUDED
    if (status == ZL303XX_OK)
    {
        if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
        {
            pClock->zl3075x.pllId = indx;

            if (pClock->zl3075x.deviceMode == ZL303XX_MODE_UNKNOWN)
            {
               pClock->zl3075x.deviceMode = pClock->pktRef ? ZL303XX_MODE_REF_TOP
                                           : ZL303XX_MODE_REF_EXTERNAL;
            }

            {
                status = example75xClockCreate(&pClock->zl3075x);
                if (status == ZL303XX_OK)
                {
                    cguId = pClock->zl3075x.zl303xx_Params;

                   /* Align with your chosen PLL mode! */
                    pClock->zl3075x.zl303xx_Params->pllParams.d75x.pllPriorMode = ZLS3075X_DPLL_MODE_AUTO_LOCK;

                #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
                /* #warning MSCC: For ZLE30360_BOARD, we set the DPLL mode to REF LOCK instead of AUTO! */
                    pClock->zl3075x.zl303xx_Params->pllParams.d75x.pllPriorMode = ZLS3075X_DPLL_MODE_REFLOCK;
                    pClock->zl3075x.zl303xx_Params->pllParams.selectedRef = 5;
                #endif
                }
            }

            if (status != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("exampleAppStartClock: Call to example75xClockCreate(%d) failed=%d",
                indx, status, 0,0,0,0);
            }
            else if (indx > ZLS3075X_DPLL_MAX)
            {
                ZL303XX_TRACE_ALWAYS("exampleAppStartClock: Count=%d exceeds number of DPLLs in device=0x%x",
                indx, ZL30362_DEV_TYPE, 0,0,0,0);
            }
        }
    }
#endif

#if defined ZLS30771_INCLUDED
    if (status == ZL303XX_OK)
    {
        if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
        {
            pClock->zl3077x.pllId = indx;

            if (pClock->zl3077x.deviceMode == ZL303XX_MODE_UNKNOWN)
            {
               pClock->zl3077x.deviceMode = pClock->pktRef ? ZL303XX_MODE_REF_TOP
                                           : ZL303XX_MODE_REF_EXTERNAL;
            }

            {
                status = example77xClockCreate(&pClock->zl3077x);
                if (status == ZL303XX_OK)
                {
                    cguId = pClock->zl3077x.zl303xx_Params;

                   /* Align with your chosen PLL mode! */
                 #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD
                    /* #warning MSCC: For ZLE30360_BOARD, we set the DPLL mode to REF LOCK instead of AUTO! */
                    pClock->zl3077x.zl303xx_Params->pllParams.d77x.pllPriorMode = ZLS3077X_DPLL_MODE_REFLOCK;
#if defined ZLE30360_770_FRANKEN_BOARD
                    pClock->zl3077x.zl303xx_Params->pllParams.selectedRef = 9;
#else
                    pClock->zl3077x.zl303xx_Params->pllParams.selectedRef = 5;
#endif
                #else
                   pClock->zl3077x.zl303xx_Params->pllParams.d77x.pllPriorMode = ZLS3077X_DPLL_MODE_AUTO_LOCK;

                    /* To set forced ref mode, use the following code. */
                    /*
                    pClock->zl3077x.zl303xx_Params->pllParams.d77x.pllPriorMode = ZLS3077X_DPLL_MODE_REFLOCK;
                    pClock->zl3077x.zl303xx_Params->pllParams.selectedRef = 5;
                    */

                #endif
                }
            }

            if (status != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("exampleAppStartClock: Call to example77xClockCreate(%d) failed=%d",
                indx, status, 0,0,0,0);
            }
            else if (indx > ZLS3077X_DPLL_MAX)
            {
                ZL303XX_TRACE_ALWAYS("exampleAppStartClock: Count=%d exceeds number of DPLLs in device=0x%x",
                indx, ZL30362_DEV_TYPE, 0,0,0,0);
            }
        }
    }
#endif

#if defined ZLS30390_INCLUDED
    /* Hook PTP to the DEVICE */
    if (status == ZL303XX_OK && indx == TARGET_DPLL)  /* Only ONCE */
    {
        pClock->ptp.config.extData = cguId;
        pClock->ptp.handle = indx;  /* Assumption of PTP handle matching clock made here! */
        status = examplePtpClockCreate(&pClock->ptp);

        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("exampleAppStartClock: Call to examplePtpClockCreate(%d) failed=%d", indx, status, 0,0,0,0);
        }
    }
#endif

#if defined APR_INCLUDED 
    if (status == ZL303XX_OK)
    {
        pClock->apr.cguId = cguId;
        pClock->apr.device.pllId = indx;
    }

    /* Hook APR to the DEVICE */
    if (status == ZL303XX_OK && indx == TARGET_DPLL)  /* Only ONCE */
    {
        status = exampleAprClockCreate(&pClock->apr);

        if (status != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("exampleAppStartClock: Call to exampleAprClockCreate(%d) failed=%d",
                                 indx, status, 0,0,0,0);
        }
    }

    {
    #if defined _ZL303XX_ZLE30360_BOARD
        /* Assign global zl303xx_Params0 to the cguId at index = 1 or 0 */
        if (indx == TARGET_DPLL)
            zl303xx_Params0 = pClock->apr.cguId;
    #elif defined _ZL303XX_ZLE1588_BOARD
        if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
        {
            ;   /* Already assigned */
        }
        else
        {
            /* Assign global zl303xx_Params0 to the cguId at index = 1 or 0 */
            if (indx == TARGET_DPLL)
            {
                zl303xx_Params0 = pClock->apr.cguId;
            }
        }
    #else
        /* Assign global zl303xx_Params0 to the cguId */
        zl303xx_Params0 = pClock->apr.cguId;
    #endif
    }
#endif

#if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD) && defined OS_LINUX
   /* Start VTSS TSU after clocks are available */
   if (status == ZL303XX_OK && indx == TARGET_DPLL) /* Called once for the NCO device */
   {
       if (status == ZL303XX_OK && (status = zl303xx_VTSS_set_clock_freq(&zl303xx_VTSS_config)) != ZL303XX_OK)
           ZL303XX_TRACE_ALWAYS("exampleAppStartClock: zl303xx_VTSS_set_clock_freq() failed (status %d).", status, 0,0,0,0,0);

       if (status == ZL303XX_OK && (status = zl303xx_VTSS_configure(&zl303xx_VTSS_config)) != ZL303XX_OK)
           ZL303XX_TRACE_ALWAYS("exampleAppStartClock: zl303xx_VTSS_configure() failed (status %d).", status, 0,0,0,0,0);

       if (status == ZL303XX_OK)
       {
            /* Align TSU PPS to the ZL3034x 1Hz Pulse */
            ZL303XX_TRACE_ALWAYS("Aligning CGU and TSU 1 PPS signals (Timeout 30 seconds)",
                                0,0,0,0,0,0);

            /* Sync to external pulse, 30 second timeout */
            if ((status = zl303xx_VTSS_sync_to_ext_pulse(30000)) != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS("exampleAppStartClock: Aligning CGU and TSU 1 PPS signals, ERROR status= %d",
                                    status, 0,0,0,0,0);
            }
       }
    }

    #if defined ZLS30361_INCLUDED
    if (status == ZL303XX_OK && indx == TARGET_DPLL) /* Called once for the NCO device */
    {
        if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
        {
            example36xCheckPhyTSClockFreq(pClock->zl3036x.zl303xx_Params);
        }
    }
    #endif
    #if defined ZLS30701_INCLUDED
    if (status == ZL303XX_OK && indx == TARGET_DPLL) /* Called once for the NCO device */
    {
        if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
        {
            example70xCheckPhyTSClockFreq(pClock->zl3070x.zl303xx_Params);
        }
    }
    #endif
    #if defined ZLS30751_INCLUDED
    if (status == ZL303XX_OK && indx == TARGET_DPLL) /* Called once for the NCO device */
    {
        if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
        {
            example75xCheckPhyTSClockFreq(pClock->zl3075x.zl303xx_Params);
        }
    }
    #endif
    #if defined ZLS30771_INCLUDED
    if (status == ZL303XX_OK && indx == TARGET_DPLL) /* Called once for the NCO device */
    {
        if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
        {
            example77xCheckPhyTSClockFreq(pClock->zl3077x.zl303xx_Params);
        }
    }
    #endif
#endif
    if (status == ZL303XX_OK)
    {
        pClock->started = ZL303XX_TRUE;
    }
    return status;
}


zlStatusE exampleAppStartPort(exampleAppS *pApp, Uint32T indx)
{
    zlStatusE status = ZL303XX_OK;
    exampleAppPortS *port = &pApp->port[indx];

#if defined ZLS30390_INCLUDED
    /* Initialize the structure with the global example port address,
     * if it has been set */
    if (examplePtpLocalAddr[indx].networkProtocol == ZL303XX_NETWORK_PROTOCOL_INVALID)
    {
    #if (defined _ZL303XX_ZLE1588_BOARD || defined _ZL303XX_ZLE30360_BOARD )
      #ifndef ZL303XX_PKT_IPV6_STR_LENGTH
      #define ZL303XX_PKT_IPV6_STR_LENGTH 46
      #endif

        if (zl303xx_GetSrcIpAddrStr())
        {
            memset(examplePtpLocalIpv4, 0x00, ZL303XX_PKT_IPV6_STR_LENGTH);
            strncpy(examplePtpLocalIpv4, zl303xx_GetSrcIpAddrStr(), ZL303XX_PKT_IPV6_STR_LENGTH-1);
        }
        else
            ZL303XX_TRACE_ALWAYS("exampleAppStartPort: SRC NULL address", 0,0,0,0,0,0);

        if (zl303xx_GetDestIpAddrStr())
        {
            memset(examplePtpDestIpv4, 0x00, ZL303XX_PKT_IPV6_STR_LENGTH);
            strncpy(examplePtpDestIpv4, zl303xx_GetDestIpAddrStr(), ZL303XX_PKT_IPV6_STR_LENGTH-1);
        }
        else
            ZL303XX_TRACE_ALWAYS("exampleAppStartPort: DEST NULL address", 0,0,0,0,0,0);
    #endif
    }

    if (port->exampleInterface != ZL303XX_EXAMPLE_INTERFACE_IGNORE)
    {
       /* Use App's provided interface */
       zl303xx_SetExampleInterface(port->exampleInterface);

       if (port->exampleInterface == ZL303XX_EXAMPLE_INTERFACE_CUSTOM_STR)
       {
          /* Use App's provided custom interface string */
          zl303xx_SetExampleInterfaceCustomStr(port->exampleInterfaceCustomStr);
       }
    }

    if ((status = examplePtpPortCreate(&port->ptp)) != ZL303XX_OK)
    {
        ZL303XX_TRACE_ALWAYS("exampleAppStartPort: Call to examplePtpPortCreate(%d) failed=%d",
                            indx, status, 0,0,0,0);
    }
    else
    {
        /* Force all PTP ports to TARGET_DPLL and earlier PTP handle assumption continuation! */
        port->ptp.config.clockHandle = pApp->clock[TARGET_DPLL].ptp.handle;
    }

#endif

    if (status == ZL303XX_OK)
    {
        port->started = ZL303XX_TRUE;
    }

    if (status != ZL303XX_OK)
        ZL303XX_TRACE_ALWAYS("exampleAppStartPort() status = %lu", (Uint32T)status, 0,0,0,0,0);

    return status;
}

zlStatusE exampleAppStartStream(exampleAppS *pApp, Uint32T indx)
{
    zlStatusE status = ZL303XX_OK;
    exampleAppStreamS *stream = &pApp->stream[indx];

#if defined ZLS30390_INCLUDED
    {
        Uint32T i = 0;

        /* Set the example interface global to the right string,
           Any examples that use only one interface are not effected
        */
        for (i=0; i<pApp->portCount; i++)
        {
           if(stream->ptp.config.portHandle == i)
           {
              exampleAppPortS *port = &pApp->port[i];
              if (port->exampleInterface != ZL303XX_EXAMPLE_INTERFACE_IGNORE)
              {
                 /* Use App's provided interface */
                 zl303xx_SetExampleInterface(port->exampleInterface);

                 if (port->exampleInterface == ZL303XX_EXAMPLE_INTERFACE_CUSTOM_STR)
                 {
                    /* Use App's provided custom interface string */
                    zl303xx_SetExampleInterfaceCustomStr(port->exampleInterfaceCustomStr);
                 }
              }
           }
        }
    }
#endif

#if defined APR_INCLUDED

  #if defined ZLS30390_INCLUDED
  #endif
    if (status == ZL303XX_OK)
    {
    #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD 
        /* #warning MSCC: Use serverId = StreamId = pApp->srvId */
        /* printf(" exampleAppStartStream: provided pApp->srvId=%d\n", pApp->srvId);*/
        stream->apr.server.serverId = pApp->srvId;  /* Allows for non-sequential serverIds (don't use indx) */
    #else
        stream->apr.server.serverId = indx; /* This assumes stream numbers start at 0! */
    #endif

    /* #warning MSCC: Forced streams to clock = TARGET_DPLL! */

    #if defined ZLS3038X_INCLUDED
        stream->apr.cguId = pApp->clock[TARGET_DPLL].apr.cguId;                 /* Associate with TARGET_DPLL! */
    #endif

    #if defined ZLS30361_INCLUDED
        if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
        {
            stream->apr.cguId = pApp->clock[TARGET_DPLL].zl3036x.zl303xx_Params;  /* Associate with CLOCK TARGET_DPLL! */
        }
    #endif
    #if defined ZLS30701_INCLUDED
        if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
        {
            stream->apr.cguId = pApp->clock[TARGET_DPLL].zl3070x.zl303xx_Params;  /* Associate with CLOCK TARGET_DPLL! */
        }
    #endif
    #if defined ZLS30751_INCLUDED
        if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
        {
            stream->apr.cguId = pApp->clock[TARGET_DPLL].zl3075x.zl303xx_Params;  /* Associate with CLOCK TARGET_DPLL! */
        }
    #endif
    #if defined ZLS30721_INCLUDED
        if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
        {
            stream->apr.cguId = pApp->clock[TARGET_DPLL].zl3072x.zl303xx_Params;  /* Associate with CLOCK TARGET_DPLL! */
        }
    #endif
    #if defined ZLS30341_INCLUDED
        if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE )
          )
        {
            stream->apr.cguId = pApp->clock[TARGET_DPLL].zl3034x.zl303xx_Params;  /* Associate with CLOCK TARGET_DPLL! */
        }
    #endif
    #if defined ZLS30771_INCLUDED
        if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
        {
            stream->apr.cguId = pApp->clock[TARGET_DPLL].zl3077x.zl303xx_Params;  /* Associate with CLOCK TARGET_DPLL! */
        }
    #endif

    #if defined ZLS30390_INCLUDED
        /* Link the APR stream's CGU ID to the PTP stream. */
        examplePtpStreamCguIdSet(&stream->ptp, stream->apr.cguId);
    #endif
    }
#endif

#if defined ZLS30390_INCLUDED
    if (status == ZL303XX_OK)
    {
    #if defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD || defined _ZL303XX_FMC_BOARD
        stream->ptp.config.requestedHandle = pApp->srvId;
    #else
        stream->ptp.config.requestedHandle = indx;
    #endif
        if ((status = examplePtpStreamCreate(&stream->ptp)) != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("exampleAppStartStream: Call to examplePtpStreamCreate(%d) failed=%d",
            indx, status, 0,0,0,0);
        }
        else
        {
        /* #warning MSCC: Forced streams to port 0! */
            /* Force all PTP streams with port0! */
            stream->ptp.config.portHandle = pApp->port[0].ptp.handle;
        }
    }

#endif

#if defined APR_INCLUDED && !defined ZLS30390_INCLUDED
    if (status == ZL303XX_OK)
    {
        /* For MASTER_ONLY, no APR stream needed. For slaveCapable streams,
         * the APR server is added in examplePtpEventStreamCreate() */
        if ((status = exampleAprStreamCreate(&stream->apr)) != ZL303XX_OK)
        {
            ZL303XX_TRACE_ALWAYS("exampleAppStartStream: Call to exampleAprStreamCreate(%d) cguId=0x%x failed=%d",
                                indx, stream->apr.cguId, status, 0,0,0);
        }
    }
#endif

#if defined APR_INCLUDED
  #if !defined ZLS30390_INCLUDED && defined START_FIRST_PKT_STREAM
    if (status == ZL303XX_OK)
    {
    /* #warning START FIRST STREAM - This logic has now been moved outside of APR when not in legacy mode! */
        if (!zl303xx_AprGetUseLegacyStreamStartUp())
        {
            /* In "Legacy" - APR will automatically start the first pkt stream with similar code to below;
             With ZLS30390 active, BMCA will evaluate and select the best one thus overridding the first pkt server */

            Uint16T activeServerId;
            zl303xx_AprDeviceRefModeE deviceRefMode;

            if ((status = zl303xx_AprGetDeviceRefMode (stream->apr.cguId, &deviceRefMode)) != ZL303XX_OK)
           {
               ZL303XX_TRACE_ALWAYS("exampleAppStart: zl303xx_AprGetDeviceRefMode() failed with status=%d", status, 0,0,0,0,0);
           }

           (void)zl303xx_AprGetDeviceCurrActiveRef(stream->apr.cguId, &activeServerId);
           if (activeServerId == 0xFFFF)     /* No active server yet - maybe this will be the one */
           {
               if (deviceRefMode != ZL303XX_ELECTRIC_MODE) /* Make first pkt stream active */
               {
                   if ((status = zl303xx_AprSetActiveRef (stream->apr.cguId, stream->apr.server.serverId)) != ZL303XX_OK)
                   {
                       ZL303XX_TRACE_ALWAYS("exampleAppStart: zl303xx_AprSetActiveRef() failed with status=%d", status, 0,0,0,0,0);
                   }
                   else
                   {
                       ZL303XX_TRACE_ALWAYS("exampleAppStart: Made pkt serverId=%d active", stream->apr.server.serverId, 0,0,0,0,0);
                   }
               }
           }
        }
    }
  #endif
#endif

    if (status == ZL303XX_OK)
    {
        stream->started = ZL303XX_TRUE;
    }

    if (status != ZL303XX_OK)
        ZL303XX_TRACE_ALWAYS("exampleAppStartStream() status = %lu", (Uint32T)status, 0,0,0,0,0);

    return status;
}


/* exampleAppStop */
/**
   Example code to stop an "application" with the provided configuration.

*******************************************************************************/
zlStatusE exampleAppStop(exampleAppS *pApp)
{
   zlStatusE status = ZL303XX_OK;
   zlStatusE retStatus = ZL303XX_OK;
   Uint32T count;

   ZL303XX_CHECK_POINTER(pApp);

   if (exampleAppIsRunning() != ZL303XX_TRUE)
   {
      ZL303XX_TRACE_ALWAYS("An appl has not been started, start one first", 0,0,0,0,0,0);
      status = ZL303XX_UNSUPPORTED_OPERATION;
      retStatus |= status;
   }

#if defined ZLS30390_INCLUDED
    /* Stop PTP Stack */
   if (retStatus == ZL303XX_OK)
   {
       if ((status = examplePtpStop()) != ZL303XX_OK)
           printf("exampleAppStop: examplePtpStop() failed=%d\n", status);
       retStatus |= status;
   }
#endif


#if (defined _ZL303XX_ZLE30360_BOARD || defined _ZL303XX_ZLE1588_BOARD) && defined OS_LINUX
   /* Shutdown Vitesse TSU API */
   if (status == ZL303XX_OK)
   {
       if ((retStatus = zl303xx_VTSS_shutdown()) != ZL303XX_OK)
           printf("exampleAppStop: zl303xx_VTSS_shutdown() failed=%d\n", status);
       retStatus |= status;
   }
#endif

#if defined ZLS30341_INCLUDED
   if (retStatus == ZL303XX_OK)
   {
      if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE )
        )
      {
   #if defined POLL_FOR_ZL303XX_INTR 
         if ((status = zl303xx_Dpll34xStopIntrTimer()) != ZL303XX_OK)
            printf("exampleAppStop: zl303xx_Dpll34xStopIntrTimer() failed=%d\n", status);
         retStatus |= status;
   #endif
      }
   }
#endif
#if defined ZLS30721_INCLUDED
   if (retStatus == ZL303XX_OK)
   {
      if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
      {
         status = zl303xx_Dpll72xTaskStop();
         if (status != ZL303XX_OK)
         {
            ZL303XX_TRACE_ALWAYS("exampleAppStop: Failed stopping 72x task=%d", status, 0,0,0,0,0);
         }
      }
   }
#endif

   if (retStatus == ZL303XX_OK)   /* Streams */
   {
      for (count = 0; count < pApp->streamCount; count++)
      {
         if (pApp->stream[count].started == ZL303XX_FALSE)
         {
         #if defined ZLS30390_INCLUDED
             if (pApp->stream[count].ptp.config.extData)
             {
                 OS_FREE(zlExampleApp.stream[count].ptp.config.extData);
                 zlExampleApp.stream[count].ptp.config.extData = NULL;
             }
         #endif
            continue;
         }
         else
         {
             pApp->stream[count].started = ZL303XX_FALSE;
         }

      #if defined ZLS30390_INCLUDED
         if (pApp->stream[count].ptp.config.mode != ZL303XX_PTP_STREAM_MASTER_ONLY)
      #endif
         {
         #if defined APR_INCLUDED
           #if !defined MUX_PTP_STREAMS_TO_APR_SLOTS
             if (pApp->stream[count].apr.cguId)
             {
                 status = exampleAprStreamRemove(&pApp->stream[count].apr);
                 if (status != ZL303XX_OK)
                 {
                     retStatus |= status;
                     ZL303XX_TRACE_ALWAYS("exampleAppStop: Failed removing APR stream=%d", count, 0,0,0,0,0);
                 }
             }
           #endif
         #endif
         #if defined ZLS30390_INCLUDED
           #if !defined MUX_PTP_STREAMS_TO_APR_SLOTS
            /* All memory should have been freed during examplePtpStop() */
             if (status == ZL303XX_OK && (status = examplePtpStreamDelete(count)) != ZL303XX_OK)
             {
                 /* Empty slot but OK */
                 if (status == ZL303XX_PARAMETER_INVALID || status == ZL303XX_TABLE_ENTRY_NOT_FOUND)
                 {
                     status = ZL303XX_OK;
                 }
                 else
                 {
                     ZL303XX_TRACE_ALWAYS("exampleAppStop: Failed removing PTP stream=%d", count, 0,0,0,0,0);
                 }
             }

             if (status == ZL303XX_OK)
             {
                 /* All memory should have been freed during examplePtpStop() */
                 if (pApp->stream[count].ptp.config.extData)
                 {
                     examplePtpStreamExtDataS *pExtData = pApp->stream[count].ptp.config.extData;

                     if (pExtData->skt)
                         status = examplePtpSocketClose(&pExtData->skt);

                     OS_FREE(pApp->stream[count].ptp.config.extData);
                     pApp->stream[count].ptp.config.extData = NULL;
                 }
             }
           #endif
         #endif

         }
      } /* for (streams) */
   }

#if defined ZLS30390_INCLUDED
   if (retStatus == ZL303XX_OK)      /* Ports */
   {
      for (count = 0; count < pApp->portCount; count++)
      {
         if (pApp->port[count].started != ZL303XX_TRUE)
            continue;
         else
            pApp->port[count].started = ZL303XX_FALSE;

         status = examplePtpPortDelete(pApp->port[count].ptp.handle);
         if (status != ZL303XX_OK)
         {
             status = ZL303XX_OK;
             ZL303XX_TRACE_ALWAYS("exampleAppStop: Already removed or failed removing PTP Port=%d", count, 0,0,0,0,0);
         }
      } /* for (ports) */
   }
#endif


   if (retStatus == ZL303XX_OK)   /* Clocks */
   {
       for (count = 0; count < pApp->clockCount; count++)
       {
       #if defined ZLS30390_INCLUDED
           if (count == TARGET_DPLL)    /* Only one PTP */
           status = examplePtpClockDelete(pApp->clock[count].ptp.handle);
           if (status != ZL303XX_OK)
           {
               retStatus |= status;
               ZL303XX_TRACE_ALWAYS("exampleAppStop: Failed removing PTP Clock=%d", count, 0,0,0,0,0);
               status = ZL303XX_OK;
           }
       #endif

       #if defined APR_INCLUDED
          if (count == TARGET_DPLL)    /* Only one APR */
          status = exampleAprClockRemove(&pApp->clock[count].apr);
          if (status != ZL303XX_OK)
          {
              retStatus |= status;
              ZL303XX_TRACE_ALWAYS("exampleAppStop: Failed removing APR Clock=%d", count, 0,0,0,0,0);
              status = ZL303XX_OK;
          }
       #endif

       #if defined ZLS30341_INCLUDED
          if(( zl303xx_GetDefaultDeviceType() == ZL3034X_DEVICETYPE )
            )
          {
             status = example34xClockRemove(&pApp->clock[count].zl3034x);
             if (status != ZL303XX_OK)
             {
                 retStatus |= status;
                 ZL303XX_TRACE_ALWAYS("exampleAppStop: Failed removing 34x Clock=%d", count, 0,0,0,0,0);
                 status = ZL303XX_OK;
             }
          }
       #endif

       #if defined ZLS30361_INCLUDED
          if( zl303xx_GetDefaultDeviceType() == ZL3036X_DEVICETYPE )
          {
             status = example36xClockRemove(&pApp->clock[count].zl3036x);
             if (status != ZL303XX_OK)
             {
                 retStatus |= status;
                 ZL303XX_TRACE_ALWAYS("exampleAppStop: Failed removing 36x Clock=%d", count, 0,0,0,0,0);
                 status = ZL303XX_OK;
             }
          }
       #endif

       #if defined ZLS30721_INCLUDED
          if( zl303xx_GetDefaultDeviceType() == ZL3072X_DEVICETYPE )
          {
             status = example72xClockRemove(&pApp->clock[count].zl3072x);
             if (status != ZL303XX_OK)
             {
                 retStatus |= status;
                 ZL303XX_TRACE_ALWAYS("exampleAppStop: Failed removing 72x Clock=%d", count, 0,0,0,0,0);
                 status = ZL303XX_OK;
             }
          }
       #endif

       #if defined ZLS30701_INCLUDED
          if( zl303xx_GetDefaultDeviceType() == ZL3070X_DEVICETYPE )
          {
             status = example70xClockRemove(&pApp->clock[count].zl3070x);
             if (status != ZL303XX_OK)
             {
                retStatus |= status;
                ZL303XX_TRACE_ALWAYS("exampleAppStop: Failed removing 70x Clock=%d", count, 0,0,0,0,0);
                status = ZL303XX_OK;
             }
          }
       #endif

       #if defined ZLS30751_INCLUDED
          if( zl303xx_GetDefaultDeviceType() == ZL3075X_DEVICETYPE )
          {
             status = example75xClockRemove(&pApp->clock[count].zl3075x);
             if (status != ZL303XX_OK)
             {
                retStatus |= status;
                ZL303XX_TRACE_ALWAYS("exampleAppStop: Failed removing 75x Clock=%d", count, 0,0,0,0,0);
                status = ZL303XX_OK;
             }
          }
       #endif

       #if defined ZLS30771_INCLUDED
          if( zl303xx_GetDefaultDeviceType() == ZL3077X_DEVICETYPE )
          {
             status = example77xClockRemove(&pApp->clock[count].zl3077x);
             if (status != ZL303XX_OK)
             {
                retStatus |= status;
                ZL303XX_TRACE_ALWAYS("exampleAppStop: Failed removing 77x Clock=%d", count, 0,0,0,0,0);
                status = ZL303XX_OK;
             }
          }
       #endif

          pApp->clock[count].started = ZL303XX_FALSE;

       } /* for (clocks) */
   }

   if (status == ZL303XX_OK)
   {
      retStatus |= exampleAppStructFree(pApp);  /* Api */
   }

   if (retStatus == ZL303XX_OK && exampleAppRunning == ZL303XX_TRUE)
   {
       exampleAppRunning = ZL303XX_FALSE;  /* Flag Appl. stopped (used by examplePtpEnvClose() */
   }

   if (retStatus == ZL303XX_OK)
   {
      ZL303XX_TRACE_ALWAYS("exampleAppStop: Application stopped\n", 0,0,0,0,0,0);
   }
   else
   {
      ZL303XX_TRACE_ALWAYS("exampleAppStop: Something failed during removal, retStatus=%d\n", retStatus, 0,0,0,0,0);
   }

   return retStatus;
}

/* exampleAppIsRunning */
/**
   Example code to return a boolean as to whether the appl is currently running.

*******************************************************************************/
zl303xx_BooleanE exampleAppIsRunning(void)
{
   return exampleAppRunning;
}

/* exampleReset1588HostRegisterOnClockCreateSet */
/**
   Set a flag to perform clearing of 1588 host registers on clock creation for Microsemi chipsets.

*******************************************************************************/
zlStatusE exampleReset1588HostRegisterOnClockCreateSet(zl303xx_BooleanE set)
{
    reset1588HostRegisterOnClockCreate = set;
    return ZL303XX_OK;
}

/* exampleReset1588HostRegisterOnClockCreateGet */
/**
   Get a flag to perform clearing of 1588 host registers on clock creation for Microsemi chipsets.

*******************************************************************************/
zl303xx_BooleanE exampleReset1588HostRegisterOnClockCreateGet(void)
{
    return reset1588HostRegisterOnClockCreate;
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/*****************   END   ****************************************************/
