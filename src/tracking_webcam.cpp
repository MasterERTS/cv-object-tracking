/**
 * @file tracking_webcam.cpp
 * @author Erwin lejeune (you@domain.com)
 * @brief Select a tracking zone from the webcam stream with mouse dragging, then press any key to start tracking
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

Mat frame;

bool drag = false;
Rect roi;
Point point1;
Point point2;
bool selected_area = false;

// If Bounding Box defined by mouse
void mouse(int event, int x, int y, int flags, void *param)
{
    if (event == EVENT_LBUTTONDOWN && !drag)
    {
        point1 = Point(x, y);
        drag = 1;
    }
    if (event == EVENT_MOUSEMOVE && drag)
    {
        Mat img = frame.clone();
        point2 = Point(x, y);
        rectangle(img, point1, point2, Scalar(0, 0, 255), 1, 8, 0);
        imshow("Webcam Stream", img);
    }
    if (event == EVENT_LBUTTONUP && drag)
    {
        point2 = Point(x, y);
        drag = 0;
        roi = Rect(point1.x, point1.y, x - point1.x, y - point1.y);
        selected_area = true;
    }
    if (event == EVENT_LBUTTONUP)
    {
        drag = 0;
    }
}

int main()
{

    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    VideoCapture cap(0);
    Mat img_roi;
    bool tracking_ready = false;
    bool roi_ready = false;

    // Check if camera opened successfully
    if (!cap.isOpened())
    {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    size_t frame_num = 0;

    // Capture frame-by-frame
    cap >> frame;

    // If the frame is empty, break immediately
    if (frame.empty())
        cout << "Error, first frame empty.";

    // Display the resulting frame
    while (selected_area == false) {
        cap >> frame;

        imshow("Webcam Stream", frame);

        char spacebar_pressed = (char)waitKey(25);

        if (spacebar_pressed == 32 && selected_area == false) {
            cout << "\nSelect Tracking Area..." << endl;
            setMouseCallback("Webcam Stream", mouse, NULL);
            waitKey(0);
            cout << "Tracking Area successfully selected." << endl;
            roi_ready = true;
        }

        if (roi_ready == true) {
            // Convert in Grayscale without showing
            cout << "\nConverting tracking area in grayscale..." << endl;
            img_roi = frame(roi);
            //imshow("Roi", img_roi);
            cvtColor(img_roi, img_roi, COLOR_RGB2GRAY);
            cout << "Tracking area succesfully converted." << endl;
            tracking_ready = true;
        }
        
    }

    if (tracking_ready == true) {
        // Declare Keypoints, Descriptors and SURF detector
        std::vector<KeyPoint> keypoints_roi, keypoints_frame;
        Mat descriptors_roi, descriptors_frame; // NOT A VECTOR !!

        Ptr<SURF> s_detector = SURF::create(400);

        // Wait for key press to start computation
        cout << "\nPress any key to start computation..." << endl;
        waitKey(0);

        while (1) {
            cap >> frame;

            Mat grayscale_frame = frame.clone();

            cvtColor(frame, grayscale_frame, COLOR_RGB2GRAY);

            imshow("Webcam Stream", grayscale_frame);

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
    }

    return 0;
}

#else

// Used this because I couldn't figure out why it didn't work on my personal computer. It highlighted the problem.
int main()
{
    std::cout << "You need the xfeatures2d module to run this program" << std::endl;
}

#endif