#include "gtk_handler_functions.h"
#include <gtk/gtk.h>
#include <stdlib.h>


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

void white_in_file(char *, Patient *) {

}

void read_file_and_display_at_uilist(GtkListStore *) {

}
