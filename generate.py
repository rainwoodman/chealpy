""" 
  generates the binding in chealpy/_ccode.pyx

  run with python generate.py > chealpy/_ccode.pyx
  remember to execute cython _ccode.pyx  afterwards!
   
"""

import argparse
ap = argparse.ArgumentParser()
ap.add_argument('precision', type=int, choices=[32, 64])
ns = ap.parse_args()

if ns.precision == 64:
    rlz0 = [ ('64', 64) ]
    rlz1 = [('', 32)]
    max_nside = "MAX_NSIDE = 1<<29"
elif ns.precision == 32:
    rlz0 =  [ ('', 32) ]
    rlz1 = [('', 32)]
    max_nside = "MAX_NSIDE = 1<<13"

# realizations, (name, (input), (output), (return))
funcs = \
  [(rlz, ('ang2pix_ring', ('nside', 'theta', 'phi'), ('ipix',))) for rlz in rlz0] +\
  [(rlz, ('ang2pix_nest', ('nside', 'theta', 'phi'), ('ipix',))) for rlz in rlz0] +\
  [(rlz, ('pix2ang_ring', ('nside', 'ipix'), ('theta', 'phi')) ) for rlz in rlz0] +\
  [(rlz, ('pix2ang_nest', ('nside', 'ipix'), ('theta', 'phi')) ) for rlz in rlz0] +\
  [(rlz, ('vec2pix_ring', ('nside', 'vec'), ('ipix',))         ) for rlz in rlz0] +\
  [(rlz, ('vec2pix_nest', ('nside', 'vec'), ('ipix',))         ) for rlz in rlz0] +\
  [(rlz, ('pix2vec_ring', ('nside', 'ipix'), ('vec',))         ) for rlz in rlz0] +\
  [(rlz, ('pix2vec_nest', ('nside', 'ipix'), ('vec',))         ) for rlz in rlz0] +\
  [(rlz, ('nest2ring', ('nside', 'ipnest'), ('ipring',))  ) for rlz in rlz0] +\
  [(rlz, ('ring2nest', ('nside', 'ipring'), ('ipnest',))  ) for rlz in rlz0] +\
  [(rlz, ('npix2nside', ('npix',), (), 'nside')           ) for rlz in rlz0] +\
  [(rlz, ('nside2npix', ('nside',), (), 'npix')           ) for rlz in rlz0] +\
  [(rlz, ('ang2vec', ('theta', 'phi'), ('vec', ))         ) for rlz in rlz1] +\
  [(rlz, ('vec2ang', ('vec', ), ('theta', 'phi'))         ) for rlz in rlz1] +\
  []
docstrings = {
 'ang2pix': 'Converting theta(0 to pi), phi(0 to 2 pi) to pix number',
 'pix2ang': 'Converting pix number to theta(0 to pi), phi(0 to 2 pi)',
 'vec2pix': 'Converting 3 vectors (..., 3) to pix number',
 'pix2vec': 'Converting pix number to 3 vectors (..., 3)',
}

# ctype, numpy.dtype
int_type = {32: ('long', 'int'), 64: ('int64_t', 'i8')}
float_type = {32: ('double', 'f8'), 64: ('double', 'f8')}
vec_type = {32: ('double *', ('f8', 3)), 64: ('double *', ('f8', 3))}
void_type = {32: ('void', ''), 64: ('void', '')}

# populate type information
var_types = {}
var_types.update([(var, int_type) for var in \
          ['npix', 'ipix', 'nside', 'ipring', 'ipnest']])
var_types.update([(var, float_type) for var in \
          ['theta', 'phi']])
var_types.update([(var, vec_type) for var in \
          ['vec']])
var_types.update([(var, void_type) for var in \
          [None]])

def isarray(var, wordsize):
  return isinstance(var_types[var][wordsize][1], tuple)

def args_proto(input, output, wordsize):
  ret = []
  for var in input:
    ctype = var_types[var][wordsize][0]
    ret += ['%s _%s' % (ctype, var)]

  for var in output:
    ctype = var_types[var][wordsize][0]
    if not isarray(var, wordsize):
      ctype = ctype + ' *'
    ret += ['%s _%s' % (ctype, var)]
  return ret

def args_call(input, output, wordsize):
  ret = []
  for var in input:
    ret += ['_%s' % var]

  for var in output:
    if not isarray(var, wordsize):
      ret += ['&_%s' % var]
    else:
      ret += ['_%s' % var]
  return ret

def arg_dtype(var, wordsize):
  return var_types[var][wordsize][1]
def arg_ctype(var, wordsize):
  return var_types[var][wordsize][0]
 
def args_nditer(input, output, wordsize):
  def arg_nditer(var, wordsize):
    if not isarray(var, wordsize):
      return [(var_types[var][wordsize][1], var)]
    else:
      base, d = var_types[var][wordsize][1]
      return [(base, '%s[...,%d]' %(var, i)) for i in range(d)]

  ops = []
  op_dtypes = []
  op_flags = []
  vars = {}

  i = 0
  for var in input:
    for dtype, name in arg_nditer(var, wordsize):
      ops += [name]
      op_dtypes += [repr(dtype)]
      op_flags += [repr(["readonly"])]
      if var in vars:
        vars[var] += [i]
      else:
        vars[var] = [i]
      i = i + 1
  for var in output:
    for dtype, name in arg_nditer(var, wordsize):
      ops += [name]
      op_dtypes += [repr(dtype)]
      op_flags += [repr(["writeonly"])]
      if var in vars:
        vars[var] += [i]
      else:
        vars[var] = [i]
      i = i + 1
  return ops, op_dtypes, op_flags, vars

def parsefunc(func):
  ret = None
  if len(func) == 3:
    name, input, output = func
  else:
    name, input, output, ret = func
  return ret, name, input, output

def generate_prototype(rlz, func):
  postfix, wordsize = rlz
  ret, name, input, output = parsefunc(func)
  template = \
  """%(ret_type)s _%(name)s%(postfix)s "%(name)s%(postfix)s" (%(args)s) nogil"""
  return template % { 
     'ret_type': arg_ctype(ret, wordsize),
     'name': name,
     'postfix': postfix,
     'args': ', '.join(args_proto(input, output, wordsize)),
     }
def generate_prototypes(funcs):
  return \
  """cdef extern from "chealpix.h": 
  """ + \
  '\n  '.join([generate_prototype(*func) for func in funcs])

def generate_ufunc(rlz, func):
  postfix, wordsize = rlz
  ret, name, input, output = parsefunc(func)
  if ret is not None:
    output_ret = list(output) + [ret]
  else:
    output_ret = output
  if name in docstrings:
    docstring = docstrings[name]
  else:
    docstring = name

  ops, op_dtypes, op_flags, vars = args_nditer(input, output_ret, wordsize)
  iterstatement = """
  iter = numpy.nditer([%(ops)s],
       op_dtypes=[%(op_dtypes)s],
       op_flags=[%(op_flags)s],
       flags = ['buffered', 'external_loop', 'zerosize_ok'],
       casting = 'unsafe')
  """ % {
     'ops': ','.join(ops),
     'op_dtypes': ','.join(op_dtypes),
     'op_flags': ','.join(op_flags)
     }

  defstatement = """def %(name)s (%(argin)s, %(argout)s):""" \
    % {
      'name' : name,
      'argin' : ','.join(input),
      'argout' : ','.join(['%s = None' % var for var in output_ret])
  }

  returnstatement = """  return %(argout)s""" \
    % {
      'argout' : ','.join(output_ret)
    }

  shapestatement = """  shape = numpy.broadcast(1, %(argin)s).shape """\
   % {
     'argin': ','.join(args_nditer(input, [], wordsize)[0])
    }

  nonestatements = []
  for var in output_ret:
    nonestatements += [
      """  if %(var)s is None: %(var)s = numpy.empty(shape, dtype=%(dtype)s)""" \
    % {
     'var': var,
     'dtype': repr(arg_dtype(var, wordsize))
    }]
  nonestatements = '\n'.join(nonestatements)
  tmpstatements = []
  for var in vars:
    if isarray(var, wordsize):
      tmpstatements += [
  """  cdef numpy.ndarray %(var)s_a = numpy.empty(0, dtype=%(dtype)s)
  cdef %(ctype)s _%(var)s = <%(ctype)s>%(var)s_a.data""" \
    % {
      'dtype': repr(arg_dtype(var, wordsize)),
      'ctype': arg_ctype(var, wordsize),
      'var': var}
  ]
    else:
      tmpstatements += [
  """  cdef %(ctype)s _%(var)s""" \
    % {
      'dtype': repr(arg_dtype(var, wordsize)),
      'ctype': arg_ctype(var, wordsize),
      'var': var}
  ]

  tmpstatements = '\n'.join(tmpstatements)

  packstatements = []
  for var in input:
    if isarray(var, wordsize):
      packstatements += [
        """        _%(var)s[%(ind1)d] = (<%(ctype)s> citer.data[%(ind2)d])[0] """ \
        % {
        'dtype': repr(arg_dtype(var, wordsize)),
        'ctype': arg_ctype(var, wordsize),
        'var': var,
        'ind1': i,
        'ind2': d, } \
        for i, d in enumerate(vars[var])]
    else:
      packstatements += [
        """        _%(var)s = (<%(ctype)s * > citer.data[%(ind2)d])[0] """ \
        % {
        'dtype': repr(arg_dtype(var, wordsize)),
        'ctype': arg_ctype(var, wordsize),
        'var': var,
        'ind2': vars[var][0], }
        ]
  packstatements = '\n'.join(packstatements)   
  
  unpackstatements = []
  for var in output_ret:
    if isarray(var, wordsize):
      unpackstatements += [
        """        (<%(ctype)s> citer.data[%(ind2)d])[0] = _%(var)s[%(ind1)d] """ \
        % {
        'dtype': repr(arg_dtype(var, wordsize)),
        'ctype': arg_ctype(var, wordsize),
        'var': var,
        'ind1': i,
        'ind2': d, } \
        for i, d in enumerate(vars[var])]
    else:
      unpackstatements += [
        """        (<%(ctype)s * > citer.data[%(ind2)d])[0] = _%(var)s """ \
        % {
        'dtype': repr(arg_dtype(var, wordsize)),
        'ctype': arg_ctype(var, wordsize),
        'var': var,
        'ind2': vars[var][0], }
        ]
  unpackstatements = '\n'.join(unpackstatements)   

  if ret is None:
    callstatement = \
     """        _%(name)s%(postfix)s ( %(args)s) """ % \
     { 'name': name,
       'postfix' : postfix,
       'args': ', '.join(args_call(input, output, wordsize)),

     }
  else:
    callstatement = \
     """        _%(ret)s = _%(name)s%(postfix)s ( %(args)s) """ % \
     { 'name': name,
       'postfix' : postfix,
       'args': ', '.join(args_call(input, output, wordsize)),
       'ret' : ret
     }
  return """
%(defstatement)s
  "%(docstring)s"
%(shapestatement)s
%(nonestatements)s
%(iterstatement)s
  cdef npyiter.CIter citer
  cdef size_t size = npyiter.init(&citer, iter)
%(tmpstatements)s
  with nogil:
    while size >0:
      while size > 0:
%(packstatements)s
%(callstatement)s
%(unpackstatements)s
        npyiter.advance(&citer)
        size = size -1
      size = npyiter.next(&citer) 
%(returnstatement)s
""" % locals()

FILE = """
# do not edit. This is auto-generated
#cython: embedsignature=True
#cython: cdivision=True
cimport numpy
cimport npyiter
from libc.stdint cimport *
import numpy
numpy.import_array()

%(max_nside)s

%(prototypes)s

%(ufuncs)s
""" 

import sys

sys.stdout.write(FILE % {
   'max_nside' : max_nside,
   'prototypes' : generate_prototypes(funcs),
   'ufuncs' : '\n'.join(generate_ufunc(*func) for func in funcs)
   })
