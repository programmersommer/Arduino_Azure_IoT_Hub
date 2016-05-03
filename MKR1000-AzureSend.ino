// Push data from MRK1000 to Azure IoT Hub
#include <SPI.h>
#include <WiFi101.h>


// Generate SAS key using https://github.com/Azure/azure-iot-sdks/releases
char hostname[] = "alexey.azure-devices.net";
char authSAS[] = "SharedAccessSignature sr=alexey.azure-devices.net%2fdevices%2fmyDevice&sig=D7OxGEm98bqAQDYk33d0DzPB92EuGMkjkzKBCsBBksc%3d&se=1493799405";

String deviceName = "myDevice";
char ssid[] = "myhomenet";      // your network SSID (name)
char pass[] = "password123";   // your network password
String uri = "/devices/myDevice/messages/events?api-version=2016-02-03";

int status = WL_IDLE_STATUS;
WiFiSSLClient client;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup begin");

   // check for the presence of the shield:
   if (WiFi.status() == WL_NO_SHIELD) {
     Serial.println("WiFi shield not present");
     // don't continue:
     while (true);
   }

   // attempt to connect to Wifi network:
   while ( status != WL_CONNECTED) {
     Serial.print("Attempting to connect to SSID: ");
     Serial.println(ssid);
     status = WiFi.begin(ssid, pass);

     // wait 10 seconds for connection:
     delay(10000);
   }
  
  Serial.println("Connected to Wi-Fi");
}

void loop() {

  httpPost("Some message from Arduino"); // this string will be send to Azure

  String response = "";
  char c;
  while (client.available()) {
    c = client.read();
    response.concat(c);
  }

  if (!response.equals("")) 
  {
    if (response.startsWith("HTTP/1.1 204")) {
      Serial.println("Data was send to Azure"); 
    } else {
      Serial.println("Error");
      Serial.println(response);
    }
  }
 
  delay(1000);
}


void httpPost(String content)
{
    // close any connection before send a new request.
    client.stop();
  
    if (client.connectSSL(hostname, 443)) {
      client.print("POST ");
      client.print(uri);
      client.println(" HTTP/1.1"); 
      client.print("Host: "); 
      client.println(hostname);
      client.print("Authorization: ");
      client.println(authSAS); 
      client.println("Connection: close");

      client.print("Content-Type: ");
      client.println("text/plain");
      client.print("Content-Length: ");
      client.println(content.length());
      client.println();
      client.println(content);
      
      delay(200);
      
    } else {
      Serial.println("HTTP POST connection failed");
    }
}

