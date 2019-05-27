//
//  event.h
//  Lab2-working
//
//  Created by Zhenyu Liu on 3/18/19.
//  Copyright © 2019 Zhenyu Liu. All rights reserved.
//

#ifndef event_h
#define event_h

#include <iostream>
#include "process.h"
using namespace std;

class Event {
    public:
    int timestamp;
    Process* process;
    processStateT old_state;
    processStateT new_state;
    stateTransitionT transition;
    int remainingCBRestore;
    int curCBRestore;
    Event(int time, Process *p, processStateT os, processStateT ns, stateTransitionT trans);
    
};



#endif /* event_h */
