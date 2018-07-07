/********************************************************************************
** Form generated from reading UI file 'ratbehavioranalysis.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RATBEHAVIORANALYSIS_H
#define UI_RATBEHAVIORANALYSIS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RatBehaviorAnalysisClass
{
public:
    QWidget *centralWidget;
    QLabel *depthLabel;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QSpinBox *lowerBound;
    QSpinBox *upperBound;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QGroupBox *groupBox;
    QRadioButton *radioButton_Grayscale;
    QRadioButton *radioButton_PseudoColor;
    QWidget *horizontalLayoutWidget;
    QGridLayout *gridLayout_2;
    QLineEdit *lineEdit_TrialNo;
    QLabel *label_5;
    QLabel *label_4;
    QComboBox *recordingFormat;
    QLabel *label_11;
    QComboBox *recordingFPS;
    QPushButton *recordingButton;
    QGroupBox *groupBox_2;
    QPushButton *backgroundButton;
    QLabel *label_BGR;
    QPushButton *backgroundSaveButton;
    QGroupBox *groupBox_3;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_3;
    QLabel *label_6;
    QSpinBox *point2_x;
    QSpinBox *point2_y;
    QLabel *label_8;
    QSpinBox *point1_y;
    QSpinBox *point1_x;
    QLabel *label_9;
    QLabel *label_7;
    QRadioButton *ROI_Off;
    QRadioButton *ROI_On;
    QPushButton *calculationButton;
    QTextEdit *featureDisplay;
    QLabel *label_10;
    QSpinBox *threshold;
    QPushButton *BtnSwitch;
    QPushButton *BtnLog;
    QPushButton *BtnTracking;
    QPushButton *BtnExpand;
    QGroupBox *groupBox_4;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_4;
    QSpinBox *color_point2_x;
    QLabel *label_12;
    QSpinBox *color_point1_y;
    QSpinBox *color_point1_x;
    QSpinBox *color_point2_y;
    QLabel *label_13;
    QLabel *label_15;
    QLabel *label_14;
    QPushButton *BtnSwitchTimeStampColor;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *RatBehaviorAnalysisClass)
    {
        if (RatBehaviorAnalysisClass->objectName().isEmpty())
            RatBehaviorAnalysisClass->setObjectName(QStringLiteral("RatBehaviorAnalysisClass"));
        RatBehaviorAnalysisClass->resize(535, 480);
        centralWidget = new QWidget(RatBehaviorAnalysisClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        depthLabel = new QLabel(centralWidget);
        depthLabel->setObjectName(QStringLiteral("depthLabel"));
        depthLabel->setGeometry(QRect(540, 10, 512, 424));
        gridLayoutWidget = new QWidget(centralWidget);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 30, 160, 61));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        lowerBound = new QSpinBox(gridLayoutWidget);
        lowerBound->setObjectName(QStringLiteral("lowerBound"));
        lowerBound->setMinimum(100);
        lowerBound->setMaximum(4500);
        lowerBound->setValue(450);

        gridLayout->addWidget(lowerBound, 0, 1, 1, 1);

        upperBound = new QSpinBox(gridLayoutWidget);
        upperBound->setObjectName(QStringLiteral("upperBound"));
        upperBound->setMinimum(400);
        upperBound->setMaximum(4500);
        upperBound->setValue(1850);

        gridLayout->addWidget(upperBound, 1, 1, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 0, 141, 21));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 100, 121, 71));
        radioButton_Grayscale = new QRadioButton(groupBox);
        radioButton_Grayscale->setObjectName(QStringLiteral("radioButton_Grayscale"));
        radioButton_Grayscale->setGeometry(QRect(10, 20, 95, 20));
        radioButton_Grayscale->setChecked(false);
        radioButton_PseudoColor = new QRadioButton(groupBox);
        radioButton_PseudoColor->setObjectName(QStringLiteral("radioButton_PseudoColor"));
        radioButton_PseudoColor->setGeometry(QRect(10, 40, 111, 20));
        radioButton_PseudoColor->setChecked(true);
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(20, 190, 211, 91));
        gridLayout_2 = new QGridLayout(horizontalLayoutWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        lineEdit_TrialNo = new QLineEdit(horizontalLayoutWidget);
        lineEdit_TrialNo->setObjectName(QStringLiteral("lineEdit_TrialNo"));

        gridLayout_2->addWidget(lineEdit_TrialNo, 0, 1, 1, 1);

        label_5 = new QLabel(horizontalLayoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 0, 0, 1, 1);

        label_4 = new QLabel(horizontalLayoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 2, 0, 1, 1);

        recordingFormat = new QComboBox(horizontalLayoutWidget);
        recordingFormat->setObjectName(QStringLiteral("recordingFormat"));

        gridLayout_2->addWidget(recordingFormat, 2, 1, 1, 1);

        label_11 = new QLabel(horizontalLayoutWidget);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_2->addWidget(label_11, 3, 0, 1, 1);

        recordingFPS = new QComboBox(horizontalLayoutWidget);
        recordingFPS->setObjectName(QStringLiteral("recordingFPS"));

        gridLayout_2->addWidget(recordingFPS, 3, 1, 1, 1);

        recordingButton = new QPushButton(centralWidget);
        recordingButton->setObjectName(QStringLiteral("recordingButton"));
        recordingButton->setGeometry(QRect(160, 290, 71, 41));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 330, 211, 61));
        backgroundButton = new QPushButton(groupBox_2);
        backgroundButton->setObjectName(QStringLiteral("backgroundButton"));
        backgroundButton->setGeometry(QRect(10, 20, 51, 28));
        label_BGR = new QLabel(groupBox_2);
        label_BGR->setObjectName(QStringLiteral("label_BGR"));
        label_BGR->setGeometry(QRect(80, 20, 61, 21));
        backgroundSaveButton = new QPushButton(groupBox_2);
        backgroundSaveButton->setObjectName(QStringLiteral("backgroundSaveButton"));
        backgroundSaveButton->setGeometry(QRect(150, 20, 51, 28));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(270, 200, 261, 111));
        gridLayoutWidget_2 = new QWidget(groupBox_3);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(10, 20, 237, 80));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(gridLayoutWidget_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_3->addWidget(label_6, 1, 0, 1, 1);

        point2_x = new QSpinBox(gridLayoutWidget_2);
        point2_x->setObjectName(QStringLiteral("point2_x"));
        point2_x->setMaximum(512);
        point2_x->setValue(360);

        gridLayout_3->addWidget(point2_x, 2, 1, 1, 1);

        point2_y = new QSpinBox(gridLayoutWidget_2);
        point2_y->setObjectName(QStringLiteral("point2_y"));
        point2_y->setMaximum(424);
        point2_y->setValue(280);

        gridLayout_3->addWidget(point2_y, 2, 2, 1, 1);

        label_8 = new QLabel(gridLayoutWidget_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_3->addWidget(label_8, 0, 1, 1, 1);

        point1_y = new QSpinBox(gridLayoutWidget_2);
        point1_y->setObjectName(QStringLiteral("point1_y"));
        point1_y->setMaximum(424);
        point1_y->setValue(90);

        gridLayout_3->addWidget(point1_y, 1, 2, 1, 1);

        point1_x = new QSpinBox(gridLayoutWidget_2);
        point1_x->setObjectName(QStringLiteral("point1_x"));
        point1_x->setMaximum(512);
        point1_x->setValue(130);

        gridLayout_3->addWidget(point1_x, 1, 1, 1, 1);

        label_9 = new QLabel(gridLayoutWidget_2);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_3->addWidget(label_9, 0, 2, 1, 1);

        label_7 = new QLabel(gridLayoutWidget_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_3->addWidget(label_7, 2, 0, 1, 1);

        ROI_Off = new QRadioButton(gridLayoutWidget_2);
        ROI_Off->setObjectName(QStringLiteral("ROI_Off"));
        ROI_Off->setChecked(false);

        gridLayout_3->addWidget(ROI_Off, 2, 3, 1, 1);

        ROI_On = new QRadioButton(gridLayoutWidget_2);
        ROI_On->setObjectName(QStringLiteral("ROI_On"));
        ROI_On->setChecked(true);

        gridLayout_3->addWidget(ROI_On, 1, 3, 1, 1);

        calculationButton = new QPushButton(centralWidget);
        calculationButton->setObjectName(QStringLiteral("calculationButton"));
        calculationButton->setGeometry(QRect(380, 360, 131, 28));
        featureDisplay = new QTextEdit(centralWidget);
        featureDisplay->setObjectName(QStringLiteral("featureDisplay"));
        featureDisplay->setGeometry(QRect(260, 10, 271, 31));
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(320, 330, 61, 16));
        threshold = new QSpinBox(centralWidget);
        threshold->setObjectName(QStringLiteral("threshold"));
        threshold->setGeometry(QRect(400, 330, 81, 22));
        threshold->setMaximum(255);
        threshold->setValue(10);
        BtnSwitch = new QPushButton(centralWidget);
        BtnSwitch->setObjectName(QStringLiteral("BtnSwitch"));
        BtnSwitch->setGeometry(QRect(300, 360, 71, 61));
        BtnLog = new QPushButton(centralWidget);
        BtnLog->setObjectName(QStringLiteral("BtnLog"));
        BtnLog->setGeometry(QRect(380, 390, 71, 41));
        BtnTracking = new QPushButton(centralWidget);
        BtnTracking->setObjectName(QStringLiteral("BtnTracking"));
        BtnTracking->setGeometry(QRect(450, 390, 71, 41));
        BtnExpand = new QPushButton(centralWidget);
        BtnExpand->setObjectName(QStringLiteral("BtnExpand"));
        BtnExpand->setGeometry(QRect(130, 400, 93, 28));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(270, 70, 261, 111));
        gridLayoutWidget_3 = new QWidget(groupBox_4);
        gridLayoutWidget_3->setObjectName(QStringLiteral("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(10, 20, 237, 80));
        gridLayout_4 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        color_point2_x = new QSpinBox(gridLayoutWidget_3);
        color_point2_x->setObjectName(QStringLiteral("color_point2_x"));
        color_point2_x->setMaximum(1920);
        color_point2_x->setValue(1300);

        gridLayout_4->addWidget(color_point2_x, 2, 1, 1, 1);

        label_12 = new QLabel(gridLayoutWidget_3);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_4->addWidget(label_12, 1, 0, 1, 1);

        color_point1_y = new QSpinBox(gridLayoutWidget_3);
        color_point1_y->setObjectName(QStringLiteral("color_point1_y"));
        color_point1_y->setMaximum(1080);
        color_point1_y->setValue(200);

        gridLayout_4->addWidget(color_point1_y, 1, 2, 1, 1);

        color_point1_x = new QSpinBox(gridLayoutWidget_3);
        color_point1_x->setObjectName(QStringLiteral("color_point1_x"));
        color_point1_x->setMaximum(1920);
        color_point1_x->setValue(600);

        gridLayout_4->addWidget(color_point1_x, 1, 1, 1, 1);

        color_point2_y = new QSpinBox(gridLayoutWidget_3);
        color_point2_y->setObjectName(QStringLiteral("color_point2_y"));
        color_point2_y->setMaximum(1080);
        color_point2_y->setValue(800);

        gridLayout_4->addWidget(color_point2_y, 2, 2, 1, 1);

        label_13 = new QLabel(gridLayoutWidget_3);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_4->addWidget(label_13, 0, 1, 1, 1);

        label_15 = new QLabel(gridLayoutWidget_3);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_4->addWidget(label_15, 2, 0, 1, 1);

        label_14 = new QLabel(gridLayoutWidget_3);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_4->addWidget(label_14, 0, 2, 1, 1);

        BtnSwitchTimeStampColor = new QPushButton(centralWidget);
        BtnSwitchTimeStampColor->setObjectName(QStringLiteral("BtnSwitchTimeStampColor"));
        BtnSwitchTimeStampColor->setGeometry(QRect(140, 110, 71, 61));
        RatBehaviorAnalysisClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RatBehaviorAnalysisClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 535, 26));
        RatBehaviorAnalysisClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RatBehaviorAnalysisClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        RatBehaviorAnalysisClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(RatBehaviorAnalysisClass);
        QObject::connect(recordingButton, SIGNAL(clicked()), RatBehaviorAnalysisClass, SLOT(recordingButtonClicked()));
        QObject::connect(backgroundButton, SIGNAL(clicked()), RatBehaviorAnalysisClass, SLOT(backgroundButtonClicked()));
        QObject::connect(backgroundSaveButton, SIGNAL(clicked()), RatBehaviorAnalysisClass, SLOT(backgroundSaveButtonClicked()));
        QObject::connect(calculationButton, SIGNAL(clicked()), RatBehaviorAnalysisClass, SLOT(calculationButtonClicked()));
        QObject::connect(BtnSwitch, SIGNAL(clicked()), RatBehaviorAnalysisClass, SLOT(BtnSwitchClicked()));
        QObject::connect(BtnTracking, SIGNAL(clicked()), RatBehaviorAnalysisClass, SLOT(BtnTrackingClicked()));
        QObject::connect(BtnLog, SIGNAL(clicked()), RatBehaviorAnalysisClass, SLOT(BtnLogClicked()));
        QObject::connect(BtnExpand, SIGNAL(clicked()), RatBehaviorAnalysisClass, SLOT(BtnExpandClicked()));
        QObject::connect(BtnSwitchTimeStampColor, SIGNAL(clicked()), RatBehaviorAnalysisClass, SLOT(BtnSwitchTimeStampColorClicked()));

        recordingFormat->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(RatBehaviorAnalysisClass);
    } // setupUi

    void retranslateUi(QMainWindow *RatBehaviorAnalysisClass)
    {
        RatBehaviorAnalysisClass->setWindowTitle(QApplication::translate("RatBehaviorAnalysisClass", "RatBehaviorAnalysis", 0));
        depthLabel->setText(QString());
        label->setText(QApplication::translate("RatBehaviorAnalysisClass", "Lower(mm)", 0));
        label_2->setText(QApplication::translate("RatBehaviorAnalysisClass", "Upper(mm)", 0));
        label_3->setText(QApplication::translate("RatBehaviorAnalysisClass", "<html><head/><body><p><span style=\" font-weight:600;\">Depth Sensor Range</span></p></body></html>", 0));
        groupBox->setTitle(QApplication::translate("RatBehaviorAnalysisClass", "Depth Display", 0));
        radioButton_Grayscale->setText(QApplication::translate("RatBehaviorAnalysisClass", "Grayscale", 0));
        radioButton_PseudoColor->setText(QApplication::translate("RatBehaviorAnalysisClass", "Pseudo Color", 0));
        label_5->setText(QApplication::translate("RatBehaviorAnalysisClass", "Trial Number", 0));
        label_4->setText(QApplication::translate("RatBehaviorAnalysisClass", "Recording Format", 0));
        recordingFormat->clear();
        recordingFormat->insertItems(0, QStringList()
         << QApplication::translate("RatBehaviorAnalysisClass", "Video", 0)
         << QApplication::translate("RatBehaviorAnalysisClass", "Raw Data", 0)
        );
        label_11->setText(QApplication::translate("RatBehaviorAnalysisClass", "FPS", 0));
        recordingFPS->clear();
        recordingFPS->insertItems(0, QStringList()
         << QApplication::translate("RatBehaviorAnalysisClass", "10", 0)
         << QApplication::translate("RatBehaviorAnalysisClass", "15", 0)
         << QApplication::translate("RatBehaviorAnalysisClass", "30", 0)
        );
        recordingButton->setText(QApplication::translate("RatBehaviorAnalysisClass", "Record", 0));
        groupBox_2->setTitle(QApplication::translate("RatBehaviorAnalysisClass", "Background Refer", 0));
        backgroundButton->setText(QApplication::translate("RatBehaviorAnalysisClass", "Get", 0));
        label_BGR->setText(QApplication::translate("RatBehaviorAnalysisClass", "Not Ready", 0));
        backgroundSaveButton->setText(QApplication::translate("RatBehaviorAnalysisClass", "Save", 0));
        groupBox_3->setTitle(QApplication::translate("RatBehaviorAnalysisClass", "Depth ROI Selection", 0));
        label_6->setText(QApplication::translate("RatBehaviorAnalysisClass", "Point 1", 0));
        label_8->setText(QApplication::translate("RatBehaviorAnalysisClass", "x", 0));
        label_9->setText(QApplication::translate("RatBehaviorAnalysisClass", "y", 0));
        label_7->setText(QApplication::translate("RatBehaviorAnalysisClass", "Point 2", 0));
        ROI_Off->setText(QApplication::translate("RatBehaviorAnalysisClass", "OFF", 0));
        ROI_On->setText(QApplication::translate("RatBehaviorAnalysisClass", "ON", 0));
        calculationButton->setText(QApplication::translate("RatBehaviorAnalysisClass", "Feature Calculation", 0));
        label_10->setText(QApplication::translate("RatBehaviorAnalysisClass", "Threshold", 0));
        BtnSwitch->setText(QApplication::translate("RatBehaviorAnalysisClass", "With\n"
"Color", 0));
        BtnLog->setText(QApplication::translate("RatBehaviorAnalysisClass", "Clear\n"
"Log", 0));
        BtnTracking->setText(QApplication::translate("RatBehaviorAnalysisClass", "Refresh\n"
"Tracking", 0));
        BtnExpand->setText(QApplication::translate("RatBehaviorAnalysisClass", "Expand>>>", 0));
        groupBox_4->setTitle(QApplication::translate("RatBehaviorAnalysisClass", "Color ROI Selection", 0));
        label_12->setText(QApplication::translate("RatBehaviorAnalysisClass", "Point 1", 0));
        label_13->setText(QApplication::translate("RatBehaviorAnalysisClass", "x", 0));
        label_15->setText(QApplication::translate("RatBehaviorAnalysisClass", "Point 2", 0));
        label_14->setText(QApplication::translate("RatBehaviorAnalysisClass", "y", 0));
        BtnSwitchTimeStampColor->setText(QApplication::translate("RatBehaviorAnalysisClass", "Switch\n"
"Time Color", 0));
    } // retranslateUi

};

namespace Ui {
    class RatBehaviorAnalysisClass: public Ui_RatBehaviorAnalysisClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RATBEHAVIORANALYSIS_H
