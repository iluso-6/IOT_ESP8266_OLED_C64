// C64 Emulator, works with esp8266 using webbrowser as display
// see
// http://www.esp8266.com/viewtopic.php?f=8&t=4704&p=28932#p28932
// based upon
// http://forum.arduino.cc/index.php?topic=193216.msg1793065#msg1793065

// Remarks speed:
// max speed is 5 frames per second, this amounts to 5 kB (kilobytes) per second
// Reduced the number of RAM bytes send from esp8266 to zero and
// als reduced the processing of this info in browser to zero bytes,
// but this has no effect on framerate (still 5 fps)
// slow speed possibly due to protocolo XMLHTTP!
// Serial debug works at 115200 baudrate, so 14kB per second and would make 14 fps possible!

//find free space 
//extern "C" {#include "user_interface.h"}

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <Adafruit_GFX.h>   // Needs a little change in original Adafruit library (See README.txt file)
#include <ESP_SSD1306.h>    // Modification of Adafruit_SSD1306 for ESP8266 compatibility

#include <Fonts/TomThumb.h>
#include <SPI.h>            // For SPI comm (needed for not getting compile error)
#include <Wire.h>  

#define OLED_RESET  16  // Pin 15 -RESET digital signal

#include "logoimage.h"
#include "vicii_page.h"
#include "gui_page_updated.h"


ESP_SSD1306 display(OLED_RESET); // FOR I2C

//const char* ssid = "DeltaFlyer";
const char* ssid = "Nasc TP";
const char* password = "Cuan+885227";
int screenbottom = 5;
ESP8266WebServer server(80);
uint8_t curkey = 0;


#define EXECUTES_PER_RUN 100
#define RAM_SIZE 24576
uint8_t RAM[RAM_SIZE];
uint8_t HIGHVIC[16];//color of background/border/sprites are situated here

uint16_t getpc();
uint8_t getop();
void exec6502(int32_t tickcount);
void reset6502();
void serout(uint8_t val) {
	//	Serial.write(val);
} 

int serIn;

//server.send_uint8(200, "text/plain", msg);   
void send_uint8(const uint8_t *data, const size_t data_length) {
/*
 else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";
  //if(server.hasArg("download")) dataType = "application/octet-stream";
*/  
  String data_type="application/octet-stream";
  //String data_type="image/png";
  server.sendHeader("Content-Length", String(data_length));
  server.send(200, data_type.c_str(), "");

  WiFiClient client = server.client();
  client.write(data, data_length);
}



//Onready vindt alleen plaats als er iets wordt verzonden naar de browser!
//responeType arrayBuffer werkt niet goed!!! ALternatief is blob?    
//    "  xmlhttp.responseType = 'arraybuffer';\n"

void writeToOled(){

 //   Serial.println (RAM[214]);
    
    display.clearDisplay();
    display.setCursor(0,0); 

 
  int row=0;
  for (int i=0; i<1000; i++){
    uint8_t petscii = RAM[i + 1024];
    if (petscii<32) petscii = petscii + 64;
 
   char c = (char)petscii;
 //  if (c == '\n') continue;
 //  if (c == ' ') continue;
   display.write(c);
 //  Serial.print (c);
//  Serial.print (char(petscii));
    int col=i % 40;
    if (col==39){ 
      row++;
      screenbottom++;
   //   Serial.println();
      display.println();
      };
 //   if (screenbottom>40){ // END OF OLED SCREEN
  //    display.println();
  //    Serial.println ("newline");
  ///    screenbottom = 40;
  //    };
  }
display.display();
}


void VICIIRefresh(){
  wdt_disable();

 /*
 if(Serial.available()) {
       int nrkeys=0;    
    //keep reading and printing from serial untill there are bytes in the serial buffer
     while (Serial.available()>0){
        serIn = Serial.read();  //read Serial        
        int key=serIn;
        
         if (key>96 && key<123) key = key - 32;
         key = key & 0x7F;
         RAM[631+nrkeys] = key ;//& 0x7F;//put key in buffer
         nrkeys++; 
        
     }        
/*         
         Serial.print("accept: ");
         Serial.print(server.arg(i).toInt());
         Serial.print(" > ");
         Serial.println(key);
       }else{
         //Serial.print("reject: ");
         //Serial.print(server.argName(i));              
         //Serial.print("=");
         //Serial.println(server.arg(i));
*/         
       
    
   //  RAM[198]=nrkeys;//number keys put in buffer
//  }

//245-246 vector keyboard decode table
//655-656 vector keyboard table setup
//648 top of screen memory
//678 PAL(1)/NTSC(0)
//2024-?2047? sprite memory

  //send video ram

  int m=0;
  uint8_t VICRAM[1024];  
  VICRAM[0]=RAM[214];//line number of cursor (read/write)
  VICRAM[1]=RAM[211];//col number of cursor (read/write)
  VICRAM[2]=RAM[243];//low byte of location of color ram (RAM[244]*256+RAM[243], 55296-56319 default )
  VICRAM[3]=RAM[244];//high byte of location of color ram (RAM[244]*256+RAM[243], 55296-56319 default )
  VICRAM[4]=RAM[646];//text color code (read/write)
  VICRAM[5]=HIGHVIC[0];//53280 border color
  VICRAM[6]=HIGHVIC[1];//53281 background (0)
  VICRAM[7]=HIGHVIC[2];//53282 background 1 (MC & ECM)
  VICRAM[8]=HIGHVIC[3];//53283 background 2 (MC & ECM)
  VICRAM[9]=HIGHVIC[4];//53284 background 3 (MC & ECM)
  m=24;
  for (int i=1024;i<2023;i++){//1024-2023 char ram
    uint8_t petscii = RAM[i];
    if (petscii<32) petscii = petscii + 64;   
    VICRAM[m]=petscii;
    Serial.println(petscii);
    m++;
  }
  send_uint8(VICRAM,1024); 
//  dumpScreenRam();
  wdt_enable(5000);  
} 


//Very nice articles about graphics mode of the VIC-II
//memory requirements of different modes: http://www.filfre.net/2013/08/how-things-work-commodore-64-and-summer-games-edition/
//location of screen RAM @ 53272 (https://www.c64-wiki.com/index.php/Screen_RAM)
//how it works: http://dustlayer.com/c64-coding-tutorials/2013/4/8/episode-2-2-writing-to-the-c64-screen

void setup () {

  Serial.begin (115200);
  Serial.println("Setup Start");
  display.begin(SSD1306_SWITCHCAPVCC);  // Switch OLED
   Serial.println("after delay"); 
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setFont(&TomThumb);
  display.setTextColor(WHITE);
  display.drawBitmap(0,0,logo_bmp,128,64,WHITE);
    display.display();
    delay(4000);
  display.invertDisplay(true);

   Serial.println("after 4000");
}

unsigned long cycles=0;

void initWifi(){
  // Start Wifi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("  ip-address: ");
  Serial.println(WiFi.localIP());
}

void initServer(){
  // set comm. pages    
  server.on ( "/",[]() {
                    Serial.println("Root page called"); 
                    server.send_P ( 200, "text/html", gui); 
                   });
  server.on ( "/VICII.js",[]() {
                    Serial.println("Javascript loaded"); 
                    server.send_P ( 200, "text/plain", vicii_page); 
                   });             
  server.on ( "/VICIIRefresh", VICIIRefresh);

//  server.onNotFound(handleNotFound);
  
  // Start the server  
  server.begin();
}

/*
void checkFreeSpace(){
  //uint32_t freeSpace=system_get_free_heap_size();
  Serial.print (system_get_free_heap_size());
  Serial.println (" free space.");
}
*/

int fase=0;
void loop () {
  if (fase!=4) {Serial.print ("Start boot fase ");Serial.println(fase);}
  if (fase==0) {initWifi();}
  if (fase==1) {initServer();}
  //if (fase==2) {checkFreeSpace();}
  if (fase==3) {reset6502();}
  if (fase!=4) {Serial.print ("..finished boot fase ");Serial.println(fase);}
  if (fase!=4) {fase++;return;}


  server.handleClient();

 int v_address = 0;
 Serial.write(27);     // Move to Home
 Serial.write("[H");
 
 for (int y=0;y<25;y++) {
   for (int x=0;x<40;x++) {
  exec6502(EXECUTES_PER_RUN); 
     if (Serial.available()) {
      curkey = Serial.read() & 0x7F;
    RAM[198]=1;
    RAM[631]=curkey; 
    }
    unsigned char petscii=RAM[v_address++];
    if (petscii<32) petscii=petscii+64;
 //   Serial.write(petscii);
   }
//   Serial.write(13);
//   Serial.write(10);
 }
 Serial.write("CURSOR: "+RAM[214]);
  writeToOled();
  cycles++;
}

