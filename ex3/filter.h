#include <iostream>
#include <opencv2/core.hpp>

/****************** 
 * Brief: (数字图像处理-->实验三) Image filter header file
 * Author:
 *     Ding Qingpeng (丁庆鹏, Student No. 18B903053)
 ******************/

enum{
    Gaussian = 1,
    mean     = 2
};

class Kernel
{
public:
    Kernel() {}
    // Kernel( int size ) { _kernel_ptr = new cv::Mat( size, size, CV_64F ); }

    void mean( int size )
    {
        _kernel = cv::Mat( size, size, CV_64F );
        
        double coeff = 1.0 / ( size * size - 1 );
        int offset = size / 2;
        cv::Mat kernel( size, size, CV_64F );
        for(int i = 0; i < size; i++)
        {
            for(int j = 0; j < size; j++)
            {
                if( i == offset && j == offset )
                {
                    _kernel.ptr<double>(i)[j] = 0.0;
                    continue;
                }
                _kernel.ptr<double>(i)[j] = coeff;
            }
        }
    }

    double conv( cv::Mat mat )
    {
        // Dimension check
        if( _kernel.rows != mat.rows || _kernel.cols != mat.cols )
        {
            std::cout << "Convolution elements dimension dismatch." << std::endl;
            return -1;
        }
            
        double result = 0.0;
        for( int i = 0; i < _kernel.rows; i++ )
            for( int j = 0; j < _kernel.cols; j++ )
                result += _kernel.ptr<double>(i)[j] * mat.ptr<unsigned char>(i)[j];
        // result = mat1.dot(mat);

        return (unsigned char)result;
    }

public:
    cv::Mat _kernel;
    // cv::Mat* _kernel_ptr;
};

/* 
 * Brief: Compute convolution
 * Parameter:
 *     mat1 -- take mat1 as f(t)
 *     mat2 -- take mat2 as g(t)
 * Return:
 *     Convolution result
 */
unsigned char conv(const cv::Mat& mat1, const cv::Mat& mat2);

/* 
 * Brief: Find median in a image block
 * Parameter:
 *     mat -- image block
 * Return:
 *     Median
 */
unsigned char findMedian( const cv::Mat& mat );

/* 
 * Brief: Add impulse (pepper) noise to image
 * Parameter:
 *     image -- destination image
 *     percentage -- percetage of pixels affected by noise
 *     visualize -- flag to determine visualization
 * Return:
 *     None
 */
void addImpulseNoise( cv::Mat& image, double percentage = 0.1);

/* 
 * Brief: Add Gaussian noise to image
 * Parameter:
 *     image -- destination image
 *     mean -- mean of Gaussian distribution
 *     stdev -- standard deviation of Gaussian distribution
 *     visualize -- flag to determine visualization
 * Return:
 *     None
 */
void addGaussianNoise( cv::Mat& image, double mean = 5, double stdev = 10);

/* 
 * Brief: Implement median filter
 * Parameter:
 *     in -- source image
 *     out -- filtered image
 *     kernelSize -- size of convolution kernel
 * Return:
 *     None
 */
void medianFilter( const cv::Mat& in, cv::Mat& out, int kernelSize = 3 );

/* 
 * Brief: Implement mean filter
 * Parameter:
 *     in -- source image
 *     out -- filtered image
 *     kernelSize -- size of convolution kernel
 * Return:
 *     None
 */
void meanFilter( const cv::Mat& in, cv::Mat& out, int kernelSize = 3 );

/* 
 * Brief: Compute PSNR of two images
 * Parameter:
 *     img1 -- image 1
 *     img2 -- image 2
 * Return:
 *     PSNR value
 */
double PSNR( const cv::Mat img1, const cv::Mat img2 );

/* 
 * Brief: Compute MSSIM of two images
 * Parameter:
 *     img1 -- image 1
 *     img2 -- image 2
 * Return:
 *     MSSIM value
 */
double MSSIM( const cv::Mat img1, const cv::Mat img2 );

/* 
 * Brief: Implement Gaussian filter
 * Parameter:
 *     in -- source image
 *     out -- filtered image
 *     kernelSize -- size of convolution kernel
 * Return:
 *     None
 */
void GaussianFilter( const cv::Mat& in, cv::Mat& out, int kernelSize = 11 );
