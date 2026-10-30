#ifndef BASE64ENC_H
#define BASE64ENC_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
    BASE64ENC_OK = 0U,
    BASE64ENC_ERROR,
} base64enc_status_t;

size_t base64enc_encode_size(size_t src_len);

base64enc_status_t base64enc_encode(char *dst, size_t dst_size, const uint8_t *src, size_t src_len);

#endif
