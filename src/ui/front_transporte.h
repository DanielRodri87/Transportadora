#ifndef FRONT_TRANSPORTE_H
#define FRONT_TRANSPORTE_H

void apply_css(GtkWidget *widget, const gchar *css);
void on_confirm_button_clicked(GtkButton *button, gpointer user_data);
void show_cadastro_cliente(GtkButton *button, gpointer user_data);
void on_gerenciar_clientes_clicked(GtkButton *button, gpointer user_data);
void create_main_window(GtkApplication *app, gpointer user_data);
void on_delete_button_clicked(GtkButton *button, gpointer user_data);
void show_client_list(GtkButton *button, gpointer user_data);
void display_client_list(GtkWidget *parent);
void on_more_button_clicked(GtkButton *button, gpointer user_data);

#endif // FRONT_TRANSPORTE_H
