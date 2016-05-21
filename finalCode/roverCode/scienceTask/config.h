// MOTOR CONTROL CONSTANTS //
// #define FRONT_RIGHT_MOTOR_PIN
// #define FRONT_LEFT_MOTOR_PIN
// #define BACK_RIGHT_MOTOR_PIN
// #define BACK_LEFT_MOTOR_PIN
#define TALON_NEUTRAL_FREQUENCY 1500
#define MOTOR_1    3 // FRONT_LEFT
#define MOTOR_2    5 // FRONT_RIGHT
#define MOTOR_3    6 // BACK_LEFT
#define MOTOR_4    9 // BACK_RIGHT
#define motor_5    8 // augar
#define motor_6    10 // drill

// ETHERNET COMMUNICATION CONSTANTS //
#define UDP_PORT         8888
#define DESTINATION_PORT 8888
#define TIMEOUT          1000

// SERIAL COMMUNICATION CONSTANTS //
#define BAUD_RATE 9600

// STEERING EQUATION CONSTANTS //
#define POTENTIOMETER A6 
#define VERTICAL_WHEEL_SEPARATION  27.6 // in
#define LATERAL_WHEEL_SEPARATION   22.4 // in
#define MAX_TURN_ANGLE 45 // degrees
#define TUNING_CONSTANT 1 // constant for error correction