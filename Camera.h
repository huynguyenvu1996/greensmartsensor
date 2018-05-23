#pragma once
#include <QTimer>
#include <QObject>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;
/*https://stackoverflow.com/questions/11184590/how-to-view-qdebug-in-visual-studio*/
/*https://stackoverflow.com/questions/28660852/qt-qthread-destroyed-while-thread-is-still-running-during-closing*/

class Camera : public QObject
{
	Q_OBJECT
public:
	Camera(QObject * parent = Q_NULLPTR);
	~Camera();
	bool initCamera(int device);
	bool startCamera(int device);
	bool stopCamera();
	Mat getCurrentFrame();
private:
	VideoCapture videoCapture;
};

