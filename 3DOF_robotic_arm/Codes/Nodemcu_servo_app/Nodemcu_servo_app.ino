#include <ESP8266WiFi.h>
#include <Servo.h>
Servo base;
Servo fist;
Servo elbow;
Servo claw;

WiFiClient client;
WiFiServer server(80);

int basePos;
int basePPos;

int fistpos;
int fistppos;

int elbowpos;
int elbowppos;

int clawpos;
int clawppos;

const char* ssid = "Micromax HS2";
const char* password = "micromax";
String  data ="";

int count = 0;
 
void setup() 
{
base.attach(D4); //base
fist.attach(D3);
elbow.attach(D5);
claw.attach(D6);

                
Serial.begin(115200);
    Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(500);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));
  server.begin();
  
  basePPos = 0;
  base.write(basePPos);

  fistppos = 0;
  fist.attach(fistppos);

   elbowppos = 0;
  elbow.write(elbowppos);

  clawppos = 0;
  claw.attach(clawppos);
   
}

void loop() 
{ client = server.available();
    if (!client) 
    {
      return; 
    }
    data = checkClient ();
    count = count +1;
    // client.println({'message':'I am Screwed xD'});
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("\r\n\r\n");
    client.print("Hello ");
    Serial.println(data);
    Serial.print("  ");
    Serial.print(count);
    if (data.startsWith("s1")) 
    {
      String dataInS = data.substring(2, data.length());
      Serial.println(dataInS);
      basePos = dataInS.toInt();
      Serial.println(basePos);
      if (basePos < basePPos)
      {
        for ( int j = basePPos; j >= basePos; j--) 
        {
          base.write(j);
          delay(25);
        }
      }
      else if (basePPos > basePPos)
      {
        for ( int i = basePPos; i <= basePos; i++) 
        {
          base.write(i);
          delay(25);
        }
      }
      basePPos = basePos;     
    }
    
    if (data.startsWith("s2")) {
      String dataInS = data.substring(2, data.length());
      Serial.println(dataInS);
      fistpos = dataInS.toInt();
      Serial.println(fistpos);
      
      if (fistpos < fistppos) {
        for ( int k = fistppos; k > fistpos; k--) {
          fist.write(k);
          delay(25);
        }
      }
      else if (fistpos > fistppos) {
        for ( int l = fistppos; l < fistpos; l++) {
          fist.write(l);
          delay(25);
        }
      }
      fistppos = fistpos;     
    }
     if (data.startsWith("s3")) {
      String dataInS = data.substring(2, data.length());
      Serial.println(dataInS);
      elbowpos = dataInS.toInt();
      Serial.println(elbowpos);
      
      if (elbowpos < elbowppos) {
        for ( int m = elbowppos; m > elbowpos; m--) {
          elbow.write(m);
          delay(25);
        }
      }
      else if (elbowpos > elbowppos) {
        for ( int n = elbowppos; n < elbowpos; n++) {
          elbow.write(n);
          delay(25);
        }
      }
      elbowppos = elbowpos;     
    }
     if (data.startsWith("s4")) {
      String dataInS = data.substring(2, data.length());
      Serial.println(dataInS);
      clawpos = dataInS.toInt();
      Serial.println(clawpos);
      
      if (clawpos < clawppos) {
        for ( int o = clawppos; o > clawpos; o--) {
          claw.write(o);
          delay(25);
        }
      }
      else if (clawpos > clawppos) {
        for ( int p = clawppos; p < clawpos; p++) {
          claw.write(p);
          delay(25);
        }
      }
      clawppos = clawpos;     
    }
    client.stop();
}


String checkClient (void)
{
  while(!client.available())
  {
  delay(1); 
  }
  String request = client.readStringUntil('\r');
  //request.body();
  /*Serial.println(request);
  request.remove(0, 5);
  Serial.println(request);
  request.remove(request.length()-9,9);
  Serial.println(request);*/
  return request;
}
