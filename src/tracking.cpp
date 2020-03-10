#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

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

  Mat img_roi = frame(Rect(point1.x, point1.y, point2.x - point1.x, point2.y - point1.y));
  cvtColor(img_roi, img_roi, CV_RGB2GRAY);
  
  imshow("Cropped Bounding Box", img_roi);

  waitKey(0);

  while (1)
  {
	cap >> frame;
	
    Mat grayscale_frame = frame.clone();

    cvtColor(frame, grayscale_frame, CV_RGB2GRAY);

    

    imshow("Frame", grayscale_frame);

    // Detect Keypoints using SURF
    int min_hessian = 400;
    std::vector<KeyPoint> keypoints_roi, keypoints_frame;
    Mat descriptors_roi, descriptors_frame; // NOT A VECTOR !!
    
    Ptr<SURF> s_detector = SURF::create(min_hessian);
    
    s_detector->detect(img_roi, keypoints_roi);
    s_detector->compute(img_roi, keypoints_roi, descriptors_roi);
    
    s_detector->detect(grayscale_frame, keypoints_frame);
    s_detector->compute(grayscale_frame, keypoints_frame, descriptors_frame);
    

    // Matching using FLANN
    FlannBasedMatcher matcher;
    vector< DMatch > matches;
    matcher.match( descriptors_roi, descriptors_frame, matches);
    
    // Draw matches
    Mat img_match;
    drawMatches(img_roi, keypoints_roi, grayscale_frame, keypoints_frame, matches, img_match);
   
	imshow("Matches", img_match);
	
	waitKey(0);
	
   
   
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
#else

int main() {
	cout << "You need the xfeatures2d module to run this program" << endl;
}

#endif
