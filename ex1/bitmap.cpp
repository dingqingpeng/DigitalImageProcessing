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
    if(argc > 3)
    {
        printHelp();
        return -1;
    }
    else
    {
    const string inputFileName = argv[1];
    if(inputFileName == "-h")
    {
        printHelp();
        return 0;
    }

    // Load file and check validation
    BitMap image( inputFileName );  // BitMap image = load(inputFileName);
    if(!image.formatCheck())
    {
        cerr << "Input file is not a bmp image." << endl;
        return -1;
    }
    
    image.showFileHeader();
    image.showInfoHeader();
    cout << "-- Displaying regenerated image --" << endl;
    bmpshow("Regenerated Image", image);
    waitKey(0);
    cout << endl;

    // Get value of certain pixel==
    int row = 123, col = 100;
    try
    {
        cout << "-- pixel value at (" << row << " ," << col << ") --" << endl;
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
    if(argc == 3)
    {
        vector<string> vName = { "-RGB", "-YIQ", "-HSI", "-YCbCr", "-XYZ" };
        int index = 0;
        for(int i = 0; i < vName.size(); i++)
        {
            if(argv[2] == vName[i])
            {
                index = i+1;
                break;
            }
        }
        
        if(index == 0)  cout << "Wrong color space type." << endl;
        else
        {
            cout << "-- Displaying color components --" << endl;
            displayColorSpace(image, index);
        }
    }
    
    return 0;
    }
}

