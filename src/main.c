#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

void display_default_window();
void on_window_1_destroy();

typedef struct _Button Button;
struct _Button
{
	GtkWidget *button;
	GtkWidget *image;
	int unique_button_number;
};

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	display_default_window();

	gtk_main();

	return 0;
}

void display_default_window()
{
	GtkBuilder *builder;
	GtkWidget *origin_image;
	GObject *window_default;
	Button *button_top_left;
	Button *button_center_left;
	Button *button_bottom_left;
	Button *button_top_right;
	Button *button_center_right;
	Button *button_bottom_right;

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "glad/window_1.glade", NULL);

	window_default = gtk_builder_get_object(builder, "window_1");

	g_signal_connect(window_default,"destroy", G_CALLBACK(on_window_1_destroy), NULL);

	origin_image = gtk_image_new();

	/*allocate memory for each element of struct Button*/
	button_top_left =  g_slice_new(Button);
	button_center_left =  g_slice_new(Button);
	button_bottom_left =  g_slice_new(Button);
	button_top_right =  g_slice_new(Button);
	button_center_right =  g_slice_new(Button);
	button_bottom_right =  g_slice_new(Button);

	/*---------------------------------------------------*/
	/*TESTING STEP*/

	/*load certain image for GtkImgage widget inside GtkButton*/
//	button_top_left->image = gtk_image_new_from_file("res/animals/1.png");
//	button_center_left->image = gtk_image_new_from_file("res/animals/2.png");
//	button_bottom_left->image = gtk_image_new_from_file("res/animals/3.png");
//	button_top_right->image = gtk_image_new_from_file("res/animals/4.png");
//	button_center_right->image = gtk_image_new_from_file("res/animals/5.png");
//	button_bottom_right->image = gtk_image_new_from_file("res/animals/6.png");

	/*load CENTER IMAGE*/
//	origin_image = gtk_image_new_from_file("res/animals/1.png");
	gtk_image_set_from_file(GTK_IMAGE(origin_image), "res/animals/1.png");
	gtk_widget_show(origin_image);

	/*Display images on screen*/
//	gtk_button_set_image(GTK_BUTTON(button_top_left->button), button_top_left->image);

	/*Display CENTER IMAGE*/
//	gtk_widget_show(origin_image);
	/*---------------------------------------------------*/


//	builder = gtk_builder_new();
//	gtk_builder_add_from_file(builder, "glad/window_1.glade", NULL);

//	window_default = gtk_builder_get_object(builder, "window_1");

//	g_signal_connect(window_default,"destroy", G_CALLBACK(on_window_1_destroy), NULL);

	gtk_window_fullscreen(GTK_WINDOW(window_default));
	gtk_widget_show(GTK_WIDGET(window_default));

	g_object_unref(builder);
}

void on_window_1_destroy()
{
	gtk_main_quit();
}
