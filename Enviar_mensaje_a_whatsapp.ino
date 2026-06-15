/*
  Ejemplo: Envío de mensaje de WhatsApp con CallMeBot
  Placa: ESP32 (Arduino Core 3.x)

  Antes de usar:
   1. Añade el número +34 644 81 58 78 a tus contactos de WhatsApp.
   2. Envíale el mensaje: "I allow callmebot to send me messages"
   3. Te responderá con tu "apikey" personal.
*/

#include <WiFi.h>
#include "CallMeBotWhatsApp.h"

// Datos de coneccion wifi
const char* ssid     = "playCode";
const char* password = "12345678";

// Número con código de país, sin '+' ni espacios
const char* phone  = "51982536878";
const char* apikey = "1208816";

// Activamos el bot
CallMeBotWhatsApp bot(phone, apikey);

void setup() {
  // activamos los mensajes por puerto serial
  Serial.begin(115200);

  // conectamos al wifi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
   Serial.print(".");
  }
  Serial.println(" conectado!");

  // Mensaje con caracteres especiales: tildes, ñ, emojis, saltos de línea
   String mensaje = "¡Hola desde ESP32! 🚀\nEsto es una prueba con ñ, áéíóú y símbolos: #, &, =.";
  // String mensaje = "Inicio exitoso";

  int httpCode = bot.sendMessage(mensaje);

  Serial.print("Código HTTP de respuesta: ");
  Serial.println(httpCode);

  if (httpCode == 200) {
    Serial.println("Mensaje enviado correctamente.");
  } else 
    Serial.println("Error al enviar el mensaje.");
  }
}

void loop() {
}
