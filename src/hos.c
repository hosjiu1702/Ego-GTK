#include <wiringSerial.h>

int main()
{
	wiringPiSetup();
		if((serial_port = serialOpen("/dev/ttyAMA0", baudrate)) < 0)
	{
		printf("\nerror during init serial");
		return 0;
	}
	
	serialPutchar(fd, 1);

	return 0;
}