#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "../back_transporte/transportadora.h"
#include <front_transporte.h>

#define WINDOW_WIDTH 1910
#define WINDOW_HEIGHT 1070
#define CLIENT_WINDOW_WIDTH 800
#define CLIENT_WINDOW_HEIGHT 800

int score;

Transportadora *transportadora = NULL;
TransportadoraFila *fila = NULL; 

Cliente *cliente_atual = NULL;
GtkWidget *dialog = NULL;
GtkWidget *nome_label, *cpf_label, *estado_label, *cidade_label, *rua_label, *numero_label, *telefone_label, *email_label;


// ###################################### CADASTRAR CLIENTES ######################################
void apply_css(GtkWidget *widget, const gchar *css)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

typedef struct
{
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

    for (int i = 0; i < label_count; i += 2)
    {
        GtkWidget *label1 = gtk_label_new(labels[i]);
        gtk_widget_set_halign(label1, GTK_ALIGN_END);
        gtk_grid_attach(GTK_GRID(grid), label1, 0, i / 2, 1, 1);

        GtkWidget *entry1 = gtk_entry_new();
        gtk_grid_attach(GTK_GRID(grid), entry1, 1, i / 2, 1, 1);

        if (strcmp(labels[i], "Nome:") == 0)
            form_data->entry_nome = entry1;
        else if (strcmp(labels[i], "Estado:") == 0)
            form_data->entry_estado = entry1;
        else if (strcmp(labels[i], "Rua:") == 0)
            form_data->entry_rua = entry1;
        else if (strcmp(labels[i], "Telefone:") == 0)
            form_data->entry_telefone = entry1;

        if (i + 1 < label_count)
        {
            GtkWidget *label2 = gtk_label_new(labels[i + 1]);
            gtk_widget_set_halign(label2, GTK_ALIGN_END);
            gtk_grid_attach(GTK_GRID(grid), label2, 2, i / 2, 1, 1);

            GtkWidget *entry2 = gtk_entry_new();
            gtk_grid_attach(GTK_GRID(grid), entry2, 3, i / 2, 1, 1);

            if (strcmp(labels[i + 1], "CPF:") == 0)
                form_data->entry_cpf = entry2;
            else if (strcmp(labels[i + 1], "Cidade:") == 0)
                form_data->entry_cidade = entry2;
            else if (strcmp(labels[i + 1], "Número:") == 0)
                form_data->entry_numero = entry2;
            else if (strcmp(labels[i + 1], "Email:") == 0)
                form_data->entry_email = entry2;
        }
    }
}

void on_confirm_button_clicked(GtkButton *button, gpointer user_data)
{
    (void)button;

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

    cadastrar_cliente(nome, cpf, estado, cidade, rua, atoi(numero), telefone, email);
}

void show_cadastro_cliente(GtkButton *button, gpointer user_data)
{
    (void)button;
    (void)user_data;

    GtkWidget *client_vbox = (GtkWidget *)user_data;
    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(client_vbox));
    iter = g_list_next(children);
    while (iter != NULL)
    {
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

void on_gerenciar_clientes_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *client_window;
    GtkWidget *client_vbox;
    GtkWidget *client_grid;
    GtkWidget *button_cadastrar;
    GtkWidget *button_exibir_clientes;
    GtkWidget *button_buscar;
    GtkWidget *empty_space;

    (void)button;
    (void)user_data;

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
    g_signal_connect(button_buscar, "clicked", G_CALLBACK(on_pesquisar_cliente), client_vbox);


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

    (void)user_data;

    cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "/mnt/c/Users/danie/OneDrive/Documentos/UFPI-2024.1/PROJETOS/Transportadora/src/ui/style.css", NULL);
    // gtk_css_provider_load_from_path(/home/ritar0drigues/Transportadora/Transportadora-1/src/ui/style.css", NULL);
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
    g_signal_connect(button_entregas, "clicked", G_CALLBACK(on_gerenciar_transportadora_clicked), NULL);

    button_devolucoes = gtk_button_new_with_label("Devoluções");
    gtk_widget_set_hexpand(button_devolucoes, TRUE);
    gtk_grid_attach(GTK_GRID(grid), button_devolucoes, 2, 0, 1, 1);
    // g_signal_connect(button_devolucoes, "clicked", G_CALLBACK(on_devolucoes_clicked), NULL);

    button_pontuacao = gtk_button_new_with_label("Pontuação");
    gtk_widget_set_hexpand(button_pontuacao, TRUE);
    gtk_grid_attach(GTK_GRID(grid), button_pontuacao, 3, 0, 1, 1);
    g_signal_connect(button_pontuacao, "clicked", G_CALLBACK(on_pontuacao_clicked), NULL);

    button_sair = gtk_button_new_with_label("Sair");
    gtk_widget_set_hexpand(button_sair, TRUE);
    gtk_grid_attach(GTK_GRID(grid), button_sair, 5, 0, 1, 1);

    logo_image = gtk_image_new_from_file("/mnt/c/Users/danie/OneDrive/Documentos/UFPI-2024.1/PROJETOS/Transportadora/src/ui/logo_transp.png");
    // logo_image = gtk_image_new_from_file("/home/ritar0drigues/Transportadora/Transportadora-1/src/ui/logo_transp.png");

    gtk_image_set_pixel_size(GTK_IMAGE(logo_image), 250);

    logo_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(logo_box), logo_image, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), logo_box, TRUE, TRUE, 0);

    g_signal_connect(button_sair, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
}


// ###################################### PONTUAÇÃO ######################################
void on_pontuacao_clicked(GtkButton *button, gpointer user_data)
{
    (void)button;
    (void)user_data;

    GtkWidget *pontuacao_window;
    GtkWidget *pontuacao_label;
    gchar *pontuacao_text;

    pontuacao_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(pontuacao_window), "Pontuação");
    gtk_window_set_default_size(GTK_WINDOW(pontuacao_window), 200, 200);

    if (transportadora == NULL)
    {
        pontuacao_text = "Pontuação: 0";
    }
    else
    {
        pontuacao_text = g_strdup_printf("Pontuação: %d", transportadora->score);
    }

    pontuacao_label = gtk_label_new(pontuacao_text);
    gtk_container_add(GTK_CONTAINER(pontuacao_window), pontuacao_label);

    gtk_widget_show_all(pontuacao_window);

}

// ##################################################### DEVOLUÇÕES #####################################################
// exibir as pessoas que tiveram produtos devolvidos, e seus respectivos produtos




// ###################################################### EXIBIR CLIENTES ######################################################

void on_delete_button_clicked(GtkButton *button, gpointer user_data)
{
    (void)button;
    Cliente *cliente = (Cliente *)user_data;

    Cliente **indirect = &lista_clientes;
    while (*indirect != cliente)
    {
        indirect = &(*indirect)->prox;
    }
    *indirect = cliente->prox;
    free(cliente);

    GtkWidget *client_vbox = gtk_widget_get_parent(gtk_widget_get_parent(GTK_WIDGET(button)));
    show_client_list(NULL, client_vbox);
}

void show_client_list(GtkButton *button, gpointer user_data)
{
    (void)button;
    GtkWidget *client_vbox = (GtkWidget *)user_data;

    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(client_vbox));
    iter = g_list_next(children);
    while (iter != NULL)
    {
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

    Cliente *atual = lista_clientes;

    while (atual != NULL)
    {
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

        GtkWidget *delete_button = gtk_button_new_with_label("Del");
        gtk_widget_set_name(delete_button, "delete-button");
        gtk_box_pack_start(GTK_BOX(client_box), delete_button, FALSE, FALSE, 5);

        g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_button_clicked), atual);

        GtkWidget *more_button = gtk_button_new_with_label("Mais");
        gtk_widget_set_name(more_button, "more-button");
        gtk_box_pack_start(GTK_BOX(client_box), more_button, FALSE, FALSE, 5);

        g_signal_connect(more_button, "clicked", G_CALLBACK(on_more_button_clicked), atual);

        // Estilo para os botões
        const gchar *buttons_css =
            "#delete-button { "
            "background-color: red; "
            "border-radius: 8px; "
            "padding: 5px 10px; "
            "transition: background-color 0.3s ease; "
            "}"
            "#delete-button:hover { "
            "background-color: darkred; "
            "}"
            "#more-button { "
            "background-color: blue; "
            "border-radius: 8px; "
            "padding: 5px 10px; "
            "transition: background-color 0.3s ease; "
            "}"
            "#more-button:hover { "
            "background-color: darkblue; "
            "}";
        apply_css(delete_button, buttons_css);
        apply_css(more_button, buttons_css);

        atual = atual->prox;
    }

    gtk_widget_show_all(parent);
}

// ####################################### MAIS INFORMAÇÕES #######################################
void on_more_button_clicked(GtkButton *button, gpointer user_data)
{
    (void)button;
    Cliente *cliente = (Cliente *)user_data;

    // Verifique se cliente ou qualquer campo específico é NULL
    if (cliente == NULL)
    {
        printf("Cliente é NULL.\n");
        return;
    }

    char numero[10];
    sprintf(numero, "%d", cliente->numero);

    // Verificar todos os campos relevantes
    if (cliente->nome == NULL || cliente->cpf == NULL || cliente->estado == NULL ||
        cliente->cidade == NULL || cliente->rua == NULL || numero == NULL ||
        cliente->telefone == NULL || cliente->email == NULL)
    {
        printf("Um ou mais campos do cliente são NULL.\n");
        return;
    }

    GtkWidget *dialog = gtk_dialog_new_with_buttons("Detalhes do Cliente", NULL, GTK_DIALOG_MODAL, ("Fechar"), GTK_RESPONSE_CLOSE, NULL);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 400);
    gtk_widget_override_background_color(dialog, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){.red = 0.941, .green = 0.859, .blue = 0.753, .alpha = 1.0});

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    // Adicionando a seção de Informações Pessoais
    GtkWidget *label_section_personal = gtk_label_new("Informações Pessoais:");
    gtk_grid_attach(GTK_GRID(grid), label_section_personal, 0, 0, 1, 1);
    gtk_widget_set_halign(label_section_personal, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(label_section_personal, 10);

    GtkWidget *label_nome = gtk_label_new(cliente->nome);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nome:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_nome, 1, 1, 1, 1);

    GtkWidget *label_cpf = gtk_label_new(cliente->cpf);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("CPF:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_cpf, 1, 2, 1, 1);

    // Adicionando a seção de Endereço
    GtkWidget *label_section_address = gtk_label_new("Endereço:");
    gtk_grid_attach(GTK_GRID(grid), label_section_address, 0, 3, 1, 1);
    gtk_widget_set_halign(label_section_address, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(label_section_address, 10);

    GtkWidget *label_estado = gtk_label_new(cliente->estado);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Estado:"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_estado, 1, 4, 1, 1);

    GtkWidget *label_cidade = gtk_label_new(cliente->cidade);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Cidade:"), 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_cidade, 1, 5, 1, 1);

    GtkWidget *label_rua = gtk_label_new(cliente->rua);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Rua:"), 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_rua, 1, 6, 1, 1);

    GtkWidget *label_numero = gtk_label_new(numero);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Número:"), 0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_numero, 1, 7, 1, 1);

    GtkWidget *label_section_contact = gtk_label_new("Contato:");
    gtk_grid_attach(GTK_GRID(grid), label_section_contact, 0, 8, 1, 1);
    gtk_widget_set_halign(label_section_contact, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(label_section_contact, 10);

    GtkWidget *label_telefone = gtk_label_new(cliente->telefone);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Telefone:"), 0, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_telefone, 1, 9, 1, 1);

    GtkWidget *label_email = gtk_label_new(cliente->email);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Email:"), 0, 10, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_email, 1, 10, 1, 1);

    gtk_widget_show_all(dialog);

    g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_destroy), NULL);
}

// ############################# GERENCIAR TRANSPORTADORA #############################
void on_gerenciar_transportadora_clicked(GtkButton *button, gpointer user_data)
{
    (void)button;
    (void)user_data;

    GtkWidget *transport_window;
    GtkWidget *transport_vbox;
    GtkWidget *transport_grid;
    GtkWidget *button_iniciar_rota;
    GtkWidget *button_adicionar_cliente;
    GtkWidget *button_adicionar_produto;
    GtkWidget *button_mostrar_fila;
    GtkWidget *button_concluir_fila;
    GtkWidget *button_concluir_rota;
    GtkWidget *button_entrega_ida;
    GtkWidget *button_entrega_volta;
    GtkWidget *button_exibir_produtos;
    GtkWidget *empty_space;

    transport_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(transport_window), "Gerenciar Transportadora");
    gtk_window_set_default_size(GTK_WINDOW(transport_window), CLIENT_WINDOW_WIDTH, CLIENT_WINDOW_HEIGHT);

    transport_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(transport_window), transport_vbox);

    transport_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(transport_grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(transport_grid), 10);
    gtk_box_pack_start(GTK_BOX(transport_vbox), transport_grid, FALSE, FALSE, 10);

    button_iniciar_rota = gtk_button_new_with_label("Iniciar Rota");
    gtk_widget_set_hexpand(button_iniciar_rota, TRUE);
    gtk_grid_attach(GTK_GRID(transport_grid), button_iniciar_rota, 0, 0, 1, 1);
    g_signal_connect(button_iniciar_rota, "clicked", G_CALLBACK(on_iniciar_rota_clicked), NULL);

    button_adicionar_cliente = gtk_button_new_with_label("Adicionar Cliente");
    gtk_widget_set_hexpand(button_adicionar_cliente, TRUE);
    gtk_grid_attach(GTK_GRID(transport_grid), button_adicionar_cliente, 1, 0, 1, 1);
    g_signal_connect(button_adicionar_cliente, "clicked", G_CALLBACK(on_adicionar_cliente_rota_clicked), NULL);

    button_adicionar_produto = gtk_button_new_with_label("Adicionar Produto");
    gtk_widget_set_hexpand(button_adicionar_produto, TRUE);
    gtk_grid_attach(GTK_GRID(transport_grid), button_adicionar_produto, 2, 0, 1, 1);
    g_signal_connect(button_adicionar_produto, "clicked", G_CALLBACK(on_adicionar_produto_cliente_clicked), NULL);

    button_mostrar_fila = gtk_button_new_with_label("Mostrar Fila");
    gtk_widget_set_hexpand(button_mostrar_fila, TRUE);
    gtk_grid_attach(GTK_GRID(transport_grid), button_mostrar_fila, 0, 1, 1, 1);
    g_signal_connect(button_mostrar_fila, "clicked", G_CALLBACK(on_mostrar_fila_clicked), NULL);

    button_exibir_produtos = gtk_button_new_with_label("Exibir Produtos");
    gtk_widget_set_hexpand(button_exibir_produtos, TRUE);
    gtk_grid_attach(GTK_GRID(transport_grid), button_exibir_produtos, 1, 1, 1, 1);
    g_signal_connect(button_exibir_produtos, "clicked", G_CALLBACK(on_exibir_produtos_clicked), NULL);

    button_concluir_rota = gtk_button_new_with_label("Concluir Rota");
    gtk_widget_set_hexpand(button_concluir_rota, TRUE);
    gtk_grid_attach(GTK_GRID(transport_grid), button_concluir_rota, 2, 1, 1, 1);
    g_signal_connect(button_concluir_rota, "clicked", G_CALLBACK(on_concluir_rota_clicked), NULL);

    button_entrega_ida = gtk_button_new_with_label("Entrega Ida");
    gtk_widget_set_hexpand(button_entrega_ida, TRUE);
    gtk_grid_attach(GTK_GRID(transport_grid), button_entrega_ida, 0, 2, 1, 1);
    g_signal_connect(button_entrega_ida, "clicked", G_CALLBACK(on_entrega_ida_clicked), NULL);

    button_entrega_volta = gtk_button_new_with_label("Entrega Volta");
    gtk_widget_set_hexpand(button_entrega_volta, TRUE);
    gtk_grid_attach(GTK_GRID(transport_grid), button_entrega_volta, 1, 2, 1, 1);
    g_signal_connect(button_entrega_volta, "clicked", G_CALLBACK(on_entrega_volta_clicked), NULL);

    empty_space = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(transport_vbox), empty_space, TRUE, TRUE, 0);

    gtk_widget_show_all(transport_window);

    // Inicialize a transportadora global
    if (transportadora == NULL) {
        transportadora = (Transportadora *)malloc(sizeof(Transportadora));
        if (transportadora == NULL) {
            fprintf(stderr, "Erro ao alocar memória para a transportadora.\n");
            return;
        }
        inicializar_transportadora(transportadora);
    }

    // Inicialize a fila de devolução global
    if (fila == NULL) {
        fila = (TransportadoraFila *)malloc(sizeof(TransportadoraFila));
        if (fila == NULL) {
            fprintf(stderr, "Erro ao alocar memória para a TransportadoraFila.\n");
            return;
        }
        fila->fila_devolucao = (Devolucao *)malloc(sizeof(Devolucao));
        if (fila->fila_devolucao == NULL) {
            fprintf(stderr, "Erro ao alocar memória para a Devolucao.\n");
            free(fila);
            fila = NULL;
            return;
        }
        fila->fila_devolucao->ini = NULL;
        fila->fila_devolucao->fim = NULL;
    }
}


void on_iniciar_rota_clicked(GtkButton *button, gpointer user_data)
{
    (void)button;
    (void)user_data;

    if (transportadora == NULL)
    {
        fprintf(stderr, "Erro: Transportadora não inicializada.\n");
        return;
    }

    ativar_rota(transportadora);
}


void on_adicionar_cliente_rota_clicked(GtkButton *button, gpointer user_data)
{
    (void)button;
    (void)user_data;

    if (transportadora == NULL)
    {
        printf("Erro: Transportadora inválida.\n");
        return;
    }

    // Criar uma janela de diálogo para pedir o CPF
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *entry;
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;

    dialog = gtk_dialog_new_with_buttons("Buscar Cliente",
                                         NULL,
                                         flags,
                                         ("_OK"),
                                         GTK_RESPONSE_ACCEPT,
                                         ("_Cancel"),
                                         GTK_RESPONSE_REJECT,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry), 11);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Digite o CPF do Cliente");
    gtk_container_add(GTK_CONTAINER(content_area), entry);

    gtk_widget_show_all(dialog);

    // Conectar a resposta do diálogo
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        const gchar *cpf = gtk_entry_get_text(GTK_ENTRY(entry));

        // Buscar o cliente
        Cliente *buscado = buscarClientePorCPF(cpf);

        if (buscado == NULL) {
            printf("Cliente com CPF %s não encontrado.\n", cpf);
        } else {
            // Cadastrar o cliente na rota
            printf("Cliente encontrado: %s\n", buscado->nome);
            cadastrar_cliente_rota(transportadora, buscado);
        }
    }

    gtk_widget_destroy(dialog);
}

void on_pesquisar_cliente()
{
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *entry;
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;

    dialog = gtk_dialog_new_with_buttons("Buscar Cliente",
                                         NULL,
                                         flags,
                                         ("_OK"),
                                         GTK_RESPONSE_ACCEPT,
                                         ("_Cancel"),
                                         GTK_RESPONSE_REJECT,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry), 11);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Digite o CPF do Cliente");
    gtk_container_add(GTK_CONTAINER(content_area), entry);

    gtk_widget_show_all(dialog);

    // Conectar a resposta do diálogo
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        const gchar *cpf = gtk_entry_get_text(GTK_ENTRY(entry));

        // Buscar o cliente
        Cliente *buscado = buscarClientePorCPF(cpf);

        if (buscado == NULL) {
            printf("Cliente com CPF %s não encontrado.\n", cpf);
        } else {
            // Exibir as informações do cliente
            printf("Cliente encontrado: %s\n", buscado->nome);
            on_more_button_clicked(NULL, buscado);
        }
    }

    gtk_widget_destroy(dialog);
}

void on_adicionar_produto_cliente_clicked(GtkButton *button, gpointer user_data)
{
    (void)button;
    (void)user_data;

    // Criar uma janela de diálogo para pedir o CPF, ID do produto e nome do produto
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *cpf_entry, *id_entry, *nome_entry;
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;

    dialog = gtk_dialog_new_with_buttons("Adicionar Produto ao Cliente",
                                         NULL,
                                         flags,
                                         ("_OK"),
                                         GTK_RESPONSE_ACCEPT,
                                         ("_Cancel"),
                                         GTK_RESPONSE_REJECT,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    
    // Criar entradas para CPF, ID do produto e nome do produto
    cpf_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(cpf_entry), 11);
    gtk_entry_set_placeholder_text(GTK_ENTRY(cpf_entry), "Digite o CPF do Cliente");
    gtk_container_add(GTK_CONTAINER(content_area), cpf_entry);

    id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_entry), "Digite o ID do Produto");
    gtk_container_add(GTK_CONTAINER(content_area), id_entry);

    nome_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(nome_entry), "Digite o Nome do Produto");
    gtk_container_add(GTK_CONTAINER(content_area), nome_entry);

    gtk_widget_show_all(dialog);

    // Conectar a resposta do diálogo
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        const gchar *cpf = gtk_entry_get_text(GTK_ENTRY(cpf_entry));
        const gchar *id_str = gtk_entry_get_text(GTK_ENTRY(id_entry));
        const gchar *nome = gtk_entry_get_text(GTK_ENTRY(nome_entry));

        // Converter id_str para int
        int id = atoi(id_str);

        // Buscar o cliente
        Cliente *buscado = buscarClientePorCPF(cpf);

        if (buscado == NULL) {
            printf("Cliente com CPF %s não encontrado.\n", cpf);
        } else {
            // Cadastrar o produto para o cliente
            cadastrar_produto_cliente(transportadora, buscado, id, nome);
        }
    }

    gtk_widget_destroy(dialog);
}

void set_background_color(GtkWidget *widget, const char *color)
{
    GdkRGBA bg_color;
    gdk_rgba_parse(&bg_color, color);
    gtk_widget_override_background_color(widget, GTK_STATE_FLAG_NORMAL, &bg_color);
}

void on_mostrar_fila_clicked(GtkButton *button, gpointer user_data)
{
    if (transportadora == NULL || transportadora->rota_on == NULL || transportadora->rota_on->tentativa1 == NULL)
    {
        printf("Erro: Fila de clientes vazia ou transportadora não inicializada.\n");
        return;
    }

    cliente_atual = transportadora->rota_on->tentativa1;

    // Criar uma janela de diálogo para exibir as informações do cliente
    dialog = gtk_dialog_new_with_buttons("Fila de Clientes",
                                         NULL,
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 400);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Caixa vertical para organizar os labels
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_box_set_homogeneous(GTK_BOX(vbox), FALSE);
    gtk_container_add(GTK_CONTAINER(content_area), vbox);

    // Adicionar cor de fundo
    set_background_color(vbox, "#F0DBC0");

    // Labels para exibir as informações do cliente
    nome_label = gtk_label_new("");
    cpf_label = gtk_label_new("");
    estado_label = gtk_label_new("");
    cidade_label = gtk_label_new("");
    rua_label = gtk_label_new("");
    numero_label = gtk_label_new("");
    telefone_label = gtk_label_new("");
    email_label = gtk_label_new("");

    // Adicionar labels na vbox
    gtk_box_pack_start(GTK_BOX(vbox), nome_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), cpf_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), estado_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), cidade_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), rua_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), numero_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), telefone_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), email_label, FALSE, FALSE, 5);

    // Caixa horizontal para os botões de navegação
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 10);

    // Adicionar cor de fundo
    set_background_color(hbox, "#F0DBC0");

    // Botões de navegação
    GtkWidget *ant_button = gtk_button_new_with_label("Anterior");
    GtkWidget *prox_button = gtk_button_new_with_label("Próximo");
    gtk_box_pack_start(GTK_BOX(hbox), ant_button, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(hbox), prox_button, TRUE, TRUE, 10);

    // Conectar os sinais dos botões às funções de navegação
    g_signal_connect(prox_button, "clicked", G_CALLBACK(on_proximo_cliente_clicked), NULL);
    g_signal_connect(ant_button, "clicked", G_CALLBACK(on_anterior_cliente_clicked), NULL);

    gtk_widget_show_all(dialog);

    // Exibir o primeiro cliente
    exibir_cliente_atual();
}

void exibir_cliente_atual()
{
    if (cliente_atual == NULL) {
        return;
    }

    char buffer[256];

    snprintf(buffer, sizeof(buffer), "Nome: %s", cliente_atual->nome);
    gtk_label_set_text(GTK_LABEL(nome_label), buffer);

    snprintf(buffer, sizeof(buffer), "CPF: %s", cliente_atual->cpf);
    gtk_label_set_text(GTK_LABEL(cpf_label), buffer);

    snprintf(buffer, sizeof(buffer), "Estado: %s", cliente_atual->estado);
    gtk_label_set_text(GTK_LABEL(estado_label), buffer);

    snprintf(buffer, sizeof(buffer), "Cidade: %s", cliente_atual->cidade);
    gtk_label_set_text(GTK_LABEL(cidade_label), buffer);

    snprintf(buffer, sizeof(buffer), "Rua: %s", cliente_atual->rua);
    gtk_label_set_text(GTK_LABEL(rua_label), buffer);

    snprintf(buffer, sizeof(buffer), "Número: %d", cliente_atual->numero);
    gtk_label_set_text(GTK_LABEL(numero_label), buffer);

    snprintf(buffer, sizeof(buffer), "Telefone: %s", cliente_atual->telefone);
    gtk_label_set_text(GTK_LABEL(telefone_label), buffer);

    snprintf(buffer, sizeof(buffer), "Email: %s", cliente_atual->email);
    gtk_label_set_text(GTK_LABEL(email_label), buffer);
}

void on_proximo_cliente_clicked(GtkButton *button, gpointer user_data)
{
    (void)button;
    (void)user_data;
    
    if (cliente_atual != NULL && cliente_atual->prox != NULL) {
        cliente_atual = cliente_atual->prox;
        exibir_cliente_atual();
    }
}

void on_anterior_cliente_clicked(GtkButton *button, gpointer user_data)
{
    if (cliente_atual != NULL && transportadora->rota_on != NULL) {
        Cliente *anterior = NULL;
        Cliente *atual = transportadora->rota_on->tentativa1;
        while (atual != cliente_atual) {
            anterior = atual;
            atual = atual->prox;
        }
        if (anterior != NULL) {
            cliente_atual = anterior;
            exibir_cliente_atual();
        }
    }
}


void on_concluir_rota_clicked()
{
    if (transportadora == NULL || transportadora->rota_on == NULL)
    {
        printf("Erro: Transportadora ou rota não inicializada.\n");
        return;
    }

    concluir_rota(transportadora);
}

void on_sim_button_clicked_ida(GtkButton *button, gpointer user_data)
{
    GtkWidget *dialog = GTK_WIDGET(user_data);
    gtk_widget_destroy(dialog);
    score += 5;
    transportadora->score = score;
    concluir_entrega_ida();

    on_exibir_produtos_clicked(NULL, NULL);

}

// Função chamada quando o botão Não é clicado
void on_nao_button_clicked_ida(GtkButton *button, gpointer user_data)
{
    GtkWidget *dialog = GTK_WIDGET(user_data);
    gtk_widget_destroy(dialog);
    tentar_novamente_entrega_ida();
}

void on_entrega_ida_clicked(GtkButton *button, gpointer user_data)
{
    if (transportadora == NULL || transportadora->rota_on == NULL || transportadora->rota_on->tentativa1 == NULL)
    {
        printf("Erro: Fila de clientes vazia ou transportadora não inicializada.\n");
        return;
    }

    cliente_atual = transportadora->rota_on->tentativa1;

    // Criar uma janela de diálogo para exibir as informações do cliente e perguntar sobre a entrega
    dialog = gtk_dialog_new_with_buttons("Entrega na Ida",
                                         NULL,
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 400);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Caixa vertical para organizar os labels
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_box_set_homogeneous(GTK_BOX(vbox), FALSE);
    gtk_container_add(GTK_CONTAINER(content_area), vbox);

    // Adicionar cor de fundo
    set_background_color(vbox, "#F0DBC0");

    // Criar e adicionar um título ao diálogo
    GtkWidget *titulo_label = gtk_label_new("Informações do Cliente");
    gtk_widget_set_name(titulo_label, "titulo-label");
    gtk_box_pack_start(GTK_BOX(vbox), titulo_label, FALSE, FALSE, 10);

    // Labels para exibir as informações do cliente
    nome_label = gtk_label_new("");
    cpf_label = gtk_label_new("");
    estado_label = gtk_label_new("");
    cidade_label = gtk_label_new("");
    rua_label = gtk_label_new("");
    numero_label = gtk_label_new("");
    telefone_label = gtk_label_new("");
    email_label = gtk_label_new("");

    // Adicionar labels na vbox
    gtk_box_pack_start(GTK_BOX(vbox), nome_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), cpf_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), estado_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), cidade_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), rua_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), numero_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), telefone_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), email_label, FALSE, FALSE, 5);

    // Pergunta de confirmação
    GtkWidget *pergunta_label = gtk_label_new("A entrega foi realizada com sucesso?");
    gtk_widget_set_name(pergunta_label, "pergunta-label");
    gtk_box_pack_start(GTK_BOX(vbox), pergunta_label, FALSE, FALSE, 20);

    // Caixa horizontal para os botões
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(button_box), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, FALSE, FALSE, 10);

    // Botões Sim e Não
    GtkWidget *sim_button = gtk_button_new_with_label("Sim");
    gtk_widget_set_name(sim_button, "sim-button");
    g_signal_connect(sim_button, "clicked", G_CALLBACK(on_sim_button_clicked_ida), dialog);
    gtk_box_pack_start(GTK_BOX(button_box), sim_button, TRUE, TRUE, 10);

    GtkWidget *nao_button = gtk_button_new_with_label("Não");
    gtk_widget_set_name(nao_button, "nao-button");
    g_signal_connect(nao_button, "clicked", G_CALLBACK(on_nao_button_clicked_ida), dialog);
    gtk_box_pack_start(GTK_BOX(button_box), nao_button, TRUE, TRUE, 10);

    gtk_widget_show_all(dialog);

    // Exibir o cliente atual
    exibir_cliente_atual();
}


void concluir_entrega_ida()
{
    if (cliente_atual == NULL) {
        return;
    }

    // Remover cliente da lista de entregas
    transportadora->rota_on->tentativa1 = cliente_atual->prox;
    free(cliente_atual);
    cliente_atual = transportadora->rota_on->tentativa1;

    printf("Entrega realizada com sucesso.\n");
}

void tentar_novamente_entrega_ida()
{
    if (cliente_atual == NULL) {
        return;
    }

    // Adicionar cliente na pilha de tentativas
    NaoEntregue *nova_tentativa = (NaoEntregue *)malloc(sizeof(NaoEntregue));
    nova_tentativa->cliente = cliente_atual;
    nova_tentativa->prox = transportadora->rota_on->tentativa2;
    transportadora->rota_on->tentativa2 = nova_tentativa;

    // Remover cliente da lista de entregas
    transportadora->rota_on->tentativa1 = cliente_atual->prox;
    cliente_atual->prox = NULL;
    cliente_atual = transportadora->rota_on->tentativa1;

    printf("Entrega não realizada. Tentativa adicionada para segunda tentativa.\n");
}

void on_sim_button_clicked_volta(GtkButton *button, gpointer user_data)
{
    GtkWidget *dialog = GTK_WIDGET(user_data);
    gtk_widget_destroy(dialog);
    score += 3;
    transportadora->score = score;

    on_exibir_produtos_clicked(NULL, NULL);

    concluir_entrega_volta();
}

// Função chamada quando o botão Não é clicado na volta
void on_nao_button_clicked_volta(GtkButton *button, gpointer user_data)
{
    GtkWidget *dialog = GTK_WIDGET(user_data);
    gtk_widget_destroy(dialog);
    score -= 1;
    transportadora->score = score;
    adicionar_lista_devolucao();
}

void on_entrega_volta_clicked(GtkButton *button, gpointer user_data)
{
    if (transportadora == NULL || transportadora->rota_on == NULL || transportadora->rota_on->tentativa2 == NULL)
    {
        printf("Erro: Pilha de tentativas vazia ou transportadora não inicializada.\n");
        return;
    }

    // Retirar o último cliente da pilha de tentativas
    NaoEntregue *tentativa = transportadora->rota_on->tentativa2;
    transportadora->rota_on->tentativa2 = tentativa->prox;
    cliente_atual = tentativa->cliente;
    free(tentativa);

    // Criar uma janela de diálogo para exibir as informações do cliente e perguntar sobre a entrega
    dialog = gtk_dialog_new_with_buttons("Entrega na Volta",
                                         NULL,
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 400);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Caixa vertical para organizar os labels
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_box_set_homogeneous(GTK_BOX(vbox), FALSE);
    gtk_container_add(GTK_CONTAINER(content_area), vbox);

    // Adicionar cor de fundo
    set_background_color(vbox, "#F0DBC0");

    // Criar e adicionar um título ao diálogo
    GtkWidget *titulo_label = gtk_label_new("Informações do Cliente");
    gtk_widget_set_name(titulo_label, "titulo-label");
    gtk_box_pack_start(GTK_BOX(vbox), titulo_label, FALSE, FALSE, 10);

    // Labels para exibir as informações do cliente
    nome_label = gtk_label_new("");
    cpf_label = gtk_label_new("");
    estado_label = gtk_label_new("");
    cidade_label = gtk_label_new("");
    rua_label = gtk_label_new("");
    numero_label = gtk_label_new("");
    telefone_label = gtk_label_new("");
    email_label = gtk_label_new("");

    // Adicionar labels na vbox
    gtk_box_pack_start(GTK_BOX(vbox), nome_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), cpf_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), estado_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), cidade_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), rua_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), numero_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), telefone_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), email_label, FALSE, FALSE, 5);

    // Pergunta de confirmação
    GtkWidget *pergunta_label = gtk_label_new("A entrega foi realizada com sucesso?");
    gtk_widget_set_name(pergunta_label, "pergunta-label");
    gtk_box_pack_start(GTK_BOX(vbox), pergunta_label, FALSE, FALSE, 20);

    // Caixa horizontal para os botões
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(button_box), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, FALSE, FALSE, 10);

    // Botões Sim e Não
    GtkWidget *sim_button = gtk_button_new_with_label("Sim");
    gtk_widget_set_name(sim_button, "sim-button");
    g_signal_connect(sim_button, "clicked", G_CALLBACK(on_sim_button_clicked_volta), dialog);
    gtk_box_pack_start(GTK_BOX(button_box), sim_button, TRUE, TRUE, 10);

    GtkWidget *nao_button = gtk_button_new_with_label("Não");
    gtk_widget_set_name(nao_button, "nao-button");
    g_signal_connect(nao_button, "clicked", G_CALLBACK(on_nao_button_clicked_volta), dialog);
    gtk_box_pack_start(GTK_BOX(button_box), nao_button, TRUE, TRUE, 10);

    gtk_widget_show_all(dialog);

    // Exibir o cliente atual
    exibir_cliente_atual();
}




void concluir_entrega_volta()
{
    if (cliente_atual == NULL) {
        return;
    }

    // Liberar memória do cliente atual
    free(cliente_atual);
    cliente_atual = NULL;

    printf("Entrega na volta realizada com sucesso.\n");
}

void adicionar_lista_devolucao()
{
    if (cliente_atual == NULL) {
        return;
    }

    // Adicionar cliente à lista de devolução
    ListaDevolucao *novo_item = (ListaDevolucao *)malloc(sizeof(ListaDevolucao));
    if (novo_item == NULL) {
        fprintf(stderr, "Erro ao alocar memória para ListaDevolucao.\n");
        return;
    }
    novo_item->cliente = cliente_atual;
    novo_item->prox = NULL;

    if (fila->fila_devolucao->fim == NULL) {
        fila->fila_devolucao->ini = novo_item;
        fila->fila_devolucao->fim = novo_item;
    } else {
        fila->fila_devolucao->fim->prox = novo_item;
        fila->fila_devolucao->fim = novo_item;
    }

    cliente_atual = NULL;
    printf("Cliente adicionado à lista de devolução.\n");
}

// void exibir_cliente_atual()
// {
//     if (cliente_atual == NULL)
//         return;

//     // Atualiza os labels com as informações do cliente
//     gtk_label_set_text(GTK_LABEL(nome_label), cliente_atual->nome);
//     gtk_label_set_text(GTK_LABEL(cpf_label), cliente_atual->cpf);
//     gtk_label_set_text(GTK_LABEL(estado_label), cliente_atual->estado);
//     gtk_label_set_text(GTK_LABEL(cidade_label), cliente_atual->cidade);
//     gtk_label_set_text(GTK_LABEL(rua_label), cliente_atual->rua);
//     char numero_str[10];
//     sprintf(numero_str, "%d", cliente_atual->numero);
//     gtk_label_set_text(GTK_LABEL(numero_label), numero_str);
//     gtk_label_set_text(GTK_LABEL(telefone_label), cliente_atual->telefone);
//     gtk_label_set_text(GTK_LABEL(email_label), cliente_atual->email);
// }

void on_exibir_produtos_clicked(GtkButton *button, gpointer user_data)
{
    (void)button;
    (void)user_data;

    GtkWidget *cpf_dialog, *cpf_entry;
    GtkWidget *content_area;
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;

    // Cria uma janela de diálogo para solicitar o CPF
    cpf_dialog = gtk_dialog_new_with_buttons("Digite o CPF do Cliente",
                                             NULL,
                                             flags,
                                             ("OK"),
                                             GTK_RESPONSE_OK,
                                             ("Cancelar"),
                                             GTK_RESPONSE_CANCEL,
                                             NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(cpf_dialog));
    cpf_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(cpf_entry), 14);
    gtk_container_add(GTK_CONTAINER(content_area), cpf_entry);

    gtk_widget_show_all(cpf_dialog);

    // Lida com a resposta do usuário
    gint response = gtk_dialog_run(GTK_DIALOG(cpf_dialog));
    if (response == GTK_RESPONSE_OK)
    {
        const gchar *cpf = gtk_entry_get_text(GTK_ENTRY(cpf_entry));
        Cliente *cliente = lista_clientes;

        // Busca o cliente pelo CPF
        while (cliente != NULL)
        {
            if (strcmp(cliente->cpf, cpf) == 0)
            {
                break;
            }
            cliente = cliente->prox;
        }

        if (cliente != NULL)
        {
            // Criar uma janela de diálogo para exibir os produtos do cliente
            GtkWidget *dialog = gtk_dialog_new_with_buttons("Produtos do Cliente",
                                                 NULL,
                                                 GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                 NULL);

            gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 400);

            GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

            // Caixa vertical para organizar os produtos
            GtkWidget *list_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_box_pack_start(GTK_BOX(content_area), list_vbox, TRUE, TRUE, 0);

            const gchar *css = "* { background-color: #F0DBC0; color: #333; padding: 10px; border: 1px solid #ccc; }";
            apply_css(list_vbox, css);

            Produto *produto_atual = cliente->produtos;
            if (produto_atual == NULL) {
                // Exibir mensagem de erro caso o cliente não tenha produtos
                GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(cpf_dialog),
                                                                 GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                 GTK_MESSAGE_ERROR,
                                                                 GTK_BUTTONS_CLOSE,
                                                                 "O cliente não possui produtos cadastrados.");
                gtk_dialog_run(GTK_DIALOG(error_dialog));
                gtk_widget_destroy(error_dialog);
            } else {
                while (produto_atual != NULL)
                {
                    GtkWidget *prod_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
                    gtk_box_pack_start(GTK_BOX(list_vbox), prod_box, FALSE, FALSE, 0);

                    const gchar *prod_box_css = "box { border-bottom: 1px solid #aaa; }";
                    apply_css(prod_box, prod_box_css);

                    char id_str[10];
                    sprintf(id_str, "%d", produto_atual->id);
                    GtkWidget *label_id = gtk_label_new(id_str);
                    gtk_widget_set_hexpand(label_id, TRUE);
                    gtk_box_pack_start(GTK_BOX(prod_box), label_id, FALSE, FALSE, 5);

                    GtkWidget *label_nome = gtk_label_new(produto_atual->nome);
                    gtk_widget_set_hexpand(label_nome, TRUE);
                    gtk_box_pack_start(GTK_BOX(prod_box), label_nome, FALSE, FALSE, 5);

                    GtkWidget *delete_button = gtk_button_new_with_label("Del");
                    gtk_widget_set_name(delete_button, "delete-button");
                    gtk_box_pack_start(GTK_BOX(prod_box), delete_button, FALSE, FALSE, 5);

                    // Você precisará implementar a função on_delete_produto_button_clicked
                    g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_produto_button_clicked), produto_atual);

                    const gchar *buttons_css =
                        "#delete-button { "
                        "background-color: red; "
                        "border-radius: 8px; "
                        "padding: 5px 10px; "
                        "transition: background-color 0.3s ease; "
                        "}"
                        "#delete-button:hover { "
                        "background-color: darkred; "
                        "}";

                    apply_css(delete_button, buttons_css);

                    produto_atual = produto_atual->prox;
                }

                gtk_widget_show_all(dialog);
            }
        }
        else
        {
            // Exibir mensagem de erro caso o cliente não seja encontrado
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(cpf_dialog),
                                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                                             GTK_MESSAGE_ERROR,
                                                             GTK_BUTTONS_CLOSE,
                                                             "Cliente não encontrado.");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
    }

    gtk_widget_destroy(cpf_dialog);
}

void on_delete_produto_button_clicked(GtkButton *button, gpointer user_data) {
    Produto *produto = (Produto *)user_data;

    // Iterar sobre a lista de clientes
    Cliente *cliente = lista_clientes;
    while (cliente != NULL) {
        Produto *anterior = NULL;
        Produto *atual = cliente->produtos;

        // Iterar sobre a lista de produtos do cliente
        while (atual != NULL) {
            if (atual == produto) {
                // Remover o produto da lista
                if (anterior == NULL) {
                    // Produto é o primeiro da lista
                    cliente->produtos = atual->prox;
                } else {
                    // Produto está no meio ou no final da lista
                    anterior->prox = atual->prox;
                }
                // Liberar a memória do produto removido
                free(atual);

                // Destruir o widget do botão
                gtk_widget_destroy(GTK_WIDGET(gtk_widget_get_parent(GTK_WIDGET(button))));
                printf("Produto removido.\n");
                return;
            }
            anterior = atual;
            atual = atual->prox;
        }
        cliente = cliente->prox;
    }
}