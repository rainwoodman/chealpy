/* -----------------------------------------------------------------------------
 *
 *  Copyright (C) 1997-2019 Krzysztof M. Gorski, Eric Hivon, Martin Reinecke,
 *                          Benjamin D. Wandelt, Anthony J. Banday,
 *                          Matthias Bartelmann,
 *                          Reza Ansari & Kenneth M. Ganga
 *
 *
 *  This file is part of HEALPix.
 *
 *  HEALPix is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  HEALPix is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with HEALPix; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  For more information about HEALPix see http://healpix.sourceforge.net
 *
 *----------------------------------------------------------------------------*/
 /*
  * chealpix.h
  */

 #ifndef CHEALPIX_H
 #define CHEALPIX_H

 #include <stdint.h>
+#include <stdlib.h>

 #ifdef __cplusplus
 extern "C" {
@@ -41,16 +42,16 @@
 /* pixel operations */
 /* ---------------- */
 /*! Sets \a *ipix to the pixel number in NEST scheme at resolution \a nside,
- which contains the position \a theta, \a phi. */
+ which contains the position \a theta, \a phi. 
+  \deprecated Use the more precise ang2pix_ring64 interface. */
 void ang2pix_nest(long nside, double theta, double phi, long *ipix);
 /*! Sets \a *ipix to the pixel number in RING scheme at resolution \a nside,
- which contains the position \a theta, \a phi. */
+ which contains the position \a theta, \a phi. 
+  \deprecated Use the more precise ang2pix_ring64 interface. */
 void ang2pix_ring(long nside, double theta, double phi, long *ipix);
 
 /*! Sets \a theta and \a phi to the angular position of the center of pixel
- \a ipix in NEST scheme at resolution \a nside. */
-void pix2ang_nest(long nside, long ipix, double *theta, double *phi);
-/*! Sets \a theta and \a phi to the angular position of the center of pixel
- \a ipix in NEST scheme at resolution \a nside. */
+ \a ipix in NEST scheme at resolution \a nside. 
+  \deprecated Use the more precise pix2ang_nest64 interface. */
 void pix2ang_ring(long nside, long ipix, double *theta, double *phi);
 
 /*! Computes the RING pixel index of pixel \a ipnest at resolution \a nside
@@ -62,7 +63,6 @@
 void nest2xyf (int nside, int pix, int *ix, int *iy, int *face_num);
 int xyf2ring (int nside, int ix, int iy, int face_num);
 void ring2xyf (int nside, int pix, int *ix, int *iy, int *face_num);
-
 
 /*! Returns \a 12*nside*nside. */
 long nside2npix(long nside);
@@ -97,35 +97,45 @@
 /* operations on Nside values up to 2^29 */
 
 /*! Sets \a *ipix to the pixel number in NEST scheme at resolution \a nside,
- which contains the position \a theta, \a phi. */
+ which contains the position \a theta, \a phi. 
+  Use this function for pixel numbers in NESTED ordering scheme. */
 void ang2pix_nest64(int64_t nside, double theta, double phi, int64_t *ipix);
 /*! Sets \a *ipix to the pixel number in RING scheme at resolution \a nside,
- which contains the position \a theta, \a phi. */
+ which contains the position \a theta, \a phi. 
+  Use this function for pixel numbers in RING ordering scheme. */
 void ang2pix_ring64(int64_t nside, double theta, double phi, int64_t *ipix);
 
 /*! Sets \a theta and \a phi to the angular position of the center of pixel
- \a ipix in NEST scheme at resolution \a nside. */
+ \a ipix in NEST scheme at resolution \a nside. 
+  Use this function for pixel numbers in NESTED ordering scheme. */
 void pix2ang_nest64(int64_t nside, int64_t ipix, double *theta, double *phi);
 /*! Sets \a theta and \a phi to the angular position of the center of pixel
- \a ipix in RING scheme at resolution \a nside. */
+ \a ipix in RING scheme at resolution \a nside. 
+  Use this function for pixel numbers in RING ordering scheme. */
 void pix2ang_ring64(int64_t nside, int64_t ipix, double *theta, double *phi);
 
 /*! Computes the RING pixel index of pixel \a ipnest at resolution \a nside
- and returns it in \a *ipring. On error, \a *ipring is set to -1. */
+ and returns it in \a *ipring. On error, \a *ipring is set to -1 and errno is
+ set to EDOM. 
+  \deprecated Use the more precise nest2ring64 interface. */
 void nest2ring(long nside, long ipnest, long *ipring);
 /*! Computes the NEST pixel index of pixel \a ipring at resolution \a nside
- and returns it in \a *ipring. On error, \a *ipnest is set to -1. */
+ and returns it in \a *ipring. On error, \a *ipnest is set to -1 and errno is
+ set to EDOM. 
+  \deprecated Use the more precise ring2nest64 interface. */
 void ring2nest(long nside, long ipring, long *ipnest);
 
 int64_t xyf2nest64 (int64_t nside, int ix, int iy, int face_num);
 void nest2xyf64 (int64_t nside, int64_t pix, int *ix, int *iy, int *face_num);
 int64_t xyf2ring64 (int64_t nside, int ix, int iy, int face_num);
 void ring2xyf64 (int64_t nside, int64_t pix, int *ix, int *iy, int *face_num);
+
 /*! Returns \a 12*nside*nside. */
 int64_t nside2npix64(int64_t nside);
 /*! Returns \a sqrt(npix/12) if this is an integer number, otherwise \a -1. */
 long npix2nside64(int64_t npix);
+
+
 
 
 /*! Sets \a *ipix to the pixel number in NEST scheme at resolution \a nside,
@@ -146,11 +156,24 @@
 /* FITS operations */
 /* --------------- */
 
-float *read_healpix_map (const char *infile, long *nside, char *coordsys,
-  char *ordering);
-
-void write_healpix_map (const float *signal, long nside, const char *filename,
-  char nest, const char *coordsys);
+/*! Reads a HEALPix map (single column of type float) from the FITS file
+ \a infile.  Returns a pointer to the map data, the resolution parameter
+ in \a *nside, the coordinate system in \a *coordsys and the ordering scheme
+ in \a *ordering.  If \a telescope is not NULL, it will be set to the value
+ of the TELESCOP keyword, or NULL if the keyword is not present. The same
+ applies to \a instrument.  If \a object is not NULL, it will be set to the
+ value of the OBJECT keyword, or zero if the keyword is not present. */
+float *read_healpix_map(const char *infile, long *nside,
+  char *coordsys, char *ordering, char **telescope, char **instrument,
+  int *object);
+
+/*! Returns the resolution parameter for a map with \a npix pixels or -1 if
+ \a npix is not a valid number of pixels (i.e. not equal to 12*nside^2 for some
+ integer nside).  If \a *nside is not NULL, the resolution parameter is
+ additionally stored in \a *nside. */
+long npix2nside_checked(long npix, long *nside);
+int64_t npix2nside64_checked(int64_t npix, int64_t *nside);
+
 
 long get_fits_size(const char *filename, long *nside, char *ordering);
