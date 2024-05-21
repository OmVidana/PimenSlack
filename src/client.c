#define _GNU_SOURCE
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "rsa.h"

#define BUFFER_SIZE 2048

int client_socket;
struct sockaddr_in server_addr;
GtkWidget *user_entry_register;
GtkWidget *password_entry_register;
GtkWidget *user_entry_group;
GtkWidget *groupname_entry_group;

static void on_register_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(user_entry_register));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry_register));

    char combined_credentials[BUFFER_SIZE * 2];
    snprintf(combined_credentials, sizeof(combined_credentials), "register\n%s\n%s", username, password);

    char *cyphered_credentials = encrypt(combined_credentials);
    send(client_socket, cyphered_credentials, strlen(cyphered_credentials), 0);

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("Server response: %s\n", buffer);
}

static void on_login_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(user_entry_register));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry_register));

    char combined_credentials[BUFFER_SIZE * 2];
    snprintf(combined_credentials, sizeof(combined_credentials), "autentificar\n%s\n%s", username, password);

    char *cyphered_credentials = encrypt(combined_credentials);
    send(client_socket, cyphered_credentials, strlen(cyphered_credentials), 0);

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("Server response: %s\n", buffer);
}

static void on_create_group_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(user_entry_group));
    const gchar *groupname = gtk_entry_get_text(GTK_ENTRY(groupname_entry_group));

    char *create_group_request;
    asprintf(&create_group_request, "creargrupo\n%s\n%s", username, groupname);
    char *cyphered_credentials = encrypt(create_group_request);
    send(client_socket, cyphered_credentials, strlen(cyphered_credentials), 0);

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("Server response: %s\n", buffer);
}

static void on_close_server_button_clicked(GtkWidget *widget, gpointer data) {
    const char *exit_message = "exit";
    send(client_socket, exit_message, strlen(exit_message), 0);
    close(client_socket);
    gtk_main_quit();
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *notebook;
    GtkWidget *register_grid;
    GtkWidget *create_group_grid;
    GtkWidget *register_tab_label;
    GtkWidget *create_group_tab_label;
    GtkWidget *register_tab;
    GtkWidget *create_group_tab;

    // Create a new window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "PimenSlack");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    // Create a notebook
    notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(window), notebook);

    // Create the Register tab
    register_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(register_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(register_grid), 5);

    GtkWidget *user_label = gtk_label_new("User:");
    GtkWidget *password_label = gtk_label_new("Password:");
    user_entry_register = gtk_entry_new();
    password_entry_register = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry_register), FALSE); // Hide password

    gtk_grid_attach(GTK_GRID(register_grid), user_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(register_grid), user_entry_register, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(register_grid), password_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(register_grid), password_entry_register, 1, 1, 1, 1);

    GtkWidget *register_button = gtk_button_new_with_label("Register");
    GtkWidget *login_button = gtk_button_new_with_label("Login");
    GtkWidget *close_server_button1 = gtk_button_new_with_label("Close Server");

    g_signal_connect(register_button, "clicked", G_CALLBACK(on_register_button_clicked), NULL);
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), NULL);
    g_signal_connect(close_server_button1, "clicked", G_CALLBACK(on_close_server_button_clicked), NULL);

    gtk_grid_attach(GTK_GRID(register_grid), register_button, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(register_grid), login_button, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(register_grid), close_server_button1, 0, 3, 2, 1);

    register_tab_label = gtk_label_new("Auth");
    register_tab = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(register_tab), register_grid);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), register_tab, register_tab_label);

    // Create the Crear Grupo tab
    create_group_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(create_group_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(create_group_grid), 5);

    GtkWidget *user_label2 = gtk_label_new("User:");
    GtkWidget *groupname_label = gtk_label_new("Group Name:");
    user_entry_group = gtk_entry_new();
    groupname_entry_group = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(create_group_grid), user_label2, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(create_group_grid), user_entry_group, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(create_group_grid), groupname_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(create_group_grid), groupname_entry_group, 1, 1, 1, 1);

    GtkWidget *create_group_button = gtk_button_new_with_label("Create Group");
    GtkWidget *close_server_button2 = gtk_button_new_with_label("Close Server");

    g_signal_connect(create_group_button, "clicked", G_CALLBACK(on_create_group_button_clicked), NULL);
    g_signal_connect(close_server_button2, "clicked", G_CALLBACK(on_close_server_button_clicked), NULL);

    gtk_grid_attach(GTK_GRID(create_group_grid), create_group_button, 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(create_group_grid), close_server_button2, 0, 3, 2, 1);

    create_group_tab_label = gtk_label_new("Crear Grupo");
    create_group_tab = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(create_group_tab), create_group_grid);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), create_group_tab, create_group_tab_label);

    // Show all widgets
    gtk_widget_show_all(window);

    // Setup socket connection
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    if (connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Error al conectar al servidor");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received <= 0) {
        perror("Error al recibir datos del servidor");
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0';
    char *decrypted = decrypt(buffer);
    printf("%s", decrypted);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.GtkApplication", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    close(client_socket);
    return status;
}
