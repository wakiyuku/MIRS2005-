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
	//�O���[�X�P�[���ŉ摜�ǂݍ���
	//Mat src = imread("D:\\download\\images\\line2.png",IMREAD_GRAYSCALE), dst;
	Mat src, dst;
	while (1) {

		Capture(&src);
		//�摜�T�C�Y�̎擾
		width = src.cols;
		height = src.rows;
		//�O���[�X�P�[����
		cvtColor(src, src, CV_RGB2GRAY);
		//��l������
		threshold(src, dst, 128, 255, THRESH_BINARY);
		//�������]
		bitwise_not(dst, dst);
		//�摜�̔����Ƃ���̗֊s�x�N�g���̎擾
		//imshow("dst",dst);
		//waitKey(10);
		findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		Vec4f output;
		//���C���p�x�̌��o
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
		//������摜�̕\��
		imshow("output", dst);
		waitKey(10);
		//��l���摜�̔��s�N�Z���̐�
		int whitepixcel = countNonZero(dst);
		//���E�̃s�N�Z�������ׂďo��
		cout << "count\n";
		print(RightTrim(dst, height, width));
		print(LeftTrim(dst, height, width));
	}
	return 0;
}

//�摜�̃g���~���O�����āA���F�s�N�Z������Ԃ��B
int Trim(Mat dst,int x, int y, int width, int height, String winName) {
	dst = Mat(dst, Rect(x, y, width, height));
	//imshow(winName, dst);
	return countNonZero(dst);
}
//�����g���~���O���s�N�Z������Ԃ�
int LeftTrim(Mat dst ,int height, int width) {
	int x, y, trimHeight, trimWidth;
	String winName = "left";
	x = 0;
	y = height / 2 - 20;
	trimHeight = 40;
	trimWidth = width / 2;
	return Trim(dst, x, y, trimWidth, trimHeight, winName);
}
//�E���g���~���O���s�N�Z������Ԃ��B
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
	cv::VideoCapture cap(1);//�f�o�C�X�̃I�[�v��
	//cap.open(0);//�������ł��ǂ��D

	if (!cap.isOpened())//�J�����f�o�C�X������ɃI�[�v���������m�F�D
	{
		//�ǂݍ��݂Ɏ��s�����Ƃ��̏���
		return -1;
	}
	cap.read(*frame);
	//imshow("test", *frame);
	//waitKey(10);
}


