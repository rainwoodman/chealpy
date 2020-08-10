import pytest

import chealpy.low as low
import chealpy.high as high
import numpy

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
  assert (ip1 == ipix).all()


@pytest.mark.parametrize("ns, nside, dpix", SUITE)
def test_pix2ang_nest(ns, nside, dpix):
  npix = ns.nside2npix(nside)
  ipix = numpy.arange(0, npix, dpix)

  theta, phi = ns.pix2ang_nest(nside, ipix)
  ip1 = ns.ang2pix_nest(nside, theta, phi)
  assert (ip1 == ipix).all()

@pytest.mark.parametrize("ns, nside, dpix", SUITE)
def test_pix2vec_nest(ns, nside, dpix):
  npix = ns.nside2npix(nside)
  ipix = numpy.arange(0, npix, dpix)

  vec = ns.pix2vec_nest(nside, ipix)
  ip1 = ns.vec2pix_nest(nside, vec)
  assert (ip1 == ipix).all()

@pytest.mark.parametrize("ns, nside, dpix", SUITE)
def test_pix2ang_ring(ns, nside, dpix):
  npix = ns.nside2npix(nside)
  ipix = numpy.arange(0, npix, dpix)

  theta, phi = ns.pix2ang_ring(nside, ipix)
  ip1 = ns.ang2pix_ring(nside, theta, phi)
  assert (ip1 == ipix).all()

@pytest.mark.parametrize("ns, nside, dpix", SUITE)
def test_pix2vec_ring(ns, nside, dpix):
  npix = ns.nside2npix(nside)
  ipix = numpy.arange(0, npix, dpix)

  vec = ns.pix2vec_ring(nside, ipix)
  ip1 = ns.vec2pix_ring(nside, vec)
  assert (ip1 == ipix).all()

