// Copyright 2026 Steven Festenstein
// SPDX-License-Identifier: BSL-1.0

/**
 * @file TestMain.cpp
 * @brief Google Test main for RealTimeGraphsTests.
 *
 * Initializes a QApplication (required for QWidget tests) before running tests.
 */

#include <gtest/gtest.h>
#include <QApplication>
#include <QByteArray>

int main(int argc, char** argv)
{
   // Headless CI (and local shells without a display) have no xcb/wayland
   // connection. Prefer offscreen unless the caller already chose a platform.
   if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM"))
   {
      qputenv("QT_QPA_PLATFORM", QByteArrayLiteral("offscreen"));
   }

   QApplication app(argc, argv);

   ::testing::InitGoogleTest(&argc, argv);

   return RUN_ALL_TESTS();
}
