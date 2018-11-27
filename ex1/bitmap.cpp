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

    // Create new image and edit pixel values
    BitMap newImage(image); // BitMap newImage = image;
    const string newFileName = "lena_editted.bmp";
    write(newFileName, newImage);

    // Display edited image
    Mat image_cv = imread(newFileName, IMREAD_COLOR);
    if(image_cv.empty())    cout << "empty" << endl;
    namedWindow("window");
    imshow("window", image_cv);
    waitKey(0);
    remove(newFileName.c_str());

    // Convert RGB to destination color space and display
    // Options:
        // RGB
        // YIQ
        // HSI
        // YCbCr
        // XYZ
    displayColorSpace(image, HSI);

    return 0;
}

