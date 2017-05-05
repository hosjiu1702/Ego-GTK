//#include <SD.h>
#define SD_ChipSelectPin 4
//#include <TMRpcm.h>
//#include <SPI.h>

#define SPEAKER_PIN 9

#define ANALOG_PIN A0

#define DUNG true

#define BUTTON_1_ANALOG_MIN_VALUE -1
#define BUTTON_1_ANALOG_MAX_VALUE 1000
#define BUTTON_2_ANALOG_MIN_VALUE 1900
#define BUTTON_2_ANALOG_MAX_VALUE 2200
#define BUTTON_3_ANALOG_MIN_VALUE 2600
#define BUTTON_3_ANALOG_MAX_VALUE 2900
#define BUTTON_4_ANALOG_MIN_VALUE 3000
#define BUTTON_4_ANALOG_MAX_VALUE 3100
#define BUTTON_5_ANALOG_MIN_VALUE 3200
#define BUTTON_5_ANALOG_MAX_VALUE 3400
#define BUTTON_6_ANALOG_MIN_VALUE 3500
#define BUTTON_6_ANALOG_MAX_VALUE 3600


//TMRpcm tmrpcm;

bool is_waiting_press_button = true;
bool is_waiting_result = true;

byte value;   // gia tri global cua nut nhan
bool result;
byte index_music;

char data[3];
/*-----------------------------------------*/
void setup() {
  Serial.begin(9600);
  Serial5.begin(9600);
  pinMode(ANALOG_PIN, INPUT);
  //pinMode(13, OUTPUT);
  //digitalWrite(13, LOW);

  /*Ham nay tam thoi bi xoa trong qua trinh phat trien - THEM VAO SAU*/
  //init_module_SD_Card();
}


/*---------------------------------*/
void loop() {

  /*Dang cho viec nhan nut*/
  if (is_waiting_press_button)
  {
    /*Neu user nhan nut thi gui gia tri do qua raspberry pi*/
    if (isPress())
    {
      send_button_value(value);
      is_waiting_press_button = false;
    }
  }

  /*Neu da nhan nut thi kiem tra ket qua tra ve*/
  else
  {
    /*Cho ket qua tra ve tu raspberry pi*/
    if (is_waiting_result)
    {
      static char i = 0;
      /*Neu co du lieu trong bo dem dang cho*/
      if(Serial5.available())
      {
        /*Luu DU LIEU nhan duoc vao day*/
        data[i] == Serial5.available();
        i++;

        /*Neu da nhan du DU LIEU*/
        if( i==3 )
        {
          i = 0;
          is_waiting_result = false;
        }
      }
    }
    /*Khi da co du lieu roi tien hanh kiem tra*/
    else
    {
      char result = data[0] - 48;
      Serial.write(result);
      unsigned char index_music = (data[1] - 48)*10 + (data[2] - 48)*1;
      /*Neu ket qua la dung*/
      if (result == 1)
      {
        Serial.println(" Playing music \"DUNG\" ");
        delay(1000);

        /*Thuc hien cong viec phat nhac VUI*/
        //tmrpcm.play(".wav");
        //Serial.println("PHAT DUNG-4");
        //delay(5500);
        //digitalWrite(SPEAKER_PIN, LOW);

      }
      /*Neu ket qua la sai*/
      else if(result == 0)
      {
        Serial.println(" Playing music \"SAI\" ");
        delay(1000);
        /*Thuc hien cong viec phat nhac BUON*/
       // tmrpcm.play("sai-4.wav");
        //Serial.println("PHAT SAI-4");
        //delay(5500);
        //digitalWrite(SPEAKER_PIN, LOW);

      }

      is_waiting_press_button = true;
      is_waiting_result = true;
    }
  }
}

byte read_button(byte analog_pin)
{
  int analog_value = analogRead(analog_pin);
  //Serial.println(analog_value);
  byte button_value = 0;

  if ( (analog_value > BUTTON_1_ANALOG_MIN_VALUE) && (analog_value < BUTTON_1_ANALOG_MAX_VALUE) )
    button_value = 1;
  else if ( (analog_value > BUTTON_2_ANALOG_MIN_VALUE) && (analog_value < BUTTON_2_ANALOG_MAX_VALUE) )
    button_value = 2;
  else if ( (analog_value > BUTTON_3_ANALOG_MIN_VALUE) && (analog_value < BUTTON_3_ANALOG_MAX_VALUE) )
    button_value = 3;
  else if ( (analog_value > BUTTON_4_ANALOG_MIN_VALUE) && (analog_value < BUTTON_4_ANALOG_MAX_VALUE) )
    button_value = 4;
  else if ( (analog_value > BUTTON_5_ANALOG_MIN_VALUE) && (analog_value < BUTTON_5_ANALOG_MAX_VALUE) )
    button_value = 5;
  else if ( (analog_value > BUTTON_6_ANALOG_MIN_VALUE) && (analog_value < BUTTON_6_ANALOG_MAX_VALUE) )
    button_value = 6;
  else
    button_value = 0;

  return button_value;
}

int send_button_value(byte button_value)
{
  Serial5.print(button_value); // DU LIEU duoc gui la ASCII
  return 1;
}

bool isPress()
{
  byte button_value = read_button(ANALOG_PIN);
  /*Neu nut nao do duoc nhan*/
  if (button_value != 0)
  {
    value = button_value;
    return true;
  }

  /*con neu khong thi tra false*/
  return false;
}

bool isGetResult()
{

}