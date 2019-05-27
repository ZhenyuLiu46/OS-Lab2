//
//  eventQueue.cpp
//  Lab2-working
//
//  Created by Zhenyu Liu on 3/18/19.
//  Copyright © 2019 Zhenyu Liu. All rights reserved.
//

#include <iostream>
#include "eventQueue.h"

void EventQueue::put_event(Event *e) {
    int flag = 0;
    list<Event*>::iterator it;
    for(it = eventQue.begin(); it != eventQue.end(); it++){
        if((*it)->timestamp > e->timestamp){
            eventQue.insert(it, e);
            flag = 1;
            break;
        }
    }
    if(flag == 0) eventQue.insert(it, e);
}

//check if empty, and pop_front
Event* EventQueue::get_event() {
    if(eventQue.size() == 0) return NULL;
    Event *temp = *eventQue.begin();
    eventQue.pop_front();
    return temp;
}

int EventQueue::getNextEventTime() {
    if(eventQue.size() == 0) return -1;
    return (*eventQue.begin())->timestamp;
}
