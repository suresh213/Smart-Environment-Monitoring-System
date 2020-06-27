#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

#define DHTTYPE DHT11  

int Distance;
int buzzer = D5;
int flame_detected = HIGH;
int Gas=digitalRead(D0);
const char* ssid = "***";  
const char* password = "***";  

ESP8266WebServer server(80);
WiFiClientSecure client;
uint8_t DHTPin = D2; 
               
DHT dht(DHTPin, DHTTYPE);                

int Temperature=dht.readTemperature();
int Humidity=dht.readHumidity();
int Gas=digitalRead(D0);
int Soil=analogRead(A0);
int Flame=digitalRead(D3);

  
void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(D0, INPUT);//gas
  pinMode(D3,INPUT);//flame
  pinMode(A0, INPUT);//soil
  pinMode(DHTPin, INPUT);
  pinMode(buzzer, OUTPUT);
  
  dht.begin();              //begin sensor

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Your IP: "); 
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
} 
   
void loop() {
  
 server.handleClient();
 mysql();
 delay(2000);
                     // Buzzer
 flame_detected = digitalRead(D3);
  if (flame_detected == LOW)
  {
    Serial.println("Flame detected...! take action immediately.");
    digitalWrite(buzzer, HIGH);
    delay(3000);
  }
  else
  {
    Serial.println("No flame detected. stay cool");
    digitalWrite(buzzer, LOW);
  }

}

void handle_OnConnect() {
int Temperature=dht.readTemperature();
int Humidity=dht.readHumidity();
int Gas=digitalRead(D0);
int Soil=analogRead(A0);
int Flame=digitalRead(D3);

  Serial.print("\n       ***  Enviroinment Monitoring System***   \n");
   Serial.print("\nTemperature = ");
  Serial.print(Temperature);
  Serial.print(" °C");
  Serial.print("\nHumidity = ");
  Serial.print(Humidity);
  Serial.print(" %");
  Serial.print("\nGas Value = ");
  Serial.print(Gas);
  Serial.print(" units");
  Serial.print("\nSoil Moisture = ");
  Serial.print(Soil);
  Serial.print(" units");
  Serial.print("\nFlame = ");
  Serial.print(Flame);
  Serial.print(" units\n");
  delay(3000);
  server.send(200, "text/html", SendHTML(Temperature,Humidity,Gas,Soil,Flame)); 
 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperature,float Humidity,int Gas,int Soil,int Flame){
  String s = "<!DOCTYPE html> <html>\n";
  s +="<head><meta http-equiv=\"refresh\" content=\"30\" charset=\"utf-8\";name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  s +="<link href=\"https://fonts.googleapis.com/css?family=Kulim+Park:300,400,600\" rel=\"stylesheet\">\n";
  s +="<title>Project</title>\n";
  s +="<style>html { font-family: 'Kulim Park'; display: block; margin: 0px auto; text-align: center;color: #333333;}\n";
  
  s +="body{margin-top: 50px;}\n";
  s +="h1 {margin: 50px auto 30px;}\n";
                                           
 
  s +=".side{display: inline-block;vertical-align: middle;position: relative;}\n";

  s +=".humidity-icon{width: 30px;height: 30px;border-radius: 50%;line-height: 36px;}\n";
  s +=".humidity-text{font-weight: 600;padding-left: 15px;font-size: 20px;width: 160px;text-align: left;}\n";
  s +=".humidity{font-weight: 300;font-size: 44px;color: #4ee44e;}\n";
  
  s +=".temperature-icon{width: 30px;height: 30px;border-radius: 50%;line-height: 40px;}\n";
  s +=".temperature-text{font-weight: 600;padding-left: 15px;font-size: 20px;width: 160px;text-align: left;}\n";
  s +=".temperature{font-weight: 300;font-size: 44px;color:red ;}\n";
  
  s +=".gas-icon{width: 30px;height: 30px;border-radius: 50%;line-height: 40px;}\n";
  s +=".gas-text{font-weight: 600;padding-left: 15px;font-size: 20px;width: 160px;text-align: left;}\n";
  s +=".gas{font-weight: 300;font-size: 44px;color:grey ;}\n";
  
  s +=".soil-icon{width: 30px;height: 30px;border-radius: 50%;line-height: 40px;}\n";
  s +=".soil-text{font-weight: 600;padding-left: 15px;font-size: 20px;width: 160px;text-align: left;}\n";
  s +=".soil{font-weight: 300;font-size: 44px;color:orange ;}\n";

  s +=".dis-icon{width: 30px;height: 30px;border-radius: 50%;line-height: 40px;}\n";
  s +=".dis-text{font-weight: 600;padding-left: 15px;font-size: 20px;width: 160px;text-align: left;}\n";
  s +=".dis{font-weight: 300;font-size: 44px;color:green ;}\n";

  s +=".flame-icon{width: 30px;height: 30px;border-radius: 50%;line-height: 40px;}\n";
  s +=".flame-text{font-weight: 600;padding-left: 15px;font-size: 20px;width: 160px;text-align: left;}\n";
  s +=".flame{font-weight: 300;font-size: 44px;color:blue ;}\n";

  
  s +=".superscript{font-size: 17px;font-weight: 600;position: absolute;right: -20px;top: 15px;}\n";
  s +=".data{padding: 10px;}\n";
  s +="</style>\n";
  s +="</head>\n";
  s+="<body>\n";
  s +="<h1>Environment Monitoring System</h1>\n";
                      
                                           //temperature values
   s +="<div class=\"data\">\n";
   s +="<div class=\"side temperature-icon\">\n";
   s+=" <img src=\"https://www.pinclipart.com/picdir/big/71-711162_temperature-temperature-clipart-png-transparent-png.png\" alt=\"tempimg\" width=\"30\" height=\"50\"></div>\n";
   s +="<div class=\"side temperature-text\">Temperature</div>\n";
   s +="<div class=\"side temperature\">";
   s +=(int)Temperature;
   s +="<span class=\"superscript\">°C</span></div>\n";
   s +="</div>\n";
  
                                       //humidity values 
   s +="<div class=\"data\">\n";
   s +="<div class=\"side humidity-icon\">\n";
   s+="<img src=\"http://icons.iconarchive.com/icons/atyourservice/service-categories/256/Humidity-icon.png\" alt=\"humimg\" width=\"43\" height=\"43\"></div>\n";
   s +="<div class=\"side humidity-text\">Humidity</div>\n";
   s +="<div class=\"side humidity\">";
   s +=(int)Humidity;
   s +="<span class=\"superscript\">%</span></div>\n";
   s +="</div>\n";

                                       //gas values
   s +="<div class=\"data\">\n";
   s +="<div class=\"side gas-icon\">\n";
   s+=" <img src=\"http://pngimg.com/uploads/smoke/smoke_PNG55246.png\" alt=\"gasimg\" width=\"40\" height=\"40\"></div>\n";
   s +="<div class=\"side gas-text\">Gas Value</div>\n";
   s +="<div class=\"side gas\">";
   s +=(int)Gas;
   s +="<span <div class=\"superscript\"></div></span>unit</div>\n";
   s +="</div>\n";
   s +="<div id=\"txt\"></div>";
   s +="</div>\n";

                                       //soil values

   s +="<div class=\"data\">\n";
   s +="<div class=\"side soil-icon\">\n";
   s+=" <img src=\"http://www.pngall.com/wp-content/uploads/2/Soil-PNG-HD-Image.png\" alt=\"soilimg\" width=\"40\" height=\"40\"></div>\n";
   s +="<div class=\"side soil-text\">Soil Moisture</div>\n";
   s +="<div class=\"side soil\">";
   s +=(int)Soil;
   s +="<span <div class=\"superscript\"></div></span>units</div>\n";
   s +="</div>\n";
   s +="<div id=\"txt\"></div>";
   s +="</div>\n";

                                        //distance values

   s +="<div class=\"data\">\n";
   s +="<div class=\"side dis-icon\">\n";
   s +=" <img src=\"https://image.flaticon.com/icons/svg/484/484141.svg\" alt=\"temperature\" width=\"40\" height=\"40\"></div>\n";
   s +="<div class=\"side dis-text\">Distance</div>\n";
   s +="<div class=\"side dis\">";
   s +=(int)Distance;
   s +="<span class=\"superscript\">cm</span></div>\n";
   s +="</div>\n";
   s +="<div id=\"txt\"></div>";
   s +="</div>\n";
   
                                               //flame values

   s +="<div class=\"data\">\n";
   s +="<div class=\"side flame-icon\">\n";
   s +=" <img src=\"https://image.flaticon.com/icons/svg/394/394631.svg\" alt=\"flameimg\" width=\"40\" height=\"40\"></div>\n";
   s +="<div class=\"side flame-text\">Flame</div>\n";
   s +="<div class=\"side flame\">";
   s +=(int)Flame;
   s +="<span <div class=\"superscript\"></div></span>units</div>\n";
   s +="</div>\n";
   s +="<div id=\"txt\"></div>";
   s +="</div>\n";

   s +=" <div style=\"text-align:center;padding:1em 0;\"> <h4><a style=\"text-decoration:none;\"\n"; 
   s +="href=\"https://www.zeitverschiebung.net/en/country/in\"><span style=\"color:gray;\"></span><br /></a></h4> ";
   s +="<iframe src=\"https://www.zeitverschiebung.net/clock-widget-iframe-v2?language=en&size=small&timezone=Asia%2FKolkata\"width=\"100%\" height=\"90\" frameborder=\"0\" seamless></iframe> </div>";   
   s +="<p>This Page will Auto Refresh for every 15 Seconds</p>";
   s +="</body>\n";
   s +="</html>\n";
   return s;

}
                                   //connection of sql
void mysql()  
{
char server[] = "192.168.43.138";
int temperaturedata=dht.readTemperature();
int humiditydata=dht.readHumidity();
int gasdata=digitalRead(D0);
int soildata=analogRead(A0);
int flamedata=digitalRead(D3);

WiFiClient client;

  if (client.connect(server, 80)) {
    Serial.println("connected with Sql");
    client.print("GET /testcode/dht.php?humidity=");    
    client.print(humiditydata);
    client.print("&temperature=");
    client.print(temperaturedata);
    client.print("&gas=");
    client.print(gasdata);
    client.print("&soil=");
    client.print(soildata);
    client.print("&flame=");
    client.print(flamedata);   
    client.print(" "); 
    client.print("HTTP/1.1");
    client.println();
    client.println("Host:192.168.43.138");
    client.println("Connection: close");
    client.println();
  } 
  else { 
    Serial.println("Failed to connect sql");
  }
}
