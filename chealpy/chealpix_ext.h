#ifndef CHEALPIX_EXT_H
#define CHEALPIX_EXT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void ang2gsp(double theta, double phi, double *x, double *y);
void gsp2ang(double x, double y, double *theta, double *phi);
void gsp2pix_ring(long nside_, double x, double y, long *ipix);
void gsp2pix_nest(long nside_, double x, double y, long *ipix);
void gsp2pix_ring64(int64_t nside_, double x, double y, int64_t *ipix);
void gsp2pix_nest64(int64_t nside_, double x, double y, int64_t *ipix);

void pix2gsp_ring(long nside_, long ipix, double *x, double *y);
void pix2gsp_nest(long nside_, long ipix, double *x, double *y);
void pix2gsp_ring64(int64_t nside_, int64_t ipix, double *x, double *y);
void pix2gsp_nest64(int64_t nside_, int64_t ipix, double *x, double *y);

void ang2ngb_ring64(int64_t nside_, double theta, double phi,
    int64_t * pix,
    double * w);
void ang2ngb_nest64(int64_t nside_, double theta, double phi,
    int64_t * pix,
    double * w);

void ang2ngb_ring(long nside_, double theta, double phi,
    long * pix,
    double * w);
void ang2ngb_nest(long nside_, double theta, double phi,
    long * pix,
    double * w);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CHEALPIX_EXT_H */
