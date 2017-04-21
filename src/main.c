#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
	GtkBuilder *builder;
	GObject *window;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "glad/main.glade", NULL);

	window = gtk_builder_get_object(builder, "window_main");
	gtk_builder_connect_signals(builder, NULL);

	gtk_window_fullscreen(GTK_WINDOW(window));
	
	//g_object_unref(builder);

	gtk_widget_show(GTK_WIDGET(window));

	gtk_main();

	return 0;
}

void on_window_main_destroy()
{
	gtk_main_quit();
}