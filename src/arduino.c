#include <SD.h>
#define SD_ChipSelectPin 4
#include <TMRpcm.h>
#include <SPI.h>

#define SPEAKER_PIN 9
#define ANALOG_PIN A0
#define DUNG true
#define BUTTON_1_ANALOG_MIN_VALUE -3
#define BUTTON_1_ANALOG_MAX_VALUE 3
#define BUTTON_2_ANALOG_MIN_VALUE 100
#define BUTTON_2_ANALOG_MAX_VALUE 630
#define BUTTON_3_ANALOG_MIN_VALUE 660
#define BUTTON_3_ANALOG_MAX_VALUE 700
#define BUTTON_4_ANALOG_MIN_VALUE 765
#define BUTTON_4_ANALOG_MAX_VALUE 785
#define BUTTON_5_ANALOG_MIN_VALUE 815
#define BUTTON_5_ANALOG_MAX_VALUE 835
#define BUTTON_6_ANALOG_MIN_VALUE 870
#define BUTTON_6_ANALOG_MAX_VALUE 890

TMRpcm tmrpcm;
bool is_waiting_press_button = true;
bool is_waiting_result = true;
byte answer;
byte DATA[2];
byte value;
/*-----------------------------------------*/
void setup() {
  Serial.begin(9600);
  pinMode(ANALOG_PIN, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
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
      byte i=0;
      /*Neu da nhan duoc du lieu*/
      if(Serial.available() > 0)
      {
        data[i] = Serial.read();
      }
      //waitAnswer();

    }
    /*Neu da nhan duoc ket qua thi kiem tra ket qua dung hay sai*/
    else
    {
      /*Neu ket qua la dung*/
      if (answer == 49)
      {
        /*Thuc hien cong viec phat nhac VUI*/
        tmrpcm.play(".wav");
        //Serial.println("PHAT DUNG-4");
        //delay(5500);
        digitalWrite(SPEAKER_PIN, LOW);

      }
      /*Neu ket qua la sai*/
      else
      {
        /*Thuc hien cong viec phat nhac BUON*/
        tmrpcm.play("sai-4.wav");
        //Serial.println("PHAT SAI-4");
        //delay(5500);
        digitalWrite(SPEAKER_PIN, LOW);

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
  Serial.print(button_value); // DU LIEU duoc gui la ASCII
  return 1;
}

void init_module_SD_Card()
{
  /*DUNG DE KHOI TAO CHO VIEC PHAT AM THANH*/
  tmrpcm.speakerPin = 9;
  if (!SD.begin(SD_ChipSelectPin))
  {
    //Serial.println("SD fail");
    return;
  }
  else
  {
    //Serial.println("OK");
  }

  tmrpcm.play("xinchao.wav");
  //delay(3000);
}

void waitAnswer()
{
  if (Serial.available() > 0)
  {
    digitalWrite(13, HIGH);
    /*
    answer = Serial.read();
    Serial.write(answer);
    //Serial.println(answer);
    */
    is_waiting_result = false;
  }
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
