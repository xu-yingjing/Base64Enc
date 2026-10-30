#include "base64enc.h"
#include <stddef.h>

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

size_t base64enc_encode_size(size_t src_len)
{
    return (((src_len + 2U) / 3U) * 4U);
}

base64enc_status_t base64enc_encode(char *dst, size_t dst_size, const uint8_t *src, size_t src_len)
{
    size_t src_pos = 0U;
    size_t dst_pos = 0U;
    uint32_t value;

    if ((src_len == 0U) ||
        (src == NULL) ||
        (dst == NULL) ||
        (dst_size < base64enc_encode_size(src_len))) {
        return BASE64ENC_ERROR;
    }

    while ((src_len - src_pos) >= 3U) {
        value = ((uint32_t)src[src_pos] << 16) | ((uint32_t)src[src_pos + 1U] << 8) | (uint32_t)src[src_pos + 2U];

        dst[dst_pos + 0U] = base64_table[(value >> 18) & 0x3FU];
        dst[dst_pos + 1U] = base64_table[(value >> 12) & 0x3FU];
        dst[dst_pos + 2U] = base64_table[(value >> 6) & 0x3FU];
        dst[dst_pos + 3U] = base64_table[value & 0x3FU];

        src_pos += 3U;
        dst_pos += 4U;
    }
 
    if ((src_len - src_pos) == 1U) {
        value = (uint32_t)src[src_pos] << 16;

        dst[dst_pos + 0U] = base64_table[(value >> 18) & 0x3FU];
        dst[dst_pos + 1U] = base64_table[(value >> 12) & 0x3FU];
        dst[dst_pos + 2U] = '=';
        dst[dst_pos + 3U] = '=';

        dst_pos += 4U;
    } else if ((src_len - src_pos) == 2U) {
        value = ((uint32_t)src[src_pos] << 16) | ((uint32_t)src[src_pos + 1U] << 8);

        dst[dst_pos + 0U] = base64_table[(value >> 18) & 0x3FU];
        dst[dst_pos + 1U] = base64_table[(value >> 12) & 0x3FU];
        dst[dst_pos + 2U] = base64_table[(value >> 6) & 0x3FU];
        dst[dst_pos + 3U] = '=';

        dst_pos += 4U;
    }

    return BASE64ENC_OK;
}
