/*
 * @file    i_o-ran-beamforming.h
 */

#ifndef _H_i_o_ran_beamforming
#define _H_i_o_ran_beamforming


/*!
 * @addtogroup  SAA_MOD_O_RAN_BEAMFORMING
 * @brief       SAA o-ran-beamforming
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <stdlib.h>
#include <string.h>

/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define i_mod_beamforming_config                                "o-ran-beamforming"
#define i_mod_o_ran_module_cap                                  "o-ran-module-cap"

#define i_para_o_ran_beamforming_max_ant_num                    "max_ant_num"
#define i_para_o_ran_beamforming_band_number_init               "max_band_num"

#define i_para_o_ran_beamforming_per_band_cong                  "beamforming-config.per-band-config."
#define i_para_o_ran_beamforming_band_number                    ".band-number"
#define i_para_o_ran_beamforming_txarray                        ".tx-array"
#define i_para_o_ran_beamforming_rxarray                        ".rx-array"
#define i_para_o_ran_beamforming_max_properties                 ".beam-information.number-of-beamforming-properties"

#define i_para_o_ran_beamforming_rt_bf_weights_update_support   "beamforming-config.per-band-config.0.static-properties.rt-bf-weights-update-support"
#define i_para_o_ran_beamforming_num_of_beam                    "beamforming-config.per-band-config.0.static-properties.number-of-beams"

#define i_para_o_ran_beamforming_band_config                    "beamforming-config.per-band-config.0.beam-information.beamforming-properties."
#define i_para_o_ran_beamforming_beam_id                        ".beam-id"
#define i_para_o_ran_beamforming_beam_type                      ".beamforming-property.beam-type"
#define i_para_o_ran_beamforming_beam_groups_id                 ".beamforming-property.beam-groups-id"
#define i_para_o_ran_beamforming_neighbor_beam                  ".beamforming-property.neighbor-beam."

#define i_para_o_ran_beamforming_type                           "beamforming-config.per-band-config.0.static-properties.beamforming-type"
#define i_para_o_ran_beamforming_frequency_max_num_beam_id      "beamforming-config.per-band-config.0.static-properties.beamforming-type.frequency.frequency-domain-beams.max-number-of-beam-id"
#define i_para_o_ran_beamforming_frequency_initial_beam_id      "beamforming-config.per-band-config.0.static-properties.beamforming-type.frequency.frequency-domain-beams.initial-beam-id"
#define i_para_o_ran_beamforming_frequency_compression_type     "beamforming-config.per-band-config.0.static-properties.beamforming-type.frequency.frequency-domain-beams.compression-type"
#define i_para_o_ran_beamforming_frequency_bitwidth             "beamforming-config.per-band-config.0.static-properties.beamforming-type.frequency.frequency-domain-beams.bitwidth"
#define i_para_o_ran_beamforming_frequency_exponent             "beamforming-config.per-band-config.0.static-properties.beamforming-type.frequency.frequency-domain-beams.compression-format.block-floating-point.exponent"
#define i_para_o_ran_beamforming_time_max_num_beam_id           "beamforming-config.per-band-config.0.static-properties.beamforming-type.time.time-domain-beams.max-number-of-beam-id"
#define i_para_o_ran_beamforming_time_initial_beam_id           "beamforming-config.per-band-config.0.static-properties.beamforming-type.time.time-domain-beams.initial-beam-id"
#define i_para_o_ran_beamforming_time_frequency_granularity     "beamforming-config.per-band-config.0.static-properties.beamforming-type.time.time-domain-beams.frequency-granularity"
#define i_para_o_ran_beamforming_time_time_granularity          "beamforming-config.per-band-config.0.static-properties.beamforming-type.time.time-domain-beams.time-granularity"
#define i_para_o_ran_beamforming_time_compression_type          "beamforming-config.per-band-config.0.static-properties.beamforming-type.time.time-domain-beams.compression-type"
#define i_para_o_ran_beamforming_time_bitwidth                  "beamforming-config.per-band-config.0.static-properties.beamforming-type.time.time-domain-beams.bitwidth"
#define i_para_o_ran_beamforming_time_exponent                  "beamforming-config.per-band-config.0.static-properties.beamforming-type.time.time-domain-beams.compression-format.block-floating-point.exponent"

/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/
extern uint32 saa_beamforming_max_ant_num ;
extern uint32 saa_beamforming_max_band_num ;

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

extern  status_t i_o_ran_beamforming_beamforming_config_per_band_config_static_properties_get(  const xmlChar   *target_para_name ,
                                                                                                xmlChar         **para_temp ) ;
extern  status_t i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_beamforming_properties_beamforming_property_get_para(  uint16          beam_id_index ,
                                                                                                                                                const xmlChar   *target_para_name,
                                                                                                                                                xmlChar         **para_temp ) ;
extern  status_t i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_beamforming_properties_beamforming_property_get_para2( uint16          beam_id_index ,
                                                                                                                                                unsigned int    list_counter ,
                                                                                                                                                const xmlChar   *target_para_name,
                                                                                                                                                xmlChar         **para_temp ) ;
extern status_t i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_beamforming_properties_get_key_num( uint32 *curindex ,
                                                                                                                            uint32  max_num_of_use ,
                                                                                                                            const xmlChar *temp_key_name ) ;
extern status_t i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_beamforming_properties_get_first_key(   xmlChar **key_name_temp ) ;
extern status_t i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_beamforming_properties_get_target_key(  uint32 curindex ,
                                                                                                                                xmlChar **key_name_temp ) ;
extern status_t i_o_ran_beamforming_beamforming_config_per_band_config_get_key_num( uint32 *curindex ,
                                                                                    uint32  max_num_of_use ,
                                                                                    const xmlChar * temp_key_name ) ;
extern status_t i_o_ran_beamforming_beamforming_config_per_band_config_get_first_key(   xmlChar **key_name_temp ) ;
extern status_t i_o_ran_beamforming_beamforming_config_per_band_config_get_target_key(  uint32  curindex ,
                                                                                        xmlChar **key_name_temp ) ;
extern status_t i_o_ran_beamforming_beamforming_config_per_band_config_get_para(    uint32 curindex ,
                                                                                uint32 ant_num ,
                                                                                const xmlChar *target_para_name ,
                                                                                xmlChar **para_temp ) ;
extern  status_t i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_get_para( xmlChar **para_temp ) ;
extern  status_t i_o_ran_beamformaing_init( void ) ;

extern  status_t i_o_ran_beamforming_saa_init_data_get(char *parameter_name, xmlChar **init_data);

#endif
