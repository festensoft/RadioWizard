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

int main(int argc, char** argv)
{
   QApplication app(argc, argv);

   ::testing::InitGoogleTest(&argc, argv);

   return RUN_ALL_TESTS();
}
