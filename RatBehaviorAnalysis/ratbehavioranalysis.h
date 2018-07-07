#ifndef RATBEHAVIORANALYSIS_H
#define RATBEHAVIORANALYSIS_H

#include <QtWidgets/QMainWindow>
#include "ui_ratbehavioranalysis.h"

#include <QMessageBox>
#include <QDate>
#include <QTime>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <QDir>
#include <QFile>
#include <QTextStream>

#include "Rat.h"

// OpenCV Header
#include <opencv2/opencv.hpp>

// Kinect for Windows SDK Header
#include <Kinect.h>

using namespace cv;

class RatBehaviorAnalysis : public QMainWindow
{
	Q_OBJECT

public:
	RatBehaviorAnalysis(QWidget *parent = 0);
	~RatBehaviorAnalysis();

	// Safe release for interfaces
	template<class Interface>
	inline void SafeRelease(Interface *& pInterfaceToRelease)	{
		if (pInterfaceToRelease != nullptr) {
			pInterfaceToRelease->Release();
			pInterfaceToRelease = nullptr;
		}
	}

	HRESULT initializeKinectSensor();
	void releaseKinectSensor();
	//Recording Functions
	void openRecordingFile();
	void closeRecordingFile();

	void doRecording();
	void doRecordingColor();
	void doBGR();
	void doCalculation();

	//FeatureExtraction Functions
	void printRatInfo(RAT rat_);
	float calculateAngle(Point center, Point nose, Point tail);
	void featureExtraction(Mat &result, Mat &result8bit, RAT &rat_);

private slots:
	void updateImageStream();
	void recordingButtonClicked();
	void backgroundButtonClicked();
	void backgroundSaveButtonClicked();
	void calculationButtonClicked();
	void BtnSwitchClicked();
	void BtnLogClicked();
	void BtnTrackingClicked();
	void BtnExpandClicked();
	void BtnSwitchTimeStampColorClicked();

private:
	Ui::RatBehaviorAnalysisClass ui;

	//Kinect - Sensor
	IKinectSensor* pSensor;

	//Kinect - Depth
	IDepthFrameReference* pDepthFrameRef;
	IDepthFrame* pDepthFrame;
	bool isDepthUpdating;

	//Kinect - Infrared
	IInfraredFrameReference* pInfraredFrameRef;
	IInfraredFrame* pInfraredFrame;
	bool isInfraredUpdating;

	//Kinect - Color
	IColorFrameReference* pColorFrameRef;
	IColorFrame* pColorFrame;
	bool isColorUpdating;

	//Kinect - MultiSource
	IMultiSourceFrameReader* pReader;   // Kinect data source
	IMultiSourceFrame* pFrame;

	//mm to grayscale
	double dAlpha;
	double dBeta;

	//depth image
	Mat mDepthImg;
	Mat mDepthImgFlipped;
	Mat mDepthImgSaved;
	Mat mImg8bit;
	Mat mImg8bitSaved;
	int BasicCounter;

	//infrared image
	Mat mInfraredImg;
	Mat mInfraredImgFlipped;
	Mat mInfraredImgSaved;
	Mat mImg8bit_IR;
	Mat mImg8bitSaved_IR;
	int BasicCounterIR;

	//color image
	Mat	mColorImg;
	Mat colorImgResized;
	Mat colorImgSaved;
	int BasicCounterColor;
	int timeStampColorCounter;

	//QT imge display
	QImage qColorImage;
	QImage qDepthImage;

	//Background Reference
	Mat mDepthRefer;
	Mat refer8bit;
	Mat refer_sum;
	bool isGettingBGR;
	int BGR_counter;

	//Recording
	int recording_FPS;
	int recording_frame_update;
	bool isRecording;
	int depthFrameRecordingCounter;
	VideoWriter outputVideo;
	VideoWriter outputVideoColor;
	QString timeNow_AVI;
	QString timeNow_FOLDER;

	//Feature Calculation
	bool isCalculating;
	QFile *trackingFile;
	QTextStream trackingStream;

	//Crop config file
	QFile *configFile;
	QTextStream configStream;

	RAT rat;
	bool rat_detected;

	QTimer * depthTimer;
};

#endif // RATBEHAVIORANALYSIS_H
