/**
Version 1.0
- Aktualisierung der Serial Print mit sinnvollen Erweiterungen
*/
// ### WIFI Stuff
#include <Arduino.h>
#include <ESP8266WiFi.h>

// ### Sensor Stuff
#include <OneWire.h>
#include <DallasTemperature.h>

// ### Definitionen
#define ONE_WIRE_BUS 2                          // Digitalport auf Pin 2 definieren fÃ¼r den DS18B20 - Temperatursensor
const char* ssid     = "wulff";                 // SSID des WLAN's
const char* password = "3689483812276369";      // Passwort des WLAN's
const char* host     = "m-wulff.de";            // InternetdomÃ¤ne 
const char* link     = "/esp8266/input.php?t="; // Link auf Input.php mit Ubergabe 
const byte  pause    = 1;                       // Messpause in Minuten

OneWire ourWire(ONE_WIRE_BUS); // Ini oneWire-Instanz 
DallasTemperature sensors(&ourWire); // Dallas Temperature Library

void setup() {

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
}

void loop() 
{
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
  delay(pause * 60 * 1000);
 
}
