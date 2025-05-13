#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "Newton 3rd law";
const char* password = "MdALRIAD#2201042";

// Define the GPIO pin for the relay
const int relayPin = 14; // Change to the GPIO pin you're using

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // Set relay pin as output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Start with the relay OFF

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
}

void loop() {
  WiFiClient client = server.available(); // Listen for incoming clients

  if (client) {
    Serial.println("New client connected");
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Parse the request
    if (request.indexOf("/ON") != -1) {
      digitalWrite(relayPin, LOW); // Turn relay ON
    } else if (request.indexOf("/OFF") != -1) {
      digitalWrite(relayPin, HIGH); // Turn relay OFF
    }

    // Send response with CSS
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<style>");
    client.println("body { font-family: Arial, sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; background-color: #f4f4f4; }");
    client.println("h1 { text-align: center; color: #333; }");
    client.println(".container { text-align: center; }");
    client.println("button { font-size: 20px; padding: 15px 30px; margin: 15px; border: none; border-radius: 8px; cursor: pointer; }");
    client.println("button:hover { background-color: #ddd; }");
    client.println(".on { background-color: #4CAF50; color: white; }");
    client.println(".off { background-color: #f44336; color: white; }");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");
    client.println("<div class=\"container\">");
    client.println("<h1>ESP32 Relay Control</h1>");
    client.println("<p><a href=\"/ON\"><button class=\"on\">Turn OFF</button></a></p>");
    client.println("<p><a href=\"/OFF\"><button class=\"off\">Turn ON</button></a></p>");
    client.println("</div>");
    client.println("</body>");
    client.println("</html>");
    client.stop();
    Serial.println("Client disconnected");
  }
}


