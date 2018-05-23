#include "FujinetFaceRecognitionPreview.h"

FujinetFaceRecognitionPreview::FujinetFaceRecognitionPreview(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setStyleSheet("background-color: #ffffff;");
	ui.edt_recognition_message->setStyleSheet("background-color: #000000; color : green;");
	connect(ui.btn_open_register_face_window, SIGNAL(clicked()), this, SLOT(openRegistrationFaceWindow()));
	ui.camera_screen->setScaledContents(true);
	startedThreadRecog = false;
	loopRecogFace = true;
	//ui.camera_screen->setFixedSize(0, 0);
	camera = new Camera();
	_timer = new QTimer(this);
	connect(_timer, &QTimer::timeout, this, &FujinetFaceRecognitionPreview::on_timeout);
	// populate your window with images, labels, etc. here
	_timer->start(100 /*call the timer every 100 ms*/);
	camera->startCamera(0);
}

FujinetFaceRecognitionPreview::~FujinetFaceRecognitionPreview()
{
	camera->stopCamera();
	delete camera;
	if (_timer) {
		delete _timer;
		_timer = NULL;
	}
}

void FujinetFaceRecognitionPreview::on_timeout()
{
	Mat frame = camera->getCurrentFrame();
	cv::resize(frame, frame, Size(640, 480), 0, 0, INTER_CUBIC);
	cv::cvtColor(frame, frame, CV_BGR2RGB);
	QImage qframe((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
	ui.camera_screen->setPixmap(QPixmap::fromImage(qframe));
	frameRegister = frame.clone();

	if (!startedThreadRecog) {
		qDebug("de");
		startedThreadRecog = true;
		FaceRecognitionLib* frl1 = new FaceRecognitionLib;
		thread = new QThread(this);
		thread->setPriority(QThread::HighPriority);
		frl1->setFrame(frame);
		connect(frl1, SIGNAL(recognizedFaceCompeleted(QString, QString)), this, SLOT(onRecognizedFaceCompeleted(QString, QString)));
		connect(frl1, SIGNAL(pushMessage(QString)), this, SLOT(onPushMessage(QString)));
		QTimer::singleShot(200, frl1, SLOT(recognizingFace()));
		frl1->moveToThread(thread);
		thread->start();
	}
}

//void FujinetFaceRecognitionPreview::onDetectedFaceCompelete(cv::Mat& frame) {
//	cv::cvtColor(frame, frame, CV_BGR2RGB);
//	QImage qframe((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
//	ui.camera_screen->setPixmap(QPixmap::fromImage(qframe));
//}
void FujinetFaceRecognitionPreview::openRegistrationFaceWindow() {
	loopRecogFace = false;
	rfw = new RegisterFaceWindow();
	rfw->show();
	this->hide();
}

void FujinetFaceRecognitionPreview::onRecognizedFaceCompeleted(QString username, QString message) {
	qDebug("-----onRecognizedFaceCompeleted-------");
	qDebug(username.toLatin1());
	ui.edt_recognition_user_name->setText(username);
	thread->quit();
	if (!thread->wait(3000)) //Wait until it actually has terminated (max. 3 sec)
	{
		thread->terminate(); //Thread didn't exit in time, probably deadlocked, terminate it!
		thread->wait(); //We have to wait again here!
	}
	delete thread;
	if (loopRecogFace) {
		FaceRecognitionLib* frl1 = new FaceRecognitionLib;
		thread = new QThread(this);
		frl1->setFrame(frameRegister);
		connect(frl1, SIGNAL(recognizedFaceCompeleted(QString, QString)), this, SLOT(onRecognizedFaceCompeleted(QString, QString)));
		connect(frl1, SIGNAL(pushMessage(QString)), this, SLOT(onPushMessage(QString)));
		QTimer::singleShot(200, frl1, SLOT(recognizingFace()));
		frl1->moveToThread(thread);
		thread->start();
	}
}


void FujinetFaceRecognitionPreview::onPushMessage(QString message) {
	ui.edt_recognition_message->append(message);
}
