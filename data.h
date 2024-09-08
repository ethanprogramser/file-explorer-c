#ifndef data
#define data

#include<stdio.h>
#include<gtk/gtk.h>
#include<dirent.h>
#include<string.h>

GtkWidget *buttons[1000];
GtkWidget *window;
GtkWidget *box;
GtkWidget *button;
GtkWidget *box2;
GtkWidget *trash_can;
GtkWidget *search_box;
GtkWidget *search_button;
GtkWidget *db;
GtkWidget *file_system;
GtkWidget *grid;
GtkWidget *scroll;
GtkWidget *back_button;
GtkWidget *forward_button;

int current_widgets;

struct values {
    char *files[100];
    int valv;
};

char *files[100] =  { 0 };
int valvs = 0;

struct values dirp(char *dirp) {
    struct values val;
    struct dirent *dir;

    DIR *d;

    d = opendir(dirp);

    val.valv = 0;
    
    while((dir = readdir(d)) != NULL) {
        val.files[val.valv] = dir->d_name;
        val.valv++;
    }
    return val;
}

void fix() {
    char buffer[100] = { 0 };
    const gchar *string = gtk_entry_get_text(GTK_ENTRY(search_box));
    strcat(buffer, string);
    char *token = strtok(buffer, "/");
    while (token != 0) {
        files[valvs] = token;
        token = strtok(0, "/");
        valvs++;
    }
    printf("%s\n", files[2]);
}


#endif
