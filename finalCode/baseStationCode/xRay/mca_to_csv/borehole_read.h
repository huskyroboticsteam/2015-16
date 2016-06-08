#ifndef borehole_read_h
#define borehole_read_h

using namespace std;

//	reads asc files from Borehole XRF software    Aug. 2007   W. T. Elam  APL/UW


int borehole_read ( const string spectrumFileName, float conditionsArray[], 
				   vector <float> &spectrum, float &ev_start, float &ev_ch, float &live_time );

//  integer returns
//		0	file read OK
//		-1	can't open file
//		-2	invalid format
//		-3	invalid version
//		-4	XUNITS not eV
//		-5	YUNITS not COUNTS
//		-6	not an XRF file
//		-7	NPOINTS keyword not found in file
//		-8	End of File encountered while reading spectrum data

#endif
