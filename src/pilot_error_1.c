#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "arduino.h"
#include "io.h"
#include "request.h"
#include "uss.h"
#include "stop.h"
#include "C_send.h"
#include "C_main.h"		//thread A
#include "py_exec.h"	//thread B

int main(void) // main function
{
	int s,mode;
	pthread_t tid_A, tid_B;

	if(io_open() != 0) return -1;
	if(arduino_open() != 0) return -1;
	if(uss_open_l() != 0) return -1;
	if(uss_open_r() != 0) return -1;
	if(stop_open() !=0) return -1;

	printf("press enter to start\n");
	getchar();

	while(1){
		// 処理を記述
		printf("1:アピール  2:乗車待機  3:ライントレース往路  4:降車待機  5:注射待機  6:ライントレース復路  7:終了\n");
		scanf("%d",&mode);
		//app(&mode);
		
		/* create therad A */
		s = pthread_create( &tid_A, NULL, (void *)C_main, &mode);
		if( s != 0 ){ printf("create error!\n"); return -1; }

		/* create therad B */
		s = pthread_create( &tid_B, NULL, (void *)py_exec, NULL);
		if( s != 0 ){ printf("create error!\n"); return -1; }
		
		pthread_join(tid_A, NULL);
		pthread_join(tid_B, NULL);
		
		pthread_exit(&tid_A);
	}

	arduino_close();
	return 0;
}
