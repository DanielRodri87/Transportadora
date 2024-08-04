#include <gtk/gtk.h>
#include <front_transporte.h>

static void activate(GtkApplication* app, gpointer user_data)
{
    create_main_window(app, user_data);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    inicializar_fila_devolucao();


    app = gtk_application_new("org.gtk.transportadora", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}