#include "gtk_handler_functions.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>


void display_message(char text[100], char secondary_text[100], char icon_name[100], GtkBuilder *builder) {
    GtkMessageDialog *msg_dialog = gtk_builder_get_object(builder, "message_dialog");

    g_object_set(msg_dialog, "text", text, NULL);
    g_object_set(msg_dialog, "secondary_text", secondary_text, NULL);
    g_object_set(msg_dialog, "icon_name", icon_name, NULL);

    // Mostra na tela
    gtk_widget_show_all(msg_dialog);
    // Mantém a janela rodando
    gtk_dialog_run(msg_dialog);
    // Esconde a janelinha
    gtk_widget_hide(msg_dialog);
}

char* concatenar(char* s1, char* s2) {
    int i, j;

    printf("Tamanho, %s %s", s1, s2);
    for (i = 0; s1[i] != '\0'; ++i);

    printf("Concatenando");
    for (j = 0; s2[j] != '\0'; ++j, ++i) {
        s1[i] = s2[j];
    }
    s1[i] = '\0';
    printf("String1: %s\n String2: %s", s1, s2);
    return &s1;
}

void write_in_file(char *filename, Patient *patient, GtkBuilder *builder) {
    FILE *file;
    int result;

    file = fopen(filename, "a");

    if(file == NULL) {
        display_message("Aviso", "Problemas na abertura do arquivo!", "dialog-error", builder);
    } else {
        result = fprintf(file,
            "NO_%s;CP_%s;EM_%s;PN_%s;DB_%s;AG_%d;DD_%s;CM_{%s};LG_{%s};AN_%d;AB_%s;AY_%s;AE_%s;AC_%s;\n",
            patient->name, patient->cpf, patient->email, patient->phone_number, patient->date_of_birth, patient->age,
            patient->date_of_diagnostic, patient->comobirdade, patient->address.logradouro, patient->address.number,
            patient->address.bairro, patient->address.city, patient->address.state, patient->address.cep
        );

        if(result == EOF)
            display_message("Erro", "Erro no registro do paciente", "dialog-error", builder);

        display_message("Sucesso", "Paciente registrado com sucesso!!!", "emblem-default", builder);
    }
    fclose(file);
}

int read_file_and_display_at_uilist(char *filename, GtkListStore *mod_list, GtkTreeIter *iter) {
    FILE *file;
    int result;

    file = fopen(filename, "rt");

    if(file == NULL) {
        // Problemas na abertura do arquivo
        return -1;
    } else {
        char linha[200];

        // Percorrer cada linha do arquivo
        // Até que chegue no final
        while(!feof(file)) {
            result = fgets(linha, 200, file);

            if(result) { // Foi possível ler a linha
                // Tratar a string(linha)
                char *splited_linha;
                Patient r_patient;

                // Quebra a string em tokens
                splited_linha = strtok(linha, ";\n");

                // Maneira de percorrer o splited_linha
                while(splited_linha) { // enquanto for != NULL
                    /* char split_cpy[50];
                    printf("Token: %s\n", splited_linha);
                    strcpy(split_cpy, splited_linha);
                    printf("Copia Token: %s\n", split_cpy);
                    char *att_id = strtok(split_cpy, "_");
                    printf("ID: %s\n", att_id);
                    while(att_id) {
                        att_id = strtok(NULL, "_");
                    } */
                    printf("Token: %s\n", splited_linha);

                    splited_linha = strtok(NULL, ";\n");
                }
                // Acaba a linha
            }
        }
    }

    fclose(file);

    // Rodou sem erros
    return 0;
}

int has_comorbidade(char comorbidade[150]) {
    if(strcmp(comorbidade, "") == 0) {
        return 0;
    } else {
        return 1;
    }
}

int is_risk_group(int pat_age, char comorbidade[150]) {
    // Se possuir comorbidade tb vai para o grupo
    if(pat_age > 65 || has_comorbidade(comorbidade)) {
        return 1;
    } else {
        return 0;
    }
}

int calculate_age(int birth_year) {
    time_t myTime;
    myTime = time(NULL);
    struct tm *tm = localtime(&myTime);
    int act_year = tm->tm_year;

    return (act_year + 1900) - birth_year;
}
