/********************************************************************************
** Form generated from reading UI file 'DB_Area_Calc.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DB_AREA_CALC_H
#define UI_DB_AREA_CALC_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DB_Area_Calc
{
public:
    QLabel *label_show_image;
    QPushButton *pushButton_open_BMP;
    QTextBrowser *log;
    QGroupBox *groupBox;
    QTextEdit *bianryTh1;
    QLabel *label;
    QLabel *label_5;
    QTextBrowser *offset;
    QGroupBox *groupBox_2;
    QTextEdit *Coner_block;
    QTextEdit *Corner_quality;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *pushButton_clear;
    QRadioButton *auto_BV;
    QRadioButton *manual_BV;
    QPushButton *pushButton_ClearBack;
    QLabel *label_2;
    QCheckBox *checkBox_Prism;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *DB_Area_Calc)
    {
        if (DB_Area_Calc->objectName().isEmpty())
            DB_Area_Calc->setObjectName(QStringLiteral("DB_Area_Calc"));
        DB_Area_Calc->resize(660, 575);
        label_show_image = new QLabel(DB_Area_Calc);
        label_show_image->setObjectName(QStringLiteral("label_show_image"));
        label_show_image->setGeometry(QRect(10, 10, 640, 480));
        pushButton_open_BMP = new QPushButton(DB_Area_Calc);
        pushButton_open_BMP->setObjectName(QStringLiteral("pushButton_open_BMP"));
        pushButton_open_BMP->setGeometry(QRect(570, 660, 81, 30));
        pushButton_open_BMP->setCheckable(false);
        log = new QTextBrowser(DB_Area_Calc);
        log->setObjectName(QStringLiteral("log"));
        log->setGeometry(QRect(299, 500, 270, 60));
        groupBox = new QGroupBox(DB_Area_Calc);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(2, 495, 291, 71));
        bianryTh1 = new QTextEdit(groupBox);
        bianryTh1->setObjectName(QStringLiteral("bianryTh1"));
        bianryTh1->setGeometry(QRect(150, 20, 41, 30));
        bianryTh1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        bianryTh1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(130, 25, 21, 21));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 19, 41, 31));
        offset = new QTextBrowser(groupBox);
        offset->setObjectName(QStringLiteral("offset"));
        offset->setGeometry(QRect(50, 20, 61, 31));
        groupBox_2 = new QGroupBox(DB_Area_Calc);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 630, 111, 101));
        Coner_block = new QTextEdit(groupBox_2);
        Coner_block->setObjectName(QStringLiteral("Coner_block"));
        Coner_block->setGeometry(QRect(60, 60, 41, 30));
        Coner_block->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        Coner_block->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        Corner_quality = new QTextEdit(groupBox_2);
        Corner_quality->setObjectName(QStringLiteral("Corner_quality"));
        Corner_quality->setGeometry(QRect(60, 20, 41, 30));
        Corner_quality->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        Corner_quality->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 25, 41, 16));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 70, 41, 16));
        pushButton_clear = new QPushButton(DB_Area_Calc);
        pushButton_clear->setObjectName(QStringLiteral("pushButton_clear"));
        pushButton_clear->setGeometry(QRect(520, 530, 50, 30));
        pushButton_clear->setCheckable(false);
        auto_BV = new QRadioButton(DB_Area_Calc);
        buttonGroup = new QButtonGroup(DB_Area_Calc);
        buttonGroup->setObjectName(QStringLiteral("buttonGroup"));
        buttonGroup->addButton(auto_BV);
        auto_BV->setObjectName(QStringLiteral("auto_BV"));
        auto_BV->setGeometry(QRect(210, 510, 89, 16));
        auto_BV->setChecked(true);
        manual_BV = new QRadioButton(DB_Area_Calc);
        buttonGroup->addButton(manual_BV);
        manual_BV->setObjectName(QStringLiteral("manual_BV"));
        manual_BV->setGeometry(QRect(210, 530, 89, 16));
        manual_BV->setChecked(false);
        pushButton_ClearBack = new QPushButton(DB_Area_Calc);
        pushButton_ClearBack->setObjectName(QStringLiteral("pushButton_ClearBack"));
        pushButton_ClearBack->setEnabled(true);
        pushButton_ClearBack->setGeometry(QRect(575, 500, 81, 30));
        pushButton_ClearBack->setCheckable(false);
        label_2 = new QLabel(DB_Area_Calc);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(390, 560, 260, 16));
        checkBox_Prism = new QCheckBox(DB_Area_Calc);
        checkBox_Prism->setObjectName(QStringLiteral("checkBox_Prism"));
        checkBox_Prism->setGeometry(QRect(590, 540, 51, 16));
        checkBox_Prism->setChecked(true);

        retranslateUi(DB_Area_Calc);

        QMetaObject::connectSlotsByName(DB_Area_Calc);
    } // setupUi

    void retranslateUi(QWidget *DB_Area_Calc)
    {
        DB_Area_Calc->setWindowTitle(QApplication::translate("DB_Area_Calc", "DB_Area_Calc_20240828", Q_NULLPTR));
        label_show_image->setText(QApplication::translate("DB_Area_Calc", "ShowImage", Q_NULLPTR));
        pushButton_open_BMP->setText(QApplication::translate("DB_Area_Calc", "Corner", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("DB_Area_Calc", "Binaryzation", Q_NULLPTR));
        bianryTh1->setHtml(QApplication::translate("DB_Area_Calc", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">50</p></body></html>", Q_NULLPTR));
        label->setText(QApplication::translate("DB_Area_Calc", "BV", Q_NULLPTR));
        label_5->setText(QApplication::translate("DB_Area_Calc", "Offset", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("DB_Area_Calc", "Corner Detect", Q_NULLPTR));
        Coner_block->setHtml(QApplication::translate("DB_Area_Calc", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">7</p></body></html>", Q_NULLPTR));
        Corner_quality->setHtml(QApplication::translate("DB_Area_Calc", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">5</p></body></html>", Q_NULLPTR));
        label_3->setText(QApplication::translate("DB_Area_Calc", "Quality", Q_NULLPTR));
        label_4->setText(QApplication::translate("DB_Area_Calc", "Block", Q_NULLPTR));
        pushButton_clear->setText(QApplication::translate("DB_Area_Calc", "Clear", Q_NULLPTR));
        auto_BV->setText(QApplication::translate("DB_Area_Calc", "Auto BV", Q_NULLPTR));
        manual_BV->setText(QApplication::translate("DB_Area_Calc", "Manual BV", Q_NULLPTR));
        pushButton_ClearBack->setText(QApplication::translate("DB_Area_Calc", "Open Image", Q_NULLPTR));
        label_2->setText(QApplication::translate("DB_Area_Calc", "Supported by songye.guo@samsung.com", Q_NULLPTR));
        checkBox_Prism->setText(QApplication::translate("DB_Area_Calc", "Prism", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DB_Area_Calc: public Ui_DB_Area_Calc {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DB_AREA_CALC_H
