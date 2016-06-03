// ETHERNET COMMUNICATION CONSTANTS //
#define UDP_PORT         8887
#define DESTINATION_PORT 8887
#define TIMEOUT          500

// SERIAL COMMUNICATION CONSTANTS //
#define BAUD_RATE 9600

// STEERING EQUATION CONSTANTS //
#define POTENTIOMETER A6 
#define VERTICAL_WHEEL_SEPARATION  27.6 // in
#define LATERAL_WHEEL_SEPARATION   22.4 // in
#define MAX_TURN_ANGLE 45 // degrees
#define TUNING_CONSTANT 1 // constant for error correction

// GPS CONSTANTS //
#define GPS_RX 4
#define GPS_TX 5
#define VALID_POS_TIMEOUT = 2000;

// MOTOR CONTROL CONSTANTS //
#define TALON_NEUTRAL_FREQUENCY 1500
#define MOTOR_1    3 // BACK_RIGHT
#define MOTOR_2    5 // FRONT_RIGHT
#define MOTOR_3    6 // FRONT_LEFT
#define MOTOR_4    9 // BACK_LEFT

// ETHERNET COMMUNICATION CONSTANTS //
#define UDP_PORT         8888
#define DESTINATION_PORT 8887
#define TIMEOUT          500

// SERIAL COMMUNICATION CONSTANTS //
#define BAUD_RATE 9600

// STEERING EQUATION CONSTANTS //
#define POTENTIOMETER A6 
#define VERTICAL_WHEEL_SEPARATION  27.6 // in
#define LATERAL_WHEEL_SEPARATION   22.4 // in
#define MAX_TURN_ANGLE 45 // degrees
#define TUNING_CONSTANT 1 // constant for error correction

// GPS CONSTANTS //
#define GPS_RX 4
#define GPS_TX 5
#define VALID_POS_TIMEOUT  2000

//First two bytes of the packet is the angle, second pair of bytes is the speed, the next 14 bytes are arm controls, the last 4 bytes are for camera movement

//First two bytes of the packet is the angle, second pair of bytes is the speed, the next 14 bytes are arm controls, the last 4 bytes are for camera movement

