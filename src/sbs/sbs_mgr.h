#ifndef _SBS_MGR_H_
#define _SBS_MGR_H_

#include "sbs_decl.h"

SBS_NAMESPACE_DECL_BEGIN

class SBS_DLL_DECL SBSMgr
{
public:
    static int Initialize();
    static int Close();

private:
    SBS_DISALLOW_CONSTRUCT(SBSMgr);
    SBS_DISALLOW_COPY_CONSTRUCT(SBSMgr);
    SBS_DISALLOW_ASSIGNMENT(SBSMgr);
};

SBS_NAMESPACE_DECL_END
#endif//_SBS_MGR_H_

