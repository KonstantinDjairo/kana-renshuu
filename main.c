#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

GtkWidget *label;
GtkWidget *entry;

// Custom mapping for 101 Kana characters
GHashTable *kana_mapping;

// Define your custom Kana mapping
void create_kana_mapping() {
// TODO: copy the mapping from the 101kana layout in fcitx-anthy, because that one is wrong (ofc)
    kana_mapping = g_hash_table_new(NULL, NULL);
    g_hash_table_insert(kana_mapping, GUINT_TO_POINTER(GDK_KEY_1), "あ");
    g_hash_table_insert(kana_mapping, GUINT_TO_POINTER(GDK_KEY_2), "い");
    g_hash_table_insert(kana_mapping, GUINT_TO_POINTER(GDK_KEY_3), "う");
    g_hash_table_insert(kana_mapping, GUINT_TO_POINTER(GDK_KEY_4), "え");
    g_hash_table_insert(kana_mapping, GUINT_TO_POINTER(GDK_KEY_5), "お");
    g_hash_table_insert(kana_mapping, GUINT_TO_POINTER(GDK_KEY_q), "か");
    g_hash_table_insert(kana_mapping, GUINT_TO_POINTER(GDK_KEY_w), "き");
    g_hash_table_insert(kana_mapping, GUINT_TO_POINTER(GDK_KEY_e), "く");
    g_hash_table_insert(kana_mapping, GUINT_TO_POINTER(GDK_KEY_r), "け");
    g_hash_table_insert(kana_mapping, GUINT_TO_POINTER(GDK_KEY_t), "こ");
    // Add more entries for other Kana characters and keys
}

// Function to retrieve Kana character from mapping
const gchar *get_kana_character(guint keyval) {
    gpointer result = g_hash_table_lookup(kana_mapping, GUINT_TO_POINTER(keyval));
    return result ? (const gchar *)result : "";
}

void generate_new_kana() {
    guint keyval = GDK_KEY_1 + rand() % 10; // This assumes keys 1-9 map to Kana
    gchar kana_character[2] = {0, 0};
    kana_character[0] = *get_kana_character(keyval);
    gtk_label_set_text(GTK_LABEL(label), kana_character);
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

void check_input(GtkWidget *widget, gpointer user_data) {
    const gchar *user_input = gtk_entry_get_text(GTK_ENTRY(widget));
    gchar expected_kana[2] = {0, 0};
    gtk_label_get_text(GTK_LABEL(label), expected_kana);

    if (g_strcmp0(user_input, expected_kana) == 0) {
        double wpm = calculate_wpm(1.0);  // 1 second for simplicity
        char result[100];
        g_sprintf(result, "Correct! Your WPM: %.2f", wpm);
        gtk_label_set_text(GTK_LABEL(label), result);
        g_timeout_add_seconds(2, generate_new_kana, NULL);
    } else {
        gtk_label_set_text(GTK_LABEL(label), "Incorrect. Try again.");
    }
}

double calculate_wpm(double time_taken) {
    return (60.0 / time_taken);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *button;

    gtk_init(&argc, &argv);

    create_kana_mapping();

    srand(time(NULL));

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Kana Typing Game");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span size='x-large'>Press Enter to start...</span>");
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);

    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), entry, TRUE, TRUE, 0);

    button = gtk_button_new_with_label("Start Game");
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    g_signal_connect(button, "clicked", G_CALLBACK(generate_new_kana), NULL);
    g_signal_connect(entry, "activate", G_CALLBACK(check_input), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
