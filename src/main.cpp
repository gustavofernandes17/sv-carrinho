#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <DNSServer.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <BlynkSimpleEsp32.h>

#include "Receptor.h"
#include "StateManager.h"
#include "EnginesController.h"

#include "camera_config.h"

// pinos que vao controlar os motores
#define PINO_MOTOR_SENTIDO_HORARIO 12
#define PINO_MOTOR_SENTIDO_ANTI_HORARIO 13

// pinos que vao controlar os servos
#define PINO_SERVO_1_BASE 2 
#define PINO_SERVO_2_CORPO 15


// Blynk e rede WIFI
char auth[] = "6Ck3mKUw32pM8TGE8bdNyfip6lxSt4Bq";
char ssid[] = "Redmi Note 7";
char pass[] = "b6ee8b98282c";

// broker
char* broker = "broker.hivemq.com";
int port = 1883;
char* device_id = "SV-CARRINHO";


WiFiClient client_wifi; 
PubSubClient mqtt_client(client_wifi);
Receptor receptor(mqtt_client); 

EnginesController engines_controller(
  PINO_MOTOR_SENTIDO_ANTI_HORARIO,
  PINO_MOTOR_SENTIDO_HORARIO,
  PINO_SERVO_1_BASE,
  PINO_SERVO_2_CORPO
);

StateManager state_manager(&engines_controller);

// função teste
void data_reception_callback(char *topic, uint8_t *payload, unsigned int lenght); 

void setup() {

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  Blynk.begin(auth, ssid, pass, "blynk.iot-cm.com", 8080); 

  receptor.setup(broker, port, device_id, data_reception_callback);
  engines_controller.setup();

  setup_camera();
}

void loop() {

  Blynk.run();
  mqtt_client.loop(); 

}

BLYNK_WRITE(V0) {
  state_manager.set_move_right(1);
}

BLYNK_WRITE(V1) {
  state_manager.set_move_left(1);
}

BLYNK_WRITE(V2) {
  state_manager.set_base_potentiometer(param.asInt());
}

BLYNK_WRITE(V3) {
  state_manager.set_body_potentiometer(param.asInt());
}


void data_reception_callback(char *topic, uint8_t *payload, unsigned int lenght) 
{
  String msg; 

  for (int i = 0; i < lenght; i++)
  {
    msg += (char) payload[i];
  }
  
  String string_topic = String(topic); 
  int value_received = msg.toInt();

  if (string_topic.equals(receptor.base_potentiometer_channel))
  {
    state_manager.set_base_potentiometer(msg.toInt());
  } 

  if (string_topic.equals(receptor.body_potentiometer_channel))
  {
    Serial.println("Pot Corpo"); 
    state_manager.set_body_potentiometer(value_received);
  }

  if (string_topic.equals(receptor.move_engine_to_right_channel))
  {
    Serial.println("movendo para a direita");
    state_manager.set_move_right(value_received); 

  }

  if (string_topic.equals(receptor.move_engine_to_left_channel))
  {
    Serial.println("movendo para a esquerda"); 
    state_manager.set_move_left(value_received); 
  }

  if (string_topic.equals(receptor.pir_left_channel))
  {
    Serial.println("sensor pir esquerdo"); 
    state_manager.set_pir_left(value_received); 
  }

  if (string_topic.equals(receptor.pir_right_channel))
  {
    Serial.println("sensor pir direito"); 
    state_manager.set_pir_left(value_received); 
  }

}
