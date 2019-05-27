//
//  main.cpp
//  Lab2-working
//
//  Created by Zhenyu Liu on 3/13/19.
//  Copyright © 2019 Zhenyu Liu. All rights reserved.
//

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <getopt.h>

#include <cstdio>


#include "process.h"
#include "event.h"
#include "eventQueue.h"
#include "scheduler.h"

using namespace std;

void verboseOutput(Process* proc, int timeInPrevState,  int curTime, string oldstate, string newstate){
    cout << curTime << " " << proc->PID << " " << timeInPrevState << ": " << oldstate << " -> " << newstate <<endl;
    
}

void preempOutPut(Process* curProc, Process* proc, int evntTimeS, int curTime, bool verbose, bool ans){
    if (verbose) {
        if (ans==false){
            if (curProc->dynamic_prio >= proc->dynamic_prio) {
                printf("---> PRIO preemption %d by %d ? %d TS=%d now=%d) --> %s\n", curProc->PID, proc->PID, 0, evntTimeS, curTime, "NO");
            }
            else{
                printf("---> PRIO preemption %d by %d ? %d TS=%d now=%d) --> %s\n", curProc->PID, proc->PID, 1, evntTimeS, curTime, "NO");
            }
        }
        else {
            printf("---> PRIO preemption %d by %d ? %d TS=%d now=%d) --> %s\n",curProc->PID, proc->PID, 1, evntTimeS, curTime, "YES");
        }
    }
}





int main(int argc, char * argv[]) {
    //Scheduler* scheduler = new FCFS_Scheduler("FCFS"); //work
    //Scheduler* scheduler = new LCFS_Scheduler("LCFS"); //work
    //Scheduler* scheduler = new SRTF_Scheduler("SRTF"); //work
    //Scheduler* scheduler = new RR_Scheduler("RR", 2); //work
    //Scheduler* scheduler = new RR_Scheduler("RR", 5); //work
    //Scheduler* scheduler = new PRIO_Scheduler("PRIO", 2); //work
    //Scheduler* scheduler = new PRIO_Scheduler("PRIO", 5, 3); //work
    //Scheduler* scheduler = new PRIO_Scheduler("PREPRIO", 2, 5); //work
    //Scheduler* scheduler = new PRIO_Scheduler("PREPRIO", 4); //work
    int c = 0;
    bool verbose = false;
    int prioIniSet = 4;
    Scheduler* scheduler;
    while (( c = getopt( argc, argv, "vs:")) != -1) {
        switch (c) {
            case 'v':
            verbose = true;
            break;
            
            case 's':
            string schedType = optarg;
            string::size_type i;
            for(i = 0; i < schedType.size(); ++i) {
                if (schedType[i]==':') {
                    break;
                }
            }
            if (i != schedType.size()) {
                prioIniSet = atoi(schedType.substr(i+1, string::npos).c_str());
                schedType = schedType.substr(0, i);
            }
            
            string quantStr =schedType.substr(1, string::npos);
            int quant = atoi(quantStr.c_str());
            
            switch (schedType[0]) {
                case 'F':
                scheduler  = new FCFS_Scheduler("FCFS");
                break;
                
                case 'L':
                scheduler  = new LCFS_Scheduler("LCFS");
                break;
                
                case 'S':
                scheduler = new SRTF_Scheduler("SRTF");
                break;
                
                case 'R':
                scheduler = new RR_Scheduler("RR", quant);
                break;
                
                case 'P':
                scheduler  = new PRIO_Scheduler("PRIO", quant, prioIniSet);
                break;
                
                case 'E':
                scheduler  = new PRIO_Scheduler("PREPRIO", quant, prioIniSet);
                break;
            }
            break;
        }
    }

    
    int idx = optind;
    string fileName = argv[idx];
    string randomfileName2 = argv[idx+1];
    
    int randomFileLength;
    int ofs = 0; //for generate random number
    vector<Process*> processes;
    vector<int> randomsNums; //save random nums
    
    ifstream inputFile;
    //inputFile.open("/Users/zhenyuliu/Desktop/OS/LAB2_STUFF/lab2_assign/input6");
    inputFile.open(fileName);
    ifstream randomFile;
    //randomFile.open("/Users/zhenyuliu/Desktop/OS/LAB2_STUFF/lab2_assign/rfile");
    randomFile.open(randomfileName2);
    
    //EventQueue for DES layer
    EventQueue evQ = EventQueue();
    
    
    randomFile >> randomFileLength;
    for(int i=0; i<randomFileLength; i++) {
        int val;
        randomFile >> val;
        randomsNums.push_back(val);
    }
    
    if(inputFile.is_open()){
        string line;
        int PID = 0;
        while (getline(inputFile, line))
        {
            istringstream ss(line);
            int AT, TC, CB, IO;
            int PRIO;
            
            ss >> AT >> TC >> CB >> IO ;
            
            PRIO = randomsNums[ofs++]%scheduler->mPrio + 1;
            
            Process *p = new Process(PID, AT, TC, CB, IO, PRIO);
            if(scheduler->schedulerName == "PRIO" || scheduler->schedulerName == "PREPRIO") p->dynamic_prio = PRIO;
            processes.push_back(p);
            
            //put everything from the input file to eventQueue
            Event* event = new Event(AT, p, STATE_CREATED,STATE_READY, TRANS_TO_READY);
            evQ.put_event(event);
            PID++;
        }
    } else {
        cout << "file not exsit";
    }

// working on simulation
    
    
    Event* evt;
    Process* CURRENT_RUNNING_PROCESS = nullptr; // this is the currently running process
    double ioUtil = 0; //io utilization
    double cpuUtil = 0; //cpu utilization
    int lastFT = 0;
    int ioLast = 0;//the time of last io finish
    int curBLastTime = 0; // for RR problem
    int curTime;
    bool CALL_SCHEDULER = false;//if to call scheduler or not
    while((evt = evQ.get_event())){
        Process *proc = evt->process; // this is the process the event works on
        curTime = evt->timestamp;
        proc->timeInPrevState =  curTime - proc->state_ts;
        
        switch(evt->transition) { // which state to transition to
            case TRANS_TO_READY:
                // must come from BLOCKED or from PREEMPTION
                // must add to run queue
                // conditional on whether something is run**
            
                if (scheduler->schedulerName == "PREPRIO" && CURRENT_RUNNING_PROCESS != nullptr) {
                    if (proc->state == STATE_BLOCK) {
                         proc -> dynamic_prio = proc -> PRIO-1;
                        // verboseOutput(proc, proc -> timeInPrevState, curTime, "BLOCK", "READY");
                    }
                    if (proc->state == STATE_CREATED) {
                        proc -> dynamic_prio = proc -> PRIO-1;
                       // verboseOutput(proc, proc -> timeInPrevState, curTime, "CREATE", "READY");
                    }
                    
                    //find the CURRENT_RUNNING_PROCESS
                    list<Event*>::iterator evtIt;
                    int evntTimeS = 0;
                    for (evtIt = evQ.eventQue.begin(); evtIt != evQ.eventQue.end(); evtIt++) {
                        if ((*evtIt)->process->PID == CURRENT_RUNNING_PROCESS->PID) {
                            evntTimeS = (*evtIt)->timestamp;
                            break;
                        }
                    }
                    
                    //do not need to be preempted because of priority
                    if (proc->dynamic_prio <= CURRENT_RUNNING_PROCESS->dynamic_prio) {
                       // preempOutPut(CURRENT_RUNNING_PROCESS, proc, evntTimeS, curTime, verbose, false);
                        proc->state_ts = curTime;
                        scheduler->addToQueue(proc);
                        CALL_SCHEDULER = true;
                    }
                    
                    //preempt
                    else if (evntTimeS != curTime && evntTimeS>0){
                       // preempOutPut(CURRENT_RUNNING_PROCESS, proc, evntTimeS, curTime, verbose, true);
                        CURRENT_RUNNING_PROCESS->remainingCB = (*evtIt)->remainingCBRestore - (curTime - CURRENT_RUNNING_PROCESS->state_ts);
                        CURRENT_RUNNING_PROCESS->curCB = (*evtIt)->curCBRestore-(curTime - CURRENT_RUNNING_PROCESS->state_ts);
                        
                        evtIt = evQ.eventQue.erase(evtIt);
                        Event* newEvt = new Event(curTime, CURRENT_RUNNING_PROCESS, STATE_RUN, STATE_READY ,TRANS_TO_PREEMPT);
                        evQ.put_event(newEvt);
                        proc->state_ts = curTime;
                        scheduler->addToQueue(proc);
                        CALL_SCHEDULER = true;
                        CURRENT_RUNNING_PROCESS = nullptr;
                    }
                    
                    //not preempt due to time
                    else{
                    //    preempOutPut(CURRENT_RUNNING_PROCESS, proc, evntTimeS, curTime, verbose, false);
                        proc->state_ts = curTime;
                        scheduler->addToQueue(proc);
                        CALL_SCHEDULER = true;
                    }
                }
            //not PREPRIO schedulers
                else {
                    if (proc->state == STATE_CREATED) {
                        proc->state_ts = curTime;
                        proc->state = STATE_READY;
                      //  verboseOutput(proc, 0, curTime, "CREATED", "READY");
                        if(CURRENT_RUNNING_PROCESS != nullptr || curTime < curBLastTime){
                            CALL_SCHEDULER = false;
                        } else {
                            CALL_SCHEDULER = true;
                        }
                    }
                    else if (proc->state == STATE_BLOCK) {
                        proc->state_ts = curTime;
                        proc->state = STATE_READY;
                     //   verboseOutput(proc, proc -> timeInPrevState, curTime, "BLOCK", "READY");
                        
                        //whenever come from block reset dynamic_prio
                        proc -> dynamic_prio = proc -> PRIO;
                        if(CURRENT_RUNNING_PROCESS != nullptr || curTime < curBLastTime){
                            CALL_SCHEDULER = false;
                        } else {
                            CALL_SCHEDULER = true;
                        }
                        
                    }else{
                        proc->state_ts = curTime;
                        proc->state = STATE_READY;
                        //verboseOutput(proc, timeInPrevState, curTime, "BLOCK", "READY");
                      //  printf("%d %d %d: RUNNG -> READY cb=%d rem=%d prio=%d\n",curTime, proc->PID, proc->timeInPrevState, proc->curCB, proc->remainingCB, proc->dynamic_prio);
                      
                        CALL_SCHEDULER = true;
                    }
                    scheduler->addToQueue(proc);
                }
                break;
            
            case TRANS_TO_DONE:
                proc->state = STATE_DONE;
                proc->state_ts = curTime;
             //   verboseOutput(proc, proc->timeInPrevState,curTime, "", "DONE");
                CURRENT_RUNNING_PROCESS = nullptr;
                CALL_SCHEDULER = true;
                break;
            
            case TRANS_TO_RUN:
                // create event for either preemption or blocking
                proc->state = STATE_RUN;
                proc->state_ts = curTime;
                if(proc->curCB == 0){
                    
                    proc->curCB = 1 + (randomsNums[(ofs++)%randomFileLength] % proc->CB);
                    if(proc->curCB > proc->remainingCB) proc->curCB = proc->remainingCB;
                }
            
                proc->CW += proc->timeInPrevState;
            
               // printf("%d %d %d: READY -> RUNNG cb=%d rem=%d prio=%d\n",curTime, proc->PID, proc->timeInPrevState, proc->curCB, proc->remainingCB, proc->dynamic_prio);
            
            
                //trying to do the real run math here, put it to block/preempt after run;
            
                if(scheduler->quantum >= proc->curCB){
                    int newCurTime = curTime + proc->curCB;
                    int temp1 = proc->remainingCB;
                    int temp2 = proc->curCB;
                    proc->remainingCB -= proc->curCB;
                    proc->curCB = 0;
                    
                    //Done, finisehd;
                    if(proc->remainingCB <= 0){
                        proc->state = STATE_DONE;
                        curTime = newCurTime - ( proc->remainingCB - 0);
                        proc->FT = newCurTime;
                        proc->TT = newCurTime - proc->AT;
                        //put it to trans_done
                        Event* event = new Event(curTime, proc, STATE_RUN,STATE_DONE, TRANS_TO_DONE);
                        event->remainingCBRestore = temp1;
                        event->curCBRestore = temp2;
                        evQ.put_event(event);
                    } else {
                        Event* event = new Event(newCurTime, proc, STATE_RUN,STATE_BLOCK, TRANS_TO_BLOCK);
                        event->remainingCBRestore = temp1;
                        event->curCBRestore = temp2;
                        evQ.put_event(event);
                    }
                }

                else { // to deal with RR scheduler
                    
                    int realCB = min(scheduler->quantum, proc->curCB);
                    int newCurTime = curTime + realCB;
                    int temp1 = proc->remainingCB;
                    int temp2 = proc->curCB;
                    
                    proc->remainingCB -= realCB;
                    proc->curCB -= realCB;
      
                    //Done
                    if(proc->remainingCB <= 0){
                        proc->FT = newCurTime;
                        proc->TT = newCurTime - proc->AT;
                        Event* event = new Event(newCurTime, proc, STATE_RUN,STATE_DONE, TRANS_TO_DONE);
                        event->remainingCBRestore = temp1;
                        event->curCBRestore = temp2;
                        evQ.put_event(event);
                        
                    } else {
                        if(scheduler->schedulerName == "PREPRIO"){
                            Event* event = new Event(newCurTime, proc, STATE_RUN,STATE_READY, TRANS_TO_PREEMPT);
                            event->remainingCBRestore = temp1;
                            event->curCBRestore = temp2;
                            evQ.put_event(event);
                            curBLastTime = newCurTime;
                        } else {
                            Event* event = new Event(newCurTime, proc, STATE_RUN,STATE_READY, TRANS_TO_READY);
                            evQ.put_event(event);
                            curBLastTime = newCurTime;
                            CURRENT_RUNNING_PROCESS = nullptr;
                        }
                    }
                }
                break;
            
            case TRANS_TO_BLOCK:
                //create an event for when process becomes READY again CALL_SCHEDULER = true;
                proc->state_ts = curTime;
                proc->state = STATE_BLOCK;
                proc->curIB = 1 + (randomsNums[(ofs++)%randomFileLength] % proc->IO);
              //  printf("%d %d %d: RUNNG -> BLOCK ib=%d rem=%d prio=%d\n",curTime, proc->PID, proc->timeInPrevState, proc->curIB, proc->remainingCB, proc->dynamic_prio);
            
                //perform block
                {
                    int newCurTime = curTime + proc->curIB;
                    proc->IT += proc->curIB;
                    Event* event = new Event(newCurTime, proc, STATE_BLOCK, STATE_READY, TRANS_TO_READY);
                    evQ.put_event(event);
                }
            
                //TO compute ioUtil
                if (curTime > ioLast) {
                    ioUtil += proc->curIB;
                    ioLast = curTime + proc->curIB;
                }
                else if (curTime + proc->curIB > ioLast) {
                    ioUtil = ioUtil +curTime + proc->curIB - ioLast;
                    ioLast = curTime + proc->curIB;
                }
            
                CURRENT_RUNNING_PROCESS = nullptr;
                CALL_SCHEDULER = true;
                break;
            
            case TRANS_TO_PREEMPT:
            
                proc->state = STATE_READY;
                proc->state_ts = curTime;

             //   printf("%d %d %d: RUNNG -> READYY  cb=%d rem=%d prio=%d\n",curTime, proc->PID, proc->timeInPrevState, proc->curCB, proc->remainingCB, proc->dynamic_prio);
                scheduler->addToQueue(proc);
            
                CALL_SCHEDULER = true;
                CURRENT_RUNNING_PROCESS = NULL;
                break;
        }
        
        //remove current event object from Memory
        delete evt;
        evt = NULL;
        
        if(CALL_SCHEDULER) {
            //cout << "thirthrithri" << endl;
            CALL_SCHEDULER = false; // reset global flag
            if (evQ.getNextEventTime() == curTime) {
                continue;//process next event from Event queue
            }
            
            if (CURRENT_RUNNING_PROCESS == nullptr) {
                
                CURRENT_RUNNING_PROCESS = scheduler->getProcess();
                if (CURRENT_RUNNING_PROCESS == nullptr){
                    continue;
                }
                
                // create event to make this process runnable for same time.
                evt = new Event(curTime, CURRENT_RUNNING_PROCESS, STATE_READY, STATE_RUN, TRANS_TO_RUN);
                evQ.put_event(evt);
            }
        }
    }
    
    //for summary output
    int count = 0;
    int sumTT = 0;
    int sumCW = 0;
    double avgTT = 0;
    double avgCW = 0;
    double throughput = 0;
    //to print header;
    if(scheduler->schedulerName == "RR" || scheduler->schedulerName == "PRIO" || scheduler->schedulerName == "PREPRIO"){
        cout << scheduler->schedulerName <<" "<< scheduler->quantum<<endl;
    } else{
        cout << scheduler->schedulerName << endl;
    }
    
    for(Process* p : processes){
        printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
               p->PID, p->AT, p->TC, p->CB, p->IO, p->PRIO, p->FT, p->TT, p->IT, p->CW);
        lastFT = max(lastFT,p->FT);
        cpuUtil += p->TC;
        count++;
        sumTT += p->TT;
        sumCW += p->CW;
    }
    cpuUtil = cpuUtil * 100 / lastFT;
    ioUtil = ioUtil * 100 / lastFT;
    avgTT = double (sumTT ) / count;
    avgCW = double (sumCW) / count;
    throughput = double (count * 100) / lastFT;
    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n", lastFT, cpuUtil, ioUtil, avgTT, avgCW, throughput);
    inputFile.close();
    randomFile.close();
    return 0;
    
}
