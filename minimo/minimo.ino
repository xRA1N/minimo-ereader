//======================
//minimo-reader
//读取spiffs中的txt文件
//显示文件内容
//======================

#include <FS.h>
#include <Arduino.h>
#include <U8g2lib.h>
U8G2_ST7565_ERC12864_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ D2, /* dc=*/ D1, /* reset=*/ D9);

void CheckFS() {                //检查文件系统
  bool fsc = SPIFFS.begin();
  if (!fsc) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 20);
    u8g2.print( "SPIFFS文件系统故障" );
    u8g2.sendBuffer();
    ESP.deepSleep(30e6);
  }
}

void CheckFile() {          //检查文件
  bool file_exist = SPIFFS.exists("/txt");
  if (file_exist) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 20);
    u8g2.print( "找到txt文件" );
    u8g2.sendBuffer();
    delay(2000);
  }
  else {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 20);
    u8g2.print( "不存在txt文件" );
    u8g2.sendBuffer();//检查文件是否存在
    ESP.deepSleep(30e6);
  }

  File f = SPIFFS.open("/txt", "r");//检查文件是否可以打开
  if (!f) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 20);
    u8g2.print("txt文件无法打开" );
    u8g2.sendBuffer();
    ESP.deepSleep(30e6);
  }
  else {
    int txtfilesize = f.size();
    u8g2.clearBuffer();
    u8g2.setCursor(0, 20);
    u8g2.print( "txt文件长度" );
    u8g2.setCursor(0, 35);
    u8g2.print(txtfilesize);
    u8g2.setCursor(0, 50);
    u8g2.print("字节");
    u8g2.sendBuffer();
    delay(1300);
  }
  f.close();
}


void setup() {
  u8g2.begin();
  u8g2.enableUTF8Print();                                       //utf8支持
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);   //中文字体
  //u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font
  //Serial.begin(115200);
  SPIFFS.begin();
  CheckFS();            //检查文件系统
  CheckFile();        //检查文件
}


void loop() {
  File f = SPIFFS.open("/txt", "r");
  u8g2.clearBuffer();

  for (int i = 0; i < f.size(); i++) {
    u8g2.setCursor(i / 3 % 10 * 12, i / 27  % 5 * 12 + 12 );
    f.seek(i, SeekSet);
    u8g2.print(char(f.read()));
  }
  u8g2.sendBuffer();
  delay(1000);

}
