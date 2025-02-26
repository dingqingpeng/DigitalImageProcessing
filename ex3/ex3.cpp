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
    cout << "-- FileName: name of image file" << endl;
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

    // Add impulse noise
    cv::Mat image_Impulse = image.clone();
    addImpulseNoise( image_Impulse, 0.02 );
    // Median filter
    cv::Mat image_medianFilter;
    medianFilter( image_Impulse, image_medianFilter );
    // Compute PSNR and MSSIM
    cout << "-- Impulse noise and median filter --" << endl;
    cout << "PSNR  = " << PSNR( image, image_medianFilter ) << endl;
    cout << "MSSIM = " << MSSIM( image, image_medianFilter ) << endl;
    cout << "-------------------------------------" << endl;
    // Display images
    cv::imshow( "Source Image", image );
    cv::moveWindow("Source Image", 0, 20);
    cv::imshow( "Impulse Noise", image_Impulse );
    cv::moveWindow("Impulse Noise", image.rows, 50);
    cv::imshow( "Median Filter", image_medianFilter );
    cv::moveWindow("Median Filter", 2*image.rows, 20);
    cv::waitKey(0);
    // cv::destroyAllWindows();
    
    // Add Gaussian noise
    cv::Mat image_Gaussian = image.clone();
    addGaussianNoise( image_Gaussian );
    // Mean filter
    cv::Mat image_meanFilter;
    meanFilter( image_Gaussian, image_meanFilter );
    // Compute PSNR and MSSIM
    cout << "-- Gaussian noise and mean filter --" << endl;
    cout << "PSNR  = " <<  PSNR( image, image_meanFilter ) << endl;
    cout << "MSSIM = " << MSSIM( image, image_meanFilter ) << endl;
    cout << "------------------------------------" << endl;
    // Display images
    cv::imshow( "Source Image", image );
    cv::moveWindow("Source Image", 0, 20);
    cv::imshow( "Gaussian Noise", image_Gaussian );
    cv::moveWindow("Gaussian Noise", image.rows, 50);
    cv::imshow( "Mean Filter", image_meanFilter );
    cv::moveWindow("Mean Filter", 2*image.rows, 20);
    cv::waitKey(0);
    // cv::destroyAllWindows();

    // // Gaussian filter
    // cv::Mat image_test;
    // GaussianFilter( image, image_test, 11 );
    // // Display images
    // cv::imshow( "Source Image", image );
    // cv::moveWindow("Source Image", 0, 20);
    // cv::imshow( "Gaussian Filter", image_test );
    // cv::moveWindow("Gaussian Filter", image.rows, 20);
    // // Compute PSNR
    // cout << "PSNR = " << PSNR( image, image_test ) << endl;
    // cv::waitKey(0);
    // cv::destroyAllWindows();

    // Wavelet filter
    cv::Mat image_waveletFilter;
    waveletFilter( image_Gaussian, image_waveletFilter );
    // Compute PSNR and MSSIM
    cout << "-- Gaussian noise and wavelet filter --" << endl;
    cout << "PSNR  = " <<  PSNR( image, image_waveletFilter ) << endl;
    cout << "MSSIM = " << MSSIM( image, image_waveletFilter ) << endl;
    cout << "------------------------------------" << endl;
    // Display images
    cv::imshow( "Source Image", image );
    cv::moveWindow("Source Image", 0, 20);
    cv::imshow( "Gaussian Noise", image_Gaussian );
    cv::moveWindow("Gaussian Noise", image.rows, 50);
    cv::imshow( "Wavelet Filter", image_waveletFilter );
    cv::moveWindow("Wavelet Filter", 2*image.rows, 20);
    cv::waitKey(0);
    // cv::destroyAllWindows();

    // Extract edge using Sobel Operator
    cv::Mat edge;
    SobelEdge( image, edge );
    cout << "-- Sobel Operator --" << endl;
    cv::imshow( "Edges", edge );
    cv::waitKey(0);
    
    return 0;
}

