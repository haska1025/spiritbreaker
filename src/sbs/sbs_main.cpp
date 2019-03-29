#include <stdio.h>
#include "sbs_mgr.h"

int main(int argc, char *argv[])
{
    int rc = 0;
    rtc::Thread *pthrMain = rtc::Thread::Current();

    rc = SBSMgr::Instance()->Initialize();
    if (rc != 0){
        printf("Initialize sbs mgr failed!\n");
    }

    pthrMain->Run();
    return 0;
}

