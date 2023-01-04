

/******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     This provides an implementation of the timer needed by the phase slope
*     limit and frequency change limit software components.
*
******************************************************************************/


#include "zl303xx_DataTypes.h"     /* Basic ZL datatypes */
#include "zl303xx_DataTypesEx.h"   /* Extended datatypes specific to this project */
#include "zl303xx_Error.h"
#include "zl303xx_Trace.h"

#ifdef OS_LINUX
#ifdef ZL_LNX_DENX
    #define NEED_POSIX_TIME
    #define NEED_SIGINFO
#endif
#ifdef ZL_LNX_INTEL
    #define NEED_POSIX_TIME
    #define NEED_SIGINFO
#endif
#ifdef ZL_LNX_CODESOURCERY
    #define NEED_SIGINFO
    #define NEED_POSIX_TIME
#endif

#include "zl303xx_LnxVariants.h"
#include "zl303xx_LinuxOs.h"

#endif

#include "zl303xx_Os.h"
#include "zl303xx_OsHeaders.h"

#include "zl303xx_HWTimer.h"



#ifdef OS_LINUX
struct sigaction myAction;
struct sigevent myEvent;
#endif

#ifdef SOCPG_PORTING
#ifdef OS_FREERTOS
#include "mss_timer.h"
#include "system_m2sxxx.h"
#include "mss_gpio.h"

#define USE_HW_TIMER
#ifdef USE_HW_TIMER
void Timer1_IRQHandler(void);
#endif

typedef void (*timer_callout_t)(Sint32T, Sint32T);

timer_callout_t g_timer_callout = 0;

#endif  /* OS_FREERTOS */
#endif  /* SOCPG_PORTING */

/**

  Function Name:
   zl303xx_SetHWTimer

  Details:
   Implements an operating system level periodic timer using POSIX and VxWorks
   interfaces. You could replace this with a H/W timer if you have one.

  Parameters:
   
   rtSignal        Realtime signal number (using Linux Realtime signals or unused in VxWorks)
   timerId         Pointer to timer Id
   osTimeDelayMs   Seconds and/or nanoSeconds - encoded in milliseconds
   callout         Pointer to handler function to call when the timer expires

  Return Value:
    Function should TRAP on any O/S call failure and terminate the entire process
   ZL303XX_OK, or _ZL303XX_ERROR on failure

******************************************************************************/
#ifdef OS_FREERTOS

#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


#define APR_CALLBACK_STACK_SIZE            4096
#define APR_CALLBACK_TASK_PRIORITY         10

/* data */
xTaskHandle apr_timer_task_id = NULL;
BaseType_t xReturned;
static timer_t *callback_array[1]; /* only one required at the moment, modify code if more required */
/* function declarations */
static void prvAPR_timer_task(void (*callout)(Sint32T, Sint32T));

/*
 * Create a task to call callback function on a periodic basis
 * */
static zlStatusE createCallbackTask(timer_t *timerId, void (*callout)(Sint32T, Sint32T))
{

	zlStatusE status = ZL303XX_OK;
	if(callback_array[0] == NULL)
	{
		callback_array[0] = (timer_t *)timerId;
		/* Create the task handling USB mouse emulation. */
		xReturned = xTaskCreate((TaskFunction_t)prvAPR_timer_task,
					(char *) "APR_CALLBACK",
					APR_CALLBACK_STACK_SIZE,
					callout,
					APR_CALLBACK_TASK_PRIORITY,
					&apr_timer_task_id);
		if( xReturned != pdPASS )
		{
			callback_array[0] = (timer_t *)NULL;
			status = ZL303XX_ERROR;
		}
	}
	else
		status = ZL303XX_ERROR;

	return status;
}

/*
 * Task calls callback function on periodic basis
 **/
static void prvAPR_timer_task(void (*callout)(Sint32T, Sint32T))
{
	int timerTicks = OS_TICKS(120); /* 120 milliseconds */
	while (OS_TICK_DELAY(timerTicks) == OS_OK)
	{
		callout(0,0);
	}
}

/*
 * Delete Periodic callback task
 **/
static osStatusT timer_delete(timer_t *timerId)
{
	osStatusT status = OS_OK;

	if(callback_array[0] == timerId)
	{
		 vTaskDelete( apr_timer_task_id );
		 callback_array[0] = (timer_t *)NULL;
	}
	else
		status = OS_ERROR;

	return status;
}

#endif



zlStatusE zl303xx_SetHWTimer(Uint32T rtSignal, timer_t *timerId, Sint32T osTimeDelayMs, void (*callout)(Sint32T, Sint32T), zl303xx_BooleanE periodic)
{
    zlStatusE status = ZL303XX_OK;
    struct itimerspec iTimerSpec;
    struct itimerspec oTimerSpec;
    Uint32T seconds = (osTimeDelayMs / TEN_e3);
    Uint32T nsecs =   (osTimeDelayMs % TEN_e3) * TEN_e6;

    memset(&iTimerSpec, 0, sizeof(struct itimerspec));
    memset(&oTimerSpec, 0, sizeof(struct itimerspec));

    if (!rtSignal)
    {
        ZL303XX_ERROR_TRAP("zl303xx_SetHWTimer: NO rtSignal" );
        return ZL303XX_INVALID_POINTER;
    }
    if (!timerId)
    {
        ZL303XX_ERROR_TRAP("zl303xx_SetHWTimer: NULL timerId" );
        return ZL303XX_INVALID_POINTER;
    }
    if (!callout)
    {
        ZL303XX_ERROR_TRAP("zl303xx_SetHWTimer: NULL callout" );
        return ZL303XX_INVALID_POINTER;
    }

    /* Setup as periodic with delay prior to first call */
    /* printf("zl303xx_SetHWTimer: rtSignal=%d, Seconds=%d, nsecs=%d\n", rtSignal, seconds, nsecs); */
    if (periodic)
    {
        iTimerSpec.it_interval.tv_sec = seconds;
        iTimerSpec.it_interval.tv_nsec = nsecs;  /* Periodic timer */
    }
    iTimerSpec.it_value.tv_sec = seconds;
    iTimerSpec.it_value.tv_nsec = nsecs;     /* Delay prior to first */

#ifdef OS_FREERTOS
#ifdef USE_HW_TIMER
    uint32_t timer1_load_value;

    timer1_load_value = osTimeDelayMs * (g_FrequencyPCLK0  / 1000);

    g_timer_callout = callout;

//    MSS_GPIO_config(MSS_GPIO_16, MSS_GPIO_OUTPUT_MODE);

    if (periodic)
    {
        MSS_TIM1_init(MSS_TIMER_PERIODIC_MODE);
    }
    else
    {
        MSS_TIM1_init(MSS_TIMER_ONE_SHOT_MODE);
    }
    MSS_TIM1_load_immediate(timer1_load_value);
    MSS_TIM1_start();
    MSS_TIM1_enable_irq();
#else
    if( createCallbackTask(timerId, callout) != ZL303XX_OK)
    	ZL303XX_ERROR_TRAP("zl303xx_SetHWTimer: createCallbackTask() failed" );
#endif
#endif

#ifdef OS_VXWORKS
    /* Warning clean-up. */
    (void) rtSignal;

    /* Create timer */
    if (timer_create(CLOCK_MONOTONIC, NULL, timerId) != OS_OK)
    {
        ZL303XX_ERROR_TRAP("zl303xx_SetHWTimer: timer_create() failed" );
    }

    /* Set callout */
    if (timer_connect(*timerId, (VOIDFUNCPTR)callout, (Sint32T)*timerId) != OS_OK)
    {
        ZL303XX_ERROR_TRAP("zl303xx_SetHWTimer: timer_connect() failed" );
    }
    /* Setup as periodic with delay prior to first call */
    if (timer_settime(*timerId, 0, &iTimerSpec, &oTimerSpec) != OS_OK)
    {
        ZL303XX_ERROR_TRAP("zl303xx_SetHWTimer: timer_settime() failed" );
    }
#endif

#ifdef OS_LINUX
    memset(&myEvent, 0, sizeof(myEvent));

#if defined _ZL303XX_OS_SIGNAL_HANDLER
    if (osSignalHandlerRegister(rtSignal, callout) != ZL303XX_OK)
    {
        ZL303XX_ERROR_TRAP( "zl303xx_SetHWTimer: osSignalHandlerRegister() failed" );
    }
#else
    {
        memset(&myAction, 0, sizeof(myAction));

    #ifdef ZL_LNX_DENX
        /* Use a union to get around a warning that assigning a function pointer to an object pointer may cause a problem since
           there is potential for differing size. This is very unlikely */
        {
            typedef void (*funcptr)(Sint32T, Sint32T);
            union
            {
                void (*funcptr) (Sint32T, Sint32T);
                __sighandler_t objptr;
            } u;
            u.funcptr = callout;
            myAction.sa_handler = u.objptr;  /* Enable */
        }
    #else
        myAction.sa_handler = (__sighandler_t) callout;
    #endif

        myAction.sa_flags = SA_RESTART;
        sigaction(rtSignal, &myAction, NULL);  /* Enable */
    }
#endif

    myEvent.sigev_signo = rtSignal;
    myEvent.sigev_notify = SIGEV_SIGNAL;
#if defined _ZL303XX_OS_SIGNAL_HANDLER && defined _LINUX_NP    /* Linux, Non-portable */
    /* If the signals are causing your handler to be killed and you are using Linux,
    you can try this non-POSIX (also non-portable) solution: */

    myEvent.sigev_notify = SIGEV_THREAD_ID; // Deliver via thread creation.
    myEvent._sigev_un._tid = getSignalTaskId();

#endif
    /* create timer */
    if (timer_create(CLOCK_MONOTONIC, (struct sigevent*)&myEvent, timerId) != OS_OK)
    {
        fprintf(stderr, "zl303xx_SetHWTimer: rtSignal=%d, in timerId=%p Seconds=%d, nsecs=%d\n",
               (int)rtSignal, timerId, iTimerSpec.it_value.tv_sec, iTimerSpec.it_value.tv_nsec);
        ZL303XX_ERROR_TRAP( "zl303xx_SetHWTimer: timer_create() failed " );
    }

    if (timer_settime(*timerId, NOT_ABSTIME, &iTimerSpec, &oTimerSpec) != OS_OK)
    {
        fprintf(stderr, "zl303xx_SetHWTimer: rtSignal=%d, in timerId=%p Seconds=%d, nsecs=%d\n",
               (int)rtSignal, timerId, iTimerSpec.it_value.tv_sec, iTimerSpec.it_value.tv_nsec);
        ZL303XX_TRACE_ALWAYS("zl303xx_SetHWTimer: timer_settime() failed; errno=%d", errno, 0,0,0,0,0 );
        ZL303XX_ERROR_TRAP("zl303xx_SetHWTimer: timer_settime() failed" );
    }
#endif  /* OS_LINUX */

    return status;
}


/**

  Function Name:
   zl303xx_DeleteHWTimer

  Details:
   Deletes an operating system level timer periodic timer using POSIX and
   VxWorks interfaces.

  Parameters:
   
        rtSignal        Realtime signal number (using Linux Realtime signals or unused in VxWorks)
        timerId         Timer Id

  Return Value:
    Function should TRAP on any O/S call failure and terminate the entire process
   ZL303XX_OK, or _ZL303XX_ERROR on failure

 *****************************************************************************/

zlStatusE zl303xx_DeleteHWTimer(Uint32T rtSignal, timer_t *timerId)
{
    zlStatusE status = ZL303XX_OK;

    if ((status = ZL303XX_CHECK_POINTER(timerId)) != ZL303XX_OK)
        return status;

#ifdef OS_LINUX
    if (*timerId && timer_delete(*timerId) != OS_OK)
    {
        ZL303XX_ERROR_TRAP("zl303xx_DeleteHWTimer: timer_delete() FAILED");
        return ZL303XX_ERROR;
    }

    *timerId = 0;

    memset(&myAction, 0, sizeof(myAction));
    myAction.sa_handler = SIG_IGN;
    sigaction (rtSignal, &myAction, NULL);  /* Disable */
    OS_TASK_DELAY(10);  /* Signal handling should finish */

  #if defined _ZL303XX_OS_SIGNAL_HANDLER
    osSignalHandlerUnregister(rtSignal);
  #endif

#elif defined(OS_FREERTOS)
 #ifdef USE_HW_TIMER
    if (*timerId && timer_delete(timerId) != OS_OK)
    {
        ZL303XX_ERROR_TRAP("zl303xx_DeleteHWTimer: timer_delete() FAILED");
        return ZL303XX_ERROR;
    }

    *timerId = 0;
    MSS_TIM1_clear_irq();
    MSS_TIM1_disable_irq(); /* todo: Assumption here is disabling is enough, review this */
    g_timer_callout = 0;

 #else
    if (*timerId && timer_delete(timerId) != OS_OK)
    {
        ZL303XX_ERROR_TRAP("zl303xx_DeleteHWTimer: timer_delete() FAILED");
        return ZL303XX_ERROR;
    }

    *timerId = 0;
 #endif
#else
    (void)rtSignal; /* Warning removal */
#endif

  
   return status;
}

#ifdef SOCPG_PORTING
#ifdef OS_FREERTOS
#ifdef USE_HW_TIMER
void Timer1_IRQHandler(void)
{
/*
    static uint8_t gpio_state = 0;

    MSS_GPIO_set_output(MSS_GPIO_16, gpio_state);

    if (0 == gpio_state)
    {
        gpio_state = 1;
    }
    else
    {
        gpio_state = 0;
    }
*/

    if(g_timer_callout != 0)
    {
    	(*g_timer_callout)(0, 0);
    }
    /* Clear TIM1 interrupt */
    MSS_TIM1_clear_irq();
}
#endif
#endif  /* OS_FREERTOS */
#endif  /* SOCPG_PORTING */
