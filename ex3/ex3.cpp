#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "filter.h"

/****************** 
 * Brief: (数字图像处理-->实验三) Image filter
 * Parameters:
 *     argv[1] -- filename
 * Author:
 *     Ding Qingpeng (丁庆鹏, Student No. 18B903053)
 ******************/

using namespace std;

void printHelp()
{
    cout << "Help: filter FileName" << endl;
    cout << "FileName:   name of image file" << endl;
}

int main(int argc, char const *argv[])
{  
    if(argc > 3)
    {
        printHelp();
        return -1;
    }

    const string inputFileName = argv[1];
    if(inputFileName == "-h")
    {
        printHelp();
        return 0;
    }

    // Load image
    cv::Mat image = cv::imread( inputFileName, cv::IMREAD_GRAYSCALE );
    cout << image.channels() << endl;

    // Add impulse noise
    cv::Mat image_Impulse = image.clone();
    addImpulseNoise( image_Impulse );

    // Add Guassian noise
    cv::Mat image_Guassian = image.clone();
    addGuassianNoise( image_Guassian );
    
    return 0;
}

