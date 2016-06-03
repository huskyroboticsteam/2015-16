#ifndef AmpTekRead_h
#define AmpTekRead_h

#include <vector>

using namespace std;

//	reads asc files from AmpTek MCA software    Jan. 2006   W. T. Elam  APL/UW

struct AmpTekSpec {
	string fileID;
	string dataType;
	string description;
	float gain;
	int threshold;
	int live_mode;
	float preset_time;
	float live_time;
	float real_time;
	string start_time;
	int serial_number;
	string cal_label;
	vector <float> cal_channel;
	vector <float> cal_energy;
	float ev_ch;
	float ev_start;
	vector <float> spectrum;
};

int amptek_read ( string fileName, AmpTekSpec &sp );

//  integer returns
//		0	file read OK
//		-1	can't open file
//		-2	can't interpret file

#endif
