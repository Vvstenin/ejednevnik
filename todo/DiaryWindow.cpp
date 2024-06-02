#include "DiaryWindow.h"
#include "utils/FileUtils.h"
#include <sstream>
#include <iostream>

DiaryWindow::DiaryWindow() {
    app = gtk_application_new("com.example.DiaryApp", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), this);


    add_default_holidays();
}

DiaryWindow::~DiaryWindow() {
    g_object_unref(app);
}

GtkApplication* DiaryWindow::get_application() {
    return app;
}

void DiaryWindow::run(int argc, char **argv) {
    g_application_run(G_APPLICATION(app), argc, argv);
}

void DiaryWindow::on_activate(GtkApplication *app, gpointer user_data) {
    DiaryWindow *self = static_cast<DiaryWindow*>(user_data);

    self->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(self->window), "Diary");
    gtk_window_set_default_size(GTK_WINDOW(self->window), 600, 400);

    self->box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(self->window), self->box);

    self->calendar = gtk_calendar_new();
    gtk_box_append(GTK_BOX(self->box), self->calendar);
    g_signal_connect(self->calendar, "day-selected", G_CALLBACK(on_calendar_day_selected), self);

    self->view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(self->view), TRUE); 
    gtk_box_append(GTK_BOX(self->box), self->view);

    self->entry = gtk_entry_new();
    gtk_box_append(GTK_BOX(self->box), self->entry);


    GtkWidget *time_label = gtk_label_new("Select Time:");
    gtk_box_append(GTK_BOX(self->box), time_label);


    GtkWidget *time_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(self->box), time_box);

   
    GtkWidget *hour_label = gtk_label_new("Hrs:");
    gtk_box_append(GTK_BOX(time_box), hour_label);

    self->hour_spin = gtk_spin_button_new_with_range(0, 23, 1);
    gtk_box_append(GTK_BOX(time_box), self->hour_spin);

    GtkWidget *minute_label = gtk_label_new("Min:");
    gtk_box_append(GTK_BOX(time_box), minute_label);

    self->minute_spin = gtk_spin_button_new_with_range(0, 59, 1);
    gtk_box_append(GTK_BOX(time_box), self->minute_spin);

    GtkWidget *second_label = gtk_label_new("Sec:");
    gtk_box_append(GTK_BOX(time_box), second_label);

    self->second_spin = gtk_spin_button_new_with_range(0, 59, 1);
    gtk_box_append(GTK_BOX(time_box), self->second_spin);

    self->add_button = gtk_button_new_with_label("Add Entry");
    gtk_box_append(GTK_BOX(self->box), self->add_button);
    g_signal_connect(self->add_button, "clicked", G_CALLBACK(on_add_button_clicked), self);

    self->delete_button = gtk_button_new_with_label("Delete Entry");
    gtk_box_append(GTK_BOX(self->box), self->delete_button);
    g_signal_connect(self->delete_button, "clicked", G_CALLBACK(on_delete_button_clicked), self);

    self->export_button = gtk_button_new_with_label("Export Entries");
    gtk_box_append(GTK_BOX(self->box), self->export_button);
    g_signal_connect(self->export_button, "clicked", G_CALLBACK(on_export_button_clicked), self);

    self->save_button = gtk_button_new_with_label("Save Edited Entry");
    gtk_box_append(GTK_BOX(self->box), self->save_button);
    g_signal_connect(self->save_button, "clicked", G_CALLBACK(on_save_button_clicked), self);

    gtk_widget_show(self->window);
}


std::string DiaryWindow::get_selected_date(GtkCalendar *calendar) {
    GDateTime *date_time = gtk_calendar_get_date(calendar);
    gint year = g_date_time_get_year(date_time);
    gint month = g_date_time_get_month(date_time);
    gint day = g_date_time_get_day_of_month(date_time);
    g_date_time_unref(date_time);
    std::ostringstream oss;
    oss << year << "-" << month << "-" << day;
    return oss.str();
}

std::string DiaryWindow::get_selected_time() {
    int hour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(hour_spin));
    int minute = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(minute_spin));
    int second = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(second_spin));
    std::ostringstream oss;
    oss << (hour < 10 ? "0" : "") << hour << ":"
        << (minute < 10 ? "0" : "") << minute << ":"
        << (second < 10 ? "0" : "") << second;
    return oss.str();
}

void DiaryWindow::save_current_entry() {
    const gchar *text = gtk_editable_get_text(GTK_EDITABLE(entry));
    if (text == nullptr || *text == '\0') {
        return;
    }

    std::string date = get_selected_date(GTK_CALENDAR(calendar));

 
    std::string time;
    if (gtk_spin_button_get_value(GTK_SPIN_BUTTON(hour_spin)) != 0 ||
        gtk_spin_button_get_value(GTK_SPIN_BUTTON(minute_spin)) != 0 ||
        gtk_spin_button_get_value(GTK_SPIN_BUTTON(second_spin)) != 0) {
        time = get_selected_time();
    }

    std::ostringstream entry_with_time;
    if (!time.empty()) {
        entry_with_time << "[" << time << "] ";
    }
    entry_with_time << text;

    if (entries.find(date) == entries.end()) {
        entries[date] = std::vector<std::string>();
    }
    entries[date].push_back(entry_with_time.str());

    gtk_editable_set_text(GTK_EDITABLE(entry), ""); 
}

void DiaryWindow::add_default_holidays() {
    std::map<std::string, std::vector<std::string>> default_holidays = {
        {"2024-1-1", {"New Year"}},
        {"2024-7-4", {"Independence Day"}},
    };

    for (const auto &holiday : default_holidays) {
        const std::string &date = holiday.first;
        const std::vector<std::string> &entries = holiday.second;

        if (this->entries.find(date) == this->entries.end()) {
            this->entries[date] = entries;
        } else {
            this->entries[date].insert(this->entries[date].end(), entries.begin(), entries.end());
        }
    }

    std::cout << "Added default holidays:" << std::endl;
    for (const auto &holiday : default_holidays) {
        std::cout << "Date: " << holiday.first << ", Entries: ";
        for (const auto &entry : holiday.second) {
            std::cout << entry << ", ";
        }
        std::cout << std::endl;
    }
}

void DiaryWindow::on_add_button_clicked(GtkButton *button, gpointer user_data) {
    DiaryWindow *self = static_cast<DiaryWindow*>(user_data);

    self->save_current_entry();
    std::string date = self->get_selected_date(GTK_CALENDAR(self->calendar));
    self->update_view(date);
}

void DiaryWindow::on_delete_button_clicked(GtkButton *button, gpointer user_data) {
    DiaryWindow *self = static_cast<DiaryWindow*>(user_data);

    std::string date = self->get_selected_date(GTK_CALENDAR(self->calendar));
    if (self->entries.find(date) != self->entries.end()) {
        if (!self->entries[date].empty()) {
            self->entries[date].pop_back();
        }
        if (self->entries[date].empty()) {
            self->entries.erase(date);
        }
    }

    self->update_view(date);
}

void DiaryWindow::on_export_button_clicked(GtkButton *button, gpointer user_data) {
    DiaryWindow *self = static_cast<DiaryWindow*>(user_data);

    std::string date = self->get_selected_date(GTK_CALENDAR(self->calendar));
    if (self->entries.find(date) != self->entries.end()) {
        std::string filename = "Diary_" + date + ".txt";
        FileUtils::write_to_file(filename, self->entries[date]);
    }
}

void DiaryWindow::on_save_button_clicked(GtkButton *button, gpointer user_data) {
    DiaryWindow *self = static_cast<DiaryWindow*>(user_data);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->view));
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    std::string date = self->get_selected_date(GTK_CALENDAR(self->calendar));
    if (self->entries.find(date) != self->entries.end()) {
        self->entries[date].clear();
        std::istringstream iss(text);
        std::string line;
        while (std::getline(iss, line)) {
            if (!line.empty()) {
                std::size_t pos = line.find(". ");
                if (pos != std::string::npos) {
                    line = line.substr(pos + 2);
                }
                self->entries[date].push_back(line);
            }
        }
    }

    g_free(text);
    self->update_view(date);
}

void DiaryWindow::on_calendar_day_selected(GtkCalendar *calendar, gpointer user_data) {
    DiaryWindow *self = static_cast<DiaryWindow*>(user_data);

    std::string date = self->get_selected_date(calendar);
    self->update_view(date);
}

void DiaryWindow::update_view(const std::string &date) {
    std::cout << "Updating view for date: " << date << std::endl;

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    std::string text;
    if (entries.find(date) != entries.end()) {
        int index = 1;
        for (const auto &entry : entries[date]) {
            text += std::to_string(index++) + ". " + entry + "\n";
        }
    }
    gtk_text_buffer_set_text(buffer, text.c_str(), -1);

    guint year, month, day;
    sscanf(date.c_str(), "%u-%u-%u", &year, &month, &day);

    if (!text.empty()) {
        gtk_calendar_mark_day(GTK_CALENDAR(calendar), day);
    } else {
        gtk_calendar_unmark_day(GTK_CALENDAR(calendar), day);
    }
}

