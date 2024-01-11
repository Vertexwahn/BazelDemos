#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include <vector>
#include <iostream>

using namespace cv;

int threshold_value = 0;
int threshold_type = 3;;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;

char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
char* trackbar_value = "Value";

void Threshold_Demo( int, void* )
{
  /* 0: Binary
     1: Binary Inverted
     2: Threshold Truncated
     3: Threshold to Zero
     4: Threshold to Zero Inverted
   */


}

class IStage {
	public:
		virtual void process(Mat image) = 0;
		virtual void visualize() = 0;
};

RNG rng(12345); 

int main() {
	// Create a VideoCapture object and use camera to capture the video
	VideoCapture cap(0);

	// Check if camera opened successfully
	if (!cap.isOpened()) {
		std::cout << "Error opening video stream" << std::endl;
		return -1;
	}

	// Default resolution of the frame is obtained.The default resolution is system dependent.
	int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);

	// Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file.
	VideoWriter video("outcpp.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height));

	while (true) {
		Mat frame;

		// Capture frame-by-frame
		cap >> frame;

		// If the frame is empty, break immediately
		if (frame.empty())
			break;

		// Write the frame into the file 'outcpp.avi'
		video.write(frame);

		// Display the resulting frame
		imshow("Frame", frame);

		// 1. Stage: Convert camera image to grayscale
		Mat gray;
		cvtColor(frame, gray, COLOR_RGB2GRAY);
		imshow("Gray Image", gray);

		// 2. Stage Threshhold
		char* window_name = "Threshold Demo";
		namedWindow( window_name, WINDOW_AUTOSIZE );

		createTrackbar( trackbar_type,
						window_name, &threshold_type,
						max_type, Threshold_Demo );

		createTrackbar( trackbar_value,
                  window_name, &threshold_value,
                  max_value, Threshold_Demo );

		Mat threshhold;
		threshold( gray, threshhold, threshold_value, max_BINARY_value,threshold_type );
		imshow( window_name, threshhold );

 		// 3. Stage Find Contours
		std::vector<std::vector<Point> > contours;
  		std::vector<Vec4i> hierarchy;
		findContours( threshhold, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

  		/// Draw contours
		Mat drawing = Mat::zeros( frame.size(), CV_8UC3 );
		for( int i = 0; i< contours.size(); i++ )
			{
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
			}

		/// Show in a window
		namedWindow( "Contours", WINDOW_AUTOSIZE );
		imshow( "Contours", drawing );

		// Press  ESC on keyboard to  exit
		char c = (char)waitKey(1);
		if (c == 27)
			break;
	}

	// When everything done, release the video capture and write object
	cap.release();
	video.release();

	// Closes all the windows
	destroyAllWindows();

	return 0;
}
