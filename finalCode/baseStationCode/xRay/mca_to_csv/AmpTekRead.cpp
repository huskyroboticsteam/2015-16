#include <exception>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include "AmpTekRead.h"
#include "Fit.h"

//	Modified Oct. 15, 2013
//		to avoid substr error if DESCRIPTION line is missing the last three characters " - "
//	Modified Oct. 24, 2013
//		refine TAG check to only look at 9 characters (in case TAG is live_data_1, etc.)

	using namespace std;

int amptek_read ( string inputFileName, AmpTekSpec &sp ) { 

//		open input file
	ifstream inputFile(inputFileName.c_str(), ios::in);
	if ( !inputFile ) {
		return -1;
	};
	
//		read file, interpret data, and place in structure

//		file identification string
	string inputLine;
	getline( inputFile, inputLine );
	if ( inputLine != "<<PMCA SPECTRUM>>" ) {
		cout << "Can't interpret file, first line is: ";
		cout << inputLine << endl;
		return -2;
	};
	sp.fileID = inputLine;

//		read spectrum parameters
	
	bool parameters = true;
	while ( parameters ) {
//			read a full line at a time
		getline( inputFile, inputLine );
//			check for start of calibration info or data
		if ( inputLine == "<<CALIBRATION>>" || inputLine == "<<DATA>>" || inputLine == "<<END>>" ) {
			parameters = false;
			break;
		};
//			set up string stream to interpret flags and number data
		istringstream inputSS(inputLine);
		string token;
		string minusChar;
		inputSS >> token;
		bool tokenOK = false;
		if ( token == "TAG" ) {
			tokenOK = true;
//				swallow dash
			inputSS >> minusChar;
			inputSS >> sp.dataType;
			if ( sp.dataType.substr( 0, 9 ) != "live_data" ) cout << "*** Warning - unkown data type: " << sp.dataType << endl;
		};
		if ( token == "DESCRIPTION" ) {
			tokenOK = true;
//				put entire rest of line (if any) into description
			if( inputLine.length() > 14 ) {		//	Oct. 15, 2013 sometimes inputLIne is only 11 chars, " - " is missing
				sp.description = inputLine.substr( 14 );
			}
		};
		if ( token == "GAIN" ) {
			tokenOK = true;
//				swallow dash
			inputSS >> minusChar;
			inputSS >> sp.gain;
		};
		if ( token == "THRESHOLD" ) {
			tokenOK = true;
//				swallow dash
			inputSS >> minusChar;
			inputSS >> sp.threshold;
		};
		if ( token == "LIVE_MODE" ) {
			tokenOK = true;
//				swallow dash
			inputSS >> minusChar;
			inputSS >> sp.live_mode;
		};
		if ( token == "PRESET_TIME" ) {
			tokenOK = true;
//				swallow dash
			inputSS >> minusChar;
			inputSS >> sp.preset_time;
		};
		if ( token == "LIVE_TIME" ) {
			tokenOK = true;
//				swallow dash
			inputSS >> minusChar;
			inputSS >> sp.live_time;
		};
		if ( token == "REAL_TIME" ) {
			tokenOK = true;
//				swallow dash
			inputSS >> minusChar;
			inputSS >> sp.real_time;
		};
		if ( token == "START_TIME" ) {
			tokenOK = true;
//				swallow dash
			inputSS >> minusChar;
			inputSS >> sp.start_time;
		};
		if ( token == "SERIAL_NUMBER" ) {
			tokenOK = true;
//				swallow dash
			inputSS >> minusChar;
			inputSS >> sp.serial_number;
		};
		if ( ! tokenOK ) {
			cout << "*** Warning - Unrecognized token: " << token <<endl;
		};
	};

//		read calibration information
	if ( inputLine == "<<CALIBRATION>>" || inputLine == "<<END>>" ) {
		bool calibration = true;
		while ( calibration ) {
//				read a full line at a time
			getline( inputFile, inputLine );
			if ( inputLine == "<<DATA>>" ) {
				calibration = false;
				break;
			};
//				set up string stream to interpret flags and number data
			istringstream inputSS(inputLine);
			string token;
			string minusChar;
			inputSS >> token;
			bool tokenOK = false;
			if ( token == "LABEL" ) {
				tokenOK = true;
//					swallow dash
				inputSS >> minusChar;
				inputSS >> sp.cal_label;
				if ( sp.cal_label != "Channel" ) cout << "*** Warning - unkown calibration label: " << sp.cal_label << endl;
			};
			if ( ! tokenOK ) {
//					assume its a channel - energy pair
				istringstream inputNumbers(inputLine);
				float ch;
				inputNumbers >> ch;
				float en;
				inputNumbers >> en;
				sp.cal_channel.push_back( ch );
				sp.cal_energy.push_back( en );
				tokenOK = true;
			};
		};
	};

//		read spectral data as channels of counts

	if ( inputLine == "<<DATA>>" ) {
		bool data = true;
		while ( data ) {
//				read a full line at a time
			getline( inputFile, inputLine );
			if ( inputLine == "<<END>>" ) {
				data = false;
				break;
			};
			istringstream inputNumbers(inputLine);
			float counts;
			inputNumbers >> counts;
			sp.spectrum.push_back( counts );
		};
	};

//		calculate slope and intercept for calibration
//void fit( vector <float> x, vector <float> y, vector <float> sig, float &a,
//	float &b, float &siga, float &sigb, float &chi2, float &q)
	float a, b, siga, sigb, chi2, q;
	vector <float> sig(0);
	fit ( sp.cal_channel, sp.cal_energy, sig, a, b, siga, sigb, chi2, q );
//		convert from keV to eV
	sp.ev_ch = 1000 * b;
	sp.ev_start = 1000 * a;

	return 0;

};