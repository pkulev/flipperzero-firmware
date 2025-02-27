#include "subghz_protocol_keeloq_common.h"

#include <furi.h>

#include <m-string.h>
#include <m-array.h>

/** Simple Learning Encrypt
 * @param data - 0xBSSSCCCC, B(4bit) key, S(10bit) serial&0x3FF, C(16bit) counter
 * @param key - manufacture (64bit)
 * @return keelog encrypt data
 */
inline uint32_t subghz_protocol_keeloq_common_encrypt(const uint32_t data, const uint64_t key) {
    uint32_t x = data, r;
    for (r = 0; r < 528; r++)
        x = (x>>1)^((bit(x,0)^bit(x,16)^(uint32_t)bit(key,r&63)^bit(KEELOQ_NLF,g5(x,1,9,20,26,31)))<<31);
    return x;
}

/** Simple Learning Decrypt
 * @param data - keelog encrypt data
 * @param key - manufacture (64bit)
 * @return 0xBSSSCCCC, B(4bit) key, S(10bit) serial&0x3FF, C(16bit) counter
 */
inline uint32_t subghz_protocol_keeloq_common_decrypt(const uint32_t data, const uint64_t key) {
    uint32_t x = data, r;
    for (r = 0; r < 528; r++)
        x = (x<<1)^bit(x,31)^bit(x,15)^(uint32_t)bit(key,(15-r)&63)^bit(KEELOQ_NLF,g5(x,0,8,19,25,30));
    return x;
}

/** Normal Learning
 * @param data - serial number (28bit)
 * @param key - manufacture (64bit)
 * @return manufacture for this serial number (64bit)
 */
inline uint64_t subghz_protocol_keeloq_common_normal_learning(uint32_t data, const uint64_t key){
    uint32_t k1,k2;

    data&=0x0FFFFFFF;
    data|=0x20000000;
    k1=subghz_protocol_keeloq_common_decrypt(data, key);

    data&=0x0FFFFFFF;
    data|=0x60000000;
    k2=subghz_protocol_keeloq_common_decrypt(data, key);

    return ((uint64_t)k2<<32)| k1; // key - shifrovanoya
}
