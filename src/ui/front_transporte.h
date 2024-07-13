#ifndef FRONT_TRANSPORTE_H
#define FRONT_TRANSPORTE_H

#include <gtk/gtk.h>

#define WINDOW_WIDTH 1910
#define WINDOW_HEIGHT 1070

void create_main_window(GtkApplication* app, gpointer user_data);
void on_gerenciar_clientes_clicked(GtkButton *button, gpointer user_data);
void show_cadastro_cliente(GtkWidget *widget, gpointer user_data);
void create_card_section(GtkWidget *parent, const char *title, const char **labels, int label_count);
void apply_css(GtkWidget *widget, const gchar *css);


#endif // FRONT_TRANSPORTE_H
