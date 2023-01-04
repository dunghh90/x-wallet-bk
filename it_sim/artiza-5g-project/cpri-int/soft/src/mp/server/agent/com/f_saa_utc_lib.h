#ifndef F_SAA_UTC_LIB
#define F_SAA_UTC_LIB

#include <stdint.h>

/** @name UTC-STRING MAX_文字数
 *  @note init_validate文字数最大値
 *  @{
 */
#define D_UTC_STRING_NUM_MAX 26  /**< UTC文字数最大値 */
/** @} */

extern int f_saa_com_set_utc_offset(int16_t offset);
extern int16_t f_saa_com_get_utc_offset(void);
extern int f_saa_com_utc_to_offset_string(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec, char *out_buffer);

#endif
