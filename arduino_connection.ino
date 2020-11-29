#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
const char* host = "api.thingspeak.com";  //Thingspeak cloud host address to send data
const char* streamId   = "P6LT7T8IALK0NQ5D"; //Channel Write API Key
const char* privateKey = "284705"; //Thingspeak unique channel number
ESP8266WiFiMulti wifiMulti; 
String charIn;
void setup()
{

    wifiMulti.addAP("?q=17.368900,78.553400", "nenuchepanu");  //SRK hotspot
    wifiMulti.addAP("?q=17.459321,78.536174", "cricket1997"); // sid hotspot
    wifiMulti.addAP("?q=17.500010,78.401527", "cricket1997"); // sid hotspot
    
Serial.begin(115200);         //baudrate
Serial.println();

//disconnect any previous connections

WiFi.disconnect();
delay(1000);

//scan for number of nearby networks & print SSIDs

Serial.print("Nearby networks found  :");
Serial.println(WiFi.scanNetworks());
delay(500);
Serial.println("List of surrounding Network SSIDs…:");
int n = WiFi.scanNetworks();
for (int i = 0; i < n; i++)
{
Serial.println(WiFi.SSID(i));
}
Serial.println();

//connect to preferred SSID

Serial.println("Connecting Wifi...");
 if(wifiMulti.run() == WL_CONNECTED) {
        Serial.println("");

Serial.println();

// print SSID name,local IP allotted ,MAC address & signal strength

Serial.println();
Serial.print("Connected to SSID          : ");
Serial.println(WiFi.SSID());

Serial.print("IP address allotted to ESP : ");
Serial.println(WiFi.localIP());

Serial.print("MAC Address of ESP         : ");
Serial.println(WiFi.macAddress());

Serial.print("Signal strength is         : ");

Serial.println(WiFi.RSSI());
 }
}
int value = 0;
void loop()
{
  if(wifiMulti.run() != WL_CONNECTED) {
setup();
    }

 
  delay(15000);
  String h = WiFi.SSID() ;
  String t = WiFi.macAddress();
  double lat=17.50;
  double lng=78.93; 
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  
  WiFiClient client;
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
 
  String url = "https://api.thingspeak.com/update?api_key=P6LT7T8IALK0NQ5D&field1=0";
  url += "?api_key=";
  url += privateKey;
  url += "&field1=";
  url += h;
  url += "&field2=";
  url += t;
  /*url += "&field3=";
  url += lat;
  url += "&field4=";
  url += lng;*/
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  while(client.connected() && !client.available()) delay(1); //waits for data
  while (client.connected() || client.available())
  {
  charIn = client.readString();
  }
  Serial.print(charIn);
  Serial.println();
  Serial.println("closing connection");
  client.stop();
  
}
