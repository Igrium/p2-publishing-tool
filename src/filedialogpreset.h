#pragma once

#include <QFileDialog>

// Use the platform's native file chooser everywhere. On Linux this goes through the
// xdg-desktop-portal platform theme (selected in main.cpp), which shows the desktop's own
// dialog (e.g. KDE/Dolphin or GTK) and falls back to Qt's dialog if no portal is available.
static constexpr const auto FILE_PICKER_OPTS = static_cast<QFileDialog::Option>( 0 );
