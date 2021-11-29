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

char* concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 para o terminador null

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 para copiar o terminador null
    return result;
}

char* substring(const char *s1, int init) {
    int i = init, c;

    const size_t len = strlen(s1) - 3; // -3 para tirar a parte dos identificadores
    // printf("Tamanho: %d\n", len);
    char *res = malloc(len + 1);

    for(c = 0; c < len; c++, i++) {
        res[c] = s1[i];
    }

    res[c] = '\0';
    return res;
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
                    char split_cpy[50];
                    char att_id[3] = {splited_linha[0], splited_linha[1], '\0'};
                    char *att_data = substring(splited_linha, 3);

                    /* printf("Token: %s\n", splited_linha);
                    printf("ID: %s\n", att_id);
                    printf("Data: %s\n", att_data); */

                    if(strcmp(att_id, "NO") == 0) {
                        // name
                        strcpy(r_patient.name, att_data);
                        // printf("Name: %s\n", r_patient.name);
                    } else if(strcmp(att_id, "CP") == 0) {
                        // CPF
                        strcpy(r_patient.cpf, att_data);
                        // printf("CPF: %s\n", r_patient.cpf);
                    } else if(strcmp(att_id, "DB") == 0) {
                        // Date_of_birth
                        strcpy(r_patient.date_of_birth, att_data);
                        // printf("date_b: %s\n", r_patient.date_of_birth);
                    } else if(strcmp(att_id, "EM") == 0) {
                        // Email
                        strcpy(r_patient.email, att_data);
                        // printf("Email: %s\n", r_patient.email);
                    } else if(strcmp(att_id, "DD") == 0) {
                        // Date_of_diagnostic
                        strcpy(r_patient.date_of_diagnostic, att_data);
                        // printf("DD: %s\n", r_patient.date_of_diagnostic);
                    } else if(strcmp(att_id, "CM") == 0) {
                        // Comorbidade
                        strcpy(r_patient.comobirdade, att_data);
                        // printf("Comorbidade: %s\n", r_patient.comobirdade);
                    } else if(strcmp(att_id, "AG") == 0) {
                        // Idade
                        r_patient.age = atoi(att_data);
                        // printf("Name: %d\n", r_patient.age);
                    } else {
                        // printf("Default\n");
                    }
                    splited_linha = strtok(NULL, ";\n");
                }
                bool hcm = FALSE;
                if(has_comorbidade(r_patient.comobirdade) == 1) {
                    hcm = TRUE;
                }

                // Acaba a linha
                // Somente adiciona uma linha na nossa lista
                gtk_list_store_append(mod_list, &iter);
                // Atribui os dados nas colunas respectivas confliguradas no glade anteriormente
                gtk_list_store_set(mod_list, &iter,
                    0, r_patient.cpf,
                    1, r_patient.name,
                    2, r_patient.date_of_birth,
                    3, r_patient.email,
                    4, r_patient.date_of_diagnostic,
                    5, hcm,
                    6, r_patient.age,
                    -1
                );
            }
        }
    }

    fclose(file);

    // Rodou sem erros
    return 0;
}

int has_comorbidade(char comorbidade[150]) {
    if(strcmp(comorbidade, "") == 0 || strcmp(comorbidade, "{}") == 0) {
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
