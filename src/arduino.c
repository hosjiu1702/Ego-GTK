  #include <SD.h>
  #define SD_ChipSelectPin 4
  #include <TMRpcm.h>
  #include <SPI.h>

  //#define __DEBUG__ __DEBUG__

  #define SPEAKER_PIN 9

  #define ANALOG_PIN A0

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

  TMRpcm speaker;
  byte value;   // gia tri global cua nut nhan
  unsigned char result;
  unsigned char index_music;

  unsigned char data[3];
  /*-----------------------------------------*/
  void setup() {
    Serial.begin(9600);
    //Serial5.begin(9600);
    pinMode(ANALOG_PIN, INPUT);

    /*Ham nay tam thoi bi xoa trong qua trinh phat trien - THEM VAO SAU*/
    init_module_SD_Card();
  }


  /*---------------------------------*/
  void loop() {

    /*Khi chua nhan duoc tin hieu thi pause*/
    while(Serial.read() != 'o'){}

    /*Cho user nhan nut*/
    while(!isPress()) 
    {
    }

    /*Neu nut duoc nhan roi thi gui gia tri di*/
    send_button_value(value);
    Serial.flush();

    /*Khi chua nhan duoc du lieu thi pause*/
    while(!Serial.available()){}

    /*Neu cp du lieu den thi "get" */
      unsigned long thoi_gian_khoi_dau = millis();
    byte i = 0;
    while( (millis() - thoi_gian_khoi_dau ) < 10 )
    {
      if(Serial.available())
      {
        data[i++] = Serial.read();
      }
    }

    Serial.flush();

    result = data[0] - 48;
    index_music = (data[1] - 48)*10 + (data[2] - 48)*1;

    #ifdef __DEBUG__
      Serial.println(result);
      Serial.println(index_music);
    #endif

    /*Phat nhac (dung - sai)*/
    if(result)
    {
      speaker.play("dung.wav");
      delay(2000);
      digitalWrite(speaker.speakerPin, LOW);
    }
    else
    {
      speaker.play("sai.wav");
      delay(2000);
      digitalWrite(speaker.speakerPin, LOW);
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

void init_module_SD_Card()
{
  /*DUNG DE KHOI TAO CHO VIEC PHAT AM THANH*/
  speaker.speakerPin = 9;
  SD.begin(SD_ChipSelectPin);
}















