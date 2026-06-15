#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Pines (ajústalos si lo deseas)
int pinIN1 = 2;
int pinIN2 = 3;
int pinIN3 = 4;
int pinIN4 = 5;

char comando = 'S';

// UUID simples
#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-cba987654321"

BLECharacteristic *pCharacteristic;

void adelante() {
  digitalWrite(pinIN1, HIGH);
  digitalWrite(pinIN2, LOW);
  digitalWrite(pinIN3, HIGH);
  digitalWrite(pinIN4, LOW);
}

void atras() {
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, HIGH);
  digitalWrite(pinIN3, LOW);
  digitalWrite(pinIN4, HIGH);
}

void izquierda() {
  digitalWrite(pinIN1, HIGH);
  digitalWrite(pinIN2, LOW);
  digitalWrite(pinIN3, LOW);
  digitalWrite(pinIN4, HIGH);
}

void derecha() {
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, HIGH);
  digitalWrite(pinIN3, HIGH);
  digitalWrite(pinIN4, LOW);
}

void detener() {
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, LOW);
  digitalWrite(pinIN3, LOW);
  digitalWrite(pinIN4, LOW);
}

class Callbacks : public BLECharacteristicCallbacks {

  void onWrite(BLECharacteristic *pCharacteristic) {

    String valor = pCharacteristic->getValue();

    if (valor.length() > 0) {

      comando = valor[0];

      Serial.println(comando);

      switch (comando) {

        case 'F':
          adelante();
          break;

        case 'B':
          atras();
          break;

        case 'L':
          izquierda();
          break;

        case 'R':
          derecha();
          break;

        case 'S':
          detener();
          break;
      }
    }
  }
};

void setup() {

  Serial.begin(115200);

  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  pinMode(pinIN3, OUTPUT);
  pinMode(pinIN4, OUTPUT);

  detener();

  BLEDevice::init("RobotESP32C3");

  BLEServer *server = BLEDevice::createServer();

  BLEService *service =
      server->createService(SERVICE_UUID);

  pCharacteristic =
      service->createCharacteristic(
          CHARACTERISTIC_UUID,
          BLECharacteristic::PROPERTY_WRITE
      );

  pCharacteristic->setCallbacks(new Callbacks());

  service->start();

  BLEAdvertising *advertising =
      BLEDevice::getAdvertising();

  advertising->start();

  Serial.println("BLE listo");
}

void loop() {
}