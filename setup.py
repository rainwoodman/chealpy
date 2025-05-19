from setuptools import setup
from Cython.Build import cythonize
from distutils.extension import Extension
from distutils.sysconfig import get_python_inc
import numpy
import sysconfig

def find_version(path):
    import re
    # path shall be a plain ascii text file.
    s = open(path, "rt").read()
    version_match = re.search(r"^__version__ = ['\"]([^'\"]*)['\"]",
                              s, re.M)
    if version_match:
        return version_match.group(1)
    raise RuntimeError("Version not found")


extensions = [
    Extension("chealpy.low", 
         sources=["chealpy/low.pyx", "chealpy/chealpix.c", "chealpy/chealpix_ext.c"],
         extra_compile_args=["-O3"],
         define_macros=[('NPY_NO_DEPRECATED_API', 'NPY_1_7_API_VERSION')],
         depends=["chealpy/chealpix.h", "chealpy/chealpix_ext.h"],
         include_dirs=[numpy.get_include(), "chealpy/", sysconfig.get_paths()['include'], get_python_inc()],
    ),
    Extension("chealpy.high", 
         sources=["chealpy/high.pyx", "chealpy/chealpix.c", "chealpy/chealpix_ext.c"],
         extra_compile_args=["-O3"],
         define_macros=[('NPY_NO_DEPRECATED_API', 'NPY_1_7_API_VERSION')],
         depends=["chealpy/chealpix.h", "chealpy/chealpix_ext.h"],
         include_dirs=[numpy.get_include(), "chealpy/", sysconfig.get_paths()['include'], get_python_inc()],
    ),
    Extension("chealpy.compress", 
         sources=["chealpy/compress.pyx", "chealpy/chealpix.c", "chealpy/chealpix_ext.c", "chealpy/hp_compress.c"],
         extra_compile_args=["-O3"],
         define_macros=[('NPY_NO_DEPRECATED_API', 'NPY_1_7_API_VERSION')],
         depends=["chealpy/chealpix.h", "chealpy/chealpix_ext.h", "chealpy/hp_compress.h"],
         include_dirs=[numpy.get_include(), "chealpy/", sysconfig.get_paths()['include'], get_python_inc()],
    ),
]

setup(name="chealpy",
      author="Yu Feng",
      author_email="yfeng1@berkeley.edu",
      description="Python Binding of chealpix",
      ext_modules = cythonize(extensions),
      install_requires=["cython", "numpy"],
      license="GPLv2+",
      package_dir = {"chealpy": "chealpy"},
      packages = [ "chealpy" ],
      url="http://github.com/rainwoodman/chealpy",
      version=find_version("chealpy/version.py"),
      zip_safe=False,
)

