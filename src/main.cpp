#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>

// https://docs.opencv.org/4.6.0/d2/d0a/tutorial_introduction_to_tracker.html

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    // show help
    if (argc < 2)
    {
        cout << " Usage: tracker <video_name>\n"
                " examples:\n"
                " example_tracking_kcf Bolt/img/%04d.jpg\n"
                " example_tracking_kcf faceocc2.webm\n"
             << endl;
        return 0;
    }
    // declares all required variables
    Rect roi;
    Mat frame;
    // create a tracker object
    Ptr<Tracker> tracker = TrackerKCF::create();
    // set input video
    std::string video = argv[1];
    VideoCapture cap(video);
    // get bounding box
    cap >> frame;
    roi = selectROI("tracker", frame);
    // quit if ROI was not selected
    if (roi.width == 0 || roi.height == 0)
        return 0;
    // initialize the tracker

    printf("%d, %d, %d, %d\r\n",roi.x, roi.y, roi.width, roi.height);

    tracker->init(frame, roi);
    // perform the tracking process
    printf("Start the tracking process, press ESC to quit.\n");

    const string source = argv[1];                                   // the source file name
    string::size_type pAt = source.find_last_of('.');                // Find extension point
    const string NAME = source.substr(0, pAt) + argv[2][0] + ".avi"; // Form the new name with container
    int ex = static_cast<int>(cap.get(CAP_PROP_FOURCC));             // Get Codec Type- Int form

    // Transform from int to char via Bitwise operators
    char EXT[] = {(char)(ex & 0XFF), (char)((ex & 0XFF00) >> 8), (char)((ex & 0XFF0000) >> 16), (char)((ex & 0XFF000000) >> 24), 0};

    Size S = Size((int)cap.get(CAP_PROP_FRAME_WIDTH), // Acquire input size
                  (int)cap.get(CAP_PROP_FRAME_HEIGHT));
    VideoWriter outputVideo; // Open the output

    outputVideo.open(NAME, ex, cap.get(CAP_PROP_FPS), S, true);

    if (!outputVideo.isOpened())
    {
        cout << "Could not open the output video for write: " << source << endl;
        return -1;
    }
    cout << "Input frame resolution: Width=" << S.width << "  Height=" << S.height
         << " of nr#: " << cap.get(CAP_PROP_FRAME_COUNT) << endl;
    cout << "Input codec type: " << EXT << endl;

    for (;;)
    {
        // get frame from the video
        cap >> frame;
        // stop the program if no more images
        if (frame.rows == 0 || frame.cols == 0)
            break;
        // update the tracking result
        tracker->update(frame, roi);
        // draw the tracked object
        rectangle(frame, roi, Scalar(255, 0, 0), 2, 1);
        // show image with the tracked object
        imshow("tracker", frame);
        outputVideo.write(frame);
        // Write frame to video output
        // quit on ESC button
        if (waitKey(1) == 27)
            break;
    }
    return 0;
}