#ifndef STATE_MANAGER_H 
#define STATE_MANAGER_H


class EnginesController;

#include <BlynkSimpleEsp32.h>

#include "EnginesController.h"

#define BASE_POT_CHANNEL 0
#define BODY_POT_CHANNEL 1
#define MOVE_LEFT_CHANNEL 2
#define MOVE_RIGHT_CHANNEL 3
#define PIR_LEFT_CHANNEL 4
#define PIR_RIGHT_CHANNEL 5 
#define FULL_STOP_CHANNEL 6

typedef struct {
    int base_potentiometer_s; 
    int body_potentiometer_s; 
    int move_left_button_s; 
    int move_right_button_s;
    int pir_left_s; 
    int pir_right_s; 
} state_package;  



class StateManager {

    public: 

        StateManager(EnginesController* engine_observer);
        
        void set_state(state_package state);

        void set_base_potentiometer(int value); 
        void set_body_potentiometer(int value);
        void set_move_right(int value); 
        void set_move_left(int value); 
        void set_pir_left(int value);
        void set_pir_right(int value);
        void set_full_stop(bool value);
  
        void notify(int channel, int data);

        

    private:
        EnginesController* engine_observer;

        int base_potentiometer_s = 0; 
        int body_potentiometer_s = 0; 
        int move_left_button_s = 0; 
        int move_right_button_s = 0;
        int pir_left_s = 0; 
        int pir_right_s = 0; 

}; 

#endif