//
// Esempio di utilizzo di Bot Telegram con ESP32
//
// maurizio.conti@fablabromagna.org
// 20 Aprile 2019
//
// dipendenze:
//
// libreria UniversalTelegramBot di Gianbacchio (adapted by Brian Lough )
// https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
//
// Libreria ArduinoJson versione 5.13.4 (non aggiornare alla 6.x
// https://arduinojson.org/
//
// Installare con il library manager nel file platformio.ini con questo comando
// lib_deps = ArduinoJson@5.13.4, UniversalTelegramBot
//

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Credenziali WiFi
char ssid[] = ""; 
char password[] = "";

// Telegram Token da Botfather
#define BOTtoken ""

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Polling ogni mezzo secondo...
int Bot_mtbs = 500;
long Bot_lasttime;

void setup() {
  Serial.begin( 115200 );

  // Si connette alla rete:
  Serial.print("Mi connetto a ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Attende di collegarsi...
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("Connesso con IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    Serial.print(".");

    // Vado a vedere se ci sono messaggi.
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    // Li processo.
    while( numNewMessages ) {
      
      for (int i=0; i<numNewMessages; i++) {
        bot.sendMessage( bot.messages[i].chat_id, bot.messages[i].text, "" );
        
        Serial.println( "\ndate: " + bot.messages[i].date );
        Serial.print( "id: " + bot.messages[i].chat_id);
        Serial.println( " (type: " + bot.messages[i].type + ")" );
        Serial.println( "text: " + bot.messages[i].text);
        Serial.println( "from: " + bot.messages[i].from_id + " (" + bot.messages[i].from_name + ")" );
        Serial.print( "long: " );
        Serial.print( bot.messages[i].longitude );
        Serial.print( ", lat: " );
        Serial.println( bot.messages[i].latitude );
        Serial.print( "---\n" );
        
      }
      numNewMessages = bot.getUpdates( bot.last_message_received + 1 );
    }

    Bot_lasttime = millis();
  }
}
