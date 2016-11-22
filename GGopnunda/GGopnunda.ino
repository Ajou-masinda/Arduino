#include <ESP8266WiFi.h>

#define SSID "NAM"
#define PASS "dnwls222"
#define DST_IP "192.168.0.197" //Deudnunda
#define DST_PORT 3030
#define PIN 13 // GPIO13 --> D7

uint8_t MAC_array[6];
char MAC_char[18];

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(PIN, OUTPUT);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.macAddress(MAC_array);
  for (int i = 0; i < sizeof(MAC_array); ++i){
    sprintf(MAC_char,"%s%02x:",MAC_char,MAC_array[i]);
  }
  
  Serial.println(MAC_char);

  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // This is your NodeMCU IP address. Could be handy for other projects

  // Serial.println("ESP8266 in sleep mode");
  // Put NodeMCU in deep sleep. When it wakes up it will run setup() again,
  // connect to WiFi, then post and/or get data, then go back to sleep and repeat
}

void loop()
{
  POST();
}

void POST(void)
{
  delay(2000);  // You can get rid of this or decrease it

  Serial.print("connecting to ");
  Serial.println(DST_IP);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(DST_IP, DST_PORT)) {
    Serial.println("connection failed");
    return;
  }
  String pubString = "{\"MAC\": \"" + MAC_char + "\", \"REQ\": \"GET_COMMAND\"}";
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
  
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    line.trim();
    Serial.print(line);
    if(line == "ON") {
      Serial.print("CMD : ON");
      digitalWrite(13, HIGH);
    }
    else {
      Serial.print("CMD : OFF");
      digitalWrite(13, LOW);
    }
  }
  Serial.println();
  Serial.println("closing connection");
}
