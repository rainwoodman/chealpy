"""
  chealpy is the Python binding of chealpix.

  chealpix is the C implementation of HealPix.
  HealPix linearizes spherical coordinate into integers.

  You will need numpy > 1.6. 

  Two precisions are provided:
    chealpy.high :upto nside=1<<29
    chealpy.low : upto nside=8192

  functions under namespace chealpy are imported from chealpy.high

  MAX_NSIDES is the limit of nsides

  wrappers are automatically generated Cython code, making use 
  of NpyIter API.

  For a feature-rich Healpix C++ binding, take a look at healpy.
  Refer to the HealPix manual for usage.

  Author: Yu Feng 2012 <yfeng1@andrew.cmu.edu>
"""

from .high import *
from .version import __version__
