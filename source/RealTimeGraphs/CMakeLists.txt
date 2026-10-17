# =============================================================================
# RealTimeGraphs — custom QPainter-based real-time graph widgets
#
# Public API (include/RealTimeGraphs/):
#   SpectrumWidget, WaterfallWidget, ConstellationWidget, OscilloscopeWidget,
#   PlotWidgetBase (+ colorPaletteCount / colorPaletteName)
#
# Private (src/): ColorMap, BandwidthSelector, PlotCursorOverlay, ColorBarWidget,
#   CommonGuiUtils, PlotPainterUtils, .cpp files.
# =============================================================================

add_library(RealTimeGraphs STATIC
   # Public headers with Q_OBJECT must be listed so AUTOMOC scans them
   # (they live outside src/ and would otherwise be missed).
   include/RealTimeGraphs/SpectrumWidget.h
   include/RealTimeGraphs/WaterfallWidget.h
   include/RealTimeGraphs/ConstellationWidget.h
   include/RealTimeGraphs/OscilloscopeWidget.h
   include/RealTimeGraphs/PlotWidgetBase.h
   src/ColorBarWidget.h
   src/SpectrumWidget.cpp
   src/WaterfallWidget.cpp
   src/ConstellationWidget.cpp
   src/OscilloscopeWidget.cpp
   src/ColorMap.cpp
   src/PlotWidgetBase.cpp
   src/BandwidthSelector.cpp
   src/PlotCursorOverlay.cpp
   src/ColorBarWidget.cpp
   src/CommonGuiUtils.cpp
   src/PlotPainterUtils.cpp
)

add_library(RadioWizard::RealTimeGraphs ALIAS RealTimeGraphs)

set_target_properties(RealTimeGraphs PROPERTIES
   AUTOMOC ON
   POSITION_INDEPENDENT_CODE ON
)

target_include_directories(RealTimeGraphs
   PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
   PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(RealTimeGraphs
   PUBLIC
      Qt6::Core
      Qt6::Gui
      Qt6::Widgets
      CommonUtils
)

if(BUILD_TESTS)
   add_subdirectory(tests)
endif()
