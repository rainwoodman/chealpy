cimport numpy
cimport npyiter
from libc.stdint cimport *
import numpy
numpy.import_array()

cdef extern from "hp_compress.h": 
  ctypedef struct hp_sparse_t:
    int nside
    ssize_t size
    int64_t * pix
    double * value

  hp_sparse_t * hp_sparse_from_dense(int nside, int64_t ipix_start, int64_t ipix_end, double * values)
  hp_sparse_t * hp_sparse_down_sample(hp_sparse_t * map)

  double hp_sparse_get(hp_sparse_t * map, int64_t ipix)
  void hp_sparse_free(hp_sparse_t * map)

cdef class SparseMap:
    cdef hp_sparse_t * cself;

    
    property size:
        def __get__(self):
            return self.cself.size

    property pix:
        def __get__(self):
            return numpy.array(<int64_t[:self.cself.size]> self.cself.pix)

    property value:
        def __get__(self):
            return numpy.array(<double[:self.cself.size]> self.cself.value)
        
    @staticmethod
    cdef create(hp_sparse_t * cself):
        r = SparseMap()
        r.cself = cself
        return r

    def __dealloc__(self):
        if self.cself != NULL:
            hp_sparse_free(self.cself)

    @staticmethod
    def from_dense(int nside, double [::1] value, int64_t ipix_start=0):
        return SparseMap.create(hp_sparse_from_dense(nside, ipix_start, ipix_start + value.shape[0], &value[0]))

    def get(self, pix):
        cdef numpy.ndarray ipix = numpy.ascontiguousarray(pix, dtype='i8')
        cdef numpy.ndarray out = numpy.empty(numpy.shape(ipix), dtype='f8')
        cdef numpy.intp_t i
        for i in range(ipix.size):
            (<double*> out.data)[i] = hp_sparse_get(self.cself, (<int64_t*>ipix.data)[i])
        return out

    def down_sample(self):
        return SparseMap.create(hp_sparse_down_sample(self.cself))

