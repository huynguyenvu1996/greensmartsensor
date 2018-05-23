#pragma once
#include <QtWidgets/QMainWindow>
#include <ImageProcessing.h>
#include <RegisterFaceWindow.h>


class FujinetFaceRecognitionPreview : public QMainWindow
{
	Q_OBJECT
		QTimer* _timer;

public:
	FujinetFaceRecognitionPreview(QWidget *parent = Q_NULLPTR);
	~FujinetFaceRecognitionPreview();
	void FujinetFaceRecognitionPreview::on_timeout();

public slots:
	void openRegistrationFaceWindow();
	void onRecognizedFaceCompeleted(QString username, QString message);
	void onPushMessage(QString message);

private:
	Ui::FujinetFaceRecognitionPreviewClass ui;
	Camera* camera;
	Mat frameRegister;
	ImageProcessing* imgp;
	RegisterFaceWindow* rfw;
	bool startedThreadRecog;
	bool loopRecogFace;
	QThread *thread;

};
