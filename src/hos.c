#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include <wiringPi.h>
#include <wiringSerial.h>
int serial_port;
char len;
int sound_id = 12;
char* index_music;

int main(int argc, char **argv)
{

	

  if (wiringPiSetup () == -1)
  {
    //fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
}

	if((serial_port = serialOpen("/dev/ttyAMA0", 9600)) < 0)
	{
		//printf("\nerror during init serial");
		return 0;
	}

		//int i;
		//unsigned char c[1] = {"1"};

		//serialPutchar(serial_port, c);
  index_music = (char *)malloc(2*sizeof(char));
  sprintf(index_music, "%d", sound_id);
  len = strlen(index_music);

  write(serial_port, "1", 1);
  write(serial_port, index_music, len);

	return 0;
}
