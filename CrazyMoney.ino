#include <GyverGFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

String c = "На яхту";
// #define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS 15 // Chip select control pin
#define TFT_DC 2 // Data Command control pin
#define TFT_RST 4 // Reset pin (could connect to RST pin)
// #define TFT_RST -1 // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST

#include "TimesNRCyr6.h"
#include "TimesNRCyr8.h"

//Adafruit_ST7735 ast = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// пример наследования в класс. Нужно реализовать методы dot и update
class ST7735 : public Adafruit_ST7735{ // public GyverGFX, 
public:
  ST7735(int CS, int DC, int RST) : Adafruit_ST7735(CS, DC, RST) {} // GyverGFX(160, 128), 
  void print_glif(const uint16_t 	glif[]) {
    //print((String)getCursorX() + " " + (String)getCursorY());
    drawRGBBitmap(getCursorX(), getCursorY()-8, glif, 8, 9);
    setCursor(getCursorX()+8, getCursorY());
  }
  /*void dot(int x, int y, uint8_t fill = GFX_FILL) {
    drawPixel(x, y, fill);
  }
  void update() {
    // ...
  }*/
};

ST7735 tft(TFT_CS, TFT_DC, TFT_RST);

#include "icons.h"


int v = 1000;

float cou[] = {
  1, 
  101.59, 
  98.32,
  2559406,
  0.000719
};

int xv, yv, xc, yc;

String FtoString(float f, int n = 7) {
  //int64_t ff=f*10000000;
  for(int i=0;i<=n;i++) {
    if(int(int64_t(f*10000000)%10000000)!=0) return String(f, 7-i);
  }
  return String(f, 0);
}

void print_fone() {
  tft.fillRect(0, 0, 160, 16, ST77XX_GREEN);
  tft.setFont(&TimesNRCyr8pt8b);
  tft.setCursor(2, 2+10);
  tft.println(c);
  tft.setFont(&TimesNRCyr6pt8b);
  xv = tft.getCursorX();
  yv = tft.getCursorY();

  tft.fillRect(0, 64, 160, 16, ST77XX_GREEN);
  tft.setFont(&TimesNRCyr8pt8b);
  tft.setCursor(2, 2+10+64);
  tft.println("Курс");
  tft.setFont(&TimesNRCyr6pt8b);
  xc = tft.getCursorX();
  yc = tft.getCursorY();
  tft.setCursor(xv, yv);
}

void update_data_disp() {
  tft.setCursor(xv, yv);
  tft.print(String(float(v*cou[0]),0));
  tft.print_glif(r);
  tft.setCursor(160-(String(float(v/cou[1]),2).length()+1)*8, tft.getCursorY());
  tft.print(String(float(v/cou[1]),2));
  tft.print_glif(r);
  tft.setCursor((160-(String(float(v/cou[2]),2).length()+1)*8)/2, tft.getCursorY());
  tft.print(String(float(v/cou[2]),2));
  tft.print_glif(r);
  tft.println();
  tft.setCursor((160-(String(float(v/cou[4]),2).length()+1)*8)/2, tft.getCursorY());
  tft.print(String(float(v/cou[4]),2));
  tft.print_glif(r);
  tft.println();
  tft.setCursor((160-(String(float(v/cou[3]),7).length()+1)*8)/2, tft.getCursorY());
  tft.print(String(float(v/cou[3]),7));
  tft.print_glif(r);

  tft.setCursor(xc, yc);

  tft.print(String(cou[0],0));
  tft.print_glif(r);
  tft.setCursor(160-(String(cou[1],2).length()+1)*8, tft.getCursorY());
  tft.print(String(cou[1],2));
  tft.print_glif(r);
  tft.setCursor((160-(String(cou[2],2).length()+1)*8)/2, tft.getCursorY());
  tft.print(String(cou[2],2));
  tft.print_glif(r);
  tft.println();
  tft.setCursor((160-(String(cou[4],7).length()+1)*8)/2, tft.getCursorY());
  tft.print(String(cou[4],7));
  tft.print_glif(r);
  tft.println();
  tft.setCursor((160-(String(cou[3],0).length()+1)*8)/2, tft.getCursorY());
  tft.print(String(cou[3],0));
  tft.print_glif(r);
}

void setup(void) {  
  Serial.begin(115200);
  Serial.println("\n\n Using the PNGdec library");
  Serial.println(FtoString(float(1000.123)));
  Serial.println(FtoString(1000.123));
  Serial.println(FtoString(1000+0.0));
  Serial.println(float(1000));
  Serial.println(1000);
  Serial.println(1000+0.0);
  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab

  Serial.println(F("Initialized"));


  uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);

  //delay(500);
  // large block of text
  //ast.fillScreen(ST77XX_BLACK);
  //ast.cp437(true);
  tft.setRotation(1);

  tft.setFont(&TimesNRCyr6pt8b); // выбор шрифта

  tft.setCursor(0, 0+8);  
  print_fone();
  //tft.println("Съешь еще этих маленьких булочек и выпей чаю");
  
  update_data_disp();

  //tft.println("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ");
  //delay(1000);
  //tft.print(char(₽));
}

void loop() {
  //tft.drawRGBBitmap(0, 0, r, 8, 9);
}
