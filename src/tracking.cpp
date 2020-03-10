#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    
  // Create a VideoCapture object and open the input file
  // If the input is the web camera, pass 0 instead of the video file name
  VideoCapture cap("../res/video1.mp4");

  // Check if camera opened successfully
  if (!cap.isOpened())
  {
    cout << "Error opening video stream or file" << endl;
    return -1;
  }

  size_t frame_num = 0;

  Mat frame;

  cap >> frame;

  Point point1(24, 44), point2(196, 210);

  rectangle(frame, point1, point2, Scalar(0, 0, 255), 1, 8, 0);
  imshow("Bounding Box", frame);

  waitKey(0);

  while (1)
  {

    Mat grayscale = frame.clone();

    cvtColor(frame, grayscale, CV_RGB2GRAY);

    cap >> frame;

    imshow("Frame", grayscale);

    // Detect Keypoints using SURF
    int min_hessian = 300;

    // Matching using FLANN

    // If the frame is empty, break immediately
    if (frame.empty())
      break;

    // Display the resulting frame

    // Press  ESC on keyboard to exit
    char c = (char)waitKey(25);
    if (c == 27)
      break;

    frame_num++;
  }

  // When everything done, release the video capture object
  cap.release();

  // Closes all the frames
  destroyAllWindows();

  return 0;
}
