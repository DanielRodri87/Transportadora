#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 1910
#define WINDOW_HEIGHT 1070
#define CLIENT_WINDOW_WIDTH 800
#define CLIENT_WINDOW_HEIGHT 800

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

void on_gerenciar_clientes_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *client_window;
    GtkWidget *client_vbox;
    GtkWidget *client_grid;
    GtkWidget *button_cadastrar;
    GtkWidget *button_buscar;
    GtkWidget *button_remover;
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

    button_buscar = gtk_button_new_with_label("Buscar Cliente");
    gtk_widget_set_hexpand(button_buscar, TRUE);
    gtk_grid_attach(GTK_GRID(client_grid), button_buscar, 1, 0, 1, 1);

    button_remover = gtk_button_new_with_label("Remover Cliente");
    gtk_widget_set_hexpand(button_remover, TRUE);
    gtk_grid_attach(GTK_GRID(client_grid), button_remover, 2, 0, 1, 1);

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
    gtk_css_provider_load_from_path(cssProvider, "/mnt/c/Users/danie/OneDrive/Documentos/UFPI-2024.1/PROJETOS/Transportadora/src/ui/style.css", NULL);
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

    logo_image = gtk_image_new_from_file("/mnt/c/Users/danie/OneDrive/Documentos/UFPI-2024.1/PROJETOS/Transportadora/src/ui/logo_transp.png");
    gtk_image_set_pixel_size(GTK_IMAGE(logo_image), 250);

    logo_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(logo_box), logo_image, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), logo_box, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
}

