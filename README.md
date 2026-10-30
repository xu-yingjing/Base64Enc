# base64enc

Base64 encoder for C99 targets with no OS, no HAL, no dynamic allocation and no writable static data.

The library is one encode path: it turns a byte buffer into the standard RFC 4648 Base64 alphabet (`A`-`Z`, `a`-`z`, `0`-`9`, `+`, `/`) with `=` padding and no line breaks. The output length is a pure function of the input length, so the caller sizes one buffer and the encoder never allocates.

## Scope

Supported:

- Any byte sequence, including embedded NULs: the encoder is byte-oriented and never treats the input as text.
- Trailing groups of one and two bytes (`xx==` and `xxx=`).

Not supported, by design:

- Decoding. Nothing here assumes a reverse mapping; a caller that needs one adds it as a second entry point.
- The URL-safe alphabet (`-` and `_`) and MIME line wrapping.
- Streaming. The output length is fixed, and the encoder writes it in one pass from a contiguous input.

## Quick start

```c
#include "base64enc.h"

static const uint8_t payload[3] = { 'M', 'a', 'n' };
static char encoded[5];   /* base64enc_encode_size(3) characters plus a terminator */

if (base64enc_encode(encoded, sizeof(encoded), payload, sizeof(payload)) != BASE64ENC_OK) {
    /* zero length, NULL pointer, or a destination that is too small */
}
encoded[base64enc_encode_size(sizeof(payload))] = '\0';
```

`base64enc_encode_size()` counts characters only; the encoder does not write a terminator, so the buffer needs one extra byte when the result is handed to a string API. The full contract, including the rejection cases, is in [doc/api.md](doc/api.md).

## Integration

Add `src/` to the include path and compile `src/base64enc.c`. There is nothing to configure and no platform code: the source includes only `<stddef.h>` and its own header.

## Verification

`sh build.sh` compiles the library and the example with the host compiler and every warning enabled (`-Wconversion`, `-Wsign-conversion`, `-Wshadow`, `-Wundef`, `-Wwrite-strings`, `-Werror`). Override the compiler with `CC=...`.

The example checks the tail cases and the rejection cases. Build and run it on a host:

```sh
cc -std=c99 -I src src/base64enc.c examples/base64enc_demo.c -o base64enc_demo && ./base64enc_demo
```

## License

BSD 3-Clause, see [LICENSE](LICENSE).
