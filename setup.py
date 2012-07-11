from numpy.distutils.core import setup, Extension
from numpy import get_include

setup(name="chealpy", version="1.0",
      package_dir = {'chealpy': 'chealpy'},
      packages = [ 'chealpy' ],
      ext_modules = [
        Extension("chealpy._ccode", 
             ["chealpy/_ccode.c",],
#             extra_compile_args=['-g -O3'], 
             extra_compile_args=['-O3'],
             libraries=[('chealpix', {'sources':['chealpix.c']})],
             include_dirs=[get_include(), '.'],
        ),
      ])

