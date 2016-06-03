/*
 *  SpectrumData.h
 *  APS1IDanalysis_SpecProcDev
 *
 *  Created by Tim Elam on 3/16/12.
 *  Copyright 2012 University of Washington. All rights reserved.
 *
 */

#ifndef SpectrumData_h
#define SpectrumData_h

#include <vector>

class SpectrumData {
public:
	SpectrumData( const std::vector <float> &counts, const float energyStart_in, 
				 const float energyPerChannel_in, const float quad_cal_in = 0 );
	SpectrumData( const std::vector <float> &counts );
	//		must have default constructor to declare arrays
	SpectrumData();
	//		energy calibration, forward and reverse
	const float energy( const int channel_in ) const {
        return energy_calc( float( channel_in ), true );
    };
	const float energy_uncorrected( const int channel_in ) const {
        return energy_calc( float( channel_in ), false );
    };
	const float energy( const float channel_in ) const {
        return energy_calc( channel_in, true );
    };
	const float energy_uncorrected( const float channel_in ) const {
        return energy_calc( channel_in, false );
    };
	const int channel( const float energy_in ) const {
        return int( channel_calc( energy_in, true ) + 0.5f );
    };
	const float channel_float( const float energy_in ) const{
        return channel_calc( energy_in, true );
    };
	const int channel_uncorrected( const float energy_in ) const {
        return int( channel_calc( energy_in, false ) + 0.5f );
    };
	const float channel_float_uncorrected( const float energy_in ) const{
        return channel_calc( energy_in, false );
    };
	const float energyPerChannel( const int channel_in ) const;
	const float live_time() const { return live_time_save; };
	const float real_time() const { return real_time_save; };
	const float energyStart() const { return energyStart_save; };
	const float energyPerChannel( ) const;
	const float quad() const { return quad_save; };
	const float offset() const { return offset_save; };
	const float tilt() const { return tilt_save; };
	//		data access functions
	int numberOfChannels() const { return measured_data.size(); };
	const std::vector <float> &energy()  const { return energy_vector; };
	const std::vector <float> &meas() const { return measured_data; };
	const std::vector <float> &sigma() const { return measured_sigma; };
	const std::vector <float> &bkg() const { return background; };
	const std::vector <float> &net() const { return measured_net; };
	const std::vector <float> &calc() const { return calculation; };
	const std::vector <float> &residual() const { return residual_calc; };
	void meas( std::vector <float> measured_data_in );
	void bkg( std::vector <float> background_in );
	void calc( std::vector <float> calculation_in );
	void live_time( const float live_time_in ) { live_time_save = live_time_in; return; };
	void real_time( const float real_time_in ) { real_time_save = real_time_in; return; };;
	void calibration( const float energyStart_in, 
                     const float energyPerChannel_in, const float quad_cal_in = 0 );
	void offset( const float offset_in );
	void tilt( const float tilt_in );
	
private:
	std::vector <float> energy_vector;
	std::vector <float> measured_data;
	std::vector <float> measured_sigma;
	std::vector <float> background;
	std::vector <float> measured_net;
	std::vector <float> calculation;
	std::vector <float> residual_calc;
	float live_time_save;
	float real_time_save;
	float energyStart_save;
	float energyPerChannel_save;
	float quad_save;
	//		these are temporary corrections to the energy calibration 
	//		that can be set without losing the original calibration
	float offset_save;
	float tilt_save;
    const float channel_calc( const float energy_in, const bool corrected = true ) const;
    const float energy_calc( const float channel_in, const bool corrected = true ) const;
    void reload_energy();
};
#endif
