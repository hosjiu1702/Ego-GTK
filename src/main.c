#include <gtk/gtk.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <pthread.h>

#define IMAGE_MAX_COUNT 83

/*-------------------------------------*/
gint img_id_not_use[IMAGE_MAX_COUNT] = {-1};

/*---------------------------------------*/
gchar* button_glade_id[6];
/*----------------------*/


/*---------------------------------------*/
gchar *button_image_id[6];

/*---------------------*/

gboolean display_default_window();
void on_window_1_destroy();
void show_result();

typedef struct _Button Button;
struct _Button
{
	GtkWidget *button;
	GtkWidget *image;
	gint id;
};

int main(int argc, char *argv[])
{

	for(gint i=0; i<6; i++)
	{
		button_glade_id[i] = (char *)malloc(50*sizeof(char));
		button_image_id[i] = (char *)malloc(50*sizeof(char));
	}
	
	strcpy(button_glade_id[0], "btn_top_left");
	strcpy(button_glade_id[1], "btn_center_left");
	strcpy(button_glade_id[2], "btn_bottom_left");
	strcpy(button_glade_id[3], "btn_top_right");
	strcpy(button_glade_id[4], "btn_center_right");
	strcpy(button_glade_id[5], "btn_bottom_right");

	strcpy(button_image_id[0], "img_top_left");
	strcpy(button_image_id[1], "img_center_left");
	strcpy(button_image_id[2], "img_bottom_left");
	strcpy(button_image_id[3], "img_top_right");
	strcpy(button_image_id[4], "img_center_right");
	strcpy(button_image_id[5], "img_bottom_right");

	gtk_init(&argc, &argv);

	g_timeout_add(50, (GSourceFunc)display_default_window, NULL);

	gtk_main();

	for(gint i=0; i<6; i++)
	{
		free(button_image_id[i]);
		free(button_glade_id[i]);
	}

	return 0;
}

gboolean display_default_window()
{
	GtkBuilder *builder;

	GObject *window_default;

	GtkWidget *origin_image;

	Button *arr_button[6];

	for(gint i=0; i<6; i++)
	{
		arr_button[i] = g_slice_new(Button);
		arr_button[i]->id = i+1;
	}

	/*Pointer to window_1.glade file*/
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "glad/window_1.glade", NULL);

	/*Get certain objects inside .glade file*/
	window_default = gtk_builder_get_object(builder, "window_1");

	/*Anh xa den cac doi tuong o ben glade*/
	for(gint i=0; i<6; i++)
	{
		arr_button[i]->button = GTK_WIDGET(gtk_builder_get_object(builder, button_glade_id[i]));
		arr_button[i]->image = GTK_WIDGET(gtk_builder_get_object(builder, button_image_id[i]));
	}
	origin_image = GTK_WIDGET(gtk_builder_get_object(builder, "origin_image"));

	/*Chon mot trong 6 nut de dua dap an vao*/
	gint result_button_id = g_random_int_range(1,7);

	/*Chon mot anh ngau nhien de hien thi*/
	gboolean same_image = FALSE;
	gint result_img_id;
	do
	{
		result_img_id = g_random_int_range(1, IMAGE_MAX_COUNT);
		for(int i=0; i<IMAGE_MAX_COUNT; i++)
		{
			if( (img_id_not_use[i] != -1) && (img_id_not_use[i] == result_img_id) )
			{
				same_image = TRUE;
				break;
			}
		}
	}
	while(same_image);
	
	gint i = 0;
	while(img_id_not_use[i] != -1) i++;
	
	img_id_not_use[i] = result_img_id;

	/*load anh vao cac button va origin_image*/
	gint images_not_allowed[5] = {-1};
	for(gint i=1; i<=6; i++)
	{
		gchar path[50];

		/* Neu la nut dap an thi gan tam anh dap an vao de hien thi
		  va hien thi luon origin image */
		if( i == result_button_id )
		{
			sprintf(path, "res/animals/%d.png", result_img_id);
			gtk_image_set_from_file(GTK_IMAGE(arr_button[i-1]->image), path);
			gtk_image_set_from_file(GTK_IMAGE(origin_image), path);
		}


		/*Gan hinh ngau nhien cho cac button*/
		else
		{
			gint rand_img;
			gboolean same_img = FALSE;

			do
			{
				rand_img = g_random_int_range(1,IMAGE_MAX_COUNT);
				for(gint j=0; j<5; j++)
				{
					if( (rand_img == images_not_allowed[j]) || (rand_img == result_img_id) )
					{
						same_img = TRUE;
						break;
					}
				}
			}
			while(same_img);
			
			for(gint k=0; k<5; k++)
			{
				if(images_not_allowed[k] == -1)
				{
					images_not_allowed[k] = rand_img;
					break;
				}
			}

			sprintf(path, "res/animals/%d.png", rand_img);
			gtk_image_set_from_file(GTK_IMAGE(arr_button[i-1]->image), path);
		}
	}

	/*Ket noi tin hieu cho cac button*/
	for(gint i=0; i<6; i++)
		g_signal_connect(G_OBJECT(arr_button[i]->button), "clicked", G_CALLBACK(show_result), &arr_button[i]);

	gtk_window_fullscreen(GTK_WINDOW(window_default));
	gtk_widget_show_all(GTK_WIDGET(window_default));

	g_object_unref(builder);

	return FALSE;
}

void on_window_1_destroy()
{
	gtk_main_quit();
}

void show_result(){}