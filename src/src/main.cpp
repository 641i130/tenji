#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
//#include <WiFi.h>
//#include <HTTPClient.h>
//#include <math.h>

Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix();
int groupBuffer[16][8]; // group each pixel with this
int pixel[16][8]; // group each pixel with this
//const int group_count = 5;
const int group_count = 16;
const int loop_count = 1;
const int d = 10;
//const int d = 10; // d * gorup_count == longest wait time of pixel
//const char* ssid = "wifi";
//const char* =password = "password";

void printm(String toprint,int times) {
  /// Print Cleanly and long enough for the string to print without cutting off
  int ueoa = -(toprint.length()*8);
  for(int x = 0;x<times;x++) {
    for (int8_t x=8; x>=ueoa; x--) {
      matrix.clear();
      matrix.setCursor(x,0);
      matrix.print(toprint);
      matrix.writeDisplay();
      delay(20);
    }
  }
}

void spawnPixels() {
  // Used for spawning pixels at the top of the bar
  for (int i = 0; i < matrix.width(); i++) {
    for (int j = 0; j < matrix.height(); j++) {
      groupBuffer[i][j] = random(group_count); // 0 1 or 2
    }
  }
  for (int i = 0; i < matrix.width(); i++) {
    pixel[0][i] = 1; // 0 1 on or off
    /*
    for (int j = 0; j < matrix.height(); j++) {
      if (random(100) > 90) {
        pixel[i][j] = 1; // 0 1 on or off
      } else {
        pixel[i][j] = 0; // 0 1 on or off
      }
    }
    */
  }

}

void resetPixels() {
  for (int i = 0; i < matrix.width(); i++) {
    for (int j = 0; j < matrix.height(); j++) {
      groupBuffer[i][j] = random(group_count); // 0 1 or 2
    }
  }
  for (int i = 0; i < matrix.width(); i++) {
    for (int j = 0; j < matrix.height(); j++) {
      if (random(100) > 50) {
        pixel[i][j] = 1; // 0 1 on or off
      } else {
        pixel[i][j] = 0; // 0 1 on or off
      }
    }
  }
}

void randomMatrix(){
  for (int g = 0; g < group_count; g++) { // Loop through each group
    for (int i = 0; i < matrix.width(); i++) {
      for (int j = 0; j < matrix.height(); j++) {
        if (g == groupBuffer[i][j]) { // if the pixel is in the group, change the pixel!
          int val = random(2);
          matrix.drawPixel(i, j, val);
          pixel[i][j] = val;
          /*if (pixel[i][j] == 0) { // invert
            matrix.drawPixel(i, j, 1);
            pixel[i][j] = 1;
          } else {
            matrix.drawPixel(i, j, 0);
            pixel[i][j] = 0;
          }*/
        }
      }
    }
    delay(d); // This compounds over each group so 50*3 groups is how the longest pixels will be blinking
    matrix.writeDisplay();
    resetPixels();
  }
}
void killPixels() {
  for (int g = 0; g < group_count; g++) { // Loop through each group
  for (int i = 0; i < matrix.width(); i++) {
    for (int j = 0; j < matrix.height(); j++) {
      if (g == groupBuffer[i][j]) { // if the pixel is in the group, change the pixel!
      matrix.drawPixel(i, j, 0);
      pixel[i][j] = 0;
      delay(d);
      matrix.writeDisplay();
      }
    }
  }
  }
}

void wakePixels() {
  for (int i = 0; i < matrix.width(); i++) {
    for (int j = 0; j < matrix.height(); j++) {
      groupBuffer[i][j] = random(group_count); // 0 1 or 2
    }
  }
  for (int i = 0; i < matrix.width(); i++) {
    for (int j = 0; j < matrix.height(); j++) {
      if (random(100) > 50) {
        pixel[i][j] = 1; // 0 1 on or off
      } else {
        pixel[i][j] = 0; // 0 1 on or off
      }
    }
  }

  for (int g = 0; g < group_count; g++) { // Loop through each group
  for (int i = 0; i < matrix.width(); i++) {
    for (int j = 0; j < matrix.height(); j++) {
      if (g == groupBuffer[i][j]) { // if the pixel is in the group, change the pixel!
      int val = random(2);
      matrix.drawPixel(i, j, val);
      pixel[i][j] = val;
      delay(d);
      matrix.writeDisplay();
      }
    }
  }
  }
}

void fallingMatrix(){
  for (int l = 0; l < 8; l++) { // Loop through each group
  for (int g = 0; g < group_count; g++) { // Loop through each group
    for (int i = 0; i < matrix.width(); i++) {
      for (int j = 0; j < matrix.height(); j++) {
        if (g == groupBuffer[i][j]) { // if the pixel is in the group, change the pixel!
          if (pixel[i][j] == 1) { // if the pixel is on, move it
            matrix.drawPixel(i, j, 0);
            pixel[i][j] = 0;
            matrix.drawPixel(i+1, j, 1);
            pixel[i+1][j] = 1;
          }
        }
      }
      matrix.writeDisplay();
    }
  }
  }
  delay(d); // This compounds over each group so 50*3 groups is how the longest pixels will be blinking
  spawnPixels();
}


void circulate() {
  // Circulate the matrix
  matrix.drawPixel(0,0,1);
  matrix.writeDisplay();
  delay(50);
  for (int x = 0; x < matrix.width(); x++) {
    for (int y = 0; y < matrix.height(); y++) {
      matrix.drawPixel(x,y,0);
      matrix.drawPixel(x,y+1,1);
      matrix.writeDisplay();
      delay(50);
    }
    matrix.drawPixel(x,0,0);
    matrix.writeDisplay();
  }
}


void setup() {
  //while (!Serial);
  Serial.begin(9600);
  Serial.println("16x8 LED Mini Matrix Test");
  matrix.begin(0x70);  // pass in the address
  matrix.setBrightness(0);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.setRotation(3);
  /*
  HTTPClient http;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    printm("Connecting...",1);
  }
  Serial.println("Connected to the WiFi network");
  printm(WiFi.localIP().toString().c_str(),3); // Convert ip to string and print the ip of the device on boot
  // make request for data and show data here:
  */
  //resetPixels();
}

void loop() {
  while (Serial.available() == 0) {
    randomMatrix();
  }
  killPixels();
  printm(Serial.readString(),1);
  wakePixels();
  Serial.flush();
}
