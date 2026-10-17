# =============================================================================
# RadioWizard - Root CMakeLists.txt (thin aggregator)
# =============================================================================
cmake_minimum_required(VERSION 4.1)

project(RadioWizard
   VERSION 1.0.0
   DESCRIPTION "SDR control, spectrum/IQ observation, signal isolation and demodulation"
   LANGUAGES CXX
)

# =============================================================================
# Global Settings
# =============================================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
   file(CREATE_LINK "${CMAKE_BINARY_DIR}/compile_commands.json"
      "${CMAKE_SOURCE_DIR}/compile_commands.json" SYMBOLIC)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
   set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
   set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
      "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# =============================================================================
# Options
# =============================================================================

option(BUILD_TESTS "Build unit tests" OFF)
option(ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)
# Sanitizers (ASan/UBSan/TSan) are applied via CMakePresets.json
# (CMAKE_CXX_FLAGS / linker flags), not CMake options — keeps ASan and
# TSan mutually exclusive at the binary level.

# =============================================================================
# Compiler Warnings
# =============================================================================

set(GCC_WARNINGS
   -Wall
   -Wextra
   -Wshadow
   -Wnon-virtual-dtor
   -Wold-style-cast
   -Wcast-align
   -Wunused
   -Woverloaded-virtual
   -Wconversion
   -Wsign-conversion
   -Wnull-dereference
   -Wdouble-promotion
   -Wformat=2
   -Wimplicit-fallthrough
)

set(MSVC_WARNINGS
   /W4
   /permissive-
   /w14640
   /w14242
   /w14254
   /w14263
   /w14265
   /w14287
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
   add_compile_options(${GCC_WARNINGS})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
   add_compile_options(${MSVC_WARNINGS})
endif()

# =============================================================================
# Clang-Tidy
# =============================================================================

if(ENABLE_CLANG_TIDY)
   find_program(CLANG_TIDY_PROGRAM clang-tidy)
   if(CLANG_TIDY_PROGRAM)
      message(STATUS "Found clang-tidy: ${CLANG_TIDY_PROGRAM}")
      # Homebrew's clang-tidy ships its own resource-dir/libc++. When the
      # build uses AppleClang + the Xcode SDK, tidy must use the *compiler's*
      # resource-dir or system headers fail (e.g. undeclared NAN/INFINITY).
      set(_clang_tidy_args
         "${CLANG_TIDY_PROGRAM}"
         "--header-filter=${CMAKE_SOURCE_DIR}/source/.*"
      )
      if(CMAKE_CXX_COMPILER)
         execute_process(
            COMMAND "${CMAKE_CXX_COMPILER}" -print-resource-dir
            OUTPUT_VARIABLE _cxx_resource_dir
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
         )
         if(_cxx_resource_dir)
            list(APPEND _clang_tidy_args
               "--extra-arg=-resource-dir=${_cxx_resource_dir}")
            message(STATUS "clang-tidy resource-dir: ${_cxx_resource_dir}")
         endif()
      endif()
      set(CMAKE_CXX_CLANG_TIDY "${_clang_tidy_args}")
   else()
      message(WARNING "clang-tidy requested but not found")
   endif()
endif()

# =============================================================================
# Dependencies
# =============================================================================

include(FetchContent)

set(COMMONUTILS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_Declare(CommonUtils
   GIT_REPOSITORY https://github.com/festensoft/CommonUtils.git
   GIT_TAG        v1.0.0
   GIT_SHALLOW    TRUE
   SYSTEM
   EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(CommonUtils)
# CommonUtils toggles BUILD_TESTING; keep FetchContent's suite off — ours is
# gated by BUILD_TESTS below.
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

# Conan / system packages
find_package(FFTW3 REQUIRED)
message(STATUS "Found FFTW3")

find_package(liquid-dsp REQUIRED)
message(STATUS "Found liquid-dsp")

find_package(SoapySDR REQUIRED)
message(STATUS "Found SoapySDR: ${SoapySDR_VERSION}")

# SoapySDR's imported target only has a single configuration, which varies
# by platform: Homebrew (macOS) ships it as "Release", while Debian/Ubuntu
# packages build it with CMAKE_BUILD_TYPE="" ("None").
set_target_properties(SoapySDR PROPERTIES
   MAP_IMPORTED_CONFIG_DEBUG "Release;None"
   MAP_IMPORTED_CONFIG_RELWITHDEBINFO "Release;None"
)

find_package(Qt6 REQUIRED)
message(STATUS "Found Qt6: ${Qt6_VERSION_STRING}")

# The Conan-installed Qt is Release-only.  Map Debug/RelWithDebInfo → "" so
# CMake falls back to the plain IMPORTED_LOCATION on Qt tool targets.
set(_qt_tool_names
   moc uic rcc qlalr tracegen cmake_automoc_parser
   qmake qtpaths syncqt tracepointgen qvkgen qsb
   macdeployqt androiddeployqt qdbusxml2cpp qdbus
)
foreach(_qt_tool IN LISTS _qt_tool_names)
   if(TARGET Qt6::${_qt_tool})
      set_target_properties(Qt6::${_qt_tool} PROPERTIES
         MAP_IMPORTED_CONFIG_DEBUG ""
         MAP_IMPORTED_CONFIG_RELWITHDEBINFO ""
      )
   endif()
endforeach()

if(BUILD_TESTS)
   find_package(GTest REQUIRED)
endif()

# =============================================================================
# Output Directories
# =============================================================================

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# =============================================================================
# Modules
# =============================================================================

# enable_testing() must run before add_subdirectory() so module tests register.
if(BUILD_TESTS)
   enable_testing()
   include(GoogleTest)
endif()

add_subdirectory(source/SdrEngine)
add_subdirectory(source/RealTimeGraphs)
add_subdirectory(source/RadioWizardMain)

# =============================================================================
# Installation
# =============================================================================

include(GNUInstallDirs)

install(TARGETS RadioWizardMain
   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
   BUNDLE DESTINATION .
)

# =============================================================================
# Summary
# =============================================================================

message(STATUS "")
message(STATUS "=== RadioWizard Configuration ===")
message(STATUS "  Version:          ${PROJECT_VERSION}")
message(STATUS "  Build type:       ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard:     ${CMAKE_CXX_STANDARD}")
message(STATUS "  Compiler:         ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Build tests:      ${BUILD_TESTS}")
message(STATUS "  Clang-tidy:       ${ENABLE_CLANG_TIDY}")
message(STATUS "  Qt6:              ${Qt6_VERSION}")
message(STATUS "  SoapySDR:         ${SoapySDR_VERSION}")
message(STATUS "================================")
message(STATUS "")
