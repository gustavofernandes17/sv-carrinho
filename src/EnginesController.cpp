#include "EnginesController.h"


EnginesController::EnginesController(
    int counter_clockwise_engine_pin, 
    int clockwise_engine_pin, 

    int base_servo_engine_pin,
    int body_servo_engine_pin
) 
{
    this->counter_clockwise_engine_pin = counter_clockwise_engine_pin;
    this->clockwise_engine_pin = clockwise_engine_pin; 

    this->base_servo_engine_pin = base_servo_engine_pin;
    this->body_servo_engine_pin = body_servo_engine_pin;

}   

void EnginesController::setup()
{
    pinMode(this->clockwise_engine_pin, OUTPUT); 
    pinMode(this->counter_clockwise_engine_pin, OUTPUT);

    ledcSetup(
        this->counter_clockwise_channel,
        this->frequency, 
        this->resolution
    ); 

    ledcSetup(
        this->clockwise_channel,
        this->frequency,
        this->resolution
    ); 

    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2); 
    ESP32PWM::allocateTimer(3); 

    this->base_servo.setPeriodHertz(50); 
    this->body_servo.setPeriodHertz(50); 

    digitalWrite(this->clockwise_engine_pin, LOW); 
    digitalWrite(this->counter_clockwise_engine_pin, LOW);

}

void EnginesController::on_update(int channel, int data) {

    switch (channel) 
    {
        case BASE_POT_CHANNEL: 

            Serial.println("moving base pot:" + String(data));
            this->base_servo.attach(this->base_servo_engine_pin, 600, 2400); 
            this->base_servo.write(data); 

            break; 

        case BODY_POT_CHANNEL: 

            Serial.println("moving body pot:" + String(data));
            this->body_servo.attach(this->body_servo_engine_pin, 600, 2400); 
            this->body_servo.write(data); 

            break; 

        case MOVE_LEFT_CHANNEL:
            
            Serial.println("moving engines to left:" + String(data));
            digitalWrite(this->clockwise_engine_pin, data == 1 ? HIGH : LOW);
            digitalWrite(this->counter_clockwise_engine_pin, LOW);

            break;

        case MOVE_RIGHT_CHANNEL: 

            Serial.println("moving engines to right:" + String(data));
            digitalWrite(this->counter_clockwise_engine_pin, data == 1 ? HIGH : LOW);
            digitalWrite(this->clockwise_engine_pin, LOW);

            break;

        case PIR_LEFT_CHANNEL: 
            digitalWrite(this->clockwise_engine_pin, data == 1 ? HIGH : LOW);
            digitalWrite(this->counter_clockwise_engine_pin, LOW);

            break;

        case PIR_RIGHT_CHANNEL: 
        
            digitalWrite(this->counter_clockwise_engine_pin, data == 1 ? HIGH : LOW);
            digitalWrite(this->clockwise_engine_pin, LOW);

            break;
    }
}