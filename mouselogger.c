#include "mouselogger.h"

// clock code https://gist.github.com/jbenet/1087739
void current_utc_time(struct timespec *ts) {
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, ts);
#endif
}
size_t dheight = NULL;
size_t dwidth = NULL;
size_t cwidth = NULL;
size_t cheight = NULL;
int rows = 5;
int cols = 7;
void displayChanged(CGDirectDisplayID display, CGDisplayChangeSummaryFlags flags, void *userInfo){

    
    CGDirectDisplayID disID = CGMainDisplayID();

    dheight = CGDisplayPixelsHigh(disID);
    dwidth = CGDisplayPixelsWide(disID);
    fprintf(logfile,"//display configs. w: %zu h: %zu\n", dwidth, dheight);
    cwidth = (dwidth/cols);
    cheight = (dheight/rows);
    fprintf(logfile, "//cell sizes. w: %zu h: %zu\n", cwidth, cheight);
    fflush(logfile);
}
// similar approach but C# here: http://stackoverflow.com/questions/6183594/handling-mouse-events-on-transparent-window-conditionally
// http://brianlipkowitz.tumblr.com/post/4881199383/quartz-event-taps
int main(int argc, const char *argv[]) {
    
    // Create an event tap to retrieve keypresses.
    CGEventMask eventMask = (CGEventMaskBit(kCGEventMouseMoved));
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, CGEventCallback, NULL
    );

    // Exit the program if unable to create the event tap.
    if(!eventTap) {
        fprintf(stderr, "ERROR: Unable to create event tap.\n");
        exit(1);
    }

    // Create a run loop source and add enable the event tap.
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);


    // Get the current time and open the logfile.
    time_t result = time(NULL);
    logfile = fopen(logfileLocation, "a");
    
    if (!logfile) {
        fprintf(stderr, "ERROR: Unable to open log file. Ensure that you have the proper permissions.\n");
        exit(1);
    }



    // Output to logfile.
    fprintf(logfile, "\n\n//Mouselogging has begun.\n//%s\n", asctime(localtime(&result)));
    fflush(logfile);

    CGDirectDisplayID disID = CGMainDisplayID();

    dheight = CGDisplayPixelsHigh(disID);
    dwidth = CGDisplayPixelsWide(disID);
    fprintf(logfile,"//display configs. w: %zu h: %zu\n", dwidth, dheight);
    cwidth = (dwidth/cols);
    cheight = (dheight/rows);
    fprintf(logfile, "//cell sizes. w: %zu h: %zu\n", cwidth, cheight);
    fflush(logfile);
    
    // Display the location of the logfile and start the loop.
    printf("Logging to: %s\n", logfileLocation);
    fflush(stdout);

    // https://developer.apple.com/reference/coregraphics/quartz_display_services?language=objc
    /* CGDirectDisplayID disID = CGMainDisplayID(); */
    /* printf("Logging to: %i\n", disID); */
    /* fflush(stdout); */
    /* CGSize disSize = CGDisplayScreenSize(disID); */
    /* size_t dheight = CGDisplayPixelsHigh(disID); */
    /* printf("Display Height: %zu\n", dheight); */
    /* fflush(stdout); */
    /* size_t dwidth = CGDisplayPixelsWide(disID); */
    /* printf("Display Width: %zu\n", dwidth); */
    /* fflush(stdout); */

    CGDisplayRegisterReconfigurationCallback(displayChanged, NULL);
    /* CFRunLoopRun(); */
    RunApplicationEventLoop();
    return 0;
}

/* 
relevant doc links: 
https://developer.apple.com/reference/coregraphics
https://developer.apple.com/reference/coregraphics/quartz_event_services
https://developer.apple.com/reference/coregraphics/cgevent
integer value fields of CGEvent: https://developer.apple.com/reference/coregraphics/cgeventfield 
*/
int getCurrentCOL(int x){
    int currentCOL = 0;
    int i;
    for(i = 0; i < cols; i = i + 1){
        /* fprintf(logfile, "i: %i, i*cwidth: %i\n", i, (i*cwidth)); */
        /* fflush(logfile); */
        if(x > (i*cwidth)){
            currentCOL = i;
        }
    } 
    return currentCOL;

}

int getCurrentROW(int y){
    int currentROW = 0;
    int i;
    for(i = 0; i < rows; i = i + 1){
        /* fprintf(logfile, "i: %i, i*cwidth: %i\n", i, (i*cwidth)); */
        /* fflush(logfile); */
        if(x > (i*cheight)){
            currentROW = i;
        }
    } 
    return currentROW;

}



// The following callback method is invoked on every keypress.
CGEventRef CGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    if (type != kCGEventMouseMoved ) { return event; }
    CGPoint po = CGEventGetLocation(event);
    int posX = (int)po.x;
    int posY = (int)po.y;
    int currentCOL = getCurrentX(posX);
    /* const char* currentCell = getCurrentCell(posX, posY); */

    fprintf(logfile, "currentCOL | currentROW: %i | %i\n", currentCOL, currentROW);
    fflush(logfile);
    /* fprintf(logfile, "x %d\n", po.x); */
    /* fprintf(logfile, "y %d\n", po.y); */
    
    /* fprintf(logfile,"x: %i y: %i\n", posX, posY); */
    
    /* fprintf(logfile,location); */
    
    /* fflush(logfile); */

    return event;
}
