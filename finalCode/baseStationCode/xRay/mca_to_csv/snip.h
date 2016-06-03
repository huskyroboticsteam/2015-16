#ifndef snip_h
#define snip_h

#include <vector>

void sgsmth( const std::vector <float> &y,  std::vector <float> &s, 
            const int ich1, const int ich2, const int iwid );

void snipbg( const  std::vector <float> &y,  std::vector <float> &yback, const int ich1, 
            const int ich2, const int fwhm, const int niter );

void snipbg_lsq( const  std::vector <float> &y,  std::vector <float> &yback, const int ich1, 
            const int ich2, const int fwhm, const int niter );


#endif
