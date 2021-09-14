#ifndef ENGINES_CONTROLLER_H
#define ENGINES_CONTROLLER_H

#include "StateManager.h"

#include "Arduino.h"
#include "ESP32Servo.h"

class EnginesController {

    public:

        void setup();
        void on_update(int channel, int data);

        EnginesController(
            int counter_clockwise_engine_pin, 
            int clockwise_engine_pin, 

            int base_servo_engine_pin,
            int body_servo_engine_pin
        ); 

    private: 

        Servo base_servo;
        Servo body_servo; 

        int counter_clockwise_engine_pin; 
        int clockwise_engine_pin; 

        int base_servo_engine_pin;
        int body_servo_engine_pin;

        const int duty_cycle = 21;
        const int frequency = 2000; 
        const int resolution = 10;

        const int counter_clockwise_channel = 5; 
        const int clockwise_channel = 3; 

}; 

#endif