#include "Camera.h"



Camera::Camera(QObject * parent) : QObject(parent)
{

}


Camera::~Camera()
{
	stopCamera();
}

bool Camera::initCamera(int device) {
	videoCapture.open(device);
	return videoCapture.isOpened();
}

bool Camera::startCamera(int device) {
	if (!this->initCamera(device)) {
		return false;
	}
	videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	//qDebug() << "Camera successfully started.";
	return true;
}

bool Camera::stopCamera() {
	if (videoCapture.isOpened()) {
		videoCapture.release();
		//qDebug() << "Camera successfully disconnected.";
		return true;
	}
	return false;
}

Mat Camera::getCurrentFrame() {
	Mat frame;
	videoCapture.read(frame);
	return frame;
}

