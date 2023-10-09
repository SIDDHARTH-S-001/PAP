// Libraries
#include <ESP8266WiFi.h>
#include <Servo.h>

WiFiClient client;
WiFiServer server(80);
const char* ssid = "network_name";
const char* password = "password";

// Objects for all the servo motors
Servo base;
Servo fist;
Servo elbow;
Servo claw;

// Required Variables
int basepos, baseppos, fistpos, fistppos, elbowpos, elbowppos, clawpos, clawppos;
float l1 = 150.0, l2 = 182.0; // all values in mm
String data = "";
int count = 0;
float x_current = 0.0, y_current = 0.0, z_current = 0.0, x_desired = 0.0, y_desired = 0.0, z_desired = 0.0;
float tolerance = 0.1, error_X, error_y, error_z, error;
int theta1 = 0, theta2 = 0, theta3 = 0, iteration = 0, max_iterations = 20;

void setup() {      // Initialize servo motors
  base.attach(D4);  //base
  fist.attach(D3);
  elbow.attach(D5);
  claw.attach(D6);

  // Commect to WiFi
  Serial.begin(115200);
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));
  server.begin();

  // Set servos to default postions
  baseppos = 0;
  base.write(baseppos);

  fistppos = 0;
  fist.attach(fistppos);

  elbowppos = 0;
  elbow.write(elbowppos);

  clawppos = 0;
  claw.attach(clawppos);
}

void loop() {
  client = server.available();
  if (!client) {
    return;
  }
  data = checkClient();
  count = count + 1;
  // client.println({'message':'I am Screwed xD'});
  client.print("HTTP/1.1 200 OK\r\n");
  client.print("\r\n\r\n");
  client.print("Hello ");
  Serial.println(data);
  Serial.print("  ");
  Serial.print(count);
  if (data.startsWith("s1")) {
    String dataInS = data.substring(2, data.length());
    Serial.println(dataInS);
    basepos = dataInS.toInt();
    Serial.println(basepos);
    if (basepos < baseppos) {
      for (int j = baseppos; j >= basepos; j--) {
        base.write(j);
        delay(25);
      }
    } else if (baseppos > baseppos) {
      for (int i = baseppos; i <= basepos; i++) {
        base.write(i);
        delay(25);
      }
    }
    forward_kine(basepos, elbowpos, fistpos, clawpos);
    baseppos = basepos;
  }

  if (data.startsWith("s2")) {
    String dataInS = data.substring(2, data.length());
    Serial.println(dataInS);
    fistpos = dataInS.toInt();
    Serial.println(fistpos);

    if (fistpos < fistppos) {
      for (int k = fistppos; k > fistpos; k--) {
        fist.write(k);
        delay(25);
      }
    } else if (fistpos > fistppos) {
      for (int l = fistppos; l < fistpos; l++) {
        fist.write(l);
        delay(25);
      }
    }
    forward_kine(basepos, elbowpos, fistpos, clawpos);
    fistppos = fistpos;
  }
  if (data.startsWith("s3")) {
    String dataInS = data.substring(2, data.length());
    Serial.println(dataInS);
    elbowpos = dataInS.toInt();
    Serial.println(elbowpos);

    if (elbowpos < elbowppos) {
      for (int m = elbowppos; m > elbowpos; m--) {
        elbow.write(m);
        delay(25);
      }
    } else if (elbowpos > elbowppos) {
      for (int n = elbowppos; n < elbowpos; n++) {
        elbow.write(n);
        delay(25);
      }
    }
    forward_kine(basepos, elbowpos, fistpos, clawpos);
    elbowppos = elbowpos;
  }
  if (data.startsWith("s4")) {
    String dataInS = data.substring(2, data.length());
    Serial.println(dataInS);
    clawpos = dataInS.toInt();
    Serial.println(clawpos);

    if (clawpos < clawppos) {
      for (int o = clawppos; o > clawpos; o--) {
        claw.write(o);
        delay(25);
      }
    } else if (clawpos > clawppos) {
      for (int p = clawppos; p < clawpos; p++) {
        claw.write(p);
        delay(25);
      }
    }
    forward_kine(basepos, elbowpos, fistpos, clawpos);
    clawppos = clawpos;
  }

  if (data.startsWith("xg")) {
    String dataInS = data.substring(2, data.length());
    Serial.println(dataInS);
    x_desired = dataInS.toInt();
  }

  if (data.startsWith("yg")) {
    String dataInS = data.substring(2, data.length());
    Serial.println(dataInS);
    y_desired = dataInS.toInt();
  }

  if (data.startsWith("zg")) {
    String dataInS = data.substring(2, data.length());
    Serial.println(dataInS);
    z_desired = dataInS.toInt();
  }

  if (data.startsWith("in")) {
    set_initial_position();
  }

  if (data.startsWith("go")) {
    inverse_kine(x_desired, y_desired, z_desired);
  }

  client.stop();
}

// Function to get web requests
String checkClient(void) {
  while (!client.available()) {
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

void set_initial_position(){
  // Set servos to default postions
  basepos = 0;
  base.write(baseppos);

  fistpos = 0;
  fist.attach(fistppos);

  elbowpos = 0;
  elbow.write(elbowppos);

  clawpos = 0;
  claw.attach(clawppos);

  forward_kine(basepos, elbowpos, fistpos, clawpos);
  baseppos = basepos;
  elbowppos = elbowpos;
  fistppos = fistpos;
  clawppos = clawpos;
}

void forward_kine(int basepos, int elbowpos, int fistpos,  int clawpos){
  // Calculate x_current, y_current and z_current
  int theta1 = basepos;
  int theta2 = elbowpos;
  int theta3 = fistpos;
  x_current =  l1*cos(theta1)*cos(theta2) + l2*cos(theta1)*cos(theta2)*cos(theta3) - l2*cos(theta1)*sin(theta2)*sin(theta3);
  y_current =  l1*cos(theta2)*sin(theta1) + l2*cos(theta2)*cos(theta3)*sin(theta1) - l2*sin(theta1)*sin(theta2)*sin(theta3);
  z_current = -l1*sin(theta2) - l2*cos(theta2)*sin(theta3) - l2*cos(theta3)*sin(theta2);
}

void inverse_kine(float x_desired, float y_desired, float z_desired){
  // Calculate position of all servo motors using Gauss-Seidel Method
  error = 1.0; // Assuming initial value 

  while (error > tolerance && iteration < max_iterations){

    float prev_theta1 = basepos;
    float prev_theta2 = elbowpos;
    float prev_theta3 = fistpos;

    forward_kine(basepos, elbowpos, fistpos, clawpos);

    float error_x = x_desired - x_current;
    float error_y = y_desired - y_current;
    float error_z = z_desired - z_current;

    // Update joint angles using Gauss-Seidel method
    theta1 += atan2(error_y, error_x);
    theta2 += atan2(sqrt(error_x * error_x + error_y * error_y) - l1, error_z - l2 * sin(theta2));
    theta3 += atan2(sin(theta3), cos(theta3));

    // Calculate the error as the change in joint angles
    error = abs(theta1 - prev_theta1) + abs(theta2 - prev_theta2) + abs(theta3 - prev_theta3);

    iteration++;
  }  
  
  if (iteration >= max_iterations) {
    Serial.println("Failed to converge to a solution.");
  } 
  
  else {
    Serial.print("Joint angles: ");
    Serial.print("Theta1: ");
    Serial.print(theta1 * 180.0 / PI);
    Serial.print(" degrees, Theta2: ");
    Serial.print(theta2 * 180.0 / PI);
    Serial.print(" degrees, Theta3: ");
    Serial.println(theta3 * 180.0 / PI);
  }

  delay(50); // Delay for visualization (adjust as needed)
}
