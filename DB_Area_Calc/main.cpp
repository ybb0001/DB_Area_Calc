#include "DB_Area_Calc.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DB_Area_Calc w;
	w.show();
	return a.exec();
}
