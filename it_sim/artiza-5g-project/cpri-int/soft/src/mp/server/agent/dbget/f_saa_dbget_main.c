/*!
 *  @skip	$Id:$
 *  @file	f_saa_dbget_main.c
 *  @brief	SAA_DBGET get from datastore.
 *  @author	KCN) Nanjo Shota
 *  @date	2019/07/03 KCN)Nanjo create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */


/*!
 * @addtogroup	SAA_DBGET
 * @brief
 * @note
 * @{
 */

#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <xmlstring.h>
#include "log.h"
#include "db_api.h"
#include "ycontrol.h"

/*!
 * @brief	main
 * @note	db_api_send_getfilterをCallする
 * @param	argc 起動パラメータ数
 * @param	argv 起動パラメータリスト
 * @return	0:正常終了、0以外:以上終了
 * @date	2019/07/03 KCN)Nanjo create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
int main (int argc, char **argv)
{
    if(argc != 1)
    {
        return -1;
    }

    char debug_level[] = "--log-level=off";
    int argc_yuma = argc;
    char **argv_yuma;
    argv_yuma = malloc(sizeof(char *) * argc_yuma);
    if(argv_yuma == NULL)
    {
        return -1;
    }
    argv_yuma[0] = argv[0];
    argv_yuma[1] = &debug_level[0];

    do
    {
        const xmlChar *subsys_id = (const xmlChar *)"subsys_dbget";
        /* (1) setup yumapro messaging service profile */
        status_t res = ycontrol_init(argc_yuma, argv_yuma, subsys_id);
        if (res != NO_ERR) {
            break;
        }

        /* (2) register services with the control layer */
        res = db_api_register_service();
        if (res != NO_ERR) {
            break;
        }

        /* (3) do 2nd stage init of the control manager (connect to server) */
        res = ycontrol_init2();
        if (res != NO_ERR) {
            break;
        }

        int wait_count = 0;
        boolean done = FALSE;
        boolean test_done = FALSE;
        const xmlChar *error_msg = NULL;
        while (!done && res == NO_ERR)
        {
            res = ycontrol_check_io();
            if (res != NO_ERR) {
                continue;
            }

            if (ycontrol_shutdown_now()) {
                // YControl has received a <shutdown-event>
                // from the server subsystem is no longer active
                // could ignore or shut down YControl IO loop
                done = TRUE;
            }

            // Using sleep to represent other program work; remove for real
            if (!done && res == NO_ERR) {
                wait_count++;
                if(wait_count == 50)
                {
                    /* 5s経過後応答ない場合は中断 */
                    break;
                }
                (void)usleep(100000);
            }

            if (db_api_service_ready() && !test_done)
            {
                res = db_api_send_getfilter((const xmlChar *)"/var/log/mp_server_datastore.xml",
                                            false,
                                            true,
                                            (const xmlChar *)"/");
                test_done = TRUE;
                if (res != NO_ERR) {
                    break;
                }
            }
            else if(db_api_service_ready() && test_done)
            {
                /* check the test edit */
                res = db_api_check_edit_ex(&error_msg);
                done = TRUE;
            }
        }
    }while(0);

    /* 5) cleanup the control layer before exit */
    ycontrol_cleanup();

    if(argv_yuma != NULL)
    {
        free(argv_yuma);
    }

    return 0;
}