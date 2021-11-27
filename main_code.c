#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gtk_handler_functions.h"

GtkBuilder *builder;
GtkWidget *window;
GtkStack *stack;
GtkNotebook *notebook;
GtkListStore *mod_covid_list;



// window signal(s)
void on_main_window_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit(); // Comando para fechar a aplicação
}

void on_login_button_clicked(GtkWidget *widget, gpointer data) {
    char *username = gtk_entry_get_text(gtk_builder_get_object(builder, "login_username"));
    char *password = gtk_entry_get_text(gtk_builder_get_object(builder, "login_password"));

    bool *remember = gtk_toggle_button_get_active(gtk_builder_get_object(builder, "remember_me"));
    login(username, password, remember);
}

void on_button_covid_pacients_clicked(GtkWidget *widget, gpointer data) {
    gtk_main_quit(); // Comando para fechar a aplicação
}

void on_button_risk_group_clicked(GtkWidget *widget, gpointer data) {
    gtk_main_quit(); // Comando para fechar a aplicação
}

void on_button_add_tools_clicked(GtkWidget *widget, gpointer data) {
    gtk_main_quit(); // Comando para fechar a aplicação
}

void on_button_quit_tools_clicked(GtkWidget *widget, gpointer data) {
    gtk_main_quit(); // Comando para fechar a aplicação
}

// methods
void login(char *username, char *password, bool *remember_me) {
    if(strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
        display_message("Bem vindo", "Usuario logado com sucesso!", "emblem-default", builder);
        g_object_set(window, "icon_name", "avatar-default", NULL);
        gtk_stack_set_visible_child_name(stack, "view_home");
    } else {
        display_message("Aviso", "Email ou senha incorretos!", "dialog-error", builder);
    }
}


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv); // Inicia o GTK

    builder = gtk_builder_new_from_file("smh_glade_ui.glade"); // Carrega o builder.

    // Manipuladores(callback_functions) dos "sinais" configurados no glade
    gtk_builder_add_callback_symbols(
        builder,
        "on_main_window_destroy",           G_CALLBACK(on_main_window_destroy),
        "on_login_button_clicked",          G_CALLBACK(on_login_button_clicked),
        "on_button_covid_pacients_clicked", G_CALLBACK(on_button_covid_pacients_clicked),
        "on_button_risk_group_clicked",     G_CALLBACK(on_button_risk_group_clicked),
        "on_button_add_tools_clicked",      G_CALLBACK(on_button_add_tools_clicked),
        "on_button_quit_tools_clicked",     G_CALLBACK(on_button_quit_tools_clicked),
        NULL
    );

    gtk_builder_connect_signals(builder, NULL);

    // Carregando o stack, o window e o modelo da lista
    stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    mod_covid_list = GTK_LIST_STORE(gtk_builder_get_object(builder, "covid_patients_list"));

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
