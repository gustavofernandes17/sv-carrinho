// inclusão de bibliotecas
// definição de constantes
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

#include "camera_management.h"

// pinos que vao controlar os motores
#define PINO_MOTOR_SENTIDO_HORARIO 12
#define PINO_MOTOR_SENTIDO_ANTI_HORARIO 13

// pinos que vao controlar os servos
#define PINO_SERVO_1_BASE 2 
#define PINO_SERVO_2_CORPO 15

#define PINO_FIM_DE_CURSO_1 4
#define PINO_FIM_DE_CURSO_2 16


// Blynk e rede WIFI
char auth[] = "6Ck3mKUw32pM8TGE8bdNyfip6lxSt4Bq";
char ssid[] = "Cesar"; // "Redmi Note 7";
char pass[] = "pedro0908fernandes"; // "b6ee8b98282c";

// broker
char* broker = "broker.hivemq.com";
int port = 1883;
char* device_id = "SV-CARRINHO";

// Instanciamento dos objetos para lidar com comunicação e Redes
WiFiClient client_wifi; 
PubSubClient mqtt_client(client_wifi);
Receptor receptor(mqtt_client); 

// Objeto que gerencia o sistema de controle dos motores
EnginesController engines_controller(
  PINO_MOTOR_SENTIDO_ANTI_HORARIO,
  PINO_MOTOR_SENTIDO_HORARIO,
  PINO_SERVO_1_BASE,
  PINO_SERVO_2_CORPO
);


// Objeto que gerencia o Estado do Microcontrolador
StateManager state_manager(&engines_controller);

// Protótipo da função que gerencia a recepção da comunicação do painel de controle 
void data_reception_callback(char *topic, uint8_t *payload, unsigned int lenght); 
void stop_engines_interrupt_callback();

void setup() {

  
  // Inicia Comunicação Serial para Depuração
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  // Inicializa o Blynk
  Blynk.begin(auth, ssid, pass, "blynk.iot-cm.com", 8080); 

  //inicializa o receptor (comunicação do painel de controle)
  receptor.setup(broker, port, device_id, data_reception_callback);
  
  // inicializa o controlador dos motores
  engines_controller.setup();

  // configurando a camera
  setup_config_cam();

  // Interrupts para o fim de curso
  attachInterrupt(PINO_FIM_DE_CURSO_1, stop_engines_interrupt_callback, RISING);
  attachInterrupt(PINO_FIM_DE_CURSO_2, stop_engines_interrupt_callback, RISING);  

}

void loop() {
  server.handleClient();
  Blynk.run();
  mqtt_client.loop(); 
}

// Gerencia os Botões Virtuais do Blynk
BLYNK_WRITE(V0) {
  state_manager.set_move_right(param.asInt());
}

BLYNK_WRITE(V1) {
  state_manager.set_move_left(param.asInt());
}

BLYNK_WRITE(V2) {
  state_manager.set_base_potentiometer(param.asInt());
}

BLYNK_WRITE(V3) {
  state_manager.set_body_potentiometer(param.asInt());
}

// função responsável por parar os motores quando o interrupt for acionado
void stop_engines_interrupt_callback()
{
  state_manager.set_full_stop(true); 
}

// implementação da função que recebe os dados do painel de controle
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

  if (string_topic.equals(receptor.full_stop_channel))
  {
    Serial.println("Desligando motores DC..."); 
    state_manager.set_full_stop(true);
  }

}