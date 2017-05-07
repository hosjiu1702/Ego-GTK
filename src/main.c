#include <gtk/gtk.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#define IMAGE_MAX_INDEX 83

typedef struct _Button Button;

struct _Button
{
	GtkWidget *button;
	GtkWidget *image;
	gint id;
};

enum
{
	false,
	true
};


static gint i = 0;
gint serial_port;
/*-------------------------------------*/

/*---------------------------------------*/
gchar *button_glade_id[6];
gchar *button_image_id[6];
/*----------------------*/
gint result_img_id; // id cua nhung tam anh

gint result_button_id; //id button chua dap an

Button *arr_button[6];

/*---------------------------------------*/
/*---------------------*/
gboolean is_waiting_for_press_button = false;
gint current_max_index = IMAGE_MAX_INDEX - 1;

/*Mang dung luu tru "id" cua file anh va am thanh*/
gint array_image_id[IMAGE_MAX_INDEX];
gint array_image_id_for_button[IMAGE_MAX_INDEX];

void on_window_1_destroy();
void show_result (GtkWidget*, gpointer);
void init_some_components();
gint init_serial(gint);
gboolean transfer_uart(gpointer);
void destroy_window_default(gpointer);
void set_image_random();
gboolean delete_func(gpointer);
void Swap(gint*, gint*);

/*Cac bien con tro nay duoc dung suot chuong trinh*/
/* 			dung de hien thi window_1 		*/
GtkBuilder *builder;
GtkWidget *window_default;
GtkWidget *window_result;
GtkWidget *image_default;
GtkWidget *image_result;


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

	/*Pointer to window_1.glade file*/
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "glad/window_1.glade", NULL);
	gtk_builder_add_from_file(builder, "glad/window_2.glade", NULL);

	/*-----------------LAY CAC OBJECT O WINDOW_1--------------------*/
	/*Get certain objects inside .glade file*/
	window_default = GTK_WIDGET(gtk_builder_get_object(builder, "window_1"));
	image_default = GTK_WIDGET(gtk_builder_get_object(builder, "image_default"));
	
	/*Vung nho duoc cap phat nay se duoc dung xuyen suot chuong trinh*/
	gint m;
	for(m=0; m<6; m++)
	{
		arr_button[m]->button = GTK_WIDGET(gtk_builder_get_object(builder, button_glade_id[m]));
		arr_button[m]->image = GTK_WIDGET(gtk_builder_get_object(builder, button_image_id[m]));
	}

	/*-----------------LAY CAC OBJECT O WINDOW_2--------------------*/
	window_result = GTK_WIDGET(gtk_builder_get_object(builder, "window_2"));
	image_result = GTK_WIDGET(gtk_builder_get_object(builder, "image_result"));

		/*FUNCTION FOR RANDOM IMAGE*/

	/*Lap de gan cac tri "id" cua image vao trong mang
	 * Duoc danh so tu 0 */
	gint a;
	for(a=0; a<IMAGE_MAX_INDEX; a++)
	{
		array_image_id[a] = a+1;
		array_image_id_for_button[a] = a+1;
	}


	set_image_random();

	/*Ket noi tin hieu cho cac button*/
	gint l;
	for(l=0; l<6; l++)
	{
		g_signal_connect(G_OBJECT(arr_button[l]->button), "clicked", G_CALLBACK(show_result), &(arr_button[l]->id));
	}

	/*Thiet lap hien thi fullscreen*/
	gtk_window_fullscreen(GTK_WINDOW(window_default));
	gtk_window_fullscreen(GTK_WINDOW(window_result));

	/*Chi hien thi window_1*/
	gtk_widget_show_all(GTK_WIDGET(window_default));

	is_waiting_for_press_button = true;

	/*Ham cho xu ly khi thuc hien xong viec hien thi window_default*/
	g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, transfer_uart, NULL, NULL);

	gtk_main();

	/*-----------------SAU KHI THOAT KHOI APPLICATION--------------------*/

	/*Free memory*/
	gint r;
	for(r=0; r<6; r++)
	{
		free(button_image_id[r]);
		free(button_glade_id[r]);
		g_slice_free(Button, arr_button[r]);
	}

	/*Xoa di doi tuong builder*/
	g_object_unref(builder);

	return 0;
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


gint
init_serial(gint baudrate)
{
	if((serial_port = serialOpen("/dev/ttyAMA0", baudrate)) < 0)
	{
		printf("\nerror during init serial");
		return 0;
	}
	else return 1;
}


void show_result(GtkWidget *widget, gpointer user_data)
{

	/*Lay dap an cua user*/
	gint *your_answer = (gint *)user_data;

	/*index of music sound fie (.wav)*/
	gint sound_id = result_img_id;

	g_print("gia tri nut nhan cua user: %d\n", *your_answer);
	g_print("gia tri nut nhan cua result: %d\n", result_button_id);
	g_print("DAP AN: %d\n", result_img_id);
	
	/*Neu dap an nhan duoc o day la dung*/
	if(*your_answer == result_button_id)
	{
		/*Thiet lap anh tick_icon cho image - Fullscreen*/
		gtk_image_set_from_file(GTK_IMAGE(image_result), "res/tick_icon.png"); //o day moi chi hien thi duoc tick icon
		gtk_widget_show_all(GTK_WIDGET(window_result));
		
		char *index_music = (char *)malloc(2*sizeof(char));
		if(sound_id > 9)
		{	
			sprintf(index_music, "%d", sound_id);
		}
		else
		{
			sprintf(index_music, "0%d", sound_id);
		}
		write(serial_port, "1", 1);
		write(serial_port, index_music, 2);
		free(index_music);
	}
	/*Neu dap an nhan duoc o day la sai*/
	else
	{
		/*Hien thi hinh anh sai - Fullscreen*/
		gtk_image_set_from_file(GTK_IMAGE(image_result), "res/wrong_icon.png"); //o day moi chi hien thi duoc wrong icon
		gtk_widget_show_all(GTK_WIDGET(window_result));

		/*Gui tin hieu SAI den arduino*/
		write(serial_port, "0", 1);
		write(serial_port, "00", 2);
	}

	/*reset de chon "cau hoi" moi*/
	g_timeout_add(2000, delete_func, NULL);

	/*An window_1*/
	gtk_widget_hide(GTK_WIDGET(window_default));
}

/*Day la idle function nen se loop lien tuc*/
gboolean
transfer_uart(gpointer user_data)
{

		/*Neu dang cho nhan cho viec nhan nut*/
		if(is_waiting_for_press_button)
		{
			/*Gui tin hieu thong bao cho arduino */
			if(i == 0)
			{
				write(serial_port, "o", 1);
				i++;
			}
  			

			/*Neu co du lieu den thi doc va phat tin hieu den button widget tuong ung*/
			if(serialDataAvail(serial_port))
			{
				/*Gia tri nay tu 1->6 (thuc te la ma ASCII)*/
				gchar pressed_button_value = (gchar)serialGetchar(serial_port);
				pressed_button_value = pressed_button_value - 48;
							
				/* Gia lap su kien "clicked" voi button tuong ung*/
				gtk_button_clicked(GTK_BUTTON(arr_button[pressed_button_value-1]->button));

				/*Vong lap idle sau se khong cho an nut nua - khong doc serial*/
				is_waiting_for_press_button = false;
			}
		}
		/*XOA DU LIEU THUA - CHONG NHIEU*/
		else
		{
			if(serialDataAvail(serial_port))
			{
				serialGetchar(serial_port);
			}
		}

	return G_SOURCE_CONTINUE;
}

/*
 * Ham nay duoc thuc hien moi khi 
 *	window_1 hien thi "cau hoi" moi
 */
void set_image_random()
{
	gchar path[50];
	gint random_index;

	if(current_max_index < 0)
		current_max_index = IMAGE_MAX_INDEX - 1;

	/*Chon ngau nhien mot chi so cua mang, 0 -> 82 */
	random_index = g_random_int_range(0, current_max_index + 1);

	/* Sap xep lai mang array_image_id[] */
	gint j;
	for(j=random_index; j<current_max_index; j++)
	{
		Swap(&array_image_id[j], &array_image_id[j+1]);
	}

	current_max_index = current_max_index - 1;

	sprintf(path, "res/animals/%d.png", array_image_id[random_index]);
	g_print("%s\n", path);
	gtk_image_set_from_file(GTK_IMAGE(image_default), path);
	result_img_id = array_image_id[random_index];

	result_button_id = g_random_int_range(1,7);

	gint n;
	gint arr_button_index[5];
	gint current_max_index_for_button = IMAGE_MAX_INDEX - 1;
	for(n=0; n<5; n++)
	{
		gboolean is_same = true;
		while(is_same)
		{
			gint button_index = g_random_int_range(0, current_max_index_for_button + 1);
			if(array_image_id_for_button[button_index] != array_image_id[random_index])
			{
				arr_button_index[n] = array_image_id_for_button[button_index];
				is_same = false;
				gint p;
				for(p=button_index; p<current_max_index_for_button; p++)
				{
					Swap(&array_image_id_for_button[p], &array_image_id_for_button[p+1]);
				}
				current_max_index_for_button = current_max_index_for_button - 1;
			}
		}
	}

	gint m;
	gint z = 0;
	for(m=0; m<6; m++)
	{
		if(m == (result_button_id-1))
		{
			gtk_image_set_from_file(GTK_IMAGE(arr_button[m]->image), path);
		}
		else
		{
			gchar path_for_button[50];
			sprintf(path_for_button, "res/animals/%d.png", arr_button_index[z++]);
			gtk_image_set_from_file(GTK_IMAGE(arr_button[m]->image), path_for_button);
		}
	}
}

gboolean
delete_func(gpointer user_data)
{
	set_image_random();

	/*Hien thi "cau hoi moi"*/
	gtk_widget_show_all(GTK_WIDGET(window_default));

	/*Xoa anh cu - refresh for next image_result*/
	gtk_image_clear(GTK_IMAGE(image_result));

	/*An cua so dap an*/
	gtk_widget_hide(GTK_WIDGET(window_result));

	is_waiting_for_press_button = true;

	i = 0;

	return G_SOURCE_REMOVE;
}

void Swap(gint *a, gint *b)
{
	gint temp;
	temp = *a;
	*a = *b;
	*b = temp;
}