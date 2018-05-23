#include "FaceRecognitionLib.h"




FaceRecognitionLib::FaceRecognitionLib(QObject * parent) : QObject(parent)
{

}


FaceRecognitionLib::~FaceRecognitionLib()
{
}

void FaceRecognitionLib::setFrame(Mat fr) {
	frameCapture = fr.clone();
}

static cv::Rect dlibRectangleToOpenCV(dlib::rectangle r)
{
	return cv::Rect(cv::Point2i(r.left(), r.top()), cv::Point2i(r.right() + 1, r.bottom() + 1));
}

/*
* Thank to:
* https://github.com/davisking/dlib/issues/16
* https://stackoverflow.com/questions/29118317/how-to-convert-mat-to-array2drgb-pixel
http://eyyuptemlioglu.blogspot.com.tr/2017/04/facial-landmarks-with-dlib-opencv-cpp-c.html
https://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/bounding_rects_circles/bounding_rects_circles.html
http://dlib.net/webcam_face_pose_ex.cpp.html
https://stackoverflow.com/questions/41376337/applying-overlays-to-image-with-varying-transparency
https://stackoverflow.com/questions/37665725/how-to-overlay-facial-keypoints-from-dlib-in-an-opencv-window?rq=1
https://github.com/davisking/dlib/blob/master/examples/face_landmark_detection_ex.cpp
https://www.youtube.com/watch?v=boHmqKnAzAg

*/
void FaceRecognitionLib::detectFace() {
	emit setProgressBarProcess(0);
	// The first thing we are going to do is load all our models.  First, since we need to
	// find faces in the image we will need a face detector:
	frontal_face_detector detector = get_frontal_face_detector();
	// We will also use a face landmarking model to align faces to a standard pose:  (see face_landmark_detection_ex.cpp for an introduction)
	shape_predictor shape_model;
	deserialize(".//data//lib//shape_predictor_5_face_landmarks.dat") >> shape_model;
	// And finally we load the DNN responsible for face recognition.
	anet_type net;
	deserialize(".//data//lib//dlib_face_recognition_resnet_model_v1.dat") >> net;
	emit setProgressBarProcess(25);



	//Convert OpenCV cv::Mat to dlib:array2d
	array2d<bgr_pixel> img;
	dlib::assign_image(img, cv_image<bgr_pixel>(frameCapture));
	std::vector<dlib::rectangle> detected_faces = detector(img);
	int number_of_detected_faces = detected_faces.size();
	std::vector<full_object_detection> shapes;
	int rect_x, rect_y, rect_w, rect_h;
	emit setProgressBarProcess(60);


	for (int i = 0; i < number_of_detected_faces; i++) {
		full_object_detection shape = shape_model(img, detected_faces[i]);
		shapes.push_back(shape);
		rect_x = detected_faces[i].left();
		rect_y = detected_faces[i].top();
		rect_w = detected_faces[i].right() - rect_x;
		rect_h = detected_faces[i].bottom() - rect_y;

		Rect face_rectangle(rect_x, rect_y, rect_w, rect_h);

		cv::rectangle(frameCapture, face_rectangle, Scalar(0, 255, 0), 3, 8, 0);

		emit setProgressBarProcess(80);

		for (int j = 0; j < 5; j++) {
			cv::circle(frameCapture, Point(shape.part(j).x(), shape.part(j).y()), 2, Scalar(0, 0, 255), 1, 16);
		}
		emit setProgressBarProcess(100);
		emit detectedFaceCompeleted(frameCapture);
	}
}

void FaceRecognitionLib::registerFace() {
	emit setProgressBarProcess(10);
	// The first thing we are going to do is load all our models.  First, since we need to
	// find faces in the image we will need a face detector:
	frontal_face_detector detector = get_frontal_face_detector();
	// We will also use a face landmarking model to align faces to a standard pose:  (see face_landmark_detection_ex.cpp for an introduction)
	shape_predictor sp;
	//deserialize("shape_predictor_5_face_landmarks.dat") >> sp;	
	deserialize(".//data//lib//shape_predictor_5_face_landmarks.dat") >> sp;
	// And finally we load the DNN responsible for face recognition.
	anet_type net;
	//deserialize("dlib_face_recognition_resnet_model_v1.dat") >> net;
	deserialize(".//data//lib//dlib_face_recognition_resnet_model_v1.dat") >> net;
	//Convert OpenCV cv::Mat to dlib:array2d
	array2d<bgr_pixel> img;
	dlib::assign_image(img, cv_image<bgr_pixel>(frameCapture));
	std::vector<matrix<rgb_pixel>> faces;
	emit setProgressBarProcess(25);
	for (auto face : detector(img))
	{
		auto shape = sp(img, face);
		matrix<rgb_pixel> face_chip;
		extract_image_chip(img, get_face_chip_details(shape, 150, 0.25), face_chip);
		faces.push_back(move(face_chip));
	}
	emit setProgressBarProcess(50);
	if (faces.size() == 0)
	{
		qDebug("No faces found in image!");
	}
	else {
		// This call asks the DNN to convert each face image in faces into a 128D vector.
		// In this 128D vector space, images from the same person will be close to each other
		// but vectors from different people will be far apart.  So we can use these vectors to
		// identify if a pair of images are from the same person or from different people.  
		matrix<float, 0, 1> face_descriptor = net(faces[0]);
		wofstream writer;
		const std::locale empty_locale = std::locale::empty();
		typedef std::codecvt_utf8<wchar_t> converter_type;
		const converter_type* converter = new converter_type;
		const std::locale utf8_locale = std::locale(empty_locale, converter);
		writer.open(".//data/database/face_description.txt", ios::app);
		if (!writer.is_open())
		{
		}
		writer.imbue(utf8_locale);
		writer << L"Hoc" << "\n";
		for (int i = 0; i < 127; i++) {
			writer << face_descriptor(i) << " ";
		}
		writer << face_descriptor(127) << "\n";
		writer.flush();
		writer.close();
	}
	emit setProgressBarProcess(100);
}

void FaceRecognitionLib::recognizingFace() {
	// The first thing we are going to do is load all our models.  First, since we need to
	// find faces in the image we will need a face detector:
	frontal_face_detector detector = get_frontal_face_detector();
	// We will also use a face landmarking model to align faces to a standard pose:  (see face_landmark_detection_ex.cpp for an introduction)
	shape_predictor sp;
	//deserialize("shape_predictor_5_face_landmarks.dat") >> sp;	
	deserialize(".//data//lib//shape_predictor_5_face_landmarks.dat") >> sp;
	// And finally we load the DNN responsible for face recognition.
	anet_type net;
	//deserialize("dlib_face_recognition_resnet_model_v1.dat") >> net;
	deserialize(".//data//lib//dlib_face_recognition_resnet_model_v1.dat") >> net;

	array2d<bgr_pixel> img;
	dlib::assign_image(img, cv_image<bgr_pixel>(frameCapture));

	emit pushMessage("Detecting face...");

	std::vector<matrix<rgb_pixel>> faces;
	for (auto face : detector(img))
	{
		auto shape = sp(img, face);
		matrix<rgb_pixel> face_chip;
		extract_image_chip(img, get_face_chip_details(shape, 150, 0.25), face_chip);
		faces.push_back(move(face_chip));

	}

	emit pushMessage("Done detected face...");

	if (faces.size() == 0)
	{
		qDebug("No faces found in image!");
		emit pushMessage("No faces found in image...");
		emit pushMessage("--------------------------\n");
		emit recognizedFaceCompeleted(QString("No user"), QString("Done recognized face!"));
	}
	else {
		std::vector<User*> listFaceDescriptionUser;
		wifstream reader;
		const std::locale empty_locale = std::locale::empty();
		typedef std::codecvt_utf8<wchar_t> converter_type;
		const converter_type* converter = new converter_type;
		const std::locale utf8_locale = std::locale(empty_locale, converter);
		reader.open(".//data/database/face_description.txt");
		if (!reader.is_open())
		{

		}
		wstring str, temp, data_str;
		reader.imbue(utf8_locale);
		int count = 0;
		while (getline(reader, str))
		{
			count++;
			//qDebug(QString::number(count).toLatin1());
			if (!str.empty()) {
				User *user = new User();
				user->name = str;
				//qDebug(QString::fromWCharArray(user->name.c_str()).toLatin1());
				for (int i = 0; i < 128; i++)
				{
					reader >> user->faceDescription[i];
					qDebug(QString::number(user->faceDescription[i]).toLatin1());
				}
				listFaceDescriptionUser.push_back(user);
				//reader >> temp;
			}
		}
		reader.close();
		qDebug("***************************************************");
		qDebug(QString::number(listFaceDescriptionUser.size()).toLatin1());

		/*for (int i = 0; i < listFaceDescriptionUser.size(); i++) {
			QString str;
			for (int j = 0; j < 128; j++) {
				str += QString::number(listFaceDescriptionUser[i].faceDescription[j]);
			}
			qDebug(str.toLatin1());
		}*/
		emit pushMessage("Start matching face...");

		matrix<float, 0, 1> face_descriptors = net(faces[0]);

		float input[128];
		for (int i = 0; i < 128; i++) {
			input[i] = face_descriptors(i);
		}

		float similar[128];

		wstring userDetect = L"";
		float sumMin = 10;

		for (int i = 0; i < listFaceDescriptionUser.size(); i++) {
			for (int j = 0; j < 128; j++) {
				similar[j] = input[j] - listFaceDescriptionUser[i]->faceDescription[j];
			}
			float sum = 0;
			for (int i = 0; i < 128; i++) {
				sum = sum + similar[i] * similar[i];
			}
			sum = sqrt(sum);
			qDebug("<------------------->");
			qDebug(QString::number(sum).toLatin1());
			qDebug("<------------------->");
			if (sum < 0.45) {
				if (sum < sumMin) {
					userDetect = listFaceDescriptionUser[i]->name;
					sumMin = sum;
				}

			}
		}
		emit pushMessage("Done matched face...");
		if (!userDetect.empty()) {
			emit recognizedFaceCompeleted(QString::fromWCharArray(userDetect.c_str()), QString("Done recognized face!"));
		}
		else {
			emit recognizedFaceCompeleted(QString("No user"), QString("Done recognized face!"));
		}
		emit pushMessage("--------------------------\n");
	}
}

std::vector<matrix<rgb_pixel>> FaceRecognitionLib::jitter_image(const matrix<rgb_pixel>& img)
{
	// All this function does is make 100 copies of img, all slightly jittered by being
	// zoomed, rotated, and translated a little bit differently. They are also randomly
	// mirrored left to right.
	thread_local random_cropper cropper;
	cropper.set_chip_dims(150, 150);
	cropper.set_randomly_flip(true);
	cropper.set_background_crops_fraction(0);
	cropper.set_translate_amount(0.02);
	cropper.set_max_rotation_degrees(3);

	std::vector<mmod_rect> raw_boxes(1), ignored_crop_boxes;
	raw_boxes[0] = shrink_rect(get_rect(img), 3);
	std::vector<matrix<rgb_pixel>> crops;

	matrix<rgb_pixel> temp;
	for (int i = 0; i < 100; ++i)
	{
		cropper(img, raw_boxes, temp, ignored_crop_boxes);
		crops.push_back(move(temp));
	}
	return crops;
}
