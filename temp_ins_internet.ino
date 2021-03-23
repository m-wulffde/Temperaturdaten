/**
Version 1.2
- Aktualisierung der Serial Print mit sinnvollen Erweiterungen
*/
// ### WIFI Stuff
#include <Arduino.h>
#include <ESP8266WiFi.h>

// ### Sensor Stuff
#include <OneWire.h>
#include <DallasTemperature.h>

// ### UDP Stuff, fÃ¼r NTP - Server
#include <WiFiUdp.h>

// ### Definitionen
#define ONE_WIRE_BUS 2                          // Digitalport auf Pin 2 definieren fÃ¼r den DS18B20 - Temperatursensor
const char* ssid     = "wulff";                 // SSID des WLAN's
const char* password = "3689483812276369";      // Passwort des WLAN's
const char* host     = "m-wulff.de";            // InternetdomÃ¤ne 
const char* link     = "/esp8266/input.php?t="; // Link auf Input.php mit Ubergabe 
const byte  pause    = 1;                       // Messpause in Minuten

OneWire ourWire(ONE_WIRE_BUS); // Ini oneWire-Instanz 
DallasTemperature sensors(&ourWire); // Dallas Temperature Library


// UDP - Server
unsigned int localPort = 2390;    
IPAddress timeServer(192, 53, 103, 108);  // ptbtime1.Ptb.De = 192, 53, 103, 108
const int NTP_PACKET_SIZE = 48; 
byte packetBuffer[NTP_PACKET_SIZE]; 
WiFiUDP udp;


void setup()
{
  // ### SERIAL
  Serial.begin(115200); // Serielle Verbindung aufbauen
  Serial.println();
  Serial.println("Starte Setup"); 
  
  // ### SENSOR ds18B20
  sensors.begin(); // Sensor Temperatursensor initialisieren 
  sensors.setResolution(TEMP_12_BIT); // Temperatursensor auf 12 BIT AuflÃ¶sung einstellen
  delay(500);
    
  // Infos rausschreiben
  Serial.println();
  Serial.print("Versuche mich mit dem folgenden Wifi zu verbinden: ");
  Serial.print(ssid);
  
  // ### WIFI Connect
  WiFi.begin(ssid, password);
    
  while (WiFi.status() != WL_CONNECTED) { // Whileschleife wird solange ausgefÃ¼hrt, bis das WLAN Verbunden ist
    delay(500); 
    Serial.print(".");
  }

  // Infos rausschreiben
  Serial.println("");
  Serial.println("WiFi Uplink laeuft");  
  Serial.print("Hier ist meine lokale IP-Adresse: ");
  Serial.println(WiFi.localIP());

  // Einmalig die Temperatur aus dem Sensor lesen
  sensors.requestTemperatures(); 

  float fCurrentTemperature = sensors.getTempCByIndex(0); // Temperatur in einer Float Variable speichern
  if ( fCurrentTemperature > -100 ) // Wenn die Messung kein Crap war, dann Schleife ausfÃ¼hren
  {
    // Infos rausschreiben
    Serial.print("Verbinden mit: ");
    Serial.println(host);

    // ### WIFIClient initialisieren
    WiFiClient client;
    if (client.connect(host, 80)) 
    {
      // URL die aufgerufen werden soll generieren
      String url = link;
      url += fCurrentTemperature;
      
      // infos rausschreiben
      Serial.print("URL Aufruf : ");
      Serial.println(url);
      
      // Einen HTTP GET Request generieren und abschicken
      client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +  "Connection: close\r\n\r\n");
      delay(10); // 10 milisekunden warten
      
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        String line = client.readStringUntil('\r');
        //Serial.print(line);
      }
      Serial.println("Website wurde aufgerufen, Verbindung schliessen");
      Serial.print("Aktuelle Temperatur ist: ");
      Serial.print(fCurrentTemperature);
      Serial.println(" Grad Celsius ");
      
    }else{
      // Error schreiben
      Serial.println("Verbindung unterbrochen"); 
    }

  }else{
    // Error, wenn der Sensor einen falschen Wert ausgibt!
    Serial.println("Error: PrÃ¼fen den DS18B20 Anschluss - Temperatur liegt unter -100 -");
  }
  
  Serial.println("--- Jetzt muss ich ein Nickerchen machen ---");

  // NTP - Server Zeit bis zur nÃ¤chsten Messung
  udp.begin(localPort);
  sendNTPpacket(timeServer); 
  delay(1000);

  int cb = udp.parsePacket();  
    udp.read(packetBuffer, NTP_PACKET_SIZE); 
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;
    int stund  = (epoch  % 86400L) / 3600 + 1;
    long Sek   = (epoch  % 3600);
   
    long schlafen = 3600 - Sek;  // Soviel Sekunden sind es noch bis zur vollen Stunde
    schlafen += 25; // 25 Sekunden drauflegen damit die Volle Stunde auch sicher erreicht wird.
    Serial.print(schlafen);  
  
  ESP.deepSleep(schlafen * 1000000); // 1000000 Microseconds = 1 Sekunde
 
}
void loop(){
}

unsigned long sendNTPpacket(IPAddress& address)
{
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
