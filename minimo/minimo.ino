//======================
//minimo-reader
//读取spiffs中的txt文件
//显示文件内容
//======================

#include <FS.h>
#include <Arduino.h>
#include <U8g2lib.h>
U8G2_ST7565_ERC12864_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ D2, /* dc=*/ D1, /* reset=*/ D9);

int i ;       // 读取文件索引
int y ;       // 屏幕光标位置
int x ;       // 屏幕光标位置
char l;      //文件当前字符
int ihan ; //汉字字节计数


void showlogo() {
  u8g2.clearBuffer();
  u8g2.setCursor(10, 35);
  u8g2.setFont( u8g2_font_bubble_tr);   //中文字体
  u8g2.print( "MINIMO" );
  u8g2.sendBuffer();
  delay(1000);
  SPIFFS.end();
}
void CheckFS() {                //检查文件系统
  bool fsc = SPIFFS.begin();
  if (!fsc) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 15);
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);   //中文字体
    u8g2.print( "   SPIFFS文件系统故障" );
    u8g2.sendBuffer();
    SPIFFS.end();
    ESP.deepSleep(30e6);
  }
}

void CheckFile() {          //检查文件
  SPIFFS.begin();
  bool file_exist = SPIFFS.exists("/txt");
  if (file_exist) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 20);
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);   //中文字体
    u8g2.print( "找到txt文件" );
    u8g2.sendBuffer();
    delay(2000);
  }
  else {		//检查文件是否存在
    u8g2.clearBuffer();
    u8g2.setCursor(0, 20);
    u8g2.print( "不存在txt文件" );
    u8g2.sendBuffer();
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
  SPIFFS.end();
}

void plot_asc() {
  if (l == 10) {
    i = i + 1;
    x = 0;
    y = y + 12;
    u8g2.setCursor(0, y);
  }
  else {
    u8g2.setFont( u8g2_font_unifont_t_latin  );  // choose a suitable font
    u8g2.print(l);
    i = i + 1;
    x = x + 7;
  }

}

void plot_han() {
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);   //中文字体
  u8g2.print(l);
  i = i + 1;
  ihan = ihan + 1;
  if (ihan > 2) {
    ihan = 0;
    x = x + 12;
  }
}


void setup() {
  u8g2.begin();
  u8g2.enableUTF8Print();                                       //utf8支持
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);   //中文字体
  u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font
  Serial.begin(9600);
  //  SPIFFS.begin();
  showlogo();
  CheckFS();            //检查文件系统
  CheckFile();        //检查文件
  u8g2.clearBuffer();

}

void loop() {
  SPIFFS.begin();
  //  Serial.print("i =");
  //  Serial.println(i);
  File f = SPIFFS.open("/txt", "r");
  f.seek(i, SeekSet);
  l = f.read();

  u8g2.setCursor(x, y + 12 );
  if (l < 127) {
    plot_asc();
  }
  else {
    plot_han();
  }

  if (x > 128 - 12)
  {
    x = 0;
    y = y + 12;
  }
  if (y > 64 - 12)
  {
    u8g2.sendBuffer();
    y = 0;
    delay(5000);
    u8g2.clearBuffer();
  }
  if (i >= f.size()) {
    i = 0;
    u8g2.sendBuffer();
    delay(30e8);
    //    ESP.deepSleep(30e6);
  }

}
