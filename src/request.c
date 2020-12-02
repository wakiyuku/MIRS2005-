#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "arduino.h"
#include "request.h"

void request_set_runmode(run_state_t state, int speed, int dist){
	command_data_t command_data;
	
	command_data.val[0] = ((state == STR) ? 2 : (state == ROT) ? 3 : (state == CRV) ? 4 : 1);//新規追加
	command_data.val[1] = speed;
	command_data.val[2] = dist;
	
	arduino_send(command_data);
}

int request_get_runmode(run_state_t *state, int *speed, int *dist){
	command_data_t command_data;
	
	command_data.val[0] = 10;
	command_data.val[1] = 0;
	command_data.val[2] = 0;
	
	arduino_clear();
	arduino_send(command_data);
	usleep(50 * 1000);
	
	if(arduino_receive(&command_data) == 0){
		*state = ((command_data.val[0] == 2) ? STR : (command_data.val[0] == 3) ? ROT : STP);
		*speed = command_data.val[1];
		*dist  = command_data.val[1];
		return 0;
	}else{
		return -1;
	}
}

int request_get_dist(int *dist_l, int *dist_r){
	command_data_t command_data;
	
	command_data.val[0] = 11;
	command_data.val[1] = 0;
	command_data.val[2] = 0;
	
	arduino_clear();
	arduino_send(command_data);
	usleep(50 * 1000);
	
	if(arduino_receive(&command_data) == 0){
		*dist_l = command_data.val[0];
		*dist_r = command_data.val[1];
		return 0;
	}else{
		return -1;
	}
}

int request_get_batt(double *volt){
	command_data_t command_data;
	
	command_data.val[0] = 12;
	command_data.val[1] = 0;
	command_data.val[2] = 0;
	
	arduino_clear();
	arduino_send(command_data);
	usleep(50 * 1000);
	
	if(arduino_receive(&command_data) == 0){
		*volt = command_data.val[0] / 100.0;
		return 0;
	}else{
		return -1;
	}
}
