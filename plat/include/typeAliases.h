#ifndef TYPE_ALIASES_H
#define TYPE_ALIASES_H

#include <cstdint>

template<typename T>
struct t_prefix {
  using T_  = const T;
  using t_  = T;
  using p_  = T*;
  using P_  = const T*;
  using r_  = T&;
  using R_  = const T&;
  using x_  = T&&;
  using X_  = const T&&;
    
  t_prefix() = delete;
};

template<>
struct t_prefix<void> {
  using t_  = void;
  using p_  = void*;
  using P_  = const void*;
    
  t_prefix() = delete;
};

template<typename T> struct t_prefix<T*>;
template<typename T> struct t_prefix<T&>;
template<typename T> struct t_prefix<const T>;

/////////////////////////////////////////////////

using t_int          = t_prefix<int>::t_;
using T_int          = t_prefix<int>::T_;
using r_int          = t_prefix<t_int>::r_;
using p_int          = t_prefix<t_int>::p_;
using P_int          = t_prefix<t_int>::P_;

using t_bool         = t_prefix<bool>::t_;
using T_bool         = t_prefix<t_bool>::T_;
using r_bool         = t_prefix<t_bool>::r_;
using p_bool         = t_prefix<t_bool>::p_;
using P_bool         = t_prefix<t_bool>::P_;

using t_char         = t_prefix<char>::t_;
using T_char         = t_prefix<t_char>::T_;
using r_char         = t_prefix<t_char>::r_;
using p_char         = t_prefix<t_char>::p_;
using P_char         = t_prefix<t_char>::P_;

using t_void         = t_prefix<void>::t_;
using p_void         = t_prefix<t_void>::p_;
using P_void         = t_prefix<t_void>::P_;

#endif /* TYPE_ALIASES_H */
