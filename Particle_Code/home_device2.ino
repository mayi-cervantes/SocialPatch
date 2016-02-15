//home_device2
// Include Neopixel Library

// This #include statement was automatically added by the Particle IDE.
#include "neopixel/neopixel.h"

//homedevice_nop2 (photon)
// This #include statement was automatically added by the Particle IDE.
#include "neopixel/neopixel.h"

// --- NEOPIXEL DETAILS ---
// IMPORTANT: Set pixel, COUNT, PIN and TYPE
#define PIXEL_COUNT 16
#define PIXEL_PIN D2
#define PIXEL_TYPE WS2812

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// our waves app
// read data from data.qld.gov.au and visualise that data
#define LED_PIN D7

// variables for API

String hookdata = ""; // store data from the API in here before processing
String hookdata2 = "";
String hookdata3 = "";
String hookdata4 = "";
String hookdata5 = "";
String hookdata6 = "";

String lightVariables[5];
//***
//String cLight;
//float waveHeightData = 0; // put our wave height data in here when the webhook has been processed
unsigned long prevMillisGetData = 0; // when did we last get data so we knows when intervalGetData has passed
//const long intervalGetData = 240000; // the period of time between grabbing data from the API
const long intervalGetData = 60000; // the period of time between grabbing data from the API
unsigned long prevProcessDelay = 0; // when we want to start our pricessing delay so we know when to start processing
const long processDelay = 10000; // was 30000 before creates a time period to allow all of the data from the API to come through
bool getData = true; // a flad to know whether we are currently getting data or not

//Visitors
bool a_visitor = false;

void setup() 
{
    Serial.begin(9600); //starts up the serial communication
    Serial.println("color update:"); //just so we know it's alive
    
    pinMode(LED_PIN, OUTPUT);
    flashBuiltInLED(3);
    strip.begin();
    strip.show();
    
    //strip.begin();
    //strip.show(); // initializes all pixels to 'off'
    
    //Particle.subscribe("hook-response/device_lighthook", getSensorData, MY_DEVICES);
    //Particle.subscribe("hook-response/cgarden", getSensorData2, MY_DEVICES);
    /*Particle.subscribe("hook-response/soil_get", getSensorData1, MY_DEVICES);
    Particle.subscribe("hook-response/temperature_get", getSensorData2, MY_DEVICES); 
    Particle.subscribe("hook-response/humid_get", getSensorData3, MY_DEVICES);     
    Particle.subscribe("hook-response/lonely", getSensorData4, MY_DEVICES);  
    Particle.subscribe("hook-response/getcount", getSensorData5, MY_DEVICES);  */
    Particle.subscribe("hook-response/getAllData", getSensorData6, MY_DEVICES);
    // Lets give ourselves 10 seconds before we actually start the program.
    // That will just give us a chance to open the serial monitor before the program sends the request
    /*for(int i=0;i<10;i++) {
        Serial.println("waiting " + String(10-i) + " seconds before we publish");
        delay(1000);
    }*/
}

void loop() 
{
    
    //colorWipe(strip.Color(255,0,0), 50); //red
    //colorWipe(strip.Color(0, 255, 0), 50); //Green
    //colorWipe(strip.Color(0, 0, 255), 50); //Blue
    //strip.show();

    // new
    //strip.setPixelColor(1, 255, 0, 255);
    strip.setBrightness(64);
    strip.show();
    //
    
    unsigned long currentMillis = millis();
    
    if (((currentMillis - prevMillisGetData >= intervalGetData) && getData) || prevMillisGetData == 0)
    {
        //hookdata = "";
        Serial.println("getting data");
       // Particle.publish("device_lighthook"); // this is what causes the webhook to be run
        //Particle.publish("cgarden"); // this is what causes the webhook to be run
        /*Particle.publish("soil_get");
        Particle.publish("temperature_get");
        Particle.publish("humid_get");
        Particle.publish("getcount");
        Particle.publish("lonely");*/
        Particle.publish("getAllData");
        prevMillisGetData = currentMillis; // reset the flag for when we need to get data from the API
        prevProcessDelay = currentMillis; //set the flag for when we will process our data - then wait for data to come in
        getData = false; // a flag to prevent us getting more data when we haven't finished processing other data
    }
    
    if (currentMillis - prevProcessDelay >= processDelay && !getData)
    {
        //String dataValue = tryExtractData(hookdata, "\"Hsig\":"); // extracts the data we want from the API data
        //Serial.print("data value is: ");
        //waveHeightData = atof(dataValue); // the API returns data as a string - this converts it to a float
        //Serial.println(waveHeightData);
        getData = true;
        Serial.print("change ring color to ");
        /*getSoildata();
        delay(500);
        gettempvalue();
        delay(500);
        getHumidity();
        delay(500);
        getLonely();*/
        processAllData();
        delay(500);
    }
}

// fill the dots one after the other with a color

void getSoildata(){
        Serial.print("soil hookdata: ");
        Serial.println(hookdata);
        Serial.print("soil");
        
        if (hookdata == "YellowYellow" || hookdata == "Yellow")
        {
            RGB.control(true);
            strip.setPixelColor(12, 250, 255, 0); // yellow
            strip.setPixelColor(13, 250, 255, 0); // yellow
            strip.setPixelColor(14, 250, 255, 0); // yellow
            strip.setPixelColor(15, 250, 255, 0); // yellow
            Serial.println("yellow");
            Particle.publish("yellow", "unplugged", 60, PUBLIC);
        }
        else if (hookdata == "RedRed" || hookdata == "Red")
        {
            RGB.control(true);
            strip.setPixelColor(12, 255, 0, 0); // red
            strip.setPixelColor(13, 255, 0, 0); // red
            strip.setPixelColor(14, 255, 0, 0); // red
            strip.setPixelColor(15, 255, 0, 0); // red
            Serial.println("red");
            Particle.publish("red_light", "dry_soil", 60, PUBLIC);
            /*if (hookdata2 == "Green"){
                Particle.publish("red", "dry", 60, PUBLIC);
                //Serial.println("work!!");
            } */
        }
        else if (hookdata == "GreenGreen" || hookdata == "Green")
        {
            RGB.control(true);

            strip.setPixelColor(12, 0, 169, 0); // green
            strip.setPixelColor(13, 0, 169, 0); // green
            strip.setPixelColor(14, 0, 169, 0); // green
            strip.setPixelColor(15, 0, 169, 0); // green
            //strip.setBrightness(64);
            //strip.show();
            Serial.println("green");
            Particle.publish("green", "OK", 60, PUBLIC);
        }
        else if (hookdata == "BlueBlue" || hookdata == "Blue")
        {
            RGB.control(true);

            strip.setPixelColor(12, 0, 125, 255); // ocean blue
            strip.setPixelColor(13, 0, 125, 255); // ocean blue
            strip.setPixelColor(14, 0, 125, 255); // ocean blue
            strip.setPixelColor(15, 0, 125, 255); // ocean blue
            //strip.setBrightness(64);
            //strip.show();
            Serial.println("blue");
            Particle.publish("blue!", "overwatered", 60, PUBLIC);
            /*if (hookdata2 == "Green"){
                Particle.publish("blue", "flooded", 60, PUBLIC);*
                //Serial.println("work!!");
            } */
        }
         else 
        {
            //strip.setBrightness(64);
            Serial.println("finished reading");
            //strip.show();
            //Particle.publish("blue", NULL, 60, PUBLIC);
        }
        
        strip.setBrightness(64);
        strip.show();
        hookdata = ""; // clear out the data we have already collected
        //getData = true; // reset so we are ready to get new data
        //delay(1000);
        
    }
        
void gettempvalue(){
        // temp values
        
        Serial.print("temp: ");
        
        if (hookdata2 == "Yellow-Temp")
        {
            RGB.control(true);
            strip.setPixelColor(4, 250, 255, 0); // yellow
            strip.setPixelColor(5, 250, 255, 0); // yellow
            strip.setPixelColor(6, 250, 255, 0); // yellow
            strip.setPixelColor(7, 250, 255, 0); // yellow
            Serial.println("yellow");
            //Particle.publish("yellow", "unplugged", 60, PUBLIC);
        }
        else if (hookdata2 == "Red-Temp")
        {
            RGB.control(true);
            strip.setPixelColor(4, 255, 0, 0); // red
            strip.setPixelColor(5, 255, 0, 0); // red
            strip.setPixelColor(6, 255, 0, 0); // red
            strip.setPixelColor(7, 255, 0, 0); // red
            //strip.setBrightness(64);
            //strip.show();
            //Serial.println(hookdata2);
            Serial.println("red");
            //Particle.publish("red!", "help", 60, PUBLIC);
            /*if (hookdata2 == "Green"){
                Particle.publish("red", "dry", 60, PUBLIC);
                //Serial.println("work!!");
            } */
        }
        else if (hookdata2 == "Green-Temp")
        {
            RGB.control(true);
            strip.setPixelColor(4, 0, 169, 0); // green
            strip.setPixelColor(5, 0, 169, 0); // green
            strip.setPixelColor(6, 0, 169, 0); // green
            strip.setPixelColor(7, 0, 169, 0); // green
            //strip.setBrightness(64);
            //strip.show();
            Serial.println("green");
            //Particle.publish("green", "OK", 60, PUBLIC);
        }
         else 
        {
            //strip.setBrightness(64);
            Serial.println("finished reading");
            //strip.show();
            //Particle.publish("blue", NULL, 60, PUBLIC);
        }        
        
        strip.setBrightness(64);
        strip.show();
        hookdata2 = ""; // clear out the data we have already collected
        //
        getData = true; // reset so we are ready to get new data
        //delay(1000);
//        delay(15000);
        }
        
    void getHumidity(){    
        // humidity values
        Serial.print("humidity: ");
        
        if (hookdata3 == "Yellow-H")
        {
            RGB.control(true);
            strip.setPixelColor(0, 250, 255, 0); // yellow
            strip.setPixelColor(1, 250, 255, 0); // yellow
            strip.setPixelColor(2, 250, 255, 0); // yellow
            strip.setPixelColor(3, 250, 255, 0); // yellow
            Serial.println("yellow");
            //Particle.publish("yellow", "unplugged", 60, PUBLIC);
        }
        else if (hookdata3 == "Red-H")
        {
            RGB.control(true);
            strip.setPixelColor(0, 255, 0, 0); // red
            strip.setPixelColor(1, 255, 0, 0); // red
            strip.setPixelColor(2, 255, 0, 0); // red
            strip.setPixelColor(3, 255, 0, 0); // red
            Serial.print("red");

        }
        else if (hookdata3 == "Green-H")
        {
            RGB.control(true);
            strip.setPixelColor(0, 0, 169, 0); // green
            strip.setPixelColor(1, 0, 169, 0); // green
            strip.setPixelColor(2, 0, 169, 0); // green
            strip.setPixelColor(3, 0, 169, 0); // green
            //strip.setBrightness(64);
            //strip.show();
            Serial.println("green");
            //Particle.publish("green", "OK", 60, PUBLIC);
        }
         else 
        {
            //strip.setBrightness(64);
            Serial.println("finished reading");
            //strip.show();
            //Particle.publish("blue", NULL, 60, PUBLIC);
        }        
        
        strip.setBrightness(64);
        strip.show();
        hookdata3 = ""; // clear out the data we have already collected
        getData = true; // reset so we are ready to get new data
        //delay(1000);
//        delay(15000);
    }
    
    
    void getLonely(){
        // Lonely values
        //Serial.print
        //int hookdata50;
        //hookdata50 = hookdata5.toInt();
        Serial.print("Lonely hookdata: ");
        
        Serial.println(hookdata4);
        
        Serial.print("lonely: ");
        
        if (hookdata4 == "Green_Friend"){
            RGB.control(true);
            strip.setPixelColor(8, 255,0,55); // pink
            strip.setPixelColor(9, 255,0,55); // pink
            strip.setPixelColor(10, 255,0,55); // pink
            strip.setPixelColor(11, 255,0,55); // pink
            Serial.println("pink");
            Particle.publish("Green_Friend", "visited", 60, PUBLIC);            
        }
        else{
            
            if(hookdata5 == "YGreen" || hookdata5 == "Y" ){
            Serial.print("hookdata5: ");
            Serial.println(hookdata5);
            RGB.control(true);    
            strip.setPixelColor(8, 0, 169, 0); // green
            strip.setPixelColor(9, 0, 169, 0); // green
            strip.setPixelColor(10, 0, 169, 0); // green
            strip.setPixelColor(11, 0, 169, 0); // green
            Serial.println("green");
            }
            else{
            RGB.control(true);    
            strip.setPixelColor(8, 0, 255, 255); // cyan
            strip.setPixelColor(9,  0, 255, 255); // cyan
            strip.setPixelColor(10,  0, 255, 255); // cyan
            strip.setPixelColor(11, 0, 255, 255); // cyan
            Serial.println("cyan");
            Particle.publish("Cyan", "lonely", 60, PUBLIC);                
        } 
        }
        strip.setBrightness(64);
        strip.show();
        hookdata4 = ""; // clear out the data we have already collected
        hookdata5 = ""; // clear out the data we have already collected
        getData = true; // reset so we are ready to get new data
        //delay(1000);
    }
    
void processAllData()
{
    
    int number_of_variables = 5;
    int start_pos = 0;
    
    for (int i=0; i < number_of_variables; i++){
        int break_pos = hookdata6.indexOf(",", start_pos);
        lightVariables[i] = hookdata6.substring(start_pos, break_pos);
        start_pos = break_pos + 1;
    }
    int z;
    for (z=0; z < number_of_variables; z++){
        Serial.println(lightVariables[z]);
    }
    
    hookdata = lightVariables[0];
    hookdata2 = lightVariables[1];
    hookdata3 = lightVariables[2];
    hookdata4 = lightVariables[3];
    hookdata5 = lightVariables[4];
    
    Serial.println("***********");
    Serial.print("hookdata soil :");
    Serial.println(hookdata);
    Serial.print("hookdata2 temp :");
    Serial.println(hookdata2);
    Serial.print("hookdata3 humidity :");
    Serial.println(hookdata3);
    Serial.print("hookdata4 PIR :");
    Serial.println(hookdata4);
    Serial.print("hookdata5 visitor :");
    Serial.println(hookdata5);
    
    hookdata6 = "";
    
    changeDisplay();
    
}

void changeDisplay()
{
    getSoildata();
    delay(500);
    gettempvalue();
    delay(500);
    getHumidity();
    delay(500);
    getLonely();
}






/*
    --- flashes the built in LED a number of times ----
    this is blocking code but it's okay for this purpose
    
*/

void flashBuiltInLED(int number_of_times)
{
    for (int i=0; i<number_of_times; i++)
    {
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        Serial.println("loading color value");
        digitalWrite(LED_PIN, LOW);
        delay(100);
    }
}

// gets the data from the webhook - it comes through in small chunks so we need to concatenate it

void getSensorData1 (const char *name, const char *data)
{
    hookdata += String(data);
    //
    Serial.println(hookdata);
}

void getSensorData2 (const char *name, const char *data)
{
    hookdata2 += String(data);
    //
    Serial.println(hookdata2);
}

void getSensorData3 (const char *name, const char *data)
{
    hookdata3 += String(data);
    //
    Serial.println(hookdata3);
}

void getSensorData4 (const char *name, const char *data)
{
    hookdata4 += String(data);
    //
    Serial.println(hookdata4);
}

void getSensorData5 (const char *name,const char *data)
{
    hookdata5 += String(data);
    //
    Serial.println(hookdata5);
}

void getSensorData6 (const char *name,const char *data)
{
    hookdata6 += String(data);
    //
    Serial.println(hookdata6);
}


void colorWipe(uint32_t c, uint8_t wait){
    for(uint16_t i=0; i<strip.numPixels(); i++)
    {
        strip.setPixelColor(i,c);
        strip.show();
        delay(wait);
    }
}
