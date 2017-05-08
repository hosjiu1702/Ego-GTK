  #include <SD.h>
  #define SD_ChipSelectPin 4
  #include <TMRpcm.h>
  #include <SPI.h>

  //#define __DEBUG__ __DEBUG__

#define SPEAKER_PIN 9

#define BUTTON_1 2
#define BUTTON_2 3
#define BUTTON_3 5
#define BUTTON_4 6
#define BUTTON_5 7
#define BUTTON_6 8

  TMRpcm arduino;
  byte value;   // gia tri global cua nut nhan
  unsigned char result;
  unsigned char index_music;

  unsigned char data[3];
  /*-----------------------------------------*/
  void setup() {
    Serial.begin(9600);
    init_button();
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
      char path[20];
      sprintf(path, "%d.wav", index_music);
      arduino.play(path);
      delay(3000);
      digitalWrite(SPEAKER_PIN, LOW);
    }
    else
    {
      arduino.play("sai.wav");
      delay(3000);
      digitalWrite(SPEAKER_PIN, LOW);
    }
}

  byte read_button()
  {
    if(digitalRead(BUTTON_1) == LOW)
      return 1;
    if(digitalRead(BUTTON_2) == LOW)
      return 2;
    if(digitalRead(BUTTON_3) == LOW)
      return 3;
    if(digitalRead(BUTTON_4) == LOW)
      return 4;
    if(digitalRead(BUTTON_5) == LOW)
      return 5;
    if(digitalRead(BUTTON_6) == LOW)
      return 6;
    
    return 0;
  }

  int send_button_value(byte button_value)
  {

    Serial.print(button_value); // DU LIEU duoc gui la ASCII
    return 1;
  }

  bool isPress()
  {
    byte button_value = read_button();
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
  arduino.speakerPin = 9;
  SD.begin(SD_ChipSelectPin);
}

void init_button()
{
    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_2, INPUT_PULLUP);
    pinMode(BUTTON_3, INPUT_PULLUP);
    pinMode(BUTTON_4, INPUT_PULLUP);
    pinMode(BUTTON_5, INPUT_PULLUP);
    pinMode(BUTTON_6, INPUT_PULLUP);
}













