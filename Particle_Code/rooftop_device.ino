// Rooftop device
// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_DHT/Adafruit_DHT.h"

// This #include statement was automatically added by the Particle IDE.
#include "neopixel/neopixel.h"

// rooftop device (photon)


// IMPORTANT: Set pixel, COUNT, PIN and TYPE
#define PIXEL_COUNT 8
#define PIXEL_PIN D2
#define PIXEL_TYPE WS2812

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

#define LED_PIN D7

// ------------------ Infrared ---------------------
// PIR: Passive InfraRed
#define PIRPIN A1
bool visitor = FALSE;
unsigned long visitor_previousMillis = 0;
const long visitor_publishInterval = 10000;

// ------------------ Soil sensor values ------------

int val = 0;//variable to store soil value
int ldrPin = A2;//Declare a variable for the soil moisture sensor 
int soilPower = D6;//Variable for Soil moisture Power
int ldrValue;
//Rather than powering the sensor through the V-USB or 3.3V pins, 
//we'll use a digital pin to power the sensor. This will 
//prevent oxidation of the sensor as it sits in the corrosive soil. 


// --- Temperature & Humidty
#define DHTPIN A5

#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

double temp;
double humidity;

String tempS;
String humidityS;

double tReading;
double hReading;

// --------------- To create non-blocking functions

unsigned long previousMillis = 0;
const long publishInterval = 3000;

//------------------ To publisH into the cloud
String eventType;

// ----------------- To get the color of the light 
String colorLight;
String colorLight1;
String colorLight2;
String colorLight3;
String colorLight4;


// ------------------ To use the build-in LED
int infoPin = D7;
int infoPin_State = LOW;
int flashes = 0;

// ------------------ To Publishing
String payload;
String thisitempayload;
String pubMessage;
int eventid = 0;
bool success;

// What is this for?
int inbuiltled = D7;

// ------------------ To Calibrate Sensor
int baselevel = 0;

void setup() 
{
    Serial.begin(9600);   // open serial over USB
    //Particle.variable("color lights", cLight, STRING);    
    //pinMode(inbuiltled, OUTPUT);
    pinMode(PIRPIN, INPUT);
    pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
    digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
    calibrate();
    strip.begin();
    strip.show(); // initializes all pixels to 'off'    
}

void loop() {
  // and the distance result in inches and centimeters:

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= publishInterval)
    {
        eventType = "update";
        getSoilReading();
        previousMillis = currentMillis;
        //getSoilReading();
        delay(200);
        getTempReading();
        delay(200);
        getHumid();
        delay(200);
        lonleyget();
        delay(200);
        Particle.publish("getSoildata");        
    }
    //This time is used so you can test the sensor and see it change in real-time.

    /*byte state = digitalRead(PIRPIN);
    unsigned long visitor_currentMillis = millis();
    if (visitor_currentMillis - visitor_previousMillis >= visitor_publishInterval && state == 1 && !visitor) {
        visitor = TRUE;
        colorLight3 = "Green Friend";
        Serial.println("new visitor, happy days!");
    
        pubMessage = "1";
        thisitempayload = createEventString(pubMessage,"New_Visitor",colorLight3);
        publishData();
        
        visitor_previousMillis = millis();
    }
    else if (visitor_currentMillis - visitor_previousMillis >= visitor_publishInterval && state == 0 && visitor)
    {
        visitor = FALSE;
        colorLight3 = "Pink";
        Serial.println("no visitors, :(");
        thisitempayload = createEventString(pubMessage,"New_Visitor",colorLight3);
        publishData();
        visitor_previousMillis = millis();
    }
    delay(1000);*/
}

void lonleyget()
{
    byte state = digitalRead(PIRPIN);
    
    if(state == 1 && !visitor)
    {
        visitor = true;
        colorLight3 = "Green_Friend";
        Serial.println("new visitor, happy days!");
        pubMessage = "1";
        thisitempayload = createEventString(pubMessage,"New_Visitor",colorLight3);
        publishData();
    }
    else if (state == 0 && visitor)
    {
        visitor = false;
        colorLight3 = "Cyan";
        Serial.println("no visitors :(");
        pubMessage = "0";
        thisitempayload = createEventString(pubMessage,"New_Visitor",colorLight3);
        publishData();
    }
    
    /*
    unsigned long visitor_currentMillis = millis();
    if (visitor_currentMillis - visitor_previousMillis >= visitor_publishInterval && state == 1 /*&& !visitor*///) {
        //visitor = TRUE;
        //visitor = 1;
        //colorLight3 = "Green_Friend";
        //Serial.println("new visitor, happy days!");
        //pubMessage = "1";
        //thisitempayload = createEventString(pubMessage,"New_Visitor",colorLight3);
        //publishData();
        
        //visitor_previousMillis = millis();
    //}
    /*else if (visitor_currentMillis - visitor_previousMillis >= visitor_publishInterval && state == 0 && visitor)
    {
        visitor = TRUE;
        colorLight3 = "Cyan";
        Serial.println("A visitor just arrived");
        pubMessage = "1";
        thisitempayload = createEventString(pubMessage,"New_Visitor",colorLight3);
        publishData();
        visitor_previousMillis = millis();
    } */ 
    
    //else if (visitor_currentMillis - visitor_previousMillis >= visitor_publishInterval && state == 0 /*&& visitor*/)
    
    //{
        //visitor = FALSE;
        //visitor = 0;
        //colorLight3 = "Cyan";
        //Serial.println("no visitors :(");
        //pubMessage = "0";
        //thisitempayload = createEventString(pubMessage,"New_Visitor",colorLight3);
        //publishData();
        //visitor_previousMillis = millis();
    //}
    
}

void getSoilReading()
{
    // ------------- Soil Reading ----------------//
    int currentSoilReading = readSoil();

    Serial.println(currentSoilReading);
    ldrValue = analogRead(ldrPin);
    //If your soil is too dry, turn on Red LED to notify you
    //This value will vary depending on your soil and plant
    if(currentSoilReading < 50)
    {
      // take control of the RGB LED
      RGB.control(true);
      //RGB.color(232, 134, 43);//set RGB LED to orange
      strip.setPixelColor(0, 255, 254, 36); // yellow
      strip.setPixelColor(1, 255, 254, 36); // yellow
      strip.setPixelColor(2, 255, 254, 36); // yellow
      strip.setPixelColor(3, 255, 254, 36); // yellow
      strip.setPixelColor(4, 255, 254, 36); // yellow
      strip.setPixelColor(5, 255, 254, 36); // yellow
      strip.setPixelColor(6, 255, 254, 36); // yellow
      strip.setPixelColor(7, 255, 254, 36); // yellow
      colorLight1 = "Yellow";
      //cLight = &colorLight[0];
      Serial.println("Sensor could be unplugged");
      Serial.println(colorLight1);
      Particle.publish("yellow_light", "sensor_unplugged", 60, PUBLIC);
    } else if (currentSoilReading < 1800) {
      // take control of the RGB LED
      RGB.control(true);
      //RGB.color(232, 134, 43);//set RGB LED to red
      strip.setPixelColor(0, 255, 0, 0); // red
      strip.setPixelColor(1, 255, 0, 0); // red
      strip.setPixelColor(2, 255, 0, 0); // red
      strip.setPixelColor(3, 255, 0, 0); // red
      strip.setPixelColor(4, 255, 0, 0); // red
      strip.setPixelColor(5, 255, 0, 0); // red
      strip.setPixelColor(6, 255, 0, 0); // red
      strip.setPixelColor(7, 255, 0, 0); // red
      colorLight1 = "Red";
      //cLight = &colorLight[0];
      Serial.println("This plant needs water!");
      Serial.println(colorLight1);
      Particle.publish("red_light", "dry_soil", 60, PUBLIC);
    }
    else if (currentSoilReading > 1800 && currentSoilReading < 3250) {
      // take control of the RGB LED
      RGB.control(true);
      //RGB.color(0, 255, 0);//set RGB LED to green
      strip.setPixelColor(0, 0, 169, 0); // green
      strip.setPixelColor(1, 0, 169, 0); // green
      strip.setPixelColor(2, 0, 169, 0); // green
      strip.setPixelColor(3, 0, 169, 0); // green
      strip.setPixelColor(4, 0, 169, 0); // green
      strip.setPixelColor(5, 0, 169, 0); // green
      strip.setPixelColor(6, 0, 169, 0); // green
      strip.setPixelColor(7, 0, 169, 0); // green
      colorLight1 = "Green";
      //cLight = &colorLight[0];
      //colorLight1[] = "Green";
      Serial.println("This is a happy plant");
      Serial.println(colorLight1);
      Particle.publish("green_light", "healthy", 60, PUBLIC);
    } else if (currentSoilReading > 3250) {
      // resume normal operation
      RGB.control(true);
      //RGB.color(0, 0, 255);//set RGB LED to blue
      strip.setPixelColor(0, 0, 0, 255); // blue
      strip.setPixelColor(1, 0, 0, 255); // blue
      strip.setPixelColor(2, 0, 0, 255); // blue
      strip.setPixelColor(3, 0, 0, 255); // blue
      strip.setPixelColor(4, 0, 0, 255); // blue
      strip.setPixelColor(5, 0, 0, 255); // blue
      strip.setPixelColor(6, 0, 0, 255); // blue
      strip.setPixelColor(7, 0, 0, 255); // blue
      colorLight1 = "Blue";
      //cLight = &colorLight[0];
      Serial.println("This zone is flooded");
      Serial.println(colorLight1);
      Particle.publish("blue_light", "overwatered", 60, PUBLIC);
    }
    strip.show();
    publishSensorValue();    
    //colorLight = colorLight1;
}

void getTempReading()
{
    tReading = (double) dht.getTempCelcius();
    tempS = String(tReading);

    Serial.println(tReading);
//This value will vary depending on your soil and plant
    if(tReading < 10)
    {
      // take control of the RGB LED
      RGB.control(true);
      //RGB.color(232, 134, 43);//set RGB LED to orange
      strip.setPixelColor(0, 255, 254, 36); // yellow
      strip.setPixelColor(1, 255, 254, 36); // yellow
      strip.setPixelColor(2, 255, 254, 36); // yellow
      strip.setPixelColor(3, 255, 254, 36); // yellow
      strip.setPixelColor(4, 255, 254, 36); // yellow
      strip.setPixelColor(5, 255, 254, 36); // yellow
      strip.setPixelColor(6, 255, 254, 36); // yellow
      strip.setPixelColor(7, 255, 254, 36); // yellow
      colorLight2 = "Yellow-Temp";
      //cLight = &colorLight[0];
      Serial.println("Sensor could be unplugged");
      Serial.println(colorLight2);
    } else if (tReading > 35) {
      colorLight2 = "Red-Temp";
      Serial.println("What a hot day!");
      Serial.println(colorLight2);
    }
    else if (tReading < 35 && tReading > 10) {
      colorLight2 = "Green-Temp";
      //cLight = &colorLight[0];
      //colorLight1[] = "Green";
      Serial.println("This is a happy plant");
      Serial.println(colorLight2);
    } 
    //colorLight = colorLight1;  
    strip.show();
    publishSensorValue123();    
    //colorLight = colorLight1; 
}

void getHumid()
{
    hReading = (double) dht.getHumidity();
    humidityS = String (hReading);
    
    // humidity readings: 0-100% humidity readings with 2-5% accuracy
    if(hReading < 5)
    {
      colorLight4 = "Yellow-H";
      Serial.println("Sensor could be unplugged");
      Serial.println(colorLight4);
    } else if (hReading > 80) {
      // take control of the RGB LED
      colorLight4 = "Red-H";
      //cLight = &colorLight[0];
      Serial.println("The humidity is too high today!");
      Serial.println(colorLight4);
    } else if (hReading > 10 && hReading < 80) {
      colorLight4 = "Green-H";
      Serial.println("This is a happy plant");
      Serial.println(colorLight4);
    }
    publishSensorValue456();    
    //colorLight = colorLight1;
    
    
}


void publishSensorValue()
{
    Serial.print(eventType);
    Serial.print(" : ");
    Serial.println(ldrValue);
    Serial.println(colorLight1);
    
    String pubMessage = String(ldrValue);
    
    eventid++;
    
    // this is the section we need to edit for the multiple publishing messages
    thisitempayload = createEventString(pubMessage,"SoilMoistureReading",colorLight1);
    //success = Particle.publish(eventType, pubMessage, 60, PRIVATE);
    publishData();
}

// publish the data to the database via the webhook
// data is put into format using the function createEventString
// createEventString must be used to format data before publishing

void publishSensorValue123()
{
    Serial.print(eventType);
    Serial.print(" : ");
    Serial.println(tReading);
    Serial.println(colorLight2);
    
    String pubMessage = String(tReading);
    
    eventid++;
    
    thisitempayload = createEventString(tempS,"Temperature Celcius", colorLight2);
    //Particle.publish("Read_Datahook", thisitempayload, 60, PUBLIC);
    delay(500);
    // this is the section we need to edit for the multiple publishing messages
   // thisitempayload = createEventString(pubMessage,"SoilMoistureReading",colorLight1);
    //success = Particle.publish(eventType, pubMessage, 60, PRIVATE);
    publishData();
}

void publishSensorValue456()
{
    Serial.print(eventType);
    Serial.print(" : ");
    Serial.println(hReading);
    Serial.println(colorLight4);
    
    String pubMessage = String(hReading);
    
    eventid++;
    

    thisitempayload = createEventString(humidityS,"Humidity", colorLight4);
    //Particle.publish("Read_Datahook", thisitempayload, 60, PUBLIC);
    delay(500);
    // this is the section we need to edit for the multiple publishing messages
   // thisitempayload = createEventString(pubMessage,"SoilMoistureReading",colorLight1);
    //success = Particle.publish(eventType, pubMessage, 60, PRIVATE);
    publishData();
}


void publishData()
{
    success = Particle.publish("Read_Datahook", thisitempayload, 60, PUBLIC);
    
    if(!success)
    {
        flashes = 4;
    }
    else
    {
        flashes = 2;
    }
    flashBuiltInLed();
}

//This is a function used to get the soil moisture content
int readSoil()
{
    digitalWrite(soilPower, HIGH);//turn D6 "On"
    delay(5000);//wait 1 second 
    val = analogRead(ldrPin);
    digitalWrite(soilPower, LOW);//turn D6 "Off"
    return val;
}

void calibrate()
{
    int cal_reading =0;
    for (int i = 0; i < 10; i++) 
    {
        cal_reading += readSoil();
        delay(300);
    }    
    baselevel = cal_reading/10;
    Serial.print("baselevel is ");
    Serial.println(baselevel);
    digitalWrite(inbuiltled, HIGH);
    delay(1000);
    digitalWrite(inbuiltled, LOW);
}

void flashBuiltInLed()
{
    //int j=0;
    
    for (int i=0; i<flashes*2; i++);
    {
        Serial.print("new data reading : ");
        //Serial.println(j);
        infoPin_State = !infoPin_State;
        digitalWrite(infoPin, infoPin_State);
        delay(300);
        //j++;
    }
}


String createEventString(String publishvalue, String variabletype, String colorLight)
{
    payload = "{\"thisdevice\":\"cowboy_zombie\", \"Type\":\"";
    payload += variabletype;
    payload += "\",\"Readings\":\"";
    payload += publishvalue;
    payload += "\",\"light\":\"";
    payload += String(colorLight);
    payload += "\",\"eventID\":\"";
    payload += String(eventid);
    payload += "\"}";
    
    Serial.println(payload);
    return payload;
}

