
#include <stdio.h>
#include "arduino.h"
#include "str60.h"

int main(){
	int speed, dist;
	
	//char buf[256];
	run_state_t state;
	
	if(arduino_open() != 0) return -1;


	
	
		printf("straight 60cm/sec 350cm \n");
	
		   speed=60;
		   dist=300;
		request_set_runmode(STR, speed, dist);
		
	
	
	arduino_close();
	return 0;
}
