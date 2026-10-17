# RadioWizard

A C++20 application for Software Defined Radio (SDR) control, spectrum and I/Q
data observation, signal isolation, and signal demodulation. RadioWizard
interfaces with SDR hardware (via SoapySDR) and visualizes signals in real time
with custom Qt 6 widgets.

## Modules

| Module | Role |
|--------|------|
| **source/SdrEngine** | Device abstraction, FFT, channel filter, demodulation |
| **source/RealTimeGraphs** | Spectrum, waterfall, constellation, oscilloscope widgets |
| **source/RadioWizardMain** | Qt GUI application |
| **CommonUtils** | FetchContent [festensoft/CommonUtils](https://github.com/festensoft/CommonUtils) v1.0.0 |

## Quick start

### Prerequisites

- **Compiler**: GCC 13+ or Clang 15+ (Linux/macOS)
- **CMake**: 4.1+
- **Conan**: 2.0+
- **Ninja** (recommended)
- **SoapySDR** (system package)
- **Linux only**: OpenGL / X11 / XCB development headers (see [Build Guide](docs/BUILD.md))

### Build

```bash
git clone https://github.com/festensoft/RadioWizard
cd RadioWizard

conan profile detect --force
conan install . --output-folder=build --build=missing \
  -s build_type=Release -s compiler.cppstd=gnu20

# Fast local iteration (or use a workflow preset — same as CI):
cmake --workflow --preset debug          # plain Debug + tests
cmake --workflow --preset debug-asan     # Debug + ASan/UBSan + tests (CI gate)
cmake --workflow --preset release        # optimized build

./build/debug/bin/RadioWizardMain
```

> The first Conan install can take a while while Qt 6 and its transitive
> dependencies are built. Subsequent installs use the binary cache.

More detail: [docs/BUILD.md](docs/BUILD.md) · [docs/DESIGN.md](docs/DESIGN.md) 

## Project structure

```
RadioWizard/
├── source/
│   ├── SdrEngine/           # include/SdrEngine + src + tests
│   ├── RealTimeGraphs/      # include/RealTimeGraphs + src + tests
│   └── RadioWizardMain/     # Qt application (src/)
├── docs/
├── sanitizer_suppressions/
└── .github/
```

Public headers live under each library's `include/<Module>/` directory.
Implementation and private headers live under `src/`.

## License

Copyright 2026 Steven Festenstein. Released under the
[Boost Software License 1.0](LICENSE).
