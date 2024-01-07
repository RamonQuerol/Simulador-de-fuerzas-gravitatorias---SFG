#pragma once

#include <gtk/gtk.h>
#include "sfgApp.h"
#include "sfgMainWindow.h"


#define SFG_AMC_WINDOW_TYPE (sfg_amc_window_get_type ())
G_DECLARE_FINAL_TYPE (SfgAmcWindow, sfg_amc_window, SFG, AMC_WINDOW, GtkDialog)


SfgAmcWindow       *sfg_amc_window_new          (SfgMainWindow *win);
void                    sfg_amc_window_open         (SfgMainWindow *win,
                                                         GFile            *file);