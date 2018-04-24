from . import _ccode 
locals().update(
  [ (name, _ccode.__dict__[name]) for name in _ccode.__dict__ if '64' not in name])
MAX_NSIDE = 1<<13

