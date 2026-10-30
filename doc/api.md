# API contract

Both functions are declared in `src/base64enc.h`. Neither allocates, neither writes outside the caller's buffer, and the encoder keeps no state between calls: the only static object is the read-only alphabet table, so the library is re-entrant.

## base64enc_encode_size

```c
size_t base64enc_encode_size(size_t src_len);
```

Returns `ceil(src_len / 3) * 4`: the exact number of characters `base64enc_encode()` writes for `src_len` input bytes, padding included, terminator excluded.

The result is a `size_t`. On the 32-bit target that bounds the callable range: the largest input with a representable output size is 3,221,225,469 bytes (encoded size 4,294,967,292). One byte more pushes the encoded size to 4,294,967,296, which no longer fits. On a 64-bit host the arithmetic is 64-bit as well, so the practical limit is what the two buffers can hold. There is no runtime check either way.

## base64enc_encode

```c
base64enc_status_t base64enc_encode(char *dst, size_t dst_size, const uint8_t *src, size_t src_len);
```

Writes exactly `base64enc_encode_size(src_len)` characters to `dst` and returns `BASE64ENC_OK`.

Returns `BASE64ENC_ERROR` and writes nothing to `dst` when:

- `src_len` is 0,
- `src` or `dst` is `NULL`,
- `dst_size` is smaller than `base64enc_encode_size(src_len)`.

`dst_size` is the capacity of `dst` in bytes. `dst` and `src` must not overlap: the encoder does not detect an in-place attempt, and a shared buffer corrupts the bytes it has not read yet.

## Encoding rules

- Alphabet: `A`-`Z`, `a`-`z`, `0`-`9`, `+`, `/` (RFC 4648 section 4).
- A final group of one byte becomes two characters plus `==`; two bytes become three characters plus `=`.
- No line breaks, no `\r`, no `\n`, and no terminator.
- The input is treated as bytes, so `0x00` inside `src` is encoded like any other value.

| `src_len` | `base64enc_encode_size()` | last characters |
| --- | --- | --- |
| 1 | 4 | `xx==` |
| 2 | 4 | `xxx=` |
| 3 | 4 | `xxxx` |
| 4 | 8 | four characters of group 2 |
| 6 | 8 | `xxxx` of group 2 |

## Adding a decoder

A decoder is not part of this release. If one is added, keep it a separate entry point with its own size function and the same budget rule: reject invalid input and undersized destinations before writing anything, and never allocate.
