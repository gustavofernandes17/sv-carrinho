#include "Receptor.h" // Inclui arquivo de cabeçalho do Receptor


// Implementa o Construtor da Classe
Receptor::Receptor(
    PubSubClient& mqtt_client
) : mqtt_client(mqtt_client)
{
    this->mqtt_client = mqtt_client; 
}

// implementa a função Setup que inicializa o objeto salvando dados 
// e informações de configuraçãp
void Receptor::setup(
    char* host, 
    int host_port,
    char* device_id,
    void (*data_reception_cb)(char *topic, uint8_t *payload, unsigned int lenght)
) 
{
    Serial.println("Iniciando Setup"); 

    // salva dados da conexão em memória
    this->host      = host; 
    this->device_id = device_id; 
    this->host_port = host_port; 
    
    // configura a conexão
    this->mqtt_client.setServer(host, host_port); 
    this->mqtt_client.setCallback(data_reception_cb); 

    // enquanto não houver alguma conexão continuar persistindo
    while (!this->mqtt_client.connected()) 
    {
        this->mqtt_client.connect(this->device_id); 

        for (int i = 0; i < this->n_channels; i++)
        {
            this->mqtt_client.subscribe(this->channels_arr[i]);
        }
    }

    Serial.println("Finished setup"); 

}