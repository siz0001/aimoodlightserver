
void FirebaseSetup()
{
  // firebase begin after wifi connect
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Serial.println("firebase send");
  Serial.println(macID);
  Serial.println(WiFi.localIP().toString());
  Firebase.setString(firebaseData, "/Controller/" + macID + "/version", version);
  Firebase.setString(firebaseData, "/Controller/" + macID + "/ID", macID);
  Firebase.setString(firebaseData, "/Controller/" + macID + "/IP", WiFi.localIP().toString());
  Firebase.getString(ledData, "/Controller/" + macID + "/ledData");
  Serial.println(ledData.stringData());
}

void DisplaySetup()
{
  // LCD begin
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.display();
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.setCursor(32, 22);
  display.print(F("IP Address: "));
  display.setCursor(32, 32);
  display.print(WiFi.localIP());
  display.print(F(":81"));
  display.setCursor(32, 42);
  display.print(F("MAC ID: "));
  display.setCursor(32, 52);
  display.print(macID);
  display.startscrollleft(0x00, 0x07);
  display.display();
}

void GetExternalIP()
{
  String str = "";
  WiFiClient client;
  if (!client.connect("api64.ipify.org", 80))// Change address below when error is happen
  // if (!client.connect("ipv4.seeip.org/jsonip/", 80))
  {
    Serial.println("Failed to connect with 'api64.ipify.org' !");
  }
  else
  {
    int timeout = millis() + 5000;
    client.print("GET /?format=json HTTP/1.1\r\nHost: api64.ipify.org\r\n\r\n"); // Change address below when error is happen
    // client.print("GET /?format=json HTTP/1.1\r\nHost: ipv4.seeip.org/jsonip/\r\n\r\n");
    while (client.available() == 0)
    {
      if (timeout - millis() < 0)
      {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    char pubIp[17], pubIp_s, pubIp_e;
    int size;
    while ((size = client.available()) > 7)
    {
      uint8_t *msg = (uint8_t *)malloc(size);
      size = client.read(msg, size);
      Serial.write(msg, size);
      int i = 0;
      while (i < size)
      {
        if (msg[i] == '{')
        {
          int currChIp = i + 7, j = 0;
          pubIp_s = (char)msg[currChIp];
          while (msg[currChIp] != '}')
          {
            pubIp_e = (char)msg[currChIp - 1];
            pubIp[j] = (char)msg[currChIp];
            currChIp++;
            j++;
          }
          pubIp[j - 1] = '\0';
          Serial.println(pubIp);
          Firebase.setString(firebaseData, "/Controller/" + macID + "/PhysicalIP", pubIp);
          break;
        }
        i++;
      }
      free(msg);
    }
  }
}
