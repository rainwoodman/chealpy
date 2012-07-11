import chealpy.low as low
import chealpy.high as high
import numpy

def suite1(ns, nside, dpix):
  npix = ns.nside2npix(nside)
  ipix = numpy.arange(0, npix, dpix)
  ip2 = ns.nest2ring(nside, ipix)
  ip1 = ns.ring2nest(nside, ip2)
  assert (ip1 == ipix).all()

  theta, phi = ns.pix2ang_nest(nside, ipix)
  ip1 = ns.ang2pix_nest(nside, theta, phi)
  assert (ip1 == ipix).all()

  vec = ns.pix2vec_nest(nside, ipix)
  ip1 = ns.vec2pix_nest(nside, vec)
  assert (ip1 == ipix).all()

def suite2(ns, nside, dpix):
  npix = ns.nside2npix(nside)
  ipix = numpy.arange(0, npix, dpix)

  theta, phi = ns.pix2ang_ring(nside, ipix)
  ip1 = ns.ang2pix_ring(nside, theta, phi)
  assert (ip1 == ipix).all()

  vec = ns.pix2vec_ring(nside, ipix)
  ip1 = ns.vec2pix_ring(nside, vec)
  assert (ip1 == ipix).all()

def testns(ns):
  print 'testing', ns.MAX_NSIDE
  nside = 1
  while nside <= ns.MAX_NSIDE:
    print 'nside = ', nside
    dpix = nside * nside / 123456 + 1
    suite1(ns, nside, dpix)
    suite2(ns, nside, dpix)

    nside1 = nside + 5
    if nside1 <= ns.MAX_NSIDE:
      print 'nside = ', nside1
      dpix = nside1 * nside1 / 123456 + 1
      suite2(ns, nside1, dpix)
    nside = nside * 2
