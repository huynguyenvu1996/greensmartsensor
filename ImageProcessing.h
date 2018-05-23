#pragma once
#include <QThread>
#include <FaceRecognitionLib.h>
#include <QDebug>
#include <QString>

class ImageProcessing : public QThread
{
	Q_OBJECT
		void run()
	{
		qDebug() << "Start processing image!";
		//frl.detectFace(frame);
		emit detectedFaceCompeleted(frame);
	}
private:
	FaceRecognitionLib frl;
	Mat frame;
public:
	ImageProcessing(QObject * parent = Q_NULLPTR);
	~ImageProcessing();
	void setFrame(Mat &fr);
signals:
	void detectedFaceCompeleted(Mat);
};
