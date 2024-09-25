#include "DB_Area_Calc.h"
#include "ui_DB_Area_Calc.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>

#include <QTextCodec>
#include <afx.h>
#include<fstream> 

using namespace std;
using namespace cv;

int WIDTH1 = 4208, HEIGHT1 = 3120;
float offset = 1.0;

typedef struct {
	int x, y;
	float tilt, edge;
}Corner_Center;

typedef struct {
	int x1, y1, x2, y2, x_diff, y_diff;
	float tilt, edge;
}Rect_Vec;
Rect_Vec rectVec[4];

typedef struct {
	int valid_Point = 0;	
	float theta, rho,v;
	Point pt1, pt2;
}Line_Data;
Line_Data Line_All[200], Line_Valid[100], Line_Rect[4];

unsigned char bianryTh1 = 50, bianryTh2 = 100;
unsigned int Corner_quality = 5, Coner_block = 7, grey_Level[256], BG_Level[256];
unsigned int min_BG = 50, max_black = 15, dia, image_Threshold = 120, image_Total_Pixel=0;
int Min_X = 99999999, Max_X = 0, Min_Y = 99999999, Max_Y = 0, Rect_Dia, mRoi_cnt=0;

Vector <Point> Edge_black;

Corner_Center Rect4[4] = { 0 }, vec[4] = { 0 }, Rect_Center = { 0 };

ofstream fout(".\\DB_Area_Calc_Data.txt");

float Min(int x, int y) {
	return x < y ? x : y;
}

DB_Area_Calc::DB_Area_Calc(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DB_Area_Calc)
{
	ui->setupUi(this);

	TCHAR lpTexts[9];
	GetPrivateProfileString(TEXT("Default_Setting"), TEXT("Offset"), TEXT("1.0"), lpTexts, 8, TEXT(".\\Setting.ini"));
	offset = atof(CT2A(lpTexts));
	string s = CT2A(lpTexts).m_szBuffer;
	ui->offset->setText(QString::fromStdString(s));
}

DB_Area_Calc::~DB_Area_Calc()
{
	fout << flush;
	fout.close();
	delete ui;
}
 

void DB_Area_Calc::load_Panel_Value() {

	memset(Rect4, 0, sizeof(Rect4));
	memset(rectVec, 0, sizeof(rectVec));
	memset(grey_Level, 0, sizeof(grey_Level));

	Corner_quality = ui->Corner_quality->document()->toPlainText().toInt();
	Coner_block = ui->Coner_block->document()->toPlainText().toInt();
	bianryTh1 = ui->bianryTh1->document()->toPlainText().toInt();
	//string tri = string((const char *)ui->tri_Block_Size->document()->toPlainText().toLocal8Bit());
	//tri_size = atof(tri.c_str());

	TCHAR lpTexts[9];
	GetPrivateProfileString(TEXT("Default_Setting"), TEXT("Offset"), TEXT("1.0"), lpTexts, 8, TEXT(".\\Setting.ini"));
	offset = atof(CT2A(lpTexts));
}


int DB_Area_Calc::Rect_Correct() {

	float limit = 13, p=-100000000, min =-100000000;
	int result = 0 ,w =-1;

	for (int i = 0; i < 4; i++) {
			
		int a = (i + 1)%4;
		int b = (i - 1);
		if (b < 0)b = 3;

		rectVec[i].x1 = Rect4[i].x - Rect4[a].x;
		if (rectVec[i].x1== 0)rectVec[i].x1++;
		rectVec[i].y1 = Rect4[i].y - Rect4[a].y;

		rectVec[i].x2 = Rect4[i].x - Rect4[b].x;
		rectVec[i].y2 = Rect4[i].y - Rect4[b].y;
		if(rectVec[i].x2 == 0)rectVec[i].x2++;

		if (abs(rectVec[i].x1) < limit)
			limit = abs(rectVec[i].x1);

		if (abs(rectVec[i].x2) < limit)
			limit = abs(rectVec[i].x1);

		rectVec[i].edge = (float)rectVec[i].y1*rectVec[i].y2 / rectVec[i].x1 / rectVec[i].x2;
	}

	if (limit < 2)limit = 2;
	if (limit > 13)limit = 13;
	limit = -1 * (22.0 / limit);


	float RA[4] = { 0 };

	bool ok = true;

	for (int i = 0; i < 4; i++) {
		if (rectVec[i].edge < limit || rectVec[i].edge>0)
			ok = false;
	
		if (rectVec[i].edge < -1)
			p = rectVec[i].edge;

		if (rectVec[i].edge > -1 && rectVec[i].edge < 0)
			p = 1 / rectVec[i].edge;

		if (min<p){
			min = p;
			w = i;
		}
	}

	if (!ok) {

		if (rectVec[w].edge>limit && rectVec[w].edge < 0){
			int c = (w + 2) % 4;
			Rect4[c].x = Rect4[w].x - (rectVec[w].x1 + rectVec[w].x2);
			Rect4[c].y = Rect4[w].y - (rectVec[w].y1 + rectVec[w].y2);
			circle(src1, Point(Rect4[c].x, Rect4[c].y), 10, Scalar(10, 255, 10), 2, 4, 0);// Marking the Corner Point	
		}
		else {
			return 1;
		}
	}

	Rect_Center.x = Rect_Center.y = 0;
	for (int i = 0; i < 4; i++) {

		int a = (i + 1) % 4;
		rectVec[i].x1 = Rect4[i].x - Rect4[a].x;
		if (rectVec[i].x1 == 0)rectVec[i].x1++;
		rectVec[i].y1 = Rect4[i].y - Rect4[a].y;
		rectVec[i].tilt = (float)rectVec[i].y1 / rectVec[i].x1;

		Rect_Center.x += Rect4[i].x;
		Rect_Center.y += Rect4[i].y;

	//	circle(dst, Point(Rect4[i].x, Rect4[i].y), 10, Scalar(10, 255, 10), 2, 4, 0);// Marking the Corner Point
		
		string s = to_string(Rect4[i].x) + " , " + to_string(Rect4[i].y) + "\n";
		ui->log->insertPlainText(s.c_str());
	}
	//imwrite("rect.bmp", dst);

	Rect_Center.x /= 4;
	Rect_Center.y /= 4;
	int x = Rect4[0].x - Rect4[2].x;
	int y = Rect4[0].y - Rect4[2].y;
	Rect_Center.edge = sqrt(x*x+y*y);

	return 0;
}


int DB_Area_Calc::remove_Edge() {

	int cnt = 0;
	for (int i = 0; i < bin.rows; i++) {
		uchar* inData = bin.ptr<uchar>(i);
		for (int j = 0; j < bin.cols; j++) {

			float a = (Rect4[1].x - Rect4[0].x)*(i - Rect4[0].y) - (Rect4[1].y - Rect4[0].y)*(j - Rect4[0].x);
			float b = (Rect4[2].x - Rect4[1].x)*(i - Rect4[1].y) - (Rect4[2].y - Rect4[1].y)*(j - Rect4[1].x);
			float c= (Rect4[0].x - Rect4[2].x)*(i - Rect4[2].y) - (Rect4[0].y - Rect4[2].y)*(j - Rect4[2].x);
			if ((a > 0 && b > 0 && c > 0) || (a < 0 && b < 0 && c < 0)) {
				cnt++;
				if (inData[j] <24)inData[j] = 0;
			}
			else{
				inData[j] = 230;
			}
		}
	}
	return cnt;
}


int DB_Area_Calc::non_Bond_Fix() {

	Rect ccomp, ccomp1;
	for (Vector <Point>::iterator it = Edge_black.begin(); it != Edge_black.end(); ++it)
	{
		if (bin.at<uchar>((*it).y, (*it).x) == 0){
			fout << *it << endl;
			floodFill(dst, *it, Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));
			floodFill(bin, *it, Scalar(255, 255, 255), &ccomp1, Scalar(1, 1, 1), Scalar(1, 1, 1));
		}
	}
	return 0;
}


int DB_Area_Calc::edge_Fix() {

	Edge_black.clear();

	int x = Rect4[0].x, y = Rect4[0].y,x1=0,y1=0;
	Rect ccomp;
	int n = 0;

	while (x != x1||y != y1) {
		x1 = x, y1 = y; n++;
		if (n > (Rect4[1].x - Rect4[0].x) * 10)
			break;
		bool RT = bin.at<uchar>(y - 1, x + 1)== 255;
		bool R = bin.at<uchar>(y, x + 1) == 255;
		bool RD = bin.at<uchar>(y + 1, x + 1)== 255;
		bool D = bin.at<uchar>(y + 1, x)== 255;
		bool LD = bin.at<uchar>(y + 1, x - 1)== 255;
		bool L = bin.at<uchar>(y, x - 1)== 255;
		bool LT = bin.at<uchar>(y - 1, x - 1)== 255;
		bool T = bin.at<uchar>(y - 1, x)== 255;

		if (R || RT || RD) {
			x++;
			if (!R&&RT)y--;
			if (!R&&RD)y++;
		}
		else if (D || LD || RD) {
			y++;
			if (!D&&LD)x--;
			if (!D&&RD)x++;
		}
		else if (L || LT || LD) {
			x--;
			if (!L&&LT)y--;
			if (!L&&LD)y++;
		}
		else if (T || LT || RT) {
			y--;
			if (!T&&LT)x--;
			if (!T&&RT)x++;
		}	

		int i = y, j = x, offset_x = 0,offset_y=0;

		if (bin.at<uchar>(Rect4[1].y, Rect4[1].x)== 255)
			offset_y = 1;
		else if(bin.at<uchar>(Rect4[2].y, Rect4[2].x)== 255 )
			offset_x = -1;
		else if (bin.at<uchar>(Rect4[3].y, Rect4[3].x)== 255)
			offset_y = -1;
		else 
			offset_x = 1;

		int x2 = offset_x * Rect_Center.edge*0.03, y2 = offset_y * Rect_Center.edge*0.03;
		if (bin.at<uchar>(y+y2, x + x2) == 0 ) {
			Edge_black.push_back(Point(x + x2, y + y2));
			while (x2 != 0 || y2 != 0) {
				if (x2 > 0)x2--;
				else if(x2<0) x2++;
				if (y2 > 0)y2--;
				else if(y2<0) y2++;
				if(bin.at<uchar>(y + y2, x + x2) != 255)
					bin.at<uchar>(y + y2, x + x2) = 0;
			}	
		}
		bin.at<uchar>(y, x) = 250;	
	}

	return 0;
}

void DB_Area_Calc::on_pushButton_open_BMP_clicked()
{
	load_Panel_Value();
	ui->log->setText("");

	QString qs_name = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image File(*.bmp *.jpg *.jpeg *.png *.pbm *.pgm *.ppm)"));
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	name = code->fromUnicode(qs_name).data();

	Mat dstImage;//目标图
	Mat normImage;//归一化后的图
	Mat scaledImage;//线性变换后的八位无符号整型的图
	
	if (name.length() < 2) return;
	image = imread(name, 1);
	out_image = image.clone();
	src1 = image.clone();
	dstImage = image.clone();
	WIDTH1 = image.cols / 64;
	HEIGHT1 = image.rows / 64;
	int Cols = image.cols * 3;
	int ret = 0;
	dia = sqrt(image.cols*image.cols + image.rows*image.rows);


	cvtColor(src1, gray, CV_BGR2GRAY);
	blur(gray, gray, Size(5, 5));

	imwrite("grey.bmp", gray);
	threshold(gray, bin, 21, 100, THRESH_BINARY);

	imwrite("bin.bmp", bin);

	int sum = 0;

	for (int i = 0; i < bin.rows; i++) {
		uchar* inData = bin.ptr<uchar>(i);
		for (int j = 0; j < bin.cols; j++) {
			if (inData[j] == 0)
				sum++;
		}
	}

	//最外层的角点
	g_grayImage = bin.clone();
	vector<Point2f> corners;
	float quality = (float)Corner_quality / 10;
	goodFeaturesToTrack(g_grayImage, corners, 100, quality, 30, Mat(), Coner_block, false, 0.04);

	for (unsigned i = 0; i < corners.size(); i++) {
		circle(src1, corners[i], 10, Scalar(10, 10, 255), 2, 4, 0);// Marking the Corner Point
																   /*	string s = to_string(corners[i].x) + " , " + to_string(corners[i].y) + "\n";
																   ui->log->insertPlainText(s.c_str());*/

		float d = sqrt(corners[i].x*corners[i].x + corners[i].y*corners[i].y);
		if (Rect4[0].edge > d || Rect4[0].edge < 1) {
			Rect4[0].edge = d;
			Rect4[0].x = corners[i].x;
			Rect4[0].y = corners[i].y;
		}

		d = sqrt((image.cols - corners[i].x)*(image.cols - corners[i].x) + corners[i].y*corners[i].y);
		if (Rect4[1].edge > d || Rect4[1].edge < 1) {
			Rect4[1].edge = d;
			Rect4[1].x = corners[i].x;
			Rect4[1].y = corners[i].y;
		}

		d = sqrt((image.cols - corners[i].x)*(image.cols - corners[i].x) + (image.rows - corners[i].y)*(image.rows - corners[i].y));
		if (Rect4[2].edge > d || Rect4[2].edge < 1) {
			Rect4[2].edge = d;
			Rect4[2].x = corners[i].x;
			Rect4[2].y = corners[i].y;
		}

		d = sqrt(corners[i].x*corners[i].x + (image.rows - corners[i].y)*(image.rows - corners[i].y));
		if (Rect4[3].edge > d || Rect4[3].edge < 1) {
			Rect4[3].edge = d;
			Rect4[3].x = corners[i].x;
			Rect4[3].y = corners[i].y;
		}

	}

	//	ret+=Rect_Correct();
	imwrite("corner.bmp", src1);

	if (ret != 0) {
		string s = "Can not find Sensor Rect Outline! \n";
		ui->log->insertPlainText(s.c_str());
		return;
	}
	remove_Edge();

	cvtColor(bin, dst, CV_GRAY2BGR);

	imwrite("cutting.bmp", dst);
	Rect ccomp;
	uchar* inData = dst.ptr<uchar>(Rect4[0].y + 15);
	if (inData[3 * (Rect4[0].x + 15)]<10)
		floodFill(dst, Point(Rect4[0].x + 15, Rect4[0].y + 15), Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));
	if (inData[3 * (Rect4[1].x - 15)]<10)
		floodFill(dst, Point(Rect4[1].x - 15, Rect4[1].y + 15), Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));

	inData = dst.ptr<uchar>(Rect4[2].y - 15);
	if (inData[3 * (Rect4[2].x - 15)]<10)
		floodFill(dst, Point(Rect4[2].x - 15, Rect4[2].y - 15), Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));
	if (inData[3 * (Rect4[3].x + 15)]<10)
		floodFill(dst, Point(Rect4[3].x + 15, Rect4[3].y - 15), Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));

	int c = Rect_Center.edge / 8;
	inData = dst.ptr<uchar>(Rect_Center.y);
	for (int i = 0; i < c * 2; i += 10) {
		if (inData[3 * (Rect_Center.x - c + i)] == 100) {
			floodFill(dst, Point(Rect_Center.x - c + i, Rect_Center.y), Scalar(25, 235, 235), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));
			break;
		}
	}

	imwrite("dis.bmp", dst);

	non_Bond_Fix();
	imwrite("bin3.bmp", bin);

	int full_Size = image.cols * image.rows;
	int edge_Size = 0;
	int remainder_Size = 0;

	for (int i = 0; i < dst.rows; i++) {
		uchar* inData = dst.ptr<uchar>(i);
		for (int j = 0; j < Cols; j += 3) {
			if (inData[j] == 250)
				remainder_Size++;
			else if (inData[j] == 230)
				edge_Size++;
		}
	}

	float DB_rate = offset*(float)(full_Size - edge_Size - remainder_Size) / (full_Size - edge_Size);

	ui->log->insertPlainText("DB Coverage Rate: ");
	string s = to_string(DB_rate) + '\n';
	ui->log->insertPlainText(to_string(DB_rate).c_str());

	int x = 640, y = 480;
	if (image.cols * 3 > image.rows * 4)
		y = image.rows * 640 / image.cols;
	else x = image.cols * 480 / image.rows;

	cv::resize(dst, display_image, Size(x, y), 0, 0);
	imwrite("display.bmp", display_image);

	ui->label_show_image->setPixmap(QPixmap("./display.bmp"));
}

unsigned int DB_Area_Calc::get_Binaryztion_Level()
{
	for (int i = 0; i < gray.rows; i++) {
		uchar* inData = gray.ptr<uchar>(i);
		for (int j = 0; j < gray.cols; j++) {
			grey_Level[inData[j]]++;

		}
	}

	unsigned int max = 0, n = 0;
	for (int i = 0; i < 50; i++) {
		if (grey_Level[i] > max) {
			max = grey_Level[i];
			n = i;
		}		
	}

	return n;
}

unsigned int DB_Area_Calc::get_Image_Threshold()
{
	for (int i = 0; i < gray.rows; i++) {
		uchar* inData = gray.ptr<uchar>(i);
		for (int j = 0; j < gray.cols; j++) {
			grey_Level[inData[j]]++;

		}
	}

	unsigned int max = 0, n = 0,k=0; 
	unsigned int peak[255] = { 0 };
	for (int i = 254; i >50; i--) {
		if (grey_Level[i] > grey_Level[i - 1] + dia*0.01 && grey_Level[i] > grey_Level[i - 2] + dia*0.03 && peak[0] == 0) {
			peak[0] = i;

		}
		if(peak[0] != 0&& peak[1] == 0&& grey_Level[i]<0.3*grey_Level[peak[0]])
			if (grey_Level[i - 1] > grey_Level[i]+ dia*0.03 && grey_Level[i - 2] > dia*0.06 + grey_Level[i]) {
				peak[1] = i;
			}
	}
	int ret = peak[1] - 3;
	if (ret < 50)ret = 50;
	return ret;
}

unsigned int DB_Area_Calc::get_Image_Threshold_low()
{
	for (int i = 0; i < gray.rows; i++) {
		uchar* inData = gray.ptr<uchar>(i);
		for (int j = 0; j < gray.cols; j++) {
			grey_Level[inData[j]]++;
		}
	}

	unsigned int max = 0, n = 0, k = 0;
	unsigned int peak[255] = { 0 };
	for (int i = 0; i <100; i++) {
		if (grey_Level[i] > grey_Level[i + 1] + dia*0.01 && grey_Level[i] > grey_Level[i + 2] + dia*0.03 && peak[0] == 0) {
			peak[0] = i;
		}
		if (peak[0] != 0 && peak[1] == 0 && grey_Level[i]<0.3*grey_Level[peak[0]])
			if (grey_Level[i + 1] > grey_Level[i] + dia*0.03 && grey_Level[i + 2] > dia*0.06 + grey_Level[i]) {
				peak[1] = i;
			}
	}
	int ret = peak[1] - 3;
	if (ret < 32)ret = 32;
	return ret;
}

unsigned int DB_Area_Calc::get_Image_100_min_max()
{
	for (int i = 0; i < gray.rows; i++) {
		uchar* inData = gray.ptr<uchar>(i);
		for (int j = 0; j < gray.cols; j++) {
			grey_Level[inData[j]]++;
		}
	}

	unsigned int BVmax = 0, BVmin= 25600000, n = 0, k = 0;
	unsigned int peak[255] = { 0 };
	for (int i = 0; i <90; i++) {
		if (grey_Level[i] >BVmax) {
			BVmax = grey_Level[i];
			peak[0] = i;
		}
	}

	for (int i = peak[0]; i < 90; i++) {
		if (grey_Level[i] < BVmin) {
			BVmin = grey_Level[i];
			peak[1] = i;
		}
	}
	return peak[1];
}

unsigned int DB_Area_Calc::get_backGround_Level()
{
	memset(BG_Level, 0, sizeof(BG_Level));
	int sum = 0;
	uchar* inData = gray.ptr<uchar>(3);
	for (int j = 0; j < gray.cols; j++) {
		BG_Level[inData[j]]++;
		sum += inData[j];
	}
	inData = gray.ptr<uchar>(gray.rows-3);
	for (int j = 0; j < gray.cols; j++) {
		BG_Level[inData[j]]++;
		sum += inData[j];
	}

	unsigned int min = 255, n = 0;
	for (int i = 0; i < 255; i++) {
		if (BG_Level[i]>20&&BG_Level[i] < min) {
			min = BG_Level[i];
			n = i;
		}
	}

	int avg = sum / 2 / gray.cols;
	return avg;
}

Point get_intersection(Point p1, Point p2, Point q1, Point q2) {

	float A1 = p2.y - p1.y;
	float B1 = p1.x - p2.x;
	float C1 = p2.x*p1.y - p1.x*p2.y;
	float A2 = q2.y - q1.y;
	float B2 = q1.x - q2.x;
	float C2 = q2.x*q1.y - q1.x*q2.y;

	Point p;
	p.y = cvRound((C1*A2 - C2*A1) / (A1*B2 - A2*B1));
	p.x = cvRound((C2*B1 - C1*B2) / (A1*B2 - A2*B1));

	return p;
}


int DB_Area_Calc::getTH(int k, int offset) {

	int sum[9] = { 0 },n=0,a=k;
	int fx = offset;
	int fy = offset;

	if (k == 1 || k == 2) fx *= -1;
	if (k == 2 || k == 3) fy *= -1;

	for (int y = Rect4[k].y + fy - 2; y <= Rect4[k].y + fy + 2; y+=2) {
		uchar* inData = gray.ptr<uchar>(y);
		uchar* isBlack = bin1.ptr<uchar>(y);
		for (int x = Rect4[k].x + fx - 2; x <= Rect4[k].x + fx + 2; x += 2) {		
			if(isBlack[x] == 0)
				sum[n++] = inData[x];
		}
	}
	sort(sum, sum + n);
	int bv= sum[n / 2];
	//if (bv < 50)bv = 50;
	return bv;
}

void DB_Area_Calc::sensor_area_calc() {

	int Cols = image.cols * 3;
	int ret = 0, n = 0, TH = 0, Valid_TH = 60;
	if (image.cols < 600)Valid_TH = image.cols / 6;

	dia = sqrt(image.cols*image.cols + image.rows*image.rows);
	image_Total_Pixel = image.cols*image.rows;

	if (dia < 750)
		blur(src1, src1, Size(3, 3));
	else if (dia < 2000)
		blur(src1, src1, Size(5, 5));
	else
		blur(src1, src1, Size(7, 7));

	cvtColor(src1, gray, CV_BGR2GRAY);

	min_BG = get_backGround_Level();
	TH = get_Image_Threshold();
	//if (min_BG < 150)TH = 50;
	//else if (min_BG < 180)TH = 85;
	//else TH = 125;

	imwrite("grey.bmp", gray);
	if (ui->manual_BV->isChecked()) TH = bianryTh1;

	threshold(gray, bin, TH, 100, THRESH_BINARY);
	imwrite("bin1.bmp", bin);
	bin1 = bin.clone();

	Canny(bin, edge, 3, 9, 3);
	imwrite("edge.bmp ", edge);
	Line_Data max_Line;
	max_Line.valid_Point = 0;

	//【3】进行霍夫线变换
	vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
	HoughLines(edge, lines, 1, CV_PI / 180, Valid_TH*0.8, 0, 0);

	int TH2 = 30;
	while (lines.size()>300 && TH2<200) {
		HoughLines(edge, lines, 1, CV_PI / 180, Valid_TH + TH2, 0, 0);
		TH2 += 30;
	}

	//【4】依次在图中绘制出每条线段
	for (size_t i = 0; i < lines.size(); i++)
	{
		Line_All[i].rho = lines[i][0], Line_All[i].theta = lines[i][1];
		Point p;
		double a = cos(Line_All[i].theta), b = sin(Line_All[i].theta);
		double x0 = a*Line_All[i].rho, y0 = b*Line_All[i].rho;

		int s1 = cvRound((x0) / b);
		int s2 = cvRound((x0 - image.cols) / b);
		int last_x = 0, last_y = 0;;
		Line_All[i].valid_Point = 0;
		if (s1 > s2)swap(s1, s2);

		for (int k = s1; k < s2; k++) {
			p.x = cvRound(x0 + k * (-b));

			if (p.x == last_x)continue;
			if (p.x < 1)continue;
			if (p.x > image.cols - 2)continue;

			p.y = cvRound(y0 + k * (a));
			if (p.y < 1)continue;
			if (p.y > image.rows - 2)continue;

			if (bin.at<uchar>(p.y - 1, p.x) == 0 && bin.at<uchar>(p.y + 1, p.x) == 100) {
				bool continuity = false;
				if (bin.at<uchar>(p.y - 1, p.x - 1) == 0 && bin.at<uchar>(p.y + 1, p.x - 1) == 100)
					if (bin.at<uchar>(p.y - 1, p.x + 1) == 0 && bin.at<uchar>(p.y + 1, p.x + 1) == 100)
						Line_All[i].valid_Point++;
			}
			else if (bin.at<uchar>(p.y - 1, p.x) == 100 && bin.at<uchar>(p.y + 1, p.x) == 0) {
				if (bin.at<uchar>(p.y - 1, p.x - 1) == 100 && bin.at<uchar>(p.y + 1, p.x - 1) == 0)
					if (bin.at<uchar>(p.y - 1, p.x + 1) == 100 && bin.at<uchar>(p.y + 1, p.x + 1) == 0)
						Line_All[i].valid_Point++;
			}
			else if (bin.at<uchar>(p.y, p.x - 1) == 0 && bin.at<uchar>(p.y, p.x + 1) == 100) {
				if (bin.at<uchar>(p.y - 1, p.x - 1) == 0 && bin.at<uchar>(p.y - 1, p.x + 1) == 100)
					if (bin.at<uchar>(p.y + 1, p.x - 1) == 0 && bin.at<uchar>(p.y + 1, p.x + 1) == 100)
						Line_All[i].valid_Point++;
			}
			else if (bin.at<uchar>(p.y, p.x - 1) == 100 && bin.at<uchar>(p.y, p.x + 1) == 0) {
				if (bin.at<uchar>(p.y - 1, p.x - 1) == 100 && bin.at<uchar>(p.y - 1, p.x + 1) == 0)
					if (bin.at<uchar>(p.y + 1, p.x - 1) == 100 && bin.at<uchar>(p.y + 1, p.x + 1) == 0)
						Line_All[i].valid_Point++;
			}

			last_x = p.x;
		}

		s1 = cvRound((-y0) / a);
		s2 = cvRound((image.cols - y0) / a);
		if (s1 > s2)swap(s1, s2);

		for (int k = s1; k < s2; k++) {

			p.y = cvRound(y0 + k * (a));
			if (p.y == last_y)continue;
			if (p.y < 1)continue;
			if (p.y > image.rows - 2)continue;

			p.x = cvRound(x0 + k * (-b));
			if (p.x < 1)continue;
			if (p.x > image.cols - 2)continue;

			if (bin.at<uchar>(p.y - 1, p.x) + bin.at<uchar>(p.y + 1, p.x) == 100)
				Line_All[i].valid_Point++;
			else if (bin.at<uchar>(p.y, p.x - 1) + bin.at<uchar>(p.y, p.x + 1) == 100)
				Line_All[i].valid_Point++;

			last_y = p.y;
		}

		if (Line_All[i].valid_Point > Valid_TH) {
			Line_All[i].pt1.x = cvRound(x0 + 2000 * (-b));
			Line_All[i].pt1.y = cvRound(y0 + 2000 * (a));
			Line_All[i].pt2.x = cvRound(x0 - 2000 * (-b));
			Line_All[i].pt2.y = cvRound(y0 - 2000 * (a));
			line(image, Line_All[i].pt1, Line_All[i].pt2, Scalar(0, 0, 255), 1, CV_AA);
			Line_Valid[n++] = Line_All[i];
		}

		if (Line_All[i].valid_Point > max_Line.valid_Point) {
			max_Line = Line_All[i];
		}
	}
	Line_Data voilid_max;
	double a = cos(max_Line.theta), b = sin(max_Line.theta);
	double x0 = a*max_Line.rho, y0 = b*max_Line.rho;

	voilid_max.pt1.x = cvRound(x0 + 2000 * (-b));
	voilid_max.pt1.y = cvRound(y0 + 2000 * (a));
	voilid_max.pt2.x = cvRound(x0 - 2000 * (-b));
	voilid_max.pt2.y = cvRound(y0 - 2000 * (a));
	line(image, voilid_max.pt1, voilid_max.pt2, Scalar(255, 255, 55), 2, CV_AA);

	memset(Line_Rect, 0, sizeof(Line_Rect));
	Line_Rect[0] = max_Line;
	imwrite("line1.bmp", image);
	//find Rect 4 line
	for (int i = 0; i < n; i++)
		if (Line_Valid[i].valid_Point < Line_Rect[0].valid_Point) {

			if (Line_Valid[i].theta == Line_Rect[0].theta || (Line_Valid[i].theta > Line_Rect[0].theta *0.99&&Line_Valid[i].theta < Line_Rect[0].theta *1.01)) {
				if (Line_Valid[i].valid_Point > Line_Rect[2].valid_Point&&abs(Line_Valid[i].rho - Line_Rect[0].rho)>image.cols / 5)
					Line_Rect[2] = Line_Valid[i];
			}
			else {
				Line_Valid[i].v = abs(Line_Rect[0].theta - Line_Valid[i].theta);
				if (Line_Valid[i].v > CV_PI / 2 * 0.99&&Line_Valid[i].v < CV_PI / 2 * 1.01) {

					if (Line_Valid[i].valid_Point > Line_Rect[1].valid_Point) {
						if (Line_Valid[i].rho<Line_Rect[1].rho*0.95 || Line_Valid[i].rho>Line_Rect[1].rho*1.05)
							Line_Rect[3] = Line_Rect[1];
						Line_Rect[1] = Line_Valid[i];
					}
					else if (abs(Line_Valid[i].rho - Line_Rect[1].rho)>image.rows / 5) {

						if (Line_Valid[i].valid_Point > Line_Rect[3].valid_Point)
							Line_Rect[3] = Line_Valid[i];

						if (Line_Valid[i].valid_Point > Line_Rect[3].valid_Point*0.7)
							if (abs(Line_Valid[i].rho - Line_Rect[1].rho)>abs(Line_Valid[3].rho - Line_Rect[1].rho)*1.1)
								Line_Rect[3] = Line_Valid[i];
					}
				}
			}
		}

	// when Rect is not perfect , auto fix with the parallel line of the Rect
	for (int k = 2; k< 4; k++) {
		if (Line_Rect[k].valid_Point == 0) {
			for (int i = 0; i < n; i++)
				if (k == 2) {
					if (Line_Valid[i].theta == Line_Rect[0].theta || (Line_Valid[i].theta > Line_Rect[0].theta *0.97&&Line_Valid[i].theta < Line_Rect[0].theta *1.03)) {
						if (Line_Valid[i].valid_Point > Line_Rect[2].valid_Point)
							Line_Rect[2] = Line_Valid[i];
					}
				}
				else if (Line_Valid[i].valid_Point != Line_Rect[1].valid_Point) {
					Line_Valid[i].v = abs(Line_Rect[0].theta - Line_Valid[i].theta);
					if (Line_Valid[i].v > CV_PI / 2 * 0.97&&Line_Valid[i].v < CV_PI / 2 * 1.03)
						if (abs(Line_Valid[i].rho - Line_Rect[1].rho)>image.cols / 5) {

							if (Line_Valid[i].valid_Point > Line_Rect[3].valid_Point) {
								Line_Rect[k] = Line_Valid[i];
								if (abs(Line_Rect[1].theta - Line_Valid[i].theta) < 0.5)
									Line_Rect[k].theta = Line_Rect[1].theta;
								else
									Line_Rect[k].theta = CV_PI - Line_Rect[1].theta;

							}
							double a = cos(Line_Rect[k].theta), b = sin(Line_Rect[k].theta);
							double x0 = a*Line_Rect[k].rho, y0 = b*Line_Rect[k].rho;
							Line_Rect[k].pt1.x = cvRound(x0 + 2000 * (-b));
							Line_Rect[k].pt1.y = cvRound(y0 + 2000 * (a));
							Line_Rect[k].pt2.x = cvRound(x0 - 2000 * (-b));
							Line_Rect[k].pt2.y = cvRound(y0 - 2000 * (a));
						}
				}
		}
	}

	for (int k = 2; k< 4; k++)
		if (Line_Rect[k].valid_Point == 0) {
			string s = "Can not find Sensor Rect Outline! \n";
			ui->log->insertPlainText(s.c_str());
			return;
		}
	int diff = Line_Rect[1].rho - Line_Rect[3].rho;
	if (diff>-5 && diff<5) {
		string s = "Can not find Sensor Rect Outline! \n";
		ui->log->insertPlainText(s.c_str());
		return;
	}

	for (int i = 0; i < 4; i++) {

		int j = (i + 1) % 4;
		Point p = get_intersection(Line_Rect[i].pt1, Line_Rect[i].pt2, Line_Rect[j].pt1, Line_Rect[j].pt2);
		circle(src1, p, 10, Scalar(10, 10, 255), 2, 4, 0);

		float d = sqrt(p.x*p.x + p.y*p.y);
		if (Rect4[0].edge > d || Rect4[0].edge < 1) {
			Rect4[0].edge = d;
			Rect4[0].x = p.x;
			Rect4[0].y = p.y;
		}

		d = sqrt((image.cols - p.x)*(image.cols - p.x) + p.y*p.y);
		if (Rect4[1].edge > d || Rect4[1].edge < 1) {
			Rect4[1].edge = d;
			Rect4[1].x = p.x;
			Rect4[1].y = p.y;
		}

		d = sqrt((image.cols - p.x)*(image.cols - p.x) + (image.rows - p.y)*(image.rows - p.y));
		if (Rect4[2].edge > d || Rect4[2].edge < 1) {
			Rect4[2].edge = d;
			Rect4[2].x = p.x;
			Rect4[2].y = p.y;
		}

		d = sqrt(p.x*p.x + (image.rows - p.y)*(image.rows - p.y));
		if (Rect4[3].edge > d || Rect4[3].edge < 1) {
			Rect4[3].edge = d;
			Rect4[3].x = p.x;
			Rect4[3].y = p.y;
		}
	}

	line(image, Point(Rect4[0].x, Rect4[0].y), Point(Rect4[1].x, Rect4[1].y), Scalar(0, 255, 255), 2, CV_AA);
	line(image, Point(Rect4[1].x, Rect4[1].y), Point(Rect4[2].x, Rect4[2].y), Scalar(0, 255, 255), 2, CV_AA);
	line(image, Point(Rect4[2].x, Rect4[2].y), Point(Rect4[3].x, Rect4[3].y), Scalar(0, 255, 255), 2, CV_AA);
	line(image, Point(Rect4[3].x, Rect4[3].y), Point(Rect4[0].x, Rect4[0].y), Scalar(0, 255, 255), 2, CV_AA);

	//	ret += Rect_Correct();
	imwrite("line2.bmp", image);

	if (ret != 0) {
		string s = "Can not find Sensor Rect Outline! \n";
		ui->log->insertPlainText(s.c_str());
		return;
	}
	///////////////////////////////////////////////////////////////////////////
	bin = gray.clone();

	Min_X = 99999999, Max_X = 0, Min_Y = 99999999, Max_Y = 0;
	Rect_Center.x = Rect_Center.y = 0;

	for (int i = 0; i < 4; i++) {
		if (Rect4[i].x > Max_X) Max_X = Rect4[i].x;
		if (Rect4[i].x < Min_X) Min_X = Rect4[i].x;
		if (Rect4[i].y > Max_Y) Max_Y = Rect4[i].y;
		if (Rect4[i].y < Min_Y) Min_Y = Rect4[i].y;
		Rect_Center.x += Rect4[i].x;
		Rect_Center.y += Rect4[i].y;
	}
	Rect_Center.x /= 4;
	Rect_Center.y /= 4;

	Min_X -= 2;
	Min_Y -= 2;
	Max_X += 2;
	Max_Y += 2;

	int w = (Max_X - Min_X) / 2;
	int h = (Max_Y - Min_Y) / 2;

	Rect_Center.edge = sqrt(w*w + h*h);
	// when 4 corner brightness is diff, use seperate BV for 4 corner
	int F[4] = { 0,1,3,2 };
	n = 0;

	for (int y = 0; y < h * 2; y += h)
		for (int x = 0; x < w * 2; x += w) {
			Mat quart = bin(Rect(Min_X + x, Min_Y + y, w, h));
			int th = getTH(F[n++], h / 16);
			if (th < 50) {
				//	if(TH > 100)
				//		th = 32;
				//	else
				th += (70 - th)*0.2;
			}
			else if (th<80) {
				th += (th - 50)*0.2;
			}
			else {
				th = 50;
			}

			threshold(quart, quart, th, 100, THRESH_BINARY);
		}
	/////////////////////////// ////////////////////////////////////////////////////
	//	threshold(gray, bin, bianryTh1 - 20, bianryTh2 - 20, THRESH_BINARY);

	remove_Edge();

	line(bin, Point(Rect4[0].x, Rect4[0].y), Point(Rect4[1].x, Rect4[1].y), Scalar(255, 255, 255), 1, 8);
	line(bin, Point(Rect4[1].x, Rect4[1].y), Point(Rect4[2].x, Rect4[2].y), Scalar(255, 255, 255), 1, 8);
	line(bin, Point(Rect4[2].x, Rect4[2].y), Point(Rect4[3].x, Rect4[3].y), Scalar(255, 255, 255), 1, 8);
	line(bin, Point(Rect4[3].x, Rect4[3].y), Point(Rect4[0].x, Rect4[0].y), Scalar(255, 255, 255), 1, 8);
	imwrite("bin2.bmp", bin);

	edge_Fix();

	int c = Rect_Center.edge / 16;	Rect ccomp;
	uchar* inData = bin.ptr<uchar>(Rect_Center.y);
	for (int i = 0; i < c * 2; i += 10) {
		if (inData[(Rect_Center.x - c + i)] == 100) {
			floodFill(bin, Point(Rect_Center.x - c + i, Rect_Center.y), Scalar(150, 150, 150), &ccomp, Scalar(1, 1, 1), Scalar(1, 1, 1));
			break;
		}
	}

	for (int i = Min_Y; i < Max_Y; i++) {
		uchar* inData = bin.ptr<uchar>(i);
		for (int j = Min_X; j < Max_X; j++) {
			if (inData[j] == 100) {

				if (inData[j - 1] == 150) {
					floodFill(bin, Point(j, i), Scalar(150, 150, 150), &ccomp, Scalar(1, 1, 1), Scalar(1, 1, 1));
				}
				else {
					floodFill(bin, Point(j, i), Scalar(1, 1, 1), &ccomp, Scalar(1, 1, 1), Scalar(1, 1, 1));
				}
			}
		}
	}

	cvtColor(bin, dst, CV_GRAY2BGR);
	imwrite("cutting.bmp", dst);

	c = Rect_Center.edge / 16;
	inData = dst.ptr<uchar>(Rect_Center.y);
	for (int i = 0; i < c * 2; i += 10) {
		if (inData[3 * (Rect_Center.x - c + i)] == 150) {
			floodFill(dst, Point(Rect_Center.x - c + i, Rect_Center.y), Scalar(25, 235, 235), &ccomp, Scalar(1, 1, 1), Scalar(1, 1, 1));
			break;
		}
	}

	int fill_offset = 15;

	if (Rect_Center.edge>500)
		fill_offset = Rect_Center.edge / 36;

	inData = dst.ptr<uchar>(Rect4[0].y + fill_offset);
	if (inData[3 * (Rect4[0].x + fill_offset)]<10)
		floodFill(dst, Point(Rect4[0].x + fill_offset, Rect4[0].y + fill_offset), Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));

	if (inData[3 * (Rect4[0].x + fill_offset * 2)]<10)
		floodFill(dst, Point(Rect4[0].x + fill_offset * 2, Rect4[0].y + fill_offset), Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));

	if (inData[3 * (Rect4[1].x - fill_offset)]<10)
		floodFill(dst, Point(Rect4[1].x - fill_offset, Rect4[1].y + fill_offset), Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));

	if (inData[3 * (Rect4[1].x - fill_offset * 2)]<10)
		floodFill(dst, Point(Rect4[1].x - fill_offset * 2, Rect4[1].y + fill_offset), Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));

	inData = dst.ptr<uchar>(Rect4[2].y - fill_offset);
	if (inData[3 * (Rect4[2].x - fill_offset)]<10)
		floodFill(dst, Point(Rect4[2].x - fill_offset, Rect4[2].y - fill_offset), Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));

	if (inData[3 * (Rect4[2].x - fill_offset * 2)]<10)
		floodFill(dst, Point(Rect4[2].x - fill_offset * 2, Rect4[2].y - fill_offset), Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));

	if (inData[3 * (Rect4[3].x + fill_offset)]<10)
		floodFill(dst, Point(Rect4[3].x + fill_offset, Rect4[3].y - fill_offset), Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));

	if (inData[3 * (Rect4[3].x + fill_offset * 2)]<10)
		floodFill(dst, Point(Rect4[3].x + fill_offset * 2, Rect4[3].y - fill_offset), Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));


	line(bin, Point(Rect4[0].x, Rect4[0].y), Point(Rect4[1].x, Rect4[1].y), Scalar(255, 255, 255), 1, 8);
	line(bin, Point(Rect4[1].x, Rect4[1].y), Point(Rect4[2].x, Rect4[2].y), Scalar(255, 255, 255), 1, 8);
	line(bin, Point(Rect4[2].x, Rect4[2].y), Point(Rect4[3].x, Rect4[3].y), Scalar(255, 255, 255), 1, 8);
	line(bin, Point(Rect4[3].x, Rect4[3].y), Point(Rect4[0].x, Rect4[0].y), Scalar(255, 255, 255), 1, 8);

	non_Bond_Fix();
	imwrite("bin4.bmp", bin);

	int full_Size = image.cols * image.rows;
	int edge_Size = 0;
	int remainder_Size = 0;

	line(dst, Point(Rect4[0].x, Rect4[0].y), Point(Rect4[1].x, Rect4[1].y), Scalar(230, 230, 230), 1, 8);
	line(dst, Point(Rect4[1].x, Rect4[1].y), Point(Rect4[2].x, Rect4[2].y), Scalar(230, 230, 230), 1, 8);
	line(dst, Point(Rect4[2].x, Rect4[2].y), Point(Rect4[3].x, Rect4[3].y), Scalar(230, 230, 230), 1, 8);
	line(dst, Point(Rect4[3].x, Rect4[3].y), Point(Rect4[0].x, Rect4[0].y), Scalar(230, 230, 230), 1, 8);

	imwrite("dis2.bmp", dst);
	for (int i = 0; i < dst.rows; i++) {
		uchar* inData = dst.ptr<uchar>(i);
		for (int j = 0; j < Cols; j += 3) {
			if (inData[j] == 250 && inData[j + 1] == 250)
				remainder_Size++;
			else if (inData[j] == 230)
				edge_Size++;
		}
	}

	float DB_rate = (float)(full_Size - edge_Size - remainder_Size) / (full_Size - edge_Size);

	float final_result = DB_rate;
	if (final_result < 0.9)
		final_result *= offset;

	ui->log->insertPlainText("DB Coverage Rate: ");
	string s = to_string(final_result) + '\n';
	ui->log->insertPlainText(s.c_str());

	int x = 640, y = 480;
	if (image.cols * 3 > image.rows * 4)
		y = image.rows * 640 / image.cols;
	else x = image.cols * 480 / image.rows;

	cv::resize(dst, display_image, Size(x, y), 0, 0);
	imwrite("display.bmp", display_image);

	ui->label_show_image->setPixmap(QPixmap("./display.bmp"));

}

int my_floodfill( Mat image, Point seedPoint ){

	BYTE seedBV = image.at<uchar>(seedPoint.y, seedPoint.x);

	if (seedBV==0) {
		image.at<uchar>(seedPoint.y, seedPoint.x) = 55;
		mRoi_cnt++;
		if (seedPoint.x > Max_X) Max_X = seedPoint.x;
		if (seedPoint.x < Min_X) Min_X = seedPoint.x;
		if (seedPoint.y > Max_Y) Max_Y = seedPoint.y;
		if (seedPoint.y < Min_Y) Min_Y = seedPoint.y;

		if (seedPoint.y > 0) {
			Point p(seedPoint.x, seedPoint.y-1);
			my_floodfill(image, p);
		}
		if (seedPoint.x > 0) {
			Point p(seedPoint.x - 1, seedPoint.y );
			my_floodfill(image, p);
		}
		if (seedPoint.y < image.rows-1) {
			Point p(seedPoint.x, seedPoint.y + 1);
			my_floodfill(image, p);
		}
		if (seedPoint.x < image.cols - 1) {
			Point p(seedPoint.x + 1, seedPoint.y );
			my_floodfill(image, p);
		}
	}
	return 0;
}

void DB_Area_Calc::prism_area_calc() {

	int Cols = image.cols * 3;
	int ret = 0, n = 0, TH = 0, Valid_TH = 48;
	if (image.cols < 600)Valid_TH = image.cols / 20;

	dia = sqrt(image.cols*image.cols + image.rows*image.rows);
	image_Total_Pixel = image.cols*image.rows;

	if (dia < 750)
		blur(src1, src1, Size(3, 3));
	else if (dia < 3000)
		blur(src1, src1, Size(5, 5));
	else
		blur(src1, src1, Size(7, 7));

	cvtColor(src1, gray, CV_BGR2GRAY);

	min_BG = get_backGround_Level();
	TH = get_Image_100_min_max();

	imwrite("grey.bmp", gray);
	if (ui->manual_BV->isChecked()) TH = bianryTh1;

	threshold(gray, bin, TH, 100, THRESH_BINARY);
	imwrite("bin1.bmp", bin);
	bin1 = bin.clone();

	Canny(bin, edge, 3, 9, 3);
	imwrite("edge.bmp ", edge);
	Line_Data max_Line;
	max_Line.valid_Point = 0;

	//【3】进行霍夫线变换
	vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
	HoughLines(edge, lines, 1, CV_PI / 180, Valid_TH, 0, 0);

	int TH2 = 30;
	while (lines.size()>300 && TH2<200) {
		HoughLines(edge, lines, 1, CV_PI / 180, Valid_TH + TH2, 0, 0);
		TH2 += 30;
	}
	memset(Line_All, 0, sizeof(Line_Rect));
	memset(Line_Valid, 0, sizeof(Line_Valid));

	int Half_Width = bin.cols / 2, Half_Height = bin.rows / 2;

	//【4】依次在图中绘制出每条线段
	for (size_t i = 0; i < lines.size(); i++)
	{
		Line_All[i].rho = lines[i][0], Line_All[i].theta = lines[i][1];
		Point p;
		double a = cos(Line_All[i].theta), b = sin(Line_All[i].theta);
		double x0 = a*Line_All[i].rho, y0 = b*Line_All[i].rho;

		int s1 = cvRound((x0) / b);
		int s2 = cvRound((x0 - image.cols) / b);
		int last_x = 0, last_y = 0;;
		Line_All[i].valid_Point = 0;
		if (s1 > s2)swap(s1, s2);
		if (s1 < -5000)s1 = -5000;
		if (s2 > 5000)s2 = 5000;
		for (int k = s1; k < s2; k++) {
			p.x = cvRound(x0 + k * (-b));

			if (p.x == last_x)continue;
			if (p.x < 10)continue;
			if (p.x > image.cols - 11)continue;

			p.y = cvRound(y0 + k * (a));
			if (p.y < 10)continue;
			if (p.y > image.rows - 11)continue;
			// UD / LR continuous Point count
			if (p.y>Half_Height&&bin.at<uchar>(p.y - 1, p.x) == 0 && bin.at<uchar>(p.y + 1, p.x) == 100) {
				if (bin.at<uchar>(p.y - 1, p.x - 1) == 0 && bin.at<uchar>(p.y + 1, p.x - 1) == 100)
					if (bin.at<uchar>(p.y - 1, p.x + 1) == 0 && bin.at<uchar>(p.y + 1, p.x + 1) == 100)
						Line_All[i].valid_Point++;
			}
			else if (p.y<Half_Height&&bin.at<uchar>(p.y - 1, p.x) == 100 && bin.at<uchar>(p.y + 1, p.x) == 0) {
				if (bin.at<uchar>(p.y - 1, p.x - 1) == 100 && bin.at<uchar>(p.y + 1, p.x - 1) == 0)
					if (bin.at<uchar>(p.y - 1, p.x + 1) == 100 && bin.at<uchar>(p.y + 1, p.x + 1) == 0)
						Line_All[i].valid_Point++;
			}
			else if (p.x>Half_Width&&bin.at<uchar>(p.y, p.x - 1) == 0 && bin.at<uchar>(p.y, p.x + 1) == 100) {
				if (bin.at<uchar>(p.y - 1, p.x - 1) == 0 && bin.at<uchar>(p.y - 1, p.x + 1) == 100)
					if (bin.at<uchar>(p.y + 1, p.x - 1) == 0 && bin.at<uchar>(p.y + 1, p.x + 1) == 100)
						Line_All[i].valid_Point++; 
			}
			else if (p.x<Half_Width&&bin.at<uchar>(p.y, p.x - 1) == 100 && bin.at<uchar>(p.y, p.x + 1) == 0) {
				if (bin.at<uchar>(p.y + 1, p.x - 1) == 100 && bin.at<uchar>(p.y + 1, p.x + 1) == 0)
					if (bin.at<uchar>(p.y - 1, p.x - 1) == 100 && bin.at<uchar>(p.y - 1, p.x + 1) == 0)
						Line_All[i].valid_Point++;
			}
			//else if (bin.at<uchar>(p.y + 1, p.x - 2) == 0 && bin.at<uchar>(p.y - 1, p.x + 1) == 100) {
			//	if (bin.at<uchar>(p.y-1 + 1, p.x-1 - 2) == 0 && bin.at<uchar>(p.y-1 - 1, p.x-1 + 1) == 100)
			//		if (bin.at<uchar>(p.y+1 + 1, p.x+1 - 1) == 0 && bin.at<uchar>(p.y+1 - 1, p.x+1 + 1) == 100)
			//			Line_All[i].valid_Point++;
			//}
			//else if (bin.at<uchar>(p.y-1, p.x - 1) == 100 && bin.at<uchar>(p.y+1, p.x + 1) == 0) {
			//	if (bin.at<uchar>(p.y+1 - 1, p.x-1 - 1) == 100 && bin.at<uchar>(p.y+1 + 1, p.x-1 + 1) == 0)
			//		if (bin.at<uchar>(p.y-1 - 1, p.x+1 - 1) == 100 && bin.at<uchar>(p.y-1 + 1, p.x+1 + 1) == 0)
			//			Line_All[i].valid_Point++;
			//}

			last_x = p.x;
		}

		s1 = cvRound((-y0) / a);
		s2 = cvRound((image.rows - y0) / a);
		if (s1 > s2)swap(s1, s2);
		if (s1 < -5000)s1 = -5000;
		if (s2 > 5000)s2 = 5000;

		for (int k = s1; k < s2; k++) {

			p.y = cvRound(y0 + k * (a));
			if (p.y == last_y)continue;
			if (p.y < 11)continue;
			if (p.y > image.rows - 12)continue;

			p.x = cvRound(x0 + k * (-b));
			if (p.x < 11)continue;
			if (p.x > image.cols - 12)continue;

			if (p.y > Half_Height&&bin.at<uchar>(p.y - 1, p.x) == 0 && bin.at<uchar>(p.y + 1, p.x) == 100)
				Line_All[i].valid_Point++;
			else if (p.y < Half_Height&&bin.at<uchar>(p.y, p.x - 1) == 0 && bin.at<uchar>(p.y, p.x + 1) == 100)
				Line_All[i].valid_Point++;
			else if (p.x > Half_Width&&bin.at<uchar>(p.y - 1, p.x) == 100 && bin.at<uchar>(p.y + 1, p.x) == 0)
				Line_All[i].valid_Point++;
			else if (p.x<Half_Width&&bin.at<uchar>(p.y, p.x - 1) == 100 && bin.at<uchar>(p.y, p.x + 1) == 0)
				Line_All[i].valid_Point++;
			//else if (bin.at<uchar>(p.y - 1, p.x - 1) + bin.at<uchar>(p.y + 1, p.x + 1) == 100)
			//	Line_All[i].valid_Point++;
			//else if (bin.at<uchar>(p.y - 1, p.x + 1) + bin.at<uchar>(p.y + 1, p.x - 1) == 100)
			//	Line_All[i].valid_Point++;

			last_y = p.y;
		}

		if (Line_All[i].valid_Point > Valid_TH) {
			Line_All[i].pt1.x = cvRound(x0 + 5000 * (-b));
			Line_All[i].pt1.y = cvRound(y0 + 5000 * (a));
			Line_All[i].pt2.x = cvRound(x0 - 5000 * (-b));
			Line_All[i].pt2.y = cvRound(y0 - 5000 * (a));
			line(image, Line_All[i].pt1, Line_All[i].pt2, Scalar(0, 0, 255), 1, CV_AA);
			Line_Valid[n++] = Line_All[i];
		}

		if (Line_All[i].valid_Point > max_Line.valid_Point) {
			max_Line = Line_All[i];
		}
	}
	Line_Data voilid_max;
	double a = cos(max_Line.theta), b = sin(max_Line.theta);
	double x0 = a*max_Line.rho, y0 = b*max_Line.rho;

	voilid_max.pt1.x = cvRound(x0 + 5000 * (-b));
	voilid_max.pt1.y = cvRound(y0 + 5000 * (a));
	voilid_max.pt2.x = cvRound(x0 - 5000 * (-b));
	voilid_max.pt2.y = cvRound(y0 - 5000 * (a));
	line(image, voilid_max.pt1, voilid_max.pt2, Scalar(255, 255, 55), 2, CV_AA);

	memset(Line_Rect, 0, sizeof(Line_Rect));
	Line_Rect[0] = max_Line;
	imwrite("line1.bmp", image);
	//find 3 line
	for (int i = 0; i < n; i++)
		if (Line_Valid[i].valid_Point < Line_Rect[0].valid_Point) {
			Line_Valid[i].v = Line_Rect[0].theta - Line_Valid[i].theta;
			if (abs(Line_Valid[i].v) > CV_PI / 2 * 0.98&&abs(Line_Valid[i].v) < CV_PI / 2 * 1.02) {
				line(image, Line_Valid[i].pt1, Line_Valid[i].pt2, Scalar(0, 0, 255), 1, CV_AA);
				if (Line_Valid[i].valid_Point > Line_Rect[1].valid_Point) 
					if (Line_Rect[1].rho == 0 || Line_Valid[i].rho<Line_Rect[1].rho + 3) {
					Line_Rect[1] = Line_Valid[i];
				}
			}
			else if (Line_Rect[0].theta> CV_PI * 3 / 4 * 0.8&&Line_Rect[0].theta< CV_PI * 3 / 4 * 1.2&&  Line_Valid[i].v > CV_PI / 4 * 0.95&&Line_Valid[i].v < CV_PI / 4 * 1.05) {
				if (Line_Valid[i].valid_Point > Line_Rect[1].valid_Point) 
					if (Line_Rect[1].rho == 0 || Line_Valid[i].rho<Line_Rect[1].rho + 15) {
					Line_Rect[1] = Line_Valid[i];
				}
			}
			else if (Line_Rect[0].theta> CV_PI / 4 * 0.8&&Line_Rect[0].theta< CV_PI * 1 / 4 * 1.2&&  Line_Valid[i].v > CV_PI / 4 * -1.05&&Line_Valid[i].v < CV_PI / 4 * -0.95) {
				if (Line_Valid[i].valid_Point > Line_Rect[1].valid_Point&&Line_Valid[i].theta >  CV_PI / 2 * 0.95&&Line_Valid[i].theta <  CV_PI / 2 * 1.05){
					if(Line_Rect[1].rho ==0||Line_Valid[i].rho<Line_Rect[1].rho+15)
						Line_Rect[1] = Line_Valid[i];
				}
				else if (Line_Valid[i].valid_Point>32 && Line_Valid[i].valid_Point > Line_Rect[1].valid_Point*0.6&&Line_Valid[i].rho<Line_Rect[1].rho) {
					Line_Rect[1] = Line_Valid[i];
				}
			}
			else if ((Line_Rect[0].theta< CV_PI / 8 ||Line_Rect[0].theta > CV_PI *7/8)&&abs(Line_Valid[i].v) > CV_PI / 4 * 0.94&&abs(Line_Valid[i].v) < CV_PI / 4 * 1.06) {
				if (Line_Valid[i].valid_Point > Line_Rect[2].valid_Point) {
					Line_Rect[2] = Line_Valid[i];
				}
			}
			else if ((abs(Line_Valid[i].v) > CV_PI * 3 / 4 * 0.97&&abs(Line_Valid[i].v) < CV_PI * 3 / 4 * 1.03)||
					(abs(Line_Valid[i].v) > CV_PI / 4 * 0.97&&abs(Line_Valid[i].v) < CV_PI / 4 * 1.03)) {
					if (Line_Valid[i].valid_Point > Line_Rect[2].valid_Point * 2) {
						Line_Rect[2] = Line_Valid[i];
					}
					else if (Line_Rect[2].valid_Point == 0 && Line_Valid[i].valid_Point > Line_Rect[2].valid_Point) {
						Line_Rect[2] = Line_Valid[i];
					}
					else if (Line_Valid[i].valid_Point > 0 && Line_Valid[i].valid_Point > Line_Rect[2].valid_Point*0.6&&Line_Valid[i].rho < Line_Rect[2].rho &&Line_Valid[i].rho > Line_Rect[2].rho-100) {
						Line_Rect[2] = Line_Valid[i];
					}
				}	
		}
	for (int k = 1; k< 3; k++)
		if (Line_Rect[k].valid_Point == 0) {
			string s = "Can not find Sensor Rect Outline! \n";
			ui->log->insertPlainText(s.c_str());
			return;
		}

	// one edge double line selection 
	for (int k = 0; k < 3; k++) {
		for (int i = 0; i < n; i++)
			if (Line_Valid[i].valid_Point < Line_Rect[k].valid_Point) 
				if (Line_Valid[i].theta > Line_Rect[k].theta* 0.99&&Line_Valid[i].theta < Line_Rect[k].theta * 1.01)
					if (abs(Line_Valid[i].rho - Line_Rect[k].rho)>3&& Line_Valid[i].valid_Point > Line_Rect[k].valid_Point*0.5)					{
						if (Line_Valid[i].theta > CV_PI * 3 / 4 * 0.9&& Line_Valid[i].theta < CV_PI * 3 / 4 * 1.1&&Line_Valid[i].rho < Line_Rect[k].rho)
							Line_Rect[k] = Line_Valid[i];
						break;
					}
	}

	// short edge diff angle selection 
	for (int k = 1; k < 3; k++) {
		if (Line_Rect[0].theta< CV_PI / 8 || Line_Rect[0].theta > CV_PI * 7 / 8) {
			Line_Rect[k].v = abs(Line_Rect[0].theta - Line_Rect[k].theta) - (CV_PI / 2);
			for (int i = 0; i < n; i++)
				if (Line_Valid[i].theta > CV_PI * 1 / 2 * 0.9&&Line_Valid[i].theta < CV_PI * 1 / 2 * 1.1) {
					Line_Valid[i].v = abs(Line_Rect[0].theta - Line_Valid[i].theta) - (CV_PI / 2);

					if (abs(Line_Valid[i].v) < abs(Line_Rect[k].v) && Line_Valid[i].rho <Line_Rect[k].rho + 15 && Line_Valid[i].valid_Point > Line_Rect[k].valid_Point*0.6) {
						Line_Rect[k] = Line_Valid[i];
						break;
					}
				}

		}else if (Line_Rect[k].theta > CV_PI * 1 / 2 * 0.9&&Line_Rect[k].theta < CV_PI * 1 / 2 * 1.1) {
			Line_Rect[k].v = abs(Line_Rect[3 - k].theta - Line_Rect[k].theta) - (CV_PI / 2);
			for (int i = 0; i < n; i++)
				if (Line_Valid[i].theta > CV_PI * 1 / 2 * 0.9&&Line_Valid[i].theta < CV_PI * 1 / 2 * 1.1) {
					Line_Valid[i].v = abs(Line_Rect[3 - k].theta - Line_Valid[i].theta) - (CV_PI / 2);

					if (abs(Line_Valid[i].v) < abs(Line_Rect[k].v) && Line_Valid[i].rho <Line_Rect[k].rho + 15 && Line_Valid[i].valid_Point > Line_Rect[k].valid_Point*0.6) {
						Line_Rect[k] = Line_Valid[i];
						break;
					}
				}
		}
	}

	for (int i = 0; i < 3; i++) {

		int j = (i + 1) % 3;
		Point p = get_intersection(Line_Rect[i].pt1, Line_Rect[i].pt2, Line_Rect[j].pt1, Line_Rect[j].pt2);
		circle(src1, p, 10, Scalar(10, 10, 255), 2, 4, 0);

		Rect4[i].x = p.x;
		Rect4[i].y = p.y;
	}

	line(image, Point(Rect4[0].x, Rect4[0].y), Point(Rect4[1].x, Rect4[1].y), Scalar(0, 255, 255), 2, CV_AA);
	line(image, Point(Rect4[1].x, Rect4[1].y), Point(Rect4[2].x, Rect4[2].y), Scalar(0, 255, 255), 2, CV_AA);
	line(image, Point(Rect4[2].x, Rect4[2].y), Point(Rect4[0].x, Rect4[0].y), Scalar(0, 255, 255), 2, CV_AA);

	//	ret += Rect_Correct();
	imwrite("line2.bmp", image);

	if (ret != 0) {
		string s = "Can not find Sensor Rect Outline! \n";
		ui->log->insertPlainText(s.c_str());
		return;
	}
	///////////////////////////////////////////////////////////////////////////
	//bin = gray.clone();

	int bg = remove_Edge();

	line(bin, Point(Rect4[0].x, Rect4[0].y), Point(Rect4[1].x, Rect4[1].y), Scalar(255, 255, 255), 1, 8);
	line(bin, Point(Rect4[1].x, Rect4[1].y), Point(Rect4[2].x, Rect4[2].y), Scalar(255, 255, 255), 1, 8);
	line(bin, Point(Rect4[2].x, Rect4[2].y), Point(Rect4[0].x, Rect4[0].y), Scalar(255, 255, 255), 1, 8);
	//imwrite("bin2.bmp", bin);

	cvtColor(bin, dst, CV_GRAY2BGR);
	imwrite("cutting.bmp", dst);

	Rect ccomp;
	uchar* inData;
	int fill_offset = 16;

	for (int i = 0; i < 3; i++) {	
		bool f = false;
		for (int y = -32; y < 32; y += 8) {
			if (f) break;
			for (int x = -32; x < 32; x += 8) {
				if (Rect4[i].y + y>8 && Rect4[i].x + x>8 && Rect4[i].y + y < image.rows-9&&Rect4[i].x + x < image.cols-9)
				if (bin.at<uchar>(Rect4[i].y + y, Rect4[i].x + x) == 0) {
					int cnt =floodFill(dst, Point( Rect4[i].x + x, Rect4[i].y + y), Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));
					int cnt1 = floodFill(bin, Point(Rect4[i].x + x, Rect4[i].y + y), Scalar(55, 55, 55), &ccomp, Scalar(1, 1, 1), Scalar(1, 1, 1));

					//f = true;
					//break;
				}
			}
		}
	}

	int full_Size = image.cols * image.rows;
	int edge_Size = 0;
	int remainder_Size = 0;

	line(dst, Point(Rect4[0].x, Rect4[0].y), Point(Rect4[1].x, Rect4[1].y), Scalar(255, 255, 255), 1, 8);
	line(dst, Point(Rect4[1].x, Rect4[1].y), Point(Rect4[2].x, Rect4[2].y), Scalar(255, 255, 255), 1, 8);
	line(dst, Point(Rect4[2].x, Rect4[2].y), Point(Rect4[0].x, Rect4[0].y), Scalar(255, 255, 255), 1, 8);

	float line1 = sqrt(pow(Rect4[0].x- Rect4[1].x, 2)+ pow(Rect4[0].y - Rect4[1].y, 2));
	float line2 = sqrt(pow(Rect4[1].x - Rect4[2].x, 2) + pow(Rect4[1].y - Rect4[2].y, 2));
	float line3 = sqrt(pow(Rect4[0].x - Rect4[2].x, 2) + pow(Rect4[0].y - Rect4[2].y, 2));

	float max_line = max(line1, max(line2, line3));
	imwrite("bin2.bmp", bin);

	Mat bin2 = bin.clone();
	//edge_fix
	for (int i = 3; i < dst.rows-3; i++) {
		uchar* inData = dst.ptr<uchar>(i);
		for (int j = 3; j < Cols-3; j += 3) {
			if (inData[j] == 255 && inData[j + 1] == 255 && inData[j + 2] == 255) {

				if (i > 420 && j / 3 < 430) {
					int xxxx = 0;
				}

				for (int y = -3; y <= 3; y+=1)
					for (int x = -3; x <= 3; x+=1) {
						if (bin.at<uchar>(i + y, j / 3 + x) == 100) {

							int cnt = floodFill(bin, Point(j / 3 + x, i + y), Scalar(0, 0, 0), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));
							if (cnt < max_line*2) {
								floodFill(bin, Point(j / 3 + x, i + y), Scalar(0, 0, 0), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));
								floodFill(dst, Point(j / 3 + x, i + y), Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));
							}
							else if(cnt < max_line*5){
								floodFill(bin, Point(j / 3 + x, i + y), Scalar(230, 230, 230), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));
								floodFill(dst, Point(j / 3 + x, i + y), Scalar(230, 230, 230), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));

							}

						}
						
						if (bin2.at<uchar>(i + y, j / 3 + x) == 0) {
							Min_X = 99999999, Max_X = 0, Min_Y = 99999999, Max_Y = 0, mRoi_cnt = 0;
							Point p(j / 3 + x, i + y);
							my_floodfill(bin2,p);
							float center_dis = sqrt(pow(j / 3-bin2.cols/2,2)+ pow(i - bin2.rows / 2, 2));
							bool bond = (abs(Max_Y - Min_Y)>abs(Max_X - Min_X) * 2)&& (center_dis>max_line/4);
							int mRect_cnt = (Max_X - Min_X)*(Max_Y - Min_Y);
							if (mRoi_cnt>mRect_cnt/3&&!bond) {
								floodFill(bin, Point(j / 3 + x, i + y), Scalar(0, 0, 0), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));
								floodFill(dst, Point(j / 3 + x, i + y), Scalar(250, 250, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));
							}
						}
					}
			}
		}
	}

	imwrite("dis2.bmp", dst);
	for (int i = 0; i < dst.rows; i++) {
		uchar* inData = dst.ptr<uchar>(i);
		for (int j = 0; j < Cols; j += 3) {
			if (inData[j] == 250 && inData[j + 1] == 250&& inData[j + 2] == 55)
				remainder_Size++;
			else if (inData[j+2] > 200)
				edge_Size++;
		}
	}

	float DB_rate = (float)(full_Size - edge_Size - remainder_Size) / (full_Size - edge_Size);

	float final_result = DB_rate;
	if (final_result < 0.9)
		final_result *= offset;

	ui->log->insertPlainText("DB Coverage Rate: ");
	string s = to_string(final_result) + '\n';
	ui->log->insertPlainText(s.c_str());

	int x = 640, y = 480;
	if (image.cols * 3 > image.rows * 4)
		y = image.rows * 640 / image.cols;
	else x = image.cols * 480 / image.rows;

	cv::resize(dst, display_image, Size(x, y), 0, 0);
	imwrite("display.bmp", display_image);

	ui->label_show_image->setPixmap(QPixmap("./display.bmp"));

}

void DB_Area_Calc::on_pushButton_ClearBack_clicked()
{
	load_Panel_Value();
	ui->log->setText("");

	QString qs_name = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image File(*.bmp *.jpg *.jpeg *.png *.pbm *.pgm *.ppm)"));
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	name = code->fromUnicode(qs_name).data();

	Mat dstImage;//目标图
	Mat normImage;//归一化后的图
	Mat scaledImage;//线性变换后的八位无符号整型的图

	if (name.length() < 2) return;
	image = imread(name, 1);
	out_image = image.clone();
	src1 = image.clone();
	dstImage = image.clone();

	WIDTH1 = image.cols / 64;
	HEIGHT1 = image.rows / 64;

	if (ui->checkBox_Prism->isChecked()) {	
		prism_area_calc();
	}
	else {
		sensor_area_calc();	
	}

}


void DB_Area_Calc::on_pushButton_clear_clicked()
{
	ui->log->setText("");
}

