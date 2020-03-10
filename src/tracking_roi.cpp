#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat frame;

bool drag = false;
Rect tracking;
Point point1;
Point point2;

// If Bounding Box defined by mouse
void mouse(int event, int x, int y, int flags, void* param)
{
    if (event == CV_EVENT_LBUTTONDOWN && !drag)
    {
        point1 = Point(x, y);
        drag = 1;
    }
    if (event == CV_EVENT_MOUSEMOVE && drag)
    {
        Mat img = frame.clone();
        point2 = Point(x, y);
        rectangle(img, point1, point2, Scalar(0, 0, 255), 1, 8, 0);
        imshow("Frame", img);
    }
    if (event == CV_EVENT_LBUTTONUP && drag)
    {
        point2 = Point(x, y);
        drag = 0;
        tracking = Rect(point1.x, point1.y, x - point1.x, y - point1.y);
    }
    if (event == CV_EVENT_LBUTTONUP)
    {
        drag = 0;
    }
}


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

    // Capture frame-by-frame
    cap >> frame;

    // If the frame is empty, break immediately
    if (frame.empty())
        cout << "Error, first frame empty.";

    // Display the resulting frame
    imshow("Frame", frame);

    cvSetMouseCallback("Frame", mouse, NULL);

    waitKey(0);

    while (1)
    {

        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
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
