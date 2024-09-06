#include<gtk/gtk.h>
#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#include "data.h"

void send() {
    gtk_container_foreach(GTK_CONTAINER (box2), (void*) gtk_widget_destroy, NULL);
    current_widgets = 0;
}

void create_and_contain(struct values val) {
    for(int i=0; i<val.valv-1;i++) {
        buttons[i] = gtk_button_new_with_label(val.files[i]);
        gtk_container_add(GTK_CONTAINER(box2), buttons[i]);
        g_signal_connect(buttons[i], "clicked", G_CALLBACK(send), NULL);
        current_widgets++;
    }
    gtk_widget_show_all(window);
}

static void home(GtkWidget *button, gpointer *data) {
    struct values r = dirp("/home/ethan");
    create_and_contain(r);
}

static void trash(GtkWidget *button, gpointer *data) {
    struct values r = dirp("/home/ethan/.local/share/Trash");
    create_and_contain(r);
}

static void search_for(GtkWidget *button, gpointer *d) {
    char buffer[100] = { 0 };
    const gchar *p = gtk_entry_get_text(GTK_ENTRY(search_box));
    strcat(buffer, p);
    struct values r = dirp(buffer);
    create_and_contain(r);
}


int main(int argc, char *argv[]) 
{
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    button = gtk_button_new_with_label("Home");
    trash_can = gtk_button_new_with_label("Trash");
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    search_box = gtk_entry_new();
    search_button = gtk_button_new_with_label("search");

    gtk_container_add(GTK_CONTAINER(window), box);

    gtk_container_add(GTK_CONTAINER(box), button);
    gtk_container_add(GTK_CONTAINER(box), trash_can);
    gtk_container_add(GTK_CONTAINER(box), box2);
    gtk_container_add(GTK_CONTAINER(box), search_box);
    gtk_container_add(GTK_CONTAINER(box), search_button);

    g_signal_connect(button, "clicked", G_CALLBACK(home), NULL);
    g_signal_connect(trash_can, "clicked", G_CALLBACK(trash), NULL);
    g_signal_connect(search_button, "clicked", G_CALLBACK(search_for),NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
} 
