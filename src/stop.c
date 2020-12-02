#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include "request.h"
#include "stop.h"

// ピン配置
static const int pin_sw_stop  = 7;

int stop_open(){
	// GPIOデバイスのオープン
	if(wiringPiSetupGpio() != 0){
		printf("failed to open GPIO\n");
		return -1;
	}else{
		// ピン設定
		pinMode(pin_sw_stop, INPUT);
		pullUpDnControl(pin_sw_stop, PUD_DOWN);
		wiringPiISR(pin_sw_stop,INT_EDGE_BOTH,stop);

		printf("GPIO opened\n");
		return 0;
	}
}

void stop(){
	//int sw_stop;
	//sw_stop = digitalRead(pin_sw_stop);
	//printf("switch_stop = %d\n", sw_stop);
	//usleep(100*1000);
	
	request_set_runmode(STP, 0, 0);
	_exit(0);
}
