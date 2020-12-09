#include "stdio.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include "arduino.h"
#include "request.h"
#define PGAIN 1f
#define DGAIN 0.002f
#define IGAIN 0.0001f
#define B_MAX 100
#define B_MIN 0
#define G_MAX 100
#define G_MIN 0
#define R_MAX 255
#define R_MIN 100
using namespace std;
using namespace cv;

int Trim(Mat,int, int, int, int);
int LeftTrim(Mat, int, int);
int RightTrim(Mat, int, int);
int Capture(Mat*);
void print(int);
float Rasio(float , float);
void GetPGAIN(int* plspeed, int* prspeed,int lwhitepix ,int rwhitepix,int speed,int base_speed);
void GetDGAIN(int* dlspeed, int* drspeed,int* past_l_pix, int* past_r_pix, int lwhitepix, int rwhitepix);
void GetIGAIN(int* ILspeed,int* IRspeed,int*accum,int lwhitepix,int rwhitepix);
int CheckColor(Mat rbg, int width, int height);
int CheckColor(Mat rbg, int width, int height);

VideoCapture cap(0);//デバイスのオープン

int main()
{
	if(arduino_open() != 0) return -1;
	vector<vector<Point>> contours;
	Mat rbg, gray,bin;
	Vec4f output;
	int width, height;
	//P制御用
	int speed, base_speed,PRspeed,PLspeed;
	speed = 100;
	base_speed = 50;
	PRspeed = PLspeed = 0;
	//D制御用
	int past_r_pix,past_l_pix,DRspeed, DLspeed;
	past_l_pix=past_r_pix=DRspeed=DLspeed=0;
	//I制御用
	int accum,IRspeed,ILspeed;
	accum = IRspeed = ILspeed = 0;
	//制御後速度
	int l_speed, r_speed;
	l_speed = 0;
	r_speed = 0;

	while (1) {
		Capture(&rbg);//画像の撮影
		width = rbg.cols;//画像サイズの取得
		height = rbg.rows;
		cvtColor(rbg, gray, CV_RGB2GRAY);//グレースケール化
		threshold(gray, bin,100, 255, THRESH_BINARY);//二値化処理
		bitwise_not(bin, bin);//白黒反転
		imshow("output", bin);//処理後画像の表示
		//左右のピクセル数を比べて出力
		cout << "count\n";
		int rwhitepix = RightTrim(bin, height, width);
		int lwhitepix = LeftTrim(bin, height, width);
		print(rwhitepix);
		print(lwhitepix);
		//P制御
		GetPGAIN(&PLspeed, &PRspeed, lwhitepix, rwhitepix,speed,base_speed);
		//D制御
		GetDGAIN(&DLspeed, &DRspeed, &past_l_pix, &past_r_pix, lwhitepix, rwhitepix);
		//I制御
		GetIGAIN(&ILspeed,&IRspeed,&accum,lwhitepix, rwhitepix);
		//走行スピード
		l_speed = PLspeed+DGAIN*DLspeed+IGAIN*ILspeed;
		r_speed = PRspeed+DGAIN*DRspeed+IGAIN*IRspeed;
		printf("PL\n%d\n",DLspeed);
		printf("PR\n%d\n\n\n",DRspeed);
		//走行スピード設定
		printf("redpix=%d\n",CheckColor(rbg, width, height));
		if(CheckColor(rbg, width, height)>3000){
			request_set_runmode(STP, 0, 0);
			printf("STOP!!!!\n");
			break;
		}
		waitKey(1);
		request_set_runmode(CRV, l_speed, r_speed);
	}
	return 0;
}

//画像のトリミングをして、白色ピクセル数を返す。
int Trim(Mat bin,int x, int y, int width, int height, String winName) {
	bin = Mat(bin, Rect(x, y, width, height));
	//imshow(winName, gray);
	return countNonZero(bin);
}
//左側トリミングしピクセル数を返す
int LeftTrim(Mat bin ,int height, int width) {
	int x, y, trimHeight, trimWidth;
	String winName = "left";
	x = 0;
	y = height / 2 - 20;
	trimHeight = 40;
	trimWidth = width / 2;
	return Trim(bin, x, y, trimWidth, trimHeight, winName);
}
//右側トリミングしピクセル数を返す。
int RightTrim(Mat bin, int height, int width) {
	int x, y, trimHeight, trimWidth;
	String winName = "right";
	x = width/2;
	y = height / 2 - 20;
	trimHeight = 40;
	trimWidth = width / 2;
	return Trim(bin, x, y, trimWidth, trimHeight, winName);
}

int CheckColor(Mat rbg, int width, int height) {
	int x, y, trimHeight, trimWidth;
	x = width / 3;
	y = height / 2 - 20;
	trimHeight = 40;
	trimWidth = width / 3;
	rbg = Mat(rbg, Rect(x, y, trimWidth, trimHeight));
	Scalar s_min = Scalar(B_MIN, G_MIN, R_MIN);
	Scalar s_max = Scalar(B_MAX, G_MAX, R_MAX);
	inRange(rbg, s_min, s_max, rbg);
	imshow("mask", rbg);
	return countNonZero(rbg);
}


void print(int i) {
	cout << i;
	cout << "\n";
}

int Capture(Mat* frame)
{
	if (!cap.isOpened())//カメラデバイスが正常にオープンしたか確認．
	{
		return -1;//読み込みに失敗したときの処理
	}
	cap.read(*frame);
	return 0;
}

float Rasio(float r, float l){
	if(r>l){
		return r/(r+l);
	}else{
		return l/(r+l);
	}
}

void GetPGAIN(int* plspeed, int* prspeed,int lwhitepix ,int rwhitepix,int speed,int base_speed){
	float pixRasio = Rasio(rwhitepix, lwhitepix);
		printf("rasio=");
		printf("%f\n",pixRasio);

	if(rwhitepix > lwhitepix){
			*plspeed = speed*pixRasio+base_speed;
			*prspeed = speed*(1-pixRasio)+base_speed;
		}else{
			*plspeed = speed*(1-pixRasio)+base_speed;
			*prspeed = speed*pixRasio+base_speed;
		}
}

void GetDGAIN(int* dlspeed, int* drspeed,int* past_l_pix, int* past_r_pix, int lwhitepix, int rwhitepix){
	printf("%d,%d,%d,%d\n",*past_l_pix,*past_r_pix,lwhitepix,lwhitepix);
	if(rwhitepix > lwhitepix){
			*dlspeed = lwhitepix-*past_l_pix;
			*drspeed = rwhitepix-*past_r_pix;
		}else{
			*dlspeed = rwhitepix-*past_r_pix;
			*drspeed = lwhitepix-*past_l_pix;
		}
		*past_l_pix=lwhitepix;
		*past_r_pix=rwhitepix;
}

void GetIGAIN(int* ILspeed,int* IRspeed,int*accum,int lwhitepix,int rwhitepix){
	*accum+=(lwhitepix-rwhitepix)/1000;
	if(accum==0){
		*ILspeed = 0;
		*IRspeed = 0;
	}else{
		*ILspeed = -*accum;
		*IRspeed = *accum;
	}
	printf("%d,%d,%d\n",*accum,*ILspeed,*IRspeed);
}


