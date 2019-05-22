#ifndef _SBS_DECL_H_
#define _SBS_DECL_H_

// Declare the ph namespace
#define SBS_NAMESPACE_DECL_BEGIN namespace sbs{
#define SBS_NAMESPACE_DECL_END } \
     using namespace sbs;

#define SBS_DISALLOW_CONSTRUCT(class_name) class_name()
#define SBS_DISALLOW_COPY_CONSTRUCT(class_name) class_name(const class_name &)
#define SBS_DISALLOW_ASSIGNMENT(class_name) class_name & operator =(const class_name &)

/* dll export or import declaration */
#ifndef SBS_STATIC_BUILD
#ifndef SBS_DLL_DECL
#   if defined(WIN32) || defined(_WIN64)
#       ifndef sbs_EXPORTS
#           define SBS_DLL_DECL __declspec(dllimport)
#       else
#           define SBS_DLL_DECL __declspec(dllexport)
#       endif//SBS_EXPORTS
#   else
#       define SBS_DLL_DECL
#   endif//_WIN32
#endif//SBS_DLL_DECL
#else
#define SBS_DLL_DECL
#endif//SBS_STATIC_BUILD

#endif//_SBS_DECL_H_

