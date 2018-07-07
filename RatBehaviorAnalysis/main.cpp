#include "ratbehavioranalysis.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RatBehaviorAnalysis w;
	w.setWindowTitle("Kinect v2 Recording");
	w.setFixedSize(w.size());
	w.show();
	return a.exec();
}
