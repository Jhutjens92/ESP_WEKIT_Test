/*
 *  This sketch sends random data over UDP on a ESP32 device
 *
 */
#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi network name and password:
const char * networkName = "SPU_WiFi";
const char * networkPswd = "12345678";

//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
const char * udpAddress = "192.168.137.1";
const int udpPort = 5005;

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

void setup(){
  // Initilize hardware serial:
  Serial.begin(115200);
  
  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
}

void loop(){
  //only send data when connected
  if(connected){
    char msg[420];
    //Send a packet
    udp.beginPacket(udpAddress,udpPort);
    String message = "{\"client\":\"WEKIT-VEST-000014A2\",\"time\":992953,\"imus\":[{\"ax\":-0.04,\"ay\":1.25,\"az\":-0.06,\"gx\":-7.33,\"gy\":-171.49,\"gz\":-7.33,\"mx\":-14.29,\"my\":5.39,\"mz\":-3.46,\"q0\":-0.26,\"q1\":-0.40,\"q2\":0.57,\"q3\":0.67},{\"ax\":-0.05,\"ay\":1.23,\"az\":-0.34,\"gx\":-7.43,\"gy\":-171.434,\"gz\":-7.34,\"mx\":-14.39,\"my\":5.39,\"mz\":-3.36,\"q0\":-0.36,\"q1\":-0.30,\"q2\":0.37,\"q3\":0.63}],\"shts\":[{\"temp\":33,\"hum\":80},{\"temp\":21,\"hum\":55}],\"pulse\":60,\"gsr\":1024}"; 
    message.toCharArray(msg,sizeof(msg));
    udp.print(msg);
    udp.endPacket();
  }
  //Wait for 1 second
  delay(1000);
}

void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));
  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(),udpPort);
          connected = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          break;
    }
}
