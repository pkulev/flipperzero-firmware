#pragma once

#include <lib/subghz/protocols/subghz_protocol_common.h>

typedef struct SubGhzHistory SubGhzHistory;

/** Allocate SubGhzHistory
 * 
 * @return SubGhzHistory* 
 */
SubGhzHistory* subghz_history_alloc(void);

/** Free SubGhzHistory
 * 
 * @param instance - SubGhzHistory instance
 */
void subghz_history_free(SubGhzHistory* instance);

/** Clear history
 * 
 * @param instance - SubGhzHistory instance
 */
void subghz_history_clean(SubGhzHistory* instance);

/** Set frequency and preset to history[idx]
 * 
 * @param instance  - SubGhzHistory instance
 * @param idx       - record index  
 * @param frequency - frequency Hz
 * @param preset    - FuriHalSubGhzPreset preset
 */
void subghz_history_set_frequency_preset(
    SubGhzHistory* instance,
    uint16_t idx,
    uint32_t frequency,
    FuriHalSubGhzPreset preset);

/** Get frequency to history[idx]
 * 
 * @param instance  - SubGhzHistory instance
 * @param idx       - record index  
 * @return frequency - frequency Hz
 */
uint32_t subghz_history_get_frequency(SubGhzHistory* instance, uint16_t idx);

/** Get preset to history[idx]
 * 
 * @param instance  - SubGhzHistory instance
 * @param idx       - record index  
 * @return preset    - FuriHalSubGhzPreset preset
 */
FuriHalSubGhzPreset subghz_history_get_preset(SubGhzHistory* instance, uint16_t idx);

/** Get history index write 
 * 
 * @param instance  - SubGhzHistory instance
 * @return idx      - current record index  
 */
uint16_t subghz_history_get_item(SubGhzHistory* instance);

/** Get type protocol to history[idx]
 * 
 * @param instance  - SubGhzHistory instance
 * @param idx       - record index  
 * @return type      - type protocol  
 */
uint8_t subghz_history_get_type_protocol(SubGhzHistory* instance, uint16_t idx);

/** Get name protocol to history[idx]
 * 
 * @param instance  - SubGhzHistory instance
 * @param idx       - record index  
 * @return name      - const char* name protocol  
 */
const char* subghz_history_get_name(SubGhzHistory* instance, uint16_t idx);

/** Get string item menu to history[idx]
 * 
 * @param instance  - SubGhzHistory instance
 * @param output    - string_t output
 * @param idx       - record index
 */
void subghz_history_get_text_item_menu(SubGhzHistory* instance, string_t output, uint16_t idx);

/** Get string the remaining number of records to history
 * 
 * @param instance  - SubGhzHistory instance
 * @param output    - string_t output
 * @return bool - is FUUL
 */
bool subghz_history_get_text_space_left(SubGhzHistory* instance, string_t output);

/** Add protocol to history
 * 
 * @param instance  - SubGhzHistory instance
 * @param context    - SubGhzProtocolCommon context
 */
void subghz_history_add_to_history(SubGhzHistory* instance, void* context);

/** Get SubGhzProtocolCommonLoad to load into the protocol decoder bin data
 * 
 * @param instance  - SubGhzHistory instance
 * @param idx       - record index
 * @return SubGhzProtocolCommonLoad*
 */
SubGhzProtocolCommonLoad* subghz_history_get_raw_data(SubGhzHistory* instance, uint16_t idx);
