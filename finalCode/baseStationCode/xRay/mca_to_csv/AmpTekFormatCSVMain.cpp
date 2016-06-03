//
//  main.cpp
//  AmpTekFormatCSV
//
//  Created by W. T. Elam on 6/10/13.
//			reads specrum from .mca file using AmpTekRead
//          and writes to CSV file for plotting
//
// Modified July 30, 2013 to read file names from list and process all at once
// Modified Feb 26, 2014 to fix data subfolders, especially csv file writes
// Modified Aug. 26, 2105 to add default calibration (10 eV/channel) for PIXL spectra


#include <exception>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <time.h>
#include "XRFconstants.h"
#include "SpectrumData.h"
#include "AmpTekRead.h"
#include "borehole_read.h"
#include "snip.h"

int main (int argc, const char * argv[])
{

    try {

    using namespace std;

//		write program header
	cout << "AmpTek .mca & EMSA .xsp Re-Format Program for X-ray Fluorescence" << endl;
	cout << "Version 1.5 (Mac)      Aug. 26, 2015      W. T. Elam  APL/UW    ";
	cout << endl;

//    string data_sub_folder = "./MTXRF_first_data_Apr2014/";
//  string data_sub_folder = "./Data_Moxtek_60kV_tube_MTXRF_Feb2014/";
//  string data_sub_folder = "./OPA_standards_data_LowVprobe_Oct2013/"; //  Unix, Linux, and Mac path delimiters
//  string data_sub_folder = ".\\OPA_standards_data_LowVprobe_Oct2013\\"; //  Windows and Dos path delimiters
  string data_sub_folder;                                             // data in same folder as executable
    cout << endl << "Data in sub-folder " << data_sub_folder << endl;

    bool error = false;
    bool done = false;
	string nothing;
	int result = 0;

	cout.setf( ios::fixed, ios::floatfield );
	cout.precision(2);

    clock_t startTime, finishTime;
    startTime = clock();

    //  **********************************************************************
    //	Get file with list of spectrum file names
    //  **********************************************************************

    bool save_spc = false;
    cout << "Enter file name for spectrum list: ";
    string fileListName;
    cin >> fileListName;
    string nothing_list;
    getline ( cin, nothing_list );  //  bypass end of line
    cout << "   Reading file list from file " << fileListName << endl;
    ifstream fileListFile( fileListName.c_str(), ios::in);
    if ( !fileListFile ) {
        cout << "Cannot open list file " << fileListName << endl;
        return -1;
    };

    //  *****************************************************
    //		loop over file list entries
    //  *****************************************************

    while ( ( ! error ) && ( ! done ) ) {

        //			get spectrum file name
        string spectrumFileName;
        getline( fileListFile, spectrumFileName );
        if ( spectrumFileName.length() <= 0 || spectrumFileName == " " ) {
            done = true;
            break;
        };
        //		cout << "Enter file name for spectrum file taken at APS 1ID: ";
        //		getline( cin, spectrumFileName );
        //		spectrumFileName = "GHSR1002301.001";
        //		cout << "Processing " << spectrumFileName << endl;
        if ( spectrumFileName == "end" || spectrumFileName == "quit" || spectrumFileName == "no" || spectrumFileName == "bye" ) {
            done = true;
            break;
        };
        cout << "Processing spectrum file " << spectrumFileName << endl;
        string spectrumPathName = data_sub_folder + spectrumFileName;		//	Unix and Mac

        float ev_start, ev_ch = 0, cal_quad, live_time, real_time;
        SpectrumData spectrum;
        if( spectrumFileName.length() - spectrumFileName.rfind( ".xsp" ) < 6 ) {
            float conditionsArray[256];
            vector <float> spec_vector;
            result = borehole_read ( spectrumPathName, conditionsArray, spec_vector, ev_start, ev_ch, live_time );
            if ( result != 0 ) {
                cout << "Can't read EMAS spectrum file, result = " << result << "  for file name " << spectrumFileName << endl;
                error = true;
            };
            //      put spectrum into SpectrumData object
            cal_quad = 0;
            if( ev_ch == 0 || isnan(ev_ch) ) {	//	Use default calibration for PIXL spectra     Aug. 26, 2015
                ev_ch = 10;
                ev_start = 0;
                cout << "Using default calibration of 10 eV per channel." << endl;
            }
            SpectrumData spectrum_temp( spec_vector, ev_start, ev_ch, cal_quad );
            spectrum = spectrum_temp;
        } else {
            //			read spectrum in format from AmpTek ADMCA application
            AmpTekSpec specData;
            cout << "Calling amptek_read." << endl;
            result = amptek_read( spectrumPathName, specData );
            if ( result != 0 ) {
                cout << "Can't read AmpTek spectrum file, result = " << result << "  for file " << spectrumPathName << endl;
                error = true;
            };
            cout << "Label: " << specData.description << endl;
        //      put spectrum into SpectrumData object
            int nc = 0;
            cal_quad = 0;
            nc = specData.spectrum.size();
            ev_start = specData.ev_start;
            ev_ch = specData.ev_ch;
            live_time = specData.live_time;
            if( ev_ch == 0 || isnan(ev_ch) ) {	//	Use dafault calibration for PIXL spectra     Aug. 26, 2015
                ev_ch = 10;
                ev_start = 0;
                cout << "Using default calibration of 10 eV per channel." << endl;
            }
            SpectrumData spectrum_temp( specData.spectrum, ev_start, ev_ch, cal_quad );
            spectrum = spectrum_temp;
        }

    //			remove background over entire spectrum
//        int width = int( 600 / ev_ch + 1 );                     //  600 eV rough value for CdTe
        int width = int( 150 / ev_ch + 1 );                     //  150 eV rough value for PIN and SDD
        int iBkgStart = 30;
        int iBkgEnd = spectrum.numberOfChannels() - 20;
        vector <float> bkg( spectrum.numberOfChannels() );
        snipbg( spectrum.meas(), bkg, iBkgStart, iBkgEnd, width, 24 );	//	24 iterations
        spectrum.bkg( bkg );

    //			write calculated spectrum to csv file
        int dotPos = spectrumPathName.rfind( ".");
        if ( dotPos <= 0 ) dotPos = spectrumPathName.length();
        string outFileName = spectrumPathName.substr( 0, dotPos ) + ".csv";
        cout << endl;
        cout << "Writing calculated spectrum to file " << outFileName << endl;
        ofstream outFile ( outFileName.c_str() );
		outFile << outFileName << endl;
        outFile << "Energy (keV)";
        outFile << ", counts";
		outFile << ", sigma";
		outFile << ", bkg";
		outFile << endl;
        int is;
        for ( is=0; is<spectrum.numberOfChannels(); is++ ) {
            outFile << spectrum.energy( is )  / 1000;
            outFile << ", " << spectrum.meas()[is];
            outFile << ", " << spectrum.sigma()[is];
            outFile << ", " << spectrum.bkg()[is];
            outFile << endl;
        };
        outFile.close();
        cout << endl;
    };

    finishTime = clock();
    double duration = (double) ( finishTime - startTime ) / CLOCKS_PER_SEC;	// obsolete name for CLOCKS_PER_SECOND
    cout << endl << "Execution finished.";
    cout << "    " << "Duration  " << duration << " secs. " << endl;




    //	end of main program, include exception handling
} catch (string s) {
	cerr << endl << "Catch string: " << s << endl;
    } catch (char* s) {
        cerr << endl << "Catch string: " << s << endl;
        //} catch (out_of_range&) {
        //	cerr << endl << "Catch out_of_range: " << endl;
        //} catch (runtime_error&) {
        //	cerr << endl << "Catch runtime_error: " << endl;
        //} catch (length_error&) {
        //	cerr << endl << "Catch length_error: " << endl;
        //} catch (invalid_argument&) {
        //	cerr << endl << "Catch invalid_argument: " << endl;
        //} catch (logic_error&) {
        //	cerr << endl << "Catch logic_error: " << endl;
    } catch (exception& e) {
        cerr << endl << "Catch exception: " << e.what() << endl;
    };
    //		Don't need this for Mac Xcode, only if application opens in separate window
    //	cout << endl << "Execution finished, press return twice to close window." << endl;
    //	string nothing;
    //	getline ( cin, nothing );
    //	getline ( cin, nothing );

return 0;
};

