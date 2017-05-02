#include <gtk/gtk.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#define IMAGE_MAX_COUNT 83
#define fd serial_port

/*-------------------------------------*/
gint img_id_not_use[IMAGE_MAX_COUNT] = {-1};

/*---------------------------------------*/
gchar* button_glade_id[6];
gchar *button_image_id[6];
/*----------------------*/
gint result_img_id;
gint result_button_id;
Button *arr_button[6];
/*---------------------------------------*/
/*---------------------*/
static gboolean main_window_is_displayed = false;

gboolean display_default_window();
void on_window_1_destroy();
void show_result (GtkButton*, gpointer)
void init_some_components();
int init_serial(gint)

typedef struct _Button Button;
struct _Button
{
	GtkWidget *button;
	GtkWidget *image;
	gint id;
};

int main(int argc, char *argv[])
{

	wiringPiSetup();
	init_serial(9600);

	/*Cap phat bo nho cho Button struct*/
	gint i;
	for(i=0; i<6; i++)
	{
		arr_button[i] = g_slice_new(Button);
		arr_button[i]->id = i+1;
	}

	init_some_components();

	gtk_init(&argc, &argv);

	display_question_window();

	g_thread_new("uart_thread", uart_transfer, NULL);

	gtk_main();
/*	
	gint i;
	for(i=0; i<6; i++)
	{
		free(button_image_id[i]);
		free(button_glade_id[i]);
	}
*/
	return 0;

}

void
display_question_window()
{
	GtkBuilder *builder;
	GtkWidget *window_default;
	GtkWidget *origin_image;

	/*Pointer to window_1.glade file*/
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "glad/window_1.glade", NULL);

	/*Get certain objects inside .glade file*/
	window_default = GTK_WIDGET(gtk_builder_get_object(builder, "window_1"));
	origin_image = GTK_WIDGET(gtk_builder_get_object(builder, "origin_image"));
	
	gint m;
	for(m=0; m<6; m++)
	{
		arr_button[m]->button = GTK_WIDGET(gtk_builder_get_object(builder, button_glade_id[m]));
		arr_button[m]->image = GTK_WIDGET(gtk_builder_get_object(builder, button_image_id[m]));
	}

	/*Chon mot trong 6 nut de dua dap an vao*/
	result_button_id = g_random_int_range(1,7);

	/*Chon mot anh ngau nhien de hien thi*/
	gboolean same_image = FALSE;
	do
	{
		result_img_id = g_random_int_range(1, IMAGE_MAX_COUNT);
		gint i;
		for(i=0; i<IMAGE_MAX_COUNT; i++)
		{
			if( (img_id_not_use[i] != -1) && (img_id_not_use[i] == result_img_id) )
			{
				same_image = TRUE;
				break;
			}
		}
	}
	while(same_image);
	
	gint q = 0;
	while(img_id_not_use[q] != -1) q++;
	
	img_id_not_use[q] = result_img_id;

	/*load anh vao cac button va origin_image*/
	gint images_not_allowed[5] = {-1};
	gint o;
	for(o=1; o<=6; o++)
	{
		gchar path[50];

		/* Neu la nut dap an thi gan tam anh dap an vao de hien thi
		  va hien thi luon origin image */
		if( o == result_button_id )
		{
			sprintf(path, "res/animals/%d.png", result_img_id);
			gtk_image_set_from_file(GTK_IMAGE(arr_button[o-1]->image), path);
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
				gint j;
				for(j=0; j<5; j++)
				{
					if( (rand_img == images_not_allowed[j]) || (rand_img == result_img_id) )
					{
						same_img = TRUE;
						break;
					}
				}
			}
			while(same_img);
				
			gint k;
			for(k=0; k<5; k++)
			{
				if(images_not_allowed[k] == -1)
				{
					images_not_allowed[k] = rand_img;
					break;
				}
			}

			sprintf(path, "res/animals/%d.png", rand_img);
			gtk_image_set_from_file(GTK_IMAGE(arr_button[o-1]->image), path);
		}
	}

	/*Ket noi tin hieu cho cac button*/
	gint l;
	for(l=0; l<6; l++)
		g_signal_connect(G_OBJECT(arr_button[l]->button), "clicked", G_CALLBACK(show_result), &(arr_button[l]->id));

	gtk_window_fullscreen(GTK_WINDOW(window_default));
	gtk_widget_show_all(GTK_WIDGET(window_default));

	main_window_is_displayed = true;

	g_object_unref(builder);
}


void on_window_1_destroy()
{
	gtk_main_quit();
}

void
init_some_components()
{
	gint p;
	for(p=0; p<6; p++)
	{
		button_glade_id[p] = (gchar *)malloc(50*sizeof(gchar));
		button_image_id[p] = (gchar *)malloc(50*sizeof(gchar));
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

}

int
init_serial(gint baudrate)
{
	if((serial_port = serialOpen("/dev/ttyAMA0", baudrate)) < 0)
	{
		printf("\nerror during init serial");
		return 0;
	}
	else return 1;
}

gpointer
uart_transfer(gpointer data)
{
	static gboolean permit_iterate = true;

	/*lap lien tuc de truyen va nhan du lieu*/
	while(1)
	{
		/*Giai doan can nhan du lieu*/
		if(main_window_is_displayed)
		{
			if(permit_iterate)
			{
				if(serialDataAvail)
				{
					gint your_answer = serialGetchar(serial_port);
					for(gint i=0; i<6; i++)
					{
						if(your_answer == arr_button[i]->id)
						{
							gtk_button_clicked(GTK_BUTTON(arr_button[i]->button));
							break;
						}
					}
					permit_iterate = false;
				}
			}
		}
	}
}

void
show_result(GtkButton *button, gpointer user_data)
{
	Button *input = (Button *)user_data; //dong nay can xem lai
	if(/*ket qua dung*/)
	{
		/*hien thi ket qua dung o day*/

		/*tra ket qua ve cho ben kia*/
	}
	else
	{
		/*hien thi ket qua sai o day*/
	}
}