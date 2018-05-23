#pragma once
#include <QWidget>
#include "ui_RegisterFaceWindow.h"
#include "Camera.h"
#include "ImageProcessing.h"

class RegisterFaceWindow : public QWidget
{
	Q_OBJECT
		QTimer* _timer;
public:
	RegisterFaceWindow(QWidget *parent = Q_NULLPTR);
	~RegisterFaceWindow();
	void on_timeout();
public slots:
	void onCaptureClicked();
	void onResetCamera();
	void onDetectedFaceCompelete(cv::Mat fr);
	void onRegisterUserClicked();
	void onProgressBarProcess(int value);
private:
	Ui::RegisterFaceWindow ui;
	Camera* camera;
	bool isCapturing;
	Mat frameRegister;
	FaceRecognitionLib frl;
};
