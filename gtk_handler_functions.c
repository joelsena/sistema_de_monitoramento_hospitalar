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

void write_in_file(char *filename, Patient *patient, GtkBuilder *builder) {
    FILE *file;
    int result;

    file = fopen(filename, "a");

    if(file == NULL) {
        display_message("Aviso", "Problemas na abertura do arquivo!", "dialog-error", builder);
    } else {
        result = fprintf(file,
            "NO_%s;CP_%s;EM_%s;PN_%s;DB_%s;AG_%d;DD_%s;HC_%d;CM_{%s};AS_%s;AN_%d;AB_%s;AY_%s;AE_%s;AC_%s;\n",
            patient->name, patient->cpf, patient->email, patient->phone_number, patient->date_of_birth, patient->age,
            patient->date_of_diagnostic, patient->has_comorbidade, patient->comobirdade, patient->address.street,
            patient->address.number, patient->address.bairro, patient->address.city, patient->address.state, patient->address.cep
        );

        if(result == EOF)
            display_message("Erro", "Erro no registro do paciente", "dialog-error", builder);

        display_message("Sucesso", "Paciente registrado com sucesso!!!", "emblem-default", builder);
    }
    fclose(file);
}

void read_file_and_display_at_uilist(GtkListStore *) {

}

int is_risk_group(int pat_age) {
    if(pat_age > 65) {
        return 1;
    } else {
        return 0;
    }
}

int has_comorbidade(char comorbidade[150]) {
    if(strcmp(comorbidade, "") == 0) {
        return 0;
    } else {
        return 1;
    }
}

int calculate_age(int birth_year) {
    time_t myTime;
    myTime = time(NULL);
    struct tm *tm = localtime(&myTime);
    int act_year = tm->tm_year;

    return (act_year + 1900) - birth_year;
}
