# OBI-ABI
## Canonical C ABI Headers for OBI

**Last Updated:** 2026-03-06  
**Status:** Draft  
**Repository Role:** ABI headers (normative shapes)

---

This repository contains the canonical C headers for the Omni Backstage Interface (OBI):

- `include/obi/obi_core_v0.h` (core host/provider and stream types)
- `include/obi/profiles/*.h` (per-profile handle structs, vtables, caps)

These headers are intended to be the single source of truth that hosts and providers compile
against.

The prose specification lives in the `OBI` repository.

Note on provider metadata:

- Providers MAY implement `obi_provider_api_v0.describe_json` (declared in `obi_core_v0.h`) to
  expose tool-friendly metadata (implemented profiles, dependency list, licensing info,
  diagnostics/host-safety behavior, etc.).
- The recommended JSON shape and licensing guidance live in the `OBI` repository
  (`OBI_PROVIDER_GUIDE.md`).

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
