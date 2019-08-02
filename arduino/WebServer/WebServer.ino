#include <SPI.h>
#include <WiFi101.h>
#include <Wire.h>

IPAddress ip(192, 168, 43, 177);

char ssid[] = "Beep boop";
char pass[] = "soix1380";
int keyIndex = 0;

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  while (!Serial) {
    ; 
  }

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }
  WiFi.config(ip);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    delay(10000);
  }
  server.begin();
  printWiFiStatus();
}

 int verifParam=0,i=0,verifVal=0,verifPrimParam=1,alert=0,j;
 int param[2];
 char key[25];
 char lock[25] = "ScaunCuRotile";
 
void loop() {
  verifPrimParam=1;
  i=0;
  j=0;
  alert=0;
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if(c=='?') verifParam=1;
        if(verifVal==1 && verifPrimParam==0) {
          param[i]=(int)c-'0';
          i++;
          verifVal=0;
        }
        if(verifParam==1 && c=='=') verifVal=1;
        if(verifPrimParam==1 && verifVal==1){
          while(c != '&'){
          c=client.read();
          key[j]=c;
          j++;
         }
          verifPrimParam=0;
          verifVal=0;
          for(int k=0; k<strlen(lock); k++)
            if(lock[k] != key[k]) alert=1;
        }
       //Serial.println(alert);
       /*if(alert == 1){ 
        client.println("Not authorized");
        client.stop();
        return;
       }
       Serial.println(c);
       Serial.println(currentLineIsBlank);*/
        if (c == '\n' && currentLineIsBlank) {
       
          
          client.println("HTTP/1.1 200 OK");
          client.println("Access-Control-Allow-Origin: *");   
          client.println("Access-Control-Allow-Methods: GET");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          Serial.println(param[0]);
          if(param[0]==5){
            Serial.println("in type 5");
            Wire.requestFrom(7,16);
            Serial.println(Wire.available());
            while (Wire.available()) {
              char val = Wire.read();
              if((val>='0' && val<='9') || val=='*')
                client.print(val);
                Serial.print(val); 
            }
            break;
          }
          else{
              Serial.println((param[0]*10)+param[1]);
              Wire.beginTransmission(8);
              Wire.write((param[0]*10)+param[1]);
              Wire.endTransmission();
            
          }
          break;
        }
        
        if (c == '\n') { 
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("client disconnected");
  }
}


void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
