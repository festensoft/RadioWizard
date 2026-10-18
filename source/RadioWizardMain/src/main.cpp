// Copyright 2026 Steven Festenstein
// SPDX-License-Identifier: BSL-1.0

#include "MainWindow.h"

#include "LoggerMacros.h"
#include "StackTrace.h"

#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QPalette>
#include <QStyleFactory>

int main(int argc, char* argv[])
{
   // Logging works immediately via CommonUtils defaults; install crash
   // handlers that dump the in-memory trace buffer and flush sinks.
   CommonUtils::StackTrace::SetPostCrashHook([](int sig)
   {
      GPCRIT("Caught fatal signal {} — see stderr for stack trace", sig);
      CommonUtils::Logging::DumpTraceBacktrace();
      CommonUtils::Logging::Flush();
   });
   CommonUtils::StackTrace::InstallSignalHandlers();

   const QApplication a(argc, argv);

   // Apply a dark color palette to the entire application.
   QApplication::setStyle(QStyleFactory::create("Fusion"));
   QPalette darkPalette;
   darkPalette.setColor(QPalette::Window,           QColor(45, 45, 48));
   darkPalette.setColor(QPalette::WindowText,       QColor(208, 208, 208));
   darkPalette.setColor(QPalette::Base,             QColor(30, 30, 30));
   darkPalette.setColor(QPalette::AlternateBase,    QColor(45, 45, 48));
   darkPalette.setColor(QPalette::ToolTipBase,      QColor(60, 60, 65));
   darkPalette.setColor(QPalette::ToolTipText,      QColor(208, 208, 208));
   darkPalette.setColor(QPalette::Text,             QColor(208, 208, 208));
   darkPalette.setColor(QPalette::Button,           QColor(55, 55, 58));
   darkPalette.setColor(QPalette::ButtonText,       QColor(208, 208, 208));
   darkPalette.setColor(QPalette::BrightText,       QColor(255, 51, 51));
   darkPalette.setColor(QPalette::Link,             QColor(42, 130, 218));
   darkPalette.setColor(QPalette::Highlight,        QColor(42, 130, 218));
   darkPalette.setColor(QPalette::HighlightedText,  QColor(240, 240, 240));
   darkPalette.setColor(QPalette::PlaceholderText,  QColor(120, 120, 120));

   // Disabled state
   darkPalette.setColor(QPalette::Disabled, QPalette::WindowText,  QColor(127, 127, 127));
   darkPalette.setColor(QPalette::Disabled, QPalette::Text,        QColor(127, 127, 127));
   darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText,  QColor(127, 127, 127));
   darkPalette.setColor(QPalette::Disabled, QPalette::Highlight,   QColor(80, 80, 80));
   darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));
   // Embed Roboto / Roboto Mono so CLI and IDE launches resolve the same
   // faces without system font packages.
   const char* const embeddedFonts[] = {
      ":/fonts/Roboto-Regular.ttf",
      ":/fonts/Roboto-Bold.ttf",
      ":/fonts/RobotoMono-Regular.ttf",
      ":/fonts/RobotoMono-Medium.ttf",
      ":/fonts/RobotoMono-Bold.ttf",
   };
   for (const char* path : embeddedFonts)
   {
      if (QFontDatabase::addApplicationFont(QString::fromUtf8(path)) < 0)
      {
         GPWARN("Failed to load embedded font {}", path);
      }
   }
   QApplication::setFont(QFont(QStringLiteral("Roboto"), 12));
   QApplication::setPalette(darkPalette);

   MainWindow w;
   w.show();
   return QApplication::exec();
}
