//
//  event.cpp
//  Lab2-working
//
//  Created by Zhenyu Liu on 3/14/19.
//  Copyright © 2019 Zhenyu Liu. All rights reserved.
//

#include <iostream>
#include "event.h"

Event :: Event(int time, Process *p, processStateT os, processStateT ns, stateTransitionT trans) {
            timestamp = time;
            process = p;
            old_state = os;
            new_state = ns;
            transition = trans;
            curCBRestore = p-> curCB;
            remainingCBRestore = p-> remainingCB;
        }
