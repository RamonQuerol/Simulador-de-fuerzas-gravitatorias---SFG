// Ventana para Anadir un cuerpo(settings)
#include <gtk/gtk.h>

#include "sfgSettingsWindow.h"
#include "sfgMainWindow.h"

struct _SfgSettingsWindow
{
    GtkDialog parent;

    GtkWidget *tiempo;
    GtkWidget *distancia;
    GtkWidget *botonConfirmar;
};

float tiempo, distancia;
gchar *unidadesTiempo;
gchar *unidadesDistancia;

G_DEFINE_TYPE(SfgSettingsWindow, sfg_settings_window, GTK_TYPE_DIALOG);
SfgSettingsWindow *settings_win;

// Función que maneja la señal de activación de la entrada
static void obtenerTiempo(GtkEntry *entry, gpointer user_data)
{
    const gchar *cadenaTiempo = gtk_editable_get_text(GTK_EDITABLE(entry));
    tiempo = atof(cadenaTiempo);
}

static void obtenerUnidadesTiempo(GtkComboBoxText *widget, gpointer user_data)
{
    unidadesTiempo = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)); 
}

static void obtenerDistancia(GtkEntry *entry, gpointer user_data)
{
    const gchar *cadenaDistancia = gtk_editable_get_text(GTK_EDITABLE(entry));
    distancia = atof(cadenaDistancia);
}

static void obtenerUnidadesDistancia(GtkComboBoxText *widget, gpointer user_data)
{
    unidadesDistancia = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)); 
}

//Función que maneja la señal de clic en el botón 
static void confirmar(GtkButton *button, gpointer user_data)
{
    if (tiempo != 0 && unidadesTiempo != NULL && distancia != 0 && unidadesDistancia != NULL)
    {
        cambiarAjustes(tiempo, distancia, unidadesTiempo, unidadesDistancia);
    }
    gtk_window_destroy((GtkWindow *)settings_win);
}

static void
sfg_settings_window_init(SfgSettingsWindow *win)
{
    gtk_widget_init_template(GTK_WIDGET(win));
    tiempo = 0;
    distancia = 0;
    unidadesTiempo = NULL;
    unidadesDistancia = NULL;
}

// Funcion que se llama al cerrar la ventana para liberar recursos
static void
sfg_settings_window_dispose(GObject *object)
{
    G_OBJECT_CLASS(sfg_settings_window_parent_class)->dispose(object);
}

static void
sfg_settings_window_class_init(SfgSettingsWindowClass *class)
{
    G_OBJECT_CLASS(class)->dispose = sfg_settings_window_dispose;
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
                                                "/com/git/sfg/settingsWindow.ui");
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgSettingsWindow, tiempo);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgSettingsWindow, distancia);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), SfgSettingsWindow, botonConfirmar);

    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), obtenerTiempo);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), obtenerDistancia);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), obtenerUnidadesTiempo);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), obtenerUnidadesDistancia);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), confirmar);
}

SfgSettingsWindow *sfg_settings_window_new(SfgMainWindow *win)
{
    settings_win = g_object_new(SFG_SETTINGS_WINDOW_TYPE, "transient-for", win, "use-header-bar", FALSE, NULL);
    return settings_win;
}

void sfg_settings_window_open(SfgMainWindow *win,
                         GFile *file)
{
}
