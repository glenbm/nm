#include "stdafx.h";

#include <cv.h>;

#include <highgui.h>;

#include "base64.h"

#include <windows.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>


using namespace std;
using namespace cv;

Mat hwnd2mat(HWND hwnd)
{
    HDC hwindowDC, hwindowCompatibleDC;

    int height, width, srcheight, srcwidth;
    HBITMAP hbwindow;
    Mat src;
    BITMAPINFOHEADER  bi;

    hwindowDC = GetDC(hwnd);
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    // get dimensions
    RECT windowsize;
    GetClientRect(hwnd, &windowsize);

    srcheight   = windowsize.bottom;
    srcwidth    = windowsize.right;
    height      = windowsize.bottom / 2;
    width       = windowsize.right / 2;

    src.create(height, width, CV_8UC4);

    // create a bitmap
    hbwindow            = CreateCompatibleBitmap(hwindowDC, width, height);
    bi.biSize           = sizeof(BITMAPINFOHEADER);
    bi.biWidth          = width;
    bi.biHeight         = -height;
    bi.biPlanes         = 1;
    bi.biBitCount       = 32;
    bi.biCompression    = BI_RGB;
    bi.biSizeImage      = 0;
    bi.biXPelsPerMeter  = 0;
    bi.biYPelsPerMeter  = 0;
    bi.biClrUsed        = 0;
    bi.biClrImportant   = 0;

    // capture the screen
    SelectObject(hwindowCompatibleDC, hbwindow);
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY);
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);

    // clean up memory
    DeleteObject (hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    return src;
}

int main(int argc, char **argv)
{
    HWND hwndDesktop = GetDesktopWindow();
    namedWindow("output", WINDOW_NORMAL);
    int key = 0;

    while(true)
    {
        Mat src = hwnd2mat(hwndDesktop); // takes 25-40 ms

        // save data to buffer
        std::vector<uchar> buffer;
        cv::imencode(".png", src, buffer);

        //string baseEncodedString = base64_encode(buffer.data(), buffer.size());

        //imwrite("C:/Users/Glen/Desktop/Gray_Image.png", src);

        imshow("output", src);

        key = waitKey(1);
    }
}
