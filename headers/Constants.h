#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

enum class Instrument { 
    ROSE, LAVENDER, LOTUS, TULIP, ORCHID, INVALID 
};                                                //has INVALID to handle errors

enum class Status {
    NEW, PARTIAL_FILL, FILL, REJECTED
};

#endif