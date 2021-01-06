//
// pi-active
// Version for the Pilaptop (Waveshare)
//
// rricharz 2021
//
// This daemon displays a row on the 5x5 LED matrix showing
// the activity of the CPU. 
//
// The speed of the LED blinking shows current CPU usage
//
// cpu usage code from Matheus (https://github.com/hc0d3r)
//
// To install:
//     chmod +x install
//     ./install
//
// To uninstall
//     chmod +x uninstall
//     ./uninstall
//


#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define ROW1 38
#define ROW5 34
#define COL1 43
#define COL5 39

int wasRunning = 1;	// makes sure that gpio pins are reset for first usage

int blinkForOneSecond(int usage)
// blink for one second, rate based on usage
{
	int n = (usage / 10) + 1;
	int d = 125 / n;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 5; j++) {
        		digitalWrite(COL1-j, 0);
        		delay(d);
			digitalWrite(COL1-j, 1);
		}
		for (int j = 3; j > 0; j--) {
        		digitalWrite(COL1-j, 0);
        		delay(d);
			digitalWrite(COL1-j, 1);
		}
    	}
}

void resetGpios()
// set all matrix gpio pins to input and high
{
    	for (int i = ROW5; i<=COL1; i++)
		pinMode(i, OUTPUT);
	for (int i = ROW5; i<=ROW1; i++)
		digitalWrite(i, 0);
	for (int i = COL5; i<=COL1; i++)
		digitalWrite(i, 1);
	digitalWrite(ROW1, 1);
}

int *parser_result(const char *buf, int size) {
// parse /proc/stat and return cpu usage
    static int ret[10];
    int i, j = 0, start = 0;

    for(i=0; i<size; i++) {
	char c = buf[i];
	if(c >= '0' && c <= '9') {
	    if(!start){
		start = 1;
		ret[j] = c-'0';
	    }
	    else {
		ret[j] *= 10;
		ret[j] += c-'0';
	    }
	}
	else if (c == '\n') {
	    break;
	}
	    else
	{
	    if(start) {
		j++;
		start = 0;
	    }
	}
    }
    return ret;
}


int main (int argc, char **argv)
{
    char buf[256];
    int size, fd, *nums;
    int prev_idle = 0;
    int prev_total = 0;
    int idle, total, i;
    double usage;
    int fanIsOn;
        
    fd = open("/proc/stat", O_RDONLY);

    wiringPiSetupGpio();		// initialize, use gpio numbering scheme
    
    do {
	if (system("pidof -x R65 >/dev/null") != 0) {

	    // R65 is currently not running

	    if (wasRunning) {
		resetGpios();
		wasRunning = 0;
	    }

	    size = read(fd, buf, sizeof(buf));
	    if(size > 0) {

		nums = parser_result(buf, size);

		idle=nums[3];

		for(i=0, total=0; i<10; i++) total += nums[i];

		int diff_idle = idle-prev_idle;
		int diff_total = total-prev_total;
		usage = (double)(((double)(1000*(diff_total-diff_idle))/(double)diff_total+5)/(double)10);

	    }
	    else {
		usage = 0.0;
	    }
                    
	    if (usage < 1.0) usage = 1.0; // avoid too long delays or underflow
	    if (usage > 99.0) usage = 99.0;

	    prev_total = total;
	    prev_idle = idle;

	    lseek(fd, 0, SEEK_SET);
                    
	    blinkForOneSecond((int)usage);               
	}
	else {
	    if (!wasRunning) resetGpios();
	    wasRunning = 1;
	    delay(1000);
	}
    
    }
    while (1); // this is a daemon, loop until killed
}
