#include "StateManager.h" // Inclui arquivo de cabeçalho

/**
 * Contrutor responsável por salvar os observadores que vão reagir as mudanças 
 * do estado.
*/
StateManager::StateManager(EnginesController* engine_observer)
{
    this->engine_observer = engine_observer; 
}

/**
 * Função responsável por alterar o estado do microcontrolador.
*/
void StateManager::set_state(state_package state)
{

    this->base_potentiometer_s = state.base_potentiometer_s; 
    this->body_potentiometer_s = state.body_potentiometer_s; 
    this->move_left_button_s = state.move_left_button_s; 
    this->move_right_button_s = state.move_right_button_s; 
    this->pir_left_s = state.pir_left_s; 
    this->pir_right_s = state.pir_right_s;

}

/**
 * Notifica os observadores que devem reagir a alguma mudança realizada no 
 * estado.
 **/
void StateManager::notify(int channel, int data)
{
    this->engine_observer->on_update(channel, data);
}


// A partir daqui são apenas funções Setters que alteram de maneira controlado 
// valores respectivos ao estado 

void StateManager::set_base_potentiometer(int value)
{
    this->base_potentiometer_s = value; 
    this->notify(BASE_POT_CHANNEL, this->base_potentiometer_s);
} 

void StateManager::set_body_potentiometer(int value)
{
    this->body_potentiometer_s = value;
    this->notify(BODY_POT_CHANNEL, this->body_potentiometer_s);
}

void StateManager::set_move_right(int value)
{
    this->move_right_button_s = value; 
    this->notify(MOVE_RIGHT_CHANNEL, this->move_right_button_s); 
} 

void StateManager::set_move_left(int value)
{
    this->move_left_button_s = value;
    this->notify(MOVE_LEFT_CHANNEL, this->move_left_button_s); 
} 

void StateManager::set_pir_left(int value)
{
    this->pir_left_s = value; 
    this->notify(PIR_LEFT_CHANNEL, this->pir_left_s);
}

void StateManager::set_pir_right(int value)
{
    this->pir_right_s = value; 
    this->notify(PIR_RIGHT_CHANNEL, this->pir_right_s);
}