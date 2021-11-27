#ifndef GTK_HANDLER_FUNCTIONS_H_INCLUDED
#define GTK_HANDLER_FUNCTIONS_H_INCLUDED
#include <gtk/gtk.h>


typedef struct {
    char name[100];
    char cpf[100];
    char date_of_birth[50];
    char email[100];
    char date_of_diagnostic[50];
} Patient;

void display_message(char [100], char [100], char [100], GtkBuilder *);
void white_in_file(char *, Patient *);
void read_file_and_display_at_uilist(GtkListStore *);

#endif // GTK_HANDLER_FUNCTIONS_H_INCLUDED
