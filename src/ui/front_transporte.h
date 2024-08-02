#ifndef FRONT_TRANSPORTE_H
#define FRONT_TRANSPORTE_H

#include "../back_transporte/transportadora.h"


void apply_css(GtkWidget *widget, const gchar *css);
void on_confirm_button_clicked(GtkButton *button, gpointer user_data);
void show_cadastro_cliente(GtkButton *button, gpointer user_data);
void on_gerenciar_clientes_clicked(GtkButton *button, gpointer user_data);
void create_main_window(GtkApplication *app, gpointer user_data);
void on_delete_button_clicked(GtkButton *button, gpointer user_data);
void show_client_list(GtkButton *button, gpointer user_data);
void display_client_list(GtkWidget *parent);
void on_more_button_clicked(GtkButton *button, gpointer user_data);

void on_iniciar_rota_clicked(GtkButton *button, gpointer user_data);
void on_gerenciar_transportadora_clicked(GtkButton *button, gpointer user_data);
void on_adicionar_produto_cliente_clicked(GtkButton *button, gpointer user_data);
void on_adicionar_cliente_rota_clicked();
void on_adicionar_produto_cliente_clicked();
void on_mostrar_fila_clicked();
void on_concluir_rota_clicked();
void on_entrega_volta_clicked();
void on_entrega_ida_clicked();
void on_exibir_produtos_clicked(GtkButton *button, gpointer user_data);


void on_anterior_cliente_clicked(GtkButton *button, gpointer user_data);
void on_proximo_cliente_clicked(GtkButton *button, gpointer user_data);
void exibir_cliente_atual();
void on_mostrar_fila_clicked(GtkButton *button, gpointer user_data);
void on_pesquisar_cliente();

void listar_clientes_fila_devolucao(GtkWidget *box, Devolucao *fila_devolucao);
void concluir_entrega_ida();
void tentar_novamente_entrega_ida();
void on_delete_produto_button_clicked(GtkButton *button, gpointer user_data);
void on_pontuacao_clicked(GtkButton *button, gpointer user_data);
void on_ok_button_clicked(GtkButton *button, gpointer user_data);


#endif // FRONT_TRANSPORTE_H