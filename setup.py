from setuptools import setup
from Cython.Build import cythonize
from distutils.extension import Extension
import numpy

def find_version(path):
    import re
    # path shall be a plain ascii text file.
    s = open(path, 'rt').read()
    version_match = re.search(r"^__version__ = ['\"]([^'\"]*)['\"]",
                              s, re.M)
    if version_match:
        return version_match.group(1)
    raise RuntimeError("Version not found")


extensions = [
    Extension("chealpy.low", 
         sources=["chealpy/low.pyx", "chealpy/chealpix.c"],
         extra_compile_args=['-O3'],
         depends=['chealpy/chealpix.h'],
         include_dirs=[numpy.get_include(), 'chealpy/'],
    ),
    Extension("chealpy.high", 
         sources=["chealpy/high.pyx", "chealpy/chealpix.c"],
         extra_compile_args=['-O3'],
         depends=['chealpy/chealpix.h'],
         include_dirs=[numpy.get_include(), 'chealpy/'],
    ),
    Extension("chealpy.compress", 
         sources=["chealpy/compress.pyx", "chealpy/chealpix.c", "chealpy/hp_compress.c"],
         extra_compile_args=['-O3'],
         depends=['chealpy/chealpix.h', 'chealpy/hp_compress.h'],
         include_dirs=[numpy.get_include(), 'chealpy/'],
    ),
]

setup(name="chealpy",
      author="Yu Feng",
      author_email="yfeng1@berkeley.edu",
      description="Python Binding of chealpix",
      ext_modules = cythonize(extensions),
      install_requires=['cython', 'numpy'],
      license="GPLv2+",
      package_dir = {'chealpy': 'chealpy'},
      packages = [ 'chealpy' ],
      url="http://github.com/rainwoodman/chealpy",
      version=find_version("chealpy/version.py"),
      zip_safe=False,
)

