#include <string>
#include <time.h> 

#include "format.h"

using std::string;

#define  SEC_PER_HOUR 3600
#define  SEC_PER_MIN 60

// helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    struct tm timeinfo;
    char buffer [10];

    timeinfo.tm_hour = seconds/SEC_PER_HOUR;
    seconds = seconds - timeinfo.tm_hour * SEC_PER_HOUR;
    timeinfo.tm_min = seconds / SEC_PER_MIN;
    timeinfo.tm_sec = seconds - timeinfo.tm_min * SEC_PER_MIN;
    strftime (buffer,10,"%H:%M:%S",&timeinfo);
    return buffer;
}
