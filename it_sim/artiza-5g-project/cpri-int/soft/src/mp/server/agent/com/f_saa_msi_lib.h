#ifndef F_SAA_MSI_LIB
#define F_SAA_MSI_LIB

#include <stdint.h>

#define D_MSG_MSI_CFM 0
#define D_MSG_MSI_REJ (-1) 
#define D_MSG_MSI_ERR (-2)

#define D_MSI_CFM                    0  //正常
#define D_MSI_REJ                  (-1) //拒否
#define D_MSI_WRONG_RESPONSE       (-2) //不正なレスポンス
#define D_MSI_REQUEST_ERR          (-3) //リクエスト送信異常
#define D_MSI_RESPONSE_ERR_SIZE    (-4) //レスポンス受信異常(サイズ不足)
#define D_MSI_RESPONSE_ERR_TIMEOUT (-5) //レスポンス受信異常(タイムアウト)
#define D_MSI_RESPONSE_ERR_OTHER   (-6) //レスポンス受信異常(その他)

extern int f_saa_com_do_msi_request(uint32_t message_id, uint32_t data_size, void *request_buffer, uint32_t cfm_id, uint32_t rej_id, uint32_t max_size, void *response_buffer, uint32_t *read_size);
#endif
