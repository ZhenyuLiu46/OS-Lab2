//
//  process.cpp
//  Lab2-working
//
//  Created by Zhenyu Liu on 3/14/19.
//  Copyright © 2019 Zhenyu Liu. All rights reserved.
//

#include <iostream>
#include "process.h"



Process::Process(int pid, int at, int tc, int cb, int io, int prio){
    PID = pid;
    AT = at;
    TC = tc;
    CB = cb;
    IO = io;
    PRIO = prio;
    state = STATE_CREATED;
    
    state_ts = 0;
    curCB = 0;
    curIB = 0;
    IT = 0;
    CW = 0;
    timeInPrevState = 0;
    remainingCB = tc;
    dynamic_prio = PRIO-1;
}
