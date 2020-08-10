
# do not edit. This is auto-generated
#cython: embedsignature=True
#cython: cdivision=True
cimport numpy
cimport npyiter
from libc.stdint cimport *
import numpy
numpy.import_array()

MAX_NSIDE = 1<<29

cdef extern from "chealpix.h": 
  void _ang2pix_ring64 "ang2pix_ring64" (int64_t _nside, double _theta, double _phi, int64_t * _ipix) nogil
  void _ang2pix_nest64 "ang2pix_nest64" (int64_t _nside, double _theta, double _phi, int64_t * _ipix) nogil
  void _pix2ang_ring64 "pix2ang_ring64" (int64_t _nside, int64_t _ipix, double * _theta, double * _phi) nogil
  void _pix2ang_nest64 "pix2ang_nest64" (int64_t _nside, int64_t _ipix, double * _theta, double * _phi) nogil
  void _vec2pix_ring64 "vec2pix_ring64" (int64_t _nside, double * _vec, int64_t * _ipix) nogil
  void _vec2pix_nest64 "vec2pix_nest64" (int64_t _nside, double * _vec, int64_t * _ipix) nogil
  void _pix2vec_ring64 "pix2vec_ring64" (int64_t _nside, int64_t _ipix, double * _vec) nogil
  void _pix2vec_nest64 "pix2vec_nest64" (int64_t _nside, int64_t _ipix, double * _vec) nogil
  void _nest2ring64 "nest2ring64" (int64_t _nside, int64_t _ipnest, int64_t * _ipring) nogil
  void _ring2nest64 "ring2nest64" (int64_t _nside, int64_t _ipring, int64_t * _ipnest) nogil
  int64_t _npix2nside64 "npix2nside64" (int64_t _npix) nogil
  int64_t _nside2npix64 "nside2npix64" (int64_t _nside) nogil
  void _ang2vec "ang2vec" (double _theta, double _phi, double * _vec) nogil
  void _vec2ang "vec2ang" (double * _vec, double * _theta, double * _phi) nogil


def ang2pix_ring (nside,theta,phi, ipix = None):
  "ang2pix_ring"
  shape = numpy.broadcast(1, nside,theta,phi).shape 
  if ipix is None: ipix = numpy.empty(shape, dtype='i8')

  iter = numpy.nditer([nside,theta,phi,ipix],
       op_dtypes=['i8','f8','f8','i8'],
       op_flags=[['readonly'],['readonly'],['readonly'],['writeonly']],
       flags = ['buffered', 'external_loop', 'zerosize_ok'],
       casting = 'unsafe')
  
  cdef npyiter.CIter citer
  cdef size_t size = npyiter.init(&citer, iter)
  cdef int64_t _nside
  cdef double _theta
  cdef double _phi
  cdef int64_t _ipix
  with nogil:
    while size >0:
      while size > 0:
        _nside = (<int64_t * > citer.data[0])[0] 
        _theta = (<double * > citer.data[1])[0] 
        _phi = (<double * > citer.data[2])[0] 
        _ang2pix_ring64 ( _nside, _theta, _phi, &_ipix) 
        (<int64_t * > citer.data[3])[0] = _ipix 
        npyiter.advance(&citer)
        size = size -1
      size = npyiter.next(&citer) 
  return ipix


def ang2pix_nest (nside,theta,phi, ipix = None):
  "ang2pix_nest"
  shape = numpy.broadcast(1, nside,theta,phi).shape 
  if ipix is None: ipix = numpy.empty(shape, dtype='i8')

  iter = numpy.nditer([nside,theta,phi,ipix],
       op_dtypes=['i8','f8','f8','i8'],
       op_flags=[['readonly'],['readonly'],['readonly'],['writeonly']],
       flags = ['buffered', 'external_loop', 'zerosize_ok'],
       casting = 'unsafe')
  
  cdef npyiter.CIter citer
  cdef size_t size = npyiter.init(&citer, iter)
  cdef int64_t _nside
  cdef double _theta
  cdef double _phi
  cdef int64_t _ipix
  with nogil:
    while size >0:
      while size > 0:
        _nside = (<int64_t * > citer.data[0])[0] 
        _theta = (<double * > citer.data[1])[0] 
        _phi = (<double * > citer.data[2])[0] 
        _ang2pix_nest64 ( _nside, _theta, _phi, &_ipix) 
        (<int64_t * > citer.data[3])[0] = _ipix 
        npyiter.advance(&citer)
        size = size -1
      size = npyiter.next(&citer) 
  return ipix


def pix2ang_ring (nside,ipix, theta = None,phi = None):
  "pix2ang_ring"
  shape = numpy.broadcast(1, nside,ipix).shape 
  if theta is None: theta = numpy.empty(shape, dtype='f8')
  if phi is None: phi = numpy.empty(shape, dtype='f8')

  iter = numpy.nditer([nside,ipix,theta,phi],
       op_dtypes=['i8','i8','f8','f8'],
       op_flags=[['readonly'],['readonly'],['writeonly'],['writeonly']],
       flags = ['buffered', 'external_loop', 'zerosize_ok'],
       casting = 'unsafe')
  
  cdef npyiter.CIter citer
  cdef size_t size = npyiter.init(&citer, iter)
  cdef int64_t _nside
  cdef int64_t _ipix
  cdef double _theta
  cdef double _phi
  with nogil:
    while size >0:
      while size > 0:
        _nside = (<int64_t * > citer.data[0])[0] 
        _ipix = (<int64_t * > citer.data[1])[0] 
        _pix2ang_ring64 ( _nside, _ipix, &_theta, &_phi) 
        (<double * > citer.data[2])[0] = _theta 
        (<double * > citer.data[3])[0] = _phi 
        npyiter.advance(&citer)
        size = size -1
      size = npyiter.next(&citer) 
  return theta,phi


def pix2ang_nest (nside,ipix, theta = None,phi = None):
  "pix2ang_nest"
  shape = numpy.broadcast(1, nside,ipix).shape 
  if theta is None: theta = numpy.empty(shape, dtype='f8')
  if phi is None: phi = numpy.empty(shape, dtype='f8')

  iter = numpy.nditer([nside,ipix,theta,phi],
       op_dtypes=['i8','i8','f8','f8'],
       op_flags=[['readonly'],['readonly'],['writeonly'],['writeonly']],
       flags = ['buffered', 'external_loop', 'zerosize_ok'],
       casting = 'unsafe')
  
  cdef npyiter.CIter citer
  cdef size_t size = npyiter.init(&citer, iter)
  cdef int64_t _nside
  cdef int64_t _ipix
  cdef double _theta
  cdef double _phi
  with nogil:
    while size >0:
      while size > 0:
        _nside = (<int64_t * > citer.data[0])[0] 
        _ipix = (<int64_t * > citer.data[1])[0] 
        _pix2ang_nest64 ( _nside, _ipix, &_theta, &_phi) 
        (<double * > citer.data[2])[0] = _theta 
        (<double * > citer.data[3])[0] = _phi 
        npyiter.advance(&citer)
        size = size -1
      size = npyiter.next(&citer) 
  return theta,phi


def vec2pix_ring (nside,vec, ipix = None):
  "vec2pix_ring"
  shape = numpy.broadcast(1, nside,vec[...,0],vec[...,1],vec[...,2]).shape 
  if ipix is None: ipix = numpy.empty(shape, dtype='i8')

  iter = numpy.nditer([nside,vec[...,0],vec[...,1],vec[...,2],ipix],
       op_dtypes=['i8','f8','f8','f8','i8'],
       op_flags=[['readonly'],['readonly'],['readonly'],['readonly'],['writeonly']],
       flags = ['buffered', 'external_loop', 'zerosize_ok'],
       casting = 'unsafe')
  
  cdef npyiter.CIter citer
  cdef size_t size = npyiter.init(&citer, iter)
  cdef int64_t _nside
  cdef numpy.ndarray vec_a = numpy.empty(0, dtype=('f8', 3))
  cdef double * _vec = <double *>vec_a.data
  cdef int64_t _ipix
  with nogil:
    while size >0:
      while size > 0:
        _nside = (<int64_t * > citer.data[0])[0] 
        _vec[0] = (<double *> citer.data[1])[0] 
        _vec[1] = (<double *> citer.data[2])[0] 
        _vec[2] = (<double *> citer.data[3])[0] 
        _vec2pix_ring64 ( _nside, _vec, &_ipix) 
        (<int64_t * > citer.data[4])[0] = _ipix 
        npyiter.advance(&citer)
        size = size -1
      size = npyiter.next(&citer) 
  return ipix


def vec2pix_nest (nside,vec, ipix = None):
  "vec2pix_nest"
  shape = numpy.broadcast(1, nside,vec[...,0],vec[...,1],vec[...,2]).shape 
  if ipix is None: ipix = numpy.empty(shape, dtype='i8')

  iter = numpy.nditer([nside,vec[...,0],vec[...,1],vec[...,2],ipix],
       op_dtypes=['i8','f8','f8','f8','i8'],
       op_flags=[['readonly'],['readonly'],['readonly'],['readonly'],['writeonly']],
       flags = ['buffered', 'external_loop', 'zerosize_ok'],
       casting = 'unsafe')
  
  cdef npyiter.CIter citer
  cdef size_t size = npyiter.init(&citer, iter)
  cdef int64_t _nside
  cdef numpy.ndarray vec_a = numpy.empty(0, dtype=('f8', 3))
  cdef double * _vec = <double *>vec_a.data
  cdef int64_t _ipix
  with nogil:
    while size >0:
      while size > 0:
        _nside = (<int64_t * > citer.data[0])[0] 
        _vec[0] = (<double *> citer.data[1])[0] 
        _vec[1] = (<double *> citer.data[2])[0] 
        _vec[2] = (<double *> citer.data[3])[0] 
        _vec2pix_nest64 ( _nside, _vec, &_ipix) 
        (<int64_t * > citer.data[4])[0] = _ipix 
        npyiter.advance(&citer)
        size = size -1
      size = npyiter.next(&citer) 
  return ipix


def pix2vec_ring (nside,ipix, vec = None):
  "pix2vec_ring"
  shape = numpy.broadcast(1, nside,ipix).shape 
  if vec is None: vec = numpy.empty(shape, dtype=('f8', 3))

  iter = numpy.nditer([nside,ipix,vec[...,0],vec[...,1],vec[...,2]],
       op_dtypes=['i8','i8','f8','f8','f8'],
       op_flags=[['readonly'],['readonly'],['writeonly'],['writeonly'],['writeonly']],
       flags = ['buffered', 'external_loop', 'zerosize_ok'],
       casting = 'unsafe')
  
  cdef npyiter.CIter citer
  cdef size_t size = npyiter.init(&citer, iter)
  cdef int64_t _nside
  cdef int64_t _ipix
  cdef numpy.ndarray vec_a = numpy.empty(0, dtype=('f8', 3))
  cdef double * _vec = <double *>vec_a.data
  with nogil:
    while size >0:
      while size > 0:
        _nside = (<int64_t * > citer.data[0])[0] 
        _ipix = (<int64_t * > citer.data[1])[0] 
        _pix2vec_ring64 ( _nside, _ipix, _vec) 
        (<double *> citer.data[2])[0] = _vec[0] 
        (<double *> citer.data[3])[0] = _vec[1] 
        (<double *> citer.data[4])[0] = _vec[2] 
        npyiter.advance(&citer)
        size = size -1
      size = npyiter.next(&citer) 
  return vec


def pix2vec_nest (nside,ipix, vec = None):
  "pix2vec_nest"
  shape = numpy.broadcast(1, nside,ipix).shape 
  if vec is None: vec = numpy.empty(shape, dtype=('f8', 3))

  iter = numpy.nditer([nside,ipix,vec[...,0],vec[...,1],vec[...,2]],
       op_dtypes=['i8','i8','f8','f8','f8'],
       op_flags=[['readonly'],['readonly'],['writeonly'],['writeonly'],['writeonly']],
       flags = ['buffered', 'external_loop', 'zerosize_ok'],
       casting = 'unsafe')
  
  cdef npyiter.CIter citer
  cdef size_t size = npyiter.init(&citer, iter)
  cdef int64_t _nside
  cdef int64_t _ipix
  cdef numpy.ndarray vec_a = numpy.empty(0, dtype=('f8', 3))
  cdef double * _vec = <double *>vec_a.data
  with nogil:
    while size >0:
      while size > 0:
        _nside = (<int64_t * > citer.data[0])[0] 
        _ipix = (<int64_t * > citer.data[1])[0] 
        _pix2vec_nest64 ( _nside, _ipix, _vec) 
        (<double *> citer.data[2])[0] = _vec[0] 
        (<double *> citer.data[3])[0] = _vec[1] 
        (<double *> citer.data[4])[0] = _vec[2] 
        npyiter.advance(&citer)
        size = size -1
      size = npyiter.next(&citer) 
  return vec


def nest2ring (nside,ipnest, ipring = None):
  "nest2ring"
  shape = numpy.broadcast(1, nside,ipnest).shape 
  if ipring is None: ipring = numpy.empty(shape, dtype='i8')

  iter = numpy.nditer([nside,ipnest,ipring],
       op_dtypes=['i8','i8','i8'],
       op_flags=[['readonly'],['readonly'],['writeonly']],
       flags = ['buffered', 'external_loop', 'zerosize_ok'],
       casting = 'unsafe')
  
  cdef npyiter.CIter citer
  cdef size_t size = npyiter.init(&citer, iter)
  cdef int64_t _nside
  cdef int64_t _ipnest
  cdef int64_t _ipring
  with nogil:
    while size >0:
      while size > 0:
        _nside = (<int64_t * > citer.data[0])[0] 
        _ipnest = (<int64_t * > citer.data[1])[0] 
        _nest2ring64 ( _nside, _ipnest, &_ipring) 
        (<int64_t * > citer.data[2])[0] = _ipring 
        npyiter.advance(&citer)
        size = size -1
      size = npyiter.next(&citer) 
  return ipring


def ring2nest (nside,ipring, ipnest = None):
  "ring2nest"
  shape = numpy.broadcast(1, nside,ipring).shape 
  if ipnest is None: ipnest = numpy.empty(shape, dtype='i8')

  iter = numpy.nditer([nside,ipring,ipnest],
       op_dtypes=['i8','i8','i8'],
       op_flags=[['readonly'],['readonly'],['writeonly']],
       flags = ['buffered', 'external_loop', 'zerosize_ok'],
       casting = 'unsafe')
  
  cdef npyiter.CIter citer
  cdef size_t size = npyiter.init(&citer, iter)
  cdef int64_t _nside
  cdef int64_t _ipring
  cdef int64_t _ipnest
  with nogil:
    while size >0:
      while size > 0:
        _nside = (<int64_t * > citer.data[0])[0] 
        _ipring = (<int64_t * > citer.data[1])[0] 
        _ring2nest64 ( _nside, _ipring, &_ipnest) 
        (<int64_t * > citer.data[2])[0] = _ipnest 
        npyiter.advance(&citer)
        size = size -1
      size = npyiter.next(&citer) 
  return ipnest


def npix2nside (npix, nside = None):
  "npix2nside"
  shape = numpy.broadcast(1, npix).shape 
  if nside is None: nside = numpy.empty(shape, dtype='i8')

  iter = numpy.nditer([npix,nside],
       op_dtypes=['i8','i8'],
       op_flags=[['readonly'],['writeonly']],
       flags = ['buffered', 'external_loop', 'zerosize_ok'],
       casting = 'unsafe')
  
  cdef npyiter.CIter citer
  cdef size_t size = npyiter.init(&citer, iter)
  cdef int64_t _npix
  cdef int64_t _nside
  with nogil:
    while size >0:
      while size > 0:
        _npix = (<int64_t * > citer.data[0])[0] 
        _nside = _npix2nside64 ( _npix) 
        (<int64_t * > citer.data[1])[0] = _nside 
        npyiter.advance(&citer)
        size = size -1
      size = npyiter.next(&citer) 
  return nside


def nside2npix (nside, npix = None):
  "nside2npix"
  shape = numpy.broadcast(1, nside).shape 
  if npix is None: npix = numpy.empty(shape, dtype='i8')

  iter = numpy.nditer([nside,npix],
       op_dtypes=['i8','i8'],
       op_flags=[['readonly'],['writeonly']],
       flags = ['buffered', 'external_loop', 'zerosize_ok'],
       casting = 'unsafe')
  
  cdef npyiter.CIter citer
  cdef size_t size = npyiter.init(&citer, iter)
  cdef int64_t _nside
  cdef int64_t _npix
  with nogil:
    while size >0:
      while size > 0:
        _nside = (<int64_t * > citer.data[0])[0] 
        _npix = _nside2npix64 ( _nside) 
        (<int64_t * > citer.data[1])[0] = _npix 
        npyiter.advance(&citer)
        size = size -1
      size = npyiter.next(&citer) 
  return npix


def ang2vec (theta,phi, vec = None):
  "ang2vec"
  shape = numpy.broadcast(1, theta,phi).shape 
  if vec is None: vec = numpy.empty(shape, dtype=('f8', 3))

  iter = numpy.nditer([theta,phi,vec[...,0],vec[...,1],vec[...,2]],
       op_dtypes=['f8','f8','f8','f8','f8'],
       op_flags=[['readonly'],['readonly'],['writeonly'],['writeonly'],['writeonly']],
       flags = ['buffered', 'external_loop', 'zerosize_ok'],
       casting = 'unsafe')
  
  cdef npyiter.CIter citer
  cdef size_t size = npyiter.init(&citer, iter)
  cdef double _theta
  cdef double _phi
  cdef numpy.ndarray vec_a = numpy.empty(0, dtype=('f8', 3))
  cdef double * _vec = <double *>vec_a.data
  with nogil:
    while size >0:
      while size > 0:
        _theta = (<double * > citer.data[0])[0] 
        _phi = (<double * > citer.data[1])[0] 
        _ang2vec ( _theta, _phi, _vec) 
        (<double *> citer.data[2])[0] = _vec[0] 
        (<double *> citer.data[3])[0] = _vec[1] 
        (<double *> citer.data[4])[0] = _vec[2] 
        npyiter.advance(&citer)
        size = size -1
      size = npyiter.next(&citer) 
  return vec


def vec2ang (vec, theta = None,phi = None):
  "vec2ang"
  shape = numpy.broadcast(1, vec[...,0],vec[...,1],vec[...,2]).shape 
  if theta is None: theta = numpy.empty(shape, dtype='f8')
  if phi is None: phi = numpy.empty(shape, dtype='f8')

  iter = numpy.nditer([vec[...,0],vec[...,1],vec[...,2],theta,phi],
       op_dtypes=['f8','f8','f8','f8','f8'],
       op_flags=[['readonly'],['readonly'],['readonly'],['writeonly'],['writeonly']],
       flags = ['buffered', 'external_loop', 'zerosize_ok'],
       casting = 'unsafe')
  
  cdef npyiter.CIter citer
  cdef size_t size = npyiter.init(&citer, iter)
  cdef numpy.ndarray vec_a = numpy.empty(0, dtype=('f8', 3))
  cdef double * _vec = <double *>vec_a.data
  cdef double _theta
  cdef double _phi
  with nogil:
    while size >0:
      while size > 0:
        _vec[0] = (<double *> citer.data[0])[0] 
        _vec[1] = (<double *> citer.data[1])[0] 
        _vec[2] = (<double *> citer.data[2])[0] 
        _vec2ang ( _vec, &_theta, &_phi) 
        (<double * > citer.data[3])[0] = _theta 
        (<double * > citer.data[4])[0] = _phi 
        npyiter.advance(&citer)
        size = size -1
      size = npyiter.next(&citer) 
  return theta,phi

