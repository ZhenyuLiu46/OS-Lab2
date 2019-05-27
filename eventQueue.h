//
//  eventQueue.h
//  Lab2-working
//
//  Created by Zhenyu Liu on 3/18/19.
//  Copyright © 2019 Zhenyu Liu. All rights reserved.
//

#ifndef eventQueue_h
#define eventQueue_h
#include "event.h"
#include <iostream>
#include <list>

using namespace std;
class EventQueue {
    public:
    EventQueue() = default;
    list<Event*> eventQue;
    Event* get_event();
    void put_event(Event*);
    int getNextEventTime();
};

#endif /* eventQueue_h */
