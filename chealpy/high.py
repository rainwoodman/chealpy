from . import _ccode 
locals().update(
  [ (name[:-2], _ccode.__dict__[name]) for name in _ccode.__dict__ if '64' in name])
MAX_NSIDE = 1<<29
from . _ccode import ang2vec, vec2ang
