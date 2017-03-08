/*
 * Summary: macros for marking symbols as exportable/importable.
 * Description: macros for marking symbols as exportable/importable.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author:
 * Derived from libxml work by Igor Zlatovic <igor@zlatkovic.com>
 * Upskirt/Sundown specific edits by Ger Hobbelt <ger@hobbelt.com>
 */

#ifndef __UPSKIRT_EXPORTS_H__
#define __UPSKIRT_EXPORTS_H__

/**
 * SDPUBFUN, SDPUBVAR
 *
 * Macros which declare an exportable function and an exportable variable.
 *
 * Please use an extra block for every platform/compiler combination when
 * modifying this, rather than overlong #ifdef lines. This helps
 * readability as well as the fact that different compilers on the same
 * platform might need different definitions.
 */

/**
 * SDPUBFUN:
 *
 * Macros which declare an exportable function
 */
#define SDPUBFUN extern
/**
 * SDPUBVAR:
 *
 * Macros which declare an exportable variable
 */
#define SDPUBVAR extern

/** DOC_DISABLE */

/*
 * Notes:
 *
 * The default behaviour is to assume a static library is being built.
 * You MUST #define LIBUPSKIRT_DYNAMIC in your code/project/makefile to
 * receive the incantations for libupskirt as a DLL/SO module.
 */

/* Windows platform with MS compiler */
#if defined(_WIN32) && defined(_MSC_VER)
  #undef SDPUBFUN
  #undef SDPUBVAR
  #if defined(IN_LIBUPSKIRT) && defined(LIBUPSKIRT_DYNAMIC)
    #define SDPUBFUN __declspec(dllexport)
    #define SDPUBVAR __declspec(dllexport) extern
  #else
    #define SDPUBFUN
    #if defined(LIBUPSKIRT_DYNAMIC)
      #define SDPUBVAR __declspec(dllimport) extern
    #else
      #define SDPUBVAR extern
    #endif
  #endif
#endif

/* Windows platform with Borland compiler */
#if defined(_WIN32) && defined(__BORLANDC__)
  #undef SDPUBFUN
  #undef SDPUBVAR
  #if defined(IN_LIBUPSKIRT) && defined(LIBUPSKIRT_DYNAMIC)
    #define SDPUBFUN __declspec(dllexport)
    #define SDPUBVAR __declspec(dllexport) extern
  #else
    #define SDPUBFUN
    #if defined(LIBUPSKIRT_DYNAMIC)
      #define SDPUBVAR __declspec(dllimport) extern
    #else
      #define SDPUBVAR extern
    #endif
  #endif
#endif

/* Windows platform with GNU compiler (Mingw) */
#if defined(_WIN32) && defined(__MINGW32__)
  #undef SDPUBFUN
  #undef SDPUBVAR
  /*
   * if defined(IN_LIBUPSKIRT) this raises problems on mingw with msys
   * _imp__upskirtfree listed as missing. Try to workaround the problem
   * by also making that declaration when compiling client code.
   */
  #if defined(IN_LIBUPSKIRT) && defined(LIBUPSKIRT_DYNAMIC)
    #define SDPUBFUN __declspec(dllexport)
    #define SDPUBVAR __declspec(dllexport) extern
  #else
    #define SDPUBFUN
    #if defined(LIBUPSKIRT_DYNAMIC)
      #define SDPUBVAR __declspec(dllimport) extern
    #else
      #define SDPUBVAR extern
    #endif
  #endif
#endif

/* Cygwin platform, GNU compiler */
#if defined(_WIN32) && defined(__CYGWIN__)
  #undef SDPUBFUN
  #undef SDPUBVAR
  #if defined(IN_LIBUPSKIRT) && defined(LIBUPSKIRT_DYNAMIC)
    #define SDPUBFUN __declspec(dllexport)
    #define SDPUBVAR __declspec(dllexport) extern
  #else
    #define SDPUBFUN
    #if defined(LIBUPSKIRT_DYNAMIC)
      #define SDPUBVAR __declspec(dllimport) extern
    #else
      #define SDPUBVAR
    #endif
  #endif
#endif

#endif /* __UPSKIRT_EXPORTS_H__ */
