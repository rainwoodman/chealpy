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

static void ang2gsp_z_phi64(double z, double s, double phi, double *x, double *y)
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
void ang2gsp(double theta, double phi, double *x, double *y)
{
  double cth=cos(theta), sth=(fabs(cth)>0.99) ? sin(theta) : -5;
  ang2gsp_z_phi64 (cth,sth,phi,x,y);
}

static void gsp2ang_z_phi64 (double x, double y,
  double *z, double *s, double *phi)
{
    x = fmodulo(x, twopi);
    y = fmodulo(y + pi, twopi) - pi;
    /* slide to the opposite side if we are slightly over the edge in y. */
    if(y > halfpi) {
        y = pi - y;
        x = x + pi;
    }
    if(y < -halfpi) {
        y = -pi - y;
        x = x + pi;
    }
    double ya = fabs(y);
    *s = -5;
    if(ya < halfpi/2) {
        /* equatorial */
        *z = (4./3 * inv_halfpi) * y;
        *phi = x;
    } else {
        /* caps */
        double xt = fmodulo(x, halfpi);
        /* extend the gray area of Figure 5.
         * stitch to the nearby triangles.
         *  */
        if(ya > xt + halfpi/2) {
            /* fill the left gray with the right white of
             * previous modulo. rotate 90 degrees (CW, y>0)*/
            x = (x - xt) + halfpi/2 - ya;
            ya = halfpi/2 + xt;
            x = fmodulo(x, twopi);
            xt = fmodulo(x, halfpi);
        } else if (ya > - xt + halfpi*3/2) {
            /* fill the right gray with the left white of
             * next modulo. rotate 90 degrees (CCW, y>0). */
            x = (x - xt) + halfpi/2 + ya;
            ya = halfpi/2 + halfpi - xt;
            x = fmodulo(x, twopi);
            xt = fmodulo(x, halfpi);
        }
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
void gsp2ang(double x, double y, double *theta, double *phi)
{
  double z,s;
  gsp2ang_z_phi64 (x,y,&z,&s,phi);
  *theta= (s<-2) ? acos(z) : atan2(s,z);
}

void gsp2pix_ring(long nside_, double x, double y, long *ipix)
{
    double theta, phi;
    gsp2ang(x, y, &theta, &phi);
    ang2pix_ring(nside_, theta, phi, ipix);
}

void gsp2pix_nest(long nside_, double x, double y, long *ipix)
{
    double theta, phi;
    gsp2ang(x, y, &theta, &phi);
    ang2pix_nest(nside_, theta, phi, ipix);
}

void gsp2pix_ring64(int64_t nside_, double x, double y, int64_t *ipix)
{
    double theta, phi;
    gsp2ang(x, y, &theta, &phi);
    ang2pix_ring64(nside_, theta, phi, ipix);
}

void gsp2pix_nest64(int64_t nside_, double x, double y, int64_t *ipix)
{
    double theta, phi;
    gsp2ang(x, y, &theta, &phi);
    ang2pix_nest64(nside_, theta, phi, ipix);
}

void pix2gsp_ring(long nside_, long ipix, double *x, double *y)
{
    double theta, phi;
    pix2ang_ring(nside_, ipix, &theta, &phi);
    ang2gsp(theta, phi, x,y);
}

void pix2gsp_nest(long nside_, long ipix, double *x, double *y)
{
    double theta, phi;
    pix2ang_nest(nside_, ipix, &theta, &phi);
    ang2gsp(theta, phi, x,y);
}

void pix2gsp_ring64(int64_t nside_, int64_t ipix, double *x, double *y)
{
    double theta, phi;
    pix2ang_ring64(nside_, ipix, &theta, &phi);
    ang2gsp(theta, phi,x,y);
}

void pix2gsp_nest64(int64_t nside_, int64_t ipix, double *x, double *y)
{
    double theta, phi;
    pix2ang_nest64(nside_, ipix, &theta, &phi);
    ang2gsp(theta, phi, x,y);
}


void ang2ngb_ring64(int64_t nside_, double theta, double phi,
    int64_t * pix,
    double * w)
{
    double step = halfpi / nside_;
    double halfstep = 0.5 * step;

    double xs, ys;
    double xc, yc;
    ang2gsp(theta, phi, &xs, &ys);
    ang2pix_ring64(nside_, theta, phi, &pix[0]);

    /* central pixel */
    pix2gsp_ring64(nside_, pix[0], &xc, &yc);
    double dx = xs - xc;
    double dy = ys - yc;

    if(dx > halfpi) dx = dx - twopi;
    if(dy > halfpi) dy = dy - twopi;

    /* step vectors */
    /* t1 = delta dot (1, 1) */
    double t1 = dx + dy;
    /* t2 = delta dot (1, -1) */
    double t2 = dx - dy;
    int s1 = t1 >=0? 1: -1;
    int s2 = t2 >=0? 1: -1;

    /* find two neighbour pixels for interpolation,
     * but avoid jumping to singularities at the pole.
     * any number >0.5 < 1.0 should work. */
    double p = 0.75 * halfstep;
    gsp2pix_ring64(nside_, xc + s1*p, yc + s1*p, &pix[1]);
    gsp2pix_ring64(nside_, xc + s2*p, yc - s2*p, &pix[2]);

    /* abs(t) interpolates between 0 and l; but due to pix constraint,
     * abs(t) < 0.5 l */
    const double l = sqrt(2) * halfstep;
    t1 = fabs(t1);
    t2 = fabs(t2);
    w[1] = l - t2;
    w[2] = l - t1;
    w[0] = w[1]*w[2];
    w[1] = t1*w[1];
    w[2] = t2*w[2];

    double inv_norm = 1 / (w[0] + w[1] + w[2]);
    w[0] *= inv_norm;
    w[1] *= inv_norm;
    w[2] *= inv_norm;
}

void ang2ngb_nest64(int64_t nside_, double theta, double phi,
    int64_t * pix,
    double * w)
{
    ang2ngb_ring64(nside_, theta, phi, pix, w);
    ring2nest64(nside_, pix[0], &pix[0]);
    ring2nest64(nside_, pix[1], &pix[1]);
    ring2nest64(nside_, pix[2], &pix[2]);
}

void ang2ngb_ring(long nside_, double theta, double phi,
    long * pix,
    double * w)
{
    int64_t pix64[3];
    ang2ngb_ring64(nside_, theta, phi, pix64, w);
    pix[0] = pix64[0];
    pix[1] = pix64[1];
    pix[2] = pix64[2];
}

void ang2ngb_nest(long nside_, double theta, double phi,
    long * pix,
    double * w)
{
    int64_t pix64[3];
    ang2ngb_nest64(nside_, theta, phi, pix64, w);
    pix[0] = pix64[0];
    pix[1] = pix64[1];
    pix[2] = pix64[2];
}
