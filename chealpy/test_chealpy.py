import pytest

import chealpy.low as low
import chealpy.high as high
import numpy
from numpy.testing import assert_almost_equal
from numpy.testing import assert_array_equal

SUITE  = [
(low, 4, 32),
(low, 128, 800),
(low, 8192, 1200),
(high, 4, 32),
(high, 128, 800),
(high, 16384, 1200),
]
@pytest.mark.parametrize("ns, nside, dpix", SUITE)
def test_nest_vs_ring(ns, nside, dpix):
  npix = ns.nside2npix(nside)
  ipix = numpy.arange(0, npix, dpix)

  ip2 = ns.nest2ring(nside, ipix)
  ip1 = ns.ring2nest(nside, ip2)
  assert_array_equal(ip1, ipix)


@pytest.mark.parametrize("ns, nside, dpix", SUITE)
def test_pix2ang_nest(ns, nside, dpix):
  npix = ns.nside2npix(nside)
  ipix = numpy.arange(0, npix, dpix)

  theta, phi = ns.pix2ang_nest(nside, ipix)
  ip1 = ns.ang2pix_nest(nside, theta, phi)
  assert_array_equal(ip1, ipix)

@pytest.mark.parametrize("ns, nside, dpix", SUITE)
def test_pix2vec_nest(ns, nside, dpix):
  npix = ns.nside2npix(nside)
  ipix = numpy.arange(0, npix, dpix)

  vec = ns.pix2vec_nest(nside, ipix)
  ip1 = ns.vec2pix_nest(nside, vec)
  assert_array_equal(ip1, ipix)

@pytest.mark.parametrize("ns, nside, dpix", SUITE)
def test_pix2ang_ring(ns, nside, dpix):
  npix = ns.nside2npix(nside)
  ipix = numpy.arange(0, npix, dpix)

  theta, phi = ns.pix2ang_ring(nside, ipix)
  ip1 = ns.ang2pix_ring(nside, theta, phi)
  assert_array_equal(ip1, ipix)

@pytest.mark.parametrize("ns, nside, dpix", SUITE)
def test_pix2vec_ring(ns, nside, dpix):
  npix = ns.nside2npix(nside)
  ipix = numpy.arange(0, npix, dpix)

  vec = ns.pix2vec_ring(nside, ipix)
  ip1 = ns.vec2pix_ring(nside, vec)
  assert_array_equal(ip1, ipix)

@pytest.mark.parametrize("ns, nside, dpix", SUITE)
def test_gsp2ang_ring(ns, nside, dpix):
  npix = ns.nside2npix(nside)
  ipix = numpy.arange(0, npix, dpix)

  theta, phi = ns.pix2ang_ring(nside, ipix)
  x, y = ns.ang2gsp(theta, phi)
  theta1, phi1 = ns.gsp2ang(x, y)

  assert_almost_equal(theta1, theta)
  assert_almost_equal(phi1, phi)

@pytest.mark.parametrize("ns, nside, dpix", SUITE)
def test_gsp2ang_nest(ns, nside, dpix):
  npix = ns.nside2npix(nside)
  ipix = numpy.arange(0, npix, dpix)

  theta, phi = ns.pix2ang_nest(nside, ipix)
  x, y = ns.ang2gsp(theta, phi)
  theta1, phi1 = ns.gsp2ang(x, y)

  assert_almost_equal(theta1, theta)
  assert_almost_equal(phi1, phi)

