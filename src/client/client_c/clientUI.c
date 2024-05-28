#include <gtk/gtk.h>

// Declaraciones de las funciones
void on_login_button_clicked(GtkButton *button, gpointer user_data);
void on_register_button_clicked(GtkButton *button, gpointer user_data);
void on_create_group_button_clicked(GtkButton *button, gpointer user_data);

// Declare a global variable to store the container widget for the cards
GtkWidget *card_container;
// Variables globales para las ventanas y widgets
GtkWidget *login_window;
GtkWidget *create_group_window;
GtkBuilder *builder;

void generate_cards(int *array, int size) {
    // Clear any existing cards
    gtk_container_foreach(GTK_CONTAINER(card_container), (GtkCallback)gtk_widget_destroy, NULL);

    // Create a new card for each element in the array
    for (int i = 0; i < size; i++) {
        GtkWidget *card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        GtkWidget *label = gtk_label_new(NULL);
        char card_text[10];
        sprintf(card_text, "%d", array[i]);
        gtk_label_set_text(GTK_LABEL(label), card_text);
        gtk_box_pack_start(GTK_BOX(card), label, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(card_container), card, FALSE, FALSE, 0);
    }
}


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Cargar la interfaz desde archivo glade
    builder = gtk_builder_new_from_file("interface.glade");

    // Conectar señales
    gtk_builder_connect_signals(builder, NULL);

    // Obtener la ventana de inicio de sesión/registro
    login_window = GTK_WIDGET(gtk_builder_get_object(builder, "login_window"));
    gtk_widget_show_all(login_window);

    // Inicializar la ventana de creación de grupo
    create_group_window = GTK_WIDGET(gtk_builder_get_object(builder, "create_group_window"));

    // Get the card container widget
    card_container = GTK_WIDGET(gtk_builder_get_object(builder, "card_container"));

    gtk_main();
    return 0;
}

// Función manejadora del botón de Login
void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    GtkEntry *username_entry = GTK_ENTRY(gtk_builder_get_object(builder, "username_entry"));
    GtkEntry *password_entry = GTK_ENTRY(gtk_builder_get_object(builder, "password_entry"));
    const gchar *username = gtk_entry_get_text(username_entry);
    const gchar *password = gtk_entry_get_text(password_entry);

    g_print("Login presionado\n");
    g_print("Usuario: %s\n", username);
    g_print("Contraseña: %s\n", password);

    gtk_widget_hide(login_window);
    gtk_widget_show_all(create_group_window);
}

// Función manejadora del botón de Register
void on_register_button_clicked(GtkButton *button, gpointer user_data) {
    GtkEntry *username_entry = GTK_ENTRY(gtk_builder_get_object(builder, "username_entry"));
    GtkEntry *password_entry = GTK_ENTRY(gtk_builder_get_object(builder, "password_entry"));
    const gchar *username = gtk_entry_get_text(username_entry);
    const gchar *password = gtk_entry_get_text(password_entry);

    g_print("Register presionado\n");
    g_print("Usuario: %s\n", username);
    g_print("Contraseña: %s\n", password);

    gtk_widget_hide(login_window);
    gtk_widget_show_all(create_group_window);
}

// Función manejadora del botón de Crear Grupo
void on_create_group_button_clicked(GtkButton *button, gpointer user_data) {
    GtkEntry *groupname_entry = GTK_ENTRY(gtk_builder_get_object(builder, "groupname_entry"));
    const gchar *group_name = gtk_entry_get_text(groupname_entry);

    g_print("Crear Grupo presionado\n");
    g_print("Nombre del Grupo: %s\n", group_name);

    // Example array
    int example_array[] = {1, 2, 3};
    int array_size = sizeof(example_array) / sizeof(int);

    // Generate cards based on the array
    generate_cards(example_array, array_size);
}
