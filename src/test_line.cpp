#include "stdio.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include "arduino.h"
#include "request.h"
#define PGAIN 1f
#define DGAIN 1f
using namespace std;
using namespace cv;

int Trim(Mat,int, int, int, int);
int LeftTrim(Mat, int, int);
int RightTrim(Mat, int, int);
int Capture(Mat*);
void print(int);
float Rasio(float , float);
void GetPGAIN(int* plspeed, int* prspeed, int rwhitepix, int lwhitepix ,int speed,int base_speed);
void GetDGAIN(int* dlspeed, int* drspeed, int* l_pix, int* r_pix, int rwhitepix , int lwhitepix);
VideoCapture cap(0);//デバイスのオープン
enum
{
	E_RIGHT,
	E_LEFT,
};

int main()
{
	if(arduino_open() != 0) return -1;
	vector< vector<Point> > contours;
	Mat src, dst;
	Vec4f output;
	int width, height;
	//P制御用
	int speed, base_speed,PRspeed,PLspeed;
	speed = 100;
	base_speed = 50;
	PRspeed = PLspeed = 0;
	//D制御用
	int r_pix[1];
	int l_pix[1];
	int DRspeed, DLspeed;
	r_pix[1]=l_pix[1]=DRspeed=DLspeed=0;
	//制御後速度
	int l_speed, r_speed;
	l_speed = 0;
	r_speed = 0;
	while (1) {
		Capture(&src);//画像の撮影
		width = src.cols;//画像サイズの取得
		height = src.rows;
		cvtColor(src, src, CV_RGB2GRAY);//グレースケール化
		threshold(src, dst, 128, 255, THRESH_BINARY);//二値化処理
		bitwise_not(dst, dst);//白黒反転
		/*findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//画像の白いところの輪郭ベクトルの取得
		//ライン角度の検出
		if (!contours[0].empty()) {
			fitLine(contours[0], output, DIST_L2, 0, 0.01, 0.01);	
		}
		int righty, lefty;
		righty = (height - output.val[2])*output.val[1] / output.val[0] + output.val[3];
		lefty = (-output.val[2] * output.val[1] / output.val[0]) + output.val[3];
		line(dst, Point(height - 1, righty), Point(0, lefty), (255,0,0), 8);*/
		imshow("output", dst);//処理後画像の表示
		waitKey(1);
		//左右のピクセル数を比べて出力
		cout << "count\n";
		int rwhitepix = RightTrim(dst, height, width);
		int lwhitepix = LeftTrim(dst, height, width);
		print(rwhitepix);
		print(lwhitepix);
		//P制御
		GetPGAIN(&PLspeed, &PRspeed, rwhitepix, lwhitepix ,speed,base_speed);
		//D制御
		GetDGAIN(&DLspeed, &DRspeed, l_pix, r_pix, rwhitepix, lwhitepix);
		l_speed = PLspeed+DLspeed;
		r_speed = PRspeed+DRspeed;
		printf("\nPL\n%d\n",DLspeed);
		printf("PR\n%d\n",DRspeed);
		request_set_runmode(CRV, l_speed, r_speed);
		waitKey(10);
		/*
		pixRasio = Rasio(rwhitepix, lwhitepix);
		printf("rasio=");
		printf("%f",pixRasio);
		if(rwhitepix > lwhitepix){
			request_set_runmode(CRV, speed*pixRasio+base_speed, speed*(1-pixRasio)+base_speed);
			waitKey(10);
			printf("right\n");
		}else{
			request_set_runmode(CRV,  speed*(1-pixRasio)+base_speed, speed*pixRasio+base_speed);
			waitKey(10);
			printf("left\n");
		}*/
	}
	return 0;
}

//画像のトリミングをして、白色ピクセル数を返す。
int Trim(Mat dst,int x, int y, int width, int height, String winName) {
	dst = Mat(dst, Rect(x, y, width, height));
	//imshow(winName, dst);
	return countNonZero(dst);
}
//左側トリミングしピクセル数を返す
int LeftTrim(Mat dst ,int height, int width) {
	int x, y, trimHeight, trimWidth;
	String winName = "left";
	x = 0;
	y = height / 2 - 20;
	trimHeight = 40;
	trimWidth = width / 2;
	return Trim(dst, x, y, trimWidth, trimHeight, winName);
}
//右側トリミングしピクセル数を返す。
int RightTrim(Mat dst, int height, int width) {
	int x, y, trimHeight, trimWidth;
	String winName = "right";
	x = width/2;
	y = height / 2 - 20;
	trimHeight = 40;
	trimWidth = width / 2;
	return Trim(dst, x, y, trimWidth, trimHeight, winName);
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

void GetPGAIN(int* plspeed, int* prspeed, int rwhitepix, int lwhitepix ,int speed,int base_speed){
	float pixRasio = Rasio(rwhitepix, lwhitepix);
		printf("rasio=");
		printf("%f",pixRasio);

	if(rwhitepix > lwhitepix){
			*plspeed = speed*pixRasio+base_speed;
			*prspeed = speed*(1-pixRasio)+base_speed;
		}else{
			*plspeed = speed*(1-pixRasio)+base_speed;
			*prspeed = speed*pixRasio+base_speed;
		}
}

void GetDGAIN(int* dlspeed, int* drspeed,int* l_pix, int* r_pix, int rwhitepix , int lwhitepix){
	l_pix[1]=l_pix[0];
	r_pix[1]=r_pix[0];
	l_pix[0]=lwhitepix;
	r_pix[0]=rwhitepix;
	printf("l_pix=%d\n",lpix[0]);
	if(rwhitepix > lwhitepix){
			*dlspeed = r_pix[1]-r_pix[0];
			*drspeed = l_pix[1]-l_pix[0];
		}else{
			*dlspeed = l_pix[1]-l_pix[0];
			*drspeed = r_pix[1]-r_pix[0];
		}
}
