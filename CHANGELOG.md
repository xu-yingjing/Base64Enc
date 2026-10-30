# Changelog

## 0.1.0

Initial public release. Extracted from an in-tree helper that encodes the JSON metadata carried by a PNG zTXt chunk.

- `base64enc_encode_size()` and `base64enc_encode()` keep the in-tree contract: the destination is budget-checked, nothing is allocated, the output is not NUL-terminated, and a rejected call writes nothing.
- New for the standalone release: `README.md`, `doc/api.md`, `examples/base64enc_demo.c` and `build.sh`. The two files under `src/` are unchanged.
- Known gaps, by design: no decoder, no URL-safe alphabet, no MIME line wrapping and no streaming interface.
