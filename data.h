#ifndef data
#define data

#include<stdio.h>
#include<gtk/gtk.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdbool.h>

GtkWidget *buttons[200];
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
GtkWidget *make_directory;
GtkWidget *entry_name;
GtkWidget *entry_submit;
GtkWidget *re_name;
GtkWidget *del;
GtkWidget *open;
GtkWidget *vlc;
GtkWidget *vim;

bool is_open = false;

char current_file[100] = { 0 };
char *files[100] =  { 0 };

int valvs = 0;
int current_widgets;

struct values {
    char *files[100];
    int valv;
};


int is_folder(const char *path) {
    struct stat pather;
    stat(path, &pather);
    return S_ISDIR(pather.st_mode);
}

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
    valvs = 0;
    const gchar *string = gtk_entry_get_text(GTK_ENTRY(search_box));
    strcat(buffer, string);
    char *token = strtok(buffer, "/");
    while (token != 0) {
        files[valvs] = token;
        token = strtok(0, "/");
        valvs++;
    }
}


#endif
