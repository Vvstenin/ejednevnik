#include <stdio.h>
#include <gtk/gtk.h>

typedef struct {
    guint year;
    guint month;
    guint day;
} Date;

static GHashTable *notes; // Хэш-таблица для хранения заметок

static void get_date(Date *date, gpointer user_data) {
    if (notes == NULL) {
        g_print("Хеш-таблица notes не инициализирована\n");
        return;
    }

    gchar *date_key = g_strdup_printf("%d-%02d-%02d", date->year, date->month + 1, date->day);

    const gchar *note = (const gchar *)g_hash_table_lookup(notes, date_key);

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(user_data),
                                         GTK_DIALOG_MODAL,
                                         GTK_MESSAGE_OTHER,
                                         GTK_BUTTONS_OK_CANCEL,
                                         "Заметка для %d.%02d.%02d:\n%s", date->year, date->month + 1, date->day, (note != NULL) ? note : "Нет заметки");
    gtk_window_present(GTK_WINDOW(dialog));
    gtk_window_destroy(GTK_WINDOW(dialog));

    g_free(date_key);
}

static void day_selected(GtkCalendar *calendar, guint year, guint month, guint day, gpointer user_data) {
    Date date = {year, month, day};
    get_date(&date, user_data);
}

static void activate(GApplication* app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Simple Calendar");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    notes = g_hash_table_new(g_str_hash, g_str_equal); // Инициализация хэш-таблицы

    GtkWidget *calendar = gtk_calendar_new();
    gtk_window_set_child(GTK_WINDOW(window), calendar);

    g_signal_connect(calendar, "day-selected", G_CALLBACK(day_selected), window);

    gtk_widget_show(window);
}

int main(int argc, char** argv) {
    GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    if (notes != NULL) {
        g_hash_table_destroy(notes); // Уничтожение хэш-таблицы после завершения работы
    }
    return status;
}