#include <stdio.h>
#include "arduino.h"
#include "request.h"
#include "number.h"

int main(){
	int i, j, mode, lenght, speed, angle;
	int dist[] = {100, 50};
	int ans[NUM_LEN];
	int num[4];
	//char buf[256];
	run_state_t state;

	if(arduino_open() != 0) return -1;

	while(1){
		printf("0:stop  1:first_long  2:first_short  3:quit\n");
		scanf("%d",&mode);

		switch(mode){
			case 0:
				request_set_runmode(STP, 0, 0);
				break;

			case 1:
				for(j = 0; j < 4; j++){
					if(j % 2 == 0){
						lenght = 0;
					}else{
						lenght = 1;
					}

					number_detect(dist[lenght]);
					number_get(ans);

					for(i = 0; i < NUM_LEN; i++){
						printf("number[%d] = %d\n", j, ans[i]);
						num[j] = ans[i];
					}

					speed = 60;
					angle = 90;

					request_set_runmode(ROT, speed, angle);
					while(1){
						request_get_runmode(&state, &speed, &angle);
						if( state == STP ) break;
					}
				}
				break;

			case 2:
				for(j = 0; j < 4; j++){
					if(j % 2 == 0){
						lenght = 1;
					}else{
						lenght = 0;
					}

					number_detect(dist[lenght]);
					number_get(ans);

					for(i = 0; i < NUM_LEN; i++){
				 		printf("number[%d] = %d\n", j, ans[i]);
				 		num[j] = ans[i];
					}
					
					speed = 60;
					angle = 90;

					request_set_runmode(ROT, speed, angle);
					while(1){
						request_get_runmode(&state, &speed, &angle);
						if( state == STP ) break;
					}
				}
				break;

			case 3:
				return 0;
			default:
				break;
		}

		speed = 60;

		if(num[0] > num[1] && num[0] > num[2] && num[0] > num[3]) angle = 0;
		if(num[1] > num[0] && num[1] > num[2] && num[1] > num[3]) angle = 90;
		if(num[2] > num[0] && num[2] > num[1] && num[2] > num[3]) angle = 180;
		if(num[3] > num[0] && num[3] > num[1] && num[3] > num[2]) angle = -90;

		request_set_runmode(ROT, speed, angle);
		while(1){
			request_get_runmode(&state, &speed, &angle);
			if( state == STP ) break;
		}
	}

	arduino_close();
	return 0;
}
