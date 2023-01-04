#ifndef F_SAA_MSG_BUFFER
#define F_SAA_MSG_BUFFER

#include <stdint.h>

#define D_MSG_BUFFER_ADD_OK 0
#define D_MSG_BUFFER_ADD_ERR (-1)

#define D_MSG_BUFFER_GET_EMPTY 1
#define D_MSG_BUFFER_GET_OK 0
#define D_MSG_BUFFER_GET_ERR_SIZE (-1)
#define D_MSG_BUFFER_GET_ERR_TIMEOUT (-2)
#define D_MSG_BUFFER_GET_ERR_OTHER (-3)

/* レスポンス待ち時間の標準値(秒) */
#define D_MSG_WAIT_TIME 10

/* メッセージ用バッファサイズの標準値 */
#define D_MSG_BUFFER_SIZE  512

extern uint32_t f_saa_com_add_request(uint32_t message_id, uint32_t datasize, void *data);
extern int f_saa_com_get_request(uint32_t max_size, uint32_t *request_id, uint32_t *message_id, uint32_t *read_size, void *data);
extern int f_saa_com_add_response(uint32_t request_id, uint32_t message_id, uint32_t datasize, void *data);
extern int f_saa_com_get_response(uint32_t request_id, uint32_t max_size, uint32_t *message_id, uint32_t *read_size, void *data, uint32_t wait_time);
extern int f_saa_com_add_notification(uint32_t message_id, uint32_t datasize, void *data);
extern int f_saa_com_get_notification(uint32_t max_size, uint32_t *message_id, uint32_t *read_size, void *data);

extern void f_saa_com_notification_lock(void);
extern void f_saa_com_notification_unlock(void);

#endif
