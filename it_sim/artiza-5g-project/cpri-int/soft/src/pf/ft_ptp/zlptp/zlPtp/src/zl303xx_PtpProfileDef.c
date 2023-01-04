

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Interface for defining and accessing profile specific routines that need
*     to executed at various points in the PTP stack processing beyond the
*     standard behavior.
*
*******************************************************************************/

/****************   INCLUDE FILES   ******************************************/

#include "zl303xx_Error.h"
#include "zl303xx_PtpProfileDef.h"

/* Profile specific headers. */
#include "zl303xx_Ptpd.h"             /* Default Profile definitions (E2E & P2P). */
#include "zl303xx_PtpG8265p1Api.h"    /* Telecom Profile (Frequency) definitions. */
#include "zl303xx_PtpG8275p1Api.h"    /* Telecom Profile (Phase) definitions. */
#include "zl303xx_PtpC37p238Api.h"    /* Power Profile definitions. */
#include "zl303xx_Ptp802p1Api.h"      /* TSN Profile definitions. */

/****************   DEFINES     **********************************************/

/****************   TYPEDEFS    **********************************************/

/* Function definitions for all bindings associated with various profiles. */
/*****************/
/* Routine to get data about the Profile.  */
typedef zlStatusE (*zl303xx_PtpProfileInfoT)(
      const zl303xx_PtpProfileS **profileInfo);

/* Routine to take a single PTP Object (Clock, Port or Stream).  */
typedef zlStatusE (*zl303xx_PtpObjectDefaultT)(
      void *pPtpObjectCreateS);

/* Routine to take a PTP Object (Clock, Port or Stream) and its respective
 * parent (Node, Clock, Port).  */
typedef zlStatusE (*zl303xx_PtpObjectParentT)(
      void *pPtpObjectCreateS, void *pPtpObjectParentS);

/* Routine to do profile specific checks on received Announce messages.  */
typedef zlStatusE (*zl303xx_PtpAnncRxCheckFnT)(
      zl303xx_ClockStreamS *pPtpStream, zl303xx_PtpV2MsgS *pPtpMsgRx, Uint8T *pInTlvBuf);

/* Routine to do profile specific checks on Uint8T values.  */
typedef zlStatusE (*zl303xx_PtpCharCheckFnT)(
      Uint8T *uint8Tnumber, zl303xx_BooleanE strictFlag);

/* Routine to do profile specific checks on zl303xx_BooleanE values.  */
typedef zlStatusE (*zl303xx_PtpBooleanCheckFnT)(
      zl303xx_BooleanE *bBoolVal, zl303xx_BooleanE strictFlag);

/* Routine to do profile specific checks on the Clock Class.  */
typedef zlStatusE (*zl303xx_PtpClockQualityCheckFnT)(
      zl303xx_ClockQuality *clockQuality, zl303xx_BooleanE strictFlag);

/* Routine to do profile specific checks on the Clock Priority Values.  */
typedef zlStatusE (*zl303xx_PtpClockPriorityCheckFnT)(
      Uint8T *priority1, Uint8T *priority2,
      zl303xx_BooleanE slaveOnly, zl303xx_BooleanE strictFlag);

/* Prototype to do range checks on Interval values (packet, query intervals).  */
typedef zlStatusE (*zl303xx_PtpIntervalCheckFnT)(
      Sint8T *interval, zl303xx_BooleanE strictFlag);

/* Routine to check an interval for a messageType in a profile.  */
typedef zlStatusE (*zl303xx_PtpMsgIntervalCheckFnT)(
      Uint32T messageType, Sint8T *log2MsgInterval, zl303xx_BooleanE strictFlag);

/* Routine to do profile specific checks on the Unicast Contract Duration.  */
typedef zlStatusE (*zl303xx_PtpUnicastDurationCheckFnT)(
      Uint32T *duration, zl303xx_BooleanE strictFlag);

/* Routine to extract BMCA data from a Foreign Master Table entry.  */
typedef zlStatusE (*zl303xx_PtpBmcaCompareDataAddFnT)(
      zl303xx_ForeignMasterRecordS *pFmtEntry, zl303xx_PtpEventClockBmcaEntryS *pBmcaDataS);

/* Routine to perform BMCA comparison between 2 Profile data sets.  */
typedef Sint32T (*zl303xx_PtpBmcaCompareFnT)(
      zl303xx_PtpEventClockBmcaEntryS *pCompDataA,
      zl303xx_PtpEventClockBmcaEntryS *pCompDataB,
      void *pExtraData);

/* Generic routine to do profile specific actions on Clock Objects.  */
typedef zlStatusE (*zl303xx_PtpClockParamHandleFnT)(
      zl303xx_PtpClockS *pPtpClock, void *pInParamS);

/* Generic routine to do profile specific actions on Port Objects.  */
typedef zlStatusE (*zl303xx_PtpPortParamHandleFnT)(
      zl303xx_PortDataS *pPtpPort, void *pInParamS);

/* Generic routine to do profile specific actions on Stream Objects.  */
typedef zlStatusE (*zl303xx_PtpStreamParamHandleFnT)(
      zl303xx_ClockStreamS *pPtpStream, void *pInParamS);


/* Enum to list the available profile binding functions. This may not be used
 * anywhere but it is good practice to add a definition each time a new routine
 * is integrated just so users are aware of what actions are available. */
typedef enum
{
   /* Routine to get data about the Profile.  */
   ZL303XX_PTP_PROFILE_FN_PROFILE_INFO = 0,

   /* Routines to set profile specific default values for PTP Objects.  */
   ZL303XX_PTP_PROFILE_FN_CLOCK_DEFAULTS_SET,
   ZL303XX_PTP_PROFILE_FN_PORT_DEFAULTS_SET,
   ZL303XX_PTP_PROFILE_FN_STREAM_DEFAULTS_SET,

   /* Routines to perform profile specific setup actions after a PTP Object
    * is created.  */
   ZL303XX_PTP_PROFILE_FN_CLOCK_POST_CREATE_ACTION,
   ZL303XX_PTP_PROFILE_FN_PORT_POST_CREATE_ACTION,
   ZL303XX_PTP_PROFILE_FN_STREAM_POST_CREATE_ACTION,

   /* Routines to perform profile specific Data Set updates for various PTP Objects */
   ZL303XX_PTP_PROFILE_FN_CLOCK_PROFILE_DS_UPDATE,
   ZL303XX_PTP_PROFILE_FN_PORT_PROFILE_DS_UPDATE,
   ZL303XX_PTP_PROFILE_FN_STREAM_PROFILE_DS_UPDATE,

   /* Routines to perform profile specific Data Checks on various PTP parameters. */
   ZL303XX_PTP_PROFILE_FN_DOMAIN_NUMBER_CHECK,
   ZL303XX_PTP_PROFILE_FN_CLOCK_CLASS_CHECK,
   ZL303XX_PTP_PROFILE_FN_CLOCK_PRIORITY_CHECK,
   ZL303XX_PTP_PROFILE_FN_ANNC_RX_TIMEOUT_CHECK,
   ZL303XX_PTP_PROFILE_FN_CONTRACT_DURATION_CHECK,

   /* Routine to do profile specific checks on received (ingress) Announce messages.  */
   ZL303XX_PTP_PROFILE_FN_ANNC_RX_CHECK,

   /* Routines to update Announce message fields of transmitted (egress) packets.  */
   ZL303XX_PTP_PROFILE_FN_ANNC_TX_HDR_UPDATE,
   ZL303XX_PTP_PROFILE_FN_ANNC_TX_DATA_UPDATE,

   /* Routines to perform profile specific Checks on various Timer or periodic
    * Intervals. */
   ZL303XX_PTP_PROFILE_FN_INTERVAL_UNICAST_QUERY_CHECK,

   /* Routines to perform profile specific Checks on PTP Message Intervals. */
   ZL303XX_PTP_PROFILE_FN_MSG_INTERVAL_CHECK,

   /* Routines to add Profile specific data to the BMCA Comparison Structure.  */
   ZL303XX_PTP_PROFILE_FN_BMCA_DATA_ADD,       /* Stream or Clock */
   ZL303XX_PTP_PROFILE_FN_BMCA_VP_DATA_ADD,    /* Virtual Port */

   /* Routine to perform the BMC Comparison Algorithm for the Profile.  */
   ZL303XX_PTP_PROFILE_FN_BMCA_EXECUTE,

   /* Routine to append a TLV to the end of a transmitted general message.  */
   ZL303XX_PTP_PROFILE_FN_TLV_APPEND,

   /* Routines to execute profile specific debug routines for PTP Objects.  */
   ZL303XX_PTP_PROFILE_FN_DEBUG_CLOCK_SHOW,
   ZL303XX_PTP_PROFILE_FN_DEBUG_PORT_SHOW,
   ZL303XX_PTP_PROFILE_FN_DEBUG_STREAM_SHOW
} zl303xx_PtpProfileBindingsE;


/* Function bindings for accessing profile specific data or executing actions. */
typedef struct
{
   /* Short description (for logging, etc. ) */
   const char *profileDesc;

   /* Profile ID (according to zl303xx_PtpProfileE). */
   zl303xx_PtpProfileE profileId;

   /* Routine to get data about the Profile.
    * This binding should be defined for all profiles. */
   zl303xx_PtpProfileInfoT profileInfoFn;

   /* Routines to set profile specific default values for PTP Objects.  */
   struct
   {
      zl303xx_PtpObjectDefaultT clock;
      zl303xx_PtpObjectDefaultT port;
      zl303xx_PtpObjectDefaultT stream;
   } defaultsSetFn;

   /* Routines to perform profile specific actions prior to a PTP Object being
    * created. These are typically final checks or backwards compatibility
    * actions required for consistency. */
   struct
   {
      zl303xx_PtpObjectParentT clock;
      zl303xx_PtpObjectParentT port;
      zl303xx_PtpObjectParentT stream;
   } preCreateActionFn;

   /* Routines to perform profile specific setup actions after a PTP Object is
    * created.  */
   struct
   {
      zl303xx_PtpObjectDefaultT clock;
      zl303xx_PtpObjectDefaultT port;
      zl303xx_PtpObjectDefaultT stream;
   } postCreateActionFn;

   /* Routines to update profile specific data sets for PTP Objects. */
   struct
   {
      zl303xx_PtpObjectDefaultT clock;
      zl303xx_PtpObjectDefaultT port;
      zl303xx_PtpObjectDefaultT stream;
   } profileDsUpdateFn;

   /* Routine to do profile specific checks on domain numbers. If the binded
    * routine is NULL then the defined MIN & MAX values are used to check the
    * range. The DEFAULT is set if strict = TRUE.*/
   struct
   {
      /* Custom, profile specific routine. If NULL then the defined values below
       * are used to do an automatic check.    */
      zl303xx_PtpCharCheckFnT rangeCheckFn;

      Uint8T min;
      Uint8T max;
      Uint8T def;
   } domain;

   /* Routine to do profile specific checks on the transportSpecific field.
    * (If NULL, then ignored).*/
   zl303xx_PtpCharCheckFnT transportSpecificCheckFn;

   /* Routine to do profile specific checks on clock quality values.
    * (If NULL, then the DEFAULT Profile check is performed).*/
   zl303xx_PtpClockQualityCheckFnT clockQualityCheckFn;

   /* Routine to do profile specific checks on priority1 and priority1 values.
    * (If NULL, then the DEFAULT Profile check is performed).*/
   zl303xx_PtpClockPriorityCheckFnT clockPriorityCheckFn;

   /* Routine to do profile specific checks on Announce Receipt Timeout values.
    * If the binded routine is NULL then the defined MIN & MAX values are used
    * to check the range. The DEFAULT is set if strict = TRUE.*/
   struct
   {
      /* Custom, profile specific routine. If NULL then the defined values below
       * are used to do an automatic check.    */
      zl303xx_PtpCharCheckFnT rangeCheckFn;

      Uint8T min;
      Uint8T max;
      Uint8T def;
   } anncRxTimeout;

   /* Routine to do profile specific checks on Stream::mode values. */
   zl303xx_PtpStreamParamHandleFnT streamModeCheckFn;
   /* Routine to do profile specific checks on Port::masterOnly values. */
   zl303xx_PtpPortParamHandleFnT portMasterOnlyCheckFn;

   /* Unicast Negotiation Related Checks */
   struct
   {
      /* Routine to do profile specific checks on Unicast Contract Durations. */
      zl303xx_PtpUnicastDurationCheckFnT contractDurationCheckFn;

      /* Routine to check if a Unicast Grant is allowed on a specified Port. */
      zl303xx_PtpClockParamHandleFnT grantDenyCheckFn;
   } unicastNeg;

   /* Routine to set any profile specific fields in a transmitted PTP message
    * header. */
   zl303xx_PtpStreamParamHandleFnT egressPktHeaderGatherFn;

   /* Routines to handle various aspects of Announce message transmission,
    * receiving and handling.  */
   struct
   {
      /* Routine to do profile specific checks on received Announce messages. */
      zl303xx_PtpAnncRxCheckFnT ingressMsgCheckFn;

      /* Gathers profile specific data for the egress Announce message. */
      zl303xx_PtpStreamParamHandleFnT egressDataGatherFn;
   } announce;

   /* Routines to check various packet / query intervals.  */
   struct
   {
      /* Unicast Query Interval */
      zl303xx_PtpIntervalCheckFnT unicastQuery;

      /* PTP Message Intervals (i.e. PPS Rates). */
      zl303xx_PtpMsgIntervalCheckFnT msgInterval;
   } intvlCheckFn;

   /* BMC Algorithm related routines.  */
   struct
   {
      /* Function to add Profile specific data to the BMCA Comparison Structure. */
      zl303xx_PtpBmcaCompareDataAddFnT dataAddFn;

      /* Function to add Profile specific data to the BMCA Comparison Structure
       * for Virtual PTP Ports.  */
      zl303xx_PtpPortParamHandleFnT virtualPortDataAddFn;

      /* Function to perform the BMCA comparison for the Profile. */
      zl303xx_PtpBmcaCompareFnT comparisonFn;
   } bmca;

   /** Function called to append any extra TLVs to the end of a message
    * (common for Announce messages in some profiles).
    *  This binding is optional and nothing is appended if NULL. */
   zl303xx_PtpTlvAppendFnT tlvAppendFn;

   /* Routines to execute profile specific debug routines for PTP Objects. */
   struct
   {
      zl303xx_PtpObjectDefaultT clockData;
      zl303xx_PtpObjectDefaultT portData;
      zl303xx_PtpObjectDefaultT streamData;
   } debugShowFn;

} zl303xx_PtpProfileDefS;

/****************   STATIC FUNCTION DECLARATIONS   ***************************/

/****************   STATIC GLOBAL VARIABLES   ********************************/

zl303xx_PtpProfileDefS ptpProfileFn[ZL303XX_PTP_PROFILE_NUM_TYPES] = {
      {  "Default Profile (E2E)",
         ZL303XX_PTP_PROFILE_DEFAULT_E2E,
         zl303xx_PtpDefaultProfileE2EInfoGet,        /* profileInfoFn */

         /* DEFAULTS SET ROUTINES */
         {
            NULL,                                  /* defaultsSetFn.clock */
            NULL,                                  /* defaultsSetFn.port */
            NULL,                                  /* defaultsSetFn.stream */
         },

         /* PRE PTP OBJECT CREATE ROUTINES */
         {
            NULL,                                  /* preCreateActionFn.clock */
            NULL,                                  /* preCreateActionFn.port */
            NULL,                                  /* preCreateActionFn.stream */
         },

         /* POST PTP OBJECT CREATE ROUTINES */
         {
            NULL,                                  /* postCreateActionFn.clock */
            NULL,                                  /* postCreateActionFn.port */
            NULL,                                  /* postCreateActionFn.stream */
         },

         /* PROFILE DS UPDATE ROUTINES */
         {
            NULL,                                  /* profileDsUpdateFn.clock */
            NULL,                                  /* profileDsUpdateFn.port */
            NULL,                                  /* profileDsUpdateFn.stream */
         },

         /* DOMIAN Checks */
         {
            NULL,                                  /* rangeCheckFn */
            PTP_CLOCK_DOMAIN_NUMBER_MIN,           /* MIN */
            PTP_CLOCK_DOMAIN_NUMBER_MAX,           /* MAX */
            ZL303XX_PTP_DEFAULT_CLOCK_DOMAIN_NUM     /* DEFAULT */
         },

         NULL,                                     /* transportSpecificCheckFn */
         NULL,  /* Full Range 0 - 255 allowed */   /* clockQualityCheckFn */
         NULL,  /* Full Range 0 - 255 allowed */   /* clockPriorityCheckFn */

         /* ANNOUNCE RX TIMEOUT Checks */
         {
            NULL,                                  /* rangeCheckFn */
            PTP_CLOCK_ANNC_RX_TIMEOUT_MIN,         /* MIN */
            PTP_CLOCK_ANNC_RX_TIMEOUT_MAX,         /* MAX */
            PTP_CLOCK_ANNC_RX_TIMEOUT_DEF          /* DEFAULT */
         },

         NULL,                                     /* streamModeCheckFn */
         NULL,                                     /* portMasterOnlyCheckFn */
         {
            zl303xx_PtpDefaultProfileDurationCheck,  /* contractDurationCheckFn */
            NULL                                   /* grantDenyCheckFn */
         },

         NULL,                                     /* egressPktHeaderGatherFn */

         {
            NULL,                                  /* ingressMsgCheckFn */
            NULL                                   /* egressDataGatherFn */
         },
         {
            NULL,                                  /* intvlCheckFn.unicastQuery */
            NULL                                   /* intvlCheckFn.msgInterval */
         },
         {
            NULL,                                  /* bmca.dataAddFn */
            NULL,                                  /* bmca.virtualPortDataAddFn */
            NULL                                   /* bmca.comparisonFn */
         },
         NULL,                                     /* tlvAppendFn */

         /* DEBUG SHOW ROUTINES */
         {
            NULL,                                  /* debugShowFn.clockData */
            NULL,                                  /* debugShowFn.portData */
            NULL                                   /* debugShowFn.streamData */
         }
      },
      {  "Default Profile (P2P)",
         ZL303XX_PTP_PROFILE_DEFAULT_P2P,
         zl303xx_PtpDefaultProfileP2PInfoGet,        /* profileInfoFn */

         /* DEFAULTS SET ROUTINES */
         {
            NULL,                                  /* defaultsSetFn.clock */
            NULL,                                  /* defaultsSetFn.port */
            NULL,                                  /* defaultsSetFn.stream */
         },

         /* PRE PTP OBJECT CREATE ROUTINES */
         {
            NULL,                                  /* preCreateActionFn.clock */
            NULL,                                  /* preCreateActionFn.port */
            NULL,                                  /* preCreateActionFn.stream */
         },

         /* POST PTP OBJECT CREATE ROUTINES */
         {
            NULL,                                  /* postCreateActionFn.clock */
            NULL,                                  /* postCreateActionFn.port */
            NULL,                                  /* postCreateActionFn.stream */
         },

         /* POST PTP OBJECT CREATE ROUTINES */
         {
            NULL,                                  /* profileDsUpdateFn.clock */
            NULL,                                  /* profileDsUpdateFn.port */
            NULL,                                  /* profileDsUpdateFn.stream */
         },

         /* DOMIAN Checks */
         {
            NULL,                                  /* rangeCheckFn */
            PTP_CLOCK_DOMAIN_NUMBER_MIN,           /* MIN */
            PTP_CLOCK_DOMAIN_NUMBER_MAX,           /* MAX */
            ZL303XX_PTP_DEFAULT_CLOCK_DOMAIN_NUM     /* DEFAULT */
         },

         NULL,                                     /* transportSpecificCheckFn */
         NULL,  /* Full Range 0 - 255 allowed */   /* clockQualityCheckFn */
         NULL,  /* Full Range 0 - 255 allowed */   /* clockPriorityCheckFn */

         /* ANNOUNCE RX TIMEOUT Checks */
         {
            NULL,                                  /* rangeCheckFn */
            PTP_CLOCK_ANNC_RX_TIMEOUT_MIN,         /* MIN */
            PTP_CLOCK_ANNC_RX_TIMEOUT_MAX,         /* MAX */
            PTP_CLOCK_ANNC_RX_TIMEOUT_DEF          /* DEFAULT */
         },

         NULL,                                     /* streamModeCheckFn */
         NULL,                                     /* portMasterOnlyCheckFn */
         {
            zl303xx_PtpDefaultProfileDurationCheck,  /* contractDurationCheckFn */
            NULL                                   /* grantDenyCheckFn */
         },

         NULL,                                     /* egressPktHeaderGatherFn */

         {
            NULL,                                  /* ingressMsgCheckFn */
            NULL                                   /* egressDataGatherFn */
         },
         {
            NULL,                                  /* intvlCheckFn.unicastQuery */
            NULL                                   /* intvlCheckFn.msgInterval */
         },
         {
            NULL,                                  /* bmca.dataAddFn */
            NULL,                                  /* bmca.virtualPortDataAddFn */
            NULL                                   /* bmca.comparisonFn */
         },
         NULL,                                     /* tlvAppendFn */

         /* DEBUG SHOW ROUTINES */
         {
            NULL,                                  /* debugShowFn.clockData */
            NULL,                                  /* debugShowFn.portData */
            NULL                                   /* debugShowFn.streamData */
         }
      },
      {  "Telecom Profile (G.8265.1)",
         ZL303XX_PTP_PROFILE_TELECOM_G8265_1,
         zl303xx_PtpG8265p1ProfileInfoGet,           /* profileInfoFn */

         /* DEFAULTS SET ROUTINES */
         {
            NULL,                                  /* defaultsSetFn.clock */
            NULL,                                  /* defaultsSetFn.port */
            zl303xx_PtpG8265p1StreamDefaults,        /* defaultsSetFn.stream */
         },

         /* PRE PTP OBJECT CREATE ROUTINES */
         {
            NULL,                                  /* preCreateActionFn.clock */
            NULL,                                  /* preCreateActionFn.port */
            NULL,                                  /* preCreateActionFn.stream */
         },

         /* POST PTP OBJECT CREATE ROUTINES */
         {
            zl303xx_PtpG8265p1ClockCreateActions,    /* postCreateActionFn.clock */
            NULL,                                  /* postCreateActionFn.port */
            NULL,                                  /* postCreateActionFn.stream */
         },

         /* POST PTP OBJECT CREATE ROUTINES */
         {
            zl303xx_PtpG8265p1ClockQLUpdate,         /* profileDsUpdateFn.clock */
            NULL,                                  /* profileDsUpdateFn.port */
            NULL,                                  /* profileDsUpdateFn.stream */
         },

         /* DOMIAN Checks */
         {
            NULL,                                  /* rangeCheckFn */
            ZL303XX_PTP_TELECOM_DOMAIN_NUM_MIN,      /* MIN */
            ZL303XX_PTP_TELECOM_DOMAIN_NUM_MAX,      /* MAX */
            ZL303XX_PTP_TELECOM_DOMAIN_NUM_DEF       /* DEFAULT */
         },

         zl303xx_PtpG8265p1TransportSpecificCheck,   /* transportSpecificCheckFn */
         zl303xx_PtpG8265p1ClockQualityCheck,        /* clockQualityCheckFn */
         NULL,  /* Full Range 0 - 255 allowed */   /* clockPriorityCheckFn */

         /* ANNOUNCE RX TIMEOUT Checks */
         {
            NULL,                                  /* rangeCheckFn */
            PTP_CLOCK_ANNC_RX_TIMEOUT_MIN,         /* MIN */
            PTP_CLOCK_ANNC_RX_TIMEOUT_MAX,         /* MAX */
            PTP_CLOCK_ANNC_RX_TIMEOUT_DEF          /* DEFAULT */
         },

         NULL,                                     /* streamModeCheckFn */
         NULL,                                     /* portMasterOnlyCheckFn */
         {
            zl303xx_PtpG8265p1UniNegDurationCheck,   /* contractDurationCheckFn */
            NULL                                   /* grantDenyCheckFn */
         },

         NULL,                                     /* egressPktHeaderGatherFn */

         {
            NULL,                                  /* ingressMsgCheckFn */
            NULL                                   /* egressDataGatherFn */
         },
         {
            zl303xx_PtpG8265p1QueryIntervalCheck,    /* intvlCheckFn.unicastQuery */
            zl303xx_PtpTelecomLogIntvlCheck          /* intvlCheckFn.msgInterval */
         },
         {
            zl303xx_PtpG8265p1BmcaDataAdd,           /* bmca.dataAddFn */
            zl303xx_PtpG8265p1BmcaVpDataAdd,         /* bmca.virtualPortDataAddFn */
            NULL                                   /* bmca.comparisonFn */
         },
         NULL,                                     /* tlvAppendFn */

         /* DEBUG SHOW ROUTINES */
         {
            zl303xx_PtpG8265p1ClockDebugShow,        /* debugShowFn.clockData */
            NULL,                                  /* debugShowFn.portData */
            NULL                                   /* debugShowFn.streamData */
         }
      },
      {  "Telecom Profile (G.8275.1 v1)",
         ZL303XX_PTP_PROFILE_TELECOM_G8275_1v1,
         zl303xx_PtpG8275p1v1ProfileInfoGet,         /* profileInfoFn */

         /* DEFAULTS SET ROUTINES */
         {
            zl303xx_PtpG8275p1ClockDefaults,         /* defaultsSetFn.clock */
            zl303xx_PtpG8275p1PortDefaults,          /* defaultsSetFn.port */
            zl303xx_PtpG8275p1StreamDefaults,        /* defaultsSetFn.stream */
         },

         /* PRE PTP OBJECT CREATE ROUTINES */
         {
            NULL,                                  /* preCreateActionFn.clock */
            zl303xx_PtpG8275PortPreCreate,           /* preCreateActionFn.port */
            NULL,                                  /* preCreateActionFn.stream */
         },

         /* POST PTP OBJECT CREATE ROUTINES */
         {
            zl303xx_PtpG8275ClockCreateActions,      /* postCreateActionFn.clock */
            NULL,                                  /* postCreateActionFn.port */
            NULL,                                  /* postCreateActionFn.stream */
         },

         /* POST PTP OBJECT CREATE ROUTINES */
         {
            zl303xx_PtpG8275ClockDataUpdate,         /* profileDsUpdateFn.clock */
            NULL,                                  /* profileDsUpdateFn.port */
            NULL,                                  /* profileDsUpdateFn.stream */
         },

         /* DOMIAN Checks */
         {
            NULL,                                  /* rangeCheckFn */
            ZL303XX_PTP_G8275p1_DOMAIN_NUM_MIN,      /* MIN */
            ZL303XX_PTP_G8275p1_DOMAIN_NUM_MAX,      /* MAX */
            ZL303XX_PTP_G8275p1_DOMAIN_NUM_DEF       /* DEFAULT */
         },

         zl303xx_PtpG8275TransportSpecificCheck,     /* transportSpecificCheckFn */
         zl303xx_PtpG8275p1v1ClockQualityCheck,      /* clockQualityCheckFn */
         zl303xx_PtpG8275PriorityCheck,              /* clockPriorityCheckFn */

         /* ANNOUNCE RX TIMEOUT Checks */
         {
            NULL,                                  /* rangeCheckFn */
            ZL303XX_PTP_G8275p1_ANNC_TIMEOUT_MIN,    /* MIN */
            ZL303XX_PTP_G8275p1_ANNC_TIMEOUT_MAX,    /* MAX */
            ZL303XX_PTP_G8275p1_ANNC_TIMEOUT_DEF     /* DEFAULT */
         },

         zl303xx_PtpG8275StreamModeCheck,            /* streamModeCheckFn */
         zl303xx_PtpG8275PortMasterOnlyCheck,        /* portMasterOnlyCheckFn */
         {
            NULL,                                  /* contractDurationCheckFn */
            zl303xx_PtpG8275GrantDenyCheck     	   /* grantDenyCheckFn */
         },

         zl303xx_PtpG8275PtpMsgHeaderGet,            /* egressPktHeaderGatherFn */

         {
            zl303xx_PtpG8275AnncRxCheck,             /* ingressMsgCheckFn */
            zl303xx_PtpG8275AnncDataGet              /* egressDataGatherFn */
         },
         {
            NULL,                                  /* intvlCheckFn.unicastQuery */
            zl303xx_PtpG8275p1MsgIntervalCheck       /* intvlCheckFn.msgInterval */
         },
         {
            zl303xx_PtpG8275BmcaDataAdd,             /* bmca.dataAddFn */
            zl303xx_PtpG8275BmcaVpDataAdd,           /* bmca.virtualPortDataAddFn */
            NULL                                   /* bmca.comparisonFn */
         },
         zl303xx_PtpG8275TlvAppend,                  /* tlvAppendFn */

         /* DEBUG SHOW ROUTINES */
         {
            zl303xx_PtpG8275ClockDebugShow,          /* debugShowFn.clockData */
            zl303xx_PtpG8275PortDebugShow,           /* debugShowFn.portData */
            zl303xx_PtpG8275StreamDebugShow          /* debugShowFn.streamData */
         }
      },
      {  "Telecom Profile (G.8275.1 v2)",
         ZL303XX_PTP_PROFILE_TELECOM_G8275_1v2,
         zl303xx_PtpG8275p1v2ProfileInfoGet,         /* profileInfoFn */

         /* DEFAULTS SET ROUTINES */
         {
            zl303xx_PtpG8275p1ClockDefaults,         /* defaultsSetFn.clock */
            zl303xx_PtpG8275p1PortDefaults,          /* defaultsSetFn.port */
            zl303xx_PtpG8275p1StreamDefaults,        /* defaultsSetFn.stream */
         },

         /* PRE PTP OBJECT CREATE ROUTINES */
         {
            NULL,                                  /* preCreateActionFn.clock */
            zl303xx_PtpG8275PortPreCreate,           /* preCreateActionFn.port */
            NULL,                                  /* preCreateActionFn.stream */
         },

         /* POST PTP OBJECT CREATE ROUTINES */
         {
            zl303xx_PtpG8275ClockCreateActions,      /* postCreateActionFn.clock */
            NULL,                                  /* postCreateActionFn.port */
            NULL,                                  /* postCreateActionFn.stream */
         },

         /* POST PTP OBJECT CREATE ROUTINES */
         {
            zl303xx_PtpG8275ClockDataUpdate,         /* profileDsUpdateFn.clock */
            NULL,                                  /* profileDsUpdateFn.port */
            NULL,                                  /* profileDsUpdateFn.stream */
         },

         /* DOMIAN Checks */
         {
            NULL,                                  /* rangeCheckFn */
            ZL303XX_PTP_G8275p1_DOMAIN_NUM_MIN,      /* MIN */
            ZL303XX_PTP_G8275p1_DOMAIN_NUM_MAX,      /* MAX */
            ZL303XX_PTP_G8275p1_DOMAIN_NUM_DEF       /* DEFAULT */
         },

         zl303xx_PtpG8275TransportSpecificCheck,     /* transportSpecificCheckFn */
         NULL,                                     /* clockQualityCheckFn */
         zl303xx_PtpG8275PriorityCheck,              /* clockPriorityCheckFn */

         /* ANNOUNCE RX TIMEOUT Checks */
         {
            NULL,                                  /* rangeCheckFn */
            ZL303XX_PTP_G8275p1_ANNC_TIMEOUT_MIN,    /* MIN */
            ZL303XX_PTP_G8275p1_ANNC_TIMEOUT_MAX,    /* MAX */
            ZL303XX_PTP_G8275p1_ANNC_TIMEOUT_DEF     /* DEFAULT */
         },

         zl303xx_PtpG8275StreamModeCheck,            /* streamModeCheckFn */
         zl303xx_PtpG8275PortMasterOnlyCheck,        /* portMasterOnlyCheckFn */
         {
            NULL,                                  /* contractDurationCheckFn */
            zl303xx_PtpG8275GrantDenyCheck           /* grantDenyCheckFn */
         },

         zl303xx_PtpG8275PtpMsgHeaderGet,            /* egressPktHeaderGatherFn */

         {
            zl303xx_PtpG8275AnncRxCheck,             /* ingressMsgCheckFn */
            zl303xx_PtpG8275AnncDataGet              /* egressDataGatherFn */
         },
         {
            NULL,                                  /* intvlCheckFn.unicastQuery */
            zl303xx_PtpG8275p1MsgIntervalCheck       /* intvlCheckFn.msgInterval */
         },
         {
            zl303xx_PtpG8275BmcaDataAdd,             /* bmca.dataAddFn */
            zl303xx_PtpG8275BmcaVpDataAdd,           /* bmca.virtualPortDataAddFn */
            NULL                                   /* bmca.comparisonFn */
         },
         zl303xx_PtpG8275TlvAppend,                  /* tlvAppendFn */

         /* DEBUG SHOW ROUTINES */
         {
            zl303xx_PtpG8275ClockDebugShow,          /* debugShowFn.clockData */
            zl303xx_PtpG8275PortDebugShow,           /* debugShowFn.portData */
            zl303xx_PtpG8275StreamDebugShow          /* debugShowFn.streamData */
         }
      },
      {  "Telecom Profile (G.8275.2)",
         ZL303XX_PTP_PROFILE_TELECOM_G8275_2,
         zl303xx_PtpG8275p2ProfileInfoGet,           /* profileInfoFn */

         /* DEFAULTS SET ROUTINES */
         {
            zl303xx_PtpG8275p2ClockDefaults,         /* defaultsSetFn.clock */
            zl303xx_PtpG8275p2PortDefaults,          /* defaultsSetFn.port */
            zl303xx_PtpG8275p2StreamDefaults,        /* defaultsSetFn.stream */
         },

         /* PRE PTP OBJECT CREATE ROUTINES */
         {
            NULL,                                  /* preCreateActionFn.clock */
            zl303xx_PtpG8275PortPreCreate,           /* preCreateActionFn.port */
            NULL,                                  /* preCreateActionFn.stream */
         },

         /* POST PTP OBJECT CREATE ROUTINES */
         {
            zl303xx_PtpG8275ClockCreateActions,      /* postCreateActionFn.clock */
            NULL,                                  /* postCreateActionFn.port */
            NULL,                                  /* postCreateActionFn.stream */
         },

         /* POST PTP OBJECT CREATE ROUTINES */
         {
            zl303xx_PtpG8275ClockDataUpdate,         /* profileDsUpdateFn.clock */
            NULL,                                  /* profileDsUpdateFn.port */
            NULL,                                  /* profileDsUpdateFn.stream */
         },

         /* DOMIAN Checks */
         {
            NULL,                                  /* rangeCheckFn */
            ZL303XX_PTP_G8275p2_DOMAIN_NUM_MIN,      /* MIN */
            ZL303XX_PTP_G8275p2_DOMAIN_NUM_MAX,      /* MAX */
            ZL303XX_PTP_G8275p2_DOMAIN_NUM_DEF       /* DEFAULT */
         },

         zl303xx_PtpG8275TransportSpecificCheck,     /* transportSpecificCheckFn */
         NULL,                                     /* clockQualityCheckFn */
         zl303xx_PtpG8275PriorityCheck,              /* clockPriorityCheckFn */

         /* ANNOUNCE RX TIMEOUT Checks */
         {
            NULL,                                  /* rangeCheckFn */
            ZL303XX_PTP_G8275p2_ANNC_TIMEOUT_MIN,    /* MIN */
            ZL303XX_PTP_G8275p2_ANNC_TIMEOUT_MAX,    /* MAX */
            ZL303XX_PTP_G8275p2_ANNC_TIMEOUT_DEF     /* DEFAULT */
         },

         zl303xx_PtpG8275StreamModeCheck,            /* streamModeCheckFn */
         zl303xx_PtpG8275PortMasterOnlyCheck,        /* portMasterOnlyCheckFn */
         {
            NULL,                                  /* contractDurationCheckFn */
            zl303xx_PtpG8275GrantDenyCheck           /* grantDenyCheckFn */
         },

         zl303xx_PtpG8275PtpMsgHeaderGet,            /* egressPktHeaderGatherFn */

         {
            zl303xx_PtpG8275AnncRxCheck,             /* ingressMsgCheckFn */
            zl303xx_PtpG8275AnncDataGet              /* egressDataGatherFn */
         },
         {
            NULL,                                  /* intvlCheckFn.unicastQuery */
            zl303xx_PtpG8275p2MsgIntervalCheck       /* intvlCheckFn.msgInterval */
         },
         {
            zl303xx_PtpG8275BmcaDataAdd,             /* bmca.dataAddFn */
            zl303xx_PtpG8275BmcaVpDataAdd,           /* bmca.virtualPortDataAddFn */
            NULL                                   /* bmca.comparisonFn */
         },
         zl303xx_PtpG8275TlvAppend,                  /* tlvAppendFn */

         /* DEBUG SHOW ROUTINES */
         {
            zl303xx_PtpG8275ClockDebugShow,          /* debugShowFn.clockData */
            zl303xx_PtpG8275PortDebugShow,           /* debugShowFn.portData */
            zl303xx_PtpG8275StreamDebugShow          /* debugShowFn.streamData */
         }
      },
      {  "Power Profile (C.37.238-2011)",
         ZL303XX_PTP_PROFILE_POWER_C37_238_2011,
         zl303xx_PtpC37p238v1ProfileInfoGet,         /* profileInfoFn */

         /* DEFAULTS SET ROUTINES */
         {
            zl303xx_PtpC37p238v1ClockDefaults,       /* defaultsSetFn.clock */
            zl303xx_PtpC37p238v1PortDefaults,        /* defaultsSetFn.port */
            zl303xx_PtpC37p238v1StreamDefaults,      /* defaultsSetFn.stream */
         },

         /* PRE PTP OBJECT CREATE ROUTINES */
         {
            NULL,                                  /* preCreateActionFn.clock */
            NULL,                                  /* preCreateActionFn.port */
            NULL,                                  /* preCreateActionFn.stream */
         },

         /* POST PTP OBJECT CREATE ROUTINES */
         {
            zl303xx_PtpC37p238ClockCreateActions,    /* postCreateActionFn.clock */
            NULL,                                  /* postCreateActionFn.port */
            NULL,                                  /* postCreateActionFn.stream */
         },

         /* POST PTP OBJECT CREATE ROUTINES */
         {
            NULL,                                  /* profileDsUpdateFn.clock */
            NULL,                                  /* profileDsUpdateFn.port */
            NULL,                                  /* profileDsUpdateFn.stream */
         },

         /* DOMIAN Checks */
         {
            NULL,                                  /* rangeCheckFn */
            PTP_CLOCK_DOMAIN_NUMBER_MIN,           /* MIN */
            PTP_CLOCK_DOMAIN_NUMBER_MAX,           /* MAX */
            DEFAULT_C37_238_2011_DOMAIN            /* DEFAULT */
         },

         NULL,                                     /* transportSpecificCheckFn */
         NULL,  /* Full Range 0 - 255 allowed */   /* clockQualityCheckFn */
         NULL,  /* Full Range 0 - 255 allowed */   /* clockPriorityCheckFn */

         /* ANNOUNCE RX TIMEOUT Checks */
         {
            NULL,                                  /* rangeCheckFn */
            PTP_CLOCK_ANNC_RX_TIMEOUT_MIN,         /* MIN */
            PTP_CLOCK_ANNC_RX_TIMEOUT_MAX,         /* MAX */
            PTP_CLOCK_ANNC_RX_TIMEOUT_DEF          /* DEFAULT */
         },

         NULL,                                     /* streamModeCheckFn */
         NULL,                                     /* portMasterOnlyCheckFn */
         {
            NULL,                                  /* contractDurationCheckFn */
            NULL                                   /* grantDenyCheckFn */
         },

         NULL,                                     /* egressPktHeaderGatherFn */

         {
            NULL,                                  /* ingressMsgCheckFn */
            NULL                                   /* egressDataGatherFn */
         },
         {
            NULL,                                  /* intvlCheckFn.unicastQuery */
            NULL                                   /* intvlCheckFn.msgInterval */
         },
         {
            NULL,                                  /* bmca.dataAddFn */
            NULL,                                  /* bmca.virtualPortDataAddFn */
            NULL                                   /* bmca.comparisonFn */
         },
         zl303xx_PtpC37p238TlvAppend,                /* tlvAppendFn */

         /* DEBUG SHOW ROUTINES */
         {
            zl303xx_PtpC37p238ClockDebugShow,        /* debugShowFn.clockData */
            zl303xx_PtpC37p238PortDebugShow,         /* debugShowFn.portData */
            zl303xx_PtpC37p238StreamDebugShow        /* debugShowFn.streamData */
         }
      },

      {  "Power Profile (C.37.238-2014)",
         ZL303XX_PTP_PROFILE_POWER_C37_238_2014,
         zl303xx_PtpC37p238v2ProfileInfoGet,         /* profileInfoFn */

         /* DEFAULTS SET ROUTINES */
         {
            zl303xx_PtpC37p238v2ClockDefaults,       /* defaultsSetFn.clock */
            zl303xx_PtpC37p238v2PortDefaults,        /* defaultsSetFn.port */
            zl303xx_PtpC37p238v2StreamDefaults,      /* defaultsSetFn.stream */
         },

         /* PRE PTP OBJECT CREATE ROUTINES */
         {
            NULL,                                  /* preCreateActionFn.clock */
            NULL,                                  /* preCreateActionFn.port */
            NULL,                                  /* preCreateActionFn.stream */
         },

         /* POST PTP OBJECT CREATE ROUTINES */
         {
            zl303xx_PtpC37p238ClockCreateActions,    /* postCreateActionFn.clock */
            NULL,                                  /* postCreateActionFn.port */
            NULL,                                  /* postCreateActionFn.stream */
         },

         /* POST PTP OBJECT CREATE ROUTINES */
         {
            NULL,                                  /* profileDsUpdateFn.clock */
            NULL,                                  /* profileDsUpdateFn.port */
            NULL,                                  /* profileDsUpdateFn.stream */
         },

         /* DOMIAN Checks */
         {
            NULL,                                  /* rangeCheckFn */
            PTP_CLOCK_DOMAIN_NUMBER_MIN,           /* MIN */
            PTP_CLOCK_DOMAIN_NUMBER_MAX,           /* MAX */
            DEFAULT_C37_238_2014_DOMAIN            /* DEFAULT */
         },

         NULL,                                     /* transportSpecificCheckFn */
         NULL,  /* Full Range 0 - 255 allowed */   /* clockQualityCheckFn */
         NULL,  /* Full Range 0 - 255 allowed */   /* clockPriorityCheckFn */

         /* ANNOUNCE RX TIMEOUT Checks */
         {
            NULL,                                  /* rangeCheckFn */
            PTP_CLOCK_ANNC_RX_TIMEOUT_MIN,         /* MIN */
            PTP_CLOCK_ANNC_RX_TIMEOUT_MAX,         /* MAX */
            PTP_CLOCK_ANNC_RX_TIMEOUT_DEF          /* DEFAULT */
         },

         NULL,                                     /* streamModeCheckFn */
         NULL,                                     /* portMasterOnlyCheckFn */
         {
            NULL,                                  /* contractDurationCheckFn */
            NULL                                   /* grantDenyCheckFn */
         },

         NULL,                                     /* egressPktHeaderGatherFn */

         {
            zl303xx_PtpC37p238AnncRxCheck,           /* ingressMsgCheckFn */
            NULL                                   /* egressDataGatherFn */
         },
         {
            NULL,                                  /* intvlCheckFn.unicastQuery */
            NULL                                   /* intvlCheckFn.msgInterval */
         },
         {
            NULL,                                  /* bmca.dataAddFn */
            NULL,                                  /* bmca.virtualPortDataAddFn */
            NULL                                   /* bmca.comparisonFn */
         },
         zl303xx_PtpC37p238TlvAppend,                /* tlvAppendFn */

         /* DEBUG SHOW ROUTINES */
         {
            zl303xx_PtpC37p238ClockDebugShow,        /* debugShowFn.clockData */
            zl303xx_PtpC37p238PortDebugShow,         /* debugShowFn.portData */
            zl303xx_PtpC37p238StreamDebugShow        /* debugShowFn.streamData */
         }
      },
      {  "TSN Profile (802.1AS 2011)",
          ZL303XX_PTP_PROFILE_TSN_802_1_2011,
          zl303xx_Ptp802p1v1ProfileInfoGet,             /* profileInfoFn */

            /* DEFAULTS SET ROUTINES */
            {
               zl303xx_Ptp802p1v1ClockDefaults,         /* defaultsSetFn.clock */
               zl303xx_Ptp802p1v1PortDefaults,          /* defaultsSetFn.port */
               zl303xx_Ptp802p1v1StreamDefaults,        /* defaultsSetFn.stream */
            },

            /* PRE PTP OBJECT CREATE ROUTINES */
            {
               NULL,                                  /* preCreateActionFn.clock */
               NULL,                                  /* preCreateActionFn.port */
               NULL,                                  /* preCreateActionFn.stream */
            },

            /* POST PTP OBJECT CREATE ROUTINES */
            {
               zl303xx_Ptp802p1ClockCreateActions,      /* postCreateActionFn.clock */
               NULL,                                  /* postCreateActionFn.port */
               NULL,                                  /* postCreateActionFn.stream */
            },

            /*Routines to update profile specific data sets for PTP Objects */
            {
               NULL,                                  /* profileDsUpdateFn.clock */
               NULL,                                  /* profileDsUpdateFn.port */
               NULL,                                  /* profileDsUpdateFn.stream */
            },

            /* DOMIAN Checks */
            {
               NULL,                                  /* rangeCheckFn */
               PTP_CLOCK_DOMAIN_NUMBER_MIN,           /* MIN */
               PTP_CLOCK_DOMAIN_NUMBER_MAX,           /* MAX */
               ZL303XX_PTP_802P1_DOMAIN_NUM_DEF         /* DEFAULT */
            },

            NULL,                                     /* transportSpecificCheckFn */
            NULL,  /* Full Range 0 - 255 allowed */   /* clockQualityCheckFn */
            NULL,  /* Full Range 0 - 255 allowed */   /* clockPriorityCheckFn */

            /* ANNOUNCE RX TIMEOUT Checks */
            {
               NULL,                                  /* rangeCheckFn */
               PTP_CLOCK_ANNC_RX_TIMEOUT_MIN,         /* MIN */
               PTP_CLOCK_ANNC_RX_TIMEOUT_MAX,         /* MAX */
               ZL303XX_PTP_802P1_ANNC_TIMEOUT_DEF        /* DEFAULT */
            },

            NULL,                                     /* streamModeCheckFn */
            NULL,                                     /* portMasterOnlyCheckFn */
            {
               NULL,                                  /* contractDurationCheckFn */
               NULL                                   /* grantDenyCheckFn */
            },

            NULL,                                     /* egressPktHeaderGatherFn */

            {
               NULL,                                  /* ingressMsgCheckFn */
               NULL                                   /* egressDataGatherFn */
            },
            {
               NULL,                                  /* intvlCheckFn.unicastQuery */
               NULL                                   /* intvlCheckFn.msgInterval */
            },
            {
               NULL,                                  /* bmca.dataAddFn */
               NULL,                                  /* bmca.virtualPortDataAddFn */
               NULL                                   /* bmca.comparisonFn */
            },
            zl303xx_Ptp802p1TlvAppend,                                     /* tlvAppendFn */

            /* DEBUG SHOW ROUTINES */
            {
               zl303xx_Ptp802p1ClockDebugShow,          /* debugShowFn.clockData */
               zl303xx_Ptp802p1PortDebugShow,           /* debugShowFn.portData */
               zl303xx_Ptp802p1StreamDebugShow          /* debugShowFn.streamData */
            }

      }
   };


/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   EXTERNAL GLOBAL VARIABLES   ******************************/

/****************   EXPORTED FUNCTION DEFINITIONS (BINDING CALLS)   **********/

/**
    zl303xx_PtpProfileInfoGet

  Details:
   Retrieves the profile description information for the specified profile.

  Parameters:
   [in]  profileId      ID of the profile for which ProfileInfo is requested.

   [out] profileInfo    Pointer to the structure to store the profile information.

  Return Value:
     ZL303XX_OK                 Success.
     ZL303XX_INVALID_POINTER    profileInfo is NULL.
     ZL303XX_PARAMETER_INVALID  profileId is undefined (in this case, the
                                    default is still returned).

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileInfoGet(
      zl303xx_PtpProfileE profileId,
      const zl303xx_PtpProfileS **profileInfo)
{
   /* Set default return values. */
   zlStatusE status = ZL303XX_OK;

   /* To make the code flow a little clearer, set the default return data. */
   if (ptpProfileFn[ZL303XX_PTP_PROFILE_DEFAULT].profileInfoFn != NULL)
   {
      status = ptpProfileFn[ZL303XX_PTP_PROFILE_DEFAULT].profileInfoFn(profileInfo);
   }

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      if ((status = ZL303XX_CHECK_POINTER(profileInfo)) != ZL303XX_OK)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpProfileInfoGet: NULL Pointer", 0,0,0,0,0,0);
      }
   }

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileInfoGet", profileId);
   }

   /* Return the data for the value (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].profileInfoFn != NULL)
      {
         status = ptpProfileFn[profileId].profileInfoFn(profileInfo);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileInfoGet */

/**
    zl303xx_PtpProfileClockStructInit

  Details:
   Set the Profile Specific default values for the Clock.

  Parameters:
   [in]  profileId      ID of the profile setting of the Clock being created.

   [out] pClockCreateS  Pointer to a Clock creation structure for which to
                              set profile specific default values.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      pClockCreateS pointer is invalid.
     ZL303XX_PARAMETER_INVALID    profileId is undefined (in this case, the
                                       default is still returned).

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileClockStructInit(
      zl303xx_PtpProfileE profileId,
      zl303xx_PtpClockCreateS *pClockCreateS)
{
   /* Set default return values. */
   zlStatusE status = ZL303XX_OK;

   /* Check input pointer. */
   if (status == ZL303XX_OK)
   {
      if (pClockCreateS == NULL)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpProfileClockStructInit: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileClockStructInit", profileId);
   }

   /* First, set the Default values for the Clock. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpClockCreateStructInit(pClockCreateS);
   }

   /* Set any necessary Clock Defaults for the profile (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].defaultsSetFn.clock != NULL)
      {
         status = ptpProfileFn[profileId].defaultsSetFn.clock(pClockCreateS);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileClockStructInit */

/**
    zl303xx_PtpProfilePortStructInit

  Details:
   Set the Profile Specific default values for the Port.

  Parameters:
   [in]  profileId      ID of the profile setting of the Clock associated
                              with the Port being created.

   [out] pPortCreateS   Pointer to a Port creation structure for which to
                              set profile specific default values.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      pPortCreateS pointer is invalid.
     ZL303XX_PARAMETER_INVALID    profileId is undefined (in this case, the
                                       default is still returned).

 *****************************************************************************/
zlStatusE zl303xx_PtpProfilePortStructInit(
      zl303xx_PtpProfileE profileId,
      zl303xx_PtpPortCreateS *pPortCreateS)
{
   /* Set default return values. */
   zlStatusE status = ZL303XX_OK;

   /* Check input pointer. */
   if (status == ZL303XX_OK)
   {
      if (pPortCreateS == NULL)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpProfilePortStructInit: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfilePortStructInit", profileId);
   }

   /* First, set the Default values for the Port. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpPortCreateStructInit(pPortCreateS);
   }

   /* Next, set any necessary Port Defaults for the profile (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].defaultsSetFn.port != NULL)
      {
         status = ptpProfileFn[profileId].defaultsSetFn.port(pPortCreateS);
      }
   }

   return status;
}  /* END zl303xx_PtpProfilePortStructInit */

/**
    zl303xx_PtpProfileStreamStructInit

  Details:
   Set the Profile Specific default values for the Stream.

  Parameters:
   [in]  profileId      ID of the profile setting of the Clock associated
                              with the Stream being created.

   [out] pStreamCreateS Pointer to a Stream creation structure for which to
                              set profile specific default values.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_INVALID_POINTER      pStreamCreateS pointer is invalid.
     ZL303XX_PARAMETER_INVALID    profileId is undefined (in this case, the
                                       default is still returned).

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileStreamStructInit(
      zl303xx_PtpProfileE profileId,
      zl303xx_PtpStreamCreateS *pStreamCreateS)
{
   /* Set default return values. */
   zlStatusE status = ZL303XX_OK;

   /* Check input pointer. */
   if (status == ZL303XX_OK)
   {
      if (pStreamCreateS == NULL)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpProfileStreamStructInit: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      /* This MACRO calls TRACE automatically. */
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileStreamStructInit", profileId);
   }

   /* First, set the Default values for the Stream. */
   if (status == ZL303XX_OK)
   {
      status = zl303xx_PtpStreamCreateStructInit(pStreamCreateS);
   }

   /* Next, set any necessary Stream Defaults for the profile (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].defaultsSetFn.stream != NULL)
      {
         status = ptpProfileFn[profileId].defaultsSetFn.stream(pStreamCreateS);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileStreamStructInit */

/**
    zl303xx_PtpProfileClockPreCreate

  Details:
   Executes any profile specific setup actions prior to a PTP Object being
   created. May involve final parameter checks, etc.

  Parameters:
   [in]  pClockCreateS  Pointer to the Clock Configuration structure.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   The pClockCreateS pointer is invalid.
     OTHER                   As specified by the Profile specific routine.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileClockPreCreate(
      zl303xx_PtpClockCreateS *pClockCreateS)
{
   zlStatusE status = ZL303XX_OK;

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointer. */
   if (status == ZL303XX_OK)
   {
      if (pClockCreateS == NULL)
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "zl303xx_PtpProfileClockPreCreate: NULL Pointer",
            0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }

      /* Check the profile Id. */
      else
      {
         /* Assign to a local variable to make the code more readable. */
         profileId = pClockCreateS->profile;
         status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileClockPreCreate", profileId);
      }
   }

   /* Execute any necessary profile specific actions (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].preCreateActionFn.clock != NULL)
      {
         status = ptpProfileFn[profileId].preCreateActionFn.clock(pClockCreateS, NULL);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileClockPreCreate */

/**
    zl303xx_PtpProfilePortPreCreate

  Details:
   Executes any profile specific setup actions prior to a PTP Object being
   created.

  Parameters:
   [in]  pPortCreateS      Pointer to the Port Configuration structure.
   [in]  pPtpParentClock   Pointer to the parent Clock of this Port.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   The pPortCreateS or pPtpParentClock pointer
                                    is invalid.
     OTHER                   As specified by the Profile specific routine.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfilePortPreCreate(
      zl303xx_PtpPortCreateS *pPortCreateS,
      zl303xx_PtpClockS *pPtpParentClock)
{
   zlStatusE status = ZL303XX_OK;

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointer. */
   if (status == ZL303XX_OK)
   {
      if ((pPortCreateS == NULL) || (pPtpParentClock == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "zl303xx_PtpProfilePortPreCreate: NULL Pointer",
            0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }

      /* Check the profile Id. */
      else
      {
         /* Assign to a local variable to make the code more readable. */
         profileId = pPtpParentClock->config.profile;
         status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfilePortPreCreate", profileId);
      }
   }

   /* Execute any necessary profile specific actions (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].preCreateActionFn.port != NULL)
      {
         status = ptpProfileFn[profileId].preCreateActionFn.port(pPortCreateS, pPtpParentClock);
      }
   }

   return status;
}  /* END zl303xx_PtpProfilePortPreCreate */

/**
    zl303xx_PtpProfileStreamPreCreate

  Details:
   Executes any profile specific setup actions prior to a PTP Object being
   created.

  Parameters:
   [in]  pStreamCreateS    Pointer to the Stream Configuration structure.
   [in]  pPtpParentPort    Pointer to the parent Port of this Stream.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   The pStreamCreateS or pPtpParentPort pointer
                                    is invalid.
     OTHER                   As specified by the Profile specific routine.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileStreamPreCreate(
      zl303xx_PtpStreamCreateS *pStreamCreateS,
      zl303xx_PortDataS *pPtpParentPort)
{
   zlStatusE status = ZL303XX_OK;

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointer. */
   if (status == ZL303XX_OK)
   {
      if ((pStreamCreateS == NULL) || (pPtpParentPort == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
            "zl303xx_PtpProfileStreamPreCreate: NULL Pointer",
            0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }

      /* Check the profile Id. */
      else
      {
         /* Assign to a local variable to make the code more readable. */
         profileId = pPtpParentPort->clock->config.profile;
         status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileStreamPreCreate", profileId);
      }
   }

   /* Execute any necessary profile specific actions (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].preCreateActionFn.stream != NULL)
      {
         status = ptpProfileFn[profileId].preCreateActionFn.stream(pStreamCreateS, pPtpParentPort);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileStreamPreCreate */

/**
    zl303xx_PtpProfileClockPostCreate

  Details:
   Executes any profile specific setup actions after a PTP Object is created.
   (For Clocks, a common action is to BIND profile specific TLV handling).

  Parameters:
   [in]  pPtpClock   Pointer to a newly created PTP clock.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   The input clock pointer is invalid.
     OTHER                   As specified by the Profile specific routine.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileClockPostCreate(
      zl303xx_PtpClockS *pPtpClock)
{
   /* Check input pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pPtpClock);

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointer. */
   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpProfileClockPostCreate: NULL Pointer", 0,0,0,0,0,0);
   }

   /* Check the profile Id. */
   else
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpClock->config.profile;
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileClockPostCreate", profileId);
   }

   /* Execute any necessary profile specific actions (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].postCreateActionFn.clock != NULL)
      {
         status = ptpProfileFn[profileId].postCreateActionFn.clock(pPtpClock);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileClockPostCreate */

/**
    zl303xx_PtpProfilePortPostCreate

  Details:
   Executes any profile specific setup actions after a PTP Object is created.

  Parameters:
   [in]  pPtpPort    Pointer to a newly created PTP port.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   The input port pointer is invalid.
     OTHER                   As specified by the Profile specific routine.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfilePortPostCreate(
      zl303xx_PortDataS *pPtpPort)
{
   /* Check input pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pPtpPort);

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointer. */
   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpProfilePortPostCreate: NULL Pointer", 0,0,0,0,0,0);
   }

   /* Check the profile Id. */
   else
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpPort->clock->config.profile;
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfilePortPostCreate", profileId);
   }

   /* Execute any necessary profile specific actions (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].postCreateActionFn.port != NULL)
      {
         status = ptpProfileFn[profileId].postCreateActionFn.port(pPtpPort);
      }
   }

   return status;
}  /* END zl303xx_PtpProfilePortPostCreate */

/**
    zl303xx_PtpProfileStreamPostCreate

  Details:
   Executes any profile specific setup actions after a PTP Object is created.

  Parameters:
   [in]  pPtpStream    Pointer to a newly created PTP Stream.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   The input Stream pointer is invalid.
     OTHER                   As specified by the Profile specific routine.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileStreamPostCreate(
      zl303xx_ClockStreamS *pPtpStream)
{
   /* Check input pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pPtpStream);

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointer. */
   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpProfileStreamPostCreate: NULL Pointer", 0,0,0,0,0,0);
   }

   /* Check the profile Id. */
   else
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpStream->portData->clock->config.profile;
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileStreamPostCreate", profileId);
   }

   /* Execute any necessary profile specific actions (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].postCreateActionFn.stream != NULL)
      {
         status = ptpProfileFn[profileId].postCreateActionFn.stream(pPtpStream);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileStreamPostCreate */

/**
    zl303xx_PtpProfileClockDsUpdate

  Details:
   Executes any profile specific setup actions after a PTP Object is created.
   (For Clocks, a common action is to BIND profile specific TLV handling).

  Parameters:
   [in]  pPtpClock   Pointer to a newly created PTP clock.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   The input clock pointer is invalid.
     OTHER                   As specified by the Profile specific routine.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileClockDsUpdate(
      zl303xx_PtpClockS *pPtpClock)
{
   /* Check input pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pPtpClock);

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointer. */
   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpProfileClockDsUpdate: NULL Pointer", 0,0,0,0,0,0);
   }

   /* Check the profile Id. */
   else
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpClock->config.profile;
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileClockDsUpdate", profileId);
   }

   /* Execute any necessary profile specific actions (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].profileDsUpdateFn.clock != NULL)
      {
         status = ptpProfileFn[profileId].profileDsUpdateFn.clock(pPtpClock);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileClockDsUpdate */

/**
    zl303xx_PtpProfilePortDsUpdate

  Details:
   Executes any profile specific setup actions after a PTP Object is created.

  Parameters:
   [in]  pPtpPort    Pointer to a newly created PTP port.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   The input port pointer is invalid.
     OTHER                   As specified by the Profile specific routine.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfilePortDsUpdate(
      zl303xx_PortDataS *pPtpPort)
{
   /* Check input pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pPtpPort);

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointer. */
   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpProfilePortDsUpdate: NULL Pointer", 0,0,0,0,0,0);
   }

   /* Check the profile Id. */
   else
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpPort->clock->config.profile;
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfilePortDsUpdate", profileId);
   }

   /* Execute any necessary profile specific actions (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].profileDsUpdateFn.port != NULL)
      {
         status = ptpProfileFn[profileId].profileDsUpdateFn.port(pPtpPort);
      }
   }

   return status;
}  /* END zl303xx_PtpProfilePortDsUpdate */

/**
    zl303xx_PtpProfileStreamDsUpdate

  Details:
   Executes any profile specific setup actions after a PTP Object is created.

  Parameters:
   [in]  pPtpStream    Pointer to a newly created PTP Stream.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   The input Stream pointer is invalid.
     OTHER                   As specified by the Profile specific routine.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileStreamDsUpdate(
      zl303xx_ClockStreamS *pPtpStream)
{
   /* Check input pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pPtpStream);

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointer. */
   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpProfileStreamDsUpdate: NULL Pointer", 0,0,0,0,0,0);
   }

   /* Check the profile Id. */
   else
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpStream->portData->clock->config.profile;
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileStreamDsUpdate", profileId);
   }

   /* Execute any necessary profile specific actions (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].profileDsUpdateFn.stream != NULL)
      {
         status = ptpProfileFn[profileId].profileDsUpdateFn.stream(pPtpStream);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileStreamDsUpdate */

/**
    zl303xx_PtpProfileDomainNumberCheck

  Details:
   Executes any profile specific routine to check the Domain Number setting.

  Parameters:
   [in]  profileId      ID of the profile for which the check is being
                              performed.
   [in]  strictFlag     Flag indicating whether an ERROR must be returned or
                              if the profile MAY set the value to a default.
   [in]  domainNumber   Pointer to the Domain Number being used. (If outside
                              the profile range, the value MAY be set to the
                              profile default and no error returned.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    profileId or domainNumber are invalid.
     ZL303XX_INVALID_POINTER      domainNumber pointer is invalid.
     OTHER                      As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileDomainNumberCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      Uint8T *domainNumber)
{
   /* Set default return value by checking the pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(domainNumber);

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileDomainNumberCheck", profileId);
   }

   /* Execute any necessary Domain Number checks (if binding exists). Otherwise,
    * the default check is performed.  */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].domain.rangeCheckFn != NULL)
      {
         status = ptpProfileFn[profileId].domain.rangeCheckFn(domainNumber, strictFlag);
      }

      /* Use the supplied range limits for the profile. */
      else
      {
         if ((*domainNumber > ptpProfileFn[profileId].domain.max) ||
             (*domainNumber < ptpProfileFn[profileId].domain.min))
         {
            if (strictFlag == ZL303XX_FALSE)
            {
               ZL303XX_TRACE_ALWAYS(
                     "   (WARNING) domainNumber(%d) outside profile range {%d,%d}. Defaulting to %d.",
                     *domainNumber,
                     ptpProfileFn[profileId].domain.min,
                     ptpProfileFn[profileId].domain.max,
                     ptpProfileFn[profileId].domain.def, 0,0);

               /* Set default, return no error. */
               *domainNumber = ptpProfileFn[profileId].domain.def;
            }
            else
            {
               ZL303XX_TRACE_ALWAYS(
                     "   (ERROR) domainNumber(%d) outside profile range {%d,%d}.",
                     *domainNumber,
                     ptpProfileFn[profileId].domain.min,
                     ptpProfileFn[profileId].domain.max, 0,0,0);

               status = ZL303XX_PARAMETER_INVALID;
            }
         }
      }
   }

   return status;
}  /* END zl303xx_PtpProfileDomainNumberCheck */

/**
    zl303xx_PtpProfileTransportSpecificCheck

  Details:
   Executes any profile specific routine to check the transportSpecific field
   of a PTP message.

  Parameters:
   [in]  profileId      ID of the profile for which the check is being
                              performed.
   [in]  strictFlag     Flag indicating whether an ERROR must be returned or
                              if the profile MAY accept the value OR set it to
                              a default.

   [in]  transportSpecific Pointer to the transportSpecific field value. (If
                                 outside the profile range, the value MAY be set
                                 to the profile default (or accepted as is) and
                                 no error returned.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    profileId or transportSpecific are invalid.
     ZL303XX_INVALID_POINTER      transportSpecific pointer is invalid.
     OTHER                      As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileTransportSpecificCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      Uint8T *transportSpecific)
{
   /* Set default return value by checking the pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(transportSpecific);

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileTransportSpecificCheck", profileId);
   }

   /* Execute any necessary Domain Number checks (if binding exists). Otherwise,
    * the default check is performed.  */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].transportSpecificCheckFn != NULL)
      {
         status = ptpProfileFn[profileId].transportSpecificCheckFn(transportSpecific, strictFlag);
      }
      /* IEEE-1588: Table D.2 allows just '0' or '1' values */
      else if (*transportSpecific > 1)
      {
         if (strictFlag == ZL303XX_TRUE)
         {
            ZL303XX_TRACE_ALWAYS("(ERROR) transportSpecific value (%u) out-of-range [0:1]",
                  *transportSpecific, 0,0,0,0,0);

            status = ZL303XX_PARAMETER_INVALID;
         }
         else
         {
            ZL303XX_TRACE_ALWAYS("(WARNING) transportSpecific value (%u) out-of-range [0:1]",
                  *transportSpecific, 0,0,0,0,0);
         }
      }
   }

   return status;
}  /* END zl303xx_PtpProfileTransportSpecificCheck */

/**
    zl303xx_PtpProfileClockClassCheck

  Details:
   Executes any profile specific routine to check the Domain Number setting.

  Parameters:
   [in]  profileId      ID of the profile for which the check is being
                              performed.
   [in]  strictFlag     Flag indicating whether an ERROR must be returned or
                              if the profile MAY set the value to a default.
   [in]  clockQuality   Pointer to the Clock Quality structure. (If member
                              values are outside the profile range, the value MAY
                              be set to the profile default and no error returned.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    profileId or clockClass are invalid.
     ZL303XX_INVALID_POINTER      clockClass pointer is invalid.
     OTHER                      As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileClockQualityCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      zl303xx_ClockQuality *clockQuality)
{
   /* Set default return value by checking the pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(clockQuality);

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileClockClassCheck", profileId);
   }

   /* Execute any necessary Clock Quality checks (if binding exists). Otherwise,
    * the default check is performed.  */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].clockQualityCheckFn != NULL)
      {
         status = ptpProfileFn[profileId].clockQualityCheckFn(clockQuality, strictFlag);
      }

      /* Right now, the default is NULL. */
      else if (ptpProfileFn[ZL303XX_PTP_PROFILE_DEFAULT].clockQualityCheckFn != NULL)
      {
         status = ptpProfileFn[ZL303XX_PTP_PROFILE_DEFAULT].clockQualityCheckFn(clockQuality, strictFlag);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileClockQualityCheck */

/**
    zl303xx_PtpProfileClockPriorityCheck

  Details:
   Executes any profile specific routine to check the Priority settings.

  Parameters:
   [in]  profileId      ID of the profile for which the check is being
                              performed.
   [in]  strictFlag     Flag indicating whether an ERROR must be returned or
                              if the profile MAY set the value to a default.
   [in]  priority1      Pointer to the Clock priority1 value being used.
   [in]  priority2      Pointer to the Clock priority2 value being used.
   [in]  bSlaveOnly     Flag indicating if this a slave-only clock.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    profileId or clockClass are invalid.
     ZL303XX_INVALID_POINTER      clockClass pointer is invalid.
     OTHER                      As specified by the Profile specific check.

  Notes:
   If either value is outside the profile range, it MAY be set to the profile
      default and no error returned.
   Passing in NULL will skip the check for that priority.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileClockPriorityCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      Uint8T *priority1,
      Uint8T *priority2,
      zl303xx_BooleanE bSlaveOnly)
{
   /* Set default return value. */
   zlStatusE status = ZL303XX_OK;

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileClockPriorityCheck", profileId);
   }

   /* Execute any necessary Clock Class checks (if binding exists). Otherwise,
    * the default check is performed.  */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].clockPriorityCheckFn != NULL)
      {
         status = ptpProfileFn[profileId].clockPriorityCheckFn(
                                       priority1, priority2, bSlaveOnly, strictFlag);
      }

      /* Right now, the default is NULL. */
      else if (ptpProfileFn[ZL303XX_PTP_PROFILE_DEFAULT].clockPriorityCheckFn != NULL)
      {
         status = ptpProfileFn[ZL303XX_PTP_PROFILE_DEFAULT].clockPriorityCheckFn(
                                       priority1, priority2, bSlaveOnly, strictFlag);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileClockPriorityCheck */

/**
    zl303xx_PtpProfileAnncReceiptTimeoutCheck

  Details:
   Executes any profile specific routine to check the Announce Receipt Timeout
   setting.

  Parameters:
   [in]  profileId      ID of the profile for which the check is being
                              performed.
   [in]  strictFlag     Flag indicating whether an ERROR must be returned or
                              if the profile MAY set the value to a default.
   [in]  anncReceiptTimeout   Pointer to the Announce Receipt Timeout being
                              set. (If outside the profile range, the value MAY
                              be set to the profile default and no error returned).

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    profileId or anncReceiptTimeout are invalid.
     ZL303XX_INVALID_POINTER      anncReceiptTimeout pointer is invalid.
     OTHER                      As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileAnncReceiptTimeoutCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      Uint8T *anncReceiptTimeout)
{
   /* Set default return value by checking the pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(anncReceiptTimeout);

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileAnncReceiptTimeoutCheck", profileId);
   }

   /* Execute any necessary Domain Number checks (if binding exists). Otherwise,
    * the default check is performed.  */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].anncRxTimeout.rangeCheckFn != NULL)
      {
         status = ptpProfileFn[profileId].anncRxTimeout.rangeCheckFn(anncReceiptTimeout, strictFlag);
      }

      /* Use the supplied range limits for the profile. */
      else
      {
         if ((*anncReceiptTimeout > ptpProfileFn[profileId].anncRxTimeout.max) ||
             (*anncReceiptTimeout < ptpProfileFn[profileId].anncRxTimeout.min))
         {
            if (strictFlag == ZL303XX_FALSE)
            {
               ZL303XX_TRACE_ALWAYS(
                     "   (WARNING) anncReceiptTimeout(%d) outside profile range {%d,%d}. Defaulting to %d.",
                     *anncReceiptTimeout,
                     ptpProfileFn[profileId].anncRxTimeout.min,
                     ptpProfileFn[profileId].anncRxTimeout.max,
                     ptpProfileFn[profileId].anncRxTimeout.def, 0,0);

               /* Set default, return no error. */
               *anncReceiptTimeout = ptpProfileFn[profileId].anncRxTimeout.def;
            }
            else
            {
               ZL303XX_TRACE_ALWAYS(
                     "   (ERROR) anncReceiptTimeout(%d) outside profile range {%d,%d}.",
                     *anncReceiptTimeout,
                     ptpProfileFn[profileId].anncRxTimeout.min,
                     ptpProfileFn[profileId].anncRxTimeout.max, 0,0,0);

               status = ZL303XX_PARAMETER_INVALID;
            }
         }
      }
   }

   return status;
}  /* END zl303xx_PtpProfileAnncReceiptTimeoutCheck */

/**
    zl303xx_PtpProfileQueryIntervalCheck

  Details:
   Executes any profile specific routine to check the Domain Number setting.

  Parameters:
   [in]  profileId      ID of the profile for which the check is being
                              performed.
   [in]  strictFlag     Flag indicating whether an ERROR must be returned or
                              if the profile MAY set the value to a default.
   [in]  queryInterval  Pointer to a Unicast Query Interval value. (If outside
                              the profile range, the value MAY be set to the
                              profile default and no error returned.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    profileId or queryInterval are invalid.
     ZL303XX_INVALID_POINTER      queryInterval pointer is invalid.
     OTHER                      As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileQueryIntervalCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      Sint8T *queryInterval)
{
   /* Set default return value by checking the pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(queryInterval);

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileQueryIntervalCheck", profileId);
   }

   /* Execute any necessary Unicast Query Interval checks (if binding exists).
    * Otherwise, the default check is performed (if it exists).  */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].intvlCheckFn.unicastQuery != NULL)
      {
         status = ptpProfileFn[profileId].intvlCheckFn.unicastQuery(
                                             queryInterval, strictFlag);
      }

      /* Right now, the default is NULL. */
      else if (ptpProfileFn[ZL303XX_PTP_PROFILE_DEFAULT].intvlCheckFn.unicastQuery != NULL)
      {
         status = ptpProfileFn[ZL303XX_PTP_PROFILE_DEFAULT].intvlCheckFn.unicastQuery(
                                             queryInterval, strictFlag);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileQueryIntervalCheck */

/**
    zl303xx_PtpProfileMsgIntervalCheck

  Details:
   Executes any profile specific routine to check the specified Message Interval.

  Parameters:
   [in]  profileId      ID of the profile for which the check is being
                              performed.
   [in]  strictFlag     Flag indicating whether an ERROR must be returned or
                              if the profile MAY set the value to a default.
   [in]  msgType        Message Type to check the interval of.
   [in]  logInterval    Pointer to a Message Interval value. (If outside
                              the profile range, the value MAY be set to the
                              profile default and no error returned).

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    Stream profileId or logInterval are invalid.
     ZL303XX_INVALID_POINTER      pPtpStream or logInterval pointer is invalid.
     OTHER                      As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileMsgIntervalCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      zl303xx_MessageTypeE msgType,
      Sint8T *logInterval)
{
   /* Set default return value by checking the ProfileID. The interval
    * pointer is checked within each sub-routine. */
   zlStatusE status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileMsgIntervalCheck", profileId);

   /* Execute any necessary Message Interval checks (if binding exists).
    * Otherwise, the default is to allow any rate).  */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].intvlCheckFn.msgInterval != NULL)
      {
         status = ptpProfileFn[profileId].intvlCheckFn.msgInterval(
                                             msgType,
                                             logInterval,
                                             strictFlag);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileMsgIntervalCheck */

/**
    zl303xx_PtpProfileUnicastDurationCheck

  Details:
   Executes any profile specific routine to check Unicast Contract Duration.

  Parameters:
   [in]  profileId      ID of the profile for which the check is being
                              performed.
   [in]  strictFlag     Flag indicating whether an ERROR must be returned or
                              if the profile MAY set the value to a default.
   [in]  duration       Pointer to the Unicast Contract Duration value. (If
                              outside the profile range, the value MAY be set to
                              the profile default and no error returned.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    profileId or duration are invalid.
     ZL303XX_INVALID_POINTER      duration pointer is invalid.
     OTHER                      As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileUnicastDurationCheck(
      zl303xx_PtpProfileE profileId,
      zl303xx_BooleanE strictFlag,
      Uint32T *duration)
{
   /* Set default return value by checking the pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(duration);

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileUnicastDurationCheck", profileId);
   }

   /* Execute any necessary Unicast Query Interval checks (if binding exists).
    * Otherwise, the default check is performed (if it exists).  */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].unicastNeg.contractDurationCheckFn != NULL)
      {
         status = ptpProfileFn[profileId].unicastNeg.contractDurationCheckFn(
                                             duration, strictFlag);
      }

      /* IEEE-1588: Clause A.9.4.2 specifies a range of 10 sec to 1000 sec which
       * is checked by the Default Profile. */
      else if (ptpProfileFn[ZL303XX_PTP_PROFILE_DEFAULT].unicastNeg.contractDurationCheckFn != NULL)
      {
         status = ptpProfileFn[ZL303XX_PTP_PROFILE_DEFAULT].unicastNeg.contractDurationCheckFn(
                                             duration, strictFlag);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileUnicastDurationCheck */

/**
    zl303xx_PtpProfileUnicastGrantDenyCheck

  Details:
   Executes any profile specific routine to check if Unicast GRANTS are
   permitted on a specified Port.

  Parameters:
   [in]  pPtpClock   Pointer to an existing PTP Clock on which this Port
                           configuration exists.
   [in]  bGrantDeny  Flag indicating the desired setting of the DENY_GRANT
                           parameter of the specified Port.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    bGrantDeny is invalid for this Port.
     ZL303XX_INVALID_POINTER      pPtpPort or bGrantDeny pointer is invalid.
     OTHER                      As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileUnicastGrantDenyCheck(
      zl303xx_PtpClockS *pPtpClock,
      zl303xx_BooleanE *bGrantDeny)
{
   zlStatusE status = ZL303XX_OK;

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      if ((pPtpClock == NULL) || (bGrantDeny == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpProfileUnicastGrantDenyCheck: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpClock->config.profile;

      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileUnicastGrantDenyCheck", profileId);
   }

   /* Execute any necessary Unicast GRANT DENY checks (if binding exists).
    * Otherwise, the default check is performed (if it exists).  */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].unicastNeg.grantDenyCheckFn != NULL)
      {
         status = ptpProfileFn[profileId].unicastNeg.grantDenyCheckFn(
               pPtpClock, bGrantDeny);
      }

      /* ELSE: there is no restriction. */
   }

   return status;
}  /* END zl303xx_PtpProfileUnicastGrantDenyCheck */

/**
    zl303xx_PtpProfilePortMasterOnlyCheck

  Details:
   Executes any profile specific routine to check if the Master-Only setting
   is allowed on a specified Port.

  Parameters:
   [in]  pPtpPort    Pointer to an existing PTP Port configured
   [in]  bMasterOnly Flag indicating the desired setting of the MASTER_ONLY
                           parameter of the specified Port.

  Return Value:
     ZL303XX_OK                   Success.
     ZL303XX_PARAMETER_INVALID    bMasterOnly is invalid for this Port.
     ZL303XX_INVALID_POINTER      pPtpPort or bMasterOnly pointer is invalid.
     OTHER                      As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfilePortMasterOnlyCheck(
      zl303xx_PortDataS *pPtpPort,
      zl303xx_BooleanE *bMasterOnly)
{
   zlStatusE status = ZL303XX_OK;

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      if ((pPtpPort == NULL) || (bMasterOnly == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpProfilePortMasterOnlyCheck: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpPort->clock->config.profile;

      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfilePortMasterOnlyCheck", profileId);
   }

   /* Execute any necessary MASTER_ONLY checks (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].portMasterOnlyCheckFn != NULL)
      {
         status = ptpProfileFn[profileId].portMasterOnlyCheckFn(
               pPtpPort, bMasterOnly);
      }

      /* ELSE: there is no restriction. */
   }

   return status;
}  /* END zl303xx_PtpProfilePortMasterOnlyCheck */

/**
    zl303xx_PtpProfileAnncRxCheck

  Details:
   Executes any profile specific routine to check if a received Announce message
   is acceptable (some profiles require appended TLVs).

  Parameters:
   [in]  pPtpStream     Pointer to the stream on which the message was received.
   [in]  pPtpMsgRx      Pointer to the received message structure (contains all
                              header and announce fields).
   [in]  pInTlvBuf      Pointer to any appended TLV buffer.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   Either of the input pointers is invalid.
     OTHER                   As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileAnncRxCheck(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgS *pPtpMsgRx,
      Uint8T *pInTlvBuf)
{
   /* Set default return values. */
   zlStatusE status = ZL303XX_OK;

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      /* TLV buffer may be NULL: it is up to the profile to check its presence
       * and if valid data exists. */
      if ((pPtpStream == NULL) || (pPtpMsgRx == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpProfileAnncRxCheck: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpStream->portData->clock->config.profile;
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileAnncRxCheck", profileId);
   }

   /* Execute any necessary Announce message checks (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].announce.ingressMsgCheckFn != NULL)
      {
         status = ptpProfileFn[profileId].announce.ingressMsgCheckFn(
                              pPtpStream,
                              pPtpMsgRx,
                              pInTlvBuf);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileAnncRxCheck */

/**
    zl303xx_PtpProfileStreamModeCheck

  Details:
   Executes any profile specific routine to check if the stream::config::mode
   setting is valid for the corresponding profile.

  Parameters:
   [in]  pPtpStream     Pointer to an existing profile stream.
   [in]  streamMode     Pointer the new mode that is being configured.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   Either of the input pointers is invalid.
     OTHER                   As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileStreamModeCheck(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpStreamModeE *streamMode)
{
   /* Set default return values. */
   zlStatusE status = ZL303XX_OK;

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      if ((pPtpStream == NULL) || (streamMode == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpProfileStreamModeCheck: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpStream->portData->clock->config.profile;
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileStreamModeCheck", profileId);
   }

   /* Execute any necessary Announce message checks (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].streamModeCheckFn != NULL)
      {
         status = ptpProfileFn[profileId].streamModeCheckFn(
                              pPtpStream, streamMode);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileStreamModeCheck */

/**
    zl303xx_PtpProfileEgressPktHeaderGet

  Details:
   Routine to update the fields of an Egress PTP message header with any Profile
   Specific values / restrictions for clocks running this Profile.

   The PTP Header structure being passed has already been data filled with
   standard values prior to this Profile Specific routine being executed.
   Therefore, profiles may update only the fields required (OR update everything
   just to be safe).

  Parameters:
   [in]  pPtpStream  Pointer to the stream on which the PTP message is about
                           to be issued. Must contain the profileId and parent
                           port + clock pointers.

   [in/out] pPtpMsgHeader  Structure containing the PTP Header data values:
                                 - IN: contains default values + messageType.
                                 - OUT: contains any profile specific values.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   Either of the input pointers is invalid.
     OTHER                   As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileEgressPktHeaderGet(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgHeaderS *pPtpMsgHeader)
{
   /* Set default return values. */
   zlStatusE status = ZL303XX_OK;

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointers & profile. */
   if (status == ZL303XX_OK)
   {
      /* pointer check */
      if ((pPtpStream == NULL) || (pPtpMsgHeader == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpProfileEgressPktHeaderGet: NULL Pointer",
               0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }

      /* profile check */
      else
      {
         /* Assign to a local variable to make the code more readable. */
         profileId = pPtpStream->portData->clock->config.profile;
         status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileEgressPktHeaderGet", profileId);
      }
   }

   /* Update any necessary header fields prior to transmission
    * (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].egressPktHeaderGatherFn != NULL)
      {
         status = ptpProfileFn[profileId].egressPktHeaderGatherFn(
                              pPtpStream,
                              pPtpMsgHeader);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileEgressPktHeaderGet */

/**
    zl303xx_PtpProfileAnncEgressDataGet

  Details:
   Routine to update the fields of an Egress Announce message with any Profile
   Specific values / restrictions for clocks running this Profile.

   The Announce data structure being passed has already been data filled with
   standard values prior to this Profile Specific routine being executed.
   Therefore, profiles may update only the fields required (OR update everything
   just to be safe).

  Parameters:
   [in]  pPtpStream  Pointer to the stream on which the Announce message is
                           about to be issued.

   [out] pAnncData   Structure containing the Announce data values for
                           this profile.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   Either of the input pointers is invalid.
     OTHER                   As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileAnncEgressDataGet(
      zl303xx_ClockStreamS *pPtpStream,
      zl303xx_PtpV2MsgAnnounceS *pAnncData)
{
   /* Set default return values. */
   zlStatusE status = ZL303XX_OK;

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      if ((pPtpStream == NULL) || (pAnncData == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpProfileAnncEgressDataGet: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpStream->portData->clock->config.profile;
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileAnncEgressDataGet", profileId);
   }

   /* Update any necessary Announce message fields prior to transmission
    * (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].announce.egressDataGatherFn != NULL)
      {
         status = ptpProfileFn[profileId].announce.egressDataGatherFn(
                              pPtpStream,
                              pAnncData);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileAnncEgressDataGet */

/**
    zl303xx_PtpProfileBmcaDataAdd

  Details:
   Executes any profile specific routine to add BMCA Data to the
   BmcaCompareDataS structure.

  Parameters:
   [in]  pFmtEntry   Pointer to a Foreign Master Table entry from which to
                           extract the Profile specific data.

   [out] pBmcaDataS  Pointer to the BMCA structure.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   Either of the input pointers is invalid.
     OTHER                   As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileBmcaDataAdd(
      zl303xx_ForeignMasterRecordS *pFmtEntry,
      zl303xx_PtpEventClockBmcaEntryS *pBmcaDataS)
{
   /* Set default return values. */
   zlStatusE status = ZL303XX_OK;

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      if ((pFmtEntry == NULL) || (pBmcaDataS == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpProfileBmcaDataAdd: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pFmtEntry->streamData->portData->clock->config.profile;
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileBmcaDataAdd", profileId);
   }

   /* Add any necessary BMCA data from the FMT entry (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].bmca.dataAddFn != NULL)
      {
         status = ptpProfileFn[profileId].bmca.dataAddFn(pFmtEntry, pBmcaDataS);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileBmcaDataAdd */

/**
    zl303xx_PtpProfileBmcaVirtualPortAdd

  Details:
   Executes any profile specific routine to add BMCA Data to the
   BmcaCompareDataS structure for a Virtual PTP Port.

  Parameters:
   [in]  pPtpVirtualPort   Pointer to an existing PTP Port configured for
                                 Virtual Port operations.

   [out] pBmcaDataS  Pointer to the BMCA structure.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   Either of the input pointers is invalid.
     OTHER                   As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileBmcaVirtualPortAdd(
      zl303xx_PortDataS *pPtpVirtualPort,
      zl303xx_PtpEventClockBmcaEntryS *pBmcaDataS)
{
   /* Set default return values. */
   zlStatusE status = ZL303XX_OK;

   /* Local variables */
   zl303xx_PtpProfileE profileId = 0;

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      if ((pPtpVirtualPort == NULL) || (pBmcaDataS == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpProfileBmcaVirtualPortAdd: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpVirtualPort->clock->config.profile;
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileBmcaVirtualPortAdd", profileId);
   }

   /* Add any necessary BMCA data from the FMT entry (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].bmca.virtualPortDataAddFn != NULL)
      {
         status = ptpProfileFn[profileId].bmca.virtualPortDataAddFn(pPtpVirtualPort, pBmcaDataS);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileBmcaVirtualPortAdd */

/**
    zl303xx_PtpProfileBmcaComparison

  Details:
   Executes the BMC comparison algorithm for the specified  profile.

  Parameters:
   [in]  profileId      ID of the profile for which to perform the BMCA.
   [in]  pCompDataA     Comparison data set A
   [in]  pCompDataB     Comparison data set B
   [in]  pExtraData     Pointer to other data that the Algorithm may require
                              (May be NULL).

   [out] result         Result of the BMC Algorithm

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   Either of the input pointers is invalid.
     OTHER                   As specified by the Profile specific check.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileBmcaComparison(
      zl303xx_PtpProfileE profileId,
      zl303xx_PtpEventClockBmcaEntryS *pCompDataA,
      zl303xx_PtpEventClockBmcaEntryS *pCompDataB,
      void *pExtraData,
      Sint8T *result)
{
   /* Set default return values. */
   zlStatusE status = ZL303XX_OK;

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      if ((pCompDataA == NULL) || (pCompDataB == NULL) || (result == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpProfileBmcaComparison: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
      else
      {
         status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileBmcaComparison", profileId);
      }
   }

   /* Execute the profile specific BMCA routine (if binding exists). Otherwise, perform the
    * default comparison. */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].bmca.comparisonFn != NULL)
      {
         *result = ptpProfileFn[profileId].bmca.comparisonFn(pCompDataA, pCompDataB, pExtraData);
      }

      else
      {
      }
   }

   return status;
}  /* END zl303xx_PtpProfileBmcaDataAdd */

/**
    zl303xx_PtpProfileTlvAppend

  Details:
   Executes any profile specific routine to append TLVs to the end of a
   specified message type.

  Parameters:
   [in]  pPtpStream     Pointer to the stream from which the message is being sent.
   [in]  messageType    PTP messageType being sent.
   [in]  pOutTlvBuf     Pointer to the start of the message Buffer to be sent.

   [out] pOutTlvBuf     Pointer to the data portion of the output TLV buffer.

  Return Value:
   Uint16T  Number of bytes appended to the end of the packet on success.
            0 otherwise

 *****************************************************************************/
Uint16T zl303xx_PtpProfileTlvAppend(
      zl303xx_ClockStreamS *pPtpStream,
      Uint32T messageType,
      Uint8T *pOutMsgBuf,
      Uint8T *pOutTlvBuf)
{
   /* Set default return values. */
   Uint16T appendedBytes = 0;
   zlStatusE status = ZL303XX_OK;

   /* Local variables */
   zl303xx_PtpProfileE profileId;

   /* Check input pointers. */
   if (status == ZL303XX_OK)
   {
      if ((pPtpStream == NULL) || (pOutTlvBuf == NULL))
      {
         ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
               "zl303xx_PtpProfileTlvAppend: NULL Pointer", 0,0,0,0,0,0);
         status = ZL303XX_INVALID_POINTER;
      }
   }

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpStream->portData->clock->config.profile;
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileTlvAppend", profileId);
   }

   /* Add any necessary TLVs (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].tlvAppendFn != NULL)
      {
         appendedBytes = ptpProfileFn[profileId].tlvAppendFn(
                              pPtpStream,
                              messageType,
                              pOutMsgBuf,
                              pOutTlvBuf);
      }
   }

   return appendedBytes;
}  /* END zl303xx_PtpProfileTlvAppend */

/**
    zl303xx_PtpProfileClockDataShow

  Details:
   Executes any profile specific routine to show profile related clock parameters.

  Parameters:
   [in]  pPtpClock   Pointer to an existing PTP clock.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   The input clock pointer is invalid.
     OTHER                   As specified by the Profile specific routine.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileClockDataShow(
      zl303xx_PtpClockS *pPtpClock)
{
   /* Check input pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pPtpClock);

   /* Local variables */
   zl303xx_PtpProfileE profileId;

   /* Check input pointer. */
   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpProfileClockDataShow: NULL Pointer", 0,0,0,0,0,0);
   }

   /* Check the profile Id. */
   else
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpClock->config.profile;
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileClockDataShow", profileId);
   }

   /* Show any necessary profile specific Debug data (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].debugShowFn.clockData != NULL)
      {
         status = ptpProfileFn[profileId].debugShowFn.clockData(pPtpClock);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileClockDataShow */

/**
    zl303xx_PtpProfilePortDataShow

  Details:
   Executes any profile specific routine to show profile related port parameters.

  Parameters:
   [in]  pPtpPort    Pointer to an existing PTP port.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   The input port pointer is invalid.
     OTHER                   As specified by the Profile specific routine.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfilePortDataShow(
      zl303xx_PortDataS *pPtpPort)
{
   /* Check input pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pPtpPort);

   /* Local variables */
   zl303xx_PtpProfileE profileId;

   /* Check input pointer. */
   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpProfilePortDataShow: NULL Pointer", 0,0,0,0,0,0);
   }

   /* Check the profile Id. */
   else
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpPort->clock->config.profile;
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfilePortDataShow", profileId);
   }

   /* Show any necessary profile specific Debug data (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].debugShowFn.portData != NULL)
      {
         status = ptpProfileFn[profileId].debugShowFn.portData(pPtpPort);
      }
   }

   return status;
}  /* END zl303xx_PtpProfilePortDataShow */

/**
    zl303xx_PtpProfileStreamDataShow

  Details:
   Executes any profile specific routine to show profile related stream parameters.

  Parameters:
   [in]  pPtpStream    Pointer to an existing PTP stream.

  Return Value:
     ZL303XX_OK                Success.
     ZL303XX_INVALID_POINTER   The input stream pointer is invalid.
     OTHER                   As specified by the Profile specific routine.

 *****************************************************************************/
zlStatusE zl303xx_PtpProfileStreamDataShow(
      zl303xx_ClockStreamS *pPtpStream)
{
   /* Check input pointer. */
   zlStatusE status = ZL303XX_CHECK_POINTER(pPtpStream);

   /* Local variables */
   zl303xx_PtpProfileE profileId;

   /* Check input pointer. */
   if (status != ZL303XX_OK)
   {
      ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
         "zl303xx_PtpProfileStreamDataShow: NULL Pointer", 0,0,0,0,0,0);
   }

   /* Check the profile Id. */
   else
   {
      /* Assign to a local variable to make the code more readable. */
      profileId = pPtpStream->portData->clock->config.profile;
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileStreamDataShow", profileId);
   }

   /* Show any necessary profile specific Debug data (if binding exists). */
   if (status == ZL303XX_OK)
   {
      if (ptpProfileFn[profileId].debugShowFn.streamData != NULL)
      {
         status = ptpProfileFn[profileId].debugShowFn.streamData(pPtpStream);
      }
   }

   return status;
}  /* END zl303xx_PtpProfileStreamDataShow */


/****************   EXPORTED FUNCTION DEFINITIONS (API)   ********************/


/* zl303xx_PtpProfileTlvAppendFnSet */
/**
   Sets the routine to append a TLV to a transmitted PTP Message for a
   specified profile. (Common for ANNOUNCE messages in some profiles).

  Parameters:
   [in]  profileId      ID of the profile for which the binding is to be updated.
   [in]  tlvAppendFn    New TLV Append routine to use (may be NULL).

  Return Value:
     ZL303XX_OK                    Success.
     ZL303XX_PARAMETER_INVALID     profileId is invalid.
     ZL303XX_RTOS_MUTEX_TAKE_FAIL  Failed to lock mutex.
     ZL303XX_RTOS_MUTEX_GIVE_FAIL  Failed to unlock mutex.

*******************************************************************************/
zlStatusE zl303xx_PtpProfileTlvAppendFnSet(
      zl303xx_PtpProfileE profileId,
      zl303xx_PtpTlvAppendFnT tlvAppendFn)
{
   zlStatusE status = ZL303XX_OK;

   /* Check the profile Id. */
   if (status == ZL303XX_OK)
   {
      status = ZL303XX_CHECK_PROFILE("zl303xx_PtpProfileTlvAppendFnSet", profileId);
   }

   /* Update the binding. */
   if (status == ZL303XX_OK)
   {
      ptpProfileFn[profileId].tlvAppendFn = tlvAppendFn;
   }

   return status;
}


