#include <gtk/gtk.h>

#include "sfgAmcWindow.h"
#include "sfgApp.h"
#include "sfgMainWindow.h"

struct _SfgAmcWindow
{
    GtkDialog parent;

    GtkWidget *numCuerpos;
    GtkWidget *botonConfirmar;
};

G_DEFINE_TYPE(SfgAmcWindow, sfg_amc_window, GTK_TYPE_DIALOG);

// Función que maneja la señal de activación de la entrada
static void obtenerNumero(GtkEntry *entry, gpointer user_data)
{
    const gchar *text = gtk_editable_get_text(GTK_EDITABLE(entry));
    printf("Valor introducido: %s\n", text);
}

// Función que maneja la señal de clic en el botón
// static void confirmar(GtkButton *button, gpointer user_data)
// {
// }

static void
sfg_amc_window_init(SfgAmcWindow *win)
{
    gtk_widget_init_template(GTK_WIDGET(win));
}

static void
sfg_amc_window_class_init(SfgAmcWindowClass *class)
{
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
                                                "/com/git/sfg/amcWindow.ui");
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgAmcWindow, numCuerpos);
    //  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), SfgAmcWindow, botonConfirmar);

    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), obtenerNumero);
    // gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), confirmar);
}

SfgAmcWindow *sfg_amc_window_new(SfgMainWindow *win)
{
    return g_object_new(SFG_AMC_WINDOW_TYPE, "transient-for", win, "use-header-bar", FALSE, NULL);
}

void sfg_amc_window_open(SfgMainWindow *win,
                         GFile *file)
{
}
