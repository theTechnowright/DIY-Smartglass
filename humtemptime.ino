#include <Wire.h>
#include <RTClib.h>
#include <U8glib.h>  // U8glib library
#include <dht.h>     // DHT library

#define dht_apin A3  // Analog pin to which the sensor is connected
dht DHT;
           

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C 
const int buttonPin = 4;  
int buttonState = 0;
// Setup RTC
  RTC_DS3231 RTC;
  char monthString[37]= {"JanFebMarAprMayJunJulAugSepOctNovDec"};
  int  monthIndex[122] ={0,3,6,9,12,15,18,21,24,27,30,33};
  String thisMonth = "";
  String thisTime = "";
  String thisDay="";
  int clockCentreX = 64; // used to fix the centre the analog clock
  int clockCentreY = 32; // used to fix the centre the analog clock

void drawTemp(void) 
{
   u8g.setFont(u8g_font_fur14);   // select font
   u8g.drawStr(0, 14,"Weather");
   u8g.drawStr(0, 35, "Temp: ");   // put string of display at position X, Y
   u8g.drawStr(0, 60, "Hum: ");
   u8g.setPrintPos(72, 35);        // set position
   u8g.print(DHT.temperature, 0);  // display temperature from DHT11 in Celsius
   u8g.println("C"); 
   u8g.setPrintPos(60, 60);        // set position
   u8g.print(DHT.humidity, 0);     // display humidity from DHT11
   u8g.println("%");
}
void drawClock(void){
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_profont15);
  //u8g.setFont(u8g_font_6x10); 
  //
  //***** RTC **********
  DateTime now = RTC.now();
  // display date at bottom of screen
  thisDay = String(now.day(), DEC) + "/"; 
  thisMonth="";
  for (int i=0; i<=2; i++){
    thisMonth += monthString[monthIndex[now.month()-1]+i];
  }  
  thisDay=thisDay + thisMonth + "/"; 
  thisDay=thisDay + String(now.year() , DEC);
  const char* newDay = (const char*) thisDay.c_str(); 
  u8g.drawStr(32,63, newDay);   
  // ********************* 
  // display time in digital format
  thisTime="";
  thisTime=String(now.hour()) + ":";
  if (now.minute() < 10){ thisTime=thisTime + "0";} // add leading zero if required
  thisTime=thisTime + String(now.minute()) + ":";
  if (now.second() < 10){ thisTime=thisTime + "0";} // add leading zero if required
  thisTime=thisTime + String(now.second());
  const char* newTime = (const char*) thisTime.c_str();
  u8g.drawStr(10,10, newTime);  
  // ********************* 
  //
  // Now draw the clock face
  u8g.drawCircle(clockCentreX, clockCentreY, 17); // main outer circle
  u8g.drawCircle(clockCentreX, clockCentreY, 1);  // small inner circle
  //
  //hour ticks
  for( int z=0; z < 360;z= z + 30 ){
  //Begin at 0° and stop at 360°
    float angle = z ;
    angle=(angle/57.29577951) ; //Convert degrees to radians
    int x2=(clockCentreX+(sin(angle)*18));
    int y2=(clockCentreY-(cos(angle)*18));
    int x3=(clockCentreX+(sin(angle)*(18-5)));
    int y3=(clockCentreY-(cos(angle)*(18-5)));
    u8g.drawLine(x2,y2,x3,y3);
  }
  // display second hand
  float angle = now.second()*6 ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  int x3=(clockCentreX+(sin(angle)*(15)));
  int y3=(clockCentreY-(cos(angle)*(15)));
  u8g.drawLine(clockCentreX,clockCentreY,x3,y3);
  //
  // display minute hand
  angle = now.minute() * 6 ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  x3=(clockCentreX+(sin(angle)*(20-8)));
  y3=(clockCentreY-(cos(angle)*(20-8)));
  u8g.drawLine(clockCentreX,clockCentreY,x3,y3);
  //
  // display hour hand
  angle = now.hour() * 30 + int((now.minute() / 12) * 6 )   ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  x3=(clockCentreX+(sin(angle)*(20-13)));
  y3=(clockCentreY-(cos(angle)*(20-13)));
  u8g.drawLine(clockCentreX,clockCentreY,x3,y3);
 }


void setup(void) {
  Serial.begin(9600);
  // If you want to set the aref to something other than 5v
  analogReference(EXTERNAL);
  Wire.begin();
  RTC.begin();

    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

void loop(void){
 buttonState = digitalRead(buttonPin);
   DHT.read11(dht_apin);  // Read apin on DHT11
    
    if (buttonState == HIGH) {
   u8g.firstPage();  
   do 
{
   drawClock();
}  while( u8g.nextPage() );
    }
    if(buttonState == LOW){
 u8g.firstPage();  
   do 
{
   drawTemp();
}  while( u8g.nextPage() );
   delay(5000);  // Delay of 2 sec before accessing DHT11 (min - 2sec
    }
}
                           /*END OF FILE*/
