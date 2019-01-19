#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>
#include "filter.h"
#include "CWavelet.h"

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
    Kernel kernel;
    kernel.mean( kernelSize );

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

void GaussianFilter( const cv::Mat& in, cv::Mat& out, int kernelSize )
{
    // Dimension check
    if( kernelSize % 2 == 0 )
    {
        std::cout << "Wrong kernel size." << std::endl;
        return;
    }

    // Construct kernel
    Kernel kernel;
    kernel.Gaussian( kernelSize, 1.5 );

    // Compute convolution with every pixel
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

double PSNR( const cv::Mat& img1, const cv::Mat& img2 )
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

double SSIM( const cv::Mat& img1, const cv::Mat& img2 )
{
    int rows = img1.rows;
    int cols = img1.cols;
    Kernel kernel;
    kernel.Gaussian( 11, 1.5 );
    
    // Compute mius
    double miu_x = kernel.conv( img1 );
    double miu_y = kernel.conv( img2 );
    // Compute error matrix
    cv::Mat error_xSqr( rows, cols, CV_64F );
    cv::Mat error_ySqr( rows, cols, CV_64F );
    cv::Mat cor_xy( rows, cols, CV_64F );
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            double error_x = img1.ptr<unsigned char>(i)[j] - miu_x;
            double error_y = img2.ptr<unsigned char>(i)[j] - miu_y;
            error_xSqr.ptr<double>(i)[j] = error_x * error_x;
            error_ySqr.ptr<double>(i)[j] = error_y * error_y;
            cor_xy.ptr<double>(i)[j] = error_x * error_y;
        }
    }
    // Compute sigmas
    double sigma_x = sqrt( kernel.conv( error_xSqr ) );
    double sigma_y = sqrt( kernel.conv( error_ySqr ) );
    double sigma_xy = kernel.conv( cor_xy );
    
    // Compute SSIM
    double K1 = 0.01;
    double K2 = 0.02;
    double  L = 255.0;
    double C1 = K1 * L * K1 * L;
    double C2 = K2 * L * K2 * L;
    double SSIM = (2*miu_x*miu_y+C1) * (2*sigma_x*sigma_y+C2) / ( (miu_x*miu_x+miu_y*miu_y+C1) * (sigma_x*sigma_x+sigma_y*sigma_y+C2) );

    // std::cout << SSIM << ", " << std::endl;

    return SSIM;
}

double MSSIM( const cv::Mat& img1, const cv::Mat& img2 )
{
    // Dimension check
    if( img1.rows != img2.rows || img1.cols != img2.cols )
    {
        std::cout << "Image dimension dismatch." << std::endl;
        return -1;
    }
    
    // Set kernel size
    // This is the same size as Gaussian kernel while computing SSIM.
    int kernelSize = 11;
    int offset = kernelSize / 2;

    cv::Mat part1( kernelSize, kernelSize, CV_8UC1 );
    cv::Mat part2( kernelSize, kernelSize, CV_8UC1 );
    int rows = img1.rows;
    int cols = img1.cols;
    double MSSIMValue = 0.0;
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

                    part1.ptr<unsigned char>(s)[t] = img1.ptr<unsigned char>( rowIndex )[ colIndex ];
                    part2.ptr<unsigned char>(s)[t] = img2.ptr<unsigned char>( rowIndex )[ colIndex ];
                }
            }
            // Compute SSIM
            MSSIMValue += SSIM( part1, part2 );
        }
    }
    MSSIMValue /= (double)( rows * cols );

    return MSSIMValue;
}

void SobelEdge( const cv::Mat& img, cv::Mat& edgeImage )
{
    edgeImage = img.clone();
    
    // Set kernel
    Kernel kernel_x, kernel_y;
    kernel_x.SobelHorizontal();
    kernel_y.SobelVertial();

    // Set kernel size
    int kernelSize = 3;
    int offset = kernelSize / 2;
    
    cv::Mat part( kernelSize, kernelSize, CV_8UC1 );
    int rows = img.rows;
    int cols = img.cols;
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

                    part.ptr<unsigned char>(s)[t] = img.ptr<unsigned char>( rowIndex )[ colIndex ];
                }
            }
            // Compute gradient value
            double gradient = abs( kernel_x.conv( part ) ) + abs( kernel_y.conv( part ) );
            edgeImage.ptr<unsigned char>(i)[j] = gradient > 125? 255: 0;
        }
    }
}

void waveletFilter( const cv::Mat& in, cv::Mat& out )
{
    int height = in.rows;
	int width  = in.cols;
    int size   = height * width;

    // Construct input sequence of DWT
	double s[size];
	for(size_t i = 0; i < size; i++)
		s[i] = (double)in.ptr<unsigned char>(i / width)[i % width];
    
    // 2DWavelet filter
    CWavelet cw;
    int Scale = 3;
	int dbn   = 3;
	cw.InitDecInfo2D(height, width, Scale, dbn);
	double *dstcoef = new double[size];
	if (!cw.thrDenoise2D(s, dstcoef))
		cerr << "Error" << endl;

    // Reconstruct image
	out = in.clone();
	
	for(size_t i = 0; i < height; i++)
		for(size_t j = 0; j < width; j++)
			out.ptr<unsigned char>(i)[j] = (unsigned char)dstcoef[ i * width + j ];

    delete[] dstcoef;
	dstcoef = NULL;
}
