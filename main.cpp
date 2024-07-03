#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_SHTC3.h>
#include <StarterKitNB.h>

//Definitions
#define PIN_VBAT WB_A0
#define VBAT_MV_PER_LSB (0.73242188F) // 3.0V ADC range and 12 - bit ADC resolution = 3000mV / 4096
#define VBAT_DIVIDER_COMP (1.73)      // Compensation factor for the VBAT divider, depend on the board (por default 1.73); 1905
#define REAL_VBAT_MV_PER_LSB (VBAT_DIVIDER_COMP * VBAT_MV_PER_LSB)

// Objects
StarterKitNB sk;
SHTC3 mySHTC3;

// NB-IoT
String apn = "m2m.entel.cl";
String user = "entelpcs";
String password = "entelpcs";
String band = "B28 LTE";
String Network = "NB";

// MQTT
String clientID = "111";
String userBroker = "martinmoreno";
String passwordBroker = "aio_qvkN13d4cIfduB8dYO0LTqbmBKFR";
String broker = "io.adafruit.com";
int port = 1883;

// msg
String topic1 = "martinmoreno/feeds/Temperatura";
String topic2 = "martinmoreno/feeds/Humedad";
String topic3 = "martinmoreno/feeds/battery";
String topic4;
float batery;             

// temp&hum
String hum = "";
String temp = "";
void errorDecoder(SHTC3_Status_TypeDef message)             
{
  switch(message)
  {
    case SHTC3_Status_Nominal : Serial.println("Nominal"); break;
    case SHTC3_Status_Error : Serial.println("Error"); break;
    case SHTC3_Status_CRC_Fail : Serial.println("CRC Fail"); break;
    default : Serial.println("Unknown return code"); break;
  }
}

void setup() {

  sk.Setup();
  delay(500);

  Wire.begin();
  errorDecoder(mySHTC3.begin()); 
  delay(500);

  sk.UserAPN(apn, user, password);
	sk.Connect(apn, band, Network);

  
}

void loop() {

  if (!sk.ConnectionStatus()) // Si no hay conexion a NB
	{
		sk.Reconnect(apn, band, Network);  // Se intenta reconecta
		delay(2000);
  }

  SHTC3_Status_TypeDef result = mySHTC3.update();
  hum = String(mySHTC3.toPercent());
  temp = String(mySHTC3.toDegC());
  batery = (analogRead(PIN_VBAT) * REAL_VBAT_MV_PER_LSB /1000);

  Serial.println(hum);
  Serial.println(temp);
  Serial.println(batery);

  if (!sk.LastMessageStatus)  // Para conectar al broker
  {
    sk.ConnectBroker(clientID, userBroker, passwordBroker, 0, broker, port);  
    delay(2000);
  }
  sk.SendMessage(temp, topic1, 0, 0, 0, 0, 10000);    // Se envia el mensaje
  delay(2000);
  sk.SendMessage(hum, topic2, 0, 0, 0, 0, 10000);    // Se envia el mensaje
  delay(2000);
  sk.SendMessage(String(batery), topic3, 0, 0, 0, 0, 10000);    // Se envia el mensaje
  delay(2000);
  


}