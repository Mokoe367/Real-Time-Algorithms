#include <iomanip>
#include <iostream>
//#include <unistd.h>
#include <vector>
#include <cmath>
#include <time.h>
#include <string>

using namespace std;

double max(double x, double y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

struct HCtask {
    int taskID;
    double lowComputationTime;
    double highComputationTime;
    double deadline;
    double virtualDeadline;
    int period;
    double switchChance;
    
    HCtask(int taskID_, double lowComputationTime_, double highComputationTime_, int period_, double deadline_, double virualDeadline_, double switchChance_) {
        taskID = taskID_;
        lowComputationTime = lowComputationTime_;
        highComputationTime = highComputationTime_;
        deadline = deadline_;
        virtualDeadline = virualDeadline_;
        period = period_;
        switchChance = switchChance_;
    }
};

double DBFLowLow(int interval, double deadline, int period, double computationTime) {
    double fraction = (interval - deadline) / period;
    double dbf = (floor(fraction) + 1) * computationTime;
    std::cout << "low low = " << dbf << std::endl;
    return dbf;
}

double DBFLowHigh(int interval, double virtualDeadline, int period, double lowComputationTime) {
    double fraction = (interval - virtualDeadline) / period;
    double dbf = (floor(fraction) + 1) * lowComputationTime;
    std::cout << "low high = " << dbf << std::endl;
    return dbf;
}

double DBFHighLow(int interval, double deadline, int period, double computationTime, int num) {
    double fraction = (interval - deadline) / (period);
    double dbf = (computationTime * num) * (floor(fraction) + 1);
    std::cout << "high low = " << dbf << std::endl;
    return dbf;
}

double DBFHighHigh(int interval, double deadline, double virtualDeadline, int period, double highComputationTime, double lowComputationTime) {
    double fraction = (interval - (deadline - virtualDeadline)) / period;
    double full = (floor(fraction) + 1) * highComputationTime;
    std::cout << "full = " << full << std::endl;
    double done = 0.0;
    std::cout << (deadline - virtualDeadline) << " " << (interval % period) << " " << deadline << std::endl;
    if((deadline - virtualDeadline) <= (interval % period) < deadline) {
        done = max(lowComputationTime - (interval % period) + deadline - virtualDeadline, 0.0);
        std::cout << "done = " << done << std::endl;
    }
    double dbf = (full - done);
    
    std::cout << "high high = " << dbf << std::endl;
    return dbf;
}

double DBFHighHighTask(HCtask task, int interval) {
    double fraction = (interval - (task.deadline - task.virtualDeadline)) / task.period;
    double full = (floor(fraction) + 1) * task.highComputationTime;
    std::cout << "full = " << full << std::endl;
    double done = 0.0;
    std::cout << (task.deadline - task.virtualDeadline) << " " << (interval % task.period) << " " << task.deadline << std::endl;
    if((task.deadline - task.virtualDeadline) <= (interval % task.period) < task.deadline) {
        done = max(task.lowComputationTime - (interval % task.period) + task.deadline - task.virtualDeadline, 0.0);
        std::cout << "done = " << done << std::endl;
    }
    double dbf = (full - done);
    
    std::cout << "high high = " << dbf << std::endl;
    return dbf;
}


int main() {
    //inerval, deadline, period, comp
    int interval = 4;
    DBFLowLow(interval, 3, 3, 1);
    DBFLowLow(interval, 4, 6, 2);

    HCtask task = HCtask(0, 1, 3, 6, 6, 3, 1.0);

    //DBFHighHigh(interval, 6, 3, 6, 3, 1);
    DBFHighLow(interval, 3, 3, 1, 1);
    DBFHighLow(interval, 4, 6, 2, 1);
   // DBFHighHighTask(task, interval);

    return 0;
}