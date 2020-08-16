#ifndef CHEALPIX_EXT_H
#define CHEALPIX_EXT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void ang2xy(double theta, double phi, double *x, double *y);
void xy2ang(double x, double y, double *theta, double *phi);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CHEALPIX_EXT_H */
