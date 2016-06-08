#ifndef Fit_h
#define Fit_h

#include <vector>

using namespace std;

void fit( vector <float> &x, vector <float> &y, vector <float> &sig, float &a,
	float &b, float &siga, float &sigb, float &chi2, float &q);

float gammq(float a, float x);

void gcf(float &gammcf, float a, float x, float &gln);

void gser(float &gamser, float a, float x, float &gln);

float gammln(float xx);

#endif
