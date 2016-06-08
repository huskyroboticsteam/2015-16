
/************************************************************************
 * FUNCTION:    sgsmth()
 * 
 * PURPOSE:     calculates a smoothed spectrum using a second-degree polynomial filter	
 * 
 * CALLED BY:
 * 
 * PARAMETERS:  y - original spectrum
 *              nchan - number of channels in spectrum
 *              ich1 - first channel number to be smoothed
 *              ich2 - last channel number to be smoothed
 *              iwid - width of the filter (2m+1), iwid<41
 * 
 * RETURN(s):   s - smoothed spectrum, only defined between ich1 and ich2
 * 
 * REVISION:    1.1
 * 
 * NOTES:       ens - 3 August 2005: revised
 *					- sgsmth() ich1 and ich2 values no longer passed by reference
 *				ens - 12 April 2005: this algorithm, developed using FORTRAN,
 *              wte - 23 May 2007:  re-worked handling of ends of range
 *              originally appeared in "Handbook of X-Ray Spectrometry"
 *              by R.E.Van Grieken and A.A Markowicz
 ************************************************************************/

#include <Windows.h>
#include <vector>
#include <cstdlib>
#include <math.h>
#include "snip.h"

#define _MIN min
#define _MAX max

using namespace std;

//void sgsmth(vector <float> &y, vector <float> &s, int nchan, int &ich1, int &ich2, int iwid)
void sgsmth( const std::vector <float> &y,  std::vector <float> &s, 
            const int ich1, const int ich2, const int iwid )

{
 	const int nchan = y.size();
	int iw, m, jch1, jch2;
	const int arraySize = 41;
	float sum;
	float c [arraySize];
	int i,j;

	//initialize array
	for(i = 0; i < arraySize; i++)
		c[i] = 0;

	//calculate filter coefficients
	iw = _MIN(iwid, arraySize);
	m = iw/2;
	sum = ((2*m - 1)*(2*m + 1)*(2*m + 3));
	for(j = -m; j <= m; j++)
		c[j + m] = (3*(3*m*m + 3*m - 1 - 5*j*j));

	//convolute spectrum with filter
	jch1 = _MAX(ich1, 0);
	jch2 = _MIN(ich2, nchan - 1);
	for(i = jch1; i <= jch2; i++)
	{
		s[i] = 0;
		for(j = -m; j <= m; j++)
		{
			int j1 = _MAX(i+j, jch1 ); 
			j1 = _MIN(j1, jch2 ); 
			s[i] = s[i] + c[j+m] * y[j1];
		}
		s[i] = s[i]/sum;
	}
	return;
}//end sgsmth()


/************************************************************************
 * FUNCTION:    snipbg()
 * 
 * PURPOSE:     calculates a continuum via peak stripping	
 * 
 * CALLED BY:
 * 
 * PARAMETERS:  y - original spectrum
 *              nchan - number of channels in spectrum
 *              ich1 - first channel number of region to calculate the continuum
 *              ich2 - last channel number of region to calculate the continuum
 *              fwhm - width parameter for smoothing and stripping algorithm, set it
 *                     to average FWHM of peaks in the spectrum, typical value 8.0
 *              niter - number of iterations of SNIP algorithm, typical 24
 * 
 * RETURN(s):   yback - calculated continuum in the region ich1-ich2
 * 
 * REVISION:    1.1
 * 
 * NOTES:       wte - 18 Dec. 2005
 *					- created header, use vector size for nchan, and eliminate fortranRound
 *				ens - 3 August 2005: revised
 *					- snipbg() ich1 and ich2 values no longer passed by reference
 *				ens - 14 April 2005: this function makes use of the sgsmth() function
 *              ens - 12 April 2005: this algorithm, developed using FORTRAN,
 *              wte - 23 May 2007:  re-worked handling of ends of range
 *              originally appeared in "Handbook of X-Ray Spectrometry"
 *              by R.E.Van Grieken and A.A Markowicz
 ************************************************************************/
//void snipbg(vector <float> &y, vector <float> &yback, int nchan, int &ich1, int &ich2, float fwhm, int niter)
void snipbg( const  std::vector <float> &y,  std::vector <float> &yback, const int ich1, 
            const int ich2, const int fwhm, const int niter )
{

	const int nchan = y.size();
	const float SQRT2 = 1.4142f;
	const int NREDUC = 8;
	int iw, i1, i2;
	int i,n;

	//smooth spectrum
	iw = fwhm;
	i1 = _MAX(ich1, 0);
	i2 = _MIN(ich2, nchan-1);
	sgsmth(y, yback, i1, i2, iw); //data smoothing routine

	//square root transformation over required spectrum region
	for( i = i1; i <= i2; i++)
	{
		yback[i] = sqrt(_MAX(yback[i], (float)0.0));
	}//end for

	//peak stripping
	float redfac = 1;
	for( n = 1; n <= niter; n++)
	{
		//set width, reduce width for last nreduc iterations
		if(n > niter-NREDUC)
			redfac = redfac/SQRT2;
		iw = int( redfac * fwhm + 0.5f );
		for( i = ich1; i <= ich2; i++)
		{
			i1 = _MAX(i-iw, ich1);
			i2 = _MIN(i+iw, ich2);
			yback[i] = _MIN(yback[i], (float)(0.5*(yback[i1]+yback[i2])));
		}
	}

	//back transformation
	for(i = ich1; i <= ich2; i++)
		yback[i] = yback[i]*yback[i];

	return;
}//end snipbg


/************************************************************************
 * FUNCTION:    snipbg_lsq()
 * 
 * PURPOSE:     calculates a continuum via peak stripping with improved
 *              estimation via least squares
 * 
 * CALLED BY:
 * 
 * PARAMETERS:  y - original spectrum
 *              nchan - number of channels in spectrum
 *              ich1 - first channel number of region to calculate the continuum
 *              ich2 - last channel number of region to calculate the continuum
 *              fwhm - width parameter for smoothing and stripping algorithm, set it
 *                     to average FWHM of peaks in the spectrum, typical value 8.0
 *              niter - number of iterations of SNIP algorithm, typical 24
 * 
 * RETURN(s):   yback - calculated continuum in the region ich1-ich2
 * 
 * REVISION:    1.0
 * 
 * NOTES:       wte - 6 June 2014   add least squares adjustment to SNIP result
 *				see also notes on snipbg function above
 ************************************************************************/
//void snipbg_lsq( const  std::vector <float> &y,  std::vector <float> &yback, const int ich1, 
//                const int ich2, const int fwhm, const int niter );
void snipbg_lsq( const  std::vector <float> &y,  std::vector <float> &yback, const int ich1, 
            const int ich2, const int fwhm, const int niter )
{
    
	const int nchan = y.size();
	const float SQRT2 = 1.4142f;
	const int NREDUC = 8;
	int iw, i1, i2;
	int i,n;
    
	//smooth spectrum
	iw = fwhm;
	i1 = _MAX(ich1, 0);
	i2 = _MIN(ich2, nchan-1);
	sgsmth(y, yback, i1, i2, iw); //data smoothing routine
    
	//square root transformation over required spectrum region
	for( i = i1; i <= i2; i++)
	{
		yback[i] = sqrt(_MAX(yback[i], (float)0.0));
	}//end for
    
	//peak stripping
	float redfac = 1;
	for( n = 1; n <= niter; n++)
	{
		//set width, reduce width for last nreduc iterations
		if(n > niter-NREDUC)
			redfac = redfac/SQRT2;
		iw = int( redfac * fwhm + 0.5f );
		for( i = ich1; i <= ich2; i++)
		{
			i1 = _MAX(i-iw, ich1);
			i2 = _MIN(i+iw, ich2);
			yback[i] = _MIN(yback[i], (float)(0.5*(yback[i1]+yback[i2])));
		}
	}
    
	//back transformation
	for(i = ich1; i <= ich2; i++)
		yback[i] = yback[i]*yback[i];

    // the SNIP algorithm gives a background that is slightly below
    //  the average value for the spectrum, causing false positives
    //  in peak fits, so attempto to use least squares to adjust it
    // form least squares sums, ignoring peak regions (only include
    //  regions that are near the background found by SNIP above
    float y_sum = 0;
    float f_sum = 0;
	for(i = ich1; i <= ich2; i++) {
        // skip this channel if counts are more than 3 sigma different than background
        if( fabs( y[i] - yback[i] ) > 3 * sqrt( yback[i] ) ) continue;
        y_sum += y[i] * yback[i];
        f_sum += yback[i] * yback[i];
    }
    // now adjust yback for a least-squares fit to the measured spectrum
	for(i = ich1; i <= ich2; i++) yback[i] *= y_sum / f_sum;

	return;
}//end snipbg_lsq

