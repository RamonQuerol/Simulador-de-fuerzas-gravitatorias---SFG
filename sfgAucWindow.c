#include <gtk/gtk.h>

#include "sfgAucWindow.h"
#include "sfgMainWindow.h"

struct _SfgAucWindow
{
    GtkDialog parent;

    GtkWidget *masa;
    GtkWidget *pos_x;
    GtkWidget *pos_y;
    GtkWidget *vel_x;
    GtkWidget *vel_y;
    //  GtkWidget *botonConfirmar;
};

float masa, posX, posY, velX, velY;
G_DEFINE_TYPE(SfgAucWindow, sfg_auc_window, GTK_TYPE_DIALOG);
SfgAucWindow *auc_win;

// Función que maneja la señal de activación de la entrada
static void obtenerMasa(GtkEntry *entry, gpointer user_data)
{
    const gchar *cadenaMasa = gtk_editable_get_text(GTK_EDITABLE(entry));
    masa = atof(cadenaMasa);
    printf("%f", masa);
}

static void obtenerPosX(GtkEntry *entry, gpointer user_data)
{
    const gchar *cadenaPosX = gtk_editable_get_text(GTK_EDITABLE(entry));
    posX = atof(cadenaPosX);
    printf("%f", posX);
}

static void obtenerPosY(GtkEntry *entry, gpointer user_data)
{
    const gchar *cadenaPosY = gtk_editable_get_text(GTK_EDITABLE(entry));
    posY = atof(cadenaPosY);
    printf("%f", posY);
}

static void obtenerVelX(GtkEntry *entry, gpointer user_data)
{
    const gchar *cadenaVelX = gtk_editable_get_text(GTK_EDITABLE(entry));
    velX = atof(cadenaVelX);
    printf("%f", velX);
}

static void obtenerVelY(GtkEntry *entry, gpointer user_data)
{
    const gchar *cadenaVelY = gtk_editable_get_text(GTK_EDITABLE(entry));
    velY = atof(cadenaVelY);
    printf("%f", velY);
    add_cuerpo(masa, posX, posY, velX, velY);
    gtk_window_destroy((GtkWindow *)auc_win);
}
// Función que maneja la señal de clic en el botón
// static void confirmar(GtkButton *button, gpointer user_data)
// {
// }

static void
sfg_auc_window_init(SfgAucWindow *win)
{
    gtk_widget_init_template(GTK_WIDGET(win));
}

// Funcion que se llama al cerrar la ventana para liberar recursos
static void
sfg_auc_window_dispose(GObject *object)
{
    G_OBJECT_CLASS(sfg_auc_window_parent_class)->dispose(object);
}

static void
sfg_auc_window_class_init(SfgAucWindowClass *class)
{
    G_OBJECT_CLASS(class)->dispose = sfg_auc_window_dispose;
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
                                                "/com/git/sfg/aucWindow.ui");
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgAucWindow, masa);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgAucWindow, pos_x);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgAucWindow, pos_y);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgAucWindow, vel_x);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgAucWindow, vel_y);

    //  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), SfgAucWindow, botonConfirmar);

    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), obtenerMasa);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), obtenerPosX);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), obtenerPosY);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), obtenerVelX);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), obtenerVelY);

    // gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), confirmar);
}

SfgAucWindow *sfg_auc_window_new(SfgMainWindow *win)
{
    auc_win = g_object_new(SFG_AUC_WINDOW_TYPE, "transient-for", win, "use-header-bar", FALSE, NULL);
    return auc_win;
}

void sfg_auc_window_open(SfgMainWindow *win,
                         GFile *file)
{
}
