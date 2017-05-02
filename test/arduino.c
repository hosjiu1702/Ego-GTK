#include <SD.h>
#define SD_ChipSelectPin 4
#include <TMRpcm.h>
#include <SPI.h>

#define SD_CARD_PIN 9
#define ANALOG_PIN A0
#define DUNG true
#define BUTTON_1_ANALOG_MIN_VALUE -3
#define BUTTON_1_ANALOG_MAX_VALUE 3
#define BUTTON_2_ANALOG_MIN_VALUE 510
#define BUTTON_2_ANALOG_MAX_VALUE 530
#define BUTTON_3_ANALOG_MIN_VALUE 680
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

/*-----------------------------------------*/
void setup() {
  Serial.begin(9600);
  pinMode(ANALOG_PIN, INPUT);
  init_module_SD_Card();
}


/*---------------------------------*/
void loop() {

  /*Dang cho viec nhan nut*/
  if (is_waiting_press_button)
  {
    byte button_value = readButton();
    send(button_value);
  }

  /*Neu da nhan nut thi kiem tra ket qua tra ve*/
  else
  {
    /*Cho ket qua tra ve tu raspberry pi*/
    if (is_waiting_result)
    {
        waitAnswer();
    }
    /*Neu da nhan duoc ket qua thi kiem tra ket qua dung hay sai*/
    else
    {
      /*Neu ket qua la dung*/
      if (answer == DUNG)
      {
        /*Thuc hien cong viec phat nhac VUI*/
        tmrpcm.play("sai.wav");
        delay(5500);
        digitalWrite(SD_CARD_PIN, LOW);
      }
      /*Neu ket qua la sai*/
      else
      {
        /*Thuc hien cong viec phat nhac BUON*/
        tmrpcm.play("sai.wav");
        delay(5500);
        digitalWrite(SD_CARD_PIN, LOW);

      }
      is_waiting_press_button = true;
    }
  }
}

byte readButton()
{
  int analog_value = analogRead(ANALOG_PIN);
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

int send(byte button_value)
{
  if (button_value == 0)
    return 0;
  serial.write(button_value);
  is_waiting_press_button = false;

  return 1;
}

void init_module_SD_Card()
{
  /*DUNG DE KHOI TAO CHO VIEC PHAT AM THANH*/
  tmrpcm.speakerPin = 9;
  if (!SD.begin(SD_ChipSelectPin))
  {
    Serial.println("SD fail");
    return;
  }
  else
  {
    Serial.println("OK");
  }

  tmrpcm.play("xinchao.wav");
  delay(3000);
}

void waitAnswer()
{
  if(Serial.available() > 0)
  {
    answer = Serial.read();
    is_waiting_result = false;
    break;
  }
}