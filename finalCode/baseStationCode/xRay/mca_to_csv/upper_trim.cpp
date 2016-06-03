/*
 *  upper_trim.cpp
 *  APS1IDanalysis
 *
 *  Created by W. T. Elam on 4/8/11.
 *
 */

#include "upper_trim.h"

using namespace std;

string upper_trim( const string inStr ) {
	static string outStr;
	outStr.erase();
	string lower;
	lower = "abcdefghijklmnopqrstuvwxyz";
	string upper;
	upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;
//		convert to upper case and kkeep track of trailing blanks
	int trail = -1;
	for( i=0; i<inStr.length(); i++ ) {
		if( inStr.substr( i, 1 ) == " " ) {
			if( trail < 0 ) trail = i;
		} else {
			trail = -1;
		};
		int j = lower.find( inStr.substr(i,1) );
		if( j < 0 || j >= lower.length() ) {
			outStr += inStr.substr(i,1);
		} else {
			outStr += upper.substr( j, 1 );
		};
	};
//		trim trailing blanks
	if( trail > 0 ) outStr.erase( trail, outStr.length() - trail );
	return outStr;
};
