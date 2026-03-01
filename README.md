# OBI-ABI
## Canonical C ABI Headers for OBI

**Last Updated:** 2026-03-01  
**Status:** Draft  
**Repository Role:** ABI headers (normative shapes)

---

This repository contains the canonical C headers for the Omni Backstage Interface (OBI):

- `include/obi/obi_core_v0.h` (core host/provider and stream types)
- `include/obi/profiles/*.h` (per-profile handle structs, vtables, caps)

These headers are intended to be the single source of truth that hosts and providers compile
against.

The prose specification lives in the `OBI` repository.

---

## Related Repositories

- `OBI`: normative/informative prose specification, profile catalog, and conformance guidance.
- `libobi`: runtime/loader implementation that uses these headers and resolves provider selection
  at runtime.

---

## Build / Install

This repo is headers-only and includes a minimal Meson build for installation.

```sh
meson setup build
meson install -C build
```
