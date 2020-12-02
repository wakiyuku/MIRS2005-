#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "arduino.h"
#include "request.h"
#include "C_send.h"
#include "C_main.h"

enum
{
	E_APPEAL = 1,	//1：アピール
	E_GET_ON,		//2：乗車待機
	E_LINE_GO,		//3：ライントレース往路
	E_GET_OFF,		//4：降車待機
	E_INJECTION,	//5：注射待機
	E_LINE_BAKE,	//6：ライントレース復路
	E_QUIT,			//7：終了
	E_START = 10,	//10:スタート
	E_ERROR = 11	//11:エラー
};

void C_main(void *mode_recv){
	int *mode = (int *)mode_recv;
	//double volt;
	//char buf[256];
	int speed, dist;
	run_state_t state;

	while(1){
		switch(*mode){
			case E_APPEAL:
				C_send(*mode);
				while(1){
					//app(mode);
					printf("会話・音楽，順番になったら2(E_GET_ON)を入力してください。\n");
					scanf("%d",mode);
					if( *mode == E_GET_ON || *mode == E_ERROR) break;
				}
				break;

			case E_GET_ON:
				C_send(*mode);
				while(1){
					//app(mode);
					printf("乗車してください。準備できたら3(E_LINE_GO)を入力してください。\n");
					scanf("%d",mode);
					if( *mode == E_LINE_GO || *mode == E_ERROR ) break;
				}
				break;

			case E_LINE_GO:
				C_send(*mode);
				//line();
				printf("speed? [cm/s]\n");
				scanf("%d",&speed);
				printf("dist? [cm]\n");
				scanf("%d",&dist);

				request_set_runmode(STR, speed, dist);
				while(1){
					request_get_runmode(&state, &speed, &dist);
					if( state == STP ) break;
				}
				*mode = 4;

/*				while(1){
					//python();
					//app(mode);
					printf("到着しました。4(E_GET_OFF)を入力してください。\n");
					scanf("%d",mode);
					if( *mode == E_GET_OFF  || *mode == E_ERROR) break;
				}
*/
				break;

			case E_GET_OFF:
				C_send(*mode);
				while(1){
					//app(mode);
					printf("降車してください。降車したら5(E_INJECTION)を入力してください。\n");
					scanf("%d",mode);
					if( *mode == E_INJECTION  || *mode == E_ERROR) break;
				}
				break;

			case E_INJECTION:
				C_send(*mode);
				while(1){
					//app(mode);
					printf("注射開始時に10(E_START)を入力してください。\n");
					scanf("%d",mode);
					if( *mode == E_START  || *mode == E_ERROR) break;
				}
				C_send(*mode);
				while(1){
					printf("注射が終わったら6(LINE_BAKE)を入力してください。\n");
					printf("音楽・声かけ\n");
					scanf("%d",mode);
					if( *mode == E_LINE_BAKE  || *mode == E_ERROR) break;
				}
				break;
				
			case E_LINE_BAKE:
					C_send(*mode);
					//app(mode);
					printf("降車してください。7(QUIT)を入力してください。またね。\n");
					scanf("%d",mode);
					if( *mode == E_QUIT  || *mode == E_ERROR) break;
				

			case E_QUIT:
				pthread_exit(0);
				
			case E_ERROR:
				C_send(*mode);
				_exit(0);

			default:
				pthread_exit(0);
		}
	}

	arduino_close();
    pthread_exit(0);
}
