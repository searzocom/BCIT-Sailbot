// Datalogger class

// Author: Matthew Knight
// File Name: datalogger.cpp
// Date: 2017-06-28

// This class generates csv lines to be either written to a file, or sent to
// the console interface of the sailbot.

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cstdio>
#include "varTable.hpp"
#include "datalogger.hpp"

// buffer length for timestamp
#define BUFLEN 24

// Datalogging flags
#define D_POS	    0x01
#define D_POS_ERR   0x02
#define D_ACC	    0x04
#define D_HEAD	    0x08
#define D_QUAT	    0x10
#define D_W_SPD	    0x20
#define D_W_DIR	    0x40

using namespace std::chrono;

// Helper function for creating timestamp string in UTC.
std::string time_stamp(system_clock::time_point time)
{
    // get milliseconds count
    system_clock::duration tp = time.time_since_epoch();
    tp -=duration_cast<seconds>(tp);
    time_t cTime = system_clock::to_time_t(time);
    tm *t = gmtime(&cTime);

    // allocate space for string
    char buf[BUFLEN];
    
    // format entire string
    snprintf(buf, BUFLEN, "%04u-%02u-%02u %02u:%02u:%02u.%03u",
	t->tm_year + 1900,
	t->tm_mon,
	t->tm_mday,
	t->tm_hour,
	t->tm_min,
	t->tm_sec,
	static_cast<unsigned>(tp/milliseconds(1))
    );
    
    return std::string(buf);
}

// Operator overloading for 2 element array
std::ostream& operator<<(
    std::ostream& stream, 
    const std::array<double, 2ul> pair
)
{
    return stream << pair[0] << "," << pair[1];
    
}

// Operator overloading for 3 element array
std::ostream& operator<<(
    std::ostream& stream, 
    const std::array<double, 3ul> thruple
)
{
    return stream << thruple[0] << "," << thruple[1] << "," << thruple[2];
}

// datalogger ctor
datalogger::datalogger(varTable *system, int newFlags)
    : systemVar(system)
    , flags(newFlags)
{}

// set flags
void datalogger::set(int newFlags)
{
    flags |= newFlags;
}

// clear flags
void datalogger::clear(int newFlags)
{
    flags &= ~newFlags;
}

// Make header
std::string datalogger::header()
{
    std::string buf;
    std::stringstream line;
    
    line << "Date/Time,";

    if (flags & D_POS)
	line << "Latitude,Longitude,";
    if (flags & D_POS_ERR)
	line << "Latitude Error,Longitude Error,";
    if (flags & D_ACC)
	line << "Acc_x,Acc_y,Acc_z,";
    if (flags & D_HEAD)
	line << "Quat_i,Quat_j,Quat_k,";
    if (flags & D_QUAT)
	line << "Heading,";
    if (flags & D_W_SPD)
	line << "Wind Speed,";
    if (flags & D_W_DIR)
	line << "Wind Direction,";

    // delete last comma and end line
    line << std::endl;
    
    line >> buf;
    return buf;
}

// sample variables
std::string datalogger::sample()
{
    std::string buf;
    std::stringstream line;

    line << time_stamp(system_clock::now()) << ",";

    if (flags & D_POS)
	line << systemVar->getPosition() << ",";
    if (flags & D_POS_ERR)
	line << systemVar->getError() << ",";
    if (flags & D_ACC)
	line << systemVar->getAcc() << ",";
    if (flags & D_HEAD)
	line << systemVar->getHeading() << ",";
    if (flags & D_QUAT)
	line << systemVar->getQuat() << ",";
    if (flags & D_W_SPD)
	line << systemVar->getWindSpeed() << ",";
    if (flags & D_W_DIR)
	line << systemVar->getWindDir() << ",";
    
    // delete last comma and end line
    line << std::endl;

    line >> buf;
    return buf;
}
