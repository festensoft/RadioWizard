# GitHub Copilot Instructions for RadioWizard

## Project overview

RadioWizard is a C++20 SDR control and visualization app:

- **Build**: CMake 4.1+ with presets; Conan 2.0 for Qt / FFTW / liquid-dsp / GTest
- **CommonUtils**: FetchContent from festensoft/CommonUtils v1.0.0 (brings spdlog)
- **Hardware**: SoapySDR (system package)
- **UI**: Qt 6 widgets

## Structure

```
source/
  SdrEngine/          # include/SdrEngine + src + tests  (STATIC)
  RealTimeGraphs/     # include/RealTimeGraphs + src + tests  (STATIC)
  RadioWizardMain/    # Qt app under src/
```

Public headers: `#include <SdrEngine/SdrEngine.h>`,
`#include <RealTimeGraphs/SpectrumWidget.h>`.
Logging: `#include "LoggerMacros.h"` then `GPINFO("...", ...)`.

## Conventions

- Indentation: 3 spaces; Allman braces; 100-col soft limit
- Members: `_` prefix; statics: `s_` prefix
- Prefer static libraries; do not reintroduce PubSub, Vita49, protobuf, or a
  vendored CommonUtils
- Keep private headers out of `include/`
