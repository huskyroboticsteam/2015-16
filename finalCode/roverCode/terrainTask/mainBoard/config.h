// MOTOR CONTROL CONSTANTS //
#define TALON_NEUTRAL_FREQUENCY 1500

#define MOTOR_1    3 // BACK_RIGHT
#define MOTOR_2    5 // FRONT_RIGHT
#define MOTOR_3    6 // FRONT_LEFT
#define MOTOR_4    9 // BACK_LEFT

// ARM CONTROL CONSTANTS //
#define ARM_TALON_NEUTRAL_FREQUENCY 2000

#define SHOLDER_ROT 0
#define SHOLDER 1
#define ELBOW 2
#define ELBOW_ROT 3
#define WRIST 4
#define WRIST_ROT 5
#define HAND 6

#define ARM_SPEED 200

// CAMERAS //

#define EYE_OF_SAURON 10


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
#define GPS_RX A3
#define GPS_TX A2
#define VALID_POS_TIMEOUT = 2000;

// First byte is all stop, second byte is pot stop, 
// next two bytes of the packet is the angle, second pair of bytes is the speed, 
// the next 14 bytes are arm controls, the last 4 bytes are for camera movement

