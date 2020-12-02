#include "pch.h"
#include <opencv2/opencv.hpp>
#include <vector>
#pragma comment(lib, "opencv_world343d.lib")
using namespace std;
using namespace cv;

int Trim(Mat,int, int, int, int);
int LeftTrim(Mat, int, int);
int RightTrim(Mat, int, int);
int Capture(Mat*);
void print(int);
int main()
{
	vector< vector<Point> > contours;
	int width, height;
	//グレースケールで画像読み込み
	//Mat src = imread("D:\\download\\images\\line2.png",IMREAD_GRAYSCALE), dst;
	Mat src, dst;
	while (1) {

		Capture(&src);
		//画像サイズの取得
		width = src.cols;
		height = src.rows;
		//グレースケール化
		cvtColor(src, src, CV_RGB2GRAY);
		//二値化処理
		threshold(src, dst, 128, 255, THRESH_BINARY);
		//白黒反転
		bitwise_not(dst, dst);
		//画像の白いところの輪郭ベクトルの取得
		//imshow("dst",dst);
		//waitKey(10);
		findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		Vec4f output;
		//ライン角度の検出
		if (!contours[0].empty()) {
			fitLine(contours[0], output, DIST_L2, 0, 0.01, 0.01);	
		}
		/*for (auto a : contours) {
			cout << a << endl;
		}*/
		cout << "vector\n";
		for (int i = 0; i < 4; i++) {
			cout << output[i];
			cout << "\n";
		}

		cout << width;
		cout << "\n";
		cout << height;
		cout << "\n";

		int righty, lefty;
		righty = (height - output.val[2])*output.val[1] / output.val[0] + output.val[3];
		lefty = (-output.val[2] * output.val[1] / output.val[0]) + output.val[3];
		line(dst, Point(height - 1, righty), Point(0, lefty), (255,0,0), 8);
		//処理後画像の表示
		imshow("output", dst);
		waitKey(10);
		//二値化画像の白ピクセルの数
		int whitepixcel = countNonZero(dst);
		//左右のピクセル数を比べて出力
		cout << "count\n";
		print(RightTrim(dst, height, width));
		print(LeftTrim(dst, height, width));
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
	cv::VideoCapture cap(1);//デバイスのオープン
	//cap.open(0);//こっちでも良い．

	if (!cap.isOpened())//カメラデバイスが正常にオープンしたか確認．
	{
		//読み込みに失敗したときの処理
		return -1;
	}
	cap.read(*frame);
	//imshow("test", *frame);
	//waitKey(10);
}


