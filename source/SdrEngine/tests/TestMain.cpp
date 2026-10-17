// Copyright 2026 Steven Festenstein
// SPDX-License-Identifier: BSL-1.0

/**
 * @file TestMain.cpp
 * @brief Custom Google Test main for SdrEngine tests.
 *
 * Logging works via CommonUtils defaults (no explicit init required).
 */

#include <gtest/gtest.h>

#include "LoggerMacros.h"

int main(int argc, char** argv)
{
   GPINFO("SdrEngine unit tests starting");
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
