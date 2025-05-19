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
import sys
import os
print(f"DEBUG: In chealpy/__init__.py, current dir: {os.getcwd()}")
print(f"DEBUG: In chealpy/__init__.py, __file__ is {__file__}")
print(f"DEBUG: In chealpy/__init__.py, listing chealpy directory ({os.path.dirname(__file__)}): {os.listdir(os.path.dirname(os.path.abspath(__file__)) if os.path.exists(os.path.dirname(os.path.abspath(__file__))) else 'Error: path does not exist')}")
print(f"DEBUG: In chealpy/__init__.py, sys.path: {sys.path}")

print("DEBUG: Attempting from .high import *")
from .high import *
print("DEBUG: Attempting from .version import __version__")
from .version import __version__
print("DEBUG: All imports in __init__.py successful (if this line is reached)")
