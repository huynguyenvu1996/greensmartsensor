#include "FujinetFaceRecognitionPreview.h"
#include <QtWidgets/QApplication>

/*https://forum.qt.io/topic/89034/passing-mat-in-form-of-signal*/
//make sure 'mat' is registered using qregistermetatype()
int main(int argc, char *argv[])
{
	qRegisterMetaType<cv::Mat>("Mat");
	QApplication a(argc, argv);
	FujinetFaceRecognitionPreview w;
	w.show();
	return a.exec();
}
