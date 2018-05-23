#include "ImageProcessing.h"

ImageProcessing::ImageProcessing(QObject *parent) : QThread(parent)
{
}

ImageProcessing::~ImageProcessing()
{
}

void ImageProcessing::setFrame(Mat &fr) {
	frame = fr;
}
