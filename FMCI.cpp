//
//  main.cpp
//  FMCI
//
//  Created by Joshua Sevilla on 4/23/24.
//

#include <iostream>
#include <vector>

struct task {
    int taskID;
    int criticalityLevel;
    int lowComputationTime;
    int highComputationTime;
    double deadline;
    double virtualDeadline;
    int period;

    task(int taskID_, int criticalityLevel_, int lowComputationTime_) {
        taskID = taskID_;
        criticalityLevel = criticalityLevel_;
        lowComputationTime = lowComputationTime_;
    }
    
    task(int taskID_, int criticalityLevel_, double lowComputationTime_, double highComputationTime_, int period_, double deadline_, double virtualDeadline_) {
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
double* lowModeUtilizations(std::vector<task> list) {
    static double results[2] = {0.0, 0.0};
    for(int i = 0; i < list.size(); i++) {
        float lowUtilization = list[i].lowComputationTime / list[i].period;
        float highUtilization = list[i].highComputationTime / list[i].period;
        results[0] += lowUtilization;
        results[1] += highUtilization;
    }
    
    return results;
}

double highModeUtilization(std::vector<task> list) {
    double result = 0.0;
    for(int i = 0; i < list.size(); i++) {
        float highUtilization = list[i].highComputationTime / list[i].period;
        result += highUtilization;
    }
    
    return result;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
