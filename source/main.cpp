// BCIT Sailbot Control System Main Thread

// Author: Matthew Knight
// File Name: main.cpp
// Date: 2017-06-05

// This is the main thread for the sailbot's control system.

#include <iostream>
#include <thread>
#include <functional>
#include <atomic>
#include <unistd.h>

#include "varTable.hpp"
#include "sbThreads.hpp"

// Main thread (server thread)
int main(void)
{

    // Initialize system variables
    varTable system;
    
    // create threads
    std::thread gpsThread(gps, &system);
    std::thread imuThread(imu, &system);
    std::thread windThread(wind, &system);

    gpsThread.detach();
    imuThread.detach();
    windThread.detach();
    
    // run forever
    while (true)
	;
}
