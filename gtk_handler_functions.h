#ifndef GTK_HANDLER_FUNCTIONS_H_INCLUDED
#define GTK_HANDLER_FUNCTIONS_H_INCLUDED
#include <gtk/gtk.h>
#include <stdbool.h>


typedef struct {
    // char street[100]; // AS Trocar para logradouro
    char logradouro[100]; // LG
    int number; // AN
    char bairro[100]; // AB
    char city[100]; // AY
    char state[100]; // AE
    char cep[20]; // AC
} Address;

typedef struct {
    char date_of_diagnostic[50]; // DD
    char date_of_birth[50]; // DB
    // int has_comorbidade; // HC
    char phone_number[50]; // PN
    char comobirdade[150]; // CM
    char email[80]; // EM
    Address address;
    char name[100]; // NO
    char cpf[15]; // CP
    int age; // AG
} Patient;

void display_message(char [100], char [100], char [100], GtkBuilder *);
int read_file_and_display_at_uilist(char *, GtkListStore *, GtkTreeIter *);
void write_in_file(char *, Patient *, GtkBuilder *);
char* concatenar(char *, char *);
int is_risk_group(int, char [150]);
int has_comorbidade(char [150]);
int calculate_age(int);

#endif // GTK_HANDLER_FUNCTIONS_H_INCLUDED
