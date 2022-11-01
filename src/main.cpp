#include <iostream>            // for standard I/O
#include <string>              // for strings
#include <iomanip>             // for controlling float print precision
#include <sstream>             // string to number conversion
#include <opencv2/core.hpp>    // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc.hpp> // Gaussian Blur
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp> // OpenCV window I/O
using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Not enough parameters" << endl;
        return -1;
    }
    stringstream conv;
    const string sourceReference = argv[1];
    int delay;
    conv << argv[2] << endl;
    conv >> delay; // take out the numbers
    int frameNum = -1;                 // Frame counter
    VideoCapture captRefrnc(sourceReference);
    if (!captRefrnc.isOpened())
    {
        cout << "Could not open reference " << sourceReference << endl;
        return -1;
    }
    Size refS = Size((int)captRefrnc.get(CAP_PROP_FRAME_WIDTH),
                     (int)captRefrnc.get(CAP_PROP_FRAME_HEIGHT));
    const char *WIN_RF = "Reference";
    // Windows
    namedWindow(WIN_RF, WINDOW_AUTOSIZE);
    moveWindow(WIN_RF, refS.width, 0);        // 750,  2 (bernat =0)
    cout << "Reference frame resolution: Width=" << refS.width << "  Height=" << refS.height
         << " of nr#: " << captRefrnc.get(CAP_PROP_FRAME_COUNT) << endl;
    Mat frameReference;
    for (;;) // Show the image captured in the window and repeat
    {
        captRefrnc >> frameReference;
        if (frameReference.empty())
        {
            cout << " < < <  Game over!  > > > ";
            break;
        }
        ++frameNum;
        imshow(WIN_RF, frameReference);
        char c = (char)waitKey(delay);
        if (c == 27)
            break;
    }
    return 0;
}