#ifndef DIARYWINDOW_H
#define DIARYWINDOW_H

#include <gtk/gtk.h>
#include <map>
#include <string>
#include <vector>

class DiaryWindow {
public:
    DiaryWindow();
    ~DiaryWindow();

    GtkApplication* get_application();
    void run(int argc, char **argv);

private:
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *calendar;
    GtkWidget *view;
    GtkWidget *entry;
    GtkWidget *add_button;
    GtkWidget *delete_button;
    GtkWidget *export_button;
    GtkWidget *save_button; 
    GtkWidget *box;
    GtkWidget *hour_spin;
    GtkWidget *minute_spin;
    GtkWidget *second_spin;
    GtkWidget *view_selector;

    std::map<std::string, std::vector<std::string>> entries;

    static void on_activate(GtkApplication *app, gpointer user_data);
    static void on_add_button_clicked(GtkButton *button, gpointer user_data);
    static void on_delete_button_clicked(GtkButton *button, gpointer user_data);
    static void on_export_button_clicked(GtkButton *button, gpointer user_data);
    static void on_save_button_clicked(GtkButton *button, gpointer user_data); 
    static void on_calendar_day_selected(GtkCalendar *calendar, gpointer user_data);
    static void on_view_selector_changed(GtkComboBoxText *combo, gpointer user_data);

    std::string get_selected_date(GtkCalendar *calendar);
    std::string get_selected_time();
    void update_view(const std::string &date);
    void update_view_for_selected_format();
    void save_current_entry();
    std::string get_week_range(const std::string &date);
    std::string get_month_range(const std::string &date);
    std::string get_year_range(const std::string &date);

    void add_default_holidays(); 
};

#endif // DIARYWINDOW_H
