#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include "stop.h"

void test_stop_2(){
	static int sw_stop;

	io_stop(&sw_stop);
	printf("switch_stop = %d\n", sw_stop);
	usleep(100*1000);
}

int main(){
	if(stop_open() != 0) return -1;
	
	while(1){
		wiringPiISR(7,INT_EDGE_BOTH,test_stop_2);
		sleep(10000);
	}
	
	return 0;
}
