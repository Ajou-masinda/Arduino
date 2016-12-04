#include <ESP8266WiFi.h>

#define SSID "DEUDNUNDA"
#define PASS "1q2w3er4"
#define DST_IP "192.168.0.197" //Deudnunda
#define DST_PORT 3030
#define PIN 13 // GPIO13 --> D7
#define SERIAL "KVN46EF2D4XC"

String MAC_str = "";
String AP_IP = "";
boolean REGISTER = false;

void setup() {
  Serial.begin(115200);
  delay(100);

  initVariant();

  pinMode(PIN, OUTPUT);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  unsigned char MAC_array[6];
  WiFi.macAddress(MAC_array);

  for (int i = 0; i < 6; ++i) {
    MAC_str += String(MAC_array[i], 16);
    if (i < 5) MAC_str += ':';
  }
  
  Serial.println(MAC_str);
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // This is your NodeMCU IP address. Could be handy for other projects
}

void loop()
{
  delay(2000);  // You can get rid of this or decrease it
  if(!REGISTER) {
    checkRegistred();
    return;
  }

  String pubString = "{\"MAC\": \"" + MAC_str + "\",\"IP\": \"" + ipToString(WiFi.localIP()) + "\", \"REQ\": \"GET_COMMAND\", \"SERIAL\": \"" + SERIAL + "\"}";
  String res = POST(pubString);
  Serial.println(res);
  if(res.length() > 0 ) {
    doCommand(res);
  }
}

String POST(String pubString)
{
  Serial.print("connecting to ");
  Serial.println(AP_IP);

  char charBuf[16];
  AP_IP.toCharArray(charBuf, 16);
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(charBuf, DST_PORT)) {
    Serial.println("connection failed");
    return "CON_FAIL";
  }

  String pubStringLength = String(pubString.length(), DEC);
  
  // We now create a URI for the request
  Serial.print("Requesting POST: ");
  // Send request to the server:
  client.println("POST /ggopnunda HTTP/1.1");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(pubStringLength);
  client.println();
  client.print(pubString);
  client.println();
  delay(500); // Can be changed

  String res = "";
  boolean start = false;
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    line.trim();
    Serial.println(line);
    
    if(line == "") {
      start = true;
      continue;
    }
    if(start) {
      res = line;
      return res;
    }
  }
  Serial.println();
  Serial.println("closing connection");

  return res;
}

void checkRegistred() 
{
  String pubString = "{\"MAC\": \"" + MAC_str + "\",\"IP\": \"" + ipToString(WiFi.localIP()) + "\", \"REQ\": \"REGISTER\", \"SERIAL\": \"" + SERIAL + "\"}";
  Serial.println(WiFi.gatewayIP());
  AP_IP = ipToString(WiFi.gatewayIP());

  String res = "";
  res = POST(pubString);

  if(res == "OK") {
    REGISTER = true;
    Serial.println("REGISTERED!!!");
  }
  return ;
}

void doCommand(String command)
{
    if(command == "ON") {
      Serial.print("CMD : ON");
      digitalWrite(13, HIGH);
    }
    else if(command == "OFF") {
      Serial.print("CMD : OFF");
      digitalWrite(13, LOW);
    }
}

String ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}
