// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
#include <vector> 
#include <algorithm>
using namespace std;

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);


vector<float> tts;
vector<float> hhs;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  pinMode(D5, OUTPUT);
  pinMode(D0, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D1, OUTPUT);

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  } else {
    tts.push_back(t); 
    hhs.push_back(h); 
  }

  // Humidité
  if (hhs.size() == 10) {
      Serial.print("Médiane ");
      Serial.print("Humidité");
      Serial.print(" : ");
      Serial.print(median(hhs));
      Serial.println(""); 
      
      Serial.print("Moyenne ");
      Serial.print("Humidité");
      Serial.print(" : ");
      Serial.print(average(hhs));
      Serial.println("");

      Serial.print("Tendance Humidité ");
      tendance(hhs, 80);
      hhs.erase(hhs.begin());
  }

  // Température
  if (tts.size() == 10) {
      Serial.print("Médiane ");
      Serial.print("Température");
      Serial.print(" : ");
      Serial.print(median(tts));
      Serial.println(""); 
      
      Serial.print("Moyenne ");
      Serial.print("Température");
      Serial.print(" : ");
      Serial.print(average(tts));
      Serial.println("");

      Serial.print("Tendance Température ");
      tendance(tts, 90);
      tts.erase(tts.begin());
  }
  
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}

float average(vector<float> vec) {
  float sum = 0;
  for (int i = 0; i < vec.size(); ++i) {
    sum += vec[i];
  }
  return sum / vec.size();
}
float median(vector<float> vec) {
  vector<float> v;
  v.swap(vec); 
  sort(v.begin(), v.end());
  return v[v.size() / 2];
}

void tendance(vector<float> vec, int qqc) {
  if ( vec[vec.size() - 1] > vec[(vec.size() - 2)] ) {
    Serial.print(" : true");
    qqc == 80 ? digitalWrite(D5, HIGH) : digitalWrite(D1, HIGH);
    qqc == 80 ? digitalWrite(D0, LOW) : digitalWrite(D2, LOW);
  }else if ( vec[vec.size() - 1] < vec[(vec.size() - 2)] ){
    Serial.print(" : false");
    qqc == 80 ? digitalWrite(D0, HIGH) : digitalWrite(D2, HIGH);
    qqc == 80 ? digitalWrite(D5, LOW) : digitalWrite(D1, LOW);
  } else {
    Serial.print(" : equal");
  }
  Serial.println(""); 
}
