#pragma once

#include <gtk/gtk.h>


#define SFG_APP_TYPE (sfg_app_get_type ())
G_DECLARE_FINAL_TYPE (SfgApp, sfg_app, SFG, APP, GtkApplication)

SfgApp     *sfg_app_new         (void);