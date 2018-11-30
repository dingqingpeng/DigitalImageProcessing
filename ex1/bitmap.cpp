#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include "bitmap.h"

/********Only used for displaying BMP file**********/
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
/********Only used for displaying BMP file**********/

/****************** 
 * Brief: (数字图像处理-->实验一) Read and display bitmap file
 * Parameters:
 *     argv[1] -- filename
 * Author:
 *     Ding Qingpeng (丁庆鹏, Student No. 18B903053)
 ******************/

using namespace std;

int main(int argc, char const *argv[])
{  
    // Load file and check validation
    const string inputFileName = argv[1];
    BitMap image( inputFileName );  // BitMap image = load(inputFileName);
    if(!image.formatCheck())
    {
        cerr << "Input file is not a bmp image." << endl;
        return -1;
    }
    
    image.showFileHeader();
    image.showInfoHeader();
    bmpshow("Regenerated Image", image);
    waitKey(0);

    // Get value of certain pixel
    int row = 123, col = 123;
    try
    {
        cout << "-- pixel value at (" << row << " ," << col << ")" << endl;
        cout << image.at(row, col) << endl;
    }
    catch(const char* msg)
    {
        cerr << msg << endl;
    }

    // Convert RGB to destination color space and display
    // Options:
        // RGB
        // YIQ
        // HSI
        // YCbCr
        // XYZ
    displayColorSpace(image, RGB);

    return 0;
}

