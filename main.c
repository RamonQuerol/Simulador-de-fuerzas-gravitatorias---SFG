#include <gtk/gtk.h>

#include "sfgApp.h"

int
main (int argc, char *argv[])
{
  return g_application_run (G_APPLICATION (sfg_app_new ()), argc, argv);
}