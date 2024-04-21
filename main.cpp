
//
//  main.cpp
//  Algorithms
//
//  Created by Joshua Sevilla on 4/14/24.
//
#include <iomanip>
#include <iostream>
//#include <unistd.h>
#include <vector>
#include <cmath>
#include <time.h>
#include <string>
#include <algorithm>
#include <queue>

struct LCtask {
    int taskID;
    double computationTime;
    double deadline;
    int period;
    double admissionRate;
    int numExecutions = 0;
    int numPasses = 1;
    std::string executions = "";
    
    LCtask(int taskID_, double computationTime_, int period_, double deadline_, double admissionRate_) {
        taskID = taskID_;
        computationTime = computationTime_;
        deadline = deadline_;
        period = period_;
        admissionRate = admissionRate_;
    }
};

struct HCtask {
    int taskID;
    double lowComputationTime;
    double highComputationTime;
    double deadline;
    double virtualDeadline;
    int period;
    double switchChance;
    
    HCtask(int taskID_, double lowComputationTime_, double highComputationTime_, int period_, double deadline_, double switchChance_) {
        taskID = taskID_;
        lowComputationTime = lowComputationTime_;
        highComputationTime = highComputationTime_;
        deadline = deadline_;
        period = period_;
        switchChance = switchChance_;
    }
};

struct task {
    int taskID;
    int criticalityLevel;
    int lowComputationTime;
    int highComputationTime;
    double deadline;
    double virtualDeadline;
    int period;
    double switchChance;
    int numExecutions = 0;
    int numPasses = 1;
    std::string executions = "";
    double admissionRate;
    int queuedCount = 0;

    task(int taskID_, int criticalityLevel_, int lowComputationTime_) {
        taskID = taskID_;
        criticalityLevel = criticalityLevel_;
        lowComputationTime = lowComputationTime_;
    }
    
    task(int taskID_, int criticalityLevel_, double lowComputationTime_, double highComputationTime_, int period_, double deadline_, double virtualDeadline_, double switchChance_, double admissionRate_) {
        taskID = taskID_;
        criticalityLevel = criticalityLevel_;
        lowComputationTime = lowComputationTime_;
        highComputationTime = highComputationTime_;
        deadline = deadline_;
        virtualDeadline = virtualDeadline_;
        period =  period_;
        switchChance = switchChance_;
        admissionRate = admissionRate_;
    }
    
};

double max(double x, double y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

void admissionControlProcedure(std::vector<LCtask>& list, int interval) {
    int b = 0;
    for(int i = 0; i < list.size(); i++) {
        list[i].numExecutions = 0;
        list[i].numPasses = 1;
        list[i].executions = "";
    }
    while(b < interval) {
        for(int i = 0; i < list.size(); i++) {
            if (list[i].numExecutions < list[i].numPasses * list[i].admissionRate) {
                list[i].executions += "1";
                list[i].numExecutions++;
            } else {
                list[i].executions += "0";
            }

            list[i].numPasses++;
        }
        b++;
        //usleep(1000000);
    }
    
}

void taskAdmissionControlProcedure(std::vector<task>& list, int interval) {
    int b = 0;
    while(b < interval) {
        for(int i = 0; i < list.size(); i++) {
            if (list[i].numExecutions < list[i].numPasses * list[i].admissionRate) {
                list[i].executions += "1";
                list[i].numExecutions++;
            } else {
                list[i].executions += "0";
            }
            list[i].numPasses++;
        }
        b++;
        //usleep(1000000);
    }
    
}

double DBFLowLow(LCtask task, int interval) {
    double fraction = floor((interval - task.deadline) / task.period);
    double dbf = (fraction + 1) * task.computationTime;
    //std::cout << "low low = " << dbf << std::endl;
    return dbf;
}

double DBFLowHigh(HCtask task, int interval) {
    double fraction = (interval - task.virtualDeadline) / task.period;
    double dbf = (floor(fraction) + 1) * task.lowComputationTime;
    //std::cout << "low high = " << dbf << std::endl;
    return dbf;
}

double DBFHighLow(LCtask task, int interval) {
    double fraction = (interval - task.deadline) / (task.period);
    double dbf = (task.computationTime * ceil(task.admissionRate * task.numPasses)) * (floor(fraction) + 1);
    //std::cout << "high low = " << dbf << std::endl;
    return dbf;
}

double DBFHighHigh(HCtask task, int interval) {
    double fraction = (interval - (task.deadline - task.virtualDeadline)) / task.period;
    double full = (floor(fraction) + 1) * task.highComputationTime;
    //std::cout << "full = " << full << std::endl;
    double done = 0.0;
   // std::cout << (task.deadline - task.virtualDeadline) << " " << (interval % task.period) << " " << task.deadline << std::endl;
    if(((task.deadline - task.virtualDeadline) <= (interval % task.period)) && ((interval % task.period) < task.deadline)) {
        done = max(task.lowComputationTime - (interval % task.period) + task.deadline - task.virtualDeadline, 0.0);
        //std::cout << "done = " << done << std::endl;
    }
    double dbf = (full - done);
    
    //std::cout << "high high = " << dbf << std::endl;
    return dbf;
}

double lemma3(std::vector<LCtask> lowTaskList, std::vector<HCtask> highTaskList) {
    double low = 0.0;
    double high = 0.0;
    double lowMax = 0.0;
    double highMax = 0.0;

    for(int i = 0; i < lowTaskList.size(); i++) {
        low += (lowTaskList[i].computationTime / lowTaskList[i].period);
        double x = lowTaskList[i].period - lowTaskList[i].deadline;
        if (x > lowMax) {
            lowMax = x;
        }
    }

    for(int i = 0; i < highTaskList.size(); i++) {
        high += highTaskList[i].lowComputationTime / highTaskList[i].period;
        double x = highTaskList[i].period - highTaskList[i].virtualDeadline;
        if (x > highMax) {
            highMax = x;
        }
    }
   // std::cout << "Lemma 1 low max = " << lowMax << std::endl;
    //std::cout << "Lemma 1 high max = " << highMax << std::endl;
    double c = low + high;
    
    //std::cout << "Lemma 1 c = " << c << std::endl;

    double value = (c / (1 - c)) * max(lowMax, highMax);

    //std::cout << "Lemma 1 value = " << value << std::endl;

    return value;
    
}

double lemma4(std::vector<LCtask> lowTaskList, std::vector<HCtask> highTaskList) {
    double c1 = 0.0;
    double c2 = 0.0;
    double lowMax = 0.0;
    double highMax = 0.0;

    for(int i = 0; i < lowTaskList.size(); i++) {
        c1 += lowTaskList[i].admissionRate * lowTaskList[i].computationTime / lowTaskList[i].period;
        double x = lowTaskList[i].period - lowTaskList[i].deadline + (lowTaskList[i].period / lowTaskList[i].admissionRate);
        if (x > lowMax) {
            lowMax = x;
        }
    }
    
    for(int i = 0; i < highTaskList.size(); i++) {
        c2 += highTaskList[i].highComputationTime / highTaskList[i].period;
        double x = highTaskList[i].period - highTaskList[i].deadline + highTaskList[i].virtualDeadline;
        if (x > highMax) {
            highMax = x;
        }
    }
    
    double c = c1 + c2;
   // std::cout << "c1 = " << c1 << " c2 = " << c2 << std::endl;
    //std::cout << "Lemma 2 c = " << c << std::endl;
    //std::cout << "lowMax = " << lowMax << " highMax = " << highMax << std::endl;
    double value = (c1 * lowMax + c2 * highMax) / (1 - c1 - c2);

    //std::cout << "Lemma 2 value = " << value << std::endl;

    if (c > 1) {
        return -1;
    } else {
        return value;
    }
    
}

bool conditionA(std::vector<LCtask> lowTaskList, std::vector<HCtask> highTaskList) {
    
    double lowLow = 0.0;
    double lowHigh = 0.0;
    double interval = lemma3(lowTaskList, highTaskList);
    
    for(int j = 0; j < interval; j++) {
       // std::cout << "interval: " << j << std::endl;
        lowLow = 0.0;
        lowHigh = 0.0;   
        for(int i = 0; i < lowTaskList.size(); i++) {
            double x = DBFLowLow(lowTaskList[i], j);
            lowLow += x;
        }
    
        for(int i = 0; i < highTaskList.size(); i++) {
            double x = DBFLowHigh(highTaskList[i], j);
            lowHigh += x;
        }
       // std::cout << "result: " << lowLow + lowHigh << std::endl;
        if(lowLow + lowHigh <= j) {
           // std::cout << "------------------" << std::endl;
            continue;
        } else {
            std::cout << "Lemma 1 fail at = " << j << std::endl;
            return false;
        }
    }
    return true;
}

bool conditionB(std::vector<LCtask> lowTaskList, std::vector<HCtask> highTaskList) {
    
    double highLow = 0.0;
    double highHigh = 0.0;
    double interval = lemma4(lowTaskList, highTaskList);

    if (interval == -1) {
        return false;
    }
    
    for(int j = 0; j < interval; j++) {
        highLow = 0.0;
        highHigh = 0.0;
      /*  std::cout << "interval: " << j << std::endl;
        admissionControlProcedure(lowTaskList, j);  
        for(int i = 0; i < lowTaskList.size(); i++) {
            std::cout << "Executions: " << lowTaskList[i].numExecutions << std::endl;
        }  */
        for(int i = 0; i < lowTaskList.size(); i++) {
            double x = DBFHighLow(lowTaskList[i], j);
            highLow += x;
        }
    
        for(int i = 0; i < highTaskList.size(); i++) {
            double x = DBFHighHigh(highTaskList[i], j);
            highHigh += x;
        }
       // std::cout << "result: " << highLow + highHigh << std::endl;
        if(highLow + highHigh <= j) {
           // std::cout << "------------------" << std::endl;
            continue;
        } else {
            std::cout << "Lemma 2 fail at = " << j << std::endl;
            return false;
        }
    }

    return true;
    
}

bool compareByDeadline(const task &a, const task &b)
{
    return a.deadline < b.deadline;
}

bool compareByVirtualDeadline(const task &a, const task &b)
{   
    if(a.criticalityLevel == 1 && b.criticalityLevel == 0) {
        return a.virtualDeadline < b.deadline;
    }
    if(a.criticalityLevel == 0 && b.criticalityLevel == 1) {
        return a.deadline < b.virtualDeadline;
    }
    if(a.criticalityLevel == 0 && b.criticalityLevel == 0) {
        return a.deadline < b.deadline;
    }
    else {
        return a.virtualDeadline < b.virtualDeadline;
    }
}

int main(int argc, const char * argv[]) {
    
    srand(time_t(0));
    
    //comp, period, deadline

    HCtask testTask4(1, 1, 3, 6, 6, 1.0);
    LCtask testTask1(2, 1, 3, 3, 0.5);
    LCtask testTask2(3, 2, 6, 4, 0.4);
    
    std::vector<LCtask> lowTaskQ;
    std::vector<HCtask> highTaskVector;
    lowTaskQ.push_back(testTask1);
    lowTaskQ.push_back(testTask2);
    highTaskVector.push_back(testTask4);
    
    double delta = 0.5;
    double q = 0.5;
    double accuracy = 0.000025;
    
    while (delta >= accuracy) {
        delta /= 2;
        for (int i = 0; i < highTaskVector.size(); i++) {
            highTaskVector[i].virtualDeadline = q * highTaskVector[i].deadline;
            std::cout << "deadline: " << highTaskVector[i].virtualDeadline << std::endl;
        }
        
        bool condA = conditionA(lowTaskQ, highTaskVector);
        bool condB = conditionB(lowTaskQ, highTaskVector);
        
        if(condA && condB) {
            std::cout << q << std::endl;
            break;
        } else if (condA == true && condB == false) {
            q = q - delta;
        } else if (condA == false && condB == true) {
            q = q + delta;
        } else {
            std::cout << "failure" << std::endl;
            break;
        }
    }
    
    int mode = 1; //0 = low, 1 = high
    std::vector<task> lowModeTaskVector;
    std::vector<task> highModeTaskVector;
    for(int i = 0; i < lowTaskQ.size(); i++) {
        task x = task(lowTaskQ[i].taskID, 0, lowTaskQ[i].computationTime, 0, lowTaskQ[i].period, lowTaskQ[i].deadline, 0, 0, lowTaskQ[i].admissionRate);
        lowModeTaskVector.push_back(x);
        highModeTaskVector.push_back(x);
    }
    
    for(int i = 0; i < highTaskVector.size(); i++) {
        task x = task(highTaskVector[i].taskID, 1, highTaskVector[i].lowComputationTime, highTaskVector[i].highComputationTime, highTaskVector[i].period, highTaskVector[i].deadline, highTaskVector[i].virtualDeadline, highTaskVector[i].switchChance, 0);
        lowModeTaskVector.push_back(x);
        highModeTaskVector.push_back(x);
    }

    sort(lowModeTaskVector.begin(), lowModeTaskVector.end(), compareByDeadline);
    sort(highModeTaskVector.begin(), highModeTaskVector.end(), compareByVirtualDeadline);
    for(int i = 0; i < lowModeTaskVector.size(); i++) {
        std::cout << lowModeTaskVector[i].deadline << std::endl;
    }
    for(int i = 0; i < highModeTaskVector.size(); i++) {
        std::cout << highModeTaskVector[i].taskID << std::endl;
    }

    int cap = 20;
    int time = 1;
    double idleTime = 0;
    double runTime = 0;
    std::queue<task> queue;
    std::vector<task> readyQ;

    for(int i = 0; i < highModeTaskVector.size(); i++) {
        queue.push(highModeTaskVector[i]);
    }
    bool switchMode = false;
    taskAdmissionControlProcedure(highModeTaskVector, cap);

    for(int i = 0; i < highModeTaskVector.size(); i++) {
       std::cout << highModeTaskVector[i].executions << std::endl;
    }

    int runCounter = 0;
    while(time < cap) {
        std::queue<task> tempQ = queue;
        for (; !tempQ.empty(); tempQ.pop())
            std::cout << tempQ.front().taskID << ' ';
        std::cout << '\n';

        std::cout << "time: " << time << std::endl;
        //task moving
        if(queue.empty()) {
            idleTime++;
            std::cout << "idle" << std::endl;
        } else {
            runCounter++;
            runTime++;
            std::cout << "running task: " << queue.front().taskID << " counter = " << runCounter << std::endl;
        }
        //task finishes

        if(mode == 0) {
            if(queue.empty()) {
                std::cout << "No task in queue" << std::endl;
            }
            else if(queue.front().criticalityLevel == 0 && runCounter % queue.front().lowComputationTime == 0) {
                std::cout << "task: " << queue.front().taskID <<  " finished" << std::endl;
                readyQ.push_back(queue.front());
                queue.pop();
                runCounter = 0;
                if(queue.empty()) {
                    std::cout << "No task queued" << std::endl;
                } else {
                    std::cout << "task: " << queue.front().taskID <<  " queued" << std::endl;
                }
            }
            else if(queue.front().criticalityLevel == 1 && runCounter % queue.front().lowComputationTime == 0) {
                std::cout << "task: " << queue.front().taskID <<  " finished" << std::endl;
                int randomNum = rand() % 100 + 1;
                if(randomNum <= queue.front().switchChance * 100) {
                    std::cout << "task: " << queue.front().taskID <<  " switch to high mode" << std::endl;
                    switchMode = true;
                } else {
                    readyQ.push_back(queue.front());
                    queue.pop();
                    runCounter = 0;
                }
                if(queue.empty()) {
                    std::cout << "No task queued" << std::endl;
                } else {
                    std::cout << "task: " << queue.front().taskID <<  " queued" << std::endl;
                }
            }
        } else {
            if(queue.empty()) {
                std::cout << "No task in queue" << std::endl;
            }
            else if(queue.front().criticalityLevel == 0 && runCounter % queue.front().lowComputationTime == 0) {
                std::cout << "task: " << queue.front().taskID <<  " finished" << std::endl;
                readyQ.push_back(queue.front());
                queue.pop();
                runCounter = 0;
                
            }
            else if(queue.front().criticalityLevel == 1 && runCounter % queue.front().highComputationTime == 0) {
                std::cout << "task: " << queue.front().taskID <<  " finished" << std::endl;
                readyQ.push_back(queue.front());
                queue.pop();
                runCounter = 0;
               
            }
        }

        if(switchMode) {
            //switch from low to high
            if(mode == 0) {
                readyQ.clear();
                while(!queue.empty()) {
                    queue.pop();
                }
                for(int i = 0; i < highModeTaskVector.size(); i++) {
                    queue.push(highModeTaskVector[i]);
                }
                switchMode = false;
                mode = 1;
            }
        }
        
        //scheduler
        for(int i = 0; i < highModeTaskVector.size(); i++) {
            if(time % highModeTaskVector[i].period == 0) {
                if(highModeTaskVector[i].criticalityLevel == 0 && highModeTaskVector[i].executions[highModeTaskVector[i].queuedCount] == '0') {
                    std::cout << "task: " << highModeTaskVector[i].taskID <<  " dropped" << std::endl;
                } else {
                    std::cout << "task: " <<highModeTaskVector[i].taskID <<  " pushed to queue" << std::endl;
                    queue.push(highModeTaskVector[i]);
                }
                highModeTaskVector[i].queuedCount++;
            }
        }
        time++;
        
    }
    double idlePercent = idleTime / (cap - 1);
    double usePercent = runTime / (cap - 1);
    std::cout << "idle / time: " << idlePercent  <<  std::endl;
    std::cout << "use / time: " << usePercent  <<  std::endl;

    return 0;
}
