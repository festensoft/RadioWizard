# =============================================================================
# RealTimeGraphs unit tests
# =============================================================================

include(GoogleTest)

add_executable(UnitTests_RealTimeGraphs
   TestMain.cpp
   OscilloscopeWidgetUt.cpp
)

set_target_properties(UnitTests_RealTimeGraphs PROPERTIES
   AUTOMOC ON
   RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_link_libraries(UnitTests_RealTimeGraphs
   PRIVATE
      RealTimeGraphs
      GTest::gtest
      GTest::gmock
      Qt6::Core
      Qt6::Gui
      Qt6::Widgets
)

target_compile_options(UnitTests_RealTimeGraphs PRIVATE -Wno-unused-result)

gtest_discover_tests(UnitTests_RealTimeGraphs
   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
   PROPERTIES
      LABELS "unit"
)
