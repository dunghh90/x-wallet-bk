

/*****************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
* Copyright 2006-2018 Microsemi Semiconductor Limited.
* All rights reserved.
*
*  Module Description:
*     Arithmetic functions, principally manipulation of TimeRepresentation types
*
*******************************************************************************/

/****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_ErrTrace.h"

#include "zl303xx_Ptpd.h"
#include "zl303xx_PtpInternal.h"

/****************   DEFINES     **********************************************/

/****************   DATA TYPES   *********************************************/

/****************   DATA STRUCTURES   ****************************************/

/****************   STATIC FUNCTION DECLARATIONS   ***************************/
#ifndef _EXTERNAL_TRANSMIT_TR_NANOSECONDS_ARE_UNSIGNED32
   _ZL303XX_LOCAL Sint32T Uint32NanoSecondsToInt32 (Sint32T ns);
#endif
#ifndef _EXTERNAL_RECEIVE_TR_NANOSECONDS_ARE_UNSIGNED32
   _ZL303XX_LOCAL Uint32T Int32NanoSecondsToUint32 (Sint32T ns);
#endif

/****************   STATIC GLOBAL VARIABLES   ********************************/

/****************   EXPORTED GLOBAL VARIABLES   ******************************/

/****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* log2PacketsPerSecond */
/**
   Converts a packets-per-second value to a log2 interval value.

  Parameters:
   [in]   pktsPerSec  Packets-per-second value to convert. Must be a power
                           of 2 (e.g., 1, 2, 4, etc.)

  Return Value:  Sint32T  Log2 interval value.

*******************************************************************************/
Sint32T log2PacketsPerSecond(Sint16T pktsPerSec)
{
   Sint8T log2value = 0;  /* This is what we want to find which is log2(pktsPerSec) */
   Uint16T mask = 1;


   /* 2^0 = 1 */
   if (pktsPerSec == 0)
   {
      return 1;
   }

   /* PPS < 0 means 1 packet every x seconds (x > 1) */
   else if (pktsPerSec < 0)
   {
      /* Convert the +ve equivalent and return the -ve result.*/
      return -log2PacketsPerSecond(-pktsPerSec);
   }

   else   /* PPS is more than 1/sec */
   {
      while (log2value > -16)  /* pktsPerSec value is 16 bits */
      {
         if (pktsPerSec & mask)
         {
            break;
         }
         mask = (mask << 1) | 1;
         log2value--;
      }
   }

   /* check there was exactly one bit set */
   if (pktsPerSec & ~mask)
   {
      ZL303XX_ERROR_NOTIFY("Packets per second must be power of 2");
      return 0;
   }

   return log2value;
}

/* log2SecondsPerPacket */
/**
   Converts a seconds-per-packet value to a log2 interval value.

  Parameters:
   [in]   secsPerPkt  Seconds-per-packet value to convert. Must be a power
                           of 2 (e.g., 1, 2, 4, etc.)

  Return Value:  Sint32T  Log2 interval value.

*******************************************************************************/
Sint32T log2SecondsPerPacket(Sint16T secsPerPkt)
{
   Sint8T log2value = 0;  /* This is what we want to find which is log2(secsPerPkt) */
   Uint16T mask = 1;

   /* Check it's within a valid range. */
   if (secsPerPkt <=0)
   {
      ZL303XX_ERROR_NOTIFY("Seconds per packet error");
      return 0;
   }

   while (log2value < 16)  /* secsPerPkt field is 16 bits wide */
   {
      if (secsPerPkt & mask)
      {
         break;
      }
      mask = (mask << 1) | 1;
      log2value++;
   }

   /* check there was exactly one bit set */
   if (secsPerPkt & ~mask)
   {
      ZL303XX_ERROR_NOTIFY("Packet interval must be power of 2");
      return 0;
   }

   return log2value;
}

/* log2IntvlToPps */
/** 

   Converts a log2 interval value to a packets per second value. Negative values
   represent seconds per packet.

  Parameters:
   [in]   log2Intvl  The value to convert.

  Return Value:  Sint32T  The packets per second value.

*******************************************************************************/
Sint32T log2IntvlToPps(Sint8T log2Intvl)
{
   Sint32T pps;

   if (log2Intvl < 0)
      pps = 1 << -log2Intvl;
   else if (log2Intvl > 0)
      pps = -(1 << log2Intvl);
   else
      pps = 1;

   return pps;
}

/*

  Function Name:
    crc_algorithm

  Details:
   Computes the checksum of a subdomain_name string

  Parameters:
       pInput   Pointer to the domain name character string
       length   The number of characters to include in the checksum

  Return Value:
   returns the CRC checksum

  Notes:
   The calculation is defined in annex C of the IEEE-1588-2002 spec

*******************************************************************************/

Uint32T crc_algorithm(Uint8T *pInput, Sint16T length)
{
  /* CRC-32 802.3 polynomial is 0x04c11db7
     bit-reversed this 0xedb88320 */
  Uint32T polynomial = 0xedb88320;
  /* crc initialized to all 1s */
  Uint32T crc = 0xffffffff;
  Uint8T *blkPtr = (Uint8T*)pInput;
  /* sequence thru each byte of the input sequence */
  while(length-- > 0)
  {
    Sint16T count;
    Uint8T data = *(blkPtr++);
    /* include each bit of the data, starting with the lsb */
    for (count=0; count<8; count++)
    {
      if((crc^data)&1)
      {
        crc = (crc>>1);
        crc ^= polynomial;
      }
      else
      {
        crc = (crc>>1);
      }
      data >>= 1;
    }
  }

  return crc^0xffffffff;
}

/*

  Function Name:
    sub_pos_times

  Details:
   Subtracts two positive TimeRepresentation values

  Parameters:
       x, y     The times to subtract

       r        The result = (x - y)

  Return Value:
   Nothing

*******************************************************************************/

void sub_pos_times(zl303xx_TimeStamp *r, zl303xx_TimeStamp *x, zl303xx_TimeStamp *y)
{
   switch (Compare_U64S(x->secondsField, y->secondsField))
   {
      case 1 : /* x->seconds > y->seconds */
         r->secondsField = Diff_U64S(x->secondsField, y->secondsField, NULL);

         if (x->nanosecondsField >= y->nanosecondsField)
         {
            r->nanosecondsField = x->nanosecondsField - y->nanosecondsField;
         }
         else /* (x->nanoseconds < y->nanoseconds) */
         {
            if (r->secondsField.lo == 0)
            {
               r->secondsField.hi--;
            }

            r->secondsField.lo--;
            r->nanosecondsField = x->nanosecondsField + TEN_e9 - y->nanosecondsField;
         }
         break;

      case 0 : /* x->seconds == y->seconds */
         if (x->nanosecondsField >= y->nanosecondsField)
         {
            r->secondsField.hi = 0;
            r->secondsField.lo = 0;
            r->nanosecondsField = x->nanosecondsField - y->nanosecondsField;
         }
         else /* (x->nanoseconds < y->nanoseconds) */
         {
            r->secondsField.hi = 0;
            r->secondsField.lo = 0;
            r->nanosecondsField = y->nanosecondsField - x->nanosecondsField;
            negateTime(r);
         }
         break;

      case -1 : /* x->seconds < y->seconds */
         r->secondsField = Diff_U64S(y->secondsField, x->secondsField, NULL);

         if (x->nanosecondsField <= y->nanosecondsField)
         {
            r->nanosecondsField = y->nanosecondsField - x->nanosecondsField;
         }
         else /* (x->nanoseconds > y->nanoseconds) */
         {
            if (r->secondsField.lo == 0)
            {
               r->secondsField.hi--;
            }

            r->secondsField.lo--;
            r->nanosecondsField = y->nanosecondsField + TEN_e9 - x->nanosecondsField;
         }
         negateTime(r);
         break;

      default :
         /* This can never happen. It would be an error in Compare_U64S(). */
         r->secondsField.hi = 0;
         r->secondsField.lo = 0;
         r->nanosecondsField = 0;
         break;
   }

   return;
}

/*

  Function Name:
    add_pos_times

  Details:
   Adds two positive TimeRepresentation values

  Parameters:
       x, y     The times to add

       r        The result = (x + y)

  Return Value:
   Nothing

*******************************************************************************/

void add_pos_times(zl303xx_TimeStamp *r, zl303xx_TimeStamp *x, zl303xx_TimeStamp *y)
{
   /* Assumes x >= 0 and y >= 0. */
   Uint32T nsec = x->nanosecondsField + y->nanosecondsField;
   Uint32T sec = nsec / TEN_e9;

   r->secondsField = Add_U64S(x->secondsField, y->secondsField, NULL);
   r->secondsField.lo += sec;
   if (r->secondsField.lo < sec)
   {
      r->secondsField.hi++;
   }

   r->nanosecondsField = nsec % TEN_e9;
}

/*

  Function Name:
    subTime

  Details:
   Subtracts two signed TimeRepresentation values

  Parameters:
       x, y     The times to subtract

       r        The result = (x - y)

  Return Value:
   Nothing

*******************************************************************************/

void subTime(zl303xx_TimeStamp *r, zl303xx_TimeStamp *x, zl303xx_TimeStamp *y)
{
  zl303xx_TimeStamp t1 = *x;
  zl303xx_TimeStamp t2 = *y;

  negateTime(&t2);
  addTime(r, &t1, &t2);
}

/*

  Function Name:
    addTime

  Details:
   Adds two signed TimeRepresentation values

  Parameters:
       x, y     The times to add

       r        The result = (x + y)

  Return Value:
   Nothing

*******************************************************************************/

void addTime(zl303xx_TimeStamp *r, zl303xx_TimeStamp *x, zl303xx_TimeStamp *y)
{
  zl303xx_TimeStamp t1 = *x;
  zl303xx_TimeStamp t2 = *y;
  zl303xx_BooleanE t1_is_negative = isNegativeTime(&t1);
  zl303xx_BooleanE t2_is_negative = isNegativeTime(&t2);

  if(!t1_is_negative && !t2_is_negative)
  {
    add_pos_times(r, &t1, &t2);
  }
  else if (t1_is_negative && t2_is_negative)
  {
    negateTime(&t1);
    negateTime(&t2);

    add_pos_times(r, &t1, &t2);
    negateTime(r);
  }
  else if(!t1_is_negative)  /* && t2_is_negative */
  {
    negateTime(&t2);

    sub_pos_times(r, &t1, &t2);
  }
  else  /* t1_is_negative && !t2_is_negative */
  {
    negateTime(&t1);

    sub_pos_times(r, &t2, &t1);
  }
}

/*

  Function Name:
    divTime

  Details:
   Divides a signed TimeRepresentation value by a signed 32-bit integer

  Parameters:
       x        The numerator
       y        The divisor

       r        The result = (x/y)

  Return Value:
   Nothing

  Notes:
   This is not a generic divide function. It is simplified to work in the cases
   we use where we can be confident that the simplification works.
   For example it does not work if the time is bigger than 32-bits

*******************************************************************************/

void divTime(zl303xx_TimeStamp *r, zl303xx_TimeStamp *x, Sint32T y)
{
  zl303xx_BooleanE pos = ZL303XX_TRUE;
  zl303xx_TimeStamp numer = *x;  /* numerator */
  Uint32T denom;               /* denominator */

  if (x->secondsField.hi > 0)
  {
     ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 1,
                   "divTime: numerator out of range (x->secondsField.hi=%lu)",
                   x->secondsField.hi, 0,0,0,0,0);

     /* Return an obviously error value */
     r->secondsField.hi = 0xFFFF;
     r->secondsField.lo = 0xFFFFFFFF;
     r->nanosecondsField = 0xFFFFFFFF;
     return;
  }

  /* Make both values positive and work out the sign of the result */
  if(isNegativeTime(&numer))
  {
    negateTime(&numer);

    if (y < 0)
    {
      pos = ZL303XX_TRUE;
    }
    else
    {
      pos = ZL303XX_FALSE;
    }
  }
  else /* n is positive */
  {
    if (y < 0)
    {
      pos = ZL303XX_FALSE;
    }
    else
    {
      pos = ZL303XX_TRUE;
    }
  }

  denom = labs(y); /* Make denominator positive */

  /* Following calculation may introduce a small inaccuracy in some circumstances when n->seconds%denom is large and
     denom is not a factor of 1e9 but this is probably only a few ns error.
     It will work for the case we currently use which is for dividing a network delay value by a small value */
  if (denom != 0)
  {
     r->nanosecondsField = ((TEN_e9+denom/2)/denom)*(numer.secondsField.lo%denom) + (Uint32T)numer.nanosecondsField/denom;
     r->secondsField.hi = 0;
     r->secondsField.lo = numer.secondsField.lo / denom;

     if(!pos)
       negateTime(r);
  }
  else
  {   /* Divide by zero error. Should never happen. */
     ZL303XX_ERROR_NOTIFY("Divide by zero\n");
     /* Return an obviously errored value */
     r->secondsField.hi = 0xFFFF;
     r->secondsField.lo = 0xFFFFFFFF;
     r->nanosecondsField = 0xFFFFFFFF;
  }
}


/*

  Function Name:
    mulTime

  Details:
   Multiplies a signed TimeRepresentation value by an unsigned 32-bit integer

  Parameters:
       x        The multiplicand
       y        The multiplier

       r        The result = (x*y)

  Return Value:
   Nothing

  Notes:
   This is not a generic multiply function. It is currently not optimised for performance since
   we know that it is called very infrequently. Also it does not handle big overflows but it works
   OK in the case we require which is where the multiplier is a small integer.

*******************************************************************************/

void mulTime(zl303xx_TimeStamp *r, zl303xx_TimeStamp *x, Uint32T y)
{
   zl303xx_BooleanE pos = ZL303XX_TRUE;
   zl303xx_TimeStamp n = *x;  /* multiplicand */
   Uint32T bitMask;

   if (y > (Uint32T)0xFFFF)
   {
      ZL303XX_ERROR_NOTIFY("Multiplier out of range");
     /* Return an obviously errored value */
     r->secondsField.hi = 0xFFFF;
     r->secondsField.lo = 0xFFFFFFFF;
     r->nanosecondsField = 0xFFFFFFFF;
   }
   else
   {
      /* Clear the result */
      OS_MEMSET(r, 0, sizeof(zl303xx_TimeStamp));

      /* Make multiplicand positive and work out the sign of the result */
      if(isNegativeTime(&n))
      {
         negateTime(&n);
         pos = ZL303XX_FALSE;
      }
      else /* n is positive */
      {
         pos = ZL303XX_TRUE;
      }

      bitMask = (Uint32T)0x80000000;
      while (bitMask)
      {
          /* Multiply r by 2 (add it to itself) */
         add_pos_times(r, r, r);

         if (y & bitMask)
         {
            /* Add the multiplicand */
            add_pos_times(r, r, &n);
         }
         bitMask = bitMask>>1;
      }
   }

   if (pos == ZL303XX_FALSE)
   {
      negateTime(r);
   }
}


/*

  Function Name:
    negateTime

  Details:
   Negates the supplied TimeRepresentation value

  Parameters:
       x        The time to negate

       x        The result = -(x)

  Return Value:
   Nothing

*******************************************************************************/

void negateTime(zl303xx_TimeStamp *x)
{
  /* Negate the supplied time */
  x->nanosecondsField = x->nanosecondsField ^ 0x80000000;
}

/*

  Function Name:
    isNegativeTime

  Details:
   Determines whether the supplied TimeRepresentation value is negative

  Parameters:
       x        The time to consider

  Return Value:
   ZL303XX_TRUE if the time is negative, ZL303XX_FALSE otherwise

*******************************************************************************/

zl303xx_BooleanE isNegativeTime(zl303xx_TimeStamp *x)
{
   return (x->nanosecondsField & 0x80000000) ? ZL303XX_TRUE : ZL303XX_FALSE;
}

/*

  Function Name:
    isZeroTime

  Details:
   Determines whether the supplied TimeRepresentation value is zero

  Parameters:
       x        The time to consider

  Return Value:
   ZL303XX_TRUE if the time is zero, ZL303XX_FALSE otherwise

*******************************************************************************/

zl303xx_BooleanE isZeroTime(zl303xx_TimeStamp *x)
{
   if ((x->nanosecondsField == 0) && (x->secondsField.lo == 0) && (x->secondsField.hi == 0))
   {
      return ZL303XX_TRUE;
   }
   else
   {
      return ZL303XX_FALSE;
   }
}

/*

  Function Name:
    networkNsToInternalNs

  Details:
   Convert a nanosecond value from a network packet format into internal
   representation format using either the standard IEEE-1588-2002 format or the
   modified (but as yet unapproved) post 2002 format.

  Parameters:
       netNs    The nanoseconds field in network format

  Return Value:
   The nanoseconds field in internal format

  Notes:
   None

*******************************************************************************/
Sint32T networkNsToInternalNs(Uint32T netNs)
{
#ifdef _EXTERNAL_RECEIVE_TR_NANOSECONDS_ARE_UNSIGNED32
   /* No conversion necessary */
   return (Sint32T)netNs;
#else
   /* External representation is "old format" */
   return Int32NanoSecondsToUint32((Sint32T)netNs);
#endif
}

/*

  Function Name:
    internalNsToNetworkNs

  Details:
   Convert a nanosecond value from internal representation format into a network
   packet format into using either the standard IEEE-1588-2002 format or the
   modified (but as yet unapproved) post 2002 format.

  Parameters:
       internalNs   The nanoseconds field in internal format

  Return Value:
   The nanoseconds field in network format

  Notes:
   None

*******************************************************************************/
Uint32T internalNsToNetworkNs(Sint32T internalNs)
{
#ifdef _EXTERNAL_TRANSMIT_TR_NANOSECONDS_ARE_UNSIGNED32
   /* No conversion necessary */
   return (Uint32T)internalNs;
#else
   /* External representation is "old format" */
   return (Uint32T)Uint32NanoSecondsToInt32(internalNs);
#endif
}

/****************   STATIC FUNCTION DEFINITIONS   ****************************/

#ifndef _EXTERNAL_TRANSMIT_TR_NANOSECONDS_ARE_UNSIGNED32
/*

  Function Name:
    Uint32NanoSecondsToInt32

  Details:
   Convert a nanosecond value from a sign-magnitude format into a two's complement
   format.

  Parameters:
       ns    The nanoseconds value in sign-magnitude format

  Return Value:
   The nanoseconds field in two's complement format

  Notes:
   Only required for PTPv1

*******************************************************************************/
Sint32T Uint32NanoSecondsToInt32 (Sint32T ns)
{
   zl303xx_TimeStamp tr;
   Sint32T nano;

   tr.secondsField.hi = 0;
   tr.secondsField.lo = 0;
   tr.nanosecondsField = ns;
   if (isNegativeTime(&tr))
   {
      negateTime(&tr);
      if (tr.nanosecondsField == 0)
      {
         /* Cannot represent -0 nanoseconds in this format so force it to -1.
            N.B. You need -0ns when the value is say -2.0s because of the formatting rules.
            Will result in a 1ns error ! */
        ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 9, "Warning: attempt to represent -0 nanosecond introduces 1ns error",0,0,0,0,0,0);
        nano = -1;
      }
      else
      {
        nano = -(Sint32T)(tr.nanosecondsField);
      }
   }
   else
   {
      nano = ns;
   }
   return nano;
}
#endif   /* !_EXTERNAL_TRANSMIT_TR_NANOSECONDS_ARE_UNSIGNED32 */

#ifndef _EXTERNAL_RECEIVE_TR_NANOSECONDS_ARE_UNSIGNED32
/*

  Function Name:
    Int32NanoSecondsToUint32

  Details:
   Convert a nanosecond value from two's complement format to a sign-magnitude
   format.

  Parameters:
       ns    The nanoseconds field in two's complement formatT

  Return Value:
   he nanoseconds value in sign-magnitude format

  Notes:
   Only required for PTPv1

*******************************************************************************/
Uint32T Int32NanoSecondsToUint32 (Sint32T ns)
{
   zl303xx_TimeStamp tr;

   tr.secondsField.hi = 0;
   tr.secondsField.lo = 0;
   if (ns < 0)
   {
      tr.nanosecondsField = -ns;
      negateTime(&tr);
   }
   else
   {
      tr.nanosecondsField = ns;
   }

   return tr.nanosecondsField;
}

#endif   /* !_EXTERNAL_RECEIVE_TR_NANOSECONDS_ARE_UNSIGNED32 */




