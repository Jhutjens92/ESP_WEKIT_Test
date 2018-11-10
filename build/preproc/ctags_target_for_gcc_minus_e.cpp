# 1 "c:\\Users\\Jordi Hutjens\\Documents\\GitHub\\ESP_WEKIT_Test\\WEKIT_V1.0_OUNL_Light.ino"
# 1 "c:\\Users\\Jordi Hutjens\\Documents\\GitHub\\ESP_WEKIT_Test\\WEKIT_V1.0_OUNL_Light.ino"
/*

 *  This sketch sends random data over UDP on a ESP32 device

 *

 */
# 5 "c:\\Users\\Jordi Hutjens\\Documents\\GitHub\\ESP_WEKIT_Test\\WEKIT_V1.0_OUNL_Light.ino"
# 6 "c:\\Users\\Jordi Hutjens\\Documents\\GitHub\\ESP_WEKIT_Test\\WEKIT_V1.0_OUNL_Light.ino" 2
# 7 "c:\\Users\\Jordi Hutjens\\Documents\\GitHub\\ESP_WEKIT_Test\\WEKIT_V1.0_OUNL_Light.ino" 2
# 8 "c:\\Users\\Jordi Hutjens\\Documents\\GitHub\\ESP_WEKIT_Test\\WEKIT_V1.0_OUNL_Light.ino" 2
# 9 "c:\\Users\\Jordi Hutjens\\Documents\\GitHub\\ESP_WEKIT_Test\\WEKIT_V1.0_OUNL_Light.ino" 2
# 10 "c:\\Users\\Jordi Hutjens\\Documents\\GitHub\\ESP_WEKIT_Test\\WEKIT_V1.0_OUNL_Light.ino" 2
# 11 "c:\\Users\\Jordi Hutjens\\Documents\\GitHub\\ESP_WEKIT_Test\\WEKIT_V1.0_OUNL_Light.ino" 2
# 12 "c:\\Users\\Jordi Hutjens\\Documents\\GitHub\\ESP_WEKIT_Test\\WEKIT_V1.0_OUNL_Light.ino" 2

// WiFi network name and password:
const char * networkName = "SPU_WiFi";
const char * networkPswd = "12345678";

//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
const char * udpAddress = "192.168.137.1";
const int udpPort = 5005;

// Global Vars
boolean connected = false;
const size_t bufferSize = 2*((2) * sizeof(ArduinoJson650_0_0::Slot)) + 2*((2) * sizeof(ArduinoJson650_0_0::Slot)) + ((6) * sizeof(ArduinoJson650_0_0::Slot)) + 2*((13) * sizeof(ArduinoJson650_0_0::Slot));

// IMUs
#define MPUS 2
MPU9250 mpu9250_0;
MPU9250 mpu9250_1;

// IMU Vars
float aX0, aY0, aZ0, aSqrt0, gX0, gY0, gZ0, mDirection0, mX0, mY0, mZ0;
float aX1, aY1, aZ1, aSqrt1, gX1, gY1, gZ1, mDirection1, mX1, mY1, mZ;

// DHT22 Vars
float dht22_Humidity0;
float dht22_Temp0;
float dht22_Humidity1;
float dht22_Temp1;

// GSR Vars
int gsr;

// Pulse Vars
int pulse;

//The udp library class
WiFiUDP udp;
DHTesp dht;

// Pin Defines

#define PULSE_PIN 39
#define GSR_PIN 36
#define SDA_IMU0_PIN 5
#define SCL_IMU0_PIN 4
#define SDA_IMU1_PIN 25
#define SCL_IMU1_PIN 26
#define DHT0_PIN 2

//  defines 
#define DHTTYPE DHT22

void setup(){
  // Initilize hardware serial:
  Serial.begin(115200);

  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
  pinMode(36, 0x01);
  pinMode(39, 0x01);

  Wire.begin(5, 4);
  Wire1.begin(25, 26);

}

void loop(){

}


//Sends the data to the UDP Server
void SendData(){
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
void WiFiEvent(system_event_id_t event){
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
