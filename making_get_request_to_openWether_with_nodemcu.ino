#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "ShwaTech";
const char* password = "12345678";
int LED = D0;

void setup () {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("connected successfully: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;  //Declare an object of class HTTPClient
    WiFiClient client;

    String apiKey = "";
    String cityID = "6413338";
    String units = "metric";
    String openWeatherMapUrl = "http://api.openweathermap.org/data/2.5/weather?id=" + cityID + "&units=" + units + "&appid=" + apiKey;

    http.begin(client, openWeatherMapUrl);  //Specify request destination
    int httpCode = http.GET();  //Send the request

    if (httpCode > 0) { //Check the returning code

      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload

      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
      }
      float temp = doc["main"]["temp"];
      int humidity = doc["main"]["humidity"];

      // Print values.
      Serial.print("Temperature: ");
      Serial.println(temp);
      Serial.print("Humidity: ");
      Serial.println(humidity);
    if (temp >= 25){
      digitalWrite(LED, HIGH);
    }
    }

    http.end();   //Close connection

  }

  delay(10000);    //Send a request every minute
}
