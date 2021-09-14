#ifndef RECEPTOR_H
#define RECEPTOR_H

#include <PubSubClient.h>

// unica função é se conectar com a internet e receber dados do painel de controle
class Receptor {

    public:
    
        char* host; 
        int host_port; 
        char* device_id;
        const int n_channels = 6;

        const char* channels_arr[6] = {
            "controlador/base",
            "controlador/corpo",
            "controlador/direita", 
            "controlador/esquerda",
            "controlador/left", 
            "controlador/right"
        };

        const char* base_potentiometer_channel = "controlador/base"; 
        const char* body_potentiometer_channel = "controlador/corpo"; 
        const char* move_engine_to_left_channel = "controlador/esquerda";
        const char* move_engine_to_right_channel = "controlador/direita"; 

        const char* pir_left_channel = "controlador/left"; 
        const char* pir_right_channel = "controlador/right";

        void setup(
            char* host,
            int host_port,
            char* device_id,
            void (*data_reception_cb)(char *topic, uint8_t *payload, unsigned int lenght)
        );

        Receptor(
            PubSubClient& mqtt_client
        );

        PubSubClient& mqtt_client;

    
}; 


#endif