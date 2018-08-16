// FirebaseNeoPixel is a sample that demonstrates how
// to set pixel data from Firebase.
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

#include <NeoPixelBrightnessBus.h>

const uint16_t PixelCount = 10; // make sure to set this to the number of pixels in your strip
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(PixelCount, PixelPin);

#include "connect.h"

void setup() {
  Serial.begin(115200);

  // TODO: initialize pixel strip
  
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  // TODO: connect to Firebase
  setPixels();
}

int StrToHex(const char* str)
{
  return (int) strtol(str, 0, 16);
}

void setPixels() {
  // TODO: get /rgbdata pixels object from Firebase
  if (Firebase.failed()) {
    Serial.println("Firebase get failed");
    Serial.println(Firebase.error());
    return;
  }

  for (int i = 0; i < PixelCount; i++) {
    // TODO: get string color from a pixel

    // TODO: transform hex string into a color value

    // TODO: set pixel color only if it's different from current one
    // and display strip with a small delay
  }
  delay(1000);
}

void loop() {
   setPixels();
}
