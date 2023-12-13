/*
 * Smartphone Controlled Robot Car
 * By Lisa Fung and Esmeé Cowing
 * December 5, 2023
 * 
 * Controlling left and right side motors separately with only one motor controller.
 * The two robot car motors on each side are connected in parallel and receive the same voltages.
 * Purpose of change: Free up D# pins to control the speed so the robot car can move in straighter line.
 *
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Arduino (GPIO) to NodeMCU (D#) pin translations
const int _D0 = 16;
const int _D1 = 5;
const int _D2 = 4;
const int _D3 = 0;
const int _D4 = 2;
const int _D5 = 14;
const int _D6 = 12;
const int _D7 = 13;
const int _D8 = 15;
const int _RX = 3;
const int _TX = 1;
const int _SD2 = 9;


// NodeMCU to component translations
// motor1 for left motors, motor2 for right motors
const int motor1pin1 = _D2;
const int motor1pin2 = _D1;
const int motor2pin1 = _D4;
const int motor2pin2 = _D3;

// ENA pins for controlling speed (A for left, B for right)
const int ENA = _D5;
const int ENB = _D6;

int speedLeft = 255;    // ENA
int speedRight = 255;   // ENB

// WiFi soft access point credentials
const char* ssid = "NodeMCU_Car";
const char* password = "12345678";

ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);

  // Motor setup
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  // Setup NodeMCU Soft Access Point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Configure web page
  server.on("/", []() {
    String html = "<html><head><style>";
    html += "button { width: 50px; height: 50px; border: none; color: white; padding: 15px 0; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }";
    html += ".forward { background-color: #4CAF50; }";
    html += ".backward { background-color: #f44336; }";
    html += ".left { background-color: #2196F3; }";
    html += ".right { background-color: #FFC107; }";
    html += ".stop { background-color: #9E9E9E; }";
    html += ".control-group { text-align: center; }";
    html += "</style></head><body>";
    html += "<h1>NodeMCU Car Control</h1>";
    html += "<div class='control-group'>";
    html += "<button class='forward' onclick=\"location.href='/forward'\">&#8593;</button><br>";
    html += "<button class='left' onclick=\"location.href='/left'\">&#8592;</button>";
    html += "<button class='stop' onclick=\"location.href='/stop'\">Stop</button>";
    html += "<button class='right' onclick=\"location.href='/right'\">&#8594;</button><br>";
    html += "<button class='backward' onclick=\"location.href='/backward'\">&#8595;</button>";
    html += "</div>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/forward", []() {
    moveForward();
    server.sendHeader("Location", "/", true);
    server.send(303, "text/plain", "Moving Forward");
  });

  server.on("/backward", []() {
    moveBackward();
    server.sendHeader("Location", "/", true);
    server.send(303, "text/plain", "Moving Backward");
  });

  server.on("/left", []() {
    moveLeft();
    server.sendHeader("Location", "/", true);
    server.send(303, "text/plain", "Turning Left");
  });

  server.on("/right", []() {
    moveRight();
    server.sendHeader("Location", "/", true);
    server.send(303, "text/plain", "Turning Right");
  });

  server.on("/stop", []() {
    stopCar();
    server.sendHeader("Location", "/", true);
    server.send(303, "text/plain", "Stopped");
  });

  // Start server
  server.begin();
  Serial.print("Server started!");
}


// Respond to client requests (smartphone/web controls)
void loop() {
  server.handleClient();
}


// Functions to control car movement

void moveForward() {
  // Logic to move forward
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);

  analogWrite(ENA, speedLeft);
  analogWrite(ENB, speedRight);
}

void moveBackward() {
  // Logic to move backward
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);

  analogWrite(ENA, speedLeft);
  analogWrite(ENB, speedRight);
}

void moveLeft() {
  // Logic to turn left
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);

  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  
  analogWrite(ENA, speedLeft);
  analogWrite(ENB, speedRight);
}

void moveRight() {
  // Logic to turn right
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);

  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

  analogWrite(ENA, speedLeft);
  analogWrite(ENB, speedRight);
}

void stopCar() {
  // Logic to stop car
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
}
