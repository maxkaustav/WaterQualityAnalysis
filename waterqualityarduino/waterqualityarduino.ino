float slopeprobe=-5.70;//calculated for temperature at 25C
float intercept=21.34;
#include "ESP8266_AT.h"

#define DOMAIN          "api.thingspeak.com"
#define PORT            "80"
#define API_WRITE_KEY   "L5R1AZZ315DE1AKI"
#define CHANNEL_ID      "963297"

#define SSID            "Kaustav"
#define PASSWORD        "cudaai$2080"
char _buffer[400];
char _buffer2[400];
uint8_t Connect_Status;
uint8_t Sample = 0;
float voltageValue[30];
float voltageValue2;
void setup() {
  Serial.begin(115200);
  while(!ESP8266_Begin());
  ESP8266_WIFIMode(BOTH_STATION_AND_ACCESPOINT);/* 3 = Both (AP and STA) */
  ESP8266_ConnectionMode(SINGLE);     /* 0 = Single; 1 = Multi */
  ESP8266_ApplicationMode(NORMAL);    /* 0 = Normal Mode; 1 = Transperant Mode */
  if(ESP8266_connected() == ESP8266_NOT_CONNECTED_TO_AP)  /*Check WIFI connection*/
  ESP8266_JoinAccessPoint(SSID, PASSWORD);    /*Connect to WIFI*/
  ESP8266_Start(0, DOMAIN, PORT); 
}

void loop() {//add minimum point to print voltage  
    Connect_Status = ESP8266_connected();
    if(Connect_Status == ESP8266_NOT_CONNECTED_TO_AP)    /*Again check connection to WIFI*/
    ESP8266_JoinAccessPoint(SSID, PASSWORD);    /*Connect to WIFI*/
    if(Connect_Status == ESP8266_TRANSMISSION_DISCONNECTED)
    ESP8266_Start(0, DOMAIN, PORT);   /*Connect to TCP port*/
    //part1=(int)
    senddata();
}
double Phdata()
{
  int Navg =20 ;
    uint16_t datapH = 0;
    for(int i=0; i<Navg; i++){ //take 10 readings and then average them
   datapH += analogRead(A0);;
  }
  datapH = datapH/Navg;

    datapH = (datapH*(5.0/1023.0)-0.88)/3.52*14.00;
  if (datapH <0){
    datapH = 0;}
    return datapH;
  
}
double Turbidity()
{
    int Navg =20 ;

  int dataturbidity = 0;
  for(int i=0; i<Navg; i++){ //take 10 readings and then average them
  dataturbidity += analogRead(A1);
  }
 
  dataturbidity = dataturbidity/Navg;
   Serial.println(dataturbidity);
    dataturbidity = (dataturbidity*-5.69) + 4000;
  if (dataturbidity <0) 
  {dataturbidity = 0;
  }
  return dataturbidity;
}

void senddata()
{
    memset(_buffer, 0, 150);
    sprintf(_buffer, "GET /update?api_key=%s&field1=%d.%0.2d&field2=%d.%0.2d", API_WRITE_KEY, (int)Phdata(),(int)(Phdata()*100)%100,(int)Turbidity(),(int)(Turbidity()*100)%100);   /*connect to thingspeak server to post data using your API_WRITE_KEY*/
    ESP8266_Send(_buffer);
    memset(_buffer2,0, 150);
    sprintf(_buffer2, "GET /update?api_key=%s&field2=%d.%0.2d", API_WRITE_KEY, (int)Turbidity(),(int)(Turbidity()*100)%100);   /*connect to thingspeak server to post data using your API_WRITE_KEY*/
    ESP8266_Send(_buffer2);
    delay(1000); /* Thingspeak server delay */
}
