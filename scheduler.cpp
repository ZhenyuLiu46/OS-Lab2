//
//  scheduler.cpp
//  Lab2-working
//
//  Created by Zhenyu Liu on 3/14/19.
//  Copyright © 2019 Zhenyu Liu. All rights reserved.
//

#include "scheduler.h"
#include "process.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>


void FCFS_Scheduler:: addToQueue(Process *p) {
    p->state = STATE_READY;
    run_queue.push_back(p);
}

Process* FCFS_Scheduler:: getProcess(){
    if(run_queue.size() == 0) return NULL;
    Process* temp =  *run_queue.begin();
    run_queue.pop_front();
    return temp;
}

void LCFS_Scheduler:: addToQueue(Process *p) {
    p->state = STATE_READY;
    run_queue.push_front(p);
}

Process* LCFS_Scheduler:: getProcess(){
    if(run_queue.size() == 0) return NULL;
    Process* temp =  *run_queue.begin();
    run_queue.pop_front();
    return temp;
}

void SRTF_Scheduler::addToQueue(Process* process){
    process->state = STATE_READY;
    list<Process*>::iterator it;
    for (it=run_queue.begin(); it!=run_queue.end(); it++) {
        if ((*it)->remainingCB > process->remainingCB) {
            run_queue.insert(it, process);
            return;
        }
    }
    run_queue.insert(it, process);
}
Process* SRTF_Scheduler::getProcess(){
    if (run_queue.size()) {
        Process* temp = run_queue.front();
        run_queue.pop_front();
        return temp;
    }
    else return NULL;
}


void RR_Scheduler:: addToQueue(Process *p) {
    p->state = STATE_READY;
    run_queue.push_back(p);
}

Process* RR_Scheduler:: getProcess(){
    if(run_queue.size() == 0) return NULL;
    Process* temp =  *run_queue.begin();
    run_queue.pop_front();
    return temp;
}

void PRIO_Scheduler:: addToQueue(Process *p) {
    if(p->state == STATE_READY){ //dynamic priority minus one
        p->dynamic_prio -= 1;
    }
    else{
       p->dynamic_prio = p->PRIO - 1;
    }
    p->state = STATE_READY;
    //if priority runs out, push to expired queue
    if(p->dynamic_prio < 0){
        p->dynamic_prio = p->PRIO - 1;
        exp_queue.push_back(p);
    } else{
        run_queue.push_back(p);
    }
   
}




Process* PRIO_Scheduler:: getProcess(){
    switched = 0;
    if(run_queue.size() == 0){
        list<Process*> temp = run_queue;
        run_queue = exp_queue;
        exp_queue = temp;
        switched = 1;
    }
    int max = -1;
    int tempPrio =0 ;
    Process* temp = nullptr;
    for(Process* p : run_queue){
        tempPrio = p->dynamic_prio;
        if(tempPrio > max){
            temp = p;
            max = tempPrio;
        }
    }
 
    run_queue.remove(temp);
    
    return temp;
}


