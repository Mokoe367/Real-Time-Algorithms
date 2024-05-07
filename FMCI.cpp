//
//  main.cpp
//  FMCI
//
//  Created by Joshua Sevilla on 4/23/24.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <string>
#include <algorithm>
#include <queue>
#include <random>

struct task {
    int taskID;
    int criticalityLevel;
    int lowComputationTime;
    int highComputationTime;
    float deadline;
    int virtualDeadline;
    int runtime = 0;
    int waitTime = 0;
    float period;
    
    task(int taskID_, int criticalityLevel_, int lowComputationTime_, int highComputationTime_, float period_, float deadline_, int virtualDeadline_) {
        taskID = taskID_;
        criticalityLevel = criticalityLevel_;
        lowComputationTime = lowComputationTime_;
        highComputationTime = highComputationTime_;
        deadline = deadline_;
        virtualDeadline = virtualDeadline_;
        period =  period_;
    }
    
};

// 0 == lowlow, 1 == lowhigh
float lowModeLowsUtilizations(std::vector<task> list) {
    float result = 0.0;
    for(int i = 0; i < list.size(); i++) {
        if(list[i].criticalityLevel == 0) {
            float lowTask = list[i].lowComputationTime / list[i].period;
            result += lowTask;
        }
    }
    return result;
}

float lowModeHighsUtilizations(std::vector<task> list) {
    float result= 0.0;
    for(int i = 0; i < list.size(); i++) {
        if(list[i].criticalityLevel == 1) {
            float highTask = list[i].lowComputationTime / list[i].period;
            result += highTask;
        }
    }
    return result;
}

double highModeUtilization(std::vector<task> list) {
    double result = 0.0;
    for(int i = 0; i < list.size(); i++) {
        if(list[i].criticalityLevel == 1) {
            float highUtilization = list[i].highComputationTime / list[i].period;
            result += highUtilization;
        }
    }
    
    return result;
}

float math7(std::vector<task> list, float lows, float highs) {
    float result = 0.0;
    for(int i = 0; i < list.size(); i++) {
        if(list[i].criticalityLevel == 1) {
            result += ((list[i].lowComputationTime / list[i].deadline) / highs) * (1 - lows) - (list[i].highComputationTime / list[i].deadline);
        }
    }
    return result;
}

float math6(std::vector<task> &list, float lows, float highs, float x) {
    float result = 0.0;
    for(int i = 0; i < list.size(); i++) {
        if(list[i].criticalityLevel == 1) {
            float utilization = ((list[i].lowComputationTime / list[i].deadline) / highs) * (1 - lows) - (list[i].highComputationTime / list[i].deadline);
            float change = utilization / (1 - x);
            result += change;
        }
    }
    return result;
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

int findTaskToRun(std::vector<task> list) {
    int shortestDeadline = 0;
    for(int i = 0; i < list.size(); i++) {
        if(compareByVirtualDeadline(list.at(i), list.at(shortestDeadline))) {
            shortestDeadline = i;
        }
    }
    return shortestDeadline;
}

void print_node(task& p){
    std::cout << p.taskID << " ";
}

float max(float x, float y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

int main(int argc, const char * argv[]) {
    // int taskID_, int criticalityLevel_, float lowComputationTime_, float highComputationTime_, float period_, float deadline_, float virtualDeadline_
    
    std::random_device r;
    std::default_random_engine generator(r());
    std::vector<task> taskVector;
    
    //comp, period, deadline
    double utilizationBound = 0.9;
    double totalUtilization = 0.0;
    int highTasks = 0;
    int numTasks = 0;
    std::uniform_real_distribution<double> periodDistribution(20, 150);
    std::uniform_real_distribution<double> utilizationDistribution(0.05, .15);

    while(totalUtilization < utilizationBound) {
        if(highTasks < 3) {
            double utilization = utilizationDistribution(generator);
            int period = periodDistribution(generator);
            double lowComp = utilization * period;
            double highComp = lowComp * 2;
            task fodderTask(numTasks, 1, lowComp, highComp, period, period, period);
            totalUtilization += utilization;
            numTasks++;
            highTasks++;
            taskVector.push_back(fodderTask);
        } else {
            double utilization = utilizationDistribution(generator);
            int period = periodDistribution(generator);
            double lowComp = utilization * period;
            if(totalUtilization + utilization > utilizationBound) {
                break;
            } else {
                totalUtilization += utilization;
                numTasks++;
                task fodderTask(numTasks, 0, lowComp, lowComp, period, period, period);
                taskVector.push_back(fodderTask);
            }
        }
    }

/*
    task task1(0, 0, 12, 12, 86, 86, 86);
    task task2(1, 1, 6, 12, 51, 51, 51);
    task task3(2, 1, 14, 28, 106, 106, 106);
    task task4(3, 1, 3, 6, 30, 30, 30);
    task task5(4, 0, 17, 17, 137, 137, 137);
    task task6(5, 0, 20, 20, 145, 145, 145);

    taskVector.push_back(task1);
    taskVector.push_back(task2);
    taskVector.push_back(task3);
    taskVector.push_back(task4);
    taskVector.push_back(task5);
    taskVector.push_back(task6);
*/
    float lowModeLowResult = lowModeLowsUtilizations(taskVector);
    float lowModeHighResult = lowModeHighsUtilizations(taskVector);
    float highModeResult = highModeUtilization(taskVector);
    float x = lowModeLowResult / (1 - lowModeHighResult);
    std::cout << "X: " << x << std::endl;
    std::cout << "Results: " << lowModeLowResult << " " << lowModeHighResult << " " << highModeResult << std::endl;
    float test = lowModeLowResult + lowModeHighResult / x;
    std::cout << "Test: " << test << std::endl;
    float lowerBound = utilizationBound - 0.05;
    float maxValue = max((lowModeLowResult + lowModeHighResult), highModeResult);
    std::cout << lowerBound << " <= " << maxValue << " <= " << utilizationBound << std::endl;
    bool wrong = false;
    if((lowerBound <= maxValue) && (maxValue <= utilizationBound)) {
        wrong = false;
    } else {
        wrong = true;
    }
    
    while(wrong) {
        std::default_random_engine generator(r());
        std::cout << "Generating new task set " << std::endl;
        taskVector.clear();
        double totalUtilization = 0.0;
        int highTasks = 0;
        int numTasks = 0;
        
        while(totalUtilization < utilizationBound) {
            if(highTasks < 3) {
                double utilization = utilizationDistribution(generator);
                int period = periodDistribution(generator);
                double lowComp = utilization * period;
                double highComp = lowComp * 2;
                task fodderTask(numTasks, 1, lowComp, highComp, period, period, period);
                totalUtilization += utilization;
                numTasks++;
                highTasks++;
                taskVector.push_back(fodderTask);
            } else {
                double utilization = utilizationDistribution(generator);
                int period = periodDistribution(generator);
                double lowComp = utilization * period;
                if(totalUtilization + utilization > utilizationBound) {
                    break;
                } else {
                    totalUtilization += utilization;
                    numTasks++;
                    task fodderTask(numTasks, 0, lowComp, lowComp, period, period, period);
                    taskVector.push_back(fodderTask);
                }
            }
        }
        float lowModeLowResult = lowModeLowsUtilizations(taskVector);
        float lowModeHighResult = lowModeHighsUtilizations(taskVector);
        float highModeResult = highModeUtilization(taskVector);
        float x = lowModeHighResult / (1 - lowModeLowResult);
        std::cout << "X: " << x << std::endl;
        std::cout << "Results: " << lowModeLowResult << " " << lowModeHighResult << " " << highModeResult << std::endl;
        float test = lowModeLowResult + lowModeHighResult / x;
        std::cout << "Test: " << test << std::endl;
        float maxValue = max(lowModeLowResult + lowModeHighResult, highModeResult);
        std::cout << lowerBound << " <= " << maxValue << " <= " << utilizationBound << std::endl;
        if((lowerBound <= maxValue) && (maxValue <= utilizationBound)) {
            wrong = false;
        } else {
            wrong = true;
        }
    }

    for(int i = 0; i < taskVector.size(); i++) {
        if(taskVector[i].criticalityLevel == 1) {
            taskVector[i].virtualDeadline = floor(taskVector[i].deadline * x);
        }
    }
    sort(taskVector.begin(), taskVector.end(), compareByVirtualDeadline);
    float omega = math7(taskVector, lowModeLowResult, lowModeHighResult);
    std::cout << "Omega: " << omega << std::endl;
    if(omega <= 0) {
        float execess = math6(taskVector, lowModeLowResult, lowModeHighResult, x);
        std::cout << execess << std::endl;
        float difference = (lowModeLowResult + execess);
        float scale = lowModeLowResult / difference;
        for(int i = 0; i < taskVector.size(); i++) {
            if(taskVector[i].criticalityLevel == 0) {
                taskVector[i].period = ceil(taskVector[i].period * scale);
            }
        }
    }

    for(int i = 0; i < taskVector.size(); i++) {
        if(taskVector[i].criticalityLevel == 0) {
            std::cout << "Task " << taskVector[i].taskID << ": Computation = " << taskVector[i].lowComputationTime << ", Period = " << taskVector[i].period << ", Deadline = " << taskVector[i].deadline << " \n";
        }
        if(taskVector[i].criticalityLevel == 1) {
            std::cout << "Task " << taskVector[i].taskID << ": Low Computation = " << taskVector[i].lowComputationTime << ", high Computation = " << taskVector[i].highComputationTime << ", Period = " << taskVector[i].period << ", Deadline = " << taskVector[i].deadline << ", Virtual Deadline = " << taskVector[i].virtualDeadline << " \n";
        }
    }
    std::cout << "\n-----\n" << std::endl;

    int cap = 300;
    int time = 1;
    double idleTime = 0;
    double runTime = 0;
    int lowTaskRunCounter = 0;
    int lowDeadlineMissCounter = 0;
    int highDeadlineMissCounter = 0;
    std::vector<task> readyQ;

    for(int i = 0; i < taskVector.size(); i++) {
        readyQ.push_back(taskVector[i]);
    }
    while(time < cap) {
        for(int i=0; i<readyQ.size(); i++){
            print_node(readyQ[i]);
        }
        std::cout << '\n';
        std::cout << "time: " << time << std::endl;
        //task finishes
        if(readyQ.empty() == true) {
            std::cout << "No task in queue" << std::endl;
            idleTime++;
        }
        else {
            int runningTask = findTaskToRun(readyQ);
            std::cout << "running task: " << readyQ.at(runningTask).taskID << std::endl;
            readyQ.at(runningTask).runtime++;
            for(int i = 0; i < readyQ.size(); i++) {
                readyQ.at(i).waitTime++;
            }
            runTime++;
            if(readyQ.at(runningTask).criticalityLevel == 0 && readyQ.at(runningTask).runtime % readyQ.at(runningTask).lowComputationTime == 0) {
                std::cout << "task: " << readyQ.at(runningTask).taskID <<  " finished" << std::endl;
                lowTaskRunCounter++;
                for(int j = 0; j < readyQ.size(); j++) {
                    if(readyQ.at(j).taskID == readyQ.at(runningTask).taskID) {
                        readyQ.erase(readyQ.begin()+j);
                    }
                }

            } else if(readyQ.at(runningTask).criticalityLevel == 1 && readyQ.at(runningTask).runtime % readyQ.at(runningTask).highComputationTime == 0) {
                std::cout << "task: " << readyQ.at(runningTask).taskID <<  " finished" << std::endl;
                for(int j = 0; j < readyQ.size(); j++) {
                    if(readyQ.at(j).taskID == readyQ.at(runningTask).taskID) {
                        readyQ.erase(readyQ.begin()+j);
                    }
                }
            }
        }
        
        //scheduler
        for(int i = 0; i < taskVector.size(); i++) {
            if(time % int(taskVector[i].period) == 0) {
                bool isQueued = false;
                for(int j = 0; j < readyQ.size(); j++) {
                    if(taskVector[i].taskID == readyQ.at(j).taskID) {
                        isQueued = true;
                    }
                }
                if(!isQueued) {
                    readyQ.push_back(taskVector[i]);
                    std::cout << "task: " << taskVector[i].taskID <<  " pushed" << std::endl;
                } else {
                    std::cout << "task: " << taskVector[i].taskID <<  " skipped" << std::endl;
                }
            }
        }

        for(int i = 0; i < readyQ.size(); i++) {
            if(readyQ.at(i).waitTime > readyQ.at(i).deadline) {
                if(readyQ.at(i).criticalityLevel == 0) {
                    lowDeadlineMissCounter++;
                } else {
                    highDeadlineMissCounter++;
                }
                print_node(readyQ[i]);
                std::cout << "missed deadline." << std::endl;
            }
        }
        time++;
        
    }
    double idlePercent = idleTime / (cap - 1);
    double usePercent = runTime / (cap - 1);
    std::cout << "idle / time: " << idlePercent  <<  std::endl;
    std::cout << "use / time: " << usePercent  <<  std::endl;
    std::cout << "Number of low tasks finished: " << lowTaskRunCounter  <<  std::endl;
    std::cout << "Number of low task missed deadlines: " << lowDeadlineMissCounter  <<  std::endl;
    std::cout << "Number of high task missed deadlines: " << highDeadlineMissCounter  <<  std::endl;
    return 0;
}
