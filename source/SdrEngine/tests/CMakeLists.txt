# =============================================================================
# SdrEngine unit tests
# =============================================================================

include(GoogleTest)

add_executable(UnitTests_SdrEngine
   TestMain.cpp
   ChannelFilterUt.cpp
   DemodulatorUt.cpp
   FftProcessorUt.cpp
   SdrCommonUtilsUt.cpp
   SdrEngineUt.cpp
   SoapySdrDeviceUt.cpp
)

# Private headers (ChannelFilter, FftProcessor) live under src/
target_include_directories(UnitTests_SdrEngine
   PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_link_libraries(UnitTests_SdrEngine
   PRIVATE
      SdrEngine
      CommonUtils
      GTest::gtest
      GTest::gmock
)

target_compile_options(UnitTests_SdrEngine PRIVATE -Wno-unused-result)

gtest_discover_tests(UnitTests_SdrEngine
   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
   PROPERTIES
      LABELS "unit"
)

set_target_properties(UnitTests_SdrEngine PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
