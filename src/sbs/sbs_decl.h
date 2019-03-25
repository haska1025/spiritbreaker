#ifndef _SBS_DECL_H_
#define _SBS_DECL_H_

// Declare the ph namespace
#define SBS_NAMESPACE_DECL_BEGIN namespace sbs{
#define SBS_NAMESPACE_DECL_END } \
     using namespace sbs;

#define SBS_DISALLOW_CONSTRUCT(class_name) class_name()
#define SBS_DISALLOW_COPY_CONSTRUCT(class_name) class_name(const class_name &)
#define SBS_DISALLOW_ASSIGNMENT(class_name) class_name & operator =(const class_name &)

#endif//_SBS_DECL_H_

