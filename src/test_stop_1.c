#include <stdio.h>
#include <unistd.h>
#include "stop.h"

int main(){
	int sw_stop;

	if(stop_open() != 0) return -1;

	while(1){
		io_stop(&sw_stop);
		printf("switch_stop = %d\n", sw_stop);
		usleep(100 * 1000);
	}

	return 0;
}
