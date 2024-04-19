
//
//  main.cpp
//  Algorithms
//
//  Created by Joshua Sevilla on 4/14/24.
//

#include <iostream>
#include <unistd.h>
#include <vector>
#include <cmath>
#include <time.h>

const int interval = 42;

struct LCtask {
    int taskID;
    int computationTime;
    int deadline;
    int period;
    float admissionRate;
    int numExecutions = 0;
    int numPasses = 1;
    std::string executions = "";
    
    LCtask(int taskID_, int computationTime_, int period_, int deadline_, float admissionRate_) {
        taskID = taskID_;
        computationTime = computationTime_;
        deadline = deadline_;
        period = period_;
        admissionRate = admissionRate_;
    }
};

struct HCtask {
    int taskID;
    int lowComputationTime;
    int highComputationTime;
    int deadline;
    float virtualDeadline;
    int period;
    float switchChance;
    
    HCtask(int taskID_, int lowComputationTime_, int highComputationTime_, int period_, int deadline_, float switchChance_) {
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
    float deadline;
    int period;
    float switchChance;
    
    task(int taskID_, int criticalityLevel_, int lowComputationTime_, int highComputationTime_, int period_, float deadline_, float switchChance_) {
        taskID = taskID_;
        criticalityLevel = criticalityLevel_;
        lowComputationTime = lowComputationTime_;
        highComputationTime = highComputationTime_;
        deadline = deadline_;
        period =  period_;
        switchChance = switchChance_;
    }
    
};

float max(float x, float y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

float DBFLowLow(LCtask task) {
    float fraction = (interval - task.deadline) / task.period;
    float dbf = (floor(fraction) + 1) * task.computationTime;
    std::cout << "low low = " << dbf << std::endl;
    return dbf;
}

float DBFLowHigh(HCtask task) {
    float fraction = (interval - task.virtualDeadline) / task.period;
    float dbf = (floor(fraction) + 1) * task.lowComputationTime;
    std::cout << "low high = " << dbf << std::endl;
    return dbf;
}

float DBFHighLow(LCtask task) {
    float fraction = (interval - task.deadline) / (task.period);
    float dbf = (task.computationTime * 1) * (floor(fraction) + 1);
    std::cout << "high low = " << dbf << std::endl;
    return dbf;
}

float DBFHighHigh(HCtask task) {
    float fraction = (interval - (task.deadline - task.virtualDeadline)) / task.period;
    float full = (floor(fraction) + 1) * task.highComputationTime;
    float done = 0.0;
    if(task.deadline > (interval % task.period) >= (task.deadline - task.virtualDeadline)) {
        done = max(task.lowComputationTime - (interval % task.period) + task.deadline - task.virtualDeadline, 0.0);
    }
    float dbf = (full - done);
    
    std::cout << "high high = " << dbf << std::endl;
    return dbf;
}

bool c1(std::vector<LCtask> lowTaskList, std::vector<HCtask> highTaskList) {
    float low = 0.0;
    float high = 0.0;
    float lowMax = 0.0;
    float highMax = 0.0;
    for(int i = 0; i < lowTaskList.size(); i++) {
        low += lowTaskList[i].computationTime / lowTaskList[i].period;
        float x = lowTaskList[i].period - lowTaskList[i].deadline;
        if (x > lowMax) {
            lowMax = x;
        }
    }
    
    for(int i = 0; i < highTaskList.size(); i++) {
        high += highTaskList[i].lowComputationTime / highTaskList[i].period;
        float x = highTaskList[i].period - highTaskList[i].virtualDeadline;
        if (x > highMax) {
            highMax = x;
        }
    }
    
    float c = low + high;
    
    if(c > 1) {
        return false;
    }
    
    if(interval < ( (c / (1 - c)) * max(lowMax, highMax) ) ) {
        return true;
    } else {
        return false;
    }
}

bool conditionA(std::vector<LCtask> lowTaskList, std::vector<HCtask> highTaskList) {
    
    float lowLow = 0.0;
    float lowHigh = 0.0;
    std::cout << c1(lowTaskList, highTaskList) << std::endl;
    
    for(int i = 0; i < lowTaskList.size(); i++) {
        float x = DBFLowLow(lowTaskList[i]);
        lowLow += x;
    }
    
    for(int i = 0; i < highTaskList.size(); i++) {
        float x = DBFLowHigh(highTaskList[i]);
        lowHigh += x;
    }
    if(lowLow + lowHigh <= interval) {
        return true;
    } else {
        return false;
    }
    
    
}

bool conditionB(std::vector<LCtask> lowTaskList, std::vector<HCtask> highTaskList) {
    
    float highLow = 0.0;
    float highHigh = 0.0;
    
    for(int i = 0; i < lowTaskList.size(); i++) {
        float x = DBFHighLow(lowTaskList[i]);
        highLow += x;
    }
    
    for(int i = 0; i < highTaskList.size(); i++) {
        float x = DBFHighHigh(highTaskList[i]);
        highHigh += x;
    }
    if(highLow + highHigh <= interval) {
        return true;
    } else {
        return false;
    }
    
}

int main(int argc, const char * argv[]) {
    
    srand(time_t(0));
    
    //comp, period, deadline
    HCtask testTask4(2, 1, 3, 6, 6, 1.0);
    LCtask testTask1(0, 1, 3, 3, 0.5);
    LCtask testTask2(1, 2, 6, 4, 0.4);
    
    std::vector<LCtask> lowTaskQ;
    std::vector<HCtask> highTaskVector;
    lowTaskQ.push_back(testTask1);
    lowTaskQ.push_back(testTask2);
    highTaskVector.push_back(testTask4);
  
    //admission control procedure
    int b = 0;
    while(b < interval) {
        if (lowTaskQ[b % lowTaskQ.size()].numExecutions < lowTaskQ[b % lowTaskQ.size()].numPasses * lowTaskQ[b % lowTaskQ.size()].admissionRate) {
            lowTaskQ[b % lowTaskQ.size()].executions += "1";
            lowTaskQ[b % lowTaskQ.size()].numExecutions++;
        }
        else {
            lowTaskQ[b % lowTaskQ.size()].executions += "0";
        }
        lowTaskQ[b % lowTaskQ.size()].numPasses++;
        b++;
        //usleep(1000000);
    }
    
    std::cout << lowTaskQ[0].executions << " 1" << std::endl;
    std::cout << lowTaskQ[1].executions << " 2" << std::endl;
    
    float delta = 0.5;
    float q = 0.5;
    float accuracy = 0.0009765625;
    
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
    
    int mode = 0; //0 = low, 1 = high
    std::vector<task> taskVector;
    for(int i = 0; i < lowTaskQ.size(); i++) {
        task x = task(lowTaskQ[i].taskID, 0, lowTaskQ[i].computationTime, 0, lowTaskQ[i].period, lowTaskQ[i].deadline, 0);
        taskVector.push_back(x);
    }
    
    for(int i = 0; i < highTaskVector.size(); i++) {
        task x = task(highTaskVector[i].taskID, 1, highTaskVector[i].lowComputationTime, highTaskVector[i].highComputationTime, highTaskVector[i].period, highTaskVector[i].virtualDeadline, highTaskVector[i].switchChance);
        taskVector.push_back(x);
    }
    
    for(int i = 0; i < taskVector.size(); i++) {
        std::cout << taskVector[i].deadline << std::endl;
    }
    
    int time = 0;
    
    return 0;
}
