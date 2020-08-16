#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "chealpix.h"

static const double twothird=2.0/3.0;
static const double pi=3.141592653589793238462643383279502884197;
static const double twopi=6.283185307179586476925286766559005768394;
static const double halfpi=1.570796326794896619231321691639751442099;
static const double inv_halfpi=0.6366197723675813430755350534900574;

/*! Returns the remainder of the division \a v1/v2.
    The result is non-negative.
    \a v1 can be positive or negative; \a v2 must be positive. */
static double fmodulo (double v1, double v2)
  {
  if (v1>=0)
    return (v1<v2) ? v1 : fmod(v1,v2);
  double tmp=fmod(v1,v2)+v2;
  return (tmp==v2) ? 0. : tmp;
  }

static void ang2xy_z_phi64(double z, double s, double phi, double *x, double *y)
{
    double za = fabs(z);
    if (za < twothird)  {
        /* equatorial */
        *x = phi;
        *y = (0.75 * halfpi) * z;
    } else {
        /* caps */
        double tmp;
        if (s > -2.) {
            /* expand near za ~ 1, notice s >= 0 */
            tmp = s/sqrt((1+za)/3.);
        } else {
            tmp = sqrt(3*(1-za));
        }
        double siga = (2-tmp);  /* siga >= 0 */
        double phi_t = fmodulo(phi,halfpi);
        *x = phi - (siga-1) * (phi_t-halfpi/2);
        *y = (halfpi/2) * siga; 
        if (z < 0) {
            *y = -*y;
        }
    }
}

/*
 * Convert :math:`\theta` :math:`\phi` to :math:`x_s` :math:`y_s`.
 * Refer to Section 4.4 of http://adsabs.harvard.edu/abs/2005ApJ...622..759G
 */
void ang2xy(double theta, double phi, double *x, double *y)
{
  double cth=cos(theta), sth=(fabs(cth)>0.99) ? sin(theta) : -5;
  ang2xy_z_phi64 (cth,sth,phi,x,y);
}

static void xy2ang_z_phi64 (double x, double y,
  double *z, double *s, double *phi)
{
    double ya = fabs(y);
    *s = -5;
    if(ya < halfpi/2) {
        /* equatorial */
        *z = (4./3 * inv_halfpi) * y;
        *phi = x;
    } else {
        /* caps */
        double xt = fmodulo(x, halfpi);
        double tmp = ya * inv_halfpi - 1;
        if(tmp < 0) {
           *phi = x-(1+0.5/tmp)*(xt-halfpi/2);
        } else {
           /* round off error or at the poles */
           *phi = x;
        }
        tmp = 1-ya*inv_halfpi;
        tmp = (2*twothird)*tmp*tmp;
        *z = 1 - tmp;
        if(*z>0.99) {
            /* return s when |z| ~ 1 */
            *s = sqrt(tmp*(2 - tmp));
        }
        if (y < 0) {
            *z = -*z;
        }
    }
}


/*
 * Convert :math:`x_s` :math:`y_s` to :math:`theta` :math:`phi`.
 * Refer to Section 4.4 of http://adsabs.harvard.edu/abs/2005ApJ...622..759G
 */
void xy2ang(double x, double y, double *theta, double *phi)
{
  double z,s;
  xy2ang_z_phi64 (x,y,&z,&s,phi);
  *theta= (s<-2) ? acos(z) : atan2(s,z);
}

