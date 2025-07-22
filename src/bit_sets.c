# include <bit_sets.h>

int bit_nextelement(const sets_t set1[static 1], size_t m, int pos)
{
  setwords_t setwd;
  int w;

  if ( m == 1 ) {
    if ( pos < 0 ) setwd = set1[0];
    else setwd = set1[0] & _BITMASK(pos);

    if ( setwd == 0 ) return -1;
    else return _FIRSTBITNZ(setwd);
  }

  if ( pos < 0 ) {
    w = 0;
    setwd = set1[0];
  }
  else {
    w = _SETWD(pos);
    setwd = set1[w] & _BITMASK(_SETBT(pos));
  }

  while ( 1 ) {
    if (setwd != 0) return _TIMESWORDSIZE(w) + _FIRSTBITNZ(setwd);
    if ((size_t)++w == m) return -1;
    setwd = set1[w];
  }
}

void bit_permset(sets_t s1[restrict static 1],
                 const sets_t s2[restrict static 1],
                 const uint32_t p[static 1], size_t m)
{
  setwords_t setw;
  uint32_t pos, b;
  if ( m == 1 ) {
    *s1 = 0;
    setw = s2[0];
    while ( setw != 0 ) {
      _TAKEBIT(b, setw);
      *s1 |= _BITT[p[b]];
    }
  }
  else {
    _EMPTYSET(s1, m);
    for ( size_t w = 0; w < m; w++ ) {
      setw = s2[w];
      while ( setw != 0 ) {
        _TAKEBIT(b, setw);
        pos = p[_TIMESWORDSIZE(w) + b];
        _ADDELEMENT(s1, pos);
      }
    }
  }
}

int bit_permaut(const sets_t s[static 1], const uint32_t p[static 1], size_t m)
{
  setwords_t setw;
  uint32_t pos, b;
  if ( m == 1 ) {
    setw = s[0];
    while ( setw != 0 ) {
      _TAKEBIT(b, setw);
      pos = p[b];
      if ( !_ISELEMENT(s, pos) ) return -1;
    }
  }
  else {
    for ( size_t w = 0; w < m; w++ ) {
      setw = s[w];
      while ( setw != 0 ) {
        _TAKEBIT(b, setw);
        pos = p[_TIMESWORDSIZE(w) + b];
        if ( !_ISELEMENT(s, pos) ) return -1;
      }
    }
  }
  return 1;
}
