#include <math.h>
#include "Fit.h"

#include <iostream>


void fit( vector <float> &x, vector <float> &y, vector <float> &sig, float &a,
	float &b, float &siga, float &sigb, float &chi2, float &q)
{
	int i;
	float wt,t,sxoss,sx,sy,st2,ss,sigdat;

	int ndata = x.size();

	bool mwt = false;

	float sigSum = 0;

	for ( i=0; i<sig.size(); i++ ) sigSum += sig[i];

	if ( sigSum > 0 ) mwt = true;

	sx = 0;

	sy = 0;

	b=0.0;
	if (mwt) {
		ss=0.0;
		for (i=0;i<ndata;i++) {
			wt=1.0/(sig[i]*sig[i]);
			ss += wt;
			sx += x[i]*wt;
			sy += y[i]*wt;
		}
	} else {
		for (i=0;i<ndata;i++) {
			sx += x[i];
			sy += y[i];
		}
		ss=ndata;
	}
	sxoss=sx/ss;

	st2 = 0;

	if (mwt) {
		for (i=0;i<ndata;i++) {
			t=(x[i]-sxoss)/sig[i];
			st2 += t*t;
			b += t*y[i]/sig[i];
		}
	} else {
		for (i=0;i<ndata;i++) {
			t=x[i]-sxoss;
			st2 += t*t;
			b += t*y[i];
		}
	}
	b /= st2;
	a=(sy-sx*b)/ss;
	siga=sqrt((1.0+sx*sx/(ss*st2))/ss);
	sigb=sqrt(1.0/st2);
	chi2=0;
	if (!mwt) {
		for (i=0;i<ndata;i++) {

			float chi = y[i]-a-b*x[i];
			chi2 += chi * chi;

		};
		q=1.0;
		if ( ndata > 2 ) {

			sigdat=sqrt(chi2/(ndata-2));

		} else {

			sigdat=0;

		};
		siga *= sigdat;
		sigb *= sigdat;
	} else {
		for (i=0;i<ndata;i++) {

			float chi = (y[i]-a-b*x[i])/sig[i];
			chi2 += chi * chi;

		};
		q=gammq(0.5*(ndata-2),0.5*(chi2));
	}
}


float gammq(float a, float x)
{
	float gamser,gammcf,gln;

	if (x < 0.0 || a <= 0.0) return 0;
	if (x < (a+1.0)) {
		gser(gamser,a,x,gln);
		return 1.0-gamser;
	} else {
		gcf(gammcf,a,x,gln);
		return gammcf;
	}
}


#define ITMAX 100
#define EPS 3.0e-7
#define FPMIN 1.0e-30f

void gcf(float &gammcf, float a, float x, float &gln)
{
	int i;
	float an,b,c,d,del,h;

	gln=gammln(a);
	b=x+1.0-a;
	c=1/FPMIN;
	d=1/b;
	h=d;
	for (i=1;i<=ITMAX;i++) {
		an = -i*(i-a);
		b += 2.0;
		d=an*d+b;
		if (fabs(d) < FPMIN) d=FPMIN;
		c=b+an/c;
		if (fabs(c) < FPMIN) c=FPMIN;
		d=1.0/d;
		del=d*c;
		h *= del;
		if (fabs(del-1.0) < EPS) break;
	}
	if (i > ITMAX) {

		gammcf = 0;

		return;

	};
	gammcf=exp(-x+a*log(x)-(gln))*h;
}


void gser(float &gamser, float a, float x, float &gln)
{
	int n;
	float sum,del,ap;

	gln=gammln(a);
	if (x <= 0.0) {
		gamser=0.0;
		return;
	} else {
		ap=a;
		del=sum=1.0/a;
		for (n=1;n<=ITMAX;n++) {
			++ap;
			del *= x/ap;
			sum += del;
			if (fabs(del) < fabs(sum)*EPS) {
				gamser=sum*exp(-x+a*log(x)-(gln));
				return;
			}
		}
//		nrerror("a too large, ITMAX too small in routine gser");
		return;
	}
}



float gammln(float xx)
{
	double x,y,tmp,ser;
	static double cof[6]={76.18009172947146,-86.50532032941677,
		24.01409824083091,-1.231739572450155,
		0.1208650973866179e-2,-0.5395239384953e-5};
	int j;

	y=x=xx;
	tmp=x+5.5;
	tmp -= (x+0.5)*log(tmp);
	ser=1.000000000190015;
	for (j=0;j<=5;j++) ser += cof[j]/++y;
	return -tmp+log(2.5066282746310005*ser/x);
}
