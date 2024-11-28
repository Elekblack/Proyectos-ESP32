/*  **************************************************************************
    **************************************************************************
    ultima revicion: Noviembre 2024

    Desarrollado para: ESP32 DEVKIT V1 DOIT de 30 Pines 
    Componentes: Driver de motor L298N mini
                 Convertidor Voltaje DC-DC Step-Down LM2596
                 Display Oled 0.96" SPI 128*64 SSD1306

    **************************************************************************
    ***************************** Copyright (C) ******************************
    **************************************************************************
    * Este código es de uso libre y puede ser copiado, modificado, 
    * distribuido o utilizado con cualquier propósito,
    * siempre y cuando se mantenga este aviso de copyright.
    * 
    * Este código se entrega "TAL CUAL", SIN GARANTÍA DE NINGÚN TIPO, 
    * ya sea expresa o implícita, incluyendo, entre otras, las garantías 
    * de comerciabilidad, idoneidad para un propósito particular 
    * y no infracción. 
    * 
    * El autor no se hace responsable por cualquier daño, pérdida o 
    * perjuicio derivado del uso de este código, ya sea directa o 
    * indirectamente.
    * 
    * ¡Úsalo bajo tu propio riesgo y disfruta programando!
    *
    **************************************************************************
*/

/* 
 *  Vamos a configurar el módulo Bluetooth HC-06 utilizando comandos AT enviados desde el monitor serie del IDE de Arduino. Una vez compilado y cargado el código en nuestro Arduino, 
 *  podemos abrir el monitor serie.
 *  Al enviar un comando AT, no se debe incluir ningún carácter de LF (nueva línea) ni CR (retorno de carro) al final del comando. En el monitor serie del IDE de Arduino, 
 *  debemos seleccionar «Sin ajuste de línea». Luego, escribimos el comando «AT» (siempre con mayúsculas) y pulsamos ENVIAR para comprobar que la comunicación es correcta. 
 *  el módulo Bluetooth nos responde con un OK.
 *  
 *  Por defecto nuestro módulo bluetooth se llama “HC-06” o “Linvor” esto se puede cambiar con el siguiente comando AT
 *  
 *  Enviar: AT+NAME<Nombre>   Ejm: AT+NAMERobot
 *  Respuesta: OKsetname
 *  
 *  El nombre puede ser de hasta 20 caracteres como máximo
 *  
 *  Por defecto viene con el código de vinculación (Pin) “1234”, para cambiarlo hay que enviar el siguiente comando AT
 *  
 *  Enviar: AT+PIN<Pin>   Ejm: AT+PIN1465
 *  
 *  Respuesta: OKsetPIN 
*/


#include <SoftwareSerial.h>   // Incluimos la librería  SoftwareSerial  
SoftwareSerial BT(10,11);    // Definimos los pines RX (11) y TX (10) del Arduino conectados al Bluetooth
 
void setup()
{
  BT.begin(9600);       // Inicializamos el puerto serie BT que hemos creado
  Serial.begin(9600);   // Inicializamos  el puerto serie  
}
 
void loop()
{
  if(BT.available())    // Si llega un dato por el puerto BT se envía al monitor serial
  {
    Serial.write(BT.read());
  }
 
  if(Serial.available())  // Si llega un dato por el monitor serial se envía al puerto BT
  {
     BT.write(Serial.read());
  }
}
