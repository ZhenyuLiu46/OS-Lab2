//
//  process.h
//  Lab2-working
//
//  Created by Zhenyu Liu on 3/14/19.
//  Copyright © 2019 Zhenyu Liu. All rights reserved.
//

#ifndef process_h
#define process_h
#include <iostream>


typedef enum { STATE_CREATED, STATE_READY, STATE_RUN, STATE_BLOCK, STATE_DONE} processStateT;
typedef enum { TRANS_TO_READY, TRANS_TO_RUN, TRANS_TO_BLOCK, TRANS_TO_PREEMPT, TRANS_TO_DONE } stateTransitionT;


class Process {
    public:
    Process(int pid, int at, int tc, int cb, int io, int prio);
    
    
    int PID; // process ID
    
    int remainingCB; // remainingCB: remaining CPU burst
    int curCB; //get a random num;
    int curIB; //get a random num;
    int state_ts; //timestamp of the enter of current state
    int timeInPrevState;

    int AT, TC, CB, IO, PRIO, FT, TT, IT, CW;
    
    int dynamic_prio;
    processStateT state;
    
};

#endif /* process_h */
