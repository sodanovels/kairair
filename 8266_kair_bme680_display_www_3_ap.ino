/**
 * WiFiManager advanced demo, contains advanced configurartion options
 * Implements TRIGGEN_PIN button press, press for ondemand configportal, hold for 3 seconds for reset settings.
 */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#define TRIGGER_PIN 12                         

// wifimanager can run in a blocking mode or a non blocking mode
// Be sure to know how to process loops with no delay() if using non blocking
bool wm_nonblocking = false; // change to true to use non blocking

WiFiManager wm; // global wm instance
WiFiManagerParameter custom_field; // global param ( for non blocking w params )

#include <ESP8266WiFi.h>
//#include <ESPAsyncWebServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
 //-----------------------------------
 //web server calls
  ESP8266WebServer server(80);
  
// Serial.begin();
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
Adafruit_BME680 bme; // I2C
#define SEALEVELPRESSURE_HPA (1013.25)

//sensor readings
/*
String readTemp() {
  return String(bme.temperature);
  //return String(1.8 * bme.readTemperature() + 32);
}
String readHumi() {
  return String(bme.humidity);
}
String readPres() {
  return String(bme.pressure / 100.0F);
}
String readGas() {
  return String(bme.gas_resistance / 1000.0);
}
String readAlt() { 
  return String(bme.readAltitude(SEALEVELPRESSURE_HPA));
} 
*/

float temperature, humidity, pressure, gas, altitude;



String SendHTML(float temperature,float humidity,float pressure,float gas, float altitude){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<html>";
  ptr +="<script>\n";
  ptr +="setInterval(loadDoc,1000);\n";
  ptr +="function loadDoc() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.body.innerHTML =this.responseText}\n";
  ptr +="};\n";
  ptr +="xhttp.open(\"GET\", \"/\", true);\n";
  ptr +="xhttp.send();\n";
  ptr +="}\n";
  ptr +="</script>\n";
  ptr +="<head>";
  ptr +="<title>YOUR KAIRAIR.COM Weather Station</title>";
  ptr +="<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  ptr +="<link href='https://fonts.googleapis.com/css?family=Open+Sans:300,400,600' rel='stylesheet'>";
  ptr +="<style>";
  ptr +="html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444;}";
  ptr +="body{margin: 0px;} ";
  ptr +="h1 {margin: 50px auto 30px;} ";
  ptr +=".side-by-side{display: table-cell;vertical-align: middle;position: relative;}";
  ptr +=".text{font-weight: 600;font-size: 19px;width: 200px;}";
  ptr +=".reading{font-weight: 300;font-size: 50px;padding-right: 25px;}";
  ptr +=".temperature .reading{color: #F29C1F;}";
  ptr +=".humidity .reading{color: #3B97D3;}";
  ptr +=".pressure .reading{color: #26B99A;}";
  ptr +=".altitude .reading{color: #955BA5;}";
  ptr +=".gas      .reading{color: #C0C0C0;}";
  ptr +=".superscript{font-size: 17px;font-weight: 600;position: absolute;top: 10px;}";
  ptr +=".data{padding: 10px;}";
  ptr +=".container{display: table;margin: 0 auto;}";
  ptr +=".icon{width:65px}";
  ptr +="</style>";
  ptr +="</head>";
  ptr +="<body>";
  ptr +="<h1>YOUR KAIRAIR.COM Weather Station</h1>";
  ptr +="<div class='container'>";
  ptr +="<div class='data temperature'>";
  ptr +="<div class='side-by-side icon'>";
  ptr +="<svg enable-background='new 0 0 19.438 54.003'height=54.003px id=Layer_1 version=1.1 viewBox='0 0 19.438 54.003'width=19.438px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><g><path d='M11.976,8.82v-2h4.084V6.063C16.06,2.715,13.345,0,9.996,0H9.313C5.965,0,3.252,2.715,3.252,6.063v30.982";
  ptr +="C1.261,38.825,0,41.403,0,44.286c0,5.367,4.351,9.718,9.719,9.718c5.368,0,9.719-4.351,9.719-9.718";
  ptr +="c0-2.943-1.312-5.574-3.378-7.355V18.436h-3.914v-2h3.914v-2.808h-4.084v-2h4.084V8.82H11.976z M15.302,44.833";
  ptr +="c0,3.083-2.5,5.583-5.583,5.583s-5.583-2.5-5.583-5.583c0-2.279,1.368-4.236,3.326-5.104V24.257C7.462,23.01,8.472,22,9.719,22";
  ptr +="s2.257,1.01,2.257,2.257V39.73C13.934,40.597,15.302,42.554,15.302,44.833z'fill=#F29C21 /></g></svg>";
  ptr +="</div>";
  ptr +="<div class='side-by-side text'>Temperature</div>";
  ptr +="<div class='side-by-side reading'>";
  ptr +=temperature;
  ptr +="<span class='superscript'>&deg;C</span></div>";
  ptr +="</div>";
  ptr +="<div class='data humidity'>";
  ptr +="<div class='side-by-side icon'>";
  ptr +="<svg enable-background='new 0 0 29.235 40.64'height=40.64px id=Layer_1 version=1.1 viewBox='0 0 29.235 40.64'width=29.235px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><path d='M14.618,0C14.618,0,0,17.95,0,26.022C0,34.096,6.544,40.64,14.618,40.64s14.617-6.544,14.617-14.617";
  ptr +="C29.235,17.95,14.618,0,14.618,0z M13.667,37.135c-5.604,0-10.162-4.56-10.162-10.162c0-0.787,0.638-1.426,1.426-1.426";
  ptr +="c0.787,0,1.425,0.639,1.425,1.426c0,4.031,3.28,7.312,7.311,7.312c0.787,0,1.425,0.638,1.425,1.425";
  ptr +="C15.093,36.497,14.455,37.135,13.667,37.135z'fill=#3C97D3 /></svg>";
  ptr +="</div>";
  ptr +="<div class='side-by-side text'>Humidity</div>";
  ptr +="<div class='side-by-side reading'>";
  ptr +=humidity;
  ptr +="<span class='superscript'>%</span></div>";
  ptr +="</div>";
  ptr +="<div class='data pressure'>";
  ptr +="<div class='side-by-side icon'>";
  ptr +="<svg enable-background='new 0 0 40.542 40.541'height=40.541px id=Layer_1 version=1.1 viewBox='0 0 40.542 40.541'width=40.542px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><g><path d='M34.313,20.271c0-0.552,0.447-1,1-1h5.178c-0.236-4.841-2.163-9.228-5.214-12.593l-3.425,3.424";
  ptr +="c-0.195,0.195-0.451,0.293-0.707,0.293s-0.512-0.098-0.707-0.293c-0.391-0.391-0.391-1.023,0-1.414l3.425-3.424";
  ptr +="c-3.375-3.059-7.776-4.987-12.634-5.215c0.015,0.067,0.041,0.13,0.041,0.202v4.687c0,0.552-0.447,1-1,1s-1-0.448-1-1V0.25";
  ptr +="c0-0.071,0.026-0.134,0.041-0.202C14.39,0.279,9.936,2.256,6.544,5.385l3.576,3.577c0.391,0.391,0.391,1.024,0,1.414";
  ptr +="c-0.195,0.195-0.451,0.293-0.707,0.293s-0.512-0.098-0.707-0.293L5.142,6.812c-2.98,3.348-4.858,7.682-5.092,12.459h4.804";
  ptr +="c0.552,0,1,0.448,1,1s-0.448,1-1,1H0.05c0.525,10.728,9.362,19.271,20.22,19.271c10.857,0,19.696-8.543,20.22-19.271h-5.178";
  ptr +="C34.76,21.271,34.313,20.823,34.313,20.271z M23.084,22.037c-0.559,1.561-2.274,2.372-3.833,1.814";
  ptr +="c-1.561-0.557-2.373-2.272-1.815-3.833c0.372-1.041,1.263-1.737,2.277-1.928L25.2,7.202L22.497,19.05";
  ptr +="C23.196,19.843,23.464,20.973,23.084,22.037z'fill=#26B999 /></g></svg>";
  ptr +="</div>";
  ptr +="<div class='side-by-side text'>Pressure</div>";
  ptr +="<div class='side-by-side reading'>";
  ptr +=pressure;
  ptr +="<span class='superscript'>hPa</span></div>";
  ptr +="</div>";
  ptr +="<div class='data gas'>";
  ptr +="<div class='side-by-side icon'>";
  ptr +="<svg enable-background='new 0 0 40.542 40.541'height=40.541px id=Layer_1 version=1.1 viewBox='0 0 40.542 40.541'width=40.542px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><g><path d='M34.313,20.271c0-0.552,0.447-1,1-1h5.178c-0.236-4.841-2.163-9.228-5.214-12.593l-3.425,3.424";
  ptr +="c-0.195,0.195-0.451,0.293-0.707,0.293s-0.512-0.098-0.707-0.293c-0.391-0.391-0.391-1.023,0-1.414l3.425-3.424";
  ptr +="c-3.375-3.059-7.776-4.987-12.634-5.215c0.015,0.067,0.041,0.13,0.041,0.202v4.687c0,0.552-0.447,1-1,1s-1-0.448-1-1V0.25";
  ptr +="c0-0.071,0.026-0.134,0.041-0.202C14.39,0.279,9.936,2.256,6.544,5.385l3.576,3.577c0.391,0.391,0.391,1.024,0,1.414";
  ptr +="c-0.195,0.195-0.451,0.293-0.707,0.293s-0.512-0.098-0.707-0.293L5.142,6.812c-2.98,3.348-4.858,7.682-5.092,12.459h4.804";
  ptr +="c0.552,0,1,0.448,1,1s-0.448,1-1,1H0.05c0.525,10.728,9.362,19.271,20.22,19.271c10.857,0,19.696-8.543,20.22-19.271h-5.178";
  ptr +="C34.76,21.271,34.313,20.823,34.313,20.271z M23.084,22.037c-0.559,1.561-2.274,2.372-3.833,1.814";
  ptr +="c-1.561-0.557-2.373-2.272-1.815-3.833c0.372-1.041,1.263-1.737,2.277-1.928L25.2,7.202L22.497,19.05";
  ptr +="C23.196,19.843,23.464,20.973,23.084,22.037z'fill=#C0C0C0 /></g></svg>";
  ptr +="</div>";
  ptr +="<div class='side-by-side text'>Gas</div>";
  ptr +="<div class='side-by-side reading'>";
  ptr +=gas;
  ptr +="<span class='superscript'>kOhms</span></div>";
  ptr +="</div>";
  ptr +="<div class='data altitude'>";
  ptr +="<div class='side-by-side icon'>";
  ptr +="<svg enable-background='new 0 0 58.422 40.639'height=40.639px id=Layer_1 version=1.1 viewBox='0 0 58.422 40.639'width=58.422px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><g><path d='M58.203,37.754l0.007-0.004L42.09,9.935l-0.001,0.001c-0.356-0.543-0.969-0.902-1.667-0.902";
  ptr +="c-0.655,0-1.231,0.32-1.595,0.808l-0.011-0.007l-0.039,0.067c-0.021,0.03-0.035,0.063-0.054,0.094L22.78,37.692l0.008,0.004";
  ptr +="c-0.149,0.28-0.242,0.594-0.242,0.934c0,1.102,0.894,1.995,1.994,1.995v0.015h31.888c1.101,0,1.994-0.893,1.994-1.994";
  ptr +="C58.422,38.323,58.339,38.024,58.203,37.754z'fill=#955BA5 /><path d='M19.704,38.674l-0.013-0.004l13.544-23.522L25.13,1.156l-0.002,0.001C24.671,0.459,23.885,0,22.985,0";
  ptr +="c-0.84,0-1.582,0.41-2.051,1.038l-0.016-0.01L20.87,1.114c-0.025,0.039-0.046,0.082-0.068,0.124L0.299,36.851l0.013,0.004";
  ptr +="C0.117,37.215,0,37.62,0,38.059c0,1.412,1.147,2.565,2.565,2.565v0.015h16.989c-0.091-0.256-0.149-0.526-0.149-0.813";
  ptr +="C19.405,39.407,19.518,39.019,19.704,38.674z'fill=#955BA5 /></g></svg>";
  ptr +="</div>";
  ptr +="<div class='side-by-side text'>Altitude</div>";
  ptr +="<div class='side-by-side reading'>";
  ptr +=altitude;
  ptr +="<span class='superscript'>m</span></div>";
  ptr +="</div>";
  ptr +="</div>";
  ptr +="</body>";
  ptr +="</html>";
  return ptr;
}


//-----------------------------------
//SSD1306 Display

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
bool displayOn = true; //boolean for turning the display on/off via a web call: 192.168.4.1/display - which toggles the display on/off.

//-----------------------------------
//turn the display on or off -- called from the webbrowser via  server.on ...
void toggleDisplay() { 

  if(!displayOn) {
    Serial.println("turn on display");
    displayOn = true;
    display.ssd1306_command(SSD1306_DISPLAYON);
  } 
  else {
    //turn off display  
    Serial.println("turn off display");
     displayOn = false;
     // turn it off
     display.ssd1306_command(SSD1306_DISPLAYOFF);
  } 
}
 
//-----------------------------------
//BLYNK

/*
BlynkTimer timer;

void myTimerEvent()
{
    // You can send any value at any time.
    // Please don't send more that 10 values per second.
    // Blynk.virtualWrite(V2, getTimeAlive(millis()));
    Blynk.virtualWrite(V4, readTemp());
    Blynk.virtualWrite(V5, readHumi());
    Blynk.virtualWrite(V6, readPres());
    Blynk.virtualWrite(V7, readGas());
    Blynk.virtualWrite(V8, readAlt());
}
*/
//-----------------------------------

//===================================
//  SETUP
//=================================== 
void setup() {

 //-----------------------------------
 //wifimanager
 
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
  Serial.begin(115200);
  Serial.setDebugOutput(true);  
  delay(3000);
  Serial.println("\n Starting");
  //wm.resetSettings(); // wipe settings
  if(wm_nonblocking) wm.setConfigPortalBlocking(false);   //DAVID: blocking vs. nonblocking ??

  //button used to reset / remove SSID/PW settings  (GPIO12 D6-8266 12E)
  pinMode(TRIGGER_PIN, INPUT);

  std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};
  wm.setMenu(menu);
  wm.setClass("invert");
  wm.setConfigPortalTimeout(180); // auto close configportal after n seconds
  
  bool res;
  res = wm.autoConnect("kairair","breathe"); // password protected ap
  if(!res) {
    Serial.println("Failed to connect or hit timeout");
     ESP.restart();
  } 
  else {
    //if you get here you have connected to the WiFi    
    Serial.println("connected...yeey :)");
  }

  bool status;
  status = bme.begin(0x77);  
  if (!status) {
    Serial.println("Could not find a valid BME680, check wiring!");
    while (1);
  }

  //Serial.println("setting up BME sensor");
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
   }
   
   display.display();
   //Serial.println(F("SSD1306 Display allocation successfullllll"));
   delay(1000); 

  server.on("/", handle_OnConnect);
  server.on("/wtf", handle_OnWtf);
  server.on("/display", handle_OnDisplay);
  server.onNotFound(handle_NotFound);
  
 // Serial.println("server ABOUT TO begin");  
  server.begin(); 
  delay(100); 
  //Serial.println("server beeeeegin");  

 // WiFi.softAP("lookisee");
}


//===================================
void loop() {
 // if(wm_nonblocking) wm.process(); // avoid delays() in loop when non-blocking and other long running code  
  checkButton();
  bme_UpdateReadings();
  //add boolean for Display on/off via webserver
  if (displayOn == true) {
    setDisplay(); 
  }
  Serial.println("calllling server.handleClient()");
  server.handleClient();
  
//-----------------------------------
  //put device to sleep
  //remember to remove the RST wiring during development 
  //and re-attach it after upload.
  /*
  Serial.println("putting device into deep sleep, to wake via timer in 30 SEC");
  ESP.deepSleep(30e6); 
  Serial.begin(115200);
  Serial.setTimeout(2000);
  Serial.println();
  Serial.println("waking from deep sleep"); 
  */
//-----------------------------------
}

void bme_UpdateReadings(){
  // Tell BME680 to begin measurement.
  unsigned long endTime = bme.beginReading();
  if (endTime == 0) {
    Serial.println(F("Failed to begin reading :("));
    return;
  }
  
  Serial.print(F("Reading started at "));
  Serial.print(millis());
  Serial.print(F(" and will finish at "));
  Serial.println(endTime);

  if (!bme.endReading()) {
    Serial.println(F("Failed to complete reading :("));
    return;
  }
  
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  gas = (bme.gas_resistance / 1000.0);
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  Serial.print(F("Reading completed at "));
  Serial.println(millis());

  Serial.print(F("Temperature = "));
  Serial.print(temperature);
  Serial.println(F(" *C"));

  Serial.print(F("Pressure = "));
  Serial.print(pressure);
  Serial.println(F(" hPa"));

  Serial.print(F("Humidity = "));
  Serial.print(humidity);
  Serial.println(F(" %"));

  Serial.print(F("Gas = "));
  Serial.print(gas);
  Serial.println(F(" KOhms"));

  Serial.print(F("Approx. Altitude = "));
  Serial.print(altitude);
  Serial.println(F(" m"));

  Serial.println();
}

void handle_OnConnect() {
 // Serial.println("handle_OnConnect called: web page should update");
  server.send(200, "text/html", SendHTML(temperature,humidity,pressure,gas,altitude)); 
}

void handle_OnWtf() {
  Serial.println("handle_Wtf called");
  server.send(200, "text/html", "What the fuckkkkkkk!"); 
}

void handle_NotFound(){
  Serial.println("404 page not found");
  server.send(404, "text/plain", "Not found");
}

void handle_OnDisplay(){
  Serial.println("toggling display");
  toggleDisplay();
  server.send(200, "text/html", "Toggle Display on/off"); ;
}



//=====================================
//setDisplay
//=====================================
void setDisplay() {
       // Clear the buffer
       display.clearDisplay();

      // display temperature
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(0,0);
      display.print("T: ");
      display.print(temperature);
      display.print(" ");
      display.setTextSize(1);
      display.cp437(true);
      display.write(248);
      display.setTextSize(1);
      display.print(" C");
      
      // display humidity
      display.setTextSize(1);
      display.setCursor(0, 10);
      display.print("H: ");
      display.print(humidity);
      display.print(" %"); 
      
      // display pressure
      display.setTextSize(1);
      display.setCursor(0, 20);
      display.print("P: ");
      display.print(pressure);
      display.print(" hPa");

      // display gas
      display.setTextSize(1);
      display.setCursor(0, 30);
      display.print("G: ");
      display.print(gas);
      display.print(" KOhms");
      
      // display alt
     display.setTextSize(1);
      display.setCursor(0, 40);
      display.print("A: ");
      display.print(altitude);
      display.print(" m"); 

      // display IP#
      display.setTextSize(1);
      display.setCursor(0, 50);
      display.print("IP#: ");
      display.print(WiFi.localIP());
      display.display();
      
      Serial.println(WiFi.localIP());
}


//============================================
//  CHECKBUTTON
//============================================
void checkButton(){
  // check for button press
  if ( digitalRead(TRIGGER_PIN) == HIGH ) {
    // poor mans debounce/press-hold, code not ideal for production
    Serial.println("bttn pressed");
    delay(50);
    if( digitalRead(TRIGGER_PIN) == HIGH ){
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      if( digitalRead(TRIGGER_PIN) == HIGH ){
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        ESP.restart();
      }
      /*
      // start portal w delay
      Serial.println("Starting config portal");
      wm.setConfigPortalTimeout(120);
      
      if (!wm.startConfigPortal("breath","password")) {
        Serial.println("failed to connect or hit timeout");
        delay(3000);
        // ESP.restart();
      } else {
        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
          // Serial.println(wm.softAPIP());
      } 
      */
    }
  }
}
