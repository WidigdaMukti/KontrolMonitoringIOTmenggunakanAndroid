#include <FirebaseESP32.h>
#include <WiFi.h>
#include <DHT.h> // Install DHT11 Library and Adafruit Unified Sensor Library

#define FIREBASE_HOST "https://iot2023-7781a-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "bs71IlbiMJROiFvCc7si5QKNaptxruVGW1up3TEf"

#define WIFI_SSID "iPhone"
#define WIFI_PASSWORD "12345678"

#define DHTPIN 15 // Connect Data pin of DHT to D2
int led = 13; // Connect LED to D5

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;
FirebaseJson json;

void setup()
{
Serial.begin(9600);

dht.begin();
pinMode(led,OUTPUT);

WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("Connecting to Wi-Fi");
while (WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(300);
}
Serial.println();
Serial.print("Connected with IP: ");
Serial.println(WiFi.localIP());
Serial.println();

Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
Firebase.reconnectWiFi(true);
}

void sensorUpdate(){
// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
float h = dht.readHumidity();
// Read temperature as Celsius (the default)
float t = dht.readTemperature();
// Read temperature as Fahrenheit (isFahrenheit 57 = true)
float f = dht.readTemperature(true);

// Check if any reads failed and exit early (to try again).
if (isnan(h) || isnan(t) || isnan(f)) {
Serial.println(F("Failed to read from DHT sensor!"));
return;
}

Serial.print(F("Humidity: "));
Serial.print(h);
Serial.print(F("% Temperature: "));
Serial.print(t);
Serial.print(F("°C ,"));
Serial.print(f);
Serial.println(F("°F "));

if (Firebase.setFloat(firebaseData, "/sensor/suhu", t))
{
Serial.println("PASSED");
Serial.println("PATH: " + firebaseData.dataPath());
Serial.println("TYPE: " + firebaseData.dataType());
Serial.println("ETag: " + firebaseData.ETag());
Serial.println("------------------------------------");
Serial.println();
}
else
{
Serial.println("FAILED");
Serial.println("REASON: " + firebaseData.errorReason());
Serial.println("------------------------------------");
Serial.println();
}

if (Firebase.setFloat(firebaseData, "/sensor/kelembapan", h))
{
Serial.println("PASSED");
Serial.println("PATH: " + firebaseData.dataPath());
Serial.println("TYPE: " + firebaseData.dataType());
Serial.println("ETag: " + firebaseData.ETag());
Serial.println("------------------------------------");
Serial.println();
}
else
{
Serial.println("FAILED");
Serial.println("REASON: " + firebaseData.errorReason());
Serial.println("------------------------------------");
Serial.println();
}
}
void loop() {
sensorUpdate();

if (Firebase.getString(ledData, "/led")){
Serial.println(ledData.stringData());
if (ledData.stringData() == "1") {
digitalWrite(led, HIGH);
}
else if (ledData.stringData() == "0"){
digitalWrite(led, LOW);
}
}
delay(100);
}