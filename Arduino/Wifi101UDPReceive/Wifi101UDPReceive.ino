#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>
#include <OSCBoards.h>

char ssid[] = "yourssid";       // your network SSID (name)
char pass[] = "yourpassword";   // your network password
int status = WL_IDLE_STATUS;    // the WiFi radio's status

unsigned int localPort = 0000;  // local port to listen on

WiFiUDP Udp;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // Check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // Attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // Wait 10 seconds for connection:
    delay(10000);
  }

  // You're connected now, so print out the data:
  Serial.println();
  Serial.println("You're connected to the network");
  Serial.println();
  printCurrentNet();
  printWiFiData();

  Udp.begin(localPort);
  Serial.print("Listening on port: ");
  Serial.println(localPort);
  Serial.println();
}

void loop() {
  OSCMessage msg;
  int size;
  
  if( (size = Udp.parsePacket())>0) {
    while(size--)
      msg.fill(Udp.read());
    
    if(!msg.hasError())
      msg.dispatch("/wek/outputs", dispatchAddress);
  }
}

void dispatchAddress(OSCMessage &msg){
  //do something with the OSCMessage...
  if (msg.isFloat(0)){
    float val = msg.getFloat(0);
    Serial.print("Message is float: ");
    Serial.println(val);
  }
}

void printWiFiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
  Serial.println();
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type: ");
  Serial.println(encryption, HEX);
  Serial.println();
}
