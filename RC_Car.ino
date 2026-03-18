//2nd ESP board (on the car)


// LIBRARIES
#include <WiFi.h>
#include <HTTPClient.h>
// END LIBRARIES

// GLOBAL VARIABLES
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";
#define PIN_IN1  14// ESP32 pin GPIO19 connected to the IN1 pin L298N
#define PIN_IN2  27 // ESP32 pin GPIO18 connected to the IN2 pin L298N
#define PIN_ENA  32 // ESP32 pin GPIO32 connected to the EN1 pin L298N
#define PIN_IN3  26 // ESP32 pin GPIO19 connected to the IN1 pin L298N
#define PIN_IN4  25 // ESP32 pin GPIO18 connected to the IN2 pin L298N
#define PIN_ENB  33 // ESP32 pin GPIO32 connected to the EN1 pin L298N

const char* serverNameXLedToggle = "http://192.168.4.1/x_status";
const char* serverNameYLedToggle = "http://192.168.4.1/y_status";
// END GLOBAL VARIABLES

// EXTERNAL FUNCTIONS
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);
  int httpResponseCode = http.GET();

  String payload = "--"; 
  if (httpResponseCode > 0) {
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return payload;
}
// END EXTERNAL FUNCTIONS


//SETUP
void setup() {

  //SET PINS

  // LN 298
  pinMode(PIN_IN1, OUTPUT); //
  pinMode(PIN_IN2, OUTPUT); //
  pinMode(PIN_ENA, OUTPUT); //enable motor1
  pinMode(PIN_IN3, OUTPUT); //
  pinMode(PIN_IN4, OUTPUT); //
  pinMode(PIN_ENB, OUTPUT); //enable motor2
  // END LN 298

  //END SETTING PINS

  Serial.begin(115200); // START SERIAL CONNECTION BETWEEN LAPTOP AND ESP32 BOARD AT 115200 BITS/SECOND

  //SET CONNECTION WITH FIRST ESP BOARD
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected with IP: ");
  Serial.println(WiFi.localIP());
  //END SETTING CONNECTION WITH FIRST ESP BOARD


}
// END SETUP

//LOOP
void loop() {
  
  // GET INFORMATION FROM FIRST ESP BOARD
  if (WiFi.status() == WL_CONNECTED) {
    String JoystickXpositionStr = httpGETRequest(serverNameXLedToggle);
    String JoystickYpositionStr = httpGETRequest(serverNameYLedToggle);

    int JoystickXposition = JoystickXpositionStr.toInt();
    int JoystickYposition = JoystickYpositionStr.toInt();

    Serial.println("LED X: " + JoystickXpositionStr);
    Serial.println("LED Y: " + JoystickYpositionStr);

    // END GETTING INFORMATION FROM FIRST ESP BOARD

    //LN298 DRIVER MOTOR CONTROL
/*
    if( 2500 < JoystickYposition && JoystickYposition < 3500 ){
      
    }
    else if(JoystickYposition < 2500){
      analogWrite(26,100);
    }
    else if(JoystickYposition > 3500){
      analogWrite(26,100);

    }

*/
  // Control motor B (e.g., for forward/backward)
  if (JoystickYposition < 2500) {
  // Move backward
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
  analogWrite(PIN_ENB, 200); // speed up
  delay(10);
}
  else if (JoystickYposition > 3500) {
  // Move forward
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENB, 200); // speed up
  delay(10);
}
  else {
  // Stop
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENB, 0); // speed up
  delay(10);
}

    // Control motor A (e.g., for forward/backward)
if (JoystickXposition < 2500) {
  // Move backward
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  analogWrite(PIN_ENA, 200); // speed up
  delay(10);

} 
else if (JoystickXposition > 3500) {
  // Move forward
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENA, 200); // speed up
  delay(10);
}
else {
  // Stop
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENA, 0); // speed up
  delay(10);
}

    //END LN298 DRIVER MOTOR CONTROL
  }
}
