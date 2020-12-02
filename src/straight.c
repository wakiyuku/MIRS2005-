#include <stdio.h>
#include "arduino.h"
#include "request.h"

int main(){
	int mode, speed, dist;

	//char buf[256];
	run_state_t state;

	if(arduino_open() != 0) return -1;


	while(1){
		printf("0:stop  1:30[cm/s]  2:60[cm/s]  3:quit\n");
		scanf("%d",&mode);

		switch(mode){
		case 0:
			request_set_runmode(STP, 0, 0);
			break;
		case 1:
			speed = 30;
			dist = 300;

			printf("speed = 30[cm/s]\n");
			printf("dist = 300[cm]\n");

			request_set_runmode(STR, speed, dist);
			while(1){
				request_get_runmode(&state, &speed, &dist);
				if( state == STP ) break;
			}

			break;
		case 2:
			speed = 60;
			dist = 300;

			printf("speed = 60[cm/s]\n");
			printf("dist = 300[cm]\n");

			request_set_runmode(STR, speed, dist);
			while(1){
				request_get_runmode(&state, &speed, &dist);
				if( state == STP ) break;
			}

			break;
		case 3:
			return 0;
		default:
			break;
		}
	}

	arduino_close();
	return 0;
}
