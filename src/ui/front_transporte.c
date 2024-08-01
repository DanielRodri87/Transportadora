#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "../back_transporte/transportadora.h"
#include <front_transporte.h>

#define WINDOW_WIDTH 1910
#define WINDOW_HEIGHT 1070
#define CLIENT_WINDOW_WIDTH 800
#define CLIENT_WINDOW_HEIGHT 800



// ###################################### CADASTRAR CLIENTES ######################################
void apply_css(GtkWidget *widget, const gchar *css)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

typedef struct {
    GtkWidget *entry_nome;
    GtkWidget *entry_cpf;
    GtkWidget *entry_estado;
    GtkWidget *entry_cidade;
    GtkWidget *entry_rua;
    GtkWidget *entry_numero;
    GtkWidget *entry_telefone;
    GtkWidget *entry_email;
} FormData;

void create_card_section(GtkWidget *parent, const char *title, const char **labels, int label_count, FormData *form_data)
{
    GtkWidget *card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(parent), card, FALSE, FALSE, 0);

    GtkWidget *title_label = gtk_label_new(title);
    gtk_widget_set_name(title_label, "section-title");
    gtk_box_pack_start(GTK_BOX(card), title_label, FALSE, FALSE, 10);

    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(card), separator, FALSE, FALSE, 10);

    GtkWidget *grid = gtk_grid_new();
    gtk_widget_set_name(grid, "grid-container");
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_box_pack_start(GTK_BOX(card), grid, FALSE, FALSE, 10);

    for (int i = 0; i < label_count; i += 2) {
        GtkWidget *label1 = gtk_label_new(labels[i]);
        gtk_widget_set_halign(label1, GTK_ALIGN_END);
        gtk_grid_attach(GTK_GRID(grid), label1, 0, i / 2, 1, 1);

        GtkWidget *entry1 = gtk_entry_new();
        gtk_grid_attach(GTK_GRID(grid), entry1, 1, i / 2, 1, 1);

        if (strcmp(labels[i], "Nome:") == 0) form_data->entry_nome = entry1;
        else if (strcmp(labels[i], "Estado:") == 0) form_data->entry_estado = entry1;
        else if (strcmp(labels[i], "Rua:") == 0) form_data->entry_rua = entry1;
        else if (strcmp(labels[i], "Telefone:") == 0) form_data->entry_telefone = entry1;

        if (i + 1 < label_count) {
            GtkWidget *label2 = gtk_label_new(labels[i + 1]);
            gtk_widget_set_halign(label2, GTK_ALIGN_END);
            gtk_grid_attach(GTK_GRID(grid), label2, 2, i / 2, 1, 1);

            GtkWidget *entry2 = gtk_entry_new();
            gtk_grid_attach(GTK_GRID(grid), entry2, 3, i / 2, 1, 1);

            if (strcmp(labels[i + 1], "CPF:") == 0) form_data->entry_cpf = entry2;
            else if (strcmp(labels[i + 1], "Cidade:") == 0) form_data->entry_cidade = entry2;
            else if (strcmp(labels[i + 1], "Número:") == 0) form_data->entry_numero = entry2;
            else if (strcmp(labels[i + 1], "Email:") == 0) form_data->entry_email = entry2;
        }
    }
}

void on_confirm_button_clicked(GtkButton *button, gpointer user_data)
{
    FormData *form_data = (FormData *)user_data;

    const char *nome = gtk_entry_get_text(GTK_ENTRY(form_data->entry_nome));
    const char *cpf = gtk_entry_get_text(GTK_ENTRY(form_data->entry_cpf));
    const char *estado = gtk_entry_get_text(GTK_ENTRY(form_data->entry_estado));
    const char *cidade = gtk_entry_get_text(GTK_ENTRY(form_data->entry_cidade));
    const char *rua = gtk_entry_get_text(GTK_ENTRY(form_data->entry_rua));
    const char *numero = gtk_entry_get_text(GTK_ENTRY(form_data->entry_numero));
    const char *telefone = gtk_entry_get_text(GTK_ENTRY(form_data->entry_telefone));
    const char *email = gtk_entry_get_text(GTK_ENTRY(form_data->entry_email));

    printf("Informações Pessoais:\nNome: %s\nCPF: %s\n", nome, cpf);
    printf("Endereço:\nEstado: %s\nCidade: %s\nRua: %s\nNúmero: %s\n", estado, cidade, rua, numero);
    printf("Contato:\nTelefone: %s\nEmail: %s\n", telefone, email);

    cadastrarCliente(nome, cpf, estado, cidade, rua, atoi(numero), telefone, email);
}

void show_cadastro_cliente(GtkButton *button, gpointer user_data)
{
    GtkWidget *client_vbox = (GtkWidget *)user_data;
    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(client_vbox));
    iter = g_list_next(children);
    while (iter != NULL) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
        iter = g_list_next(iter);
    }
    g_list_free(children);

    GtkWidget *form_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(client_vbox), form_vbox, TRUE, TRUE, 10);

    const gchar *css = "* { background-color: #F0DBC0; color: #333; padding: 20px; border: 1px dashed #ccc; }";
    apply_css(form_vbox, css);

    FormData *form_data = g_malloc(sizeof(FormData));
    memset(form_data, 0, sizeof(FormData)); // Initialize all pointers to NULL

    const char *info_pessoais[] = {"Nome:", "CPF:"};
    create_card_section(form_vbox, "Informações Pessoais", info_pessoais, 2, form_data);

    const char *endereco[] = {"Estado:", "Cidade:", "Rua:", "Número:"};
    create_card_section(form_vbox, "Endereço", endereco, 4, form_data);

    const char *contato[] = {"Telefone:", "Email:"};
    create_card_section(form_vbox, "Contato", contato, 2, form_data);

    GtkWidget *confirm_button = gtk_button_new_with_label("Confirmar");
    gtk_box_pack_start(GTK_BOX(form_vbox), confirm_button, FALSE, FALSE, 10);
    g_signal_connect(confirm_button, "clicked", G_CALLBACK(on_confirm_button_clicked), form_data);

    gtk_widget_show_all(client_vbox);
}

void on_gerenciar_clientes_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *client_window;
    GtkWidget *client_vbox;
    GtkWidget *client_grid;
    GtkWidget *button_cadastrar;
    GtkWidget *button_exibir_clientes;
    GtkWidget *button_buscar;
    GtkWidget *empty_space;

    client_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(client_window), "Gerenciar Clientes");
    gtk_window_set_default_size(GTK_WINDOW(client_window), CLIENT_WINDOW_WIDTH, CLIENT_WINDOW_HEIGHT);

    client_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(client_window), client_vbox);

    client_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(client_grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(client_grid), 10);
    gtk_box_pack_start(GTK_BOX(client_vbox), client_grid, FALSE, FALSE, 10);

    button_cadastrar = gtk_button_new_with_label("Cadastrar Cliente");
    gtk_widget_set_hexpand(button_cadastrar, TRUE);
    gtk_grid_attach(GTK_GRID(client_grid), button_cadastrar, 0, 0, 1, 1);
    g_signal_connect(button_cadastrar, "clicked", G_CALLBACK(show_cadastro_cliente), client_vbox);

    button_exibir_clientes = gtk_button_new_with_label("Exibir Clientes");
    gtk_widget_set_hexpand(button_exibir_clientes, TRUE);
    gtk_grid_attach(GTK_GRID(client_grid), button_exibir_clientes, 1, 0, 1, 1);
    g_signal_connect(button_exibir_clientes, "clicked", G_CALLBACK(show_client_list), client_vbox);

    button_buscar = gtk_button_new_with_label("Buscar Cliente");
    gtk_widget_set_hexpand(button_buscar, TRUE);
    gtk_grid_attach(GTK_GRID(client_grid), button_buscar, 2, 0, 1, 1);
    g_signal_connect(button_buscar, "clicked", G_CALLBACK(on_buscar_button_clicked), NULL);

    empty_space = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(client_vbox), empty_space, TRUE, TRUE, 0);

    gtk_widget_show_all(client_window);
}

void create_main_window(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *grid;
    GtkWidget *button_clientes;
    GtkWidget *button_entregas;
    GtkWidget *button_devolucoes;
    GtkWidget *button_pontuacao;
    GtkWidget *button_sair;
    GtkWidget *logo_image;
    GtkWidget *logo_box;
    GtkCssProvider *cssProvider;

    cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "/home/ritar0drigues/Transportadora/src/ui/style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Transportadora");
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 0);

    button_clientes = gtk_button_new_with_label("Gerenciar Clientes");
    gtk_widget_set_hexpand(button_clientes, TRUE);
    gtk_grid_attach(GTK_GRID(grid), button_clientes, 0, 0, 1, 1);
    g_signal_connect(button_clientes, "clicked", G_CALLBACK(on_gerenciar_clientes_clicked), NULL);

    button_entregas = gtk_button_new_with_label("Gerenciar Entregas");
    gtk_widget_set_hexpand(button_entregas, TRUE);
    gtk_grid_attach(GTK_GRID(grid), button_entregas, 1, 0, 1, 1);

    button_devolucoes = gtk_button_new_with_label("Devoluções");
    gtk_widget_set_hexpand(button_devolucoes, TRUE);
    gtk_grid_attach(GTK_GRID(grid), button_devolucoes, 2, 0, 1, 1);

    button_pontuacao = gtk_button_new_with_label("Pontuação");
    gtk_widget_set_hexpand(button_pontuacao, TRUE);
    gtk_grid_attach(GTK_GRID(grid), button_pontuacao, 3, 0, 1, 1);

    button_sair = gtk_button_new_with_label("Sair");
    gtk_widget_set_hexpand(button_sair, TRUE);
    gtk_grid_attach(GTK_GRID(grid), button_sair, 4, 0, 1, 1);

    logo_image = gtk_image_new_from_file("/home/ritar0drigues/Transportadora/src/ui/logo_transp.png");
    gtk_image_set_pixel_size(GTK_IMAGE(logo_image), 250);

    logo_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(logo_box), logo_image, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), logo_box, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
}

// ###################################################### EXIBIR CLIENTES ######################################################


void on_delete_button_clicked(GtkButton *button, gpointer user_data)
{
    Cliente *cliente = (Cliente *)user_data;

    Cliente **indirect = &lista_clientes;
    while (*indirect != cliente) {
        indirect = &(*indirect)->prox;
    }
    *indirect = cliente->prox;
    free(cliente);

    GtkWidget *client_vbox = gtk_widget_get_parent(gtk_widget_get_parent(GTK_WIDGET(button)));
    show_client_list(NULL, client_vbox);
}

// Função para mostrar a lista de clientes
void show_client_list(GtkButton *button, gpointer user_data)
{
    GtkWidget *client_vbox = (GtkWidget *)user_data;

    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(client_vbox));
    iter = g_list_next(children);
    while (iter != NULL) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
        iter = g_list_next(iter);
    }
    g_list_free(children);

    display_client_list(client_vbox);
}


void display_client_list(GtkWidget *parent)
{
    GtkWidget *list_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(parent), list_vbox, TRUE, TRUE, 0);

    const gchar *css = "* { background-color: #F0DBC0; color: #333; padding: 10px; border: 1px solid #ccc; }";
    apply_css(list_vbox, css);

    Cliente* atual = lista_clientes;

    while (atual != NULL) {
        GtkWidget *client_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        gtk_box_pack_start(GTK_BOX(list_vbox), client_box, FALSE, FALSE, 0);

        const gchar *client_box_css = "box { border-bottom: 1px solid #aaa; }";
        apply_css(client_box, client_box_css);

        GtkWidget *label_nome = gtk_label_new(atual->nome);
        gtk_widget_set_hexpand(label_nome, TRUE);
        gtk_box_pack_start(GTK_BOX(client_box), label_nome, FALSE, FALSE, 5);

        GtkWidget *label_cpf = gtk_label_new(atual->cpf);
        gtk_widget_set_hexpand(label_cpf, TRUE);
        gtk_box_pack_start(GTK_BOX(client_box), label_cpf, FALSE, FALSE, 5);

        GtkWidget *label_email = gtk_label_new(atual->email);
        gtk_widget_set_hexpand(label_email, TRUE);
        gtk_box_pack_start(GTK_BOX(client_box), label_email, FALSE, FALSE, 5);

        GtkWidget *delete_button = gtk_button_new_with_label("Delete");
        gtk_widget_set_name(delete_button, "delete-button");
        gtk_box_pack_start(GTK_BOX(client_box), delete_button, FALSE, FALSE, 5);

        g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_button_clicked), atual);

        // Estilo para o botão delete
        const gchar *delete_button_css = 
            "#delete-button { "
            "background-color: red; "
            "border-radius: 8px; "
            "padding: 5px 10px; "
            "transition: background-color 0.3s ease; "
            "}"
            "#delete-button:hover { "
            "background-color: darkred; "
            "}";
        apply_css(delete_button, delete_button_css);

        atual = atual->prox;
    }

    gtk_widget_show_all(parent);
}

// ###################################################### BUSCAR CLIENTES ######################################################


void show_buscar_cliente(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *entry;
    GtkWidget *buscar_button;
    GtkWidget *cancel_button;

    // Criar o diálogo de busca de clientes
    dialog = gtk_dialog_new_with_buttons("Buscar Cliente",
                                         GTK_WINDOW(user_data),
                                         GTK_DIALOG_MODAL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Criar uma entrada de texto para inserir o CPF do cliente
    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Digite o CPF do Cliente");
    gtk_box_pack_start(GTK_BOX(content_area), entry, TRUE, TRUE, 0);

    // Criar o botão de buscar e conectar a função de callback
    buscar_button = gtk_button_new_with_label("Buscar");
    g_signal_connect(buscar_button, "clicked", G_CALLBACK(on_buscar_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(content_area), buscar_button, TRUE, TRUE, 0);

    // Criar o botão de cancelar
    cancel_button = gtk_button_new_with_label("Cancelar");
    g_signal_connect(cancel_button, "clicked", G_CALLBACK(gtk_widget_destroy), dialog);
    gtk_box_pack_start(GTK_BOX(content_area), cancel_button, TRUE, TRUE, 0);

    gtk_widget_show_all(dialog);
}

void on_buscar_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog;
    GtkEntry *entry = GTK_ENTRY(user_data);
    const gchar *cpf = gtk_entry_get_text(entry);

    // Obter a transportadora (ou outra estrutura que contenha a lista de clientes)
    Transportadora *transportadora = /* Código para obter a instância de Transportadora */;
    
    // Buscar o cliente
    Cliente *cliente_encontrado = buscar_cliente(transportadora, cpf);

    // Exibir o resultado da busca
    if (cliente_encontrado) {
        dialog = gtk_message_dialog_new(NULL,
                                        GTK_DIALOG_MODAL,
                                        GTK_MESSAGE_INFO,
                                        GTK_BUTTONS_OK,
                                        "Cliente com CPF '%s' encontrado!",
                                        cpf);
    } else {
        dialog = gtk_message_dialog_new(NULL,
                                        GTK_DIALOG_MODAL,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_OK,
                                        "Cliente com CPF '%s' não encontrado.",
                                        cpf);
    }

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}