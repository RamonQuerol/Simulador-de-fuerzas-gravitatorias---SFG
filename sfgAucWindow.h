#pragma once

#include <gtk/gtk.h>
#include "sfgMainWindow.h"

#define SFG_AUC_WINDOW_TYPE (sfg_auc_window_get_type ())
G_DECLARE_FINAL_TYPE (SfgAucWindow, sfg_auc_window, SFG, AUC_WINDOW, GtkDialog)


SfgAucWindow       *sfg_auc_window_new          (SfgMainWindow *win);
void                    sfg_auc_window_open         (SfgMainWindow *win,
                                                         GFile            *file);