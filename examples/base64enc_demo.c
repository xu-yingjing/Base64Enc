#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "base64enc.h"

typedef struct {
    const char *text;
    const char *want;
} vector_t;

static const vector_t vectors[] = {
    { "M", "TQ==" },
    { "Ma", "TWE=" },
    { "Man", "TWFu" },
    { "fo", "Zm8=" },
    { "foobar", "Zm9vYmFy" },
};

static int check_encode(const vector_t *vector)
{
    char out[16];
    size_t src_len = strlen(vector->text);
    size_t size = base64enc_encode_size(src_len);

    if (size != strlen(vector->want)) {
        printf("FAIL %-8s size %lu, want %lu characters\n",
               vector->text,
               (unsigned long)size,
               (unsigned long)strlen(vector->want));
        return 1;
    }

    if (base64enc_encode(out, sizeof(out), (const uint8_t *)vector->text, src_len) != BASE64ENC_OK) {
        printf("FAIL %-8s encode returned an error\n", vector->text);
        return 1;
    }

    out[size] = '\0';

    if (strcmp(out, vector->want) != 0) {
        printf("FAIL %-8s got %s, want %s\n", vector->text, out, vector->want);
        return 1;
    }

    printf("ok   %-8s -> %s\n", vector->text, out);
    return 0;
}

static int check_rejections(void)
{
    char out[8];
    int failures = 0;

    if (base64enc_encode(out, 3U, (const uint8_t *)"Man", 3U) != BASE64ENC_ERROR) {
        printf("FAIL undersized destination was accepted\n");
        failures++;
    }

    if (base64enc_encode(NULL, 8U, (const uint8_t *)"Man", 3U) != BASE64ENC_ERROR) {
        printf("FAIL NULL destination was accepted\n");
        failures++;
    }

    if (base64enc_encode(out, 8U, NULL, 3U) != BASE64ENC_ERROR) {
        printf("FAIL NULL source was accepted\n");
        failures++;
    }

    if (base64enc_encode(out, 8U, (const uint8_t *)"Man", 0U) != BASE64ENC_ERROR) {
        printf("FAIL zero-length source was accepted\n");
        failures++;
    }

    return failures;
}

int main(void)
{
    int failures = 0;
    size_t i;

    for (i = 0U; i < (sizeof(vectors) / sizeof(vectors[0])); i++) {
        failures += check_encode(&vectors[i]);
    }

    failures += check_rejections();

    if (failures != 0) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }

    printf("all checks passed\n");
    return 0;
}
