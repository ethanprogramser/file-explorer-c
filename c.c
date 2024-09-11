#include<gtk/gtk.h>
#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>

#include "data.h"


void send() {
    if(is_open) {
      gtk_widget_destroy(re_name);
      gtk_widget_destroy(del);
      gtk_widget_destroy(open);
      is_open = false;
    }
    gtk_container_foreach(GTK_CONTAINER(box2), (void*) gtk_widget_destroy, NULL);
    current_widgets = 0;
}

static void del_file(GtkWidget *butt, gpointer *pr)  
{
    char buffer[100] = { 0 };

    const gchar *l = gtk_entry_get_text(GTK_ENTRY(search_box));

    strcat(buffer, l);
    strcat(buffer, current_file);

    unlink(buffer);

    send();

    gtk_widget_show_all(window); 
}

static void open_file(GtkWidget *button, gpointer *pr) 
{

}

static void rename_file(GtkWidget *button, gpointer *pr) 
{

}


static void open_or_press(GtkWidget *button, gpointer *pr) {
    const gchar *lr = gtk_button_get_label(GTK_BUTTON(button));
    strcpy(current_file, lr);
    printf("%s\n", current_file);

    char buffer[100] = { 0 };
    const gchar *l = gtk_entry_get_text(GTK_ENTRY(search_box));
    const gchar *r = gtk_button_get_label(GTK_BUTTON(button));

    strcat(buffer, l);
    strcat(buffer, r);

    if(is_folder(buffer)) 
    {
        send();
        struct values val = dirp(buffer);
        for(int i=0; i<val.valv-1;i++) {
            buttons[i] = gtk_button_new_with_label(val.files[i]);
            gtk_container_add(GTK_CONTAINER(box2), buttons[i]);
            g_signal_connect(buttons[i], "clicked", G_CALLBACK(open_or_press), NULL);
            current_widgets++;
        }
        strcat(buffer, "/");
        gtk_entry_set_text(GTK_ENTRY(search_box), buffer);
    }
    else 
    {
          is_open = true;
          re_name = gtk_button_new_with_label("rename");
          del = gtk_button_new_with_label("delete");
          open = gtk_button_new_with_label("open");

          g_signal_connect(re_name, "clicked", G_CALLBACK(rename_file), NULL);
          g_signal_connect(del, "clicked", G_CALLBACK(del_file), NULL);
          g_signal_connect(open, "clicked", G_CALLBACK(open_file), NULL);

          gtk_grid_attach_next_to(GTK_GRID(grid), open, make_directory, GTK_POS_RIGHT, 10, 10);
          gtk_grid_attach_next_to(GTK_GRID(grid), re_name, open, GTK_POS_RIGHT, 10, 10);
          gtk_grid_attach_next_to(GTK_GRID(grid), del, re_name, GTK_POS_RIGHT, 10, 10);
    }

    gtk_widget_show_all(window);
}

void create_and_contain(struct values val) {
    for(int i=0; i<val.valv-1;i++) {
        buttons[i] = gtk_button_new_with_label(val.files[i]);
        gtk_container_add(GTK_CONTAINER(box2), buttons[i]);
        g_signal_connect(buttons[i], "clicked", G_CALLBACK(open_or_press), NULL);
        current_widgets++;
    }
    gtk_widget_show_all(window);
}

void reload()
{
    char buffer[100] = { 0 };
    const gchar *p = gtk_entry_get_text(GTK_ENTRY(search_box));
    strcat(buffer, p);
    struct values r = dirp(buffer);
    create_and_contain(r);
    gtk_entry_set_text(GTK_ENTRY(search_box), buffer); 
}

static void home(GtkWidget *button, gpointer *data) {
    struct values r = dirp("/home/ethan");
    send();
    create_and_contain(r);
    gtk_entry_set_text(GTK_ENTRY(search_box), "/home/ethan/");
}

static void trash(GtkWidget *button, gpointer *data) {
    struct values r = dirp("/home/ethan/.local/share/Trash");
    send();
    create_and_contain(r);
    gtk_entry_set_text(GTK_ENTRY(search_box), "/home/ethan/.local/share/Trash");
}

static void search_for(GtkWidget *button, gpointer *d) {
    char buffer[100] = { 0 };
    
    const gchar *p = gtk_entry_get_text(GTK_ENTRY(search_box));
    
    strcat(buffer, p);
    struct values r = dirp(buffer);
    send();
    
    create_and_contain(r);
}

static void downloads(GtkWidget *buttons, gpointer *R) {
    struct values r = dirp("/home/ethan/Downloads");
    send();
    create_and_contain(r);
    
    gtk_entry_set_text(GTK_ENTRY(search_box), "/home/ethan/Downloads");
}

static void file_sys(GtkWidget *button, gpointer *b) {
    struct values r = dirp("//");
    send();
    create_and_contain(r);
    
    gtk_entry_set_text(GTK_ENTRY(search_box), "//");
}

static void back_arrow() {
    fix();
    
    char buffer[100] = { 0 };
    strcat(buffer, "/");
    
    for(int i=0;i<valvs - 1; i++) {
        strcat(buffer, files[i]);
        strcat(buffer, "/");
    }
    
    struct values r = dirp(buffer);
    send();
    create_and_contain(r);
    
    gtk_entry_set_text(GTK_ENTRY(search_box), buffer);
}

static void create_folder(GtkWidget *button, gpointer *data) {
  char buffer[100] = { 0 };

  const gchar *p = gtk_entry_get_text(GTK_ENTRY(search_box));
  const gchar *r = gtk_entry_get_text(GTK_ENTRY(entry_name));
  
  strcat(buffer, p);
  strcat(buffer, r);
  
  mkdir(buffer, 0777);
  
  gtk_widget_destroy(entry_name);
  gtk_widget_destroy(entry_submit);
  
  reload();
  
  gtk_widget_show_all(window);
}

static void namefolder(GtkWidget *button, gpointer *da) 
{
    //system("konsole --hold -e nvim /home/ethan/projects/c/file_explorer/fix/p.c");
    entry_name = gtk_entry_new();
    entry_submit = gtk_button_new_with_label("create");
    
    gtk_widget_set_halign(entry_name, GTK_ALIGN_START);
    gtk_widget_set_halign(entry_submit, GTK_ALIGN_START);
    gtk_widget_set_valign(entry_submit, GTK_ALIGN_START);
    gtk_widget_set_valign(entry_name, GTK_ALIGN_START);

    gtk_grid_attach_next_to(GTK_GRID(grid), entry_name, trash_can, GTK_POS_BOTTOM, 20, 20);
    gtk_grid_attach_next_to(GTK_GRID(grid), entry_submit, entry_name, GTK_POS_BOTTOM, 20, 20);
    g_signal_connect(entry_submit, "clicked", G_CALLBACK(create_folder), NULL);
    gtk_widget_show_all(window);
} 

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    grid = gtk_grid_new();
    button = gtk_button_new_with_label("Home");
    trash_can = gtk_button_new_with_label("Trash");
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    db = gtk_button_new_with_label("Downloads");
    file_system = gtk_button_new_with_label("File System");
    search_box = gtk_entry_new();
    search_button = gtk_button_new_with_label("search");
    scroll = gtk_scrolled_window_new(NULL, NULL);
    back_button = gtk_button_new_with_label("<");
    make_directory = gtk_button_new_with_label("new folder");


    

    //setup scrollbar
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), 
                            GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);

    //setup grid
    
    gtk_widget_set_halign(grid, GTK_ALIGN_START);
    gtk_widget_set_valign(grid, GTK_ALIGN_START);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), true);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), true);


    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_container_add(GTK_CONTAINER(scroll), box2);

    gtk_grid_attach(GTK_GRID(grid), search_box, 0, 0, 50, 10);
    gtk_grid_attach_next_to(GTK_GRID(grid), search_button, search_box, GTK_POS_RIGHT, 10,10);
    gtk_grid_attach_next_to(GTK_GRID(grid), back_button, search_box, GTK_POS_LEFT, 10, 10);
    gtk_grid_attach_next_to(GTK_GRID(grid), scroll, search_box, GTK_POS_BOTTOM, 60, 70);
    gtk_grid_attach_next_to(GTK_GRID(grid), make_directory, scroll, GTK_POS_BOTTOM, 10, 10);
    gtk_grid_attach_next_to(GTK_GRID(grid), button, scroll, GTK_POS_LEFT, 10, 10);
    gtk_grid_attach_next_to(GTK_GRID(grid), db, button, GTK_POS_BOTTOM, 10, 10);
    gtk_grid_attach_next_to(GTK_GRID(grid), file_system, db, GTK_POS_BOTTOM,10, 10);
    gtk_grid_attach_next_to(GTK_GRID(grid), trash_can, file_system,GTK_POS_BOTTOM, 10, 10);
    
    g_signal_connect(button, "clicked", G_CALLBACK(home), NULL);
    g_signal_connect(trash_can, "clicked", G_CALLBACK(trash), NULL);
    g_signal_connect(search_button, "clicked", G_CALLBACK(search_for),NULL);
    g_signal_connect(db, "clicked", G_CALLBACK(downloads), NULL);
    g_signal_connect(file_system,"clicked", G_CALLBACK(file_sys), NULL);
    g_signal_connect(back_button, "clicked", G_CALLBACK(back_arrow),NULL);
    g_signal_connect(make_directory, "clicked", G_CALLBACK(namefolder), NULL);

    struct values r = dirp("/home/ethan/");
    create_and_contain(r);
    gtk_entry_set_text(GTK_ENTRY(search_box), "/home/ethan/");

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
  }


