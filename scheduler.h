//
//  scheduler.h
//  Lab2-working
//
//  Created by Zhenyu Liu on 3/14/19.
//  Copyright © 2019 Zhenyu Liu. All rights reserved.
//

#ifndef scheduler_h
#define scheduler_h

#include "process.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <list>

using namespace std;
class Scheduler {
    public:
    string schedulerName;
    int quantum;
    int mPrio;
    int switched = 0;

    Scheduler(string name, int quan=10000 , int maxPrio = 4) { schedulerName = name; quantum=quan; mPrio = maxPrio; };
    // return the the first process from the scheduler
    virtual Process* getProcess() = 0;
    // put an event into the ready queue
    virtual void addToQueue(Process *p) = 0;
};

class FCFS_Scheduler: public Scheduler {
    public:
    list<Process*> run_queue;
    FCFS_Scheduler(string name, int quan=10000 , int maxPrio = 4) : Scheduler(name, quan, maxPrio) {  };
    Process* getProcess();
    void addToQueue(Process *p);
};

class LCFS_Scheduler: public Scheduler {
    public:
    LCFS_Scheduler(string type, int quant = 10000 , int maxPrio = 4) : Scheduler(type, quant, maxPrio) {}
    list<Process*> run_queue;
    void addToQueue(Process*);
    Process* getProcess();
   
};

class SRTF_Scheduler: public Scheduler {
    public:
    SRTF_Scheduler(string type, int quant = 10000 , int maxPrio = 4) : Scheduler(type, quant, maxPrio) {}
    list<Process*> run_queue;
    void addToQueue(Process*);
    Process* getProcess();
    
};

class RR_Scheduler: public Scheduler {
    public:
    list<Process*> run_queue;
    RR_Scheduler(string name, int quan=10000 , int maxPrio = 4) : Scheduler(name, quan, maxPrio) {  };
    Process* getProcess();
    void addToQueue(Process *p);
};

class PRIO_Scheduler: public Scheduler {
    public:
    list<Process*> run_queue;
    list<Process*> exp_queue;
    PRIO_Scheduler(string name, int quan=10000, int maxPrio = 4) : Scheduler(name, quan, maxPrio) {  };
    
    std::string getName();
    Process* getProcess();
    void addToQueue(Process *p);
};


#endif /* scheduler_h */
