#ifndef F_SAA_NTF_LIB
#define F_SAA_NTF_LIB

#include <stdint.h>

extern void f_saa_com_init_notification_table(void);
extern void f_saa_com_reg_notification_handler(uint32_t message_id, void (* handler)(void *data, uint32_t size));
extern void(*f_saa_com_get_notification_handler(uint32_t message_id))(void *data, uint32_t size);
extern void f_saa_com_log_notification(uint32_t message_id, uint32_t size, void *data, void (*handler)(void *, uint32_t));

#endif
