#ifndef GTK_HANDLER_FUNCTIONS_H_INCLUDED
#define GTK_HANDLER_FUNCTIONS_H_INCLUDED
#include <gtk/gtk.h>
#include <stdbool.h>


typedef struct {
    char street[100]; // AS
    int number; // AN
    char bairro[100]; // AB
    char city[100]; // AY
    char state[100]; // AE
    char cep[100]; // AC
} Address;

typedef struct {
    char date_of_diagnostic[50]; // DD
    char date_of_birth[50]; // DB
    int has_comorbidade; // HC
    char phone_number[100]; // PN
    char comobirdade[150]; // CM
    char email[100]; // EM
    Address address;
    char name[100]; // NO
    char cpf[100]; // CP
    int age; // AG
} Patient;

void display_message(char [100], char [100], char [100], GtkBuilder *);
void read_file_and_display_at_uilist(GtkListStore *);
void write_in_file(char *, Patient *, GtkBuilder *);
int is_risk_group(int);
int has_comorbidade(char [150]);
int calculate_age(int);

#endif // GTK_HANDLER_FUNCTIONS_H_INCLUDED
