#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>
#include "filter.h"

#define PI 3.1415926

/****************** 
 * Brief: (数字图像处理-->实验三) Image filter Implementation
 * Author:
 *     Ding Qingpeng (丁庆鹏, Student No. 18B903053)
 ******************/

unsigned char conv(const cv::Mat& mat1, const cv::Mat& mat2)
{
    // Dimension check
    if( mat1.rows != mat2.rows || mat1.cols != mat2.cols )
    {
        std::cout << "Convolution elements dimension dismatch." << std::endl;
        return -1;
    }
        
    double result = 0.0;
    for( int i = 0; i < mat1.rows; i++ )
        for( int j = 0; j < mat1.cols; j++ )
            result += mat1.ptr<unsigned char>(i)[j] * mat2.ptr<double>(i)[j];
    // result = mat1.dot(mat2);

    return (unsigned char)result;
}

unsigned char findMedian( const cv::Mat& mat )
{
    int rows = mat.rows;
    int cols = mat.cols;
    int    n = rows * cols;
    unsigned char data[n];
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            data[i * rows + j] = mat.ptr<unsigned char>(i)[j];

    // Bubble sort
    unsigned char temp;
    for(int i = 0; i < n - 1; i++)
    {
        for(int j = 0; j < n - i; j++)
        {
            if( data[j] > data[j+1] )
            {
                temp = data[j];
                data[j] = data[j+1];
                data[j+1] = temp;
            }
        }
    }

    return data[n/2];
}

void addImpulseNoise( cv::Mat& image, double percentage )
{
    std::random_device rd;
    std::mt19937 gen(rd());
    int rows = image.rows;
    int cols = image.cols;
    
    for(int i = 0; i < (int)rows * cols * percentage; i++)
    {
        int row = gen() % rows;
        int col = gen() % cols;
        image.ptr<unsigned char>(row)[col] = gen() % 2 * 255;
    }
}

void addGaussianNoise( cv::Mat& image, double mean, double stdev )
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(mean, stdev);
    int rows = image.rows;
    int cols = image.cols;
    int temp;

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            temp = image.ptr<unsigned char>(i)[j] + d(gen);
            temp = temp > 255? 255: temp;
            temp = temp <   0?   0: temp;
            image.ptr<unsigned char>(i)[j] = temp;
        }
    }
}

void medianFilter( const cv::Mat& in, cv::Mat& out, int kernelSize )
{
    // Dimension check
    if( kernelSize % 2 == 0 )
    {
        std::cout << "Wrong kernel size." << std::endl;
        return;
    }

    int offset = kernelSize / 2;
    out = in.clone();
    cv::Mat part( kernelSize, kernelSize, CV_8UC1 );
    int rows = in.rows;
    int cols = in.cols;
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            // Construct image part
            for(int s = 0; s < kernelSize; s++)
            {
                for(int t = 0; t < kernelSize; t++)
                {
                    int rowIndex = i - offset + s;
                    int colIndex = j - offset + t;
                    rowIndex = rowIndex <    0?    0: rowIndex;
                    rowIndex = rowIndex > rows? rows: rowIndex;
                    colIndex = colIndex <    0?    0: colIndex;
                    colIndex = colIndex > cols? cols: colIndex;
                    part.ptr<unsigned char>(s)[t] = in.ptr<unsigned char>( rowIndex )[ colIndex ];
                }
            }
            // Find median
            out.ptr<unsigned char>(i)[j] = findMedian( part );
        }
    }
}

void meanFilter( const cv::Mat& in, cv::Mat& out, int kernelSize )
{
    // Dimension check
    if( kernelSize % 2 == 0 )
    {
        std::cout << "Wrong kernel size." << std::endl;
        return;
    }

    // Construct kernel
    // double coeff = 1.0 / ( kernelSize * kernelSize - 1 );
    int offset = kernelSize / 2;
    // cv::Mat kernel( kernelSize, kernelSize, CV_64F );
    // for(int i = 0; i < kernelSize; i++)
    // {
    //     for(int j = 0; j < kernelSize; j++)
    //     {
    //         if( i == offset && j == offset )
    //         {
    //             kernel.ptr<double>(i)[j] = 0.0;
    //             continue;
    //         }
    //         kernel.ptr<double>(i)[j] = coeff;
    //     }
    // }

    Kernel kernel;
    kernel.mean( kernelSize );

    out = in.clone();
    cv::Mat part( kernelSize, kernelSize, CV_8UC1 );
    int rows = in.rows;
    int cols = in.cols;
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            // Construct image part
            for(int s = 0; s < kernelSize; s++)
            {
                for(int t = 0; t < kernelSize; t++)
                {
                    int rowIndex = i - offset + s;
                    int colIndex = j - offset + t;
                    // Deal with boarders
                    rowIndex = rowIndex <    0?    0: rowIndex;
                    rowIndex = rowIndex > rows? rows: rowIndex;
                    colIndex = colIndex <    0?    0: colIndex;
                    colIndex = colIndex > cols? cols: colIndex;

                    part.ptr<unsigned char>(s)[t] = in.ptr<unsigned char>( rowIndex )[ colIndex ];
                }
            }
            // Compute convolution
            out.ptr<unsigned char>(i)[j] = (unsigned char)( kernel.conv( part ) + 0.5 );
        }
    }
}

double PSNR( const cv::Mat img1, const cv::Mat img2 )
{
    // Dimension check
    if( img1.rows != img2.rows || img1.cols != img2.cols )
    {
        std::cout << "Image dimension dismatch." << std::endl;
        return -1;
    }
    
    // Compute MSE
    size_t sum = 0;
    int rows = img1.rows;
    int cols = img1.cols;
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            int pixel1 = img1.ptr<unsigned char>(i)[j];
            int pixel2 = img2.ptr<unsigned char>(i)[j];
            int error  = pixel1 - pixel2;
            sum += (size_t)(error * error);
        }
    }
    double MSE = double(sum) / ( rows * cols );
    double PSNR = 20.0 * log10( 255.0 / sqrt( MSE ) );
    return PSNR;
}

double MSSIM( const cv::Mat img1, const cv::Mat img2 )
{
    ;
}

void GaussianFilter( const cv::Mat& in, cv::Mat& out, int kernelSize )
{
    // Dimension check
    if( kernelSize % 2 == 0 )
    {
        std::cout << "Wrong kernel size." << std::endl;
        return;
    }
    
    // Compute Gaussian kernel
    double sigma = 1.5;
    double twiceSigmaSquare = 2.0 * sigma * sigma;
    double GaussianCoeff = 1.0 / PI / twiceSigmaSquare;
    double sum = 0.0;
    int offset = kernelSize / 2;
    cv::Mat kernel( kernelSize, kernelSize, CV_64F );
    for(int i = 0; i < kernelSize; i++)
    {
        for(int j = 0; j < kernelSize; j++)
        {
            int x = i - offset;
            int y = j - offset;
            double exponent = -(double)( x*x + y*y ) / twiceSigmaSquare;
            kernel.ptr<double>(i)[j] = GaussianCoeff * exp( exponent );
            sum += kernel.ptr<double>(i)[j];
        }
    }
    // Normalize
    for(int i = 0; i < kernelSize; i++)
    {
        for(int j = 0; j < kernelSize; j++)
        {
            kernel.ptr<double>(i)[j] /= sum;
        }
    }

    // Compute convolution with every pixel
    out = in.clone();
    cv::Mat part( kernelSize, kernelSize, CV_8UC1 );
    int rows = in.rows;
    int cols = in.cols;
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            // Construct image part
            for(int s = 0; s < kernelSize; s++)
            {
                for(int t = 0; t < kernelSize; t++)
                {
                    int rowIndex = i - offset + s;
                    int colIndex = j - offset + t;
                    // Deal with boarders
                    rowIndex = rowIndex <    0?    0: rowIndex;
                    rowIndex = rowIndex > rows? rows: rowIndex;
                    colIndex = colIndex <    0?    0: colIndex;
                    colIndex = colIndex > cols? cols: colIndex;

                    part.ptr<unsigned char>(s)[t] = in.ptr<unsigned char>( rowIndex )[ colIndex ];
                }
            }
            // Compute convolution
            out.ptr<unsigned char>(i)[j] = (unsigned char)( conv( part, kernel ) + 0.5 );
        }
    }
}
