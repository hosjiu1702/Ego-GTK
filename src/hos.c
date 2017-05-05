#include <wiringSerial.h>

int main()
{
	wiringPiSetup();

	int serial_port;
	if((serial_port = serialOpen("/dev/ttyAMA0", 9600)) < 0)
	{
		printf("\nerror during init serial");
		return 0;
	}
	
	serialPutchar(serial_port, 1);

	while(1){}
	return 0;
}