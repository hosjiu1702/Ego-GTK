#include <gtk/gtk.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

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

const gint IMAGE_MAX_COUNT = 83;
gint serial_port;
/*-------------------------------------*/
gint img_id_not_use[IMAGE_MAX_COUNT] = {-1};

/*---------------------------------------*/
gchar *button_glade_id[6];
gchar *button_image_id[6];
/*----------------------*/
static gint result_img_id; // id cua nhung tam anh

static gint result_button_id; //id button chua dap an

static Button *arr_button[6];

/*---------------------------------------*/
/*---------------------*/
gboolean is_waiting_for_press_button = false;

void on_window_1_destroy();
void show_result (GtkWidget*, gpointer);
void init_some_components();
gint init_serial(gint);
gboolean transfer_uart(gpointer);
void destroy_window_default(gpointer);
void set_image_random();
gboolean delete_func(gpointer);

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

	/*Van con su dung cho nhung lan ke tiep => comment dong ben duoi*/
	/* g_object_unref(builder); */

	is_waiting_for_press_button = true;

	/*Ham cho xu ly khi thuc hien xong viec hien thi window_default*/
	g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, transfer_uart, NULL, NULL);

	/* g_thread_new("uart_thread", uart_transfer, NULL); */

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
	/*Xoa di window_default cu*/
	/*---------khong nen destroy vi con dung cho viec hien thi lan sau o window_1---*/
	//gtk_widget_destroy(GTK_WIDGET(window_default));

	/*Giai phong bo nho da cap truoc do*/
/*
	for(gint i=0; i<6; i++)
		g_slice_free(Button, &arr_button[i]);
*/
	/*An window_1*/
	gtk_widget_hide(GTK_WIDGET(window_default));

	/*Lay dap an cua user*/
	gint *your_answer = (gint *)user_data;

	gint sound_id = result_img_id;
	/*Neu dap an nhan duoc o day la dung*/
	if(*your_answer == result_button_id)
	{
		/*Thiet lap anh tick_icon cho image - Fullscreen*/
		gtk_image_set_from_file(GTK_IMAGE(image_result), "res/tick_icon.png"); //o day moi chi hien thi duoc tick icon
		gtk_widget_show_all(GTK_WIDGET(window_result));

		/*Gui tin hieu DUNG den arduino de phat nhac */
		serialPutchar(serial_port, 1);
		
		/*gui sound-id dung cho viec am thanh tuong ung voi image*/
		serialPutchar(serial_port, sound_id);

		/*Tam dung 3s*/
		//g_usleep(3000000);
	}
	/*Neu dap an nhan duoc o day la sai*/
	else
	{
		/*Hien thi hinh anh sai - Fullscreen*/
		gtk_image_set_from_file(GTK_IMAGE(image_result), "res/wrong_icon.png"); //o day moi chi hien thi duoc wrong icon
		gtk_widget_show_all(GTK_WIDGET(window_result));

		/*Gui tin hieu SAI den arduino de phat nhac */
		serialPutchar(serial_port, 0);
		
		/*Tam dung 3s*/
		//g_usleep(3000000);

	}

	/*reset de chon "cau hoi" moi*/
	g_timeout_add(2000, delete_func, NULL);

	/*KHONG XOA VI CON DE HIEN THI LAN SAU*/
	/*Xoa "cua so dap an"*/
	//gtk_widget_destroy(GTK_WIDGET(window));

	/*-------------------------Hien thi "cau hoi" tiep theo------------------------*/

	//g_usleep(1000000);
	/*hide window_2*/
	//gtk_widget_hide(GTK_WIDGET(window_result));

	/*Chon ngau nhien anh cho "cau hoi" ke tiep nay */
	//set_image_random();

	/*Hien thi "cau hoi" va cho user tuong tac*/
	//gtk_widget_show_all(GTK_WIDGET(window_default));

	/**/
//	is_waiting_for_press_button = false;
}

/*Day la idle function nen se loop lien tuc*/
gboolean
transfer_uart(gpointer user_data)
{

		/*Neu dang cho nhan cho viec nhan nut*/
		if(is_waiting_for_press_button)
		{
			/*Neu co du lieu den thi doc va phat tin hieu den button widget tuong ung*/
			if(serialDataAvail(serial_port))
			{
				/*Gia tri nay tu 1->6*/
				gchar pressed_button_value = (gchar)serialGetchar(serial_port);
							
				/* Gia lap su kien "clicked" voi button tuong ung*/
				gtk_button_clicked(GTK_BUTTON(arr_button[pressed_button_value-1]->button));

				/*Vong lap idle sau se khong cho an nut nua - khong doc serial*/
				is_waiting_for_press_button = false;
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
	/*Chon mot trong 6 nut de dua DAP AN vao*/
	result_button_id = g_random_int_range(1,7);

	/*Chon mot anh ngau nhien de hien thi*/
	gboolean same_image = FALSE;
	do
	{
		/*Chon ngau nhien mot tam anh (it's id)*/
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

	/*load anh vao cac button va image_default*/
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
			gtk_image_set_from_file(GTK_IMAGE(image_default), path);
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
}

gboolean
delete_func(gpointer user_data)
{
	/*Xoa anh cu - refresh for next image_result*/
	gtk_image_clear(GTK_IMAGE(image_result));

	/*An cua so dap an*/
	gtk_widget_hide(GTK_WIDGET(window_result));

	result_button_id = g_random_int_range(1,7);
	gboolean same_image = FALSE;
	do
	{
		/*Chon ngau nhien mot tam anh (it's id)*/
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
	
	/*Hien thi "cau hoi moi"*/
	gtk_widget_show_all(GTK_WIDGET(window_default));

	return G_SOURCE_REMOVE;
}