#include <stdio.h>
#include "arduino.h"
#include "str30.h"

int main(){
	int mode, speed, dist;
	double volt;
	//char buf[256];
	run_state_t state;
	
	if(arduino_open() != 0) return -1;


	while(1){
		printf("straight 30cm/sec 350cm \n");
	
		   speed=30;
		   dist=300;


	}
	
	arduino_close();
	return 0;
}
