/**
 * @file tracking.cpp
 * @author Erwin lejeune (you@domain.com)
 * @brief track a hardcoded object from video1
 * @version 0.1
 * @date 2020-03-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

#if MXNET_USE_OPENCV
#include <opencv2/opencv.hpp>
+ #include<opencv2 / imgproc / types_c.h>
#endif // MXNET_USE_OPENCV

#if MXNET_USE_OPENCV
#include <opencv2/opencv.hpp>
    + #include<opencv2 / imgproc / types_c.h>
#include // NOLINT()
#include // NOLINT()
#include // NOLINT(*)
#endif

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

  // Draw Rectangle with given intervals
  Point point1(24, 44), point2(196, 210);
  rectangle(frame, point1, point2, Scalar(0, 0, 255), 1, 8, 0);

  // Create rectangle object to crop from main frame
  Rect roi = Rect(point1.x, point1.y, point2.x - point1.x, point2.y - point1.y);

  // Show reference roi
  imshow("Bounding Box", frame);

  // Convert in Grayscale without showing
  Mat img_roi = frame(roi);
  cvtColor(img_roi, img_roi, COLOR_RGB2GRAY);

  // Declare Keypoints, Descriptors and SURF detector
  std::vector<KeyPoint> keypoints_roi, keypoints_frame;
  Mat descriptors_roi, descriptors_frame; // NOT A VECTOR !!

  Ptr<SURF> s_detector = SURF::create(400);

  // Wait for key press to start computation
  waitKey(0);

  while (1)
  {
    cap >> frame;

    Mat grayscale_frame = frame.clone();

    cvtColor(frame, grayscale_frame, COLOR_RGB2GRAY);

    imshow("Frame", grayscale_frame);

    // Detect and Compute using SURF.
    s_detector->detect(img_roi, keypoints_roi);
    s_detector->compute(img_roi, keypoints_roi, descriptors_roi);

    s_detector->detect(grayscale_frame, keypoints_frame);
    s_detector->compute(grayscale_frame, keypoints_frame, descriptors_frame);

    // Matching using FLANN
    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptors_roi, descriptors_frame, matches);

    // Draw matches
    Mat img_match;
    drawMatches(img_roi, keypoints_roi, grayscale_frame, keypoints_frame, matches, img_match);

    imshow("Matches", img_match);

    // Get all keypoints that are good matches

    std::vector<Point2f> roi_pts;
    std::vector<Point2f> frame_pts;

    // Get the keypoints from matches only
    for (size_t i = 0; i < matches.size(); i++)
    {
      roi_pts.push_back(keypoints_roi[matches[i].queryIdx].pt);
      frame_pts.push_back(keypoints_frame[matches[i].trainIdx].pt);
    }

    // Find homography
    Mat homography = findHomography(roi_pts, frame_pts, RANSAC);

    // Get ROI corners ( easy after cropping in the first place )
    std::vector<Point2f> roi_corners(4);
    roi_corners[0] = Point(0, 0);
    roi_corners[1] = Point(img_roi.cols, 0);
    roi_corners[2] = Point(img_roi.cols, img_roi.rows);
    roi_corners[3] = Point(0, img_roi.rows);
    std::vector<Point2f> frame_corners(4);

    perspectiveTransform(roi_corners, frame_corners, homography);

    // Draw lines between corners
    line(img_match, frame_corners[0] + Point2f(img_roi.cols, 0), frame_corners[1] + Point2f(img_roi.cols, 0), Scalar(0, 0, 255), 4);
    line(img_match, frame_corners[1] + Point2f(img_roi.cols, 0), frame_corners[2] + Point2f(img_roi.cols, 0), Scalar(0, 0, 255), 4);
    line(img_match, frame_corners[2] + Point2f(img_roi.cols, 0), frame_corners[3] + Point2f(img_roi.cols, 0), Scalar(0, 0, 255), 4);
    line(img_match, frame_corners[3] + Point2f(img_roi.cols, 0), frame_corners[0] + Point2f(img_roi.cols, 0), Scalar(0, 0, 255), 4);

    //-- Show detected matches
    imshow("Matches & Object detection", img_match);

    // If the frame is empty, break immediately
    if (frame.empty() or img_match.empty())
      break;

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

// Used this because I couldn't figure out why it didn't work on my personal computer. It highlighted the problem.
int main()
{
  std::cout << "You need the xfeatures2d module to run this program" << std::endl;
}

#endif
