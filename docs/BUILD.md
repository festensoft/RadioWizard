# Build Guide

Detailed instructions for building RadioWizard.

## Prerequisites

### All platforms

- **CMake 4.1+**
- **Python 3.8+** (for Conan)
- **Conan 2.0+**: `pip install conan`
- **Ninja** (recommended)

### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install -y \
   build-essential cmake ninja-build python3-pip lcov \
   libgl-dev libx11-xcb-dev libfontenc-dev libxkbfile-dev \
   libxcb-cursor-dev libxcb-icccm4-dev libxcb-keysyms1-dev \
   libxcb-shape0-dev libxcb-xkb-dev libxrender-dev \
   libsoapysdr-dev

pip3 install conan
```

Optional SoapySDR modules for your hardware:

```bash
# sudo apt-get install -y soapysdr-module-rtlsdr soapysdr-module-plutosdr
```

### macOS

```bash
xcode-select --install
brew install cmake ninja python lcov soapysdr
pip3 install conan
```

## Setup

```bash
conan profile detect --force

conan install . --output-folder=build --build=missing \
  -s build_type=Release -s compiler.cppstd=gnu20
```

> Use `gnu20` (not `20`) for `compiler.cppstd` on Linux — Qt 6 needs GNU
> extensions such as `__int128`. On macOS, `20` is typically fine.

If `conan install` fails building `openal-soft` with GCC 15, this project
already disables OpenAL in Qt (`with_openal = False`) since RadioWizard does
not use it.

## Configure, build, test

Prefer workflow presets (configure + build + test in one step). CI runs the
same commands:

```bash
cmake --workflow --preset debug          # plain Debug + tests
cmake --workflow --preset debug-asan     # Debug + ASan/UBSan + tests (CI)
cmake --workflow --preset release        # Release build
cmake --workflow --preset debug-tsan     # TSan (local; Clang)
cmake --workflow --preset debug-clang-tidy  # clang-tidy (local; Clang)
```

| Preset | Build type | Tests | Sanitizers | Clang-Tidy | CI |
|--------|------------|-------|------------|------------|----|
| `debug` | Debug | ✅ | — | — | — |
| `debug-asan` | Debug | ✅ | ASan + UBSan | — | ✅ |
| `debug-tsan` | Debug | ✅ | TSan (Clang) | — | — |
| `debug-clang-tidy` | Debug | ✅ | — | ✅ (Clang) | — |
| `release` | Release | — | — | — | ✅ |

ASan/UBSan and TSan are separate presets and must not be combined.

### Compilers on macOS

macOS has **two** Clang toolchains people commonly hit:

| Toolchain | Typical path | CMake id |
|-----------|--------------|----------|
| **Apple Clang** (Xcode) | `/usr/bin/clang++` | `AppleClang` |
| **Homebrew LLVM** | `/opt/homebrew/opt/llvm/bin/clang++` | `Clang` |

Presets do **not** set `CMAKE_CXX_COMPILER=clang++` on macOS. That bare name often resolves to Homebrew LLVM, which then fights the Xcode SDK (`-isysroot`) and breaks in libc++ (`NAN` / `INFINITY` undeclared). Use the default AppleClang toolchain. A hidden `linux-clang` preset exists if you want to force Clang on Linux only — do not inherit it into macOS presets (CMake disables a preset that inherits a false-conditioned parent).

## Run

```bash
./build/debug/bin/RadioWizardMain
# or, after debug-asan:
./build/debug-asan/bin/RadioWizardMain
```

## Dependencies

| Package | Source | Purpose |
|---------|--------|---------|
| festensoft/CommonUtils v1.0.0 | FetchContent | Logging, DataHandler, CircularBuffer, StackTrace |
| spdlog | via CommonUtils | Logging backend (private) |
| fftw | Conan | FFT (SdrEngine) |
| liquid-dsp | Conan | Filters, NCO, resampling, demod |
| qt | Conan | GUI (RealTimeGraphs, RadioWizardMain) |
| gtest | Conan (tests) | Unit tests |
| SoapySDR | System | SDR hardware abstraction |

## CMake options

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_TESTS` | OFF | Build unit tests (debug presets turn this on) |
| `ENABLE_CLANG_TIDY` | OFF | clang-tidy during the build (`debug-clang-tidy`) |

Sanitizer flags come from presets (`CMAKE_CXX_FLAGS` / linker flags), not
CMake options.
