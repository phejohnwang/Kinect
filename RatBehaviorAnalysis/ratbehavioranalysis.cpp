//Kinect v2 recording tool used for rodent behavioral research
// Time stamps added on color frame
// Depth recording: 1) raw 16 bit depth image  (preferred) 2) converted 8 bit grayscale video 
// Color recording: color video with extracted roi
// Color_resize currently disabled for efficiency
// Cropping supported for both
// Images are flipped for recording use
// Output tracking results in txt file (Currently disabled)
// Changable FPS 10/15/30
//	Switched to MultiSourceFrameReader for 100% sync
//	Recording appears under ./Video
//	Depth range 000,000~999,999
//	Read config.txt for cropping info
//  Averaging in BGRefer cancelled for stability
//  Recording raw IR stream as video / frames
//  Next: enable crop for IR stream

#include "ratbehavioranalysis.h"

#define DEPTHMS 1
#define TRACKING false
#define CONFIG true

//scale factor used to resize the color frame
#define COLOR_RESIZE 1		//2-960*540 3-640*360

const int iWidth = 512;
const int iHeight = 424;

const int iColorWidth = 1920;
const int iColorHeight = 1080;

const double timestampColor1[5] = {255, 0, 255, 0, 0};
const double timestampColor2[5] = {255, 0, 0, 255, 0 };
const double timestampColor3[5] = {255, 0, 0, 0, 255 };

RatBehaviorAnalysis::RatBehaviorAnalysis(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	mDepthImg.create(iHeight, iWidth, CV_16UC1);
	mImg8bit.create(iHeight, iWidth, CV_8UC1);

	mInfraredImg.create(iHeight, iWidth, CV_16UC1);
	mImg8bit_IR.create(iHeight, iWidth, CV_8UC1);

	mColorImg.create(iColorHeight, iColorWidth, CV_8UC4);
	//colorImgResized.create(iColorHeight / COLOR_RESIZE, iColorWidth / COLOR_RESIZE, CV_8UC4);

	BasicCounter = 1;
	//BasicCounterColor = 1;

	isGettingBGR = false;

	isRecording = false;
	depthFrameRecordingCounter = 1;
	//Set trial number to 1~1000
	ui.lineEdit_TrialNo->setValidator(new QIntValidator(1, 1000, this));
	ui.lineEdit_TrialNo->setText("1");

	isCalculating = false;
	rat_detected = false;

	ui.featureDisplay->setReadOnly(true);

	isDepthUpdating = false;
	isColorUpdating = true;
	isInfraredUpdating = false;

	ui.depthLabel->setAlignment(Qt::AlignCenter);

	ui.recordingFPS->setCurrentIndex(2);

	recording_FPS = 30;
	recording_frame_update = 1;

	timeStampColorCounter = 0;

	this->setFixedSize(QSize::QSize(240, 480));

	if (TRACKING) {
		QString trackingFileName = QString("tracking_%1.txt").arg(QTime::currentTime().toString("hh_mm_ss_zzz"));
		trackingFile = new QFile(trackingFileName);
		if (trackingFile->open(QIODevice::WriteOnly | QFile::Text)) {
			trackingStream.setDevice(trackingFile);
			trackingStream << "Tracking Start" << endl;
		}
	}

	//Read config.txt
	if (CONFIG) {
		QString configFileName = QString("./Config/config.txt");
		configFile = new QFile(configFileName);
		if (configFile->open(QIODevice::ReadOnly | QFile::Text)) {
			configStream.setDevice(configFile);
			// load depth/infrared format, 0 = video, 1 = raw
			int format_indicator;
			configStream >> format_indicator;
			ui.recordingFormat->setCurrentIndex(format_indicator);
			// load depth range info
			int lb, ub;
			configStream >> lb >> ub;
			ui.lowerBound->setValue(lb);
			ui.upperBound->setValue(ub);
			// load cropping info - color
			int crop_point_x, crop_point_y;
			configStream >> crop_point_x >> crop_point_y;
			ui.color_point1_x->setValue(crop_point_x);
			ui.color_point1_y->setValue(crop_point_y);
			configStream >> crop_point_x >> crop_point_y;
			ui.color_point2_x->setValue(crop_point_x);
			ui.color_point2_y->setValue(crop_point_y);
			// load cropping info - depth
			int check_indicator;
			configStream >> check_indicator;
			// 0 for OFF, 1 for ON, 2 for Infrared
			if (check_indicator == 0) {
				isDepthUpdating = true;
				ui.ROI_Off->setChecked(true);
			}
			else if (check_indicator == 1) {
				isDepthUpdating = true;
				ui.ROI_On->setChecked(true);
				configStream >> crop_point_x >> crop_point_y;
				ui.point1_x->setValue(crop_point_x);
				ui.point1_y->setValue(crop_point_y);
				configStream >> crop_point_x >> crop_point_y;
				ui.point2_x->setValue(crop_point_x);
				ui.point2_y->setValue(crop_point_y);
			}
			else if (check_indicator == 2) {
				isInfraredUpdating = true;
			}
			// close
			configFile->close();
		}
		else {
			QMessageBox::about(this, tr("Warning"), tr("No config file found!"));
		}
	}

	initializeKinectSensor();

	depthTimer = new QTimer();
	connect(depthTimer, SIGNAL(timeout()), this, SLOT(updateImageStream()));
	depthTimer->start(DEPTHMS);
}

RatBehaviorAnalysis::~RatBehaviorAnalysis()
{
	releaseKinectSensor();
}

HRESULT RatBehaviorAnalysis::initializeKinectSensor()
{
	HRESULT hr;

	// 1a. Get default Sensor
	hr = GetDefaultKinectSensor(&pSensor);
	if (FAILED(hr)) return hr;

	if (pSensor == nullptr) {
		QMessageBox::information(nullptr, QStringLiteral("Fail"), QStringLiteral("Can't Find Kinect Sensor"), QMessageBox::Yes);
		return E_FAIL;
	}

	// 1b. Open sensor
	hr = pSensor->Open();

	// 2a. Get frame source
	//if (SUCCEEDED(hr)) {
	//	hr = pSensor->get_DepthFrameSource(&pFrameSource);
	//	hr = pSensor->get_ColorFrameSource(&pColorFrameSource);
	//}

	// 3a. Get frame reader
	if (SUCCEEDED(hr)) {
		//hr = pFrameSource->OpenReader(&pFrameReader);
		//hr = pColorFrameSource->OpenReader(&pColorFrameReader);
		if (isDepthUpdating) {
			hr = pSensor->OpenMultiSourceFrameReader(
				FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color,
				&pReader);
		}
		else if (isInfraredUpdating) {
			hr = pSensor->OpenMultiSourceFrameReader(
				FrameSourceTypes::FrameSourceTypes_Color | FrameSourceTypes::FrameSourceTypes_Infrared,
				&pReader);
		}
	}

	if (FAILED(hr)) return E_FAIL;
	
	return hr;
}

void RatBehaviorAnalysis::releaseKinectSensor()
{
	SafeRelease(pReader);
	if (pSensor != nullptr) pSensor->Close();
	SafeRelease(pSensor);
}

void RatBehaviorAnalysis::updateImageStream()
{
	pFrame = nullptr;
	if (pReader->AcquireLatestFrame(&pFrame) != S_OK) return;

	//update Depth image
	if (isDepthUpdating ) {
		pDepthFrame = nullptr;
		pDepthFrameRef = nullptr;

		if (pFrame->get_DepthFrameReference(&pDepthFrameRef) == S_OK) {
			// grayscale conversion
			double upper = ui.upperBound->value();
			double lower = ui.lowerBound->value();
			dAlpha = -255.0f / (upper - lower);
			dBeta = 255.0f * upper / (upper - lower);

			if (pDepthFrameRef->AcquireFrame(&pDepthFrame) == S_OK) {
				unsigned int bufferSize = iWidth * iHeight * sizeof(unsigned short);

				// 4c. copy the depth map to image
				//if (pFrame->CopyFrameDataToArray(iWidth * iHeight, reinterpret_cast<UINT16*>(mDepthImg.data)) == S_OK)
				if (pDepthFrame->AccessUnderlyingBuffer(&bufferSize, reinterpret_cast<UINT16**>(&mDepthImg.data)) == S_OK) 	{
					flip(mDepthImg, mDepthImgFlipped, 1);
					// 4d. convert from 16bit to 8bit
					mDepthImgFlipped.convertTo(mImg8bit, CV_8U, dAlpha, dBeta);
					//flip(mImg8bit, mImg8bit, 1);

					//ui.featureDisplay->setText(QString::number(mDepthImgFlipped.channels()));

					//ROI Selection
					if (ui.ROI_On->isChecked()) {
						//	rectangle(mImg8bit, Point(ui.point1_x->value(), ui.point1_y->value()), Point(ui.point2_x->value(), ui.point2_y->value()), Scalar::all(0), 2, 8);
						mImg8bitSaved = mImg8bit(Rect(Point(ui.point1_x->value(), ui.point1_y->value()), Point(ui.point2_x->value(), ui.point2_y->value())));
					}
					else {
						mImg8bitSaved = mImg8bit.clone();
					}

					//Adding Time Stamp for each frame
					/*QDate qdateObj = QDate::currentDate();
					QTime qtimeObj = QTime::currentTime();

					QString timeCurrentFrame = QString("%1-%2-%3 %4:%5:%6.%7")
						.arg(qdateObj.year(), 4, 10, QLatin1Char('0'))
						.arg(qdateObj.month(), 2, 10, QLatin1Char('0'))
						.arg(qdateObj.day(), 2, 10, QLatin1Char('0'))
						.arg(qtimeObj.hour(), 2, 10, QLatin1Char('0'))
						.arg(qtimeObj.minute(), 2, 10, QLatin1Char('0'))
						.arg(qtimeObj.second(), 2, 10, QLatin1Char('0'))
						.arg(qtimeObj.msec(), 3, 10, QLatin1Char('0'));*/

					//ui.featureDisplay->clear();
					//ui.featureDisplay->append(timeCurrentFrame);
					//putText(mImg8bitSaved, timeCurrentFrame.toStdString(), Point(10, 15), CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0), 1);
					//imshow("cropped", mImg8bitSaved);
					//Display
					//imshow("Depth Image", mImg8bitSaved);
					if (ui.radioButton_Grayscale->isChecked()) {
						//QImage qDepthImage = QImage((const unsigned char*)(mImg8bitSaved.data), mImg8bitSaved.cols, mImg8bitSaved.rows, mImg8bitSaved.step, QImage::Format_Indexed8);
						//ui.depthLabel->setPixmap(QPixmap::fromImage(qDepthImage));
						imshow("Depth Image", mImg8bitSaved);
					}
					else {
						//Color Mapping
						Mat depthToColor;
						applyColorMap(mImg8bitSaved, depthToColor, COLORMAP_JET);
						cvtColor(depthToColor, depthToColor, CV_BGR2RGB);
						//qDepthImage = QImage((const unsigned char*)(depthToColor.data), depthToColor.cols, depthToColor.rows, depthToColor.step, QImage::Format_RGB888);
						//ui.depthLabel->setPixmap(QPixmap::fromImage(qDepthImage));
						imshow("Depth Image", depthToColor);
					}
				}
				// 4e. release frame
				pDepthFrame->Release();
			}
		
			pDepthFrameRef->Release();
		}
	}

	//update IR image, no need to ROI cut as IR image will be recorded as 8 bit Video or 16 bit raw (replacing depth stream)
	if (isInfraredUpdating) {
		pInfraredFrame = nullptr;
		pInfraredFrameRef = nullptr;

		if (pFrame->get_InfraredFrameReference(&pInfraredFrameRef) == S_OK) {
			// grayscale conversion
			double upper = ui.upperBound->value();
			double lower = ui.lowerBound->value();
			//lower = 4000.0;
			//upper = 14000;
			//dAlpha = -255.0f / (upper - lower);
			//dBeta = 255.0f * upper / (upper - lower);
			dAlpha = 255.0 / 20000;
			dBeta = 0.0;

			if (pInfraredFrameRef->AcquireFrame(&pInfraredFrame) == S_OK) {
				unsigned int bufferSize = iWidth * iHeight * sizeof(unsigned short);
				if (pInfraredFrame->AccessUnderlyingBuffer(&bufferSize, reinterpret_cast<UINT16**>(&mInfraredImg.data)) == S_OK) {
					flip(mInfraredImg, mInfraredImgFlipped, 1);
					// 4d. convert from 16bit to 8bit
					//imwrite("1.png", mInfraredImgFlipped);
					mInfraredImgFlipped.convertTo(mImg8bit_IR, CV_8U, dAlpha, dBeta);
					imshow("Infrared Image", mImg8bit_IR);
				}
				pInfraredFrame->Release();
			}
			pInfraredFrameRef->Release();
		}
	}

	//update Color image
	if (isColorUpdating) {
		pColorFrame = nullptr;
		pColorFrameRef = nullptr;

		if (pFrame->get_ColorFrameReference(&pColorFrameRef) == S_OK) {
			if (pColorFrameRef->AcquireFrame(&pColorFrame) == S_OK) {
				UINT uBufferSizeColor = iColorHeight * iColorWidth * 4 * sizeof(BYTE);
				// 4c. Copy to OpenCV image
				if (pColorFrame->CopyConvertedFrameDataToArray(uBufferSizeColor, mColorImg.data, ColorImageFormat_Bgra) == S_OK) {
					//cv::resize(mColorImg, colorImgResized, Size(iColorWidth / COLOR_RESIZE, iColorHeight / COLOR_RESIZE));

					flip(mColorImg, mColorImg, 1);
					//colorImgSaved = colorImgResized(Rect(Point(COLOR_ROI_F_X, COLOR_ROI_F_Y), Point(COLOR_ROI_S_X, COLOR_ROI_S_Y)));
					colorImgSaved = mColorImg(Rect(Point(ui.color_point1_x->value(), ui.color_point1_y->value()), Point(ui.color_point2_x->value(), ui.color_point2_y->value())));

					//Adding Time Stamp for each frame
					QDate qdateObj = QDate::currentDate();
					QTime qtimeObj = QTime::currentTime();

					QString timeCurrentFrame = QString("%1-%2-%3 %4:%5:%6.%7")
						.arg(qdateObj.year(), 4, 10, QLatin1Char('0'))
						.arg(qdateObj.month(), 2, 10, QLatin1Char('0'))
						.arg(qdateObj.day(), 2, 10, QLatin1Char('0'))
						.arg(qtimeObj.hour(), 2, 10, QLatin1Char('0'))
						.arg(qtimeObj.minute(), 2, 10, QLatin1Char('0'))
						.arg(qtimeObj.second(), 2, 10, QLatin1Char('0'))
						.arg(qtimeObj.msec(), 3, 10, QLatin1Char('0'));

					//ui.featureDisplay->clear();
					//ui.featureDisplay->append(timeCurrentFrame);
					putText(colorImgSaved, timeCurrentFrame.toStdString(), Point(50, 50), CV_FONT_HERSHEY_COMPLEX, 0.5, 
						Scalar(timestampColor1[timeStampColorCounter], timestampColor2[timeStampColorCounter], timestampColor3[timeStampColorCounter]), 1);
					imshow("Color Image", colorImgSaved);
					//moveWindow("Color Image", 0, 200);
					//ui.featureDisplay->setText(QString::number(colorImgSaved.channels()));
					//qColorImage = QImage((const unsigned char*)(colorImgSaved.data), colorImgSaved.cols, colorImgSaved.rows, colorImgSaved.step, QImage::Format_ARGB32);
					//ui.depthLabel->setPixmap(QPixmap::fromImage(qColorImage));
				}
				// 4e. release frame
				pColorFrame->Release();
			}
			pColorFrameRef->Release();
		}
	}

	//Check Processing Every 3 Frames (10Hz) or 1 Frame (30Hz)
	//ui.featureDisplay->append(QString::number(BasicCounter));
	if (BasicCounter >= recording_frame_update) {
		//Check recording
		if (isRecording) {
			doRecording();
			doRecordingColor();
		}
		//Check Background
		if (isGettingBGR) doBGR();
		//Check feature calculation
		if (isCalculating) doCalculation();

		BasicCounter = 1;
	}
	else
		BasicCounter++;

	SafeRelease(pFrame);
}

void RatBehaviorAnalysis::doRecording()
{
	if (ui.recordingFormat->currentIndex() == 0) {
		if (isDepthUpdating)
			outputVideo << mImg8bitSaved;
		else if (isInfraredUpdating)
			outputVideo << mImg8bit_IR;
	}
	else if (ui.recordingFormat->currentIndex() == 1) {
		QString filename = QString("%1/Sample%2.png")
			.arg(timeNow_FOLDER)
			.arg(depthFrameRecordingCounter, 6, 10, QLatin1Char('0'));

		if (isDepthUpdating) {
			if (ui.ROI_On->isChecked()) {
				mDepthImgSaved = mDepthImgFlipped(Rect(Point(ui.point1_x->value(), ui.point1_y->value()), Point(ui.point2_x->value(), ui.point2_y->value())));
				imwrite(filename.toStdString(), mDepthImgSaved);
			}
			else {
				imwrite(filename.toStdString(), mDepthImgFlipped);
			}
		}
		else if (isInfraredUpdating) {
			imwrite(filename.toStdString(), mInfraredImgFlipped);
		}

		depthFrameRecordingCounter++;

		if (depthFrameRecordingCounter >= 999999)	//555 min = 33,333s = 999,999 frames @ 30 FPS
			depthFrameRecordingCounter = 1;
	}
}

void RatBehaviorAnalysis::doRecordingColor()
{
	if (isColorUpdating) {
		cvtColor(colorImgSaved, colorImgSaved, COLOR_BGRA2BGR);
		outputVideoColor << colorImgSaved;
	}
}

void RatBehaviorAnalysis::doBGR()
{	//Acquire the 16 bit background image
	// Do grayscale averaging on background dephth images
	//if (BGR_counter >= 10) {
	//	refer_sum += mDepthImg;
	//	//add(refer_sum, mDepthImg, refer_sum);
	//	mDepthRefer = refer_sum / BGR_counter;
	//	//mDepthRefer = mDepthImg.clone();
	//	flip(mDepthRefer, mDepthRefer, 1);
	//	mDepthRefer.convertTo(refer8bit, CV_8U, dAlpha, dBeta);
	//	isGettingBGR = false;
	//	ui.label_BGR->setText("Ready");
	//	imshow("Refer", refer8bit);
	//}	else {
	//	refer_sum += mDepthImg;
	//	//add(refer_sum, mDepthImg, refer_sum);
	//	BGR_counter++;
	//	ui.label_BGR->setText(QString::number(BGR_counter));
	//}
	mDepthRefer = mDepthImgFlipped.clone();
	mDepthRefer.convertTo(refer8bit, CV_8U, dAlpha, dBeta);
	isGettingBGR = false;
	ui.label_BGR->setText("Ready");
	imshow("Refer", refer8bit);
}

void RatBehaviorAnalysis::doCalculation()
{
	//Currently disabled
	//Mat result, result8bit;
	//featureExtraction(result, result8bit, rat);
	//printRatInfo(rat);
}

void RatBehaviorAnalysis::openRecordingFile()
{
	//Determine frame rate selected by the user
	if (ui.recordingFPS->currentIndex() == 0) {
		recording_FPS = 10;
		recording_frame_update = 3;
	}
	else if (ui.recordingFPS->currentIndex() == 1) {
		recording_FPS = 15;
		recording_frame_update = 2;
	}
	else {
		recording_FPS = 30;
		recording_frame_update = 1;
	}

	//Acquire current date & time
	QDate qdateObj = QDate::currentDate();
	QTime qtimeObj = QTime::currentTime();

	if (ui.recordingFormat->currentIndex() == 0) {
		timeNow_AVI = QString("./Video/Trial_%1_%2_%3_%4_%5_%6_%7.avi")
			.arg(ui.lineEdit_TrialNo->text())
			.arg(qdateObj.year(), 4, 10, QLatin1Char('0'))
			.arg(qdateObj.month(), 2, 10, QLatin1Char('0'))
			.arg(qdateObj.day(), 2, 10, QLatin1Char('0'))
			.arg(qtimeObj.hour(), 2, 10, QLatin1Char('0'))
			.arg(qtimeObj.minute(), 2, 10, QLatin1Char('0'))
			.arg(qtimeObj.second(), 2, 10, QLatin1Char('0'));

		if (isDepthUpdating)
			outputVideo.open(timeNow_AVI.toStdString(), CV_FOURCC('M', 'J', 'P', 'G'), recording_FPS, mImg8bitSaved.size(), false);
		else if (isInfraredUpdating)
			outputVideo.open(timeNow_AVI.toStdString(), CV_FOURCC('M', 'J', 'P', 'G'), recording_FPS, mImg8bit_IR.size(), false);
	}
	else if (ui.recordingFormat->currentIndex() == 1) {
		timeNow_FOLDER = QString("./Video/Trial_%1_%2_%3_%4_%5_%6_%7")
			.arg(ui.lineEdit_TrialNo->text())
			.arg(qdateObj.year(), 4, 10, QLatin1Char('0'))
			.arg(qdateObj.month(), 2, 10, QLatin1Char('0'))
			.arg(qdateObj.day(), 2, 10, QLatin1Char('0'))
			.arg(qtimeObj.hour(), 2, 10, QLatin1Char('0'))
			.arg(qtimeObj.minute(), 2, 10, QLatin1Char('0'))
			.arg(qtimeObj.second(), 2, 10, QLatin1Char('0'));
		QDir().mkdir(timeNow_FOLDER);
	}
	if (isColorUpdating) {
		timeNow_AVI = QString("./Video/Trial_%1_%2_%3_%4_%5_%6_%7_Color.avi")
			.arg(ui.lineEdit_TrialNo->text())
			.arg(qdateObj.year(), 4, 10, QLatin1Char('0'))
			.arg(qdateObj.month(), 2, 10, QLatin1Char('0'))
			.arg(qdateObj.day(), 2, 10, QLatin1Char('0'))
			.arg(qtimeObj.hour(), 2, 10, QLatin1Char('0'))
			.arg(qtimeObj.minute(), 2, 10, QLatin1Char('0'))
			.arg(qtimeObj.second(), 2, 10, QLatin1Char('0'));

		//outputVideoColor.open(timeNow_AVI.toStdString(), CV_FOURCC('M', 'J', 'P', 'G'), recording_FPS, colorImgSaved.size(), true);
		outputVideoColor.open(timeNow_AVI.toStdString(), CV_FOURCC('M', 'P', '4', '2'), recording_FPS, colorImgSaved.size(), true);
	}
	isRecording = true;
	depthFrameRecordingCounter = 1;

	//ui.featureDisplay->append(QString::number(ui.recordingFPS->currentIndex()));
	//ui.featureDisplay->append(QString::number(int(isRecording)));
}

void RatBehaviorAnalysis::closeRecordingFile()
{
	isRecording = false;

	if (isColorUpdating) {
		outputVideoColor.release();
	}

	if (ui.recordingFormat->currentIndex() == 0) {
		outputVideo.release();
	}
}

void RatBehaviorAnalysis::recordingButtonClicked()
{
	if (isRecording) {
		closeRecordingFile();
		ui.recordingButton->setText("Record");
		ui.recordingFormat->setEnabled(true);
		ui.recordingFPS->setEnabled(true);
		ui.BtnSwitch->setEnabled(true);
	}
	else {
		ui.recordingFormat->setDisabled(true);
		ui.recordingFPS->setDisabled(true);
		ui.BtnSwitch->setDisabled(true);
		openRecordingFile();
		ui.recordingButton->setText("Stop");
	}
}

void RatBehaviorAnalysis::backgroundButtonClicked()
{
	ui.label_BGR->setText("Processing...");
	//BGR_counter = 1;
	//refer_sum = Mat::zeros(424, 512, CV_16UC1);
	isGettingBGR = true;	
}

void RatBehaviorAnalysis::backgroundSaveButtonClicked()
{
	if (ui.label_BGR->text() == "Ready") {
		QDate qdateObj = QDate::currentDate();
		QTime qtimeObj = QTime::currentTime();

		QString timeNow_BGR = QString("./Background/Refer_%1_%2_%3_%4_%5_%6_%7.png")
			.arg(ui.lineEdit_TrialNo->text())
			.arg(qdateObj.year(), 4, 10, QLatin1Char('0'))
			.arg(qdateObj.month(), 2, 10, QLatin1Char('0'))
			.arg(qdateObj.day(), 2, 10, QLatin1Char('0'))
			.arg(qtimeObj.hour(), 2, 10, QLatin1Char('0'))
			.arg(qtimeObj.minute(), 2, 10, QLatin1Char('0'))
			.arg(qtimeObj.second(), 2, 10, QLatin1Char('0'));
		imwrite(timeNow_BGR.toStdString(), mDepthRefer);
	}
}

void RatBehaviorAnalysis::calculationButtonClicked()
{
	if (isCalculating) {
		ui.calculationButton->setText("Calculation");
		isCalculating = false;
	}
	else {
		if (ui.label_BGR->text() == "Ready") {
			isCalculating = true;
			ui.calculationButton->setText("Stop");
		}
		else {
			ui.featureDisplay->append("Background image not ready!");
		}
	}
}

void RatBehaviorAnalysis::printRatInfo(RAT rat_)
{
	ui.featureDisplay->clear();

	if (rat_detected) {
		QString ratInfo = QString("Rat #: %1 \tCenter: (%2, %3)\tNose: (%4, %5), Tail: (%6, %7)\tSpeed: %8")
			.arg(rat_.id)
			.arg(rat_.centroid.x)
			.arg(rat_.centroid.y)
			.arg(rat_.nose.x)
			.arg(rat_.nose.y)
			.arg(rat_.tail.x)
			.arg(rat_.tail.y)
			.arg(rat_.speed);
		//QString ratInfo = QString("Center: (%1, %2)\tSpeed: %3")
		//	.arg(rat_.centroid.x)
		//	.arg(rat_.centroid.y)
		//	.arg(rat_.speed);

		ui.featureDisplay->append(ratInfo);
		if (TRACKING) {
			trackingStream << QTime::currentTime().toString("hh.mm.ss.zzz") << " " << ratInfo << endl;
		}
	}
	else {
		ui.featureDisplay->append("No Rat Detected!");
	}
	//cout << "Rotational Angle: " << rat_.NCT_Angle << endl;
	//cout << "Length: " << rat_.mouselength << "\tWidth: " << rat_.mousewidth << "\nEllipticity: " << rat_.ellipticity << endl;
	//cout << "Body Area: " << rat_.bodyarea << "\nBody Radius: " << rat_.bodyradius << endl;
	//cout << "Circularity: " << rat_.circularity << endl;
	//cout << "Vx: " << rat_.vx << "\tVy: " << rat_.vy << "\tSpeed: " << rat_.speed << endl;
}

float RatBehaviorAnalysis::calculateAngle(Point center, Point nose, Point tail)
{
	float aa, bb, cc, tmp, angle;
	aa = (center.x - nose.x) * (center.x - nose.x) + (center.y - nose.y) * (center.y - nose.y);
	bb = (center.x - tail.x) * (center.x - tail.x) + (center.y - tail.y) * (center.y - tail.y);
	cc = (nose.x - tail.x) * (nose.x - tail.x) + (nose.y - tail.y) * (nose.y - tail.y);

	tmp = 2 * sqrtf(aa * bb);
	tmp = (aa + bb - cc) / tmp;

	angle = acosf(tmp);

	return (angle * 180 / 3.1415926);
}

void RatBehaviorAnalysis::featureExtraction(Mat &result, Mat &result8bit, RAT &rat_)
{  // old version code
	Point cage_origin_d = Point(ui.point1_x->value(), ui.point1_y->value());
	Point cage_end_d = Point(ui.point2_x->value(), ui.point2_y->value());

	vector<vector<Point>> contours_body;
	vector<Vec4i>hierarchy;
	Point2f vertices[4];

	//Calculates the difference between depth image and its reference
	//subtract(img_ref, img, result);
	subtract(mDepthRefer, mDepthImg, result);

	//Nose filter - Choose Median Filter over Gaussian Filter & Bilateral Filter
	medianBlur(result, result, 5);

	//ROI Extraction
	result.convertTo(result8bit, CV_8U);	//16 bit to 8 bit
	Mat roi(result8bit, Rect(cage_origin_d, cage_end_d));
	Mat roi_show(mImg8bit, Rect(cage_origin_d, cage_end_d));
	//imshow("ROI", roi);

	//THRESH_BINARY
	threshold(roi, roi, ui.threshold->value(), 255, THRESH_BINARY);
	//imshow("ROI_binary", roi);

	//----------------------Find rat shape-----------------------------------//

	findContours(roi, contours_body, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//Contour filtering based on area
	for (int i = 0; i < contours_body.size(); i++)
	{
		if (contourArea(Mat(contours_body[i])) <= 200)
			drawContours(roi, contours_body, i, 0, -1, 8, hierarchy, 0, Point());
		else
			drawContours(roi, contours_body, i, 255, -1, 8, hierarchy, 0, Point());
	}
	//imshow("ROI_binary", roi);

	Mat roi_body = roi.clone();
	erode(roi_body, roi_body, Mat(), Point(-1, -1), 3);
	dilate(roi_body, roi_body, Mat(), Point(-1, -1), 3);
	//erode(roi_body, roi_body, Mat(), Point(-1, -1), 5);

	//imshow("roi_body", roi_body);
	//imwrite("roi_body.jpg", roi_body);

	//Rat Body Shape Contour
	findContours(roi_body, contours_body, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	//Check if the rat is in the cage
	if (contours_body.size()  < 1) {
		rat_detected = false;
		return;		//rat data remains same as last frame (if rat is detected last frame)
	}
	else if (contours_body.size() == 1) {
		rat_.id = 0;
	}
	else if (contours_body.size() > 1) {
		//take the contour with the largest area as	rat body shape
		int maxId = 0;
		int maxArea = -1;

		for (int i = 0; i < contours_body.size(); i++) {
			if (contourArea(Mat(contours_body[i])) > maxArea) {
				maxId = i;
				maxArea = contourArea(Mat(contours_body[i]));
			}
		}
		rat_.id = maxId;
	}

	vector<Moments>mu(1);
	vector<RotatedRect> minEllipse(1);

	//Centroid calculation
	mu[0] = moments(Mat(contours_body[rat_.id]), false);

	int center_x = (int)(mu[0].m10 / mu[0].m00);
	int center_y = (int)(mu[0].m01 / mu[0].m00);
	circle(roi_show, Point(center_x, center_y), 3, 0, -1, 8, 0);

	//Calculate the speed of centroid
	if (rat_detected) {
		rat_.vx = center_x + cage_origin_d.x - rat_.centroid.x;
		rat_.vy = center_y + cage_origin_d.y - rat_.centroid.y;
		rat_.speed = rat_.vx * rat_.vx + rat_.vy * rat_.vy;
		rat_.speed = sqrt(rat_.speed);
	}
	else {
		rat_.vx = 0;
		rat_.vy = 0;
		rat_.speed = 0;
	}

	//Coordinates transformation
	rat_.centroid.x = center_x + cage_origin_d.x;
	rat_.centroid.y = center_y + cage_origin_d.y;

	//Ellipse fitting & display
	minEllipse[0] = fitEllipse(Mat(contours_body[rat_.id]));
	ellipse(roi_show, minEllipse[0], 255, 1, 8);
	minEllipse[0].points(vertices);
	for (int j = 0; j < 4; j++)
		line(roi_show, vertices[j], vertices[(j + 1) % 4], 255);

	//----------------------Feature calculation-----------------------------------//

	//Body area S is computed simply by summing all the pixels of the body region.
	rat_.bodyarea = contourArea(Mat(contours_body[rat_.id]));

	//Body radius R(the longest distance between centroid and rat body contour).
	//And nose is the point in rat shape that has the longest distance with the centroid
	rat_.bodyradius = 0;
	rat_.nose.x = rat_.nose.y = 0;
	double compare;
	for (int j = 0; j < contours_body[rat_.id].size(); j++)  {
		compare = (contours_body[rat_.id][j].x - center_x) * (contours_body[rat_.id][j].x - center_x)
			+ (contours_body[rat_.id][j].y - center_y) * (contours_body[rat_.id][j].y - center_y);
		if (rat_.bodyradius < compare) {
			rat_.bodyradius = compare;
			rat_.nose = contours_body[rat_.id][j];
		}
	}
	rat_.bodyradius = sqrt(rat_.bodyradius);

	//Ellipticity ¦Ñ(fitted ellipse from rat body contour) with mouse length and width
	rat_.mouselength = minEllipse[0].size.height;
	rat_.mousewidth = minEllipse[0].size.width;
	rat_.ellipticity = minEllipse[0].size.height / minEllipse[0].size.width;

	//+The circularity E is the square proportion relationship exists between S and R.
	// S / (R^2)
	//different from the definition in wiki-> (4*Pi*S) / (Perimeter ^2)
	rat_.circularity = rat_.bodyarea / (rat_.bodyradius * rat_.bodyradius);

	//Get tail, the point in rat shape that has the largest distance sum 
	//	 distance sum = distance with the nose point + distance with the centroid point
	rat_.tail.x = rat_.tail.y = 0;
	double point2nose = 0;
	double point2centroid = 0;
	double pointSum;
	double maxSum = 0;
	double compare_tail;
	for (int j = 0; j < contours_body[rat_.id].size(); j++)  {
		point2nose = (contours_body[rat_.id][j].x - rat_.nose.x) * (contours_body[rat_.id][j].x - rat_.nose.x)
			+ (contours_body[rat_.id][j].y - rat_.nose.y) * (contours_body[rat_.id][j].y - rat_.nose.y);
		point2centroid = (contours_body[rat_.id][j].x - center_x) * (contours_body[rat_.id][j].x - center_x)
			+ (contours_body[rat_.id][j].y - center_y) * (contours_body[rat_.id][j].y - center_y);
		pointSum = sqrt(point2nose) + sqrt(point2centroid);
		//pointSum = point2nose + point2centroid;

		if (maxSum < pointSum) {
			rat_.tail = contours_body[rat_.id][j];
			maxSum = pointSum;
		}
	}

	rat_detected = true;

	//Show nose & tail in color
	Mat img_nose;
	cv::resize(roi_show, img_nose, Size(0, 0), 2, 2);
	cvtColor(img_nose, img_nose, CV_GRAY2RGB);

	circle(img_nose, 2 * rat_.tail, 5, Scalar(0, 255, 0), -1, 8, 0);
	circle(img_nose, 2 * rat_.nose, 5, Scalar(0, 0, 255), -1, 8, 0);
	line(img_nose, 2 * rat_.nose, 2 * Point(center_x, center_y), Scalar(255, 0, 0), 5);
	line(img_nose, 2 * rat_.tail, 2 * Point(center_x, center_y), Scalar(255, 0, 0), 5);

	//Coordinates transformation
	rat_.tail.x += cage_origin_d.x;
	rat_.tail.y += cage_origin_d.y;
	rat_.nose.x += cage_origin_d.x;
	rat_.nose.y += cage_origin_d.y;

	//nose-centroid-tail angle
	//rat_.NCT_Angle = calculateAngle(rat_.centroid, rat_.nose, rat_.tail);

	imshow("Feature Extraction", img_nose);
}

void RatBehaviorAnalysis::BtnSwitchClicked()
{
	if (ui.BtnSwitch->text() == tr("Depth\nOnly")){
		//isDepthUpdating = true;
		isColorUpdating = true;
		ui.BtnSwitch->setText(tr("With\nColor"));
	}
	else {
		isColorUpdating = false;
		//isDepthUpdating = true;
		ui.BtnSwitch->setText(tr("Depth\nOnly"));
	}
}

void RatBehaviorAnalysis::BtnLogClicked()
{
	ui.featureDisplay->clear();
}

void RatBehaviorAnalysis::BtnTrackingClicked()
{
	if (TRACKING) {
		trackingStream.reset();
		trackingFile->close();
		QString trackingFileName = QString("tracking_%1.txt").arg(QTime::currentTime().toString("hh_mm_ss_zzz"));
		trackingFile = new QFile(trackingFileName);
		if (trackingFile->open(QIODevice::WriteOnly | QFile::Text)) {
			trackingStream.setDevice(trackingFile);
			trackingStream << "Tracking Start" << endl;
		}
	}
}

void RatBehaviorAnalysis::BtnExpandClicked()
{
	if (ui.BtnExpand->text() == tr("Expand>>>")) {
		ui.BtnExpand->setText("Close<<<");
		this->setFixedSize(QSize::QSize(535, 480));
	}
	else {
		ui.BtnExpand->setText("Expand>>>");
		this->setFixedSize(QSize::QSize(240, 480));
	}
}

void RatBehaviorAnalysis::BtnSwitchTimeStampColorClicked()
{
	if (timeStampColorCounter < 4) timeStampColorCounter++;
	else timeStampColorCounter = 0;
}