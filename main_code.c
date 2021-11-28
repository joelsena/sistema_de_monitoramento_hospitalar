#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gtk_handler_functions.h"

GtkBuilder *builder;
GtkWidget *window;
GtkStack *stack;
GtkNotebook *notebook;
GtkListStore *mod_covid_list;
GtkListStore *mod_risk_group_list;



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
    GtkTreeIter *iter;

    // Limpa p list storage para não repetir na UI
    gtk_list_store_clear(mod_covid_list);

    int res = read_file_and_display_at_uilist("lista_de_pacientes_com_covid.txt", mod_covid_list, iter);

    if(res == -1) {
        display_message("Aviso", "Problemas na leitura do arquivo!", "dialog-error", builder);
    }
}

void on_button_risk_group_clicked(GtkWidget *widget, gpointer data) {
    gtk_main_quit(); // Comando para fechar a aplicação
}

void on_button_add_tools_clicked(GtkWidget *widget, gpointer data) {
    g_object_set(window, "title", "Adicionar paciente", NULL);
    gtk_stack_set_visible_child_name(stack, "view_add_patient");
}

void on_button_quit_tools_clicked(GtkWidget *widget, gpointer data) {
    g_object_set(window, "title", "Autenticacao", NULL);
    g_object_set(window, "icon_name", "changes-prevent", NULL);
    gtk_stack_set_visible_child_name(stack, "view_login");
}

void on_add_patient_back_button_clicked(GtkWidget *widget, gpointer data) {
    g_object_set(window, "title", "Pagina Inicial", NULL);
    gtk_stack_set_visible_child_name(stack, "view_home");
}

void on_add_patient_save_button_clicked(GtkWidget *widget, gpointer data) {
    char *pat_name = gtk_entry_get_text(gtk_builder_get_object(builder, "patient_name"));
    char *pat_cpf = gtk_entry_get_text(gtk_builder_get_object(builder, "patient_cpf"));
    char *pat_phone_number = gtk_entry_get_text(gtk_builder_get_object(builder, "patient_phone_number"));
    char *pat_date_birth = gtk_entry_get_text(gtk_builder_get_object(builder, "patient_date_birth"));
    char *pat_email = gtk_entry_get_text(gtk_builder_get_object(builder, "patient_email"));
    char *pat_date_diagnostic = gtk_entry_get_text(gtk_builder_get_object(builder, "patient_date_diagnostic"));
    char *pat_comorbidade = gtk_entry_get_text(gtk_builder_get_object(builder, "patient_comorbidade"));
    char *pat_address_cep = gtk_entry_get_text(gtk_builder_get_object(builder, "patient_address_cep"));
    char *pat_address_bairro  = gtk_entry_get_text(gtk_builder_get_object(builder, "patient_address_bairro"));
    char *pat_address_city  = gtk_entry_get_text(gtk_builder_get_object(builder, "patient_address_city"));
    char *pat_address_state  = gtk_entry_get_text(gtk_builder_get_object(builder, "patient_address_state"));
    char *pat_address_logradouro  = gtk_entry_get_text(gtk_builder_get_object(builder, "patient_address_logradouro"));
    int *pat_address_number  = gtk_entry_get_text(gtk_builder_get_object(builder, "patient_address_number"));

    Patient patient;

    if(strcmp(pat_cpf, "") == 0) {
        display_message("Aviso!!!", "Campo 'cpf' obrigatorio!!", "dialog-error", builder);
    } else {
        strcpy(patient.name, pat_name);
        strcpy(patient.cpf, pat_cpf);
        strcpy(patient.phone_number, pat_phone_number);
        strcpy(patient.date_of_birth, pat_date_birth);
        strcpy(patient.email, pat_email);
        strcpy(patient.date_of_diagnostic, pat_date_diagnostic);
        strcpy(patient.comobirdade, pat_comorbidade);
        strcpy(patient.address.cep, pat_address_cep);
        strcpy(patient.address.bairro, pat_address_bairro);
        strcpy(patient.address.city, pat_address_city);
        strcpy(patient.address.state, pat_address_state);
        strcpy(patient.address.logradouro, pat_address_logradouro);
        patient.address.number = atoi(pat_address_number);

        int pat_birth_year;
        int pat_date_birth_cpy[50];

        char *split_date;

        // Fazer uma copia da idade vindo do entry
        strcpy(pat_date_birth_cpy, pat_date_birth);
        split_date = strtok(pat_date_birth_cpy, "/ ");
        while(split_date) {
            if(strlen(split_date) == 4) {
                // String com o ano, portanto tamanho 4
                pat_birth_year = atoi(split_date);
            }

            split_date = strtok(NULL, "/ ");
        }

        patient.age = calculate_age(pat_birth_year);
        // patient.has_comorbidade = has_comorbidade(patient.comobirdade);
        // Verifica se pertence ao grupo de risco
        if(is_risk_group(patient.age, patient.comobirdade) == 1) {
            // Grupo de risco
            write_in_file("lista_de_pacientes_com_covid_grupo_de_risco.txt", &patient, builder);
        } else {
            // Fora do grupo de risco
            write_in_file("lista_de_pacientes_com_covid.txt", &patient, builder);
        }
        // TO-DO: Salvar os dados no arquivo
    }
}

// methods
void login(char *username, char *password, bool *remember_me) {
    if(strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
        display_message("Bem vindo", "Usuario logado com sucesso!", "emblem-default", builder);
        g_object_set(window, "icon_name", "avatar-default", NULL);
        g_object_set(window, "title", "Pagina Inicial", NULL);
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
        "on_main_window_destroy",                 G_CALLBACK(on_main_window_destroy),
        "on_login_button_clicked",                G_CALLBACK(on_login_button_clicked),
        "on_button_covid_pacients_clicked",       G_CALLBACK(on_button_covid_pacients_clicked),
        "on_button_risk_group_clicked",           G_CALLBACK(on_button_risk_group_clicked),
        "on_button_add_tools_clicked",            G_CALLBACK(on_button_add_tools_clicked),
        "on_button_quit_tools_clicked",           G_CALLBACK(on_button_quit_tools_clicked),
        "on_add_patient_back_button_clicked",     G_CALLBACK(on_add_patient_back_button_clicked),
        "on_add_patient_save_button_clicked",     G_CALLBACK(on_add_patient_save_button_clicked),
        NULL
    );

    gtk_builder_connect_signals(builder, NULL);

    // Carregando o stack, o window e o modelo da lista
    stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    mod_covid_list = GTK_LIST_STORE(gtk_builder_get_object(builder, "covid_patients_list"));
    mod_risk_group_list = GTK_LIST_STORE(gtk_builder_get_object(builder, "risk_group_list"));

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
