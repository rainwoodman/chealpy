from numpy.distutils.core import setup, Extension
from numpy import get_include

setup(name="chealpy", version="0.1.1",
      author="Yu Feng",
      author_email="yfeng1@berkeley.edu",
      description="Python Binding of chealpix",
      zip_safe=False,
      license="GPLv2+",
#      download_url="https://github.com/rainwoodman/chealpy/zipball/master",
      url="http://github.com/rainwoodman/chealpy",
      package_dir = {'chealpy': 'chealpy'},
      packages = [ 'chealpy' ],
      ext_modules = [
        Extension("chealpy._ccode", 
             ["chealpy/_ccode.c",],
             extra_compile_args=['-O3'],
             libraries=[('chealpix', {'sources':['chealpix.c']})],
             depends=['chealpix.h'],
             include_dirs=[get_include(), '.'],
        ),
      ])

