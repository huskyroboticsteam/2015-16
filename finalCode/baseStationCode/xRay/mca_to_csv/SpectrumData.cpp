/*
 *  SpectrumData.cpp
 *  APS1IDanalysis_SpecProcDev
 *
 *  Created by Tim Elam on 3/16/12.
 *  Copyright 2012 University of Washington. All rights reserved.
 *
 */

#include "SpectrumData.h"
#include <math.h>

using namespace std;

SpectrumData::SpectrumData( const vector <float> &counts_in, const float energyStart_in, 
						   const float energyPerChannel_in, const float quad_cal_in ) {
	energyStart_save = energyStart_in;
	energyPerChannel_save = energyPerChannel_in;
	quad_save = quad_cal_in;
	tilt_save = 0;
	offset_save = 0;
	if( counts_in.size() <= 0 ) return;
	measured_data.resize( counts_in.size() );
	measured_sigma.resize( counts_in.size() );
	energy_vector.resize( counts_in.size() );
	int i;
	for( i=0; i<counts_in.size(); i++ ) {
		measured_data[i] = counts_in[i];
		measured_sigma[i] = ( counts_in[i] > 0? sqrt( counts_in[i] + 2 ): sqrt(2) );
	}
    reload_energy();
};

SpectrumData::SpectrumData( const vector <float> &counts_in ) {
	energyStart_save = 0;
	energyPerChannel_save = 1;
	quad_save = 0;
	offset_save = 0;
	tilt_save = 0;
	if( counts_in.size() <= 0 ) return;
	measured_data.resize( counts_in.size() );
	measured_sigma.resize( counts_in.size() );
	energy_vector.resize( counts_in.size() );
	int i;
	for( i=0; i<counts_in.size(); i++ ) {
		measured_data[i] = counts_in[i];
		measured_sigma[i] = ( counts_in[i] > 0? sqrt( counts_in[i] + 2 ): sqrt(2) );
	}
    reload_energy();
};

SpectrumData::SpectrumData() {
	energyStart_save = 0;
	energyPerChannel_save = 1;
	quad_save = 0;
	offset_save = 0;
	tilt_save = 0;
};

const float SpectrumData::energy_calc( const float channel_in, const bool corrected ) const {
    float temp_energy = energyStart_save + channel_in * energyPerChannel_save 
    + channel_in * channel_in * quad_save;
    if( corrected ) temp_energy += temp_energy * tilt_save + offset_save;
    return temp_energy;
};

const float SpectrumData::channel_calc( const float energy_in, const bool corrected ) const {
    float energy_calc = energy_in;
    if( corrected ) energy_calc = ( energy_in - offset_save ) / ( 1 + tilt_save );
    float nc = (*this).numberOfChannels();
	if( quad_save * nc * nc < 0.01f ) {
        float ch = ( energy_calc - energyStart_save ) / energyPerChannel_save;
		return ( energy_calc - energyStart_save ) 
                   / energyPerChannel_save;
	} else {
		double b = energyPerChannel_save;
		double c = energyStart_save - energy_calc;
		double b2_4ac = b*b - 4 * quad_save * c;
		if( b2_4ac < 0 ) return 0;
		double guess_plus = ( - b + sqrt( b2_4ac ) ) / ( 2 * quad_save );
		double guess_minus = ( - b - sqrt( b2_4ac ) ) / ( 2 * quad_save );
		if( 0 <= guess_plus && guess_plus < measured_data.size() ){
			return float(guess_plus);
		} else if (  0 <= guess_minus && guess_minus < measured_data.size()  ) {
			return float(guess_minus);
		} else {
			return 0;
		}
	}

};

const float SpectrumData::energyPerChannel( const int channel_in ) const {
	if( quad_save == 0 ) {
		return energyPerChannel_save + tilt_save;
	} else {
		return  2 * channel_in * quad_save + energyPerChannel_save + tilt_save;
	}
};

const float SpectrumData::energyPerChannel( ) const {
	if( quad_save == 0 ) {
		return energyPerChannel_save + tilt_save;
	} else {
		int center_channel = measured_data.size() / 2;
		return  2 * center_channel * quad_save + energyPerChannel_save + tilt_save;
	}
};

void SpectrumData::meas( std::vector <float> counts_in ) {
	measured_data.resize( counts_in.size() );
	measured_sigma.resize( counts_in.size() );
	int i;
	for( i=0; i<counts_in.size(); i++ ) {
		measured_data[i] = counts_in[i];
		measured_sigma[i] = ( counts_in[i] > 0? sqrt( counts_in[i] + 2 ): sqrt(2) );
	}
};

void SpectrumData::bkg( std::vector <float> background_in ) {
	background.resize( background_in.size() );
	measured_net.resize( background_in.size() );
	int i;
	for( i=0; i<background_in.size(); i++ ) {
		background[i] = background_in[i];
        measured_net[i] = measured_data[i] - background[i];
	}
};

void SpectrumData::calc( std::vector <float> calculation_in ) {
	calculation.resize( calculation_in.size() );
	int i;
	for( i=0; i<calculation_in.size(); i++ ) {
		calculation[i] = calculation_in[i];
	}
    if( measured_net.size() == calculation_in.size() ) {
        for( i=0; i<calculation_in.size(); i++ ) {
            residual_calc.resize( calculation_in.size() );
            residual_calc[i] = measured_net[i] - calculation[i];
        }
    }
};

void SpectrumData::calibration( const float energyStart_in, 
				 const float energyPerChannel_in, const float quad_cal_in ) {
	energyStart_save = energyStart_in;
	energyPerChannel_save = energyPerChannel_in;
	quad_save = quad_cal_in;
	tilt_save = 0;
	offset_save = 0;
    reload_energy();
};

void SpectrumData::offset( const float offset_in ) { 
	offset_save = offset_in; 
    reload_energy();
};

void SpectrumData::tilt( const float tilt_in ) { 
	tilt_save = tilt_in; 
    reload_energy();
};

void SpectrumData::reload_energy() {
    energy_vector.resize( measured_data.size() );
	int i;
	for( i=0; i<energy_vector.size(); i++ ) {
		energy_vector[i] = (*this).energy(i);
	}
};

