#include <SPI.h>
#include <Ethernet.h>

// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xFD, 0xAE, 0x2F, 0x1E, 0xE0 };
// IP address for the shield
IPAddress ip(192, 168, 1, 80);

// Initialize the Ethernet server library
EthernetServer server(80);

void setup() {
  // Open serial communications
  Serial.begin(9600);

  // Start the Ethernet connection and the server
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // Listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("New client");
    // An http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // If the newline is received, send a standard HTTP response,
        // then output the value of the potentiometer
        if (c == '\n' && currentLineIsBlank) {
          // Send HTTP header
          // Send a standard HTTP response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println("Connection: close");
          client.println("Access-Control-Allow-Origin: *"); // Allow all origins
          client.println();

          // Send the JSON content
          client.print("{ \"a0\": ");
          client.print(analogRead(A0));
          client.println(" }");
          break;
        }
        if (c == '\n') {
          // Starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // A character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // Give the client time to receive the data
    delay(1);
    // Close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
}
