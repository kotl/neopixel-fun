//
// Copyright 2018 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseNeoPixel is a sample that demonstrates how
// to set pixel data from Firebase.
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

#include <NeoPixelBrightnessBus.h>

const uint16_t PixelCount = 10; // make sure to set this to the number of pixels in your strip
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(PixelCount, PixelPin);

#include "connect.h"

//#define USE_STREAMING

void setup() {
  Serial.begin(115200);

  strip.Begin();
  strip.SetBrightness(100); // 0 ... 255
  strip.Show(); // Initialize all pixels to 'off'

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

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  setPixels();
#ifdef USE_STREAMING
  Firebase.stream("/rgbdata");
#endif
}

int StrToHex(const char* str)
{
  return (int) strtol(str, 0, 16);
}

void setPixels() {
  FirebaseObject pixels = Firebase.get("/rgbdata");
  if (Firebase.failed()) {
    Serial.println("Firebase get failed");
    Serial.println(Firebase.error());
    return;
  }

  for (int i = 0; i < PixelCount; i++) {
    String pixelColor = pixels.getString(String("p") + i);
    RgbColor color = HtmlColor(StrToHex(pixelColor.c_str()));
    if (color != strip.GetPixelColor(i)) {
      strip.SetPixelColor(i, color);
      Serial.printf("Set pixel %d, pixelColor: %s\n", i, pixelColor.c_str());
      delay(500);
      strip.Show();
    }
  }
  delay(1000);
}

void loop() {
#ifdef USE_STREAMING
    stream();
#else
   setPixels();
#endif
}

void stream() {
  if (Firebase.failed()) {
    Serial.println("streaming error");
    Serial.println(Firebase.error());
  }
  if (Firebase.available()) {
    FirebaseObject event = Firebase.readEvent();
    String eventType = event.getString("type");
    eventType.toLowerCase();
    if (eventType == "put") {
      String path = event.getString("path").substring(2);
      int index = StrToHex(path.c_str());
      String data = event.getString("data");
      if (data.length() > 0) {
        Serial.printf("path: %s, index: '%d', data: '%s'\n", path.c_str(), index, data.c_str());
        strip.SetPixelColor(index, HtmlColor(StrToHex(data.c_str())));
      }
    }
  }
  strip.Show();
  delay(200);
}

