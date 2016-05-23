#ifndef __GPS_H__
#define __GPS_H__

#include <stdint.h>

extern char gps_time[7];       // HHMMSS
extern uint32_t gps_seconds;   // seconds after midnight
extern char gps_date[7];       // DDMMYY
extern float gps_lat;
extern float gps_lon;
extern char gps_aprs_lat[9];
extern char gps_aprs_lon[10];
extern float gps_course;
extern float gps_speed;
extern float gps_altitude;
extern char result[32];
void gps_setup();
bool gps_decode(char c);

#endif

