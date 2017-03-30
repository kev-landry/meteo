/*
 * *************************
 * Capteur : DHT11
 *
 * Les broches sont numÃ©rotÃ©es de la gauche vers la droite lorsque l'on regarde le capteur de face
 *
 * Broche nÂ°1 connectÃ©e au +3.3V
 * Broche nÂ°2 (data) connectÃ©e Ã  la broche 'D4' du NodeMcu (Pin 2 pour l'arduino) avec une rÃ©sistance de 10 K reliÃ©e au +3.3v
 * Broche nÂ°3 non connectÃ©e
 * Broche nÂ°4 connectÃ©e Ã  la masse (GND)
 *
 * Installer la bibliothÃ¨que 'DHT sensor library by Adarftuit'
 * Cette librairie necessite une librairie supplÃ©mentaire qui est disponible Ã  l'adresse : https://github.com/adafruit/Adafruit_Sensor
 * et qui doit Ãªtre installÃ©e selon ce tutoriel : https://learn.adafruit.com/adafruit-all-about-arduino-libraries-install-use/installing-a-library-on-linux
 *
 */

// DÃ©claration des librairies
#include <DHT.h>                      // pour communiquer avec le capteur DHT
#include <ESP8266WiFi.h>              // pour le support du wifi
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>        // pour utliser le wifi

// PrÃ©paration du capteur DHT
#define DHTPIN 2                      // broche sur laquelle est raccordÃ©e la data du capteur (la broche 'D4' du NodeMcu correspond Ã  la broche 2 de l'arduino)
#define DHTTYPE DHT11                 // prÃ©cise la rÃ©fÃ©rence du capteur DHT (DHT11 ou DHT21 ou DHT22)
DHT dht(DHTPIN, DHTTYPE);             // Initialisation du capteur DHT

// Initialisation du wifi
ESP8266WiFiMulti WiFiMulti;

// fonction de dÃ©marrage
void setup() {

  // DÃ©marrage du bus sÃ©rie
  Serial.begin(115200);               // vitesse
  Serial.println("Bonjour");          // Ã©criture d'un petit message...
  Serial.println("DHT11 + IFTTT + Google spreadsheets");
  Serial.println("");

  dht.begin();                        // DÃ©marrage du capteur DHT11

  WiFiMulti.addAP("ssid", "mdp");     // Connexion au rÃ©seau wifi

}

// boucle infinie
void loop() {

  delay(10000);                        // attendre 10 secondes

  float t = dht.readTemperature();    // mesurer la tempÃ©rature (en Â° Celsius)
  float h = dht.readHumidity();       // mesurer l'humiditÃ© (en %)

  if (isnan(h) || isnan(t)) {         // test si des valeurs ont Ã©tÃ© rÃ©cupÃ©rÃ©es

    // s'il y a un problÃ¨me...

    Serial.println("Failed to read from DHT sensor!");    // affiche un message d'erreur
    return;                           // quitte pour retenter une lecture

  } else {

    // si tout va bien...

    // conversion des valeurs en entier
    int temp = int(t);
    int humi = int(h);

    // affichage des valeurs dans le bus sÃ©rie
    Serial.print("Temperature : ");
    Serial.print(temp);
    Serial.print(" *C\t");
    Serial.print("HumiditÃ© : ");
    Serial.print(humi);
    Serial.println(" %");

    // envoi des donnÃ©es vers internet
    if((WiFiMulti.run() == WL_CONNECTED)) {   // Si le wifi est connectÃ©

          HTTPClient http;                    // prÃ©paration d'une requÃ¨te HTTP au serveur

          Serial.print("HTTP begin... ");
          http.begin("http://maker.ifttt.com/trigger/temp_request/with/key/xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");   // connexion au serveur

          http.addHeader("Content-Type", "application/json");    // envoi d'un entÃªte pour prÃ©ciser notre format de donnÃ©es : json

          String value = "{\"value1\":\"" + String(temp) + "\",\"value2\":\"" + String(humi) + "\"}";    // prÃ©paration du json {"value1":"22","value2":"55"}

          int httpCode = http.POST(value);    // envoi les donnÃ©es et rÃ©cupÃ¨re un code de retour

          if(httpCode == 200 ) {              // si le code de retour est bon (200)
             Serial.println("POST : ok");
          } else {                            // si le code de retour n'est pas bon (diffÃ©rent de 200)
             Serial.print("POST : failed, error : ");
             Serial.println(http.errorToString(httpCode).c_str());
          }
          http.end();                    // on ferme la connexion au serveur
    }
  }
}
