#ifndef XRFconstants_h
#define XRFconstants_h

//	various constants for use in x-ray fluorescence calculations

#define MINIMUM 1.0e-30f
#define MAXIMUM 1.0e+30f
//	From Physics Today Buyer's Guide, August 2001
//	(CODATA recommended values 1998)
#define PI 3.141592653598793f	//	CRC Handbook 1996 page A-1
#define AVOGADRO 6.02214199e23f
#define HC 12.39841856f	//	keV Angstrom (from h=4.135 667 27 e-15 eV sec; c=299 792 458 meter/sec)
#define FOURPIINV 0.079577472f
#define RADDEG 0.017453293f
#define DEGRAD 57.2957795f 
#define ELECTRON_CHARGE 1.602176462E-19f
#define RE2 7.940787e-26f	//	classical electron radius squared [cm2]
#define ME 510998.902f	//	elecron rest mass in eV
#define ALPHA_INV 137.03599976f	//	inverse of fine structure constant

#define CM_MM 0.1f
#define MM_CM 10.0f
#define NM_CM 0.0000001f     //  nanometers to centimeters
#define CM_MICRON 0.0001f
#define MICRON_CM 10000.0f
#define MICRON_MM 1000.0f
#define GAS_MOLE_VOLUME 22413.996f	//	cm3 = milliLiters

//	sigma = fwhm * 1 / [ 2 * sqrt( ln(2) ]
#define FWHM_SIGMA 1.66510922f		//	this is [ 2 * sqrt( ln(2) ], not 1 / [ 2 * sqrt( ln(2) ]
#define SIGMA_FWHM 0.6005612f

//		added July 8, 2011  for thin film specimens
#define EXP_FLOAT_TEST	70	//	~= ln(10^-30)   1-exp(-70) = zero for floats
#define THIN_SEC_FLUOR_TEST	1	//	used to turn off secondary fluorescence for very thin films


#endif
