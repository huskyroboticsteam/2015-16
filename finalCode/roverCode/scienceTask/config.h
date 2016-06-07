// MOTOR CONTROL CONSTANTS //
// #define FRONT_RIGHT_MOTOR_PIN
// #define FRONT_LEFT_MOTOR_PIN
// #define BACK_RIGHT_MOTOR_PIN
// #define BACK_LEFT_MOTOR_PIN
#define TALON_NEUTRAL_FREQUENCY 1500
//#define MOTOR_1    3 // FRONT_LEFT
//#define MOTOR_2    5 // FRONT_RIGHT
//#define MOTOR_3    6 // BACK_LEFT
//#define MOTOR_4    11 // BACK_RIGHT
#define MOTOR_5    9// augar
#define MOTOR_6    6 // drill
#define TURNTABLE 7
#define SOIL_SENSOR1 A0 //humidity sensor
#define SOIL_SENSOR2 A1 //humidity sensor

#define HUM_RED 2
#define HUM_GREEN 3
#define HUM_BLUE 4
#define TEMP_RED 5
#define TEMP_GREEN A2 
#define TEMP_BLUE A3

#define THERM_PIN 8


// ETHERNET COMMUNICATION CONSTANTS //
#define UDP_PORT         7777
#define DESTINATION_PORT 7777
#define TIMEOUT          1000

// SERIAL COMMUNICATION CONSTANTS //
#define BAUD_RATE 9600

// STEERING EQUATION CONSTANTS //
#define POTENTIOMETER A6 
#define VERTICAL_WHEEL_SEPARATION  27.6 // in
#define LATERAL_WHEEL_SEPARATION   22.4 // in
#define MAX_TURN_ANGLE 45 // degrees
#define TUNING_CONSTANT 1 // constant for error correction
