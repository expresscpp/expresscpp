#pragma once

#ifdef EXPRESSCPP_STATIC_DEFINE
    /* We want a static library */
# define EXPRESSCPP_API

#else
    /* We want a shared library */
# ifdef _MSC_VER
#   define EXPRESSCPP_LIBRARY_IMPORT __declspec(dllimport)
#   define EXPRESSCPP_LIBRARY_EXPORT __declspec(dllexport)
# else
#   define EXPRESSCPP_LIBRARY_IMPORT
#   define EXPRESSCPP_LIBRARY_EXPORT __attribute__((visibility("default")))
# endif

#endif


#ifndef EXPRESSCPP_API
#  ifdef EXPRESSCPP_EXPORTS
      /* We are building this library */
#    define EXPRESSCPP_API EXPRESSCPP_LIBRARY_EXPORT
#  else
      /* We are using this library */
#    define EXPRESSCPP_API EXPRESSCPP_LIBRARY_IMPORT
#  endif
#endif
