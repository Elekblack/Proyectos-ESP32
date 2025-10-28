// ------
// Proyecto: Robot prototipo con ESP32
// ------
// Autor: ElekBlack
// Licencia: MIT
// ------
// ultima revicion: Noviembre 2024
// Desarrollado para: ESP32 DEVKIT V1 DOIT de 30 Pines 
// Componentes: Driver de motor L298N mini
//              Convertidor Voltaje DC-DC Step-Down LM2596
//              Display Oled 0.96" SPI 128*64 SSD1306
// ------

#include "BluetoothSerial.h"  // Biblioteca para comunicacion Bluetooth
#include <Wire.h>             // Biblioteca para comunicacion I2C
#include <Adafruit_GFX.h>     // Biblioteca para comunicacion con dispositivos Adafruit IO
#include <Adafruit_SSD1306.h> // Biblioteca para controlador de pantalla

// Configuracion de la pantalla OLED
#define ANCHO_PANTALLA 128 // ancho pantalla OLED
#define ALTO_PANTALLA 64   // alto pantalla OLED
#define OLED_RESET -1
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, OLED_RESET); // Objeto de la clase Adafruit_SSD1306
#define DIRECCION_PANTALLA 0x3C //Direccion i2c de la pantalla

// Configuración del Bluetooth
BluetoothSerial SerialBT;

// Pines del motor A
#define IN1 27
#define IN2 26

// Pines del motor B
#define IN3 25
#define IN4 33

// Pines de los encoders
#define ENCODER_A 35
#define ENCODER_B 34

// Pines de leds
#define PIN_LED_1 4
#define PIN_LED_2 2

// Variables globales para los contadores
volatile long encoderA_count = 0;
volatile long encoderB_count = 0;

// Funciones de interrupción para los encoders
void IRAM_ATTR encoderA_ISR() {
    encoderA_count++;
}

void IRAM_ATTR encoderB_ISR() {
    encoderB_count++;
}

void setup() {
  // Inicializar el Bluetooth
  SerialBT.begin("ESP32_Robot"); // Nombre del dispositivo Bluetooth
  
  // Inicializar comunicacion serie
  Serial.begin(115200);  

  // Configurar pines de los encoders
    pinMode(ENCODER_A, INPUT);
    pinMode(ENCODER_B, INPUT);

    // Adjuntar interrupciones a los encoders
    attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoderA_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCODER_B), encoderB_ISR, RISING);

// Iniciar pantalla OLED en la dirección 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, DIRECCION_PANTALLA)) {  // SSD1306_SWITCHCAPVCC: indica que se activa el voltaje de 3,3V interno para la pantalla.
    #ifdef __DEBUG__
      Serial.println("No se encuentra la pantalla OLED");
    #endif
    while (true);
  }

  // Configuración de pines para los led
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);

  // Configuración de pines para control de motores como salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Detener los motores al iniciar
  stopMotors();
  
  // enviar mensaje a la pantalla
  escribirPantalla(true, "Sistema iniciado");
}

void loop() {
  // Verificar si hay datos disponibles en el Bluetooth
  if (SerialBT.available()) {
    char commando = SerialBT.read(); // Leer el comando enviado

    // Ejecutar la acción basada en el comando
    switch (commando) {
      case 'F': // Adelante
        moveForward();
        break;
      case 'B': // Atrás
        moveBackward();
        break;
      case 'L': // Izquierda
        turnLeft();
        break;
      case 'R': // Derecha
        turnRight();
        break;
      case 'S': // Detener
        stopMotors();
        break;
      case '1': // Detener
        turnLeftEncoder(10);
        break;
      case '2': // Detener
        turnRightEncoder(10);
        break;
      default:
        escribirPantalla(false, "Comando no reconocido.");
        break;
    }
  }
}

// Funciones para controlar el movimiento
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  escribirPantalla(false, "Avanzando...");
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  escribirPantalla(false, "Retrocediendo...");
}

void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  digitalWrite(PIN_LED_1, HIGH);
  digitalWrite(PIN_LED_2, LOW);
  escribirPantalla(false, "Giro a la izquierda...");
}

void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  digitalWrite(PIN_LED_1, LOW);
  digitalWrite(PIN_LED_2, HIGH);
  escribirPantalla(false, "Giro a la derecha...");
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(PIN_LED_1, LOW);
  digitalWrite(PIN_LED_2, LOW);
  escribirPantalla(false, "Detenido.");
}

void turnLeftEncoder(long pulsos) {
  // Resetear contadores
    encoderA_count = 0;
    encoderB_count = 0;

  // Mientras no se alcance el objetivo
    while (abs(encoderA_count) < pulsos || abs(encoderB_count) < pulsos) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

      digitalWrite(PIN_LED_1, HIGH);
      digitalWrite(PIN_LED_2, LOW);
      escribirPantalla(false, "Girando pulsos");
    }
    stopMotors();
}

void turnRightEncoder(long pulsos) {
  // Resetear contadores
    encoderA_count = 0;
    encoderB_count = 0;

  // Mientras no se alcance el objetivo
    while (abs(encoderA_count) < pulsos || abs(encoderB_count) < pulsos) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

      digitalWrite(PIN_LED_1, LOW);
      digitalWrite(PIN_LED_2, HIGH);
      escribirPantalla(false, "Girando pulsos");
    }
    stopMotors();
}

void escribirPantalla(boolean limpiarBuffer, String texto) {
  // Limpiar buffer
  if (limpiarBuffer=true) {
    display.clearDisplay();
    display.setTextSize(1); // Tamaño del texto
    display.setTextColor(SSD1306_WHITE); // Color blanco
    display.setCursor(0, 10); // Posición del cursor (x, y)
  }
  // Muestra texto
  display.println(texto);
  // Muestra en la pantalla
  display.display();
}
