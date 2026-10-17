"""
RadioWizard Conan 2.0 Configuration

Manages external dependencies for the RadioWizard SDR application —
SDR control, spectrum/IQ observation, signal isolation, and demodulation.

spdlog is not listed here: CommonUtils (FetchContent) finds or fetches it.
Sanitizers / clang-tidy / tests are controlled by CMake presets, not Conan.
"""

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, cmake_layout


class RadioWizardConan(ConanFile):
   name = "radiowizard"
   version = "1.0.0"
   description = "SDR control, spectrum/IQ observation, signal isolation and demodulation"
   license = "BSL-1.0"

   settings = "os", "compiler", "build_type", "arch"

   options = {
      "shared": [True, False],
      "fPIC": [True, False],
   }

   default_options = {
      "shared": False,
      "fPIC": True,
   }

   def requirements(self):
      self.requires("fftw/3.3.10")
      self.requires("liquid-dsp/1.6.0")
      # 6.10.1 fails to compile with newer Apple SDK/Clang toolchains
      # (e.g., __yield declaration in qyieldcpu.h on arm64), so pin to
      # a newer patch release.
      self.requires("qt/6.10.3")

   def build_requirements(self):
      # Always available so any preset with BUILD_TESTS=ON can find GTest.
      self.test_requires("gtest/1.14.0")

   def configure(self):
      if self.options.shared:
         self.options.rm_safe("fPIC")

      self.options["qt/*"].shared = True
      self.options["qt/*"].qtshadertools = True
      self.options["qt/*"].qtmultimedia = True
      self.options["qt/*"].gui = True
      self.options["qt/*"].widgets = True
      self.options["qt/*"].opengl = "desktop"
      self.options["qt/*"].with_pq = False
      self.options["qt/*"].with_odbc = False
      self.options["qt/*"].with_sqlite3 = False
      # openal-soft fails to build with GCC 15; RadioWizard does not use OpenAL.
      self.options["qt/*"].with_openal = False

   def layout(self):
      cmake_layout(self)

   def generate(self):
      deps = CMakeDeps(self)
      deps.generate()

      tc = CMakeToolchain(self)
      tc.generate()
