// Ventana para anadir muchos cuerpos(AMC)
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
SfgAmcWindow *amc_win;
const gchar *cadena;

// Función que maneja el introducir un numero
static void obtenerNumero(GtkEntry *entry, gpointer user_data)
{
    cadena = gtk_editable_get_text(GTK_EDITABLE(entry));
}

//Función que maneja la señal de clic en el botón 
static void confirmar(GtkButton *button, gpointer user_data)
{
    if (cadena != NULL)
    {
        int num = atoi(cadena);
        add_cuerpos(num);
    }
    gtk_window_destroy((GtkWindow *)amc_win);
}

static void
sfg_amc_window_init(SfgAmcWindow *win)
{
    gtk_widget_init_template(GTK_WIDGET(win));
    cadena = NULL;
}

// Funcion que se llama al cerrar la ventana para liberar recursos
static void
sfg_amc_window_dispose(GObject *object)
{
    G_OBJECT_CLASS(sfg_amc_window_parent_class)->dispose(object);
}

static void
sfg_amc_window_class_init(SfgAmcWindowClass *class)
{
    G_OBJECT_CLASS(class)->dispose = sfg_amc_window_dispose;
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
                                                "/com/git/sfg/amcWindow.ui");
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgAmcWindow, numCuerpos);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), SfgAmcWindow, botonConfirmar);

    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), obtenerNumero);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), confirmar);
}

SfgAmcWindow *sfg_amc_window_new(SfgMainWindow *win)
{
    amc_win = g_object_new(SFG_AMC_WINDOW_TYPE, "transient-for", win, "use-header-bar", FALSE, NULL);
    return amc_win;
}

void sfg_amc_window_open(SfgMainWindow *win,
                         GFile *file)
{
}
