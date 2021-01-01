#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define Relay1            D0

#define WLAN_SSID       "NETGEAR64"             // Your SSID
#define WLAN_PASS       "magicaltrail658"        // Your password

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL (Other value is 1883)
#define AIO_USERNAME    "ForsythCreations"            // Replace it with your username
#define AIO_KEY         "aio_tcUt86DlmghJOxC06Zq625Zd5IDj"   // Replace with your Project Auth Key

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/


// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Avatar State Controller"); // FeedName

void MQTT_connect();

String lastRead = "";

//------------------------- neopixel stuff -------------------------------

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define ARM_PIN    D6
#define HEAD_PIN    D8

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 39
#define NUMhead 60 // Popular NeoPixel ring size 40 - 4

// Declare our NeoPixel strip object:
Adafruit_NeoPixel arms(LED_COUNT, ARM_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel head(NUMhead, HEAD_PIN, NEO_GRB + NEO_KHZ800);

//--------------------------------------------------------------------------
void setup() {

  //neopixel stuff
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  arms.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  arms.show();            // Turn OFF all pixels ASAP
  head.begin();
  head.show();
  arms.setBrightness(20); // Set BRIGHTNESS to about 1/5 (max = 255)
  head.setBrightness(20);

  //
  Serial.begin(115200);

  pinMode(Relay1, OUTPUT);

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&Light1);
  //mqtt.publish("Avatar State Controller", "0");

  for (int i = 0; i < LED_COUNT; i++) { // For each pixel in strip...
    arms.setPixelColor(i, arms.Color(0, 0, 30));
    arms.show();
  }
  runArrow(0, 0, 30);

}

void loop() {
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &Light1) {
      Serial.print(F("Got: "));
      lastRead = (char *)Light1.lastread;
      Serial.println(lastRead);
      int Light1_State = atoi((char *)Light1.lastread);
      neoPixelHandler(Light1_State);
    }
  }
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");

}


void neoPixelHandler(int input)
{
  if (input == 0)
  {
    Serial.println("Off");
    digitalWrite(Relay1, HIGH);

    for (int i = LED_COUNT; i >= 0 ; i--) { // For each pixel in strip...
      arms.setPixelColor(i, arms.Color(0, 0, 30));
      arms.show();
      delay(20);
    }
    runArrow(0, 0, 30);
  }
  else if (input == 1)
  {
    Serial.println("On");
    digitalWrite(Relay1, LOW);

    for (int i = 0; i < LED_COUNT; i++) { // For each pixel in strip...
      arms.setPixelColor(i, arms.Color(50, 50, 50));
      arms.show();
      delay(20);
    }
    runArrow(50, 50, 50);
  }
}

void runArrow(int red, int green, int blue)
{
  Serial.println("");
  const int linesInArrow = 8;
  int arrowManager[linesInArrow] = {36, 5, 5, 4, 4, 3, 2, 1};
  int startLED = 0;
  int endLED = arrowManager[0];
  for (int k = 0; k < linesInArrow; k++) //for every line
  {
    if (k == 0)
    {
      for (int m = startLED; m < endLED; m++) { //for every LED in the given line
        head.setPixelColor(m, head.Color(red, green, blue));
        head.show();
        delay(20);
      }
    }
    else
    {
      for (int j = startLED; j < endLED; j++) { //for every LED in the given line
        //Serial.println(j);
        head.setPixelColor(j, head.Color(red, green, blue));
      }
    }
    //Serial.println("----------------------------------");
    head.show();
    delay(30);

    startLED = summer(arrowManager, 0, k);
    endLED = summer(arrowManager, 0, k + 1);
  }
}

int summer(int arrowArray[], int startIndex, int endIndex)
{
  int sum = 0;
  if (startIndex == endIndex)
  {
    return arrowArray[startIndex];
  }
  else
  {
    for (int i = startIndex; i <= endIndex; i++)
    {
      sum += arrowArray[i];
    }
  }
  return sum;
}
