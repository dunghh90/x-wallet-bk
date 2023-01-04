

/*******************************************************************************

   $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa

   Copyright 2006-2018 Microsemi Semiconductor Limited.
   All rights reserved.
   The timing algorithms implemented in the software code are Patent Pending.

*******************************************************************************/

#include "zl303xx_Global.h"
#include "zl303xx_Trace.h"
#include "zl303xx_Apr.h"
#include "zl303xx_AprStateMachine.h"
#include "zl303xx_HoldoverUtils.h"

/*****************   GLOBAL VARIABLES   ********************************/
/*
zl303xx_AprPktServerSMLookupTable:
[gstV,   gstL, gstH, gstPA]            -   Server State
[0,      0,    0,    0]                -   ZL303XX_REF_FAILED
[0,      0,    0,    1]                -   ZL303XX_REF_FAILED
[0,      0,    1,    0]                -   ZL303XX_HOLDOVER
[0,      0,    1,    1]                -   ZL303XX_HOLDOVER
[0,      1,    0,    0]                -   ZL303XX_HOLDOVER
[0,      1,    0,    1]                -   ZL303XX_HOLDOVER
[0,      1,    1,    0]                -   ZL303XX_UNKNOWN
[0,      1,    1,    1]                -   ZL303XX_UNKNOWN
[1,      0,    0,    0]                -   ZL303XX_FREQ_LOCK_ACQUIRING
[1,      0,    0,    1]                -   ZL303XX_FREQ_LOCK_ACQUIRING
[1,      0,    1,    0]                -   ZL303XX_HOLDOVER
[1,      0,    1,    1]                -   ZL303XX_HOLDOVER
[1,      1,    0,    0]                -   ZL303XX_FREQ_LOCK_ACQUIRED
[1,      1,    0,    1]                -   ZL303XX_PHASE_LOCK_ACQUIRED
[1,      1,    1,    0]                -   ZL303XX_UNKNOWN
[1,      1,    1,    1]                -   ZL303XX_UNKNOWN
*/
static const zl303xx_AprStateE zl303xx_AprPktServerSMLookupTable[16] =
                    {
                       ZL303XX_REF_FAILED,
                       ZL303XX_REF_FAILED,
                       ZL303XX_HOLDOVER,
                       ZL303XX_HOLDOVER,
                       ZL303XX_HOLDOVER,
                       ZL303XX_HOLDOVER,
                       ZL303XX_UNKNOWN,
                       ZL303XX_UNKNOWN,
                       ZL303XX_FREQ_LOCK_ACQUIRING,
                       ZL303XX_FREQ_LOCK_ACQUIRING,
                       ZL303XX_HOLDOVER,
                       ZL303XX_HOLDOVER,
                       ZL303XX_FREQ_LOCK_ACQUIRED,
                       ZL303XX_PHASE_LOCK_ACQUIRED,
                       ZL303XX_UNKNOWN,
                       ZL303XX_UNKNOWN
                    };

/* zl303xx_AprElecRefSMLookupTable
 * [RF,  L, H, SE]   -  Server State
 * [0,   0, 0, 0]    -  ZL303XX_FREQ_LOCK_ACQUIRING
 * [0,   0, 0, 1]    -  ZL303XX_FREQ_LOCK_ACQUIRING
 * [0,   0, 1, 0]    -  ZL303XX_HOLDOVER
 * [0,   0, 1, 1]    -  ZL303XX_HOLDOVER
 * [0,   1, 0, 0]    -  ZL303XX_FREQ_LOCK_ACQUIRED
 * [0,   1, 0, 1]    -  ZL303XX_PHASE_LOCK_ACQUIRED
 * [0,   1, 1, 0]    -  ZL303XX_UNKNOWN
 * [0,   1, 1, 1]    -  ZL303XX_UNKNOWN
 * [1,   0, 0, 0]    -  ZL303XX_REF_FAILED
 * [1,   0, 0, 1]    -  ZL303XX_REF_FAILED
 * [1,   0, 1, 0]    -  ZL303XX_HOLDOVER
 * [1,   0, 1, 1]    -  ZL303XX_HOLDOVER
 * [1,   1, 0, 0]    -  ZL303XX_REF_FAILED
 * [1,   1, 0, 1]    -  ZL303XX_REF_FAILED
 * [1,   1, 1, 0]    -  ZL303XX_UNKNOWN
 * [1,   1, 1, 1]    -  ZL303XX_UNKNOWN
 */

static const zl303xx_AprStateE zl303xx_AprElecRefSMLookupTable[16] =
{
      ZL303XX_FREQ_LOCK_ACQUIRING,
      ZL303XX_FREQ_LOCK_ACQUIRING,
      ZL303XX_HOLDOVER,
      ZL303XX_HOLDOVER,
      ZL303XX_FREQ_LOCK_ACQUIRED,
      ZL303XX_PHASE_LOCK_ACQUIRED,
      ZL303XX_UNKNOWN,
      ZL303XX_UNKNOWN,
      ZL303XX_REF_FAILED,
      ZL303XX_REF_FAILED,
      ZL303XX_HOLDOVER,
      ZL303XX_HOLDOVER,
      ZL303XX_REF_FAILED,
      ZL303XX_REF_FAILED,
      ZL303XX_UNKNOWN,
      ZL303XX_UNKNOWN
};

/* zl303xx_AprCGUHwManualOptLookupTable
 * [MF,  MH]   -  CGU State
 * [0,   0 ]   -  Depending on the other flags
 * [0,   1 ]   -  ZL303XX_MANUAL_HOLDOVER
 * [1,   0 ]   -  ZL303XX_MANUAL_FREERUN
 * [1,   1 ]   -  ZL303XX_UNKNOWN
 */
static const zl303xx_AprStateE zl303xx_AprCGUHwManualOptLookupTable[] =
{
      ZL303XX_UNKNOWN,
      ZL303XX_MANUAL_HOLDOVER,
      ZL303XX_MANUAL_FREERUN,
      ZL303XX_UNKNOWN
};

/* zl303xx_AprHybridSMLookupTable
 * [RF,  hwL,  hwH,  PA]   -  CGU State
 * [0,   0,    0,    0 ]   -  ZL303XX_FREQ_LOCK_ACQUIRING
 * [0,   0,    0,    1 ]   -  ZL303XX_FREQ_LOCK_ACQUIRING
 * [0,   0,    1,    0 ]   -  ZL303XX_HOLDOVER
 * [0,   0,    1,    1 ]   -  ZL303XX_HOLDOVER
 * [0,   1,    0,    0 ]   -  ZL303XX_FREQ_LOCK_ACQUIRED
 * [0,   1,    0,    1 ]   -  ZL303XX_PHASE_LOCK_ACQUIRED
 * [0,   1,    1,    0 ]   -  ZL303XX_UNKNOWN
 * [0,   1,    1,    1 ]   -  ZL303XX_UNKNOWN
 * [1,   0,    0,    0 ]   -  ZL303XX_REF_FAILED
 * [1,   0,    0,    1 ]   -  ZL303XX_REF_FAILED
 * [1,   0,    1,    0 ]   -  ZL303XX_HOLDOVER
 * [1,   0,    1,    1 ]   -  ZL303XX_HOLDOVER
 * [1,   1,    0,    0 ]   -  ZL303XX_REF_FAILED
 * [1,   1,    0,    1 ]   -  ZL303XX_REF_FAILED
 * [1,   1,    1,    0 ]   -  ZL303XX_UNKNOWN
 * [1,   1,    1,    1 ]   -  ZL303XX_UNKNOWN
 */
static const zl303xx_AprStateE zl303xx_AprHybridSMLookupTable[] =
{
      ZL303XX_FREQ_LOCK_ACQUIRING,
      ZL303XX_FREQ_LOCK_ACQUIRING,
      ZL303XX_HOLDOVER,
      ZL303XX_HOLDOVER,
      ZL303XX_FREQ_LOCK_ACQUIRED,
      ZL303XX_PHASE_LOCK_ACQUIRED,
      ZL303XX_UNKNOWN,
      ZL303XX_UNKNOWN,
      ZL303XX_REF_FAILED,
      ZL303XX_REF_FAILED,
      ZL303XX_HOLDOVER,
      ZL303XX_HOLDOVER,
      ZL303XX_REF_FAILED,
      ZL303XX_REF_FAILED,
      ZL303XX_UNKNOWN,
      ZL303XX_UNKNOWN

};

extern	zl303xx_BooleanE	ptpLockedFlag;
zl303xx_BooleanE	ptpLockedFlag = ZL303XX_FALSE;


/** zl303xx_AprProcess_L_Flag

   This function process the L1, L2 and L3 flags by applying the masks of L2 and L3
   to create the L flag for the specified server.
   The function is called every 9600ms.

  Parameters:
   [in]    hwParams                  Pointer to the device structure
   [in]    serverId                  The identifier of server

   [out]   L                         Pointer to the L flag

  Return Value: ZL303XX_OK                        if successful
****************************************************************************/
zlStatusE zl303xx_AprProcess_L_Flag
            (
            void *hwParams,
            Uint16T serverId,
            zl303xx_BooleanE *L
            )
{
   zl303xx_AprServerStatusFlagsS statusFlags;
   zl303xx_AprServerSMDataS *pSmData;
   zlStatusE status;
   zl303xx_BooleanE L2, L3, L4;

    status = ZL303XX_CHECK_POINTERS(hwParams, L);
    if (status != ZL303XX_OK)
    {
       return status;
    }

    status = zl303xx_AprGetServerStatusFlags(hwParams, serverId, &statusFlags);

    ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 1,
                "zl303xx_AprProcess_L_Flag: zl303xx_AprGetServerStatusFlags gave L1=%d, L2=%d, L3=%d, L4=%d",
                statusFlags.L1,statusFlags.L2,statusFlags.L3, statusFlags.L4, 0, 0);

    if (status == ZL303XX_OK)
    {
       status = zl303xx_AprGetServerSMData(hwParams, serverId, &pSmData);

       if (status == ZL303XX_OK)
       {
          L2 = L2_MASK(pSmData->lockFlagsMask) | statusFlags.L2;
          L3 = L3_MASK(pSmData->lockFlagsMask) | statusFlags.L3;
          L4 = L4_MASK(pSmData->lockFlagsMask) | statusFlags.L4;


          *L = statusFlags.L1 & L2 & L3 & L4;

          status = zl303xx_AprSetServerSMFlag
                      (
                      hwParams,
                      serverId,
                      ZL303XX_SERVER_CLK_L_FLAG,
                      L
                      );
          /*ZL303XX_TRACE_ALWAYS("+++++++L=%u", *L, 0,0,0,0,0);*/

          ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 1,
                      "zl303xx_AprProcess_L_Flag: called  zl303xx_AprSetServerSMFlag with L=0x%x",
                      *L, 0, 0, 0, 0, 0);
       }
    }

    return status;
}


/** zl303xx_AprApplyGSTon_H_Flag

   This function process H flag by applying the corresponding Guard Soak Time
   (GST) if the H flag changes from 0 to 1 (enter to Holdover) for the specified server.
   The function is called every 9600ms.

  Parameters:
   [in]    hwParams                  Pointer to the device structure
   [in]    serverId                  The identifier of server

   [out]   gstH                      pointer to the H flag after GST applied

  Return Value: ZL303XX_OK                        if successful
****************************************************************************/
zlStatusE zl303xx_AprApplyGSTon_H_Flag
            (
            void *hwParams,
            Uint16T serverId,
            zl303xx_BooleanE *gstH
            )
{
   zl303xx_AprServerStatusFlagsS statusFlags;
   zl303xx_AprServerSMDataS *pSmData;
   zlStatusE status;

    status = ZL303XX_CHECK_POINTERS(hwParams, gstH);
    if (status != ZL303XX_OK)
    {
       return status;
    }

    status = zl303xx_AprGetServerStatusFlags(hwParams, serverId, &statusFlags);

    if (status == ZL303XX_OK)
    {
       *gstH = statusFlags.gstH;

       status = zl303xx_AprGetServerSMData(hwParams, serverId, &pSmData);

       if (status == ZL303XX_OK)
       {
          if (statusFlags.gstH == ZL303XX_FALSE)
          {
             /* Check if the algorithm H flag is set */
             if (statusFlags.H == ZL303XX_TRUE)
             {
                /* Check the guard soak timer, unit of enterHoldoverGST is 10 sec */
                if (pSmData->enterHoldoverGSTCnt >= pSmData->enterHoldoverGST)
                {
                   *gstH = ZL303XX_TRUE;
                   pSmData->enterHoldoverGSTCnt = 0;
                }
                else
                {
                   pSmData->enterHoldoverGSTCnt++;
                   /* the resolution of enterHoldoverGSTCnt is 9600ms. if the counter=n*24 (n=1,2,3...),
                    * subtract the rounding */
                   if ((pSmData->enterHoldoverGSTCnt%24) == 0)
                   {
                      pSmData->enterHoldoverGSTCnt--;
                   }
                }
             }
             else /* algorithm H=0 */
             {
                pSmData->enterHoldoverGSTCnt = 0;
             }
          }
          else /* APR is in Holdover (gstH=1), whenever the algorithm H=0, exit the holdover (gstH=0). */
          {
             if (statusFlags.H == ZL303XX_FALSE)
             {
                 *gstH = ZL303XX_FALSE;

             }

          }

          status = zl303xx_AprSetServerSMFlag
                      (
                      hwParams,
                      serverId,
                      ZL303XX_SERVER_CLK_GST_H_FLAG,
                      gstH
                      );
          /*ZL303XX_TRACE_ALWAYS("+++++++H=%u, gstH=%u", statusFlags.H, *gstH,0,0,0,0);*/

       }
    }

    return status;

}

/** zl303xx_AprApplyGSTon_VLPA_Flags

   This function process V/L/PA flag by applying the corresponding Guard Soak Timer
   (GST) on it if the flag changes from 1 to 0 (exit from Valid/Lock/PhaseAligned)
   for the specified server.
   The function is called every 9600ms.

  Parameters:
   [in]    hwParams                  Pointer to the device structure
   [in]    serverId                  The identifier of server
   [in]    whichFlag                 The identifier of flag to be applied

   [out]   flag                      Pointer to the flag after its GST applied

  Return Value: ZL303XX_OK                        if successful
****************************************************************************/
zlStatusE zl303xx_AprApplyGSTon_VLPA_Flags
            (
            void *hwParams,
            Uint16T serverId,
            zl303xx_AprServerStatusFlagsE whichFlag,
            zl303xx_BooleanE *flag
            )
{
   zl303xx_AprServerStatusFlagsS statusFlags;
   zl303xx_AprServerSMDataS *pSmData;
   zlStatusE status;
    zl303xx_BooleanE algFlag, gstFlag;
    Uint32T *GSTimer, *GSTimerCnt;


    status = ZL303XX_CHECK_POINTERS(hwParams, flag);
    if (status != ZL303XX_OK)
    {
       return status;
    }

    status = zl303xx_AprGetServerStatusFlags(hwParams, serverId, &statusFlags);

    if (status == ZL303XX_OK)
    {
       status = zl303xx_AprGetServerSMData(hwParams, serverId, &pSmData);

       if (status == ZL303XX_OK)
       {
          switch (whichFlag)
          {
          case ZL303XX_SERVER_CLK_GST_L_FLAG:
             algFlag = statusFlags.L;
             gstFlag = statusFlags.gstL;
             GSTimer = &(pSmData->exitLFlagGST);
             GSTimerCnt = &(pSmData->exitLFlagGSTCnt);

             break;
          case ZL303XX_SERVER_CLK_GST_V_FLAG:
             algFlag = statusFlags.V;
             gstFlag = statusFlags.gstV;
             GSTimer = &(pSmData->exitVFlagGST);
             GSTimerCnt = &(pSmData->exitVFlagGSTCnt);

             break;
          case ZL303XX_SERVER_CLK_GST_PA_FLAG:
             algFlag = statusFlags.PA;
             gstFlag = statusFlags.gstPA;
             GSTimer = &(pSmData->exitPAFlagGST);
             GSTimerCnt = &(pSmData->exitPAFlagGSTCnt);

             break;

          case ZL303XX_SERVER_CLK_L1_FLAG :
          case ZL303XX_SERVER_CLK_L2_FLAG :
          case ZL303XX_SERVER_CLK_L3_FLAG :
          case ZL303XX_SERVER_CLK_L4_FLAG :
          case ZL303XX_SERVER_CLK_L_FLAG :
          case ZL303XX_SERVER_CLK_V_FLAG :
          case ZL303XX_SERVER_CLK_S_FLAG :
          case ZL303XX_SERVER_CLK_U_FLAG :
          case ZL303XX_SERVER_CLK_U1_FLAG :
          case ZL303XX_SERVER_CLK_PE_FLAG :
          case ZL303XX_SERVER_CLK_PA_FLAG :
          case ZL303XX_SERVER_CLK_PA_HOLDOVER_FLAG :
          case ZL303XX_SERVER_CLK_H_FLAG :
          case ZL303XX_SERVER_CLK_GST_H_FLAG :
          case ZL303XX_SERVER_CLK_OOR_FLAG :
          case ZL303XX_SERVER_CLK_TT_ERR_FLAG :
          case ZL303XX_SERVER_CLK_OUTAGE_FLAG :
          case ZL303XX_SERVER_CLK_OUTLIER_FLAG :
          case ZL303XX_SERVER_CLK_FRR_FLAG :
          case ZL303XX_SERVER_CLK_RRR_FLAG :
          case ZL303XX_SERVER_CLK_STEP_FLAG :
          case ZL303XX_SERVER_CLK_PKT_LOSS_FWD_FLAG :
          case ZL303XX_SERVER_CLK_PKT_LOSS_REV_FLAG :
          case ZL303XX_SERVER_CLK_PATH_FLAG :
          case ZL303XX_SERVER_CLK_FWD_STATE :
          case ZL303XX_SERVER_CLK_REV_STATE :
          case ZL303XX_SERVER_CLK_STATE :
          case ZL303XX_SERVER_CLK_PSL_ON :
          case ZL303XX_SERVER_CLK_SA_ON :
          case ZL303XX_SERVER_CLK_NUM_TYPES :
          default:
             ZL303XX_TRACE_ALWAYS("AAGVLPAF: Cannot apply the GST on invalid flag %d", whichFlag, 0,0,0,0,0);
             status = ZL303XX_PARAMETER_INVALID;
             return status;
          }

          *flag = gstFlag;

          if (gstFlag == ZL303XX_TRUE)
          {
             /* Check if the corresponding algorithm flag is set 0 */
             if (algFlag == ZL303XX_FALSE)
             {
                /* Check the corresponding guard soak timer, the unit is 10 sec */
                if (*GSTimerCnt >= *GSTimer)
                {
                   *flag = ZL303XX_FALSE;
                   *GSTimerCnt = 0;
                }
                else
                {
                   *GSTimerCnt = *GSTimerCnt + 1;
                   /* the resolution of timer counter is 9600ms. if the counter=n*24 (n=1,2,3...),
                    * subtract the rounding */
                   if ((*GSTimerCnt%24) == 0)
                   {
                      *GSTimerCnt = *GSTimerCnt - 1;
                   }

                }
             }
             else /* algorithm flag is 1 */
             {
                *GSTimerCnt = 0;
             }
          }
          else /* the flag applied GST is 0, whenever the algorithm flag is 1, the flag becomes to 1. */
          {
             if (algFlag == ZL303XX_TRUE)
             {
                 *flag = ZL303XX_TRUE;

             }

          }
          status = zl303xx_AprSetServerSMFlag
                      (
                      hwParams,
                      serverId,
                      whichFlag,
                      flag
                      );

       }
    }

    return status;

}


/** zl303xx_AprPktServerSM

   This function performs the server state machine operations to update the state
   for the specified packet server.
   The function is called every 9600ms.

  Parameters:
   [in]    hwParams                  Pointer to the device structure
   [in]    serverId                  The identifier of server

   [out]   state                     Pointer to the state value

  Return Value: ZL303XX_OK                        if successful
****************************************************************************/
zlStatusE zl303xx_AprPktServerSM
            (
            void *hwParams,
            Uint16T serverId,
            zl303xx_AprStateE *state
            )
{
   zlStatusE status;
   zl303xx_BooleanE bL = ZL303XX_FALSE, bGstV = ZL303XX_FALSE, bGstL = ZL303XX_FALSE,
                  bGstH = ZL303XX_FALSE, bGstPA = ZL303XX_FALSE;
   Uint8T gstV, gstL, gstH, gstPA;
   Uint8T lookupTableIndex;
   zl303xx_AprStateE serverState;
   zl303xx_AprDeviceRefModeE devRefMode;
   zl303xx_AprServerStatusFlagsS statusFlags;

   status = ZL303XX_CHECK_POINTERS(hwParams, state);
   if (status != ZL303XX_OK)
   {
      return status;
   }

   ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 1,
               "zl303xx_AprPktServerSM: start",
               0, 0, 0, 0, 0, 0);

   /* Process the L flag */
   status = zl303xx_AprProcess_L_Flag(hwParams, serverId, &bL);
   if (status != ZL303XX_OK)
   {
      return status;
   }

   status = zl303xx_AprGetDeviceRefMode (hwParams,  &devRefMode);

   ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 1,
               "zl303xx_AprPktServerSM: devRefMode=%d",
               devRefMode, 0, 0, 0, 0, 0);


   /* In packet/hybrid mode, if the server has valid packets, apply the GST on H flag */
   if ((status == ZL303XX_OK) && (devRefMode != ZL303XX_ELECTRIC_MODE))
   {
       zl303xx_BooleanE tsValid = ZL303XX_FALSE;
       zl303xx_BooleanE LocalSA = ZL303XX_FALSE;

      status = zl303xx_AprIsServerClkStreamValid(hwParams, serverId, &tsValid);
      if (status == ZL303XX_OK)
      {
       status = zl303xx_AprGetServerStatusFlags (hwParams, serverId, &statusFlags);
       if (status == ZL303XX_OK)
       {
          LocalSA = statusFlags.SA;
       }
      }
      /* TH: [#13633]*/
      if ((tsValid == ZL303XX_TRUE) || (LocalSA == ZL303XX_TRUE))
      {
          /* Apply the GST on H flag */
          status = zl303xx_AprApplyGSTon_H_Flag(hwParams, serverId, &bGstH);
          if (status != ZL303XX_OK)
          {
             return status;
          }
      }
   }

   /* Apply the GST on V flag */
   status = zl303xx_AprApplyGSTon_VLPA_Flags(hwParams, serverId, ZL303XX_SERVER_CLK_GST_V_FLAG, &bGstV);
   if (status != ZL303XX_OK)
   {
      return status;
   }

   /* Apply the GST on L flag */
   status = zl303xx_AprApplyGSTon_VLPA_Flags(hwParams, serverId, ZL303XX_SERVER_CLK_GST_L_FLAG, &bGstL);
   if (status != ZL303XX_OK)
   {
      return status;
   }

   /* Apply the GST on PA flag */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_AprApplyGSTon_VLPA_Flags(hwParams, serverId, ZL303XX_SERVER_CLK_GST_PA_FLAG, &bGstPA);
      if (status != ZL303XX_OK)
      {
         return status;
      }
   }


   gstV = (bGstV == ZL303XX_TRUE) ?1 :0;
   gstL = (bGstL == ZL303XX_TRUE) ?1 :0;
   gstH = (bGstH == ZL303XX_TRUE) ?1 :0;
   gstPA = (bGstPA == ZL303XX_TRUE) ?1 :0;

   lookupTableIndex = ((gstV<<3)|(gstL<<2)|(gstH<<1)|gstPA)&0xF;
   serverState = zl303xx_AprPktServerSMLookupTable[lookupTableIndex];
   *state = serverState;
   /* Update the server SM state flag */
   status = zl303xx_AprSetServerSMFlag
               (
               hwParams,
               serverId,
               ZL303XX_SERVER_CLK_STATE,
               state
               );

   ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 1,
               "zl303xx_AprPktServerSM: serverId %d Called zl303xx_AprSetServerSMFlag with lookupTableIndex=0x%x serverState=0x%x",
               serverId, lookupTableIndex, serverState, 0, 0, 0);


   return status;

}



/** zl303xx_AprElecRefSM

   This function performs the server state machine operations to update the state
   for the current active electrical reference if the device is in electrical or
   hybrid mode.
   The function is called every 9600ms.

  Parameters:
   [in]    hwParams                  Pointer to the device structure
   [out]   state                     Pointer to the state value

  Return Value: ZL303XX_OK                        if successful
****************************************************************************/
zlStatusE zl303xx_AprElecRefSM
            (
            void *hwParams,
            zl303xx_AprStateE *state
            )
{
   zl303xx_AprElecStatusFlagsS statusFlags;
   zlStatusE status;
   Uint8T RF, L, H, SE;
   Uint8T lookupTableIndex;
   zl303xx_AprStateE elecRefState;
   zl303xx_AprDeviceRefModeE devRefMode;

   status = ZL303XX_CHECK_POINTERS(hwParams, state);
   if (status != ZL303XX_OK)
   {
      return status;
   }

   status = zl303xx_AprGetDeviceRefMode (hwParams,  &devRefMode);

   if (status == ZL303XX_OK)
   {
      /* The state machine only runs when the device is in Electrical/Hybrid mode */
      if ((devRefMode == ZL303XX_ELECTRIC_MODE) || (devRefMode == ZL303XX_HYBRID_MODE))
      {
         status = zl303xx_AprGetElecRefStatusFlags(hwParams, &statusFlags);
         RF = (statusFlags.RF == ZL303XX_TRUE) ?1 :0;
         L = (statusFlags.L == ZL303XX_TRUE) ?1 :0;
         H = (statusFlags.H == ZL303XX_TRUE) ?1 :0;
         SE = (statusFlags.SE == ZL303XX_TRUE) ?1 :0;

         lookupTableIndex = ((RF<<3)|(L<<2)|(H<<1)|SE)&0xF;
         elecRefState = zl303xx_AprElecRefSMLookupTable[lookupTableIndex];
         *state = elecRefState;

         /* Update the ElecRef SM state */
         status = zl303xx_AprSetElecRefSMState
                     (
                     hwParams,
                     *state
                     );

         ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 1,
                     "zl303xx_AprElecRefSM: Called zl303xx_AprSetElecRefSMState with lookupTableIndex=0x%x elecRefState=0x%x",
                     lookupTableIndex, elecRefState, 0, 0, 0, 0);
      }
   }

   return status;

}



/** zl303xx_AprCGUStateMachine

   This function performs the CGU state machine operations to update the state
   for the specified CGU device.
   The function is called every 9600ms.

  Parameters:
   [in]    hwParams                  Pointer to the device structure
   [out]   state                     Pointer to the state value

  Return Value: ZL303XX_OK                        if successful
****************************************************************************/
static zlStatusE zl303xx_AprCGUStateMachine_R
            (
            void *hwParams,
            zl303xx_AprStateE *state
            )
{
   zl303xx_AprCGUStatusFlagsS cguFlags, cguLastFlags;
   zlStatusE status;
   Uint8T hwMH, hwMF, hwH, hwL, RF, SE, swH, swL, V, PA;
   Uint8T lookupTableIndex;
   zl303xx_AprStateE cguState = ZL303XX_UNKNOWN;
   zl303xx_AprDeviceRefModeE devRefMode;

   status = ZL303XX_CHECK_POINTERS(hwParams, state);
   if (status != ZL303XX_OK)
   {
      return status;
   }

   ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 2, "zl303xx_AprCGUStateMachine:\n", 0, 0, 0, 0, 0, 0);  /* Intentional */

   status = zl303xx_AprGetLastCGUStatusFlags(hwParams, &cguLastFlags);
   if (status != ZL303XX_OK)
   {
       ZL303XX_TRACE_ALWAYS ("zl303xx_AprCGUStateMachine: Call to zl303xx_AprGetLastCGUStatusFlags() failed=%d",
                           status, 0,0,0,0,0);
      return status;
   }

   status = zl303xx_AprGetCGUStatusFlags(hwParams, &cguFlags);
   if (status != ZL303XX_OK)
   {
       ZL303XX_TRACE_ALWAYS ("zl303xx_AprCGUStateMachine: Call to zl303xx_AprGetCGUStatusFlags() failed=%d",
                           status, 0,0,0,0,0);
      return status;
   }

   status = zl303xx_AprGetDeviceRefMode (hwParams,  &devRefMode);
   if (status != ZL303XX_OK)
   {
       ZL303XX_TRACE_ALWAYS ("zl303xx_AprCGUStateMachine: Call to zl303xx_AprGetDeviceRefMode() failed=%d",
                           status, 0,0,0,0,0);
      return status;
   }

   /* Special Case:  MANUAL HOLDOVER or MANUAL FREERUN don't flow all the way through the function */
   if (status == ZL303XX_OK && (cguFlags.hwMF || cguFlags.hwMH))
   {
      hwMF = (cguFlags.hwMF == ZL303XX_TRUE) ?1 :0;
      hwMH = (cguFlags.hwMH == ZL303XX_TRUE) ?1 :0;

      lookupTableIndex = ((hwMF << 1)|hwMH) & 0x3;
      cguState = zl303xx_AprCGUHwManualOptLookupTable[lookupTableIndex];

      ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 1,
                  "zl303xx_AprCGUStateMachine: HWMF or HWMH called zl303xx_AprSetCGUSMState with lookupTableIndex=0x%x cguState=0x%x",
                  lookupTableIndex, cguState, 0, 0, 0, 0);


      *state = cguState;

      /* PHASE_LOCK is always holdover CLEAR */
      if (cguState == ZL303XX_PHASE_LOCK_ACQUIRED)
      {
          ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 3, "zl303xx_AprCGUStateMachine: hwMH=%u or hwMF=%d PHASE CLEAR", hwMH, hwMF, 0, 0, 0, 0);

          if ((status =  zl303xx_DpllHoldoverStateChange(hwParams, ZL303XX_HO_CLEAR)) != ZL303XX_OK)
          {
              ZL303XX_TRACE_ALWAYS ("zl303xx_AprCGUStateMachine: Call to zl303xx_DpllHoldoverStateChange() failed=%d (hwMF or hwMH PHASE)",
                                  status, 0,0,0,0,0);
          }
      }

      /* Update the CGU state */
      status = zl303xx_AprSetCGUSMState(hwParams, cguState);

      if (status == ZL303XX_OK)
      {
          /* Print wheter to INCREMENT or FREEZE */
          switch (devRefMode)
          {
          case ZL303XX_ELECTRIC_MODE:

              /* HO special case of MANUAL_HOLDOVER in ELECTRIC */
              if (cguFlags.hwMH)
              {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 3, "zl303xx_AprCGUStateMachine: ELECTRIC hwMH INCREMENT", 0, 0, 0, 0, 0, 0);
              }
              /* HO special case of MANUAL FREERUN in ELECTRIC */
              if (cguFlags.hwMF)
              {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 3, "zl303xx_AprCGUStateMachine: ELECTRIC hwMF FREEZE", 0, 0, 0, 0, 0, 0);
              }
          break;

          case ZL303XX_PACKET_MODE:

              /* HO special case of MANUAL_HOLDOVER in PACKET */
              if (cguFlags.hwMH)
              {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 3, "zl303xx_AprCGUStateMachine: PACKET hwMH INCREMENT", 0, 0, 0, 0, 0, 0);
              }
              /* HO special case of MANUAL FREERUN in PACKET */
              if (cguFlags.hwMF)
              {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 3, "zl303xx_AprCGUStateMachine: PACKET hwMF FREEZE", 0, 0, 0, 0, 0, 0);
              }
          break;

          case ZL303XX_HYBRID_MODE:
              /* HO special case of MANUAL_HOLDOVER in HYBRID */
              if (cguFlags.hwMH)
              {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 3, "zl303xx_AprCGUStateMachine: HYBRID hwMH INCREMENT", 0, 0, 0, 0, 0, 0);
              }
              /* HO special case of MANUAL FREERUN in HYBRID */
              if (cguFlags.hwMF)
              {
                  ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 3, "zl303xx_AprCGUStateMachine: HYBRID hwMF FREEZE", 0, 0, 0, 0, 0, 0);
              }

          break;

          case ZL303XX_NOT_SUPPORT_DEV_REF_MODE:
          default:
              ZL303XX_TRACE_ALWAYS("zl303xx_AprCGUStateMachine: INVALID devRefMode=%d", devRefMode, 0, 0, 0, 0, 0);
              status = ZL303XX_INVALID_MODE;

          break;

          } /* END: switch */


          /* INCREMENT or FREEZE  - Applicable to all modes */
          if (status == ZL303XX_OK && cguFlags.hwMH)
          {
              if ((status =  zl303xx_DpllHoldoverStateChange(hwParams, ZL303XX_HO_INCREMENT)) != ZL303XX_OK)
              {
                  ZL303XX_TRACE_ALWAYS ("zl303xx_AprCGUStateMachine: Call to zl303xx_DpllHoldoverStateChange() failed=%d (INCREMENT hwMH)",
                                      status, 0,0,0,0,0);
              }
          }
          /* HO special case of MANUAL FREERUN in HYBRID */
          if (status == ZL303XX_OK && cguFlags.hwMF)
          {
              if ((status =  zl303xx_DpllHoldoverStateChange(hwParams, ZL303XX_HO_FREEZE)) != ZL303XX_OK)
              {
                  ZL303XX_TRACE_ALWAYS ("zl303xx_AprCGUStateMachine: Call to zl303xx_DpllHoldoverStateChange() failed=%d (FREEZE hwMF)",
                                      status, 0,0,0,0,0);
              }
          }
      }

      return status;
   }


   /* State change based on mode */
   if (status == ZL303XX_OK)
   {
      zl303xx_BooleanE inPktModeNoActiveServer = ZL303XX_FALSE;  /* Flag for pkt monitor mode (not active) */

      ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 1,
                  "zl303xx_AprCGUStateMachine: devRefMode=%d",
                  devRefMode, 0, 0, 0, 0, 0);

      hwH = (cguFlags.hwH == ZL303XX_TRUE) ?1 :0;
      hwL = (cguFlags.hwL == ZL303XX_TRUE) ?1 :0;
      RF = (cguFlags.RF == ZL303XX_TRUE) ?1 :0;
      SE = (cguFlags.SE == ZL303XX_TRUE) ?1 :0;
      swH = (cguFlags.swH == ZL303XX_TRUE) ?1 :0;
      swL = (cguFlags.swL == ZL303XX_TRUE) ?1 :0;
      V = (cguFlags.V == ZL303XX_TRUE) ?1 :0;

      switch (devRefMode)
      {
      case ZL303XX_ELECTRIC_MODE:
         lookupTableIndex = ((RF<<3)|(hwL<<2)|(hwH<<1)|SE)&0xF;
         cguState = zl303xx_AprElecRefSMLookupTable[lookupTableIndex];

         ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 1,
                     "zl303xx_AprCGUStateMachine: ELEC mode with lookupTableIndex=0x%x cguState=0x%x",
                     lookupTableIndex, cguState, 0, 0, 0, 0);


         if (cguState == ZL303XX_HOLDOVER || cguState == ZL303XX_REF_FAILED || cguState == ZL303XX_MANUAL_HOLDOVER)
         {
             ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 1, "zl303xx_AprCGUStateMachine: ELECTRIC INCREMENT", 0, 0, 0, 0, 0, 0);

             if ((status =  zl303xx_DpllHoldoverStateChange(hwParams, ZL303XX_HO_INCREMENT)) != ZL303XX_OK)
             {
                 ZL303XX_TRACE_ALWAYS ("zl303xx_AprCGUStateMachine: Call to zl303xx_DpllHoldoverStateChange() failed=%d (ELECTRIC INCREMENT)",
                                     status, 0,0,0,0,0);
             }
         }
         else   /* Others are FREEZE */
         {
             ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 1, "zl303xx_AprCGUStateMachine: ELECTRIC FREEZE", 0, 0, 0, 0, 0, 0);

             if ((status =  zl303xx_DpllHoldoverStateChange(hwParams, ZL303XX_HO_FREEZE)) != ZL303XX_OK)
             {
                 ZL303XX_TRACE_ALWAYS ("zl303xx_AprCGUStateMachine: Call to zl303xx_DpllHoldoverStateChange() failed=%d (ELECTRIC FREEZE)",
                                     status, 0,0,0,0,0);
             }
         }

         break;

      case ZL303XX_PACKET_MODE:
         /* HO special case of SERVO_HOLDOVER in PACKET */
         if (cguFlags.swMH == ZL303XX_TRUE)
         {
             ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 1, "zl303xx_AprCGUStateMachine: PACKET swMH INCREMENT", 0, 0, 0, 0, 0, 0);

            cguState = ZL303XX_MANUAL_SERVO_HOLDOVER;

            if ((status =  zl303xx_DpllHoldoverStateChange(hwParams, ZL303XX_HO_INCREMENT)) != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS ("zl303xx_AprCGUStateMachine: Call to zl303xx_DpllHoldoverStateChange() failed=%d (PACKET swMH)",
                                    status, 0,0,0,0,0);
            }
         }
         else
         {
             Uint16T activeServerId;
             static Uint8T bWarmStart = ZL303XX_FALSE;

             if (cguFlags.V == ZL303XX_TRUE && cguFlags.WS == ZL303XX_TRUE)
             {
                 bWarmStart = ZL303XX_TRUE;
             }

             (void)zl303xx_AprGetDeviceCurrActiveRef(hwParams, &activeServerId);

             if (activeServerId == 0xFFFF)
             {
                 V = 0;   /* This will FORCE PktServerSM to re-evaluate (ie force ref_fail) */
                 inPktModeNoActiveServer = ZL303XX_TRUE;   /* Flag this condition for a special print string */
             }

             {
                 zl303xx_AprServerStatusFlagsS statusFlags;

                 status = zl303xx_AprGetServerStatusFlags(hwParams, activeServerId, &statusFlags);
                 if(( activeServerId == 0xFFFF ) || ( status != ZL303XX_OK))
                 {
                    PA = 0;
                 }
                 else
                 {
                    PA = statusFlags.PA;
                 }
             }

             if (bWarmStart & cguFlags.V) /* transient warmstart condition */
             {
                 lookupTableIndex = ((swL<<2)|(swH<<1)|PA)&0xF;   /* Ignore the valid bit transition  during warmstart only */
                 bWarmStart = ZL303XX_FALSE;
             }
             else
             {
                 lookupTableIndex = ((V<<3)|(swL<<2)|(swH<<1)|PA)&0xF;
             }

             cguState = zl303xx_AprPktServerSMLookupTable[lookupTableIndex];

             if (inPktModeNoActiveServer)
             {
                 cguState = ZL303XX_NO_ACTIVE_SERVER;   /* No active server */
             }


             if (cguState == ZL303XX_HOLDOVER || cguState == ZL303XX_REF_FAILED || cguState == ZL303XX_MANUAL_HOLDOVER || cguState == ZL303XX_MANUAL_SERVO_HOLDOVER)
             {
                 ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 1, "zl303xx_AprCGUStateMachine: PACKET INCREMENT", 0, 0, 0, 0, 0, 0);

                 if ((status =  zl303xx_DpllHoldoverStateChange(hwParams, ZL303XX_HO_INCREMENT)) != ZL303XX_OK)
                 {
                     ZL303XX_TRACE_ALWAYS ("zl303xx_AprCGUStateMachine: Call to zl303xx_DpllHoldoverStateChange() failed=%d (PACKET INCREMENT)",
                                         status, 0,0,0,0,0);
                 }
             }
             else   /* Others are FREEZE */
             {
                 ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 1, "zl303xx_AprCGUStateMachine: PACKET FREEZE", 0, 0, 0, 0, 0, 0);

                 if ((status =  zl303xx_DpllHoldoverStateChange(hwParams, ZL303XX_HO_FREEZE)) != ZL303XX_OK)
                 {
                     ZL303XX_TRACE_ALWAYS ("zl303xx_AprCGUStateMachine: Call to zl303xx_DpllHoldoverStateChange() failed=%d (PACKET FREEZE)",
                                         status, 0,0,0,0,0);
                 }
             }

             ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 1,
                         "zl303xx_AprCGUStateMachine: PKT mode with lookupTableIndex=0x%x cguState=0x%x",
                         lookupTableIndex, cguState, 0, 0, 0, 0);
         }

         break;

      case ZL303XX_HYBRID_MODE:
          /* HO special case of SERVO_HOLDOVER in HYBRID */
         if (cguFlags.swMH == ZL303XX_TRUE)
         {
            cguState = ZL303XX_MANUAL_SERVO_HOLDOVER;

            ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 1, "zl303xx_AprCGUStateMachine: HYBRID swMH INCREMENT", 0, 0, 0, 0, 0, 0);

            if ((status =  zl303xx_DpllHoldoverStateChange(hwParams, ZL303XX_HO_INCREMENT)) != ZL303XX_OK)
            {
                ZL303XX_TRACE_ALWAYS ("zl303xx_AprCGUStateMachine: Call to zl303xx_DpllHoldoverStateChange() failed=%d(HYBRID swMH)",
                                    status, 0,0,0,0,0);
            }
         }
         else
         {
             Uint16T activeServerId;

            (void)zl303xx_AprGetDeviceCurrActiveRef(hwParams, &activeServerId);
            if (activeServerId == 0xFFFF)
            {
                V = 0;   /* This will FORCE HybridSM to re-evaluate (force ref_fail) */
                inPktModeNoActiveServer = ZL303XX_TRUE;   /* Flag this condition for a special print string */
            }

            {
                 zl303xx_AprServerStatusFlagsS statusFlags;

                 status = zl303xx_AprGetServerStatusFlags(hwParams, activeServerId, &statusFlags);
                 PA = statusFlags.PA;
                 if(( activeServerId == 0xFFFF ) || ( status != ZL303XX_OK))
                 {
                    PA = 0;
                 }
                 else
                 {
                    PA = statusFlags.PA;
                 }
            }

            lookupTableIndex = ((RF<<3)|(hwL<<2)|(hwH<<1)|PA)&0xF;
            cguState = zl303xx_AprHybridSMLookupTable[lookupTableIndex];
            if (inPktModeNoActiveServer)
            {
                cguState = ZL303XX_NO_ACTIVE_SERVER;   /* No active server */
            }

            ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 1,
                        "zl303xx_AprCGUStateMachine: HYBRD mode with lookupTableIndex=0x%x cguState=0x%x",
                        lookupTableIndex, cguState, 0, 0, 0, 0);


            if (cguState == ZL303XX_FREQ_LOCK_ACQUIRED || cguState == ZL303XX_FREQ_LOCK_ACQUIRING || cguState == ZL303XX_HOLDOVER ||
                 cguState == ZL303XX_REF_FAILED || cguState == ZL303XX_MANUAL_HOLDOVER || cguState == ZL303XX_MANUAL_SERVO_HOLDOVER)
            {
                ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 1, "zl303xx_AprCGUStateMachine: HYBRID INCREMENT", 0, 0, 0, 0, 0, 0);

                if ((status =  zl303xx_DpllHoldoverStateChange(hwParams, ZL303XX_HO_INCREMENT)) != ZL303XX_OK)
                {
                    ZL303XX_TRACE_ALWAYS ("zl303xx_AprCGUStateMachine: Call to zl303xx_DpllHoldoverStateChange() failed=%d (HYBRID INCREMENT)",
                                        status, 0,0,0,0,0);
                }
            }
            else   /* Others are FREEZE */
            {
                ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 1, "zl303xx_AprCGUStateMachine: PACKET FREEZE", 0, 0, 0, 0, 0, 0);

                if ((status =  zl303xx_DpllHoldoverStateChange(hwParams, ZL303XX_HO_FREEZE)) != ZL303XX_OK)
                {
                    ZL303XX_TRACE_ALWAYS ("zl303xx_AprCGUStateMachine: Call to zl303xx_DpllHoldoverStateChange() failed=%d (PACKET FREEZE)",
                                        status, 0,0,0,0,0);
                }
            }
         }

         break;

      case ZL303XX_NOT_SUPPORT_DEV_REF_MODE :
      default:
          ZL303XX_TRACE_ALWAYS("zl303xx_AprCGUStateMachine: INVALID devRefMode=%d", devRefMode, 0, 0, 0, 0, 0);
          status = ZL303XX_INVALID_MODE;

         break;
      } /* END: switch */

      if ( (status == ZL303XX_OK) || (inPktModeNoActiveServer == ZL303XX_TRUE) )
      {
          /* Update the CGU state */
          *state = cguState;
          status = zl303xx_AprSetCGUSMState(hwParams, cguState);

          ZL303XX_TRACE(ZL303XX_MOD_ID_INIT, 1,
                      "zl303xx_AprCGUStateMachine: Called zl303xx_AprSetCGUSMState with cguState=%u",
                      cguState, 0, 0, 0, 0, 0);
      }

      /* PHASE_LOCK is always holdover CLEAR */
      if (status == ZL303XX_OK && (cguState == ZL303XX_PHASE_LOCK_ACQUIRED))
      {
          ZL303XX_TRACE(ZL303XX_MOD_ID_HO_UTILS, 1, "zl303xx_AprCGUStateMachine: PHASE CLEAR", 0, 0, 0, 0, 0, 0);

          if ((status =  zl303xx_DpllHoldoverStateChange(hwParams, ZL303XX_HO_CLEAR)) != ZL303XX_OK)
          {
              ZL303XX_TRACE_ALWAYS ("zl303xx_AprCGUStateMachine: Call to zl303xx_DpllHoldoverStateChange() failed=%d (ELECTRIC PHASE)",
                                  status, 0,0,0,0,0);
          }
      }
   }

   return status;
}

const char* zl303xx_AprCGUStateMachine_StateNameStr[17] = 
{
	"FREQ_LOCK_ACQUIRING",
	"FREQ_LOCK_ACQUIRED",
	"PHASE_LOCK_ACQUIRED",
	"HOLDOVER",
	"REF_FAILED",
	"NO_ACTIVE_SERVER",
	"UNKNOWN",
	"MANUAL_FREERUN",
	"MANUAL_HOLDOVER",
	"MANUAL_SERVO_HOLDOVER",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"INITIAL"	/* fj original */
};

#include <string.h>
#include "f_rrh_def.h"
#include "f_du_reg_eth.h"
#include "BPF_COM_PLOG.h"
#include "f_com_inc.h"
#include "zl303xx_RdWr.h"
#include "zl303xx_Dpll77x.h"

typedef struct {
	unsigned short   state;
	char phaseAlignAccuracy_sign;
	char mostRecentCalculatedOffset_sign;
	unsigned int phaseAlignAccuracy_hi;
	unsigned int phaseAlignAccuracy_lo;
	unsigned int mostRecentCalculatedOffset_hi;
	unsigned int mostRecentCalculatedOffset_lo;
	unsigned int mostRecentCalculatedOffset_hi2[10];
	unsigned int mostRecentCalculatedOffset_lo2[10];
	unsigned int adjustmentNetworkQuality2[10];
	int          freqOffset;
	char startElement[10];
	char mostRecentCalculatedOffset_sign2[10];
	char adjustmentUsable[10];
	char adjustmentApplied[10];
}ptp_ploginfo_t;	/* 184 */


extern UCHAR	*f_ptp_sfnfrm_flag;
extern UCHAR	*f_ptp_ns_flag;

#define D_PTP_FHSYNC_FAULT_SUPPRESS_VAL	300
static unsigned int f_ptpw_faultCeaseCount = 0;
static long     f_ptpw_faultSuppressCount = 0;
static unsigned int f_ptpw_faultSuppressFlg = D_RRH_ON;


static ptp_ploginfo_t f_ptpw_info_data;
static char ptplog_path[] = "/proc/rru/ptplog";
static char ptpinfo_path[] = "/proc/rru/ptpinfo";
static zl303xx_AprStateE f_ptpw_preAprState = 0x10;
extern zlStatusE zl303xx_AprGetNext1HzMetricElement
            (
            void *hwParams,
            Uint16T serverId,
            Sint32T *startElement,
            zl303xx_BooleanE *lastElement,
            Uint64S *adjustment,
            zl303xx_BooleanE *adjustmentUsable,
            zl303xx_BooleanE *adjustmentApplied,
            Uint32T *adjustmentNetworkQuality
            );
extern zlStatusE zl303xx_AprGetServerFreqOffset (void *hwParams, Sint32T *freqOffset);


static void Info_AlignmentAccuracy(
            void *hwParams,
            char* sign_phaseAlignAccuracy,
            Uint64S* phaseAlignAccuracyV,
            char* sign_mostRecentCalculatedOffset,
            Uint64S* mostRecentCalculatedOffsetV
)
{
	Uint64S phaseAlignAccuracy;
	Sint64T T64_phaseAlignAccuracy;
	char sign[2] = { 0 };
	Uint64S mostRecentCalculatedOffset;
	Uint32T mostRecentEstimatedNetworkQuality;
	zl303xx_BooleanE mostRecentAssigned;
	Sint64T T64_mostRecentCalculatedOffset;

	*sign_phaseAlignAccuracy = '-';
	*sign_mostRecentCalculatedOffset = '-';
	memset(phaseAlignAccuracyV,0,sizeof(*phaseAlignAccuracyV));
	memset(mostRecentCalculatedOffsetV,0,sizeof(*mostRecentCalculatedOffsetV));
	

	zlStatusE status = zl303xx_AprGetDevicePhaseAlignmentAccuracy(hwParams, &phaseAlignAccuracy);
	if(status == ZL303XX_OK){
		
		ZL303XX_CONVERT_TO_64_SIGNED(T64_phaseAlignAccuracy, phaseAlignAccuracy);
		
		if( T64_phaseAlignAccuracy < 0 )
		{
		    T64_phaseAlignAccuracy = -T64_phaseAlignAccuracy;
		    sign[0] = '-';
		}
		ZL303XX_CONVERT_FROM_64(phaseAlignAccuracy, T64_phaseAlignAccuracy);
		
		status = zl303xx_AprGetDevice1HzMostRecentAlignmentDifference(hwParams,
					&mostRecentCalculatedOffset,
					&mostRecentEstimatedNetworkQuality,
					&mostRecentAssigned );
		
		if(status == ZL303XX_OK){
			char sign2[5] = " ";

			ZL303XX_CONVERT_TO_64_SIGNED(T64_mostRecentCalculatedOffset, mostRecentCalculatedOffset);
			if( T64_mostRecentCalculatedOffset < 0 )
			{
			    T64_mostRecentCalculatedOffset = -T64_mostRecentCalculatedOffset;
			    sign2[0] = '-';
			}
			ZL303XX_CONVERT_FROM_64(mostRecentCalculatedOffset, T64_mostRecentCalculatedOffset);

			if(sign[0] != '-'){
				*sign_phaseAlignAccuracy = '+';
			}
			if(sign2[0] != '-'){
				*sign_mostRecentCalculatedOffset = '+';
			}

			memcpy(phaseAlignAccuracyV,&phaseAlignAccuracy,sizeof(phaseAlignAccuracy));
			memcpy(mostRecentCalculatedOffsetV,&mostRecentCalculatedOffset,sizeof(mostRecentCalculatedOffset));
		}
	}else{
		phaseAlignAccuracyV->hi		   = 0xdeadbeef;	/* 無効値(適当なdeadbeef値)としておき、Faultになるようにする */
		mostRecentCalculatedOffsetV->hi = 0xdeadbeef;	/* 無効値(適当なdeadbeef値)としておき、Faultになるようにする */
	}
	return;
}



static void Info_MetricElement(
            void *hwParams,
            ptp_ploginfo_t* info_p
)
{
	zlStatusE status;
	Sint32T startElement[10];
	Sint32T startElement_count = -1;
	zl303xx_BooleanE lastElement = ZL303XX_FALSE;
	Uint64S adjustment[10];
	zl303xx_BooleanE adjustmentUsable[10];
	zl303xx_BooleanE adjustmentApplied[10];
	Uint32T adjustmentNetworkQuality[10];
	Sint64T T64_mostRecentCalculatedOffset[10];
	Uint64S mostRecentCalculatedOffset[10];
	char sign[10];
	int	count = 0;
	int	maxcount = 0;

	do{
		startElement[maxcount] = -1;
		status = zl303xx_AprGetNext1HzMetricElement(hwParams, 0, 
					&startElement_count,&lastElement,&adjustment[maxcount],&adjustmentUsable[maxcount],
					&adjustmentApplied[maxcount],&adjustmentNetworkQuality[maxcount]);
		startElement[maxcount] = startElement_count;

		ZL303XX_CONVERT_TO_64_SIGNED(T64_mostRecentCalculatedOffset[maxcount], adjustment[maxcount]);
		if( T64_mostRecentCalculatedOffset[maxcount] < 0 ){
		    T64_mostRecentCalculatedOffset[maxcount] = -T64_mostRecentCalculatedOffset[maxcount];
		    sign[maxcount] = '-';
		}
		else{
		    sign[maxcount] = ' ';
		}
		ZL303XX_CONVERT_FROM_64(mostRecentCalculatedOffset[maxcount], T64_mostRecentCalculatedOffset[maxcount]);
		maxcount++;
		if(maxcount > 9){
			break;
		}
	} while (status == ZL303XX_OK && lastElement == ZL303XX_FALSE);

	for(count=0; count < 10 ; count++){
		info_p->startElement[count]                     = (char)startElement[count];
		info_p->mostRecentCalculatedOffset_sign2[count] = (char)sign[count];
		info_p->mostRecentCalculatedOffset_hi2[count]   = (unsigned int)mostRecentCalculatedOffset[count].hi;
		info_p->mostRecentCalculatedOffset_lo2[count]   = (unsigned int)mostRecentCalculatedOffset[count].lo;
		info_p->adjustmentNetworkQuality2[count]        = (unsigned int)adjustmentNetworkQuality[count];
		info_p->adjustmentUsable[count]                 = (char)adjustmentUsable[count];
		info_p->adjustmentApplied[count]                = (char)adjustmentApplied[count];
	}
	return;
}

static void Info_AprGetServerFreqOffset(
            void *hwParams,
            ptp_ploginfo_t* info_p
){
	zlStatusE status;
	Sint32T freqOffset;
	
	status = zl303xx_AprGetServerFreqOffset(hwParams,&freqOffset);
	if(status == ZL303XX_OK){
		info_p->freqOffset = (int)freqOffset;
	}else{
		info_p->freqOffset = -1;
	}
	return;
}


static ZLS3077X_DpllHWModeE fj_holdover_mode_get(
            void *hwParams
)
{
	zlStatusE status;
	Uint32T data = 0;
	zl303xx_ParamsS *zl303xx_Params = hwParams;


	if(zl303xx_Params == NULL){
		ZL303XX_TRACE_ALWAYS ("zl303xx_Params is NULL",0,0,0,0,0,0);
		return ZLS3077X_DPLL_MODE_MASK;
	}

	if(zl303xx_Params->pllParams.pllId != 3){
		ZL303XX_TRACE_ALWAYS ("ERROR Unexpected PLLID:%d",zl303xx_Params->pllParams.pllId,0,0,0,0,0);
		return ZLS3077X_DPLL_MODE_MASK;
	}

	/* PLL 3で判断する */
	status = zl303xx_Read(zl303xx_Params, NULL,ZLS3077X_DPLL_MODE_REFSEL_REG(3), &data);
	if(ZL303XX_OK != status){
		ZL303XX_TRACE_ALWAYS ("ERROR Get Hold over status %d",data,0,0,0,0,0);
	}
	return (ZLS3077X_DpllHWModeE)(data & ZLS3077X_DPLL_MODE_MASK);
}

static void fj_holdover_mode_set(
            void *hwParams,
 ZLS3077X_DpllHWModeE mode
 
)
{
	zlStatusE status1;
	zlStatusE status3;
	Uint32T hwRef;
	zl303xx_ParamsS *zl303xx_Params = hwParams;


	if(zl303xx_Params->pllParams.pllId != 3){
		return;
	}

	hwRef = ((zl303xx_Params->pllParams.selectedRef & ZLS3077X_DPLL_REF_SEL_MASK) << ZLS3077X_REF_SEL_OFFSET_SHIFT);

	if(mode == ZLS3077X_DPLL_MODE_HOLDOVER){
          status1 = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                                    ZLS3077X_DPLL_MODE_REFSEL_REG(1),
                                    mode, ZLS3077X_DPLL_MODE_MASK, NULL);
          status3 = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                                    ZLS3077X_DPLL_MODE_REFSEL_REG(3),
                                    mode, ZLS3077X_DPLL_MODE_MASK, NULL);

		ZL303XX_TRACE_ALWAYS ("Mode(%d) to Holdover hwRef:%d",mode,hwRef,0,0,0,0);


	}else{
            status1 = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                                       ZLS3077X_DPLL_MODE_REFSEL_REG(1),
                                       (hwRef | mode),
                                       ZLS3077X_DPLL_REFMODE_MASK, NULL);
            status3 = zl303xx_ReadModWrite(zl303xx_Params, NULL,
                                       ZLS3077X_DPLL_MODE_REFSEL_REG(3),
                                       (hwRef | mode),
                                       ZLS3077X_DPLL_REFMODE_MASK, NULL);


		ZL303XX_TRACE_ALWAYS ("Holdover to REF(%d) hwRef:%d",mode,hwRef,0,0,0,0);

	}

	if((status1 != ZL303XX_OK) || (status3 != ZL303XX_OK )){
		ZL303XX_TRACE_ALWAYS ("ERROR Get Hold over status %d %d %d %d",mode,status1,status3,hwRef,0,0);
	}
	return;
}


extern zl303xx_ParamsS *zl303xx_Params0;   /* The device instance structure */

zlStatusE zl303xx_AprCGUStateMachine
            (
            void *hwParams,
            zl303xx_AprStateE *state
            )
{
	zlStatusE status = zl303xx_AprCGUStateMachine_R(hwParams,state);
	unsigned int syncE_ptp_ClockClass;
	ZLS3077X_DpllHWModeE hwMode;
	zlStatusE status_holdover = 0;	/* zl303xx_AprSetHoldoverは保留 */

	if(status == ZL303XX_OK){
		char sign_phaseAlignAccuracy=0;
		Uint64S phaseAlignAccuracy;
		char sign_mostRecentCalculatedOffset=0;
		Uint64S mostRecentCalculatedOffset;
		
		switch(*state){
			case ZL303XX_FREQ_LOCK_ACQUIRING:
			case ZL303XX_FREQ_LOCK_ACQUIRED:
			case ZL303XX_PHASE_LOCK_ACQUIRED:
			case ZL303XX_HOLDOVER:
			case ZL303XX_REF_FAILED:
			case ZL303XX_MANUAL_FREERUN:
			case ZL303XX_MANUAL_HOLDOVER:
			case ZL303XX_MANUAL_SERVO_HOLDOVER:
				memset(&f_ptpw_info_data,0,sizeof(f_ptpw_info_data));

				Info_AlignmentAccuracy(hwParams,
									&sign_phaseAlignAccuracy,&phaseAlignAccuracy,
									&sign_mostRecentCalculatedOffset,&mostRecentCalculatedOffset);

				f_ptpw_info_data.state = (unsigned short)*state;
				f_ptpw_info_data.phaseAlignAccuracy_sign = sign_phaseAlignAccuracy;
				if(sign_phaseAlignAccuracy == '+'){
					f_ptpw_info_data.phaseAlignAccuracy_sign = ' ';
				}else{
					f_ptpw_info_data.phaseAlignAccuracy_sign = sign_phaseAlignAccuracy;
				}
				f_ptpw_info_data.phaseAlignAccuracy_hi = (unsigned int)phaseAlignAccuracy.hi;
				f_ptpw_info_data.phaseAlignAccuracy_lo = (unsigned int)phaseAlignAccuracy.lo;
				if(sign_mostRecentCalculatedOffset == '+'){
					f_ptpw_info_data.mostRecentCalculatedOffset_sign = ' ';
				}else{
					f_ptpw_info_data.mostRecentCalculatedOffset_sign = sign_mostRecentCalculatedOffset;
				}

				f_ptpw_info_data.mostRecentCalculatedOffset_hi = (unsigned int)mostRecentCalculatedOffset.hi;
				f_ptpw_info_data.mostRecentCalculatedOffset_lo = (unsigned int)mostRecentCalculatedOffset.lo;

				Info_MetricElement(hwParams,&f_ptpw_info_data);
				
				Info_AprGetServerFreqOffset(hwParams,&f_ptpw_info_data);

				bpf_com_plog_com(ptpinfo_path , E_PTPINFO_LV_DATA,sizeof(f_ptpw_info_data),&f_ptpw_info_data,D_BPF_COM_PLOG_IPOFS);
				if(
					(*state == ZL303XX_PHASE_LOCK_ACQUIRED)  && 
					(f_ptpw_info_data.phaseAlignAccuracy_lo > D_DU_PTP_UNLOCK_THRESHOLD_VAL)
				){
					ZL303XX_TRACE_ALWAYS ("ERROR Status is %s but the PTP threshold was exceeded.Cur Accuracy:%c0x%X%08X pre Status %s",
												zl303xx_AprCGUStateMachine_StateNameStr[*state & 0xF],
												f_ptpw_info_data.phaseAlignAccuracy_sign,
												f_ptpw_info_data.phaseAlignAccuracy_hi,
												f_ptpw_info_data.phaseAlignAccuracy_lo,
												zl303xx_AprCGUStateMachine_StateNameStr[f_ptpw_preAprState],
												0);
				}
				break;
			case ZL303XX_NO_ACTIVE_SERVER:
			default:
				/* Do nothing */
				break;
		}
		
		if(f_ptpw_preAprState != *state){
			bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
				"AprCGUStateMachine Change Status(%d) %s -> Status(%d) %s",
							f_ptpw_preAprState,zl303xx_AprCGUStateMachine_StateNameStr[f_ptpw_preAprState],
							*state,zl303xx_AprCGUStateMachine_StateNameStr[*state & 0xF]);
			f_ptpw_preAprState = *state & 0xF;

			/* SVレジスタへの書込み */
			switch(*state){
				case ZL303XX_PHASE_LOCK_ACQUIRED:
					break;/* fault回復と同期する */
				case ZL303XX_FREQ_LOCK_ACQUIRING:
				case ZL303XX_FREQ_LOCK_ACQUIRED:
				case ZL303XX_HOLDOVER:
				case ZL303XX_MANUAL_FREERUN:
				case ZL303XX_MANUAL_HOLDOVER:
				case ZL303XX_MANUAL_SERVO_HOLDOVER:
					/* sync locked status(B30-B28) FREERUN */
					f_com_SVCtl(E_DU_SVCTL_OPE_SYNC, D_DU_SVCTL_CHG, D_DU_SVCTL_ON_SYNC_STATE_FREERUN);
					break;
				case ZL303XX_REF_FAILED:			/* Ref*/
				case ZL303XX_NO_ACTIVE_SERVER:
				default:
					/* sync locked status(B30-B28) FREERUN */
					f_com_SVCtl(E_DU_SVCTL_OPE_SYNC, D_DU_SVCTL_CHG, D_DU_SVCTL_ON_SYNC_STATE_FREERUN);
					/* PTP Locked Status(B25) OFF */
					break;
			}
		}

		/* LINK状態を取得 */
		Uint32T info;
		Uint32T info_val;
		info = M_DU_REG_ETH_PORT0(D_DU_REG_ETH_IRQMACSTI);
		(void)BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, info, D_RRH_ON , &info_val); 

		/* LINKUP ? */
		if((info_val & D_RRH_ON) == 0){
			struct timespec	boottime;
			if(f_ptpw_faultSuppressCount == 0){
				(void)clock_gettime( CLOCK_MONOTONIC_COARSE   , &boottime);
				f_ptpw_faultSuppressCount = boottime.tv_sec;
				bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_INFO,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
					"Suppressed Fronthaul Synchronization Error(s:%ld) c:%u ",f_ptpw_faultSuppressCount,f_ptpw_faultCeaseCount);
			}else{
				if(f_ptpw_faultSuppressFlg == D_RRH_ON){
					(void)clock_gettime( CLOCK_MONOTONIC_COARSE   , &boottime);
					/* 5分経過(300秒経過)したら、抑止解除 */
					if((boottime.tv_sec - f_ptpw_faultSuppressCount) >= D_PTP_FHSYNC_FAULT_SUPPRESS_VAL ){
						bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_INFO,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
							"UnSuppressed Fronthaul Synchronization Error(s:%ld ~ e:%ld) c:%u ",f_ptpw_faultSuppressCount,boottime.tv_sec,f_ptpw_faultCeaseCount);
						f_ptpw_faultSuppressFlg = D_RRH_OFF;
						/* すでにfault状態の場合は強制的にfaultにする*/
						if( f_ptpw_faultCeaseCount < 2 ){
							f_com_SVCtl(E_DU_SVCTL_OPE_FID03_ALM, D_DU_SVCTL_ON, D_DU_FLTID_PTP_UNLOCK);
						}
					}
				}
			}
		}else{
			if(f_ptpw_faultSuppressFlg == D_RRH_OFF){
				bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
					"Detected FH L1 Link down while unsuppressing(s:%ld) c:%u ",f_ptpw_faultSuppressCount,f_ptpw_faultCeaseCount);
				f_com_SVCtl(E_DU_SVCTL_OPE_FID03_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_PTP_UNLOCK);
			}
			f_ptpw_faultSuppressFlg = D_RRH_ON;
			f_ptpw_faultSuppressCount = 0;
		}

		/*	Fault条件:以下の条件が10s以上継続した場合に回復とする、それ以外はFault発生する	*/
		/*	The phase alignment accuracy =±80(ns)以内										*/
		/*	The current phase alignment accuracy =±80(ns)以内								*/
		/*	Device Lock state = ZL303XX_PHASE_LOCK_ACQUIRED									*/
		if(
			(*state != ZL303XX_PHASE_LOCK_ACQUIRED) || 
			((sign_phaseAlignAccuracy == '+') && ((phaseAlignAccuracy.hi > 0))) ||
			((sign_phaseAlignAccuracy == '+') && ((phaseAlignAccuracy.hi == 0) && (phaseAlignAccuracy.lo > D_DU_PTP_UNLOCK_THRESHOLD_VAL))) ||
			((sign_phaseAlignAccuracy == '+') && ((mostRecentCalculatedOffset.hi > 0))) ||
			((sign_mostRecentCalculatedOffset == '+') && ((mostRecentCalculatedOffset.hi == 0) && (mostRecentCalculatedOffset.lo > D_DU_PTP_UNLOCK_THRESHOLD_VAL))) ||
			((sign_phaseAlignAccuracy == '-') && ((phaseAlignAccuracy.hi > 0))) ||
			((sign_phaseAlignAccuracy == '-') && ((phaseAlignAccuracy.hi == 0) && (phaseAlignAccuracy.lo > D_DU_PTP_UNLOCK_THRESHOLD_VAL))) ||
			((sign_phaseAlignAccuracy == '-') && ((mostRecentCalculatedOffset.hi > 0))) ||
			((sign_mostRecentCalculatedOffset == '-') && ((mostRecentCalculatedOffset.hi == 0) && (mostRecentCalculatedOffset.lo > D_DU_PTP_UNLOCK_THRESHOLD_VAL)))
		){
			/* Alarm mask check  */
			/* 自律送信モードon時        => eeprom3c2が1のときであり、PTPソフト自体が起動しないため考慮不要 */
			/* Sync側のFH PortがL1断の時 => 現状のport番号を取得し、L1断を識別したらアラーム回復		    */
			if(f_ptpw_faultSuppressFlg == D_RRH_OFF){
				/* fault raise */
				f_com_SVCtl(E_DU_SVCTL_OPE_FID03_ALM, D_DU_SVCTL_ON, D_DU_FLTID_PTP_UNLOCK);
			}

			Uint32T regVal;
			/* LOCKしてる場合はFREE RUNとする */
			(void)BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV16INF, D_DU_SVCTL_SYNC_STATE_ALL , &regVal);
			if( regVal == D_DU_SVCTL_SYNC_STATE_LOCKED ){
				f_com_SVCtl(E_DU_SVCTL_OPE_SYNC, D_DU_SVCTL_CHG, D_DU_SVCTL_ON_SYNC_STATE_FREERUN);
			}

			/* PTP Unlock によるStop wave */
			f_com_SVCtl(E_DU_SVCTL_OPE_STP_WAV, D_DU_SVCTL_ON, D_DU_SVCTL_ON_WAVE_STOP_ALL_PTP_UNLOCK);

			/* PTP Locked Status(B25) ON */
			f_com_SVCtl(E_DU_SVCTL_OPE_SPLAN, D_DU_SVCTL_OFF, D_DU_SVCTL_ON_PTP_STATE_LOCKED);
			ptpLockedFlag = ZL303XX_FALSE;

			f_ptpw_faultCeaseCount = 0;
			if( *f_ptp_sfnfrm_flag == 1 ){
				*f_ptp_ns_flag = 0;
			}
		}
		else{
			/* 10秒以上継続かつ1ppsのsfn load完了後にFault回復する							*/
			/* StateMachineの遷移は約10秒周期のため2回目で回復させる						*/
			if(f_ptpw_faultCeaseCount >= 1){
				/* 回復直後 */
				if(f_ptpw_faultCeaseCount == 1){
					f_ptpw_faultCeaseCount = 2;	/* keep */
					if( *f_ptp_sfnfrm_flag == 1 ) {
						if(*f_ptp_ns_flag > 0){
							bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
								"1PPS SFN load not compleated yet(last:%dsec).",*f_ptp_ns_flag);
							f_ptpw_faultCeaseCount = 1;
						}
					}
					if(f_ptpw_faultCeaseCount == 2){
						/* sync locked status(B30-B28) Locked */
						f_com_SVCtl(E_DU_SVCTL_OPE_SYNC, D_DU_SVCTL_CHG, D_DU_SVCTL_ON_SYNC_STATE_LOCKED);
						/* PTP Unlock によるStop wave 解除 */
						f_com_SVCtl(E_DU_SVCTL_OPE_STP_WAV, D_DU_SVCTL_OFF, D_DU_SVCTL_ON_WAVE_STOP_ALL_PTP_UNLOCK);
						/* PTP Locked Status(B25) ON */
						f_com_SVCtl(E_DU_SVCTL_OPE_SPLAN, D_DU_SVCTL_ON, D_DU_SVCTL_ON_PTP_STATE_LOCKED);
						ptpLockedFlag = ZL303XX_TRUE;
						if(f_ptpw_faultSuppressFlg == D_RRH_OFF){
							/* fault cease */
							f_com_SVCtl(E_DU_SVCTL_OPE_FID03_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_PTP_UNLOCK);
							bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_INFO,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
							"Fault Cease Fronthaul Synchronization Error(%d) ",*f_ptp_ns_flag);
						}
					}
				}else{
					f_ptpw_faultCeaseCount = 2;	/* keep */
				}
			}else{
				/* Check of "SFN FRM correction's flag"	*/
				if( *f_ptp_sfnfrm_flag == 1 ){

					bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_INFO,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
							"Starting 1PPS SFN load after 2sec(cur:%d) by PTP LOCK ...",*f_ptp_ns_flag);

					/* Set of "ns correction's flag"	*/
					*f_ptp_ns_flag = 2;						/* (ハード要望)1回飛ばすため1ではなく2を設定 */

				}
				f_ptpw_faultCeaseCount++;
			}
		}
		
		if(*state  != ZL303XX_NO_ACTIVE_SERVER ){
			/* 品質classが低下中の場合、強制HoldoverONとし、回復するのを待つ */
			/* 品質class回復後、強制HoldoverOffする	 */
			hwMode = fj_holdover_mode_get(zl303xx_Params0);
			if(hwMode != ZLS3077X_DPLL_MODE_MASK){
				(void)BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV16INF, 0x3, &syncE_ptp_ClockClass);
				if(f_ptpw_faultCeaseCount > 0){
					if(syncE_ptp_ClockClass != 0){
						/* SyncE or PTP annonce message内の品質class低下中 */
						if(hwMode != ZLS3077X_DPLL_MODE_HOLDOVER){
							fj_holdover_mode_set(zl303xx_Params0,ZLS3077X_DPLL_MODE_HOLDOVER);
							bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
									"Clock Class down(%s) sts:%d Mode(%d) -> Mode(HOLDOVER) SyncE:%d PTPAnnounce:%d link:0x%x", 
									zl303xx_AprCGUStateMachine_StateNameStr[*state & 0xF],status_holdover,
									hwMode,
									syncE_ptp_ClockClass & 1 ,((syncE_ptp_ClockClass & 2) >> 1) & 1,info_val);
						}else{
							/* already ZLS3077X_DPLL_MODE_HOLDOVER */
						}
					}else{
						/* SyncE or PTP annonce message内の品質class回復 */
						if(hwMode == ZLS3077X_DPLL_MODE_HOLDOVER){
							fj_holdover_mode_set(zl303xx_Params0,ZLS3077X_DPLL_MODE_REFLOCK);
								bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
								"Clock Class recover(%s) sts:%d Mode(HOLDOVER) -> Mode(REFLOCK) SyncE:%d PTPAnnounce:%d link:0x%x", 
								zl303xx_AprCGUStateMachine_StateNameStr[*state & 0xF],status_holdover,
								syncE_ptp_ClockClass & 1 ,((syncE_ptp_ClockClass & 2) >> 1) & 1 ,info_val);
						}
					}
				}
				else{
					if(hwMode == ZLS3077X_DPLL_MODE_HOLDOVER){
						/* L1 LINK down もしくは SyncE QSSM PRC かつ ptp announce clock class OKの場合、もとの状態に戻す */
						if( ( (info_val & D_RRH_ON) != 0 ) || ( syncE_ptp_ClockClass == 0 ) ){
							fj_holdover_mode_set(zl303xx_Params0,ZLS3077X_DPLL_MODE_REFLOCK);
							
							bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_SERIOUS,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
									"Clock Class recover(%s) sts:%d Mode(HOLDOVER) -> Mode(REFLOCK) SyncE:%d PTPAnnounce:%d link:0x%x", 
									zl303xx_AprCGUStateMachine_StateNameStr[*state & 0xF],status_holdover,
									syncE_ptp_ClockClass & 1 ,((syncE_ptp_ClockClass & 2) >> 1) & 1 ,info_val);
						}
					}
				}
			}
		}
	}else{
		bpf_com_plog_trace( ptplog_path  ,M_BPF_COM_PLOG_LEVEL(E_TRC_LV_CRITICAL,E_TRC_NO_PTPLOG)  , __FILE__ , __LINE__ ,
				"AprCGUStateMachine ERROR %d",status);
	}
	return status;
}
