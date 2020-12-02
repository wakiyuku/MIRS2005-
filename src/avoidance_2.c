#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "io.h"
#include "uss.h"
#include "arduino.h"
#include "request.h"

int GetRandom(int min,int max);

int main(){
    float a = 12;
    float b = 0;
    float x,lenght;
    int speed,mode=1,dist,doti;
    long uss_l, uss_r;
	  int flag_l = 0, flag_r = 0;
    int sw_f, sw_l, sw_r;
    run_state_t state;
    if(arduino_open() != 0) return -1;

	if(io_open() != 0) return -1;

	//if(uss_open_l() != 0) return -1;
	//if(uss_open_r() != 0) return -1;
	if(uss_open_l() != 0){
		printf(" left uss open faild !!\n");
		flag_l = -1;
	}
	if(uss_open_r() != 0){
		printf(" right uss open faild !!\n");
		flag_r = -1;
	}
	if( flag_l == -1 && flag_r == -1) return -1;

    while(1){

        switch(mode){
            case 1 :
                speed=30;
                dist=290;
                request_set_runmode(STR, speed, dist);
		while(1){
		      request_get_runmode(&state, &speed, &dist);
		      if( state == STP ) break;
		      
                      uss_l = uss_get_l();
                      uss_r = uss_get_r();
                      io_get_sw(&sw_f, &sw_l, &sw_r);
                      printf("uss_l = %6ld, uss_r = %6ld ,switch_f = %d\n", uss_l, uss_r, sw_f);
		      usleep(10 * 1000);
		      
		      if((uss_l - uss_r) > 150 || (uss_r - uss_l) > 150){
			request_set_runmode(STP, 0, 0);
			mode=2;
			break;
		      }	
		      
/*		      if(uss_l > 280 || uss_r > 280){
			request_set_runmode(STP, 0, 0);
			mode=2;
			break;
		      }						
*/		      
		      if(uss_l >= uss_r){
			lenght = (float)uss_r;
		      }else{
			lenght = (float)uss_l;
		      }
		      
		      if (uss_r < 0 || uss_l < 0){
			request_set_runmode(STP, 0, 0);
			mode=2;
			break;
		      }
		      
			if(uss_l == uss_r){
			  x = 3.14/2;
			  lenght = (float)uss_r;
			} else if(uss_r < uss_l){
			  x = atan(a/((float)uss_l - (float)uss_r));
			  lenght = (float)uss_r*sin(x)-b;
			} else {
			 x = atan(a/((float)uss_r - (float)uss_l));
			 lenght = (float)uss_l*sin(x)-b;
			}

		      printf("angle = %.2f\n", x);
		      printf("lenght = %.2f\n", lenght);
		      
		      if(x < 3.14/4){
                          request_set_runmode(STP, 0, 0);
                          mode=2;
                          break;
                          }
		      
		      if(15 < lenght && lenght < 35){
                          request_set_runmode(STP, 0, 0);
                          mode=2;
                          break;
                          }

  /*                    if(sw_f == 0){
                          request_set_runmode(STP, 0, 0);
                          mode=2;
                          break;
                          }
  */
			                }

            case 2 :
                usleep(1000 * 1000);
                srand((unsigned int)time(NULL));
                speed=40;
                doti=GetRandom(1,2);
                if(doti==1){
                  dist=GetRandom(90,180);
                  request_set_runmode(ROT, speed, dist);
                }
                if(doti==2){
                  dist=GetRandom(-180,-90);
                  request_set_runmode(ROT, speed, dist);
                }
			          while(1){
				              request_get_runmode(&state, &speed, &dist);
				              if( state == STP ) break;
			          }
                mode=1;
                break;
        }
    }
}
int GetRandom(int min,int max){
    return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));}
