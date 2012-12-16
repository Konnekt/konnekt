#ifndef MSC_COMPAT_H
#define MSC_COMPAT_H

#ifdef _MSC_VER
  #define _argv __argv
  #define _argc __argc

  #define _RTLENTRY  __cdecl
  #define _USERENTRY __cdecl
#endif

#endif