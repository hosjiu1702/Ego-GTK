#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>
int serial_port;
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

	write(serial_port, "12", 2);



	return 0;
}
