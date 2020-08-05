#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int timeElapsed = 0;
int quantum;
int roundToServe = 0; 
int rRComplete = 0;
int nPPComplete = 0;
int sJFComplete = 0;
int processesCompleted = 0;

class Process {
    private:
        int pid;
        int arrival;
        int burst;
        int remaining;
        int exitTime;
        int firstContact;
        bool contact;
        bool complete;
        int queue;
        int priority;
    public:
        Process() {
            pid = 0;
            arrival = 0;
            burst = 0;
            remaining = 0;
            exitTime = 0;
            firstContact = 0;
            queue = 0;
            priority = 0;
            contact = false;
            complete = false;
        }

        Process (const Process & process) {
            pid = process.pid;
            arrival = process.arrival;
            burst = process.burst;
            remaining = process.remaining;
            complete = process.complete;
            exitTime = process.exitTime;
            firstContact = process.firstContact;
            contact = process.contact;
            queue = process.queue;
            priority = process.priority;
        }

        void setPid(int pid) {
            this->pid = pid;
        }

        void setArrival(int arrival) {
            this->arrival = arrival;
        }

        void setBurst(int burst) {
            this->burst = burst;
        }

        void setRemaining(int remaining) {
            this->remaining = remaining;
        }

        void setComplete(bool complete) {
            this->complete = complete;
        }

        void setExitTime(int exitTime) {
            this->exitTime = exitTime;
        }

        void setFirstContact(int firstContact) {
            this->firstContact = firstContact;
        }

        void setContact(bool contact) {
            this->contact = contact;
        }

        void setqueue(int queue) {
            this->queue = queue;
        }

        void setPriority(int priority){
            this->priority = priority; 
        }

        int getPid() {
            return pid;
        }

        int getArrival() {
            return arrival;
        }

        int getBurst() {
            return burst;
        }

        int getRemaining() {
            return remaining;
        }

        bool getComplete() {
            return complete;
        }
 
        int getExitTime() {
            return exitTime;
        }

        int getFirstContact() {
            return firstContact;
        }

        bool getContact() {
            return contact;
        }

        int getqueue() {
            return queue;
        }

        int getPriority(){
            return priority;
        }
};

void queueSortArrival(Process *List, int count) { //implemented Selection Sort
    int i, j, minIndex;
    Process *temp = new Process();
    for (i = 0; i < count-1; i++){
        minIndex = i;
        for (j = i+1; j < count; j++){
            if ((List + j)->getArrival() < (List + minIndex)->getArrival()) {
                minIndex = j;
                *temp = *(List + j);
                *(List + j) = *(List + i);
                *(List + i) = *temp;
            }
        }
    }
}

void queueAssignment(Process *List, Process *qA, Process *qB, Process *qC, int count) { //Populates array with contents from file
    int i, nA = 0, nB = 0, nC = 0;
    for (i = 0; i < count; i++){
        if ((List + i)->getqueue() == 1){
            *(qA + nA) = *(List + i);
            nA++;
        } else if ((List + i)->getqueue() == 2){
            *(qB + nB) = *(List + i);
            nB++;
        } else {
            *(qC + nC) = *(List + i);
            nC++;
        }
    }
}

void printQueue(Process *queue, int count) {
    int i;
    cout << "PID \t" << "Arrival Time \t" << "Burst Time \t" << "Queue \t" << "Priority \t" << endl; 
    for (i = 0; i < count; i++) {
        cout << (queue + i)->getPid() << "\t    " << (queue + i)->getArrival() << "\t\t    " << (queue + i)->getBurst() << "\t\t  " << (queue + i)->getqueue() << "\t";
        if ((queue + i)->getPriority() > 0)
            cout << "    " << (queue + i)->getPriority();
        cout << "\n";
    }
}


void refreshCompletedProcess() {
    system("cls");
    cout << "Time Elapsed (in seconds): " << timeElapsed << endl;
    cout << "Processes Completed: " << processesCompleted << endl;
}

void displayTime(int increase) {
    int i;
    for (i = 0; i < increase; i++){
        Sleep(1000);
        timeElapsed++;
        refreshCompletedProcess();
    }
}

int roundRobin(Process *qA, int count) {
	int flag = 0;
    if ((qA + roundToServe)->getArrival() <= timeElapsed) {
        if ((qA + roundToServe)->getRemaining() > 0) {
            flag = 1;
            if ((qA + roundToServe)->getBurst() == (qA + roundToServe)->getRemaining()) {
                (qA + roundToServe)->setFirstContact(timeElapsed);
                (qA + roundToServe)->setContact(true);
            }			
            if ((qA + roundToServe)->getRemaining() <= quantum){
                (qA + roundToServe)->setComplete(true);
                (qA + roundToServe)->setExitTime(timeElapsed + (qA + roundToServe)->getRemaining());                
                displayTime((qA + roundToServe)->getRemaining());
                (qA + roundToServe)->setRemaining(0);
                rRComplete++;
                processesCompleted++;
                refreshCompletedProcess();
                if (rRComplete == count)
                    return flag;
            } else {
                displayTime(quantum);
                int remainder = (qA + roundToServe)->getRemaining() - quantum;
                (qA + roundToServe)->setRemaining(remainder);            
            }	                         	        		        
            if (roundToServe == count-1)
                roundToServe = 0;
            else
                roundToServe++; 
                
            while ((qA + roundToServe)->getRemaining() == 0) {
                if (rRComplete == count) {
                    flag = 0;
                    break;
                } else {
                    if (roundToServe == count-1)
                        roundToServe = 0;
                    else
                        roundToServe++; 
                } 					 	
            }            	
        }	
    }													        	
    return flag;       
}

int nPP(Process *qB, int count) {
    int flag = 0, indexPriority, i, readyCount = 0, checkPoint = -1; 
    for (i = 0; i < count; i++) {
        if ((qB + i)->getArrival() <= timeElapsed) {
            readyCount++;           
        }            
    }    
    for (i = 0; i < readyCount; i++) {
        if ((qB + i)->getComplete() == false){
            checkPoint = i;
            break;
        }
    }
    
    if (checkPoint == -1)
        return flag;
    else {  
        flag = 1;      
        for (i = checkPoint; i < readyCount; i++) {
            if ((qB + i)->getComplete() == false) {
                if ((qB + i)->getPriority() < (qB + checkPoint)->getPriority()) {
                    checkPoint = i;
                }
            }
        }
        indexPriority = checkPoint;
        (qB + indexPriority)->setRemaining(0);
        (qB + indexPriority)->setComplete(true);
        (qB + indexPriority)->setContact(true);
        (qB + indexPriority)->setFirstContact(timeElapsed);
        (qB + indexPriority)->setExitTime(timeElapsed + (qB + indexPriority)->getBurst());
        displayTime((qB + indexPriority)->getBurst());
        nPPComplete++;
        processesCompleted++;
        refreshCompletedProcess();
    }
    return flag;
}

int sJF(Process *qC, int count) {
    int flag = 0;
    if ((qC + 0)->getArrival() <= timeElapsed){
        int shortestIndex, j;
        shortestIndex = 0;
        flag = 1;
        for (j = 1; j < count; j++){
            if ((qC + j)->getArrival() <= timeElapsed) {
                if ((qC + j)->getComplete() == false) {
                    if ((qC + j)->getBurst() < (qC + shortestIndex)->getBurst()) {
                        shortestIndex = j;                        
                    }                                        
                }                
            } else 
                break;             
        }
        if (flag == 1) {
            (qC + shortestIndex)->setRemaining(0);
            (qC + shortestIndex)->setComplete(true);
            (qC + shortestIndex)->setContact(true);
            (qC + shortestIndex)->setExitTime(timeElapsed + (qC + shortestIndex)->getBurst());
            (qC + shortestIndex)->setFirstContact(timeElapsed);
            displayTime((qC + shortestIndex)->getBurst());
            sJFComplete++;
            processesCompleted++;
            refreshCompletedProcess();
        }        
    }
    return flag;
}

void scheduler(Process *qA, Process *qB, Process *qC, int qAC, int qBC, int qCC) {
    int f1 = 0, f2 = 0, f3 = 0;
    if (rRComplete < qAC) {
        f1 = roundRobin(qA, qAC);    
        while (f1 == 1)
            f1 = roundRobin(qA, qAC);    
    }
        
    if (nPPComplete < qBC) 
        f2 = nPP(qB, qBC);
    if (sJFComplete < qCC) 
        f3 = sJF(qC, qCC);
    
    if (processesCompleted != (qAC + qBC + qCC))
        if ((f1+ f2+ f3) == 0)
            displayTime(1);
}

void restructure(Process *list, Process *A, Process *B, Process *C, int cA, int cB, int cC) {
    int i;
    for (i = 0; i < cA; i++)
        *(list + i) = *(A + i);
    
    for (i = 0; i < cB; i++)
        *(list + i + cA) = *(B + i);
    
    for (i = 0; i < cC; i++)
        *(list + i + cA + cB) = *(C + i);
}

int averageTurnAroundTime(Process *queue, int count) {
    int tAround = 0, avg;
    for (int i = 0; i < count; i++){
        tAround += ((queue + i)->getExitTime() - (queue + i)->getArrival());
    }
    avg = tAround/count;
    return avg;
}

int averageWaitingTime(Process *queue, int count) {
    int wait = 0, avg;
    for (int i = 0; i < count; i++){
        wait += ((queue + i)->getExitTime() - (queue + i)->getArrival()) - (queue + i)->getBurst();
    }
    avg = wait/count;
    return avg;
}

int averageResponseTime(Process *queue, int count) {
    int response = 0, avg;
    for (int i = 0; i < count; i++){
        response += (queue + i)->getFirstContact() - (queue + i)->getArrival();
    }
    avg = response/count;
    return avg;
}

int main() {
    int i, processCount, pid, arrival, burst, qACount = 0, qBCount = 0, qCCount = 0;
    int avgTurnTime, avgWaitTime, avgResponseTime;
    Process *processList, *qA, *qB, *qC;
    srand(time(0));
    cin >> processCount;
    processList = new Process[processCount];
    for (i = 0; i < processCount; i++) {
        cin >> pid >> arrival >> burst;
        if (burst >= 1 && burst <= 3){ //Assumes Burst will not be less than 1
            qACount++;
            (processList + i)->setqueue(1);
            (processList + i)->setRemaining(burst);

        } else if (burst > 3 && burst <= 6) {
            qBCount++;
            (processList + i)->setqueue(2);
        } else {
            qCCount++;
            (processList + i)->setqueue(3);

        } 
        (processList + i)->setPid(pid);
        (processList + i)->setArrival(arrival);
        (processList + i)->setBurst(burst);        
        (processList + i)->setComplete(false);
        if ((processList + i)->getqueue() == 2)
            (processList + i)->setPriority((rand()%3)+1); //Proprity was given through random assignment of a value in the range 1 - 3. If object is not in queue 2 it will remain as 0.
    }

    qA = new Process[qACount];
    qB = new Process[qBCount];
    qC = new Process[qCCount];

    queueSortArrival(processList, processCount);
    queueAssignment(processList, qA, qB, qC, processCount);
    cout << "\nProcesses Assigned to respective Queues." << endl;

    cout << "\nEnter Quantum for Round Robin Algorithm: ";
    cin >> quantum;
    while (quantum < 1){
        cout << "Invalid Entry! Try Again.\nQuantum: ";
        cin >> quantum;
    }
    refreshCompletedProcess();
   
    while (processesCompleted < processCount) { //Scheduler called until all processes completed.
        scheduler(qA, qB, qC, qACount, qBCount, qCCount);        
    }
    refreshCompletedProcess();
    restructure(processList, qA, qB, qC, qACount, qBCount, qCCount);
    queueSortArrival(processList, processCount);
    
    avgTurnTime = averageTurnAroundTime(processList, processCount);
    avgWaitTime = averageWaitingTime(processList, processCount);
    avgResponseTime = averageResponseTime(processList, processCount);
    
    printQueue(processList, processCount);
    cout << "\nAverage Turnaround: " << avgTurnTime;
    cout << "\nAverage Waiting: " << avgWaitTime;
    cout << "\nAverage Response: " << avgResponseTime;
    cout << "\n\n(Note: Queues numbered 1, 2 and 3 represent Round Robin, NPP and SJF respectively.";
    cout << "\n(Note: Only processes belonging to Queue 2 will have a priority to display.";
    return 0;
}
