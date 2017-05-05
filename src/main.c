/*#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>
int main()
{

	while(1)
	{
	int serial_port;

  if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
}

	if((serial_port = serialOpen("/dev/ttyAMA0", 9600)) < 0)
	{
		printf("\nerror during init serial");
		return 0;
	}

	int i;
	unsigned int c = 1;
		
	for(i=0; i<10; i++)
	{
		serialPutchar(serial_port, c);
	}

	}

	return 0;
