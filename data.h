#ifndef data
#define data

#include<stdio.h>
#include<gtk/gtk.h>
#include<dirent.h>

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

int current_widgets;

struct values {
    char *files[100];
    int valv;
};

struct values dirp(char *dirp) {
    struct values val;
    struct dirent *dir;

    DIR *d;

    d = opendir(dirp);

    int i = 0;
    val.valv = 0;
    
    while((dir = readdir(d)) != NULL) {
        val.files[i] = dir->d_name;
        val.valv++;
        i++;
    }
    return val;
}




#endif
