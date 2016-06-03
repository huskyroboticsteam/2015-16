#include <exception>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include "borehole_read.h"
#include "XRFparameterIndex.h"
#include "upper_trim.h"

//  Added parameter input for optic type   W. T. Elam APL/UW Sep. 30, 2013
//  (Already included in XRFparameterIndex.h and XRFconditions.h by Nick Yang Sep. 2013)

	using namespace std;

void parseEMASkeyword( const string strIn, const string delim, string &sKeyword, string &sValue );

int borehole_read ( const string spectrumFileName, float conditionsArray[], 
				   vector <float> &spectrum, float &ev_start, float &ev_ch, float &live_time ) {

//		set up keywords for conditions
vector <string> paramName( XRF_PARAMETER_LAST + 1 );
paramName[ANODE_Z_INDEX] = "anode_z";
paramName[KV_INDEX] = "beamkv";
paramName[TUBE_INC_ANGLE_INDEX] = "tube_inc_angle";
paramName[TUBE_TAKEOFF_ANGLE_INDEX] = "tube_takeoff_angle";
paramName[TUBE_BE_WINDOW_INDEX] = "tube_be_window";
paramName[TUBE_CURRENT_INDEX] = "tube_current";
paramName[FILTER_Z_INDEX] = "filter_z";
paramName[FILTER_THICK_INDEX] = "filter_thick";
paramName[EXCIT_ANGLE_INDEX] = "excit_angle";
paramName[EMERG_ANGLE_INDEX] = "emerg_angle";
paramName[SOLID_ANGLE_INDEX] = "solid_angle";
paramName[PATH_TYPE_INDEX] = "path_type";
paramName[INC_PATH_LENGTH_INDEX] = "inc_path_length";
paramName[EMERG_PATH_LENGTH_INDEX] = "emerg_path_length";
paramName[WINDOW_TYPE_INDEX] = "window_type";
paramName[WINDOW_THICK_INDEX] = "window_thick";
paramName[DETECTOR_TYPE_INDEX] = "detector_type";
paramName[TEST_OPTIC_TYPE_INDEX] = "optic_type";
paramName[MINIMUM_ENERGY_INDEX] = "minimum_energy";


//		open input file
	ifstream inputFile(spectrumFileName.c_str(), ios::in);
	if ( !inputFile ) {
		return -1;
	};
	
//		read file, interpret data, and place in structure

    string strRead, sKeyword, sValue;
	int numChannels;
    bool bNumChanFound;
    bNumChanFound = false;
	while (sKeyword != "#SPECTRUM") {
    
        //Process EMSA keywords
		getline( inputFile, strRead );
		if ( !inputFile ) {
			return -2;
		};
		parseEMASkeyword(strRead, ":", sKeyword, sValue);
        sKeyword = upper_trim(sKeyword);
//			work-around for early typographical error in spectrum save 
		if( sKeyword == "SOLID_ANLGE" ) sKeyword = "SOLID_ANGLE";
		if (sKeyword == "#FORMAT") {
            if (sValue != "EMSA/MAS Spectral Data File") {
                return -2;
            };
        } else if (sKeyword == "#VERSION") {
            if (sValue != "1.0") {
                return -3;
            };
        } else if (sKeyword == "#TITLE") {
 //           txtOperator.TEXT = sValue
        } else if (sKeyword == "#DATE") {
 //           txtAcqDate.TEXT = sValue
        } else if (sKeyword == "#TIME") {
 //           txtAcqTime.TEXT = sValue
        } else if (sKeyword == "#OWNER") {
 //           txtOperator.TEXT = sValue
        } else if (sKeyword == "#NPOINTS") {
			istringstream val( sValue );
            val >> numChannels;
            bNumChanFound = true;
        } else if (sKeyword == "#XUNITS") {
            if (sValue != "eV") {
                return -4;
            };
        } else if (sKeyword == "#YUNITS") {
            if (sValue != "COUNTS") {
                return -5;
            };
        } else if (sKeyword == "#XPERCHAN") {
			istringstream val( sValue );
            val >> ev_ch;
        } else if (sKeyword == "#OFFSET") {
 			istringstream val( sValue );
			val >> ev_start;
        } else if (sKeyword == "#LIVETIME") {
 			istringstream val( sValue );
			val >> live_time;
       } else if (sKeyword == "#SIGNALTYPE") {
            if (sValue != "XRF") {
                return -6;
            };
		};

//			Process Conditions keywords (not part of EMAS format)
		int i;
		for( i=0; i<XRF_PARAMETER_LAST + 1; i++ ) {
			string upper_test = upper_trim( paramName[i] );
            if (sKeyword == upper_test) {
 				istringstream val( sValue );
                val >> conditionsArray[i];
				break;
            };
		};

	};

    //Process spectrum
    
	if( ! bNumChanFound ) return -7;
	spectrum.resize( numChannels );
	int i;
	for( i=0; i<numChannels; i++ ) {
        inputFile >> spectrum[i];
		if ( !inputFile ) {
			return -8;
		};
	};

	return 0;

};


void parseEMASkeyword( const string strIn, const string delim, string &sKeyword, string &sValue) {
// strIn - in, input string
// del - in, delimiter character
// sKeyword - out, keyword up to delimiter (not inlcuding delimiter)
// sValue - out, rest of string after delimiter and following blank, if any
	int i, j;
	int slen;
    slen = strIn.length();
	j = strIn.find( delim );
	if( j >= 0 && j < strIn.length() ) {
		sKeyword = strIn.substr( 0, j );
		if( strIn.substr( j + 1, 1 ) == " ") j = j + 1;
		sValue = strIn.substr( j + 1, slen);
	} else {
		sKeyword = strIn.substr( 0, slen );
		sValue.erase();
	};
};


