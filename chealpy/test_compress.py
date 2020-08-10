import pytest

import numpy
from chealpy.high import nside2npix
from chealpy.compress import SparseMap
from numpy.testing import assert_allclose

class TestSparseMap:
    def test_from_dense(self):
        i = numpy.arange(nside2npix(32)) 
        a = numpy.arange(nside2npix(32), dtype='f8') 
        s = SparseMap.from_dense(32, a)
        assert_allclose(s.get(i), a)

    def test_sparse_map_down_sample(self):
        i = numpy.arange(nside2npix(8)) 
        a = numpy.arange(nside2npix(8), dtype='f8') 
        s = SparseMap.from_dense(8, a)
        s2 = s.down_sample()
        i2 = numpy.arange(nside2npix(4)) 
        a2 = numpy.arange(nside2npix(4), dtype='f8') 
        assert_allclose(s2.get(i2), a.reshape(-1, 4).mean(axis=-1))

    def test_sparse_map_down_sample_edges(self):
        i = numpy.arange(nside2npix(8)) 
        a = numpy.arange(nside2npix(8), dtype='f8') 
        s = SparseMap.from_dense(8, a[3:], ipix_start=3)
        s2 = s.down_sample()
        i2 = numpy.arange(nside2npix(4)) 
        a2 = numpy.arange(nside2npix(4), dtype='f8') 
        afill = a.copy()
        afill[:3] = a[3]
        assert_allclose(s2.get(i2), afill.reshape(-1, 4).mean(axis=-1))

