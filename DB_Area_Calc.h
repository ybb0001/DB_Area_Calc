#ifndef DB_Area_Calc_H
#define DB_Area_Calc_H

#include <QWidget>
#include "core.hpp"
#include"highgui.hpp"
#include <algorithm>

namespace Ui {
class DB_Area_Calc;
}

class DB_Area_Calc : public QWidget
{
    Q_OBJECT

public:
    explicit DB_Area_Calc(QWidget *parent = 0);
    ~DB_Area_Calc();

private slots:
 
	void on_pushButton_open_BMP_clicked();
	void on_pushButton_ClearBack_clicked();
	void on_pushButton_clear_clicked();
	void load_Panel_Value();
	unsigned int get_Binaryztion_Level();
	unsigned int get_backGround_Level();
	unsigned int get_Image_Threshold();

	int Rect_Correct();
	int remove_Edge();
	int non_Bond_Fix();
	int edge_Fix();
	int getTH(int k, int offset);

private:
    Ui::DB_Area_Calc *ui;
    cv::Mat image,display_image;
	cv::Mat temp_image;
	cv::Mat out_image,src1;
	cv::Mat gray_image;
	cv::Mat dst, edge, gray, g_grayImage, bin,bin1,bin2;

    QImage img;
    QImage imgScaled;
	std::string name;
};

#endif // DB_Area_Calc_H
