#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 1910
#define WINDOW_HEIGHT 1070

void create_main_window(GtkApplication* app, gpointer user_data)
{
    GtkWidget* window;
    GtkWidget* vbox;
    GtkWidget* grid;
    GtkWidget* button_clientes;
    GtkWidget* button_entregas;
    GtkWidget* button_devolucoes;
    GtkWidget* button_pontuacao;
    GtkWidget* button_sair;
    GtkWidget* logo_image;
    GtkWidget* logo_box;
    GtkCssProvider *cssProvider;

    cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "/mnt/c/Users/danie/OneDrive/Documentos/UFPI-2024.1/PROJETOS/Transportadora/src/ui/style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Transportadora");
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // Espaçamento zero
    gtk_container_add(GTK_CONTAINER(window), vbox);

    grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10); 
    gtk_grid_set_row_spacing(GTK_GRID(grid), 0); // Remover espaçamento entre linhas

    // Criar botões
    button_clientes = gtk_button_new_with_label("Gerenciar Clientes");
    gtk_widget_set_hexpand(button_clientes, TRUE);    
    gtk_grid_attach(GTK_GRID(grid), button_clientes, 0, 0, 1, 1);

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
    gtk_box_pack_start(GTK_BOX(vbox), logo_box, FALSE, FALSE, 0);
    gtk_widget_set_halign(logo_box, GTK_ALIGN_CENTER);

    gtk_container_add(GTK_CONTAINER(logo_box), logo_image);
    gtk_widget_show_all(logo_box);

    gtk_widget_show_all(window);
}
