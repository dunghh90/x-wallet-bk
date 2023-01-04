#define SELECTED 1


/******************************************************************************
 *
 *  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
 *
 *  Copyright 2006-2018 Microsemi Semiconductor Limited.
 *  All rights reserved.
 *
 *  Module Description:
 *     This file provides a basic Command Line Interpreter for the ZL303XX_ API.
 *     While the CLI is running, all tasks will remain active.
 *     The CLI may also be run in singleton mode where one command only is processed and
 *     the CLI continues to loop thereby ensuring all child tasks continue to run (~= daemon).
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sched.h>
#include <unistd.h>
#include <mcheck.h>

#include "zl303xx_Global.h"  /* This should always be the first SDK include file */
#include <sched.h>
#include <bits/local_lim.h> /* PTHREAD_STACK_MIN */
#include "zl303xx_Trace.h"
#include "zl303xx_LinuxOs.h"

#if defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
#include "zl303xx_DeviceIf.h"
#endif

#if defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30721_INCLUDED
#include "zl303xx.h"
#endif





#if defined ZLS30341_INCLUDED
   #include "zl303xx_ApiConfig.h"
   #include "zl303xx_Example34x.h"
#endif

#if defined ZLS30361_INCLUDED
   #include "zl303xx_Example36x.h"
#endif

#if defined ZLS30721_INCLUDED
   #include "zl303xx_Example72x.h"
#endif

#if defined ZLS30701_INCLUDED
   #include "zl303xx_Example70x.h"
#endif

#if defined ZLS30751_INCLUDED
   #include "zl303xx_Example75x.h"
#endif

#if defined ZLS30771_INCLUDED
   #include "zl303xx_Example77x.h"
#endif

#if defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
#include "zl303xx_DebugMisc.h"
#endif

#if !(defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED)
#include "zl303xx_ExampleAprGlobals.h"
#endif

/****************************************  EXTERNS  */


#if   defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
extern zl303xx_ParamsS *zl303xx_Params0;   /* The device instance structure */
#endif







/*************************************  DEFINES  *******************/

#ifndef TRUE
    #define TRUE 1
    #define FALSE 0
#endif

#define PROMPT  "\n\n>> "
#define SEPARATORS ", \t\n\r"



/*************************************  GLOBALS  **************************/
char program_name[64];                 /* name of the program (for errors) */
signed int exampleLaunched = -1;       /* -1 indicate not set, 0 indicates launched and all others are error codes */

#if defined ZLS30701_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED
zl303xx_BooleanE zl303xx_BlockNCOWrites = ZL303XX_FALSE;
#endif

/*************************************  LOCAL FUNCTIONS  **************************/
void config(void);
void config(void)
{
    OS_TASK_DELAY(1000);    /* System load dependant, 1000 minimum */
}


#if defined _ZL303XX_OS_SIGNAL_HANDLER
zlStatusE cliExit(void) __attribute__((noreturn));
zlStatusE cliExit(void)
{
    /* Do any required cleanup here */

    exampleShutdown();

    exit(0);
}

void cliHandleQuitSignal(Sint32T sig, Sint32T pid) __attribute__((noreturn));
void cliHandleQuitSignal(Sint32T sig, Sint32T pid)
{

    fprintf(stdout, "\n****** Quit signal %u received from pid %u - stopping application ******\n\n", sig, pid);
    fflush(stdout);

    /* Exit the CLI cleanly */
    cliExit();
}
#endif

/* Converts a hexadecimal string to integer */
int xtoi(const char* xs, Uint32T* result);
int xtoi(const char* xs, Uint32T* result)
{
    size_t szlen = strlen(xs);
    int i, xv, fact;

    if (szlen > 0)
    {
        /* Converting more than 32bit hexadecimal value? */
        if (szlen > 8)
            return 2; /* exit - number too big */

        /* Begin conversion here */
        *result = 0;
        fact = 1;

        /* Run until no more character to convert */
        for(i = szlen-1; i >= 0 ; i--)
        {
            if (isxdigit(*(xs+i)))
            {
                if (*(xs+i)>=97)
                {
                    xv = ( *(xs+i) - 97) + 10;
                }
                else if ( *(xs+i) >= 65)
                {
                    xv = (*(xs+i) - 65) + 10;
                }
                else
                {
                    xv = *(xs+i) - 48;
                }
                *result += (xv * fact);
                fact *= 16;
            }
            else
            {
                /* Conversion was abnormally terminated
                  by non hexadecimal digit, hence
                  returning only the converted with
                  an error value 4 (illegal hex character)
                 */
                return 4;
            }
        }
    }

    /* Nothing to convert */
    return 1;
}

/*
 * usage -- tell the user how to use this program    *
 */
void usage(void);
void usage(void)
{
    fprintf(stderr,"Usage is: %s [singletonCommand]\n", program_name);

/* ---------------------- Example Code -------------------------------------- */
    fprintf(stderr,"Example Code:\n");

#if SELECTED
    #if defined ZLS3034X_SW_VARIANT || defined ZLS30341_INCLUDED
      #if defined ZLS30341_INCLUDED
        fprintf(stderr,"\texample341Start\n");
      #else
        fprintf(stderr,"\texample34xSlave\n");
        fprintf(stderr,"\texample34xSlaveHybrid\n");
        fprintf(stderr,"\texample34xSlaveWarmStart\n");
        fprintf(stderr,"\texample34xMaster\n");
      #endif
        fprintf(stderr,"\texampleShutdown\n");
    #endif

    #if defined ZLS3036X_SW_VARIANT
        fprintf(stderr,"\texample36xSlave\n");
        fprintf(stderr,"\texample36xSlaveHybrid\n");
        fprintf(stderr,"\texample36xMaster\n");
        fprintf(stderr,"\texampleShutdown\n");
    #endif
    #if defined ZLS30361_INCLUDED
        fprintf(stderr,"\texample36x\n");
        fprintf(stderr,"\texampleShutdown\n");
    #endif

    #if defined ZLS3072X_SW_VARIANT
        fprintf(stderr,"\texample72xSlave\n");
        fprintf(stderr,"\texample72xSlaveHybrid\n");
        fprintf(stderr,"\texample72xMaster\n");
        fprintf(stderr,"\texampleShutdown\n");
    #endif
    #if defined ZLS30721_INCLUDED
        fprintf(stderr,"\texample72x\n");
        fprintf(stderr,"\texampleShutdown\n");
    #endif
#endif

#if SELECTED
    #if defined ZLS3070X_SW_VARIANT
        fprintf(stderr,"\texample70xSlave\n");
        fprintf(stderr,"\texample70xSlaveHybrid\n");
        fprintf(stderr,"\texample70xMaster\n");
        fprintf(stderr,"\texampleShutdown\n");
    #endif
    #if defined ZLS30701_INCLUDED
        fprintf(stderr,"\texample70x\n");
        fprintf(stderr,"\texampleShutdown\n");
    #endif
#endif

#if SELECTED
    #if defined ZLS3075X_SW_VARIANT
        fprintf(stderr,"\texample75xSlave\n");
        fprintf(stderr,"\texample75xSlaveHybrid\n");
        fprintf(stderr,"\texample75xMaster\n");
        fprintf(stderr,"\texampleShutdown\n");
    #endif
    #if defined ZLS30751_INCLUDED
        fprintf(stderr,"\texample75x\n");
        fprintf(stderr,"\texampleShutdown\n");
    #endif
#endif


#if SELECTED
    #if defined ZLS3077X_SW_VARIANT
        fprintf(stderr,"\texample77xSlave\n");
        fprintf(stderr,"\texample77xSlaveHybrid\n");
        fprintf(stderr,"\texample77xMaster\n");
        fprintf(stderr,"\texampleShutdown\n");
    #endif
    #if defined ZLS30771_INCLUDED
        fprintf(stderr,"\texample77x\n");
        fprintf(stderr,"\texampleShutdown\n");
    #endif
#endif

#ifdef APR_ONLY
    fprintf(stderr,"\texampleAprMain\n");
    fprintf(stderr,"\texampleAprHybrid\n");
    fprintf(stderr,"\texampleShutdown\n");
#endif

/* ---------------------- Getters and Setters ------------------------------- */
    fprintf(stderr,"\nGetters and Setters:\n");


#if defined APR_ONLY
    fprintf(stderr, "\tenable1Hz\n");
    fprintf(stderr, "\tdisable1Hz\n");
#endif

/* --------------------------- Debug Code ----------------------------------- */
    fprintf(stderr,"\nDebug Code:\n");

    fprintf(stderr,"\tsetTraceLevel mode,level\n");

#if defined APR_ONLY
    fprintf(stderr,"\tdumpRawTS streamId enable secsToLog \n");

    fprintf(stderr,"\tsetAprLogLevel level\n");
#endif

#if defined ZLS30341_INCLUDED
    fprintf(stderr,"\tregRead dev#, page, addr, size, overlay\n");
    fprintf(stderr,"\tregWrite dev#, page, addr, size, data, overlay\n");
#endif


/* --------------------------- Test Code ------------------------------------ */

/* --------------------------- General -------------------------------------- */
    fprintf(stderr,"\nCLI Commands:\n");
    fprintf(stderr,"\tusage/help - show this screen\n");
    fprintf(stderr,"\twait x - delay the CLI task for x milliseconds\n");
    fprintf(stderr,"\tversion - Display version info\n");
    fprintf(stderr,"\texit - stop running the example CLI\n");
}


extern const char zl303xx_ApiReleaseSwId[];
extern const char zl303xx_ApiReleaseVersion[];
extern const char zl303xx_ApiReleaseDate[];
extern const char zl303xx_ApiReleaseTime[];
#if defined APR_ONLY 
extern const char zl303xx_AprReleaseSwId[];
extern const char zl303xx_AprReleaseVersion[];
extern const char zl303xx_AprReleaseDate[];
extern const char zl303xx_AprReleaseTime[];
#endif

#if defined APR_ONLY
void getAllAprStatistics(void);
void getAllAprStatistics(void)
{
   printf("Starting zl303xx_DebugGetAllAprStatistics\n");
   zl303xx_DebugGetAllAprStatistics(zl303xx_Params0);
}
#endif

void printVersionInfo(void);
void printVersionInfo(void)
{
    printf("API Version: %s %s, %s \n", zl303xx_ApiReleaseSwId, zl303xx_ApiReleaseVersion, zl303xx_ApiReleaseDate);
    #if defined APR_ONLY 
    printf("APR Version: %s %s, %s \n", zl303xx_AprReleaseSwId, zl303xx_AprReleaseVersion, zl303xx_AprReleaseDate);
    #endif
}


void setTraceLevel(char * cmd);
void setTraceLevel(char * cmd)
{
    int dbgLevel, dbgMode = 0;

    cmd = strtok(NULL, SEPARATORS);
    if (cmd != NULL)
    {
        dbgMode = atoi(cmd);
        cmd = strtok(NULL, SEPARATORS);
        if (cmd != NULL)
        {
            dbgLevel = atoi(cmd);
            printf("Starting zl303xx_TraceSetLevel [%d ,%d]\n", dbgMode, dbgLevel);
            zl303xx_TraceSetLevel(dbgMode, dbgLevel);
        }
        else
        {
            printf("Invalid Parameter - Debug level could not be determined\n");
        }
    }
    else
    {
        printf("Invalid Parameter - Debug mode could not be determined\n");
    }
}


#if defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED

extern zlStatusE devRead(zl303xx_ParamsS *zl303xx_Params, Uint32T page, Uint32T regAddr, Uint32T regSize, Uint32T overlay);
extern zlStatusE devWrite(zl303xx_ParamsS *zl303xx_Params, Uint32T page, Uint32T regAddr, Uint32T regSize, Uint32T data, Uint32T overlay);

void regRead(char * cmd);
void regRead(char * cmd)
{
    Uint32T dev = 0, page = 0, addr = 0, size = 0, overlay = 0;
    zl303xx_ParamsS *thisDev = NULL;

    /* Device */
    cmd = strtok(NULL, SEPARATORS);
    if (cmd != NULL)
    {
        dev = (Uint32T) atol(cmd);
        if (dev == 0)
        {
            if (zl303xx_Params0 == NULL)
            {
                printf("Device zl303xx_Params0 not configured, please run example code before attempting a register read.\n");
                return;
            }
            else
                thisDev = zl303xx_Params0;
        }
        else
        {
            printf("Device zl303xx_Params%d not configured, please run example code before attempting a register read.\n", dev);
            return;
        }
    }
    else
    {
        printf("Invalid Parameter - dev# could not be determined\n");
        return;
    }

    /* Page */
    cmd = strtok(NULL, SEPARATORS);
    if (cmd != NULL)
    {
        if ((cmd[0] == '0') && (cmd[1] == 'x'))
        {
            int status = 0;
            status = (Uint32T) xtoi(cmd+2, &page);
            if (status != 1)
            {
                printf("Could not convert %s to an integer: status %d\n", cmd, status);
            }
        }
        else
        {
            page = (Uint32T) atol(cmd);
        }
        /* Addr */
        cmd = strtok(NULL, SEPARATORS);
        if (cmd != NULL)
        {
            if ((cmd[0] == '0') && (cmd[1] == 'x'))
            {
                int status = 0;
                status = (Uint32T) xtoi(cmd+2, &addr);
                if (status != 1)
                {
                    printf("Could not convert %s to an integer: status %d\n", cmd, status);
                }
            }
            else
            {
                addr = (Uint32T) atol(cmd);
            }
            /* Size */
            cmd = strtok(NULL, SEPARATORS);
            if (cmd != NULL)
            {
                size = (Uint32T) atol(cmd);
                /* Overlay */
                cmd = strtok(NULL, SEPARATORS);
                if (cmd != NULL)
                {
                    overlay = (Uint32T) atol(cmd);
                    devRead(thisDev, page, addr, size, overlay);
                }
                else
                {
                    printf("Invalid Parameter - Overlay could not be determined\n");
                }
            }
            else
            {
                printf("Invalid Parameter - Register size could not be determined\n");
            }
        }
        else
        {
            printf("Invalid Parameter - Register address could not be determined\n");
        }
    }
    else
    {
        printf("Invalid Parameter - Register page could not be determined\n");
    }
}

void regWrite(char * cmd);
void regWrite(char * cmd)
{
    Uint32T dev = 0, page = 0, addr = 0, size = 0, overlay = 0, data = 0;
    zl303xx_ParamsS *thisDev = NULL;

    /* Device */
    cmd = strtok(NULL, SEPARATORS);
    if (cmd != NULL)
    {
        dev = (Uint32T) atol(cmd);
        if (dev == 0)
        {
            if (zl303xx_Params0 == NULL)
            {
                printf("Device zl303xx_Params0 not configured, please run example code before attempting a register read.\n");
                return;
            }
            else
                thisDev = zl303xx_Params0;
        }
        else {
            printf("Device zl303xx_Params%d not configured, please run example code before attempting a register read.\n", dev);
            return;
        }
    }
    else
    {
        printf("Invalid Parameter - dev# could not be determined\n");
        return;
    }

    /* Page */
    cmd = strtok(NULL, SEPARATORS);
    if (cmd != NULL)
    {
        if ((cmd[0] == '0') && (cmd[1] == 'x'))
        {
            int status = 0;
            status = (Uint32T) xtoi(cmd+2, &page);
            if (status != 1)
            {
                printf("Could not convert %s to an integer: status %d\n", cmd, status);
            }
        }
        else
        {
            page = (Uint32T) atol(cmd);
        }
        /* Addr */
        cmd = strtok(NULL, SEPARATORS);
        if (cmd != NULL)
        {
            if ((cmd[0] == '0') && (cmd[1] == 'x'))
            {
                int status = 0;
                status = (Uint32T) xtoi(cmd+2, &addr);
                if (status != 1)
                {
                    printf("Could not convert %s to an integer: status %d\n", cmd, status);
                }
            }
            else
            {
                addr = (Uint32T) atol(cmd);
            }
            /* Size */
            cmd = strtok(NULL, SEPARATORS);
            if (cmd != NULL)
            {
                size = (Uint32T) atol(cmd);
                /* Data */
                cmd = strtok(NULL, SEPARATORS);
                if (cmd != NULL)
                {
                    if ((cmd[0] == '0') && (cmd[1] == 'x'))
                    {
                        int status = 0;
                        status = (Uint32T) xtoi(cmd+2, &data);
                        if (status != 1)
                        {
                            printf("Could not convert %s to an integer: status %d\n", cmd, status);
                        }
                    }
                    else
                    {
                        data = (Uint32T) atol(cmd);
                    }
                    /* Overlay */
                    cmd = strtok(NULL, SEPARATORS);
                    if (cmd != NULL)
                    {
                        overlay = (Uint32T) atol(cmd);
                        devWrite(thisDev, page, addr, size, data, overlay);
                    }
                    else
                    {
                        printf("Invalid Parameter - Overlay could not be determined\n");
                    }
                }
            }
            else
            {
                printf("Invalid Parameter - Register size could not be determined\n");
            }
        }
        else
        {
            printf("Invalid Parameter - Register address could not be determined\n");
        }
    }
    else
    {
        printf("Invalid Parameter - Register page could not be determined\n");
    }
}

#endif


#if defined APR_ONLY
void setAprLogLevel(char * cmd);
void setAprLogLevel(char * cmd)
{
    int level = 0;

    cmd = strtok(NULL, SEPARATORS);
    if (cmd != NULL)
    {
        level = atoi(cmd);
        printf("Setting zl303xx_SetAprLogLevel to level %d\n", level);
        zl303xx_SetAprLogLevel(level);
    }
    else
    {
        printf("Invalid Parameter - Log Level could not be determined\n");
    }
}
/**
 * Enables or disables 1Hz tracking in APR
 */
void enable1Hz(zl303xx_BooleanE bEnable);
void enable1Hz(zl303xx_BooleanE bEnable)
{
    zlStatusE status = ZL303XX_ERROR;

    status = exampleAprSet1HzEnabled(zl303xx_Params0, bEnable);

    if (status != ZL303XX_OK)
    {
       printf("enable1Hz() failed with status = %lu\n", (Uint32T) status);
    }
    else if (bEnable == ZL303XX_TRUE)
        printf("enable1Hz() enabled\n");
    else
        printf("enable1Hz() disabled\n");
}



#endif



/***************************************************************************
 **                              M A I N                                  **
 ***************************************************************************/
int main(int argc, char *argv[])
{
    zl303xx_BooleanE singleMode = ZL303XX_FALSE;
    unsigned int ncmds = argc - 1;  /* Number of commands left to process */

    char buff[2048];
    char *cmd;
    struct sched_param schedParam;

    /* Set the default device type */
  #if defined ZLS30341_INCLUDED
    zl303xx_SetDefaultDeviceType(ZL3034X_DEVICETYPE);
  #elif defined ZLS30361_INCLUDED
    zl303xx_SetDefaultDeviceType(ZL3036X_DEVICETYPE);
  #elif defined ZLS30701_INCLUDED
    zl303xx_SetDefaultDeviceType(ZL3070X_DEVICETYPE);
  #elif defined ZLS30721_INCLUDED
    zl303xx_SetDefaultDeviceType(ZL3072X_DEVICETYPE);
  #elif defined ZLS30751_INCLUDED
    zl303xx_SetDefaultDeviceType(ZL3075X_DEVICETYPE);
  #elif defined ZLS30771_INCLUDED
    zl303xx_SetDefaultDeviceType(ZL3077X_DEVICETYPE);
  #else
    printf("MSCC Error: No DEFAULT DEVICE TYPE set!\n");
  #endif


    /* Setup stdout and stderr to be line buffered (better for file redirection) */
    (void)setvbuf(stdout, (char *)NULL, _IOLBF, 0);
    (void)setvbuf(stderr, (char *)NULL, _IOLBF, 0);

    /* Save the program name for possible future use */
    memset(program_name, 0x00, sizeof(program_name));
    memcpy(program_name, argv[0], sizeof(program_name)-1);

    printf("%s built on %s %s\n", program_name, __DATE__, __TIME__);
    printf("\n");

    printVersionInfo();

    /* Check for a single argument which would be singleton command such as one of the examples */
    if (argc > 1)
    {
        singleMode = ZL303XX_TRUE;
        fprintf(stderr, "LinuxCli: Singleton command mode\n");
    }

    printf(PROMPT);

    while((singleMode == ZL303XX_TRUE) || (fgets(buff,sizeof(buff),stdin) != NULL) || 1)
    {
        if (ncmds > 0)
        {
            /* Process each command from the arguments */
            printf("Processing command \"%s\"\n", argv[argc - ncmds]);
            strncpy(buff, argv[argc - ncmds], sizeof(buff));
            ncmds--;
        }

        cmd = strtok(buff, SEPARATORS);

        if (cmd == NULL)
        {
            printf(PROMPT);
            continue;
        }


#if SELECTED
    #if defined ZLS3034X_SW_VARIANT || defined ZLS30341_INCLUDED
       #if defined ZLS30341_INCLUDED
            else if(!strcmp(cmd,"example341Start"))
            {
                exampleLaunched = example341Start();
            }
       #else
            else if(!strcmp(cmd,"example34xSlave"))
            {
                exampleLaunched = example34xSlave();
            }
            else if(!strcmp(cmd,"example34xSlaveHybrid"))
            {
                exampleLaunched = example34xSlaveHybrid();
            }
            else if(!strcmp(cmd,"example34xSlaveWarmStart"))
            {
                exampleLaunched = example34xSlaveWarmStart();
            }
            else if(!strcmp(cmd,"example34xMaster"))
            {
                exampleLaunched = example34xMaster();
            }
        #endif
            else if(!strcmp(cmd,"exampleShutdown"))
            {
                exampleShutdown();
            }
    #endif

    #if defined ZLS3036X_SW_VARIANT
            else if(!strcmp(cmd,"example36xSlave"))
            {
                exampleLaunched = example36xSlave();
            }
            else if(!strcmp(cmd,"example36xSlaveHybrid"))
            {
                exampleLaunched = example36xSlaveHybrid();
            }
            else if(!strcmp(cmd,"example36xMaster"))
            {
                exampleLaunched = example36xMaster();
            }
            else if(!strcmp(cmd,"exampleShutdown"))
            {
                exampleShutdown();
            }
    #endif
    #if defined ZLS30361_INCLUDED
            else if(!strcmp(cmd,"example36x"))
            {
                exampleLaunched = example36x();
            }
            else if(!strcmp(cmd,"exampleShutdown"))
            {
                exampleShutdown();
            }
    #endif

    #if defined ZLS3072X_SW_VARIANT
            else if(!strcmp(cmd,"example72xSlave"))
            {
                exampleLaunched = example72xSlave();
            }
            else if(!strcmp(cmd,"example72xSlaveHybrid"))
            {
                exampleLaunched = example72xSlaveHybrid();
            }
            else if(!strcmp(cmd,"example72xMaster"))
            {
                exampleLaunched = example72xMaster();
            }
            else if(!strcmp(cmd,"exampleShutdown"))
            {
                exampleShutdown();
            }
    #endif
    #if defined ZLS30721_INCLUDED
            else if(!strcmp(cmd,"example72x"))
            {
                exampleLaunched = example72x();
            }
            else if(!strcmp(cmd,"exampleShutdown"))
            {
                exampleShutdown();
            }
    #endif

    #if defined ZLS3070X_SW_VARIANT
            else if(!strcmp(cmd,"example70xSlave"))
            {
                exampleLaunched = example70xSlave();
            }
            else if(!strcmp(cmd,"example70xSlaveHybrid"))
            {
                exampleLaunched = example70xSlaveHybrid();
            }
            else if(!strcmp(cmd,"example70xMaster"))
            {
                exampleLaunched = example70xMaster();
            }
            else if(!strcmp(cmd,"exampleShutdown"))
            {
                exampleShutdown();
            }
    #endif
    #if defined ZLS30701_INCLUDED
            else if(!strcmp(cmd,"example70x"))
            {
                exampleLaunched = example70x();
            }
            else if(!strcmp(cmd,"exampleShutdown"))
            {
                exampleShutdown();
            }
    #endif

    #if defined ZLS3075X_SW_VARIANT
            else if(!strcmp(cmd,"example75xSlave"))
            {
                exampleLaunched = example75xSlave();
            }
            else if(!strcmp(cmd,"example75xSlaveHybrid"))
            {
                exampleLaunched = example75xSlaveHybrid();
            }
            else if(!strcmp(cmd,"example75xMaster"))
            {
                exampleLaunched = example75xMaster();
            }
            else if(!strcmp(cmd,"exampleShutdown"))
            {
                exampleShutdown();
            }
    #endif
    #if defined ZLS30751_INCLUDED
            else if(!strcmp(cmd,"example75x"))
            {
                exampleLaunched = example75x();
            }
            else if(!strcmp(cmd,"exampleShutdown"))
            {
                exampleShutdown();
            }
    #endif

    #if defined ZLS3077X_SW_VARIANT
            else if(!strcmp(cmd,"example77xSlave"))
            {
                exampleLaunched = example77xSlave();
            }
            else if(!strcmp(cmd,"example77xSlaveHybrid"))
            {
                exampleLaunched = example77xSlaveHybrid();
            }
            else if(!strcmp(cmd,"example77xMaster"))
            {
                exampleLaunched = example77xMaster();
            }
            else if(!strcmp(cmd,"exampleShutdown"))
            {
                exampleShutdown();
            }
    #endif
    #if defined ZLS30771_INCLUDED
            else if(!strcmp(cmd,"example77x"))
            {
                exampleLaunched = example77x();
            }
            else if(!strcmp(cmd,"exampleShutdown"))
            {
                exampleShutdown();
            }
    #endif
#endif


#ifdef APR_ONLY
        else if(!strcmp(cmd,"exampleAprMain"))
        {
            exampleLaunched = exampleAprMain();
        }
        else if(!strcmp(cmd,"exampleAprHybrid"))
        {
            exampleLaunched = exampleAprHybrid();
        }
        else if(!strcmp(cmd,"exampleShutdown"))
        {
            exampleLaunched = exampleShutdown();
        }
#endif


#if defined ZLS30341_INCLUDED
        else if(!strcmp(cmd,"regRead"))
        {
            regRead(cmd);
        }
        else if(!strcmp(cmd,"regWrite"))
        {
            regWrite(cmd);
        }
#endif


#if defined APR_ONLY
        else if((!strcmp(cmd,"setAprLogLevel")) ||  (!strcmp(cmd,"al")))
        {
            setAprLogLevel(cmd);
        }
        else if(!strcmp(cmd,"getAllAprStatistics") || !strcmp(cmd,"aas"))
        {
           getAllAprStatistics();
        }
        else if(!strcmp(cmd,"getAprServerStatus") || !strcmp(cmd,"ass"))
        {
           zl303xx_GetAprServerStatus(zl303xx_Params0, 0);
           zl303xx_GetAprDeviceStatus(zl303xx_Params0);
        }
        else if((!strcmp(cmd,"enable1Hz")) ||  (!strcmp(cmd,"e1hz")))
        {
            enable1Hz(ZL303XX_TRUE);
        }
        else if((!strcmp(cmd,"disable1Hz")) ||  (!strcmp(cmd,"d1hz")))
        {
            enable1Hz(ZL303XX_FALSE);
        }
#endif

#if defined ZLS30341_INCLUDED && !(defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED)
        else if((!strcmp(cmd,"wide1Hz")) ||  (!strcmp(cmd,"w1hz")))
        {
            wide1Hz();
        }
#endif


#if !(defined ZLS30341_INCLUDED || defined ZLS30361_INCLUDED || defined ZLS30701_INCLUDED || defined ZLS30721_INCLUDED || defined ZLS30751_INCLUDED || defined ZLS30771_INCLUDED)
        else if(!strcmp(cmd,"dumpRawTS") || !strcmp(cmd,"rts"))
        {
            int enable = 0, streamId = 0, secsToLog = 0;

            cmd = strtok(NULL, SEPARATORS);
            if (cmd != NULL)
            {
                streamId = atoi(cmd);
                cmd = strtok(NULL, SEPARATORS);
                if (cmd != NULL)
                {
                    enable = atoi(cmd);
                    cmd = strtok(NULL, SEPARATORS);
                    if (cmd != NULL)
                    {
                        secsToLog = atoi(cmd);
                        if (enable)
                            exampleEnableTSLoggingOnServer(streamId, secsToLog);
                        else
                            exampleDisableTSLoggingOnServer (streamId);
                    }
                    else
                    {
                        if (enable)
                            printf("Invalid Parameter - secondsToLog could not be determined\n");
                        else
                            exampleDisableTSLoggingOnServer(streamId);
                    }
                }
                else
                {
                    printf("Invalid Parameter - Enable/Disable boolean could not be determined\n");
                }
            }
            else
            {
                printf("Invalid Parameter - Server Id could not be determined\n");
            }
        }
#endif

        else if(!strcmp(cmd,"config"))
        {
            config();
        }
        else if(!strcmp(cmd,"schedMinMax"))
        {
            printf("Sched min=%d, max=%d\n", sched_get_priority_min(SCHED_POLICY), sched_get_priority_max(SCHED_POLICY));    /* SCHED_FIFO or SCHED_RR */
        }
        else if((!strcmp(cmd,"setTraceLevel")) ||  (!strcmp(cmd,"st")))
        {
            setTraceLevel(cmd);
        }
        else if((!strcmp(cmd,"usage")) ||  (!strcmp(cmd,"help")))
        {
            usage();
        }
        else if(!strcmp(cmd,"wait"))
        {
            if((cmd = strtok(NULL, SEPARATORS)) == NULL)
            {
                printf("Missing wait time argument\n");
            }
            else
            {
                Uint32T t = atoi(cmd);
                printf("Waiting %d...\n", t);
                OS_TASK_DELAY(t);
            }
        }
        else if(!strcmp(cmd,"version") || !strcmp(cmd,"versions") || !strcmp(cmd,"ver"))
        {
            printVersionInfo();
        }
        else if(!strcmp(cmd,"exit"))
        {
            break;
        }
        else
        {
            printf("Command not found\n");
        }

        if (singleMode == ZL303XX_TRUE && ncmds == 0)
        {
            /* If we are done processing cmd line args then wait for input from stdin */
            singleMode = ZL303XX_FALSE;
        }


        sleep(1);
        printf(PROMPT);

        memset(buff, 0, sizeof(buff));

    }   /* while */

    printf("\nExiting CLI, Goodbye!\n");

    return (0);
}


