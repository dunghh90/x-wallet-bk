/*!
 *  @skip	$Id:$
 *  @file	f_saa_dbapi_main.c
 *  @brief	SAA_DBAPI Set initial value in datastore.
 *  @author	KCN) Nanjo Shota
 *  @date	2019/03/22 KCN)Nanjo create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */


/*!
 * @addtogroup	SAA_DBAPI
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

#define DBAPI_PARAM_NUM 3

#define DBAPI_SUBSYS_ID     1
#define DBAPI_VALUE         2
#define DBAPI_DEBUG_LEVEL   3

/*!
 * @brief	main
 * @note	db_api_send_edit_full2をCallする
 * @param	argc 起動パラメータ数
 * @param	argv 起動パラメータリスト
 * @return	0:正常終了、0以外:以上終了
 * @date	2019/03/22 KCN)Nanjo create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
int main (int argc, char **argv)
{
    /* argv[1] : subsys-id   */
    /* argv[2] : value       */
    /* argv[3] : debug-level */
    if(argc != (DBAPI_PARAM_NUM + 1))
    {
        return -1;
    }

    int argc_yuma = argc - DBAPI_PARAM_NUM;
    char **argv_yuma;
    argv_yuma = malloc(sizeof(char *) * argc_yuma);
    if(argv_yuma == NULL)
    {
        return -1;
    }
    argv_yuma[0] = argv[0];
    argv_yuma[1] = argv[DBAPI_DEBUG_LEVEL];

    do
    {
        const xmlChar *subsys_id = (const xmlChar *)argv[DBAPI_SUBSYS_ID];
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

        const xmlChar *path_str = (const xmlChar *)"/";
        const xmlChar *operation_str = (const xmlChar *)"merge";
        const xmlChar *value_str = (const xmlChar *)argv[DBAPI_VALUE];
        const xmlChar *patch_id_str = NULL;
        boolean system_edit = FALSE;
        const xmlChar *insert_point = NULL;
        const xmlChar *insert_where = NULL;

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
                (void)usleep(100000);
            }

            if (db_api_service_ready() && !test_done)
            {
                res =db_api_send_edit_full2(path_str,
                                           operation_str,
                                           value_str,
                                           patch_id_str,
                                           system_edit,
                                           insert_point,
                                           insert_where,
                                           TRUE);
                test_done = TRUE;
                if(res != NO_ERR)
                {
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

