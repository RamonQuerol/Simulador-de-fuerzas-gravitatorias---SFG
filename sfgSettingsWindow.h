#pragma once

#include <gtk/gtk.h>
#include "sfgMainWindow.h"

#define SFG_SETTINGS_WINDOW_TYPE (sfg_settings_window_get_type ())
G_DECLARE_FINAL_TYPE (SfgSettingsWindow, sfg_settings_window, SFG, SETTINGS_WINDOW, GtkDialog)


SfgSettingsWindow       *sfg_settings_window_new          (SfgMainWindow *win);
void                    sfg_settings_window_open         (SfgMainWindow *win,
                                                         GFile            *file);