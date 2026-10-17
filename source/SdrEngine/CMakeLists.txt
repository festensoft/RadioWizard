# =============================================================================
# SdrEngine — SDR device abstraction + FFT / channel-filter pipeline
#
# Public API (include/SdrEngine/):
#   SdrEngine, ISdrDevice, Demodulator, SdrTypes
#
# Private (src/): SoapySdrDevice, ChannelFilter, FftProcessor, and .cpp files.
# No Qt dependency — uses DataHandler (CommonUtils) for data distribution.
# =============================================================================

add_library(SdrEngine STATIC
   src/SdrEngine.cpp
   src/SoapySdrDevice.cpp
   src/Demodulator.cpp
   src/SdrCommonUtils.cpp
   src/ChannelFilter.cpp
   src/FftProcessor.cpp
)

add_library(RadioWizard::SdrEngine ALIAS SdrEngine)

target_include_directories(SdrEngine
   PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
   PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(SdrEngine
   PUBLIC
      CommonUtils
   PRIVATE
      FFTW3::fftw3f
      liquid-dsp::liquid-dsp
      SoapySDR
)

if(BUILD_TESTS)
   add_subdirectory(tests)
endif()
